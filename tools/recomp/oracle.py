#!/usr/bin/env python3
"""Frame oracle (recomp Phase 2): is a build with replaced functions
behaviourally identical to the stock recompiled build?

The host (tools/recomp/runtime, `--oracle-log`) writes per graphics frame the
hash of every display list it walks and of each 4 KiB page of the N64's
8 MiB of rdram (pages only when they change). This tool compares runs:

  compare  A.log B.log [--baseline S2.log ...]     two logs (+ stock repeats)
  run      --stock-host BIN --repl-host BIN ...      run both builds, compare

Determinism. The input script is frame-clocked (`--clock frames`) and the
game's clock reads are virtual, but the runtime's VI/audio threads still run
on wall time, so a few pages (audio and scheduler state, stacks) differ
between two runs of the SAME build. A page is therefore compared at a frame
only when every baseline (stock) run agrees on it at that frame; the rest are
"unstable" at that frame and reported, not judged. Display lists are judged
the same way (a frame whose baseline runs disagree on the display lists ends
the deterministic horizon). With one baseline run every page is judged.

Verdict:
  EQUIVALENT        no judged display list or page differs (and every
                    replaced function was called at least once)
  NOT EXERCISED     equivalent, but a replaced function was never called
  DIFFERENT         first differing frame, then display list and/or pages
                    (N64 address ranges; with --datasyms, the data symbols
                    in each page)

Stdlib only, Python 3.9+. Logs are private (rdram hashes of the ROM's code
running); keep them under build/.
"""
import argparse
import json
import os
import re
import shlex
import subprocess
import sys
from pathlib import Path

PAGE = 4096
RAM_BASE = 0x80000000


class OracleError(RuntimeError):
    pass


# ------------------------------------------------------------------ log format

def iter_log(lines):
    """Yield ("F", frame, {dl, dls, dlb, ram}, {page: hash}) per frame (the
    pages that changed since the previous frame), ("E", frame, text),
    ("R", name, calls), ("END", frame, why), ("H", text) for header lines."""
    pending = None
    for raw in lines:
        line = raw.rstrip("\n")
        if not line:
            continue
        if line.startswith("P"):
            if pending is None:
                raise OracleError("page line before any frame")
            for item in line[1:].split():
                page, h = item.split(":")
                pending[3][int(page)] = h
            continue
        if pending is not None:
            yield pending
            pending = None
        if line.startswith("F "):
            parts = line.split()
            fields = dict(kv.split("=", 1) for kv in parts[2:])
            pending = ("F", int(parts[1]), fields, {})
        elif line.startswith("E "):
            _, frame, text = line.split(" ", 2)
            yield ("E", int(frame), text)
        elif line.startswith("R "):
            _, name, calls = line.split()
            yield ("R", name, int(calls))
        elif line.startswith("END"):
            parts = line.split()
            yield ("END", int(parts[1]), " ".join(parts[2:]))
        elif line.startswith("#"):
            yield ("H", line[1:].strip())
        else:
            raise OracleError(f"unrecognised log line: {line[:60]}")
    if pending is not None:
        yield pending


class Run:
    """One log, read frame by frame (keeps the current page snapshot only)."""

    def __init__(self, path):
        self.path = str(path)
        self.events, self.repl, self.header, self.end = [], {}, [], None
        self._it = iter_log(open(path))
        self.pages = {}
        self.frame = 0
        self.fields = None

    def next_frame(self):
        """Advance to the next frame; False at the end."""
        for item in self._it:
            kind = item[0]
            if kind == "F":
                _, self.frame, self.fields, changed = item
                self.pages.update(changed)
                return True
            if kind == "E":
                self.events.append((item[1], item[2]))
            elif kind == "R":
                self.repl[item[1]] = item[2]
            elif kind == "END":
                self.end = (item[1], item[2])
            elif kind == "H":
                self.header.append(item[1])
        return False

    def drain(self):
        while self.next_frame():
            pass


def page_range(page):
    lo = RAM_BASE + page * PAGE
    return "0x%08X-0x%08X" % (lo, lo + PAGE - 1)


