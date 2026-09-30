#!/usr/bin/env python3
"""triage.py: rank every not-yet-spliced game function by expected payoff / cost (stdlib only).

    triage.py [--top N] [--class abi|ipa|all] [--json] [--min-words A] [--max-words B] [--no-closure]

Candidates: tools/cloud/score.py targets() minus
  * functions defined in src/blob/*.c and src/blob/groups/*/ (group.json members/claims, sources),
  * cloud/matches/*.c (file stem or definitions), and
  * claims of the drafted groups under cloud/work/ipa-groups/*/group.json.

Features per function: words, class (ipakit.deps: ABI / IPA-leaf / IPA-caller / IPA-both / TAIL), minimal
closure size in functions and words (IPA), callers count, near-miss / draft sources found under cloud/work/,
a drafted IPA group that already contains it, m2c seedability (no indirect calls, sane size, real head).

Score (transparent, see `estimate`):   expected = p_success * (words + 8);   cost = 1 + words/60 + closure_words/100;
priority = expected / cost.  p_success starts from a per-class prior and is raised by an existing near-miss or draft
source and lowered by size and closure size.  The prior numbers are guesses calibrated on the cloud rounds (ABI
singles with a near-miss finish most often; large IPA closures almost never without a human), not measurements.

Output rows: {fn, priority, p_success, words, class, closure_funcs, closure_words, callers, seeds, group, reason}.
"""
import argparse
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(HERE.parent))

PRIOR = {"ABI": 0.45, "IPA-leaf": 0.20, "IPA-caller": 0.12, "IPA-both": 0.10, "TAIL": 0.0}
DEF_RX = re.compile(r"(?m)^[A-Za-z_][^;{}()=\n]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{")


def is_ipa(cls):
    return str(cls).startswith("IPA")


# --- exclusion sets -----------------------------------------------------------------------------

def defined_names(text, wanted):
    return {m.group(1) for m in DEF_RX.finditer(text)} & wanted


def spliced_names(wanted, root=ROOT):
    """{name: why} for functions already spliced, matched in cloud/matches, or claimed by a group."""
    root = Path(root)
    out = {}
    for p in sorted((root / "src" / "blob").glob("*.c")):
        if p.stem in wanted:
            out.setdefault(p.stem, "src/blob/" + p.name)
        for n in defined_names(p.read_text(errors="replace"), wanted):
            out.setdefault(n, "src/blob/" + p.name)
    for gj in sorted((root / "src" / "blob" / "groups").glob("*/group.json")):
        try:
            spec = json.loads(gj.read_text())
        except ValueError:
            continue
        for n in list(spec.get("members", [])) + list(spec.get("claims", [])):
            if n in wanted:
                out.setdefault(n, "src/blob/groups/%s" % gj.parent.name)
    for p in sorted((root / "cloud" / "matches").glob("*.c")):
        if p.stem in wanted:
            out.setdefault(p.stem, "cloud/matches/" + p.name)
        for n in defined_names(p.read_text(errors="replace"), wanted):
            out.setdefault(n, "cloud/matches/" + p.name)
    for gj in sorted((root / "cloud" / "work" / "ipa-groups").glob("*/group.json")):
        try:
            spec = json.loads(gj.read_text())
        except ValueError:
            continue
        for n in spec.get("claims", []):
            if n in wanted:
                out.setdefault(n, "group claim cloud/work/ipa-groups/%s" % gj.parent.name)
    return out


def draft_sources(wanted, root=ROOT):
    """{fn: [relative path, ...]} of draft / near-miss sources under cloud/work (near-miss base.c first)."""
    root = Path(root)
    work = root / "cloud" / "work"
    out = {}
    if not work.is_dir():
        return out
    for p in sorted(work.rglob("*.c")):
        rel = p.relative_to(root)
        parts = rel.parts
        if "tools" in parts[:3] or "ipa-groups" in parts:
            continue
        try:
            text = p.read_text(errors="replace")
        except OSError:
            continue
        names = defined_names(text, wanted)
        if p.stem in wanted:
            names.add(p.stem)
        if p.name == "base.c" and p.parent.name in wanted:
            names.add(p.parent.name)
        for n in names:
            out.setdefault(n, []).append(str(rel))
    for lst in out.values():
        lst.sort(key=lambda s: (0 if "/near-miss/" in s else 1, s))
    return out


def near_miss_index(root=ROOT):
    """{fn: strict words differing} from cloud/work/near-miss/INDEX.md (historical, rescored by gen_nearmiss_index)."""
    out = {}
    p = Path(root) / "cloud" / "work" / "near-miss" / "INDEX.md"
    try:
        for line in p.read_text().splitlines():
            m = re.match(r"\|\s*(\w+)\s*\|\s*[-\d]+\s*\|\s*(\d+)\s*\|", line)
            if m:
                out[m.group(1)] = int(m.group(2))
    except OSError:
        pass
    return out


