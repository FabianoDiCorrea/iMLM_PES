import argparse
import csv
import json
import math
from collections import defaultdict
from pathlib import Path

import numpy as np

from calibrar_overall_playersdb import load_patch_rows


CSV_FEATURES = [
    "height", "offensive_awareness", "ball_control", "dribbling", "tight_possession",
    "low_pass", "lofted_pass", "finishing", "heading", "place_kicking", "curl",
    "speed", "acceleration", "kicking_power", "jump", "physical_contact", "balance",
    "stamina", "defensive_awareness", "ball_winning", "aggression", "gk_awareness",
    "gk_catching", "gk_clearing", "gk_reflexes", "gk_reach",
]

PATCH_FEATURES = {
    "height": "height", "offensive_awareness": "atk", "ball_control": "ball_ctrl",
    "dribbling": "drib", "tight_possession": "tight_pos", "low_pass": "lowpass",
    "lofted_pass": "loftpass", "finishing": "finish", "heading": "header",
    "place_kicking": "place_kick", "curl": "swerve", "speed": "speed",
    "acceleration": "exp_pwr", "kicking_power": "kick_pwr", "jump": "jump",
    "physical_contact": "phys_cont", "balance": "body_ctrl", "stamina": "stamina",
    "defensive_awareness": "def", "ball_winning": "ball_win", "aggression": "aggres",
    "gk_awareness": "gk", "gk_catching": "catching", "gk_clearing": "clearing",
    "gk_reflexes": "reflex", "gk_reach": "cover",
}

SYMMETRIC_GROUPS = {
    "LB": "FB", "RB": "FB", "LMF": "SMF", "RMF": "SMF", "LWF": "WF", "RWF": "WF",
}


def position_group(position):
    return SYMMETRIC_GROUPS.get(position, position)


def csv_matrix(rows):
    return np.array([[float(row[feature]) for feature in CSV_FEATURES] for row in rows], dtype=float)


def fit_linear(rows, alpha):
    x = csv_matrix(rows)
    y = np.array([float(row["overall_rating"]) for row in rows], dtype=float)
    mean = x.mean(axis=0)
    std = x.std(axis=0)
    std[std == 0] = 1.0
    design = np.column_stack([np.ones(len(rows)), (x - mean) / std])
    regularizer = np.eye(design.shape[1]) * alpha
    regularizer[0, 0] = 0.0
    coefficients = np.linalg.solve(
        design.T @ design + regularizer,
        design.T @ y,
    )
    linear_scores = design @ coefficients
    curve = np.polynomial.Chebyshev.fit(linear_scores, y - linear_scores, 2)
    return mean, std, coefficients, curve


def raw_csv_score(rows, model):
    mean, std, coefficients, _ = model
    design = np.column_stack([np.ones(len(rows)), (csv_matrix(rows) - mean) / std])
    return design @ coefficients


def round_pes(values):
    return np.floor(values + 0.5).astype(int)


def predict_csv(rows, model):
    raw = raw_csv_score(rows, model)
    return round_pes(raw + model[3](raw))


def predict_patch(row, model):
    mean, std, coefficients, curve = model
    values = np.array([
        float(row.get(PATCH_FEATURES[feature], 0) or 0)
        for feature in CSV_FEATURES
    ])
    raw = float(np.r_[1.0, (values - mean) / std] @ coefficients)
    return int(math.floor(raw + float(curve(raw)) + 0.5))


def metrics(expected, predicted):
    errors = [int(prediction) - int(value) for value, prediction in zip(expected, predicted)]
    absolute = [abs(error) for error in errors]
    return {
        "count": len(errors),
        "exact": sum(error == 0 for error in errors),
        "within1": sum(error <= 1 for error in absolute),
        "mae": round(sum(absolute) / len(absolute), 4) if absolute else 0.0,
        "maxError": max(absolute, default=0),
    }


def select_alpha(rows):
    candidates = []
    for alpha in [0.01, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0]:
        all_expected = []
        all_predictions = []
        for fold in range(5):
            train = [row for row in rows if int(row["_row_index"]) % 5 != fold]
            test = [row for row in rows if int(row["_row_index"]) % 5 == fold]
            model = fit_linear(train, alpha)
            all_expected.extend(int(row["overall_rating"]) for row in test)
            all_predictions.extend(predict_csv(test, model).tolist())
        validation = metrics(all_expected, all_predictions)
        candidates.append((validation["mae"], validation["maxError"], -validation["exact"], alpha, validation))
    _, _, _, alpha, validation = min(candidates)
    return alpha, validation


def export_model(models, alphas):
    output = {
        "version": "pes2021-csv-linear-curve2-v1",
        "warning": "Modelo de pesquisa aproximado; nao substitui formula exata do executavel.",
        "features": CSV_FEATURES,
        "symmetricGroups": SYMMETRIC_GROUPS,
        "positions": {},
    }
    for group, (mean, std, coefficients, curve) in sorted(models.items()):
        output["positions"][group] = {
            "alpha": alphas[group],
            "mean": mean.tolist(),
            "std": std.tolist(),
            "linearCoefficients": coefficients.tolist(),
            "curveDomain": list(curve.domain),
            "curveWindow": list(curve.window),
            "curveCoefficients": curve.coef.tolist(),
        }
    return output


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--csv", required=True)
    parser.add_argument("--database", required=True)
    parser.add_argument("--targets", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--report", required=True)
    args = parser.parse_args()

    with Path(args.csv).open(encoding="utf-8-sig", newline="") as source:
        rows = list(csv.DictReader(source))
    groups = defaultdict(list)
    for index, row in enumerate(rows):
        row["_row_index"] = index
        groups[position_group(row["registered_position"])].append(row)

    models = {}
    alphas = {}
    public_report = {}
    for group, group_rows in sorted(groups.items()):
        alpha, validation = select_alpha(group_rows)
        models[group] = fit_linear(group_rows, alpha)
        alphas[group] = alpha
        public_report[group] = {
            "samples": len(group_rows),
            "alpha": alpha,
            "crossValidation": validation,
        }

    patch_rows = load_patch_rows(Path(args.database), Path(args.targets))
    patch_predictions = [
        predict_patch(row, models[position_group(row["position"])])
        for row in patch_rows
    ]
    patch_expected = [row["overall"] for row in patch_rows]
    patch_details = [
        {
            "id": row["id"], "name": row["name"], "position": row["position"],
            "pes": row["overall"], "calculated": prediction,
            "error": prediction - row["overall"], "imlmCurrent": row["imlm"],
        }
        for row, prediction in zip(patch_rows, patch_predictions)
    ]
    report = {
        "sourceRows": len(rows),
        "publicByPosition": public_report,
        "patch": metrics(patch_expected, patch_predictions),
        "patchDetails": patch_details,
    }
    Path(args.output).write_text(json.dumps(export_model(models, alphas), ensure_ascii=False, indent=2), encoding="utf-8")
    Path(args.report).write_text(json.dumps(report, ensure_ascii=False, indent=2), encoding="utf-8")
    print(json.dumps({"sourceRows": len(rows), "patch": report["patch"]}, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
