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
# saves_reload needs the pak image saves.press wrote (--pak FILE); not runnable alone. Phase 9:
# player_reload.press is the same flow with `pak from saves` (the harness makes the image).
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
    f = rp.frontier_facts(frontier)[0].get(name) or {}       # keyed by today's names (Phase 10)
    return [r for r in f.get("register_params") or () if r not in rp._ABI_IN]


GENERIC_STACK = tuple(range(16, 48, 4))


def retail_clobbers(text, p=None, register_params=False):
    """{callee: [callee-saved registers it writes without a save]} for the game
    functions a draft's text names, from the RETAIL bodies (regmap_verify's
    interprocedural effects, transitive through static callees). Phase 11
    (w31r): build/frontier.json has unsaved_callee_regs only for the functions
    its scan covered; a MATCHED IPA callee such as func_800DCD58 (writes s0,s1)
    had none, got no clobber shim, and w29c/w30d's control_settings drafts
    faulted on the s0 it destroyed (`lb a0,1(s0)`, s0 = 0). Callees with
    non-o32 register parameters are left out (they need a real table entry)
    unless register_params (Phase 12: their table entries' clobbers are
    widened with the retail set)."""
    import ipa_detect
    import regmap_verify as rv
    try:
        image, funcs = rv.load_inputs(p or paths())
    except Exception:                 # noqa: BLE001 -- no ROM here: frontier facts only
        return {}
    words = set(re.findall(r"[A-Za-z_][A-Za-z0-9_]*", text))
    ip = rv.Interproc(image, funcs)
    out = {}
    for f in funcs:
        if f["name"] not in words:
            continue
        body = ipa_detect.body_words(image, f["vram"], f["size"])
        if not body or (not register_params and ipa_detect.register_params(body)):
            continue
        regs = ip.unsaved(f["vram"])
        if regs:
            out[f["name"]] = regs
    return out


def clobber_shims(table, frontier=REPO / "build" / "frontier.json", retail=None):
    """ipa_callees entries for callees that take o32 arguments but write
    callee-saved registers without saving them (frontier facts
    unsaved_callee_regs, no register_params): a shim that passes a0-a3 and
    eight stack words through unchanged (f12/f14 are not touched) and saves
    the clobbered registers around the call, so the draft's s-registers
    survive as they would in the whole-program build."""
    facts = dict(rp.frontier_facts(frontier)[0])       # keyed by today's names (Phase 10)
    for name, regs in (retail or {}).items():          # Phase 11: retail-derived, for callees the scan lacks
        f = dict(facts.get(name) or {})
        f["unsaved_callee_regs"] = sorted(set(f.get("unsaved_callee_regs") or ()) | set(regs))
        f["_retail"] = True
        facts[name] = f
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
                     "evidence": "draft_check: %s (o32 pass-through + saves)" % (
                         "retail unsaved callee-saved writes (regmap_verify.Interproc)" if f.get("_retail")
                         else "frontier unsaved_callee_regs")}
    return out


OVL_NAME_RE = re.compile(r"^(ovl_[ab])[:_](\w+)$")


def split_overlay_name(name):
    """'ovl_a:NAME' / 'ovl_a_NAME' -> ('ovl_a', 'NAME'); anything else -> (None, name)."""
    m = OVL_NAME_RE.match(name)
    return (m.group(1), m.group(2)) if m else (None, name)


def widen_clobbers(table, retail, members=()):
    """Phase 12 (w32r): widen each ipa_callees table entry's clobbers with the
    retail body's unsaved callee-saved set (func_800DD0C0's hand-written entry
    lacked s0,s1,s3: the control_settings pak_nopak fault). Draft members are
    left alone. Returns [(callee, added registers)]; updates table in place."""
    out = []
    for cname, regs in sorted(retail.items()):
        e = table.get(cname)
        if e is None or cname in members:
            continue
        extra = [r for r in regs if r not in (e.get("clobbers") or ())]
        if extra:
            table[cname] = dict(e, clobbers=list(e.get("clobbers") or ()) + extra)
            out.append((cname, extra))
    return out


def find_population(name, game_functions=None, image=None):
    """'game', 'static' or (Phase 12) 'ovl_a' / 'ovl_b' for an overlay function.
    Both overlay images load at 0x8038A400, so a func_XXXXXXXX name can exist in
    both: then the image must be named (`ovl_b:NAME`)."""
    sys.path.insert(0, str(HERE))
    import gen_config as gc
    if image is None:
        game = game_functions if game_functions is not None else gc.load_game_functions(REPO / "asm" / "us" / "blob")
        if any(f.get("name") == name for f in game):
            return "game"
    hits = []
    for ovl in ((image,) if image else ("ovl_a", "ovl_b")):
        p = REPO / "asm" / "us" / ovl / "extents.json"
        if p.is_file() and any(f.get("name") == name for f in gc.load_extents_functions(p)):
            hits.append(ovl)
    if len(hits) > 1:
        raise DCError(f"{name} exists in both overlay images: name one (ovl_a:{name} or ovl_b:{name})")
    if hits:
        return hits[0]
    if image:
        raise DCError(f"{name} is not a function of {image} (asm/us/{image}/extents.json)")
    return "static"


def overlay_census_scripts(pop, name, census_dir=CENSUS_DIR):
    """census_scripts for an overlay function, from census/overlays.json (Phase 11
    `census --overlays`; records named ovl_a_NAME)."""
    try:
        doc = json.loads((census_dir / "overlays.json").read_text())
    except (OSError, ValueError):
        return None, False
    rec = doc.get("functions", {}).get("%s_%s" % (pop, name))
    if rec is None:
        return None, False
    calls = rec.get("scripts", {})
    return sorted(calls, key=lambda s: (-calls[s], s)), True


