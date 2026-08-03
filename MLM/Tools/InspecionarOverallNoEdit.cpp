#include "../../PesEditLib/EditDatabase.h"
#include "../../PesEditLib/PlayerManager.h"
#include "../../PesEditLib/BinaryIO.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

struct Target {
    uint32_t id;
    int pesOverall;
    const char* name;
};

static int ReadBits(const uint8_t* data, int absoluteBit, int bits) {
    int currentByte = absoluteBit / 8;
    int startBit = absoluteBit % 8;
    return BinaryIO::read_data(startBit, bits, currentByte, data);
}

int main() {
    const char* editPath = "M:/Projetos DevDuo/PES-Edit-Research-Codex/EDIT00000000";
    EditDatabase db;
    if (!db.Load(editPath)) {
        std::cerr << "Falha ao abrir EDIT" << std::endl;
        return 1;
    }

    std::vector<Target> targets = {
        {120134, 73, "Robinho Junior"},
        {40352, 83, "Neymar"},
        {126746, 79, "Gabriel Brazao"},
        {129510, 83, "Bento"},
        {114593, 85, "Bremer"},
        {46815, 86, "Ederson"},
        {43989, 80, "Danilo"},
        {117505, 80, "Carlos Cuesta"},
    };

    auto& players = db.Players().GetAll();
    const uint8_t* data = db.GetRawData();
    std::map<uint32_t, int> indexById;
    for (const auto& player : players) {
        indexById[player.id] = player.global_index;
    }

    std::vector<int> candidate7;
    std::vector<int> candidate8;
    const int recordBits = 312 * 8;

    for (int bit = 0; bit <= recordBits - 7; ++bit) {
        bool ok = true;
        for (const auto& target : targets) {
            auto it = indexById.find(target.id);
            if (it == indexById.end()) { ok = false; break; }
            int absoluteBit = ((0x7C + it->second * 312) * 8) + bit;
            if (ReadBits(data, absoluteBit, 7) != target.pesOverall) { ok = false; break; }
        }
        if (ok) candidate7.push_back(bit);
    }

    for (int bit = 0; bit <= recordBits - 8; ++bit) {
        bool ok = true;
        for (const auto& target : targets) {
            auto it = indexById.find(target.id);
            if (it == indexById.end()) { ok = false; break; }
            int absoluteBit = ((0x7C + it->second * 312) * 8) + bit;
            if (ReadBits(data, absoluteBit, 8) != target.pesOverall) { ok = false; break; }
        }
        if (ok) candidate8.push_back(bit);
    }

    std::cout << "Candidatos 7-bit iguais ao overall PES em todos os alvos: " << candidate7.size() << std::endl;
    for (int bit : candidate7) std::cout << "  bit=" << bit << " byte+" << (bit / 8) << " startBit=" << (bit % 8) << std::endl;

    std::cout << "Candidatos 8-bit iguais ao overall PES em todos os alvos: " << candidate8.size() << std::endl;
    for (int bit : candidate8) std::cout << "  bit=" << bit << " byte+" << (bit / 8) << " startBit=" << (bit % 8) << std::endl;

    std::cout << "Alvos localizados:" << std::endl;
    for (const auto& target : targets) {
        auto it = indexById.find(target.id);
        std::cout << "  " << target.id << " idx=" << (it == indexById.end() ? -1 : it->second) << " PES=" << target.pesOverall << std::endl;
    }

    return 0;
}

