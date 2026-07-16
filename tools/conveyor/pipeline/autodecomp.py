"""Automated decompilation seeding (no human in the loop).

For each unmatched function: decompile its own asm with mips_to_c into a
structurally-correct C seed (far better than a distant arcade candidate),
make it self-contained (inline the shim so the permuter's `cpp -nostdinc`
preprocess works), and submit a permuter_search. The farm then harvests:
score 0 -> matched -> lock/promote. This is the standard shipped-decomp loop
(m2c seed -> permuter -> match) wired into the conveyor pool.

    python3 -m tools.conveyor.pipeline.autodecomp seed [--limit N] [--budget S]
    python3 -m tools.conveyor.pipeline.autodecomp one <target_id>

Yield is partial by nature: scalar/clean functions match readily; struct-heavy
ones need better type context; some never converge. But it turns the entire
unmatched backlog into real, autonomous pool work.
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

from ..client import DEFAULT_DATA, Http, load_token
from ..coordinator import db as dbmod
from ..coordinator.store import BlobStore
from . import seeds as seedsmod
from . import farm as farmmod
from . import disasm as disasmmod

REPO = Path(__file__).resolve().parents[3]
ASM_DIR = REPO / "asm" / "us"
M2C = REPO / "tools" / "mips_to_c" / "m2c.py"
SHIM = REPO / "tools" / "conveyor" / "seeds" / "shim" / "conveyor_shim.h"
_GLABEL_RE = re.compile(r"^\s*glabel\s+(\S+)")

# Preprocessed type context for m2c: the OS headers parse cleanly (the game
# headers make m2c's parser assert). It gives m2c real struct layouts so
# struct-using functions type correctly and compile; it also carries the
# scalar typedefs, so a seed built from it is self-contained for the permuter's
# `cpp -nostdinc` preprocess (no separate shim needed).
_CTX_HEADERS = ["types.h"] + [f"PR/{p.name}" for p in
                              sorted((REPO / "include" / "PR").glob("*.h"))]
# LLM-grown struct/type context (autodecomp #1); included last so it can add
# globals and structs m2c needs. Regenerating the context picks up edits.
M2C_TYPES = REPO / "include" / "m2c_types.h"
_context_cache = None
_M2C_DIAGNOSTICS = {}


def _context():
    """(preprocessed_context_path, context_text), built once per process."""
    global _context_cache
    if _context_cache is not None:
        return _context_cache
    import tempfile
    head = "\n".join(f'#include "{h}"' for h in _CTX_HEADERS) + "\n"
    if M2C_TYPES.is_file():
        head += f'#include "{M2C_TYPES}"\n'
    src = Path(tempfile.mkdtemp(prefix="m2cctx-")) / "ctx.h"
    src.write_text(head)
    out = src.with_name("ctx.c")
    proc = subprocess.run(
        ["cpp", "-P", "-nostdinc", "-I", str(REPO / "include"),
         "-I", str(REPO / "include" / "PR"), "-D_LANGUAGE_C",
         str(src), "-o", str(out)],
        capture_output=True, text=True)
    text = out.read_text() if out.exists() and proc.returncode == 0 else ""
    _context_cache = (str(out) if text else None, text)
    return _context_cache


def _asm_index():
    """{glabel_name: asm_file} across asm/us/*.s (unconverted segments). Splat
    now emits canonical glabels (e.g. `glabel viGetTimeToDeadline`), so the
    target_id keys directly."""
    idx = {}
    for f in sorted(ASM_DIR.glob("*.s")):
        for line in f.read_text(errors="replace").splitlines():
            m = _GLABEL_RE.match(line)
            if m:
                idx.setdefault(m.group(1), f)
    return idx


# Macros the preprocessed context drops (cpp consumed them) but m2c output uses.
_PRELUDE = "#define NULL ((void *)0)\n#define TRUE 1\n#define FALSE 0\n"


def _clean_m2c(body):
    """Strip m2c's mechanical output warts so the seed compiles: diagnostic
    lines that leak into stdout, and `?` placeholders m2c emits for parameter
    types it couldn't infer (default them to s32 — the permuter fixes the rest;
    ternary `?` is left alone)."""
    lines = [l for l in body.splitlines()
             if not l.lstrip().startswith(("Warning:", "Error:", "GLOBAL_ASM"))]
    text = "\n".join(lines)
    # `(? arg` / `, ? arg` (an unknown parameter type) -> s32
    text = re.sub(r"([(,]\s*)\?(\s+\w)", r"\1s32\2", text)
    return text


def m2c_seed(target_id, vaddr, asm_idx, diagnostics=None):
    """Self-contained C seed for a target from its own asm, or None if m2c
    can't decompile it (missing asm / failure)."""
    asm_file = asm_idx.get(target_id) or asm_idx.get(f"func_{vaddr:08X}")
    if asm_file is None:
        return None
    ctx_path, ctx_text = _context()
    cmd = [sys.executable, str(M2C), str(asm_file), "-f", target_id]
    if ctx_path:
        cmd += ["--context", ctx_path]
    proc = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    if diagnostics is not None:
        diagnostics[target_id] = proc.stderr
    body = _clean_m2c(proc.stdout.strip())
    if proc.returncode != 0 or not body or "def " in body[:20]:
        return None
    if ctx_text:
        # Self-contained via the context (has scalar types + OS structs). Drop
        # the target's own prototype from the context so it doesn't conflict
        # with m2c's definition; C89 needs no prototypes for the rest.
        proto = re.compile(rf"^[^\n]*\b{re.escape(target_id)}\s*\([^;{{]*\)\s*;",
                           re.M)
        prelude = proto.sub("", ctx_text)
        return _PRELUDE + prelude + "\n" + body + "\n"
    # Fallback: minimal shim (scalar types only).
    return _PRELUDE + SHIM.read_text() + "\n" + body + "\n"


def _resolve_targets(conn, population, specification):
    """Resolve an id list or @file, rejecting unknown/conflicted targets."""
    if not specification:
        return None
    if specification.startswith("@"):
        path = Path(specification[1:])
        try:
            names = [line.strip() for line in path.read_text().splitlines()
                     if line.strip()]
        except OSError as exc:
            sys.exit(f"cannot read targets {path}: {exc}")
    else:
        names = [name.strip() for name in specification.split(",") if name.strip()]
    if not names:
        sys.exit("no targets specified")
    if len(set(names)) != len(names):
        names = list(dict.fromkeys(names))
    placeholders = ",".join("?" for _ in names)
    rows = conn.execute(
        f"SELECT target_id,population,gate_reason FROM n64_target"
        f" WHERE target_id IN ({placeholders})", names,
    ).fetchall()
    found = {row["target_id"]: row for row in rows}
    for name in names:
        row = found.get(name)
        if row is None or row["population"] != population:
            sys.exit(f"unknown {population} target: {name}")
        gate = row["gate_reason"] or ""
        if gate.startswith("extent_conflict"):
            sys.exit(f"refusing {name}: {gate}")
    return names


def _population_rows(conn, population, targets, limit):
    """Select seedable rows; the default static SQL remains the legacy query."""
    if population == "static" and targets is None:
        return conn.execute(
            "SELECT t.target_id, t.address FROM n64_target t"
            " JOIN function_status f USING (target_id)"
            " WHERE f.status IN ('unmatched','seeded') AND t.target_o_sha IS NOT NULL"
            " AND t.population='static' AND t.insn_count IS NOT NULL"
            " ORDER BY t.insn_count LIMIT ?", (limit,)).fetchall()
    parameters = [population]
    where = ["f.status IN ('unmatched','seeded')", "t.target_o_sha IS NOT NULL",
             "t.population=?", "t.insn_count IS NOT NULL"]
    if targets:
        where.append("t.target_id IN (" + ",".join("?" for _ in targets) + ")")
        parameters.extend(targets)
    sql = (
        "SELECT t.target_id,t.address,t.gate_reason FROM n64_target t"
        " JOIN function_status f USING (target_id) WHERE " + " AND ".join(where)
        + " ORDER BY t.insn_count"
    )
    if limit:
        sql += " LIMIT ?"
        parameters.append(limit)
    rows = conn.execute(sql, parameters).fetchall()
    if targets:
        by_name = {row["target_id"]: row for row in rows}
        return [by_name[name] for name in targets if name in by_name]
    return rows


def _asm_for_rows(conn, population, rows):
    if population == "static":
        return _asm_index()
    return {row["target_id"]: disasmmod.derive(conn, row["target_id"])
            for row in rows}


def submit_one(conn, store, http, toolkit_sha, target_id, vaddr, asm_idx,
               budget_seconds):
    seed = m2c_seed(target_id, vaddr, asm_idx, diagnostics=_M2C_DIAGNOSTICS)
    if seed is None:
        return "no_seed"
    try:
        bundle, m_sha, job = seedsmod.build_search_bundle(
            conn, store, target_id, seed,
            farmmod._flagset_for(conn, target_id), toolkit_sha,
            budget={"wall_seconds": budget_seconds, "iterations": None})
    except KeyError:
        return "no_target_o"
    _, out = http.call("POST", "/api/v1/blobs", raw=bundle.read_bytes())
    job.update(bundle_sha=out["sha256"], target_id=target_id, priority=30)
    http.call("POST", "/api/v1/work", body=[job])
    with dbmod.tx(conn):
        conn.execute(
            "UPDATE function_status SET seed_kind='m2c',"
            " updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now') WHERE target_id=?",
            (target_id,))
        farmmod._set_status(conn, target_id, "in_search")
    return "seeded"


def _conn(data):
    d = Path(data)
    return dbmod.connect(d / "conveyor.db"), BlobStore(d / "blobs")


ROM_AUTO = REPO / "src" / "rom_auto"


def _extract_fn(m2c_out, target_id):
    """Just the target's function definition (drop m2c's extern preamble), so
    it can rely on rom_auto.h for declarations instead of m2c's guessed ones
    (which conflict with the real headers)."""
    m = re.search(rf"^[A-Za-z_][\w ,*]*\b{re.escape(target_id)}\s*\([^;{{]*\)\s*\{{",
                  m2c_out, re.M)
    if not m:
        return None
    i, depth = m.end() - 1, 0
    for j in range(m.end() - 1, len(m2c_out)):
        if m2c_out[j] == "{":
            depth += 1
        elif m2c_out[j] == "}":
            depth -= 1
            if depth == 0:
                return m2c_out[m.start():j + 1]
    return None


def emit_src(target_id, vaddr, asm_idx):
    """Clean, promotable C source for a matched function: rom_auto.h (all real
    types) + the m2c function body. Returns text, or None if m2c can't emit it."""
    asm_file = asm_idx.get(target_id) or asm_idx.get(f"func_{vaddr:08X}")
    if asm_file is None:
        return None
    ctx_path, _ = _context()
    cmd = [sys.executable, str(M2C), str(asm_file), "-f", target_id]
    if ctx_path:
        cmd += ["--context", ctx_path]
    proc = subprocess.run(cmd, capture_output=True, text=True, timeout=120)
    if proc.returncode != 0:
        return None
    fn = _extract_fn(_clean_m2c(proc.stdout), target_id)
    if not fn:
        return None
    return f'/* Auto-decompiled by mips_to_c (autodecomp). */\n#include "rom_auto.h"\n\n{fn}\n'


