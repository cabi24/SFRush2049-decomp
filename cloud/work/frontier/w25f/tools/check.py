#!/usr/bin/env python3
"""w16g static promotion dry-run (builder, run from the scratch root).

    python3 check.py SRC FN TARGET_ID [--flags F] [--tu-flags F] [--additions H]

Replays, without writing anything, the two checks a static promotion runs:

1. LOCK  - `pipeline.lock add SRC:FN --flags F [--target TARGET_ID]`'s verify_entry:
   reduced_tu(SRC, FN) compiled `cc -c F -I include -I include/PR -I <toolkit>/shim`
   (jobs/compile_score.compile_one), scored with jobs/scoring.score against the
   conveyor inventory target .o (tgt/TARGET_ID.o = BlobStore[n64_target.target_o_sha]).
2. TU    - what `promote run` splices: `#include "rom_tu.h"` + ADDITIONS (proposed
   rom_tu.h lines) + FN's body only, compiled with the Makefile's ROM-TU CFLAGS
   (TU flags; default = -G 0 -mips2 -O2 -non_shared -I include -I include/PR
   -D_LANGUAGE_C -Wab,-r4300_mul -Xcpluscomm -I src/rom), scored the same way.

For each object: scoring.score (default), strict (stack_differences=True),
reloc_blind_score, and an exact compare of `objdump -dr` (words + relocation
type/symbol/addend) against the target.
"""
import argparse, os, re, shlex, subprocess, sys, tempfile
from pathlib import Path

ROOT = Path.cwd()
TK = Path(os.environ.get("TK", os.path.expanduser(
    "~/rush2049/cache/toolkits/d4c39cbc85750cd02d3494f318c65e770bbf7d4b10aae137bcf5b5f149137c5b")))
os.environ["CONVEYOR_TOOLKIT"] = str(TK)
sys.path[:0] = [str(ROOT), str(ROOT / "tools/conveyor/jobs")]
import scoring  # noqa: E402
from tools.conveyor.seeds import context as contextmod  # noqa: E402
from tools.conveyor.seeds.extract_candidates import extract_named_function  # noqa: E402

LOCK_FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"
TU_FLAGS = ("-G 0 -mips2 -O2 -non_shared -I include -I include/PR -D_LANGUAGE_C "
            "-Wab,-r4300_mul -Xcpluscomm -I src/rom")


def cc(src_text, flags, obj, incs=()):
    src = obj.with_suffix(".c")
    src.write_text(src_text)
    cmd = [str(TK / "ido/cc"), "-c"] + shlex.split(flags)
    for i in incs:
        cmd += ["-I", str(i)]
    p = subprocess.run(cmd + ["-o", str(obj), str(src)], capture_output=True, text=True, cwd=ROOT)
    if p.returncode:
        return " | ".join(l for l in (p.stderr + p.stdout).splitlines() if "rror" in l)[:400] or "cc failed"
    return None


def dr(obj):
    out = subprocess.run(["mips-linux-gnu-objdump", "-dr", "-j", ".text", str(obj)],
                         capture_output=True, text=True, check=True).stdout
    rows = []
    for l in out.splitlines():
        m = re.match(r"\s*[0-9a-f]+:\s+([0-9a-f]{8})\s", l)
        if m:
            rows.append(m.group(1))
        m = re.match(r"\s*[0-9a-f]+:\s+(R_MIPS_\w+)\s+(\S+)", l)
        if m:
            rows.append(m.group(1) + " " + m.group(2))
    while rows and rows[-1] == "00000000":
        rows.pop()
    return rows


