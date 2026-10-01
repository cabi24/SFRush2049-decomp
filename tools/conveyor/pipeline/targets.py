"""N64 target inventory: populate n64_target rows and produce per-function
target objects (.o) for the Scorer.

Sources:
- work/**/info.txt          — name, address, category, flags (1,319 functions)
- build/game_code.bin       — decompressed game code (RAM 0x80086A50+), the
                              752-function "extracted" population
- baserom.us.z64            — static population; ROM offset = vaddr - 0x7FFFF400

Target .o files come in two tiers (003, specs/003-reloc-aware-targets):

- `reloc_aware`: static targets assembled from their splat asm region, which
  carries `%hi/%lo/jal` symbol operands, so the object carries real
  relocations. Gated by a per-target round-trip check (masked-word equality
  against the ROM) before it may replace the raw-word object.
- `raw_word`: everything else — dynamic game-code targets, and any static
  target whose region is missing, won't assemble, or fails the gate. Built by
  assembling the raw instruction words (`.word`) so no disassembler round-trip
  can distort them (V1 conservatism, preserved as the fallback).

`n64_target.target_o_sha` points at the chosen object; `tier`/`gate_reason`
record which path it took. When the object bytes change, the target's derived
`matrix_entry` evidence is superseded (purged) in the same transaction.
"""
import dataclasses
import hashlib
import os
import re
import struct
import subprocess
import sys
import tempfile
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
GAME_CODE_BASE = 0x80086A50
GAME_CODE_BIN = REPO / "build" / "game_code.bin"
BASEROM = REPO / "baserom.us.z64"
STATIC_ROM_DELTA = 0x7FFFF400  # vaddr - delta = ROM offset (verified: strlen)
ASM_DIR = REPO / "asm" / "us"

EXTENT_REPORT_TARGETS = (
    "game_loop", "game_mode_handler", "attract_or_transition", "process_inputs",
    "sound_control", "playgame_state_change", "RaceStateMachine_Update",
    "countdown", "countdown_handler", "Input_ProcessGameplayPad",
)

_SIZE_RE = re.compile(r"\((\d+)\s*bytes\)")

# A splat instruction line: `/* <off> <vaddr> <word> */  <mnemonic ...>`.
# We key regions by <vaddr> and gate against <word>.
_REGION_LINE_RE = re.compile(
    r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/"
)
_GLABEL_RE = re.compile(r"^\s*glabel\s+(\S+)")
_ENDLABEL_RE = re.compile(r"^\s*endlabel\s+(\S+)")


def load_work_inventory(work_dir=None):
    """Parse every work/**/info.txt into {name, address, category, flags, size?}."""
    work_dir = Path(work_dir or REPO / "work")
    entries = []
    for info in sorted(work_dir.rglob("info.txt")):
        fields = {}
        for line in info.read_text().splitlines():
            if ":" in line:
                key, _, value = line.partition(":")
                fields[key.strip()] = value.strip()
        if "address" not in fields or "name" not in fields:
            continue
        size = None
        m = _SIZE_RE.search(fields.get("comment", ""))
        if m:
            size = int(m.group(1))
        entries.append(
            {
                "name": fields["name"],
                "address": int(fields["address"], 16),
                "category": fields.get("category", ""),
                "flags": fields.get("compiler_flags", ""),
                "size": size,
            }
        )
    # Dedup by address (first name wins), then infer missing sizes from the
    # gap to the next function.
    by_addr = {}
    for e in entries:
        by_addr.setdefault(e["address"], e)
    ordered = sorted(by_addr.values(), key=lambda e: e["address"])
    for i, e in enumerate(ordered):
        gap = ordered[i + 1]["address"] - e["address"] if i + 1 < len(ordered) else None
        if e["size"] is None:
            e["size"] = gap if gap is not None else 256
        elif gap is not None:
            e["size"] = min(e["size"], gap)
    # target_id (name) is the n64_target primary key, but a handful of game-code
    # functions share a heuristic name at different addresses. Collapse to one
    # entry per name (lowest address wins, deterministically) so populate is
    # idempotent — otherwise two entries upsert the same row and ping-pong its
    # object every run, faking a supersession churn (breaks FR-010/SC-007).
    by_name = {}
    for e in ordered:
        by_name.setdefault(e["name"], e)
    return list(by_name.values())


_image_cache = {}

JR_RA = 0x03E00008
HEAD_MAX_WORDS = 4


def _ends_transfer(word):
    """jr $ra, an unconditional b (beq $zero,$zero), or j: control never
    falls through past the delay slot that follows."""
    return word == JR_RA or (word >> 16) == 0x1000 or (word >> 26) == 2


def falls_through_into(image_bytes, address):
    """True when code before ``address`` can fall through into it: walking
    back over zero padding, the first non-zero word is not the delay slot of
    a jr $ra / b / j. Such an address is not a function start; it is a point
    inside a larger routine (the old inventory named several such tails)."""
    offset = address - GAME_CODE_BASE
    word_at = lambda off: struct.unpack_from(">I", image_bytes, off)[0]
    p = offset - 4
    while p >= 4:
        if _ends_transfer(word_at(p - 4)):
            return False              # p is a delay slot: the previous code ended
        if word_at(p) != 0:
            return True               # live code runs straight into `address`
        p -= 4                        # zero padding
    return False


def stranded_head(image_bytes, address, max_words=HEAD_MAX_WORDS):
    """Words a function lost from its start: how many instructions directly
    before ``address`` are really its first ones.

    The old inventory's prologue scan sometimes started a function one to
    four instructions late (typically after a ``lui``/load of a global, or an
    ``sll``/``sra`` narrowing pair), stranding the real head in a tiny opaque
    run. Walking backwards, a word joins the head while it is non-zero, is
    not ``jr $ra``, and is not the delay slot of a preceding ``jr $ra``/``b``/
    ``j``. The walk must end at padding (zero) or at such a delay slot, i.e.
    at a point no code falls through from; otherwise nothing is claimed."""
    offset = address - GAME_CODE_BASE
    word_at = lambda off: struct.unpack_from(">I", image_bytes, off)[0]
    count = 0
    while count < max_words:
        off = offset - 4 * (count + 1)
        if off < 0:
            return 0
        word = word_at(off)
        if word == 0 or word == JR_RA:
            break
        if off >= 4 and _ends_transfer(word_at(off - 4)):
            break                       # `word` is a delay slot: not ours
        count += 1
    if count == 0:
        return 0
    head_start = offset - 4 * count
    # What precedes the head must be the end of other code: optional zero
    # padding, then the delay slot of a jr $ra / b / j. A lone zero after a
    # conditional branch is a delay-slot nop inside a function, not padding.
    cursor = head_start - 4
    while cursor >= 0 and word_at(cursor) == 0 and cursor >= 4 and not _ends_transfer(word_at(cursor - 4)):
        cursor -= 4
    if cursor < 4 or not _ends_transfer(word_at(cursor - 4)):
        return 0
    # No branch in the preceding code may land on the head: if one does, the
    # words belong to that code (a loop tail), not to this function.
    lo = max(0, head_start - 4 * 2048)
    for off in range(lo, head_start, 4):
        word = word_at(off)
        opcode = word >> 26
        is_branch = opcode in {0x01, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17} or (
            opcode in {0x10, 0x11, 0x12} and ((word >> 21) & 0x1F) == 0x08)
        if not is_branch:
            continue
        imm = word & 0xFFFF
        if imm & 0x8000:
            imm -= 0x10000
        target = off + 4 + imm * 4
        if head_start <= target < offset:
            return 0
    return count


