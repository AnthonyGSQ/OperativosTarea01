#include <iostream>

#include "BlockHandler.hpp"
#include "Filesystem.hpp"

int main()
{
    Filesystem fs("testDisk.bin", 10);
    fs.getBlockHandler()->printMeta();
    // fs.loadFromFile();
    fs.createFile("007.txt", "     0000             0000        7777777777777777/========___________\n   00000000         00000000      7777^^^^^^^7777/ || ||   ___________\n  000    000       000    000     777       7777/=========//\n 000      000     000      000             7777// ((     //\n0000      0000   0000      0000           7777//   \\   //\n0000      0000   0000      0000          7777//========//\n0000      0000   0000      0000         7777\n0000      0000   0000      0000        7777\n 000      000     000      000        7777\n  000    000       000    000       77777\n   00000000         00000000       7777777\n     0000             0000        777777777\n");
    fs.createFile("006.txt", "     0000             0000        7777777777777777/========___________\n   00000000         00000000      7777^^^^^^^7777/ || ||   ___________\n  000    000       000    000     777       7777/=========//\n 000      000     000      000             7777// ((     //\n0000      0000   0000      0000           7777//   \\   //\n0000      0000   0000      0000          7777//========//\n0000      0000   0000      0000         7777\n0000      0000   0000      0000        7777\n 000      000     000      000        7777\n  000    000       000    000       77777\n   00000000         00000000       7777777\n     0000             0000        777777777\n");
    fs.print("007.txt");
    fs.print("006.txt");
    fs.saveToFile();
    fs.getBlockHandler()->printMeta();
    fs.getBlockHandler()->printAllBlocks();



    Filesystem fs2("testDisk.bin");
    fs2.loadFromFile();
    fs2.getBlockHandler()->printMeta();
    fs2.getBlockHandler()->printAllBlocks();
    fs2.print("007.txt");
    fs2.print("006.txt");

    return 0;
}

