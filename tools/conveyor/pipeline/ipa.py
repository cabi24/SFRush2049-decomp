"""Detect functions shaped by IDO -O3 interprocedural register allocation.

    python3 -m tools.conveyor.pipeline.ipa scan      # writes build/ipa_members.json
    python3 -m tools.conveyor.pipeline.ipa report
    python3 -m tools.conveyor.pipeline.ipa groups [--cap N]   # build/ipa_groups.json

IDO -O3 lets a static callee take parameters in non-ABI registers and lets a
caller keep values in caller-save registers across a call to a callee it knows
leaves them alone. Neither can be reproduced by compiling one function at -O2,
which is all the pipeline does today. See docs/COMPILER_SETTINGS.md
("Interprocedural register allocation in game code") and
specs/010-ipa-call-groups/planning.md.

Two signatures, both read from a function's own derived assembly:

- **callee**: a register is live on entry that O32 never passes in -- a
  temporary, `$v*`, `$at`, a non-argument FP register, or a callee-saved
  register used other than being saved to the stack.
- **caller (preserve)**: a caller-save register is read after a `jal` with no
  write since that call.

A function that calls a detected callee is a member too, because its call
sites set the callee's registers. Members are excluded from -O2 permuter
searches (farm flywheel); the cheap compile-only sweep still covers them.
"""
import argparse
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
MEMBERS_JSON = REPO / "build" / "ipa_members.json"
GROUPS_JSON = REPO / "build" / "ipa_groups.json"
GROUP_CAP_INSNS = 1000

# O32 inputs: argument registers, stack/return address, FP argument registers.
ABI_INPUTS = {"a0", "a1", "a2", "a3", "sp", "ra", "zero",
              "f12", "f13", "f14", "f15", "gp"}
CALLER_SAVE = ({f"t{i}" for i in range(10)} | {"a0", "a1", "a2", "a3", "at", "v1"}
               | {f"f{i}" for i in range(4, 20)})
CALLEE_SAVE = ({f"s{i}" for i in range(8)} | {"fp", "s8"}
               | {f"f{i}" for i in range(20, 32)})

BRANCHES = {"b", "beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez",
            "beql", "bnel", "beqzl", "bnezl", "blezl", "bgtzl", "bltzl", "bgezl",
            "bc1t", "bc1f", "bc1tl", "bc1fl", "bltzal", "bgezal"}
STORES = {"sb", "sh", "sw", "swl", "swr", "swc1", "sdc1", "sd"}
READS_ONLY = STORES | BRANCHES | {"jr", "mtlo", "mthi", "teq", "tne", "break",
                                  "syscall", "cache", "sync"}
# op rs, rt -> writes hi/lo only
HILO = {"mult", "multu", "div", "divu", "dmult", "dmultu", "ddiv", "ddivu"}
# op rt, fs where the first operand is read and the second written
TO_COP1 = {"mtc1", "ctc1", "dmtc1"}
# FP compares write only the condition bit
FP_COMPARE_PREFIX = "c."

_REG = re.compile(r"\$(\w+)")
_LABEL = re.compile(r"^\s*(\.L\w+|\w+):\s*$")
_INSN = re.compile(r"^\s+([a-z][\w.]*)\s*(.*)$")


def _regs(text):
    out = []
    for r in _REG.findall(text):
        if r.isdigit():             # c1_fcsr renumbered to $31 and the like
            continue
        out.append({"fp": "s8", "s8": "s8"}.get(r, r))
    return out


def parse(asm_text):
    """[(labels, op, operand_text)] in order, one entry per instruction."""
    insns, pending = [], []
    for line in asm_text.splitlines():
        if line.startswith("glabel") or not line.strip():
            continue
        m = _LABEL.match(line)
        if m:
            pending.append(m.group(1))
            continue
        m = _INSN.match(line)
        if m:
            insns.append((pending, m.group(1), m.group(2)))
            pending = []
    return insns