def cmd_lockmatches(args):
    """Emit a clean promotable source for every matched-but-unlocked function
    and lock it (re-verify score 0 on the pool). Turns auto-decomp matches into
    real, checked-in, promotable sources."""
    import json
    from . import lock as lockmod
    conn, _ = _conn(args.data)
    asm_idx = _asm_index()
    ROM_AUTO.mkdir(parents=True, exist_ok=True)
    locked = {e.get("target_id") for e in lockmod.load_lock().values()}
    rows = conn.execute(
        "SELECT f.target_id, t.address FROM function_status f"
        " JOIN n64_target t USING (target_id)"
        " WHERE f.status='matched' AND t.population='static'"
        " ORDER BY t.insn_count").fetchall()
    todo = [r for r in rows if r["target_id"] not in locked]
    print(f"{len(todo)} matched-unlocked functions to emit+lock")
    ok, emitfail, lockfail = [], [], []
    for r in todo:
        tid = r["target_id"]
        src = emit_src(tid, r["address"], asm_idx)
        if not src:
            emitfail.append(tid)
            continue
        path = ROM_AUTO / f"{tid}.c"
        path.write_text(src)
        flag = farmmod._flagset_for(conn, tid)
        res = subprocess.run(
            [sys.executable, "-m", "tools.conveyor.pipeline.lock", "add",
             f"src/rom_auto/{tid}.c:{tid}", "--flags", flag, "--wait", "180"],
            cwd=REPO, capture_output=True, text=True, timeout=240)
        if "score0" in res.stdout or "(score0)" in res.stdout:
            ok.append(tid)
        else:
            lockfail.append((tid, res.stdout.strip().splitlines()[-1] if res.stdout.strip() else "?"))
            path.unlink(missing_ok=True)
    print(f"locked {len(ok)}: {sorted(ok)}")
    if emitfail:
        print(f"emit-failed {len(emitfail)}: {sorted(emitfail)}")
    if lockfail:
        print(f"lock-failed {len(lockfail)}:")
        for t, why in lockfail[:12]:
            print(f"  {t}: {why[:70]}")