def report(tag, obj, target):
    s = scoring.score(target, obj)
    st = scoring.score(target, obj, stack_differences=True)
    rb = scoring.reloc_blind_score(target, obj)
    a, b = dr(target), dr(obj)
    same = "identical" if a == b else "DIFFERS(%d rows)" % sum(x != y for x, y in zip(a, b))
    print(f"{tag}: true={s} strict={st} reloc_blind={rb} objdump-dr {same} ({len(a)} rows)")
    return s == 0 and st == 0 and a == b


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src"); ap.add_argument("fn"); ap.add_argument("target")
    ap.add_argument("--flags", default=None, help="default: the source's /* flags: */ line")
    ap.add_argument("--tu-flags", default=TU_FLAGS)
    ap.add_argument("--additions", default=None)
    ap.add_argument("--ctx", default=None, help="context_check.py JSONL: FN's row preamble is the additions "
                    "(what promote_batch.py splices before the body)")
    ap.add_argument("--tu", default=None, help="real ROM TU (src/rom/lib_X.c): its text before FN's slot, "
                    "GLOBAL_ASM lines removed, replaces the bare rom_tu.h include")
    a = ap.parse_args()
    src = Path(a.src)
    text = src.read_text()
    first = text.splitlines()[0]
    m = re.match(r"/\* flags: (.*) \*/$", first)
    flags = a.flags or (m.group(1) if m else LOCK_FLAGS)
    target = ROOT / "tgt" / (a.target + ".o")
    ok = True
    with tempfile.TemporaryDirectory() as t:
        t = Path(t)
        red = contextmod.reduced_tu(text, a.fn)
        err = cc(red, flags, t / "lock.o", [ROOT / "include", ROOT / "include/PR", TK / "shim"])
        print(f"== {a.fn} (target {a.target}) src={src}")
        print(f"LOCK flags=[{flags}]")
        if err:
            print("LOCK: COMPILE FAIL " + err); ok = False
        else:
            ok &= report("LOCK", t / "lock.o", target)
        body = extract_named_function(src, a.fn)
        adds = Path(a.additions).read_text() if a.additions else ""
        if a.ctx:
            import json
            row = [r for r in map(json.loads, Path(a.ctx).read_text().splitlines()) if r["function"] == a.fn][-1]
            if row["status"] != "ok":
                print("CTX: context_check refused: " + row.get("reason", "")); ok = False
            else:
                adds += "\n".join(row["preamble"]) + "\n"
                a.additions = f"{a.ctx} preamble ({len(row['preamble'])} stmts, {len(row['dropped'])} dropped)"
        head = '#include "rom_tu.h"\n'
        if a.tu:
            real = (ROOT / a.tu).read_text()
            slot = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/%s/%s.s")' % (Path(a.tu).stem, a.fn)
            if slot not in real:
                print("TU: no passthrough slot " + slot); sys.exit(1)
            head = "\n".join(l for l in real[:real.index(slot)].splitlines()
                             if not l.startswith("#pragma GLOBAL_ASM")) + "\n"
            # Earlier promoted bodies become their prototypes (what is visible
            # at the slot), so the object holds FN alone and scores whole.
            from tools.conveyor.seeds.extract_candidates import extract_functions
            out, pos = [], 0
            for _name, st, en in extract_functions(head):
                out.append(head[pos:st]); out.append(head[st:head.index("{", st)].rstrip() + ";")
                pos = en
            head = "".join(out) + head[pos:]
        if a.tu and a.ctx:  # w25f: promote_batch --prefix-dedup (statements already before the slot are dropped)
            norm = lambda x: " ".join(x.split())
            have = {norm(l) for l in head.splitlines() if l.strip()}
            adds = "".join(l + "\n" for l in adds.splitlines()
                           if (l.startswith("#") and not (l.startswith("#include") and norm(l) in have))
                           or (not l.startswith("#") and norm(l) not in have))
        tu = head + adds + "\n" + body + "\n"
        print(f"TU   flags=[{a.tu_flags}] tu={a.tu or 'rom_tu.h only'} additions={a.additions}")
        err = cc(tu, a.tu_flags, t / "tu.o")
        if err:
            print("TU: COMPILE FAIL " + err); ok = False
        else:
            ok &= report("TU", t / "tu.o", target)
    print("RESULT", "PASS" if ok else "FAIL")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
