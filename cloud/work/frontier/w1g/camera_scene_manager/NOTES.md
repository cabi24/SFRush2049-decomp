# camera_scene_manager (0x800C2BE0, 614 words) -- not matched

State: `best.c` (= the body in `groups/camera_scene_manager/group.c`) emits 613 of 614 words;
`452/614 words differ` strictly, 151 rows in an instruction-aligned diff (was 552 strict / 458 aligned rows,
598 words). Most of the strict count is a handful of register renames that propagate.

Scorer: `python3 tools/cloud/score.py group cand/camera_scene_manager` (see RESULTS.md).

## What is settled (each verified by the diff going to zero in that region)

- Whole preamble, the first loop, and the per-player head up to the wheel counters.
- The five zeroing stores are ONE chained assignment
  `D_80157238 = D_8015B248 = D_8015B258 = D_8015F730 = D_8015F728 = 0;`. Only that gives retail's address
  registers (`$a3`, `$t0`) for the two non-hoisted globals, the `move a0,zero / move a2,zero` zero copies and
  the constant-folded `li 1` stores in the first `!= 8` block. Five separate statements give `lui at` stores
  and a real load in the first block.
- The counters are `x++` in all four blocks; order in the second block is `D_80157238, D_8015B258, D_8015F730`.
- `D_80161388[0..2] = fabsf(...)` then `D_80156BC8 = D_80156BD8 = D_80156CE4 = 0;` (chained again: address
  registers `$a0`/`$v1` for two of them, `lui at` for the third).
- The three direction-flip pairs: `fl = ply->flags;` before each pair, `ply->flags = fl & ~A;` followed by
  `*(u32 *)&ply->flags |= B;` (two stores, as retail).
- Event codes: the wheelie-style block records code 6 (not 8) and the final one code 11 (not 10); the last test
  reads `D_80152038[i].count` (+0x18), not `.active`; `(D_801543CC - x) > 1` with int 1 (retail materialises
  a fresh 1.0f there although `$f30` already holds 1.0f).
- Literals are natural: 2.69f, 0.15f, 0.4712389f (x3), 3.1415927f. Their .rodata order and even the
  unreferenced 0.15f word at +0x14 (`0x80123F4C` in retail) come out as in retail.
- `func_800C1B60(idx, code)` argument order (see group.c).

## What is open

The three blocks after the calls to `func_800C2944/26C4/2430` (flags 0x80 / 0x100 / 0x1) are almost certainly
three more sibling functions inlined by umerge: retail has three caller-less `jr ra` stubs at
`0x800C2418/2420/2428`, exactly between `func_800C220C` and `func_800C2430`, the callers run in reverse
address order (`2944, 26C4, 2430, [2428, 2420, 2418], 220C, 2004`), and the third block is `func_800C220C`'s
body with other constants (including the same doubled `lui at,0x3e80`). Written as inlined functions with
pointer parameters `(Ply *p, PCar *car, s32 idx)` the code is byte-for-byte the same as written in line, so
the choice does not change the residual.

Residual lanes, in order of size:

1. **Register colouring of three short ranges.** Retail: cached `ply->flags` in `$a1`, cached `D_80157238` in
   `$v1`, the counter `n` in `$v0`. With an explicit `fl` variable we get `$a1/$v1` right but `n` in `$a0`
   because the masked value takes `$v0` as a uopt temp; with no `fl` at all (plain `ply->flags &= ...`, uopt
   CSE) the statement order and the `move`/`lw` re-materialisations are exactly retail's but the three
   registers are rotated (`$v1/$v0/$a0`). `fl &= ~0x80; ply->flags = fl;` gives `$t8` and `n` in `$v0`
   but emits `move a1,t8` before the store (retail: after).
2. **`D_80157238` after the two IPA calls** (tail): retail keeps one copy in `$v1` with reloads placed in the
   predecessors (PRE); ours reloads into `$t7/$t9` at each use.
3. `D_8016139C += *pdt` stores through `$a0` in retail, through `lui at` here (loads already use `$a0`).
4. Preheader order of the `$s2/$s4` set-up.
5. Float registers of `a28/a2C/a30` in the 0x80 block (`$f12/$f14/$f2` vs `$f2/$f12/$f0`).

Tried without effect on lane 1: chained `fl = ply->flags = ...`, `ply->flags &= m; fl = ply->flags;`,
`fl = (ply->flags &= m)`, named `x,y,z` for the three sums, array-index form `D_801569B8[i].flags` for the whole
loop (fixes the reload after `st->f60 +=` but un-registers the first loop's pointer).

Best next hypothesis: the main loop keeps `fl` only for the two tests at the top (that is what survives the
float store to `st->f60`), and the three inlined siblings each have their own `fl = p->flags` like
`func_800C2004`, taking `idx` only and recomputing `p = &D_801569B8[idx]`; the unification with the loop's
pointer then has to come from the loop being written with `&D_801569B8[i]` for exactly the uses that feed them.
