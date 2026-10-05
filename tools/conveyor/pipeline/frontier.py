"""Call-graph frontier of the unmatched game functions.

    python3 -m tools.conveyor.pipeline.frontier scan            # writes build/frontier.json
    python3 -m tools.conveyor.pipeline.frontier report
    python3 -m tools.conveyor.pipeline.frontier next [--recipe single|group|unit] [--limit N]
    python3 -m tools.conveyor.pipeline.frontier hubs [--limit N]
    python3 -m tools.conveyor.pipeline.frontier show NAME
    python3 -m tools.conveyor.pipeline.frontier stubs [--unlocked] [--json]
    python3 -m tools.conveyor.pipeline.frontier calibrate
    python3 -m tools.conveyor.pipeline.frontier assign --agents N --per M
    (`next` and `assign` also take --exclude-file PATH, --skip-in-flight, --json)

The game image was optimized as one whole-program IDO -O3 unit and emitted
callee-first (specs/010-ipa-call-groups/research/s2-s5-spikes.md, S3). A
function's code can depend on its callees' bodies (register-clobber summaries,
inlining) and, for a register-parameter callee, on its callers. So the order
that keeps every compile in real context is bottom-up over the call graph:
match a function once everything it calls is already matched.

This module derives that order from three inputs only -- the layout
(build/blob_layout.json), the splice lock (blob_matched.lock.json) and the
image's own `jal` words -- plus the IPA scan (build/ipa_members.json) when it
exists. It reads no scores and claims nothing about how close any source is.

Per unmatched function:

- **layer**: 1 when every in-image callee is locked; otherwise one more than
  its deepest unmatched callee. Layer 1 is the frontier.
- **recipe**: how it has to be compiled to have a chance of matching.
  `single` -- not an IPA member: alone at -O2 (blob_splice).
  `group`  -- keeps values across calls, sets a callee's register
              parameters, or writes callee-saved registers it never saves: a whole-program -O3 group with its callees as
              context (blob_group). The callees are real once it is ready.
  `unit`   -- takes parameters in non-ABI registers: needs its real setters
              (callers) in the group as well, so it is worked as a joint unit.
- **unit**: the functions that have to be compiled together (itself for
  `single`; one hop of register-parameter partners otherwise), and whether
  every callee outside the unit is locked. **component** is the size of the
  transitive register-parameter closure, for scale.
- **unlocks**: unmatched callers for which this is the *only* unmatched
  callee (they become ready the moment it lands), and all transitive
  dependents.
- **evidence**: existing source for the function that is not spliced --
  a group that carries it as unclaimed context, a cloud match, a near-miss.
  Presence only; it is not a score.

Rerun `scan` after any splice, extent change or `ipa scan`.

Whole-program signatures read from the image words (no IPA scan needed).
`frontier calibrate` reprints the table below from the current lock; a
detector may change `recipe` only while it flags none of the standalone locks
(lock entries without "group"). Measured 2026-10-04, 687 locked (553
standalone, 134 group members), 529 unmatched:

    detector                          standalone   group members   unmatched
    unsaved callee-saved write           0/553         25/134        98/529
    temp ring, strict                    0/553         23/134       102/529
    temp ring, t5 arm                    0/553         23/134       135/529
    temp ring (either arm) -> group      0/553         34/134       180/529
    inlined-callee loads (hint only)     0/553          1/134         0/529

- **unsaved_callee_regs**: writes s0-s7/s8 without saving them. -> `group`.
- **temp_ring**: ugen's expression temporaries go t6,t7,t8,t9 and then start
  again at t6 (a "wrap") although t0-t5 were free. Compiled alone the ring
  continues into t5 and below once more than four temporaries are live, so a
  function that wraps without ever touching the low temporaries was compiled
  with them reserved for its callers: an internal member. Two arms, both
  clean on the lock: *strict* (>= 1 wrap, no write to t0-t5 at all) and *t5*
  (>= 3 wraps, t5 never written; t0-t4 may hold pool variables). The t5 arm
  has one standalone lock at 2 wraps (func_8009E8B4), so its threshold has a
  margin of one wrap; treat a `ring` flag from that arm alone as strong but
  not proof. -> `group`.
- **hints: inlined_callee**: >= 3 loads into ra,t5,t4,... in descending
  register order, then stores of those registers in the same order: the
  argument copies of an inlined callee (Input_ApplyPadConfig, which is
  Input_InitPadHandlers inlined). It flags no standalone lock, but exactly
  one function in the whole image (that example, since locked as a group), so
  it cannot be calibrated for recall and is shipped as an informational hint
  that does NOT change `recipe`.

**Provisional set** (cloud/work/frontier/provisional.json, tracked):
functions whose body is proven only in a group with stand-in callers. They
count as satisfied when layering and readying their callers, are never
counted as matched, stay out of `next`/`assign`, and are listed on their own
line in `report` and flagged in `show`. An entry is ignored once the function
is really locked.

**Caller-less stubs** (`stubs`): every 8-byte `jr ra; nop` function no `jal`
or tail `j` in the image reaches, with lock state, data-word references to
its address, and its address neighbours -- the lookup for "is this a deleted
static that was inlined next door?".
"""
import argparse
import json
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
LAYOUT_JSON = REPO / "build" / "blob_layout.json"
LOCKFILE = REPO / "blob_matched.lock.json"
MEMBERS_JSON = REPO / "build" / "ipa_members.json"
FRONTIER_JSON = REPO / "build" / "frontier.json"
GROUP_ROOTS = (REPO / "src" / "blob" / "groups", REPO / "cloud" / "work" / "ipa-groups")
CLOUD_MATCHES = REPO / "cloud" / "matches"
NEAR_MISS = REPO / "cloud" / "work" / "near-miss"
PROVISIONAL_JSON = REPO / "cloud" / "work" / "frontier" / "provisional.json"

