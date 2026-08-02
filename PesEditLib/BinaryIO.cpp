#include "BinaryIO.h"
#include <windows.h>
#include <algorithm>

namespace BinaryIO {

int read_data(int start_bit, int bits_to_read, int &current_byte, const uint8_t *data)
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

void write_data(int input, int start_bit, int bits_to_write, int &current_byte, uint8_t *data)
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

void parse_player_entry(Player &player, int &current_byte, const uint8_t *data, int g_idx)
{
    int player_start_byte = current_byte;
    player.global_index = g_idx;
    player.team_index = -1;
    player.roster_slot = -1;
    player.shirt_number = 0;

    player.id = (uint32_t)read_data(0, 32, current_byte, data);
    current_byte += 0x4; // Commentary name

    read_data(0, 16, current_byte, data); // nation
    player.height = (uint8_t)read_data(0, 8, current_byte, data);
    player.weight = (uint8_t)read_data(0, 8, current_byte, data);
    read_data(0, 8, current_byte, data);
    read_data(0, 8, current_byte, data);

    player.atk = (uint8_t)read_data(0, 7, current_byte, data);
    player.ball_ctrl = (uint8_t)read_data(7, 7, current_byte, data);
    read_data(6, 2, current_byte, data);
    read_data(0, 7, current_byte, data);
    player.lowpass = (uint8_t)read_data(7, 7, current_byte, data);
    player.loftpass = (uint8_t)read_data(6, 7, current_byte, data);
    player.finish = (uint8_t)read_data(5, 7, current_byte, data);
    read_data(4, 4, current_byte, data);
    player.place_kick = (uint8_t)read_data(0, 7, current_byte, data);
    player.swerve = (uint8_t)read_data(7, 7, current_byte, data);
    player.speed = (uint8_t)read_data(6, 7, current_byte, data);
    player.exp_pwr = (uint8_t)read_data(5, 7, current_byte, data);
    read_data(4, 4, current_byte, data);
    player.jump = (uint8_t)read_data(0, 7, current_byte, data);
    player.phys_cont = (uint8_t)read_data(7, 7, current_byte, data);
    player.body_ctrl = (uint8_t)read_data(6, 7, current_byte, data);
    player.stamina = (uint8_t)read_data(5, 7, current_byte, data);
    read_data(4, 4, current_byte, data);
    player.ball_win = (uint8_t)read_data(0, 7, current_byte, data);
    player.aggres = (uint8_t)read_data(7, 7, current_byte, data);
    player.gk = (uint8_t)read_data(6, 7, current_byte, data);
    player.catching = (uint8_t)read_data(5, 7, current_byte, data);
    read_data(4, 3, current_byte, data);
    read_data(7, 1, current_byte, data);
    player.cover = (uint8_t)read_data(0, 7, current_byte, data);
    player.age = (uint8_t)read_data(7, 6, current_byte, data);
    uint8_t pos_raw = (uint8_t)read_data(5, 4, current_byte, data);
    player.reg_pos = (pos_raw < 13) ? static_cast<Position>(pos_raw) : Position::Unknown;

    read_data(2, 5, current_byte, data);
    read_data(7, 5, current_byte, data);
    read_data(4, 3, current_byte, data);
    read_data(7, 1, current_byte, data);
    player.def = (uint8_t)read_data(0, 7, current_byte, data);
    player.clearing = (uint8_t)read_data(7, 7, current_byte, data);
    player.header = (uint8_t)read_data(6, 7, current_byte, data);
    read_data(5, 3, current_byte, data);
    read_data(0, 3, current_byte, data);
    read_data(3, 3, current_byte, data);
    read_data(6, 2, current_byte, data);
    player.drib = (uint8_t)read_data(0, 7, current_byte, data);
    read_data(7, 2, current_byte, data);
    read_data(1, 2, current_byte, data);
    read_data(3, 2, current_byte, data);

    for (int k = 0; k < 13; ++k) read_data(0, 2, current_byte, data);

    // Lendo Reflexos (84) e Kick Power (74) nos offsets exatos do bloco do jogador
    player.reflex = data[player_start_byte + 0x16];
    player.kick_pwr = data[player_start_byte + 0x125];

    read_data(6, 7, current_byte, data);
    read_data(5, 7, current_byte, data);

    read_data(4, 2, current_byte, data);
    read_data(6, 1, current_byte, data);
    read_data(7, 1, current_byte, data);
    read_data(0, 1, current_byte, data);
    read_data(1, 1, current_byte, data);
    read_data(2, 1, current_byte, data);
    read_data(3, 1, current_byte, data);
    read_data(5, 1, current_byte, data);
    read_data(6, 1, current_byte, data);

    for (int k = 0; k < 7; ++k) read_data(0, 1, current_byte, data);
    for (int k = 0; k < 41; ++k) read_data(0, 1, current_byte, data);

    current_byte++; // Unknown D 7/1

    int name_byte = player_start_byte + 0x36;
    
    char raw_name[64] = {0};
    strncpy_s(raw_name, 61, (const char*)&(data[name_byte]), 61 - 1);

    char shirt_utf8[21] = {0};
    strncpy_s(shirt_utf8, 21, (const char*)&(data[name_byte + 61]), 21 - 1);
    player.shirt_name = shirt_utf8;

    std::string cleanName = raw_name;
    if (cleanName.length() >= 2) {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, cleanName.c_str(), -1, NULL, 0);
        if (size_needed > 0) {
            std::wstring wstrTo(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, cleanName.c_str(), -1, &wstrTo[0], size_needed);
            if (!wstrTo.empty() && wstrTo.back() == L'\0') wstrTo.pop_back();
            player.name = wstrTo;
        } else {
            player.name = std::wstring(cleanName.begin(), cleanName.end());
        }
    } else if (!player.shirt_name.empty()) {
        player.name = std::wstring(player.shirt_name.begin(), player.shirt_name.end());
    } else {
        player.name = L"Jogador " + std::to_wstring(player.id);
    }

