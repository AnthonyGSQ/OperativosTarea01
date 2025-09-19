#include <iostream>

#include "BlockHandler.hpp"
#include "Filesystem.hpp"

int main()
{
    // Filesystem fs = Filesystem("../disks/testDisk.text", 10);

    // fs.createFile("007.txt", "     0000             0000        7777777777777777/========___________\n   00000000         00000000      7777^^^^^^^7777/ || ||   ___________\n  000    000       000    000     777       7777/=========//\n 000      000     000      000             7777// ((     //\n0000      0000   0000      0000           7777//   \\   //\n0000      0000   0000      0000          7777//========//\n0000      0000   0000      0000         7777\n0000      0000   0000      0000        7777\n 000      000     000      000        7777\n  000    000       000    000       77777\n   00000000         00000000       7777777\n     0000             0000        777777777\n");
    // fs.print("007.txt");
    // fs.deleteFile("007.txt");

        // 1. ASCII art que vamos a guardar
    std::string asciiArt =
        "     0000             0000        7777777777777777/========___________\n"
        "   00000000         00000000      7777^^^^^^^7777/ || ||   ___________\n"
        "  000    000       000    000     777       7777/=========//\n"
        " 000      000     000      000             7777// ((     //\n"
        "0000      0000   0000      0000           7777//   \\   //\n"
        "0000      0000   0000      0000          7777//========//\n"
        "0000      0000   0000      0000         7777\n"
        "0000      0000   0000      0000        7777\n"
        " 000      000     000      000        7777\n"
        "  000    000       000    000       77777\n"
        "   00000000         00000000       7777777\n"
        "     0000             0000        777777777\n";

    // 2. Crear archivo binario en la carpeta discs/
    std::ofstream outFile("discs/ascii_art.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error al crear el archivo\n";
        return 1;
    }

    // Guardar los bytes del string en el archivo
    outFile.write(asciiArt.c_str(), asciiArt.size());
    outFile.close();

    std::cout << "Archivo ascii_art.bin creado correctamente.\n";

    // 3. Inicializar Filesystem usando el constructor con tamaño
    // Calcular cantidad de bloques necesarios para el contenido
    int totalBlocks = asciiArt.size() / BLOCK_SIZE;
    if (asciiArt.size() % BLOCK_SIZE != 0) totalBlocks++; // redondeo hacia arriba

    // Crear Filesystem con tamaño conocido
    Filesystem fs("discs/ascii_art.bin", totalBlocks);

    // 4. Probar loadFromFile()
    if (fs.loadFromFile()) {
        std::cout << "Archivo cargado correctamente desde el disco virtual.\n";
    } else {
        std::cout << "Error al cargar desde el archivo.\n";
    }

    return 0;
}