def scan_extent(image_bytes, address):
    """Return the instruction count ending at the first eligible ``jr $ra``.

    ``image_bytes`` is the complete game-code image mapped at
    :data:`GAME_CODE_BASE`.  A forward direct branch keeps the scan alive
    through its target; jumps and backward branches do not.  The string
    ``"scan_overrun"`` denotes reaching the image boundary or 16 KiB cap
    without a terminating return and its delay slot.
    """
    offset = address - GAME_CODE_BASE
    if offset < 0 or offset % 4 or offset >= len(image_bytes):
        raise ValueError(f"address {address:#x} outside game-code image")

    image_end = GAME_CODE_BASE + len(image_bytes)
    bound = min(address + 16 * 1024, image_end)
    furthest = address - 4
    pc = address
    while pc + 4 <= bound:
        word = struct.unpack_from(">I", image_bytes, pc - GAME_CODE_BASE)[0]
        opcode = word >> 26

        # Direct PC-relative branch encodings: REGIMM, beq/bne/blez/bgtz,
        # their likely forms, and bc0/bc1/bc2.  J/JAL are deliberately absent.
        is_branch = opcode in {0x01, 0x04, 0x05, 0x06, 0x07,
                               0x14, 0x15, 0x16, 0x17}
        if opcode in {0x10, 0x11, 0x12} and ((word >> 21) & 0x1F) == 0x08:
            is_branch = True
        if is_branch:
            immediate = word & 0xFFFF
            if immediate & 0x8000:
                immediate -= 0x10000
            target = pc + 4 + immediate * 4
            if target > pc:
                furthest = max(furthest, target)

        # SPECIAL / JR / rs=$ra.  The delay slot must fit inside both bounds.
        # >= not >: a shared-return leaf branches directly to its jr, so
        # furthest == pc at the true end (contract §3 amendment).
        if (word & 0xFFE0003F) == 0x03E00008 and pc >= furthest:
            return (pc - address) // 4 + 2 if pc + 8 <= bound else "scan_overrun"
        pc += 4

    return "scan_overrun"


def branch_target(word, pc):
    """Direct conditional branch destination, or None (calls are separate)."""
    op = word >> 26
    branch = op in {1, 4, 5, 6, 7, 20, 21, 22, 23} or (
        op in {16, 17, 18} and (word >> 21) & 31 == 8)
    if not branch:
        return None
    imm = word & 0xFFFF
    return pc + 4 + 4 * (imm - 0x10000 if imm & 0x8000 else imm)