CLUSTERS_MD = REPO / "build" / "m2c_clusters.md"
ARCADE = REPO / "reference" / "repos" / "rushtherock"
_MEMBER_RE = re.compile(r"\b([A-Za-z_]\w*)\s*->")
_BEFORE_RE = re.compile(r"before ['`]([A-Za-z_*]\w*)")
# tokens that are never a missing type: C keywords, our scalar types, and
# m2c's local-variable / register naming conventions.
_KNOWN = set("void int char short long float double signed unsigned const"
             " struct union enum static extern return if else while for do"
             " goto switch case default break continue sizeof typedef".split())
_KNOWN |= {"u8", "s8", "u16", "s16", "u32", "s32", "u64", "s64", "f32", "f64",
           "vu8", "vs8", "vu16", "vs16", "vu32", "vs32", "vf32", "OSTime",
           "UNK_TYPE", "UNK_RET"}
_LOCAL = re.compile(r"^(var_|arg\d|sp[0-9A-Fa-f]+|temp_|phi_|f\d+$|v[01]$|"
                    r"a[0-3]$|t\d$|s\d$|ret|pad|dummy|D_[0-9A-Fa-f]+$)")


_NOISE = {"unaligned", "bitwise", "aka", "note", "unk", "incomplete", "type",
          "value", "token", "declaration", "specifiers", "identifier"}


