#!/usr/bin/env python3
"""w29n: rebind the research receipts that the game rename (renames.json) moved.

    python3 cloud/work/frontier/w29n/tools/rebind.py [--base REV] [--write]

Each rebind is proved before anything is written (dry run by default):

  source pins   a receipt pins the sha256 of a src/blob file. Proof: the file at
                --base hashes to the pinned value and the live file is exactly
                that text renamed (static_rename.permute_c with the table).
  census pins   a receipt pins a digest over the whole parsed asm/us/blob target
                inventory (name + words). Proof: the same digest over today's
                targets with the rename undone (new name -> old name) equals the
                pinned value, so only names moved; the new digest is then pinned.
  name lookups  dot_frame_dispatch selects audio_doppler_full by name; it moves to
                ShowCountdown (same address) in verify.py and its receipt.
  test line     test_ipakit_heads expects a registered head to be called
                func_<address>; it now applies the table first.
Packets that replay renamed targets or compile against renamed symbols are
renamed as a whole by rebind_packets.py.

The edits are listed in cloud/work/frontier/w29n/RESULTS.md ("Receipts").
"""
import argparse
import hashlib
import json
import re
import struct
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(REPO))
from tools.cloud import score  # noqa: E402
from tools.conveyor.pipeline import static_rename  # noqa: E402

MAP = REPO / "cloud/work/frontier/w29n/renames.json"
FR = REPO / "cloud/work/frontier"

SOURCE_PINS = [   # (receipt, pinned live file)
    ("dot_frame_dispatch_20261005/verification.json", "src/blob/Effects_UpdateEmitters.c"),
    ("dot_runtime_a_resource_cache_20261006/verification.json", "src/blob/groups/audio_frame_sync/group.c"),
]


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def packed(words):
    return struct.pack(">%dI" % len(words), *words)


def blob_targets():
    old = score.ASM_DIR
    score.ASM_DIR = REPO / "asm/us/blob"
    try:
        return score.targets()
    finally:
        score.ASM_DIR = old


def digest_name_words(table):          # dot_static_10450 / dot_static_12234
    h = hashlib.sha256()
    for n, ws in sorted(table.items()):
        h.update(n.encode() + b"\0" + packed(ws))
    return h.hexdigest()


def digest_name_sha(table):            # dot_runtime_b_hud_teardown
    return sha(json.dumps({n: sha(packed(w)) for n, w in sorted(table.items())}, sort_keys=True).encode())


CENSUS_PINS = [   # (receipt, digest function)
    ("dot_static_10450_20261013/verification.json", digest_name_words),
    ("dot_static_12234_20261013/verification.json", digest_name_words),
    ("dot_runtime_b_hud_teardown_20261012/verification.json", digest_name_sha),
    ("dot_runtime_b_radial_impulse_20261012/verification.json", digest_name_sha),
]

LOOKUPS = [       # (file, old text, new text, count): lookups of a renamed function by name
    ("dot_frame_dispatch_20261005/verify.py",
     "'D_80152032','audio_doppler_full','math_utility'",
     "'D_80152032','ShowCountdown','math_utility'", 1),
    # its receipt: the selected binding and the genuine-context relocation recorded for the
    # live Effects_UpdateEmitters.c, which now calls ShowCountdown (same address 0x800B61FC)
    ("dot_frame_dispatch_20261005/verification.json",
     '"audio_doppler_full": 2148229628,', '"ShowCountdown": 2148229628,', 1),
    ("dot_frame_dispatch_20261005/verification.json",
     '"symbol": "audio_doppler_full"', '"symbol": "ShowCountdown"', 1),
]
TEST_PATCHES = [  # (test file, old text, new text): exact, applied once
    ("tests/cloud/test_ipakit_heads.py",
     "from ipakit import Corpus, Func, IMAGE_BASE, load_corpus, heads, unique_targets  # noqa: E402\n",
     "from ipakit import Corpus, Func, IMAGE_BASE, load_corpus, heads, unique_targets  # noqa: E402\n"
     "import json  # noqa: E402\n"
     "W29N_RENAMES = {r['old']: r['new'] for r in json.loads(\n"
     "    (ROOT / 'cloud/work/frontier/w29n/renames.json').read_text())['renames'] if r['status'] == 'accepted'}\n"),
    ("tests/cloud/test_ipakit_heads.py",
     "                self.assertEqual((f.name, len(f.words)), ('func_%08X' % a, words))\n",
     "                # a registered head keeps its func_ name unless a symbol rename moved it\n"
     "                # (w29n: cloud/work/frontier/w29n/renames.json, e.g. func_80108154 = animLapTime)\n"
     "                name = 'func_%08X' % a\n"
     "                self.assertEqual((f.name, len(f.words)), (W29N_RENAMES.get(name, name), words))\n"),
    ("tests/cloud/test_ipakit_heads.py",
     "            self.assertEqual((f.name, len(f.words)), ('func_%08X' % a, words))\n",
     "            name = 'func_%08X' % a   # w29n: func_8010221C = drawMain\n"
     "            self.assertEqual((f.name, len(f.words)), (W29N_RENAMES.get(name, name), words))\n"),
]
EDITED_PINS = [   # (receipt, file edited by LOOKUPS that the receipt pins)
    ("dot_frame_dispatch_20261005/verification.json", "cloud/work/frontier/dot_frame_dispatch_20261005/verify.py"),
]


