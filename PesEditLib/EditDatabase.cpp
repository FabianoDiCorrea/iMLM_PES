#include "EditDatabase.h"
#include "PlayerManager.h"
#include "TeamManager.h"
#include "TransferManager.h"
#include "BinaryIO.h"
#include "../PesTransferTest/pes_crypt.h"
#include <cstdlib>

EditDatabase::EditDatabase()
    : m_raw_data(nullptr), m_raw_size(0), m_num_players(0), m_num_teams(0), m_descriptor(nullptr)
{
    m_player_manager = std::make_unique<PlayerManager>(*this);
    m_team_manager = std::make_unique<TeamManager>(*this);
    m_transfer_manager = std::make_unique<TransferManager>(*this);
}

EditDatabase::~EditDatabase()
{
    if (m_descriptor) {
        destroyFileDescriptor(static_cast<FileDescriptor*>(m_descriptor));
        m_descriptor = nullptr;
    }
}

bool EditDatabase::Load(const std::string &filepath)
{
    if (m_descriptor) {
        destroyFileDescriptor(static_cast<FileDescriptor*>(m_descriptor));
        m_descriptor = nullptr;
    }

    uint32_t raw_size = 0;
    uint8_t *raw_input = readFile(filepath.c_str(), &raw_size);
    if (!raw_input) return false;

    FileDescriptor *descriptor = createFileDescriptor();
    decryptWithKey(descriptor, raw_input, MasterKeyPes21);
    free(raw_input);

    m_descriptor = descriptor;
    m_raw_data = descriptor->data;
    m_raw_size = descriptor->fileHeader->dataSize;

    m_num_players = m_raw_data[0x60] + (m_raw_data[0x61] * 256);
    m_num_teams = m_raw_data[0x64] + (m_raw_data[0x65] * 256);

    if (!m_player_manager->LoadAll()) return false;
    if (!m_team_manager->LoadAll()) return false;

    SyncRostersAndPlayers();
    return true;
}

void EditDatabase::SyncRostersAndPlayers()
{
    int roster_byte = 0x9D4648;
    auto &teams = m_team_manager->GetAll();
    auto &players = m_player_manager->GetAll();

    for (uint32_t i = 0; i < m_num_teams; ++i) {
        int roster_start_byte = roster_byte;
        uint32_t roster_team_id = BinaryIO::read_data(0, 32, roster_byte, m_raw_data);

        Team* pTargetTeam = nullptr;
        int t_ind = -1;
        for (size_t t = 0; t < teams.size(); ++t) {
            if (teams[t].id == roster_team_id) {
                t_ind = (int)t;
                pTargetTeam = &teams[t];
                break;
            }
        }

        // Se o time estiver na lista de times (ou se index i corresponder)
        if (!pTargetTeam && i < teams.size()) {
            pTargetTeam = &teams[i];
            t_ind = i;
        }

        if (pTargetTeam) {
            Team &target_team = *pTargetTeam;
            target_team.roster_offset = roster_start_byte;
            target_team.player_count = 0;

            for (int k = 0; k < 40; ++k) {
                target_team.players[k] = BinaryIO::read_data(0, 32, roster_byte, m_raw_data);
                if (target_team.players[k] != 0) {
                    target_team.player_count++;
                }
            }
            for (int k = 0; k < 40; ++k) {
                target_team.numbers[k] = (uint16_t)BinaryIO::read_data(0, 16, roster_byte, m_raw_data);
            }

            for (int k = 0; k < 40; ++k) {
                if (target_team.players[k] != 0) {
                    for (uint32_t p = 0; p < m_num_players; ++p) {
                        if (players[p].id == target_team.players[k]) {
                            players[p].team_index = t_ind;
                            players[p].roster_slot = k;
                            players[p].shirt_number = target_team.numbers[k];
                            break;
                        }
                    }
                }
            }
        }

        roster_byte = roster_start_byte + 0x11C;
    }
}

bool EditDatabase::Save(const std::string &filepath)
{
    if (!m_descriptor) return false;

    // Salvar modificações dos times no raw_data
    uint8_t *data = m_raw_data;
    if (data && m_num_teams > 0) {
        auto& teams = m_team_manager->GetAll();
        int current_byte = 0x8ED2FC;
        for (uint32_t i = 0; i < m_num_teams; ++i) {
            BinaryIO::write_team_entry(teams[i], current_byte, data);
        }
    }

    // Salvar modificações dos jogadores no raw_data
    if (data && m_num_players > 0) {
        auto& players = m_player_manager->GetAll();
        int current_byte = 0x7C;
        for (uint32_t i = 0; i < m_num_players; ++i) {
            BinaryIO::write_player_entry(players[i], current_byte, data);
        }
    }

    int output_size = 0;
    uint8_t *encrypted_output = encryptWithKey(static_cast<FileDescriptor*>(m_descriptor), &output_size, MasterKeyPes21);
    writeFile(filepath.c_str(), encrypted_output, output_size);
    free(encrypted_output);
    return true;
}

bool EditDatabase::Validate(std::vector<std::string> &out_errors) const
{
    out_errors.clear();
    if (!m_raw_data) {
        out_errors.push_back("No raw database loaded.");
        return false;
    }

    uint32_t current_num_players = m_raw_data[0x60] + (m_raw_data[0x61] * 256);
    if (current_num_players != m_num_players) {
        out_errors.push_back("Player count header mismatch.");
    }

    return out_errors.empty();
}

PlayerManager& EditDatabase::Players() { return *m_player_manager; }
TeamManager& EditDatabase::Teams() { return *m_team_manager; }
TransferManager& EditDatabase::Transfers() { return *m_transfer_manager; }
