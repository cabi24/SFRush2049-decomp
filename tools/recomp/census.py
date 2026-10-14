#!/usr/bin/env python3
"""census: which lockstep scripts reach which unmatched game functions (recomp Phase 7, w27r).

    python3 -m tools.recomp.census [--scripts all|a,b] [--functions unmatched|all|NAME,...] [--jobs 10]

On the recomp host (rocky). One capture build with a probe (no replacement) on
every chosen game function, recording a0-a3 (plus the function's frontier
`register_params` registers), v0 at exit, and the return address (gen_config
--extra-hooks writes $ra at every static jal site of a probed function; the
recompiled code does not). Each script runs headless in lockstep with
--call-log; per script the census keeps, per function, the call count, first
and last frame, the call sites reached and per argument register the range,
number of distinct values (up to 256) and kind.

Writes (committed: they name functions, frames and values, no ROM bytes):
  tools/recomp/census/<script>.json   per-script detail
  tools/recomp/census/census.json     {functions: {NAME: {size, vram, scripts: {script: calls}}}}
                                      (draft_check's default script choice)
  tools/recomp/census/README.md       summary tables: reached functions, and the
                                      unmatched functions NO script reaches
"Unmatched" = game functions (asm/us/blob) not in blob_matched.lock.json.

`--overlays` (Phase 11, w31r): probe every function of the two runtime images
(ovl_a, ovl_b; matched or not, ovl_matched.lock.json says which) instead and
write only tools/recomp/census/overlays.json + OVERLAYS.md (which scripts
execute which overlay functions; no call sites: the $ra hooks cover the game
image only). The game census files are left alone.
"""
import argparse
import datetime
import json
import os
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
if str(HERE) not in sys.path:
    sys.path.insert(0, str(HERE))
import draft_check as dc  # noqa: E402
import oracle  # noqa: E402
import regmap_verify as rv  # noqa: E402

OUT = HERE / "census"
MAX_DISTINCT = 256


def unmatched_game(funcs, lock=REPO / "blob_matched.lock.json"):
    locked = set(json.loads(Path(lock).read_text()))
    return [f for f in funcs if f["name"] not in locked]


def probe_entries(chosen):
    entries, sets = {}, []
    for f in chosen:
        pop = f.get("population", "game")          # Phase 11: overlay functions (script_probe ovl_a:NAME)
        facts = dc.register_params_fact(f["name"]) if pop == "game" else []
        regs = ["a0", "a1", "a2", "a3"] + [r for r in facts if r not in ("a0", "a1", "a2", "a3")]
        regs = [r for r in regs if r in dc.rp._GPR_NAMES or r.startswith("f")] + ["ra"]
        rid = "c_" + ("" if pop == "game" else pop + "_") + f["name"]
        entries[rid] = {"function": f["name"], "population": pop, "mode": "capture", "state": "probe",
                        "prototype": "void %s(void)" % f["name"], "capture_args": regs,
                        "capture_ret": "v0", "note": "census probe"}
        sets.append(rid)
    return {"note": "census", "entries": entries, "sets": {"draft": sets}}


def summarise(log, names):
    """{name: {calls, first_frame, last_frame, sites: {hex: n}, args: {reg: {...}}}} from a call log."""
    fns, out = {}, {}
    with open(log) as fh:
        for item in oracle.iter_calls(fh):
            if item[0] == "fn":
                fid = item[2].partition("=")[0]
                fns[item[1]] = (fid[2:] if fid.startswith("c_") else fid, [x.split(":")[0] for x in item[3]])
            elif item[0] == "c":
                name, labels = fns[item[3]]
                st = out.setdefault(name, {"calls": 0, "first_frame": item[2], "last_frame": item[2],
                                           "sites": {}, "_vals": {}})
                st["calls"] += 1
                st["last_frame"] = item[2]
                vals = dict(zip(labels, (int(v.split(":")[0], 16) for v in item[4])))
                ra = vals.pop("ra", 0)
                site = "0x%08X" % ((ra - 8) & 0xFFFFFFFF) if ra else "indirect"
                st["sites"][site] = st["sites"].get(site, 0) + 1
                for r, v in vals.items():
                    a = st["_vals"].setdefault(r, {"min": v, "max": v, "set": set()})
                    a["min"], a["max"] = min(a["min"], v), max(a["max"], v)
                    if len(a["set"]) <= MAX_DISTINCT:
                        a["set"].add(v)
    for name, st in out.items():
        args = {}
        for r, a in st.pop("_vals").items():
            n = len(a["set"])
            args[r] = {"min": "0x%08X" % a["min"], "max": "0x%08X" % a["max"],
                       "distinct": n if n <= MAX_DISTINCT else ">%d" % MAX_DISTINCT,
                       "kind": rv.kind_of(a["set"], r)}
            if n <= 8:
                args[r]["values"] = ["0x%08X" % v for v in sorted(a["set"])]
        st["args"] = args
    return out


