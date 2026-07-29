#ifndef BINARY_IO_H
#define BINARY_IO_H

#include <cstdint>
#include "PesTypes.h"

namespace BinaryIO {
    int read_data(int start_bit, int bits_to_read, int &current_byte, const uint8_t *data);
    void write_data(int input, int start_bit, int bits_to_write, int &current_byte, uint8_t *data);
    
    void parse_player_entry(Player &player, int &current_byte, const uint8_t *data, int g_idx);
    void write_player_entry(const Player &player, int &current_byte, uint8_t *data);
    void parse_team_entry(Team &team, int &current_byte, const uint8_t *data, int g_idx);
}

#endif // BINARY_IO_H
