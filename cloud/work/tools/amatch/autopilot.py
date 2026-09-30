#!/usr/bin/env python3
"""autopilot.py: batch driver that lets local compute do the matching and hands LLMs a SMALL worklist (stdlib only).

    autopilot.py [--top N] [--class abi|ipa|all] [--fn NAME ...] [--budget N] [--seconds S] [--jobs J]
                 [--parallel P] [--seed S] [--redo] [--write] [--json]
    autopilot.py --bench [--limit N] [--fn NAME ...] [--budget N]     corpus benchmark through this machinery
    autopilot.py --selftest                                           3 known-solvable corpus functions, end to end
    autopilot.py --report-only                                        regenerate report.md / worklist.json from state

Pipeline per worklist item (triage.py ranks them; or name them with --fn):
  ABI single : seed = best of {cloud/work near-miss/draft sources, private patched m2c (groupgen), flag sweep
               -O1/-O2/-O3}; if the seed already matches, done; else search.py (mutation search, --budget evals);
               a hit is re-verified with the real `tools/cloud/score.py fn`.
  IPA item   : seed = the drafted group it belongs to (copied) or groupgen.make_plan([fn]) + emit_and_compile;
               search.py group; a hit is re-verified with `score.py group --claims`.  Claims = members that score
               a strict MATCH (stand-ins are never claimed).
Nothing in the repo is written unless --write is given: results go to build/autopilot/out/
(matches/<fn>.c with the `/* flags: .. */` first line, groups/<name>/ with group.json claims updated).
--write then puts ABI matches in cloud/matches/<fn>.c (never overwriting) and groups into cloud/work/ipa-groups/<name>/.

Resumable: build/autopilot/state.json records every finished item (matched / needs_llm / error); a rerun skips them
unless --redo.  Outputs: build/autopilot/report.md (matched list + a capped residual per unfinished item: score,
frame/register summary, aligned diff hunks only) and build/autopilot/worklist.json (the needs_llm items with a
`shape` classified from the score: register_only / immediate_or_scheduling / frame_mismatch / structural_rewrite /
extra_or_missing_insns / compile_error / ipa_context_missing / ...) and a machine `suggest`.
"""
import argparse
import concurrent.futures as cf
import json
import os
import re
import shutil
import statistics
import subprocess
import sys
import tempfile
import threading
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(HERE.parent))
sys.path.insert(0, str(ROOT / "tools" / "cloud"))

AP_DIR = Path(os.environ.get("AUTOPILOT_DIR", ROOT / "build" / "autopilot"))
SEARCH_PY = HERE / "search.py"
SCORE_PY = ROOT / "tools" / "cloud" / "score.py"
DEFAULT_FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"
FINAL = ("matched", "needs_llm")
# Corpus functions that search.py solves from their first-attempt source quickly (checked by --selftest on IDO hosts;
# if one stops solving, the selftest falls back to the first corpus singles with a small edit ratio).
SELFTEST_FNS = ["Input_SetAnalogBounds", "func_800A7D6C", "struct_init_and_call", "transmission_shift",
                "func_800A61B0"]
SELFTEST_NEED = 3


# --- score helpers (pure) -------------------------------------------------------------------------------------

def score_key(r):
    """Lower is better."""
    if not r or (r.get("err") and not r.get("words")):
        return (1, 1, 10 ** 9, 10 ** 9)
    return (0 if r.get("matched") else 1, -r.get("aligned_exact", 0), r.get("strict_diff", 10 ** 9),
            abs(r.get("size", 0) - r.get("target_size", 0)))


def with_opt(flags, opt):
    return re.sub(r"-O\d", opt, flags) if re.search(r"-O\d", flags) else flags + " " + opt


def frame_of(words):
    for w in words[:16]:
        if w >> 16 == 0x27BD and w & 0x8000:
            return 0x10000 - (w & 0xFFFF)
    return 0


SUGGEST = {
    "compile_error": ("llm", "seed does not compile or define the function: fix declarations/types by hand"),
    "unresolved_symbols": ("llm", "externs/callees unresolved: declare them with the right types in the TU"),
    "relocation_unverified": ("llm", "words match but relocations are unverified: define the data symbol locally/check types"),
    "ipa_context_missing": ("llm", "group does not build or a callee/caller is missing: extend the group (deps.py closure)"),
    "structural_rewrite": ("llm", "size/opcode stream far from target: rewrite the function structure from the asm/arcade source"),
    "extra_or_missing_insns": ("llm", "opcodes mostly aligned but instruction count differs: locals/inlining/loop peel, then rerun search"),
    "frame_mismatch": ("machine+llm", "frame size differs: pad-local/volatile pad, named-local count; rerun search with more budget"),
    "register_only": ("machine", "same opcodes, registers differ: rerun with larger --budget/--seed, probe.py, decl-order permutation"),
    "immediate_or_scheduling": ("machine+llm", "opcode+reg align but words differ: constants/offsets/delay-slot scheduling; check types and field offsets"),
    "near_miss": ("machine", "few words differ: rerun with other seeds; if 50 variants do not move it, it is IDO/as1 (playbook rule 6)"),
    "partial_structure": ("llm", "about half the opcode stream aligns: fix control-flow shape, then search"),
}


