#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <direct.h>
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
    if (!strTo.empty() && strTo.back() == '\0') strTo.pop_back();
    return strTo;
}

std::string JsonEscape(const std::string& input) {
    std::string out;
    out.reserve(input.size() + 8);
    for (char c : input) {
        switch (c) {
        case '"': out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\b': out += "\\b"; break;
        case '\f': out += "\\f"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            if ((unsigned char)c >= 32) out += c;
            break;
        }
    }
    return out;
}

std::string PlayerName(const Player& p) {
    std::string name = WStringToString(p.name);
    if (name.empty()) name = p.shirt_name;
    if (name.empty()) name = "Jogador " + std::to_string(p.id);
    return name;
}

void WriteIntArray(std::ofstream& jsonFile, const uint8_t* values, int count) {
    jsonFile << "[";
    for (int idx = 0; idx < count; ++idx) {
        if (idx > 0) jsonFile << ", ";
        jsonFile << static_cast<int>(values[idx]);
    }
    jsonFile << "]";
}

void WriteOverallByPosition(std::ofstream& jsonFile, const Player& p) {
    jsonFile << "{";
    const char* keys[13] = {"GOL","ZC","LE","LD","VOL","MLG","MLE","MLD","MAT","PTE","PTD","SA","CA"};
    for (int idx = 0; idx < 13; ++idx) {
        if (idx > 0) jsonFile << ", ";
        jsonFile << "\"" << keys[idx] << "\": " << p.overall_by_position[idx];
    }
    jsonFile << "}";
}

