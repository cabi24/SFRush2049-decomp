#!/usr/bin/env python3
"""Whole-program experiment driver (frontier/wholeprog).

Runs ONLY inside an isolated copy of the repo subset on the builder:
    ~/rush2049/scratch/frontier/wholeprog/{tools,src,asm,blob_matched.lock.json}
with this directory rsynced to ~/rush2049/scratch/frontier/wholeprog/wp/.

    python3 wp/wp.py inventory                 # parse sources, dedup map, call graphs
    python3 wp/wp.py baseline                  # every locked body in its locked recipe
    python3 wp/wp.py o3single                  # every standalone single alone at -O3
    python3 wp/wp.py build TAG --set all --ctx keep --keep groups [--standins]

Nothing here writes outside wp/ (runs/, cache/).  score.py is imported, not edited.
"""
import argparse
import concurrent.futures
import contextlib
import hashlib
import io
import json
import os
import re
import shlex
import shutil
import subprocess
import sys
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
sys.path.insert(0, str(ROOT / "tools" / "cloud"))
import score  # noqa: E402

RUNS = HERE / "runs"
CACHE = HERE / "cache"
JOBS = int(os.environ.get("WP_JOBS", "4"))
O3 = "-g0 -O3 -mips2 -G 0 -non_shared"
import threading  # noqa: E402
SCORE_LOCK = threading.Lock()
KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "do", "else"}


# --- source parsing ----------------------------------------------------------

def scan_defs(text):
    """Top-level function definitions: [{name, static, head, open, close}].
    open/close are the offsets of the body's braces."""
    out = []
    i, n = 0, len(text)
    depth = pdepth = 0
    stmt = 0
    cur = None
    while i < n:
        c = text[i]
        if c == "/" and text[i + 1:i + 2] == "*":
            j = text.find("*/", i + 2)
            i = n if j < 0 else j + 2
            continue
        if c == "/" and text[i + 1:i + 2] == "/":
            j = text.find("\n", i)
            i = n if j < 0 else j
            continue
        if c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            i = j + 1
            continue
        if c == "#" and (i == 0 or text[i - 1] == "\n"):
            j = text.find("\n", i)
            i = n if j < 0 else j
            if depth == 0:
                stmt = i
            continue
        if c == "(":
            pdepth += 1
        elif c == ")":
            pdepth -= 1
        elif c == "{":
            if depth == 0 and pdepth == 0:
                head = text[stmt:i]
                clean = re.sub(r"/\*.*?\*/", " ", head, flags=re.S)
                kr = bool(KR.search(clean)) and clean.rstrip().endswith(";")
                if (kr or re.search(r"\)\s*$", clean)) and "=" not in clean:
                    m = None
                    for m in re.finditer(r"([A-Za-z_]\w*)\s*\(", clean):
                        if m.group(1) not in KEYWORDS:
                            break
                    if m:
                        cur = dict(name=m.group(1),
                                   static=bool(re.search(r"\bstatic\b", clean)),
                                   head=stmt, open=i, kr=None)
                        if kr:
                            # K&R definition: strip from the parameter list on
                            k = re.search(re.escape(m.group(1)) + r"\s*\(", head)
                            cur["kr"] = stmt + k.end() - 1
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0 and cur is not None:
                cur["close"] = i
                out.append(cur)
                cur = None
                stmt = i + 1
        elif c == ";" and depth == 0 and pdepth == 0:
            # a K&R definition's parameter declarations sit between ) and {
            if not KR.search(re.sub(r"/\*.*?\*/", " ", text[stmt:i], flags=re.S)):
                stmt = i + 1
        i += 1
    return out


KR = re.compile(r"^[^=(]*\([\w\s,]*\)\s*[A-Za-z_]")
IDENT = re.compile(r"[A-Za-z_]\w*")


# --- preprocessing -------------------------------------------------------------