def classify_shape(r, want=None, ipa=False):
    """Classify an unmatched score dict into (shape, suggest_who, hint).  Pure function (unit tested)."""
    err = (r or {}).get("err")
    if not r or (err and not r.get("words")):
        shape = "ipa_context_missing" if ipa else "compile_error"
        if err and re.search(r"undefined|unresolved|no target", err):
            shape = "ipa_context_missing" if ipa else "unresolved_symbols"
        return (shape,) + SUGGEST[shape]
    t = max(1, r.get("target_size") or r.get("target_words") or 1)
    size = r.get("size", 0)
    ex, op, orr = r.get("aligned_exact", 0) / t, r.get("aligned_opcode", 0) / t, r.get("aligned_opcode_reg", 0) / t
    if r.get("strict_diff", 1) == 0 and not r.get("extra"):
        shape = "relocation_unverified" if (r.get("unverified") or r.get("unresolved")) else "near_miss"
    elif r.get("unresolved") and ex < 0.9:
        shape = "ipa_context_missing" if ipa else "unresolved_symbols"
    elif abs(size - t) / t > 0.25 or op < 0.6:
        shape = "structural_rewrite"
    elif size != t and op >= 0.85:
        shape = "extra_or_missing_insns"
    elif want and r.get("words") and frame_of(want) != frame_of(r["words"]):
        shape = "frame_mismatch"
    elif ex >= 0.9:
        shape = "near_miss"
    elif op >= 0.9 and orr < 0.9:
        shape = "register_only"
    elif orr >= 0.9:
        shape = "immediate_or_scheduling"
    elif op >= 0.75:
        shape = "register_only" if orr < op else "immediate_or_scheduling"
    else:
        shape = "partial_structure"
    return (shape,) + SUGGEST[shape]


def with_flags_line(src, flags):
    body = re.sub(r"\A\s*/\*\s*flags:.*?\*/[ \t]*\n?", "", src, count=1)
    return "/* flags: %s */\n%s" % (flags, body.lstrip("\n") if body.startswith("\n") else body)


def compact_residual(text, max_lines=60):
    """Keep only the Score, Frame and Aligned diff sections of a report.py report, capped."""
    keep, cur = [], None
    for ln in text.splitlines():
        if ln.startswith("## "):
            cur = ln[3:].split(":")[0].strip()
        if cur and (cur.startswith("Score") or cur.startswith("Frame") or cur.startswith("Aligned diff")):
            keep.append(ln)
    out = []
    for ln in keep:          # drop blank runs
        if ln.strip() or (out and out[-1].strip()):
            out.append(ln)
    if len(out) > max_lines:
        cut = len(out) - max_lines
        out = out[:max_lines] + ["... (%d more lines cut; run report.py on the result dir)" % cut]
    return "\n".join(out)


# --- default collaborators (each replaceable in tests) ------------------------------------------------------------

def _last_json(text):
    for ln in reversed(text.strip().splitlines()):
        ln = ln.strip()
        if ln.startswith("{"):
            try:
                return json.loads(ln)
            except ValueError:
                pass
    return None


def search_cli(kind, path, fn, flags, budget, seconds, jobs, seed, out, timeout):
    """Run amatch/search.py as a subprocess; returns its --json result dict (or {"error": ..})."""
    if kind == "fn":
        cmd = [sys.executable, str(SEARCH_PY), "fn", str(path), fn, "--flags", flags]
    else:
        cmd = [sys.executable, str(SEARCH_PY), "group", str(path)]
    cmd += ["--budget", str(budget), "--jobs", str(jobs), "--seed", str(seed), "--out", str(out), "--json", "--quiet"]
    if seconds:
        cmd += ["--seconds", str(seconds)]
    try:
        p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return {"matched": False, "error": "timeout"}
    r = _last_json(p.stdout)
    if r is None:
        return {"matched": False, "error": "no JSON (rc=%s) %s" % (p.returncode, (p.stderr or p.stdout)[-300:])}
    return r


