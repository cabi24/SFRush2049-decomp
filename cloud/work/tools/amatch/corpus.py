#!/usr/bin/env python3
"""corpus.py: regression corpus of (start_source, goal_source) pairs mined from git history.

For every function matched so far (cloud/matches/*.c and the claimed members of
cloud/work/ipa-groups/*/group.json) the corpus records the goal (the final matching
source) and the start (the earliest committed version of that function that compiles and
is not already the answer), the start's strict/aligned score, and a heuristic label for
the kind of edit that turned start into goal.  It is the regression set for the
mutation search: a search that cannot reproduce these fixes from the start state is
missing a mutation.

    corpus.py list [--class C] [--json]
    corpus.py show FN                      metadata plus a unified diff start -> goal
    corpus.py stats [--write]              edit-class histogram (--write refreshes STATS.md)
    corpus.py materialize FN start|goal --out FILE    full compilable translation unit
    corpus.py bench --search-cmd CMD --budget N --jobs J [--class C] [--limit K]
    corpus.py mine                         (re)build corpus_data/ from git history (needs IDO)

Storage (corpus_data/): index.json, header.h (the shared m2c context header), and per
function FN.start.c / FN.goal.c.  Translation units are stored *packed*: any run of 8+
lines equal to header.h is replaced by one marker line `/*@@HDR lo hi@@*/` (header.h
lines lo..hi-1), so the files hold only what is specific to the function.  `unpack`
restores the exact original; `materialize` does that.  Group members are stored as the
function body only (kind "group", not compilable alone, skipped by bench).

bench contract: the search command is run as
    CMD FILE FN --flags "FLAGS" --budget N --json
and must print (last JSON line of stdout)
    {"matched": bool, "evals": int, "seconds": float, "best_score": {...}}
Stdlib only.
"""
import argparse
import concurrent.futures as cf
import difflib
import json
import os
import re
import shlex
import statistics
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
DATA = HERE / "corpus_data"
INDEX = DATA / "index.json"
HEADER = DATA / "header.h"
SCORE_PY = ROOT / "tools" / "cloud" / "score.py"
NEAR_PY = ROOT / "cloud" / "work" / "bigfish" / "near.py"
DEFAULT_FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"
DEFAULT_SEARCH = "python3 cloud/work/tools/amatch/search.py"
HDR_RE = re.compile(r"^/\*@@HDR (\d+) (\d+)@@\*/$")
HDR_MIN = 8
HEADER_SRC = "cloud/matches/func_800A4770.c"  # first 3696 lines = canonical m2c context
HEADER_LINES = 3696

CLASSES = ["loop-form", "type-change", "local-dropped", "local-added", "decl-order",
           "extern-to-defined", "operand-flip", "stmt-order", "literal-type",
           "case-order", "other"]
EXTRA_TAGS = ["rewrite"]  # size tag, not an edit class


# ---------------------------------------------------------------- packing

def pack(text, header_lines):
    """Replace runs of >= HDR_MIN lines equal to header_lines by marker lines."""
    lines = text.split("\n")
    sm = difflib.SequenceMatcher(None, header_lines, lines, autojunk=False)
    out, prev = [], 0
    for a, b, n in sm.get_matching_blocks():
        out.extend(lines[prev:b])
        if n >= HDR_MIN:
            out.append("/*@@HDR %d %d@@*/" % (a, a + n))
        else:
            out.extend(lines[b:b + n])
        prev = b + n
    return "\n".join(out)


def unpack(packed, header_lines):
    out = []
    for ln in packed.split("\n"):
        m = HDR_RE.match(ln)
        if m:
            out.extend(header_lines[int(m.group(1)):int(m.group(2))])
        else:
            out.append(ln)
    return "\n".join(out)


def literal_part(packed):
    return "\n".join(ln for ln in packed.split("\n") if not HDR_RE.match(ln))


def header_lines():
    return HEADER.read_text().split("\n")


# ---------------------------------------------------------------- C scanning

_MASK_RE = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\\n])*\"|'(?:\\.|[^'\\\n])*'", re.S)


def mask(text):
    """Blank comments/strings (same length, newlines kept) so brace scans are safe."""
    return _MASK_RE.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), text)


