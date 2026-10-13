#!/usr/bin/env python3
"""regmap_verify: confirm or refute a register-parameter mapping of an internal
(IDO -O3 IPA) game function from its retail call sites (recomp Phase 7, w27r).

    python3 -m tools.recomp.regmap_verify NAME 'a0=handle,t3=mark,s0=buf:ptr' [--scripts ...] [--remote rocky]

A mapping item is REG=LABEL[:KIND], KIND one of ptr (KSEG0 RAM address), int
(|value| < 0x10000 as signed 32-bit), float (an f register, or a GPR holding
float bits), any. Evidence, three kinds:

  static callee   the retail body's live-in registers (read on some path before
                  any write; tools/recomp/ipa_detect.py dataflow, o32 argument
                  registers included). A claimed register that is not live-in is
                  not a parameter; a live-in register missing from the claim is.
  static callers  every `jal NAME` in the game image: the caller, and for each
                  claimed / live-in register the instruction that last set it
                  before the call (delay slot first, then a backward linear scan
                  of the caller, at most 64 instructions; "not set nearby" means
                  the value comes from further back: a loop-carried or saved
                  register).
  dynamic         a capture build with a probe on NAME records at every call the
                  return address and ALL integer registers plus the even FP
                  registers; the lockstep scripts run headless. Per call site
                  (return address - 8): calls, and per register the number of
                  distinct values, range and kind.

Verdict: CONSISTENT when every claimed register is live-in, is of the claimed kind
at every call, and no live-in register is unclaimed; else REFUTED with the reasons.
CONSISTENT is evidence for a prototype, not a proof of meaning (labels are yours).
Exit 0 CONSISTENT, 1 REFUTED, 3 never called (static evidence only), 2 error.
Runs on the recomp host; `--remote rocky` from the Pi. Game functions only.
"""
import argparse
import json
import os
import re
import shlex
import struct
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[1]
if str(HERE) not in sys.path:
    sys.path.insert(0, str(HERE))
import draft_check as dc  # noqa: E402
import ipa_detect  # noqa: E402
import oracle  # noqa: E402