def member_params(specs, members, unit):
    """--params values -> {member: 'p=r,...'}; a unit needs 'MEMBER:' prefixes."""
    out = {}
    for spec in specs:
        if unit:
            m, sep, rest = spec.partition(":")
            if not sep or m.strip() not in members:
                raise DCError(f"--unit --params: {spec!r} must start with one of the members ({', '.join(members)}:)")
            out[m.strip()] = rest
        else:
            if members[0] in out:
                raise DCError("--params given twice")
            out[members[0]] = spec
    return out


def census_scripts_for(members, census_dir=CENSUS_DIR):
    """census_scripts over several functions: scripts ordered by their calls of any member."""
    if len(members) == 1:
        return census_scripts(members[0], census_dir)
    try:
        doc = json.loads((census_dir / "census.json").read_text())
    except (OSError, ValueError):
        return None, False
    fns = doc.get("functions", {})
    if not any(m in fns for m in members):
        return None, False
    tot = {}
    for m in members:
        for sc, n in (fns.get(m) or {}).get("scripts", {}).items():
            tot[sc] = tot.get(sc, 0) + n
    return sorted(tot, key=lambda sc: (-tot[sc], sc)), True


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
    per = {}
    for k, n in v.get("replaced_calls", {}).items():
        rid, _, rest = k.partition("=")
        if rid == "draft" or rid.startswith("draft__"):
            per[rest.partition(":")[2] or rid] = per.get(rest.partition(":")[2] or rid, 0) + n
    ncalls = sum(per.values())
    res["calls_of_fn"] = ncalls
    res["calls_per_member"] = per
    res["captured_calls"] = (v.get("calls") or {}).get("calls")
    calls = v.get("calls") or {}
    if v.get("crash") is not None:
        # Phase 9: the draft's host faulted (the stock runs did not): a divergence
        # with the fault's N64 address, PC, call chain (oracle.fault_report)
        res["crash"] = v["crash"]
        if not ncalls and calls.get("fn_call"):
            res["calls_of_fn"] = calls.get("fn_call")
    if calls.get("verdict") == "DIFFERENT":
        res["status"] = "DIVERGES"
        res["first"] = calls
    elif v.get("crash") is not None:
        res["status"] = "DIVERGES"
        res["first"] = {"what": "crash", "frame": v["crash"].get("frame")}
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


def divergence_lines(res, name, members=None):
    """Human lines for a diverging script (retail vs draft); `members`: a unit."""
    out = []
    c = res.get("first")
    member = str(c.get("name") or "").split(" ")[0] if c else ""
    if c and members and member in members:
        out.append("FIRST DIVERGING MEMBER: %s (unit %s)" % (member, ", ".join(members)))
    if c:
        what = {"args": "arguments differ at entry", "return": "return value differs",
                "effect": "memory written differs", "sequence": "call sequence differs",
                "length": "one side stops calling",
                "crash": "the draft FAULTED (host signal; retail did not)"}.get(c.get("what"), c.get("what"))
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
        if res.get("crash") is not None:
            if c.get("what") != "crash":
                out.append("  later the draft FAULTED at frame %s:" % res["crash"].get("frame"))
            out += oracle.fault_lines(res["crash"]) if res["crash"].get("signal") else [
                "  (no [rush] FAULT lines: rebuild the hosts with this tools revision)"]
            if c.get("what") == "crash":
                out.append("  note: a wild N64 address (outside 0x80000000-0x80FFFFFF) is a TLB exception on the "
                           "console too: read the faulting instruction's base register back to the draft line that "
                           "loaded it (a wrong field offset or stride is the usual cause)")
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


# ------------------------------------------------------------------ cleanup (Phase 8: --gc)

GC_KEEP_TREES = int(os.environ.get("RUSH_DC_KEEP", "12"))      # function build trees kept (most recently used)
GC_DAYS = float(os.environ.get("RUSH_DC_DAYS", "3"))           # runs / verify / probe / census / inbox kept (days)
GC_KEEP_RUNS = 5                                               # newest run dirs kept per function regardless of age


def _du(path):
    total = 0
    for root, _dirs, files in os.walk(path):
        for f in files:
            try:
                total += os.lstat(os.path.join(root, f)).st_size
            except OSError:
                pass
    return total


def _tree_used(fn_dir):
    """Last use of a function build tree: newest mtime of its cap/repl `lock`
    (build_side opens it for writing on every build)."""
    ts = [(fn_dir / k / "lock").stat().st_mtime for k in ("cap", "repl") if (fn_dir / k / "lock").is_file()]
    return max(ts) if ts else fn_dir.stat().st_mtime


def _busy(fn_dir):
    """True when a build holds a tree lock right now."""
    for k in ("cap", "repl"):
        lk = fn_dir / k / "lock"
        if not lk.is_file():
            continue
        with open(lk, "a") as fh:
            try:
                fcntl.flock(fh, fcntl.LOCK_EX | fcntl.LOCK_NB)
                fcntl.flock(fh, fcntl.LOCK_UN)
            except OSError:
                return True
    return False


