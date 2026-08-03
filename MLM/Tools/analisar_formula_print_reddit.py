import json
import math
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

# Reusa a função calc() e estruturas do teste transcrito do print.
sys.path.insert(0, r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\Tools")
import testar_formula_print as fp


def load():
    db = json.loads(Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\UI\pes_database.json").read_text(encoding="utf-8-sig"))
    targets = json.loads(Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\Tools\overall_targets.json").read_text(encoding="utf-8-sig"))
    by_key, by_id = {}, {}
    for club in db["clubes"]:
        for p in club["jogadores"]:
            by_key[(int(club["id"]), int(p["id"]))] = p
            by_id.setdefault(int(p["id"]), p)
    rows = []
    for target in targets:
        player = by_key.get((int(target["clubId"]), int(target["playerId"]))) or by_id.get(int(target["playerId"]))
        if not player:
            continue
        rows.append({"target": target, "player": player, "base": fp.calc(player, target["posicaoPes"], "sum_div_weight_plus25"), "pes": int(target["overallPesJogo"])})
    return rows


def round_pes(x):
    return int(math.floor(x + 0.5))


def fit_affine(group_rows):
    n = len(group_rows)
    xs = [r["base"] for r in group_rows]
    ys = [r["pes"] for r in group_rows]
    mx, my = sum(xs)/n, sum(ys)/n
    var = sum((x-mx)**2 for x in xs)
    if var == 0:
        a = 1.0
    else:
        a = sum((x-mx)*(y-my) for x,y in zip(xs,ys))/var
    b = my - a*mx
    return a, b


def main():
    rows = load()
    groups = defaultdict(list)
    for row in rows:
        groups[row["target"]["posicaoPes"]].append(row)

    out = []
    out.append("# Teste da fórmula parcial do print Reddit")
    out.append("")
    out.append("A base usada é `sum(peso * (atributo - 25)) / soma_pesos + 25`, que foi a única normalização plausível com o print parcial.")
    out.append("Depois foi testado um ajuste linear por posição apenas para medir se o esqueleto de pesos está perto dos alvos GOGOSZ.")
    out.append("")
    for pos, group_rows in sorted(groups.items()):
        a, b = fit_affine(group_rows)
        ok_base = 0
        ok_fit = 0
        base_abs = []
        fit_abs = []
        details = []
        for row in group_rows:
            base_pred = round_pes(row["base"])
            fit_value = a * row["base"] + b
            fit_pred = round_pes(fit_value)
            base_diff = base_pred - row["pes"]
            fit_diff = fit_pred - row["pes"]
            ok_base += base_diff == 0
            ok_fit += fit_diff == 0
            base_abs.append(abs(base_diff))
            fit_abs.append(abs(fit_diff))
            details.append((row["target"].get("nome"), row["pes"], row["base"], base_pred, fit_value, fit_pred, base_diff, fit_diff))
        out.append(f"## {pos}")
        out.append("")
        out.append(f"- Amostras: {len(group_rows)}")
        out.append(f"- Base print: {ok_base}/{len(group_rows)} OK, MAE {sum(base_abs)/len(base_abs):.2f}")
        out.append(f"- Base print + ajuste linear local: {ok_fit}/{len(group_rows)} OK, MAE {sum(fit_abs)/len(fit_abs):.2f}")
        out.append(f"- Ajuste local: `overall = {a:.4f} * base + {b:.4f}`")
        out.append("")
        out.append("| Jogador | PES | Base | Pred base | Pred ajustado | Erro base | Erro ajustado |")
        out.append("|---|---:|---:|---:|---:|---:|---:|")
        for name, pes, base, base_pred, fit_value, fit_pred, base_diff, fit_diff in details:
            out.append(f"| {name} | {pes} | {base:.2f} | {base_pred} | {fit_pred} | {base_diff} | {fit_diff} |")
        out.append("")
    Path(r"M:\Projetos DevDuo\PES-Edit-Research-Codex\MLM\Data\OVERALL_FORMULA_PRINT_REDDIT_TESTE.md").write_text("\n".join(out), encoding="utf-8")
    print("relatorio gerado", len(rows), "amostras")

if __name__ == "__main__":
    main()
