#!/usr/bin/env python3
"""report.py: residual report for an unfinished function (stdlib only).

    report.py RESULT_DIR [--lines N] [--json]      a search.py output dir (search_log.json + best.c / best_group/)
    report.py FILE FUNC [--flags ..] [--lines N]   compile FILE and report (no search history)
    report.py group DIR [--lines N]                report a group dir (no search history)

Prints compact markdown designed so a model reads ONLY this for an unfinished function:
score summary, frame size and register usage (target vs ours, with prologue/epilogue), the
instruction-aligned diff hunks (disassembled, at most --lines lines), a tally of register
substitutions, the mutations tried (best score deltas, per-class stats) and mutation classes
not yet tried.
"""
import argparse
import json
import re
import struct
import sys
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(HERE.parent))
sys.path.insert(0, str(ROOT / "tools" / "cloud"))
from amatch import aligned, builder  # noqa: E402
import score  # noqa: E402

GPR = ("zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 "
       "gp sp fp ra").split()
CALLEE_SAVED = {16, 17, 18, 19, 20, 21, 22, 23, 30}
STATIC_CLASSES = [
    "loop-form", "counter-type", "local-drop", "local-inline", "decl-order", "pad-local",
    "extern-defined", "u32-launder", "knr-prototype", "param-type", "shared-exit", "shift-form",
    "operand-flip", "literal-type", "cond-chain", "if-else-swap", "fp-scale-add", "case-order",
    "stmt-order", "opt-level", "group-keep", "group-standin-calls", "group-callee-params",
]


# --- tiny MIPS decoder (register usage only) -------------------------------------------

def regs_used(w):
    """(gprs, fprs) touched by a word; approximate but enough to compare allocations."""
    op = w >> 26
    rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    g, f = set(), set()
    if op == 0:
        fn = w & 63
        if fn in (0, 2, 3):
            g |= {rt, rd}
        elif fn == 8:
            g.add(rs)
        elif fn == 9:
            g |= {rs, rd}
        elif fn in (0x0C, 0x0D, 0x0F):
            pass
        elif fn in (0x10, 0x12):
            g.add(rd)
        elif fn in (0x11, 0x13):
            g.add(rs)
        elif 0x18 <= fn <= 0x1F:
            g |= {rs, rt}
        else:
            g |= {rs, rt, rd}
    elif op == 1:
        g.add(rs)
    elif op == 2:
        pass
    elif op == 3:
        g.add(31)
    elif op in (4, 5):
        g |= {rs, rt}
    elif op in (6, 7):
        g.add(rs)
    elif op == 0x0F:
        g.add(rt)
    elif op == 0x11:
        if rs in (0, 4, 2, 6):                    # mfc1 mtc1 cfc1 ctc1
            g.add(rt)
            if rs in (0, 4):
                f.add(rd)
        elif rs >= 16:
            fn = w & 63
            f |= {rd, (w >> 6) & 31}
            if fn not in (4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 32, 33, 34, 35, 36, 37):
                f.add(rt)
    elif op in (0x31, 0x35, 0x39, 0x3D):
        g.add(rs)
        f.add(rt)
    elif op in (0x10, 0x12, 0x13):
        pass
    else:
        g |= {rs, rt}
    g.discard(0)
    return g, f


def reg_fields(w):
    """Register fields for substitution tallies: [(field, value)]."""
    op = w >> 26
    rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
    if op == 0:
        return [("rs", rs), ("rt", rt), ("rd", rd)]
    if op in (2, 3, 0x10, 0x11, 0x12, 0x13):
        return []
    return [("rs", rs), ("rt", rt)]


def gname(n):
    return GPR[n]


def fsum(words):
    g, f = Counter(), Counter()
    for w in words:
        a, b = regs_used(w)
        g.update(a)
        f.update(b)
    return g, f


def frame_info(words):
    """(frame bytes or 0, saved regs [(reg, off)], epilogue restore size)."""
    frame, saved = 0, []
    for w in words[:16]:
        if w >> 16 == 0x27BD and w & 0x8000:
            frame = 0x10000 - (w & 0xFFFF)
        elif (w >> 26) == 0x2B and ((w >> 21) & 31) == 29:
            r = (w >> 16) & 31
            if r in CALLEE_SAVED or r == 31:
                saved.append((gname(r), w & 0xFFFF))
    return frame, saved