def preprocessed(path, flags=""):
    """IDO `cc -E` output of a source (378 of the sources still carry #define /
    #if lines, so definitions can only be found after preprocessing)."""
    path = Path(path)
    raw = path.read_bytes()
    extra = ["-Xcpluscomm"] if "-Xcpluscomm" in flags else []
    key = hashlib.sha256(raw + b"|E|" + " ".join(extra).encode()).hexdigest()
    cached = CACHE / (key + ".i")
    if cached.exists():
        return cached.read_text()
    proc = subprocess.run([score.ido("cc"), "-E", *extra, path.name], cwd=path.parent,
                          capture_output=True, text=True, errors="replace")
    if proc.returncode != 0 or not proc.stdout.strip():
        raise SystemExit(f"cc -E failed for {path}: {proc.stderr[-500:]}")
    CACHE.mkdir(exist_ok=True)
    cached.write_text(proc.stdout)
    return proc.stdout


# --- inventory -----------------------------------------------------------------

def load_lock():
    return json.loads((ROOT / "blob_matched.lock.json").read_text())


def source_files():
    """[(file_id, path, kind, owner, flags)] for every locked single and group file."""
    lock = load_lock()
    files = []
    groups = {}
    for name, rec in sorted(lock.items()):
        if "group" in rec:
            groups.setdefault(rec["group"], []).append(name)
        else:
            files.append((f"s_{name}.c", ROOT / rec["source"], "single", name, rec["flagset"]))
    for group in sorted(groups):
        gdir = ROOT / "src" / "blob" / "groups" / group
        spec = json.loads((gdir / "group.json").read_text())
        for f in spec["files"]:
            files.append((f"g_{group}__{f}", gdir / f, "group", group, spec["flags"]))
    return files, groups


def real_callgraph():
    """{callee: set(callers)} from jal words of the retail targets."""
    tg = score.targets()
    addr = score.image_symbols()
    by_addr = {}
    for name, a in addr.items():
        if name in tg:
            by_addr[a] = name
    callers = {name: set() for name in tg}
    for name, words in tg.items():
        for w in words:
            if w >> 26 == 3:
                callee = by_addr.get(0x80000000 | ((w & 0x03FFFFFF) << 2))
                if callee and callee != name:
                    callers[callee].add(name)
    return callers, addr


def inventory():
    lock = load_lock()
    files, groups = source_files()
    inv = {}
    for fid, path, kind, owner, flags in files:
        text = preprocessed(path, flags)
        defs = scan_defs(text)
        inv[fid] = dict(path=str(path), kind=kind, owner=owner, flags=flags,
                        defs=[dict(name=d["name"], static=d["static"]) for d in defs])
    # canonical definition of every global function
    canon = {}
    for fid, rec in inv.items():
        for d in rec["defs"]:
            if d["static"]:
                continue
            name = d["name"]
            locked = lock.get(name)
            if rec["kind"] == "single" and rec["owner"] == name:
                canon[name] = fid
            elif (rec["kind"] == "group" and locked is not None
                  and locked.get("group") == rec["owner"]):
                canon[name] = fid
    context = {}
    for fid, rec in sorted(inv.items()):
        for d in rec["defs"]:
            if not d["static"] and d["name"] not in canon:
                context.setdefault(d["name"], []).append(fid)
    missing = sorted(set(lock) - set(canon))
    return dict(files=inv, canon=canon, context=context, missing=missing, groups=groups)


# --- transformed sources ---------------------------------------------------------

DEAD = ("\n  if (0) {\n" + "".join(f"    __wp_dead[{i}] = {i + 1};\n" for i in range(8)) + "  }\n")


def transform(text, fid, keep_names, block=()):
    """Strip (to a prototype) every global definition whose name is not in
    keep_names for this file.  Functions named in `block` get a dead `if (0)`
    block appended to their body: umerge sizes a callee before optimisation, so
    this stops it being inlined without changing its code (the trick the
    existing groups use by hand).  Returns (text, kept defs with body refs)."""
    defs = scan_defs(text)
    kept = []
    pieces = ["extern int __wp_dead[];\n"] if any(d["name"] in block for d in defs) else []
    pos = 0
    for d in defs:
        if d["static"] or d["name"] in keep_names:
            body = text[d["open"]:d["close"] + 1]
            kept.append(dict(name=d["name"], static=d["static"],
                             refs=sorted(set(IDENT.findall(body)) - {d["name"]})))
            if d["name"] in block:
                pieces.append(text[pos:d["close"]])
                pieces.append(DEAD)
                pos = d["close"]
            continue
        if d.get("kr") is not None:
            pieces.append(text[pos:d["kr"]])
            pieces.append("();")
        else:
            pieces.append(text[pos:d["open"]])
            pieces.append(";")
        pos = d["close"] + 1
    pieces.append(text[pos:])
    return "".join(pieces), kept


