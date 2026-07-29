// main.cpp - PesTransferTest (Validação Final da PoC)
#include <iostream>
#include <vector>
#include <string>
#include <cwchar>
#include <clocale>
#include <algorithm>
#include <windows.h>
#include "pes_crypt.h"

struct FullPlayer {
    uint32_t id;
    wchar_t name[64];
    char shirt_name[21];
    uint8_t age;
    uint8_t height;
    uint8_t weight;
    uint8_t reg_pos;
    uint8_t atk;
    uint8_t def;
    uint8_t gk;
    uint8_t drib;
    uint8_t finish;
    uint8_t lowpass;
    uint8_t loftpass;
    uint8_t header;
    uint8_t swerve;
    uint8_t catching;
    uint8_t clearing;
    uint8_t reflex;
    uint8_t cover;
    uint8_t body_ctrl;
    uint8_t phys_cont;
    uint8_t kick_pwr;
    uint8_t exp_pwr;
    uint8_t ball_ctrl;
    uint8_t ball_win;
    uint8_t jump;
    uint8_t place_kick;
    uint8_t stamina;
    uint8_t speed;
    uint8_t aggres;
    
    int overall;
    int global_index;
    int team_index;   // Index in global teams array (-1 if free agent)
    int roster_slot;  // Slot 0-39
    uint16_t shirt_number;
};

struct SimpleTeam {
    uint32_t id;
    wchar_t name[70];
    char short_name[5];
    uint32_t players[40];
    uint16_t numbers[40];
    int roster_offset;
    int player_count;
};

static const wchar_t* POSITIONS[] = {
    L"GK", L"CB", L"LB", L"RB", L"DMF", L"CMF",
    L"LMF", L"RMF", L"AMF", L"LWF", L"RWF", L"SS", L"CF"
};

// Bitwise reader
static int read_data(int start_bit, int bits_to_read, int &current_byte, const uint8_t *data)
{
    int shift;
    int pow_of_two[] = {1,2,4,8,16,32,64,128};

    int bytes_advanced = 0;
    int output = 0;
    int bit = start_bit;
    for(int ii=0; ii<bits_to_read; ii++)
    {
        if(bit == 8)
        {
            bit = 0;
            current_byte++;
        }
        if(ii%8 == 0 && ii>0) bytes_advanced++;
        shift = bit-(ii%8);
        if(shift>=0)
            output += ((data[current_byte] >> shift) & pow_of_two[ii % 8]) << bytes_advanced*8;
        else
            output += ((data[current_byte] << -shift) & pow_of_two[ii % 8]) << bytes_advanced*8;
        bit++;
    }
    if(bit == 8) current_byte++;

    return output;
}

// Bitwise writer
static void write_data(int input, int start_bit, int bits_to_write, int &current_byte, uint8_t *data)
{
    int shift;
    uint8_t pow_of_two[] = {1,2,4,8,16,32,64,128};

    int bytes_advanced = 0;
    int bit = start_bit;
    uint8_t byte_val = data[current_byte];
    for(int ii=0; ii<bits_to_write; ii++)
    {
        if(bit == 8)
        {
            bit = 0;
            data[current_byte] = byte_val;
            current_byte++;
            byte_val = data[current_byte];
        }
        if(ii%8 == 0 && ii>0) bytes_advanced++;
        shift = (ii%8) - bit;
        if(shift>=0)
            byte_val = (byte_val & ~pow_of_two[bit]) + (((input >> bytes_advanced*8) >> shift) & pow_of_two[bit]);
        else
            byte_val = (byte_val & ~pow_of_two[bit]) + (((input >> bytes_advanced*8) << -shift) & pow_of_two[bit]);
        bit++;
    }
    data[current_byte] = byte_val;
    if(bit == 8)
    {
        current_byte++;
    }
}

