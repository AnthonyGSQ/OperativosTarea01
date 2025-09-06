#include <iostream>
#include "BlockHandler.hpp"

int main() {
    std::cout << "=== Probando Sistema de Bloques ===" << std::endl;
    
    // Crear un BlockHandler con 10 bloques
    const char* filename = "test_filesystem.dat";
    int totalBlocks = 10;
    
    BlockHandler handler(filename, totalBlocks);
    
    std::cout << "\n1. Estado inicial del sistema:" << std::endl;
    handler.printAllBlocks();
    
    // Crear un directorio
    std::cout << "\n2. Creando un DirectoryBlock..." << std::endl;
    int dirPos = handler.setDirectoryBlock();
    std::cout << "DirectoryBlock creado en posición: " << dirPos << std::endl;
    
    // Crear un nodo ASCII
    std::cout << "\n3. Creando un NodeBlock..." << std::endl;
    int nodePos = handler.setNodeBlock(150, 1); // ASCII de 150 chars, permisos de lectura
    std::cout << "NodeBlock creado en posición: " << nodePos << std::endl;
    
    // Crear algunos bloques de datos
    std::cout << "\n4. Creando DataBlocks..." << std::endl;
    int dataPos1 = handler.setDataBlock("ASCII Art Part 1: \n  /\\_/\\  \n ( o.o ) \n  > ^ <", 50);
    int dataPos2 = handler.setDataBlock("ASCII Art Part 2: \n   ___   \n  |   |  \n  |___|", 40);
    std::cout << "DataBlock 1 creado en posición: " << dataPos1 << std::endl;
    std::cout << "DataBlock 2 creado en posición: " << dataPos2 << std::endl;
    
    std::cout << "\n5. Estado después de crear bloques:" << std::endl;
    handler.printAllBlocks();
    
    // Probar acceso a bloques específicos
    std::cout << "\n6. Probando acceso a bloques específicos:" << std::endl;
    
    // Acceder al MetaDataBlock
    GenericalBlock* metaBlock = handler.getBlock(0);
    if (metaBlock && metaBlock->type == BlockType::MetaData) {
        MetaDataBlock* metaData = metaBlock->content.metaData;
        std::cout << "MetaData - Total Blocks: " << metaData->totalBlocks << std::endl;
        std::cout << "MetaData - Block Size: " << metaData->blockSize << std::endl;
        std::cout << "MetaData - Free Blocks: " << metaData->totalFreeBlocks << std::endl;
    }
    
    // Acceder al DirectoryBlock
    GenericalBlock* dirBlock = handler.getBlock(dirPos);
    if (dirBlock && dirBlock->type == BlockType::Directory) {
        DirectoryBlock* directory = dirBlock->content.directory;
        std::cout << "Directory - Entry Count: " << directory->entryCount << std::endl;
    }
    
    // Acceder al NodeBlock
    GenericalBlock* nodeBlock = handler.getBlock(nodePos);
    if (nodeBlock && nodeBlock->type == BlockType::Node) {
        NodeBlock* node = nodeBlock->content.node;
        std::cout << "Node - ASCII Size: " << node->asciiSize << std::endl;
        std::cout << "Node - Permissions: " << node->permissions << std::endl;
    }
    
    // Acceder a DataBlocks
    GenericalBlock* dataBlock1 = handler.getBlock(dataPos1);
    if (dataBlock1 && dataBlock1->type == BlockType::Data) {
        DataBlock* data1 = dataBlock1->content.data;
        std::cout << "Data 1 Content: " << data1->content << std::endl;
    }
    
    GenericalBlock* dataBlock2 = handler.getBlock(dataPos2);
    if (dataBlock2 && dataBlock2->type == BlockType::Data) {
        DataBlock* data2 = dataBlock2->content.data;
        std::cout << "Data 2 Content: " << data2->content << std::endl;
    }
    
    // Probar liberación de un bloque
    std::cout << "\n7. Liberando DataBlock en posición " << dataPos1 << "..." << std::endl;
    if (handler.freeDataBlock(dataPos1)) {
        std::cout << "DataBlock liberado exitosamente" << std::endl;
    } else {
        std::cout << "Error al liberar DataBlock" << std::endl;
    }
    
    std::cout << "\n8. Estado final después de liberar bloque:" << std::endl;
    handler.printAllBlocks();
    
    // Verificar que los bloques libres se actualizaron
    GenericalBlock* updatedMetaBlock = handler.getBlock(0);
    if (updatedMetaBlock && updatedMetaBlock->type == BlockType::MetaData) {
        MetaDataBlock* metaData = updatedMetaBlock->content.metaData;
        std::cout << "MetaData - Free Blocks actualizados: " << metaData->totalFreeBlocks << std::endl;
    }
    
    std::cout << "\n=== Prueba completada ===" << std::endl;
    
    return 0;
}