def git_show(base, rel):
    p = subprocess.run(["git", "show", f"{base}:{rel}"], cwd=REPO, capture_output=True)
    if p.returncode:
        raise SystemExit(f"refusing: {rel} missing at {base}")
    return p.stdout


def rebind_tokens(text, names):
    rx = re.compile(r"(?<![A-Za-z0-9_])(%s)(?![A-Za-z0-9_])" % "|".join(map(re.escape, sorted(names, key=len, reverse=True))))
    return rx.sub(lambda m: names[m.group(1)], text)


def replace_once(path, old, new, write, log):
    text = path.read_text()
    n = text.count(old)
    if n == 0 and text.count(new):
        log.append(f"{path.relative_to(REPO)}: {new[:12]} already pinned")
        return
    if n == 0:
        raise SystemExit(f"refusing: {path.relative_to(REPO)}: pinned value {old[:16]} not found")
    if write:
        path.write_text(text.replace(old, new))
    log.append(f"{path.relative_to(REPO)}: {old[:12]} -> {new[:12]} ({n}x)")


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--base", default="HEAD", help="the pre-rename revision (replay.sh: HEAD)")
    p.add_argument("--write", action="store_true")
    p.add_argument("--map", default=None,
                   help="the batch being applied (batch.py output): census digests undo only its renames, "
                        "since the pinned digests already carry every earlier batch (default: the whole table)")
    a = p.parse_args()
    names = static_rename.mapping(static_rename.load_map(a.map or MAP))
    inverse = {v: k for k, v in names.items()}
    log = []
    for receipt, rel in SOURCE_PINS:
        base = git_show(a.base, rel)
        live = (REPO / rel).read_bytes()
        if static_rename.permute_c(base.decode(), names).encode() != live:
            raise SystemExit(f"refusing: {rel} is not the {a.base} text renamed")
        replace_once(FR / receipt, sha(base), sha(live), a.write, log)
    table = blob_targets()
    undone = {inverse.get(n, n): w for n, w in table.items()}
    for receipt, fn in CENSUS_PINS:
        old, new = fn(undone), fn(table)
        replace_once(FR / receipt, old, new, a.write, log)
    for rel, old, new, count in LOOKUPS:
        path = FR / rel
        text = path.read_text()
        # a later batch may have renamed other tokens of the edited text (w30n: D_80152032 -> gCountDown)
        if text.count(old) == 0 and (text.count(new) == count
                                     or text.count(rebind_tokens(new, names)) == count):
            log.append(f"{rel}: name edit already applied")
            continue
        if text.count(old) != count:
            raise SystemExit(f"refusing: {rel}: {old!r} not found {count}x")
        if a.write:
            path.write_text(text.replace(old, new))
        log.append(f"{rel}: {old!r} -> {new!r}")
    for receipt, rel in EDITED_PINS:
        # proof: the live file is the base text with exactly the LOOKUPS edits for it
        base = git_show(a.base, rel).decode()
        want = base
        for f, old, new, _c in LOOKUPS:
            if "cloud/work/frontier/" + f == rel:
                want = want.replace(old, new)
        live = (REPO / rel).read_text() if a.write else want
        if live != want:
            raise SystemExit(f"refusing: {rel} is not the base text with the listed name edits")
        replace_once(FR / receipt, sha(base.encode()), sha(want.encode()), a.write, log)
    for rel, old, new in TEST_PATCHES:
        path = REPO / rel
        text = path.read_text()
        if new in text:
            log.append(f"{rel}: test patch already applied")
            continue
        if text.count(old) != 1:
            raise SystemExit(f"refusing: {rel}: patch anchor not found once")
        if a.write:
            path.write_text(text.replace(old, new))
        log.append(f"{rel}: test patch")
    for line in log:
        print(line)
    print("written" if a.write else "dry run: nothing written")


if __name__ == "__main__":
    main()
