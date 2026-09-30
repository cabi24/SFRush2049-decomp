#!/usr/bin/env python3
"""search.py: deterministic optimizer over mutation sequences (stdlib only).

    search.py FILE FUNC [--flags ..] [--budget N] [--seconds S] [--jobs J] [--seed S] [--out DIR] [--json]
    search.py group DIR [--budget N] [--seconds S] [--jobs J] [--seed S] [--out DIR] [--json]

Mode fn mutates a whole translation unit and scores FUNC; mode group mutates the source files
of an IPA group dir (group.c / members) and scores every member with builder.compile_group,
refusing any candidate that regresses a member that already matched (or is claimed and matched).

Objective (lexicographic, lower is better): not strict-matched, then a NEAR bucket (strict_diff
<= NEAR_WORDS: these are ranked by strict_diff first, then aligned_exact; farther candidates keep the
aligned_exact-first ranking because a one-instruction shift inflates strict_diff), then |size - target|,
then mutation cost.  Group mode: not group matched, matched members (more), summed aligned_exact,
summed strict_diff, size, cost.

Stages (one shared eval budget, --budget evals / --seconds):
  0. (fn mode, real compiler only) -O1/-O2/-O3 variants of the start as extra search roots
     (`--no-flag-dim` disables),
  1. best-first over every single mutation of each root,
  2. cross product of pairs of the best-scoring compiling singles (neutral / improving first; edits
     are merged on the root source, so independent edits combine without re-enumeration), then
     greedy growth of the best merged-edit nodes (3-6 cooperating edits),
  3. beam over depth 2..3 (beam width --beam; the frontier also keeps neutral nodes),
  4. annealing / random walk with restarts over sequences of 1-3 mutations, tabu on source
     hashes, adaptive per-class mutation weights (classes that improved the score get sampled more).
A matched path is then minimised (greedy drop of each step that is not needed).  Target-derived
hints (globals whose addresses the target words materialise) are registered with mutate.set_hints.
Evaluation is parallel through builder.score_many; results come back in job order so a run is
a function of (inputs, --seed) apart from the --seconds cutoff.  On a strict MATCH the search stops and
re-verifies with the real `tools/cloud/score.py fn|group`; only a passing re-verification is
reported as matched.

Mutations come from amatch.mutate: mutations(src, fn=None, catalog=None) -> [Mutation] with
name, new_src, cost, target, id.  Output (--out, default build/amatch_runs/<fn>/):
best.c (or best_group/), search_log.json (every evaluated candidate: hash, mutation path, scores),
result.json.  --json prints {matched, evals, seconds, best_score, best_src_path, log_path, tried_mutations}.
"""
import argparse
import hashlib
import json
import math
import os
import random
import re
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(HERE.parent))
from amatch import builder  # noqa: E402

BIG = 10 ** 9
SCORE_PY = ROOT / "tools" / "cloud" / "score.py"
SCORE_KEYS = ("strict_diff", "size", "target_size", "extra", "aligned_exact", "aligned_opcode",
              "aligned_opcode_reg", "matched")


# --- mutation plumbing ------------------------------------------------------------------

def default_mutator():
    from amatch import mutate  # noqa: WPS433 (late: mutate.py is written by another module)
    return mutate


def _norm(m):
    """Normalize a Mutation-ish object to a dict."""
    def g(k, d=None):
        if isinstance(m, dict):
            return m.get(k, d)
        return getattr(m, k, d)
    if isinstance(m, (tuple, list)):
        return {"name": m[0], "new_src": m[1], "cost": float(m[2]) if len(m) > 2 else 1.0,
                "target": None, "id": m[0], "desc": None, "edits": None, "flags": None}
    name = g("name", "?")
    return {"name": name, "new_src": g("new_src"), "cost": float(g("cost", 1.0) or 0.0),
            "target": g("target"), "id": g("id", name), "desc": g("desc"),
            "edits": g("edits"), "flags": g("set_flags")}


def mut_class(name):
    """Adaptive-weight class of a mutation name: its leading word (before ':', '@', '(', space)."""
    return re.split(r"[^\w\-]", str(name), 1)[0] or "?"


def sha(text):
    return hashlib.sha1(text.encode("utf-8", "replace")).hexdigest()[:16]


def state_hash(files, flags=None):
    h = hashlib.sha1()
    for n in sorted(files):
        h.update(n.encode() + b"\0" + files[n].encode("utf-8", "replace") + b"\1")
    if flags:
        h.update(b"\2" + flags.encode())
    return h.hexdigest()[:16]


NEAR_WORDS = 8
OPT_LEVELS = ("-O1", "-O2", "-O3")


