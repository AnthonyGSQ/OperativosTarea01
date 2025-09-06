#include "BlockHandler.hpp"
#include <cstring>
#include <iostream>

BlockHandler::BlockHandler(const char* filename, int totalBlocks) {
    this->filename = filename;
    this->totalBlocks = totalBlocks;
    
    // array of all the blocks
    blocks = new GenericalBlock[totalBlocks];
    
    // initialization of all the blocks as empty blocks
    for (int i = 0; i < totalBlocks; ++i) {
        blocks[i].type = BlockType::Empty;
        blocks[i].content.metaData = nullptr;
    }
    
    // since this is the constructor, we create the metaData block from the
    // start
    setMetaDataBlock();
}

BlockHandler::~BlockHandler() {
    // free for the specific current block type
    for (int i = 0; i < totalBlocks; ++i) {
        switch (blocks[i].type) {
            case BlockType::MetaData:
                // free bitmap
                delete[] blocks[i].content.metaData->bitmap;
                delete blocks[i].content.metaData;
                break;
            case BlockType::Directory:
                delete blocks[i].content.directory;
                break;
            case BlockType::Node:
                delete blocks[i].content.node;
                break;
            case BlockType::Data:
                delete blocks[i].content.data;
                break;
            case BlockType::Empty:
                break;
        }
    }
    delete[] blocks;
}

int* BlockHandler::getBlockData(int position) {
    if (position >= 0 && position < totalBlocks) {
        return reinterpret_cast<int*>(&blocks[position]);
    }
    return nullptr;
}

int BlockHandler::findEmptyBlockPosition() {
    for (int i = 1; i < totalBlocks; ++i) {
        if (blocks[i].type == BlockType::Empty) {
            return i;
        }
    }
    return -1;
}

int BlockHandler::setMetaDataBlock() {
    if (totalBlocks > 0) {
        blocks[0].type = BlockType::MetaData;
        blocks[0].content.metaData = new MetaDataBlock();
        blocks[0].content.metaData->totalBlocks = totalBlocks;
        blocks[0].content.metaData->blockSize = BLOCK_SIZE;
        blocks[0].content.metaData->totalFreeBlocks = totalBlocks - 1;
        blocks[0].content.metaData->unit = static_cast<void*>(blocks);
        
        // Inicializar el bitmap
        blocks[0].content.metaData->bitmap = new int[totalBlocks];
        for (int i = 0; i < totalBlocks; ++i) {
            blocks[0].content.metaData->bitmap[i] = 0;
        }
        // position of the metaData block
        blocks[0].content.metaData->bitmap[0] = 1;
        
        return 0;
    }
    return -1;
}

int BlockHandler::setDirectoryBlock() {
    int pos = findEmptyBlockPosition();
    if (pos == -1) return -1;
    
    blocks[pos].type = BlockType::Directory;
    blocks[pos].content.directory = new DirectoryBlock();
    blocks[pos].content.directory->entryCount = 0;
    
    // mark the current position in the bitmap as busy
    blocks[0].content.metaData->bitmap[pos] = 1;
    blocks[0].content.metaData->totalFreeBlocks--;
    
    return pos;
}

int BlockHandler::setNodeBlock(int asciiSize, int permissions) {
    int pos = findEmptyBlockPosition();
    if (pos == -1) return -1;
    
    blocks[pos].type = BlockType::Node;
    blocks[pos].content.node = new NodeBlock();
    blocks[pos].content.node->asciiSize = asciiSize;
    blocks[pos].content.node->permissions = permissions;
    blocks[pos].content.node->asciiParts = nullptr;
    
    blocks[0].content.metaData->bitmap[pos] = 1; // mark as busy
    blocks[0].content.metaData->totalFreeBlocks--;
    
    return pos;
}

int BlockHandler::setDataBlock(const char* data, int dataSize) {
    int pos = findEmptyBlockPosition();
    if (pos == -1) return -1;
    
    blocks[pos].type = BlockType::Data;
    blocks[pos].content.data = new DataBlock();
    blocks[pos].content.data->content = std::string(data, dataSize);
    
    blocks[0].content.metaData->bitmap[pos] = 1; // mark as busy
    blocks[0].content.metaData->totalFreeBlocks--;
    
    return pos;
}

GenericalBlock* BlockHandler::getBlock(int position) {
    if (position >= 0 && position < totalBlocks) {
        return &blocks[position];
    }
    return nullptr;
}
bool BlockHandler::saveToFile() {
    // TODO: Implementar guardado a archivo
    return false;
}

bool BlockHandler::loadFromFile() {
    // TODO: Implementar carga desde archivo
    return false;
}

void BlockHandler::printAllBlocks() {
    for (int i = 0; i < totalBlocks; ++i) {
        switch (blocks[i].type) {
            case BlockType::MetaData:
            std::cout<<"Bloque "<<i<<" de tipo MetaData"<<std::endl;    
            break;
            case BlockType::Directory:
            std::cout<<"Bloque "<<i<<" de tipo Directory"<<std::endl;    
            break;
            case BlockType::Node:
            std::cout<<"Bloque "<<i<<" de tipo Node"<<std::endl;    
            break;
            case BlockType::Data:
            std::cout<<"Bloque "<<i<<" de tipo Data"<<std::endl;    
            break;
            case BlockType::Empty:
                std::cout<<"Bloque "<<i<<" de tipo Empty"<<std::endl;
        }
    }
}

bool BlockHandler::freeDataBlock(int position) {
    if (blocks[position].type != BlockType::Data) {
        std::cout<<"Error: The requested block to delete isn't a data Block Type\n";
        return false;
    }
    blocks[position].content.data->content = "";
    blocks[position].type = BlockType::Empty;
    // search where the MetaData block is to mark the bitmap[position] as free
    for (int i = 0; i < totalBlocks; ++i) {
        if (blocks[i].type == BlockType::MetaData) {
            blocks[i].content.metaData->bitmap[position] = 0;
            break;
        }
    }    
    return true;
}