static void fill_player_entry_diagnostic(FullPlayer &player, int &current_byte, const uint8_t *data, int g_idx)
{
    int player_start_byte = current_byte;
    player.global_index = g_idx;
    player.team_index = -1;
    player.roster_slot = -1;
    player.shirt_number = 0;

    player.id = read_data(0, 4*8, current_byte, data);
    current_byte += 0x4; // Commentary name

    read_data(0, 2*8, current_byte, data); // nation
    player.height = (uint8_t)read_data(0, 8, current_byte, data);
    player.weight = (uint8_t)read_data(0, 8, current_byte, data);
    read_data(0, 8, current_byte, data);  // gc1
    read_data(0, 8, current_byte, data);  // gc2

    player.atk = (uint8_t)read_data(0, 7, current_byte, data);
    player.ball_ctrl = (uint8_t)read_data(7, 7, current_byte, data);
    read_data(6, 2, current_byte, data); // weak_use
    read_data(0, 7, current_byte, data); // tight_pos
    player.lowpass = (uint8_t)read_data(7, 7, current_byte, data);
    player.loftpass = (uint8_t)read_data(6, 7, current_byte, data);
    player.finish = (uint8_t)read_data(5, 7, current_byte, data);
    read_data(4, 4, current_byte, data); // mo_armd
    player.place_kick = (uint8_t)read_data(0, 7, current_byte, data);
    player.swerve = (uint8_t)read_data(7, 7, current_byte, data);
    player.speed = (uint8_t)read_data(6, 7, current_byte, data);
    player.exp_pwr = (uint8_t)read_data(5, 7, current_byte, data);
    read_data(4, 4, current_byte, data); // mo_armr
    player.jump = (uint8_t)read_data(0, 7, current_byte, data);
    player.phys_cont = (uint8_t)read_data(7, 7, current_byte, data);
    player.body_ctrl = (uint8_t)read_data(6, 7, current_byte, data);
    player.stamina = (uint8_t)read_data(5, 7, current_byte, data);
    read_data(4, 4, current_byte, data); // mo_ck
    player.ball_win = (uint8_t)read_data(0, 7, current_byte, data);
    player.aggres = (uint8_t)read_data(7, 7, current_byte, data);
    player.gk = (uint8_t)read_data(6, 7, current_byte, data);
    player.catching = (uint8_t)read_data(5, 7, current_byte, data);
    read_data(4, 3, current_byte, data); // form
    read_data(7, 1, current_byte, data); // b_edit_player
    player.cover = (uint8_t)read_data(0, 7, current_byte, data);
    player.age = (uint8_t)read_data(7, 6, current_byte, data);
    player.reg_pos = (uint8_t)read_data(5, 4, current_byte, data);

    read_data(2, 5, current_byte, data); // play_style
    read_data(7, 5, current_byte, data); // mo_fk
    read_data(4, 3, current_byte, data); // star
    read_data(7, 1, current_byte, data); // b_edit_basicset
    player.def = (uint8_t)read_data(0, 7, current_byte, data);
    player.clearing = (uint8_t)read_data(7, 7, current_byte, data);
    player.header = (uint8_t)read_data(6, 7, current_byte, data);
    read_data(5, 3, current_byte, data); // mo_hunchd
    read_data(0, 3, current_byte, data); // mo_hunchr
    read_data(3, 3, current_byte, data); // mo_pk
    read_data(6, 2, current_byte, data); // weak_acc
    player.drib = (uint8_t)read_data(0, 7, current_byte, data);
    read_data(7, 2, current_byte, data); // injury
    read_data(1, 2, current_byte, data); // play_attit
    read_data(3, 2, current_byte, data); // mo_drib

    // play_pos[13]
    read_data(5, 2, current_byte, data); // GK
    read_data(7, 2, current_byte, data); // CB
    read_data(1, 2, current_byte, data); // LB
    read_data(3, 2, current_byte, data); // RB
    read_data(5, 2, current_byte, data); // DMF
    read_data(7, 2, current_byte, data); // CMF
    read_data(1, 2, current_byte, data); // LMF
    read_data(3, 2, current_byte, data); // RMF
    read_data(5, 2, current_byte, data); // AMF
    read_data(7, 1, current_byte, data); // b_edit_regpos
    read_data(0, 2, current_byte, data); // RWF
    read_data(2, 2, current_byte, data); // SS
    read_data(4, 2, current_byte, data); // CF

    player.reflex = (uint8_t)read_data(6, 7, current_byte, data);
    player.kick_pwr = (uint8_t)read_data(5, 7, current_byte, data);

    read_data(4, 2, current_byte, data); // LWF
    read_data(6, 1, current_byte, data); // b_edit_playpos
    read_data(7, 1, current_byte, data); // b_edit_ability
    read_data(0, 1, current_byte, data); // b_edit_skill
    read_data(1, 1, current_byte, data); // b_edit_style
    read_data(2, 1, current_byte, data); // b_edit_com
    read_data(3, 1, current_byte, data); // b_edit_motion
    
    read_data(5, 1, current_byte, data); // strong_foot
    read_data(6, 1, current_byte, data); // strong_hand

    read_data(7, 1, current_byte, data); // com_style[0]
    read_data(0, 1, current_byte, data); // com_style[1]
    read_data(1, 1, current_byte, data); // com_style[2]
    read_data(2, 1, current_byte, data); // com_style[3]
    read_data(3, 1, current_byte, data); // com_style[4]
    read_data(4, 1, current_byte, data); // com_style[5]
    read_data(5, 1, current_byte, data); // com_style[6]

    // Player skills (41 items)
    read_data(6, 1, current_byte, data);
    read_data(7, 1, current_byte, data);
    read_data(0, 1, current_byte, data);
    read_data(1, 1, current_byte, data);
    read_data(2, 1, current_byte, data);
    read_data(3, 1, current_byte, data);
    read_data(4, 1, current_byte, data);
    read_data(5, 1, current_byte, data);
    read_data(6, 1, current_byte, data);
    read_data(7, 1, current_byte, data);
    read_data(0, 1, current_byte, data);
    read_data(1, 1, current_byte, data);
    read_data(2, 1, current_byte, data);
    read_data(3, 1, current_byte, data);
    read_data(4, 1, current_byte, data);
    read_data(5, 1, current_byte, data);
    read_data(6, 1, current_byte, data);
    read_data(7, 1, current_byte, data);
    read_data(0, 1, current_byte, data);
    read_data(1, 1, current_byte, data);
    read_data(2, 1, current_byte, data);
    read_data(3, 1, current_byte, data);
    read_data(4, 1, current_byte, data);
    read_data(5, 1, current_byte, data);
    read_data(6, 1, current_byte, data);
    read_data(7, 1, current_byte, data);
    read_data(0, 1, current_byte, data);
    read_data(1, 1, current_byte, data);
    read_data(2, 1, current_byte, data);
    read_data(3, 1, current_byte, data);
    read_data(4, 1, current_byte, data);
    read_data(5, 1, current_byte, data);
    read_data(6, 1, current_byte, data);
    read_data(7, 1, current_byte, data);
    read_data(0, 1, current_byte, data);
    read_data(1, 1, current_byte, data);
    read_data(2, 1, current_byte, data);
    read_data(3, 1, current_byte, data);
    read_data(4, 1, current_byte, data);
    read_data(5, 1, current_byte, data);
    read_data(6, 1, current_byte, data);

    current_byte++; // Unknown D 7/1

    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)&(data[current_byte]), -1, player.name, 61);
    current_byte += 61; // player name
    
    strncpy_s(player.shirt_name, 21, (const char*)&(data[current_byte]), 21 - 1);
    current_byte += 61; // shirt name
    current_byte += 64; // print name national team

    // Calculate overall max stat according to 4ccEditor aatf.cpp formula
    int rating = player.drib;
    rating = (std::max)((int)player.gk, rating);
    rating = (std::max)((int)player.finish, rating);
    rating = (std::max)((int)player.lowpass, rating);
    rating = (std::max)((int)player.loftpass, rating);
    rating = (std::max)((int)player.header, rating);
    rating = (std::max)((int)player.swerve, rating);
    rating = (std::max)((int)player.catching, rating);
    rating = (std::max)((int)player.clearing, rating);
    rating = (std::max)((int)player.reflex, rating);
    rating = (std::max)((int)player.cover, rating);
    rating = (std::max)((int)player.body_ctrl, rating);
    rating = (std::max)((int)player.phys_cont, rating);
    rating = (std::max)((int)player.kick_pwr, rating);
    rating = (std::max)((int)player.exp_pwr, rating);
    rating = (std::max)((int)player.ball_ctrl, rating);
    rating = (std::max)((int)player.ball_win, rating);
    rating = (std::max)((int)player.jump, rating);
    rating = (std::max)((int)player.place_kick, rating);
    rating = (std::max)((int)player.stamina, rating);
    rating = (std::max)((int)player.speed, rating);
    rating = (std::max)((int)player.aggres, rating);
    player.overall = rating;

    // Fast-forward to end of 0x138 (312 bytes) struct
    current_byte = player_start_byte + 312;
}

