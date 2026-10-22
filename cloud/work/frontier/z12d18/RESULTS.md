# z12d18 — func_80012D18 (static, lib_11640, IDO 7.1 `-g0 -O2`), 2026-10-21

NEAR MISS, not a submission. `best.c` takes the w23a baseline from **578 to 187** differing `objdump -dr` rows
(`obj71.sh`, trace71 toolkit, scratch `watchman2:~/rush2049/scratch/frontier/z12d18`, fidelity 54/54). The body is now
**788 = 788 rows** (baseline 781: 6 words short); opcode-aligned equal 769/788 before the last scheduling fixes; every
named local sits at retail's frame offset (stack-offset usage profile identical to retail).

## Levers (each measured; rows after)

| Step | Lever | Rows |
|---|---|---:|
| base | w23a `best.c` | 578 |
| 1 | `cmd->volume[i] = (V->level[i] = V->target[i] * V->gain) << 16` (and the delta branch): retail sign-extends the stored short (`sll/sra` x8) instead of reloading `lh` | 542 |
| 2 | `loopRem / 32`, `mainRem / 32` in the **non-flags** ADPCM case only (retail: `bgez/sra/addiu` signed divide there, plain `sra` in the flags case) | 548 (+8 words, now aligned) |
| 3 | the `flags & 1` cases end with `goto adpcm_tail` / `goto pcm_tail` into the non-flags cases' shared tails (retail `b 0x6c0` / `b 0x780` with the tail's registers preloaded); IDO does not merge tails itself | 491 (788 = 788) |
| 4 | `unsigned int position = func_8001E688(V->position)` (no double local; uopt still places the conversion per branch) | 487 |
| 5 | `unsigned int excess` declared after `state`, assigned in the third condition: cmd/state/address/length/padded/mainRem/loopRem homes all at retail offsets, frame 120 | 412 |
| 6 | range planning statement order: third branch computes `offset` before storing `address[0]` (retail reuses the test's `loop_start` load: no reload); second and fourth branch order | 230 |
| 7 | more statement order (third/fourth branch, else-path sub-branch) | 221 |
| 8 | 2 bytes of local area below `loopRem` (`probe`, disclosed): first expression home at -76 like retail's `44(sp)` | 217 |
| 9 | non-flags ADPCM loop/main statement order; `cmd->unknown44 = 0` after `position4C` | 205 |
| 10 | `frames = (V->rate * 192 + 0xFFF) >> 12; frames += 6;`: the second definition raises frames' priority (tot 6 -> 8, save 1.5 -> 2.0, ties the excess expression and is decided first), so frames a1 / excess a2 like retail; equals the force71 oracle | 187 |

## Remaining residual (187 rows)

- **frames/excess register swap: FIXED** by step 10. Not moved by: excess as its own statement, flipped compare,
  `int excess`, `register`, two-step or initialised excess (uopt copy-propagates every excess form).
- **`V->data` in v0 + `move a3,v0` in the jal delay slot** (flags PCM/ADPCM cases): ours colours the CSE'd load into
  a3/t0 directly (argument preference).
- **loopRem at the join in a0** (ours a1); force to a0 is declined (a0 forbidden by an interfering web).
- ugen ring residuals (t6/t8 swaps) in the tail and the `count++` address loads; a few as1 store-order ties.
- The 2-byte `probe` is an exact frame residual; its identity is unknown (a named `short level` instead of the
  chained assignments removes the cfe assignment temp and comes out 4 bytes short: 218).

## Method and tools

`variants.py BASE SPEC...` applies named edit sets and scores each (`EDITS` holds every edit tried);
`rows.py SRC...` scores and prints the differing row ranges. Analysis was an opcode-level alignment of target vs
ours (`difflib` on mnemonics), then reading the target around each misaligned block.

Behaviour: every lever is semantics-preserving (`/ 32` equals `>> 5` for the 6-bit remainders; the goto tails run the
same statements; reorders move independent stores only).
