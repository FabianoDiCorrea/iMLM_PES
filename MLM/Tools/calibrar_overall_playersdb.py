import argparse
import json
import math
from collections import defaultdict
from pathlib import Path

import numpy as np


FEATURES = [
    "height", "atk", "ball_ctrl", "drib", "tight_pos", "lowpass", "loftpass",
    "finish", "header", "place_kick", "swerve", "def", "ball_win", "aggres",
    "speed", "exp_pwr", "kick_pwr", "jump", "phys_cont", "body_ctrl", "stamina",
    "gk", "catching", "clearing", "reflex", "cover", "weak_acc_scaled",
]

POSITION_MAP = {
    "GK": "GOL", "CB": "ZC", "LB": "LE", "RB": "LD", "DMF": "VOL",
    "CMF": "MLG", "LMF": "MLE", "RMF": "MLD", "AMF": "MAT",
    "LWF": "PTE", "RWF": "PTD", "SS": "SA", "CF": "CA",
}


def round_pes(value):
    return max(40, min(99, int(math.floor(value + 0.5))))


def feature_value(row, feature):
    if feature == "weak_acc_scaled":
        weak_acc = float(row.get("weak_acc", 1) or 1)
        return math.floor(59 * (weak_acc - 1) / 3 + 40)
    return float(row.get(feature, 0) or 0)


def matrix(rows):
    return np.array([[feature_value(row, feature) for feature in FEATURES] for row in rows], dtype=float)


def fit_ridge(rows, alpha):
    x = matrix(rows)
    mean = x.mean(axis=0)
    std = x.std(axis=0)
    std[std == 0] = 1.0
    scaled = (x - mean) / std
    design = np.column_stack([np.ones(len(rows)), scaled])
    y = np.array([row["overall"] for row in rows], dtype=float)
    regularizer = np.eye(design.shape[1]) * alpha
    regularizer[0, 0] = 0
    coefficients = np.linalg.solve(design.T @ design + regularizer, design.T @ y)
    raw_weights = coefficients[1:] / std
    intercept = coefficients[0] - float(mean @ raw_weights)
    return intercept, raw_weights


def predict(row, model):
    value = model[0] + sum(feature_value(row, feature) * weight for feature, weight in zip(FEATURES, model[1]))
    return round_pes(value)


def metrics(rows, predictions):
    errors = [prediction - int(row["overall"]) for row, prediction in zip(rows, predictions)]
    absolute = [abs(error) for error in errors]
    return {
        "count": len(rows),
        "exact": sum(error == 0 for error in errors),
        "within1": sum(error <= 1 for error in absolute),
        "mae": round(sum(absolute) / len(absolute), 4) if absolute else 0,
        "maxError": max(absolute, default=0),
    }


def cross_validate(rows, alpha, folds=5):
    predictions = []
    expected = []
    for fold in range(folds):
        train = [row for row in rows if int(row["id"]) % folds != fold]
        test = [row for row in rows if int(row["id"]) % folds == fold]
        if not train or not test:
            continue
        model = fit_ridge(train, alpha)
        predictions.extend(predict(row, model) for row in test)
        expected.extend(test)
    return metrics(expected, predictions)


def select_models(players):
    grouped = defaultdict(list)
    for player in players:
        grouped[player["position"]].append(player)
    models = {}
    reports = {}
    alphas = [0.01, 0.1, 0.5, 1, 2, 5, 10, 20, 50, 100, 200]
    for position, rows in sorted(grouped.items()):
        candidates = []
        for alpha in alphas:
            validation = cross_validate(rows, alpha)
            candidates.append((validation["mae"], validation["maxError"], -validation["exact"], alpha, validation))
        _, _, _, alpha, validation = min(candidates)
        model = fit_ridge(rows, alpha)
        train_predictions = [predict(row, model) for row in rows]
        models[position] = model
        reports[position] = {
            "samples": len(rows),
            "alpha": alpha,
            "crossValidation": validation,
            "train": metrics(rows, train_predictions),
        }
    return models, reports


def load_patch_rows(database_path, targets_path):
    database = json.loads(database_path.read_text(encoding="utf-8-sig"))
    targets = json.loads(targets_path.read_text(encoding="utf-8-sig"))
    by_key = {}
    by_id = {}
    for club in database["clubes"]:
        for player in club["jogadores"]:
            by_key[(int(club["id"]), int(player["id"]))] = player
            by_id.setdefault(int(player["id"]), player)
    rows = []
    reverse_positions = {value: key for key, value in POSITION_MAP.items()}
    for target in targets:
        player = by_key.get((int(target["clubId"]), int(target["playerId"]))) or by_id.get(int(target["playerId"]))
        position = reverse_positions.get(target["posicaoPes"])
        if player is None or position is None:
            continue
        stats = dict(player.get("stats", {}))
        row = dict(stats)
        row.update({
            "id": int(player["id"]),
            "name": target.get("nome", player.get("nome", "")),
            "position": position,
            "height": player.get("altura", 0),
            "weak_acc": player.get("piorPePrecisao", stats.get("weak_acc", 1)),
            "overall": int(target["overallPesJogo"]),
            "imlm": int(player.get("overall", 0)),
        })
        rows.append(row)
    return rows


def export_model(models):
    result = {"version": "playersdb-pes21-ridge-v1", "features": FEATURES, "positions": {}}
    for position, (intercept, weights) in sorted(models.items()):
        result["positions"][position] = {
            "intercept": float(intercept),
            "weights": {feature: float(weight) for feature, weight in zip(FEATURES, weights)},
        }
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--samples", required=True)
    parser.add_argument("--database", required=True)
    parser.add_argument("--targets", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--report", required=True)
    args = parser.parse_args()

    players = json.loads(Path(args.samples).read_text(encoding="utf-8-sig"))
    models, public_report = select_models(players)
    patch_rows = load_patch_rows(Path(args.database), Path(args.targets))
    patch_predictions = [predict(row, models[row["position"]]) for row in patch_rows]
    patch_details = []
    for row, prediction in zip(patch_rows, patch_predictions):
        patch_details.append({
            "id": row["id"], "name": row["name"], "position": row["position"],
            "pes": row["overall"], "calculated": prediction,
            "error": prediction - row["overall"], "imlmCurrent": row["imlm"],
        })
    report = {
        "publicSamples": len(players),
        "publicByPosition": public_report,
        "patch": metrics(patch_rows, patch_predictions),
        "patchDetails": patch_details,
    }
    Path(args.output).write_text(json.dumps(export_model(models), ensure_ascii=False, indent=2), encoding="utf-8")
    Path(args.report).write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")
    print(json.dumps({
        "publicSamples": len(players),
        "publicCrossValidation": {position: data["crossValidation"] for position, data in public_report.items()},
        "patch": report["patch"],
    }, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
