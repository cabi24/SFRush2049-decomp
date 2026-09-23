"""Audit hand-written declarations against the binary they describe.

    python3 -m tools.conveyor.pipeline.declaudit [--header include/game_types.h]

The hand context (`include/game_types.h`) wins over the generated layer by
006 precedence, so a wrong declaration there is authoritative and silently
poisons every caller: a callee hand-declared `void` whose return value a
caller consumes makes m2c emit `M2C_ERROR(/* Read from unset register $v0 */)`
at each such call site. Seven such declarations were found on 2026-09-23;
`slot_state_setup` alone is called by 52 functions, and correcting the seven
moved five targets out of `partial_decomp` (two straight to `compiled`).

This is the evidence check the hand context never had: for every declared
function that is also an extracted target, compare the declaration against
the derived assembly (`build/m2c_asm/*.s`) of the function AND of its
callers. Exit status 1 when any contradiction is found, so it can gate a run.

These rules are a CANDIDATE filter, not an oracle: they read registers, not
dataflow. Ground truth is m2c's own analysis — a wrong `void` shows up as
`M2C_ERROR(/* Read from unset register $v0 */)` immediately after the call
in the raw output, so confirm a candidate by decompiling one of its callers
before editing the header (that is how audio_frame_sync was confirmed and
how billboard_render / countdown were rejected).

Evidence rules:

* `void_but_returns` — some caller reads $v0 after the call, in the same
  basic block, before anything rewrites it. The callee's own epilogue is NOT
  used: $v0 written near a return is very often scratch (`audio_frame_sync`,
  `visual_objects_update`, `func_80087110` all look like returns by that test
  and are not), whereas a consuming caller is exactly the condition that
  breaks decompilation.
* `too_few_params` — the body reads an argument register the declaration
  does not cover.
"""
import argparse
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
HEADER = REPO / "include" / "game_types.h"
ASM_DIR = REPO / "build" / "m2c_asm"
ARG_REGS = ("$a0", "$a1", "$a2", "$a3")

_PROTO_RE = re.compile(
    r"^\s*(?:extern\s+)?([A-Za-z_][\w \t\*]*?)\s+([A-Za-z_]\w*)\s*\(([^;{]*)\)\s*;", re.M)
# `\s*` not `\s+`: operand-less instructions (nop) must stay in the
# stream or delay-slot indexing silently shifts by one.
_INSN_RE = re.compile(r"^\s{4}(\S+)\s*(.*)$")
_LABEL_RE = re.compile(r"^\.L[0-9A-Fa-f]+:$")
_TARGET_RE = re.compile(r"(\.L[0-9A-Fa-f]+)\s*$", re.M)
# instructions whose first register operand is a source, not a destination
_NO_DEST = {"sw", "sb", "sh", "sd", "swc1", "sdc1", "jr", "j", "jal", "jalr",
            "mult", "multu", "div", "divu"}


def declarations(header=HEADER):
    """{name: (return_type, params)} from a hand-written header."""
    text = Path(header).read_text()
    return {m.group(2): (m.group(1).strip(), m.group(3).strip())
            for m in _PROTO_RE.finditer(text)}


def _instructions(path, with_labels=False):
    """[(mnemonic, operands)], or [(mnemonic, operands, starts_block)] when
    `with_labels`.

    The derived assembly labels EVERY instruction (`.L<vaddr>:`), so a label
    by itself says nothing about control flow — a block starts only where a
    branch or jump actually targets it. A scan that ignores real block
    boundaries reads a $v0 defined on another path as if it had flowed from
    the preceding instruction.
    """
    text = Path(path).read_text(errors="replace")
    targets = set(_TARGET_RE.findall(text))
    out, pending = [], None
    for line in text.splitlines():
        stripped = line.strip()
        if not line.startswith("    ") and _LABEL_RE.match(stripped):
            pending = stripped[:-1]
            continue
        match = _INSN_RE.match(line)
        if match:
            if with_labels:
                out.append((match.group(1), match.group(2), pending in targets))
            else:
                out.append((match.group(1), match.group(2)))
            pending = None
    return out


