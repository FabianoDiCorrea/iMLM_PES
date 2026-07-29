#ifndef EDIT_DATABASE_H
#define EDIT_DATABASE_H

#include <string>
#include <vector>
#include <memory>
#include "PesTypes.h"

class PlayerManager;
class TeamManager;
class TransferManager;

class EditDatabase {
public:
    EditDatabase();
    ~EditDatabase();

    bool Load(const std::string &filepath);
    bool Save(const std::string &filepath);
    bool Validate(std::vector<std::string> &out_errors) const;

    PlayerManager& Players();
    TeamManager& Teams();
    TransferManager& Transfers();

    uint8_t* GetRawData() { return m_raw_data; }
    uint32_t GetRawSize() const { return m_raw_size; }
    uint32_t GetNumPlayers() const { return m_num_players; }
    uint32_t GetNumTeams() const { return m_num_teams; }

private:
    uint8_t *m_raw_data;
    uint32_t m_raw_size;
    uint32_t m_num_players;
    uint32_t m_num_teams;
    void *m_descriptor;

    std::unique_ptr<PlayerManager> m_player_manager;
    std::unique_ptr<TeamManager> m_team_manager;
    std::unique_ptr<TransferManager> m_transfer_manager;

    void SyncRostersAndPlayers();
};

#endif // EDIT_DATABASE_H