def uses_defs(op, operands):
    """(reads, writes, is_call) for one instruction."""
    regs = _regs(operands)
    if op == "nop" or not regs and op not in ("jal",):
        return set(), set(), op == "jal"
    if op == "jal":
        return set(regs), {"ra", "v0", "v1", "f0", "f1", "f2", "f3"}, True
    if op == "jalr":
        return set(regs[-1:]), {"ra", "v0", "v1", "f0", "f1", "f2", "f3"}, True
    if op in READS_ONLY:
        # `sw $s0,16($sp)` reads s0; a store of a callee-saved register to the
        # stack is a save, not a use (handled by the caller of this function).
        return set(regs), set(), False
    if op in HILO:
        return set(regs), {"hi", "lo"}, False
    if op in ("mfhi", "mflo"):
        return {"hi" if op == "mfhi" else "lo"}, set(regs[:1]), False
    if op in TO_COP1:
        return set(regs[:1]), set(regs[1:2]), False
    if op.startswith(FP_COMPARE_PREFIX):
        return set(regs), {"fcc"}, False
    # Default: first register written, the rest read (loads: base register).
    return set(regs[1:]), set(regs[:1]), False


def _targets(op, operands):
    labels = re.findall(r"(\.L\w+)", operands)
    return labels if (op in BRANCHES or op == "j") else []


def analyze(asm_text):
    """{'live_in': set, 'preserved_reads': set} for one function's asm.

    live_in: registers read on some path before any write, excluding O32
    inputs and stack saves of callee-saved registers.
    preserved_reads: caller-save registers read after a call with no write
    since that call (on the straight-line path within a basic block chain).
    """
    insns = parse(asm_text)
    if not insns:
        return {"live_in": set(), "preserved_reads": set()}
    index = {}
    for i, (labels, _, _) in enumerate(insns):
        for label in labels:
            index[label] = i

    # Successors, with MIPS delay slots: a branch at i transfers after i+1.
    succ = {}
    for i, (_, op, operands) in enumerate(insns):
        nxt = []
        if op in ("jr",) and "$ra" in operands:
            nxt = []                         # return (delay slot handled below)
        elif op == "j" or op == "b":
            nxt = [index[t] for t in _targets(op, operands) if t in index]
        elif op in BRANCHES:
            nxt = [index[t] for t in _targets(op, operands) if t in index] + [i + 2]
        else:
            nxt = [i + 1]
        succ[i] = nxt
    # The delay slot instruction runs before the transfer.
    def step(i):
        _, op, _ = insns[i]
        if op in BRANCHES or op in ("j", "jr"):
            return [i + 1], succ[i]
        if op in ("jal", "jalr"):
            # The delay slot runs before the callee does.
            return [i + 1], [i + 2]
        return [], succ[i]

    # Forward dataflow to a fixpoint over (pc -> state at entry):
    #   written: registers written on EVERY path so far (meet = intersection)
    #   clobbered: caller-save registers a call may have clobbered and that
    #              no write has refreshed on SOME path (meet = union)
    ALL = None  # "unvisited" marker for written (top of the lattice)
    state = {0: (frozenset(), frozenset())}
    work = [0]
    live_in, preserved = set(), set()

    def transfer(pc, written, clobbered):
        w, cl = set(written), set(clobbered)
        delay, nexts = step(pc)
        call_writes = None
        for j in [pc] + delay:
            if j >= len(insns):
                continue
            _, op, operands = insns[j]
            reads, writes, is_call = uses_defs(op, operands)
            if op in STORES and "$sp" in operands:
                reads = {r for r in reads if r not in CALLEE_SAVE}
            for r in reads:
                if r in cl:
                    preserved.add(r)
                if r not in w and r not in ABI_INPUTS and r not in ("hi", "lo", "fcc"):
                    live_in.add(r)
            if is_call:
                call_writes = writes     # applied after the delay slot
                continue
            w |= writes
            cl -= writes
        if call_writes is not None:
            # Argument registers a call consumes are invisible in its operands.
            # If a higher argument register was freshly set (by now, including
            # the delay slot) but a lower one still holds a value from before
            # an earlier call, that lower one is passed through preserved.
            args = ["a0", "a1", "a2", "a3"]
            fresh = [a for a in args if a in w and a not in cl]
            # A float first/second argument travels in $f12/$f14 and leaves
            # the matching $a0/$a1 slot unset under O32.
            float_slots = {"a0": "f12", "a1": "f14"}
            if fresh:
                top = max(args.index(a) for a in fresh)
                for a in args[:top]:
                    fr = float_slots.get(a)
                    if fr and fr in w and fr not in cl:
                        continue
                    if a in cl:
                        preserved.add(a)
            w |= call_writes
            cl -= call_writes
            cl |= CALLER_SAVE - call_writes
            w |= CALLER_SAVE
        return nexts, frozenset(w), frozenset(cl)

    iterations = 0
    while work and iterations < 100000:
        iterations += 1
        pc = work.pop()
        if pc >= len(insns):
            continue
        written, clobbered = state[pc]
        nexts, w, cl = transfer(pc, written, clobbered)
        for n in nexts:
            if n >= len(insns):
                continue
            old = state.get(n)
            new_state = (w, cl) if old is None else (old[0] & w, old[1] | cl)
            if new_state != old:
                state[n] = new_state
                work.append(n)
    return {"live_in": live_in, "preserved_reads": preserved}


