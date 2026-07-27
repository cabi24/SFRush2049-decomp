"""ROM-truth pre-screen for locked matches.

A `matched.lock.json` entry means a candidate scored 0 against the pipeline's
compile_score *target object*. Promotion (feature 004) is stricter: it splices
the C into the real ROM and gates on the full-ROM SHA-1. Those disagree when the
target object diverges from the actual ROM — e.g. m2c emits a redundant local
whose dead store the compiler drops, shifting a stack slot by 4 bytes
(osPiRawReadWord: spill at 0x18 vs the ROM's 0x1C). Such a lock is a near-miss
wearing a "matched" label; promoting it just burns a full ROM build for a
guaranteed rollback.

This screens each unpromoted locked function *without* a ROM build: compile the
locked source standalone with IDO on the builder, disassemble just that
function, and masked-diff its instruction words against the real bytes in
`baserom.us.z64` (relocations masked exactly as the scorer does). 0 => the code
reproduces the ROM and is worth a real promotion attempt; >0 => a near-miss to
send back to the permuter / near-miss closer instead.

    python3 -m tools.conveyor.pipeline.romtruth [--json]

The full-ROM SHA-1 gate in `promote` remains authoritative; this only predicts,
so promotion never trusts it blindly.
"""
import argparse
import json
import os
import re
import sqlite3
import subprocess
import sys
from pathlib import Path

from . import targets as targetsmod
from ..client import DEFAULT_DATA
from ..jobs import scoring

REPO = targetsmod.REPO
BUILDER = os.environ.get("CONVEYOR_BUILDER", "watchman2")
BUILDER_REPO = os.environ.get("CONVEYOR_BUILDER_REPO", "~/rush2049/repo")
BASEROM = REPO / "baserom.us.z64"
DELTA = targetsmod.STATIC_ROM_DELTA  # vaddr - DELTA = ROM file offset
CC = "tools/ido-static-recomp/build/out/cc"
OBJDUMP = "mips-linux-gnu-objdump"
LOCKFILE = REPO / "matched.lock.json"

_LABEL_RE = re.compile(r"^\s*([0-9a-f]+)\s+<(.+)>:\s*$")


def _rom_words(rom, addr, n):
    off = addr - DELTA
    return [int.from_bytes(rom[off + 4 * i: off + 4 * i + 4], "big")
            for i in range(n)]


def _strip_tail_zeros(words):
    """Drop trailing 0x0 padding (nops to alignment) so a body that matches but
    whose slot carries extra padding words isn't falsely flagged."""
    ws = list(words)
    while ws and ws[-1] == 0:
        ws.pop()
    return ws


def _func_words(disasm, name):
    """Instruction words of just `name`'s .text plus its byte start offset,
    from `objdump -d` (handles multi-function objects like os_ai.c)."""
    words, start, inside = [], None, False
    for line in disasm.splitlines():
        m = _LABEL_RE.match(line)
        if m:
            if inside:
                break  # reached the next function
            if m.group(2) == name:
                inside, start = True, int(m.group(1), 16)
            continue
        if not inside:
            continue
        parts = line.split("\t")
        if len(parts) < 2 or not parts[0].strip().endswith(":"):
            continue
        tok = parts[1].strip().split()
        if tok:
            try:
                words.append(int(tok[0], 16))
            except ValueError:
                pass
    return words, start


def _func_relocs(relocs, start, count):
    """Reloc (index, mask) sites within the function, rebased to its start."""
    sites, in_text = [], False
    for line in relocs.splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            in_text = "[.text]" in line
            continue
        if not in_text:
            continue
        s = line.strip()
        if not s:
            in_text = False
            continue
        p = s.split()
        try:
            off = int(p[0], 16)
        except (ValueError, IndexError):
            continue
        idx = (off - start) // 4
        if 0 <= idx < count:
            sites.append((idx, scoring._RELOC_MASKS.get(p[1], scoring._MASK_UNKNOWN)))
    return sites


