#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include "BlockStructures.hpp"
#include "BlockHandler.hpp"

using std::string, std::cout, std::cin, std::endl;

class Filesystem
{
private:
   BlockHandler* files; // Blockhandler object to manage files internally

   std::ifstream readDisk; // file stream for operations that need to read from the disk
   std::ofstream writeDisk; // file stream for operations that need to write to the disk

public:
   /**
    * @brief Open an existing disk
    * 
    * @param disk which disk will be accessed
    */
   Filesystem(string disk);

   /**
    * @brief Creates a new disk of specified size
    * 
    * @param size Size of disk to be created
    * @param path path to the disk file
    */
   Filesystem(string path, int size);

   ~Filesystem();

   /**
    * @brief Creates a file within the disk.
    * 
    * @param pName file name
    * @param pContent content of the file
    * @return -1 on failure. Returns amount of blocks allocated for file otherwise. Including nodes.
    */
   int createFile(string pName, string pContent);

   /**
    * @brief Deletes the specified file.
    * 
    * @param pName 
    * @return -1 on failure and 0 on success.
    */
   int deleteFile(string pName);

   /**
    * @brief prints a given file
    * 
    * @param pName name of the file within the disk
    */
   void print(string pName);
};