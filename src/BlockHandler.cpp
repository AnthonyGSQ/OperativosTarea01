#include "BlockHandler.hpp"

#include <iostream>

BlockHandler::BlockHandler() {

}

BlockHandler::~BlockHandler() {

}

bool BlockHandler::init() {

}
bool BlockHandler::saveToFile(const std::string& filename) {

}
bool BlockHandler::loadFromFile(const std::string& filename) {

}
void BlockHandler::setDirectoryBlock(int position, const DirectoryBlock& block) {

}
void BlockHandler::setNodeBlock(int position, const NodeBlock& block) {

}
void BlockHandler::setDataBlock(int position, const DataBlock& block) {

}
DirectoryBlock BlockHandler::getDirectoryBlock(int position) const {

}
NodeBlock BlockHandler::getNodeBlock(int position) const {

}
DataBlock BlockHandler::getDataBlock(int position) const {

}