ABI_INPUTS = {"a0", "a1", "a2", "a3", "sp", "ra", "zero",
              "f12", "f13", "f14", "f15", "gp"}


def functions(document):
    """[(vaddr, size, target_id)] in address order from a layout document."""
    out = [(e["vaddr"], e["size"], e["target_id"])
           for region in document["regions"] for e in region["entries"]
           if e["kind"] == "function"]
    return sorted(out)


def call_graph(funcs, image, base):
    """({target: set(in-image callees)}, {target: set(addresses outside every
    function start)}) from the image's `jal` words and out-of-extent `j`s."""
    starts = {vaddr: name for vaddr, _, name in funcs}
    calls, external = {}, {}
    for vaddr, size, name in funcs:
        off = vaddr - base
        words = struct.unpack(f">{size // 4}I", image[off:off + size - size % 4])
        inside, outside = set(), set()
        for w in words:
            op = w >> 26
            if op not in (2, 3):
                continue
            target = (vaddr & 0xF0000000) | ((w & 0x03FFFFFF) << 2)
            if op == 2 and vaddr <= target < vaddr + size:
                continue                    # a jump inside the function
            if target in starts:
                inside.add(starts[target])
            elif op == 3:
                outside.add(target)
        calls[name] = inside - {name}
        external[name] = outside
    return calls, external


def unsaved_map(funcs, image, base):
    out = {}
    for vaddr, size, name in funcs:
        off = vaddr - base
        regs = unsaved_callee_writes(
            struct.unpack(f">{size // 4}I", image[off:off + size - size % 4]))
        if regs:
            out[name] = regs
    return out


# Opcodes whose rt field is a written integer register.
_WRITES_RT = ({8, 9, 10, 11, 12, 13, 14, 15, 24, 25}          # immediates, lui
              | {32, 33, 34, 35, 36, 37, 38, 39, 55})         # loads
_NO_RD = {8, 9, 16, 17, 18, 19, 24, 25, 26, 27, 12, 13}       # jr/jalr handled, hi/lo, mult/div
_CALLEE_SAVED = set(range(16, 24)) | {30}
_REG_NAMES = {**{16 + i: f"s{i}" for i in range(8)}, 30: "s8"}


def unsaved_callee_writes(words):
    """Callee-saved integer registers a function writes without storing them
    to its frame first: legal only when whole-program -O3 knows no caller
    needs them, so the function cannot match compiled alone."""
    written, saved = set(), set()
    for w in words:
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        if op == 0:
            if (w & 63) not in _NO_RD and w != 0:
                written.add(rd)
        elif op in _WRITES_RT:
            written.add(rt)
        elif op == 17 and rs in (0, 2):                 # mfc1 / cfc1
            written.add(rt)
        elif op in (43, 63) and rs == 29:               # sw / sd reg, off(sp)
            saved.add(rt)
    return sorted(_REG_NAMES[r] for r in (written & _CALLEE_SAVED) - saved)


def _words(image, base, vaddr, size):
    off = vaddr - base
    return struct.unpack(f">{size // 4}I", image[off:off + size - size % 4])


def _int_writes(words):
    """[(word index, integer register written)] in address order."""
    out = []
    for i, w in enumerate(words):
        op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        if op == 0:
            if (w & 63) not in _NO_RD and w != 0:
                out.append((i, rd))
        elif op in _WRITES_RT:
            out.append((i, rt))
        elif op == 17 and rs in (0, 2):                 # mfc1 / cfc1
            out.append((i, rt))
    return out


def _is_branch_likely(w):
    op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
    return (op in (20, 21, 22, 23)                      # beql bnel blezl bgtzl
            or (op == 1 and rt in (2, 3, 18, 19))       # bltzl bgezl (+al)
            or (op == 17 and rs == 8 and rt & 2))       # bc1fl bc1tl


_RING = (14, 15, 24, 25)                                # t6 t7 t8 t9
_LOW_TEMPS = {8 + i: f"t{i}" for i in range(6)}         # t0..t5
RING_STRICT_WRAPS = 1       # with no t0-t5 write at all
RING_T5_WRAPS = 3           # with t5 never written