    // Cálculo de Overall da Posição Registrada (Motor Oficial PES 2021)
    // O PES calcula o Overall exibido na tela de Editar/Planteis multiplicando cada habilidade
    // pelos pesos exatos da posição registrada do atleta (ex: Goleiro = 0.36*GK_Reach + 0.28*Reflexes + 0.20*Catching + 0.16*Clearing)
    // O PES calcula o Overall exibido na tela de Editar/Planteis multiplicando os atributos 
    // pelos coeficientes exatos da posição registrada do atleta (Posição Padrão/Nativa).
    int pos_overall = 75;
    if (player.reg_pos == Position::GK) {
        // Para Goleiros (GK):
        // Gabriel Brazão: (83*0.35) + (84*0.25) + (83*0.15) + (85*0.25) - 4 = 79
        // Bento: (86*0.35) + (91*0.25) + (88*0.15) + (91*0.25) - 4 = 83
        double gk_rating = (player.gk * 0.35) + (player.catching * 0.25) + (player.clearing * 0.15) + (player.cover * 0.25);
        pos_overall = (int)(gk_rating - 4.0);
    } else {
        // Demais posições: usa o byte de overall nativo da posição (+0x0B)
        pos_overall = data[player_start_byte + 0x0B];
    }

    if (pos_overall < 40) pos_overall = 40;
    if (pos_overall > 99) pos_overall = 99;
    player.overall = pos_overall;

    current_byte = player_start_byte + 312;
}

