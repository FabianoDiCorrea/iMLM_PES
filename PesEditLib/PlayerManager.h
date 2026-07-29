#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <vector>
#include <string>
#include "PesTypes.h"

class EditDatabase;

class PlayerManager {
public:
    explicit PlayerManager(EditDatabase &db);

    bool LoadAll();
    Player* Get(uint32_t player_id);
    const Player* Get(uint32_t player_id) const;
    std::vector<Player*> Find(const std::wstring &name_substring);
    
    std::vector<Player>& GetAll() { return m_players; }

    // API de Edição de Atributos do Jogador
    bool UpdatePlayerAge(uint32_t player_id, uint8_t new_age);
    bool UpdatePlayerHeight(uint32_t player_id, uint8_t new_height);
    bool UpdatePlayerWeight(uint32_t player_id, uint8_t new_weight);
    bool UpdatePlayerPosition(uint32_t player_id, Position new_pos);
    bool UpdatePlayerSkills(uint32_t player_id, const Player &skills);
    bool UpdatePlayer(const Player &player);
    bool SavePlayer(uint32_t player_id);

private:
    EditDatabase &m_db;
    std::vector<Player> m_players;
};

#endif // PLAYER_MANAGER_H