def draft_groups(wanted, root=ROOT):
    """{fn: group dir name} for members of drafted IPA groups (cloud/work/ipa-groups)."""
    out = {}
    for gj in sorted((Path(root) / "cloud" / "work" / "ipa-groups").glob("*/group.json")):
        try:
            spec = json.loads(gj.read_text())
        except ValueError:
            continue
        for n in spec.get("members", []):
            if n in wanted and not n.startswith("__standin"):
                out.setdefault(n, gj.parent.name)
    return out


# --- estimate -------------------------------------------------------------------------------------------

def estimate(f):
    """f: feature dict -> (priority, p_success, reason list). Pure function (unit tested)."""
    cls, words = f["class"], f["words"]
    p = PRIOR.get(cls, 0.1)
    why = ["%s class (prior %.2f)" % (cls, p)]
    srcs = f.get("seeds") or []
    nd = f.get("near_diff")
    if any("/near-miss/" in s for s in srcs):
        p = min(0.9, p * 1.7 + 0.1)
        why.append("has near-miss source")
        if nd is not None and nd <= 8:
            p = min(0.95, p + 0.2)
            why.append("near-miss index: %d words differ" % nd)
    elif srcs:
        p = min(0.85, p * 1.4 + 0.05)
        why.append("has %d draft source(s)" % len(srcs))
    if f.get("group"):
        p = min(0.85, p * 1.3 + 0.05)
        why.append("member of drafted group %s" % f["group"])
    p /= 1 + words / 150.0
    if words > 150:
        why.append("large (%dw)" % words)
    cw = f.get("closure_words") or 0
    if is_ipa(cls):
        p /= 1 + cw / 300.0
        why.append("closure %d funcs / %d words" % (f.get("closure_funcs") or 0, cw))
    if f.get("callers"):
        why.append("%d caller(s)" % f["callers"])
    if f.get("m2c_seedable"):
        why.append("m2c-seedable")
    else:
        p *= 0.6
        why.append("not m2c-seedable (%s)" % (f.get("m2c_note") or "?"))
    cost = 1 + words / 60.0 + cw / 100.0
    pri = p * (words + 8) / cost
    return pri, p, why


def features(args_class="all", min_words=0, max_words=10 ** 9, closure=True, root=ROOT, model=None, tgts=None):
    from ipakit import deps
    if tgts is None:
        sys.path.insert(0, str(ROOT / "tools" / "cloud"))
        import score
        tgts = score.targets()
    wanted = set(tgts)
    done = spliced_names(wanted, root)
    drafts = draft_sources(wanted, root)
    groups = draft_groups(wanted, root)
    nm = near_miss_index(root)
    m = model or deps.model_cache()
    rows = []
    for fn in sorted(tgts):
        if fn in done:
            continue
        words = len(tgts[fn])
        if not (min_words <= words <= max_words):
            continue
        cls = m.classify(fn)[0] if fn in m.infos else "ABI"
        if cls == "TAIL":
            continue
        if args_class == "abi" and cls != "ABI":
            continue
        if args_class == "ipa" and not is_ipa(cls):
            continue
        cf = cw = None
        if is_ipa(cls) and closure:
            try:
                reasons, _ = m.minimal_closure([fn], "direct")
                cf, cw = len(reasons), m.words(reasons)
            except Exception:  # noqa: BLE001 - keep triage alive on odd closures
                cf, cw = None, None
        elif not is_ipa(cls):
            cf, cw = 1, words
        seedable, note = True, "ok"
        if fn in getattr(m, "indirect", {}):
            seedable, note = False, "indirect jalr calls"
        elif words > 400:
            seedable, note = False, "over 400 words"
        elif getattr(m.corpus.funcs.get(fn), "discovered", False):
            note = "discovered head (targets from image)"
        f = {"fn": fn, "class": cls, "words": words, "closure_funcs": cf, "closure_words": cw,
             "callers": len(m.callers.get(fn, {})), "seeds": drafts.get(fn, []),
             "group": groups.get(fn), "near_diff": nm.get(fn), "m2c_seedable": seedable, "m2c_note": note}
        pri, p, why = estimate(f)
        f.update(priority=round(pri, 3), p_success=round(p, 3), reason="; ".join(why))
        rows.append(f)
    rows.sort(key=lambda r: (-r["priority"], r["fn"]))
    return rows


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--top", type=int, default=30)
    ap.add_argument("--class", dest="cls", choices=["abi", "ipa", "all"], default="all")
    ap.add_argument("--min-words", type=int, default=0)
    ap.add_argument("--max-words", type=int, default=10 ** 9)
    ap.add_argument("--no-closure", action="store_true", help="skip IPA closure sizing (faster)")
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args(argv)
    rows = features(a.cls, a.min_words, a.max_words, not a.no_closure)
    rows = rows[:a.top] if a.top else rows
    if a.json:
        print(json.dumps(rows, indent=1))
        return 0
    print("%-4s %-26s %-10s %5s %6s %5s  %s" % ("#", "function", "class", "words", "p", "pri", "reason"))
    for i, r in enumerate(rows, 1):
        print("%-4d %-26s %-10s %5d %6.2f %5.1f  %s" % (i, r["fn"], r["class"], r["words"], r["p_success"],
                                                       r["priority"], r["reason"]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
