#include <iostream>
#include <windows.h>
#include "../../PesEditLib/EditDatabase.h"
#include "../../PesEditLib/PlayerManager.h"

#include "../../PesEditLib/BinaryIO.h"

std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (size_needed <= 0) return "";
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

int main() {
    EditDatabase db;
    std::string path = "C:/Users/fabia/OneDrive/Documents/KONAMI/eFootball PES 2021 SEASON UPDATE/76561198347723698/save/EDIT00000000";
    if (!db.Load(path)) return 1;
    uint8_t* raw = db.GetRawData();

    std::cout << "=== INSPECAO DE ATRIBUTOS DO GABRIEL BRAZAO (ID 126746) ===\n";
    uint32_t ids[] = {126746, 129510};
    for (uint32_t target_id : ids) {
        for (int i = 0x7C; i < 0x8ED2FC; i += 312) {
            uint32_t val = raw[i] | (raw[i+1] << 8) | (raw[i+2] << 16) | (raw[i+3] << 24);
            if (val == target_id) {
                Player p;
                int cb = i;
                BinaryIO::parse_player_entry(p, cb, raw, 0);
                std::cout << "Jogador ID " << target_id << " (" << WStringToString(p.name) << "):\n";
                for (int k = 0; k < 312; ++k) {
                    uint8_t b = raw[i + k];
                    if (b >= 75 && b <= 95) {
                        printf(" Offset +0x%02X (%3d): %d (0x%02X)\n", k, k, b, b);
                    }
                }
                std::cout << "\n";
                break;
            }
        }
    }
    return 0;
}
