#!/usr/bin/env python3
"""draft_check: does a draft BEHAVE like the retail function? (recomp Phase 7, lane w27r)

    python3 -m tools.recomp.draft_check NAME path/to/draft.c [--scripts race,two_player|all]
            [--params handle=a0,mark=t3] [--callee 'void f(s32 h, f32 x)=a0,f22'] [--remote rocky]

Builds the recomp twice (incrementally, cached per function on the recomp host):
  retail side  the stock game with a capture hook on NAME (records every call's
               arguments, return value and the words of N64 memory it changed);
  draft side   the same, but NAME's body replaced by the draft compiled with the
               project's IDO (replace.py `ido` mode: exact N64 memory semantics).
Then runs the lockstep input scripts headless (null renderer, --clock frames) on
both and compares call by call (oracle.py run). Prints one of

  EQUIVALENT over N calls / K scripts   every call of NAME identical (args, return,
                                        memory effect) and every judged frame identical.
                                        BEHAVIOUR ONLY: this is NOT a match and NOT
                                        coverage (score.py / blob_unit decide that).
  DIVERGES                              script, frame, call number, what differs
                                        (argument / return / memory words), retail vs draft.
  NEVER CALLED                          no script reaches NAME (write a script; see
                                        tools/recomp/census/README.md for what reaches what).

Exit status 0 EQUIVALENT, 1 DIVERGES, 3 never called, 2 error (build failure: the
IDO/ld message is printed).

Register-parameter functions (IDO -O3 IPA; build/frontier.json facts
`register_params`): a draft of such a function needs `--params NAME=REG,...` for
every prototype parameter (the hook then calls the draft through an inbound shim
that moves those registers into o32 slots, and the capture records those
registers). A draft that CALLS such a function gets register shims from
tools/recomp/ipa_callees.json, or from `--callee 'PROTOTYPE=REG,...'`; any other
register-parameter callee is called o32 and named in a WARNING (a divergence after
such a call may be the harness, not the draft). Prove a mapping first with
`python3 -m tools.recomp.regmap_verify`.

Runs on the recomp host (rocky). From another machine (the Pi) add `--remote rocky`:
the draft, the local headers it includes and this checkout's include/ are copied to
rocky (`~/rush2049/recomp/dc/inbox/`) and the same command runs in the service tree
`~/rush2049/recomp/dc/repo` there; output streams back. Stdlib only, Python 3.9+.
"""
import argparse
import fcntl
import getpass
import hashlib
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
if str(HERE) not in sys.path:
    sys.path.insert(0, str(HERE))
import oracle  # noqa: E402
import replace as rp  # noqa: E402

RECOMP_HOME = Path(os.environ.get("RUSH_RECOMP_HOME", str(Path.home() / "rush2049" / "recomp")))
TOOLKITS = Path(os.environ.get("RUSH_TOOLKITS", str(Path.home() / "rush2049" / "cache" / "toolkits")))
IDO53 = "796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5"
IDO71 = "4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557"
DC_HOME = Path(os.environ.get("RUSH_DC_HOME", str(RECOMP_HOME / "dc")))
REMOTE_TREE = "rush2049/recomp/dc/repo"      # relative to the remote home
REMOTE_INBOX = "rush2049/recomp/dc/inbox"
CENSUS_DIR = HERE / "census"
SCRIPT_DIR = HERE / "scripts"
# saves_reload needs the pak image saves.press wrote (--pak FILE); not runnable alone.
UNRUNNABLE = {"saves_reload"}
DEFAULT_MAX_SCRIPTS = 6
REG_RE = re.compile(r"^(?:[atsv][0-9]|s8|fp|gp|f([0-9]|[12][0-9]|3[01])(:d)?|sp\+\d+)$")


class DCError(RuntimeError):
    pass


def paths():
    return {
        "rom": Path(os.environ.get("RUSH2049_ROM", str(RECOMP_HOME / "rom" / "baserom.us.z64"))),
        "n64recomp_src": RECOMP_HOME / "src" / "N64Recomp",
        "n64recomp": RECOMP_HOME / "build" / "N64Recomp" / "N64Recomp",
        "rsprecomp": RECOMP_HOME / "build" / "N64Recomp" / "RSPRecomp",
        "runtime": RECOMP_HOME / "src" / "N64ModernRuntime",
        "ido53": TOOLKITS / IDO53,
        "ido71": TOOLKITS / IDO71,
    }


# ------------------------------------------------------------------ inputs