def flag_variants(flags):
    """Other -O levels of a flag string (the same string with -O1/-O2/-O3 swapped)."""
    m = re.search(r"(?<!\S)-O[0-3s]?(?!\S)", flags or "")
    if not m:
        return []
    return [(f"flags:{lv}", flags[:m.start()] + lv + flags[m.end():]) for lv in OPT_LEVELS
            if lv != m.group(0)]


def combine_edits(src, edits_a, edits_b):
    """Source after applying both edit lists (on the same base `src`), or None when they overlap."""
    eds = sorted(list(edits_a) + list(edits_b), key=lambda x: (x[0], x[1]))
    prev = -1
    for i, (st, en, _t) in enumerate(eds):
        if st < prev or (i and st == en == eds[i - 1][0] == eds[i - 1][1]):
            return None
        prev = max(prev, en)
    out, pos = [], 0
    for st, en, t in eds:
        out.append(src[pos:st])
        out.append(t)
        pos = en
    out.append(src[pos:])
    return "".join(out)


def target_hints(words):
    """Names `D_xxxxxxxx` / `func_xxxxxxxx` for addresses the target words materialise (lui + addiu /
    ori / load / store pairs, and jal targets).  Used only as candidate identifiers."""
    val, out = {}, set()
    for w in words or ():
        op, rs, rt = (w >> 26) & 63, (w >> 21) & 31, (w >> 16) & 31
        imm = w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        if op == 0x0F:
            val[rt] = (imm << 16) & 0xFFFFFFFF
        elif op in (0x09, 0x0D) and rs in val:
            a = (val[rs] + (simm if op == 0x09 else imm)) & 0xFFFFFFFF
            val[rt] = a
            if 0x80000000 <= a < 0x80400000:
                out.add("D_%08X" % a)
        elif op in (0x20, 0x21, 0x23, 0x24, 0x25, 0x28, 0x29, 0x2B, 0x31, 0x39, 0x35, 0x3D) and rs in val:
            a = (val[rs] + simm) & 0xFFFFFFFF
            if 0x80000000 <= a < 0x80400000:
                out.add("D_%08X" % a)
        elif op == 0x03:
            out.add("func_%08X" % (0x80000000 | ((w & 0x3FFFFFF) << 2)))
    return out


class Node:
    """A candidate source state plus its provenance."""
    __slots__ = ("files", "h", "path", "cost", "key", "energy", "score", "cls", "flags", "steps", "mut",
                 "raw", "root", "medits")

    def __init__(self, files, path=(), cost=0.0, flags=None, steps=()):
        self.files = files
        self.flags = flags
        self.h = state_hash(files, flags)
        self.path = tuple(path)
        self.cost = cost
        self.key = None
        self.energy = None
        self.score = None
        self.cls = None
        self.steps = tuple(steps)   # (fname, name, desc) replay records
        self.mut = None             # the single mutation dict that produced this node from its root
        self.raw = None             # energy without the cost term
        self.root = None
        self.medits = None          # merged edits relative to the root source (singles and their merges)


# --- objective -----------------------------------------------------------------------------

def _member_energy(d):
    if d.get("matched"):
        return 0.0
    t = d.get("target_size") or d.get("target_words") or 0
    return (t - d.get("aligned_exact", 0)) + 0.25 * d.get("strict_diff", t) \
        + 0.02 * abs(d.get("size", 0) - t)


def fn_energy(r):
    """Smooth scalar for annealing: half strict word diffs, half instructions missing from the
    aligned sequence (a one-word shift shows up in strict but not in aligned), plus a size term."""
    t = r.get("target_size") or r.get("target_words") or 0
    return 0.5 * (t - r.get("aligned_exact", 0)) + 0.5 * r.get("strict_diff", t) \
        + 0.02 * abs(r.get("size", 0) - t)


def fn_objective(r, cost):
    if r.get("err"):
        return (1, 1, BIG, BIG, BIG, cost), 1e6
    sd, ae = r["strict_diff"], r["aligned_exact"]
    near = sd <= NEAR_WORDS
    # near the target a differing word is a real difference: rank strict first; further away a
    # one-instruction shift inflates strict_diff, so aligned_exact leads
    key = (0 if r.get("matched") else 1, 0 if near else 1, sd if near else -ae, -ae if near else sd,
           abs(r["size"] - r["target_size"]), cost)
    return key, (-1.0 if r.get("matched") else fn_energy(r) + 0.01 * cost)