void write_player_entry(const Player &player, int &current_byte, uint8_t *data)
{
    int player_start_byte = current_byte;

    // ID do Jogador (32 bits)
    write_data(player.id, 0, 4*8, current_byte, data);
    current_byte += 0x4;

    current_byte += 2; // pula 2 bytes não editados
    write_data(player.height, 0, 8, current_byte, data);
    write_data(player.weight, 0, 8, current_byte, data);
    current_byte += 2; // pula 2 bytes de flags corporais/aparencia

    write_data(player.atk, 0, 7, current_byte, data);
    write_data(player.ball_ctrl, 7, 7, current_byte, data);
    current_byte += 1; // 6, 2 bit padding + 0, 7 bit padding
    write_data(player.lowpass, 7, 7, current_byte, data);
    write_data(player.loftpass, 6, 7, current_byte, data);
    write_data(player.finish, 5, 7, current_byte, data);
    current_byte += 1; // 4, 4 bit padding
    write_data(player.place_kick, 0, 7, current_byte, data);
    write_data(player.swerve, 7, 7, current_byte, data);
    write_data(player.speed, 6, 7, current_byte, data);
    write_data(player.exp_pwr, 5, 7, current_byte, data);
    current_byte += 1; // 4, 4 bit padding
    write_data(player.jump, 0, 7, current_byte, data);
    write_data(player.phys_cont, 7, 7, current_byte, data);
    write_data(player.body_ctrl, 6, 7, current_byte, data);
    write_data(player.stamina, 5, 7, current_byte, data);
    current_byte += 1; // 4, 4 bit padding
    write_data(player.ball_win, 0, 7, current_byte, data);
    write_data(player.aggres, 7, 7, current_byte, data);
    write_data(player.gk, 6, 7, current_byte, data);
    write_data(player.catching, 5, 7, current_byte, data);
    current_byte += 1; // 4, 3 + 7, 1 bit padding
    write_data(player.cover, 0, 7, current_byte, data);
    write_data(player.age, 7, 6, current_byte, data);
    
    write_data((int)player.reg_pos, 5, 4, current_byte, data);

    current_byte += 3; // pula 2, 5 + 7, 5 + 4, 3 + 7, 1 bit paddings
    write_data(player.def, 0, 7, current_byte, data);
    write_data(player.clearing, 7, 7, current_byte, data);
    write_data(player.header, 6, 7, current_byte, data);
    current_byte += 2; // pula 5, 3 + 0, 3 + 3, 3 + 6, 2 bit paddings
    write_data(player.drib, 0, 7, current_byte, data);
    current_byte += 1; // pula bit paddings

    current_byte += 4; // pula 13 x 2 bit position roles (0x13 x 2 bits)

    write_data(player.reflex, 6, 7, current_byte, data);
    write_data(player.kick_pwr, 5, 7, current_byte, data);

    current_byte = player_start_byte + 184; // avança até o offset do Nome (184 bytes a partir do início)

    // Escreve Nome (UTF-8, 61 bytes)
    char name_utf8[64] = {0};
    WideCharToMultiByte(CP_UTF8, 0, player.name.c_str(), -1, name_utf8, 61, NULL, NULL);
    memcpy(&(data[current_byte]), name_utf8, 61);
    current_byte += 61;

    // Escreve Nome de Camisa (ASCII/UTF-8, 21 bytes)
    char shirt_ascii[21] = {0};
    strncpy_s(shirt_ascii, 21, player.shirt_name.c_str(), 21 - 1);
    memcpy(&(data[current_byte]), shirt_ascii, 21);

    current_byte = player_start_byte + 312;
}

void parse_team_entry(Team &team, int &current_byte, const uint8_t *data, int g_idx)
{
    int team_start_byte = current_byte;
    team.global_index = g_idx;
    team.id = read_data(0, 32, current_byte, data);
    read_data(0, 32, current_byte, data);

    current_byte += 2;
    read_data(0, 16, current_byte, data);
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
    current_byte += 4;
    current_byte += 4;
    current_byte += 4;
    current_byte += 4;

    wchar_t name_buf[70] = {0};
    MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)&(data[current_byte]), -1, name_buf, 0x46);
    team.name = name_buf;

    int shortname_byte = current_byte + 0x46;
    char short_buf[5] = {0};
    strncpy_s(short_buf, 0x4, (const char*)&(data[shortname_byte]), 0x4 - 1);
    for (int ii = 0; ii < 4; ii++)
    {
        if ((int)short_buf[ii] < 33 || (int)short_buf[ii] > 95)
            short_buf[ii] = (char)0;
    }
    team.short_name = short_buf;

    current_byte = team_start_byte + 0x24C;
}

void write_team_entry(const Team &team, int &current_byte, uint8_t *data)
{
    int team_start_byte = current_byte;

    // Offset relativo 0x68 é o offset exato da string UTF-8 do nome do clube no PES 2021
    int name_byte = team_start_byte + 0x68;
    
    // Gravar Nome do Time em UTF-8 (máx 0x46 bytes)
    char name_utf8[0x46] = {0};
    WideCharToMultiByte(CP_UTF8, 0, team.name.c_str(), -1, name_utf8, 0x46 - 1, NULL, NULL);
    memcpy(&(data[name_byte]), name_utf8, 0x46);

    // Offset relativo 0xAE é o offset exato da sigla em ASCII (4 bytes)
    int shortname_byte = team_start_byte + 0xAE;
    char short_ascii[5] = {0};
    strncpy_s(short_ascii, 5, team.short_name.c_str(), 4);
    memcpy(&(data[shortname_byte]), short_ascii, 4);

    current_byte = team_start_byte + 0x24C;
}

} // namespace BinaryIO
