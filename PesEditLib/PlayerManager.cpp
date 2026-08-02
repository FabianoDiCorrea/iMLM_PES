#include "PlayerManager.h"
#include "EditDatabase.h"
#include "BinaryIO.h"
#include <algorithm>

PlayerManager::PlayerManager(EditDatabase &db) : m_db(db) {}

bool PlayerManager::LoadAll()
{
    m_players.clear();
    uint8_t *data = m_db.GetRawData();
    uint32_t count = m_db.GetNumPlayers();

    if (!data || count == 0) return false;

    m_players.resize(count);
    int current_byte = 0x7C;
    for (uint32_t i = 0; i < count; ++i) {
        BinaryIO::parse_player_entry(m_players[i], current_byte, data, i);
    }
    return true;
}

Player* PlayerManager::Get(uint32_t player_id)
{
    for (auto &p : m_players) {
        if (p.id == player_id) return &p;
    }
    return nullptr;
}

const Player* PlayerManager::Get(uint32_t player_id) const
{
    for (const auto &p : m_players) {
        if (p.id == player_id) return &p;
    }
    return nullptr;
}

std::vector<Player*> PlayerManager::Find(const std::wstring &name_substring)
{
    std::vector<Player*> results;
    std::wstring lower_query = name_substring;
    for (auto &ch : lower_query) ch = towlower(ch);

    for (auto &p : m_players) {
        std::wstring pname = p.name;
        for (auto &ch : pname) ch = towlower(ch);

        if (pname.find(lower_query) != std::wstring::npos) {
            results.push_back(&p);
        }
    }
    return results;
}

bool PlayerManager::UpdatePlayerAge(uint32_t player_id, uint8_t new_age)
{
    Player* p = Get(player_id);
    if (!p) return false;
    p->age = new_age;
    return SavePlayer(player_id);
}

bool PlayerManager::UpdatePlayerHeight(uint32_t player_id, uint8_t new_height)
{
    Player* p = Get(player_id);
    if (!p) return false;
    p->height = new_height;
    return SavePlayer(player_id);
}

bool PlayerManager::UpdatePlayerWeight(uint32_t player_id, uint8_t new_weight)
{
    Player* p = Get(player_id);
    if (!p) return false;
    p->weight = new_weight;
    return SavePlayer(player_id);
}

bool PlayerManager::UpdatePlayerPosition(uint32_t player_id, Position new_pos)
{
    Player* p = Get(player_id);
    if (!p) return false;
    p->reg_pos = new_pos;
    return SavePlayer(player_id);
}

bool PlayerManager::UpdatePlayerSkills(uint32_t player_id, const Player &skills)
{
    Player* p = Get(player_id);
    if (!p) return false;

    p->atk = skills.atk;
    p->def = skills.def;
    p->ball_ctrl = skills.ball_ctrl;
    p->drib = skills.drib;
    p->lowpass = skills.lowpass;
    p->loftpass = skills.loftpass;
    p->finish = skills.finish;
    p->place_kick = skills.place_kick;
    p->swerve = skills.swerve;
    p->header = skills.header;
    p->ball_win = skills.ball_win;
    p->aggres = skills.aggres;
    p->gk = skills.gk;
    p->catching = skills.catching;
    p->clearing = skills.clearing;
    p->reflex = skills.reflex;
    p->cover = skills.cover;
    p->speed = skills.speed;
    p->exp_pwr = skills.exp_pwr;
    p->body_ctrl = skills.body_ctrl;
    p->phys_cont = skills.phys_cont;
    p->jump = skills.jump;
    p->stamina = skills.stamina;
    p->kick_pwr = skills.kick_pwr;

    // Recalcula o Overall com base nas tabelas de peso oficiais por Posição do PES
    double score = 0.0;
    switch (p->reg_pos) {
    case Position::GK:
        score = (p->gk * 0.22) + (p->catching * 0.22) + (p->clearing * 0.20) + 
                (p->reflex * 0.22) + (p->cover * 0.14);
        break;
    case Position::CB:
        score = (p->def * 0.28) + (p->ball_win * 0.25) + (p->phys_cont * 0.18) + 
                (p->header * 0.15) + (p->jump * 0.14);
        break;
    case Position::LB:
    case Position::RB:
        score = (p->speed * 0.20) + (p->stamina * 0.18) + (p->def * 0.18) + 
                (p->loftpass * 0.16) + (p->ball_win * 0.14) + (p->exp_pwr * 0.14);
        break;
    case Position::DMF:
        score = (p->ball_win * 0.22) + (p->def * 0.20) + (p->lowpass * 0.20) + 
                (p->stamina * 0.15) + (p->phys_cont * 0.13) + (p->loftpass * 0.10);
        break;
    case Position::CMF:
        score = (p->lowpass * 0.22) + (p->ball_ctrl * 0.20) + (p->stamina * 0.18) + 
                (p->loftpass * 0.15) + (p->drib * 0.13) + (p->def * 0.12);
        break;
    case Position::LMF:
    case Position::RMF:
        score = (p->speed * 0.20) + (p->drib * 0.20) + (p->ball_ctrl * 0.18) + 
                (p->lowpass * 0.16) + (p->loftpass * 0.14) + (p->stamina * 0.12);
        break;
    case Position::AMF:
        score = (p->lowpass * 0.22) + (p->ball_ctrl * 0.22) + (p->drib * 0.20) + 
                (p->atk * 0.14) + (p->finish * 0.12) + (p->loftpass * 0.10);
        break;
    case Position::LWF:
    case Position::RWF:
        score = (p->speed * 0.22) + (p->drib * 0.22) + (p->exp_pwr * 0.18) + 
                (p->ball_ctrl * 0.15) + (p->finish * 0.13) + (p->atk * 0.10);
        break;
    case Position::SS:
        score = (p->ball_ctrl * 0.20) + (p->drib * 0.20) + (p->atk * 0.20) + 
                (p->finish * 0.18) + (p->lowpass * 0.12) + (p->speed * 0.10);
        break;
    case Position::CF:
        score = (p->finish * 0.25) + (p->atk * 0.25) + (p->header * 0.15) + 
                (p->phys_cont * 0.15) + (p->kick_pwr * 0.10) + (p->jump * 0.10);
        break;
    default:
        score = (p->atk + p->def + p->drib + p->speed + p->stamina) / 5.0;
        break;
    }
    p->overall = (int)(score + 0.5); // Arredondamento preciso


    return SavePlayer(player_id);
}

bool PlayerManager::UpdatePlayer(const Player &player)
{
    Player* p = Get(player.id);
    if (!p) return false;

    *p = player;
    return SavePlayer(player.id);
}

bool PlayerManager::SavePlayer(uint32_t player_id)
{
    const Player* p = Get(player_id);
    if (!p || p->global_index < 0) return false;

    uint8_t *data = m_db.GetRawData();
    if (!data) return false;

    int current_byte = 0x7C + (p->global_index * 312);
    BinaryIO::write_player_entry(*p, current_byte, data);
    return true;
}