def all_scripts():
    return sorted(p.stem for p in SCRIPT_DIR.glob("*.press") if p.stem not in UNRUNNABLE)


def census_scripts(name, census_dir=CENSUS_DIR):
    """(scripts that call NAME per the census, True) / ([], True) when the
    census covers NAME and nothing calls it / (None, False) when NAME is not
    in the census (unknown: run everything)."""
    summary = census_dir / "census.json"
    try:
        doc = json.loads(summary.read_text())
    except (OSError, ValueError):
        return None, False
    if name not in doc.get("functions", {}):
        return None, False
    calls = doc["functions"][name].get("scripts", {})
    return sorted(calls, key=lambda s: (-calls[s], s)), True


def parse_params(spec, prototype):
    """'handle=a0,mark=t3' (or 'a0=handle,...') against the prototype ->
    (capture_args ['handle=a0', ...] in prototype order, ipa params ['a0', ...])."""
    _ret, _name, params = rp.parse_prototype(prototype)
    names = [p for _t, p in params]
    mapping = {}
    for item in [x.strip() for x in spec.split(",") if x.strip()]:
        if "=" not in item:
            raise DCError(f"--params: {item!r} is not NAME=REG")
        a, b = [x.strip() for x in item.split("=", 1)]
        if REG_RE.match(a) and not REG_RE.match(b):
            a, b = b, a
        if not REG_RE.match(b):
            raise DCError(f"--params: {b!r} is not a register (a0..a3, t0..t9, s0..s8, v0/v1, fN, sp+N)")
        if a not in names:
            raise DCError(f"--params: {a!r} is not a parameter of {prototype}")
        mapping[a] = b
    missing = [n for n in names if n not in mapping]
    if missing:
        raise DCError(f"--params: no register for {', '.join(missing)} (prototype {prototype})")
    return ["%s=%s" % (n, mapping[n]) for n in names], [mapping[n] for n in names]


def parse_callee(spec):
    """'void f(s32 h, f32 x)=a0,f22[/s1,s2]' -> (name, ipa_callees entry)."""
    m = re.match(r"^(.*\))\s*=\s*([^/]+)(?:/(.*))?$", spec.strip())
    if not m:
        raise DCError(f"--callee: expected 'PROTOTYPE=REG,...[/CLOBBER,...]', got {spec!r}")
    proto = " ".join(m.group(1).split())
    _ret, name, params = rp.parse_prototype(proto)
    regs = [r.strip() for r in m.group(2).split(",") if r.strip()]
    if len(regs) != len(params):
        raise DCError(f"--callee {name}: {len(params)} parameters, {len(regs)} registers")
    e = {"prototype": proto, "params": regs, "evidence": "draft_check --callee (lane hypothesis)"}
    if m.group(3):
        e["clobbers"] = [r.strip() for r in m.group(3).split(",") if r.strip()]
    return name, e


def register_params_fact(name, frontier=REPO / "build" / "frontier.json"):
    try:
        f = json.loads(Path(frontier).read_text()).get("facts", {}).get(name) or {}
    except (OSError, ValueError):
        return []
    return [r for r in f.get("register_params") or () if r not in rp._ABI_IN]


GENERIC_STACK = tuple(range(16, 48, 4))


def clobber_shims(table, frontier=REPO / "build" / "frontier.json"):
    """ipa_callees entries for callees that take o32 arguments but write
    callee-saved registers without saving them (frontier facts
    unsaved_callee_regs, no register_params): a shim that passes a0-a3 and
    eight stack words through unchanged (f12/f14 are not touched) and saves
    the clobbered registers around the call, so the draft's s-registers
    survive as they would in the whole-program build."""
    try:
        facts = json.loads(Path(frontier).read_text()).get("facts", {})
    except (OSError, ValueError):
        return {}
    out = {}
    for name, f in facts.items():
        if not isinstance(f, dict) or name in table or not f.get("unsaved_callee_regs"):
            continue
        if [r for r in f.get("register_params") or () if r not in rp._ABI_IN]:
            continue
        n = 4 + len(GENERIC_STACK)
        out[name] = {"prototype": "void %s(%s)" % (name, ", ".join("s32 p%d" % i for i in range(n))),
                     "params": ["a0", "a1", "a2", "a3"] + ["sp+%d" % o for o in GENERIC_STACK],
                     "clobbers": list(f["unsaved_callee_regs"]),
                     "evidence": "draft_check: frontier unsaved_callee_regs (o32 pass-through + saves)"}
    return out