def gc_plan(dc_home=None, keep=None, days=None, now=None):
    """[(path, why)] to delete under dc_home. Policy (README "draft_check"
    cleanup): build trees (dc/trees/<checkout>/<NAME>, ~250 MB each): keep the
    `keep` most recently used over all checkouts, never one being built; retail
    log caches (dc/cache/<NAME>): drop a function whose tree is gone, and keep
    only the newest retail binary key otherwise; run dirs (dc/runs/<NAME>/*):
    keep the newest GC_KEEP_RUNS per function and any younger than `days`;
    verify/, probe/, census/ runs and inbox/ drafts older than `days`."""
    dc_home = Path(dc_home or DC_HOME)
    keep = GC_KEEP_TREES if keep is None else keep
    days = GC_DAYS if days is None else days
    now = time.time() if now is None else now
    old = now - days * 86400
    out = []
    trees = [d for c in sorted((dc_home / "trees").glob("*")) if c.is_dir() for d in sorted(c.iterdir())
             if d.is_dir()]
    trees.sort(key=_tree_used, reverse=True)
    kept_names = set()
    for i, d in enumerate(trees):
        if i < keep or _busy(d):
            kept_names.add(d.name)
        else:
            out.append((d, "build tree #%d by last use (keep %d)" % (i + 1, keep)))
    for d in sorted((dc_home / "cache").glob("*")):
        if not d.is_dir():
            continue
        if d.name not in kept_names:
            out.append((d, "retail log cache of a collected tree"))
            continue
        keys = sorted((k for k in d.iterdir() if k.is_dir()), key=lambda k: k.stat().st_mtime, reverse=True)
        out += [(k, "retail log cache of an older retail build") for k in keys[1:]]
    for d in sorted((dc_home / "runs").glob("*")):
        if not d.is_dir():
            continue
        runs = sorted((r for r in d.iterdir() if r.is_dir()), key=lambda r: r.stat().st_mtime, reverse=True)
        out += [(r, "run dir older than %g days (newest %d kept)" % (days, GC_KEEP_RUNS))
                for r in runs[GC_KEEP_RUNS:] if r.stat().st_mtime < old]
    for sub in ("verify", "probe", "census", "inbox"):
        for d in sorted((dc_home / sub).glob("*")):
            if sub == "verify" and d.is_dir():
                cands = [r for r in d.iterdir() if r.is_dir()]
            else:
                cands = [d]
            out += [(r, "%s entry older than %g days" % (sub, days)) for r in cands if r.stat().st_mtime < old]
    return out


def gc_main(args):
    plan = gc_plan(keep=args.keep_trees, days=args.gc_days)
    freed = 0
    for path, why in plan:
        n = _du(path)
        freed += n
        print("  %s %s (%.0f MB): %s" % ("would delete" if args.dry_run else "delete", path, n / 1e6, why))
        if not args.dry_run:
            shutil.rmtree(path, ignore_errors=True)
    left = _du(DC_HOME) if DC_HOME.is_dir() else 0
    print("draft_check --gc: %d entr%s, %.1f GB %s; %s now %.1f GB" % (
        len(plan), "y" if len(plan) == 1 else "ies", freed / 1e9, "to free" if args.dry_run else "freed",
        DC_HOME, left / 1e9))
    return 0


def auto_gc():
    """After a run: collect when the trees exceed the keep count by 4 (cheap check)."""
    try:
        n = sum(1 for c in (DC_HOME / "trees").glob("*") for d in c.iterdir() if d.is_dir())
        if n > GC_KEEP_TREES + 4:
            for path, _why in gc_plan():
                shutil.rmtree(path, ignore_errors=True)
    except OSError:
        pass


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


def _strip_remote(argv):
    out, skip = [], False
    for a in argv:
        if skip:
            skip = False
            continue
        if a in ("--remote", "--remote-tree", "--include-from"):
            skip = True
            continue
        if not a.startswith(("--remote=", "--remote-tree=", "--include-from=")):
            out.append(a)
    return out


def probe_callees_main(args):
    """--probe-callees: regmap_verify --derive for each named callee."""
    import regmap_verify as rv
    rc = 0
    for callee in [c.strip() for c in args.probe_callees.split(",") if c.strip()]:
        argv = [callee, "--derive"] + (["--static-only"] if args.static_only else []) + \
               (["--scripts", args.scripts] if args.scripts else [])
        rc = max(rc, rv.main(argv))
        print()
    return rc


def suggest_callees(names, out=print):
    """Derived --callee lines for register-parameter callees the table lacks
    (static evidence only: cheap; printed under the WARNING line)."""
    try:
        import regmap_verify as rv
        image, funcs = rv.load_inputs(paths())
    except Exception as exc:          # noqa: BLE001 -- advice only
        out("  (no --callee suggestion: %s)" % exc)
        return
    for n in names:
        try:
            d = rv.derive(n, image, funcs, None, rv.unsaved_fact(n))
        except Exception:             # noqa: BLE001
            continue
        regs = [r for r, _k in d["params"]]
        if all(r in ("a0", "a1", "a2", "a3", "f12", "f14") for r in regs) and not d["clobbers"]:
            continue
        out("  suggested (derived; order by register number, check it): --callee '%s'" % rv.callee_spec(d))


# ------------------------------------------------------------------ lane summary (Phase 9)

LANE_ROOT = REPO / "cloud" / "work" / "frontier"


def lane_drafts(lane, root=LANE_ROOT):
    """[(name, best.c, extra args)] for every <lane>/<NAME>/best.c (NAME = the
    directory: the function, or for a unit its first member). A `dc.args` file
    next to best.c holds extra draft_check arguments (one line, shell syntax:
    --unit A,B,C / --params / --callee / --scripts ...); with --unit its NAME
    list replaces the directory name, and `--name NAME` names a single draft
    whose directory is not its function (w30a/audio_options holds
    game_results_render)."""
    d = Path(lane)
    if not d.is_dir():
        d = Path(root) / lane
    if not d.is_dir():
        raise DCError(f"no lane directory {lane} (looked in {root})")
    out = []
    for best in sorted(d.glob("*/best.c")):
        extra = []
        f = best.parent / "dc.args"
        if f.is_file():
            extra = shlex.split(" ".join(ln for ln in f.read_text().splitlines() if not ln.lstrip().startswith("#")))
        name = best.parent.name
        if "--name" in extra:              # Phase 11: a single draft in a directory not named after it
            i = extra.index("--name")
            if i + 1 < len(extra):
                name = extra[i + 1]
                del extra[i:i + 2]
        if "--unit" in extra:
            i = extra.index("--unit")
            if i + 1 < len(extra) and not extra[i + 1].startswith("-"):
                name = extra.pop(i + 1)
        out.append((name, best, extra))
    return out


