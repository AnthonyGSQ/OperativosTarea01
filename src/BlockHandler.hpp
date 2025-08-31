
#ifndef BLOCKHANDLER_HPP
#define BLOCKHANDLER_HPP

#include "BlockStructures.hpp"
#include <vector>


// Enum to identify block types in the system
enum class BlockType { Directory, Node, Data };


class BlockHandler {
private:
    // MetaDataBlock: Stores global information about the file system
    // (only one per system)
    MetaDataBlock metaDataBlock;

    // DirectoryBlocks: Each block contains a map of drawing names to their
    // respective node block indices
    std::vector<DirectoryBlock> directoryBlocks;

    // NodeBlocks: Each block contains references to the data blocks that
    // store the ASCII drawing
    std::vector<NodeBlock> nodeBlocks;

    // DataBlocks: Each block stores a fixed-size portion of an ASCII drawing
    std::vector<DataBlock> dataBlocks;

    // Function that would create all the neccesary blocks
    bool init();
    // Saves all blocks to a .dat file (binary format)
    bool saveToFile(const std::string& filename);

    // Loads all blocks from a .dat file (binary format)
    bool loadFromFile(const std::string& filename);

public:
    // Constructor: Initializes all necessary blocks when the handler is created
    BlockHandler();

    // Destructor: Cleans up resources used by the block handler
    ~BlockHandler();

    // Setters: Update a block at a given position (by value)
    void setDirectoryBlock(int position, const DirectoryBlock& block);
    void setNodeBlock(int position, const NodeBlock& block);
    void setDataBlock(int position, const DataBlock& block);

    // Getters: Retrieve a block at a given position
    DirectoryBlock getDirectoryBlock(int position) const;
    NodeBlock getNodeBlock(int position) const;
    DataBlock getDataBlock(int position) const;
};

#endif