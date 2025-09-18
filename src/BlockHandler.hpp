#ifndef BLOCKHANDLER_HPP
#define BLOCKHANDLER_HPP

#include <string>
#include <array>
#include "BlockStructures.hpp"

class BlockHandler
{
private:
    const char *filename;
    int totalBlocks;
    // array with all the blocks of the file system
    GenericalBlock *blocks;

public:
    BlockHandler(const char *filename, int totalBlocks);
    ~BlockHandler();

    // setters to create all the type of blocks of the system
    int setMetaDataBlock();
    int setDirectoryBlock();
    int setNodeBlock(int asciiSize, int permissions);
    int setDataBlock(const char *data, int dataSize);

    // get the block of a specific position;
    GenericalBlock *getBlock(int position);

    // function to read all the data from a specific block
    int *getBlockData(int position);
    // function that returns the first free block finded in the bitmap
    int findEmptyBlockPosition();
    // function that returns the bitmap size
    int getTotalBlocks() const { return totalBlocks; }

    // print all the type blocks in order
    void printAllBlocks();

    // free a specific block, once it its free, the metaData block update his
    // bitmap to show the bloc
    bool freeDataBlock(int position);
    bool freeNodeBlock(int position);
};

#endif