def current_names(name, extra, stale=None):
    """Phase 10: a lane directory and its dc.args keep the function names of
    the day they were written; renames (wave 29, w29n) changed the draft text
    but not those. Map NAME (a comma list for --unit), `--params MEMBER:...`
    prefixes and the function names in `--callee` prototypes to today's names.
    Returns (name, extra, [(old, new)])."""
    stale = rp.stale_function_names() if stale is None else stale
    done = []

    def cur(n):
        if n in stale:
            done.append((n, stale[n]))
            return stale[n]
        return n
    name = ",".join(cur(n) for n in name.split(","))
    out, it = [], iter(extra)
    for a in it:
        if a in ("--params", "--callee"):
            v = next(it, "")
            if a == "--params" and ":" in v.split("=", 1)[0]:
                m, rest = v.split(":", 1)
                v = cur(m) + ":" + rest
            elif a == "--callee":
                v = re.sub(r"(\w+)(\s*\()", lambda mm: cur(mm.group(1)) + mm.group(2), v, count=1)
            out += [a, v]
        else:
            out.append(a)
    return name, out, sorted(set(done))


def summarize_output(text, rc):
    """(verdict, detail) from one draft_check run's stdout."""
    verdict = {0: "EQUIVALENT", 1: "DIVERGES", 3: "NEVER CALLED"}.get(rc, "ERROR")
    detail = ""
    for line in text.splitlines():
        if line.startswith("VERDICT: EQUIVALENT"):
            detail = line[len("VERDICT: EQUIVALENT "):]
        elif line.startswith("FIRST DIVERGENCE:") and not detail:
            detail = line[len("FIRST DIVERGENCE: "):]
        elif line.startswith("  fault:") and verdict == "DIVERGES" and "FAULTED" in detail:
            detail += " |" + line[len("  fault:"):]
    if verdict == "ERROR":
        tail = [ln.strip() for ln in text.strip().splitlines() if ln.strip()]
        detail = tail[-1] if tail else "?"
    return verdict, detail


def forwarded_options(argv, valued=("--lane", "--json"), optional=("--all-lanes",),
                      flags=("--refresh", "--include-matched")):
    """The command line minus the summary-mode options (what applies to every
    draft): `valued` options drop their value, `optional` ones their value when
    the next word is not an option, `flags` drop alone."""
    fwd, i = [], 0
    while i < len(argv):
        a = argv[i]
        if a in valued:
            i += 2
            continue
        if a in optional:
            i += 2 if i + 1 < len(argv) and not argv[i + 1].startswith("-") else 1
            continue
        if a in flags or a.startswith(tuple(o + "=" for o in valued + optional)):
            i += 1
            continue
        fwd.append(a)
        i += 1
    return fwd


MATCHED_LOCK = REPO / "blob_matched.lock.json"


OVL_MATCHED_LOCK = REPO / "ovl_matched.lock.json"


def matched_functions(lock=MATCHED_LOCK, ovl_lock=OVL_MATCHED_LOCK):
    """Game names in blob_matched.lock.json, plus (Phase 12) the overlay locks'
    functions as ovl_a:NAME / ovl_a_NAME (draft names of overlay drafts)."""
    out = set()
    try:
        out |= set(json.loads(Path(lock).read_text()))
    except (OSError, ValueError):
        pass
    try:
        doc = json.loads(Path(ovl_lock).read_text())
    except (OSError, ValueError):
        doc = {}
    for img in ("a", "b"):
        for nm in doc.get(img) or {}:
            out |= {"ovl_%s:%s" % (img, nm), "ovl_%s_%s" % (img, nm)}
    return out


LANES_CACHE = DC_HOME / "lanes" / "cache.json"


# Bump when a harness change can change a verdict for an unchanged draft (a shim, capture or comparison fix):
# every cached --all-lanes row is then rerun. Draft, dc.args, scripts and ipa_callees.json are in the key anyway.
CACHE_EPOCH = "w32r-1"


def key_scripts(name, opts):
    """The scripts a draft_check run with these options would use (the same
    choice as local_main) plus every script they start from (`pak from`)."""
    spec = None
    for i, a in enumerate(opts):
        if a == "--scripts" and i + 1 < len(opts):
            spec = opts[i + 1]
        elif a.startswith("--scripts="):
            spec = a.split("=", 1)[1]
    if spec in (None, "census", "reached"):
        scripts, covered = census_scripts_for(name.split(","))
        if not covered:
            scripts = all_scripts()
        elif spec != "reached":
            scripts = scripts[:DEFAULT_MAX_SCRIPTS]
    elif spec == "all":
        scripts = all_scripts()
    else:
        scripts = [x.strip() for x in spec.split(",") if x.strip()]
    out, todo = [], list(scripts)
    while todo:
        sc = todo.pop(0)
        if sc in out:
            continue
        out.append(sc)
        try:
            frm = oracle.load_script(sc).get("pak_from")
        except oracle.OracleError:
            frm = None
        if frm:
            todo.append(frm)
    return out