def main(argv=None):
    ap = argparse.ArgumentParser(prog="python3 -m tools.recomp.census", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--scripts", default="all")
    ap.add_argument("--functions", default="unmatched")
    ap.add_argument("--jobs", type=int, default=10)
    ap.add_argument("--out", default=str(OUT))
    ap.add_argument("--keep-logs", action="store_true")
    ap.add_argument("--tree", default=None, help="commit the tree was copied from (the service tree has no .git)")
    ap.add_argument("--overlays", action="store_true",
                    help="probe every ovl_a/ovl_b function; write census/overlays.json + OVERLAYS.md only")
    args = ap.parse_args(argv)
    if args.overlays:
        return overlay_census(args)
    p = dc.paths()
    import gen_config as gc
    rom = gc.read_rom(p["rom"])
    image = gc.extract_images(rom)["game"]
    funcs = [f for f in gc.load_game_functions(REPO / "asm" / "us" / "blob") if f.get("name") and f.get("size")]
    if args.functions == "unmatched":
        chosen = unmatched_game(funcs)
    elif args.functions == "all":
        chosen = funcs
    else:
        want = set(args.functions.split(","))
        chosen = [f for f in funcs if f["name"] in want]
    scripts = dc.all_scripts() if args.scripts == "all" else args.scripts.split(",")
    run_dir = dc.DC_HOME / "census" / time.strftime("%Y%m%d-%H%M%S")
    run_dir.mkdir(parents=True, exist_ok=True)
    manifest = run_dir / "replacements.json"
    manifest.write_text(json.dumps(probe_entries(chosen), indent=1))
    hooks, seen = [], set()
    for f in chosen:
        for at, caller, _v, _s in rv.call_sites(image, funcs, f["vram"]):
            if (caller, at) not in seen:
                seen.add((caller, at))
                hooks.append({"func": caller, "before_vram": "0x%08X" % at,
                              "text": "{ ctx->r31 = (gpr)(int32_t)0x%08XU; }" % (at + 8)})
    (run_dir / "hooks.json").write_text(json.dumps(hooks, indent=1))
    print("census: %d functions, %d static call sites, %d scripts; run dir %s" % (
        len(chosen), len(hooks), len(scripts), run_dir), flush=True)
    t0 = time.time()
    cap = dc.build_side("cap", "@census-" + args.functions.replace(",", "_")[:40], manifest, run_dir, p, None, 12,
                        extra=["--extra-hooks", str(run_dir / "hooks.json")])
    print("  built in %.0f s" % (time.time() - t0), flush=True)
    names = {f["name"] for f in chosen}
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)

    def one(script):
        sc = oracle.load_script(script)
        frames = sc["frames"] or 1800
        rd = run_dir / "runs" / script
        rd.mkdir(parents=True, exist_ok=True)
        cmd = oracle.host_command(cap["host"], cap["rom"], rd / "o.log", rd / "home", frames, sc["press"],
                                  frames / 30.0 * 6 + 300, calls=rd / "calls", pak=sc["pak"])
        cmd += oracle.script_host_args(sc, cap["host"], cap["rom"], run_dir)     # pak from / poke (Phase 9)
        t = time.time()
        with open(rd / "err", "w") as fe:
            rc = subprocess.run([str(c) for c in cmd], stdout=subprocess.DEVNULL, stderr=fe).returncode
        if rc != 0:
            return script, None, "host exit %d: %s" % (rc, dc._tail(rd / "err", 3))
        summ = summarise(rd / "calls", names)
        if not args.keep_logs:
            (rd / "calls").unlink()
        doc = {"script": script, "frames": frames, "pak": sc["pak"], "seconds": round(time.time() - t, 1),
               "functions_probed": len(names), "functions_reached": len(summ),
               "functions": dict(sorted(summ.items()))}
        (out / (script + ".json")).write_text(compact_json(doc))
        return script, summ, None
    results = {}
    with ThreadPoolExecutor(max(1, args.jobs)) as ex:
        for script, summ, err in ex.map(one, scripts):
            if err:
                print("  %-14s ERROR %s" % (script, err), flush=True)
                continue
            results[script] = summ
            print("  %-14s %3d of %d functions reached, %d calls" % (
                script, len(summ), len(names), sum(s["calls"] for s in summ.values())), flush=True)
    write_summary(out, chosen, results, args.functions, args.tree)
    return 0


OVL_LOCK = REPO / "ovl_matched.lock.json"