def run(cmd, cwd, log):
    t = time.time()
    proc = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    dt = time.time() - t
    with open(log, "a") as f:
        f.write(f"$ {' '.join(cmd[:6])} ... rc={proc.returncode} {dt:.1f}s\n")
        f.write((proc.stdout + proc.stderr)[-4000:])
    return proc, dt


def cc_j(work, fname, flags):
    """cc -j with a content cache."""
    text = (work / fname).read_bytes()
    key = hashlib.sha256(text + flags.encode()).hexdigest()
    unit = work / re.sub(r"\.c$", ".u", fname)
    cached = CACHE / (key + ".u")
    if cached.exists() and not os.environ.get("WP_NOCACHE"):
        shutil.copy(cached, unit)
        return fname, 0, ""
    proc = subprocess.run([score.ido("cc"), "-j", *shlex.split(flags), fname],
                          cwd=work, capture_output=True, text=True)
    if proc.returncode == 0 and unit.exists():
        CACHE.mkdir(exist_ok=True)
        shutil.copy(unit, cached)
    return fname, proc.returncode, (proc.stdout + proc.stderr)[-1500:]


def stages(work, units, obj, log, opt="-O3"):
    common = ["-mips2", "-EB", "-g0", opt]
    ido = score.ido
    steps = [
        ("uld", [ido("uld"), "-L/usr/lib/mips2/nonshared", "-_SYSTYPE_SVR4", "-mips2", "-non_shared",
                 "-g0", "-no_AutoGnum", "-kp", "keep.txt", *units, "-ko", "linked"]),
        ("usplit", [ido("usplit"), "-mips2", "-o", "split", "-t", "st", "linked"]),
        # WP_NOINLINE=1: what `cc -O3 -noinline` passes (umerge -noinline)
        ("umerge", [ido("umerge"), "-v", "-Olimit", "5000", "-mips2",
                    *(["-noinline"] if os.environ.get("WP_NOINLINE") else []),
                    "-EB", "-g0", opt, "split", "-o", "merged", "-t", "st"]),
        ("uopt", [ido("uopt"), "-G", "0", "-Olimit", "5000", *common, "merged", "opt", "-t", "st", "optlog"]),
        ("ugen", [ido("ugen"), "-G", "0", *common, "opt", "-o", "gen", "-t", "st", "-temp", "ugtmp"]),
        ("as1", [ido("as1"), "-elf", "-G", "0", "-p0", *common, score.R4300_AS1, "-Olimit", "5000",
                 "gen", "-o", str(obj), "-t", "st"]),
    ]
    if os.environ.get("WP_NOMERGE"):
        # probe: no umerge stage (the procedure inliner); uopt reads usplit's output
        steps = [(n, ["split" if a == "merged" else a for a in c])
                 for n, c in steps if n != "umerge"]
    times = {}
    for name, cmd in steps:
        proc, dt = run(cmd, work, log)
        times[name] = round(dt, 2)
        if name == "umerge":
            # umerge -v lists every procedure and "inlining <callee>" under its caller
            (Path(work) / "umerge.log").write_text(proc.stdout + proc.stderr)
        if proc.returncode != 0:
            return times, f"{name} failed: " + (proc.stderr or proc.stdout)[-3000:]
    return times, None


_memo = {}


def fast_score():
    """score.targets()/image_symbols() re-hash every region file per call; verify
    once per process and reuse (the experiment tree is not being edited)."""
    if not _memo:
        _memo["t"] = score.targets()
        _memo["s"] = score.image_symbols()
        score.targets = lambda: _memo["t"]
        score.image_symbols = lambda: _memo["s"]


def stub_tail(tail):
    return all(w in (0x03E00008, 0) for w in tail)