_DEF_RE = re.compile(r"(?m)^[A-Za-z_][^\n;{}()=]*?\b(\w+)\s*\(")


def _match(m, i, open_c, close_c):
    depth = 0
    for j in range(i, len(m)):
        c = m[j]
        if c == open_c:
            depth += 1
        elif c == close_c:
            depth -= 1
            if depth == 0:
                return j
    return -1


def find_defs(text, names=None):
    """{name: (start, end)} of function definitions (first occurrence of each)."""
    if names is not None and not any(n in text for n in names):
        return {}
    m = mask(text)
    res = {}
    for mo in _DEF_RE.finditer(m):
        name = mo.group(1)
        if names is not None and name not in names:
            continue
        if name in res:
            continue
        p = _match(m, mo.end() - 1, "(", ")")
        if p < 0:
            continue
        k = p + 1
        while k < len(m) and m[k] in " \t\r\n":
            k += 1
        if k < len(m) and m[k] == "{":
            e = _match(m, k, "{", "}")
            if e > 0:
                res[name] = (mo.start(), e + 1)
    return res


def extract_fn(text, fn):
    d = find_defs(text, {fn})
    if fn not in d:
        return None
    a, b = d[fn]
    return text[a:b]


def norm_ws(s):
    return " ".join(mask(s).split())


_TOK_RE = re.compile(r"""0[xX][0-9a-fA-F]+[uUlL]*|\d+\.\d*(?:[eE][-+]?\d+)?[fFlL]?|\.\d+[fFlL]?|\d+[uUlL]*[fF]?|
                         [A-Za-z_]\w*|<<=|>>=|<<|>>|<=|>=|==|!=|&&|\|\||\+\+|--|->|[-+*/%&|^]=|\S""", re.X)


def tokens(s):
    return _TOK_RE.findall(mask(s))


TYPE_WORDS = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "void", "char",
              "short", "int", "long", "float", "double", "unsigned", "signed", "volatile", "const",
              "struct", "union", "vs8", "vu8", "vs16", "vu16", "vs32", "vu32", "vs64", "vu64",
              "F32", "S32", "S16", "S08", "U32", "U16", "U08", "BOOL", "M2C_UNK", "register", "static"}


def is_num(t):
    return bool(re.match(r"^(0[xX][0-9a-fA-F]+|\d|\.\d)", t))


def num_value(t):
    t2 = re.sub(r"[uUlLfF]+$", "", t) if not t.lower().startswith("0x") else re.sub(r"[uUlL]+$", "", t)
    try:
        return float(int(t2, 16)) if t2.lower().startswith("0x") else float(t2)
    except ValueError:
        return None


_LOCAL_RE = re.compile(
    r"^\s+(?:(?:volatile|register|const|static)\s+)*((?:struct\s+)?[A-Za-z_]\w*)\s*((?:\*\s*)*)"
    r"([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*(?:=[^;]*)?;\s*$")


def fn_parts(body):
    """(signature_text, [(type, name)] top-level local declarations in order)."""
    i = body.find("{")
    sig = body[:i]
    locs = []
    depth = 0
    for ln in body[i + 1:].split("\n"):
        if depth == 0:
            m = _LOCAL_RE.match(mask(ln))
            if m and (m.group(1) in TYPE_WORDS or m.group(1).startswith("struct")
                      or m.group(1)[0].isupper() or m.group(1).endswith("_t")):
                locs.append((m.group(1) + m.group(2).replace(" ", ""), m.group(3)))
                continue
            if ln.strip() and not ln.strip().startswith("/*"):
                # first non-declaration statement ends the declaration block
                if not m:
                    break
        depth += ln.count("{") - ln.count("}")
    return sig, locs


def file_decls(text, used):
    """{ident: (is_extern, normalised-decl-minus-extern)} for file-scope decls naming `used`."""
    m = mask(text)
    out = {}
    depth = 0
    start = 0
    n = len(m)
    i = 0
    while i < n:
        c = m[i]
        if c == "#" and (i == 0 or m[i - 1] == "\n"):
            j = m.find("\n", i)
            i = n if j < 0 else j
            start = i
            continue
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                start = i + 1
        elif c == ";" and depth == 0:
            stmt = text[start:i]
            start = i + 1
            toks = tokens(stmt)
            if toks and toks[0] != "typedef":
                k = len(toks)
                for z, t in enumerate(toks):
                    if t in "[=(":
                        k = z
                        break
                ids = [t for t in toks[:k] if re.match(r"[A-Za-z_]\w*$", t)]
                if ids and ids[-1] in used and "{" not in stmt:
                    ext = "extern" in toks
                    key = " ".join(t for t in toks if t != "extern")
                    out.setdefault(ids[-1], (ext, key))
        i += 1
    return out


