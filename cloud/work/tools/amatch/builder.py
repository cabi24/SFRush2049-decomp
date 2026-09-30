#!/usr/bin/env python3
"""builder.py: in-process compile + score for the matching engine (stdlib only).

Reuses tools/cloud/score.py internals (compile_single, compile_group, text_words, symbols,
relocate) so strict results are identical to `score.py fn|group`, and adds the instruction-
aligned scores from aligned.py.

    compile_fn(src_text, fn, flags=None, ...)     -> result dict (never raises)
    compile_group(group_dir, overrides=None, ...) -> result dict (never raises)
    score_many(jobs, jobs_n=4, ...)               -> [result, ...] in job order (process pool)

fn result keys: the shared score dict (strict_diff, size, target_size, extra, aligned_exact,
aligned_opcode, aligned_opcode_reg, target_words, matched, unverified) plus unresolved, errors,
name, words (resolved compiled slice), err (None or text), cached, secs.
group result: {"kind": "group", "members": {name: score dict}, "context": {...}, "matched": bool,
"emitted": {name: words}, "err": None|text, ...}. A compile error gives err text and worst-case
scores (strict_diff = target size, aligned = 0) so optimizers can rank it last.

Group overrides: {"files": {name: text}, "flags": str, "keep": [...], "members": [...],
"context": [...], "targets": {...}}. Targets not in asm/us/blob come from group.json "targets"
({name: {"addr": "0x..", "words": N}}), read from the inflated game image (extscore.py technique).

Cache: build/amatch_cache/<2 hex>/<sha256>.json keyed on content+flags+IDO+score.py identity.

CLI:  builder.py fn FILE FUNC [--flags ".."] [--json] [--no-cache] [--hunks]
      builder.py group DIR [--json] [--no-cache]
If --flags is omitted for `fn`, a leading `/* flags: ... */` comment in FILE is used, else the default.
"""
import argparse
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
import tempfile
import time
import zlib
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT / "tools" / "cloud"))
sys.path.insert(0, str(HERE.parent))
import score  # noqa: E402
from amatch import aligned  # noqa: E402

VERSION = 1
DEFAULT_FLAGS = score.DEFAULT_FLAGS
CACHE_DIR = Path(os.environ.get("AMATCH_CACHE", ROOT / "build" / "amatch_cache"))
IMAGE_BASE = 0x80086A50
DEFAULT_TIMEOUT = 120.0

_targets = None
_addresses = None
_image = None


# --- cached verified inputs -----------------------------------------------------------

def get_targets():
    global _targets
    if _targets is None:
        _targets = score.targets()
    return _targets


def get_addresses():
    global _addresses
    if _addresses is None:
        _addresses = score.image_symbols()
    return _addresses


def _game_image():
    global _image
    if _image is None:
        data = (ROOT / "assets/us/data.bin").read_bytes()
        _image = zlib.decompressobj(-15).decompress(data[0xB0CB10 - 0x10000:0xB0CB10 - 0x10000 + 326180])
    return _image


def extra_targets(spec_targets):
    out = {}
    for n, t in (spec_targets or {}).items():
        o = int(t["addr"], 16) - IMAGE_BASE
        img = _game_image()
        out[n] = list(struct.unpack(">%dI" % t["words"], img[o:o + 4 * t["words"]]))
    return out


def ido_available():
    return (score.IDO / "cc").exists()


def _ido_id():
    try:
        st = (score.IDO / "cc").stat()
        return f"{st.st_size}:{st.st_mtime_ns}"
    except OSError:
        return "noido"


def _env_id():
    try:
        st = Path(score.__file__).stat()
        sums = (score.ASM_DIR / "SHA256SUMS").read_bytes()
        return f"{VERSION}:{st.st_size}:{st.st_mtime_ns}:{hashlib.sha1(sums).hexdigest()}"
    except OSError:
        return str(VERSION)


_ENV = None


def _key(*parts):
    global _ENV
    if _ENV is None:
        _ENV = _env_id() + "|" + _ido_id()
    h = hashlib.sha256(_ENV.encode())
    for p in parts:
        h.update(b"\0")
        h.update(json.dumps(p, sort_keys=True).encode())
    return h.hexdigest()


def _cache_get(key):
    p = CACHE_DIR / key[:2] / (key + ".json")
    try:
        return json.loads(p.read_text())
    except (OSError, ValueError):
        return None


def _cache_put(key, result):
    d = CACHE_DIR / key[:2]
    try:
        d.mkdir(parents=True, exist_ok=True)
        fd, tmp = tempfile.mkstemp(dir=d, suffix=".tmp")
        with os.fdopen(fd, "w") as f:
            json.dump(result, f)
        os.replace(tmp, d / (key + ".json"))
    except OSError:
        pass