def temp_ring(words):
    """{'writes', 'wraps', 'low_written'}: writes to t6-t9 in address order,
    how often a t9 write is directly followed by a t6 write, and which of
    t0-t5 the function writes anywhere. A branch-likely delay slot repeats the
    instruction at its target, so those slots are left out of the sequence."""
    skip = {i + 1 for i, w in enumerate(words) if _is_branch_likely(w)}
    writes = _int_writes(words)
    ring = [r for i, r in writes if r in _RING and i not in skip]
    low = sorted({_LOW_TEMPS[r] for _, r in writes if r in _LOW_TEMPS})
    wraps = sum(1 for a, b in zip(ring, ring[1:]) if a == 25 and b == 14)
    return {"writes": len(ring), "wraps": wraps, "low_written": low}


def ring_arms(info):
    """Names of the calibrated arms a temp_ring() result satisfies."""
    arms = []
    if info["wraps"] >= RING_STRICT_WRAPS and not info["low_written"]:
        arms.append("strict")
    if info["wraps"] >= RING_T5_WRAPS and "t5" not in info["low_written"]:
        arms.append("t5")
    return arms


_INLINE_POOL = (31, 13, 12, 11, 10, 9, 8)               # ra t5 t4 t3 t2 t1 t0
_INLINE_NAMES = {31: "ra", **_LOW_TEMPS}
INLINE_MIN_RUN = 3
_INLINE_GAP = 2


def inlined_callee_runs(words):
    """[(word index, [registers])]: runs of >= INLINE_MIN_RUN loads into
    ra,t5,t4,... in strictly descending register order (at most _INLINE_GAP
    other instructions between two loads), followed by non-stack stores of
    exactly those registers in the same order."""
    def load_reg(w):
        rt = (w >> 16) & 31
        return rt if 32 <= w >> 26 <= 39 and rt in _INLINE_POOL else None

    out, i, n = [], 0, len(words)
    while i < n:
        first = load_reg(words[i])
        if first is None:
            i += 1
            continue
        seq, j, gap, end = [first], i + 1, 0, i + 1
        while j < n and gap <= _INLINE_GAP:
            reg = load_reg(words[j])
            if reg is None:
                gap += 1
            elif reg < seq[-1]:
                seq.append(reg)
                gap, end = 0, j + 1
            else:
                break
            j += 1
        if len(seq) >= INLINE_MIN_RUN:
            window = words[end:end + 3 * len(seq) + 8]
            stored = [(w >> 16) & 31 for w in window
                      if w >> 26 in (40, 41, 43) and (w >> 21) & 31 != 29
                      and (w >> 16) & 31 in seq]
            if stored[:len(seq)] == seq:
                out.append((i, [_INLINE_NAMES[r] for r in seq]))
        i = end
    return out


def signature_maps(funcs, image, base):
    """({name: temp_ring() + 'arms'} for flagged functions,
        {name: [hint strings]}) over every layout function."""
    rings, hints = {}, {}
    for vaddr, size, name in funcs:
        words = _words(image, base, vaddr, size)
        info = temp_ring(words)
        arms = ring_arms(info)
        # Only the strict arm changes the recipe. The t5 arm stopped being
        # clean on 2026-10-05 (func_8010C2E4, a standalone match, has 4 wraps
        # with t5 unwritten), so it is reported as a hint.
        if "strict" in arms:
            rings[name] = dict(info, arms=arms)
        elif arms:
            hints.setdefault(name, []).append(
                f"temp_ring_t5@{info['wraps']}wraps")
        runs = inlined_callee_runs(words)
        if runs:
            hints.setdefault(name, []).extend(
                f"inlined_callee@0x{vaddr + 4 * i:08X}:{','.join(regs)}"
                for i, regs in runs)
    return rings, hints


STUB_WORDS = (0x03E00008, 0)                            # jr ra; nop


def stubs(funcs, image, base, calls, matched):
    """Caller-less `jr ra; nop` functions with their address neighbours.

    A stub with no `jal`/tail-`j` reaching it is what whole-program -O3
    leaves of a deleted internal procedure; the body it used to have is
    usually inlined into a neighbour. `data_refs` counts image words outside
    every function that equal the stub's address (a function-pointer table
    would keep an empty function alive for a different reason)."""
    called = set()
    for callees in calls.values():
        called |= set(callees)
    ordered = sorted(funcs)
    spans = [(v, v + s) for v, s, _ in ordered]
    wanted = {v for v, s, n in ordered
              if s == 8 and n not in called and _words(image, base, v, s) == STUB_WORDS}
    refs = dict.fromkeys(wanted, 0)
    if wanted:
        span_i = 0
        for off in range(0, len(image) - len(image) % 4, 4):
            addr = base + off
            while span_i < len(spans) and spans[span_i][1] <= addr:
                span_i += 1
            if span_i < len(spans) and spans[span_i][0] <= addr:
                continue                                # inside a function
            word = struct.unpack_from(">I", image, off)[0]
            if word in refs:
                refs[word] += 1

    def side(index):
        if not 0 <= index < len(ordered):
            return None
        v, s, n = ordered[index]
        return {"name": n, "address": f"0x{v:08X}", "size": s,
                "locked": n in matched, "stub": v in wanted}

    out = []
    for index, (v, s, n) in enumerate(ordered):
        if v not in wanted:
            continue
        prev, nxt = side(index - 1), side(index + 1)
        if prev is not None:
            prev["adjacent"] = ordered[index - 1][0] + ordered[index - 1][1] == v
        if nxt is not None:
            nxt["adjacent"] = v + s == ordered[index + 1][0]
        out.append({"name": n, "address": f"0x{v:08X}", "locked": n in matched,
                    "data_refs": refs[v], "prev": prev, "next": nxt})
    return out