def verify_fn_real(src, fn, flags):
    with tempfile.TemporaryDirectory(prefix="ap-v-") as tmp:
        p = Path(tmp) / "src.c"
        p.write_text(src)
        r = subprocess.run([sys.executable, str(SCORE_PY), "fn", str(p), fn, "--flags", flags],
                           capture_output=True, text=True, cwd=ROOT)
    return r.returncode == 0, (r.stdout + r.stderr)[-400:]


def verify_group_real(gdir, claims):
    """score.py group --claims on a copy whose group.json lists exactly `claims`."""
    gdir = Path(gdir)
    spec = json.loads((gdir / "group.json").read_text())
    if spec.get("targets"):
        return None, "group has extracted-head targets: score.py cannot verify (builder strict score only)"
    with tempfile.TemporaryDirectory(prefix="ap-vg-") as tmp:
        d = Path(tmp) / "g"
        shutil.copytree(gdir, d)
        spec["claims"] = list(claims)
        (d / "group.json").write_text(json.dumps(spec))
        r = subprocess.run([sys.executable, str(SCORE_PY), "group", str(d), "--claims"],
                           capture_output=True, text=True, cwd=ROOT)
    return r.returncode == 0 and bool(claims), (r.stdout + r.stderr)[-400:]


_m2c_lock = threading.Lock()
_sm_cache = {}


def m2c_seed(fn):
    """(TU text, info) for one function from the private patched m2c (as ipakit/groupgen.py builds its seeds)."""
    from ipakit import deps, groupgen, sigs
    with _m2c_lock:
        dm = deps.model_cache()
        if "sm" not in _sm_cache:
            _sm_cache["sm"] = sigs.SigModel(dm.corpus, dm.infos)
        plan = groupgen.make_plan([fn], dmodel=dm)
        text, info = groupgen.build_sources(plan, dm, _sm_cache["sm"])
    return text, info


def gen_group(fn, gdir):
    """groupgen.make_plan([fn]) + emit_and_compile into gdir.  Returns info dict."""
    from ipakit import deps, groupgen, sigs
    with _m2c_lock:
        dm = deps.model_cache()
        if "sm" not in _sm_cache:
            _sm_cache["sm"] = sigs.SigModel(dm.corpus, dm.infos)
        plan = groupgen.make_plan([fn], dmodel=dm)
        info, _ = groupgen.emit_and_compile(plan, gdir, dm, _sm_cache["sm"], force=True, compile_=False)
    return info


def residual_text(src, fn, flags, log, lines):
    from amatch import report
    return compact_residual(report.report_fn(src, fn, flags, log, lines=lines), lines + 40)


def residual_group_text(gdir, log, lines):
    from amatch import report
    return compact_residual(report.report_group(gdir, log, lines=lines), lines + 40)


# --- the driver -----------------------------------------------------------------------------------------------------

