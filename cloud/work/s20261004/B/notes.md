# s20261004 lane B: notes

All five targets are IPA call-group callees. Each one uses `$s` registers without
saving them, so the work used the whole-program `-O3` group build
(`tools/cloud/score.py group`). The flags were `-g0 -O3 -mips2 -G 0 -non_shared`,
and the scorer adds `as1 -r4300_mul`. Extents come from the scanner/closure files
`build/m2c_asm/<fn>.json` and match the `.text.<fn>` word counts in
`asm/us/blob/blob_800ef5b0.s`.

Scoring ran in watchman2:~/rush2049/scratch/s20261004_B. That directory has copies of
`tools/cloud/score.py` and `asm/us/blob`, and the scorer checked them against
SHA256SUMS. The only change to the copied scorer is that the diff-print count can
be set with the `SHOW` env var. It does not change scoring.
Helpers: `run.sh <dir>` (score), `sdiff.sh <dir> <fn>` (aligned word diff),
`dump.sh <dir> <fn>` (objdump).

## Strict true-0 matches (score.py `MATCH`, exit 0, no --allow-unverified)

| function | source | group dir |
|---|---|---|
| func_800F1114 (63 w) | func_800F1114.c | g1114 |
| func_800F2718 (92 w) | func_800F2718.c | g2718 (func_800F1210 is a non-matching stand-in, context only) |
| func_800F2888 (104 w) | func_800F2888.c | g2888 |

Each group dir holds a byte-identical copy of the source plus `group.json`.

Levers that mattered:
- Static IPA callees need at least two call sites or a big enough body, or umerge
  inlines them and the symbol ends up as an empty `jr ra`.
- func_800F2888: `if (D_80143F18[i] == 0) continue;`, not an enclosing `if`, gives
  the right register order (constants in s0/s1, i in s2) and stops the loop-tail
  reload from being duplicated. Comparing against `0xFFFFFFFF` (unsigned) stops IDO
  from sharing one `-1` register between the `sh -1` and the compare.
- func_800F87A0 (near-miss): giving stores of the same constant different
  signedness (u8 field vs s16 global) keeps IDO from hoisting `1` into an s-reg.
  Defining func_800F84B0 inside the group lets IPA keep `6` in `$a3` across the
  call, as the target does.

## func_800F857C: best 2 missing instructions (score.py 95/116 because of the shift)

Best source: `g857C/group.c`. Its context is slot_state_setup, adapted from lane E's
draft, which takes its argument in `$s2`. Aligned residual
(`sdiff.sh g857C func_800F857C`): only the two `move $s0,$v0` after each
`jal slot_state_setup` are missing, plus 2 branch offsets that change as a result.
Everything else matches word for word.

That dead copy appears after 94 of ~102 `slot_state_setup` call sites in the
whole image, always into `$s0` and almost always dead. It looks like something
systematic, not per-function source.
Attempts that failed:
- result assigned to a local, to the param, or to the later y/i variables
- `(void)` and expression-statement uses, `if(0)` uses, and unreachable uses
- a global or static global
- calling slot_state_setup without a prototype (K&R)
- `volatile` or address-taken locals. These keep the value, but on the stack
  (frame 72 vs 40, 4/116 words).

## func_800F87A0: best 3 words + jump table (cannot be strict 0 as-is)

Best source: `g87A0/group.c`. It contains func_800F857C (near-miss), an approximate
func_800F84B0, and slot_state_setup as context. Aligned residual
(`sdiff.sh g87A0 func_800F87A0`): the target keeps the address of D_8015694C in
`$s0` (`lui s0; addiu s0; lw a0,0(s0)`) but never reads `$s0` again. We emit
`lui a0; lw a0`. This looks like the same dead-`$s0` mechanism as in func_800F857C.
The rest matches, apart from the switch table relocation.

The 7-entry `switch (D_801391E0)` jump table (`D_8012461C`) compiles to our own
`.rodata` and the scorer reports it as section-relative "unverified". So a strict
true-0 under the current scorer would also need that data relocation verified.
Variants tried for the prologue:
- local pointer
- dead re-reads of D_8015694C
- `resource_type_select(D_8015694C)` (rereads)
