#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#include "Filesystem.hpp"

Filesystem::Filesystem(string disk)
{
   // TODO: Constructor that opens an existing disk file
}

Filesystem::Filesystem(string path, int size)
{
   this->files = new BlockHandler(path.c_str(), size);
   if (this->files == nullptr)
   {
      throw std::runtime_error("Error at Filesystem::Filesystem(string, int)");
   }
}

Filesystem::~Filesystem()
{
   delete this->files;
}

int Filesystem::createFile(string pName, string pContent)
{
   int pos = -1;

   int blockAmnt;
   string blockString;

   pos = this->files->setNodeBlock(pContent.size(), 1);
   this->files->getBlock(1)->content.directory->directoryMap.emplace(pName, pos);

   if (pos == -1)
   {
      throw std::runtime_error("Filesystem::CreateFile(string, string) failed when creating Node Block");
      return pos;
   }

   blockAmnt = pContent.size() / BLOCK_SIZE + (pContent.size() % BLOCK_SIZE != 0);
   int dataPos;

   for (int i = 0; i < blockAmnt; i++)
   {
      blockString = pContent.substr(i * BLOCK_SIZE, BLOCK_SIZE);
      dataPos = this->files->setDataBlock(blockString.c_str(), blockString.size());
      this->files->getBlock(pos)->content.node->asciiParts[i] = dataPos;
   }

   return ++blockAmnt;
}

int Filesystem::deleteFile(string pName)
{
   int pos = -1;

   pos = this->files->getBlock(1)->content.directory->directoryMap[pName];

   if ((this->files->freeNodeBlock(pos)) == false)
   {
      throw std::runtime_error("Filesystem::deleteFile(string) could not delete file.");
   }
   else
   {
      pos = 0;
   }

   this->files->getBlock(1)->content.directory->directoryMap.erase(pName);

   return pos;
}

void Filesystem::print(string pName)
{
   int index = this->files->getBlock(1)->content.directory->directoryMap[pName];

   int fileSize = this->files->getBlock(index)->content.node->asciiSize;

   fileSize = fileSize / BLOCK_SIZE + (fileSize % BLOCK_SIZE != 0);

   int dataPos;
   for (int i = 0; i < fileSize; i++)
   {
      dataPos = this->files->getBlock(index)->content.node->asciiParts[i];

      cout << this->files->getBlock(dataPos)->content.data->content;
   }

   cout << endl;
}

bool Filesystem::saveToFile()
{
   // TODO: Implementar guardado a archivo
   return false;

   writeDisk.open(this->files->getFilename(), std::ios::binary | std::ios::trunc);
   if (!writeDisk.is_open()) {
      std::cerr << "Error opening file" << std::endl;
      return false;
   }

   char blockBuffer[BLOCK_SIZE];

   // Loop through all blocks and write them to the file
   for (int i = 0; i < this->files->getTotalBlocks(); ++i) {
      // Get the current block
      GenericalBlock* block = this->files->getBlock(i);

      memset(blockBuffer, 0, BLOCK_SIZE);
      size_t offset = 0;

      // Write block type
      memcpy(blockBuffer, reinterpret_cast<const char*>(&block->type), sizeof(BlockType));
      offset += sizeof(BlockType);

      // Switch based on block type and write relevant data
      switch (block->type) {
         case BlockType::MetaData: {
            MetaDataBlock* meta = block->content.metaData;
            memcpy(blockBuffer + offset, &meta->totalBlocks, sizeof(int));
            offset += sizeof(int);
            memcpy(blockBuffer + offset, &meta->blockSize, sizeof(int));
            offset += sizeof(int);
            memcpy(blockBuffer + offset, &meta->totalFreeBlocks, sizeof(int));
            offset += sizeof(int);
            // Save Bitmap
            memcpy(blockBuffer + offset, meta->bitmap, sizeof(int) * meta->totalBlocks);
            offset += sizeof(int) * meta->totalBlocks;
            break;
         }
         case BlockType::Directory: {
            DirectoryBlock* dir = block->content.directory;
            memcpy(blockBuffer + offset, &dir->entryCount, sizeof(int));
            offset += sizeof(int);
            int mapSize = dir->directoryMap.size();
            memcpy(blockBuffer + offset, &mapSize, sizeof(int));
            offset += sizeof(int);
            for (const auto& entry : dir->directoryMap) {
               int nameLen = entry.first.size();
               memcpy(blockBuffer + offset, &nameLen, sizeof(int));
               offset += sizeof(int);
               memcpy(blockBuffer + offset, entry.first.c_str(), nameLen);
               offset += nameLen;
               memcpy(blockBuffer + offset, &entry.second, sizeof(int));
               offset += sizeof(int);
            }
            break;
         }
         case BlockType::Node: {
            NodeBlock* node = block->content.node;
            memcpy(blockBuffer + offset, &node->asciiSize, sizeof(int));
            offset += sizeof(int);
            memcpy(blockBuffer + offset, &node->permissions, sizeof(int));
            offset += sizeof(int);
            int partsCount = node->asciiSize / BLOCK_SIZE + (node->asciiSize % BLOCK_SIZE != 0);
            for (int j = 0; j < partsCount; ++j) {
               memcpy(blockBuffer + offset, &node->asciiParts[j], sizeof(int));
               offset += sizeof(int);
            }
            break;
         }
         case BlockType::Data: {
            DataBlock* data = block->content.data;
            int contentLen = data->content.size();
            memcpy(blockBuffer + offset, &contentLen, sizeof(int));
            offset += sizeof(int);
            memcpy(blockBuffer + offset, data->content.c_str(), contentLen);
            offset += contentLen;
            break;
         }
         case BlockType::Empty:
            break;
      }
      // Finally, write the entire block buffer to the file
      writeDisk.write(blockBuffer, BLOCK_SIZE);
   }
   writeDisk.close();
   return true;
}

bool Filesystem::loadFromFile()
{
   // TODO: Implementar carga desde archivo
   return false;
}
