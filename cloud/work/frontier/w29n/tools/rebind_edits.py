#!/usr/bin/env python3
"""w30n: exact, proved token-rename edits inside fixtures that also hold names which must NOT move.

    python3 cloud/work/frontier/w29n/tools/rebind_edits.py [--base REV] [--write]

rebind_packets.py renames every old name in a whole packet. Some fixtures mix the
game names a batch renames with names that keep their old spelling on purpose
(runtime-image packets: overlay sources and asm/us/ovl_*/symbols.json are frozen
with the old spellings), so a whole-packet rename would break them. For those,
EDITS lists the exact strings that refer to the GAME side. Rules (refuse otherwise):

  1. each new string is exactly its old string with the batch's renames applied as
     whole-identifier tokens (renames.json, accepted rows), and it occurs `count`
     times after the edit where the old string occurred `count` times before;
  2. PINS: in the listed receipts (JSON, indent 2), a sha256 is re-pinned only where the
     receipt says WHICH file it pins — the value of a key naming the file
     (`"src/blob/x.c": "<sha>"`, also a bare file name for the packet's own files) or a
     `*sha256` field of an object whose `path` names it — and only when that file is an
     edited file (its --base bytes hashed to the pin) or a src/blob file that is exactly
     its --base text renamed (static_rename.permute_c). Hash-only matching is not used:
     cloud/matches copies are byte-identical to the src/blob sources they were
     installed from, and keep their spelling.
Idempotent: an edit already applied (old absent, new present `count` times) is skipped.
"""
import argparse
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(REPO))
from tools.conveyor.pipeline import static_rename  # noqa: E402

MAP = REPO / "cloud/work/frontier/w29n/renames.json"
HEX = re.compile(r"\b[0-9a-f]{64}\b")

# (file, old text, new text, count)
EDITS = [
    # overlay-A packet; its group/host sources are runtime-image code (old spellings stay), but the
    # declaration provenance it checks is the GAME source src/blob/func_800B24EC.c (renamed)
    ("cloud/work/frontier/dot_runtime_a_availability_root_20261012/verify.py",
     "require('extern volatile u8 D_80140BDC;' in volatile_source.read_text(),'table-count declaration provenance')",
     "require('extern volatile u8 gNumModels;' in volatile_source.read_text(),'table-count declaration provenance')", 1),
]
# receipts whose pins follow the edited files / renamed sources
PINS = [
    "cloud/work/frontier/dot_runtime_a_availability_root_20261012/verification.json",
    # runtime-image-B packet: its count contract pins the GAME sources that share D_801543CA
    "cloud/work/frontier/dot_runtime_b_radius_effect_20261012/verification.json",
    "cloud/work/frontier/dot_runtime_b_radial_impulse_20261012/verification.json",
    # game HUD-bar packets whose semantic harness derives addresses from the D_<addr> spelling of the
    # names its own sources use (cloud/matches copies, not renamed): only their context-source pins move
    "cloud/work/frontier/dot_blit_family_20261005/verification.json",
    "cloud/work/frontier/dot_new_multiblit_20261005/verification.json",
]

# (receipt, key, file): a hash field that names its file only by its key
FIELDS = [
    ("cloud/work/frontier/dot_runtime_a_availability_root_20261012/verification.json",
     "texture_lookup_source_sha256", "src/blob/func_800B24EC.c"),
]

# (receipt, old text, new text): object-section hashes (and the string table's size, 3 bytes shorter) that a rename moves without a source file to
# re-hash. Each is proved by hand and recorded in cloud/work/frontier/w30n/RESULTS.md: the fresh IDO object of
# dot_steady_camera's group has .text/.rel.text/.rodata/.options/.reginfo identical to the receipt; its
# .strtab with `gCamera` renamed back to `D_80150B70` hashes to the pinned d688083c…, and its .symtab with
# st_name offsets remapped to that string table hashes to the pinned 4249206b… (rocky, 2026-10-13).
OBJECT_PINS = [
    ("cloud/work/frontier/dot_steady_camera_20261005/verification.json",
     '".strtab": {\n        "bytes": 353,\n        "sha256": "d688083cb2a98ed183046094af7ac4dae47c2ee19dca08136c5f35dd09dea78a"',
     '".strtab": {\n        "bytes": 350,\n        "sha256": "6591c47d57329c089215cd6ab444dc5724d854422cadbb639ff4cfe3eef9c2d1"'),
    ("cloud/work/frontier/dot_steady_camera_20261005/verification.json",
     "4249206b6707acce7dafea70a1eb199f488a079c9c7d02b4a1d6b379b2a10194",
     "3daec3b30f7aa7e91f60db8dd0f0dc27c6294195ace9afe820b8d26eebd55044"),
]