def _is_typeish(tok):
    return (tok and len(tok) >= 4 and tok not in _KNOWN and tok not in _NOISE
            and not _LOCAL.match(tok) and re.match(r"^[A-Za-z_]\w*$", tok)
            and not tok[0].isdigit())


def _seed_compile_errors(seed):
    """(ok, [(missing_type, source_line)]) — compile a seed and, on failure,
    best-effort the undefined *type* behind each error (not the local var)."""
    import tempfile
    f = Path(tempfile.mktemp(suffix=".c")); f.write_text(seed)
    pp = subprocess.run(["cpp", "-P", "-nostdinc", "-DPERMUTER", str(f)],
                        capture_output=True, text=True)
    if pp.returncode != 0:
        return False, []
    src_lines = pp.stdout.splitlines()
    g = Path(tempfile.mktemp(suffix=".c")); g.write_text(pp.stdout)
    cc = subprocess.run(["mips-linux-gnu-gcc", "-c", "-fsyntax-only",
                         "-std=gnu89", str(g)], capture_output=True, text=True)
    if cc.returncode == 0:
        return True, []
    out, seen = [], set()
    for el in cc.stderr.splitlines():
        m = re.search(r":(\d+):\d+: error: (.+)", el)
        if not m:
            continue
        ln, msg = int(m.group(1)), m.group(2)
        srcline = src_lines[ln - 1].strip() if 1 <= ln <= len(src_lines) else ""
        tok = None
        if "not a structure" in msg or "has no member" in msg:
            mm = _MEMBER_RE.search(srcline)          # `foo->bar`  -> foo
            tok = mm.group(1) if mm else None
        elif "undeclared" in msg or "unknown type" in msg:
            mm = re.search(r"['`]([A-Za-z_]\w*)'", msg)
            tok = mm.group(1) if mm else None
        else:
            # parse error "... before 'X'": the missing type is the identifier
            # on the source line just before X.
            mm = _BEFORE_RE.search(msg)
            after = mm.group(1) if mm else None
            words = re.findall(r"[A-Za-z_]\w*", srcline)
            if after and after in words:
                i = words.index(after)
                tok = words[i - 1] if i > 0 else None
            if not _is_typeish(tok):
                tok = next((w for w in words if _is_typeish(w)), None)
        if not _is_typeish(tok) or tok in seen:
            continue
        seen.add(tok)
        out.append((tok, srcline[:100]))
    return False, out


