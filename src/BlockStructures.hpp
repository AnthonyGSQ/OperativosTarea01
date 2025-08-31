#ifndef BLOCKSTRUCTURES_HPP
#define BLOCKSTRUCTURES_HPP

#include <array>
#include <string>
#include <vector>
#include <map>

constexpr int BLOCK_SIZE = 256;
struct MetaDataBlock {
    int totalSpace;
    int blockSize = BLOCK_SIZE;
    int blockCount;
    int totalFreeBlocks;
    std::string SADAversion;
    std::vector<int> bitmap;
    std::string creationDate;
};

struct DirectoryBlock {
    std::map<std::string, int> directoryMap;
    std::string creationDate;
};
struct NodeBlock {
    std::vector<int> asciiParts;
    int asciiSize;
    // 1 for read, 2 for write permissions
    int permissions;
    std::string creationDate;
};
struct DataBlock {
    std::array<char, BLOCK_SIZE> data;
};

#endif