def load_datasyms(path):
    """{page: [names]} from rush2049.datasyms.toml (gen_config output)."""
    out = {}
    if not path or not Path(path).is_file():
        return out
    for m in re.finditer(r'\{ name = "([^"]+)", vram = 0x([0-9A-Fa-f]{8}) \}', Path(path).read_text()):
        vram = int(m.group(2), 16)
        if RAM_BASE <= vram < RAM_BASE + (8 << 20):
            out.setdefault((vram - RAM_BASE) // PAGE, []).append(m.group(1))
    return out


# ------------------------------------------------------------------ comparison

def compare(candidate, baselines, max_report=8):
    """Compare a candidate Run against one or more baseline Runs (stock).

    Returns a dict: verdict, frames, horizon (last frame every baseline agreed
    on the display lists, None = all), first_diff {frame, dl, pages}, unstable
    page counts, replaced-call counts, event differences."""
    if not baselines:
        raise OracleError("need at least one baseline run")
    first = None
    unstable = {}
    horizon = None
    frames = 0
    judged_pages = 0
    while True:
        more = [b.next_frame() for b in baselines]
        more_c = candidate.next_frame()
        if not all(more) or not more_c:
            break
        f = candidate.frame
        if any(b.frame != f for b in baselines):
            raise OracleError(f"frame numbering differs at {f}")
        frames = f
        dls = {b.fields["dl"] for b in baselines}
        dl_stable = len(dls) == 1 and horizon is None
        if len(dls) != 1 and horizon is None:
            horizon = f - 1
        dl_diff = dl_stable and candidate.fields["dl"] not in dls
        diff_pages = []
        all_pages = set(candidate.pages)
        for b in baselines:
            all_pages |= set(b.pages)
        for p in sorted(all_pages):
            hs = {b.pages.get(p) for b in baselines}
            if len(hs) != 1:
                unstable[p] = unstable.get(p, 0) + 1
                continue
            judged_pages += 1
            if candidate.pages.get(p) not in hs:
                diff_pages.append(p)
        if (dl_diff or diff_pages) and first is None:
            first = {"frame": f, "dl": dl_diff,
                     "dl_hash": {"candidate": candidate.fields["dl"], "baseline": sorted(dls)[0]},
                     "dls": {"candidate": candidate.fields.get("dls"), "baseline": baselines[0].fields.get("dls")},
                     "pages": diff_pages, "pages_total": len(diff_pages)}
            break
    for r in [candidate] + list(baselines):
        r.drain()
    ev_c = [(f, t) for f, t in candidate.events]
    ev_b = [(f, t) for f, t in baselines[0].events]
    result = {
        "frames": frames,
        "baselines": len(baselines),
        "horizon": horizon,
        "first_diff": first,
        "unstable_pages": {page_range(p): n for p, n in sorted(unstable.items())},
        "replaced_calls": dict(candidate.repl),
        "events_candidate": ev_c,
        "events_baseline": ev_b,
        "candidate_end": candidate.end,
        "baseline_end": baselines[0].end,
    }
    if first is not None:
        result["verdict"] = "DIFFERENT"
    elif candidate.repl and any(n == 0 for n in candidate.repl.values()):
        result["verdict"] = "NOT EXERCISED"
    else:
        result["verdict"] = "EQUIVALENT"
    if first is None and ev_c != ev_b and result["verdict"] == "EQUIVALENT":
        result["verdict"] = "DIFFERENT"
        result["first_diff"] = {"frame": None, "events": True}
    return result


def report(result, datasyms=None, max_pages=8):
    """Human-readable verdict lines."""
    lines = []
    v = result["verdict"]
    fd = result["first_diff"]
    hz = result["horizon"]
    hz_txt = "all frames" if hz is None else "frames 1-%d" % hz
    if v == "DIFFERENT" and fd and fd.get("frame"):
        what = []
        if fd["dl"]:
            what.append("display lists (dl %s vs stock %s, %s vs %s lists)" % (
                fd["dl_hash"]["candidate"], fd["dl_hash"]["baseline"], fd["dls"]["candidate"],
                fd["dls"]["baseline"]))
        if fd["pages"]:
            what.append("%d rdram page(s)" % fd["pages_total"])
        lines.append("DIFFERENT at frame %d: %s" % (fd["frame"], "; ".join(what)))
        for p in fd["pages"][:max_pages]:
            syms = (datasyms or {}).get(p, [])
            lines.append("  page %4d %s%s" % (p, page_range(p),
                                              ("  " + ", ".join(syms[:6]) + (" ..." if len(syms) > 6 else ""))
                                              if syms else ""))
        if len(fd["pages"]) > max_pages:
            lines.append("  ... %d more pages" % (len(fd["pages"]) - max_pages))
    elif v == "DIFFERENT":
        lines.append("DIFFERENT: events differ (overlay loads)")
    else:
        lines.append("%s over %d frames (judged: %s, %d baseline run(s))" % (
            v, result["frames"], hz_txt, result["baselines"]))
    for name, calls in sorted(result["replaced_calls"].items()):
        lines.append("  replaced %s: %d calls" % (name, calls))
    if result["unstable_pages"]:
        lines.append("  unstable across baseline runs (not judged while unstable): %d pages: %s" % (
            len(result["unstable_pages"]), ", ".join(list(result["unstable_pages"])[:6])
            + (" ..." if len(result["unstable_pages"]) > 6 else "")))
    if result["candidate_end"] != result["baseline_end"]:
        lines.append("  ends: candidate %s, baseline %s" % (result["candidate_end"], result["baseline_end"]))
    return lines


# ------------------------------------------------------------------ running the host

DEFAULT_PRESS = ("5:START,8:START,11:A,14:A,17:A,20:START,23:A,26:A,29:A,32:A,35:START,"
                 "38:A,41:A,44:A,47:A,50:A")


def host_command(host, rom, log, home, frames, press, seconds):
    return [str(host), "--rom", str(rom), "--home", str(home), "--clock", "frames",
            "--frames", str(frames), "--seconds", str(seconds), "--press", press,
            "--oracle-log", str(log)]


def run_hosts(jobs):
    """Run [(cmd, stderr_path)] in parallel; raise if one fails."""
    procs = []
    for cmd, err in jobs:
        procs.append((cmd, subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=open(err, "w"))))
    for cmd, p in procs:
        if p.wait() != 0:
            raise OracleError("host exited %d: %s" % (p.returncode, " ".join(shlex.quote(c) for c in cmd)))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("compare", help="compare logs")
    c.add_argument("baseline")
    c.add_argument("candidate")
    c.add_argument("--baseline", dest="more", action="append", default=[],
                   help="more stock runs (pages they disagree on are not judged)")
    r = sub.add_parser("run", help="run stock and replaced hosts, then compare")
    r.add_argument("--stock-host", required=True)
    r.add_argument("--repl-host", required=True)
    r.add_argument("--rom-stock", required=True, help="synthetic ROM of the stock build")
    r.add_argument("--rom-repl", required=True, help="synthetic ROM of the replaced build")
    r.add_argument("--out", required=True, help="directory for logs and the verdict")
    r.add_argument("--frames", type=int, default=1800, help="graphics frames (30/s; default 60 s)")
    r.add_argument("--press", default=DEFAULT_PRESS, help="input script, seconds = frames/30")
    r.add_argument("--stock-runs", type=int, default=2, help="stock repeats (determinism baseline)")
    r.add_argument("--reuse-stock", action="store_true", help="keep existing stock logs in --out")
    for p in (c, r):
        p.add_argument("--datasyms", help="build/recomp/rush2049.datasyms.toml (names pages)")
        p.add_argument("--json", help="write the result as JSON")
    args = ap.parse_args(argv)
    try:
        if args.cmd == "compare":
            baselines = [Run(args.baseline)] + [Run(p) for p in args.more]
            cand = Run(args.candidate)
        else:
            out = Path(args.out)
            out.mkdir(parents=True, exist_ok=True)
            seconds = args.frames / 30.0 * 4 + 120
            jobs, stock_logs = [], []
            for i in range(args.stock_runs):
                log = out / ("stock%d.log" % (i + 1))
                stock_logs.append(log)
                if args.reuse_stock and log.is_file():
                    continue
                jobs.append((host_command(args.stock_host, args.rom_stock, log, out / ("home_s%d" % i),
                                          args.frames, args.press, seconds), out / ("stock%d.err" % (i + 1))))
            jobs.append((host_command(args.repl_host, args.rom_repl, out / "repl.log", out / "home_r",
                                      args.frames, args.press, seconds), out / "repl.err"))
            run_hosts(jobs)
            baselines = [Run(p) for p in stock_logs]
            cand = Run(out / "repl.log")
            if not args.json:
                args.json = str(out / "verdict.json")
        result = compare(cand, baselines)
    except OracleError as exc:
        print("oracle: %s" % exc, file=sys.stderr)
        return 2
    for line in report(result, load_datasyms(args.datasyms)):
        print(line)
    if args.json:
        Path(args.json).write_text(json.dumps(result, indent=1, sort_keys=True) + "\n")
    return 0 if result["verdict"] == "EQUIVALENT" else 1


if __name__ == "__main__":
    sys.exit(main())
