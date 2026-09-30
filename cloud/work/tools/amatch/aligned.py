#!/usr/bin/env python3
"""aligned.py: fast word-sequence scoring for MIPS word lists (pure functions, stdlib only).

    strict_diff(want, got)            positional difference count (score.py semantics)
    lcs_len(a, b)                     bit-parallel LCS length over hashable items
    lcs_pairs(a, b)                   LCS alignment [(i, j), ...] (bit-parallel + traceback)
    aligned_scores(want, got)         {aligned_exact, aligned_opcode, aligned_opcode_reg}
    diff_hunks(want, got, level)      [(target_idx, compiled_idx, kind, want, got), ...]
    score_against(name, got, ...)     the score dict defined in AUTOMATION_DESIGN.md

Keys: 'exact' compares whole words; 'opcode' compares instruction shape only (opcode, funct,
regimm/cop1 sub-opcode; registers and immediates ignored); 'opcode_reg' compares shape plus
all register fields (immediates, shift amounts, jump/branch targets masked).

CLI: aligned.py TARGET_NAME WORDS_FILE [--json]   (WORDS_FILE: hex words, whitespace separated)
"""
import argparse
import json
import sys
from pathlib import Path

try:  # Python 3.10+
    _popcount = int.bit_count
except AttributeError:  # pragma: no cover
    def _popcount(x):
        return bin(x).count("1")


# --- instruction keys --------------------------------------------------------

def shape(w):
    """Opcode shape: ignores registers and immediates."""
    op = w >> 26
    if op == 0:
        return (0, w & 0x3F)
    if op == 1:
        return (1, (w >> 16) & 0x1F)
    if op == 0x11:
        fmt = (w >> 21) & 0x1F
        return (op, fmt, (w & 0x3F) if fmt >= 16 else 0)
    if op == 0x10:
        fmt = (w >> 21) & 0x1F
        return (op, fmt, (w & 0x3F) if fmt >= 16 else 0)
    return (op,)


def shape_reg(w):
    """Opcode shape plus register fields; immediates, shamt, and targets masked."""
    op = w >> 26
    rs, rt, rd = (w >> 21) & 0x1F, (w >> 16) & 0x1F, (w >> 11) & 0x1F
    if op == 0:
        return (0, w & 0x3F, rs, rt, rd)
    if op == 1:
        return (1, rt, rs)
    if op in (2, 3):
        return (op,)
    if op == 0x11 or op == 0x10:
        fmt = rs
        if fmt == 8:  # bc1 / bc0: offset masked
            return (op, fmt, rt & 3)
        return (op, w & 0x03FFFFFF)
    return (op, rs, rt)


def words_key(words, level):
    if level == "exact":
        return list(words)
    if level == "opcode":
        return [shape(w) for w in words]
    if level == "opcode_reg":
        return [shape_reg(w) for w in words]
    raise ValueError(level)


# --- strict ---------------------------------------------------------------------

def strict_diff(want, got, masks=None):
    """Positional differences; missing compiled words count as differing.
    masks: {index: mask} applied to both sides at that index (relocation bits)."""
    masks = masks or {}
    bad = 0
    for i, w in enumerate(want):
        if i >= len(got):
            bad += 1
            continue
        m = masks.get(i, 0xFFFFFFFF)
        if (got[i] & m) != (w & m):
            bad += 1
    return bad


# --- bit-parallel LCS (Crochemore / Hyyro) -----------------------------------------

def _match_masks(a):
    masks = {}
    for i, x in enumerate(a):
        masks[x] = masks.get(x, 0) | (1 << i)
    return masks


def lcs_len(a, b):
    n = len(a)
    if n == 0 or not b:
        return 0
    masks = _match_masks(a)
    full = (1 << n) - 1
    v = full
    for x in b:
        m = masks.get(x)
        if m is None:
            continue
        u = v & m
        v = ((v + u) | (v - u)) & full
    return n - _popcount(v)


def lcs_pairs(a, b):
    """One LCS alignment as increasing (i, j) index pairs with a[i] == b[j]."""
    n, m = len(a), len(b)
    if n == 0 or m == 0:
        return []
    masks = _match_masks(a)
    full = (1 << n) - 1
    rows = [full]
    v = full
    for x in b:
        mk = masks.get(x)
        if mk is not None:
            u = v & mk
            v = ((v + u) | (v - u)) & full
        rows.append(v)

    def L(i, j):  # LCS(a[:i], b[:j])
        if i == 0 or j == 0:
            return 0
        return i - _popcount(rows[j] & ((1 << i) - 1))

    pairs = []
    i, j = n, m
    cur = L(i, j)
    while i > 0 and j > 0 and cur > 0:
        if a[i - 1] == b[j - 1] and L(i - 1, j - 1) == cur - 1:
            pairs.append((i - 1, j - 1))
            i -= 1
            j -= 1
            cur -= 1
        elif L(i - 1, j) == cur:
            i -= 1
        else:
            j -= 1
    pairs.reverse()
    return pairs