# --- disassembly ---------------------------------------------------------------------------

_dis_cache = {}


def disasm(words):
    """{word: text}; one objdump run for all unseen words, per-word fallback, else ''."""
    need = sorted({w for w in words if w is not None and w not in _dis_cache})
    if need:
        done = False
        try:
            import tempfile
            with tempfile.NamedTemporaryFile(suffix=".bin") as f:
                f.write(b"".join(struct.pack(">I", w) for w in need))
                f.flush()
                out = score._run([score.OBJDUMP, "-D", "-b", "binary", "-m", "mips:4300", "-EB",
                                  f.name]).stdout
            seen = 0
            for line in out.splitlines():
                m = re.match(r"\s*([0-9a-f]+):\s+[0-9a-f]{8}\s+(.*)", line)
                if m:
                    i = int(m.group(1), 16) // 4
                    if i < len(need):
                        _dis_cache[need[i]] = re.sub(r"\s+", " ", m.group(2)).strip()
                        seen += 1
            done = seen == len(need)
        except (OSError, Exception):  # noqa: BLE001 (display only)
            done = False
        if not done:
            for w in need:
                if w not in _dis_cache:
                    try:
                        _dis_cache[w] = score.disasm_word(w)
                    except Exception:  # noqa: BLE001
                        _dis_cache[w] = ""
    return {w: _dis_cache.get(w, "") for w in words if w is not None}


def fmt(w, d):
    return "-" * 8 if w is None else f"{w:08x} {d.get(w, '')}".rstrip()


# --- sections -----------------------------------------------------------------------------------

def pct(a, b):
    return f"{100.0 * a / b:.0f}%" if b else "n/a"


def sec_summary(name, r, base, log):
    t = r["target_size"]
    lines = [f"## Score: `{name}`", "",
             f"- strict: **{'MATCH' if r.get('matched') else str(r['strict_diff']) + '/' + str(t) + ' words differ'}**"
             f" (size {r['size']} vs target {t}, extra nonzero words {r.get('extra', 0)})",
             f"- aligned: exact {r['aligned_exact']}/{t} ({pct(r['aligned_exact'], t)}), "
             f"opcode {r['aligned_opcode']} ({pct(r['aligned_opcode'], t)}), "
             f"opcode+reg {r['aligned_opcode_reg']} ({pct(r['aligned_opcode_reg'], t)})"]
    if r.get("unresolved"):
        lines.append(f"- unresolved symbols: {', '.join(r['unresolved'])}")
    if r.get("unverified"):
        lines.append(f"- unverified relocations: {len(r['unverified'])}")
    if r.get("err"):
        lines.append(f"- COMPILE ERROR: {r['err'].splitlines()[0][:200]}")
    if log:
        b = log.get("baseline") or {}
        bs = b if "strict_diff" in b else None
        if bs and log.get("kind") == "fn":
            lines.append(f"- search: {log['evals']} evals in {log['seconds']}s; start strict {bs['strict_diff']} / "
                         f"aligned {bs['aligned_exact']} -> best strict {r['strict_diff']} / aligned {r['aligned_exact']}; "
                         f"best mutation path: {log['best']['path'] or '(none, start is best)'}")
        else:
            lines.append(f"- search: {log['evals']} evals in {log['seconds']}s; best path: {log['best']['path'] or '(none)'}")
    return lines


