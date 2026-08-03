import json
import math
from collections import defaultdict
from pathlib import Path

COLUMNS = ["CA", "SA", "PTE", "PTD", "MAT", "MLE", "MLD", "MLG", "LE", "LD", "VOL", "ZC", "GOL"]

# Coeficientes lidos da imagem do post Mimo. Contexto provável: PES Mobile/eFootball.
COEF = {
    "atk":              [0.21, 0.18, 0.17, 0.17, 0.18, 0.095, 0.095, 0.096, 0.055, 0.055, 0.072, 0.01, 0.0],
    "ball_ctrl":        [0.11, 0.22, 0.17, 0.17, 0.20, 0.16, 0.16, 0.18, 0.08, 0.08, 0.14, 0.025, 0.0],
    "drib":             [0.062,0.13, 0.16, 0.16, 0.12, 0.12, 0.12, 0.096,0.069,0.069,0.043,0.019, 0.0],
    "tight_pos":        [0.041,0.084,0.091,0.091,0.076,0.076,0.076,0.052,0.034,0.034,0.011,0.0,   0.004],
    "lowpass":          [0.035,0.088,0.067,0.067,0.20, 0.14, 0.14, 0.21, 0.078,0.078,0.11, 0.043,0.025],
    "loftpass":         [0.013,0.084,0.11, 0.11, 0.15, 0.20, 0.20, 0.16, 0.14, 0.14, 0.12, 0.067,0.044],
    "finish":           [0.36, 0.28, 0.16, 0.16, 0.18, 0.093,0.093,0.077,0.023,0.023,0.027,0.028,0.005],
    "header":           [0.067,0.014,0.021,0.021,0.028,0.005,0.005,0.023,0.03, 0.03, 0.056,0.049,0.004],
    "place_kick":       [0.012,0.028,0.01, 0.01, 0.015,0.018,0.018,0.009,0.024,0.024,0.008,0.016,0.008],
    "swerve":           [0.016,0.0,  0.013,0.013,0.001,0.025,0.025,0.01, 0.034,0.034,0.019,0.009,0.001],
    "def":              [0.0,  0.0,  0.016,0.016,0.036,0.057,0.057,0.079,0.14, 0.14, 0.22, 0.29, 0.007],
    "ball_win":         [0.016,0.017,0.006,0.006,0.007,0.012,0.012,0.091,0.088,0.088,0.13, 0.20, 0.0],
    "aggres":           [0.013,0.006,0.013,0.013,0.008,0.015,0.015,0.04, 0.032,0.032,0.11, 0.078,0.0],
    # Defensive engagement não existe no PES21; aproximação: 0 por padrão para não contaminar.
    "def_engagement":   [0.008,0.016,0.026,0.026,0.022,0.02, 0.02, 0.027,0.028,0.028,0.02, 0.021,0.0],
    "speed":            [0.098,0.07, 0.23, 0.23, 0.096,0.19, 0.19, 0.057,0.23, 0.23, 0.048,0.14, 0.013],
    "exp_pwr":          [0.13, 0.11, 0.16, 0.16, 0.096,0.16, 0.16, 0.089,0.18, 0.18, 0.072,0.14, 0.039],
    "kick_pwr":         [0.12, 0.092,0.059,0.059,0.084,0.032,0.032,0.073,0.016,0.016,0.056,0.033,0.051],
    "jump":             [0.063,0.042,0.029,0.029,0.015,0.022,0.022,0.01, 0.039,0.039,0.035,0.11, 0.14],
    "phys_cont":        [0.085,0.034,0.036,0.036,0.021,0.019,0.019,0.052,0.10, 0.10, 0.12, 0.20, 0.078],
    "body_ctrl":        [0.088,0.075,0.07, 0.07, 0.023,0.056,0.056,0.017,0.033,0.033,0.008,0.04, 0.0],
    "stamina":          [0.043,0.057,0.044,0.044,0.099,0.15, 0.15, 0.19, 0.20, 0.20, 0.20, 0.064,0.002],
    "gk":               [0,0,0,0,0,0,0,0,0,0,0,0,0.29],
    "catching":         [0,0,0,0,0,0,0,0,0,0,0,0,0.23],
    "clearing":         [0,0,0,0,0,0,0,0,0,0,0,0,0.18],
    "reflex":           [0,0,0,0,0,0,0,0,0,0,0,0,0.16],
    "cover":            [0,0,0,0,0,0,0,0,0,0,0,0,0.23],
    "height_s":         [0.10, 0.078,0.05, 0.05, 0.038,0.014,0.014,0.031,0.05, 0.05, 0.068,0.14, 0.19],
}


