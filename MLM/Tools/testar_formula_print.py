import json
import math
from pathlib import Path

POS_INDEX = {
    "GOL": 0,
    "ZC": 1,
    "LE": 2,
    "LD": 3,
    "VOL": 4,
    "MLG": 5,
    "MLE": 6,
    "MLD": 7,
    "MAT": 8,
    "PTE": 9,
    "PTD": 10,
    "SA": 11,
    "CA": 12,
}

# Pesos transcritos do print enviado. Cada lista tem 13 posições:
# GK, CB, LB, RB, DMF, CMF, LMF, RMF, AMF, LWF, RWF, SS, CF.
W = {
    "height": [186, 136, 49, 49, 61, 37, 12, 12, 37, 49, 49, 62, 99],
    "offensive_awareness": [0, 14, 61, 61, 61, 98, 98, 98, 171, 159, 159, 173, 210],
    "ball_control": [13, 27, 86, 86, 122, 171, 171, 171, 196, 159, 159, 210, 123],
    "dribbling": [0, 14, 61, 61, 37, 98, 110, 122, 122, 159, 159, 123, 62],
    "tight_possession": [0, 0, 37, 37, 24, 49, 73, 61, 73, 86, 86, 86, 37],
    "low_pass": [27, 41, 61, 61, 122, 208, 135, 135, 135, 196, 73, 73, 99, 37],
    "lofted_pass": [40, 68, 147, 147, 122, 159, 196, 196, 159, 98, 98, 74, 12],
    "finishing": [0, 27, 24, 24, 37, 73, 86, 86, 184, 159, 159, 284, 358],
    "set_piece_taking": [0, 14, 24, 24, 12, 12, 24, 24, 12, 12, 12, 12, 12],
    "curl": [0, 14, 24, 24, 12, 12, 24, 24, 12, 12, 12, 12, 12],
    "heading": [0, 55, 24, 24, 61, 24, 12, 24, 24, 24, 24, 25, 62],
    "defensive_awareness": [13, 286, 147, 147, 220, 86, 49, 49, 24, 12, 12, 12, 0],
    "tackling": [0, 191, 86, 86, 122, 86, 24, 24, 24, 12, 12, 12, 12],
    "aggression": [0, 82, 37, 37, 98, 37, 12, 12, 12, 12, 12, 12, 12],
    "kicking_power": [53, 27, 24, 24, 49, 73, 24, 24, 73, 61, 61, 99, 123],
    "speed": [13, 136, 220, 220, 61, 61, 196, 196, 98, 220, 220, 86, 99],
    "acceleration": [40, 150, 184, 184, 61, 86, 159, 159, 86, 159, 159, 99, 123],
    "physical_contact": [80, 204, 98, 98, 122, 49, 24, 24, 24, 37, 37, 37, 86],
    "balance": [0, 24, 24, 12, 24, 61, 61, 24, 73, 73, 74, 86, 37],
    "stamina": [133, 109, 37, 37, 37, 12, 12, 12, 12, 24, 24, 37, 62],
    "gk_awareness": [279, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    "gk_catching": [226, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    "gk_clearing": [226, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    "gk_reflexes": [173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    "gk_reach": [173, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
    "jump": [0, 68, 196, 196, 196, 196, 147, 147, 86, 49, 49, 49, 37],
    "weak_foot_usage": [4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4],
    "weak_foot_accuracy": [0, 14, 24, 24, 24, 24, 24, 24, 24, 24, 12, 12, 12],
}

MAP = {
    "height": "height",
    "offensive_awareness": "atk",
    "ball_control": "ball_ctrl",
    "dribbling": "drib",
    "tight_possession": "tight_pos",
    "low_pass": "lowpass",
    "lofted_pass": "loftpass",
    "finishing": "finish",
    "set_piece_taking": "place_kick",
    "curl": "swerve",
    "heading": "header",
    "defensive_awareness": "def",
    "tackling": "ball_win",
    "aggression": "aggres",
    "kicking_power": "kick_pwr",
    "speed": "speed",
    "acceleration": "exp_pwr",
    "physical_contact": "phys_cont",
    "balance": "body_ctrl",
    "stamina": "stamina",
    "gk_awareness": "gk",
    "gk_catching": "catching",
    "gk_clearing": "clearing",
    "gk_reflexes": "reflex",
    "gk_reach": "cover",
    "jump": "jump",
    "weak_foot_usage": "weak_use",
    "weak_foot_accuracy": "weak_acc",
}


def pes_round(x):
    return int(math.floor(x + 0.5))


def calc(player, pos, mode):
    idx = POS_INDEX[pos]
    stats = dict(player.get("stats", {}))
    stats["height"] = player.get("altura", 0)
    total = 0
    weight_sum = 0
    for source, dest in MAP.items():
        w = W[source][idx]
        if w == 0:
            continue
        v = stats.get(dest, 0) or 0
        if source == "height":
            v = v - 111
        if source in ("weak_foot_usage", "weak_foot_accuracy"):
            if mode == "weak_raw_no_minus25":
                total += v * w
            elif mode == "weak_minus1_no_minus25":
                total += (v - 1) * w
            else:
                total += (v - 25) * w
        else:
            total += (v - 25) * w
        weight_sum += w
    if mode in ("sum_div_100", "weak_raw_no_minus25", "weak_minus1_no_minus25"):
        return total / 100.0
    if mode == "sum_div_weight_times_100":
        return total / weight_sum * 100.0 if weight_sum else 40
    if mode == "sum_div_weight_plus25":
        return total / weight_sum + 25 if weight_sum else 40
    raise ValueError(mode)


def main():
    db = json.loads(Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\UI\pes_database.json").read_text(encoding="utf-8-sig"))
    targets = json.loads(Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\Tools\overall_targets.json").read_text(encoding="utf-8-sig"))
    by_key = {}
    by_id = {}
    for club in db["clubes"]:
        for p in club["jogadores"]:
            by_key[(int(club["id"]), int(p["id"]))] = p
            by_id.setdefault(int(p["id"]), p)

    modes = ["sum_div_100", "weak_raw_no_minus25", "weak_minus1_no_minus25", "sum_div_weight_times_100", "sum_div_weight_plus25"]
    for mode in modes:
        print("\nMODE", mode)
        ok = 0
        diffs = []
        for t in targets:
            pos = t["posicaoPes"]
            if pos not in POS_INDEX:
                continue
            p = by_key.get((int(t["clubId"]), int(t["playerId"]))) or by_id.get(int(t["playerId"]))
            val = calc(p, pos, mode)
            pred = pes_round(val)
            diff = pred - int(t["overallPesJogo"])
            diffs.append(abs(diff))
            if diff == 0:
                ok += 1
            print(f"{t['nome'][:20]:20} {pos:3} PES={t['overallPesJogo']:2} calc={val:7.2f} pred={pred:3} diff={diff:3}")
        print("OK", ok, "/", len(diffs), "MAE", sum(diffs)/len(diffs))

if __name__ == "__main__":
    main()