def sec_frame(want, got):
    wf, ws = frame_info(want)
    gf, gs = frame_info(got)
    wg, wfp = fsum(want)
    gg, gfp = fsum(got)
    lines = ["", "## Frame and registers", "",
             f"- frame: target {wf} bytes, ours {gf} bytes" + ("" if wf == gf else f"  **(delta {gf - wf:+d})**"),
             f"- callee-saved stores in prologue: target {', '.join(f'{r}@{o}' for r, o in ws) or 'none'}; "
             f"ours {', '.join(f'{r}@{o}' for r, o in gs) or 'none'}"]

    def names(c):
        return " ".join(gname(n) for n in sorted(c))
    only_w, only_g = set(wg) - set(gg), set(gg) - set(wg)
    lines.append(f"- GPRs used: target [{names(wg)}]; ours [{names(gg)}]")
    lines.append(f"- target-only: [{names(only_w)}]  ours-only: [{names(only_g)}]")
    wsv = sorted(n for n in wg if n in CALLEE_SAVED)
    gsv = sorted(n for n in gg if n in CALLEE_SAVED)
    lines.append(f"- callee-saved regs used: target {len(wsv)} [{names(wsv)}], ours {len(gsv)} [{names(gsv)}]")
    if wfp or gfp:
        lines.append(f"- FPRs used: target [{' '.join('f%d' % n for n in sorted(wfp))}]; "
                     f"ours [{' '.join('f%d' % n for n in sorted(gfp))}]")
    # per-register use count drift (where the allocator differs most)
    drift = sorted(((abs(wg.get(n, 0) - gg.get(n, 0)), n) for n in set(wg) | set(gg)), reverse=True)[:5]
    drift = [(d, n) for d, n in drift if d]
    if drift:
        lines.append("- largest use-count differences: " + ", ".join(
            f"{gname(n)} {wg.get(n, 0)}->{gg.get(n, 0)}" for _, n in drift))
    # prologue / epilogue
    def ends(words):
        k = max((i for i, w in enumerate(words) if w == 0x03E00008), default=len(words) - 2)
        return words[max(0, k - 4):k + 2]
    wp, gp = want[:8], got[:8]
    we, ge = ends(want), ends(got)
    d = disasm(wp + gp + we + ge)
    lines += ["", "target prologue | ours prologue", "```"]
    for i in range(max(len(wp), len(gp))):
        a = fmt(wp[i], d) if i < len(wp) else ""
        b = fmt(gp[i], d) if i < len(gp) else ""
        lines.append(f"{a:34s}| {b}")
    lines += ["```", "", "target epilogue | ours epilogue", "```"]
    for i in range(max(len(we), len(ge))):
        a = fmt(we[i], d) if i < len(we) else ""
        b = fmt(ge[i], d) if i < len(ge) else ""
        lines.append(f"{a:34s}| {b}")
    lines.append("```")
    return lines


def sec_hunks(want, got, limit):
    hunks = aligned.diff_hunks(want, got)
    runs = aligned.group_hunks(hunks)
    kinds = Counter(h[2] for h in hunks)
    subs = Counter()
    opdiff = 0
    for t, c, kind, w, g in hunks:
        if kind != "sub":
            continue
        if aligned.shape(w) != aligned.shape(g):
            opdiff += 1
            continue
        for (fn_, a), (_, b) in zip(reg_fields(w), reg_fields(g)):
            if a != b:
                subs[(gname(a), gname(b))] += 1
    lines = ["", "## Aligned diff", "",
             f"{len(hunks)} differing words in {len(runs)} hunks: {kinds.get('sub', 0)} substitutions "
             f"({opdiff} change the opcode), {kinds.get('del', 0)} target words missing from ours, "
             f"{kinds.get('ins', 0)} extra words in ours."]
    if subs:
        lines.append("Register substitutions (target -> ours, count): " + ", ".join(
            f"{a}->{b} x{n}" for (a, b), n in subs.most_common(8)))
    if not hunks:
        return lines + ["", "No aligned differences."]
    need = []
    for run in runs:
        for t, c, kind, w, g in run:
            need += [w, g]
        t0 = run[0][0]
        if t0 is not None and t0 > 0:
            need.append(want[t0 - 1])
    d = disasm(need)
    lines += ["", "```", "- target only   + ours only   (index = word offset from function start)"]
    used = 0
    omitted = 0
    for ri, run in enumerate(runs):
        block = []
        t0 = run[0][0]
        if t0 is not None and t0 > 0:
            block.append(f"  [{t0 - 1:3d}]        {fmt(want[t0 - 1], d)}")
        for t, c, kind, w, g in run:
            if kind in ("sub", "del"):
                block.append(f"- [{t:3d}]        {fmt(w, d)}")
            if kind in ("sub", "ins"):
                block.append(f"+ [{c:3d}] ours   {fmt(g, d)}")
        if used + len(block) > limit:
            omitted += len(runs) - ri
            break
        used += len(block)
        lines += block + [""]
    if omitted:
        lines.append(f"... {omitted} more hunk(s) omitted (--lines {limit})")
    elif lines[-1] == "":
        lines.pop()
    lines.append("```")
    return lines