GPRS = ["at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "s8", "ra"]
FPRS = ["f%d" % n for n in range(0, 32, 2)]
KINDS = ("ptr", "int", "float", "any")
RAM_LO, RAM_HI = 0x80000000, 0x80800000


class VerifyError(RuntimeError):
    pass


def parse_mapping(spec):
    """'a0=handle,t3=mark:int' (or 'handle=a0') -> [(reg, label, kind)]."""
    out = []
    regre = re.compile(r"^(?:[atsv][0-9]|s8|fp|at|f([0-9]|[12][0-9]|3[01]))$")
    for item in [x.strip() for x in spec.split(",") if x.strip()]:
        if "=" not in item:
            raise VerifyError(f"mapping item {item!r} is not REG=LABEL[:KIND]")
        a, b = [x.strip() for x in item.split("=", 1)]
        kind = "any"
        for side in (a, b):
            if ":" in side:
                base, _, k = side.partition(":")
                if k not in KINDS:
                    raise VerifyError(f"kind {k!r} not one of {', '.join(KINDS)}")
                kind = k
        a, b = a.split(":")[0], b.split(":")[0]
        if regre.match(b) and not regre.match(a):
            a, b = b, a
        if not regre.match(a):
            raise VerifyError(f"{item!r}: no register named (a0..a3, t0..t9, s0..s8, v0/v1, fN)")
        reg = "s8" if a == "fp" else a
        if reg.startswith("f") and int(reg[1:]) % 2:
            raise VerifyError(f"{reg}: name the even FP register")
        out.append((reg, b, kind))
    if not out:
        raise VerifyError("empty mapping")
    return out


# ------------------------------------------------------------------ static evidence

_LOADS = {0x20: "lb", 0x21: "lh", 0x23: "lw", 0x24: "lbu", 0x25: "lhu", 0x31: "lwc1", 0x35: "ldc1"}
_IMM = {0x08: "addi", 0x09: "addiu", 0x0A: "slti", 0x0B: "sltiu", 0x0C: "andi", 0x0D: "ori", 0x0E: "xori"}
_R3 = {0x20: "add", 0x21: "addu", 0x22: "sub", 0x23: "subu", 0x24: "and", 0x25: "or", 0x26: "xor",
       0x27: "nor", 0x2A: "slt", 0x2B: "sltu"}


def disasm(word):
    """A short mnemonic for the instructions that set argument registers."""
    g = ipa_detect.GPR_NAMES
    op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
    imm = word & 0xFFFF
    simm = imm - 0x10000 if imm & 0x8000 else imm
    if word == 0:
        return "nop"
    if op == 0:
        fn = word & 63
        if fn == 0x25 and rt == 0:
            return "move %s, %s" % (g[rd], g[rs])
        if fn == 0x21 and rt == 0:
            return "move %s, %s" % (g[rd], g[rs])
        if fn in _R3:
            return "%s %s, %s, %s" % (_R3[fn], g[rd], g[rs], g[rt])
        if fn in (0, 2, 3):
            return "%s %s, %s, %d" % ({0: "sll", 2: "srl", 3: "sra"}[fn], g[rd], g[rt], (word >> 6) & 31)
        if fn in (0x10, 0x12):
            return "%s %s" % ({0x10: "mfhi", 0x12: "mflo"}[fn], g[rd])
        return "special.%02X" % fn
    if op == 0xF:
        return "lui %s, 0x%X" % (g[rt], imm)
    if op in _IMM:
        return "%s %s, %s, %s" % (_IMM[op], g[rt], g[rs], hex(imm) if op in (0xC, 0xD, 0xE) else simm)
    if op in _LOADS:
        dst = ("f%d" % rt) if op in (0x31, 0x35) else g[rt]
        return "%s %s, %d(%s)" % (_LOADS[op], dst, simm, g[rs])
    if op == 0x11:
        fmt, fs, fd = rs, rd, (word >> 6) & 31
        if fmt == 4:
            return "mtc1 %s, f%d" % (g[rt], fs)
        if fmt == 0:
            return "mfc1 %s, f%d" % (g[rt], fs)
        if fmt in (0x10, 0x11) and (word & 63) == 6:
            return "mov.%s f%d, f%d" % ("s" if fmt == 0x10 else "d", fd, fs)
        return "cop1 %08X" % word
    return "%08X" % word


def _key(reg):
    return ("f", int(reg[1:]) & ~1) if reg.startswith("f") else ("r", ipa_detect.GPR_NAMES.index(reg))


def call_sites(image, functions, target):
    """[(site vram, caller name, caller vram, caller size)] of `jal target`."""
    base, data = image
    out = []
    for f in functions:
        words = ipa_detect.body_words(image, f["vram"], f["size"])
        if not words:
            continue
        for i, w in enumerate(words):
            if w >> 26 == 3:
                at = f["vram"] + 4 * i
                if (((at + 4) & 0xF0000000) | ((w & 0x03FFFFFF) << 2)) == target:
                    out.append((at, f["name"], f["vram"], f["size"]))
    return out


def setter(image, site, caller_vram, reg, limit=64):
    """(vram, text) of the instruction that last set `reg` before the call at
    `site`: the delay slot, then backwards in the caller; None if not found."""
    key = _key(reg)
    words = ipa_detect.body_words(image, site + 4, 4)
    if words:
        _r, wr, _c = ipa_detect.decode(words[0])
        if key in wr:
            return site + 4, disasm(words[0]) + "   (delay slot)"
    at = site - 4
    n = 0
    while at >= caller_vram and n < limit:
        w = ipa_detect.body_words(image, at, 4)[0]
        _r, wr, call = ipa_detect.decode(w)
        if key in wr:
            return at, disasm(w)
        if call and key in ipa_detect._CALL_WRITES:
            return at, "(clobbered by the call at 0x%08X: value is that callee's leftover)" % at
        at -= 4
        n += 1
    return None


# ------------------------------------------------------------------ derivation (--derive / draft_check --probe-callees)

_FP_SAVED = {("f", n) for n in range(20, 32, 2)}
_GPR_SAVED = {("r", n) for n in ipa_detect._CALLEE_SAVED_GPR}


class Interproc:
    """Register effects of game functions, transitively through their static
    `jal` callees: reads(vram) = live-in set (o32 registers included; a
    callee's reads count at the call), writes(vram) = caller-save registers
    it may write plus callee-saved ones it writes without a prologue save.
    jalr, calls outside the game image and recursion fall back to the default
    (every caller-save register written, nothing read). Needed because IDO -O3
    keeps a register parameter alive across a call to a callee that does not
    touch it (sound_update_channel reads `force` in t0 after its first call)."""

    def __init__(self, image, funcs):
        self.image = image
        self.by_vram = {f["vram"]: f for f in funcs}
        self.memo = {}
        self.busy = set()

    def target(self, word, at=None):
        if word >> 26 != 3:
            return None
        return 0x80000000 | ((word & 0x03FFFFFF) << 2)

    def call_effects(self, word):
        t = self.target(word)
        if t is None or t not in self.by_vram or t in self.busy:
            return None
        return self.effects(t)

    def effects(self, vram):
        if vram in self.memo:
            return self.memo[vram]
        f = self.by_vram[vram]
        words = ipa_detect.body_words(self.image, vram, f["size"])
        if not words:
            return None
        self.busy.add(vram)
        try:
            live = ipa_detect.register_params(words, include_o32=True, call_effects=self.call_effects)
            keys = {_key(r) for r in live}
            writes, saved = set(), set()
            for w in words:
                _r, wr, call = ipa_detect.decode(w)
                if ipa_detect._is_save(w):
                    rt = (w >> 16) & 31
                    saved.add(("f", rt & ~1) if w >> 26 in (0x39, 0x3D) else ("r", rt))
                writes |= wr
                if call:
                    e = self.call_effects(w)
                    writes |= set(e[1]) if e is not None else set(ipa_detect._CALL_WRITES)
            writes.add(("r", ipa_detect.RA)) if any(ipa_detect.decode(w)[2] for w in words) else None
            out_w = {k for k in writes if k in ipa_detect._CALL_WRITES or
                     ((k in _GPR_SAVED or k in _FP_SAVED) and k not in saved)}
            res = (frozenset(keys), frozenset(out_w))
        finally:
            self.busy.discard(vram)
        self.memo[vram] = res
        return res

    def unsaved(self, vram):
        e = self.effects(vram)
        return sorted((ipa_detect.reg_name(k) for k in (e[1] if e else ()) if k in _GPR_SAVED or k in _FP_SAVED),
                      key=_reg_order)


def _reg_order(r):
    if r.startswith("f"):
        return (2, int(r[1:]))
    return (0 if r in ("a0", "a1", "a2", "a3") else 1, ipa_detect.GPR_NAMES.index(r))


_O32_SEQ = ("a0", "a1", "a2", "a3")


def static_kind(text):
    """Kind of a parameter from the instruction that set it (None: unknown)."""
    if not text or text.startswith("("):
        return None
    t = text.split("   ")[0]
    if t.startswith(("lwc1", "ldc1", "mov.s", "mov.d", "mtc1")) or re.match(r"^\S+ f\d+", t):
        return "float"
    m = re.match(r"^lui \w+, 0x([0-9A-F]+)", t)
    if m:
        return "ptr" if 0x8000 <= int(m.group(1), 16) < 0x8080 else None
    if t.startswith(("lb ", "lbu ", "lh ", "lhu ", "slti", "sltiu", "andi", "sltu", "slt ")) or \
            t.startswith("move ") and t.endswith(", zero"):
        return "int"
    m = re.match(r"^addiu (\w+), (\w+), (-?\d+)", t)
    if m and m.group(2) == "zero":
        return "int"
    if m and m.group(1) == m.group(2) and m.group(1) != "sp":
        return "ptr"            # the low half of a lui/addiu address pair
    return None


def derive(name, image, funcs, stats=None, facts_unsaved=None):
    """{params: [(reg, kind)], clobbers, ret, evidence} for NAME: the live-in
    registers of its retail body (interprocedural), ordered a0-a3 first, then
    by where the static call sites set them (IDO evaluates IPA register
    arguments in prototype order, w26d: earliest set = earlier parameter), FP
    last on ties; kinds from the dynamic capture (stats) or the setters."""
    fn = next((f for f in funcs if f["name"] == name), None)
    if fn is None:
        raise VerifyError(f"{name} is not a game function")
    ip = Interproc(image, funcs)
    eff = ip.effects(fn["vram"])
    if eff is None:
        raise VerifyError(f"{name}: no body words")
    live = sorted((ipa_detect.reg_name(k) for k in eff[0] if k not in ipa_detect._NOT_PARAMS), key=_reg_order)
    sites = call_sites(image, funcs, fn["vram"])
    pos, kinds_static = {}, {}
    for at, _caller, cv, _cs in sites:
        for r in live:
            s = setter(image, at, cv, r)
            if s is None:
                continue
            # delay slot = latest (distance 0); else distance back from the call
            d = 0 if s[0] == at + 4 else (at - s[0]) // 4 + 1
            pos.setdefault(r, []).append(d)
            k = static_kind(s[1])
            if k:
                kinds_static.setdefault(r, set()).add(k)

    # Order: a0-a3 (o32 slots), then the other GPRs, then FP, each by register
    # number. The call sites' setter order is NOT used: the scheduler moves
    # the argument setters (menu_input_process sets func_800B6748's t2..a0 in
    # reverse, engine_sound_sync sets f22..f28 forwards with f28 in the delay
    # slot); every known IPA callee numbers its parameters upwards
    # (func_800B6748 t0-t2, entity_transform_calc f22-f28). The order is what
    # the draft's C call must follow: a guess to check against the source.
    params = []
    for r in sorted(live, key=_reg_order):
        kind = None
        if stats:
            allv = set().union(*(st["regs"].get(r, set()) for st in stats.values()))
            k = kind_of(allv, r)
            kind = k if k in ("ptr", "int", "float") else ("int" if k == "zero" else None)
        if kind is None and len(kinds_static.get(r, ())) == 1:
            kind = next(iter(kinds_static[r]))
        if kind is None and r.startswith("f"):
            kind = "float"
        params.append((r, kind or "int"))
    clobbers = ip.unsaved(fn["vram"])
    for r in facts_unsaved or ():
        if r not in clobbers:
            clobbers.append(r)
    clobbers = [r for r in sorted(set(clobbers), key=_reg_order) if r not in [p for p, _k in params]]
    own = ipa_detect.body_words(image, fn["vram"], fn["size"]) or []
    writes_v0 = any(("r", 2) in ipa_detect.decode(w)[1] for w in own)
    return {"function": name, "params": params, "clobbers": clobbers,
            "ret": "s32" if writes_v0 else "void", "sites": len(sites),
            "order_evidence": {r: sorted(pos.get(r, [])) for r in live}}


_CTYPE = {"ptr": "void *", "int": "s32", "float": "f32"}


def callee_spec(d):
    """draft_check --callee syntax for a derive() result."""
    args = ", ".join("%s%sp%d" % (_CTYPE[k], "" if _CTYPE[k].endswith("*") else " ", i)
                     for i, (_r, k) in enumerate(d["params"])) or "void"
    regs = ",".join(r for r, _k in d["params"])
    spec = "%s %s(%s)=%s" % (d["ret"], d["function"], args, regs)
    if d["clobbers"]:
        spec += "/" + ",".join(d["clobbers"])
    return spec


# ------------------------------------------------------------------ dynamic evidence

def kind_of(values, reg):
    """Classify a set of 32-bit values."""
    if not values:
        return "-"
    if all(v == 0 for v in values):
        return "zero"
    if all(RAM_LO <= v < RAM_HI or v == 0 for v in values):
        return "ptr"
    sv = [v - (1 << 32) if v & 0x80000000 else v for v in values]
    if all(-0x10000 < v < 0x10000 for v in sv):
        return "int"
    if reg.startswith("f") or all(_plausible_float(v) for v in values):
        return "float"
    return "mixed"


def _plausible_float(v):
    if v == 0:
        return True
    e = (v >> 23) & 0xFF
    return 0x60 <= e <= 0x9F


def kind_ok(claimed, seen):
    if claimed == "any" or seen in ("-",):
        return True
    if seen == "zero":
        return claimed in ("ptr", "int", "float")
    return claimed == seen


def fmt_vals(values):
    vs = sorted(values)
    if len(vs) <= 3:
        return ",".join("%08X" % v for v in vs)
    return "%08X..%08X" % (vs[0], vs[-1])


# ------------------------------------------------------------------ main

def main(argv=None):
    argv = list(sys.argv[1:] if argv is None else argv)
    ap = argparse.ArgumentParser(prog="python3 -m tools.recomp.regmap_verify", description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("name")
    ap.add_argument("mapping", nargs="?", default=None,
                    help="REG=LABEL[:KIND],... e.g. 'a0=handle:int,t3=mark:int,s0=buf:ptr' (not with --derive)")
    ap.add_argument("--derive", action="store_true",
                    help="no claim: derive the register-parameter mapping from the retail body and its call "
                    "sites (interprocedural live-in, setter order, kinds from the capture unless --static-only) "
                    "and print it in draft_check --callee syntax")
    ap.add_argument("--scripts", default=None, help="comma list or all (default: census, else all)")
    ap.add_argument("--static-only", action="store_true", help="no capture run")
    ap.add_argument("--jobs", type=int, default=6)
    ap.add_argument("--json", default=None)
    ap.add_argument("--remote", default=None, help="run on this ssh host (e.g. rocky)")
    ap.add_argument("--remote-tree", default=dc.REMOTE_TREE)
    args = ap.parse_args(argv)
    try:
        if args.derive == bool(args.mapping):
            raise VerifyError("give a mapping, or --derive without one")
        mapping = parse_mapping(args.mapping) if args.mapping else None
        if args.remote:
            fwd, skip = [], False
            for a in argv:
                if skip:
                    skip = False
                    continue
                if a in ("--remote", "--remote-tree"):
                    skip = True
                    continue
                if not a.startswith(("--remote=", "--remote-tree=")):
                    fwd.append(a)
            cmd = "cd %s && python3 -m tools.recomp.regmap_verify %s" % (
                args.remote_tree, " ".join(shlex.quote(a) for a in fwd))
            return subprocess.run(["ssh", args.remote, cmd]).returncode
        if args.derive:
            return run_derive(args)
        return run(args, mapping)
    except (VerifyError, dc.DCError, oracle.OracleError) as exc:
        print("regmap_verify: ERROR: %s" % exc, file=sys.stderr)
        return 2


def run(args, mapping):
    p = dc.paths()
    sys.path.insert(0, str(HERE))
    import gen_config as gc
    rom = gc.read_rom(p["rom"])
    images = gc.extract_images(rom)
    image = images["game"]
    funcs = gc.load_game_functions(REPO / "asm" / "us" / "blob")
    funcs = [f for f in funcs if f.get("name") and f.get("size")]
    fn = next((f for f in funcs if f["name"] == args.name), None)
    if fn is None:
        raise VerifyError(f"{args.name} is not a game function")
    words = ipa_detect.body_words(image, fn["vram"], fn["size"])
    # Interprocedural (Phase 8): a register a callee does not write survives the
    # call, and a register a callee reads is read at the call (sound_update_channel
    # reads t0 after its first call; the w24t intra-procedural flow missed it).
    ip = Interproc(image, funcs)
    live = ipa_detect.register_params(words, include_o32=True, call_effects=ip.call_effects)
    live = [r for r in live if r not in ("zero", "gp", "sp", "ra")]
    fwd = [r for r in ipa_detect.register_params(words, include_o32=True, call_reads=True) if r not in live]
    claimed = [r for r, _l, _k in mapping]
    print("regmap_verify %s (0x%08X, %d B): claim %s" % (args.name, fn["vram"], fn["size"],
                                                        ", ".join("%s=%s%s" % (r, l, "" if k == "any" else ":" + k)
                                                                  for r, l, k in mapping)))
    print("  static callee: live-in registers (read before written on some path, through its callees): %s"
          % (", ".join(live) or "none"))
    if fwd:
        print("  static callee: unset when a call is made (forwarded to a callee if they are arguments): %s"
              % ", ".join(fwd))
    facts = dc.register_params_fact(args.name)
    if facts:
        print("  frontier facts register_params: %s" % ", ".join(facts))
    problems = []
    for r in claimed:
        if r not in live and r not in fwd:
            problems.append("%s is claimed but the callee never reads it before writing it" % r)
        elif r not in live:
            print("  note: %s is only passed on to a callee unread (a forwarded parameter)" % r)
    for r in live:
        if r not in claimed:
            problems.append("%s is live-in but not claimed (a parameter, or a value the callee reads "
                            "uninitialised on some path)" % r)
    sites = call_sites(image, funcs, fn["vram"])
    show = claimed + [r for r in live if r not in claimed]
    print("  static callers: %d jal site(s)" % len(sites))
    for at, caller, cv, _cs in sites:
        print("    0x%08X in %s" % (at, caller))
        for r in show:
            s = setter(image, at, cv, r)
            print("      %-4s %s" % (r, ("0x%08X  %s" % s) if s else "not set within 64 instructions before the call"))
    verdict = None
    stats = {}
    if not args.static_only:
        stats, ncalls, scripts = dynamic(args, fn, p, sites)
        if ncalls == 0:
            print("  dynamic: NEVER CALLED in %s" % ", ".join(scripts))
            verdict = "NEVER CALLED"
        else:
            print("  dynamic: %d calls over %s" % (ncalls, ", ".join(scripts)))
            for site in sorted(stats):
                st = stats[site]
                caller = next((f["name"] for f in funcs if f["vram"] <= site < f["vram"] + f["size"]), "?")
                print("    site 0x%08X (%s): %d calls%s" % (site, caller, st["calls"],
                                                           "" if any(s[0] == site for s in sites) else
                                                           "  [indirect: jalr / not a static jal]"))
                for r in show:
                    vals = st["regs"].get(r, set())
                    k = kind_of(vals, r)
                    tag = "claimed" if r in claimed else "UNCLAIMED live-in"
                    print("      %-4s %-17s %3d distinct  %-19s %s" % (r, tag, len(vals), fmt_vals(vals), k))
            for r, label, kind in mapping:
                allv = set().union(*(st["regs"].get(r, set()) for st in stats.values()))
                seen = kind_of(allv, r)
                if not kind_ok(kind, seen):
                    problems.append("%s (%s) claimed %s, seen %s (%s)" % (r, label, kind, seen, fmt_vals(allv)))
            others = [r for r in GPRS + FPRS if r not in show and r != "ra"]
            varying = []
            for r in others:
                per_site = [len(st["regs"].get(r, ())) for st in stats.values()]
                if per_site and max(per_site) > 1:
                    varying.append(r)
            print("  other registers (not live-in): varying at entry: %s (leftovers; the callee does not read "
                  "them before writing)" % (", ".join(varying) or "none"))
    if verdict is None:
        verdict = "REFUTED" if problems else "CONSISTENT"
    print()
    for pr in problems:
        print("  - " + pr)
    if verdict == "CONSISTENT":
        print("VERDICT: CONSISTENT: the claimed registers are exactly the live-in set%s." % (
            ", each of the claimed kind at every captured call" if stats else " (static evidence only)"))
    elif verdict == "NEVER CALLED":
        print("VERDICT: %s (static evidence only: %s)" % (
            "NEVER CALLED", "consistent" if not problems else "REFUTED: see above"))
    else:
        print("VERDICT: REFUTED (%d problem(s) above)" % len(problems))
    if args.json:
        Path(args.json).write_text(json.dumps({
            "function": args.name, "mapping": mapping, "live_in": live, "problems": problems,
            "verdict": verdict, "sites": [[hex(a), c] for a, c, _v, _s in sites],
            "dynamic": {hex(k): {"calls": v["calls"], "regs": {r: sorted(x)[:64] for r, x in v["regs"].items()}}
                        for k, v in stats.items()}}, indent=1) + "\n")
    return {"CONSISTENT": 0, "REFUTED": 1, "NEVER CALLED": 3}[verdict]


def load_inputs(p):
    sys.path.insert(0, str(HERE))
    import gen_config as gc
    rom = gc.read_rom(p["rom"])
    image = gc.extract_images(rom)["game"]
    funcs = [f for f in gc.load_game_functions(REPO / "asm" / "us" / "blob") if f.get("name") and f.get("size")]
    return image, funcs


def unsaved_fact(name, frontier=REPO / "build" / "frontier.json"):
    try:
        f = json.loads(Path(frontier).read_text()).get("facts", {}).get(name) or {}
    except (OSError, ValueError):
        return []
    return list(f.get("unsaved_callee_regs") or ())


def derive_report(d, out=print):
    """Lines for a derive() result; returns the --callee spec or None (plain o32)."""
    regs = [r for r, _k in d["params"]]
    o32 = all(r in _O32_SEQ or r in ("f12", "f14") for r in regs)
    out("  derived parameters (prototype order): %s" % (
        ", ".join("%s:%s" % (r, k) for r, k in d["params"]) or "none"))
    for r in regs:
        ev = d["order_evidence"].get(r) or []
        if r not in _O32_SEQ:
            out("    %-4s set %s instruction(s) before the jal at the %d static site(s) (0 = delay slot)" % (
                r, ",".join(str(x) for x in ev) or "-", len(ev)))
    out("  callee-saved registers written without a save (clobbers): %s" % (", ".join(d["clobbers"]) or "none"))
    if o32 and not d["clobbers"]:
        out("  plain o32: no --callee needed")
        return None
    spec = callee_spec(d)
    out("  --callee '%s'" % spec)
    return spec


def run_derive(args):
    p = dc.paths()
    image, funcs = load_inputs(p)
    fn = next((f for f in funcs if f["name"] == args.name), None)
    if fn is None:
        raise VerifyError(f"{args.name} is not a game function")
    stats = None
    print("regmap_verify --derive %s (0x%08X, %d B)" % (args.name, fn["vram"], fn["size"]))
    if not args.static_only:
        sites = call_sites(image, funcs, fn["vram"])
        stats, ncalls, scripts = dynamic(args, fn, p, sites)
        print("  dynamic: %d calls over %s" % (ncalls, ", ".join(scripts)) if ncalls else
              "  dynamic: NEVER CALLED in %s (kinds from the setters only)" % ", ".join(scripts))
    d = derive(args.name, image, funcs, stats, unsaved_fact(args.name))
    spec = derive_report(d)
    print("  (derived from retail evidence, not proven: parameter ORDER is by register number and the "
          "return type is a guess; check both against the draft's source, then confirm with "
          "`regmap_verify NAME 'REG=label,...'`)")
    if args.json:
        Path(args.json).write_text(json.dumps(dict(d, callee=spec), indent=1) + "\n")
    return 0


def dynamic(args, fn, p, sites):
    """Capture build with a probe on NAME recording every register; run the
    scripts; {site: {"calls": n, "regs": {reg: set(values)}}}."""
    name = args.name
    regs = GPRS + FPRS
    entry = {"function": name, "population": "game", "mode": "capture", "state": "probe",
             "prototype": "void %s(void)" % name, "capture_args": regs, "capture_ret": "void",
             "note": "regmap_verify probe"}
    run_dir = dc.DC_HOME / "verify" / name / ("%s-%d" % (time.strftime("%Y%m%d-%H%M%S"), os.getpid()))
    run_dir.mkdir(parents=True, exist_ok=True)
    manifest = run_dir / "replacements.json"
    manifest.write_text(json.dumps({"note": "regmap_verify", "entries": {"draft": entry},
                                    "sets": {"draft": ["draft"]}}, indent=1))
    # The recompiled code does not write $ra on a jal: set it at every static
    # call site, so the capture's ra names the site (jalr sites stay unnamed).
    hooks = [{"func": caller, "before_vram": "0x%08X" % at,
              "text": "{ ctx->r31 = (gpr)(int32_t)0x%08XU; }" % (at + 8)} for at, caller, _v, _s in sites]
    (run_dir / "hooks.json").write_text(json.dumps(hooks, indent=1))
    cap = dc.build_side("cap", name + "@verify", manifest, run_dir, p, None, 8,
                        extra=["--extra-hooks", str(run_dir / "hooks.json")])
    if args.scripts in (None, "census"):
        scripts, covered = dc.census_scripts(name)
        if not covered:
            scripts = dc.all_scripts()
    elif args.scripts == "all":
        scripts = dc.all_scripts()
    else:
        scripts = [s.strip() for s in args.scripts.split(",") if s.strip()]
    stats = {}

    def one(script):
        sc = oracle.load_script(script)
        frames = sc["frames"] or 1800
        out = run_dir / "runs" / script
        out.mkdir(parents=True, exist_ok=True)
        cmd = oracle.host_command(cap["host"], cap["rom"], out / "o.log", out / "home", frames, sc["press"],
                                  frames / 30.0 * 4 + 120, calls=out / "calls", pak=sc["pak"])
        with open(out / "err", "w") as fe:
            rc = subprocess.run([str(c) for c in cmd], stdout=subprocess.DEVNULL, stderr=fe).returncode
        if rc != 0:
            raise VerifyError("host failed on %s: %s" % (script, dc._tail(out / "err", 5)))
        return out / "calls"
    with ThreadPoolExecutor(max(1, args.jobs)) as ex:
        logs = list(ex.map(one, scripts))
    ncalls = 0
    for log in logs:
        labels = []
        with open(log) as fh:
            for item in oracle.iter_calls(fh):
                if item[0] == "fn":
                    labels = [lab.split(":")[0] for lab in item[3]]
                elif item[0] == "c":
                    vals = dict(zip(labels, (int(v.split(":")[0], 16) for v in item[4])))
                    site = (vals.get("ra", 0) - 8) & 0xFFFFFFFF
                    st = stats.setdefault(site, {"calls": 0, "regs": {}})
                    st["calls"] += 1
                    ncalls += 1
                    for r, v in vals.items():
                        s = st["regs"].setdefault(r, set())
                        if len(s) < 4096:
                            s.add(v)
        try:
            log.unlink()        # large; the summary is printed
        except OSError:
            pass
    for script in scripts:
        for f in ("home",):
            import shutil
            shutil.rmtree(run_dir / "runs" / script / f, ignore_errors=True)
    for kind in ("cap",):
        for f in ("rush2049_recomp", "rush2049.recomp.z64"):
            try:
                (run_dir / kind / f).unlink()
            except OSError:
                pass
    return stats, ncalls, scripts


if __name__ == "__main__":
    sys.exit(main())