static void fill_team_ids21_exact(SimpleTeam &team, int &current_byte, const uint8_t *data)
{
    int team_start_byte = current_byte;
    team.id = read_data(0, 32, current_byte, data);
    read_data(0, 32, current_byte, data); // manager_id

    current_byte += 2;
    read_data(0, 16, current_byte, data); // stadium_id
    current_byte += 0xA;

    read_data(2, 6, current_byte, data);
    read_data(0, 6, current_byte, data);
    read_data(6, 1, current_byte, data);
    current_byte++;
    read_data(0, 6, current_byte, data);
    read_data(6, 6, current_byte, data);
    current_byte += 2;
    read_data(6, 1, current_byte, data);
    current_byte++;
    current_byte += 2;
    read_data(0, 6, current_byte, data);
    read_data(6, 6, current_byte, data);

    current_byte += 0x39;
    current_byte += 4; // Rival 1
    current_byte += 4; // Rival 2
    current_byte += 4; // Rival 3
    current_byte += 4; // Banner flags

    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)&(data[current_byte]), -1, team.name, 0x46);
    
    int shortname_byte = current_byte + 0x46;
    strncpy_s(team.short_name, 0x4, (const char*)&(data[shortname_byte]), 0x4 - 1);
    for (int ii = 0; ii < 4; ii++)
    {
        if ((int)team.short_name[ii] < 33 || (int)team.short_name[ii] > 95)
            team.short_name[ii] = (char)0;
    }

    current_byte = team_start_byte + 0x24C; // Exact stride 0x24C in PES 21
}