def _delta(e, b):
    sc = e["score"]
    if "aligned_exact" in sc:
        return (sc["aligned_exact"] - b["aligned_exact"], b["strict_diff"] - sc["strict_diff"])
    m = sc.get("members", {})
    bm = b.get("members", {})
    return (sum(d["aligned_exact"] for d in m.values()) - sum(d["aligned_exact"] for d in bm.values()),
            sum(d["strict_diff"] for d in bm.values()) - sum(d["strict_diff"] for d in m.values()))


def sec_tried(log, top=8):
    if not log:
        return ["", "## Mutations tried", "", "No search history (report built from a bare source)."]
    base = log.get("baseline")
    cands = [e for e in log["candidates"] if e.get("path") and e.get("score") and not e["score"].get("err")
             and not e.get("rejected")]
    total = len([e for e in log["candidates"] if e.get("path")])
    errs = len([e for e in log["candidates"] if e.get("score") and e["score"].get("err")])
    lines = ["", "## Mutations tried", "",
             f"{total} mutated candidates evaluated ({errs} compile errors). Deltas are relative to the start "
             f"(aligned_exact gained / strict words removed)."]
    if base and cands:
        scored = sorted(cands, key=lambda e: (-_delta(e, base)[0], -_delta(e, base)[1], len(e["path"])))
        lines += ["", "Best candidates:", ""]
        seen = set()
        n = 0
        for e in scored:
            key = tuple(e["path"])
            if key in seen:
                continue
            seen.add(key)
            da, ds = _delta(e, base)
            lines.append(f"- `{' + '.join(e['path'])}`: aligned {da:+d}, strict {ds:+d}")
            n += 1
            if n >= top:
                break
        single = [e for e in cands if len(e["path"]) == 1]
        if single:
            worse = sorted(single, key=lambda e: (_delta(e, base)[0], _delta(e, base)[1]))[:3]
            lines += ["", "Worst singles (for contrast): " + "; ".join(
                f"`{e['path'][0]}` {_delta(e, base)[0]:+d}" for e in worse)]
    cs = log.get("class_stats") or {}
    if cs:
        lines += ["", "| class | tried | improved | best gain | weight |", "|---|--:|--:|--:|--:|"]
        for c, v in sorted(cs.items(), key=lambda kv: (-kv[1]["improved"], -kv[1]["tried"]))[:14]:
            lines.append(f"| {c} | {v['tried']} | {v['improved']} | {v['best_delta']} | {v['weight']} |")
    return lines


def tried_classes(log):
    out = set()
    if log:
        out |= set((log.get("class_stats") or {}))
        for e in log.get("candidates", []):
            for p in e.get("path", []):
                out.add(re.split(r"[^\w\-]", p.split(":", 1)[-1] if re.match(r"^\w+\.c:", p) else p, 1)[0])
    return out


def sec_untried(log, best_src, fn, mutator=None):
    tried = tried_classes(log)
    lines = ["", "## Untried mutation classes", ""]
    applicable = Counter()
    try:
        if mutator is None:
            from amatch import mutate as mutator
        if best_src is not None:
            for m in mutator.mutations(best_src, fn):
                name = m["name"] if isinstance(m, dict) else getattr(m, "name", "?")
                applicable[re.split(r"[^\w\-]", str(name), 1)[0]] += 1
    except Exception:  # noqa: BLE001 (mutate.py may not exist yet)
        applicable = Counter()
    if applicable:
        un = {c: n for c, n in applicable.items() if c not in tried}
        if un:
            lines.append("Applicable to the current best source but never tried: " + ", ".join(
                f"{c} ({n} sites)" for c, n in sorted(un.items())))
        else:
            lines.append("Every mutation class with an applicable site on the best source was tried.")
        lines.append("Tried: " + (", ".join(sorted(tried)) or "none"))
    else:
        rest = [c for c in STATIC_CLASSES if c not in tried]
        lines.append("Mutation catalog unavailable or no applicable site; classes from the design list not in the "
                     "search log: " + ", ".join(rest))
    lines.append("Beyond the catalog (LLM judgement): restructure control flow, change a struct/array element type, "
                 "split or merge expressions, change a callee prototype, or decide the gap is IDO/as1 scheduling "
                 "(about 50 variants without movement on the same difference = stop, playbook rule 6).")
    return lines


