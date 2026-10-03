# Image A near-misses (2026-10-03, `-g0 -O2 -mips2 -G 0 -non_shared`)

NONMATCH research. Score with `tools/cloud/score.py fn <file> <name> --targets asm/us/ovl_a`.

| Function | Words | Residual | Tried |
|---|---|---|---|
| `func_80395E24` | 7/26 | Schedule only. Target branches on `owner` before loading the count; ours hoists the `lh` above the `beqz`. | count reused as param 1 (needed to avoid homing `a1`); assignment inside the `if` homes `a1` and swaps registers (26); early return (25); comma-for (26) |
| `func_8039A448` | 8/35 | ugen temp ring only. Target restarts at `t6` for each clause result; ours continues `t0`–`t2`. | `!(mode == k) \|\| …` spelling (8, best); ternaries (21); split statements / `ok` variable (23–34); `!X` forms (8) |
| `func_80392F28` | 2/55 | ugen temp ring only: the constant 5 takes `t0`, where the target has `t6` (allocated before the `== 4` branch's temps). Needs a per-branch `flag = 1`; without it, 48 words differ. | inverted last test (10); store order inside the `< 29` branch (2–6) |
| `func_80393004` | 45/45 | Loop shape wrong (the target unrolls the flag loop ×4 into two arrays); first draft only. | — |
| `func_8039B120` | 5/61 | ugen temp ring only (`t6`/`t7` in the target, `t1`/`t2` in ours) in the 2nd and 4th `if`. Must use `D_803BAD90->` directly; a local copy swaps `v0`/`v1` (23). | — |
| `func_8038F648` | 42/59 | The target forms `&D_8002EB90` in a register and loads at offset 0, twice. Array and struct declarations do not reproduce it. Button-combo matcher over the 164-byte records set by `func_8038FF90`. | `f32[]`, struct member |
| `func_803931C0` | 17/69 | ugen temp ring plus the cursor loop. The target needs `switch (D_803BA804)` (read back after the store); with `switch (mode)` it is 69/69. | slot/row locals (23–40) |
| `func_8038F744` | 18/22 | Allocation and schedule of the base pointer. Target keeps the base in `t6`, recomputes `&D_803B65E4[6]` and uses `bnez` + `nop`. | first shape only |

Lessons that produced the four matches in this pass (`cloud/matches/ovl_a/`):
- When the target never homes argument N but overwrites `aN` with a loaded
  value, the original reused the parameter as a local
  (`count = D_8014A108;`). Leaving it unused makes IDO emit `sw aN,…(sp)`.
  Twin scanners `func_80396EE8`/`func_80396F44` matched only this way.
- `D_8014A118` is a 76-byte player table whose pointer at `+0x48` is the
  owner; both `func_80395E24` and `func_8039A448` index it.
