import argparse
import json
import math
from collections import defaultdict
from pathlib import Path

import numpy as np


POSITION_GROUPS = {
    "GOL": "GOL",
    "ZC": "ZC",
    "LE": "LAT",
    "LD": "LAT",
    "VOL": "MEIO",
    "MLG": "MEIO",
    "MLE": "MEIO_LADO",
    "MLD": "MEIO_LADO",
    "MAT": "MEIA_OF",
    "PTE": "PONTA",
    "PTD": "PONTA",
    "SA": "ATACANTE",
    "CA": "CA",
}

FEATURES_BY_GROUP = {
    "GOL": [
        "gk", "catching", "clearing", "reflex", "cover",
        "jump", "phys_cont", "body_ctrl", "height", "weight", "form", "injury",
    ],
    "ZC": [
        "def", "ball_win", "aggres", "header", "jump", "phys_cont", "body_ctrl",
        "speed", "exp_pwr", "stamina", "height", "weight", "form", "injury",
        "ball_ctrl", "lowpass", "loftpass",
    ],
    "LAT": [
        "speed", "exp_pwr", "stamina", "def", "ball_win", "aggres",
        "ball_ctrl", "drib", "tight_pos", "lowpass", "loftpass", "atk",
        "phys_cont", "body_ctrl", "form", "injury", "weak_use", "weak_acc",
    ],
    "MEIO": [
        "lowpass", "loftpass", "ball_ctrl", "tight_pos", "drib", "stamina",
        "def", "ball_win", "aggres", "atk", "finish", "speed", "exp_pwr",
        "kick_pwr", "phys_cont", "body_ctrl", "form", "injury", "weak_use", "weak_acc",
    ],
    "MEIO_LADO": [
        "speed", "exp_pwr", "stamina", "drib", "ball_ctrl", "tight_pos",
        "lowpass", "loftpass", "atk", "def", "ball_win", "body_ctrl", "form", "injury",
    ],
    "MEIA_OF": [
        "atk", "ball_ctrl", "tight_pos", "drib", "lowpass", "loftpass", "finish",
        "place_kick", "swerve", "speed", "exp_pwr", "kick_pwr", "body_ctrl", "stamina",
        "form", "weak_use", "weak_acc",
    ],
    "PONTA": [
        "atk", "ball_ctrl", "tight_pos", "drib", "speed", "exp_pwr", "finish",
        "lowpass", "loftpass", "swerve", "stamina", "body_ctrl", "form", "weak_use", "weak_acc",
    ],
    "ATACANTE": [
        "atk", "finish", "ball_ctrl", "tight_pos", "drib", "lowpass", "speed",
        "exp_pwr", "kick_pwr", "header", "jump", "phys_cont", "body_ctrl", "stamina",
        "form", "weak_use", "weak_acc",
    ],
    "CA": [
        "atk", "finish", "header", "ball_ctrl", "tight_pos", "drib", "lowpass",
        "speed", "exp_pwr", "kick_pwr", "jump", "phys_cont", "body_ctrl", "stamina",
        "aggres", "height", "weight", "form", "injury", "weak_use", "weak_acc",
    ],
}

MANUAL_BASELINE_FEATURES = {
    "GOL": ["gk", "catching", "clearing", "reflex", "cover"],
    "ZC": ["def", "ball_win", "phys_cont", "header", "jump", "aggres", "body_ctrl"],
    "LAT": ["speed", "exp_pwr", "stamina", "def", "ball_win", "loftpass", "ball_ctrl", "drib", "atk"],
    "MEIO": ["lowpass", "loftpass", "ball_ctrl", "tight_pos", "drib", "stamina", "def", "ball_win", "phys_cont", "body_ctrl"],
    "CA": ["atk", "finish", "ball_ctrl", "tight_pos", "speed", "exp_pwr", "header", "jump", "kick_pwr", "phys_cont", "body_ctrl", "stamina"],
}


def round_pes(value: float) -> int:
    return max(40, min(99, int(math.floor(value + 0.5))))


def load_players(database_path: Path):
    db = json.loads(database_path.read_text(encoding="utf-8-sig"))
    by_club_player = {}
    by_player = defaultdict(list)
    for club in db.get("clubes", []):
        for player in club.get("jogadores", []):
            key = (int(club["id"]), int(player["id"]))
            by_club_player[key] = player
            by_player[int(player["id"])].append((club, player))
    return by_club_player, by_player


def flatten_player(player):
    stats = dict(player.get("stats", {}))
    flat = {
        "age": player.get("idade", 0),
        "height": player.get("altura", 0),
        "weight": player.get("peso", 0),
        "form": player.get("formaFisica", stats.get("form", 0)),
        "injury": player.get("resistenciaLesao", stats.get("injury", 0)),
        "weak_use": player.get("piorPeUso", stats.get("weak_use", 0)),
        "weak_acc": player.get("piorPePrecisao", stats.get("weak_acc", 0)),
        "style": player.get("estiloJogo", 0),
        "star": player.get("estrela", 0),
    }
    flat.update(stats)
    return flat