def sha(b):
    return hashlib.sha256(b).hexdigest()


def git_bytes(base, rel):
    p = subprocess.run(["git", "show", f"{base}:{rel}"], cwd=REPO, capture_output=True)
    return p.stdout if p.returncode == 0 else None


def tokens(text, names):
    rx = re.compile(r"(?<![A-Za-z0-9_])(%s)(?![A-Za-z0-9_])" % "|".join(map(re.escape, sorted(names, key=len, reverse=True))))
    return rx.sub(lambda m: names[m.group(1)], text)


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--base", default="HEAD")
    p.add_argument("--write", action="store_true")
    p.add_argument("--map", default=None, help="the batch table (default: whole renames.json)")
    a = p.parse_args()
    names = static_rename.mapping(static_rename.load_map(a.map or MAP))
    pins, log = {}, []
    texts = {}
    for rel, old, new, count in EDITS:
        if tokens(old, names) != new:
            sys.exit(f"refusing: {rel}: the new text is not the old text renamed")
        path = REPO / rel
        text = texts.get(rel, path.read_text())
        if text.count(old) == 0 and text.count(new) == count:
            log.append(f"{rel}: edit already applied")
            continue
        if text.count(old) != count:
            sys.exit(f"refusing: {rel}: {old[:50]!r} found {text.count(old)}x, expected {count}")
        texts[rel] = text.replace(old, new)
        log.append(f"edit    {rel}: {old[:40]!r} -> {new[:40]!r}")
    for rel, text in texts.items():
        base = git_bytes(a.base, rel)
        if base is not None:
            pins[rel] = (sha(base), sha(text.encode()))
    changed = subprocess.run(["git", "diff", "--name-only", a.base, "--", "src/blob"], cwd=REPO,
                             capture_output=True, text=True, check=True).stdout.split()
    for rel in changed:
        live = REPO / rel
        old = git_bytes(a.base, rel)
        if old is None or not live.is_file() or not rel.endswith((".c", ".h")):
            continue
        if static_rename.permute_c(old.decode(), names).encode() == live.read_bytes():
            pins[rel] = (sha(old), sha(live.read_bytes()))

    def pinned(key, value, packet):
        for rel, (old, new) in pins.items():
            if value == old and key in (rel, str(Path(rel).relative_to(packet)) if rel.startswith(packet + "/") else rel):
                return new
        return None

    def walk(x, packet, log_rel):
        if isinstance(x, dict):
            out = {}
            path = x.get("path") if isinstance(x.get("path"), str) else None
            for k, v in x.items():
                new = None
                if isinstance(v, str) and HEX.fullmatch(v):
                    new = pinned(k, v, packet) or (pinned(path, v, packet) if path and k.endswith("sha256") else None)
                    for receipt, field, rel in FIELDS:
                        if not new and receipt == log_rel and k == field and rel in pins and pins[rel][0] == v:
                            new = pins[rel][1]
                if new:
                    log.append(f"pin     {log_rel}: {k} {v[:12]} -> {new[:12]}")
                out[k] = new or walk(v, packet, log_rel)
            return out
        if isinstance(x, list):
            return [walk(v, packet, log_rel) for v in x]
        return x
    for rel in PINS:
        path = REPO / rel
        text = texts.get(rel, path.read_text())
        doc = json.loads(text)
        if json.dumps(doc, indent=2) + "\n" != text:
            sys.exit(f"refusing: {rel} is not json.dumps(indent=2) text")
        new = json.dumps(walk(doc, str(Path(rel).parent), rel), indent=2) + "\n"
        if new != text:
            texts[rel] = new
    for rel, old, new in OBJECT_PINS:
        text = texts.get(rel, (REPO / rel).read_text())
        if old in text:
            texts[rel] = text.replace(old, new)
            log.append(f"pin     {rel}: object section {old[:12]} -> {new[:12]} ({text.count(old)}x)")
    if a.write:
        for rel, text in texts.items():
            (REPO / rel).write_text(text)
    for line in log:
        print(line)
    print(f"{len(texts)} files {'written' if a.write else 'would change (dry run)'}")


if __name__ == "__main__":
    main()