def group_objective(r, cost, protected):
    if r.get("err"):
        return (1, BIG, BIG, BIG, BIG, cost), 1e6, False
    mem = r.get("members", {})
    if not mem:
        return (1, BIG, BIG, BIG, BIG, cost), 1e6, False
    for n in protected:
        if n in mem and not mem[n].get("matched"):
            return (1, BIG, BIG, BIG, BIG, cost), 1e6, True
    nm = sum(1 for d in mem.values() if d.get("matched"))
    al = sum(d.get("aligned_exact", 0) for d in mem.values())
    sd = sum(d.get("strict_diff", 0) for d in mem.values())
    sz = sum(abs(d.get("size", 0) - d.get("target_size", 0)) for d in mem.values())
    key = (0 if r.get("matched") else 1, -nm, -al, sd, sz, cost)
    e = -1.0 if r.get("matched") else sum(_member_energy(d) for d in mem.values()) + 0.01 * cost
    return key, e, False


def summarize(r, kind):
    if kind == "fn":
        s = {k: r.get(k) for k in SCORE_KEYS}
        s["err"] = (r["err"].splitlines()[0][:160] if r.get("err") else None)
        return s
    mem = r.get("members", {})
    return {"matched": r.get("matched"), "err": (r["err"].splitlines()[0][:160] if r.get("err") else None),
            "members": {n: {k: d.get(k) for k in SCORE_KEYS} for n, d in mem.items()}}


# --- the searcher ---------------------------------------------------------------------------------