def load_provisional(path=PROVISIONAL_JSON):
    """{name: {"evidence": path, "note": text}}; empty when the file is
    missing or unreadable (the frontier must keep working without it)."""
    try:
        doc = json.loads(Path(path).read_text())
    except (OSError, ValueError):
        return {}
    return {k: v for k, v in doc.items()
            if isinstance(v, dict) and not k.startswith("_")}


def in_flight(matched, cloud_matches=CLOUD_MATCHES):
    """Names with a cloud/matches/NAME.c that are not locked yet."""
    return {p.stem for p in Path(cloud_matches).glob("*.c")} - set(matched)


def read_names(path):
    """Names from a one-per-line file; blank lines and #-comments ignored,
    and only the first field of a line counts (a `next` listing works)."""
    out = set()
    for line in Path(path).read_text().splitlines():
        line = line.split("#", 1)[0].strip()
        if line:
            out.add(line.split()[0])
    return out


def _closure(graph, start):
    seen, stack = set(), [start]
    while stack:
        for nxt in graph.get(stack.pop(), ()):
            if nxt not in seen:
                seen.add(nxt)
                stack.append(nxt)
    return seen


def analyze(calls, size, matched, ipa=None, unsaved=None, provisional=None,
            rings=None, hints=None):
    """Frontier facts for every unmatched function.

    calls: {name: set(callees)}; size: {name: bytes}; matched: set of locked
    names; ipa: an ipa_members.json document or None; unsaved: {name:
    [callee-saved registers written without a save]}; provisional: names
    proven with stand-in callers only (satisfied as callees, still unmatched);
    rings: {name: temp_ring() result} for functions the ring detector flags;
    hints: {name: [informational strings]} (never changes a recipe).
    Returns {name: {...}} for the unmatched functions only."""
    ipa = ipa or {}
    unsaved = unsaved or {}
    rings = rings or {}
    hints = hints or {}
    provisional = {n for n in (provisional or ()) if n in calls and n not in matched}
    satisfied = set(matched) | provisional
    callee_regs = {t: sorted(set(r) - ABI_INPUTS)
                   for t, r in ipa.get("callees", {}).items()}
    callee_regs = {t: r for t, r in callee_regs.items() if r}
    preservers = ipa.get("preservers", {})
    setter_of = ipa.get("callers", {})          # caller -> [reg-param callees]
    setters = {}
    for caller, callees in setter_of.items():
        for callee in callees:
            setters.setdefault(callee, set()).add(caller)

    unmatched = [n for n in calls if n not in matched]
    pending = {n: calls[n] - satisfied for n in unmatched}

    # Layers: longest chain of unmatched callees below a function.
    layer = {}

    def depth(n, trail=()):
        if n in layer:
            return layer[n]
        if n in trail:                      # recursion cycle: break it here
            return 0
        below = [depth(c, trail + (n,)) for c in pending[n] if c in pending]
        layer[n] = 1 + max(below, default=0)
        return layer[n]

    sys.setrecursionlimit(max(sys.getrecursionlimit(), 10000))
    for n in unmatched:
        depth(n)

    callers = {}
    for n, cs in calls.items():
        for c in cs:
            callers.setdefault(c, set()).add(n)

    def unit(n):
        """Functions compiled together with `n`: a register-parameter callee
        with its setters, a setter with the callees whose registers it sets.
        One hop only -- S2 found a member's code depends on its partners
        existing, not on their bodies, so partners of partners stay out."""
        return {n} | set(setter_of.get(n, ())) | setters.get(n, set())

    def component(n):
        """The transitive version: everything tied together by register
        parameters. Its size says how far a small group is from the real
        whole-program compile."""
        seen, stack = {n}, [n]
        while stack:
            x = stack.pop()
            for y in (set(setter_of.get(x, ())) | setters.get(x, set())) - seen:
                seen.add(y)
                stack.append(y)
        return seen

    out = {}
    for n in unmatched:
        members = unit(n)
        open_members = sorted(m for m in members if m not in matched)
        outside = set()
        for m in open_members:
            outside |= calls.get(m, set()) - members - satisfied
        if n in callee_regs:
            recipe = "unit"
        elif (n in preservers or n in setter_of or len(members) > 1
              or unsaved.get(n) or n in rings):
            recipe = "group"
        else:
            recipe = "single"
        waiting = [c for c in callers.get(n, ()) if c not in matched]
        sole = sorted(c for c in waiting if pending[c] == {n})
        dependents = {d for d in _closure(callers, n) if d not in matched}
        out[n] = {
            "size": size[n],
            "layer": layer[n],
            "recipe": recipe,
            "ready": not pending[n],
            "blockers": sorted(pending[n]),
            "callees": sorted(calls[n]),
            "register_params": callee_regs.get(n, []),
            "preserved": preservers.get(n, []),
            "unsaved_callee_regs": unsaved.get(n, []),
            "temp_ring": rings.get(n),
            "signatures": ([s for s, hit in (
                ("register_params", n in callee_regs),
                ("preserved", n in preservers),
                ("sets_register_params", n in setter_of),
                ("unsaved_callee_regs", bool(unsaved.get(n))),
                ("temp_ring", n in rings)) if hit]),
            "hints": list(hints.get(n, [])),
            "provisional": n in provisional,
            "provisional_callees": sorted(calls[n] & provisional),
            "unit": sorted(members),
            "unit_open": open_members,
            "unit_open_bytes": sum(size.get(m, 0) for m in open_members),
            "unit_blockers": sorted(outside),
            "unit_ready": not outside,
            "component": len(component(n)),
            "unmatched_callers": len(waiting),
            "sole_blocker_for": sole,
            "sole_unlock_bytes": sum(size[c] for c in sole),
            "dependents": len(dependents),
            "dependent_bytes": sum(size[d] for d in dependents),
        }
    return out


