import argparse
import html
import json
import re
import time
import urllib.request
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path


PESDB_BASE = "https://pesdb.net/pes2021/"

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


def fetch(url: str) -> str:
    req = urllib.request.Request(
        url,
        headers={
            "User-Agent": "Mozilla/5.0 iMLM-PES research",
            "Accept-Language": "en-US,en;q=0.9",
        },
    )
    with urllib.request.urlopen(req, timeout=25) as response:
        return response.read().decode("utf-8", errors="replace")


def strip_text(markup: str) -> str:
    text = re.sub(r"<(script|style).*?</\1>", "\n", markup, flags=re.I | re.S)
    text = re.sub(r"<[^>]+>", "\n", text)
    text = html.unescape(text)
    lines = [line.strip() for line in text.splitlines()]
    return "\n".join(line for line in lines if line)


def after_label(text: str, label: str) -> str:
    match = re.search(rf"^{re.escape(label)}:\s*(.*)$", text, flags=re.M)
    return match.group(1).strip() if match else ""


def int_after_label(text: str, label: str):
    value = after_label(text, label)
    match = re.search(r"(\d+)", value)
    return int(match.group(1)) if match else None


def parse_player(player_id: int, markup: str):
    text = strip_text(markup)
    name = after_label(text, "Player Name")
    position_match = re.search(r"^Position:\s*\n+([A-Z]+)$", text, flags=re.M)
    position = position_match.group(1) if position_match else ""
    overall = int_after_label(text, "Overall Rating")

    if not name or not position or overall is None:
        return None

    player = {
        "id": player_id,
        "name": name,
        "position": position,
        "overall": overall,
        "height": int_after_label(text, "Height"),
        "weight": int_after_label(text, "Weight"),
        "age": int_after_label(text, "Age"),
    }

    for pes_label, key in STAT_LABELS.items():
        value = int_after_label(text, pes_label)
        if value is not None:
            player[key] = value

    if all(key in player for key in STAT_LABELS.values()):
        return player
    return None


def collect_ids(page_start: int, page_end: int, page_step: int, delay: float):
    ids = []
    seen = set()
    for page in range(page_start, page_end + 1, page_step):
        url = PESDB_BASE if page == 1 else f"{PESDB_BASE}?page={page}"
        markup = fetch(url)
        for match in re.finditer(r"href=[\"'](?:\./)?\?id=(\d+)[\"']", markup):
            player_id = int(match.group(1))
            if player_id not in seen:
                seen.add(player_id)
                ids.append(player_id)
        time.sleep(delay)
    return ids


def fetch_player(player_id: int, delay: float):
    try:
        player = parse_player(player_id, fetch(f"{PESDB_BASE}?id={player_id}"))
        return player_id, player, None
    except Exception as error:
        return player_id, None, str(error)
    finally:
        if delay:
            time.sleep(delay)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--page-start", type=int, default=1)
    parser.add_argument("--page-end", type=int, default=499)
    parser.add_argument("--page-step", type=int, default=20)
    parser.add_argument("--limit", type=int, default=500)
    parser.add_argument("--delay", type=float, default=0.05)
    parser.add_argument("--workers", type=int, default=6)
    parser.add_argument("--output", required=True)
    args = parser.parse_args()

    ids = collect_ids(args.page_start, args.page_end, args.page_step, args.delay)[: args.limit]
    samples = []
    failures = 0
    with ThreadPoolExecutor(max_workers=max(1, args.workers)) as executor:
        futures = [executor.submit(fetch_player, player_id, args.delay) for player_id in ids]
        for index, future in enumerate(as_completed(futures), start=1):
            player_id, player, error = future.result()
            if player:
                samples.append(player)
            else:
                failures += 1
            if index % 50 == 0:
                print(f"coletados={len(samples)} falhas={failures} processados={index}")

    samples.sort(key=lambda player: player["id"])

    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(samples, ensure_ascii=False, indent=2), encoding="utf-8")

    by_position = {}
    for player in samples:
        by_position[player["position"]] = by_position.get(player["position"], 0) + 1
    print(json.dumps({"samples": len(samples), "failures": failures, "by_position": by_position}, ensure_ascii=False))


if __name__ == "__main__":
    main()
