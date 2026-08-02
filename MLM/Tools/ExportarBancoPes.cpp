#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../../PesEditLib/EditDatabase.h"
#include "../../PesEditLib/PlayerManager.h"
#include "../../PesEditLib/TeamManager.h"
#include <windows.h>

std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return "";

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (size_needed <= 0) return "";
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &strTo[0], size_needed, NULL, NULL);
    
    std::string cleanStr = "";
    for (char c : strTo) {
        if (c == '"') cleanStr += "'";
        else if (c == '\\') cleanStr += '/';
        else if ((unsigned char)c >= 32) cleanStr += c;
    }
    return cleanStr;
}

int main() {
    std::string editPath = "C:/Users/fabia/OneDrive/Documents/KONAMI/eFootball PES 2021 SEASON UPDATE/76561198347723698/save/EDIT00000000";
    EditDatabase db;
    if (!db.Load(editPath)) return 1;

    auto& times = db.Teams().GetAll();
    auto& todosJogadores = db.Players().GetAll();

    std::ofstream jsonFile("../UI/pes_database.json");
    if (!jsonFile.is_open()) return 1;

    jsonFile << "{\n";
    jsonFile << "  \"clubes\": [\n";

    bool primeiroClube = true;
    uint32_t totalClubesLidos = 0;

    for (auto& t : times) {
        std::string nomeTime = WStringToString(t.name);
        if (!nomeTime.empty() && nomeTime.find("Unused") == std::string::npos && t.id > 0) {
            if (!primeiroClube) jsonFile << ",\n";
            primeiroClube = false;

            std::string sigla = (!t.short_name.empty()) ? t.short_name : nomeTime.substr(0, 3);

            jsonFile << "    {\n";
            jsonFile << "      \"id\": " << t.id << ",\n";
            jsonFile << "      \"nome\": \"" << nomeTime << "\",\n";
            jsonFile << "      \"sigla\": \"" << sigla << "\",\n";
            jsonFile << "      \"jogadores\": [\n";

            bool primeiroJogador = true;
            // Lê os 40 slots de elenco do time
            for (int slot = 0; slot < 40; ++slot) {
                uint32_t pId = t.players[slot];
                if (pId == 0) continue;

                Player* pPtr = db.Players().Get(pId);
                if (!pPtr) continue;

                Player& p = *pPtr;
                if (!primeiroJogador) jsonFile << ",\n";
                primeiroJogador = false;

                std::string pNome = WStringToString(p.name);
                if (pNome.empty()) pNome = p.shirt_name;
                if (pNome.empty()) pNome = "Jogador " + std::to_string(p.id);

                jsonFile << "        {\n";
                jsonFile << "          \"id\": " << p.id << ",\n";
                jsonFile << "          \"nome\": \"" << pNome << "\",\n";
                jsonFile << "          \"nomeCamisa\": \"" << p.shirt_name << "\",\n";
                jsonFile << "          \"idade\": " << static_cast<int>(p.age) << ",\n";
                jsonFile << "          \"altura\": " << static_cast<int>(p.height) << ",\n";
                jsonFile << "          \"peso\": " << static_cast<int>(p.weight) << ",\n";
                jsonFile << "          \"overall\": " << p.overall << ",\n";
                jsonFile << "          \"posicao\": " << static_cast<int>(p.reg_pos) << ",\n";
                jsonFile << "          \"stats\": {\n";
                jsonFile << "            \"atk\": " << static_cast<int>(p.atk) << ",\n";
                jsonFile << "            \"ball_ctrl\": " << static_cast<int>(p.ball_ctrl) << ",\n";
                jsonFile << "            \"drib\": " << static_cast<int>(p.drib) << ",\n";
                jsonFile << "            \"lowpass\": " << static_cast<int>(p.lowpass) << ",\n";
                jsonFile << "            \"loftpass\": " << static_cast<int>(p.loftpass) << ",\n";
                jsonFile << "            \"finish\": " << static_cast<int>(p.finish) << ",\n";
                jsonFile << "            \"header\": " << static_cast<int>(p.header) << ",\n";
                jsonFile << "            \"place_kick\": " << static_cast<int>(p.place_kick) << ",\n";
                jsonFile << "            \"swerve\": " << static_cast<int>(p.swerve) << ",\n";
                jsonFile << "            \"speed\": " << static_cast<int>(p.speed) << ",\n";
                jsonFile << "            \"exp_pwr\": " << static_cast<int>(p.exp_pwr) << ",\n";
                jsonFile << "            \"kick_pwr\": " << static_cast<int>(p.kick_pwr) << ",\n";
                jsonFile << "            \"jump\": " << static_cast<int>(p.jump) << ",\n";
                jsonFile << "            \"phys_cont\": " << static_cast<int>(p.phys_cont) << ",\n";
                jsonFile << "            \"body_ctrl\": " << static_cast<int>(p.body_ctrl) << ",\n";
                jsonFile << "            \"stamina\": " << static_cast<int>(p.stamina) << ",\n";
                jsonFile << "            \"def\": " << static_cast<int>(p.def) << ",\n";
                jsonFile << "            \"ball_win\": " << static_cast<int>(p.ball_win) << ",\n";
                jsonFile << "            \"aggres\": " << static_cast<int>(p.aggres) << ",\n";
                jsonFile << "            \"gk\": " << static_cast<int>(p.gk) << ",\n";
                jsonFile << "            \"catching\": " << static_cast<int>(p.catching) << ",\n";
                jsonFile << "            \"clearing\": " << static_cast<int>(p.clearing) << ",\n";
                jsonFile << "            \"reflex\": " << static_cast<int>(p.reflex) << ",\n";
                jsonFile << "            \"cover\": " << static_cast<int>(p.cover) << "\n";
                jsonFile << "          }\n";
                jsonFile << "        }";
            }

            jsonFile << "\n      ]\n";
            jsonFile << "    }";
            totalClubesLidos++;
        }
    }

    jsonFile << "\n  ]\n";
    jsonFile << "}\n";
    jsonFile.close();

    std::cout << "[SUCESSO] pes_database.json exportado com " << totalClubesLidos << " clubes!\n";
    return 0;
}