def round_pes(value):
    return int(math.floor(value + 0.5))


def height_scaled(height):
    # Hipóteses possíveis: o post usa Height_s; mantemos a mais simples e calibramos intercepto.
    return height


def score_no_intercept(player, pos):
    idx = COLUMNS.index(pos)
    stats = dict(player.get("stats", {}))
    total = 0.0
    for key, values in COEF.items():
        coef = values[idx]
        if coef == 0:
            continue
        if key == "height_s":
            value = height_scaled(player.get("altura", 0))
        elif key == "def_engagement":
            value = 0
        else:
            value = stats.get(key, 0) or 0
        total += coef * value
    return total


def main():
    db = json.loads(Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\UI\pes_database.json").read_text(encoding="utf-8-sig"))
    targets = json.loads(Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\Tools\overall_targets.json").read_text(encoding="utf-8-sig"))
    by_key = {}
    by_id = {}
    for club in db["clubes"]:
        for player in club["jogadores"]:
            by_key[(int(club["id"]), int(player["id"]))] = player
            by_id.setdefault(int(player["id"]), player)

    rows = []
    for target in targets:
        pos = target["posicaoPes"]
        if pos not in COLUMNS:
            continue
        player = by_key.get((int(target["clubId"]), int(target["playerId"]))) or by_id.get(int(target["playerId"]))
        if not player:
            continue
        raw = score_no_intercept(player, pos)
        rows.append({"target": target, "player": player, "pos": pos, "pes": int(target["overallPesJogo"]), "raw": raw, "current": int(player["overall"])})

    groups = defaultdict(list)
    for row in rows:
        groups[row["pos"]].append(row)

    lines = []
    lines.append("# Teste dos coeficientes Mimo contra GOGOSZ")
    lines.append("")
    lines.append("Coeficientes transcritos da imagem. Como o post não inclui interceptos, foi estimado um intercepto por posição usando a média dos alvos disponíveis.")
    lines.append("`Defensive Engagement` foi tratado como zero porque não existe no parser PES21 atual.")
    lines.append("")

    total_ok_current = sum(1 for r in rows if r["current"] == r["pes"])
    total_mae_current = sum(abs(r["current"] - r["pes"]) for r in rows) / len(rows)
    all_pred = []

    for pos, group in sorted(groups.items()):
        intercept = sum(r["pes"] - r["raw"] for r in group) / len(group)
        ok = 0
        abs_errors = []
        lines.append(f"## {pos}")
        lines.append("")
        lines.append(f"- Amostras: {len(group)}")
        lines.append(f"- Intercepto estimado: {intercept:.4f}")
        lines.append("")
        lines.append("| Jogador | PES | Atual | Mimo bruto | Mimo pred | Erro Mimo |")
        lines.append("|---|---:|---:|---:|---:|---:|")
        for r in group:
            pred_value = r["raw"] + intercept
            pred = round_pes(pred_value)
            diff = pred - r["pes"]
            ok += diff == 0
            abs_errors.append(abs(diff))
            all_pred.append((pred, r["pes"]))
            lines.append(f"| {r['target'].get('nome')} | {r['pes']} | {r['current']} | {r['raw']:.2f} | {pred} | {diff} |")
        lines.append("")
        lines.append(f"- Resultado Mimo + intercepto local: {ok}/{len(group)} OK, MAE {sum(abs_errors)/len(abs_errors):.2f}")
        lines.append("")

    total_ok_mimo = sum(1 for pred, pes in all_pred if pred == pes)
    total_mae_mimo = sum(abs(pred - pes) for pred, pes in all_pred) / len(all_pred)
    lines.insert(2, f"- Fórmula atual iMLM: {total_ok_current}/{len(rows)} OK, MAE {total_mae_current:.2f}")
    lines.insert(3, f"- Coeficientes Mimo + intercepto por posição: {total_ok_mimo}/{len(rows)} OK, MAE {total_mae_mimo:.2f}")
    Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\Data\OVERALL_MIMO_COEFFICIENTS_TEST.md").write_text("\n".join(lines), encoding="utf-8")
    print(json.dumps({"samples": len(rows), "currentOk": total_ok_current, "currentMae": total_mae_current, "mimoOk": total_ok_mimo, "mimoMae": total_mae_mimo}, ensure_ascii=False))

if __name__ == "__main__":
    main()
