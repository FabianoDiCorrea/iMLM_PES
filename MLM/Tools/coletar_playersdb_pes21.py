import argparse
import html
import json
import random
import re
import time
import urllib.request
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path


PLAYER_URL = "https://playersdb.app/player?id={}"

POSITION_CODES = {
    0: "GK",
    1: "CB",
    2: "LB",
    3: "RB",
    4: "DMF",
    5: "CMF",
    6: "LMF",
    7: "RMF",
    8: "AMF",
    9: "LWF",
    10: "RWF",
    11: "SS",
    12: "CF",
}

STAT_LABELS = {
    "Offensive Awareness": "atk",
    "Ball Control": "ball_ctrl",
    "Dribbling": "drib",
    "Tight Possession": "tight_pos",
    "Low Pass": "lowpass",
    "Lofted Pass": "loftpass",
    "Finishing": "finish",
    "Heading": "header",
    "Place Kicking": "place_kick",
    "Curl": "swerve",
    "Speed": "speed",
    "Acceleration": "exp_pwr",
    "Kicking Power": "kick_pwr",
    "Jump": "jump",
    "Physical Contact": "phys_cont",
    "Balance": "body_ctrl",
    "Stamina": "stamina",
    "Defensive Awareness": "def",
    "Ball Winning": "ball_win",
    "Aggression": "aggres",
    "GK Awareness": "gk",
    "GK Catching": "catching",
    "GK Clearing": "clearing",
    "GK Reflexes": "reflex",
    "GK Reach": "cover",
    "Weak Foot Usage": "weak_use",
    "Weak Foot Accuracy": "weak_acc",
    "Form": "form",
    "Injury Resistance": "injury",
}


def load_candidates(dataset_path: Path):
    lines = dataset_path.read_text(encoding="utf-8-sig").splitlines()
    header = json.loads(lines[0])
    index = {name: position for position, name in enumerate(header)}
    candidates = []
    for line in lines[1:]:
        row = json.loads(line)
        versions = row[index["game_versions"]] or []
        reg_position = row[index["registeredPosition"]]
        if "PES 2021" not in versions or reg_position not in POSITION_CODES:
            continue
        candidates.append({
            "id": int(row[index["konamiID"]]),
            "name": (row[index["playerName"]] or [""])[0],
            "position": POSITION_CODES[reg_position],
            "height": row[index["height"]],
            "weight": row[index["weight"]],
            "age": row[index["age"]],
        })
    return candidates


def fetch_markup(player_id: int):
    request = urllib.request.Request(
        PLAYER_URL.format(player_id),
        headers={
            "User-Agent": "Mozilla/5.0 iMLM-PES research",
            "Accept-Language": "en-US,en;q=0.9",
        },
    )
    with urllib.request.urlopen(request, timeout=30) as response:
        return response.read().decode("utf-8", errors="replace")


def parse_pes21(candidate, markup):
    start = markup.find('id="stats-pes21"')
    if start < 0:
        return None
    section = markup[start:start + 70000]
    player = dict(candidate)
    for label, key in STAT_LABELS.items():
        pattern = rf"title=['\"]{re.escape(label)}['\"].*?class=['\"]stat-badge[^'\"]*['\"][^>]*>\s*(\d+)\s*</span>"
        match = re.search(pattern, section, flags=re.I | re.S)
        if not match:
            return None
        player[key] = int(match.group(1))
    overall_match = re.search(
        r"Overall Rating</h6>.*?font-weight-bold[^>]*>\s*(\d+)\s*</div>",
        section,
        flags=re.I | re.S,
    )
    if not overall_match:
        return None
    player["overall"] = int(overall_match.group(1))
    player["name"] = html.unescape(player["name"])
    return player


def fetch_player(candidate, delay, retries):
    error = None
    for attempt in range(retries + 1):
        try:
            player = parse_pes21(candidate, fetch_markup(candidate["id"]))
            return player, None
        except Exception as current_error:
            error = str(current_error)
            time.sleep(min(8.0, 0.75 * (2 ** attempt)))
    if delay:
        time.sleep(delay)
    return None, error


def balanced_candidates(candidates, limit, seed):
    rng = random.Random(seed)
    by_position = defaultdict(list)
    for candidate in candidates:
        by_position[candidate["position"]].append(candidate)
    selected = []
    per_position = max(1, limit // len(POSITION_CODES))
    for position in POSITION_CODES.values():
        pool = by_position[position]
        rng.shuffle(pool)
        selected.extend(pool[:per_position])
    if len(selected) < limit:
        selected_ids = {candidate["id"] for candidate in selected}
        remainder = [candidate for candidate in candidates if candidate["id"] not in selected_ids]
        rng.shuffle(remainder)
        selected.extend(remainder[:limit - len(selected)])
    rng.shuffle(selected)
    return selected[:limit]


def write_players(output_path, players):
    ordered = sorted(players, key=lambda player: (player["position"], player["id"]))
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(ordered, ensure_ascii=False, indent=2), encoding="utf-8")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--dataset", required=True)
    parser.add_argument("--output", required=True)
    parser.add_argument("--limit", type=int, default=650)
    parser.add_argument("--workers", type=int, default=6)
    parser.add_argument("--delay", type=float, default=0.03)
    parser.add_argument("--retries", type=int, default=2)
    parser.add_argument("--seed", type=int, default=20210826)
    args = parser.parse_args()

    output_path = Path(args.output)
    players = []
    if output_path.exists():
        players = json.loads(output_path.read_text(encoding="utf-8-sig"))
    collected_ids = {int(player["id"]) for player in players}
    candidates = [
        candidate
        for candidate in balanced_candidates(load_candidates(Path(args.dataset)), args.limit, args.seed)
        if candidate["id"] not in collected_ids
    ]
    failures = 0
    with ThreadPoolExecutor(max_workers=max(1, args.workers)) as executor:
        futures = [
            executor.submit(fetch_player, candidate, args.delay, args.retries)
            for candidate in candidates
        ]
        for processed, future in enumerate(as_completed(futures), start=1):
            player, error = future.result()
            if player:
                players.append(player)
            else:
                failures += 1
            if processed % 50 == 0:
                write_players(output_path, players)
                print(f"coletados={len(players)} falhas={failures} processados={processed}", flush=True)

    write_players(output_path, players)
    by_position = defaultdict(int)
    for player in players:
        by_position[player["position"]] += 1
    print(json.dumps({
        "samples": len(players),
        "failures": failures,
        "byPosition": dict(sorted(by_position.items())),
    }, ensure_ascii=False), flush=True)


if __name__ == "__main__":
    main()