def build_dataset(database_path, targets_path):
    by_club_player, by_player = load_players(database_path)
    targets = json.loads(targets_path.read_text(encoding="utf-8-sig"))
    rows = []
    missing = []
    for target in targets:
        club_id = int(target["clubId"])
        player_id = int(target["playerId"])
        player = by_club_player.get((club_id, player_id))
        if player is None and player_id in by_player:
            player = by_player[player_id][0][1]
        if player is None:
            missing.append(target)
            continue
        row = {
            "target": target,
            "player": player,
            "flat": flatten_player(player),
            "pes": int(target["overallPesJogo"]),
            "imlm": int(player["overall"]),
            "pos": target["posicaoPes"],
            "group": POSITION_GROUPS.get(target["posicaoPes"], target["posicaoPes"]),
        }
        rows.append(row)
    return rows, missing


def standardize_matrix(rows, feature_names):
    x = np.array([[float(row["flat"].get(name, 0) or 0) for name in feature_names] for row in rows], dtype=float)
    mean = x.mean(axis=0)
    std = x.std(axis=0)
    std[std == 0] = 1.0
    return (x - mean) / std, mean, std


def fit_ridge(rows, feature_names, alpha):
    x_scaled, mean, std = standardize_matrix(rows, feature_names)
    x_design = np.column_stack([np.ones(len(rows)), x_scaled])
    y = np.array([float(row["pes"]) for row in rows], dtype=float)
    reg = np.eye(x_design.shape[1]) * alpha
    reg[0, 0] = 0.0
    coef = np.linalg.solve(x_design.T @ x_design + reg, x_design.T @ y)
    pred = x_design @ coef
    rounded = [round_pes(v) for v in pred]
    return {
        "coef": coef,
        "mean": mean,
        "std": std,
        "pred": pred,
        "rounded": rounded,
    }


def evaluate_predictions(rows, rounded):
    diffs = [int(pred) - int(row["pes"]) for pred, row in zip(rounded, rows)]
    abs_diffs = [abs(diff) for diff in diffs]
    return {
        "count": len(rows),
        "ok": sum(1 for diff in diffs if diff == 0),
        "max_abs_error": max(abs_diffs) if abs_diffs else 0,
        "mae": sum(abs_diffs) / len(abs_diffs) if abs_diffs else 0,
        "diffs": diffs,
    }


def leave_one_out(rows, feature_names, alpha):
    if len(rows) < 4:
        return None
    rounded = []
    for idx in range(len(rows)):
        train = [row for pos, row in enumerate(rows) if pos != idx]
        test = [rows[idx]]
        model = fit_ridge(train, feature_names, alpha)
        values = []
        for name, value in zip(feature_names, [float(test[0]["flat"].get(name, 0) or 0) for name in feature_names]):
            values.append((value - model["mean"][len(values)]) / model["std"][len(values)])
        pred = float(np.array([1.0] + values) @ model["coef"])
        rounded.append(round_pes(pred))
    return evaluate_predictions(rows, rounded)


def top_coefficients(model, feature_names, limit=10):
    pairs = []
    for name, coef, std in zip(feature_names, model["coef"][1:], model["std"]):
        raw_weight = coef / std
        pairs.append({"feature": name, "scaledCoef": float(coef), "rawWeightApprox": float(raw_weight)})
    return sorted(pairs, key=lambda item: abs(item["scaledCoef"]), reverse=True)[:limit]


def calibrate(rows):
    by_group = defaultdict(list)
    for row in rows:
        by_group[row["group"]].append(row)

    report = {}
    for group, group_rows in sorted(by_group.items()):
        features = FEATURES_BY_GROUP.get(group, FEATURES_BY_GROUP.get(row["pos"], []))
        if not features:
            continue
        usable_features = [name for name in features if any((row["flat"].get(name, 0) or 0) != 0 for row in group_rows)]
        alpha_candidates = [0.01, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 25.0]
        best = None
        for alpha in alpha_candidates:
            effective_features = usable_features
            if len(group_rows) <= 3:
                effective_features = [name for name in MANUAL_BASELINE_FEATURES.get(group, usable_features) if name in usable_features]
            try:
                model = fit_ridge(group_rows, effective_features, alpha)
            except np.linalg.LinAlgError:
                continue
            metrics = evaluate_predictions(group_rows, model["rounded"])
            loo = leave_one_out(group_rows, effective_features, alpha)
            sort_key = (
                loo["mae"] if loo else 999,
                metrics["mae"],
                metrics["max_abs_error"],
                alpha,
            )
            if best is None or sort_key < best["sortKey"]:
                best = {
                    "sortKey": sort_key,
                    "alpha": alpha,
                    "features": effective_features,
                    "model": model,
                    "metrics": metrics,
                    "loo": loo,
                }
        if best is None:
            continue
        details = []
        for row, pred, diff in zip(group_rows, best["model"]["rounded"], best["metrics"]["diffs"]):
            details.append({
                "playerId": row["target"]["playerId"],
                "clubId": row["target"]["clubId"],
                "nome": row["target"].get("nome", row["player"].get("nome")),
                "posicaoPes": row["pos"],
                "pes": row["pes"],
                "imlmAtual": row["imlm"],
                "calibrado": int(pred),
                "diffCalibrado": int(diff),
                "diffAtual": int(row["imlm"] - row["pes"]),
            })
        report[group] = {
            "samples": len(group_rows),
            "alpha": best["alpha"],
            "features": best["features"],
            "trainMetrics": {key: value for key, value in best["metrics"].items() if key != "diffs"},
            "leaveOneOutMetrics": None if best["loo"] is None else {key: value for key, value in best["loo"].items() if key != "diffs"},
            "interceptScaled": float(best["model"]["coef"][0]),
            "topCoefficients": top_coefficients(best["model"], best["features"]),
            "details": details,
        }
    return report


