#include "TeamManager.h"
#include "EditDatabase.h"
#include "BinaryIO.h"

TeamManager::TeamManager(EditDatabase &db) : m_db(db) {}

bool TeamManager::LoadAll()
{
    m_teams.clear();
    uint8_t *data = m_db.GetRawData();
    uint32_t count = m_db.GetNumTeams();

    if (!data || count == 0) return false;

    m_teams.resize(count);
    int current_byte = 0x8ED2FC;
    for (uint32_t i = 0; i < count; ++i) {
        BinaryIO::parse_team_entry(m_teams[i], current_byte, data, i);
    }
    return true;
}

Team* TeamManager::Get(uint32_t team_id)
{
    for (auto &t : m_teams) {
        if (t.id == team_id) return &t;
    }
    return nullptr;
}

const Team* TeamManager::Get(uint32_t team_id) const
{
    for (const auto &t : m_teams) {
        if (t.id == team_id) return &t;
    }
    return nullptr;
}

std::vector<uint32_t> TeamManager::GetRoster(uint32_t team_id) const
{
    const Team *t = Get(team_id);
    if (!t) return {};

    std::vector<uint32_t> roster;
    for (int k = 0; k < 40; ++k) {
        if (t->players[k] != 0) {
            roster.push_back(t->players[k]);
        }
    }
    return roster;
}
