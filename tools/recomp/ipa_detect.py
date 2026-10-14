#!/usr/bin/env python3
"""Detect callees a standalone replacement build calls wrongly (w24t).

IDO -O3 compiled the game as one unit, so a static callee may take
parameters in registers o32 never passes (entity_transform_calc reads x,y,z,w
from $f22/$f24/$f26/$f28; audio_effect_setup reads `effect` from $s0). A
replacement compiled on its own calls such a callee with o32 conventions and
the retail body reads whatever was left in those registers: w21h's
engine_sound_sync draft was judged DIFFERENT at frame 706 for that reason
alone (w23f/w23r). `pipeline.ipa` cannot see the FP case (its derived asm
spells COP1 instructions as `.word`), so this module reads the RETAIL words
directly, stdlib only (it runs on the recomp host):

    register_params(words)  registers a body reads before any write, in
                            address order, that o32 does not pass in
                            (anything but zero, a0-a3, sp, ra, gp, f12, f14);
                            stores of callee-saved registers to the stack are
                            saves, not reads. FP registers are named by their
                            even half (f22 covers f22/f23).
    called_functions(elf)   names a replacement object reaches with jal
                            (R_MIPS_26 against undefined symbols).
    detect(...)             {callee: "register parameters f22,f24,..."}.

"Read before written" is a must-write dataflow over the body's branches
(delay slots and branch-likely modelled; jump-table code in address order).
Calibration (tools/recomp/README.md, w24t notes) is on every game function
against the IPA scan and the standalone locks.
"""
import struct

ZERO, GP, SP, RA = 0, 28, 29, 31
_O32_GPR = {ZERO, 4, 5, 6, 7, GP, SP, RA}
_O32_FPR = {12, 14}
_CALLEE_SAVED_GPR = set(range(16, 24)) | {30}
GPR_NAMES = ["zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6",
             "t7", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp",
             "s8", "ra"]
# After a call every caller-save register counts as written: a later read of
# one is the callee's IPA convention (sound_update_channel reads $t0 its
# callee leaves), not a parameter of this body.
_CALL_WRITES = ({("r", n) for n in list(range(1, 16)) + [24, 25, RA]}
                | {("f", n) for n in range(0, 20, 2)})
R_MIPS_26 = 4


def _g(n):
    return ("r", n)


def _f(n):
    return ("f", n & ~1)