class Autopilot:
    def __init__(self, out_dir=AP_DIR, write=False, budget=400, seconds=600, jobs=None, parallel=1, seed=0,
                 residual_lines=50, flag_sweep=True, log=None, **hooks):
        from amatch import builder
        self.builder = builder
        self.out = Path(out_dir)
        self.write = write
        self.budget, self.seconds, self.seed = budget, seconds, seed
        self.jobs = jobs or (os.cpu_count() or 1)
        self.parallel = max(1, parallel)
        self.residual_lines = residual_lines
        self.flag_sweep = flag_sweep
        self.log = log or (lambda m: print(m, file=sys.stderr, flush=True))
        self.compile_fn = hooks.get("compile_fn") or builder.compile_fn
        self.compile_group = hooks.get("compile_group") or builder.compile_group
        self.search = hooks.get("search") or search_cli
        self.verify_fn = hooks.get("verify_fn") or verify_fn_real
        self.verify_group = hooks.get("verify_group") or verify_group_real
        self.m2c_seed = hooks.get("m2c_seed") or m2c_seed
        self.gen_group = hooks.get("gen_group") or gen_group
        self.residual = hooks.get("residual") or residual_text
        self.residual_group = hooks.get("residual_group") or residual_group_text
        self.repo = Path(hooks.get("repo") or ROOT)
        self.lock = threading.Lock()
        self.state_path = self.out / "state.json"
        self.state = self._load()

    # state ------------------------------------------------------------------------------------
    def _load(self):
        try:
            return json.loads(self.state_path.read_text())
        except (OSError, ValueError):
            return {"version": 1, "items": {}}

    def _save(self):
        self.out.mkdir(parents=True, exist_ok=True)
        tmp = self.state_path.with_suffix(".tmp")
        tmp.write_text(json.dumps(self.state, indent=1, default=str))
        os.replace(tmp, self.state_path)

    def record(self, rec):
        with self.lock:
            self.state["items"][rec["fn"]] = rec
            self._save()

    # seeds --------------------------------------------------------------------------------------
    def _seed_candidates(self, item):
        """[(origin, src, flags)] in preference order."""
        out = []
        base_flags = item.get("flags") or DEFAULT_FLAGS
        for rel in (item.get("seed_files") or item.get("seeds") or [])[:3]:
            p = Path(rel)
            p = p if p.is_absolute() else self.repo / rel
            try:
                src = p.read_text()
            except OSError:
                continue
            out.append((str(rel), src, self.builder.parse_flags_comment(src) or base_flags))
        return out

    def _best_seed(self, item):
        fn = item["fn"]
        cands = self._seed_candidates(item)
        scored = []
        for origin, src, flags in cands:
            scored.append((self.compile_fn(src, fn, flags), origin, src, flags))
        t = lambda r: max(1, r.get("target_size") or 1)            # noqa: E731
        close = any(s[0].get("matched") or s[0].get("aligned_exact", 0) >= 0.8 * t(s[0]) for s in scored)
        note = None
        if not close and not item.get("no_m2c") and item.get("class", "ABI") == "ABI":
            try:
                text, info = self.m2c_seed(fn)
                if isinstance(info, dict) and info.get("m2c") and not all(info["m2c"].values()):
                    note = "m2c could not decompile; seed is a stub"
                scored.append((self.compile_fn(text, fn, DEFAULT_FLAGS), "m2c", text, DEFAULT_FLAGS))
            except BaseException as exc:                              # noqa: BLE001
                if isinstance(exc, KeyboardInterrupt):
                    raise
                note = "m2c seed failed: %s: %s" % (type(exc).__name__, str(exc)[:160])
        if not scored:
            return None, note or "no seed source"
        scored.sort(key=lambda s: score_key(s[0]))
        r, origin, src, flags = scored[0]
        if self.flag_sweep and not r.get("matched") and not item.get("no_sweep") and not r.get("err"):
            for opt in ("-O1", "-O3", "-O2"):
                f2 = with_opt(flags, opt)
                if f2 == flags:
                    continue
                r2 = self.compile_fn(src, fn, f2)
                if score_key(r2) < score_key(r):
                    r, flags, origin = r2, f2, origin + " " + opt
        return {"result": r, "origin": origin, "src": src, "flags": flags}, note

    # single -----------------------------------------------------------------------------------------
    def run_single(self, item, jobs):
        fn = item["fn"]
        wd = self.out / "work" / fn
        wd.mkdir(parents=True, exist_ok=True)
        rec = {"fn": fn, "kind": "single", "class": item.get("class", "ABI"), "words": item.get("words"),
               "priority": item.get("priority"), "ts": time.time(), "reason": item.get("reason", "")}
        seed, note = self._best_seed(item)
        if seed is None:
            rec.update(status="needs_llm", shape="compile_error", suggest="llm",
                       hint=note, seed_origin=None)
            return rec
        r, src, flags = seed["result"], seed["src"], seed["flags"]
        rec.update(seed_origin=seed["origin"], flags=flags, seed_note=note,
                   seed_score=_brief(r))
        (wd / "seed.c").write_text(src)
        final_src, final_r, log = src, r, None
        if not r.get("matched") and not (r.get("err") and not r.get("words")):
            res = self.search("fn", wd / "seed.c", fn, flags, self.budget, self.seconds, jobs, self.seed,
                              wd / "search", (self.seconds or 3600) + 300)
            rec["search"] = {k: res.get(k) for k in ("matched", "evals", "seconds", "tried_mutations", "error")}
            bp = res.get("best_src_path")
            if bp and Path(bp).exists():
                final_src = Path(bp).read_text()
                final_r = self.compile_fn(final_src, fn, flags)
            lp = res.get("log_path")
            try:
                log = json.loads(Path(lp).read_text()) if lp else None
            except (OSError, ValueError):
                log = None
            claimed = bool(res.get("matched")) or bool(final_r.get("matched"))
        else:
            claimed = bool(r.get("matched"))
        if claimed and final_r.get("matched"):
            ok, msg = self.verify_fn(final_src, fn, flags)
            if ok:
                text = with_flags_line(final_src, flags)
                p = self._emit_match(fn, text)
                rec.update(status="matched", match_file=str(p), score=_brief(final_r))
                return rec
            rec["verify_failed"] = msg
        (wd / "best.c").write_text(final_src)
        shape, who, hint = classify_shape(final_r, self.builder.get_targets().get(fn))
        rec.update(status="needs_llm", shape=shape, suggest=who, hint=hint, score=_brief(final_r),
                   best_src=str(wd / "best.c"))
        try:
            rec["residual"] = self.residual(final_src, fn, flags, log, self.residual_lines)
        except BaseException as exc:                                   # noqa: BLE001
            if isinstance(exc, KeyboardInterrupt):
                raise
            rec["residual"] = "(residual report failed: %s: %s)" % (type(exc).__name__, exc)
        return rec

    def _emit_match(self, fn, text):
        d = self.out / "out" / "matches"
        d.mkdir(parents=True, exist_ok=True)
        p = d / (fn + ".c")
        p.write_text(text)
        if self.write:
            dest = self.repo / "cloud" / "matches" / (fn + ".c")
            if dest.exists() and dest.read_text() != text:
                self.log("%s: cloud/matches/%s.c exists and differs; left alone (new copy in %s)" % (fn, fn, p))
            else:
                dest.parent.mkdir(parents=True, exist_ok=True)
                dest.write_text(text)
                p = dest
        return p

    # group --------------------------------------------------------------------------------------------
    def run_group(self, item, jobs):
        fn = item["fn"]
        name = item.get("group") or fn
        gdir = self.out / "work" / ("group_" + name)
        if gdir.exists():
            shutil.rmtree(gdir)
        rec = {"fn": fn, "kind": "group", "group": name, "class": item.get("class"), "words": item.get("words"),
               "priority": item.get("priority"), "ts": time.time(), "reason": item.get("reason", "")}
        draft = self.repo / "cloud" / "work" / "ipa-groups" / name
        try:
            if item.get("group") and (draft / "group.json").exists():
                shutil.copytree(draft, gdir)
                rec["seed_origin"] = "drafted group cloud/work/ipa-groups/%s" % name
            else:
                info = self.gen_group(fn, gdir)
                rec["seed_origin"] = "groupgen.make_plan([%s])" % fn
                rec["stubbed"] = (info or {}).get("stubbed") if isinstance(info, dict) else None
        except BaseException as exc:                                    # noqa: BLE001
            if isinstance(exc, KeyboardInterrupt):
                raise
            rec.update(status="needs_llm", shape="ipa_context_missing", suggest="llm",
                       hint="group generation failed: %s: %s" % (type(exc).__name__, str(exc)[:200]))
            return rec
        spec = json.loads((gdir / "group.json").read_text())
        rec["members"] = spec.get("members")
        base = self.compile_group(gdir)
        if base.get("err") and not base.get("matched"):
            rec.update(status="needs_llm", shape="ipa_context_missing", suggest="llm",
                       hint="seed group does not build: " + str(base["err"]).splitlines()[0][:200])
            return rec
        best_dir, res, log = gdir, {}, None
        if not base.get("matched"):
            res = self.search("group", gdir, fn, None, self.budget, self.seconds, jobs, self.seed,
                              self.out / "work" / ("search_" + name), (self.seconds or 3600) + 600)
            rec["search"] = {k: res.get(k) for k in ("matched", "evals", "seconds", "tried_mutations", "error")}
            bp = res.get("best_src_path")
            if bp and Path(bp).is_dir():
                best_dir = Path(bp)
            try:
                log = json.loads(Path(res["log_path"]).read_text()) if res.get("log_path") else None
            except (OSError, ValueError):
                log = None
        gr = self.compile_group(best_dir)
        standin = lambda n: n.startswith("__standin")                  # noqa: E731
        matched = [n for n in spec["members"] if gr.get("members", {}).get(n, {}).get("matched") and not standin(n)]
        prior = set(spec.get("claims") or [])
        claims = sorted(prior | set(matched), key=spec["members"].index)
        rec["score"] = {n: _brief(d) for n, d in (gr.get("members") or {}).items()}
        if fn in matched:
            ok, msg = self.verify_group(best_dir, claims)
            if ok is False:
                rec["verify_failed"] = msg
            else:
                outd = self.out / "out" / "groups" / name
                if outd.exists():
                    shutil.rmtree(outd)
                shutil.copytree(best_dir, outd, ignore=shutil.ignore_patterns("search_log.json", "result.json"))
                sj = json.loads((outd / "group.json").read_text())
                sj["claims"] = claims
                (outd / "group.json").write_text(json.dumps(sj, indent=2) + "\n")
                dest = None
                if self.write:
                    dest = self.repo / "cloud" / "work" / "ipa-groups" / name
                    shutil.copytree(outd, dest, dirs_exist_ok=True)
                rec.update(status="matched", match_file=str(dest or outd), claims=claims,
                           verified_by=("score.py group --claims" if ok else "builder strict score only (%s)" % msg[:80]))
                return rec
        unm = [n for n in spec["members"] if n not in matched and not standin(n) and gr.get("members", {}).get(n)]
        worst = gr["members"].get(fn) or (gr["members"][unm[0]] if unm else {})
        shape, who, hint = classify_shape(worst, self.builder.get_targets().get(fn), ipa=True)
        rec.update(status="needs_llm", shape=shape, suggest=who, hint=hint, best_src=str(best_dir),
                   partial_claims=claims if matched else [])
        try:
            rec["residual"] = self.residual_group(best_dir, log, self.residual_lines)
        except BaseException as exc:                                   # noqa: BLE001
            if isinstance(exc, KeyboardInterrupt):
                raise
            rec["residual"] = "(residual report failed: %s: %s)" % (type(exc).__name__, exc)
        return rec

    # batch ----------------------------------------------------------------------------------------------
    def process(self, item, jobs=None):
        jobs = jobs or max(1, self.jobs // self.parallel)
        t0 = time.time()
        try:
            rec = (self.run_group if str(item.get("class", "ABI")).startswith("IPA") else self.run_single)(item, jobs)
        except BaseException as exc:                                     # noqa: BLE001
            if isinstance(exc, KeyboardInterrupt):
                raise
            rec = {"fn": item["fn"], "kind": "single", "status": "error", "ts": time.time(),
                   "error": "%s: %s" % (type(exc).__name__, exc), "class": item.get("class")}
        rec["secs"] = round(time.time() - t0, 1)
        self.record(rec)
        self.log("%-26s %-10s %s%s (%.0fs)" % (rec["fn"], rec["status"], rec.get("shape", ""),
                                               "  " + str(rec.get("match_file", "")) if rec["status"] == "matched" else "",
                                               rec["secs"]))
        return rec

    def run(self, items, redo=False):
        todo = [i for i in items if redo or self.state["items"].get(i["fn"], {}).get("status") not in FINAL]
        skipped = len(items) - len(todo)
        if skipped:
            self.log("resuming: %d finished item(s) skipped (use --redo)" % skipped)
        if self.parallel == 1:
            for i in todo:
                self.process(i)
        else:
            with cf.ThreadPoolExecutor(self.parallel) as ex:
                list(ex.map(self.process, todo))
        return self.write_reports([i["fn"] for i in items])

    # reports --------------------------------------------------------------------------------------------
    def write_reports(self, fns=None):
        recs = [r for n, r in self.state["items"].items() if fns is None or n in fns]
        matched = [r for r in recs if r["status"] == "matched"]
        need = [r for r in recs if r["status"] == "needs_llm"]
        errs = [r for r in recs if r["status"] == "error"]
        need.sort(key=lambda r: -(r.get("priority") or 0))
        wl = []
        for r in need:
            wl.append({k: r.get(k) for k in ("fn", "kind", "group", "class", "words", "shape", "suggest", "hint",
                                             "reason", "flags", "seed_origin", "best_src", "score", "partial_claims",
                                             "search")})
            wl[-1]["reason_shape"] = "%s: %s" % (r.get("shape"), r.get("hint"))
        self.out.mkdir(parents=True, exist_ok=True)
        (self.out / "worklist.json").write_text(json.dumps(wl, indent=1, default=str))
        L = ["# Autopilot report", "",
             "%d item(s): %d matched, %d need an LLM, %d error(s).%s" % (
                 len(recs), len(matched), len(need), len(errs),
                 "" if self.write else "  Dry run: matches are in `build/autopilot/out/`, the repo is untouched."), ""]
        L += ["## Matched", ""]
        L += ["- `%s` (%s%s) -> %s" % (r["fn"], r["kind"], ", claims " + ",".join(r["claims"]) if r.get("claims") else "",
                                    r.get("match_file")) for r in matched] or ["(none)"]
        L += ["", "## Needs LLM (%d)" % len(need), ""]
        shapes = {}
        for r in need:
            shapes.setdefault(r.get("shape"), []).append(r["fn"])
        for s, names in sorted(shapes.items(), key=lambda x: -len(x[1])):
            L.append("- **%s** (%d): %s" % (s, len(names), ", ".join(names)))
        for r in need:
            L += ["", "### `%s`  [%s / %s]" % (r["fn"], r.get("shape"), r.get("suggest")), "",
                  "%s  Seed: %s; flags `%s`." % (r.get("hint") or "", r.get("seed_origin"), r.get("flags") or ""),
                  "Best source: `%s`" % r.get("best_src"), "", r.get("residual") or "(no residual)"]
        if errs:
            L += ["", "## Errors", ""] + ["- `%s`: %s" % (r["fn"], r.get("error")) for r in errs]
        (self.out / "report.md").write_text("\n".join(L) + "\n")
        return {"matched": [r["fn"] for r in matched], "needs_llm": [r["fn"] for r in need],
                "errors": [r["fn"] for r in errs], "report": str(self.out / "report.md"),
                "worklist": str(self.out / "worklist.json")}


def _brief(r):
    return {k: r.get(k) for k in ("strict_diff", "size", "target_size", "aligned_exact", "aligned_opcode",
                                  "aligned_opcode_reg", "matched") if r and k in r}


# --- bench / selftest -------------------------------------------------------------------------------------------------

def bench_items(fns=None, cls=None, limit=None, need_compile=True):
    from amatch import corpus
    idx = corpus.load_index()
    hl = corpus.header_lines()
    ents = [e for e in corpus.with_start(idx) if e["kind"] == "single"
            and (not need_compile or (e.get("start") or {}).get("score", {}).get("compiles"))]
    if fns:
        ents = [e for e in ents if e["fn"] in fns]
    if cls:
        ents = [e for e in ents if cls in e["edit"]["classes"]]
    if limit:
        ents = ents[:limit]
    return [(e, corpus.unpack((corpus.DATA / e["start_file"]).read_text(), hl)) for e in ents]


def run_bench(ap_opts, fns=None, cls=None, limit=None, out_json=None, redo=True):
    tmp = tempfile.mkdtemp(prefix="ap-bench-")
    pairs = bench_items(fns, cls, limit)
    items = []
    for e, text in pairs:
        p = Path(tmp) / (e["fn"] + ".c")
        p.write_text(text)
        items.append({"fn": e["fn"], "class": "ABI", "flags": e["flags"], "seed_files": [str(p)], "no_m2c": True,
                      "no_sweep": True, "words": None})
    opts = dict(ap_opts)
    opts["out_dir"] = Path(tmp) / "out"
    opts["write"] = False
    a = Autopilot(**opts)
    t0 = time.time()
    a.run(items, redo=True)
    rows = {}
    results = []
    for (e, _), it in zip(pairs, items):
        r = a.state["items"].get(it["fn"], {})
        results.append((e, r))
        for name in ["ALL"] + list(e["edit"]["classes"]):
            d = rows.setdefault(name, {"n": 0, "hit": 0, "evals": [], "secs": [], "err": 0})
            d["n"] += 1
            d["err"] += r.get("status") == "error"
            if r.get("status") == "matched":
                d["hit"] += 1
                d["secs"].append(r.get("secs") or 0)
                d["evals"].append((r.get("search") or {}).get("evals") or 0)
    print("autopilot bench: budget=%d evals, %d functions, %.0fs total" % (a.budget, len(items), time.time() - t0))
    print("%-18s %5s %7s %6s %10s %9s %4s" % ("edit class", "n", "matched", "rate", "med evals", "med sec", "err"))
    for name in ["ALL"] + sorted(k for k in rows if k != "ALL"):
        d = rows[name]
        print("%-18s %5d %7d %5.0f%% %10s %9s %4d" % (
            name, d["n"], d["hit"], 100.0 * d["hit"] / max(1, d["n"]),
            "%d" % statistics.median(d["evals"]) if d["evals"] else "-",
            "%.1f" % statistics.median(d["secs"]) if d["secs"] else "-", d["err"]))
    missed = [it["fn"] for it in items if a.state["items"].get(it["fn"], {}).get("status") != "matched"]
    if missed:
        print("not matched: " + " ".join(missed))
    if out_json:
        Path(out_json).write_text(json.dumps([dict(r, fn=e["fn"], classes=e["edit"]["classes"])
                                              for e, r in results], indent=1, default=str))
    shutil.rmtree(tmp, ignore_errors=True)
    return rows


def selftest(ap_opts):
    from amatch import builder
    if not builder.ido_available():
        print("selftest: SKIP (IDO missing: run tools/cloud/setup.sh)")
        return 0
    pairs = bench_items(SELFTEST_FNS)
    if len(pairs) < SELFTEST_NEED:
        pairs = pairs + [p for p in bench_items(limit=12) if p[0]["fn"] not in {q[0]["fn"] for q in pairs}]
    tmp = tempfile.mkdtemp(prefix="ap-self-")
    items = []
    for e, text in pairs[:len(SELFTEST_FNS)]:
        p = Path(tmp) / (e["fn"] + ".c")
        p.write_text(text)
        items.append({"fn": e["fn"], "class": "ABI", "flags": e["flags"], "seed_files": [str(p)], "no_m2c": True,
                      "no_sweep": True})
    opts = dict(ap_opts)
    opts.update(out_dir=Path(tmp) / "out", write=False, budget=max(opts.get("budget", 0), 300))
    a = Autopilot(**opts)
    res = a.run(items, redo=True)
    ok = len(res["matched"]) >= SELFTEST_NEED
    rep = Path(res["report"]).exists() and Path(res["worklist"]).exists()
    for fn in res["matched"]:
        f = Path(tmp) / "out" / "out" / "matches" / (fn + ".c")
        ok &= f.exists() and f.read_text().startswith("/* flags:")
    print("selftest: matched %d/%d (%s), report+worklist written: %s -> %s" % (
        len(res["matched"]), len(items), ", ".join(res["matched"]) or "-", rep, "PASS" if ok and rep else "FAIL"))
    if not (ok and rep):
        print("  needs_llm: %s" % res["needs_llm"])
    shutil.rmtree(tmp, ignore_errors=True)
    return 0 if ok and rep else 1


# --- CLI ------------------------------------------------------------------------------------------------------------------

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--top", type=int, default=10, help="worklist size from triage.py")
    ap.add_argument("--class", dest="cls", choices=["abi", "ipa", "all"], default="abi")
    ap.add_argument("--fn", action="append", help="work on these functions instead of the triage ranking")
    ap.add_argument("--budget", type=int, default=400, help="search evals per item (group: same)")
    ap.add_argument("--seconds", type=float, default=600, help="wall clock per item search")
    ap.add_argument("--jobs", "-j", type=int, default=os.cpu_count() or 1, help="total compile workers")
    ap.add_argument("--parallel", "-p", type=int, default=None, help="items in flight (default: min(jobs, 4))")
    ap.add_argument("--seed", type=int, default=0)
    ap.add_argument("--out", default=str(AP_DIR))
    ap.add_argument("--redo", action="store_true")
    ap.add_argument("--write", action="store_true", help="write matches into cloud/matches and cloud/work/ipa-groups")
    ap.add_argument("--no-sweep", action="store_true", help="skip the -O1/-O3 flag sweep on seeds")
    ap.add_argument("--residual-lines", type=int, default=50)
    ap.add_argument("--report-only", action="store_true")
    ap.add_argument("--bench", action="store_true")
    ap.add_argument("--limit", type=int)
    ap.add_argument("--edit-class", dest="edit_class")
    ap.add_argument("--json-out")
    ap.add_argument("--selftest", action="store_true")
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args(argv)
    par = a.parallel or min(a.jobs, 4)
    opts = dict(out_dir=a.out, write=a.write, budget=a.budget, seconds=a.seconds, jobs=a.jobs, parallel=par,
                seed=a.seed, residual_lines=a.residual_lines, flag_sweep=not a.no_sweep)
    if a.selftest:
        return selftest(opts)
    if a.bench:
        from amatch import builder
        if not builder.ido_available():
            sys.exit("bench needs IDO (tools/cloud/setup.sh)")
        run_bench(opts, a.fn, a.edit_class, a.limit, a.json_out)
        return 0
    if a.report_only:
        res = Autopilot(**opts).write_reports()
        print(json.dumps(res) if a.json else "wrote %(report)s and %(worklist)s" % res)
        return 0
    from amatch import builder, triage
    if not builder.ido_available():
        sys.exit("autopilot needs IDO (tools/cloud/setup.sh)")
    rows = triage.features("all" if a.fn else a.cls)
    if a.fn:
        by = {r["fn"]: r for r in rows}
        items = [by[n] for n in a.fn if n in by]
        missing = [n for n in a.fn if n not in by]
        if missing:
            print("not candidates (already spliced/matched/claimed or unknown): " + " ".join(missing), file=sys.stderr)
    else:
        items = rows[:a.top]
    for it in items:
        it["seed_files"] = it.get("seeds")
    res = Autopilot(**opts).run(items, redo=a.redo)
    print(json.dumps(res) if a.json else
          "matched %d, needs_llm %d, errors %d\nreport: %s\nworklist: %s" % (
              len(res["matched"]), len(res["needs_llm"]), len(res["errors"]), res["report"], res["worklist"]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
