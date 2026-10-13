#!/usr/bin/env python3
"""script_probe: what does an input script reach, and what is on screen? (recomp Phase 8, lane w28r)

    python3 -m tools.recomp.script_probe SCRIPT [SCRIPT ...] [--functions unreached|unmatched|NAME,...]
            [--text] [--text-from F] [--text-to F] [--frames N] [--pak FILE] [--jobs N] [--remote rocky]

For writing `tools/recomp/scripts/*.press` files that reach code no script reaches
yet. One capture build (cached per function set in the draft_check build trees)
with a probe on each chosen game function (default: the functions the committed
census lists as reached by NO script) and a text hook in the game's string queue
(menu_input_process 0x800B6994: every string the menus, HUD and result screens
draw). Each SCRIPT (a name in tools/recomp/scripts/ or a path to a .press file)
runs headless in lockstep; printed per script:

  reached   every probed function that was called: first frame, calls
  text      (--text) `frame  string` each time a string appears on screen
            (a string drawn every frame is listed once, when it appears): the
            headless replacement for looking at the screen while writing a script

Exit 0. Runs on the recomp host; `--remote rocky` from the Pi copies the given
.press files that are not committed and runs in the service tree there.
After adding a script, refresh the census (python3 -m tools.recomp.census).
"""
import argparse
import json
import os
import re
import shlex
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
if str(HERE) not in sys.path:
    sys.path.insert(0, str(HERE))
import census  # noqa: E402
import draft_check as dc  # noqa: E402
import oracle  # noqa: E402

TEXT_FUNC = "menu_input_process"     # (u8 *text, s16 len): copies the string into the draw queue
TEXT_HOOK_OFFSET = 4                 # after `addiu sp`: a0/a1 still the arguments; inside the body, so a
                                     # capture probe's bypass re-entry runs it once per call


def text_hook(funcs):
    f = next((f for f in funcs if f["name"] == TEXT_FUNC), None)
    if f is None:
        return []
    return [{"func": TEXT_FUNC, "before_vram": "0x%08X" % (f["vram"] + TEXT_HOOK_OFFSET),
             "text": "{ void rush_text_log(uint8_t*, recomp_context*); rush_text_log(rdram, ctx); }"}]


def unreached(census_dir=census.OUT):
    doc = json.loads((Path(census_dir) / "census.json").read_text())
    return sorted(n for n, f in doc["functions"].items() if not f.get("scripts"))


def choose(funcs, spec):
    if spec == "unreached":
        want = set(unreached())
        return [f for f in funcs if f["name"] in want]
    if spec == "unmatched":
        return census.unmatched_game(funcs)
    want = set(x.strip() for x in spec.split(",") if x.strip())
    missing = want - {f["name"] for f in funcs}
    if missing:
        raise dc.DCError("not game functions: " + ", ".join(sorted(missing)))
    return [f for f in funcs if f["name"] in want]


def reached(calls_path, frame_of_fn=None):
    """{name: {"calls": n, "first": frame, "last": frame}} from a call log."""
    fns, out = {}, {}
    with open(calls_path) as fh:
        for item in oracle.iter_calls(fh):
            if item[0] == "fn":
                fid = item[2].partition("=")[0]
                fns[item[1]] = fid[2:] if fid.startswith("c_") else fid
            elif item[0] == "c":
                st = out.setdefault(fns[item[3]], {"calls": 0, "first": item[2], "last": item[2]})
                st["calls"] += 1
                st["last"] = item[2]
    return out


_NUMERIC = re.compile(r"^[0-9:./ %-]*$")


def read_text(path, lo=None, hi=None, numeric=False):
    out = []
    try:
        lines = Path(path).read_text(errors="replace").splitlines()
    except OSError:
        return out
    for ln in lines:
        f, _, t = ln.partition("\t")
        if not f.isdigit():
            continue
        f = int(f)
        if (lo is None or f >= lo) and (hi is None or f <= hi) and (numeric or not _NUMERIC.match(t)):
            out.append((f, t))
    return out