def cmd_clusters(args):
    """Run m2c across the unmatched backlog, and rank the undefined
    types/symbols blocking compilation by how many functions each blocks —
    the LLM's worklist (autodecomp #1). Writes an evidence report."""
    conn, _ = _conn(args.data)
    targets = _resolve_targets(conn, args.population, args.targets)
    rows = _population_rows(conn, args.population, targets, args.limit)
    asm_idx = _asm_for_rows(conn, args.population, rows)
    from collections import defaultdict
    clusters = defaultdict(lambda: {"fns": set(), "lines": []})
    compiled = attempted = 0
    for r in rows:
        seed = m2c_seed(r["target_id"], r["address"], asm_idx,
                        diagnostics=_M2C_DIAGNOSTICS)
        if not seed:
            continue
        attempted += 1
        ok, errs = _seed_compile_errors(seed)
        if ok:
            compiled += 1
            continue
        for tok, srcline in errs:
            cl = clusters[tok]
            cl["fns"].add(r["target_id"])
            if srcline and srcline not in cl["lines"] and len(cl["lines"]) < 4:
                cl["lines"].append(srcline)
    ranked = sorted(clusters.items(), key=lambda kv: -len(kv[1]["fns"]))
    CLUSTERS_MD.parent.mkdir(parents=True, exist_ok=True)
    lines = [f"# m2c compile-failure clusters",
             f"\nattempted={attempted} compiled={compiled} "
             f"({100 * compiled // max(attempted, 1)}%) "
             f"blocked={attempted - compiled}\n",
             "Each row is an undefined type/symbol; define it in "
             "`tools/conveyor/seeds/m2c_types.h` to unlock its functions.\n"]
    for tok, cl in ranked[:args.top]:
        arc = _arcade_hint(tok)
        lines.append(f"\n## `{tok}` — blocks {len(cl['fns'])} functions")
        lines.append(f"functions: {', '.join(sorted(cl['fns'])[:12])}")
        for sl in cl["lines"]:
            lines.append(f"    {sl}")
        if arc:
            lines.append(f"arcade ref: {arc}")
    CLUSTERS_MD.write_text("\n".join(lines) + "\n")
    print(f"attempted={attempted} compiled={compiled} "
          f"({100 * compiled // max(attempted, 1)}%); "
          f"top blockers -> {CLUSTERS_MD}")
    for tok, cl in ranked[:min(args.top, 12)]:
        print(f"  {len(cl['fns']):3}  {tok}")


NEARMISS_DIR = REPO / "work" / "nearmiss"