# --- timeouts around IDO subprocesses ----------------------------------------------------

class _Timeout:
    """Temporarily wrap score._run so every IDO step gets a timeout."""
    def __init__(self, secs, cwd=None):
        self.secs = secs
        self.cwd = cwd

    def __enter__(self):
        self.orig = score._run
        secs, cwd = self.secs, self.cwd

        def run(cmd, **kw):
            kw.setdefault("timeout", secs)
            if cwd and "cwd" not in kw:  # -O3 -c leaves <name>.u in cwd: isolate per job
                kw["cwd"] = cwd
            try:
                return self.orig(cmd, **kw)
            except subprocess.TimeoutExpired:
                return subprocess.CompletedProcess(cmd, 124, "", f"timeout after {secs}s")
        score._run = run

    def __exit__(self, *a):
        score._run = self.orig


# --- scoring of one compiled object ----------------------------------------------------------

def _worst(want_len, err, name=None):
    d = {"strict_diff": want_len, "size": 0, "target_size": want_len, "target_words": want_len,
         "extra": 0, "aligned_exact": 0, "aligned_opcode": 0, "aligned_opcode_reg": 0,
         "unresolved": [], "unverified": [], "errors": [], "matched": False,
         "words": [], "err": err}
    if name:
        d["name"] = name
    return d