def _written_regs(asm_text):
    out = set()
    for _, op, operands in parse(asm_text):
        out |= uses_defs(op, operands)[1]
    return out


def scan(conn, asm_for):
    """Classify every gate-passed extracted target. `asm_for(target_id)` returns
    the derived asm text or None."""
    rows = conn.execute(
        "SELECT target_id,address,insn_count,gate_reason FROM n64_target"
        " WHERE population='extracted' AND address IS NOT NULL"
        " AND insn_count IS NOT NULL").fetchall()
    rows = [r for r in rows
            if not (r["gate_reason"] or "").startswith("extent_conflict")]
    callees, preservers, calls, written = {}, {}, {}, {}
    for row in rows:
        text = asm_for(row["target_id"])
        if text is None:
            continue
        result = analyze(text)
        if result["live_in"]:
            callees[row["target_id"]] = sorted(result["live_in"])
        if result["preserved_reads"]:
            preservers[row["target_id"]] = sorted(result["preserved_reads"])
        calls[row["target_id"]] = sorted(set(re.findall(r"\bjal\s+(\w+)", text)))
        written[row["target_id"]] = _written_regs(text)
    # A caller is a member only if it sets one of the special registers of an
    # IPA function it calls; merely calling one (with ABI registers) is fine,
    # and such callers do match at -O2.
    special = {t: set(r) - ABI_INPUTS for t, r in callees.items()}
    callers = {}
    for t, called in calls.items():
        hits = sorted(c for c in called
                      if c in special and special[c] & written.get(t, set()))
        if hits:
            callers[t] = hits
    members = sorted(set(callees) | set(preservers) | set(callers))
    return {"callees": callees, "preservers": preservers, "callers": callers,
            "members": members, "scanned": len(rows)}


def call_graph(conn, image_bytes, base=0x80086A50):
    """({target: set(direct callees)}, {target: insn_count}) from the image's
    jal instructions, over gate-passed extracted targets."""
    import struct
    rows = conn.execute(
        "SELECT target_id,address,insn_count,gate_reason FROM n64_target"
        " WHERE population='extracted' AND address IS NOT NULL"
        " AND insn_count IS NOT NULL").fetchall()
    rows = [r for r in rows
            if not (r["gate_reason"] or "").startswith("extent_conflict")]
    by_addr = {r["address"]: r["target_id"] for r in rows}
    calls, size = {}, {}
    for r in rows:
        off = r["address"] - base
        words = struct.unpack(f">{r['insn_count']}I",
                              image_bytes[off:off + 4 * r["insn_count"]])
        calls[r["target_id"]] = {
            by_addr.get(0x80000000 | ((w & 0x03FFFFFF) << 2))
            for w in words if w >> 26 == 3} - {None, r["target_id"]}
        size[r["target_id"]] = r["insn_count"]
    return calls, size


