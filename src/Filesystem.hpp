#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "BlockStructures.hpp"
#include "BlockHandler.hpp"

using std::string, std::cout, std::cin, std::endl;

class Filesystem
{
private:
   GenericalBlock* blocks;

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
    */
   Filesystem(int size);
   ~Filesystem();

   int CreateFile(string pName, string pContent);
};