#include "TransferManager.h"
#include "EditDatabase.h"
#include "PlayerManager.h"
#include "TeamManager.h"
#include "BinaryIO.h"

TransferManager::TransferManager(EditDatabase &db) : m_db(db) {}

TransferResult TransferManager::TransferPlayer(uint32_t player_id, uint32_t dest_team_id)
{
    TransferResult res;
    res.player_id = player_id;

    Player *player = m_db.Players().Get(player_id);
    if (!player) {
        res.message = "Player not found.";
        return res;
    }
    res.player_name = player->name;

    Team *dest_team = m_db.Teams().Get(dest_team_id);
    if (!dest_team) {
        res.message = "Destination team not found.";
        return res;
    }
    res.dest_team_name = dest_team->name;

    uint8_t *data = m_db.GetRawData();
    int orig_team_idx = player->team_index;
    int orig_slot = player->roster_slot;

    if (orig_team_idx != -1) {
        Team &orig_team = m_db.Teams().GetAll()[orig_team_idx];
        res.source_team_name = orig_team.name;
        res.old_slot = orig_slot;

        orig_team.players[orig_slot] = 0;
        orig_team.numbers[orig_slot] = 0;

        for (int k = orig_slot; k < 39; ++k) {
            orig_team.players[k] = orig_team.players[k+1];
            orig_team.numbers[k] = orig_team.numbers[k+1];
        }
        orig_team.players[39] = 0;
        orig_team.numbers[39] = 0;
        orig_team.player_count--;

        int w_offset = orig_team.roster_offset + 4;
        for (int k = 0; k < 40; ++k) BinaryIO::write_data(orig_team.players[k], 0, 32, w_offset, data);
        for (int k = 0; k < 40; ++k) BinaryIO::write_data(orig_team.numbers[k], 0, 16, w_offset, data);
    } else {
        res.source_team_name = L"Free Agent / Sem Clube";
    }

    int dest_slot = -1;
    for (int k = 0; k < 40; ++k) {
        if (dest_team->players[k] == 0) {
            dest_slot = k;
            break;
        }
    }

    if (dest_slot == -1) {
        res.message = "Destination team roster is full (40 slots).";
        return res;
    }

    res.new_slot = dest_slot;
    dest_team->players[dest_slot] = player->id;
    dest_team->numbers[dest_slot] = (player->shirt_number > 0) ? player->shirt_number : 10;
    dest_team->player_count++;

    int w_offset = dest_team->roster_offset + 4;
    for (int k = 0; k < 40; ++k) BinaryIO::write_data(dest_team->players[k], 0, 32, w_offset, data);
    for (int k = 0; k < 40; ++k) BinaryIO::write_data(dest_team->numbers[k], 0, 16, w_offset, data);

    // Update in-memory player reference
    player->team_index = dest_team->global_index;
    player->roster_slot = dest_slot;

    res.success = true;
    res.message = "Transfer completed successfully.";
    return res;
}