# --- drivers -----------------------------------------------------------------------------------------

def fn_words(r):
    return list(r.get("words") or [])


def report_fn(src, fn, flags, log=None, lines=60, mutator=None):
    r = builder.compile_fn(src, fn, flags)
    out = [f"# Residual report: {fn}", ""]
    if r.get("err") and not r.get("words"):
        out += sec_summary(fn, r, None, log)
        out += sec_tried(log)
        return "\n".join(out) + "\n"
    want = builder.get_targets()[fn]
    got = fn_words(r)
    out += sec_summary(fn, r, None, log)
    out += sec_frame(want, got)
    out += sec_hunks(want, got, lines)
    out += sec_tried(log)
    out += sec_untried(log, src, fn, mutator)
    out.append(f"\nFlags: `{flags}`. Verify with: `python3 tools/cloud/score.py fn FILE {fn} --flags \"{flags}\"`")
    return "\n".join(out) + "\n"


def report_group(gdir, log=None, lines=60, mutator=None):
    gdir = Path(gdir)
    r = builder.compile_group(gdir)
    spec = json.loads((gdir / "group.json").read_text())
    out = [f"# Residual report: group {gdir.name}", ""]
    if r.get("err"):
        out.append(f"COMPILE ERROR: {r['err'].splitlines()[0][:200]}")
    targets = dict(builder.get_targets())
    targets.update(builder.extra_targets(spec.get("targets")))
    claims = set(spec.get("claims") or [])
    per = max(12, lines // max(1, len([m for m, d in r["members"].items() if not d.get("matched")])))
    for name, d in r["members"].items():
        flag = " (claimed)" if name in claims else ""
        if d.get("matched"):
            out += [f"## `{name}`{flag}: MATCH", ""]
            continue
        out += sec_summary(name + flag, d, None, None)
        if d.get("words") and name in targets:
            out += sec_frame(targets[name], d["words"])
            out += sec_hunks(targets[name], d["words"], per)
        out.append("")
    out += sec_tried(log)
    src = "\n".join((gdir / n).read_text() for n in spec["files"])
    out += sec_untried(log, src, None, mutator)
    return "\n".join(out) + "\n"


def report_dir(d, lines=60, mutator=None):
    d = Path(d)
    log = json.loads((d / "search_log.json").read_text())
    if log["kind"] == "fn":
        src = (d / "best.c").read_text()
        meta = log["meta"]
        return report_fn(src, meta["fn"], meta["flags"], log, lines, mutator)
    return report_group(d / "best_group", log, lines, mutator)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("args", nargs="+")
    ap.add_argument("--flags", default=None)
    ap.add_argument("--lines", type=int, default=60, help="max diff lines shown")
    a = ap.parse_args(argv)
    x = a.args
    if x[0] == "group" and len(x) == 2:
        text = report_group(x[1], None, a.lines)
    elif len(x) == 1 and (Path(x[0]) / "search_log.json").exists():
        text = report_dir(x[0], a.lines)
    elif len(x) == 1 and (Path(x[0]) / "group.json").exists():
        text = report_group(x[0], None, a.lines)
    elif len(x) == 2:
        src = Path(x[0]).read_text()
        flags = a.flags or builder.parse_flags_comment(src) or builder.DEFAULT_FLAGS
        text = report_fn(src, x[1], flags, None, a.lines)
    else:
        ap.error("give RESULT_DIR, FILE FUNC, or group DIR")
    sys.stdout.write(text)
    return 0


if __name__ == "__main__":
    sys.exit(main())