def score_member(obj, name, want, words, functions):
    """Same comparison as score.compare, plus aligned scores. Never raises."""
    start = functions.get(name)
    if start is None:
        return _worst(len(want), f"{name} is not a defined function in the compiled object", name)
    end = min((o for o in functions.values() if o > start), default=len(words) * 4)
    target_end = start + len(want) * 4
    extra = sum(w != 0 for w in words[target_end // 4:end // 4])
    cmp_end = min(target_end, end)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, cmp_end, get_addresses())
    cmp_slice = resolved[start // 4:cmp_end // 4]
    full = resolved[start // 4:end // 4]
    while len(full) > len(want) and full[-1] == 0:  # alignment padding
        full = full[:-1]
    # masks keyed by .text byte offset -> function-relative word index
    m = {(o - start) // 4: v for o, v in masks.items()}
    # strict compare exactly as score.compare (missing words count as differing)
    d = aligned.score_dict(want, cmp_slice, m, extra, unresolved, unverified, errors,
                           size=len(full))
    # aligned scores should see the whole emitted function, not just the compared prefix
    d.update(aligned.aligned_scores(want, full, m))
    d["name"] = name
    d["words"] = full
    d["err"] = None
    d["matched_allow_unverified"] = (d["strict_diff"] == 0 and extra == 0 and not unresolved
                                     and not errors)
    return d


# --- single function ---------------------------------------------------------------------------

def parse_flags_comment(src_text):
    m = re.match(r"\s*/\*\s*flags:\s*(.*?)\s*\*/", src_text)
    return m.group(1) if m else None


def compile_fn(src_text, fn, flags=None, timeout=DEFAULT_TIMEOUT, use_cache=True,
               extra=None, include_dir=None):
    """Compile src_text (one translation unit) with IDO and score function `fn`.
    `extra`: {name: words} additional targets. include_dir: directory copied-next-to for
    relative #includes. Returns a result dict; never raises."""
    t0 = time.time()
    flags = flags or DEFAULT_FLAGS
    key = None
    try:
        targets = dict(get_targets())
        if extra:
            targets.update(extra)
        want = targets.get(fn)
        if want is None:
            return dict(_worst(0, f"no target section .text.{fn}", fn), kind="fn", cached=False,
                        secs=time.time() - t0)
        if use_cache:
            key = _key("fn", src_text, fn, flags, len(want), hashlib.sha1(
                struct.pack(f">{len(want)}I", *want)).hexdigest())
            hit = _cache_get(key)
            if hit is not None:
                hit["cached"] = True
                hit["secs"] = time.time() - t0
                return hit
        if not ido_available():
            return dict(_worst(len(want), "IDO missing: run tools/cloud/setup.sh", fn),
                        kind="fn", cached=False, secs=time.time() - t0)
        with tempfile.TemporaryDirectory(prefix="am-") as tmp:
            tmpd = Path(tmp)
            src = tmpd / "src.c"
            src.write_text(src_text)
            obj = tmpd / "out.o"
            try:
                with _Timeout(timeout, tmpd):
                    score.compile_single(src, flags, obj)
            except SystemExit as exc:
                res = dict(_worst(len(want), str(exc), fn), kind="fn")
            else:
                words = score.text_words(obj)
                functions = score.symbols(obj)
                res = dict(score_member(obj, fn, want, words, functions), kind="fn")
        res["flags"] = flags
        res["cached"] = False
        res["secs"] = time.time() - t0
        if key and not (res["err"] and "timeout" in res["err"]):
            _cache_put(key, res)
        return res
    except BaseException as exc:  # never raise out of the builder
        if isinstance(exc, KeyboardInterrupt):
            raise
        return dict(_worst(0, f"builder error: {type(exc).__name__}: {exc}", fn), kind="fn",
                    cached=False, secs=time.time() - t0)


# --- groups ---------------------------------------------------------------------------------------

def _load_group(group_dir, overrides):
    group_dir = Path(group_dir)
    spec = json.loads((group_dir / "group.json").read_text())
    ov = dict(overrides or {})
    files = {}
    for name in spec["files"]:
        files[name] = (group_dir / name).read_text()
    for name, text in (ov.pop("files", None) or {}).items():
        files[name] = text
        if name not in spec["files"]:
            spec["files"] = list(spec["files"]) + [name]
    for k, v in ov.items():
        spec[k] = v
    return spec, files


def compile_group(group_dir, overrides=None, timeout=DEFAULT_TIMEOUT, use_cache=True):
    """Build an IPA group like `score.py group` and score every member and context entry."""
    t0 = time.time()
    try:
        spec, files = _load_group(group_dir, overrides)
        extra = extra_targets(spec.get("targets"))
        targets = dict(get_targets())
        targets.update(extra)
        members = list(spec["members"])
        context = list(spec.get("context", []))
        want_lens = {n: len(targets[n]) for n in members + context if n in targets}
        key = None
        if use_cache:
            key = _key("group", {k: v for k, v in spec.items() if k not in ("generated",)},
                       files, sorted((n, hashlib.sha1(struct.pack(f">{len(w)}I", *w)).hexdigest())
                                     for n, w in targets.items() if n in want_lens))
            hit = _cache_get(key)
            if hit is not None:
                hit["cached"] = True
                hit["secs"] = time.time() - t0
                return hit
        if not ido_available():
            return _group_err(spec, targets, "IDO missing: run tools/cloud/setup.sh", t0)
        with tempfile.TemporaryDirectory(prefix="amg-") as tmp:
            tmpd = Path(tmp)
            (tmpd / "group.json").write_text(json.dumps(spec))
            for n, text in files.items():
                (tmpd / n).write_text(text)
            obj = tmpd / "out.o"
            try:
                with _Timeout(timeout):
                    score.compile_group(tmpd, obj)
            except SystemExit as exc:
                return _group_err(spec, targets, str(exc), t0, key)
            words = score.text_words(obj)
            functions = score.symbols(obj)
            order = sorted(functions.items(), key=lambda x: x[1])
            emitted = {n: ((order[i + 1][1] if i + 1 < len(order) else len(words) * 4) - o) // 4
                       for i, (n, o) in enumerate(order)}
            res = {"kind": "group", "err": None, "emitted": emitted, "members": {}, "context": {}}
            for names, bucket in ((members, "members"), (context, "context")):
                for n in names:
                    want = targets.get(n)
                    if want is None:
                        res[bucket][n] = _worst(0, f"no target for {n}", n)
                    else:
                        res[bucket][n] = score_member(obj, n, want, words, functions)
        res["matched"] = bool(members) and all(res["members"][n]["matched"] for n in members)
        res["matched_allow_unverified"] = bool(members) and all(
            res["members"][n]["matched_allow_unverified"] for n in members)
        res["cached"] = False
        res["secs"] = time.time() - t0
        if key:
            _cache_put(key, res)
        return res
    except BaseException as exc:
        if isinstance(exc, KeyboardInterrupt):
            raise
        return {"kind": "group", "err": f"builder error: {type(exc).__name__}: {exc}", "members": {},
                "context": {}, "emitted": {}, "matched": False, "cached": False,
                "secs": time.time() - t0}


def _group_err(spec, targets, err, t0, key=None):
    res = {"kind": "group", "err": err, "emitted": {}, "matched": False,
           "matched_allow_unverified": False, "cached": False, "secs": time.time() - t0,
           "members": {}, "context": {}}
    for names, bucket in ((spec["members"], "members"), (spec.get("context", []), "context")):
        for n in names:
            res[bucket][n] = _worst(len(targets.get(n, [])), err, n)
    if key and "timeout" not in err:
        _cache_put(key, res)
    return res


# --- parallel API -------------------------------------------------------------------------------------

def run_job(job):
    """job: {"kind": "fn", "src": text, "fn": name, "flags": str?} or
    {"kind": "group", "dir": path, "overrides": {...}?}; plus optional timeout/use_cache."""
    common = {"timeout": job.get("timeout", DEFAULT_TIMEOUT), "use_cache": job.get("use_cache", True)}
    if job["kind"] == "fn":
        extra = None
        if job.get("targets"):
            extra = extra_targets(job["targets"])
        return compile_fn(job["src"], job["fn"], job.get("flags"), extra=extra, **common)
    return compile_group(job["dir"], job.get("overrides"), **common)


def _init_worker():
    get_targets()
    get_addresses()


def score_many(jobs, jobs_n=None, strip_words=False):
    """Score many jobs in parallel; results in job order. jobs_n=1 runs in-process.
    Identical jobs in one batch are compiled once."""
    jobs = list(jobs)
    jobs_n = jobs_n or os.cpu_count() or 1
    uniq, index = [], []
    seen = {}
    for j in jobs:
        k = json.dumps(j, sort_keys=True, default=str)
        if k not in seen:
            seen[k] = len(uniq)
            uniq.append(j)
        index.append(seen[k])
    if jobs_n <= 1 or len(uniq) <= 1:
        results = [run_job(j) for j in uniq]
    else:
        with ProcessPoolExecutor(max_workers=jobs_n, initializer=_init_worker) as ex:
            results = list(ex.map(run_job, uniq, chunksize=1))
    out = [results[i] for i in index]
    if strip_words:
        out = [_strip(r) for r in out]
    return out


def _strip(r):
    r = dict(r)
    r.pop("words", None)
    for b in ("members", "context"):
        if b in r:
            r[b] = {n: {k: v for k, v in d.items() if k != "words"} for n, d in r[b].items()}
    return r


# --- CLI --------------------------------------------------------------------------------------------------

KEYS = ("strict_diff", "size", "target_size", "extra", "aligned_exact", "aligned_opcode",
        "aligned_opcode_reg", "matched")


def _line(d):
    if d.get("err"):
        return f"ERROR: {d['err'].splitlines()[0] if d['err'] else ''}"
    s = " ".join(f"{k}={d[k]}" for k in KEYS)
    if d["unresolved"]:
        s += f" unresolved={d['unresolved']}"
    if d["unverified"]:
        s += f" unverified={len(d['unverified'])}"
    if d["errors"]:
        s += f" errors={d['errors']}"
    return s


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="mode", required=True)
    f = sub.add_parser("fn")
    f.add_argument("file")
    f.add_argument("func")
    f.add_argument("--flags", default=None)
    f.add_argument("--hunks", action="store_true", help="print aligned diff hunks")
    g = sub.add_parser("group")
    g.add_argument("dir")
    for c in (f, g):
        c.add_argument("--json", action="store_true")
        c.add_argument("--no-cache", action="store_true")
        c.add_argument("--timeout", type=float, default=DEFAULT_TIMEOUT)
        c.add_argument("--words", action="store_true", help="include words in --json output")
    a = ap.parse_args(argv)
    if a.mode == "fn":
        text = Path(a.file).read_text()
        flags = a.flags or parse_flags_comment(text) or DEFAULT_FLAGS
        r = compile_fn(text, a.func, flags, timeout=a.timeout, use_cache=not a.no_cache)
        if a.json:
            print(json.dumps(r if a.words else _strip(r)))
        else:
            print(f"{a.func}: {_line(r)}  [{r['secs']:.2f}s{' cached' if r['cached'] else ''}]")
            if a.hunks and not r.get("err"):
                want = get_targets()[a.func]
                for h in aligned.diff_hunks(want, r["words"]):
                    t, c, kind, w, gw = h
                    print(f"  {kind:3s} want[{t}]={'-' if w is None else f'{w:08x}'} "
                          f"got[{c}]={'-' if gw is None else f'{gw:08x}'}")
    else:
        r = compile_group(a.dir, timeout=a.timeout, use_cache=not a.no_cache)
        if a.json:
            print(json.dumps(r if a.words else _strip(r)))
        else:
            if r["err"]:
                print(f"ERROR: {r['err']}")
            for bucket in ("members", "context"):
                for n, d in r[bucket].items():
                    print(f"{'member' if bucket == 'members' else 'context'} {n}: {_line(d)}")
            print(f"group matched={r['matched']} [{r['secs']:.2f}s{' cached' if r['cached'] else ''}]")
    return 0


if __name__ == "__main__":
    sys.exit(main())
