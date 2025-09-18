#ifndef BLOCKSTRUCTURES_HPP
#define BLOCKSTRUCTURES_HPP
#include <map>
#include <string>

constexpr int BLOCK_SIZE = 256;

struct MetaDataBlock {
    int totalBlocks;
    int blockSize = BLOCK_SIZE;
    int totalFreeBlocks;
    // array with all the blocks in the file system (forward declaration)
    void* unit;  // Será un puntero a GenericalBlock*
    // map that indicates which blocks are used and which not (0 = free, 1 = used)
    int* bitmap;
};

struct DirectoryBlock {
    // amounth of different Node Blocks (ascii draws)
    int entryCount;
    // map with all the ascii draws and his respective Node Block index
    std::map<std::string, int> directoryMap;
};

struct NodeBlock {
    int asciiSize;
    // 1 for read, 2 for write permissions
    int permissions;
    // array with all the ascii parts index of the unit
    int* asciiParts;
};

struct DataBlock {
    // when the user ask to see the content of one ascii draw, this string will
    // have the corresponding ascii part of this data block
    std::string content;
};

enum class BlockType { Empty, MetaData, Directory, Node, Data };

// this struct exist because for the array we need a generical type of block
struct GenericalBlock {
    BlockType type;
    union {
        MetaDataBlock* metaData;
        DirectoryBlock* directory;
        NodeBlock* node;
        DataBlock* data;
    } content;  
};

#endif