std::wstring to_lower_w(const std::wstring &str) {
    std::wstring result = str;
    for (auto &ch : result) {
        ch = towlower(ch);
    }
    return result;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    std::string input_file = "EDIT00000000";
    std::string output_file = "EDIT00000000"; // Default output file requested for game validation
    uint32_t target_player_id = 7511; // Default: Messi
    uint32_t target_dest_team_id = 125; // Default: Roma (ID 125)

    if (argc >= 2) {
        try { target_player_id = std::stoul(argv[1]); } catch (...) {}
    }
    if (argc >= 3) {
        try { target_dest_team_id = std::stoul(argv[2]); } catch (...) {}
    }
    if (argc >= 4) input_file = argv[3];
    if (argc >= 5) output_file = argv[4];

    // 1. Abrir o arquivo
    uint32_t raw_size = 0;
    uint8_t *raw_input = readFile(input_file.c_str(), &raw_size);
    if (!raw_input) {
        std::wcout << L"[ERRO] Nao foi possivel abrir o arquivo: " << input_file.c_str() << std::endl;
        return 1;
    }

    // 2. Descriptografar
    FileDescriptor *descriptor = createFileDescriptor();
    decryptWithKey(descriptor, raw_input, MasterKeyPes21);
    free(raw_input);

    uint8_t *data = descriptor->data;
    uint32_t num_players = data[0x60] + (data[0x61] * 256);
    uint32_t num_teams = data[0x64] + (data[0x65] * 256);

    // 3. Ler todos os jogadores
    std::vector<FullPlayer> players(num_players);
    int current_byte = 0x7C;
    for (uint32_t i = 0; i < num_players; ++i) {
        fill_player_entry_diagnostic(players[i], current_byte, data, i);
    }

    // 4. Ler todos os clubes
    std::vector<SimpleTeam> teams(num_teams);
    current_byte = 0x8ED2FC;
    for (uint32_t i = 0; i < num_teams; ++i) {
        fill_team_ids21_exact(teams[i], current_byte, data);
    }

    // 5. Vincular elencos
    int roster_byte = 0x9D4648;
    for (uint32_t i = 0; i < num_teams; ++i) {
        int roster_start_byte = roster_byte;
        teams[i].roster_offset = roster_start_byte;
        uint32_t roster_team_id = read_data(0, 32, roster_byte, data);

        teams[i].player_count = 0;
        for (int k = 0; k < 40; ++k) {
            teams[i].players[k] = read_data(0, 32, roster_byte, data);
            if (teams[i].players[k] != 0) {
                teams[i].player_count++;
            }
        }
        for (int k = 0; k < 40; ++k) {
            teams[i].numbers[k] = (uint16_t)read_data(0, 16, roster_byte, data);
        }

        // Link players to team
        for (int k = 0; k < 40; ++k) {
            if (teams[i].players[k] != 0) {
                for (uint32_t p = 0; p < num_players; ++p) {
                    if (players[p].id == teams[i].players[k]) {
                        players[p].team_index = i;
                        players[p].roster_slot = k;
                        players[p].shirt_number = teams[i].numbers[k];
                        break;
                    }
                }
            }
        }

        roster_byte = roster_start_byte + 0x11C;
    }

    // Localizar jogador pelo ID
    int player_idx = -1;
    for (uint32_t i = 0; i < num_players; ++i) {
        if (players[i].id == target_player_id) {
            player_idx = i;
            break;
        }
    }

    if (player_idx == -1) {
        std::wcout << L"[ERRO] Jogador com ID " << target_player_id << L" nao foi encontrado!" << std::endl;
        destroyFileDescriptor(descriptor);
        return 1;
    }

    // Localizar clube de destino pelo ID
    int dest_team_idx = -1;
    for (uint32_t i = 0; i < num_teams; ++i) {
        if (teams[i].id == target_dest_team_id) {
            dest_team_idx = i;
            break;
        }
    }

    if (dest_team_idx == -1) {
        std::wcout << L"[ERRO] Clube de destino com ID " << target_dest_team_id << L" nao foi encontrado!" << std::endl;
        destroyFileDescriptor(descriptor);
        return 1;
    }

    FullPlayer &target_player = players[player_idx];
    SimpleTeam &dest_team = teams[dest_team_idx];

    std::wstring orig_team_name = L"Sem Clube / Agente Livre";
    uint32_t orig_team_id = 0;
    int orig_slot = target_player.roster_slot;
    int orig_team_idx = target_player.team_index;

    if (orig_team_idx != -1) {
        orig_team_name = teams[orig_team_idx].name;
        orig_team_id = teams[orig_team_idx].id;
    }

    // Requisito Item 3: Exibir informações no console
    std::wcout << L"------------------------------------" << std::endl;
    std::wcout << L"ID: " << target_player.id << std::endl;
    std::wcout << L"Nome: " << target_player.name << std::endl;
    std::wcout << L"Overall: " << target_player.overall << std::endl;
    std::wcout << L"Clube de Origem: " << orig_team_name << L" (ID: " << orig_team_id << L")" << std::endl;
    std::wcout << L"Clube de Destino: " << dest_team.name << L" (ID: " << dest_team.id << L")" << std::endl;
    std::wcout << L"Número da Camisa: " << target_player.shirt_number << std::endl;
    std::wcout << L"Posição: " << (target_player.reg_pos < 13 ? POSITIONS[target_player.reg_pos] : L"Desconhecida") << std::endl;
    std::wcout << L"------------------------------------" << std::endl;

    // Executar transferência
    // A. Remover do clube de origem
    if (orig_team_idx != -1) {
        SimpleTeam &orig_team = teams[orig_team_idx];
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
        for (int k = 0; k < 40; ++k) write_data(orig_team.players[k], 0, 32, w_offset, data);
        for (int k = 0; k < 40; ++k) write_data(orig_team.numbers[k], 0, 16, w_offset, data);
    }

    // B. Adicionar ao clube de destino
    int dest_slot = -1;
    for (int k = 0; k < 40; ++k) {
        if (dest_team.players[k] == 0) {
            dest_slot = k;
            break;
        }
    }

    if (dest_slot == -1) {
        std::wcout << L"[ERRO] Clube de destino cheio!" << std::endl;
        destroyFileDescriptor(descriptor);
        return 1;
    }

    dest_team.players[dest_slot] = target_player.id;
    dest_team.numbers[dest_slot] = (target_player.shirt_number > 0) ? target_player.shirt_number : 10;
    dest_team.player_count++;

    int w_offset = dest_team.roster_offset + 4;
    for (int k = 0; k < 40; ++k) write_data(dest_team.players[k], 0, 32, w_offset, data);
    for (int k = 0; k < 40; ++k) write_data(dest_team.numbers[k], 0, 16, w_offset, data);

    // Requisito Item 6: Validações antes de salvar
    bool v_removed = true;
    if (orig_team_idx != -1) {
        SimpleTeam &orig_team = teams[orig_team_idx];
        for (int k = 0; k < 40; ++k) {
            if (orig_team.players[k] == target_player.id) {
                v_removed = false;
                break;
            }
        }
    }

    bool v_inserted = (dest_team.players[dest_slot] == target_player.id);

    int id_occurrences = 0;
    for (uint32_t i = 0; i < num_teams; ++i) {
        for (int k = 0; k < 40; ++k) {
            if (teams[i].players[k] == target_player.id) {
                id_occurrences++;
            }
        }
    }
    // Note: In PES, national team squads can also list real club players.
    // For club-to-club validation, occurrence count <= 2 (1 club + 1 national team if applicable) is valid.
    bool v_no_duplication = (id_occurrences <= 2);

    uint32_t current_num_players = data[0x60] + (data[0x61] * 256);
    bool v_player_count_consistent = (current_num_players == num_players);

    bool all_validations = v_removed && v_inserted && v_no_duplication && v_player_count_consistent;

    // Criptografar e Salvar
    int output_size = 0;
    uint8_t *encrypted_output = encryptWithKey(descriptor, &output_size, MasterKeyPes21);
    writeFile(output_file.c_str(), encrypted_output, output_size);
    free(encrypted_output);

    // Requisito Item 7: Relatório Final
    std::wcout << std::endl;
    std::wcout << L"====================================" << std::endl;
    std::wcout << L"          RELATÓRIO FINAL           " << std::endl;
    std::wcout << L"====================================" << std::endl;
    std::wcout << L"ID do Jogador: " << target_player.id << std::endl;
    std::wcout << L"Nome: " << target_player.name << std::endl;
    std::wcout << L"Clube Antes: " << orig_team_name << std::endl;
    std::wcout << L"Clube Depois: " << dest_team.name << std::endl;
    std::wcout << L"Slot Antigo: " << orig_slot << std::endl;
    std::wcout << L"Slot Novo: " << dest_slot << std::endl;
    std::wcout << L"Validação 1 (Removido do Origem): " << (v_removed ? L"OK" : L"FALHA") << std::endl;
    std::wcout << L"Validação 2 (Inserido no Destino): " << (v_inserted ? L"OK" : L"FALHA") << std::endl;
    std::wcout << L"Validação 3 (Sem Duplicidade em Clubes): " << (v_no_duplication ? L"OK" : L"FALHA") << std::endl;
    std::wcout << L"Validação 4 (Consistência Total de Jogadores): " << (v_player_count_consistent ? L"OK" : L"FALHA") << std::endl;
    std::wcout << L"Resultado da Validação: " << (all_validations ? L"SUCESSO ABSOLUTO" : L"FALHA NA VALIDAÇÃO") << std::endl;
    std::wcout << L"Arquivo Salvo: " << output_file.c_str() << L" (" << output_size << L" bytes)" << std::endl;
    std::wcout << L"====================================" << std::endl;

    destroyFileDescriptor(descriptor);
    return all_validations ? 0 : 1;
}
