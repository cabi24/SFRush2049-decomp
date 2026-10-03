# D11 packet: func_800D1248, the last 9 words

**For:** Astra (repo-only dot worker). **Status:** NONMATCH, ready to match.
The owner lifts the `dot_response` pause for this function when this packet is
merged. Read `STATUS.md` in this directory first; it records every lever tried.

## Target

- `func_800D1248` at `0x800D1248`, 324 bytes / 81 words, game image
  (`asm/us/blob`). Flags `-g0 -O2 -mips2 -G 0 -non_shared` (the scorer
  adds `-Wab,-r4300_mul`).
- Behaviour: when the state at `+0x6C4` is −2, reset it to −1 and clear
  `+0x71C`. Copy the vector `+0x660..0x668` → `+0x6D4..0x6DC`. Call
  `math_utility(+0x678, +0x6E0)` and the reset helper `func_800D11BC`.
  Set the gear/mode bytes (`+0x730`, `+0x3F4`, `+0x3F6`) to 2 or 1
  depending on whether `(f32)+0x6BC >= 44.0f`. Fill four per-wheel ratios.
  Flag `D_8014A96C[+0x7C6]` (stride 0x808) as 1.

## Evidence so far

| Source | Score |
|---|---|
| `cloud/work/near-miss/func_800D1248/base.c` (older best) | 14/81 |
| `func_800D1248.c` here: separate `s16 idx` for the final index | **9/81** |
| `func_800D1248.permuter_best.c` (farm permuter) | strict MATCH, **with a forbidden keeper** |

The 9-word residual is words 7–14 and 16. In the block after the
early-return test, the target addresses `arg0` through `a2`; ours reuses the
still-valid incoming `a0`. Both emit `move a2,a0` at entry, and the workbench
reports identical uopt colorings. The permuter shows what flips it: empty
short-circuit blocks right after the early return,
`if ((arg0 && arg0) && arg0) {}`. These create labels, and ugen stops
reusing `a0` at a label. That construct is a fabricated keeper and **must not
be submitted.** The task is to find the natural C that leaves the same
control flow there.

## Already ruled out (each stayed at 9 unless noted)

Early return against wrapping `if`; `goto`; single-case `switch`;
`do { if break } while (0)`; local copy of `arg0` (void or `s8 *`);
`s8 *`/`s32`/K&R parameter; statement reorderings; locals scoped inside the
block; `func_800D11BC` defined in the same file; `-g*` (worse). Two-term
`if (arg0 && arg0) {}`, `if (arg0) {}`, `(void)arg0;`, a bare label,
`while (0) {}` and `switch (0) { default: }` are also ruled out. Folding a
null check into the early return gives 28–78 words.

## Suggested directions

1. **A stripped debug or assert macro.** Find a macro in the arcade code
   (`reference/repos/rushtherock/`, via lookup request if your checkout lacks
   it) or in N64-era SDK headers that expands, with debugging off, to a
   short-circuit condition with an empty or constant body. Try the shapes it
   produces right after the early return.
2. **An inlined-guard shape.** For example, a validity test on a field (not
   `arg0` itself) that IDO folds away but leaves blocks behind.
3. **Count matters.** Two `&&` terms did not work and three did. Measure the
   minimum block or label structure needed, then search for a natural
   construct with that shape (`||` chains, nested `if`, ternary statements).
4. If nothing natural works after a bounded effort (about 20 directed
   variants), record the attempts here and stop. Do not submit the keeper.

## Rules

True strict zero only (`tools/cloud/score.py fn SRC func_800D1248`). C89/IDO.
No fake keepers, unused formals, pressure locals or protected-path edits.
Submit a match as `cloud/matches/func_800D1248.c` with line 1
`/* flags: -g0 -O2 -mips2 -G 0 -non_shared */`. Update this file with each
round's variants and results. Integration (lock and splice) stays with the
maintainer.