def discover_groups(members_doc, calls, size, cap=GROUP_CAP_INSNS):
    """Merge each IPA member's matching unit into groups of bounded size.

    A member's unit: itself; every function that sets its register parameters
    (callers), and for a caller the callee whose registers it sets (recursively);
    for a preserver, each function it calls plus that function's whole callee
    closure, because the preserved register survives only if the callee's
    clobber summary says so. Units over `cap` instructions are left out; units
    that overlap merge, and merged groups over 2 * cap are dropped."""
    def closure(x):
        seen, stack = set(), [x]
        while stack:
            for z in calls.get(stack.pop(), ()):
                if z not in seen:
                    seen.add(z)
                    stack.append(z)
        return seen

    setters = {}
    for caller, callees in members_doc["callers"].items():
        for callee in callees:
            setters.setdefault(callee, set()).add(caller)

    def unit(t, seen):
        if t in seen:
            return set()
        seen.add(t)
        u = {t} | setters.get(t, set())
        for callee in members_doc["callers"].get(t, ()):
            u |= unit(callee, seen)
        if t in members_doc["preservers"]:
            for x in calls.get(t, ()):
                u |= {x} | closure(x)
        return u

    insns = lambda u: sum(size.get(m, 0) for m in u)
    units = {t: unit(t, set()) for t in members_doc["members"]}
    kept = {t: u for t, u in units.items() if insns(u) <= cap}
    parent = {}

    def find(x):
        while parent.setdefault(x, x) != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    for t, u in kept.items():
        for m in u:
            parent[find(m)] = find(t)
    merged = {}
    for t, u in kept.items():
        merged.setdefault(find(t), set()).update(u)
    groups = []
    for members in merged.values():
        if insns(members) > 2 * cap:
            continue
        ipa_members = sorted(members & set(members_doc["members"]))
        groups.append({
            "id": ipa_members[0],
            "members": sorted(members),
            "ipa_members": ipa_members,
            "insns": insns(members),
        })
    groups.sort(key=lambda g: (g["insns"], g["id"]))
    return groups


def load_members(path=MEMBERS_JSON):
    """Set of IPA member target ids, or an empty set if no scan exists."""
    try:
        return set(json.loads(Path(path).read_text())["members"])
    except (OSError, ValueError, KeyError):
        return set()


def main():
    from ..client import DEFAULT_DATA
    from ..coordinator import db as dbmod
    from . import disasm

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(DEFAULT_DATA))
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("scan")
    sub.add_parser("report")
    g = sub.add_parser("groups")
    g.add_argument("--cap", type=int, default=GROUP_CAP_INSNS)
    args = parser.parse_args()

    if args.command == "report":
        doc = json.loads(MEMBERS_JSON.read_text())
        print(f"scanned {doc['scanned']}: {len(doc['members'])} IPA members "
              f"({len(doc['callees'])} callees, {len(doc['preservers'])} preservers, "
              f"{len(doc['callers'])} callers of callees)")
        return 0

    conn = dbmod.connect(Path(args.data) / "conveyor.db")

    if args.command == "groups":
        from . import blob_layout
        members_doc = json.loads(MEMBERS_JSON.read_text())
        calls, size = call_graph(conn, Path(blob_layout.IMAGE).read_bytes())
        groups = discover_groups(members_doc, calls, size, args.cap)
        covered = set().union(*(g["members"] for g in groups)) if groups else set()
        doc = {"cap_insns": args.cap, "groups": groups,
               "ipa_members_covered": len(covered & set(members_doc["members"])),
               "ipa_members_total": len(members_doc["members"])}
        GROUPS_JSON.write_text(json.dumps(doc, indent=1) + "\n")
        print(f"{len(groups)} groups (cap {args.cap} insns) covering "
              f"{doc['ipa_members_covered']}/{doc['ipa_members_total']} IPA members, "
              f"{len(covered)} functions -> {GROUPS_JSON}")
        return 0

    def asm_for(target_id):
        try:
            return Path(disasm.derive(conn, target_id)).read_text()
        except Exception:
            return None

    doc = scan(conn, asm_for)
    MEMBERS_JSON.parent.mkdir(parents=True, exist_ok=True)
    MEMBERS_JSON.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
    print(f"scanned {doc['scanned']}: {len(doc['members'])} IPA members "
          f"({len(doc['callees'])} callees, {len(doc['preservers'])} preservers, "
          f"{len(doc['callers'])} callers of callees) -> {MEMBERS_JSON}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
