# CalcResPathBounds: strict N64 single match

Deliverable: `cloud/matches/func_800B9740.c` (game code, 0x800B9740, 408 B).
The exact source SHA-256 is
`42ad10a492b2de5170ca7c10f781b3a004cb56637c429ecb61903084563107ce`.
Current master input basis: `58425072b49e95f0fbe5c4c239988fb4885f7c5c`.

```
python3 tools/cloud/score.py fn cloud/matches/func_800B9740.c func_800B9740 --flags '-g0 -O3 -mips2 -G 0 -non_shared'
func_800B9740:
  MATCH
```

This is a standalone match of all 102 words with no unresolved relocations,
unverified fields, errors, or nonzero excess words. The scorer adds the normal
`-Wab,-r4300_mul`. No production source or lock was changed. Installation,
whole-program nonregression, image identity, and cartridge SHA-1 remain the
coordinator's gates.

## Source and review

The donor identity is GameCube/MAT3 `GameSource/src/pathcommon.c`
`CalcResPathBounds`. It computes signed-16 coordinate extrema for primary
resurrection points and points included in an alternate path of kind 1. The N64
layouts are 6-byte points and 16-byte alternate-path records. Empty selections
retain the original +/-32767 sentinels.

The donor loop shares `j` between the alternate-path and three-axis loops,
visits all eligible alternates without an early break, and uses ternary extrema
assignments that store even when an extremum is unchanged. Expressing the
exclusive endpoint as a byte offset `count * sizeof(Vertex)` closes the final
addition-operand-order residual. That source spelling is explicitly disclosed.
No compiler-specific PowerPC allocation assumption was imported, and no unused
locals, dead reads, padding arrays, ABI stand-ins, or false prototypes were added.

The independent reviewer recompiled the transferred source with stock IDO5.3
and the current master scorer/targets. A transfer-only extra terminal blank line
was removed to reproduce the original source digest exactly. A separate
reconstruction with a named point pointer did not match, confirming that merely
transcribing the high-level behavior was insufficient.

## Reproducible controls

`python3 cloud/work/frontier/dot_mat3_res_path_bounds_20261017/verify.py`
validates source and target-row digests, recompiles in a fresh temporary working
directory, and requires the candidate to match and every negative control to
fail. It pins no compiler, scorer, whole-assembly text, lock, or build-system
hashes. A single pytest entry covers that bounded replay.

- Ordinary typed endpoint arithmetic: 1/102 differing words
- Early exit from the alternate scan: 24/102
- Conditional-only extrema stores: 32/102
- Wrong kind constant: 84/102 plus three nonzero excess words

The older w14r draft replays here at 81/102 plus two excess words; its historical
report recorded 83/102. That historical score is not presented as an exact
same-tool comparison.

Ownership line to add in the publication PR:
`| game | func_800B9740 / CalcResPathBounds (408 B), strict single MATCH | Astra/dot lane4 | 2026-10-17 | MAT3 loop/type insight; independent IDO replay; coordinator installation pending |`