def evidence(names, group_roots=GROUP_ROOTS, cloud_matches=CLOUD_MATCHES,
             near_miss=NEAR_MISS):
    """{name: [repo-relative paths]} of unspliced source that mentions being
    this function. Existence only."""
    found = {}

    def add(name, path):
        try:
            path = path.relative_to(REPO)
        except ValueError:
            pass
        found.setdefault(name, []).append(str(path))

    wanted = set(names)
    for root in group_roots:
        for spec_path in sorted(Path(root).glob("*/group.json")):
            try:
                spec = json.loads(spec_path.read_text())
            except (OSError, ValueError):
                continue
            for key in ("members", "context"):
                for name in spec.get(key, ()) or ():
                    if name in wanted:
                        add(name, spec_path.parent)
    for name in wanted:
        match = Path(cloud_matches) / f"{name}.c"
        if match.exists():
            add(name, match)
        near = Path(near_miss) / name
        if near.exists():
            add(name, near)
    return {n: sorted(set(p)) for n, p in found.items()}


def layer_table(facts):
    """[(layer, functions, bytes)] ascending."""
    rows = {}
    for f in facts.values():
        count, total = rows.get(f["layer"], (0, 0))
        rows[f["layer"]] = (count + 1, total + f["size"])
    return [(k, *rows[k]) for k in sorted(rows)]


def _is_ready(f):
    return f["unit_ready"] if f["recipe"] == "unit" else f["ready"]


def queue(facts, recipe=None, min_bytes=0, exclude=()):
    """Ready work, best first: own bytes plus the bytes it alone unblocks.
    Provisional functions are left out (their body exists; they close when
    their callers land), as is anything named in `exclude`."""
    exclude = set(exclude)
    rows = [(n, f) for n, f in facts.items()
            if _is_ready(f) and f["size"] >= min_bytes
            and not f.get("provisional") and n not in exclude
            and (recipe is None or f["recipe"] == recipe)]
    rows.sort(key=lambda r: (-(r[1]["size"] + r[1]["sole_unlock_bytes"]), r[0]))
    return rows


_RECIPE_ORDER = {"unit": 0, "group": 1, "single": 2}


def bundles(facts, exclude=()):
    """Ready work as indivisible bundles, best first.

    A `unit` or `group` function travels with the open members of its unit
    (register-parameter partners); overlapping units merge. A bundle with an
    excluded member is dropped whole. Each bundle is {"names": ready-queue
    members in rank order, "with": other open partners that have to be
    written in the same group (provisional ones included), "recipe": the
    strongest recipe present, "functions": count of non-provisional members,
    "bytes": their size, "value": summed rank of the queue members}."""
    exclude = set(exclude)
    rows = queue(facts)
    rank = {n: i for i, (n, _) in enumerate(rows)}
    parent = {}

    def find(x):
        while parent.setdefault(x, x) != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    for n, f in rows:
        find(n)
        if f["recipe"] != "single":
            for m in f["unit_open"]:
                parent[find(m)] = find(n)
    grouped = {}
    for m in list(parent):
        grouped.setdefault(find(m), set()).add(m)
    out = []
    for members in grouped.values():
        if members & exclude:
            continue
        names = sorted((m for m in members if m in rank), key=rank.get)
        if not names:
            continue
        work = [m for m in members if not facts[m].get("provisional")]
        out.append({
            "names": names,
            "with": sorted(members - set(names)),
            "recipe": min((facts[m]["recipe"] for m in names), key=_RECIPE_ORDER.get),
            "functions": len(work),
            "bytes": sum(facts[m]["size"] for m in work),
            "value": sum(facts[m]["size"] + facts[m]["sole_unlock_bytes"] for m in names),
            "_rank": rank[names[0]],
        })
    out.sort(key=lambda b: b["_rank"])
    for b in out:
        del b["_rank"]
    return out