def decode(word):
    """(reads, writes, is_call) of one MIPS II/III instruction (the 64-bit
    forms appear in libultra's long-long helpers); register keys are
    ("r", n) / ("f", even n); hi/lo and FP condition codes are left out."""
    op = word >> 26
    rs, rt, rd = (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
    reads, writes, call = set(), set(), False
    if op == 0:
        fn = word & 63
        if fn in (0, 2, 3):
            reads, writes = {_g(rt)}, {_g(rd)}
        elif fn in (4, 6, 7, 0x14, 0x16, 0x17):        # sllv..srav, dsllv..dsrav
            reads, writes = {_g(rt), _g(rs)}, {_g(rd)}
        elif fn == 8:
            reads = {_g(rs)}
        elif fn == 9:
            reads, writes, call = {_g(rs)}, {_g(rd)}, True
        elif fn in (0x10, 0x12):
            writes = {_g(rd)}
        elif fn in (0x11, 0x13):
            reads = {_g(rs)}
        elif 0x18 <= fn <= 0x1F:
            reads = {_g(rs), _g(rt)}
        elif 0x20 <= fn <= 0x2F:
            reads, writes = {_g(rs), _g(rt)}, {_g(rd)}
        elif 0x30 <= fn <= 0x36:
            reads = {_g(rs), _g(rt)}
        elif fn in (0x38, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F):   # dsll..dsra32
            reads, writes = {_g(rt)}, {_g(rd)}
    elif op == 1:
        reads = {_g(rs)}
        call = rt in (0x10, 0x11, 0x12, 0x13)
    elif op == 3:
        call = True
    elif op in (4, 5, 0x14, 0x15):
        reads = {_g(rs), _g(rt)}
    elif op in (6, 7, 0x16, 0x17):
        reads = {_g(rs)}
    elif 8 <= op <= 0xE or op in (0x18, 0x19, 0x1A, 0x1B, 0x37):   # + daddi(u), ldl/ldr, ld
        reads, writes = {_g(rs)}, {_g(rt)}
    elif op == 0xF:
        writes = {_g(rt)}
    elif op == 0x11:
        fmt, ft, fs, fd = rs, rt, rd, (word >> 6) & 31
        fn = word & 63
        if fmt in (0, 1):                  # mfc1, dmfc1
            reads, writes = {_f(fs)}, {_g(rt)}
        elif fmt == 2:                     # cfc1
            writes = {_g(rt)}
        elif fmt in (4, 5):                # mtc1, dmtc1
            reads, writes = {_g(rt)}, {_f(fs)}
        elif fmt == 6:                     # ctc1
            reads = {_g(rt)}
        elif fmt in (0x10, 0x11, 0x14, 0x15):
            if fn <= 3:
                reads, writes = {_f(fs), _f(ft)}, {_f(fd)}
            elif fn <= 0x0F or 0x20 <= fn <= 0x25:
                reads, writes = {_f(fs)}, {_f(fd)}
            elif fn >= 0x30:
                reads = {_f(fs), _f(ft)}
    elif 0x20 <= op <= 0x27:
        # lwl/lwr merge into rt, but IDO emits them as a pair that writes
        # the whole register: not a read of rt.
        reads, writes = {_g(rs)}, {_g(rt)}
    elif 0x28 <= op <= 0x2E or op == 0x3F:  # + sd
        reads = {_g(rs), _g(rt)}
    elif op in (0x31, 0x35):               # lwc1, ldc1
        reads, writes = {_g(rs)}, {_f(rt)}
    elif op in (0x39, 0x3D):               # swc1, sdc1
        reads = {_g(rs), _f(rt)}
    elif op in (0x2F, 0x33):               # cache, pref
        reads = {_g(rs)}
    return reads, writes, call


def _is_save(word, restores=None):
    """A store of a callee-saved register to the stack (prologue save).
    restores: optional context from _save_context(body); then a store whose
    register the body neither reloads from that slot nor ever writes, in a body
    that makes calls, is an outgoing STACK ARGUMENT, i.e. a read (Phase 11,
    w31r: retail control_cell_text passes its s7 register parameter on with
    `jal camera_auto_follow; sw s7,24(sp)`; taken for a save, s7 was missing
    from its live-in set)."""
    op, base, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
    if base != SP:
        return False
    if op == 0x2B:
        ok = rt in _CALLEE_SAVED_GPR
    else:
        ok = op in (0x39, 0x3D) and rt >= 20
    if not ok or restores is None:
        return ok
    loads, written, calls = restores
    if (_LOAD_OF[op], rt, word & 0xFFFF) in loads or not calls:
        return True
    key = ("r", rt) if op == 0x2B else ("f", rt)
    return key in written or (op != 0x2B and ("f", rt + 1) in written)


_LOAD_OF = {0x2B: 0x23, 0x39: 0x31, 0x3D: 0x35}        # sw -> lw, swc1 -> lwc1, sdc1 -> ldc1


def _restores(words):
    """Save-detection context of a body: ({(op, rt, offset)} of every load from
    the stack, {register keys the body writes}, whether it makes a call)."""
    loads = {(w >> 26, (w >> 16) & 31, w & 0xFFFF) for w in words
             if (w >> 26) in (0x23, 0x31, 0x35) and ((w >> 21) & 31) == SP}
    written, calls = set(), False
    for w in words:
        _r, wr, call = decode(w)
        written |= wr
        calls = calls or bool(call)
    return loads, written, calls


_ARG_REGS = (("r", 4), ("r", 5), ("r", 6), ("r", 7), ("f", 12), ("f", 14))
_NOT_PARAMS = {("r", ZERO), ("r", GP), ("r", SP), ("r", RA)}


def _abi(key):
    kind, n = key
    return n in _O32_GPR if kind == "r" else n in _O32_FPR


def reg_name(key):
    kind, n = key
    return GPR_NAMES[n] if kind == "r" else "f%d" % n


def _flow(words):
    """Per instruction, its successor indices (delay slots modelled; branch
    likely skips its slot when not taken) and whether it is a call's delay
    slot. Targets outside the body, `jr` other than via ra and jump tables
    end the path (callers fall back to address order for unreached code)."""
    n = len(words)
    succ = [[] for _ in range(n)]
    call_slot = [False] * n
    for i, w in enumerate(words):
        op, rs, rt = w >> 26, (w >> 21) & 31, (w >> 16) & 31
        off = (w & 0xFFFF) - (0x10000 if w & 0x8000 else 0)
        target = i + 1 + off
        branch = (op in (4, 5, 6, 7) or (op == 1 and rt in (0, 1, 0x10, 0x11))
                  or (op == 0x11 and rs == 8 and not (rt & 2)))
        likely = (op in (0x14, 0x15, 0x16, 0x17) or (op == 1 and rt in (2, 3, 0x12, 0x13))
                  or (op == 0x11 and rs == 8 and rt & 2))
        if i + 1 >= n:
            continue
        if branch or likely:
            succ[i] = [i + 1] + ([i + 2] if likely and i + 2 < n else [])
            always = op == 4 and rs == 0 and rt == 0          # `b`
            slot = [t for t in (target,) if 0 <= t < n] + ([] if likely or always or i + 2 >= n else [i + 2])
            succ[i + 1] = ("SLOT", slot)
            if op == 1 and rt & 0x10:
                call_slot[i + 1] = True
        elif op == 2:
            succ[i] = [i + 1]
            succ[i + 1] = ("SLOT", [])          # j: leaves (or loops) -- end the path
        elif op == 3 or (op == 0 and (w & 63) == 9):
            succ[i] = [i + 1]
            succ[i + 1] = ("SLOT", [i + 2] if i + 2 < n else [])
            call_slot[i + 1] = True
        elif op == 0 and (w & 63) == 8:
            succ[i] = [i + 1]
            succ[i + 1] = ("SLOT", [])
        elif not succ[i]:
            succ[i] = [i + 1]
    return [s[1] if isinstance(s, tuple) else s for s in succ], call_slot


def register_params(words, include_o32=False, call_reads=False, call_effects=None):
    """Sorted names of registers read on some path from the entry before any
    write that o32 does not pass in (see module doc): a must-write dataflow
    over the body's branches; code the flow does not reach (jump tables) is
    judged in address order. include_o32: also the o32 argument registers
    (a0-a3, f12, f14): the whole live-in set (regmap_verify). call_reads: a
    call counts as reading a0-a3/f12/f14 (arguments forwarded unread).
    call_effects: optional function(call word) -> (reads, writes) register
    key sets of that callee (interprocedural mode, regmap_verify --derive:
    a register the callee neither reads nor writes survives the call, and a
    register the callee reads is read at the call), or None for the default
    (every caller-save register written, nothing read)."""
    n = len(words)
    dec = [decode(w) for w in words]
    succ, call_slot = _flow(words)

    def effects(i):                # i = delay slot index; the call is at i - 1
        e = call_effects(words[i - 1]) if call_effects is not None and i > 0 else None
        return e if e is not None else (frozenset(), _CALL_WRITES)
    IN = [None] * n
    IN[0] = frozenset()
    work = [0]
    while work:
        i = work.pop()
        out = IN[i] | dec[i][1] | (effects(i)[1] if call_slot[i] else set())
        for j in succ[i]:
            new = out if IN[j] is None else IN[j] & out
            if IN[j] is None or new != IN[j]:
                IN[j] = frozenset(new)
                work.append(j)
    found, linear = set(), set()
    restores = _restores(words)
    for i, w in enumerate(words):
        reads, writes, call = dec[i]

        have = IN[i] if IN[i] is not None else linear
        if call_reads and call_slot[i]:
            # the call (after this delay slot) passes a0-a3/f12/f14 on: an
            # argument register the body forwards unread is live-in too
            found.update(r for r in _ARG_REGS if r not in have and r not in writes)
        if call_effects is not None and call_slot[i]:
            found.update(r for r in effects(i)[0] if r not in have and r not in writes and r not in _NOT_PARAMS
                         and (not _abi(r) or include_o32))
        if not _is_save(w, restores):
            found.update(r for r in reads if r not in have and
                         (not _abi(r) or (include_o32 and r not in _NOT_PARAMS)))
        linear |= writes
        if call:
            linear |= (effects(i + 1)[1] if i + 1 < n else _CALL_WRITES)
    found.discard(("r", ZERO))
    return sorted((reg_name(k) for k in found), key=lambda s: (s[0] == "f", len(s), s))


def body_words(image, vram, size):
    base, data = image
    off = vram - base
    if off < 0 or off + size > len(data) or size % 4:
        return None
    return list(struct.unpack_from(">%dI" % (size // 4), data, off))


def called_functions(elf):
    """Names the object reaches with jal: R_MIPS_26 against undefined symbols."""
    return sorted({r["sym"]["name"] for r in elf.get("relocs", ())
                   if r["type"] == R_MIPS_26 and r.get("sym") and r["sym"]["shndx"] == 0
                   and r["sym"]["name"]})


def detect(callees, table, functions, images):
    """{callee: why} for callees (names) whose retail body reads non-o32
    registers. table: {name: vram} (replace.address_table); functions:
    {population: [{name, vram, size}]}; images: {population: (vram, bytes)}."""
    extents = {}
    for pop, funcs in (functions or {}).items():
        for f in funcs:
            extents.setdefault(f["vram"], (pop, f["size"]))
    out = {}
    for name in callees:
        vram = table.get(name)
        if vram is None or vram not in extents:
            continue
        pop, size = extents[vram]
        image = (images or {}).get(pop)
        words = body_words(image, vram, size) if image else None
        if not words:
            continue
        regs = register_params(words)
        if regs:
            out[name] = "register parameters " + ",".join(regs)
    return out


def scan(functions, images, populations=("game",)):
    """{name: [regs]} over whole populations (calibration)."""
    out = {}
    for pop in populations:
        for f in functions.get(pop, []):
            words = body_words(images[pop], f["vram"], f["size"]) if pop in images else None
            regs = register_params(words) if words else []
            if regs:
                out[f["name"]] = regs
    return out
