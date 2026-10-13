#!/usr/bin/env python3
"""w29n: rename the w29n game names inside research packets that replay against them.

    python3 cloud/work/frontier/w29n/tools/rebind_packets.py [--write] PATH...

PATH is a packet directory or a test file. Some packets replay the live game
targets (asm/us/blob, keyed by function name) or compile their own sources
against asm/us/blob/symbols.json, so after the rename their verifiers look up
names that no longer exist (KeyError 'track_process_main', "unresolved symbol
slot_state_lookup"). Rebinding is the same rename, applied to the packet
(lane w23g's rebind_packets.py rules, for game names):

  1. every .c/.h/.py/.json file of the packet (and each named test file) gets
     the accepted function and data renames of renames.json as whole-identifier
     tokens (C code by static_rename.permute_c; Python and JSON as tokens, since
     verifiers embed C in strings and receipts key results by name); a name
     followed by a file extension is a path and stays (files keep their names).
     Markdown and other text stay as recorded history.
  2. every sha256 in the packet's .py/.json files and the named tests that pinned
     a file whose current form is a proven rename gets that file's current hash:
     proven = a file rewritten by rule 1 (its bytes before hashed to the pin), or
     a src/blob source that is exactly its --base text renamed (permute_c).
     Rule 2 repeats until no pin changes (a receipt may pin its own verifier).
Nothing is written without --write; every change is printed.
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


def sha(data):
    return hashlib.sha256(data).hexdigest()


def git_bytes(base, rel):
    p = subprocess.run(["git", "show", f"{base}:{rel}"], cwd=REPO, capture_output=True)
    return p.stdout if p.returncode == 0 else None


def tokens(text, names):
    # a name followed by a file extension is a file of the packet or the repo (group/func_800F1210.c):
    # files keep their names, so paths are not renamed
    rx = re.compile(r"(?<![A-Za-z0-9_])(%s)(?![A-Za-z0-9_]|\.(?:c|h|s|o|json|py)\b)"
                    % "|".join(map(re.escape, sorted(names, key=len, reverse=True))))
    return rx.sub(lambda m: names[m.group(1)], text)


def files_of(paths):
    out = []
    for p in paths:
        p = (REPO / p).resolve()
        if p.is_dir():
            out += sorted(f for f in p.rglob("*") if f.is_file() and "__pycache__" not in f.parts
                          and f.suffix in (".c", ".h", ".py", ".json"))
        else:
            out.append(p)
    return out


def proven_src_pins(base, names):
    """{old sha: new sha} for src/blob files that are exactly their base text renamed."""
    out = {}
    changed = subprocess.run(["git", "diff", "--name-only", base, "--", "src/blob"], cwd=REPO,
                             capture_output=True, text=True, check=True).stdout.split()
    for rel in changed:
        live = REPO / rel
        if not live.is_file() or not rel.endswith((".c", ".h")):
            continue
        old = git_bytes(base, rel)
        if old is None:
            continue
        if static_rename.permute_c(old.decode(), names).encode() == live.read_bytes():
            out[sha(old)] = sha(live.read_bytes())
    return out


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("paths", nargs="+")
    p.add_argument("--base", default="894a4d1ef", help="the pre-rename revision")
    p.add_argument("--write", action="store_true")
    a = p.parse_args()
    names = static_rename.mapping(static_rename.load_map(MAP))
    files = files_of(a.paths)
    texts = {f: f.read_text() for f in files}
    new = {}
    pins = proven_src_pins(a.base, names)
    for f, text in texts.items():
        t = static_rename.permute_c(text, names) if f.suffix in (".c", ".h") else tokens(text, names)
        if t != text:
            new[f] = t
            pins[sha(text.encode())] = sha(t.encode())
            print(f"rename  {f.relative_to(REPO)}")
    for _round in range(5):
        moved = False
        for f in files:
            if f.suffix not in (".py", ".json"):
                continue
            text = new.get(f, texts[f])
            t = HEX.sub(lambda m: pins.get(m.group(0), m.group(0)), text)
            if t != text:
                for old in set(HEX.findall(text)) - set(HEX.findall(t)):
                    print(f"pin     {f.relative_to(REPO)}: {old[:12]} -> {pins[old][:12]}")
                before = new.get(f, texts[f])
                new[f] = t
                pins[sha(before.encode())] = sha(t.encode())
                # a pin of the original bytes must follow to the final bytes
                for k, v in list(pins.items()):
                    if v == sha(before.encode()):
                        pins[k] = sha(t.encode())
                moved = True
        if not moved:
            break
    if a.write:
        for f, t in new.items():
            f.write_text(t)
    print(f"{len(new)} files {'written' if a.write else 'would change (dry run)'}")


if __name__ == "__main__":
    main()