def cmd_nearmiss(args):
    """Package the near-miss functions (permuter got close but didn't reach 0)
    for an LLM to close: the current best source + the target disassembly it
    must match, per function. These are usually one instruction / cast / order
    away — cheap, high-yield human/LLM work (autodecomp #2)."""
    import json
    conn, store = _conn(args.data)
    binary = _objdump()
    rows = conn.execute(
        "SELECT target_id, result_sha FROM work_unit"
        " WHERE job_type='permuter_search' AND state='DONE'"
        " AND result_sha IS NOT NULL ORDER BY created_at DESC").fetchall()
    seen, items = set(), []
    for r in rows:
        tid = r["target_id"]
        if not tid or tid in seen:
            continue
        seen.add(tid)
        result, artifacts = farmmod._read_result(store, r["result_sha"])
        if not result or result.get("exit") != "ok":
            continue
        best = result.get("payload", {}).get("final_best_score")
        if best is None or not (1 <= best <= args.max):
            continue
        st = conn.execute("SELECT status FROM function_status WHERE target_id=?",
                          (tid,)).fetchone()
        if st and st["status"] in ("matched", "verified"):
            continue
        tsha = conn.execute("SELECT target_o_sha FROM n64_target WHERE target_id=?",
                            (tid,)).fetchone()
        src = (artifacts.get("best.c") or b"").decode(errors="replace")
        tasm = ""
        if tsha and tsha["target_o_sha"]:
            blob = store.get(tsha["target_o_sha"])
            if blob:
                tasm = subprocess.run([binary, "-dr", str(blob)],
                                      capture_output=True, text=True).stdout
        d = NEARMISS_DIR / tid
        d.mkdir(parents=True, exist_ok=True)
        (d / "source.c").write_text(src)
        (d / "target.asm").write_text(tasm)
        (d / "flags.txt").write_text(farmmod._flagset_for(conn, tid))
        items.append((tid, best))
    items.sort(key=lambda x: x[1])
    print(f"packaged {len(items)} near-misses (best 1-{args.max}) -> {NEARMISS_DIR}")
    for tid, best in items:
        print(f"  best={best:>4}  {tid}")


def _objdump():
    from ..jobs import scoring
    return scoring._objdump_path()


def _arcade_hint(token):
    """First arcade/reference line that defines or declares the token."""
    if not ARCADE.is_dir() or len(token) < 3:
        return None
    try:
        r = subprocess.run(
            ["grep", "-rIhE", "-m1",
             rf"(struct|typedef|extern|#define).*\b{re.escape(token)}\b",
             str(ARCADE)], capture_output=True, text=True, timeout=20)
        return r.stdout.strip().splitlines()[0][:100] if r.stdout.strip() else None
    except Exception:
        return None


AUTO_WORK = REPO / "work" / "auto"


def cmd_harvest(args):
    """Direct, robust replacement for the farm's flaky harvest: scan finished
    permuter searches, and for every score-0 hit save its best.c and mark the
    function matched. No verify_promote / cluster fan-out (the parts that
    stalled the farm) — just bank the win so nothing is stranded."""
    conn, store = _conn(args.data)
    asm_idx = _asm_index()
    seen, banked, base0 = set(), [], []
    rows = conn.execute(
        "SELECT target_id, address, result_sha FROM work_unit"
        " JOIN n64_target USING (target_id)"
        " WHERE job_type='permuter_search' AND state='DONE'"
        " AND result_sha IS NOT NULL ORDER BY created_at DESC").fetchall()
    for r in rows:
        tid = r["target_id"]
        if not tid or tid in seen:
            continue
        seen.add(tid)
        result, artifacts = farmmod._read_result(store, r["result_sha"])
        if not result or result.get("exit") != "ok":
            continue
        pl = result.get("payload", {})
        if pl.get("final_best_score") != 0:
            continue
        st = conn.execute("SELECT status FROM function_status WHERE target_id=?",
                          (tid,)).fetchone()
        if st and st["status"] in ("matched", "verified"):
            continue
        best_c = artifacts.get("best.c")
        is_base0 = not best_c
        if is_base0:
            # base=0: m2c's own seed compiled byte-identical (no permuter step,
            # so no best.c). m2c is deterministic — regenerate the exact seed
            # that scored 0 and bank that.
            seed = m2c_seed(tid, r["address"], asm_idx)
            if not seed:
                continue
            best_c = seed.encode()
        d = AUTO_WORK / tid
        d.mkdir(parents=True, exist_ok=True)
        (d / "matched.c").write_bytes(best_c)
        with dbmod.tx(conn):
            conn.execute(
                "UPDATE function_status SET status='matched', best_score=0,"
                " seed_kind='m2c', updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now')"
                " WHERE target_id=?", (tid,))
        (base0 if is_base0 else banked).append(tid)
    print(f"harvested {len(banked) + len(base0)} score-0 -> {AUTO_WORK}")
    if banked:
        print(f"  permuter wins ({len(banked)}): {sorted(banked)}")
    if base0:
        print(f"  m2c direct-hit / base-0 ({len(base0)}): {sorted(base0)}")


