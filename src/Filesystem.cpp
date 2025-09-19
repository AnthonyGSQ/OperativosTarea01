#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

#include "Filesystem.hpp"

Filesystem::Filesystem(string disk)
{
   // TODO: Constructor that opens an existing disk file
   this->files = new BlockHandler(disk.c_str(), 0);
   if (this->files == nullptr)
   {
      throw std::runtime_error("Error at Filesystem::Filesystem(string)");
   }
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
   // Construir la ruta completa al disco virtual (carpeta ../discs/)
   std::string fullPath = std::string(files->getFilename());
   // Abrir el archivo en modo binario
   readDisk.open(fullPath, std::ios::binary);
   if (!readDisk.is_open())
   {
      std::cout << "Error: no se pudo abrir " << fullPath << " para lectura.\n";
      return false;
   }
   // Ir al final del archivo para calcular el tamaño total en bytes
   readDisk.seekg(0, std::ios::end);
   std::streampos fileSize = readDisk.tellg();
   // Calcular cuántos bloques de 256 bytes hay en el archivo
   int totalBlocksFromFile = fileSize / BLOCK_SIZE + (fileSize % BLOCK_SIZE != 0);
   // Volver al inicio del archivo para empezar a leer
   readDisk.seekg(0, std::ios::beg);
   // Reinicializar el BlockHandler con la nueva cantidad de bloques
   delete files;
   files = new BlockHandler(files->getFilename(), totalBlocksFromFile);
   // === Bucle principal de lectura, bloque por bloque ===
   for (int i = 0; i < totalBlocksFromFile; ++i)
   {
      // Crear un buffer de 256 bytes para leer el bloque
      char buffer[BLOCK_SIZE] = {0};
      readDisk.read(buffer, BLOCK_SIZE);
      // Verificar si la lectura fue exitosa
      if (readDisk.fail())
      {
         std::cout << "Error al leer bloque " << i << ".\n";
         readDisk.close();
         return false;
      }
      size_t offset = 0;
      // El primer byte del bloque indica el tipo (MetaData, Directory, Node, Data, Empty)
      BlockType type;
      memcpy(&type, buffer, sizeof(BlockType));
      offset += sizeof(BlockType);
      // Clasificar el bloque según su tipo
      switch (type)
      {
         // -------------------------------
         case BlockType::MetaData:
         {
            MetaDataBlock meta;
            // Copiar los primeros campos del bloque de metadatos
            memcpy(&meta.totalBlocks, buffer + offset, sizeof(int));
            offset += sizeof(int);
            memcpy(&meta.blockSize, buffer + offset, sizeof(int));
            offset += sizeof(int);
            memcpy(&meta.totalFreeBlocks, buffer + offset, sizeof(int));
            offset += sizeof(int);

            // Copiar el bitmap (array de ints que indica bloques libres/ocupados)
            meta.bitmap = new int[meta.totalBlocks];
            memcpy(meta.bitmap, buffer + offset, sizeof(int) * meta.totalBlocks);
            break;
         }

         // -------------------------------
         case BlockType::Directory:
         {
            DirectoryBlock* dir = new DirectoryBlock();
            memcpy(&dir->entryCount, buffer + offset, sizeof(int));
            offset += sizeof(int);
            int mapSize;
            memcpy(&mapSize, buffer + offset, sizeof(int));
            offset += sizeof(int);
            for (int i = 0; i < mapSize; ++i) {
               int nameLen;
               memcpy(&nameLen, buffer + offset, sizeof(int));
               offset += sizeof(int);
               std::string name(buffer + offset, nameLen);
               offset += nameLen;
               int pos;
               memcpy(&pos, buffer + offset, sizeof(int));
               offset += sizeof(int);
               dir->directoryMap[name] = pos;
            }
            // Asignar el bloque leído al arreglo de bloques
            files->getBlock(i)->type = BlockType::Directory;
            files->getBlock(i)->content.directory = dir;
            break;
         }
         // -------------------------------
         case BlockType::Node:
         {
            NodeBlock* node = new NodeBlock();
            memcpy(&node->asciiSize, buffer + offset, sizeof(int));
            offset += sizeof(int);
            memcpy(&node->permissions, buffer + offset, sizeof(int));
            offset += sizeof(int);
            int partsCount = node->asciiSize / BLOCK_SIZE + (node->asciiSize % BLOCK_SIZE != 0);
            node->asciiParts = new int[partsCount];
            for (int j = 0; j < partsCount; ++j) {
               memcpy(&node->asciiParts[j], buffer + offset, sizeof(int));
               offset += sizeof(int);
            }
            // Asignar el bloque leído al arreglo de bloques
            files->getBlock(i)->type = BlockType::Node;
            files->getBlock(i)->content.node = node;
            break;
         }
         // -------------------------------
         case BlockType::Data:
         {
            DataBlock data;
            // Copiar el contenido de datos (texto/binario)
            data.content.assign(buffer + offset, BLOCK_SIZE);

            // Guardar el bloque de datos en el BlockHandler
            files->setDataBlock(data.content.c_str(), data.content.size());
            break;
         }
         // -------------------------------
         case BlockType::Empty:
         default:
            // Si el bloque está vacío o no reconocido, no hacemos nada
            break;
         }
   }

   // Cerrar el archivo después de leer todo
   readDisk.close();
   return true;
}
