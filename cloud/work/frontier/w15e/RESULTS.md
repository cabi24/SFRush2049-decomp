# w15e RESULTS: entity_spawn_init (0x8008EA10, 5544 B, 1386 words, -O3)

| function | bytes | state | flags | scorer output (Pi, `blob_unit --tag w15e --remote-dir rush2049/scratch/frontier/w15e score entity_spawn_init --with <file> --neighbours`) |
|---|---:|---|---|---|
| entity_spawn_init | 5544 | NONMATCH, 61 words; frame 728 = retail, size 1386 = retail | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL entity_spawn_init: 61 of 1386 words differ` / `locked bodies that differ in this unit: 0` (`entity_spawn_init/best.c`) |

Start point: w14e best was `1356 of 1386 words differ`, frame 728/520 (body 1391 words). Nothing to integrate: no
match, no group, no cloud/matches file. Files: `entity_spawn_init/best.c` (lowest strict count, 61),
`entity_spawn_init/best_alt.c` (71 strict, but the function top is structurally closer: 30 mnemonic rows vs 35).
Drafts a2..a37 and vbatch dirs b0..b8 are kept for replay; `probe/ord*.c` are the operand-order probes.

## Progress (unit, us.sh; words/ops = aligned differing rows, strict = scorer count)
| draft | change | strict | words | ops | frame |
|---|---|---|---|---|---|
| w14e best | baseline | 1356 | 1181 | 149 | 728/520 |
| a3 | named-local layout reproduced (gaps filled) + residual words below the colours | 1353 | 1348 | 149 | 728/728 |
| a5 | `s16 surface`; `Model *model = &D_8014A250[slot]`; `v->handle = func_8008E26C(...)` per branch | 1352 | 944 | 83 | 728/728 |
| a10 | timestep store first; `model_transform_setup(h, first = 0, 15)` | 1352 | 939 | 76 | 728/728 |
| a11 | `pos[1] += suspension` after the pos[2] random term | 1354 | 890 | 61 | 728/728 |
| a14 | lifetime sites 2/4 spell 0.5 as `.5f` | 1348 | 762 | 57 | 728/728 |
| a20 | every pos accumulation `pos[k] = pos[k] + B * A` (+2 residual words) | 1354 | 268 | 33 | 728/728 |
| a21 | `Car *car = &D_80152818[slot];` as initialiser | 204 | 263 | 32 | 728/728 |
| a25 | case 4/5 height: `car->up[k] * D_8011744C[D_8014A250[slot].type]` (no named scale) | 198 | 257 | 32 | 728/728 |
| a27 | case 5: no `resource` local in the func_8008D870 argument | 140 | 190 | 30 | 728/728 |
| a31 | colour stores `{ s16 h = v->handle.word; D_8012E700[h].rgba.word = colorN; }`; -4 residual words | 72 | 90 | 30 | 728/728 |
| a36 (best_alt) | `while (count != i)` | 71 | 89 | 30 | 728/728 |
| a37 (best) | + `mph = car->mph; mph = mph >> 2; mph = fabsf((f32)mph);` + `surface = model->surface[type];` as first statement | 61 | 99 | 35 | 728/728 |

## Mechanisms found (generalise)
1. **FP literal spelling splits uopt constant webs.** uopt keys real constants by their text: `0.5f` and `.5f`
   (also `0.50f`, `(f32)0.5`) are different constants. Retail rematerialises 0.5 (`lui 0x3f00; mtc1`) at two
   lifetime sites while the other 0.5 uses share callee-saved $f22; spelling those two `.5f` reproduces it and
   re-syncs the FP temp ring for the rest of the function. Look for this whenever retail rematerialises a
   constant that a callee-saved register already holds.
2. **`x = x + E` is not `x += E`.** Probes (`probe/ord3.c`..`ord8.c`, -O3):
   `p += (R-2.5f)*G` -> mul(sub,G), add(p,prod); `p = p + (R-2.5f)*G` -> mul(G,sub), add(prod,p);
   `p = p + G*(R-2.5f)` -> mul(sub,G), add(prod,p); `p = E + p` behaves like `p += E`. In the `x = x + E`
   form a register-variable leaf is always put on the right of a mul (`p = p + s*G` and `p = p + G*s` both give
   mul(G,s)); a memory expression is not (`p = p + G * T[i]` gives mul(T,G) even when PRE later CSEs T[i]).
   The norm/ops diffs hide operand order; the add/mul order changes ugen's free order and so the whole FP
   ring downstream. func_8008E408's `o->pos[0] = o->pos[0] + o->vel[0]` is the same idiom.
3. **Variable spelling decides which values become coloured webs.** `mph = car->mph; mph = mph >> 2;
   mph = fabsf((f32)mph);` (three assignments) puts the raw load in mph's register (v0) like retail; the
   one-expression form leaves it in the temp ring and moves count to v0. A block-local `s16 h` per colour store
   gives retail's v0 handle; `D_8012E700[v->handle.part.id]` or `[(s16)v->handle.word]` gives temps.
4. **Initialiser vs statement changes copy propagation:** `Car *car = &D_80152818[slot];` (initialiser)
   versus `car = ...;` statement moved the car web and cut 1150 strict words (alignment of the whole body).
5. Inlined `func_8008E06C(handle, &color)` at the 6 colour stores is REFUTED twice: umerge then stops inlining
   4 of the 19 func_8008B2E4 sites (budget), and retail has no jal to either.
6. Inlined func_8008B2E4 costs 16 B of frame per site (probe/p1..p4.c: 16/32/48/64).

## What remains (61 strict words)
- Function top (~45 words): retail keeps `surface` purely in memory (lhu + sh at entry, `lh 666(sp)` reloads) and
  holds the `&D_8014A250[slot]` web in t1 across the pre-loop code, spilling it at the loop preheader
  (`sw t1,92(sp)`). Ours colours surface's first piece (t1, save 0.09, tot 1.0 > best 0) and spills the model
  web at its definition. Oracle: `force.sh a34 entity_spawn_init p1:w12=s` (split surface) fixes everything
  at the top except the model web, so the residual is colouring. Best next hypothesis: retail's model web
  takes t1 first and leaves surface with no free register; find the source form that gives the model web a
  colourable pre-loop piece (tried: initialiser vs statement vs before-loop assignment; surface u16/s16;
  surface statement position).
- Spill homes: retail model at 92 and type at 84; ours 96 and 88 (5 spill-temp homes after a 628-byte named
  area). Retail fits a 624-byte named area with one more coloured expression web before `model`; the residual
  word count (pD00..pD45) must then drop by one. Not identified.
- best_alt (71) keeps the original one-line mph and declaration-initialised surface; a37's changes trade 10
  strict words for a more scrambled top schedule, so revisit both once the model web is understood.

## Frame facts (measured)
- Retail named locals, top-down: pos 708, delta 696, spacing 692, previous 668, surface 666 (s16), mph 664,
  first 662, count 648, i 644, colours 628..608 (color0 highest). best.c reproduces all of them.
- Frame = 80 + named area + inline areas (19 x 16) + spill homes, rounded to 8.
- Unused locals carried as residual: pB0..pB2 (gap 652..661), pC1..pC3 (632..643), pD00..pD45 below the
  colours. This is near the exact residual but not proven exact (see spill homes above).

## Notes
- Builder scratch: watchman2 `~/rush2049/scratch/frontier/w15e` (toolkit installed with `--reuse wtk`).
  An install mistake (argument order) briefly created `~/--reuse` on the builder; it was removed.
- vbatch with `--keep entity_spawn_init,func_8008B2E4` and the locked func_8008B2E4/rand pasted into the
  file tracks the unit closely (b0..b8 use that form); unit files (aN.c) do not paste the helper.
- No permission denials.
