#include <string>
#include <iostream>
#include <fstream>

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
}

bool Filesystem::loadFromFile()
{
   // TODO: Implementar carga desde archivo
   return false;
}