def _head_switch(image, pc, address, bound):
    """Prove the adjacent IDO range-check/table dispatch, without table guessing.

    sltiu test,index,N; beq test,zero,default; sll index,index,2;
    lui base,hi; addu base,base,index; lw dest,lo(base); jr dest.
    The caller must additionally prove execution through the check's fallthrough.
    """
    if pc - 24 < address:
        return {"reason": "missing_switch_check"}
    words = struct.unpack_from(">7I", image, pc - 24 - GAME_CODE_BASE)
    check, branch, shift, upper, add, load, jump = words
    rs = lambda w: (w >> 21) & 31
    rt = lambda w: (w >> 16) & 31
    rd = lambda w: (w >> 11) & 31
    index, test, base, dest = rs(check), rt(check), rt(upper), rs(jump)
    count = check & 0xFFFF
    if not (check >> 26 == 11 and 0 < count <= 1024
            and index != 0 and test not in (0, index)
            and branch >> 26 == 4 and {rs(branch), rt(branch)} == {test, 0}
            and shift >> 26 == 0 and shift & 63 == 0 and rs(shift) == 0
            and rt(shift) == rd(shift) == index and (shift >> 6) & 31 == 2
            and upper >> 26 == 15 and rs(upper) == 0 and base not in (0, index)
            and add >> 26 == 0 and add & 63 == 33 and rd(add) == base
            and {rs(add), rt(add)} == {base, index}
            and load >> 26 == 35 and rs(load) == base and rt(load) == dest
            and jump >> 26 == 0 and jump & 63 == 8
            and dest not in (0, 31) and jump & 0x1FFFFF == 8):
        return {"reason": "unsupported_switch_dispatch"}
    imm = load & 0xFFFF
    table = (((upper & 0xFFFF) << 16)
             + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
    end = GAME_CODE_BASE + len(image)
    if table % 4 or not GAME_CODE_BASE <= table < table + count * 4 <= end:
        return {"reason": "invalid_switch_table", "table": table, "entries": count}
    if table < bound and table + count * 4 > address:
        return {"reason": "switch_table_overlaps_code", "table": table}
    destinations = list(struct.unpack_from(">%dI" % count, image, table - GAME_CODE_BASE))
    for i, target in enumerate(destinations):
        if target % 4 or not address <= target < bound:
            return {"reason": "escaping_switch_entry", "table": table,
                    "entry": i, "target": target}
    return {"at": pc, "check": pc - 24, "table": table,
            "entries": count, "destinations": destinations}


def scan_head_extent(image, address, bound):
    """Bounded control-flow proof for a newly discovered head.

    Follow both branch arms, including early returns and delay slots. Reject
    unproved indirect jumps, escaping branches, fall-through at the bound, and stack
    imbalance. This deliberately does not change the historical 005 scanner.
    Instruction decoding and incoming-branch checks belong to closure.heads.
    """
    end = GAME_CODE_BASE + len(image)
    if address % 4 or bound % 4 or not GAME_CODE_BASE <= address < bound <= end:
        return {"reason": "invalid_bound"}
    if bound - address > 16 * 1024:
        bound = address + 16 * 1024
    word_at = lambda pc: struct.unpack_from(">I", image, pc - GAME_CODE_BASE)[0]

    def stack_delta(word):
        if word >> 26 in (9, 25) and (word >> 16) & 0x3FF == 0x3BD:
            imm = word & 0xFFFF
            return imm - 0x10000 if imm & 0x8000 else imm
        return 0

    def transfer(word):
        return (branch_target(word, address) is not None or word >> 26 in (2, 3)
                or word >> 26 == 0 and word & 63 in (8, 9))

    switches = {}
    for pc in range(address, bound, 4):
        word = word_at(pc)
        if word >> 26 == 0 and word & 63 == 8 and word != JR_RA:
            switches[pc] = _head_switch(image, pc, address, bound)
    checks = {s["check"]: pc for pc, s in switches.items() if "check" in s}
    # A dispatch route is valid only through consecutive instructions, starting
    # at sltiu and then the checked branch's untaken arm. Entering midway cannot
    # inherit it. Keep separate states at joins so an unsafe route is not hidden.
    pending = [(address, 0, None)]
    depths, seen, covered, returns, used_switches = {}, set(), set(), set(), set()
    while pending:
        pc, sp, route = pending.pop()
        if not address <= pc < bound:
            return {"reason": "fallthrough_at_bound", "at": pc}
        if pc in depths and depths[pc] != sp:
            return {"reason": "inconsistent_stack", "at": pc}
        depths[pc] = sp
        if (pc, route) in seen:
            continue
        seen.add((pc, route))
        if route is not None and route[1] != pc:
            route = None
        covered.add(pc)
        word = word_at(pc)
        sp += stack_delta(word)
        if sp > 0 or sp < -16384:
            return {"reason": "invalid_stack", "at": pc}
        op, target = word >> 26, branch_target(word, pc)
        if not transfer(word):
            if pc in checks:
                route = (checks[pc], pc + 4)
            elif route is not None:
                route = (route[0], pc + 4)
            pending.append((pc + 4, sp, route))
            continue
        if pc + 8 > bound:
            return {"reason": "missing_delay_slot", "at": pc}
        slot = word_at(pc + 4)
        if transfer(slot):
            return {"reason": "transfer_in_delay_slot", "at": pc + 4}
        covered.add(pc + 4)
        slot_sp = sp + stack_delta(slot)
        if word == JR_RA:
            if slot_sp != 0:
                return {"reason": "unbalanced_return", "at": pc}
            returns.add(pc)
        elif op == 0 and word & 63 == 8:
            switch = switches[pc]
            if "reason" in switch:
                return {"reason": "indirect_jump", "at": pc,
                        "switch_reason": switch["reason"], "switch_detail": switch}
            if route != (pc, pc):
                return {"reason": "unproven_switch_guard", "at": pc}
            used_switches.add(pc)
            pending.extend((target, slot_sp, None)
                           for target in sorted(set(switch["destinations"])))
        elif op == 3 or op == 0 and word & 63 == 9:
            pending.append((pc + 8, slot_sp, None))
        elif op == 2:
            target = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
            if not address <= target < bound:
                return {"reason": "escaping_jump", "at": pc, "target": target}
            pending.append((target, slot_sp, None))
        else:
            if not address <= target < bound:
                return {"reason": "escaping_branch", "at": pc, "target": target}
            # REGIMM branch-and-link needs a callee summary; leave it unclaimed.
            if op == 1 and (word >> 16) & 31 >= 16:
                return {"reason": "branch_and_link", "at": pc}
            pending.append((target, slot_sp, None))
            always = (op in (4, 20) and (word >> 21) & 31 == (word >> 16) & 31
                      or op == 1 and (word >> 21) & 31 == 0
                      and (word >> 16) & 31 in (1, 3))
            if not always:
                likely = op in (20, 21, 22, 23) or (
                    op == 1 and (word >> 16) & 31 in (2, 3)) or (
                    op in (16, 17, 18) and word & 0x20000)
                checked = (route is not None and route[0] in switches
                           and switches[route[0]].get("check") == pc - 4)
                next_route = (route[0], pc + 8) if checked else None
                pending.append((pc + 8, sp if likely else slot_sp, next_route))
    if not returns:
        return {"reason": "no_return"}
    last = max(covered) + 4
    if last != max(returns) + 8:
        return {"reason": "code_after_last_return"}
    # Even unreachable blocks must not branch outside the claimed body.
    for pc in range(address, last, 4):
        target = branch_target(word_at(pc), pc)
        if target is not None and not address <= target < last:
            return {"reason": "escaping_branch", "at": pc, "target": target}
    result = {"insn_count": (last - address) // 4,
              "returns": sorted(returns), "reachable_words": len(covered)}
    if used_switches:
        result["switches"] = [switches[pc] for pc in sorted(used_switches)]
    return result


def _image(path):
    if path not in _image_cache:
        _image_cache[path] = path.read_bytes()
    return _image_cache[path]


def function_words(address, size):
    """Raw big-endian instruction words for a function, from whichever image
    holds that address. Returns list of 8-hex-digit strings."""
    if GAME_CODE_BASE <= address < GAME_CODE_BASE + GAME_CODE_BIN.stat().st_size:
        data = _image(GAME_CODE_BIN)
        off = address - GAME_CODE_BASE
    else:
        data = _image(BASEROM)
        off = address - STATIC_ROM_DELTA
        if off < 0 or off >= len(data):
            raise ValueError(f"address {address:#x} outside known images")
    chunk = data[off : off + (size // 4) * 4]
    return [f"{w:08X}" for (w,) in struct.iter_unpack(">I", chunk)]


def assemble_words(words, out_o, func_name="func"):
    asm = [".set noreorder", ".text", f".globl {func_name}", f"{func_name}:"]
    asm += [f"    .word 0x{w}" for w in words]
    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as f:
        f.write("\n".join(asm) + "\n")
        src = f.name
    subprocess.run(
        ["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-o", str(out_o), src],
        check=True,
    )


# --- reloc-aware assembly from splat asm regions (003) -----------------------


@dataclasses.dataclass
class Region:
    """One `glabel`..`endlabel` function region from a splat `.s` file."""
    name: str          # the asm glabel (func_XXXXXXXX), not the target_id
    vaddr: int         # vaddr of the first instruction (the region key)
    lines: list        # assembler-ready text (comments stripped from insns)
    words: list        # ROM instruction words, 8-hex strings, in order


# KSEG1 de-symbolization (003 review fix). splat symbolizes MMIO addresses
# (DPC_CLOCK_REG = 0xA4100010, …), but IDO compiles #define'd KSEG1 addresses
# to literal immediates with NO relocation — so a relocation against a KSEG1
# symbol in a target is a disassembly artifact, not ROM truth, and it penalizes
# every correctly-matched candidate (SC-004 regression: 4 locked functions).
# Rule: an instruction whose %hi/%lo symbol resolves into KSEG1
# (0xA0000000..0xBFFFFFFF) is emitted as its raw `.word` — the ROM word IS the
# literal IDO produced. RAM symbols keep their relocations.
_HILO_SYM_RE = re.compile(r"%(?:hi|lo)\(([A-Za-z_][A-Za-z0-9_]*)")
_SYM_DEF_RE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*0x([0-9A-Fa-f]+)\s*;")
_ADDR_NAME_RE = re.compile(r"^(?:D|func|jtbl)_([0-9A-Fa-f]{8})$")
KSEG1_BASE, KSEG1_END = 0xA0000000, 0xC0000000

_symbol_map_cache = None


def _symbol_map():
    """{name: address} from symbol_addrs.us.txt + hardware_regs.ld (both are
    `NAME = 0xADDR;` lines). Cached; missing files contribute nothing."""
    global _symbol_map_cache
    if _symbol_map_cache is None:
        m = {}
        for fname in ("symbol_addrs.us.txt", "hardware_regs.ld"):
            path = REPO / fname
            if not path.is_file():
                continue
            for line in path.read_text(errors="replace").splitlines():
                d = _SYM_DEF_RE.match(line)
                if d:
                    m.setdefault(d.group(1), int(d.group(2), 16))
        _symbol_map_cache = m
    return _symbol_map_cache


def _resolve_symbol(name):
    """Best-effort address for an asm symbol: the symbol tables first, then
    splat's address-bearing name patterns (D_/func_/jtbl_XXXXXXXX)."""
    addr = _symbol_map().get(name)
    if addr is not None:
        return addr
    m = _ADDR_NAME_RE.match(name)
    return int(m.group(1), 16) if m else None


def _is_kseg1_ref(mnemonic_text):
    """True iff the instruction references a %hi/%lo symbol that resolves into
    KSEG1 (uncached MMIO space — never link-time relocated in a real build)."""
    m = _HILO_SYM_RE.search(mnemonic_text)
    if not m:
        return False
    addr = _resolve_symbol(m.group(1))
    return addr is not None and KSEG1_BASE <= addr < KSEG1_END


def static_asm_paths(asm_dir):
    """Static assembly in whole segments and converted ROM-TU slots."""
    root = Path(asm_dir)
    return sorted(root.glob("*.s")) + sorted((root / "nonmatchings" / "rom").rglob("*.s"))


def index_asm_regions(asm_dir=None):
    """{first_instruction_vaddr: Region} over static assembly inputs.

    A region runs from `glabel <name>` to `endlabel <name>`. Instruction lines
    (`/* off vaddr word */  mnemonic`) contribute their word to the gate and,
    comment-stripped, their mnemonic to the assembler input; interior label
    lines (`.L…:`) and any blanks/directives are kept verbatim — the assembler
    tolerates them and the gate judges the result. Instructions referencing
    KSEG1 (MMIO) symbols are emitted as their raw `.word` instead (see
    _is_kseg1_ref) so the object matches IDO's literal-immediate codegen.
    """
    asm_dir = Path(asm_dir) if asm_dir else ASM_DIR
    regions = {}
    for path in static_asm_paths(asm_dir):
        cur = None
        for line in path.read_text(errors="replace").splitlines():
            g = _GLABEL_RE.match(line)
            if g:
                cur = {"name": g.group(1), "vaddr": None, "lines": [], "words": []}
                continue
            e = _ENDLABEL_RE.match(line)
            if e:
                if cur is not None and cur["vaddr"] is not None:
                    regions[cur["vaddr"]] = Region(
                        cur["name"], cur["vaddr"], cur["lines"], cur["words"])
                cur = None
                continue
            if cur is None:
                continue
            m = _REGION_LINE_RE.search(line)
            if m:
                if cur["vaddr"] is None:
                    cur["vaddr"] = int(m.group(1), 16)
                word = m.group(2).upper()
                cur["words"].append(word)
                # Everything after the closing `*/` is the mnemonic; the
                # comment (the only `*/` on the line in MIPS asm) is dropped.
                mnemonic = line.split("*/", 1)[1].rstrip()
                if _is_kseg1_ref(mnemonic):
                    cur["lines"].append(f"    .word 0x{word}")
                else:
                    cur["lines"].append(mnemonic)
            else:
                cur["lines"].append(line.rstrip())
    return regions


class AssembleError(Exception):
    """Assembler rejected a region; carries the first stderr line as the
    gate_reason detail."""


def _data_field_aliases(lines):
    """Express the documented PIF status alias through its containing buffer.

    IDO naturally emits buffer+60 for the end of the fifteen-word RAM array.
    Splat's separate historical label names that same pifstatus field. Keep
    relocation addends comparable only when authoritative addresses prove
    the field relationship; other symbols and instruction words stay intact.
    """
    base = _resolve_symbol("__osSiDmaBuffer")
    field = _resolve_symbol("__osSiDmaRetry")
    if base is None or field != base + 0x3C:
        return list(lines)
    return [re.sub(r"(%(?:hi|lo)\()__osSiDmaRetry(\))",
                   r"\1__osSiDmaBuffer+0x3c\2", line) for line in lines]


def _sdk_field_offsets(kind):
    """Conservative o32 field proof from matching live and canonical SDK types.

    No header parsing guesses: accept only the exact documented unsigned
    scalar types and complete ordered task declaration. Missing/drifted types
    leave the historical relocation names unchanged.
    """
    def clean(path):
        text = (REPO / path).read_text()
        return re.sub(r"/\*.*?\*/|//[^\n]*", " ", text, flags=re.S)
    try:
        live = clean("include/types.h")
        sdk = clean("reference/repos/ultralib/include/PR/ultratypes.h")
        if not all(re.search(r"typedef\s+unsigned\s+long\s+long\s+u64\s*;", text)
                   for text in (live, sdk)):
            return None
        if kind == "time":
            if not all(re.search(r"typedef\s+u64\s+OSTime\s*;", clean(path)) for path in
                       ("include/PR/os_time.h", "reference/repos/ultralib/include/PR/os_time.h")):
                return None
            # Big-endian N64 u64: the second u32 word begins at byte four.
            return {"low_word": 4}
        if kind != "task":
            return None
        if not all(re.search(r"typedef\s+unsigned\s+(?:int|long)\s+u32\s*;", text)
                   for text in (live, sdk)):
            return None
        fields = [("u32", "type"), ("u32", "flags"),
                  ("u64*", "ucode_boot"), ("u32", "ucode_boot_size"),
                  ("u64*", "ucode"), ("u32", "ucode_size"),
                  ("u64*", "ucode_data"), ("u32", "ucode_data_size"),
                  ("u64*", "dram_stack"), ("u32", "dram_stack_size"),
                  ("u64*", "output_buff"), ("u64*", "output_buff_size"),
                  ("u64*", "data_ptr"), ("u32", "data_size"),
                  ("u64*", "yield_data_ptr"), ("u32", "yield_data_size")]
        for text in (live, clean("reference/repos/ultralib/include/PR/sptask.h")):
            match = re.search(r"typedef\s+struct\s*\{([^{}]*)\}\s*OSTask_t\s*;", text, re.S)
            if not match:
                return None
            declarations = [re.sub(r"\s+", "", part) for part in match.group(1).split(";") if part.strip()]
            if declarations != [typ + name for typ, name in fields]:
                return None
        # Both live struct and SDK aligned union expose t at byte zero.
        live_wrapper = re.search(r"typedef\s+struct\s*\{\s*OSTask_t\s+t\s*;\s*\}\s*OSTask\s*;", live)
        sdk_task = clean("reference/repos/ultralib/include/PR/sptask.h")
        sdk_wrapper = re.search(r"typedef\s+union\s*\{\s*OSTask_t\s+t\s*;\s*long\s+long\s+int\s+force_structure_alignment\s*;\s*\}\s*OSTask\s*;", sdk_task)
        if not live_wrapper or not sdk_wrapper:
            return None
        # Every verified field is an o32 u32 or pointer, four bytes each.
        return {name: 4 * index for index, (_, name) in enumerate(fields)}
    except OSError:
        return None


def _typed_data_field_aliases(lines, target_id):
    """Equivalent SDK field relocations, guarded by types and all addresses."""
    relationships = []
    if target_id == "osGetTime":
        offsets = _sdk_field_offsets("time")
        if offsets is not None:
            relationships = [("gViTimeAccumLo", "gViTimeAccumHi", offsets["low_word"])]
    elif target_id == "osViModeTableGet":
        offsets = _sdk_field_offsets("task")
        if offsets is not None:
            fields = ("ucode", "ucode_data", "dram_stack", "output_buff",
                      "output_buff_size", "data_ptr", "yield_data_ptr")
            relationships = [("gViModePtr" + str(index), "gViModeTempBuffer", offsets[field])
                             for index, field in enumerate(fields)]
    # Validate the complete relationship family before replacing any operand.
    if not relationships or any(_resolve_symbol(base) is None or
                                _resolve_symbol(alias) != _resolve_symbol(base) + offset
                                for alias, base, offset in relationships):
        return list(lines)
    result = list(lines)
    for alias, base, offset in relationships:
        result = [re.sub(r"(%(?:hi|lo)\()" + re.escape(alias) + r"(\))",
                         lambda match: match.group(1) + base + "+" + hex(offset) + match.group(2), line)
                  for line in result]
    return result


def assemble_region(region, target_id, out_o):
    """Assemble a region into a relocatable object named `target_id`. Raises
    AssembleError (first stderr line preserved) on assembler failure."""
    asm = [
        ".set noreorder",
        ".set noat",
        ".section .text",
        f".globl {target_id}",
        f"{target_id}:",
    ]
    # Splat uses o32 float aliases such as $fa0/$ft0f. They are defined
    # by the ROM assembler prelude, not by GNU as itself. Reuse only those
    # register declarations; the prelude's macros and gp mode are unrelated
    # to this isolated relocatable target.
    prelude = REPO / "tools" / "asm-processor" / "prelude.inc"
    aliases = [line for line in prelude.read_text().splitlines()
               if re.match(r"^\s*\.set\s+\$f\w+,\s*\$f\d+\s*$", line)]
    asm[0:0] = aliases
    asm += _typed_data_field_aliases(_data_field_aliases(region.lines), target_id)
    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as f:
        f.write("\n".join(asm) + "\n")
        src = f.name
    try:
        proc = subprocess.run(
            ["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-o", str(out_o), src],
            capture_output=True, text=True,
        )
    finally:
        os.unlink(src)
    if proc.returncode != 0:
        # Preserve the first real error, stripped of the temp-file path so the
        # gate_reason is deterministic across runs (FR-010). GNU as format:
        # `<path>:<line>: Error: <msg>` — keep from `<line>:` onward.
        detail = "assembler failed"
        for line in (proc.stderr or "").splitlines():
            if ": Error:" in line:
                detail = line.split(".s:", 1)[-1].strip() if ".s:" in line \
                    else line.strip()
                break
        raise AssembleError(detail)


def _strip_trailing_nops(words):
    """Drop trailing 0x00000000 (nop) padding words. The assembler pads `.text`
    to a 16-byte boundary while splat's `endlabel` excludes the ROM's own
    alignment nops, so a byte-identical region can differ only in trailing
    padding — bookkeeping the linker owns, not function content (research D3)."""
    n = len(words)
    while n > 0 and words[n - 1] == 0:
        n -= 1
    return words[:n]


def _gate_decide(rom_words, new_words, sites):
    """Pure gate decision over two int-word lists + the new object's reloc
    sites: (ok, reason). Trailing nop padding on either side is ignored, then
    lengths must match and the masked words must be equal. Reuses
    jobs/scoring's mask helpers — one mask logic in the codebase (research D3).
    Factored out so tests can drive it without an assembler."""
    from ..jobs import scoring

    rom = _strip_trailing_nops(list(rom_words))
    new = _strip_trailing_nops(list(new_words))
    if len(new) != len(rom):
        return False, f"length_mismatch {len(new)} != {len(rom)}"
    if scoring._masked_diff(rom, new, sites) != 0:
        masked_t, masked_c = list(rom), list(new)
        for i, mask in sites:
            if i < len(masked_c):
                masked_c[i] &= mask
            if i < len(masked_t):
                masked_t[i] &= mask
        idx = next((i for i in range(len(masked_t)) if masked_t[i] != masked_c[i]), 0)
        return False, f"word_mismatch@{idx}"
    return True, None


def gate_target(rom_words, new_o):
    """Round-trip gate: (ok, reason). The reassembled object's instruction
    words, masked at its own relocation sites, must equal the original ROM
    words masked identically. `rom_words` are 8-hex strings (region ROM words)."""
    from ..jobs import scoring

    binary = scoring._objdump_path()
    # -dz (disassemble-zeroes): without it objdump collapses runs of zero words
    # to `...`, which _parse_text_words skips — undercounting interior/trailing
    # nops against the raw ROM words and faking a length_mismatch. The permuter
    # Scorer disassembles with -z for the same reason.
    new_words = scoring._parse_text_words(scoring._objdump(binary, "-dz", str(new_o)))
    sites = scoring._parse_relocs(scoring._objdump(binary, "-r", str(new_o)))
    return _gate_decide([int(w, 16) for w in rom_words], new_words, sites)


# --- reloc-aware EXTRACTED targets (2026-09-24) ------------------------------
# A raw-word target bakes absolute addresses into its words: a call reads
# `0C0296CF`, a global read carries its real %hi/%lo immediates. No compiled
# candidate can reproduce that — the assembler emits a relocation with a zeroed
# field — so every call and every global reference counted as a permanent
# mismatch and any extracted function that referenced anything was incapable of
# scoring 0. (The only two that ever did were the two that reference nothing.)
# Evidence: specs/007-population-closure/research/reloc-scoring-finding.md.
#
# The derived asm already symbolizes jal/%hi/%lo, so assembling THAT gives a
# target carrying real relocations against the same names the candidate's
# declarations use. It is written for mips_to_c, though, and re-emits synthetic
# `lui`s to fix m2c's %hi/%lo binding; those are not ROM instructions and must
# be dropped or the target gains words the ROM never had.

_DERIVED_LABEL_RE = re.compile(r"^\.L[0-9A-Fa-f]+:$")
# objdump prints the FP control register by name; GNU as only accepts the
# number, so `cfc1 $t6,c1_fcsr` is rejected outright (30 targets).
_AS_SPELLING = ((r"\bc1_fcsr\b", "$31"), (r"\bc1_fir\b", "$0"))


def target_asm_from_derived(derived_text, target_id):
    """Assembler source for a reloc-aware extracted target.

    normalize_objdump emits, per instruction: its `.L<vaddr>:` label, then any
    synthetic instructions, then the real one. So the last instruction in each
    label block is the ROM instruction and the rest are m2c scaffolding. Labels
    are kept — branches reference them."""
    lines = [".set noreorder", ".set noat", ".section .text",
             f".globl {target_id}", f"{target_id}:"]
    block = []

    def flush():
        if block:
            lines.append(block[-1])      # the real instruction; drop synthetics
            block.clear()

    for line in derived_text.splitlines():
        stripped = line.strip()
        if line.startswith("glabel "):
            continue
        if _DERIVED_LABEL_RE.match(stripped):
            flush()
            lines.append(stripped)
        elif line.startswith("    ") and stripped:
            for pattern, replacement in _AS_SPELLING:
                line = re.sub(pattern, replacement, line)
            block.append(line)
    flush()
    return "\n".join(lines) + "\n"


def assemble_text(asm_text, out_o):
    """Assemble prepared source, raising AssembleError like assemble_region."""
    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as handle:
        handle.write(asm_text)
        src = handle.name
    try:
        proc = subprocess.run(
            ["mips-linux-gnu-as", "-march=vr4300", "-mabi=32", "-o", str(out_o), src],
            capture_output=True, text=True,
        )
    finally:
        os.unlink(src)
    if proc.returncode != 0:
        detail = "assembler failed"
        for line in (proc.stderr or "").splitlines():
            if ": Error:" in line:
                detail = line.split(".s:", 1)[-1].strip() if ".s:" in line else line.strip()
                break
        raise AssembleError(detail)


def relocate_extracted(conn, store, limit=None, context_sha=None, names=None):
    """Upgrade extracted targets from raw-word to reloc-aware objects.

    Each target is assembled from its derived asm and must pass the same
    round-trip gate feature 003 uses for static targets: masked at the new
    object's own relocation sites, its words must equal the ROM's. A failure
    keeps the existing raw-word object and records why. Objects that change
    supersede their evidence, exactly as re-extraction does."""
    from ..coordinator import db as dbmod
    from . import disasm as disasmmod

    rows = conn.execute(
        "SELECT target_id, address, insn_count, target_o_sha, tier, gate_reason"
        " FROM n64_target WHERE population='extracted' AND address IS NOT NULL"
        " AND insn_count IS NOT NULL ORDER BY address"
    ).fetchall()
    summary = Counter()
    reasons = Counter()
    superseded_targets = purged_rows = 0
    with tempfile.TemporaryDirectory() as tmp:
        out_o = Path(tmp) / "target.o"
        for row in rows:
            if names is not None and row["target_id"] not in set(names):
                continue
            reason = row["gate_reason"] or ""
            if reason.startswith("extent_conflict") or reason.startswith("scan_overrun"):
                summary["skipped_gate"] += 1
                continue
            if limit and summary["attempted"] >= limit:
                break
            summary["attempted"] += 1
            target_id = row["target_id"]
            try:
                asm_path = disasmmod.derive(conn, target_id, context_sha=context_sha)
            except Exception as exc:
                summary["no_disasm"] += 1
                reasons[f"no_disasm: {type(exc).__name__}"] += 1
                continue
            try:
                assemble_text(target_asm_from_derived(
                    asm_path.read_text(errors="replace"), target_id), out_o)
            except AssembleError as exc:
                summary["assemble_error"] += 1
                reasons[f"assemble_error: {exc}"] += 1
                continue
            rom_words = function_words(row["address"], row["insn_count"] * 4)
            ok, why = gate_target(rom_words, out_o)
            if not ok:
                summary["gate_failed"] += 1
                reasons[why.split("@")[0]] += 1
                continue
            o_sha = store.put_file(out_o)
            summary["reloc_aware"] += 1
            if o_sha == row["target_o_sha"] and row["tier"] == "reloc_aware":
                summary["unchanged"] += 1
                continue
            with dbmod.tx(conn):
                changed, purged = _supersede_target(
                    conn, target_id, row["target_o_sha"], o_sha)
                if changed:
                    superseded_targets += 1
                    purged_rows += purged
                conn.execute(
                    "UPDATE n64_target SET target_o_sha=?, tier='reloc_aware'"
                    " WHERE target_id=?", (o_sha, target_id))
                conn.execute(
                    "INSERT OR IGNORE INTO blob (sha256, kind, size_bytes, created_at)"
                    " VALUES (?, 'target', ?, strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                    (o_sha, store.size(o_sha) or 0))
            summary["upgraded"] += 1
    summary["superseded_targets"] = superseded_targets
    summary["purged_evidence_rows"] = purged_rows
    return dict(summary), dict(reasons)


def _fallback_category(reason):
    """Coarse bucket of a gate_reason for the coverage histogram."""
    if reason is None:
        return None
    for prefix in ("assemble_error", "word_mismatch", "length_mismatch"):
        if reason.startswith(prefix):
            return prefix
    return reason  # no_asm_region


def _supersede_target(conn, target_id, previous_sha, new_sha):
    """Apply feature 003's object-identity supersession contract.

    The caller must invoke this in the same transaction as its n64_target
    update.  Returns ``(changed, purged_matrix_rows)``.
    """
    if previous_sha == new_sha:
        return False, 0
    cur = conn.execute("DELETE FROM matrix_entry WHERE target_id=?", (target_id,))
    return True, max(cur.rowcount, 0)


def _extent_plan(conn, inventory):
    """Scan extracted inventory and return per-name repair metadata."""
    image = _image(GAME_CODE_BIN)
    previous = {
        row["target_id"]: row
        for row in conn.execute(
            "SELECT target_id, insn_count, gate_reason FROM n64_target"
            " WHERE population='extracted'"
        )
    }
    plan = {}
    for entry in inventory:
        address = entry["address"]
        if not (GAME_CODE_BASE <= address < GAME_CODE_BASE + len(image)):
            continue
        try:
            scanned = scan_extent(image, address)
        except ValueError:
            scanned = "scan_overrun"
        plan[entry["name"]] = {
            "address": address,
            "scanned": scanned,
            "previous": previous.get(entry["name"]),
            "container": None,
            "head": 0,
        }

    extents = [
        (name, item["address"], item["address"] + item["scanned"] * 4)
        for name, item in plan.items() if isinstance(item["scanned"], int)
    ]
    # 007: closure-registered functions are containers too — an inventory
    # row that starts inside one is a function suffix (late prologue scan)
    # and must stay extent_conflict across re-extraction.
    extents += [
        (row["target_id"], row["address"], row["address"] + row["insn_count"] * 4)
        for row in conn.execute(
            "SELECT target_id,address,insn_count FROM n64_target"
            " WHERE population='extracted' AND gate_reason='discovered'"
            " AND insn_count IS NOT NULL")
    ]
    for name, item in plan.items():
        containers = [
            extent for extent in extents
            if extent[0] != name and extent[1] < item["address"] < extent[2]
        ]
        if containers:
            # Name the tightest containing function, deterministically.
            item["container"] = min(
                containers, key=lambda extent: (extent[2] - extent[1], extent[1], extent[0])
            )[0]
    # Pull a function's start back over a stranded head (see stranded_head),
    # unless those words already belong to another planned extent.
    for name, item in plan.items():
        if item["container"] or not isinstance(item["scanned"], int):
            continue
        head = stranded_head(image, item["address"])
        if not head and falls_through_into(image, item["address"]):
            # Not a start at all (and no stranded head explains it): a tail
            # of an unregistered routine. Keep it out of the population.
            item["container"] = "fallthrough"
            continue
        if not head:
            continue
        start = item["address"] - 4 * head
        if any(other != name and lo < item["address"] and hi > start
               for other, lo, hi in extents):
            continue
        item["address"] = start
        item["scanned"] += head
        item["head"] = head
    return plan


def populate(conn, store, work_dir=None, limit=None, names=None):
    """Fill n64_target rows and build target .o blobs. Static targets attempt
    reloc-aware assembly (region → assemble → gate) with a raw-word fallback;
    dynamic targets stay raw-word. When a target's object bytes change, its
    matrix_entry evidence is superseded in the same transaction. Returns a
    summary dict and prints the FR-009 coverage report."""
    from ..coordinator import db as dbmod

    inventory = load_work_inventory(work_dir)
    if limit:
        inventory = inventory[:limit]
    regions = index_asm_regions()
    extent_plan = _extent_plan(conn, inventory)
    if names is not None:
        # Plan over the whole inventory (containers need every extent), then
        # rebuild only the named targets.
        inventory = [e for e in inventory if e["name"] in set(names)]

    built, skipped = 0, 0
    tiers = {"reloc_aware": 0, "raw_word_static": 0, "raw_word_dynamic": 0}
    fallbacks = Counter()
    superseded_targets, purged_rows = 0, 0
    extent_counts = Counter()

    with tempfile.TemporaryDirectory() as tmp:
        tmpdir = Path(tmp)
        for e in inventory:
            population = (
                "extracted"
                if GAME_CODE_BASE <= e["address"] < GAME_CODE_BASE + 0x9E0A0
                else "static"
            )
            extent = extent_plan.get(e["name"])
            if extent is not None:
                scanned = extent["scanned"]
                if isinstance(scanned, int):
                    e = dict(e, size=scanned * 4, address=extent["address"])
            try:
                words = function_words(e["address"], e["size"])
                if not words:
                    raise ValueError("empty function body")
            except (ValueError, subprocess.CalledProcessError) as exc:
                print(f"  skip {e['name']}: {exc}", file=sys.stderr)
                skipped += 1
                continue

            tier = "raw_word"
            gate_reason = None
            o_path = None
            # Static targets try the reloc-aware path (contract §populate 1-4).
            if population == "static":
                region = regions.get(e["address"])
                if region is None:
                    gate_reason = "no_asm_region"
                else:
                    reloc_o = tmpdir / "reloc.o"
                    try:
                        assemble_region(region, e["name"], reloc_o)
                    except AssembleError as exc:
                        gate_reason = f"assemble_error: {exc}"
                    else:
                        ok, reason = gate_target(region.words, reloc_o)
                        if ok:
                            tier, gate_reason, o_path = "reloc_aware", None, reloc_o
                        else:
                            gate_reason = reason

            if o_path is None:
                # Raw-word object: gate fallback (static) or dynamic population.
                raw_o = tmpdir / "raw.o"
                try:
                    assemble_words(words, raw_o, e["name"])
                except subprocess.CalledProcessError as exc:
                    print(f"  skip {e['name']}: {exc}", file=sys.stderr)
                    skipped += 1
                    continue
                o_path = raw_o
                if population != "static":
                    previous = extent["previous"] if extent else None
                    if extent and extent["container"]:
                        gate_reason = f"extent_conflict:{extent['container']}"
                    elif extent and extent["scanned"] == "scan_overrun":
                        gate_reason = "scan_overrun"
                    elif previous and previous["insn_count"] == len(words):
                        prior_reason = previous["gate_reason"] or ""
                        gate_reason = ("extent_repaired"
                                       if prior_reason == "extent_repaired" else None)
                    else:
                        gate_reason = "extent_repaired"

            o_sha = store.put_file(o_path)
            asm_sha = hashlib.sha256("\n".join(words).encode()).hexdigest()

            if tier == "reloc_aware":
                tiers["reloc_aware"] += 1
            elif population == "static":
                tiers["raw_word_static"] += 1
                fallbacks[_fallback_category(gate_reason)] += 1
            else:
                tiers["raw_word_dynamic"] += 1

            if extent is not None:
                if extent["container"]:
                    extent_counts["conflict"] += 1
                elif extent["scanned"] == "scan_overrun":
                    extent_counts["scan_overrun"] += 1
                elif extent["previous"] and extent["previous"]["insn_count"] == len(words):
                    extent_counts["agree"] += 1
                else:
                    extent_counts["repaired"] += 1

            with dbmod.tx(conn):
                prev = conn.execute(
                    "SELECT target_o_sha FROM n64_target WHERE target_id=?",
                    (e["name"],),
                ).fetchone()
                prev_sha = prev["target_o_sha"] if prev else None
                changed, purged = _supersede_target(
                    conn, e["name"], prev_sha, o_sha
                )
                if changed:
                    superseded_targets += 1
                    purged_rows += purged
                conn.execute(
                    "INSERT INTO n64_target (target_id, address, population,"
                    " insn_count, target_asm_sha, target_o_sha, tier, gate_reason)"
                    " VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
                    " ON CONFLICT(target_id) DO UPDATE SET address=excluded.address,"
                    " population=excluded.population, insn_count=excluded.insn_count,"
                    " target_asm_sha=excluded.target_asm_sha,"
                    " target_o_sha=excluded.target_o_sha, tier=excluded.tier,"
                    " gate_reason=excluded.gate_reason",
                    (e["name"], e["address"], population, len(words), asm_sha,
                     o_sha, tier, gate_reason),
                )
                conn.execute(
                    "INSERT INTO function_status (target_id, status, updated_at)"
                    " VALUES (?, 'unmatched', strftime('%Y-%m-%dT%H:%M:%fZ','now'))"
                    " ON CONFLICT(target_id) DO NOTHING",
                    (e["name"],),
                )
                conn.execute(
                    "INSERT OR IGNORE INTO blob (sha256, kind, size_bytes, created_at)"
                    " VALUES (?, 'target', ?, strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                    (o_sha, store.size(o_sha) or 0),
                )
            built += 1

    # FR-009 coverage report.
    print(f"target tiers: reloc_aware={tiers['reloc_aware']} "
          f"raw_word_static={tiers['raw_word_static']} "
          f"raw_word_dynamic={tiers['raw_word_dynamic']}")
    top = "  ".join(f"{cat}={n}" for cat, n in fallbacks.most_common())
    print(f"gate fallbacks (static): {tiers['raw_word_static']}"
          + (f" — top reasons: {top}" if top else ""))
    print(f"superseded: {superseded_targets} targets, "
          f"{purged_rows} evidence rows purged")
    print(f"extents: {extent_counts['agree']} agree, "
          f"{extent_counts['repaired']} repaired, "
          f"{extent_counts['conflict']} conflict")
    for target_id in EXTENT_REPORT_TARGETS:
        extent = extent_plan.get(target_id)
        if extent is None:
            continue
        before = extent["previous"]["insn_count"] if extent["previous"] else None
        after = extent["scanned"]
        before_end = (extent["address"] + before * 4) if before is not None else None
        after_end = (extent["address"] + after * 4) if isinstance(after, int) else after
        before_text = f"{before_end:#010x}" if before_end is not None else "missing"
        after_text = f"{after_end:#010x}" if isinstance(after_end, int) else after_end
        print(f"  {target_id}: {before_text} -> {after_text}")

    return {
        "built": built, "skipped": skipped, "total": len(inventory),
        "tiers": tiers, "fallbacks": dict(fallbacks),
        "superseded_targets": superseded_targets, "purged_rows": purged_rows,
        "extents": dict(extent_counts),
    }


def main():
    import argparse

    from ..coordinator import db as dbmod
    from ..coordinator.store import BlobStore

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--data", default=str(Path("~/.conveyor").expanduser()))
    parser.add_argument("--limit", type=int, default=None)
    parser.add_argument("--relocate-extracted", action="store_true",
                        help="upgrade extracted targets from raw-word to "
                             "reloc-aware objects (assembled from derived asm, "
                             "behind the 003 round-trip gate)")
    parser.add_argument("--repair-heads", action="store_true",
                        help="rebuild only the extracted targets whose start "
                             "moves back over a stranded head, then upgrade "
                             "them to reloc-aware objects")
    args = parser.parse_args()
    conn = dbmod.connect(Path(args.data) / "conveyor.db")
    store = BlobStore(Path(args.data) / "blobs")
    if args.repair_heads:
        plan = _extent_plan(conn, load_work_inventory())
        current = {r["target_id"]: r["address"] for r in conn.execute(
            "SELECT target_id,address FROM n64_target WHERE population='extracted'")}
        rows = {r["target_id"]: r for r in conn.execute(
            "SELECT target_id,gate_reason FROM n64_target WHERE population='extracted'")}
        names = sorted(n for n, item in plan.items()
                       if (item["head"] and current.get(n) != item["address"])
                       or (item["container"] == "fallthrough" and n in rows
                           and not (rows[n]["gate_reason"] or "").startswith("extent_conflict")))
        for n in names:
            if plan[n]["container"] == "fallthrough":
                print(f"  {n}: 0x{plan[n]['address']:08X} is reached by fall-through;"
                      " marking extent_conflict:fallthrough")
        for n in names:
            if not plan[n]["head"]:
                continue
            print(f"  {n}: start 0x{plan[n]['address'] + 4 * plan[n]['head']:08X}"
                  f" -> 0x{plan[n]['address']:08X} (+{plan[n]['head']} insns)")
        if not names:
            print("no stranded heads to repair")
            return
        summary = populate(conn, store, names=names)
        print(f"rebuilt {summary['built']} targets, superseded "
              f"{summary['superseded_targets']} ({summary['purged_rows']} evidence rows)")
        reloc, reasons = relocate_extracted(conn, store, names=names)
        print("reloc-aware: " + "  ".join(f"{k}={v}" for k, v in sorted(reloc.items())))
        return
    if args.relocate_extracted:
        summary, reasons = relocate_extracted(conn, store, limit=args.limit)
        print("extracted targets: "
              + "  ".join(f"{k}={v}" for k, v in sorted(summary.items())))
        for reason, count in sorted(reasons.items(), key=lambda kv: -kv[1])[:8]:
            print(f"  {count:5d}  {reason}")
        return
    summary = populate(conn, store, limit=args.limit)
    print(f"targets: {summary['built']} built, {summary['skipped']} skipped, "
          f"{summary['total']} in inventory")


if __name__ == "__main__":
    main()