void WritePlayerJson(std::ofstream& jsonFile, const Player& p, int indent) {
    std::string pad(indent, ' ');
    std::string pNome = JsonEscape(PlayerName(p));
    std::string shirtName = JsonEscape(p.shirt_name);

    jsonFile << pad << "{\n";
    jsonFile << pad << "  \"id\": " << p.id << ",\n";
    jsonFile << pad << "  \"nome\": \"" << pNome << "\",\n";
    jsonFile << pad << "  \"nomeCamisa\": \"" << shirtName << "\",\n";
    jsonFile << pad << "  \"idade\": " << static_cast<int>(p.age) << ",\n";
    jsonFile << pad << "  \"altura\": " << static_cast<int>(p.height) << ",\n";
    jsonFile << pad << "  \"peso\": " << static_cast<int>(p.weight) << ",\n";
    jsonFile << pad << "  \"overall\": " << p.overall << ",\n";
    jsonFile << pad << "  \"overallPesJogo\": " << p.overall << ",\n";
    jsonFile << pad << "  \"overallFormulaVersao\": \"imlm-pes21-pos-v0.3\",\n";
    jsonFile << pad << "  \"overallPorPosicao\": ";
    WriteOverallByPosition(jsonFile, p);
    jsonFile << ",\n";
    jsonFile << pad << "  \"posicao\": " << static_cast<int>(p.reg_pos) << ",\n";
    jsonFile << pad << "  \"formaFisica\": " << static_cast<int>(p.form) << ",\n";
    jsonFile << pad << "  \"resistenciaLesao\": " << static_cast<int>(p.injury) << ",\n";
    jsonFile << pad << "  \"piorPeUso\": " << static_cast<int>(p.weak_use) << ",\n";
    jsonFile << pad << "  \"piorPePrecisao\": " << static_cast<int>(p.weak_acc) << ",\n";
    jsonFile << pad << "  \"peDominante\": " << static_cast<int>(p.strong_foot) << ",\n";
    jsonFile << pad << "  \"maoDominante\": " << static_cast<int>(p.strong_hand) << ",\n";
    jsonFile << pad << "  \"estiloJogo\": " << static_cast<int>(p.play_style) << ",\n";
    jsonFile << pad << "  \"atitudeJogo\": " << static_cast<int>(p.play_attit) << ",\n";
    jsonFile << pad << "  \"estrela\": " << static_cast<int>(p.star) << ",\n";
    jsonFile << pad << "  \"dribleMotion\": " << static_cast<int>(p.dribble_motion) << ",\n";
    jsonFile << pad << "  \"stats\": {\n";
    jsonFile << pad << "    \"atk\": " << static_cast<int>(p.atk) << ",\n";
    jsonFile << pad << "    \"ball_ctrl\": " << static_cast<int>(p.ball_ctrl) << ",\n";
    jsonFile << pad << "    \"tight_pos\": " << static_cast<int>(p.tight_pos) << ",\n";
    jsonFile << pad << "    \"drib\": " << static_cast<int>(p.drib) << ",\n";
    jsonFile << pad << "    \"lowpass\": " << static_cast<int>(p.lowpass) << ",\n";
    jsonFile << pad << "    \"loftpass\": " << static_cast<int>(p.loftpass) << ",\n";
    jsonFile << pad << "    \"finish\": " << static_cast<int>(p.finish) << ",\n";
    jsonFile << pad << "    \"header\": " << static_cast<int>(p.header) << ",\n";
    jsonFile << pad << "    \"place_kick\": " << static_cast<int>(p.place_kick) << ",\n";
    jsonFile << pad << "    \"swerve\": " << static_cast<int>(p.swerve) << ",\n";
    jsonFile << pad << "    \"speed\": " << static_cast<int>(p.speed) << ",\n";
    jsonFile << pad << "    \"exp_pwr\": " << static_cast<int>(p.exp_pwr) << ",\n";
    jsonFile << pad << "    \"kick_pwr\": " << static_cast<int>(p.kick_pwr) << ",\n";
    jsonFile << pad << "    \"jump\": " << static_cast<int>(p.jump) << ",\n";
    jsonFile << pad << "    \"phys_cont\": " << static_cast<int>(p.phys_cont) << ",\n";
    jsonFile << pad << "    \"body_ctrl\": " << static_cast<int>(p.body_ctrl) << ",\n";
    jsonFile << pad << "    \"stamina\": " << static_cast<int>(p.stamina) << ",\n";
    jsonFile << pad << "    \"def\": " << static_cast<int>(p.def) << ",\n";
    jsonFile << pad << "    \"ball_win\": " << static_cast<int>(p.ball_win) << ",\n";
    jsonFile << pad << "    \"aggres\": " << static_cast<int>(p.aggres) << ",\n";
    jsonFile << pad << "    \"gk\": " << static_cast<int>(p.gk) << ",\n";
    jsonFile << pad << "    \"catching\": " << static_cast<int>(p.catching) << ",\n";
    jsonFile << pad << "    \"clearing\": " << static_cast<int>(p.clearing) << ",\n";
    jsonFile << pad << "    \"reflex\": " << static_cast<int>(p.reflex) << ",\n";
    jsonFile << pad << "    \"cover\": " << static_cast<int>(p.cover) << ",\n";
    jsonFile << pad << "    \"weak_use\": " << static_cast<int>(p.weak_use) << ",\n";
    jsonFile << pad << "    \"weak_acc\": " << static_cast<int>(p.weak_acc) << ",\n";
    jsonFile << pad << "    \"form\": " << static_cast<int>(p.form) << ",\n";
    jsonFile << pad << "    \"injury\": " << static_cast<int>(p.injury) << "\n";
    jsonFile << pad << "  },\n";
    jsonFile << pad << "  \"posicoesJogaveis\": ";
    WriteIntArray(jsonFile, p.playable_pos, 13);
    jsonFile << ",\n";
    jsonFile << pad << "  \"estilosCOM\": ";
    WriteIntArray(jsonFile, p.com_style, 7);
    jsonFile << ",\n";
    jsonFile << pad << "  \"habilidadesJogador\": ";
    WriteIntArray(jsonFile, p.play_skill, 41);
    jsonFile << "\n";
    jsonFile << pad << "}";
}

bool IsValidTeam(const Team& t) {
    std::string nomeTime = WStringToString(t.name);
    return !nomeTime.empty() && nomeTime.find("Unused") == std::string::npos && t.id > 0;
}

bool IsValidPlayer(const Player& p) {
    return p.id > 0 && (!PlayerName(p).empty());
}

