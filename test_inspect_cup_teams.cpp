#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "PesTransferTest/pes_crypt.h"

int main() {
    uint32_t size = 0;
    uint8_t* raw = readFile("CUP00000000", &size);
    if (!raw) return 1;

    FileDescriptor* desc = createFileDescriptor();
    decryptWithKey(desc, raw, MasterKeyPes21);
    free(raw);

    uint8_t* data = desc->data;
    uint32_t dataSize = desc->fileHeader->dataSize;

    std::cout << "--- ANALISANDO ESTRUTURA INTERNA DO BRACKET / PARTICIPANTES DO CUP00000000 ---\n";
    std::cout << "Mapeando ocorrencia de IDs de times na regiao 0x0 - 0x100000...\n";

    // 2453 é o ID do Bahia. Vamos ver o contexto ao redor das ocorrencias do Bahia no binario!
    // Na busca anterior, os offsets foram: 0x1dd758, 0x1ef0a8, 0x212660, 0x2133b8
    
    // Vamos procurar por IDs de times proximos como Cuiabá (ID no edit), Athletico PR, etc.
    // Vamos varrer a regiao entre 0x000F7000 e 0x00100000 procurando por IDs de 16 ou 32 bits
    for (size_t i = 0x0; i < 0x200000; i += 2) {
        uint16_t id16 = data[i] | (data[i+1] << 8);
        if (id16 == 2453) {
            std::cout << "ID16 Bahia (2453) no offset 0x" << std::hex << i << " (" << std::dec << i << ")\n";
        }
    }

    destroyFileDescriptor(desc);
    return 0;
}