def assign(facts, agents, per, exclude=()):
    """(batches, skipped): `agents` disjoint lists of bundles, each holding at
    most `per` functions. Bundles are taken best first until the batches are
    full, then dealt recipe by recipe (units, groups, singles) to the batch
    with the fewest functions so far, so every agent gets a share of each
    recipe and of the high-value work. A bundle is never split; one larger
    than `per` is returned in `skipped`."""
    chosen, skipped, budget = [], [], agents * per
    for b in bundles(facts, exclude):
        if b["functions"] > per:
            skipped.append(b)
        elif b["functions"] <= budget:
            chosen.append(b)
            budget -= b["functions"]
    batches = [[] for _ in range(agents)]
    load = [0] * agents
    order = sorted(range(len(chosen)),
                   key=lambda i: (_RECIPE_ORDER[chosen[i]["recipe"]], i))
    turn = 0
    for i in order:
        b = chosen[i]
        fits = [a for a in range(agents) if load[a] + b["functions"] <= per]
        if not fits:
            skipped.append(b)
            continue
        a = min(fits, key=lambda a: (load[a], (a - turn) % agents))
        batches[a].append(b)
        load[a] += b["functions"]
        turn = (a + 1) % agents
    return batches, skipped


def calibrate(words_of, lock):
    """[(detector, changes_recipe, standalone hits, group hits, unmatched
    hits, [standalone names])] against the current lock. words_of: {name:
    words}; lock: the splice lock document."""
    standalone = [n for n in words_of if n in lock and "group" not in lock[n]]
    grouped = [n for n in words_of if n in lock and "group" in lock[n]]
    unmatched = [n for n in words_of if n not in lock]
    info = {n: temp_ring(w) for n, w in words_of.items()}
    detectors = (
        ("unsaved callee-saved write", True,
         lambda n: bool(unsaved_callee_writes(words_of[n]))),
        ("temp ring, strict", True, lambda n: "strict" in ring_arms(info[n])),
        ("temp ring, t5 arm", False, lambda n: "t5" in ring_arms(info[n])),
        ("temp ring (either arm)", False, lambda n: bool(ring_arms(info[n]))),
        ("inlined-callee loads", False,
         lambda n: bool(inlined_callee_runs(words_of[n]))),
    )
    rows = []
    for label, changes, hit in detectors:
        false = sorted(n for n in standalone if hit(n))
        rows.append((label, changes, len(false), sum(map(hit, grouped)),
                     sum(map(hit, unmatched)), false))
    return rows, (len(standalone), len(grouped), len(unmatched))


def hubs(facts):
    """Unmatched functions ranked by the unmatched bytes that depend on them."""
    rows = [(n, f) for n, f in facts.items() if f["dependents"]]
    rows.sort(key=lambda r: (-r[1]["dependent_bytes"], r[0]))
    return rows


def build(layout_path=LAYOUT_JSON, lockfile=LOCKFILE, members_path=MEMBERS_JSON,
          provisional_path=PROVISIONAL_JSON):
    document = json.loads(Path(layout_path).read_text())
    funcs = functions(document)
    image_path = Path(document["image"]["path"])
    if not image_path.is_absolute():
        image_path = REPO / image_path
    base = int(document["image"]["base"], 16)
    image = image_path.read_bytes()
    calls, external = call_graph(funcs, image, base)
    unsaved = unsaved_map(funcs, image, base)
    rings, hints = signature_maps(funcs, image, base)
    size = {name: s for _, s, name in funcs}
    address = {name: v for v, _, name in funcs}
    matched = set(json.loads(Path(lockfile).read_text())) & set(size)
    provisional = {n: v for n, v in load_provisional(provisional_path).items()
                   if n in size and n not in matched}
    stub_rows = stubs(funcs, image, base, calls, matched)
    try:
        ipa = json.loads(Path(members_path).read_text())
    except (OSError, ValueError):
        ipa = None
    facts = analyze(calls, size, matched, ipa, unsaved, provisional, rings, hints)
    found = evidence(facts)
    for name, f in facts.items():
        f["address"] = f"0x{address[name]:08X}"
        f["external_calls"] = sorted(f"0x{t:08X}" for t in external[name])
        f["evidence"] = found.get(name, [])
        if name in provisional:
            f["provisional_evidence"] = provisional[name].get("evidence")
            f["provisional_note"] = provisional[name].get("note")
    return {
        "image_sha256": document["image"].get("sha256"),
        "ipa_scan": ipa is not None,
        "functions": len(size),
        "matched": len(matched),
        "unmatched": len(facts),
        "unmatched_bytes": sum(f["size"] for f in facts.values()),
        "layers": [list(row) for row in layer_table(facts)],
        "provisional": sorted(provisional),
        "provisional_bytes": sum(size[n] for n in provisional),
        "stubs": stub_rows,
        "in_flight": sorted(in_flight(matched) & set(facts)),
        "facts": facts,
    }