def cmd_seed(args):
    conn, store = _conn(args.data)
    http = Http(args.coordinator, load_token(args.token, args.data))
    toolkit_sha = http.pinned_toolkit()
    targets = _resolve_targets(conn, args.population, args.targets)
    rows = _population_rows(conn, args.population, targets, args.limit)
    asm_idx = _asm_for_rows(conn, args.population, rows)
    counts = {}
    for r in rows:
        outcome = submit_one(conn, store, http, toolkit_sha, r["target_id"],
                             r["address"], asm_idx, args.budget)
        counts[outcome] = counts.get(outcome, 0) + 1
        if outcome == "seeded" and args.verbose:
            print(f"  seeded {r['target_id']}")
    print("autodecomp seed: " + "  ".join(f"{k}={v}" for k, v in sorted(counts.items())))


def cmd_one(args):
    conn, store = _conn(args.data)
    http = Http(args.coordinator, load_token(args.token, args.data))
    specification = args.targets or args.target
    targets = _resolve_targets(conn, args.population, specification)
    if not targets or len(targets) != 1:
        sys.exit("one requires exactly one target")
    rows = _population_rows(conn, args.population, targets, 1)
    if not rows:
        sys.exit(f"target is not seedable: {targets[0]}")
    row = rows[0]
    asm_idx = _asm_for_rows(conn, args.population, rows)
    outcome = submit_one(conn, store, http, http.pinned_toolkit(), targets[0],
                        row["address"], asm_idx, args.budget)
    print(f"{targets[0]}: {outcome}")


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--data", default=str(DEFAULT_DATA))
    p.add_argument("--coordinator", default="http://127.0.0.1:8323")
    p.add_argument("--token", default=None)
    sub = p.add_subparsers(dest="command", required=True)
    s = sub.add_parser("seed")
    s.add_argument("--limit", type=int, default=200)
    s.add_argument("--budget", type=int, default=1200)
    s.add_argument("--verbose", action="store_true")
    s.add_argument("--population", choices=("static", "extracted"),
                   default="static")
    s.add_argument("--targets")
    s.set_defaults(func=cmd_seed)
    s = sub.add_parser("one")
    s.add_argument("target", nargs="?")
    s.add_argument("--budget", type=int, default=1200)
    s.add_argument("--population", choices=("static", "extracted"),
                   default="static")
    s.add_argument("--targets")
    s.set_defaults(func=cmd_one)
    s = sub.add_parser("harvest")
    s.set_defaults(func=cmd_harvest)
    s = sub.add_parser("clusters")
    s.add_argument("--limit", type=int, default=250)
    s.add_argument("--top", type=int, default=25)
    s.add_argument("--population", choices=("static", "extracted"),
                   default="static")
    s.add_argument("--targets")
    s.set_defaults(func=cmd_clusters)
    s = sub.add_parser("lockmatches")
    s.set_defaults(func=cmd_lockmatches)
    s = sub.add_parser("nearmiss")
    s.add_argument("--max", type=int, default=80)
    s.set_defaults(func=cmd_nearmiss)
    args = p.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