def _masked(words, masks):
    if not masks:
        return list(words)
    return [w & masks.get(i, 0xFFFFFFFF) for i, w in enumerate(words)]


def aligned_scores(want, got, masks=None):
    """Aligned LCS counts. masks ({compiled index: mask}) blank relocation fields on both sides
    at the same index (a positional approximation; exact for equal-length code)."""
    g = _masked(got, masks)
    w = _masked(want, masks)
    return {
        "aligned_exact": lcs_len(w, g),
        "aligned_opcode": lcs_len(words_key(want, "opcode"), words_key(got, "opcode")),
        "aligned_opcode_reg": lcs_len(words_key(w, "opcode_reg"), words_key(g, "opcode_reg")),
    }


# --- hunks ----------------------------------------------------------------------------

def diff_hunks(want, got, level="exact", masks=None):
    """Instruction-aligned diff. Returns [(target_idx, compiled_idx, kind, want, got)]:
    kind 'del' (target word absent from compiled; compiled_idx is the insertion point),
    'ins' (extra compiled word; target_idx is the insertion point), 'sub' (paired replace).
    want/got are the raw words (None for the missing side). Runs of del+ins between two
    matched anchors are paired position by position as 'sub'."""
    w = _masked(want, masks)
    g = _masked(got, masks)
    kw, kg = words_key(w, level), words_key(g, level)
    pairs = lcs_pairs(kw, kg) + [(len(want), len(got))]
    hunks = []
    pi = pj = 0
    for (i, j) in pairs:
        dels = list(range(pi, i))
        inss = list(range(pj, j))
        for k in range(max(len(dels), len(inss))):
            ti = dels[k] if k < len(dels) else None
            cj = inss[k] if k < len(inss) else None
            if ti is not None and cj is not None:
                hunks.append((ti, cj, "sub", want[ti], got[cj]))
            elif ti is not None:
                hunks.append((ti, (inss[-1] + 1 if inss else pj), "del", want[ti], None))
            else:
                hunks.append(((dels[-1] + 1 if dels else pi), cj, "ins", None, got[cj]))
        pi, pj = i + 1, j + 1
    return hunks


def group_hunks(hunks):
    """Group consecutive hunks into runs: [[hunk, ...], ...] (adjacent target or compiled idx)."""
    out, cur = [], []
    for h in hunks:
        if cur and not (h[0] is not None and h[0] - cur[-1][0] <= 1
                        or h[1] is not None and h[1] - cur[-1][1] <= 1):
            out.append(cur)
            cur = []
        cur.append(h)
    if cur:
        out.append(cur)
    return out


# --- score dict ---------------------------------------------------------------------------

def score_dict(want, got, masks=None, extra=0, unresolved=(), unverified=(), errors=(),
               size=None):
    """The shared score dict. `got` is the compiled slice after relocation resolution
    (length = size, the function's emitted word count, possibly longer than `want`)."""
    sd = strict_diff(want, got, masks)
    d = {
        "strict_diff": sd,
        "size": len(got) if size is None else size,
        "target_size": len(want),
        "target_words": len(want),
        "extra": extra,
    }
    d.update(aligned_scores(want, got, masks))
    d["unresolved"] = list(unresolved)
    d["unverified"] = list(unverified)
    d["errors"] = list(errors)
    d["matched"] = (sd == 0 and extra == 0 and not d["unresolved"]
                    and not d["errors"] and not d["unverified"])
    return d


def score_against(name, got, masks=None, targets=None, **kw):
    """Score compiled words `got` against target `name`. `targets` defaults to
    tools/cloud/score.py targets() (import deferred so this module stays pure)."""
    if targets is None:
        root = Path(__file__).resolve().parents[4]
        sys.path.insert(0, str(root / "tools" / "cloud"))
        import score
        targets = score.targets()
    if name not in targets:
        raise KeyError(f"no target {name}")
    return score_dict(targets[name], got, masks, **kw)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("name")
    ap.add_argument("words_file")
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()
    got = [int(x, 16) for x in Path(a.words_file).read_text().split()]
    d = score_against(a.name, got)
    if a.json:
        print(json.dumps(d))
    else:
        print(" ".join(f"{k}={v}" for k, v in d.items()))
    return 0


if __name__ == "__main__":
    sys.exit(main())