int main() {
    std::string editPath = "C:/Users/fabia/OneDrive/Documents/KONAMI/eFootball PES 2021 SEASON UPDATE/76561198347723698/save/EDIT00000000";
    EditDatabase db;
    if (!db.Load(editPath)) return 1;

    auto& times = db.Teams().GetAll();
    auto& todosJogadores = db.Players().GetAll();

    _mkdir("../Data");

    std::ofstream jsonFile("../UI/pes_database.json");
    std::ofstream baseFile("../Data/imlm_base_pes21.json");
    if (!jsonFile.is_open() || !baseFile.is_open()) return 1;

    uint32_t totalClubesLidos = 0;
    uint32_t totalJogadoresElenco = 0;
    std::set<uint32_t> jogadoresUnicosElenco;

    jsonFile << "{\n";
    jsonFile << "  \"clubes\": [\n";

    bool primeiroClube = true;
    for (auto& t : times) {
        if (!IsValidTeam(t)) continue;
        if (!primeiroClube) jsonFile << ",\n";
        primeiroClube = false;

        std::string nomeTime = JsonEscape(WStringToString(t.name));
        std::string sigla = JsonEscape(!t.short_name.empty() ? t.short_name : nomeTime.substr(0, 3));

        jsonFile << "    {\n";
        jsonFile << "      \"id\": " << t.id << ",\n";
        jsonFile << "      \"nome\": \"" << nomeTime << "\",\n";
        jsonFile << "      \"sigla\": \"" << sigla << "\",\n";
        jsonFile << "      \"jogadores\": [\n";

        bool primeiroJogador = true;
        for (int slot = 0; slot < 40; ++slot) {
            uint32_t pId = t.players[slot];
            if (pId == 0) continue;

            Player* pPtr = db.Players().Get(pId);
            if (!pPtr || !IsValidPlayer(*pPtr)) continue;

            if (!primeiroJogador) jsonFile << ",\n";
            primeiroJogador = false;
            WritePlayerJson(jsonFile, *pPtr, 8);

            jogadoresUnicosElenco.insert(pId);
            totalJogadoresElenco++;
        }

        jsonFile << "\n      ]\n";
        jsonFile << "    }";
        totalClubesLidos++;
    }

    jsonFile << "\n  ]\n";
    jsonFile << "}\n";

    baseFile << "{\n";
    baseFile << "  \"metadata\": {\n";
    baseFile << "    \"schema\": \"imlm-pes21-canonical-base\",\n";
    baseFile << "    \"schemaVersion\": \"0.1\",\n";
    baseFile << "    \"fonte\": \"EDIT00000000\",\n";
    baseFile << "    \"editPath\": \"" << JsonEscape(editPath) << "\",\n";
    baseFile << "    \"overallPolicy\": \"overallPesJogo calculado por formula posicional iMLM PES21 v0.3 calibrada contra alvos PES; usar como padrao inicial do universo e calibrar contra amostras do PES\",\n";
    baseFile << "    \"totalClubes\": " << totalClubesLidos << ",\n";
    baseFile << "    \"totalSlotsElenco\": " << totalJogadoresElenco << ",\n";
    baseFile << "    \"totalJogadoresUnicosEmElencos\": " << jogadoresUnicosElenco.size() << "\n";
    baseFile << "  },\n";
    baseFile << "  \"jogadores\": [\n";

    bool primeiroJogadorBase = true;
    uint32_t totalJogadoresBase = 0;
    std::set<uint32_t> idsExportados;
    for (auto& p : todosJogadores) {
        if (!IsValidPlayer(p)) continue;
        if (idsExportados.count(p.id)) continue;
        idsExportados.insert(p.id);

        if (!primeiroJogadorBase) baseFile << ",\n";
        primeiroJogadorBase = false;
        WritePlayerJson(baseFile, p, 4);
        totalJogadoresBase++;
    }

    baseFile << "\n  ],\n";
    baseFile << "  \"clubes\": [\n";

    bool primeiroClubeBase = true;
    for (auto& t : times) {
        if (!IsValidTeam(t)) continue;
        if (!primeiroClubeBase) baseFile << ",\n";
        primeiroClubeBase = false;

        std::string nomeTime = JsonEscape(WStringToString(t.name));
        std::string sigla = JsonEscape(!t.short_name.empty() ? t.short_name : nomeTime.substr(0, 3));

        baseFile << "    {\n";
        baseFile << "      \"id\": " << t.id << ",\n";
        baseFile << "      \"nome\": \"" << nomeTime << "\",\n";
        baseFile << "      \"sigla\": \"" << sigla << "\",\n";
        baseFile << "      \"elenco\": [";
        bool primeiroSlot = true;
        for (int slot = 0; slot < 40; ++slot) {
            uint32_t pId = t.players[slot];
            if (pId == 0) continue;
            Player* pPtr = db.Players().Get(pId);
            if (!pPtr || !IsValidPlayer(*pPtr)) continue;
            if (!primeiroSlot) baseFile << ", ";
            primeiroSlot = false;
            baseFile << "{\"slot\": " << slot << ", \"playerId\": " << pId << ", \"numero\": " << t.numbers[slot] << "}";
        }
        baseFile << "]\n";
        baseFile << "    }";
    }

    baseFile << "\n  ]\n";
    baseFile << "}\n";

    jsonFile.close();
    baseFile.close();

    std::cout << "[SUCESSO] pes_database.json exportado com " << totalClubesLidos << " clubes!\n";
    std::cout << "[SUCESSO] imlm_base_pes21.json exportado com " << totalJogadoresBase << " jogadores unicos!\n";
    return 0;
}