def write_markdown(report, rows, output_path):
    total = len(rows)
    current_ok = sum(1 for row in rows if row["imlm"] == row["pes"])
    current_mae = sum(abs(row["imlm"] - row["pes"]) for row in rows) / total if total else 0
    calibrated_details = [detail for group in report.values() for detail in group["details"]]
    calibrated_ok = sum(1 for detail in calibrated_details if detail["diffCalibrado"] == 0)
    calibrated_mae = sum(abs(detail["diffCalibrado"]) for detail in calibrated_details) / len(calibrated_details) if calibrated_details else 0

    lines = []
    lines.append("# Relatório de calibração de overall PES 2021 / GOGOSZ")
    lines.append("")
    lines.append("## Resumo")
    lines.append("")
    lines.append(f"- Amostras reais: {total}")
    lines.append(f"- Fórmula atual: {current_ok}/{total} OK, MAE {current_mae:.2f}")
    lines.append(f"- Modelo calibrado em treino: {calibrated_ok}/{len(calibrated_details)} OK, MAE {calibrated_mae:.2f}")
    lines.append("")
    lines.append("> Observação: modelo calibrado em treino não é prova de fórmula oficial. A prova vem do erro em novas amostras e validação leave-one-out por posição.")
    lines.append("")

    for group, data in report.items():
        lines.append(f"## {group}")
        lines.append("")
        lines.append(f"- Amostras: {data['samples']}")
        lines.append(f"- Treino: {data['trainMetrics']['ok']}/{data['trainMetrics']['count']} OK, MAE {data['trainMetrics']['mae']:.2f}, max {data['trainMetrics']['max_abs_error']}")
        if data["leaveOneOutMetrics"]:
            loo = data["leaveOneOutMetrics"]
            lines.append(f"- Leave-one-out: {loo['ok']}/{loo['count']} OK, MAE {loo['mae']:.2f}, max {loo['max_abs_error']}")
        else:
            lines.append("- Leave-one-out: amostras insuficientes")
        lines.append("- Principais sinais encontrados:")
        for coef in data["topCoefficients"][:8]:
            sign = "+" if coef["scaledCoef"] >= 0 else "-"
            lines.append(f"  - {sign} `{coef['feature']}` ({coef['scaledCoef']:.2f})")
        lines.append("")
        lines.append("| Jogador | PES | Atual | Calibrado | Erro atual | Erro calibrado |")
        lines.append("|---|---:|---:|---:|---:|---:|")
        for detail in data["details"]:
            lines.append(f"| {detail['nome']} | {detail['pes']} | {detail['imlmAtual']} | {detail['calibrado']} | {detail['diffAtual']} | {detail['diffCalibrado']} |")
        lines.append("")

    output_path.write_text("\n".join(lines), encoding="utf-8")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--database", required=True)
    parser.add_argument("--targets", required=True)
    parser.add_argument("--json-output", required=True)
    parser.add_argument("--md-output", required=True)
    args = parser.parse_args()

    rows, missing = build_dataset(Path(args.database), Path(args.targets))
    report = calibrate(rows)
    output = {
        "summary": {
            "samples": len(rows),
            "missing": missing,
            "currentOk": sum(1 for row in rows if row["imlm"] == row["pes"]),
            "currentMae": sum(abs(row["imlm"] - row["pes"]) for row in rows) / len(rows) if rows else 0,
        },
        "groups": report,
    }
    Path(args.json_output).write_text(json.dumps(output, ensure_ascii=False, indent=2), encoding="utf-8")
    write_markdown(report, rows, Path(args.md_output))
    print(json.dumps(output["summary"], ensure_ascii=False))


if __name__ == "__main__":
    main()

