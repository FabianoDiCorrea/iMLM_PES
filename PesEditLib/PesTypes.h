#ifndef PES_TYPES_H
#define PES_TYPES_H

#include <string>
#include <vector>
#include <cstdint>

enum class Position : uint8_t {
    GK = 0, CB = 1, LB = 2, RB = 3, DMF = 4, CMF = 5,
    LMF = 6, RMF = 7, AMF = 8, LWF = 9, RWF = 10, SS = 11, CF = 12,
    Unknown = 255
};

struct Player {
    uint32_t id = 0;
    std::wstring name = L"";
    std::string shirt_name = "";
    uint8_t age = 0;
    uint8_t height = 0;
    uint8_t weight = 0;
    Position reg_pos = Position::Unknown;
    
    // Key Stats
    uint8_t atk = 0;
    uint8_t def = 0;
    uint8_t gk = 0;
    uint8_t drib = 0;
    uint8_t finish = 0;
    uint8_t lowpass = 0;
    uint8_t loftpass = 0;
    uint8_t header = 0;
    uint8_t swerve = 0;
    uint8_t catching = 0;
    uint8_t clearing = 0;
    uint8_t reflex = 0;
    uint8_t cover = 0;
    uint8_t body_ctrl = 0;
    uint8_t phys_cont = 0;
    uint8_t kick_pwr = 0;
    uint8_t exp_pwr = 0;
    uint8_t ball_ctrl = 0;
    uint8_t ball_win = 0;
    uint8_t jump = 0;
    uint8_t place_kick = 0;
    uint8_t stamina = 0;
    uint8_t speed = 0;
    uint8_t aggres = 0;
    uint8_t tight_pos = 0;

    uint8_t weak_use = 0;
    uint8_t weak_acc = 0;
    uint8_t form = 0;
    uint8_t injury = 0;
    uint8_t play_attit = 0;
    uint8_t play_style = 0;
    uint8_t star = 0;
    uint8_t strong_foot = 0;
    uint8_t strong_hand = 0;
    uint8_t dribble_motion = 0;
    uint8_t playable_pos[13] = {0};
    uint8_t com_style[7] = {0};
    uint8_t play_skill[41] = {0};

    int overall = 0;
    int overall_by_position[13] = {0};
    int global_index = -1;
    int team_index = -1;
    int roster_slot = -1;
    uint16_t shirt_number = 0;
};

struct Team {
    uint32_t id = 0;
    std::wstring name = L"";
    std::string short_name = "";
    uint32_t players[40] = {0};
    uint16_t numbers[40] = {0};
    int roster_offset = 0;
    int player_count = 0;
    int global_index = -1;
};

struct TransferResult {
    bool success = false;
    uint32_t player_id = 0;
    std::wstring player_name = L"";
    std::wstring source_team_name = L"";
    std::wstring dest_team_name = L"";
    int old_slot = -1;
    int new_slot = -1;
    std::string message = "";
};

#endif // PES_TYPES_H