def score_members(obj, names):
    """{name: status dict}.  MATCH = strict; MATCH_UNVERIFIED = only local
    data-section relocations unverified; ABSENT = no symbol in the object."""
    res = {}
    fast_score()
    syms = score.symbols(obj)
    words = score.text_words(obj)
    offs = sorted(set(syms.values())) + [len(words) * 4]
    for name in names:
        buf = io.StringIO()
        try:
            # redirect_stdout is process-global: serialise scoring across threads
            with SCORE_LOCK, contextlib.redirect_stdout(buf):
                c = score.compare(obj, name, show=6)
        except SystemExit as exc:
            res[name] = dict(status="ABSENT", detail=str(exc)[:200])
            continue
        if c.accepted(False):
            st = "MATCH"
        elif c.accepted(True):
            st = "MATCH_UNVERIFIED"
        elif c.differing == 0 and c.extra_words == 0 and not c.errors:
            st = "UNRESOLVED"
        elif (c.differing == 0 and not c.errors and not c.unresolved and
              stub_tail(words[syms[name] // 4 + c.total:
                              offs[offs.index(syms[name]) + 1] // 4])):
            # body identical; the words up to the next symbol are the emptied
            # bodies (jr ra; nop) of deleted procedures, which carry no symbol
            st = "MATCH_STUBTAIL"
        else:
            st = "DIFF"
        size = (offs[offs.index(syms[name]) + 1] - syms[name]) // 4
        res[name] = dict(status=st, words=size, differing=c.differing, total=c.total, extra=c.extra_words,
                         unresolved=c.unresolved[:4], nunverified=len(c.unverified),
                         errors=c.errors[:3], diff=buf.getvalue()[:900])
    return res


def tally(res):
    out = {}
    for r in res.values():
        out[r["status"]] = out.get(r["status"], 0) + 1
    return out


def emission_order(obj, addr):
    syms = score.symbols(obj)
    order = [n for n, _ in sorted(syms.items(), key=lambda kv: kv[1])]
    known = [n for n in order if n in addr or score.address_named(n) is not None]
    addrs = [addr[n] if n in addr else score.address_named(n) for n in known]
    inversions = sum(1 for a, b in zip(addrs, addrs[1:]) if b < a)
    # longest run of functions already in increasing address order (LIS)
    import bisect
    tails = []
    for a in addrs:
        k = bisect.bisect_left(tails, a)
        if k == len(tails):
            tails.append(a)
        else:
            tails[k] = a
    return order, dict(functions=len(order), with_address=len(known),
                       adjacent_inversions=inversions,
                       longest_increasing_subsequence=len(tails),
                       is_address_order=inversions == 0)


# --- commands ------------------------------------------------------------------

def cmd_inventory(args):
    inv = inventory()
    (HERE / "inventory.json").write_text(json.dumps(inv, indent=1))
    files = inv["files"]
    ndefs = sum(len(r["defs"]) for r in files.values())
    nstatic = sum(d["static"] for r in files.values() for d in r["defs"])
    alldefs = {}
    for fid, r in files.items():
        for d in r["defs"]:
            if not d["static"]:
                alldefs.setdefault(d["name"], []).append(fid)
    dup = {n: f for n, f in alldefs.items() if len(f) > 1}
    extra_single = {fid: [d["name"] for d in r["defs"] if d["name"] != r["owner"]]
                    for fid, r in files.items() if r["kind"] == "single" and len(r["defs"]) != 1}
    print(f"files {len(files)}  definitions {ndefs} (static {nstatic})")
    print(f"distinct global functions {len(alldefs)}; locked with canonical def {len(inv['canon'])}; "
          f"locked missing a def {len(inv['missing'])}: {inv['missing'][:10]}")
    print(f"context-only (not locked) functions {len(inv['context'])}, "
          f"of which stand-ins {sum(1 for n in inv['context'] if 'standin' in n)}")
    print(f"functions defined in more than one file: {len(dup)}")
    locked_dup = [n for n in dup if n in inv["canon"]]
    print(f"  of which locked (a group re-defines a locked body as context): {len(locked_dup)}")
    print(f"singles whose file defines other functions too: {len(extra_single)} "
          f"{list(extra_single.items())[:5]}")


def cmd_baseline(args):
    lock = load_lock()
    files, groups = source_files()
    out = HERE / "runs" / "baseline"
    out.mkdir(parents=True, exist_ok=True)
    res = {}

    def single(item):
        fid, path, kind, owner, flags = item
        obj = out / (fid + ".o")
        try:
            score.compile_single(path, flags, obj)
        except SystemExit as exc:
            return {owner: dict(status="COMPILE_ERROR", detail=str(exc)[:300])}
        r = score_members(obj, [owner])
        obj.unlink()
        return r

    t = time.time()
    with concurrent.futures.ThreadPoolExecutor(JOBS) as ex:
        for r in ex.map(single, [f for f in files if f[2] == "single"]):
            res.update(r)
    for group, members in sorted(groups.items()):
        obj = out / (group + ".o")
        try:
            score.compile_group(ROOT / "src" / "blob" / "groups" / group, obj)
        except SystemExit as exc:
            for m in members:
                res[m] = dict(status="COMPILE_ERROR", detail=str(exc)[:300])
            continue
        res.update(score_members(obj, members))
        obj.unlink()
    (out / "result.json").write_text(json.dumps(res, indent=1))
    print(f"baseline: {len(res)} members, {tally(res)}  ({time.time() - t:.0f}s)")


def cmd_o3single(args):
    files, _ = source_files()
    out = HERE / "runs" / "o3single"
    out.mkdir(parents=True, exist_ok=True)
    res = {}

    def single(item):
        fid, path, kind, owner, flags = item
        obj = out / (fid + ".o")
        new = re.sub(r"-O2\b", "-O3", flags)
        try:
            score.compile_single(path, new, obj)
        except SystemExit as exc:
            return {owner: dict(status="COMPILE_ERROR", detail=str(exc)[:300], was=flags)}
        r = score_members(obj, [owner])
        r[owner]["was"] = flags
        obj.unlink()
        return r

    with concurrent.futures.ThreadPoolExecutor(JOBS) as ex:
        for r in ex.map(single, [f for f in files if f[2] == "single"]):
            res.update(r)
    (out / "result.json").write_text(json.dumps(res, indent=1))
    o2 = {k: v for k, v in res.items() if "-O2" in v["was"]}
    o3 = {k: v for k, v in res.items() if "-O2" not in v["was"]}
    print(f"singles locked at -O2, rebuilt alone at -O3: {len(o2)} -> {tally(o2)}")
    print(f"singles locked at -O3 (unchanged recipe): {len(o3)} -> {tally(o3)}")


def cmd_build(args):
    lock = load_lock()
    inv = inventory()
    files, groups = source_files()
    callers, addr = real_callgraph()
    work = RUNS / args.tag
    if work.exists():
        shutil.rmtree(work)
    work.mkdir(parents=True)
    log = work / "build.log"

    # which files
    selected = []
    for item in files:
        fid, path, kind, owner, flags = item
        if args.set == "singles" and kind != "single":
            continue
        if args.set == "groups" and kind != "group":
            continue
        selected.append(item)
    if args.limit:
        selected = selected[args.offset:args.offset + args.limit]
    if args.only:
        wanted = set(Path(args.only).read_text().split())
        selected = [s for s in selected if s[3] in wanted]
    if args.order != "name":
        # link order: by retail address of the file's lowest locked function
        def first_addr(item):
            names = [d["name"] for d in inv["files"][item[0]]["defs"]]
            return min([addr[n] for n in names if n in addr and n in lock] or [0xFFFFFFFF])
        selected.sort(key=first_addr, reverse=args.order == "raddr")
    sel_ids = {s[0] for s in selected}

    # canonical definition per function inside the selection
    canon = {n: f for n, f in inv["canon"].items() if f in sel_ids}
    if args.ctx == "keep":
        for name, fids in inv["context"].items():
            here = [f for f in fids if f in sel_ids]
            if here:
                canon[name] = here[0]
    if args.prefer_group_def:
        # use a group's context definition instead of the locked standalone body
        for name in Path(args.prefer_group_def).read_text().split():
            alt = [f for f in sorted(sel_ids) if f.startswith("g_")
                   and any(d["name"] == name for d in inv["files"][f]["defs"])]
            only = os.environ.get("WP_PREFER_FROM")
            alt = [f for f in alt if not only or only in f] or alt
            if alt:
                canon[name] = alt[0]
    # a locked function whose canonical file is outside the selection may still be
    # defined as context by a selected group file
    if args.ctx == "keep":
        for fid in sorted(sel_ids):
            for d in inv["files"][fid]["defs"]:
                if not d["static"] and d["name"] not in canon:
                    canon[d["name"]] = fid

    # functions with no retail target (stand-in callers) that several files define
    # under one name are different procedures: rename them per file
    tg = score.targets()
    multi = {}
    for fid in sorted(sel_ids):
        for d in inv["files"][fid]["defs"]:
            if not d["static"] and d["name"] not in tg:
                multi.setdefault(d["name"], []).append(fid)
    renames = {}
    for name, fids in multi.items():
        if len(fids) > 1:
            for k, fid in enumerate(fids):
                renames.setdefault(fid, {})[name] = f"{name}__u{k}"
    summary_renamed = sum(len(v) for v in renames.values())

    blockset = set(Path(args.block).read_text().split()) if args.block else set()
    defs = {}       # name -> refs
    fnames = []
    statics = 0
    for fid, path, kind, owner, flags in selected:
        mine = {n for n, f in canon.items() if f == fid}
        text = preprocessed(path, flags)
        for old, new in renames.get(fid, {}).items():
            text = re.sub(r"\b%s\b" % re.escape(old), new, text)
            mine.discard(old)
            mine.add(new)
        text, kept = transform(text, fid, mine, blockset)
        (work / fid).write_text(text)
        fnames.append((fid, flags))
        for d in kept:
            if d["static"]:
                statics += 1
            else:
                defs[d["name"]] = d["refs"]

    members = sorted(n for n in defs if n in lock)
    defined = set(defs)

    # stand-ins: a caller for every function with real callers missing from the unit
    standins = []
    if args.standins:
        for n in sorted(defined):
            real = callers.get(n)
            if real is None:
                continue
            if real and len(real - defined) >= args.standin_min:
                standins.append(n)
        text = "".join(f"extern void {n}();\nvoid __wps_{n}(void)\n{{\n"
                       + f"  {n}();\n" * args.standin_calls + "}\n" for n in standins)
        (work / "wp_standins.c").write_text(text)
        fnames.append(("wp_standins.c", O3))
        for n in standins:
            defs["__wps_" + n] = [n]
        defined = set(defs)

    called = set()
    for n, refs in defs.items():
        called.update(r for r in refs if r in defined)

    if args.keep == "all":
        keep = set(defined)
    elif args.keep == "roots":
        keep = defined - called
    elif args.keep == "real":
        # internal only when every real caller is in the unit (or a stand-in exists)
        keep = set()
        for n in defined:
            real = callers.get(n)
            if real is None or not real:
                keep.add(n)               # stand-in, context without target, or true root
            elif not real <= defined and not args.standins:
                keep.add(n)
    elif args.keep == "groups":
        nonkeep, anykeep = set(), set()
        for group in groups:
            gdir = ROOT / "src" / "blob" / "groups" / group
            spec = json.loads((gdir / "group.json").read_text())
            gdefs, gkeep = set(), set()
            for f in spec["files"]:
                fid = f"g_{group}__{f}"
                rn = renames.get(fid, {})
                gkeep |= {rn.get(k, k) for k in spec["keep"]}
                if fid in inv["files"]:
                    gdefs |= {rn.get(d["name"], d["name"]) for d in inv["files"][fid]["defs"]
                              if not d["static"]}
            if args.internal == "members":
                # only the group's own locked members may be internal
                gdefs = {n for n in gdefs if lock.get(n, {}).get("group") == group}
            elif args.internal == "notsingle":
                # ... or context that is not a locked standalone body
                gdefs = {n for n in gdefs if n not in lock or "group" in lock[n]}
            nonkeep |= gdefs - gkeep
            anykeep |= gkeep
        if args.conflict == "keep":
            nonkeep -= anykeep
        keep = defined - nonkeep
    else:
        raise SystemExit("unknown keep rule")
    keep |= {n for n in defined if n.startswith("__wps_")}
    if args.keep_extra:
        keep |= set(Path(args.keep_extra).read_text().split()) & defined
    if args.unkeep:
        keep -= set(Path(args.unkeep).read_text().split())
    (work / "keep.txt").write_text("".join(k + "\n" for k in sorted(keep)))

    t0 = time.time()
    failed = []
    with concurrent.futures.ThreadPoolExecutor(JOBS) as ex:
        futs = [ex.submit(cc_j, work, fid,
                          (O3 + (" -Xcpluscomm" if "-Xcpluscomm" in flags else "")))
                for fid, flags in fnames]
        for f in futs:
            name, rc, msg = f.result()
            if rc != 0:
                failed.append((name, msg))
    t_cc = time.time() - t0
    summary = dict(tag=args.tag, args=vars(args), files=len(fnames), members=len(members),
                   defined=len(defined), statics=statics, keep=len(keep),
                   standins=len(standins), renamed=summary_renamed, blocked=len(blockset & defined), cc_seconds=round(t_cc, 1))
    if failed:
        summary["error"] = f"cc -j failed for {len(failed)} files"
        summary["cc_failed"] = failed[:20]
        (work / "result.json").write_text(json.dumps(summary, indent=1))
        print(json.dumps(summary, indent=1)[:3000])
        return 1
    units = [re.sub(r"\.c$", ".u", f) for f, _ in fnames]
    obj = work / "wp.o"
    t1 = time.time()
    times, err = stages(work, units, obj, log)
    summary["stage_seconds"] = times
    summary["link_to_object_seconds"] = round(time.time() - t1, 1)
    if err:
        summary["error"] = err
        (work / "result.json").write_text(json.dumps(summary, indent=1))
        print(json.dumps(summary, indent=1)[:4000])
        return 1
    t2 = time.time()
    res = score_members(obj, members)
    summary["score_seconds"] = round(time.time() - t2, 1)
    order, osum = emission_order(obj, addr)
    summary["emission"] = osum
    # callee-first check on the unit's own call edges (as S3 measured on retail)
    pos = {n: i for i, n in enumerate(order)}
    edges = [(c, r) for c, rs in defs.items() for r in rs if r in pos and c in pos and r in defs]
    summary["emission"]["call_edges"] = len(edges)
    summary["emission"]["callee_before_caller"] = sum(pos[r] < pos[c] for c, r in edges)
    summary["tally"] = tally(res)
    present = set(order)
    summary["defined_but_absent"] = len([n for n in defined if n not in present])
    (work / "result.json").write_text(json.dumps(dict(summary=summary, members=res, order=order,
                                                      keep=sorted(keep), refs=defs,
                                                      canon={n: canon.get(n) for n in defs}),
                                       indent=1))
    for junk in (() if os.environ.get("WP_KEEP") else ("linked", "split", "merged", "opt", "gen")):
        for p in work.glob(junk + "*"):
            if p.is_file():
                p.unlink()
    if not os.environ.get("WP_KEEP"):
        for p in list(work.glob("*.u")) + list(work.glob("s_*.c")) + list(work.glob("g_*.c")):
            p.unlink()
    print(json.dumps(summary, indent=1))
    return 0


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("inventory")
    sub.add_parser("baseline")
    sub.add_parser("o3single")
    b = sub.add_parser("build")
    b.add_argument("tag")
    b.add_argument("--set", default="all", choices=["all", "singles", "groups"])
    b.add_argument("--ctx", default="keep", choices=["keep", "strip"])
    b.add_argument("--keep", default="all", choices=["all", "roots", "real", "groups"])
    b.add_argument("--standins", action="store_true",
                   help="add a caller for every function with retail callers missing from the unit")
    b.add_argument("--standin-min", type=int, default=1)
    b.add_argument("--standin-calls", type=int, default=1)
    b.add_argument("--conflict", default="internal", choices=["internal", "keep"],
                   help="groups rule: a function internal in one group and kept in another")
    b.add_argument("--internal", default="any", choices=["any", "notsingle", "members"],
                   help="groups rule: which group definitions may be made internal")
    b.add_argument("--limit", type=int, default=0)
    b.add_argument("--offset", type=int, default=0)
    b.add_argument("--only", default=None)
    b.add_argument("--keep-extra", default=None)
    b.add_argument("--order", default="name", choices=["name", "addr", "raddr"],
                   help="uld link order of the files")
    b.add_argument("--prefer-group-def", default=None)
    b.add_argument("--unkeep", default=None, help="file of names forced internal")
    b.add_argument("--block", default=None,
                   help="file of function names that get a dead-code inline blocker")
    args = ap.parse_args()
    return dict(inventory=cmd_inventory, baseline=cmd_baseline, o3single=cmd_o3single,
                build=cmd_build)[args.cmd](args)


if __name__ == "__main__":
    sys.exit(main())
