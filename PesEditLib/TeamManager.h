#ifndef TEAM_MANAGER_H
#define TEAM_MANAGER_H

#include <vector>
#include <string>
#include "PesTypes.h"

class EditDatabase;

class TeamManager {
public:
    explicit TeamManager(EditDatabase &db);

    bool LoadAll();
    Team* Get(uint32_t team_id);
    const Team* Get(uint32_t team_id) const;
    std::vector<uint32_t> GetRoster(uint32_t team_id) const;

    std::vector<Team>& GetAll() { return m_teams; }

private:
    EditDatabase &m_db;
    std::vector<Team> m_teams;
};

#endif // TEAM_MANAGER_H