def _summary(doc):
    facts = doc["facts"]
    lines = [f"{doc['unmatched']} unmatched game functions, {doc['unmatched_bytes']} bytes"
             f" ({doc['matched']}/{doc['functions']} locked)"]
    if not doc["ipa_scan"]:
        lines.append("no build/ipa_members.json: every function is reported as `single`")
    lines.append("layer  functions    bytes")
    for layer, count, total in doc["layers"]:
        lines.append(f"{layer:5d}  {count:9d}  {total:7d}")
    lines.append("ready now, by recipe:")
    for recipe in ("single", "group", "unit"):
        rows = queue(facts, recipe)
        lines.append(f"  {recipe:6s} {len(rows):4d} functions  "
                     f"{sum(f['size'] for _, f in rows):7d} bytes")
    names = doc.get("provisional", [])
    lines.append(f"provisional (stand-in proof; satisfied as callees, NOT matched): "
                 f"{len(names)} functions  {doc.get('provisional_bytes', 0)} bytes"
                 + (f"\n  {', '.join(names)}" if names else ""))
    ring = [n for n, f in facts.items() if f.get("temp_ring")]
    ring_only = [n for n in ring if facts[n]["signatures"] == ["temp_ring"]]
    lines.append(f"signatures among unmatched: "
                 f"unsaved callee-saved {sum(1 for f in facts.values() if f['unsaved_callee_regs'])}, "
                 f"temp ring {len(ring)} ({len(ring_only)} with no other signature), "
                 f"inlined-callee hints {sum(1 for f in facts.values() if f.get('hints'))}")
    stub_rows = doc.get("stubs", [])
    lines.append(f"caller-less jr-ra stubs: {len(stub_rows)} "
                 f"({sum(1 for s in stub_rows if s['locked'])} locked); `frontier stubs`")
    return "\n".join(lines)


def _row(name, f):
    notes = []
    if f["register_params"]:
        notes.append("regs=" + ",".join(f["register_params"]))
    if len(f["unit_open"]) > 1:
        notes.append(f"unit={len(f['unit_open'])}fn/{f['unit_open_bytes']}b")
    if f["component"] > len(f["unit"]):
        notes.append(f"component={f['component']}")
    if f["evidence"]:
        notes.append("src")
    if f.get("temp_ring"):
        notes.append("ring")
    if f.get("hints"):
        notes.append("inline?")
    if f.get("provisional"):
        notes.append("PROVISIONAL")
    elif f.get("provisional_callees"):
        notes.append(f"prov_callees={len(f['provisional_callees'])}")
    return (f"{name:32s} {f['address']} {f['size']:6d} L{f['layer']:<2d} {f['recipe']:6s} "
            f"unlocks={len(f['sole_blocker_for']):2d}/{f['sole_unlock_bytes']:<6d} "
            f"deps={f['dependents']:3d}/{f['dependent_bytes']:<6d} {' '.join(notes)}")


def _queue_filters(parser):
    parser.add_argument("--exclude-file", action="append", default=[],
                        metavar="PATH", help="names to skip, one per line")
    parser.add_argument("--skip-in-flight", action="store_true",
                        help="skip names with a cloud/matches/NAME.c not yet locked")
    parser.add_argument("--json", action="store_true")


def _excluded(args, doc):
    names = set()
    for path in args.exclude_file:
        names |= read_names(path)
    if args.skip_in_flight:
        names |= set(doc["in_flight"])
    return names


def _stub_side(side):
    if side is None:
        return "-"
    state = "locked" if side["locked"] else "UNMATCHED"
    extra = " stub" if side["stub"] else ""
    gap = "" if side["adjacent"] else " (gap)"
    return f"{side['name']} {side['size']}b {state}{extra}{gap}"