# ---------------------------------------------------------------- classification

def classify(start_tu, goal_tu, fn, start_body=None, goal_body=None):
    """Heuristic edit-class labels for start -> goal.  Inputs are (unpacked-or-packed) TU text
    or, with *_body given, function text only.  Returns dict(classes, primary, ratio, detail)."""
    sb = start_body if start_body is not None else extract_fn(start_tu, fn) or ""
    gb = goal_body if goal_body is not None else extract_fn(goal_tu, fn) or ""
    ta, tb = tokens(sb), tokens(gb)
    sm = difflib.SequenceMatcher(None, ta, tb, autojunk=False)
    ops = [o for o in sm.get_opcodes() if o[0] != "equal"]
    changed = sum(max(i2 - i1, j2 - j1) for _, i1, i2, j1, j2 in ops)
    ratio = changed / max(len(ta), len(tb), 1)
    cls, detail = set(), {}

    # loops
    def loopcount(t):
        return tuple(t.count(k) for k in ("for", "while", "do", "goto"))
    if loopcount(ta) != loopcount(tb):
        cls.add("loop-form")
        detail["loops"] = [loopcount(ta), loopcount(tb)]

    # switch case order
    ca = [ta[i + 1] for i, t in enumerate(ta[:-1]) if t == "case"]
    cb = [tb[i + 1] for i, t in enumerate(tb[:-1]) if t == "case"]
    if ca != cb and sorted(ca) == sorted(cb):
        cls.add("case-order")

    # locals and signature
    sig_a, loc_a = fn_parts(sb)
    sig_b, loc_b = fn_parts(gb)
    na = [n for _, n in loc_a]
    nb = [n for _, n in loc_b]
    if len(loc_b) < len(loc_a) or [n for n in na if n not in nb]:
        if [n for n in na if n not in nb]:
            cls.add("local-dropped")
    if [n for n in nb if n not in na] and len(loc_b) > len(loc_a):
        cls.add("local-added")
    common_a = [n for n in na if n in nb]
    common_b = [n for n in nb if n in na]
    if common_a != common_b:
        cls.add("decl-order")
    tymap = dict((n, t) for t, n in loc_a)
    if any(tymap.get(n) not in (None, t) for t, n in loc_b):
        cls.add("type-change")
    if tokens(sig_a)[:] != tokens(sig_b)[:]:
        pa = [t for t in tokens(sig_a) if t in TYPE_WORDS or t == "*"]
        pb = [t for t in tokens(sig_b) if t in TYPE_WORDS or t == "*"]
        if pa != pb:
            cls.add("type-change")

    # token replace blocks
    for tag, i1, i2, j1, j2 in ops:
        a, b = ta[i1:i2], tb[j1:j2]
        if a and b and all(t in TYPE_WORDS or t == "*" for t in a + b):
            cls.add("type-change")
        elif not a and len(b) >= 3 and b[0] == "(" and b[-1] == ")" and all(t in TYPE_WORDS or t == "*" for t in b[1:-1]):
            cls.add("type-change")  # cast added
        elif not b and len(a) >= 3 and a[0] == "(" and a[-1] == ")" and all(t in TYPE_WORDS or t == "*" for t in a[1:-1]):
            cls.add("type-change")  # cast dropped
        if len(a) == 1 and len(b) == 1 and is_num(a[0]) and is_num(b[0]):
            va, vb = num_value(a[0]), num_value(b[0])
            if va is not None and va == vb and a[0] != b[0]:
                cls.add("literal-type")
        if a and b and sorted(a) == sorted(b) and a != b and len(a) >= 2:
            cls.add("operand-flip")
    flipmap = {">": "<", ">=": "<=", "<": ">", "<=": ">="}
    wa = sorted(flipmap.get(t, t) for t in ta)
    wb = sorted(flipmap.get(t, t) for t in tb)
    if ta != tb and wa == wb:
        if not cls & {"loop-form", "case-order", "decl-order", "operand-flip"}:
            cls.add("operand-flip" if changed <= 12 else "stmt-order")

    # file-scope declarations
    if start_tu is not None and goal_tu is not None and start_body is None:
        used = set(t for t in set(ta) | set(tb) if re.match(r"[A-Za-z_]\w*$", t)) - {fn}
        da = file_decls(literal_part(start_tu).replace(sb, ""), used)
        db = file_decls(literal_part(goal_tu).replace(gb, ""), used)
        for k in set(da) & set(db):
            ea, ka = da[k]
            eb, kb = db[k]
            if ea and not eb:
                cls.add("extern-to-defined")
            elif eb and not ea:
                cls.add("extern-to-defined")
            elif ka != kb:
                cls.add("type-change")
        detail["decls_changed"] = sorted(k for k in set(da) & set(db) if da[k] != db[k])[:8]

    if ratio >= 0.5:
        detail["rewrite"] = True
    if not cls:
        cls.add("other")
    primary = next(c for c in CLASSES if c in cls)
    detail["ops"] = len(ops)
    detail["tokens_start"] = len(ta)
    detail["tokens_goal"] = len(tb)
    return {"classes": [c for c in CLASSES if c in cls], "primary": primary,
            "ratio": round(ratio, 3), "rewrite": ratio >= 0.5, "detail": detail}