def draft_key(name, best, extra, fwd):
    """Cache key of one summary row (Phase 11): the draft and every file it
    includes, NAME, its dc.args options, the forwarded options (not --jobs),
    the text of every script the run uses (key_scripts: the census default
    choice included), the ipa_callees.json entries the draft names, and
    CACHE_EPOCH."""
    h = hashlib.sha256(CACHE_EPOCH.encode())
    text = b""
    for f in local_files(best):
        data = f.read_bytes()
        text += data + b"\n"
        h.update(str(f.name).encode() + b"\0" + data + b"\0")
    opts, it = [], iter(fwd)
    for a in it:                       # parallelism and run-dir housekeeping do not change a verdict
        if a in ("--jobs", "--remote-tree"):
            next(it, None)
        elif not a.startswith(("--jobs=", "--keep")):
            opts.append(a)
    h.update(json.dumps([name, extra, opts]).encode())
    files = [SCRIPT_DIR / (sc + ".press") for sc in key_scripts(name, list(extra) + list(fwd))]
    for f in files:
        h.update(f.name.encode() + b"\0" + (f.read_bytes() if f.is_file() else b"missing"))
    # only the ipa_callees.json entries the draft can use (named in its text, or NAME itself): a new entry for
    # an unrelated callee does not invalidate every row
    try:
        table = json.loads((HERE / "ipa_callees.json").read_text()).get("callees", {})
    except (OSError, ValueError):
        table = {}
    words = set(re.findall(rb"[A-Za-z_][A-Za-z0-9_]*", text)) | {n.encode() for n in name.split(",")}
    used = {n: e for n, e in table.items() if n.encode() in words}
    h.update(json.dumps(used, sort_keys=True).encode())
    return h.hexdigest()[:24]


def _load_cache(path):
    try:
        return json.loads(Path(path).read_text())
    except (OSError, ValueError):
        return {}