def find_population(name, game_functions=None):
    sys.path.insert(0, str(HERE))
    import gen_config as gc
    game = game_functions if game_functions is not None else gc.load_game_functions(REPO / "asm" / "us" / "blob")
    if any(f.get("name") == name for f in game):
        return "game"
    for ovl in ("ovl_a", "ovl_b"):
        p = REPO / "asm" / "us" / ovl / "extents.json"
        if p.is_file() and any(f.get("name") in (name, name.replace(ovl + "_", ""))
                               for f in gc.load_extents_functions(p)):
            raise DCError(f"{name} is an overlay function: not supported (overlays load per mode; "
                          "the overlays are Astra's this wave)")
    return "static"


# ------------------------------------------------------------------ build (incremental, per function)

def _run(cmd, log, cwd=None, env=None):
    with open(log, "a") as fh:
        fh.write("$ " + " ".join(shlex.quote(str(c)) for c in cmd) + "\n")
        fh.flush()
        p = subprocess.run([str(c) for c in cmd], cwd=cwd, env=env, stdout=fh, stderr=subprocess.STDOUT)
    return p.returncode


def _tail(path, n=40, pattern=None):
    try:
        lines = Path(path).read_text(errors="replace").splitlines()
    except OSError:
        return ""
    if pattern:
        hit = [i for i, line in enumerate(lines) if re.search(pattern, line)]
        if hit:
            return "\n".join(lines[hit[0]:hit[0] + n])
    return "\n".join(lines[-n:])


def build_side(kind, name, manifest, run_dir, p, ipa_table=None, jobs=8, extra=()):
    """Build the retail capture side (kind 'cap') or the draft side ('repl') of
    NAME in DC_HOME/trees/NAME/KIND. Generated files are synced with checksums,
    so ninja recompiles only what changed. Returns {host, rom, datasyms, gen_log}
    with host/rom COPIED into run_dir/KIND (the tree may be rebuilt meanwhile)."""
    # one build tree per (checkout, function): cmake binds a tree to its runtime source dir
    tag = "%s-%s" % (REPO.name, hashlib.sha256(str(REPO).encode()).hexdigest()[:8])
    tree = DC_HOME / "trees" / tag / name / kind
    tree.mkdir(parents=True, exist_ok=True)
    log = run_dir / ("build_%s.log" % kind)
    with open(tree / "lock", "w") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        stage, live = tree / "stage", tree / "out"
        shutil.rmtree(stage, ignore_errors=True)
        cmd = [sys.executable, HERE / "gen_config.py", "--rom", p["rom"], "--n64recomp-src", p["n64recomp_src"],
               "--out", stage, "--replacements", manifest, "--ido53", p["ido53"], "--ido71", p["ido71"]]
        cmd += ["--capture-set", "draft"] if kind == "cap" else ["--replace-set", "draft"]
        if ipa_table and kind == "repl":
            cmd += ["--ipa-table", ipa_table]
        cmd += list(extra)
        gen_log = run_dir / ("gen_%s.log" % kind)
        if _run(cmd, gen_log, cwd=str(REPO)) != 0:
            raise DCError("%s build: gen_config failed:\n%s" % (
                "draft" if kind == "repl" else "retail", _tail(gen_log, 40, r"gen_config:|Error|error")))
        if _run([p["n64recomp"], "rush2049.recomp.toml"], log, cwd=str(stage)) != 0 or \
                _run([p["rsprecomp"], "rush2049.rsp_audio.toml"], log, cwd=str(stage)) != 0:
            raise DCError("%s build: N64Recomp failed:\n%s" % (kind, _tail(log)))
        live.mkdir(exist_ok=True)
        if _run(["rsync", "-rc", "--delete", "--exclude=/host/", str(stage) + "/", str(live) + "/"], log) != 0:
            raise DCError("rsync failed: " + _tail(log, 5))
        host = live / "host"
        env = dict(os.environ, CC="clang", CXX="clang++")
        if not (host / "build.ninja").is_file():
            rc = _run(["cmake", "-S", HERE / "runtime", "-B", host, "-G", "Ninja",
                       "-DN64MODERNRUNTIME_DIR=%s" % p["runtime"], "-DRECOMP_OUT=%s" % live], log, env=env)
        else:
            rc = _run(["cmake", host], log, env=env)
        if rc != 0 or _run(["ninja", "-j", str(jobs), "-C", host], log, env=env) != 0:
            raise DCError("%s build: host compile failed:\n%s" % (kind, _tail(log, 30, r"error|FAILED")))
        dst = run_dir / kind
        dst.mkdir(exist_ok=True)
        shutil.copy2(host / "rush2049_recomp", dst / "rush2049_recomp")
        shutil.copy2(live / "rom" / "rush2049.recomp.z64", dst / "rush2049.recomp.z64")
        shutil.copy2(live / "rush2049.datasyms.toml", dst / "rush2049.datasyms.toml")
        shutil.copy2(live / "manifest.json", dst / "manifest.json")
    return {"host": dst / "rush2049_recomp", "rom": dst / "rush2049.recomp.z64",
            "datasyms": dst / "rush2049.datasyms.toml", "manifest": dst / "manifest.json", "gen_log": gen_log}