def _calibration():
    document = json.loads(LAYOUT_JSON.read_text())
    image_path = Path(document["image"]["path"])
    if not image_path.is_absolute():
        image_path = REPO / image_path
    base, image = int(document["image"]["base"], 16), image_path.read_bytes()
    words_of = {n: _words(image, base, v, s) for v, s, n in functions(document)}
    return calibrate(words_of, json.loads(LOCKFILE.read_text()))


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("scan")
    sub.add_parser("report")
    n = sub.add_parser("next")
    n.add_argument("--recipe", choices=("single", "group", "unit"))
    n.add_argument("--min-bytes", type=int, default=0)
    n.add_argument("--limit", type=int, default=40)
    for p in (n,):
        _queue_filters(p)
    a = sub.add_parser("assign", help="disjoint batches of ready work")
    a.add_argument("--agents", type=int, required=True)
    a.add_argument("--per", type=int, required=True)
    _queue_filters(a)
    st = sub.add_parser("stubs", help="caller-less jr-ra stubs and neighbours")
    st.add_argument("--unlocked", action="store_true")
    st.add_argument("--json", action="store_true")
    sub.add_parser("calibrate", help="detector hit rates against the lock")
    h = sub.add_parser("hubs")
    h.add_argument("--limit", type=int, default=30)
    s = sub.add_parser("show")
    s.add_argument("name")
    args = parser.parse_args()

    if args.command == "scan":
        doc = build()
        FRONTIER_JSON.parent.mkdir(parents=True, exist_ok=True)
        FRONTIER_JSON.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
        print(_summary(doc))
        print(f"-> {FRONTIER_JSON}")
        return 0

    # Reports always reflect the current lock; the JSON is for other tools.
    doc = build()
    facts = doc["facts"]
    if args.command == "report":
        print(_summary(doc))
    elif args.command == "next":
        rows = queue(facts, args.recipe, args.min_bytes,
                     _excluded(args, doc))[:args.limit]
        if args.json:
            print(json.dumps([dict(f, name=name) for name, f in rows],
                             indent=1, sort_keys=True))
        else:
            for name, f in rows:
                print(_row(name, f))
    elif args.command == "assign":
        batches, skipped = assign(facts, args.agents, args.per, _excluded(args, doc))
        if args.json:
            print(json.dumps({"batches": batches, "skipped": skipped},
                             indent=1, sort_keys=True))
        else:
            for index, batch in enumerate(batches, 1):
                print(f"== agent {index}: {sum(b['functions'] for b in batch)} functions, "
                      f"{sum(b['bytes'] for b in batch)} bytes")
                for b in batch:
                    for name in b["names"]:
                        print(_row(name, facts[name]))
                    for name in b["with"]:
                        print(f"  + {name} (same {b['recipe']} as {b['names'][0]}, "
                              f"{facts[name]['size']}b"
                              f"{', provisional' if facts[name].get('provisional') else ''})")
            for b in skipped:
                print(f"skipped (does not fit --per {args.per}): "
                      f"{', '.join(b['names'] + b['with'])}", file=sys.stderr)
    elif args.command == "stubs":
        rows = [s for s in doc["stubs"] if not (args.unlocked and s["locked"])]
        if args.json:
            print(json.dumps(rows, indent=1, sort_keys=True))
        else:
            for s in rows:
                print(f"{s['name']:20s} {s['address']} "
                      f"{'locked   ' if s['locked'] else 'UNMATCHED'} "
                      f"data_refs={s['data_refs']}  prev: {_stub_side(s['prev'])}"
                      f"  next: {_stub_side(s['next'])}")
            print(f"{len(rows)} caller-less stubs "
                  f"({sum(1 for s in rows if s['locked'])} locked, "
                  f"{sum(1 for s in rows if s['data_refs'])} with a data-word reference)")
    elif args.command == "calibrate":
        rows, (n_single, n_group, n_open) = _calibration()
        print(f"{'detector':32s} {'recipe':7s} standalone  group members  unmatched")
        for label, changes, single, group, unmatched, names in rows:
            print(f"{label:32s} {'group' if changes else 'hint':7s} "
                  f"{single:4d}/{n_single:<5d} {group:6d}/{n_group:<6d} "
                  f"{unmatched:5d}/{n_open}"
                  + (f"   standalone hits: {', '.join(names)}" if names else ""))
    elif args.command == "hubs":
        for name, f in hubs(facts)[:args.limit]:
            print(_row(name, f) + ("" if f["ready"] else
                                   f" blocked_by={len(f['blockers'])}"))
    elif args.command == "show":
        f = facts.get(args.name)
        if f is None:
            print(f"{args.name}: locked, or not a layout function", file=sys.stderr)
            return 1
        print(_row(args.name, f))
        for key in ("blockers", "callees", "external_calls", "register_params",
                    "preserved", "unsaved_callee_regs", "unit", "unit_open", "unit_blockers",
                    "sole_blocker_for", "evidence", "signatures", "hints",
                    "provisional_callees"):
            print(f"  {key}: {', '.join(f[key]) or '-'}")
        ring = f.get("temp_ring")
        print("  temp_ring: " + (f"{ring['wraps']} wraps in {ring['writes']} t6-t9 writes, "
                                 f"low temps written: {','.join(ring['low_written']) or 'none'}"
                                 f" (arms: {','.join(ring['arms'])})" if ring else "-"))
        if f.get("provisional"):
            print(f"  provisional: yes -- proven with stand-in callers only, not matched"
                  f"\n    evidence: {f.get('provisional_evidence') or '-'}"
                  f"\n    note: {f.get('provisional_note') or '-'}")
        else:
            print("  provisional: -")
        near = [s for s in doc["stubs"]
                if args.name in ((s["prev"] or {}).get("name"), (s["next"] or {}).get("name"))]
        print("  stub_neighbours: " + (", ".join(
            f"{s['name']}@{s['address']}({'locked' if s['locked'] else 'unmatched'})"
            for s in near) or "-"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
