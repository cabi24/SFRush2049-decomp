#!/usr/bin/env python3
"""Frame oracle (recomp Phase 2): is a build with replaced functions
behaviourally identical to the stock recompiled build?

The host (tools/recomp/runtime, `--oracle-log`) writes per graphics frame the
hash of every display list it walks and of each 4 KiB page of the N64's
8 MiB of rdram (pages only when they change). This tool compares runs:

  compare  A.log B.log [--baseline S2.log ...]     two logs (+ stock repeats)
  run      --stock-host BIN --repl-host BIN ...      run both builds, compare; on a
                                                     page difference rerun both to that
                                                     frame with --dump-at and list words
  worddiff A.bin B.bin [--datasyms F]                word diff of two rdram dumps

Thread stacks (from osCreateThread) are masked out of the page hashes and
hashed separately: their dead frames differ whenever a replaced body's frame
differs, which is not behaviour; a stack-only difference is reported, not
judged.

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
        elif line.startswith("S "):
            _, frame, top = line.split()
            yield ("S", int(frame), int(top, 16))
        elif line.startswith("T "):
            _, frame, thread = line.split()
            yield ("T", int(frame), int(thread, 16))
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
        self.stacks = []
        self.threads = []
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
            elif kind == "S":
                self.stacks.append(item[2])
            elif kind == "T":
                self.threads.append(item[2])
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
    stack_diff = None
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
        stks = {b.fields.get("stk") for b in baselines}
        if stack_diff is None and len(stks) == 1 and candidate.fields.get("stk") not in stks:
            stack_diff = f
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
        "stack_diff_frame": stack_diff,
        "stacks": ["0x%08X" % t for t in sorted(set(candidate.stacks))],
        "threads": sorted(set(candidate.threads)),
        "thread_host_words": next(([int(x, 16) for x in h.split()[1].split(",")] for h in candidate.header
                                   if h.startswith("thread_host_words")), []),
        "events_candidate": ev_c,
        "events_baseline": ev_b,
        "candidate_end": candidate.end,
        "baseline_end": baselines[0].end,
    }
    if first is not None:
        result["verdict"] = "DIFFERENT"
    elif candidate.repl and any(n == 0 for name, n in candidate.repl.items() if "=capture:" not in name):
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
    if result.get("stack_diff_frame") is not None:
        lines.append("  thread-stack contents differ from frame %d (masked out of the page hashes: dead "
                     "frames differ whenever the code differs; not judged)" % result["stack_diff_frame"])
    for name, calls in sorted(result["replaced_calls"].items()):
        lines.append("  replaced %s: %d calls" % (name, calls))
    if result["unstable_pages"]:
        lines.append("  unstable across baseline runs (not judged while unstable): %d pages: %s" % (
            len(result["unstable_pages"]), ", ".join(list(result["unstable_pages"])[:6])
            + (" ..." if len(result["unstable_pages"]) > 6 else "")))
    if result["candidate_end"] != result["baseline_end"]:
        lines.append("  ends: candidate %s, baseline %s" % (result["candidate_end"], result["baseline_end"]))
    return lines


MAX_STACK = 0x2000   # runtime/src/oracle.cpp kMaxStack


def stack_ranges(tops):
    """[(lo, hi)] N64 address ranges the host masks as thread stacks (same rule
    as runtime/src/oracle.cpp: from each top down to the next lower top, at
    most MAX_STACK bytes)."""
    out, prev = [], 0
    for top in sorted(set(tops)):
        lo = max(top - MAX_STACK, prev)
        if lo < top:
            out.append((lo, top))
        prev = top
    return out


def thread_word_ranges(result):
    """[(lo, hi)] of the OSThread host-pointer words the host masks (log "T"
    lines + the "# thread_host_words" header)."""
    offs = result.get("thread_host_words") or []
    return [(t + o, t + o + 4) for t in result.get("threads", []) for o in offs]


def word_diff(a, b, datasyms_path=None, limit=40, skip=()):
    """[(address, a_word, b_word, symbol+offset)] between two rdram dumps
    (host --dump-at, big-endian 8 MiB); `skip` = [(lo, hi)] address ranges
    (thread stacks, pages the baselines disagree on) left out."""
    syms = []
    if datasyms_path and Path(datasyms_path).is_file():
        for m in re.finditer(r'\{ name = "([^"]+)", vram = 0x([0-9A-Fa-f]{8}) \}', Path(datasyms_path).read_text()):
            syms.append((int(m.group(2), 16), m.group(1)))
        syms.sort()
    import bisect
    keys = [v for v, _ in syms]
    out = []
    n = min(len(a), len(b))
    for off in range(0, n - 3, 4):
        if a[off:off + 4] != b[off:off + 4]:
            addr = RAM_BASE + off
            if any(lo <= addr < hi for lo, hi in skip):
                continue
            i = bisect.bisect_right(keys, addr) - 1
            where = "%s+0x%X" % (syms[i][1], addr - syms[i][0]) if i >= 0 and addr - syms[i][0] < 0x10000 else ""
            out.append((addr, a[off:off + 4].hex(), b[off:off + 4].hex(), where))
            if len(out) >= limit:
                break
    return out


# ------------------------------------------------------------------ per-call capture (Phase 3)

def iter_calls(lines):
    """Parse a host --call-log: yields ("fn", index, name, labels, ret_label),
    ("c", seq, frame, fn, [args]), ("C", seq, frame, fn, ret, effect, overflow),
    ("W", seq, addr, old, new), ("END", calls)."""
    for raw in lines:
        line = raw.rstrip("\n")
        if not line:
            continue
        if line.startswith("# fn "):
            parts = line.split()
            idx, name = int(parts[2]), parts[3]
            kv = dict(p.split("=", 1) for p in parts[4:] if "=" in p)
            labels = [] if kv.get("args", "-") == "-" else kv["args"].split(",")
            yield ("fn", idx, name, labels, kv.get("ret", "?"))
        elif line.startswith("#"):
            continue
        elif line.startswith("c "):
            _, seq, frame, fn, args = line.split(" ", 4)
            yield ("c", int(seq), int(frame), int(fn), [] if args == "-" else args.split(","))
        elif line.startswith("C "):
            parts = line.split()
            yield ("C", int(parts[1]), int(parts[2]), int(parts[3]), parts[4][2:], parts[5][2:],
                   len(parts) > 6 and parts[6] == "overflow")
        elif line.startswith("W "):
            _, seq, addr, old, new = line.split()
            yield ("W", int(seq), int(addr, 16), old, new)
        elif line.startswith("END"):
            yield ("END", int(line.split()[1]))
        else:
            raise OracleError(f"unrecognised call-log line: {line[:60]}")


def _fn_name(fns, idx):
    """Function name of a call-log "# fn" entry (`id=mode:function`)."""
    name = fns.get(idx, (str(idx),))[0]
    if "=" not in name:
        return name
    rid, _, rest = name.partition("=")
    func = rest.partition(":")[2] or rid
    return func if func == rid else "%s (%s)" % (func, rid)


def compare_calls(stock_lines, repl_lines):
    """First differing call record between a stock capture log and a replaced
    log (same functions, same order). Returns None when either log has no
    captured functions, else {"verdict": "SAME"|"DIFFERENT", "calls": n, ...}
    with, for DIFFERENT: seq (global entry order), frame, fn, name, fn_call
    (1-based call number of that function), what (args | return | effect |
    sequence | length) and the differing fields [(label, stock, replaced)]."""
    fs, fr = {}, {}
    per_fn = {}
    a_it, b_it = iter_calls(stock_lines), iter_calls(repl_lines)

    def nxt(it, fns):
        for item in it:
            if item[0] == "fn":
                fns[item[1]] = (item[2], item[3], item[4])
                continue
            if item[0] in ("W",):
                continue
            return item
        return None

    calls = 0
    open_calls = {}
    stack = []                    # open (seq, fn) on the stock side, outermost first
    while True:
        a, b = nxt(a_it, fs), nxt(b_it, fr)
        if not fs or not fr:
            return None
        if calls == 0 and a is not None and b is not None:
            def layout(fns):
                return {k: (v[0].partition("=")[0], v[0].rpartition(":")[2], tuple(v[1]), v[2])
                        for k, v in fns.items()}
            if layout(fs) != layout(fr):
                raise OracleError("the call logs capture different functions or layouts (build the stock "
                                  "side with --capture-set SET and the replaced side with --replace-set SET "
                                  "from the same tools revision and the same --probe-callees)")
        if a is None and b is None:
            return {"verdict": "SAME", "calls": calls}
        if a is None or b is None or a[0] == "END" or b[0] == "END":
            if (a and a[0] == "END") and (b and b[0] == "END"):
                return {"verdict": "SAME", "calls": calls}
            last = a if (a and a[0] != "END") else b
            return {"verdict": "DIFFERENT", "what": "length", "calls": calls,
                    "seq": last[1] if last and last[0] in "cC" else None,
                    "frame": last[2] if last and last[0] in "cC" else None,
                    "fn": last[3] if last and last[0] in "cC" else None,
                    "name": _fn_name(fs, last[3]) if last and last[0] in "cC" else None,
                    "fn_call": None,
                    "fields": [("log", "ends" if not a or a[0] == "END" else "continues",
                                "ends" if not b or b[0] == "END" else "continues")]}
        if a[0] == "c":
            calls += 1
            per_fn[a[3]] = per_fn.get(a[3], 0) + 1
            open_calls[a[1]] = per_fn[a[3]]
            stack.append((a[1], a[3]))
        inside = [{"seq": q, "name": _fn_name(fs, f), "fn_call": open_calls.get(q)}
                  for q, f in stack if q != a[1]]
        base = {"verdict": "DIFFERENT", "calls": calls, "seq": a[1], "frame": a[2], "fn": a[3],
                "name": _fn_name(fs, a[3]), "inside": inside,
                "fn_call": open_calls.get(a[1]) if a[0] in "cC" else None}
        if a[0] != b[0] or a[3] != b[3] or a[1] != b[1]:
            def desc(x, fns):
                kind = "entry" if x[0] == "c" else "exit"
                return "%s of %s (seq %d, frame %d)" % (kind, _fn_name(fns, x[3]), x[1], x[2])
            return dict(base, what="sequence", fields=[("next record", desc(a, fs), desc(b, fr))])
        if a[0] == "c":
            if a[4] != b[4]:
                labels = fs.get(a[3], ("", [], ""))[1]
                fields = [(labels[i] if i < len(labels) else "arg%d" % i, x, y)
                          for i, (x, y) in enumerate(zip(a[4], b[4])) if x != y]
                return dict(base, what="args", fields=fields)
            continue
        # exit record
        if a[4] != b[4]:
            return dict(base, what="return", fields=[(fs.get(a[3], ("", [], "v0"))[2], a[4], b[4])])
        if a[5] != b[5] or a[6] != b[6]:
            return dict(base, what="effect", fields=[("effect words:hash", a[5], b[5])],
                        overflow=a[6] or b[6])
        if a[2] != b[2]:
            return dict(base, what="sequence", fields=[("exit frame", str(a[2]), str(b[2]))])
        stack[:] = [x for x in stack if x[0] != a[1]]


def effect_words(lines, seq):
    """{addr: (old, new)} of call `seq` from a --call-detail log."""
    out = {}
    for item in iter_calls(lines):
        if item[0] == "W" and item[1] == seq:
            out[item[2]] = (item[3], item[4])
    return out


def effect_diff(stock, repl, syms=None):
    """[(addr, stock (old,new) or None, replaced (old,new) or None, where)] for
    words whose effect differs between the two calls."""
    import bisect
    keys = [v for v, _ in (syms or [])]
    out = []
    for addr in sorted(set(stock) | set(repl)):
        a, b = stock.get(addr), repl.get(addr)
        if a is not None and b is not None and a[1] == b[1]:
            continue
        where = ""
        if syms:
            i = bisect.bisect_right(keys, addr) - 1
            if i >= 0 and addr - syms[i][0] < 0x10000:
                where = "%s+0x%X" % (syms[i][1], addr - syms[i][0])
        out.append((addr, a, b, where))
    return out


def load_symbol_list(path):
    syms = []
    if path and Path(path).is_file():
        for m in re.finditer(r'\{ name = "([^"]+)", vram = 0x([0-9A-Fa-f]{8}) \}', Path(path).read_text()):
            syms.append((int(m.group(2), 16), m.group(1)))
        syms.sort()
    return syms


def call_report(cr, words=None):
    """Lines for a compare_calls result (and its effect word diff)."""
    if cr is None:
        return ["  per-call: no call logs to compare (stock host without --capture-set hooks?)"]
    if cr["verdict"] == "SAME":
        return ["  per-call: all %d captured calls identical (arguments, returns, effects)" % cr["calls"]]
    head = "FIRST DIFFERING CALL: seq %s = %s call %s, frame %s: %s" % (
        cr.get("seq"), cr.get("name"), cr.get("fn_call"), cr.get("frame"), cr["what"])
    lines = [head]
    if cr.get("inside"):
        lines.append("  inside: " + " > ".join("%s call %s (seq %s)" % (o["name"], o["fn_call"], o["seq"])
                                               for o in cr["inside"]))
    for label, x, y in cr.get("fields", []):
        lines.append("  %s: stock %s, replaced %s" % (label, x, y))
    for addr, a, b, where in (words or [])[:24]:
        fa = "%s->%s" % a if a else "unchanged"
        fb = "%s->%s" % b if b else "unchanged"
        lines.append("  word 0x%08X stock %s replaced %s  %s" % (addr, fa, fb, where))
    if words and len(words) > 24:
        lines.append("  ... %d more words" % (len(words) - 24))
    return lines


# ------------------------------------------------------------------ input scripts

SCRIPT_DIR = Path(__file__).resolve().parent / "scripts"
_BUTTONS = {"A", "B", "Z", "START", "UP", "DOWN", "LEFT", "RIGHT", "L", "R", "CU", "CD", "CL", "CR",
            "STICKUP", "STICKDOWN", "STICKLEFT", "STICKRIGHT"}


def parse_script(text, name="script"):
    """A .press script -> {"press": host --press string, "frames": N or None,
    "controllers": n}. Lines: `TIME BUTTON[@PORT] [DURATION]` (seconds of game
    time = frames/30; PORT 1-4, default 1; duration default 0.2), `frames N`
    (default run length), `# comment`. Times need not be sorted."""
    items, frames, ports = [], None, {1}
    for n, raw in enumerate(text.splitlines(), 1):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        parts = line.split()
        if parts[0] == "frames":
            if len(parts) != 2 or not parts[1].isdigit():
                raise OracleError(f"{name}:{n}: frames takes one integer")
            frames = int(parts[1])
            continue
        if len(parts) not in (2, 3):
            raise OracleError(f"{name}:{n}: expected TIME BUTTON[@PORT] [DURATION]")
        try:
            t = float(parts[0])
            dur = float(parts[2]) if len(parts) == 3 else None
        except ValueError:
            raise OracleError(f"{name}:{n}: bad number in {line!r}")
        button, _, port = parts[1].partition("@")
        if button not in _BUTTONS:
            raise OracleError(f"{name}:{n}: unknown button {button}")
        if port:
            if port not in ("1", "2", "3", "4"):
                raise OracleError(f"{name}:{n}: port must be 1-4")
            ports.add(int(port))
        if t < 0 or (dur is not None and dur <= 0):
            raise OracleError(f"{name}:{n}: times must be >= 0 and durations > 0")
        item = "%g:%s" % (t, button + ("@" + port if port and port != "1" else ""))
        if dur is not None:
            item += ":%g" % dur
        items.append((t, n, item))
    if not items:
        raise OracleError(f"{name}: no presses")
    items.sort()
    return {"press": ",".join(i for _, _, i in items), "frames": frames, "controllers": max(ports)}


def load_script(arg):
    """--script NAME (tools/recomp/scripts/NAME.press) or a path."""
    p = Path(arg)
    if not p.is_file():
        p = SCRIPT_DIR / (arg if arg.endswith(".press") else arg + ".press")
    if not p.is_file():
        raise OracleError(f"no input script {arg} (looked in {SCRIPT_DIR})")
    return parse_script(p.read_text(), p.name)


# ------------------------------------------------------------------ running the host

DEFAULT_PRESS = ("5:START,8:START,11:A,14:A,17:A,20:START,23:A,26:A,29:A,32:A,35:START,"
                 "38:A,41:A,44:A,47:A,50:A")


def host_command(host, rom, log, home, frames, press, seconds, dump=None, calls=None, detail=None):
    cmd = [str(host), "--rom", str(rom), "--home", str(home), "--clock", "frames",
           "--frames", str(frames), "--seconds", str(seconds), "--press", press,
           "--oracle-log", str(log)]
    if dump:
        cmd += ["--dump-at", "%d:%s" % dump]
    if calls:
        cmd += ["--call-log", str(calls)]
    if detail:
        cmd += ["--call-detail", str(detail)]
    return cmd


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
    c.add_argument("--calls", nargs=2, metavar=("STOCK_CALLS", "REPL_CALLS"),
                   help="also compare two --call-log files (first differing call)")
    k = sub.add_parser("calls", help="first differing call of two --call-log files")
    k.add_argument("stock")
    k.add_argument("repl")
    k.add_argument("--datasyms")
    sp = sub.add_parser("script", help="print a .press script as the host's --press string")
    sp.add_argument("name")
    w = sub.add_parser("worddiff", help="word diff of two --dump-at rdram images")
    w.add_argument("a")
    w.add_argument("b")
    w.add_argument("--datasyms")
    w.add_argument("--limit", type=int, default=40)
    r = sub.add_parser("run", help="run stock and replaced hosts, then compare")
    r.add_argument("--stock-host", required=True)
    r.add_argument("--repl-host", required=True)
    r.add_argument("--rom-stock", required=True, help="synthetic ROM of the stock build")
    r.add_argument("--rom-repl", required=True, help="synthetic ROM of the replaced build")
    r.add_argument("--out", required=True, help="directory for logs and the verdict")
    r.add_argument("--frames", type=int, default=1800, help="graphics frames (30/s; default 60 s)")
    r.add_argument("--press", default=None, help="input script as a --press string (seconds = frames/30)")
    r.add_argument("--script", default=None,
                   help="input script: NAME (tools/recomp/scripts/NAME.press) or a path; default = race")
    r.add_argument("--stock-runs", type=int, default=2, help="stock repeats (determinism baseline)")
    r.add_argument("--reuse-stock", action="store_true", help="keep existing stock logs in --out")
    r.add_argument("--no-calls", action="store_true",
                   help="skip the per-call comparison (it needs --stock-host built with "
                   "gen_config --capture-set SET for the replaced set)")
    for p in (c, r):
        p.add_argument("--datasyms", help="build/recomp/rush2049.datasyms.toml (names pages)")
        p.add_argument("--json", help="write the result as JSON")
    args = ap.parse_args(argv)
    if args.cmd == "worddiff":
        for addr, wa, wb, where in word_diff(Path(args.a).read_bytes(), Path(args.b).read_bytes(),
                                             args.datasyms, args.limit):
            print("0x%08X %s %s  %s" % (addr, wa, wb, where))
        return 0
    try:
        if args.cmd == "script":
            sc = load_script(args.name)
            print(sc["press"])
            if sc["frames"]:
                print("# frames %d, controllers %d" % (sc["frames"], sc["controllers"]), file=sys.stderr)
            return 0
        if args.cmd == "calls":
            with open(args.stock) as fa, open(args.repl) as fb:
                cr = compare_calls(fa, fb)
            for line in call_report(cr):
                print(line)
            return 0 if cr is None or cr["verdict"] == "SAME" else 1
        cr = None
        if args.cmd == "compare":
            baselines = [Run(args.baseline)] + [Run(p) for p in args.more]
            cand = Run(args.candidate)
            result = compare(cand, baselines)
            if args.calls:
                with open(args.calls[0]) as fa, open(args.calls[1]) as fb:
                    cr = compare_calls(fa, fb)
        else:
            out = Path(args.out)
            out.mkdir(parents=True, exist_ok=True)
            press = args.press
            frames = args.frames
            if args.script:
                sc = load_script(args.script)
                press = press or sc["press"]
                if sc["frames"] and args.frames == 1800 and "--frames" not in (argv or sys.argv):
                    frames = sc["frames"]
            press = press or DEFAULT_PRESS
            seconds = frames / 30.0 * 4 + 120
            calls = not args.no_calls
            jobs, stock_logs = [], []
            for i in range(args.stock_runs):
                log = out / ("stock%d.log" % (i + 1))
                stock_logs.append(log)
                if args.reuse_stock and log.is_file():
                    continue
                jobs.append((host_command(args.stock_host, args.rom_stock, log, out / ("home_s%d" % i),
                                          frames, press, seconds,
                                          calls=out / "stock.calls" if calls and i == 0 else None),
                             out / ("stock%d.err" % (i + 1))))
            jobs.append((host_command(args.repl_host, args.rom_repl, out / "repl.log", out / "home_r",
                                      frames, press, seconds, calls=out / "repl.calls" if calls else None),
                         out / "repl.err"))
            run_hosts(jobs)
            baselines = [Run(p) for p in stock_logs]
            cand = Run(out / "repl.log")
            if not args.json:
                args.json = str(out / "verdict.json")
            result = compare(cand, baselines)
            result.update(script=args.script or ("--press" if args.press else "race (default)"),
                          press=press, frames_requested=frames)
            if calls and (out / "stock.calls").is_file() and (out / "repl.calls").is_file():
                with open(out / "stock.calls") as fa, open(out / "repl.calls") as fb:
                    cr = compare_calls(fa, fb)
                if cr and cr["verdict"] == "DIFFERENT" and cr["what"] == "effect":
                    # Word-level effect of that call: rerun both to the call's frame with --call-detail.
                    f = (cr["frame"] or 0) + 2
                    da, db = out / "detail_s.calls", out / "detail_r.calls"
                    run_hosts([(host_command(args.stock_host, args.rom_stock, out / "detail_s.log",
                                             out / "home_cs", f, press, seconds, calls=da, detail=cr["seq"]),
                                out / "detail_s.err"),
                               (host_command(args.repl_host, args.rom_repl, out / "detail_r.log",
                                             out / "home_cr", f, press, seconds, calls=db, detail=cr["seq"]),
                                out / "detail_r.err")])
                    with open(da) as fa, open(db) as fb:
                        cr["words"] = effect_diff(effect_words(fa, cr["seq"]), effect_words(fb, cr["seq"]),
                                                  load_symbol_list(args.datasyms))
            fd = result.get("first_diff") or {}
            if fd.get("frame") and fd.get("pages"):
                # Word-level detail: rerun stock 1 and the candidate to that frame with a dump.
                f = fd["frame"]
                da, db = out / "stock_at_diff.bin", out / "repl_at_diff.bin"
                run_hosts([(host_command(args.stock_host, args.rom_stock, out / "dump_s.log", out / "home_ds", f,
                                         press, seconds, (f, da)), out / "dump_s.err"),
                           (host_command(args.repl_host, args.rom_repl, out / "dump_r.log", out / "home_dr", f,
                                         press, seconds, (f, db)), out / "dump_r.err")])
                skip = stack_ranges([int(t, 16) for t in result["stacks"]])
                skip += thread_word_ranges(result)
                skip += [(int(r.split("-")[0], 16), int(r.split("-")[1], 16) + 1) for r in result["unstable_pages"]]
                fd["words"] = word_diff(da.read_bytes(), db.read_bytes(), args.datasyms, skip=skip)
    except OracleError as exc:
        print("oracle: %s" % exc, file=sys.stderr)
        return 2
    if cr is not None:
        result["calls"] = {k: v for k, v in cr.items() if k != "words"}
        if cr.get("words") is not None:
            result["calls"]["words"] = [["0x%08X" % a, x, y, w] for a, x, y, w in cr["words"]]
        if cr["verdict"] == "DIFFERENT":
            result["verdict"] = "DIFFERENT"
    lines = []
    if cr is not None and cr["verdict"] == "DIFFERENT":
        lines += call_report(cr, cr.get("words"))
        fd = result.get("first_diff")
        lines.append("  frames: " + (report(result, load_datasyms(args.datasyms))[0]
                                     if fd and fd.get("frame") else "no judged frame difference"))
        lines += report(result, load_datasyms(args.datasyms))[1:]
    else:
        lines += report(result, load_datasyms(args.datasyms))
        if cr is not None or args.cmd == "run":
            lines += call_report(cr) if cr is not None else []
    for line in lines:
        print(line)
    if cr is None or cr["verdict"] != "DIFFERENT":
        for addr, wa, wb, where in (result.get("first_diff") or {}).get("words", [])[:16]:
            print("  word 0x%08X stock %s replaced %s  %s" % (addr, wa, wb, where))
    if args.json:
        Path(args.json).write_text(json.dumps(result, indent=1, sort_keys=True, default=list) + "\n")
    return 0 if result["verdict"] == "EQUIVALENT" else 1


if __name__ == "__main__":
    sys.exit(main())
