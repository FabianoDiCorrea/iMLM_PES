import json
import math
from pathlib import Path

# Arrays do print Reddit + return completo:
# k = Math.floor((k + 500) / 1000)
W = {
    "height": [186,136,49,49,61,37,12,12,37,49,49,62,99],
    "atk": [0,14,61,61,61,98,98,98,171,159,159,173,210],
    "ball_ctrl": [13,27,86,86,122,171,171,171,196,159,159,210,123],
    "drib": [0,14,61,61,37,98,110,122,122,159,159,123,62],
    "tight_pos": [0,0,37,37,24,49,73,61,73,86,86,86,37],
    "lowpass": [27,41,61,61,122,208,135,135,135,196,73,73,99,37],
    "loftpass": [40,68,147,147,122,159,196,196,159,98,98,74,12],
    "finish": [0,27,24,24,37,73,86,86,184,159,159,284,358],
    "place_kick": [0,14,24,24,12,12,24,24,12,12,12,12,12],
    "swerve": [0,14,24,24,12,12,24,24,12,12,12,12,12],
    "header": [0,55,24,24,61,24,12,24,24,24,24,25,62],
    "def": [13,286,147,147,220,86,49,49,24,12,12,12,0],
    "ball_win": [0,191,86,86,122,86,24,24,24,12,12,12,12],
    "aggres": [0,82,37,37,98,37,12,12,12,12,12,12,12],
    "kick_pwr": [53,27,24,24,49,73,24,24,73,61,61,99,123],
    "speed": [13,136,220,220,61,61,196,196,98,220,220,86,99],
    "exp_pwr": [40,150,184,184,61,86,159,159,86,159,159,99,123],
    "phys_cont": [80,204,98,98,122,49,24,24,24,37,37,37,86],
    "body_ctrl": [0,24,24,12,24,61,61,24,73,73,74,86,37],
    "jump": [133,109,37,37,37,12,12,12,12,24,24,37,62],
    "stamina": [0,68,196,196,196,196,147,147,86,49,49,49,37],
    "gk": [279,0,0,0,0,0,0,0,0,0,0,0,0],
    "catching": [226,0,0,0,0,0,0,0,0,0,0,0,0],
    "cover": [226,0,0,0,0,0,0,0,0,0,0,0,0],
    "reflex": [173,0,0,0,0,0,0,0,0,0,0,0,0],
    "clearing": [173,0,0,0,0,0,0,0,0,0,0,0,0],
    "weak_acc_scaled": [4,4,4,4,4,4,4,4,4,4,4,4,4],
    "def_engagement": [0,14,24,24,24,24,24,24,24,24,24,12,12],
}

MAPPINGS = {
    "pes21_current_order": {"GOL":0,"ZC":1,"LE":2,"LD":3,"VOL":4,"MLG":5,"MLE":6,"MLD":7,"MAT":8,"PTE":9,"PTD":10,"SA":11,"CA":12},
    "reddit_caption_guess": {"GOL":0,"ZC":1,"LD":2,"LE":3,"VOL":4,"MLG":5,"MLD":6,"MLE":7,"MAT":8,"PTD":9,"PTE":10,"SA":11,"CA":12},
}


def calc(player, pos, mapping, def_engagement_value=25):
    t = mapping[pos]
    stats = player["stats"]
    k = 0
    k += (player["altura"] - 111 - 25) * W["height"][t]
    for key in ["atk","ball_ctrl","drib","tight_pos","lowpass","loftpass","finish","place_kick","swerve","header","def","ball_win","aggres","kick_pwr","speed","exp_pwr","phys_cont","body_ctrl","jump","stamina","gk","catching","cover","reflex","clearing"]:
        k += (stats.get(key, 0) - 25) * W[key][t]
    weak_scaled = math.floor(59 * stats.get("weak_acc", 1) / 3 + 40)
    k += (weak_scaled - 25) * W["weak_acc_scaled"][t]
    k += (def_engagement_value - 25) * W["def_engagement"][t]
    return math.floor((k + 500) / 1000)


def main():
    db = json.loads(Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\UI\pes_database.json").read_text(encoding="utf-8-sig"))
    targets = json.loads(Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\Tools\overall_targets.json").read_text(encoding="utf-8-sig"))
    by_key, by_id = {}, {}
    for club in db["clubes"]:
        for p in club["jogadores"]:
            by_key[(int(club["id"]), int(p["id"]))] = p
            by_id.setdefault(int(p["id"]), p)
    rows = []
    for target in targets:
        p = by_key.get((int(target["clubId"]), int(target["playerId"]))) or by_id.get(int(target["playerId"]))
        if p:
            rows.append((target,p))

    lines = []
    lines.append("# Teste da fórmula Reddit com return completo")
    lines.append("")
    for mapping_name, mapping in MAPPINGS.items():
        for de in [25, 40, 50, 60, 70]:
            details=[]; ok=0; abs_errors=[]
            for target,p in rows:
                pred = calc(p, target["posicaoPes"], mapping, de)
                diff = pred - int(target["overallPesJogo"])
                ok += diff == 0
                abs_errors.append(abs(diff))
                details.append((target.get("nome"), target["posicaoPes"], target["overallPesJogo"], p["overall"], pred, diff))
            lines.append(f"## {mapping_name} / defensive_engagement={de}")
            lines.append("")
            lines.append(f"- OK: {ok}/{len(rows)}")
            lines.append(f"- MAE: {sum(abs_errors)/len(abs_errors):.2f}")
            lines.append(f"- Max erro: {max(abs_errors)}")
            lines.append("")
            lines.append("| Jogador | Pos | PES | Atual | Reddit | Erro |")
            lines.append("|---|---:|---:|---:|---:|---:|")
            for d in details:
                lines.append(f"| {d[0]} | {d[1]} | {d[2]} | {d[3]} | {d[4]} | {d[5]} |")
            lines.append("")
    out = Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\Data\OVERALL_REDDIT_FULL_FORMULA_TEST.md")
    out.write_text("\n".join(lines), encoding="utf-8")
    print("gerado", out)

if __name__ == "__main__":
    main()