def _save_cache(path, key, entry):
    """Add one row under a lock, merging with what other --all-lanes runs wrote meanwhile."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(str(path) + ".lock", "w") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        cache = _load_cache(path)
        cache[key] = entry
        tmp = path.with_suffix(".tmp")
        tmp.write_text(json.dumps(cache, indent=1, sort_keys=True) + "\n")
        os.replace(str(tmp), str(path))


def check_drafts(drafts, fwd, label, json_path=None, cache_path=None, refresh=False, include_matched=False):
    """Run draft_check on [(lane, name, best.c, extra)], one line per draft, then
    one Markdown table and the verdict counts. A function in
    blob_matched.lock.json is MATCHED (not run: its draft is history) unless
    include_matched. With cache_path, a row whose key (draft_key) is cached is
    reused (`cached` column) instead of rebuilt and rerun."""
    rows = []
    stale = rp.stale_function_names()
    matched = set() if include_matched else matched_functions()
    cache = _load_cache(cache_path) if cache_path else {}
    for lane, name, best, extra in drafts:
        name, extra, renamed = current_names(name, extra, stale)
        if renamed:
            print("  NOTE %s/%s: renamed since the draft was saved: %s" % (
                lane, best.parent.name, ", ".join("%s -> %s" % r for r in renamed)), flush=True)
        row = {"lane": lane, "dir": best.parent.name, "function": name, "draft": str(best)}
        members = name.split(",")
        if all(m in matched for m in members):
            row.update(verdict="MATCHED", detail="in %s: no draft check needed" % (
                "ovl_matched.lock.json" if members[0].startswith("ovl_") else "blob_matched.lock.json"), seconds=0)
        else:
            key = draft_key(name, best, extra, fwd) if cache_path else None
            if key and not refresh:            # reread: concurrent runs over other globs add rows meanwhile
                cache = _load_cache(cache_path)
            hit = cache.get(key) if key and not refresh else None
            if hit:
                row.update(verdict=hit["verdict"], detail=hit["detail"], seconds=0, cached=hit.get("date", True))
            else:
                unit = ["--unit"] if "--unit" in extra else []
                ex = [x for x in extra if x != "--unit"]
                cmd = [sys.executable, "-m", "tools.recomp.draft_check"] + unit + [name, str(best)] + ex + fwd
                t0 = time.time()
                proc = subprocess.run(cmd, cwd=str(REPO), capture_output=True, text=True)
                verdict, detail = summarize_output(proc.stdout + proc.stderr, proc.returncode)
                row.update(verdict=verdict, detail=detail, seconds=round(time.time() - t0))
                if key and verdict != "ERROR":
                    _save_cache(cache_path, key, {"verdict": verdict, "detail": detail, "function": name,
                                                  "draft": str(best), "date": time.strftime("%Y-%m-%d")})
        rows.append(row)
        print("  %-5s %-36s %-12s %s%s" % (lane, name[:36], row["verdict"], row["detail"][:150],
                                          " (cached)" if row.get("cached") else ""), flush=True)
    print()
    print("| lane | function | verdict | detail |")
    print("|---|---|---|---|")
    for r in rows:
        print("| %s | %s | %s | %s%s |" % (r["lane"], r["function"], r["verdict"], r["detail"].replace("|", "/"),
                                          " (cached)" if r.get("cached") else ""))
    counts = {}
    for r in rows:
        counts[r["verdict"]] = counts.get(r["verdict"], 0) + 1
    print("\n%s: %d drafts: %s (EQUIVALENT is behaviour only, never a match)" % (
        label, len(rows), ", ".join("%d %s" % (n, v) for v, n in sorted(counts.items()))))
    if json_path:
        Path(json_path).write_text(json.dumps(rows, indent=1) + "\n")
    return rows


def lane_main(args, argv):
    """--lane LANE: draft_check every <lane>/<NAME>/best.c, one table."""
    drafts = lane_drafts(args.lane)
    if not drafts:
        print("draft_check --lane %s: no */best.c" % args.lane)
        return 3
    fwd = forwarded_options(argv)     # every other option (--scripts, --remote, --jobs, ...) applies to each draft
    lane = Path(args.lane).name
    check_drafts([(lane, n, b, e) for n, b, e in drafts], fwd, "LANE %s" % args.lane, args.json,
                 include_matched=True)
    return 0


DEFAULT_LANE_GLOBS = "w2[8-9]*,w3*"


def all_lane_dirs(globs, root=LANE_ROOT):
    out = []
    for g in [x.strip() for x in globs.split(",") if x.strip()]:
        out += [d for d in sorted(Path(root).glob(g)) if d.is_dir() and d not in out]
    return out


def all_lanes_main(args, argv):
    """--all-lanes [GLOB,...]: every <lane>/<NAME>/best.c of the matching lane
    directories (default w2[8-9]*,w3*) in one table; results cached per draft
    (draft_key) in DC_HOME/lanes/cache.json; matched functions are not run.
    With --remote HOST the drafts (and the files they include, and dc.args)
    are copied into the service tree's cloud/work/frontier first and the whole
    summary runs there."""
    globs = args.all_lanes or DEFAULT_LANE_GLOBS
    dirs = all_lane_dirs(globs)
    drafts = []
    for d in dirs:
        drafts += [(d.name, n, b, e) for n, b, e in lane_drafts(str(d))]
    if args.remote:
        files = set()
        for _l, _n, best, _e in drafts:
            files.update(local_files(best))
            if (best.parent / "dc.args").is_file():
                files.add((best.parent / "dc.args").resolve())
        rel = sorted(str(f.relative_to(REPO.resolve())) for f in files if str(f).startswith(str(REPO.resolve())))
        with tempfile.NamedTemporaryFile("w", suffix=".lst", delete=False) as fl:
            fl.write("\n".join(rel) + "\n")
        rc = subprocess.run(["rsync", "-a", "--files-from=" + fl.name, str(REPO) + "/",
                             "%s:%s/" % (args.remote, args.remote_tree)]).returncode
        os.unlink(fl.name)
        if rc != 0:
            raise DCError("cannot copy the lane drafts to %s:%s" % (args.remote, args.remote_tree))
        fwd = ["--all-lanes", globs] + forwarded_options(_strip_remote(argv), flags=())
        cmd = "cd %s && python3 -m tools.recomp.draft_check %s" % (
            args.remote_tree, " ".join(shlex.quote(a) for a in fwd))
        print("draft_check --all-lanes: %d drafts in %d lane directories; running on %s" % (
            len(drafts), len(dirs), args.remote), flush=True)
        return subprocess.run(["ssh", args.remote, cmd]).returncode
    print("draft_check --all-lanes %s: %d drafts in %d lane directories (%s)" % (
        globs, len(drafts), len(dirs), ", ".join(d.name for d in dirs)), flush=True)
    fwd = forwarded_options(argv)
    check_drafts(drafts, fwd, "ALL LANES %s" % globs, args.json, cache_path=LANES_CACHE, refresh=args.refresh,
                 include_matched=args.include_matched)
    return 0


# ------------------------------------------------------------------ main

def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    ap = argparse.ArgumentParser(prog="python3 -m tools.recomp.draft_check", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("name", nargs="?", default=None,
                    help="function to replace (inventory name, game or static); with --unit a comma "
                    "list of the functions the draft defines, all replaced together")
    ap.add_argument("draft", nargs="?", default=None,
                    help="C file defining NAME (a /* flags: ... */ header picks the IDO flags)")
    ap.add_argument("--scripts", default=None,
                    help="comma-separated tools/recomp/scripts names, 'all', or 'reached' (every script the census "
                    "says calls NAME); default: the 6 census scripts with the most calls of NAME, all scripts "
                    "when NAME is not in the census")
    ap.add_argument("--params", action="append", default=[],
                    help="register parameters of NAME itself: PARAM=REG,... for every prototype parameter "
                    "(IDO -O3 IPA functions; e.g. handle=a0,mark=t3); with --unit prefix the member: "
                    "'MEMBER:PARAM=REG,...' (repeat per member)")
    ap.add_argument("--unit", action="store_true",
                    help="NAME is a comma list: the draft is ONE translation unit defining every member; it is "
                    "compiled and linked once and all members are replaced together (calls between members stay "
                    "inside the unit, with the unit's own conventions; only the outermost member call on a "
                    "thread is compared, on both sides). The first divergence names the member")
    ap.add_argument("--callee", action="append", default=[],
                    help="register convention of a callee the draft calls: 'PROTOTYPE=REG,...[/CLOBBER,...]' "
                    "(adds to tools/recomp/ipa_callees.json for this run)")
    ap.add_argument("--probe-callees", default=None, metavar="CALLEE[,CALLEE...]",
                    help="derive the register-parameter convention of these internal callees from the retail "
                    "body and its captured call sites (regmap_verify --derive) and print each in --callee syntax; "
                    "no draft needed (NAME is only a label). Add --static-only to skip the capture run")
    ap.add_argument("--static-only", action="store_true", help="with --probe-callees: no capture run")
    ap.add_argument("--gc", action="store_true",
                    help="clean up the recomp host's draft_check directory (README: cleanup policy) and exit; "
                    "NAME is ignored (give any word, e.g. --gc all); with --dry-run only list")
    ap.add_argument("--dry-run", action="store_true", help="with --gc: list what would be deleted")
    ap.add_argument("--keep-trees", type=int, default=None,
                    help="with --gc: function build trees to keep (default %d, $RUSH_DC_KEEP)" % GC_KEEP_TREES)
    ap.add_argument("--gc-days", type=float, default=None,
                    help="with --gc: keep runs/probe/verify/census/inbox entries younger than this many days "
                    "(default %g, $RUSH_DC_DAYS)" % GC_DAYS)
    ap.add_argument("--ignore-register-params", action="store_true",
                    help="build a function the frontier lists with register_params without --params "
                    "(o32 inbound: only right if those registers are not really parameters)")
    ap.add_argument("--statics", default=None,
                    help="NAME=0xADDR,... (or FUNCTION:NAME=0xADDR) function-local statics -> retail addresses "
                         "(Phase 10: initialised and uninitialised, named from the draft's source)")
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
    ap.add_argument("--lane", default=None, metavar="LANE",
                    help="summary mode (Phase 9): check every cloud/work/frontier/LANE/<NAME>/best.c (or a "
                    "directory path) and print one table; per-draft options in <NAME>/dc.args; the other options "
                    "(--scripts, --remote, ...) apply to every draft")
    ap.add_argument("--all-lanes", nargs="?", const=DEFAULT_LANE_GLOBS, default=None, metavar="GLOB,...",
                    help="summary over many lanes (Phase 11): every cloud/work/frontier/<GLOB>/<NAME>/best.c "
                    "(default %s) in one table; cached per draft (DC_HOME/lanes/cache.json, --refresh to rerun); "
                    "functions in blob_matched.lock.json are listed MATCHED and not run (--include-matched)"
                    % DEFAULT_LANE_GLOBS)
    ap.add_argument("--refresh", action="store_true", help="with --all-lanes: ignore cached rows")
    ap.add_argument("--include-matched", action="store_true", help="with --all-lanes: also run matched functions")
    args = ap.parse_args(argv)
    if args.all_lanes:
        try:
            return all_lanes_main(args, argv)
        except DCError as exc:
            print("draft_check: ERROR: %s" % exc, file=sys.stderr)
            return 2
    if args.lane:
        try:
            return lane_main(args, argv)
        except DCError as exc:
            print("draft_check: ERROR: %s" % exc, file=sys.stderr)
            return 2
    if args.name is None and not args.gc and not args.probe_callees:
        ap.error("NAME is required (or --lane LANE / --all-lanes)")
    try:
        if args.gc:
            if args.remote:
                cmd = "cd %s && python3 -m tools.recomp.draft_check %s" % (
                    args.remote_tree, " ".join(shlex.quote(a) for a in _strip_remote(argv)))
                return subprocess.run(["ssh", args.remote, cmd]).returncode
            return gc_main(args)
        if args.probe_callees:
            if args.remote:
                fwd = _strip_remote(argv)
                cmd = "cd %s && python3 -m tools.recomp.draft_check %s" % (
                    args.remote_tree, " ".join(shlex.quote(a) for a in fwd))
                return subprocess.run(["ssh", args.remote, cmd]).returncode
            return probe_callees_main(args)
        if args.draft is None:
            raise DCError("no draft given (only --probe-callees runs without one)")
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
    image = None
    if not args.unit:
        image, args.name = split_overlay_name(args.name)
    members = [m.strip() for m in args.name.split(",") if m.strip()] if args.unit else [args.name]
    if not members or len(set(members)) != len(members):
        raise DCError("--unit: give distinct member names, comma separated")
    if not args.unit and "," in args.name:
        raise DCError("several functions given: add --unit to check them as one unit")
    name = members[0]
    text = draft.read_text(errors="replace")
    protos = {}
    for m in members:
        try:
            protos[m] = rp.find_prototype(text, m)
        except rp.ReplaceError:
            raise DCError(f"{draft.name} has no definition of {m}")
    pops = {m: find_population(m, image=image) for m in members}
    pop = pops[name]
    if len(set(pops.values())) != 1:
        raise DCError("--unit members span populations: %s" % pops)
    entry = {"function": name, "population": pop, "source": str(draft), "mode": "ido", "state": "draft",
             "allow_ipa": True, "note": "draft_check run (NOT a match)"}
    if args.unit:
        entry["unit"] = members
        entry["unit_capture"] = {}
    if args.include_dir:
        entry["include_dirs"] = [str(Path(d).resolve()) for d in args.include_dir]
    if args.compiler:
        entry["compiler"] = args.compiler
    if args.statics:
        entry["statics"] = {k: int(v, 16) for k, v in (x.split("=") for x in args.statics.split(",") if x)}
    table = rp.load_ipa_table()
    params = member_params(args.params, members, args.unit)
    for m in members:
        cap_fields = {}
        regs_fact = register_params_fact(m) if pops[m] in ("game", "static") else []
        proto = protos[m]
        if m in params:
            cap, regs = parse_params(params[m], proto)
            ret = rp._lenient_type(rp.parse_prototype(proto)[0])
            cap_fields = {"capture_args": cap,
                          "capture_ret": "void" if ret["c"] == "void" else ("f0" if ret["float"] else "v0")}
            table[m] = {"prototype": proto, "params": regs, "evidence": "draft_check --params (lane hypothesis)"}
        elif m in table and "params" in table[m]:
            # committed ipa_callees.json convention of m: inbound shim + capture of those registers
            tproto = table[m]["prototype"]
            tnames = [pn for _t, pn in rp.parse_prototype(tproto)[2]]
            ret = rp._lenient_type(rp.parse_prototype(tproto)[0])
            cap_fields = {"capture_args": ["%s=%s" % (pn, r) for pn, r in zip(tnames, table[m]["params"])],
                          "capture_ret": "void" if ret["c"] == "void" else ("f0" if ret["float"] else "v0")}
        elif regs_fact and not args.ignore_register_params:
            raise DCError(f"{m} takes register parameters ({','.join(regs_fact)}, build/frontier.json facts): "
                          f"pass --params {'%s:' % m if args.unit else ''}PARAM=REG,... for every parameter of "
                          f"`{proto}` (e.g. --params {'%s:' % m if args.unit else ''}"
                          f"{','.join('%s=%s' % (pn, 'REG') for _t, pn in rp.parse_prototype(proto)[2])}); "
                          "prove the mapping with python3 -m tools.recomp.regmap_verify")
        if m == name:
            entry.update(cap_fields)
        elif cap_fields:
            entry["unit_capture"][m] = cap_fields
    draft_text = "\n".join(f.read_text(errors="replace") for f in local_files(draft))
    auto = clobber_shims(table, retail=retail_clobbers(draft_text))
    for cname, extra in widen_clobbers(table, retail_clobbers(draft_text, register_params=True), members):
        print("  NOTE %s: clobbers widened with the retail unsaved set (+%s)" % (cname, ",".join(extra)))
    for m in members:
        auto.pop(m, None)            # members: inbound only through --params / the table
    table.update(auto)
    for spec in args.callee:
        cname, ce = parse_callee(spec)
        table[cname] = ce
    key = name if not args.unit else "%s+unit%d" % (name, len(members))
    if pop.startswith("ovl_"):
        key = "%s_%s" % (pop, key)          # both images have func_8038A400.. names
    run_dir = DC_HOME / "runs" / key / ("%s-%d" % (time.strftime("%Y%m%d-%H%M%S"), os.getpid()))
    run_dir.mkdir(parents=True, exist_ok=True)
    manifest = run_dir / "replacements.json"
    manifest.write_text(json.dumps({"note": "draft_check", "entries": {"draft": entry},
                                    "sets": {"draft": ["draft"]}}, indent=1))
    ipa_path = run_dir / "ipa_callees.json"
    ipa_path.write_text(json.dumps({"note": "draft_check: committed table + run options",
                                    "callees": table}, indent=1))
    print("draft_check %s <- %s%s" % (", ".join(members) + (" (unit)" if args.unit else ""), args.origin or draft,
                                       " [%s]" % pop if pop != "game" else ""))
    print("  draft sha256 %s; run dir %s" % (hashlib.sha256(draft.read_bytes()).hexdigest()[:16], run_dir))
    if args.scripts in (None, "census", "reached"):
        scripts, covered = (census_scripts_for(members) if not pop.startswith("ovl_")
                            else overlay_census_scripts(pop, name))
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
        scripts, how = list(dict.fromkeys(s.strip() for s in args.scripts.split(",") if s.strip())), "chosen"
        for s in scripts:
            if not (SCRIPT_DIR / (s + ".press")).is_file():
                raise DCError(f"no script {s} (have: {', '.join(all_scripts())})")
    print("  scripts (%s): %s" % (how, ", ".join(scripts) or "none"), flush=True)
    t0 = time.time()
    with ThreadPoolExecutor(2) as ex:
        bj = int(os.environ.get("RUSH_DC_BUILD_JOBS") or 8)     # ninja -j per side (two sides build at once)
        fc = ex.submit(build_side, "cap", key, manifest, run_dir, p, None, bj)
        fr = ex.submit(build_side, "repl", key, manifest, run_dir, p, ipa_path, bj)
        cap, repl = fc.result(), fr.result()
    man = json.loads(Path(repl["manifest"]).read_text())
    rinfos = [r for r in man.get("replacements", {}).get("replaced", [])
              if r.get("id") == "draft" or str(r.get("id", "")).startswith("draft__")]
    rinfo = rinfos[0] if rinfos else {}
    warn = [ln for ln in Path(repl["gen_log"]).read_text(errors="replace").splitlines()
            if "WARNING" in ln or "ipa note" in ln or "ipa shim" in ln or "ipa body" in ln]
    print("  built in %.0f s (draft compiled with %s)" % (time.time() - t0, entry.get("compiler") or
                                                          rp.DEFAULT_COMPILER[pop]))
    for ln in warn:
        print("  " + ln.strip())
    o32_called = sorted({n for ln in warn for m in re.finditer(r"o32 calls for ([\w,]+)", ln)
                         for n in m.group(1).split(",") if n})
    if o32_called:
        suggest_callees(o32_called)
    for r in rinfos:
        if r.get("ipa_inbound"):
            print("  inbound register shim%s: %s" % (" (%s)" % r["function"] if args.unit else "",
                                                    table[r["function"]]["params"]))
    if rinfo.get("private_data"):
        print("  NOTE private data (not bound to retail): %s" % ", ".join(rinfo["private_data"]))
    if rinfo.get("statics_private"):
        print("  NOTE function-local statics kept private (pass --statics NAME=0xADDR or FUNCTION:NAME=0xADDR): %s"
              % ", ".join(rinfo["statics_private"]))
        for prob in rinfo.get("static_layout_problems") or ():
            print("  NOTE static layout: %s" % prob)
    cap_key = (sha256_file(cap["host"])[:12] + sha256_file(cap["rom"])[:8])
    results = []
    if scripts:
        with ThreadPoolExecutor(max(1, args.jobs)) as ex:
            futs = [ex.submit(run_script, s, key, cap, repl, run_dir, cap_key, args.frames) for s in scripts]
            for f in futs:
                r = f.result()
                results.append(r)
                tag = r["status"]
                extra = ""
                if tag in ("EQUIVALENT", "DIVERGES") and args.unit:
                    extra = "%d member calls (%s)" % (r["calls_of_fn"], ", ".join(
                        "%s %d" % (m, r["calls_per_member"].get(m, 0)) for m in members))
                elif tag in ("EQUIVALENT", "DIVERGES"):
                    extra = "%d calls of %s" % (r["calls_of_fn"], name)
                if r.get("crash") is not None:
                    extra += " - draft FAULTED at frame %s" % r["crash"].get("frame")
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
    if args.unit:
        summary["unit"] = members
    print()
    if div:
        for line in divergence_lines(div[0], name, members if args.unit else None):
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
        if args.unit:
            tot = {m: sum((r.get("calls_per_member") or {}).get(m, 0) for r in eq) for m in members}
            print("  unit members (outermost calls compared): %s" % ", ".join("%s %d" % kv for kv in tot.items()))
            if any(v == 0 for v in tot.values()):
                print("  NOTE never called from outside the unit: %s (exercised only through other members, "
                      "inside their compared calls)" % ", ".join(m for m, v in tot.items() if v == 0))
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
    auto_gc()
    return rc


if __name__ == "__main__":
    sys.exit(main())