class Searcher:
    def __init__(self, kind, start_files, *, fn=None, flags=None, group_dir=None, mutable=None,
                 jobs=4, seed=0, budget=2000, seconds=None, mutator=None, catalog=None,
                 evaluator=None, beam=5, max_depth=3, log=None, progress=None, flag_dim=False,
                 hints=None):
        self.kind = kind
        self.flag_dim = bool(flag_dim) and kind == "fn" and bool(flags)
        self.hints = hints
        self.fn = fn
        self.flags = flags
        self.group_dir = group_dir
        self.files0 = dict(start_files)
        self.mutable = list(mutable or sorted(start_files))
        self.jobs = max(1, jobs)
        self.rng = random.Random(seed)
        self.seed = seed
        self.budget = budget
        self.seconds = seconds
        self.mutator = mutator
        self.catalog = catalog
        self.evaluator = evaluator or (lambda js: builder.score_many(js, jobs_n=self.jobs))
        self.beam = beam
        self.max_depth = max_depth
        self.batch = max(2, self.jobs * 2)
        self.progress = progress
        self.t0 = time.time()
        self.evals = 0
        self.seen = {}            # hash -> log index (tabu and dedupe)
        self.log = []
        self.weights = {}         # mutation class -> weight
        self.class_stats = {}     # class -> [tried, improved, best_delta]
        self.best = None
        self.base = None
        self.protected = set()
        self.matched_node = None
        self.matched_raw = None
        self._mcache = {}
        self._mcache_order = []
        self.roots = []
        self.level = []
        self.min_evals = 0

    # -- limits
    def elapsed(self):
        return time.time() - self.t0

    def out_of_budget(self, reserve=0):
        if self.matched_node is not None:
            return True
        if self.evals + reserve >= self.budget:
            return True
        return self.seconds is not None and self.elapsed() >= self.seconds

    # -- mutation enumeration (memoized, small LRU: each list holds full sources)
    def mutations_of(self, node):
        if node.h in self._mcache:
            return self._mcache[node.h]
        out = []
        if self.flag_dim and node.flags:
            for nm, fl in flag_variants(node.flags):
                out.append((self.mutable[0], {"name": nm, "new_src": node.files[self.mutable[0]], "cost": 5.0,
                                              "target": "flags", "id": nm, "desc": fl, "edits": None,
                                              "flags": fl}))
        for fname in self.mutable:
            text = node.files[fname]
            fnarg = self.fn if self.kind == "fn" else None
            try:
                muts = self.mutator.mutations(text, fnarg, self.catalog) \
                    if self.catalog is not None else self.mutator.mutations(text, fnarg)
            except Exception as exc:  # a broken mutator must not kill the run
                self.log_note(f"mutator error on {fname}: {type(exc).__name__}: {exc}")
                muts = []
            for m in muts:
                m = _norm(m)
                if m["new_src"] is None or m["new_src"] == text:
                    continue
                out.append((fname, m))
        self._mcache[node.h] = out
        self._mcache_order.append(node.h)
        while len(self._mcache_order) > 6:
            self._mcache.pop(self._mcache_order.pop(0), None)
        return out

    def log_note(self, msg):
        self.log.append({"note": msg})

    def child(self, node, fname, m):
        files = dict(node.files)
        files[fname] = m["new_src"]
        label = m["name"] if len(self.mutable) == 1 else f"{fname}:{m['name']}"
        c = Node(files, node.path + (label,), node.cost + m["cost"], m.get("flags") or node.flags,
                 node.steps + ((fname, m["name"], m.get("desc")),))
        c.cls = mut_class(m["name"])
        c.mut = m
        c.root = node.root if node.root is not None else node
        if node.root is None and m.get("edits") and node.flags == c.flags:
            c.medits = tuple(m["edits"])
        return c

    def weight(self, m):
        return self.weights.get(mut_class(m["name"]), 1.0) / (1.0 + max(0.0, m["cost"]))

    # -- evaluation
    def job(self, node):
        if self.kind == "fn":
            j = {"kind": "fn", "src": node.files[self.mutable[0]], "fn": self.fn}
            if node.flags or self.flags:
                j["flags"] = node.flags or self.flags
            return j
        return {"kind": "group", "dir": str(self.group_dir), "overrides": {"files": node.files}}

    def evaluate(self, nodes, parents=None):
        """Evaluate unseen nodes (respecting budget). Returns the list of evaluated nodes."""
        todo, seen_here = [], set()
        for i, n in enumerate(nodes):
            if n.h in self.seen or n.h in seen_here:
                continue
            seen_here.add(n.h)
            todo.append((i, n))
        room = self.budget - self.evals
        todo = todo[:max(0, room)]
        if not todo:
            return []
        results = self.evaluator([self.job(n) for _, n in todo])
        done = []
        for (i, n), r in zip(todo, results):
            self.evals += 1
            self.record(n, r, parents[i] if parents else None)
            done.append(n)
        return done

    def record(self, n, r, parent):
        if self.kind == "fn":
            n.key, n.energy = fn_objective(r, n.cost)
            rejected = False
            n.raw = n.energy - 0.01 * n.cost
        else:
            n.key, n.energy, rejected = group_objective(r, n.cost, self.protected)
            n.raw = n.energy - 0.01 * n.cost
        n.score = summarize(r, self.kind)
        entry = {"hash": n.h, "path": list(n.path), "cost": round(n.cost, 3),
                 **({"flags": n.flags} if n.flags and n.flags != self.flags else {}),
                 "parent": parent.h if parent is not None else None,
                 "score": n.score, "energy": round(n.energy, 3), "secs": round(r.get("secs", 0), 3),
                 "cached": bool(r.get("cached"))}
        if rejected:
            entry["rejected"] = "regresses a matching member"
        self.seen[n.h] = len(self.log)
        self.log.append(entry)
        # credit assignment for adaptive weights
        if parent is not None and parent.energy is not None and n.path:
            cl = n.cls or mut_class(n.path[-1])
            st = self.class_stats.setdefault(cl, [0, 0, 0.0])
            st[0] += 1
            delta = parent.energy - n.energy
            w = self.weights.get(cl, 1.0)
            if r.get("err"):
                w = max(0.2, w * 0.9)
            elif delta > 1e-9:
                st[1] += 1
                st[2] = max(st[2], delta)
                w = min(25.0, w * 1.6)
            elif delta < -1e-9:
                w = max(0.25, w * 0.97)
            self.weights[cl] = w
        if self.best is None or n.key < self.best.key:
            self.best = n
            if self.progress:
                self.progress(self)
        if self.kind == "group" and not rejected and r.get("members"):
            # matched members of a new best join the protected set
            if self.best is n:
                self.protected |= {m for m, d in r["members"].items() if d.get("matched")}
        if n.key[0] == 0 and not rejected:
            self.matched_node = n
            self.matched_raw = r

    # -- stages
    def run(self):
        if self.hints is not None and self.kind == "fn" and hasattr(self.mutator, "set_hints"):
            self.mutator.set_hints(self.hints)
        self.base = Node(self.files0, flags=self.flags if self.kind == "fn" else None)
        self.base.root = None
        self.evaluate([self.base])
        if self.kind == "group" and self.base.score:
            self.protected = {m for m, d in self.base.score.get("members", {}).items() if d["matched"]}
        if self.out_of_budget():
            return self
        self.roots = [self.base]
        if self.flag_dim and not self.out_of_budget():
            alts = []
            for nm, fl in flag_variants(self.flags):
                c = Node(self.files0, (nm,), 5.0, fl, ((self.mutable[0], nm, fl),))
                c.cls, c.root = "flags", None
                alts.append(c)
            done = self.evaluate(alts, [self.base] * len(alts))
            # an alternative -O level becomes a root only when it is strictly better; otherwise it stays
            # reachable as an ordinary `flags:` mutation of any node
            ok = [n for n in done if n.energy < 1e5 and n.raw < self.base.raw - 1e-9]
            ok.sort(key=lambda n: n.key)
            self.roots += ok[:1]
        self.stage_singles()
        if not self.out_of_budget():
            self.stage_pairs()
        if not self.out_of_budget():
            self.stage_grow()
        if not self.out_of_budget():
            self.stage_beam()
        if not self.out_of_budget():
            self.stage_anneal()
        return self

    def order_muts(self, muts):
        muts = list(muts)
        self.rng.shuffle(muts)                     # seeded tie-break
        muts.sort(key=lambda fm: fm[1]["cost"])   # stable: cheap first
        return muts

    def eval_children(self, parent, muts, cap=None):
        """Evaluate children of parent for the given mutations in batches; stop early on match."""
        kids = []
        for fname, m in muts:
            if cap is not None and len(kids) >= cap:
                break
            c = self.child(parent, fname, m)
            if c.h in self.seen:
                continue
            kids.append(c)
        done = []
        for i in range(0, len(kids), self.batch):
            if self.out_of_budget():
                break
            chunk = kids[i:i + self.batch]
            done += self.evaluate(chunk, [parent] * len(chunk))
        return done

    def stage_singles(self):
        self.singles_total = 0
        self.level = []
        self.by_root = {}
        for root in self.roots:
            muts = self.order_muts(self.mutations_of(root))
            self.singles_total += len(muts)
            cap = max(self.batch, int(self.budget * 0.3 / len(self.roots)))
            kids = self.eval_children(root, muts, cap=cap)
            self.by_root[root.h] = kids
            self.level += kids
            if self.out_of_budget():
                break
        self.level += [r for r in self.roots if r is not self.base]

    def stage_pairs(self):
        """Cross product of the 64 best-scoring compiling singles (neutral and improving ones first:
        plateau moves): independent edits of the same root are merged textually, so a fix that needs two or three cooperating changes is reachable
        even when no single change scores better."""
        if self.kind != "fn":
            return
        start = self.evals
        share = max(self.batch, int(self.budget * 0.3))
        for root in self.roots:
            if self.out_of_budget():
                return
            kids = [k for k in self.by_root.get(root.h, [])
                    if k.mut and k.mut.get("edits") and k.raw is not None and k.energy < 1e5]
            kids.sort(key=lambda n: (n.raw, n.cost))
            kids = kids[:64]
            if len(kids) < 2:
                continue
            src0 = root.files[self.mutable[0]]
            combos = []
            for i in range(len(kids)):
                for j in range(i + 1, len(kids)):
                    a, b = kids[i], kids[j]
                    combos.append((a.raw + b.raw, a.cost + b.cost, i, j))
            combos.sort()
            cands, parents = [], []
            for _r, _c, i, j in combos:
                a, b = kids[i], kids[j]
                if a.mut["name"].split("_")[0] == "flags":
                    continue
                txt = combine_edits(src0, a.mut["edits"], b.mut["edits"])
                if txt is None or txt == src0:
                    continue
                files = dict(root.files)
                files[self.mutable[0]] = txt
                c = Node(files, a.path + b.path[-1:], a.cost + b.cost - root.cost, root.flags,
                         a.steps + b.steps[-1:])
                c.cls, c.root = a.cls, root
                c.mut = None
                c.medits = tuple(a.medits) + tuple(b.medits)
                if c.h in self.seen:
                    continue
                cands.append(c)
                parents.append(a)
            for k in range(0, len(cands), self.batch):
                if self.out_of_budget() or self.evals - start >= share:
                    break
                self.evaluate(cands[k:k + self.batch], parents[k:k + self.batch])
                self.level += [c for c in cands[k:k + self.batch] if c.key is not None]

    def stage_grow(self):
        """Greedy forward selection over merged edits: start from the best one- or two-edit nodes and keep
        adding the single edit that helps most (non-overlapping edits merge textually), so three or four
        cooperating edits (drop pad locals, inline a temp, retype a cast, ...) are reachable."""
        if self.kind != "fn":
            return
        start = self.evals
        share = max(self.batch, int(self.budget * 0.25))
        pool = {}
        for n in self.level:
            if n.medits and n.key is not None and n.energy < 1e5 and n.root is not None:
                pool.setdefault(n.root.h, []).append(n)
        for rh, nodes in pool.items():
            root = nodes[0].root
            src0 = root.files[self.mutable[0]]
            singles = sorted([n for n in self.by_root.get(rh, []) if n.medits and n.energy < 1e5],
                             key=lambda n: (n.raw, n.cost))[:80]
            starts = sorted(nodes, key=lambda n: n.key)[:3]
            for cur in starts:
                for _depth in range(5):
                    if self.out_of_budget() or self.evals - start >= share:
                        return
                    cands, parents = [], []
                    for k in singles:
                        if k.steps[-1] in cur.steps:
                            continue
                        txt = combine_edits(src0, cur.medits, k.medits)
                        if txt is None or txt == src0:
                            continue
                        files = dict(root.files)
                        files[self.mutable[0]] = txt
                        c = Node(files, cur.path + k.path[-1:], cur.cost + k.cost - root.cost, root.flags,
                                 cur.steps + k.steps[-1:])
                        c.cls, c.root, c.mut = k.cls, root, None
                        c.medits = tuple(cur.medits) + tuple(k.medits)
                        if c.h in self.seen:
                            continue
                        cands.append(c)
                        parents.append(cur)
                    done = []
                    for i in range(0, len(cands), self.batch):
                        if self.out_of_budget() or self.evals - start >= share:
                            break
                        done += self.evaluate(cands[i:i + self.batch], parents[i:i + self.batch])
                    done = [c for c in done if c.key is not None and c.energy < 1e5]
                    self.level += done
                    if not done:
                        break
                    bestc = min(done, key=lambda n: n.key)
                    if bestc.key >= cur.key:
                        break
                    cur = bestc

    def stage_beam(self):
        frontier = self.level
        for depth in range(2, self.max_depth + 1):
            if not frontier or self.out_of_budget():
                return
            ranked = sorted(frontier, key=lambda n: n.key)
            picked = ranked[:self.beam]
            ids = {n.h for n in picked}
            # plateau moves: keep a few distinct neutral nodes too (same score as the base, other edits)
            neutral = [n for n in ranked[self.beam:] if n.raw is not None and n.energy < 1e5
                       and self.base.raw is not None and n.raw <= self.base.raw + 1e-9 and n.h not in ids]
            frontier = picked + neutral[:self.beam]
            nxt = []
            share = max(self.batch, int(self.budget * 0.5 / max(1, self.max_depth - 1)))
            for p in frontier:
                if self.out_of_budget():
                    return
                muts = self.order_muts(self.mutations_of(p))
                nxt += self.eval_children(p, muts, cap=max(self.batch, share // max(1, len(frontier))))
            frontier = nxt

    # -- path minimisation (fn mode): drop every step the match does not need
    def replay(self, steps):
        cur = Node(self.files0, flags=self.base.flags)
        for (fname, name, desc) in steps:
            found = None
            for fn2, m in self.mutations_of(cur):
                if fn2 == fname and m["name"] == name and m.get("desc") == desc:
                    found = m
                    break
            if found is None:
                return None
            cur = self.child(cur, fname, found)
        return cur

    def minimize(self, node):
        if self.kind != "fn" or len(node.steps) <= 1:
            return node
        best = node
        steps = list(node.steps)
        changed = True
        while changed and len(steps) > 1:
            changed = False
            for i in range(len(steps)):
                trial = steps[:i] + steps[i + 1:]
                n = self.replay(trial)
                if n is None or n.h == best.h:
                    continue
                self.min_evals += 1
                r = self.evaluator([self.job(n)])[0]
                key, energy = fn_objective(r, n.cost)
                if key[0] == 0:
                    n.key, n.energy, n.raw = key, energy, energy
                    n.score = summarize(r, "fn")
                    best, steps, changed = n, trial, True
                    break
        return best

    def sample(self, node):
        muts = self.mutations_of(node)
        if not muts:
            return None
        ws = [self.weight(m) for _, m in muts]
        return self.rng.choices(muts, weights=ws, k=1)[0]

    def random_sequence(self, base):
        L = self.rng.choices((1, 2, 3), weights=(5, 3, 2))[0]
        cur = base
        for _ in range(L):
            pick = self.sample(cur)
            if pick is None:
                break
            cur = self.child(cur, *pick)
        return cur if cur is not base else None

    def stage_anneal(self):
        cur = self.best
        T0 = 2.0
        start_evals = self.evals
        stale, last_best = 0, self.best.key
        pool = [self.base]
        while not self.out_of_budget():
            frac = min(1.0, (self.evals - start_evals) / max(1, self.budget - start_evals))
            if self.seconds:
                frac = max(frac, min(1.0, self.elapsed() / self.seconds))
            T = max(0.05, T0 * (1.0 - frac))
            bases, cands = [], []
            tries = 0
            while len(cands) < self.batch and tries < self.batch * 12:
                tries += 1
                r = self.rng.random()
                base = cur if r < 0.7 else (self.best if r < 0.9 else self.rng.choice(pool))
                c = self.random_sequence(base)
                if c is None or c.h in self.seen or any(c.h == x.h for x in cands):
                    continue
                cands.append(c)
                bases.append(base)
            if not cands:
                # neighbourhood exhausted around here: restart from elsewhere or stop
                if stale > 40:
                    break
                cur = self.rng.choice(pool)
                stale += 10
                continue
            done = self.evaluate(cands, bases)
            by = {c.h: b for c, b in zip(cands, bases)}
            for c in done:
                if c.key[0] == 1 and c.energy < 1e5 and len(pool) < 24 and c.energy < self.base.energy:
                    pool.append(c)
                d = c.energy - cur.energy
                if d <= 0 or self.rng.random() < math.exp(-d / T):
                    if c.energy < 1e5:
                        cur = c
            if self.best.key < last_best:
                last_best, stale = self.best.key, 0
                cur = self.best
            else:
                stale += 1
            if stale and stale % 6 == 0:          # restart
                cur = self.best if self.rng.random() < 0.6 else self.rng.choice(pool)


# --- verification ------------------------------------------------------------------------------

def verify_fn(src_text, fn, flags):
    with tempfile.TemporaryDirectory(prefix="sv-") as tmp:
        p = Path(tmp) / "src.c"
        p.write_text(src_text)
        r = subprocess.run([sys.executable, str(SCORE_PY), "fn", str(p), fn, "--flags", flags],
                           capture_output=True, text=True, cwd=ROOT)
    return r.returncode == 0, (r.stdout + r.stderr)[-600:]


def verify_group(group_dir, files):
    with tempfile.TemporaryDirectory(prefix="sv-") as tmp:
        d = Path(tmp) / "g"
        shutil.copytree(group_dir, d)
        for n, t in files.items():
            (d / n).write_text(t)
        r = subprocess.run([sys.executable, str(SCORE_PY), "group", str(d)],
                           capture_output=True, text=True, cwd=ROOT)
    return r.returncode == 0, (r.stdout + r.stderr)[-600:]


# --- drivers ---------------------------------------------------------------------------------------

def _finish(s, out, label, kind, meta, verify):
    out = Path(out)
    out.mkdir(parents=True, exist_ok=True)
    best = s.matched_node or s.best
    secs = s.elapsed()
    verified, vmsg = None, None
    full_path = list(best.path)
    if s.matched_node is not None:
        mini = s.minimize(best)
        verified, vmsg = verify(mini)
        if verified:
            best = mini
        elif mini is not best:
            verified, vmsg = verify(best)
    matched = bool(verified)
    if kind == "fn":
        best_path = out / "best.c"
        text = best.files[s.mutable[0]]
        if best.flags and best.flags != s.flags:
            cm = "/* flags: %s */" % best.flags
            text = re.sub(r"\A/\*\s*flags:[^*]*\*/", cm, text) if re.match(r"/\*\s*flags:", text) else cm + "\n" + text
        best_path.write_text(text)
    else:
        best_path = out / "best_group"
        if best_path.exists():
            shutil.rmtree(best_path)
        if s.group_dir is not None and Path(s.group_dir).exists():
            shutil.copytree(s.group_dir, best_path)
        else:
            best_path.mkdir()
        for n, t in best.files.items():
            (best_path / n).write_text(t)
    tried = {}
    for e in s.log:
        for p in e.get("path", [])[-1:]:
            tried[p] = tried.get(p, 0) + 1
    classes = {c: {"tried": v[0], "improved": v[1], "best_delta": round(v[2], 2),
                   "weight": round(s.weights.get(c, 1.0), 2)} for c, v in sorted(s.class_stats.items())}
    log_path = out / "search_log.json"
    log_doc = {"kind": kind, "label": label, "seed": s.seed, "meta": meta, "evals": s.evals,
               "seconds": round(secs, 2), "baseline": s.base.score if s.base else None,
               "best": {"hash": best.h, "path": list(best.path), "score": best.score},
               "matched": matched, "builder_matched": s.matched_node is not None,
               "verify": vmsg, "class_stats": classes, "candidates": s.log}
    log_path.write_text(json.dumps(log_doc, indent=1, default=str))
    result = {"matched": matched, "evals": s.evals, "seconds": round(secs, 2),
              "best_score": best.score, "best_src_path": str(best_path), "log_path": str(log_path),
              "tried_mutations": len([e for e in s.log if e.get("path")]),
              "best_path": list(best.path), "full_path": full_path, "best_flags": best.flags,
              "min_evals": s.min_evals}
    if s.matched_node is not None and not matched:
        result["verify_failed"] = vmsg
    (out / "result.json").write_text(json.dumps(result, indent=1, default=str))
    return result


def run_fn(src_text, fn, flags=None, budget=2000, seconds=None, jobs=4, seed=0, out=None,
           mutator=None, catalog=None, evaluator=None, verify=None, beam=5, progress=None,
           flag_dim=None, hints=None):
    flags = flags or builder.parse_flags_comment(src_text) or builder.DEFAULT_FLAGS
    mutator = mutator or default_mutator()
    out = out or ROOT / "build" / "amatch_runs" / fn
    if flag_dim is None:
        flag_dim = evaluator is None       # only with the real compiler
    if hints is None and evaluator is None:
        try:
            hints = target_hints(builder.get_targets().get(fn))
        except Exception:                  # hints are an optimisation, never a requirement
            hints = None
    s = Searcher("fn", {"src.c": src_text}, fn=fn, flags=flags, jobs=jobs, seed=seed, budget=budget,
                 seconds=seconds, mutator=mutator, catalog=catalog, evaluator=evaluator, beam=beam,
                 progress=progress, flag_dim=flag_dim, hints=hints)
    s.run()
    verify = verify or (lambda node: verify_fn(node.files["src.c"], fn, node.flags or flags))
    return _finish(s, out, fn, "fn", {"fn": fn, "flags": flags}, verify)


def run_group(group_dir, budget=2000, seconds=None, jobs=4, seed=0, out=None, mutator=None,
              catalog=None, evaluator=None, verify=None, files=None, beam=5, progress=None):
    group_dir = Path(group_dir)
    spec = json.loads((group_dir / "group.json").read_text())
    names = list(spec["files"])
    start = {n: (group_dir / n).read_text() for n in names}
    mutable = [n for n in (files or names) if n in start]
    mutator = mutator or default_mutator()
    out = out or ROOT / "build" / "amatch_runs" / group_dir.name
    s = Searcher("group", start, group_dir=group_dir, mutable=mutable, jobs=jobs, seed=seed,
                 budget=budget, seconds=seconds, mutator=mutator, catalog=catalog,
                 evaluator=evaluator, beam=beam, progress=progress)
    s.run()
    verify = verify or (lambda node: verify_group(group_dir, node.files))
    return _finish(s, out, group_dir.name, "group",
                   {"group": str(group_dir), "claims": spec.get("claims"), "members": spec["members"],
                    "flags": spec.get("flags")}, verify)


def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    if not argv or argv[0] not in ("group", "fn", "-h", "--help"):
        argv.insert(0, "fn")
    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("--budget", type=int, default=2000, help="max compile evaluations")
    common.add_argument("--seconds", type=float, default=None, help="wall-clock limit")
    common.add_argument("--jobs", "-j", type=int, default=os.cpu_count() or 1)
    common.add_argument("--seed", type=int, default=0)
    common.add_argument("--out", default=None)
    common.add_argument("--json", action="store_true")
    common.add_argument("--beam", type=int, default=5)
    common.add_argument("--quiet", action="store_true")
    common.add_argument("--no-flag-dim", action="store_true", help="do not search -O1/-O2/-O3 variants")
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="mode", required=True)
    f = sub.add_parser("fn", parents=[common])
    f.add_argument("file")
    f.add_argument("func")
    f.add_argument("--flags", default=None)
    g = sub.add_parser("group", parents=[common])
    g.add_argument("dir")
    g.add_argument("--files", nargs="*", default=None, help="mutate only these group files")
    a = ap.parse_args(argv)

    def progress(s):
        if not a.quiet and not a.json:
            print(f"[{s.elapsed():6.1f}s eval {s.evals}] best {s.best.score if s.kind == 'fn' else s.best.key} "
                  f"path={list(s.best.path)}", file=sys.stderr)

    if a.mode == "fn":
        text = Path(a.file).read_text()
        r = run_fn(text, a.func, a.flags, a.budget, a.seconds, a.jobs, a.seed,
                   Path(a.out) if a.out else None, progress=progress, beam=a.beam,
                   flag_dim=False if a.no_flag_dim else None)
    else:
        r = run_group(a.dir, a.budget, a.seconds, a.jobs, a.seed, Path(a.out) if a.out else None,
                      files=a.files, progress=progress, beam=a.beam)
    if a.json:
        print(json.dumps(r))
    else:
        print(f"matched={r['matched']} evals={r['evals']} seconds={r['seconds']} "
              f"tried={r['tried_mutations']}\nbest_score={json.dumps(r['best_score'])}\n"
              f"best_src_path={r['best_src_path']}\nlog_path={r['log_path']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