def sha256_file(path):
    h = hashlib.sha256()
    with open(path, "rb") as fh:
        for chunk in iter(lambda: fh.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


# ------------------------------------------------------------------ runs

STOCK_FILES = ("stock1.log", "stock2.log", "stock.calls")


def run_script(script, name, cap, repl, run_dir, cap_key, frames=None):
    """oracle.py run for one script with the retail logs cached per
    (retail binary + ROM, script text, frames). Returns a result dict."""
    sc_path = SCRIPT_DIR / (script + ".press")
    sc = oracle.load_script(script)
    nframes = frames or sc["frames"] or 1800
    skey = hashlib.sha256(sc_path.read_bytes()).hexdigest()[:12]
    cache = DC_HOME / "cache" / name / cap_key / ("%s-%s-%d" % (script, skey, nframes))
    out = run_dir / "runs" / script
    out.mkdir(parents=True, exist_ok=True)
    cached = all((cache / f).is_file() for f in STOCK_FILES)
    if cached:
        for f in STOCK_FILES:
            try:
                os.link(cache / f, out / f)
            except OSError:
                shutil.copy2(cache / f, out / f)
    cmd = [sys.executable, str(HERE / "oracle.py"), "run", "--stock-host", str(cap["host"]),
           "--rom-stock", str(cap["rom"]), "--repl-host", str(repl["host"]), "--rom-repl", str(repl["rom"]),
           "--out", str(out), "--script", script, "--frames", str(nframes), "--reuse-stock",
           "--datasyms", str(cap["datasyms"]), "--json", str(out / "verdict.json")]
    t0 = time.time()
    proc = subprocess.run(cmd, capture_output=True, text=True)
    (out / "oracle.txt").write_text(proc.stdout + proc.stderr)
    res = {"script": script, "frames": nframes, "seconds": round(time.time() - t0, 1), "rc": proc.returncode,
           "stock_cached": cached}
    if proc.returncode == 2 or not (out / "verdict.json").is_file():
        res["status"] = "ERROR"
        res["error"] = (proc.stderr or proc.stdout)[-1500:]
        return res
    v = json.loads((out / "verdict.json").read_text())
    if not cached and all((out / f).is_file() for f in STOCK_FILES):
        cache.parent.mkdir(parents=True, exist_ok=True)
        tmp = Path(tempfile.mkdtemp(prefix=".tmp", dir=str(cache.parent)))
        for f in STOCK_FILES:
            shutil.copy2(out / f, tmp / f)
        try:
            os.rename(tmp, cache)
        except OSError:
            shutil.rmtree(tmp, ignore_errors=True)       # another run cached it first
    ncalls = sum(n for k, n in v.get("replaced_calls", {}).items() if k.startswith("draft="))
    res["calls_of_fn"] = ncalls
    res["captured_calls"] = (v.get("calls") or {}).get("calls")
    calls = v.get("calls") or {}
    if calls.get("verdict") == "DIFFERENT":
        res["status"] = "DIVERGES"
        res["first"] = calls
    elif v.get("verdict") == "DIFFERENT":
        res["status"] = "DIVERGES"
        res["frame_diff"] = v.get("first_diff")
    elif ncalls == 0:
        res["status"] = "NEVER CALLED"
    else:
        res["status"] = "EQUIVALENT"
    res["frames_judged"] = v.get("frames")
    return res


def _fmt_word(x):
    return "%s->%s" % tuple(x) if x else "unchanged"


def divergence_lines(res, name):
    """Human lines for a diverging script (retail vs draft)."""
    out = []
    c = res.get("first")
    if c:
        what = {"args": "arguments differ at entry", "return": "return value differs",
                "effect": "memory written differs", "sequence": "call sequence differs",
                "length": "one side stops calling"}.get(c.get("what"), c.get("what"))
        out.append("FIRST DIVERGENCE: script %s, frame %s, %s call #%s (global seq %s): %s" % (
            res["script"], c.get("frame"), c.get("name"), c.get("fn_call"), c.get("seq"), what))
        if c.get("inside"):
            out.append("  inside: " + " > ".join("%s call #%s" % (o["name"], o["fn_call"]) for o in c["inside"]))
        for label, a, b in c.get("fields", []):
            if label == "effect words:hash":
                label, a, b = "words written", a.split(":")[0], b.split(":")[0]
            out.append("  %s: retail %s | draft %s" % (label, a, b))
        words = c.get("words") or []
        for addr, a, b, where in words[:24]:
            out.append("  word %s: retail %s | draft %s  %s" % (addr, _fmt_word(a), _fmt_word(b), where))
        if len(words) > 24:
            out.append("  ... %d more words (runs/%s/oracle.txt)" % (len(words) - 24, res["script"]))
        if c.get("what") == "args" and c.get("fn_call") == 1:
            out.append("  note: arguments differ on the FIRST call: both sides' callers are retail, so the "
                       "prototype/capture registers are wrong (--params?) or an earlier effect leaked")
    else:
        fd = res.get("frame_diff") or {}
        if fd.get("events"):
            out.append("FIRST DIVERGENCE: script %s: overlay load/inflate events differ (no call differed)"
                       % res["script"])
        else:
            out.append("FIRST DIVERGENCE: script %s, frame %s: %s%d rdram page(s) differ although every call "
                       "of %s matched (memory outside the captured effects, e.g. a caller's stack frame or "
                       "another thread)" % (res["script"], fd.get("frame"),
                                            "display lists and " if fd.get("dl") else "",
                                            fd.get("pages_total") or 0, name))
            for addr, a, b, where in (fd.get("words") or [])[:16]:
                out.append("  word %s: retail %s | draft %s  %s" % (addr, a, b, where))
    return out


# ------------------------------------------------------------------ remote (Pi -> rocky)

_INC = re.compile(r'^\s*#\s*include\s*"([^"]+)"', re.M)


def local_files(draft):
    """The draft plus every file it includes with "..." that exists relative to
    the including file (recursively)."""
    seen, todo = [], [Path(draft).resolve()]
    while todo:
        f = todo.pop()
        if f in seen:
            continue
        seen.append(f)
        for inc in _INC.findall(f.read_text(errors="replace")):
            cand = (f.parent / inc).resolve()
            if cand.is_file():
                todo.append(cand)
    return seen


def remote_main(args, argv):
    host = args.remote
    draft = Path(args.draft).resolve()
    if not draft.is_file():
        raise DCError(f"no draft {args.draft}")
    files = local_files(draft)
    root = Path(os.path.commonpath([str(f.parent) for f in files]))
    inc_from = Path(args.include_from) if args.include_from else None
    if inc_from is None:
        top = subprocess.run(["git", "rev-parse", "--show-toplevel"], capture_output=True, text=True)
        cand = Path(top.stdout.strip()) / "include" if top.returncode == 0 else None
        inc_from = cand if cand and cand.is_dir() else REPO / "include"
    rid = "%s-%s-%s-%d" % (getpass.getuser(), args.name, time.strftime("%Y%m%d-%H%M%S"), os.getpid())
    with tempfile.TemporaryDirectory() as tmp:
        stage = Path(tmp) / rid
        for f in files:
            dst = stage / "src" / f.relative_to(root)
            dst.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(f, dst)
        shutil.copytree(inc_from, stage / "include")
        rc = subprocess.run(["ssh", host, "mkdir -p %s" % REMOTE_INBOX]).returncode
        if rc == 0:
            rc = subprocess.run(["rsync", "-a", str(stage), "%s:%s/" % (host, REMOTE_INBOX)]).returncode
        if rc != 0:
            raise DCError(f"cannot copy the draft to {host}:{REMOTE_INBOX}")
    rel = draft.relative_to(root)
    passthrough = []
    skip_next = False
    for i, a in enumerate(argv):
        if skip_next:
            skip_next = False
            continue
        if a in ("--remote", "--remote-tree", "--include-from"):
            skip_next = True
            continue
        if a.startswith(("--remote=", "--remote-tree=", "--include-from=")):
            continue
        passthrough.append(a)
    # replace the local draft path (positional 2) with the inbox copy
    remote_draft = "$HOME/%s/%s/src/%s" % (REMOTE_INBOX, rid, rel)
    pos = [i for i, a in enumerate(passthrough) if a == args.draft]
    if not pos:
        raise DCError("cannot find the draft argument to forward")
    parts = [shlex.quote(a) for a in passthrough]
    parts[pos[0]] = '"%s"' % remote_draft
    cmd = "cd %s && python3 -m tools.recomp.draft_check %s --include-dir \"$HOME/%s/%s/include\" --origin %s" % (
        args.remote_tree, " ".join(parts), REMOTE_INBOX, rid, shlex.quote(str(draft)))
    print("draft_check: running on %s (inbox %s/%s)" % (host, REMOTE_INBOX, rid), flush=True)
    return subprocess.run(["ssh", host, cmd]).returncode


# ------------------------------------------------------------------ main

def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    ap = argparse.ArgumentParser(prog="python3 -m tools.recomp.draft_check", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("name", help="function to replace (inventory name, game or static)")
    ap.add_argument("draft", help="C file defining NAME (a /* flags: ... */ header picks the IDO flags)")
    ap.add_argument("--scripts", default=None,
                    help="comma-separated tools/recomp/scripts names, 'all', or 'reached' (every script the census "
                    "says calls NAME); default: the 6 census scripts with the most calls of NAME, all scripts "
                    "when NAME is not in the census")
    ap.add_argument("--params", default=None,
                    help="register parameters of NAME itself: PARAM=REG,... for every prototype parameter "
                    "(IDO -O3 IPA functions; e.g. handle=a0,mark=t3)")
    ap.add_argument("--callee", action="append", default=[],
                    help="register convention of a callee the draft calls: 'PROTOTYPE=REG,...[/CLOBBER,...]' "
                    "(adds to tools/recomp/ipa_callees.json for this run)")
    ap.add_argument("--ignore-register-params", action="store_true",
                    help="build a function the frontier lists with register_params without --params "
                    "(o32 inbound: only right if those registers are not really parameters)")
    ap.add_argument("--statics", default=None, help="NAME=0xADDR,... function-local statics -> retail addresses")
    ap.add_argument("--compiler", default=None, help="ido5.3 | ido7.1 (default by population)")
    ap.add_argument("--frames", type=int, default=None, help="frames per script (default: the script's)")
    ap.add_argument("--jobs", type=int, default=int(os.environ.get("RUSH_DC_JOBS", "6")),
                    help="scripts run in parallel (default 6; each run is 1-3 processes)")
    ap.add_argument("--keep", action="store_true", help="keep the run directory's host/ROM copies")
    ap.add_argument("--json", default=None, help="also write the summary JSON here")
    ap.add_argument("--remote", default=None, help="run on this ssh host (e.g. rocky) from the Pi")
    ap.add_argument("--remote-tree", default=REMOTE_TREE, help="service tree on the remote (from its home)")
    ap.add_argument("--include-from", default=None, help="include/ to ship with --remote (default: the "
                    "current checkout's)")
    ap.add_argument("--include-dir", action="append", default=[], help=argparse.SUPPRESS)
    ap.add_argument("--origin", default=None, help=argparse.SUPPRESS)
    args = ap.parse_args(argv)
    try:
        if args.remote:
            return remote_main(args, argv)
        return local_main(args)
    except (DCError, rp.ReplaceError, oracle.OracleError) as exc:
        print("draft_check: ERROR: %s" % exc, file=sys.stderr)
        return 2


def local_main(args):
    p = paths()
    for k in ("rom", "n64recomp", "rsprecomp", "runtime", "ido53"):
        if not Path(p[k]).exists():
            raise DCError(f"{k} not found at {p[k]}: run on the recomp host (rocky) or pass --remote rocky")
    draft = Path(args.draft).resolve()
    if not draft.is_file():
        raise DCError(f"no draft {args.draft}")
    name = args.name
    text = draft.read_text(errors="replace")
    try:
        proto = rp.find_prototype(text, name)
    except rp.ReplaceError:
        raise DCError(f"{draft.name} has no definition of {name}")
    pop = find_population(name)
    entry = {"function": name, "population": pop, "source": str(draft), "mode": "ido", "state": "draft",
             "allow_ipa": True, "note": "draft_check run (NOT a match)"}
    if args.include_dir:
        entry["include_dirs"] = [str(Path(d).resolve()) for d in args.include_dir]
    if args.compiler:
        entry["compiler"] = args.compiler
    if args.statics:
        entry["statics"] = {k: int(v, 16) for k, v in (x.split("=") for x in args.statics.split(",") if x)}
    table = rp.load_ipa_table()
    regs_fact = register_params_fact(name)
    if args.params:
        cap, regs = parse_params(args.params, proto)
        ret = rp._lenient_type(rp.parse_prototype(proto)[0])
        entry["capture_args"] = cap
        entry["capture_ret"] = "void" if ret["c"] == "void" else ("f0" if ret["float"] else "v0")
        table[name] = {"prototype": proto, "params": regs, "evidence": "draft_check --params (lane hypothesis)"}
    elif name in table and "params" in table[name]:
        # committed ipa_callees.json convention of NAME: inbound shim + capture of those registers
        tproto = table[name]["prototype"]
        tnames = [pn for _t, pn in rp.parse_prototype(tproto)[2]]
        entry["capture_args"] = ["%s=%s" % (pn, r) for pn, r in zip(tnames, table[name]["params"])]
        ret = rp._lenient_type(rp.parse_prototype(tproto)[0])
        entry["capture_ret"] = "void" if ret["c"] == "void" else ("f0" if ret["float"] else "v0")
    elif regs_fact and not args.ignore_register_params:
        raise DCError(f"{name} takes register parameters ({','.join(regs_fact)}, build/frontier.json facts): "
                      f"pass --params PARAM=REG,... for every parameter of `{proto}` (e.g. "
                      f"--params {','.join('%s=%s' % (pn, 'REG') for _t, pn in rp.parse_prototype(proto)[2])}); "
                      "prove the mapping with python3 -m tools.recomp.regmap_verify")
    auto = clobber_shims(table)
    auto.pop(name, None)             # NAME itself: inbound only through --params
    table.update(auto)
    for spec in args.callee:
        cname, ce = parse_callee(spec)
        table[cname] = ce
    run_dir = DC_HOME / "runs" / name / ("%s-%d" % (time.strftime("%Y%m%d-%H%M%S"), os.getpid()))
    run_dir.mkdir(parents=True, exist_ok=True)
    manifest = run_dir / "replacements.json"
    manifest.write_text(json.dumps({"note": "draft_check", "entries": {"draft": entry},
                                    "sets": {"draft": ["draft"]}}, indent=1))
    ipa_path = run_dir / "ipa_callees.json"
    ipa_path.write_text(json.dumps({"note": "draft_check: committed table + run options",
                                    "callees": table}, indent=1))
    print("draft_check %s <- %s%s" % (name, args.origin or draft, " [%s]" % pop if pop != "game" else ""))
    print("  draft sha256 %s; run dir %s" % (hashlib.sha256(draft.read_bytes()).hexdigest()[:16], run_dir))
    if args.scripts in (None, "census", "reached"):
        scripts, covered = census_scripts(name)
        if covered and not scripts:
            how = "census: no script calls %s" % name
        elif covered and args.scripts != "reached" and len(scripts) > DEFAULT_MAX_SCRIPTS:
            how = "census: %d scripts call %s; the %d with the most calls (--scripts reached: all %d)" % (
                len(scripts), name, DEFAULT_MAX_SCRIPTS, len(scripts))
            scripts = scripts[:DEFAULT_MAX_SCRIPTS]
        elif covered:
            how = "census: %d script(s) call %s" % (len(scripts), name)
        else:
            scripts, how = all_scripts(), "not in the census: all scripts"
    elif args.scripts == "all":
        scripts, how = all_scripts(), "all scripts"
    else:
        scripts, how = [s.strip() for s in args.scripts.split(",") if s.strip()], "chosen"
        for s in scripts:
            if not (SCRIPT_DIR / (s + ".press")).is_file():
                raise DCError(f"no script {s} (have: {', '.join(all_scripts())})")
    print("  scripts (%s): %s" % (how, ", ".join(scripts) or "none"), flush=True)
    t0 = time.time()
    with ThreadPoolExecutor(2) as ex:
        fc = ex.submit(build_side, "cap", name, manifest, run_dir, p, None, 8)
        fr = ex.submit(build_side, "repl", name, manifest, run_dir, p, ipa_path, 8)
        cap, repl = fc.result(), fr.result()
    man = json.loads(Path(repl["manifest"]).read_text())
    rinfo = next((r for r in man.get("replacements", {}).get("replaced", []) if r.get("id") == "draft"), {})
    warn = [ln for ln in Path(repl["gen_log"]).read_text(errors="replace").splitlines()
            if "WARNING" in ln or "ipa note" in ln or "ipa shim" in ln or "ipa body" in ln]
    print("  built in %.0f s (draft compiled with %s)" % (time.time() - t0, entry.get("compiler") or
                                                          rp.DEFAULT_COMPILER[pop]))
    for ln in warn:
        print("  " + ln.strip())
    if rinfo.get("ipa_inbound"):
        print("  inbound register shim: %s" % table[name]["params"])
    if rinfo.get("private_data"):
        print("  NOTE private data (not bound to retail): %s" % ", ".join(rinfo["private_data"]))
    if rinfo.get("statics_private"):
        print("  NOTE function-local statics kept private (pass --statics NAME=0xADDR): %s"
              % ", ".join(rinfo["statics_private"]))
    cap_key = (sha256_file(cap["host"])[:12] + sha256_file(cap["rom"])[:8])
    results = []
    if scripts:
        with ThreadPoolExecutor(max(1, args.jobs)) as ex:
            futs = [ex.submit(run_script, s, name, cap, repl, run_dir, cap_key, args.frames) for s in scripts]
            for f in futs:
                r = f.result()
                results.append(r)
                tag = r["status"]
                extra = ""
                if tag in ("EQUIVALENT", "DIVERGES"):
                    extra = "%d calls of %s" % (r["calls_of_fn"], name)
                elif tag == "ERROR":
                    extra = r["error"].strip().splitlines()[-1] if r["error"].strip() else "?"
                print("  %-14s %-12s %s  (%d frames, %.0f s%s)" % (r["script"], tag, extra, r["frames"],
                                                                 r["seconds"], ", retail cached"
                                                                 if r.get("stock_cached") else ""), flush=True)
    div = [r for r in results if r["status"] == "DIVERGES"]
    eq = [r for r in results if r["status"] == "EQUIVALENT"]
    never = [r for r in results if r["status"] == "NEVER CALLED"]
    err = [r for r in results if r["status"] == "ERROR"]
    summary = {"function": name, "draft": str(args.origin or draft), "scripts": results, "run_dir": str(run_dir)}
    print()
    if div:
        for line in divergence_lines(div[0], name):
            print(line)
        for r in div[1:]:
            c = r.get("first") or {}
            print("  also diverges: %s (frame %s, call #%s, %s)" % (r["script"], c.get("frame"), c.get("fn_call"),
                                                                   c.get("what", "frame pages")))
        print("VERDICT: DIVERGES (%d of %d scripts; behaviour differs from retail: the logic is wrong, or a "
              "harness gap named above)" % (len(div), len(results)))
        summary["verdict"], rc = "DIVERGES", 1
    elif err:
        for r in err:
            print("ERROR in %s:\n%s" % (r["script"], r["error"]))
        summary["verdict"], rc = "ERROR", 2
    elif eq:
        n = sum(r["calls_of_fn"] for r in eq)
        print("VERDICT: EQUIVALENT over %d calls / %d scripts (%s)%s" % (
            n, len(eq), ", ".join("%s %d" % (r["script"], r["calls_of_fn"]) for r in eq),
            "; never called in: " + ", ".join(r["script"] for r in never) if never else ""))
        print("  Behaviour only: every call's arguments, return value and memory writes match retail. This is "
              "NOT a match and NOT coverage (score.py / blob_unit decide that); never put it in cloud/matches.")
        summary["verdict"], rc = "EQUIVALENT", 0
    else:
        print("VERDICT: NEVER CALLED: function %s never called in these scripts (%s); the draft built but was "
              "not exercised. See tools/recomp/census/ for what the scripts reach." % (
                  name, ", ".join(scripts) or "none"))
        summary["verdict"], rc = "NEVER CALLED", 3
    summary["calls"] = sum(r.get("calls_of_fn") or 0 for r in results)
    (run_dir / "summary.json").write_text(json.dumps(summary, indent=1, default=str) + "\n")
    if args.json:
        Path(args.json).write_text(json.dumps(summary, indent=1, default=str) + "\n")
    if not args.keep:
        for kind in ("cap", "repl"):
            for f in ("rush2049_recomp", "rush2049.recomp.z64"):
                try:
                    (run_dir / kind / f).unlink()
                except OSError:
                    pass
        for r in results:
            for f in ("home_s0", "home_s1", "home_r", "home_cs", "home_cr", "home_ds", "home_dr"):
                shutil.rmtree(run_dir / "runs" / r["script"] / f, ignore_errors=True)
    print("  details: %s" % run_dir)
    return rc


if __name__ == "__main__":
    sys.exit(main())