def overlay_functions():
    """Every function of runtime images A and B, tagged population/label/matched."""
    import gen_config as gc
    try:
        lock = json.loads(OVL_LOCK.read_text())
    except (OSError, ValueError):
        lock = {}
    out = []
    for pop, key in (("ovl_a", "a"), ("ovl_b", "b")):
        for f in gc.load_extents_functions(REPO / "asm" / "us" / pop / "extents.json"):
            out.append(dict(f, population=pop, label="%s_%s" % (pop, f["name"]),
                            matched=f["name"] in (lock.get(key) or {})))
    return out


def overlay_census(args):
    """Phase 11: which scripts execute which overlay functions (image A = the
    front end, image B = the battle/weapons image; both at 0x8038A400, the host
    dispatches by load state)."""
    p = dc.paths()
    chosen = overlay_functions()
    scripts = dc.all_scripts() if args.scripts == "all" else args.scripts.split(",")
    run_dir = dc.DC_HOME / "census" / ("ovl-" + time.strftime("%Y%m%d-%H%M%S"))
    run_dir.mkdir(parents=True, exist_ok=True)
    manifest = run_dir / "replacements.json"
    manifest.write_text(json.dumps(probe_entries(chosen), indent=1))
    print("census --overlays: %d functions, %d scripts; run dir %s" % (len(chosen), len(scripts), run_dir), flush=True)
    t0 = time.time()
    cap = dc.build_side("cap", "@census-overlays", manifest, run_dir, p, None, 12)
    print("  built in %.0f s" % (time.time() - t0), flush=True)

    def one(script):
        sc = oracle.load_script(script)
        frames = sc["frames"] or 1800
        rd = run_dir / "runs" / script
        rd.mkdir(parents=True, exist_ok=True)
        cmd = oracle.host_command(cap["host"], cap["rom"], rd / "o.log", rd / "home", frames, sc["press"],
                                  frames / 30.0 * 6 + 300, calls=rd / "calls", pak=sc["pak"])
        cmd += oracle.script_host_args(sc, cap["host"], cap["rom"], run_dir)
        with open(rd / "err", "w") as fe:
            rc = subprocess.run([str(c) for c in cmd], stdout=subprocess.DEVNULL, stderr=fe).returncode
        if rc != 0:
            return script, None, "host exit %d: %s" % (rc, dc._tail(rd / "err", 3))
        summ = summarise(rd / "calls", None)
        if not args.keep_logs:
            (rd / "calls").unlink()
        return script, summ, None
    results = {}
    with ThreadPoolExecutor(max(1, args.jobs)) as ex:
        for script, summ, err in ex.map(one, scripts):
            if err:
                print("  %-18s ERROR %s" % (script, err), flush=True)
                continue
            results[script] = summ
            print("  %-18s %3d of %d overlay functions reached" % (script, len(summ), len(chosen)), flush=True)
    write_overlay_summary(Path(args.out), chosen, results, args.tree)
    return 0


def write_overlay_summary(out, chosen, results, commit=None):
    commit = commit or subprocess.run(["git", "-C", str(REPO), "rev-parse", "--short", "HEAD"],
                                      capture_output=True, text=True).stdout.strip() or "unknown"
    funcs = {}
    for f in sorted(chosen, key=lambda f: (f["population"], f["vram"])):
        sc = {s: r[f["label"]]["calls"] for s, r in sorted(results.items()) if f["label"] in r}
        first = min((r[f["label"]]["first_frame"] for r in results.values() if f["label"] in r), default=None)
        funcs[f["label"]] = {"image": f["population"], "name": f["name"], "vram": "0x%08X" % f["vram"],
                             "size": f["size"], "matched": f["matched"], "scripts": sc}
        if first is not None:
            funcs[f["label"]]["first_frame"] = first
    doc = {"generated": datetime.date.today().isoformat(), "tree": commit, "scripts": sorted(results),
           "functions": funcs}
    out.mkdir(parents=True, exist_ok=True)
    (out / "overlays.json").write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
    lines = ["# Overlay call census (recomp Phase 11, lane w31r)", "",
             "Generated by `python3 -m tools.recomp.census --overlays` on rocky (%s, tree %s): one capture build "
             "with a probe on every function of the two runtime images (image A = front end, image B = the "
             "battle/weapon image; both load at 0x8038A400 through inflate_entry and the host dispatches calls by "
             "which image is loaded), every committed script run headless. Detail: `overlays.json`." % (
                 doc["generated"], commit), ""]
    for pop in ("ovl_a", "ovl_b"):
        fs = {n: f for n, f in funcs.items() if f["image"] == pop}
        r = {n: f for n, f in fs.items() if f["scripts"]}
        um = {n: f for n, f in fs.items() if not f["matched"]}
        rum = {n: f for n, f in r.items() if not f["matched"]}
        lines += ["## %s: %d of %d functions reached (%d B of %d B); unmatched: %d of %d reached (%d B of %d B)" % (
            pop, len(r), len(fs), sum(f["size"] for f in r.values()), sum(f["size"] for f in fs.values()),
            len(rum), len(um), sum(f["size"] for f in rum.values()), sum(f["size"] for f in um.values())), "",
            "| function | bytes | matched | first frame | scripts | calls (script: n, top 4) |",
            "|---|---:|---|---:|---:|---|"]
        for n, f in sorted(fs.items(), key=lambda kv: kv[1]["vram"]):
            top = sorted(f["scripts"].items(), key=lambda kv: -kv[1])[:4]
            lines.append("| %s | %d | %s | %s | %d | %s |" % (
                f["name"], f["size"], "yes" if f["matched"] else "", f.get("first_frame", ""), len(f["scripts"]),
                ", ".join("%s: %d" % kv for kv in top) or "never"))
        lines.append("")
    (out / "OVERLAYS.md").write_text("\n".join(lines))