def _unpromoted_locks(conn):
    nt = {r["target_id"]: r for r in conn.execute(
        "SELECT target_id, address, insn_count FROM n64_target")}
    out = []
    for spec, e in sorted(json.loads(LOCKFILE.read_text()).items()):
        src, _, _ = spec.rpartition(":")
        if src.startswith("src/rom/"):
            continue  # already promoted
        r = nt.get(e["target_id"])
        if r:
            out.append((e["target_id"], e["flagset"], src,
                        r["address"], r["insn_count"]))
    return out


def _remote_build(items):
    """One ssh: compile + objdump -d/-r each source on the builder. Returns
    {target_id: {"COMPILE","DIS","REL"}}."""
    lines = ["set +e", "cd " + BUILDER_REPO]
    for tid, fl, src, _, _ in items:
        o = f"/tmp/romtruth_{tid}.o"
        lines += [
            f'echo "@@@{tid}@@@COMPILE"',
            f"{CC} -c {fl} -Iinclude -Iinclude/PR -D_LANGUAGE_C "
            f"-Wab,-r4300_mul -Xcpluscomm -o {o} {src} 2>&1",
            f'echo "@@@{tid}@@@DIS"', f"{OBJDUMP} -d {o} 2>/dev/null",
            f'echo "@@@{tid}@@@REL"', f"{OBJDUMP} -r {o} 2>/dev/null",
            f'echo "@@@{tid}@@@END"']
    out = subprocess.run(["ssh", BUILDER, "bash -s"], input="\n".join(lines),
                         capture_output=True, text=True).stdout
    blocks, cur, sect, buf = {}, None, None, {}
    for line in out.splitlines():
        m = re.match(r"@@@(.+?)@@@(\w+)$", line)
        if m:
            tid, tag = m.group(1), m.group(2)
            if tag == "COMPILE":
                cur, sect, buf = tid, "COMPILE", {"COMPILE": [], "DIS": [], "REL": []}
            elif tag == "END":
                blocks[cur], cur = buf, None
            else:
                sect = tag
            continue
        if cur and sect:
            buf[sect].append(line)
    return blocks


def screen():
    rom = BASEROM.read_bytes()
    conn = sqlite3.connect(str(DEFAULT_DATA / "conveyor.db"))
    conn.row_factory = sqlite3.Row
    items = _unpromoted_locks(conn)
    blocks = _remote_build(items)
    results = []
    for tid, fl, src, addr, n in items:
        b = blocks.get(tid, {})
        comp = "\n".join(b.get("COMPILE", [])).strip()
        if any(x in comp for x in ("Fatal", "Error", "cfe:")) or not b.get("DIS"):
            results.append({"target": tid, "verdict": "compile_fail",
                            "detail": comp.splitlines()[-1][:60] if comp else "?"})
            continue
        cw, start = _func_words("\n".join(b["DIS"]), tid)
        if start is None:
            results.append({"target": tid, "verdict": "no_symbol", "detail": ""})
            continue
        sites = _func_relocs("\n".join(b["REL"]), start, len(cw))
        cand = _strip_tail_zeros(cw)
        target = _strip_tail_zeros(_rom_words(rom, addr, n))
        diff = scoring._masked_diff(target, cand, sites)
        results.append({"target": tid, "verdict": "rom_true" if diff == 0 else "false_0",
                        "diff": diff, "flagset": fl, "source": src})
    return results


def main():
    ap = argparse.ArgumentParser(description="ROM-truth pre-screen for locked matches")
    ap.add_argument("--json", action="store_true", help="machine-readable output")
    args = ap.parse_args()
    results = screen()
    if args.json:
        print(json.dumps(results, indent=2))
        return
    true_ = [r for r in results if r["verdict"] == "rom_true"]
    false_ = [r for r in results if r["verdict"] == "false_0"]
    other = [r for r in results if r["verdict"] not in ("rom_true", "false_0")]
    print(f"{'target':22}  verdict")
    for r in results:
        if r["verdict"] == "rom_true":
            v = "ROM-TRUE  (promote)"
        elif r["verdict"] == "false_0":
            v = f"FALSE-0   off by {r['diff']} (near-miss)"
        else:
            v = f"{r['verdict'].upper()}: {r.get('detail', '')}"
        print(f"{r['target']:22}  {v}")
    print(f"\n{len(true_)} ROM-true, {len(false_)} false-0, {len(other)} other")


if __name__ == "__main__":
    main()