# ---------------------------------------------------------------- scoring (IDO)

def ido_available():
    return (ROOT / "tools" / "cloud" / "ido" / "cc").exists() and SCORE_PY.exists()


def score_fn(path, fn, flags, timeout=120):
    """Strict + aligned score of FN in FILE.  {compiles, matched, strict_diff, target_words, aligned_exact}."""
    r = {"compiles": False, "matched": False, "strict_diff": None, "target_words": None,
         "aligned_exact": None, "aligned_pct": None}
    try:
        p = subprocess.run([sys.executable, str(SCORE_PY), "fn", str(path), fn, "--flags", flags],
                           cwd=ROOT, capture_output=True, text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return r
    out = p.stdout + p.stderr
    m = re.search(r"(\d+)/(\d+) words differ", out)
    if m:
        r.update(compiles=True, strict_diff=int(m.group(1)), target_words=int(m.group(2)))
    elif re.search(r"^\s*MATCH\s*$", out, re.M) and p.returncode == 0:
        r.update(compiles=True, matched=True, strict_diff=0)
    elif "no target section" in out or "error" in out.lower() or "unresolved" in out.lower():
        return r
    else:
        return r
    try:
        q = subprocess.run([sys.executable, str(NEAR_PY), str(path), fn, "--flags", flags],
                           cwd=ROOT, capture_output=True, text=True, timeout=timeout)
        m = re.search(r"want (\d+) words, got (\d+).*aligned exact (\d+) \((\d+)%\)", q.stdout)
        if m:
            r["target_words"] = int(m.group(1))
            r["aligned_exact"] = int(m.group(3))
            r["aligned_pct"] = int(m.group(4))
            r["size"] = int(m.group(2))
    except subprocess.TimeoutExpired:
        pass
    return r


# ---------------------------------------------------------------- git mining

def git(*args, text=True):
    p = subprocess.run(["git", *args], cwd=ROOT, capture_output=True, check=True)
    return p.stdout.decode("utf-8", "replace") if text else p.stdout


def list_blobs(pathspecs=("cloud", "tools/cloud")):
    """[(commit, path, blob)] for every .c added/modified in history, oldest first."""
    out = git("log", "--reverse", "--no-renames", "--raw", "--no-abbrev", "--diff-filter=AM",
              "--format=C %H", "--", *pathspecs)
    res, cur = [], None
    for ln in out.split("\n"):
        if ln.startswith("C "):
            cur = ln[2:]
        elif ln.startswith(":"):
            meta, path = ln.split("\t", 1)
            if path.endswith(".c"):
                res.append((cur, path, meta.split()[3]))
    return res


def read_blobs(shas):
    p = subprocess.Popen(["git", "cat-file", "--batch"], cwd=ROOT, stdin=subprocess.PIPE,
                         stdout=subprocess.PIPE)
    data = {}
    for s in shas:
        p.stdin.write((s + "\n").encode())
        p.stdin.flush()
        hdr = p.stdout.readline().split()
        size = int(hdr[2])
        data[s] = p.stdout.read(size).decode("utf-8", "replace")
        p.stdout.read(1)
    p.stdin.close()
    p.wait()
    return data


def parse_flags(text, default=DEFAULT_FLAGS):
    m = re.match(r"\s*/\*\s*flags:\s*(.*?)\s*\*/", text)
    return m.group(1) if m else default


def goal_targets():
    """[dict(fn, kind, group, flags, path, text)] from HEAD."""
    res = []
    for p in sorted((ROOT / "cloud" / "matches").glob("*.c")):
        t = git("show", "HEAD:cloud/matches/" + p.name)
        res.append(dict(fn=p.stem, kind="single", group=None, flags=parse_flags(t),
                        path="cloud/matches/" + p.name, text=t))
    for g in sorted((ROOT / "cloud" / "work" / "ipa-groups").glob("*/group.json")):
        j = json.loads(g.read_text())
        rel = g.parent.relative_to(ROOT).as_posix()
        for fn in j.get("claims", []):
            for f in j.get("files", []):
                try:
                    t = git("show", "HEAD:%s/%s" % (rel, f))
                except subprocess.CalledProcessError:
                    continue
                if extract_fn(t, fn):
                    res.append(dict(fn=fn, kind="group", group=g.parent.name, flags=j["flags"],
                                    path="%s/%s" % (rel, f), text=t))
                    break
    return res


def mine(jobs=6, verbose=True):
    DATA.mkdir(exist_ok=True)
    if not HEADER.exists():
        lines = git("show", "HEAD:" + HEADER_SRC).split("\n")[:HEADER_LINES]
        HEADER.write_text("\n".join(lines) + "\n")
    hl = header_lines()
    targets = goal_targets()
    # one entry per function: a matches/ single wins over a group duplicate
    seen = {}
    for t in targets:
        if t["fn"] not in seen or (seen[t["fn"]]["kind"] == "group" and t["kind"] == "single"):
            seen[t["fn"]] = t
    targets = sorted(seen.values(), key=lambda t: t["fn"])
    names = {t["fn"] for t in targets}
    blobs = list_blobs()
    uniq = list(dict.fromkeys(b for _, _, b in blobs))
    if verbose:
        print("scanning %d blobs for %d functions" % (len(uniq), len(names)), file=sys.stderr)
    texts = read_blobs(uniq)
    versions = {n: [] for n in names}   # chronological, distinct bodies
    seen_norm = {n: set() for n in names}
    for commit, path, blob in blobs:
        t = texts[blob]
        for fn, (a, b) in find_defs(t, names).items():
            nb = norm_ws(t[a:b])
            if nb in seen_norm[fn]:
                continue
            seen_norm[fn].add(nb)
            versions[fn].append(dict(commit=commit, path=path, blob=blob, body=t[a:b]))
    del texts

    entries, dropped = [], []

    def work(t):
        fn = t["fn"]
        goal_body = extract_fn(t["text"], fn)
        rec = dict(fn=fn, kind=t["kind"], group=t["group"], flags=t["flags"],
                   goal=dict(path=t["path"]), versions_seen=len(versions[fn]))
        vs = versions[fn]
        if not vs:
            return None, (fn, "no version found in history")
        gnorm = norm_ws(goal_body)
        first = vs[0]
        rec["first_seen"] = dict(commit=first["commit"][:10], path=first["path"])
        chosen, skipped, tried = None, 0, []
        if norm_ws(first["body"]) == gnorm:
            rec.update(status="no_start", no_start_reason="first appearance already the matching source")
        else:
            for v in vs[:8]:
                if norm_ws(v["body"]) == gnorm:
                    break  # later versions are the goal itself
                if t["kind"] == "group":
                    chosen = v
                    break
                tu = git("show", "%s:%s" % (v["commit"], v["path"]))
                with tempfile.TemporaryDirectory() as d:
                    f = Path(d) / (fn + ".c")
                    f.write_text(tu)
                    sc = score_fn(f, fn, t["flags"])
                tried.append(sc)
                if sc["matched"]:
                    rec.update(status="no_start", no_start_reason="first compiling version already matches")
                    break
                if sc["compiles"]:
                    chosen = v
                    chosen["score"] = sc
                    chosen["tu"] = tu
                    break
                skipped += 1
            if chosen is None and "status" not in rec:
                rec.update(status="no_start", no_start_reason="no compiling non-matching start version")
        if chosen is not None:
            rec["status"] = "ok"
            rec["start"] = dict(commit=chosen["commit"][:10], path=chosen["path"],
                                skipped_versions=skipped, score=chosen.get("score"))
        if t["kind"] == "single":
            rec["_goal_src"] = t["text"]
            rec["_start_src"] = chosen["tu"] if chosen else None
        else:
            rec["_goal_src"] = goal_body
            rec["_start_src"] = chosen["body"] if chosen else None
        rec["_goal_body"] = goal_body
        rec["_start_body"] = extract_fn(chosen["tu"], fn) if chosen and t["kind"] == "single" else (
            chosen["body"] if chosen else None)
        return rec, None

    with cf.ThreadPoolExecutor(jobs) as ex:
        for rec, drop in ex.map(work, targets):
            if drop:
                dropped.append(drop)
            else:
                entries.append(rec)
    for old in DATA.glob("*.[sg]*.c"):
        old.unlink()
    for rec in entries:
        fn = rec["fn"]
        gs, ss = rec.pop("_goal_src"), rec.pop("_start_src")
        gb, sb = rec.pop("_goal_body"), rec.pop("_start_body")
        single = rec["kind"] == "single"
        (DATA / (fn + ".goal.c")).write_text(pack(gs, hl) if single else gs)
        rec["goal_file"] = fn + ".goal.c"
        rec["packed"] = single
        if ss is not None:
            (DATA / (fn + ".start.c")).write_text(pack(ss, hl) if single else ss)
            rec["start_file"] = fn + ".start.c"
            rec["edit"] = classify(ss if single else None, gs if single else None, fn,
                                   None if single else sb, None if single else gb)
        else:
            rec["start_file"] = None
    entries.sort(key=lambda r: r["fn"])
    INDEX.write_text(json.dumps(dict(version=1, header="header.h", header_src=HEADER_SRC,
                                     entries=entries, dropped=dropped), indent=1) + "\n")
    write_stats()
    return entries, dropped


# ---------------------------------------------------------------- access

def load_index():
    return json.loads(INDEX.read_text())


def entry(idx, fn):
    for e in idx["entries"]:
        if e["fn"] == fn:
            return e
    raise KeyError(fn)


def read_source(e, which):
    """Full text (TU for singles, function body for groups) of 'start' or 'goal'."""
    f = e[which + "_file"]
    if not f:
        return None
    t = (DATA / f).read_text()
    return unpack(t, header_lines()) if e.get("packed") else t


def with_start(idx):
    return [e for e in idx["entries"] if e.get("start_file")]


# ---------------------------------------------------------------- stats

def histogram(idx):
    allc = {c: 0 for c in CLASSES}
    prim = {c: 0 for c in CLASSES}
    for e in with_start(idx):
        for c in e["edit"]["classes"]:
            allc[c] += 1
        prim[e["edit"]["primary"]] += 1
    return allc, prim


def stats_text(idx):
    ents = idx["entries"]
    ws = with_start(idx)
    allc, prim = histogram(idx)
    n = len(ws)
    lines = ["# Regression corpus statistics", "",
             "Generated by `python3 cloud/work/tools/amatch/corpus.py stats --write` from git history.",
             "Labels are heuristic token-diff classes (see `classify` in corpus.py); an edit can carry",
             "several labels, so the `any label` column sums to more than the pair count.", "",
             "- entries: %d (%d single functions from `cloud/matches`, %d IPA group members)"
             % (len(ents), sum(e["kind"] == "single" for e in ents), sum(e["kind"] == "group" for e in ents)),
             "- with a start state (start -> goal pairs): %d" % n,
             "- `no_start` (first committed version already matching, or nothing compiling): %d"
             % sum(e.get("status") == "no_start" for e in ents),
             "- dropped (no version found in history): %d" % len(idx.get("dropped", [])),
             "- large rewrites (>= 50%% of tokens changed): %d" % sum(e["edit"]["rewrite"] for e in ws),
             "", "| class | any label | primary | share of pairs |", "|---|---:|---:|---:|"]
    for c in sorted(CLASSES, key=lambda c: (-allc[c], c)):
        lines.append("| %s | %d | %d | %s |" % (c, allc[c], prim[c], ("%.0f%%" % (100 * allc[c] / n)) if n else "-"))
    lines += ["", "Primary = first label in priority order: " + ", ".join(CLASSES) + ".", ""]
    sc = [e["start"]["score"] for e in ws if e["start"].get("score")]
    if sc:
        d = [s["strict_diff"] for s in sc if s.get("strict_diff") is not None]
        ap = [s["aligned_pct"] for s in sc if s.get("aligned_pct") is not None]
        lines += ["## Start-state scores (singles)", "",
                  "- strict differing words: median %s, min %s, max %s"
                  % (statistics.median(d), min(d), max(d)) if d else "",
                  "- aligned-exact percent: median %s" % statistics.median(ap) if ap else "", ""]
    lines += ["## Per entry", "", "| function | kind | flags | start strict diff | aligned % | labels |", "|---|---|---|---:|---:|---|"]
    for e in ents:
        s = (e.get("start") or {}).get("score") or {}
        lines.append("| %s | %s | `%s` | %s | %s | %s |" % (
            e["fn"], e["kind"], e["flags"].replace("-g0 ", "").replace(" -mips2 -G 0 -non_shared", ""),
            s.get("strict_diff", "-") if s else "-", s.get("aligned_pct", "-") if s else "-",
            ",".join(e["edit"]["classes"]) if e.get("edit") else e.get("no_start_reason", "-")))
    if idx.get("dropped"):
        lines += ["", "## Dropped", ""] + ["- %s: %s" % tuple(d) for d in idx["dropped"]]
    return "\n".join(l for l in lines) + "\n"


def write_stats():
    (DATA / "STATS.md").write_text(stats_text(load_index()))


# ---------------------------------------------------------------- bench

def run_search(search_cmd, path, fn, flags, budget, timeout):
    cmd = shlex.split(search_cmd) + [str(path), fn, "--flags", flags, "--budget", str(budget), "--json"]
    t0 = time.time()
    try:
        p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return dict(matched=False, evals=None, seconds=time.time() - t0, error="timeout")
    res = None
    for ln in reversed(p.stdout.strip().split("\n")):
        ln = ln.strip()
        if ln.startswith("{"):
            try:
                res = json.loads(ln)
                break
            except ValueError:
                pass
    if res is None:
        return dict(matched=False, evals=None, seconds=time.time() - t0,
                    error="no JSON (rc=%s) %s" % (p.returncode, (p.stderr or p.stdout)[-200:]))
    res.setdefault("seconds", time.time() - t0)
    return res


def bench(search_cmd, budget, jobs, cls=None, limit=None, timeout=3600, fns=None, out_json=None):
    idx = load_index()
    hl = header_lines()
    todo = [e for e in with_start(idx) if e["kind"] == "single" and (e.get("start") or {}).get("score", {}).get("compiles")]
    if cls:
        todo = [e for e in todo if cls in e["edit"]["classes"]]
    if fns:
        todo = [e for e in todo if e["fn"] in fns]
    if limit:
        todo = todo[:limit]
    tmp = tempfile.TemporaryDirectory()

    def one(e):
        f = Path(tmp.name) / (e["fn"] + ".c")
        f.write_text(unpack((DATA / e["start_file"]).read_text(), hl))
        return e, run_search(search_cmd, f, e["fn"], e["flags"], budget, timeout)

    results = []
    with cf.ThreadPoolExecutor(jobs) as ex:
        for e, r in ex.map(one, todo):
            results.append((e, r))
    tmp.cleanup()
    rows = {}

    def add(name, e, r):
        d = rows.setdefault(name, dict(n=0, hit=0, evals=[], secs=[], err=0))
        d["n"] += 1
        if r.get("error"):
            d["err"] += 1
        if r.get("matched"):
            d["hit"] += 1
            if r.get("evals") is not None:
                d["evals"].append(r["evals"])
            d["secs"].append(r.get("seconds") or 0)

    for e, r in results:
        add("ALL", e, r)
        for c in e["edit"]["classes"]:
            add(c, e, r)
    print("budget=%d evals per function, %d functions, search: %s" % (budget, len(results), search_cmd))
    print("%-18s %5s %7s %6s %10s %9s %4s" % ("edit class", "n", "matched", "rate", "med evals", "med sec", "err"))
    for name in ["ALL"] + [c for c in CLASSES if c in rows]:
        d = rows[name]
        print("%-18s %5d %7d %5.0f%% %10s %9s %4d" % (
            name, d["n"], d["hit"], 100 * d["hit"] / d["n"],
            "%d" % statistics.median(d["evals"]) if d["evals"] else "-",
            "%.1f" % statistics.median(d["secs"]) if d["secs"] else "-", d["err"]))
    missed = [e["fn"] for e, r in results if not r.get("matched")]
    if missed:
        print("not matched: " + " ".join(missed))
    if out_json:
        Path(out_json).write_text(json.dumps([dict(fn=e["fn"], classes=e["edit"]["classes"], **r)
                                              for e, r in results], indent=1))
    return rows, results


# ---------------------------------------------------------------- CLI

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("list")
    p.add_argument("--class", dest="cls")
    p.add_argument("--json", action="store_true")
    p = sub.add_parser("show")
    p.add_argument("fn")
    p = sub.add_parser("stats")
    p.add_argument("--write", action="store_true")
    p = sub.add_parser("materialize")
    p.add_argument("fn")
    p.add_argument("which", choices=["start", "goal"])
    p.add_argument("--out", required=True)
    p = sub.add_parser("bench")
    p.add_argument("--search-cmd", default=DEFAULT_SEARCH)
    p.add_argument("--budget", type=int, default=200)
    p.add_argument("--jobs", type=int, default=4)
    p.add_argument("--class", dest="cls")
    p.add_argument("--limit", type=int)
    p.add_argument("--fn", action="append")
    p.add_argument("--timeout", type=int, default=3600)
    p.add_argument("--json-out")
    p = sub.add_parser("mine")
    p.add_argument("--jobs", type=int, default=6)
    a = ap.parse_args(argv)

    if a.cmd == "mine":
        if not ido_available():
            sys.exit("mine needs IDO (tools/cloud/setup.sh)")
        ents, dropped = mine(a.jobs)
        print("%d entries, %d with start, %d dropped" % (len(ents), len(with_start(load_index())), len(dropped)))
        return
    idx = load_index()
    if a.cmd == "list":
        for e in idx["entries"]:
            if a.cls and a.cls not in (e.get("edit") or {}).get("classes", []):
                continue
            if a.json:
                print(json.dumps(e))
                continue
            s = (e.get("start") or {}).get("score") or {}
            print("%-28s %-6s %-9s diff=%-4s al%%=%-4s %s" % (
                e["fn"], e["kind"], e["status"], s.get("strict_diff", "-"), s.get("aligned_pct", "-"),
                ",".join(e["edit"]["classes"]) if e.get("edit") else e.get("no_start_reason", "")))
    elif a.cmd == "show":
        e = entry(idx, a.fn)
        print(json.dumps(e, indent=1))
        if e.get("start_file"):
            sb = extract_fn(read_source(e, "start"), a.fn) if e["kind"] == "single" else read_source(e, "start")
            gb = extract_fn(read_source(e, "goal"), a.fn) if e["kind"] == "single" else read_source(e, "goal")
            print("".join(difflib.unified_diff(sb.splitlines(True), gb.splitlines(True), "start", "goal")))
    elif a.cmd == "stats":
        txt = stats_text(idx)
        if a.write:
            (DATA / "STATS.md").write_text(txt)
        print("\n".join(txt.split("\n")[:24]))
    elif a.cmd == "materialize":
        e = entry(idx, a.fn)
        src = read_source(e, a.which)
        if src is None:
            sys.exit("%s has no %s" % (a.fn, a.which))
        Path(a.out).write_text(src)
    elif a.cmd == "bench":
        bench(a.search_cmd, a.budget, a.jobs, a.cls, a.limit, a.timeout, a.fn, a.json_out)


if __name__ == "__main__":
    main()