def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    ap = argparse.ArgumentParser(prog="python3 -m tools.recomp.script_probe", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("scripts", nargs="+", help="script names (tools/recomp/scripts) or .press paths")
    ap.add_argument("--functions", default="unreached",
                    help="probed functions: unreached (census: no script reaches them; default), unmatched, or "
                    "NAME,...")
    ap.add_argument("--text", action="store_true", help="print the on-screen text timeline")
    ap.add_argument("--text-from", type=int, default=None)
    ap.add_argument("--text-to", type=int, default=None)
    ap.add_argument("--all-text", action="store_true",
                    help="also list strings of digits/punctuation only (HUD counters; hidden by default)")
    ap.add_argument("--frames", type=int, default=None, help="override the script's frame count")
    ap.add_argument("--pak", default=None, help="controller pak image to start from (e.g. one saves.press wrote)")
    ap.add_argument("--pak-write", default=None, metavar="FILE",
                    help="copy --pak (or a fresh image) to FILE and let the run write it (a save to reuse)")
    ap.add_argument("--jobs", type=int, default=int(os.environ.get("RUSH_DC_JOBS", "6")))
    ap.add_argument("--json", default=None)
    ap.add_argument("--remote", default=None)
    ap.add_argument("--remote-tree", default=dc.REMOTE_TREE)
    args = ap.parse_args(argv)
    try:
        if args.remote:
            return remote(args, argv)
        return run(args)
    except (dc.DCError, oracle.OracleError) as exc:
        print("script_probe: ERROR: %s" % exc, file=sys.stderr)
        return 2


def remote(args, argv):
    """Copy uncommitted .press paths to the remote inbox; run there."""
    rid = "probe-%s-%d" % (time.strftime("%Y%m%d-%H%M%S"), os.getpid())
    fwd, files = [], []
    skip = False
    for a in argv:
        if skip:
            skip = False
            continue
        if a in ("--remote", "--remote-tree"):
            skip = True
            continue
        if a.startswith(("--remote=", "--remote-tree=")):
            continue
        if a.endswith(".press") and Path(a).is_file():
            files.append(Path(a))
            a = "$HOME/%s/%s/%s" % (dc.REMOTE_INBOX, rid, Path(a).name)
            fwd.append('"%s"' % a)
            continue
        fwd.append(shlex.quote(a))
    if files:
        subprocess.run(["ssh", args.remote, "mkdir -p %s/%s" % (dc.REMOTE_INBOX, rid)], check=True)
        subprocess.run(["rsync", "-a"] + [str(f) for f in files] + ["%s:%s/%s/" % (args.remote, dc.REMOTE_INBOX, rid)],
                       check=True)
    cmd = "cd %s && python3 -m tools.recomp.script_probe %s" % (args.remote_tree, " ".join(fwd))
    return subprocess.run(["ssh", args.remote, cmd]).returncode


def run(args):
    p = dc.paths()
    import gen_config as gc
    funcs = [f for f in gc.load_game_functions(REPO / "asm" / "us" / "blob") if f.get("name") and f.get("size")]
    chosen = choose(funcs, args.functions)
    names = {f["name"] for f in chosen}
    tag = "@probe-" + (args.functions if args.functions in ("unreached", "unmatched") else
                       "%d-%s" % (len(names), sorted(names)[0][:24]))
    run_dir = dc.DC_HOME / "probe" / ("%s-%d" % (time.strftime("%Y%m%d-%H%M%S"), os.getpid()))
    run_dir.mkdir(parents=True, exist_ok=True)
    manifest = run_dir / "replacements.json"
    manifest.write_text(json.dumps(census.probe_entries(chosen), indent=1))
    (run_dir / "hooks.json").write_text(json.dumps(text_hook(funcs), indent=1))
    t0 = time.time()
    cap = dc.build_side("cap", tag, manifest, run_dir, p, None, 8,
                        extra=["--extra-hooks", str(run_dir / "hooks.json")])
    print("script_probe: %d probed functions (%s); built in %.0f s; run dir %s" % (
        len(names), args.functions, time.time() - t0, run_dir), flush=True)

    def one(script):
        sc = oracle.load_script(script)
        stem = Path(script).stem
        frames = args.frames or sc["frames"] or 1800
        out = run_dir / "runs" / stem
        out.mkdir(parents=True, exist_ok=True)
        cmd = oracle.host_command(cap["host"], cap["rom"], out / "o.log", out / "home", frames, sc["press"],
                                  frames / 30.0 * 6 + 300, calls=out / "calls", pak=sc["pak"])
        cmd += ["--text-log", str(out / "text.log")]
        cmd += oracle.lever_args(sc)                 # poke / pak fail
        if sc.get("pak_from") and not (args.pak or args.pak_write):
            args_pak = oracle.pak_image(sc["pak_from"], cap["host"], cap["rom"], run_dir)
            cmd += ["--pak", str(args_pak)]
        elif sc.get("pak_from") and args.pak_write and not args.pak:
            args.pak = str(oracle.pak_image(sc["pak_from"], cap["host"], cap["rom"], run_dir))
        if args.pak_write:
            src = Path(args.pak) if args.pak else None
            dst = Path(args.pak_write)
            if src:
                dst.write_bytes(src.read_bytes())
            elif dst.exists():
                dst.unlink()            # a fresh image (the host creates and writes it)
            # w29r: --pak-write was passed only with --pak, so a fresh image was never written
            cmd += ["--pak", str(dst.resolve()), "--pak-write"]
        elif args.pak:
            cmd += ["--pak", str(Path(args.pak).resolve())]
        t = time.time()
        with open(out / "err", "w") as fe:
            rc = subprocess.run([str(c) for c in cmd], stdout=subprocess.DEVNULL, stderr=fe).returncode
        if rc != 0:
            return stem, None, None, "host exit %d: %s" % (rc, dc._tail(out / "err", 3))
        r = reached(out / "calls")
        try:
            (out / "calls").unlink()
        except OSError:
            pass
        return stem, r, read_text(out / "text.log", args.text_from, args.text_to, args.all_text), round(time.time() - t, 1)
    summary = {}
    with ThreadPoolExecutor(max(1, args.jobs)) as ex:
        for stem, r, text, extra in ex.map(one, args.scripts):
            print()
            if r is None:
                print("%s: ERROR %s" % (stem, extra))
                continue
            print("%s (%s s): %d of %d probed functions reached" % (stem, extra, len(r), len(names)))
            for n, st in sorted(r.items(), key=lambda kv: kv[1]["first"]):
                print("  reached %-28s first frame %6d, %d calls" % (n, st["first"], st["calls"]))
            if args.text:
                print("  text (frame: strings that appear, one line per frame):")
                byf = {}
                for f, t in text:
                    byf.setdefault(f, []).append(t)
                for f in sorted(byf):
                    print("    %6d  %s" % (f, " | ".join(byf[f])))
            summary[stem] = {"reached": r, "text": text}
    if args.json:
        Path(args.json).write_text(json.dumps(summary, indent=1) + "\n")
    print("\n  run dir %s" % run_dir)
    return 0


if __name__ == "__main__":
    sys.exit(main())
