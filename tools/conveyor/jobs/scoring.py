"""Scoring wrapper: one canonical similarity metric for the whole pipeline.

Wraps the decomp-permuter Scorer (score 0 = byte-identical assembly) using the
toolkit-bundled mips objdump. Works both inside a toolkit on a node
($CONVEYOR_TOOLKIT set) and in the repo (vendored permuter + system objdump).
"""
import difflib
import os
import shutil
import subprocess
import sys
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import _permuter  # noqa: E402

Scorer = _permuter.Scorer


# A custom objdump_command replaces the permuter's executable AND arguments,
# so the MIPS disassembly flags must be restated here.
_MIPS_ARGS = "-drz -m mips:4300"


def _objdump_path():
    """Path to the mips objdump binary (toolkit preferred, else system), with
    the toolkit's bundled shared libs added to LD_LIBRARY_PATH as a side
    effect. Shared by the permuter Scorer command and the reloc-blind score."""
    toolkit = os.environ.get("CONVEYOR_TOOLKIT")
    if toolkit:
        bundled = Path(toolkit) / "bin" / "objdump"
        if bundled.is_file():
            lib = Path(toolkit) / "lib"
            if lib.is_dir():
                # Bundled shared libs travel with the binary.
                current = os.environ.get("LD_LIBRARY_PATH", "")
                os.environ["LD_LIBRARY_PATH"] = f"{lib}:{current}" if current else str(lib)
            return str(bundled)
    for name in ("mips-linux-gnu-objdump", "mips64-elf-objdump", "mips-elf-objdump"):
        found = shutil.which(name)
        if found:
            return found
    raise RuntimeError("no mips objdump available (toolkit or system)")


def objdump_command():
    return f"{_objdump_path()} {_MIPS_ARGS}"


_scorer_cache = {}


def get_scorer(target_o):
    """Scorer for a target object file, cached per path."""
    key = str(target_o)
    if key not in _scorer_cache:
        _scorer_cache[key] = Scorer(
            target_o=key,
            stack_differences=False,
            algorithm="difflib",
            debug_mode=False,
            ign_branch_targets=True,
            objdump_command=objdump_command(),
        )
    return _scorer_cache[key]


def score(target_o, cand_o):
    """Score a candidate object against a target object. 0 = identical."""
    result, _ = get_scorer(target_o).score(str(cand_o) if cand_o else None)
    return result


PENALTY_INF = Scorer.PENALTY_INF


# --- relocation-blind score (002) -------------------------------------------
#
# The secondary metric of specs/002-corpus-candidates. The true score above
# treats a candidate's zeroed relocation fields as differences from the
# target's linker-patched absolute addresses; this score masks exactly those
# fields (the ones the candidate's own relocation table names) so an
# instruction-identical candidate reads 0. Implemented per
# contracts/scoring-reloc-blind.md — the three helpers are pure so they can be
# unit-tested without objdump.

# Relocation type -> word mask (bits KEPT for comparison).
_RELOC_MASKS = {
    "R_MIPS_HI16": 0xFFFF0000,  # keep opcode/registers, zero the low 16-bit imm
    "R_MIPS_LO16": 0xFFFF0000,
    "R_MIPS_26": 0xFC000000,    # keep the 6-bit opcode, zero the 26-bit target
}
# Unknown relocation types are NOT normalized (keep the whole word).
_MASK_UNKNOWN = 0xFFFFFFFF


def _parse_text_words(objdump_stdout):
    """32-bit instruction words of the .text section, in address order, from
    `objdump -d` output. Reads the hex word column only (arch/mnemonic
    formatting is irrelevant), skipping `...` ellipsis and label lines."""
    words = []
    in_text = False
    for line in objdump_stdout.splitlines():
        if line.startswith("Disassembly of section"):
            in_text = line.rstrip().endswith(".text:")
            continue
        if not in_text:
            continue
        parts = line.split("\t")
        if len(parts) < 2:
            continue
        addr = parts[0].strip()
        if not addr.endswith(":"):
            continue
        try:
            int(addr[:-1], 16)
        except ValueError:
            continue
        token = parts[1].strip().split()
        if not token:
            continue
        try:
            words.append(int(token[0], 16))
        except ValueError:
            continue
    return words


def _parse_relocs(objdump_stdout):
    """[(instruction_index, mask)] for the candidate's .text relocations, from
    `objdump -r` output. instruction_index = offset // 4."""
    sites = []
    in_text = False
    for line in objdump_stdout.splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            in_text = "[.text]" in line
            continue
        if not in_text:
            continue
        stripped = line.strip()
        if not stripped:
            in_text = False  # a blank line ends the section's record block
            continue
        parts = stripped.split()
        if len(parts) < 2:
            continue
        try:
            offset = int(parts[0], 16)
        except ValueError:
            continue  # the "OFFSET TYPE VALUE" header row
        mask = _RELOC_MASKS.get(parts[1], _MASK_UNKNOWN)
        sites.append((offset // 4, mask))
    return sites


def _masked_diff(t_words, c_words, sites):
    """Differing-word count between two word sequences after masking each
    reloc site in both (the target is masked positionally at the candidate's
    sites — correct only where the streams align, the only case that claims 0).
    """
    t = list(t_words)
    c = list(c_words)
    n = len(t)
    for i, mask in sites:
        if i < len(c):
            c[i] &= mask
        if i < n:
            t[i] &= mask
    total = 0
    for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(a=t, b=c).get_opcodes():
        if tag == "equal":
            continue
        elif tag == "replace":
            total += max(i2 - i1, j2 - j1)
        elif tag == "delete":
            total += i2 - i1
        elif tag == "insert":
            total += j2 - j1
    return total


def _objdump(binary, flag, obj):
    return subprocess.run(
        [binary, flag, str(obj)], capture_output=True, text=True, check=True
    ).stdout


def reloc_blind_score(target_o, cand_o):
    """Relocation-blind score of a candidate object against a target object:
    the masked-word diff over their .text sections, masking the fields the
    candidate's own relocations patch. 0 = masked-word identical."""
    binary = _objdump_path()
    t_words = _parse_text_words(_objdump(binary, "-d", target_o))
    c_words = _parse_text_words(_objdump(binary, "-d", cand_o))
    sites = _parse_relocs(_objdump(binary, "-r", cand_o))
    return _masked_diff(t_words, c_words, sites)