def compact_json(doc):
    """One function per line (diffable, ~half the size of indented JSON)."""
    head = {k: v for k, v in doc.items() if k != "functions"}
    lines = ["{"] + [" %s: %s," % (json.dumps(k), json.dumps(v)) for k, v in sorted(head.items())]
    fl = [" %s: %s" % (json.dumps(n), json.dumps(f, sort_keys=True, separators=(",", ":")))
          for n, f in sorted(doc["functions"].items())]
    lines.append(' "functions": {')
    lines += [x + ("," if i < len(fl) - 1 else "") for i, x in enumerate(fl)]
    return "\n".join(lines) + "\n }\n}\n"


def write_summary(out, chosen, results, which, commit=None):
    commit = commit or subprocess.run(["git", "-C", str(REPO), "rev-parse", "--short", "HEAD"],
                                      capture_output=True, text=True).stdout.strip() or "unknown"
    funcs = {}
    for f in sorted(chosen, key=lambda f: f["vram"]):
        sc = {s: r[f["name"]]["calls"] for s, r in sorted(results.items()) if f["name"] in r}
        funcs[f["name"]] = {"vram": "0x%08X" % f["vram"], "size": f["size"], "scripts": sc}
    doc = {"generated": datetime.date.today().isoformat(), "tree": commit, "functions_set": which,
           "unmatched_source": "game functions not in blob_matched.lock.json",
           "scripts": sorted(results), "functions": funcs}
    (out / "census.json").write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
    reached = {n: f for n, f in funcs.items() if f["scripts"]}
    unreached = {n: f for n, f in funcs.items() if not f["scripts"]}
    lines = ["# Call census of the unmatched game functions (recomp Phase 7)", "",
             "Generated by `python3 -m tools.recomp.census` on rocky (%s, tree %s): one capture build with a probe on "
             "each of the %d %s game functions, every lockstep script in `tools/recomp/scripts/` run headless "
             "(scripts saying `pak from S` start from the pak image S leaves). Per-script detail (calls, first/last frame, "
             "call sites, a0-a3 and register-parameter ranges and kinds): `<script>.json`; the register values are "
             "whatever the caller left there at entry, meaningful only for registers that are parameters "
             "(regmap_verify decides which). draft_check picks its default scripts from `census.json`." % (doc["generated"], commit, len(funcs), which), "",
             "**%d of %d reached by at least one script (%d B of %d B); %d never reached (%d B).**" % (
                 len(reached), len(funcs), sum(f["size"] for f in reached.values()),
                 sum(f["size"] for f in funcs.values()), len(unreached),
                 sum(f["size"] for f in unreached.values())), "",
             "## Unmatched functions NO script reaches (largest first)", "",
             "| function | address | bytes |", "|---|---|---:|"]
    for n, f in sorted(unreached.items(), key=lambda kv: -kv[1]["size"]):
        lines.append("| %s | %s | %d |" % (n, f["vram"], f["size"]))
    lines += ["", "## Reached functions: calls per script", "",
              "| function | bytes | scripts | calls (script: n) |", "|---|---:|---:|---|"]
    for n, f in sorted(reached.items(), key=lambda kv: kv[1]["vram"]):
        lines.append("| %s | %d | %d | %s |" % (n, f["size"], len(f["scripts"]), ", ".join(
            "%s: %d" % (s, c) for s, c in sorted(f["scripts"].items(), key=lambda kv: -kv[1]))))
    (out / "README.md").write_text("\n".join(lines) + "\n")


if __name__ == "__main__":
    sys.exit(main())