def _writes_v0(mnemonic, operands):
    if mnemonic in _NO_DEST or mnemonic.startswith("b"):
        return False
    return operands.split(",")[0].strip() == "$v0"


def _reads_v0(mnemonic, operands):
    regs = [p.strip() for p in re.split(r"[,()]", operands) if p.strip().startswith("$")]
    sources = regs if (mnemonic in _NO_DEST or mnemonic.startswith("b")) else regs[1:]
    return "$v0" in sources


def call_sites(asm_dir=ASM_DIR):
    """{callee: [(caller, instruction index)]} across every derived function."""
    sites = {}
    for path in sorted(Path(asm_dir).glob("*.s")):
        for index, (mnemonic, operands) in enumerate(_instructions(path)):
            if mnemonic == "jal":
                sites.setdefault(operands.strip(), []).append((path.stem, index))
    return sites


def _corpus(asm_dir):
    return (call_sites(asm_dir),
            {p.stem: _instructions(p, with_labels=True)
             for p in Path(asm_dir).glob("*.s")})


def return_value_is_used(name, asm_dir=ASM_DIR, window=6, corpus=None):
    """True if any caller consumes $v0 after calling `name`."""
    sites, bodies = corpus if corpus is not None else _corpus(asm_dir)
    for caller, index in sites.get(name, ()):
        insns = bodies[caller]
        # index + 1 is the delay slot, which executes before the callee runs.
        for step in range(index + 2, min(len(insns), index + 2 + window)):
            mnemonic, operands, starts_block = insns[step]
            if starts_block:
                break          # branch target: $v0 may be defined elsewhere
            if _reads_v0(mnemonic, operands):
                return True
            if _writes_v0(mnemonic, operands) or mnemonic in ("jal", "jalr"):
                break
    return False


def argument_registers_read(insns):
    """How many argument registers the function reads before writing them."""
    written, read = set(), set()
    for mnemonic, operands in insns:
        regs = [p.strip() for p in re.split(r"[,()]", operands)
                if p.strip().startswith("$")]
        if not regs:
            continue
        sources = regs if (mnemonic in _NO_DEST or mnemonic.startswith("b")) else regs[1:]
        for reg in sources:
            if reg in ARG_REGS and reg not in written:
                read.add(reg)
        if not (mnemonic in _NO_DEST or mnemonic.startswith("b")):
            written.add(regs[0])
        if mnemonic in ("jal", "jalr"):
            written.update(ARG_REGS)          # a call clobbers them
    return max([ARG_REGS.index(r) + 1 for r in read], default=0)


def audit(header=HEADER, asm_dir=ASM_DIR):
    """([(name, kind, detail)], declarations_checked)."""
    asm_dir = Path(asm_dir)
    corpus = _corpus(asm_dir)
    findings, checked = [], 0
    for name, (ret, params) in sorted(declarations(header).items()):
        path = asm_dir / f"{name}.s"
        if not path.is_file():
            continue          # not an extracted target, or not derived yet
        checked += 1
        if ret == "void" and return_value_is_used(name, asm_dir, corpus=corpus):
            findings.append((name, "void_but_returns", "a caller consumes its $v0"))
        declared = 0 if params in ("void", "") else len(
            [p for p in params.split(",") if p.strip()])
        actual = argument_registers_read(_instructions(path))
        if actual > declared:
            findings.append((name, "too_few_params",
                             f"declared {declared}, reads {actual} arg registers"))
    return findings, checked


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--header", default=str(HEADER))
    parser.add_argument("--asm-dir", default=str(ASM_DIR))
    args = parser.parse_args()
    findings, checked = audit(args.header, args.asm_dir)
    print(f"declaudit: {checked} hand declarations checked against derived asm")
    for name, kind, detail in findings:
        print(f"  {name}: {kind} — {detail}")
    print(f"contradictions: {len(findings)}")
    return 1 if findings else 0


if __name__ == "__main__":
    sys.exit(main())
