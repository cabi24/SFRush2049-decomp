# Wave 10, lane w10b: six large/medium singles

Date 2026-10-06. Builder scratch `watchman2:~/rush2049/scratch/frontier/w10b` (copied from `base`, then
src/blob, include, tools/cloud, asm/us/blob, cloud/work/tools and the lock rsynced from the Pi). At most
2 threads. Nothing committed, spliced or pushed. No permission denials.

All six assigned functions had earlier attempts (w1c, w2c, w6d, w7b, w7c). Started from each best.

| Function | Bytes | State | Flags | Scorer output (exact) | File |
|---|---:|---|---|---|---|
| `draw_text` | 556 | **strict MATCH**, unit EQUAL, 0 neighbours differ | `-g0 -O3 -mips2 -G 0 -non_shared` (also MATCH at `-O2`) | `MATCH` / `EQUAL draw_text: 139 words (kept, c_draw_text.c)` / `locked bodies that differ in this unit: 0` | `cloud/matches/draw_text.c` |
| `func_80109A60` | 1,268 | 26 words off (was 304 words, 35 mnemonic rows) | `-O3`, unit only | `FAIL func_80109A60: 26 of 317 words differ` | `func_80109A60/best.c` |
| `particle_lifetime_set` | 732 | 59 words off (unchanged best); one of its two ring causes found | `-O3` | `FAIL particle_lifetime_set: 59 of 183 words differ` | `particle_lifetime_set/best.c`, `alt_*.c` |
| `audio_doppler_full` | 952 | 153 words off (was 167); frame now 256 = retail | `-O3`, unit | `FAIL audio_doppler_full: 153 of 238 words differ; compiled body is 240 words, target 238` | `audio_doppler_full/best.c` |
| `func_8010D3C0` | 704 | 72 words off (unchanged, w7b best) | `-O3` | `FAIL func_8010D3C0: 72 of 176 words differ` (+ own-rodata notes) | `func_8010D3C0/best.c` |
| `func_800F7F3C` | 1,396 | 117 words off (unchanged, w1c best); oracle located the rest | `-O3` | `FAIL func_800F7F3C: 117 of 349 words differ` | `func_800F7F3C/best.c` |

Commands (repo root, Pi):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10b score NAME --with FILE --neighbours
scp FILE watchman2:rush2049/scratch/frontier/w10b/cand/NAME.c && ssh watchman2 'cd ~/rush2049/scratch/frontier/w10b && \
  IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido \
  python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
```

## Integration notes

- `draw_text`: plain single, `cloud/matches/draw_text.c`, line 1 `/* flags: -g0 -O3 -mips2 -G 0 -non_shared */`.
  No literals of its own except `0.0f` (`mtc1 zero`), no own rodata, no overrides, supersedes nothing.
  `splice_singles.py draw_text` should take it as is.
- Nothing else is spliceable.

## draw_text — strict MATCH (103 → 0)

Semantics: merge one save slot's best times into the global top-5 tables (12 tracks × 3 categories; times
in `D_80150F88[i].times[j][5]`, owning entry in `D_80151690[i][j][5]`). Walk the five global ranks `k` and
the slot's own sorted list with a second cursor `l`; when the slot's next positive time fills an empty rank
or beats it, shift the tail down, insert time and owner at `k`, advance `l`. No arcade ancestor found.

Two levers, both found from the instrumented-uopt trace (`tools/ctrace.sh`, proc 619):
1. **Index cursor `src[l]`, `l++` instead of a walking pointer `*src`, `src++`.** The extra induction web
   raises the interference count of the loop-invariant webs (bounds 4/5/60/12, `&D_80150F88`, the `j*20`
   and source-base IVs) from 21 to 22. uopt colours webs with ≥ 22 interferences in its *priority* phase
   (`p1`, highest `save` first) and the rest afterwards in *web-number* order (`p2`). With 21 they were
   coloured last in p2 and got s4–s8; with 22 they are coloured first by priority and land in a1–a3/t0–t3,
   exactly as retail (103 → 13 words).
2. **`times`/`owners` assigned at the j level before the k loop.** p2 colours in web-number order (first
   appearance); retail has `times` (t4) and `owners` (t5) coloured before `k` (s0) (13 → 0).

```
blob_unit --tag w10b score draw_text --with cloud/matches/draw_text.c --neighbours
  EQUAL draw_text: 139 words (kept, c_draw_text.c)
  locked bodies that differ in this unit: 0
score.py fn cloud/matches/draw_text.c draw_text --flags "-g0 -O3 -mips2 -G 0 -non_shared"   ->  MATCH
same at -O2                                                                                 ->  MATCH
```

## func_80109A60 — 26 words (was 304 / 35 mnemonic rows)

w7b's `best_unit.c` (arcade `hud.c:AnimateDot` with `Hidden` static and inlined) plus three source changes,
each measured in the unit:
1. `D_801543CA` and `D_8002E8E8` (the clock struct, `.tick` at +636) `volatile`: retail reads them as
   `lui; addiu; lh 0()` and `la; lw 636()` (35 → 30 rows).
2. **The map size is written twice per branch** — `map_width = D_801161C4 - 8; map_height =
   (D_801161C4 - 8) * world_height / world_width;` (and the mirror) — so uopt has a CSE temp for
   `D_801161C4 - 8` in a0 that is copied into map_width/map_height (`move v1,a0` / `move t1,a0`). That temp
   is what pushes `blt` off a0 into t0 (retail `move t0,a0`, `move a0,t0` before each call). Body length
   becomes 317 = retail (30 → 23 rows).
3. **`u32 id = blt->AnimID; slot = D_80142DB4[id & 0xf];`**: retail keeps AnimID in v1 (a coloured web);
   without the local it is a ring temp and the whole temp ring is off by one. 23 rows → 26 words.

Residual (26 words, two independent items; oracle-confirmed):
- `multu a0,a3` vs retail `multu a3,a0` (map-height product). With the CSE temp, uopt puts the temp first
  whatever the source order (`world_height * (D-8)` and `(D-8) * world_height` both give a0,a3). A named
  `s32 msize` (`best_named_msize.c`) gives retail's operand order but then msize and map_width tie at
  save 3.0 and msize (lower web number) takes v1: 59 words.
- size (`blt->Height`) vs `D_80151AD0` tie at save 1.0: ours count t3 / size t4, retail size t3 / count t4.
- Oracle: `best_named_msize.c` traced (proc 955) and forced `p1:w74=c3,p1:w79=c2,p1:w14=c11,p1:w67=c10`
  → `differing rows 0` (a full match). So the whole residual is these two ties.
- Still artificial: w7b's `if (flash) { }` (without it: 35 rows). About 45 variants this pass (size
  placement, msize declaration/position/ordering (16-form sweep, all identical), map-size reuse forms,
  inlined world_* expressions (worse)).

Next: a natural source whose map-size temp is a *named* variable numbered after map_width, or a change that
gives size more savings than the D_80151AD0 PRE web (both ties flip together if one more block lies in the
count web's range — compare the `if (flash) { }` case).

## particle_lifetime_set — 59 words (best unchanged); one ring cause found

Built an instrumented **ugen** (free-list trace, `workbench instrument-ugen`; `tools/ugt.sh FILE NAME`
prints `DKWB-FREELIST` events with source lines plus the listing). The ring is least-recently-freed,
`t6 t7 t8 t9 [t0 coloured] t1 … t5`; the name struct copy on line 63 pops t7/t8/t9.
- Cause 1 (fixed in `alt_scale_after_address.c` and `alt_motion_pointer.c`): retail's `D_801391E4` load
  (scale) comes *after* the `D_80114264[n-1].slot[i]` address in ugen order (scale = t8, then the
  `*224` mul t5, add t6). Loading the four rect values unscaled first and then `scale = D_801391E4; right
  *= scale; …`, or a `Motion *m = &…slot[i]` pointer assigned before `scale`, reproduces that part exactly.
  Both alternatives cost elsewhere (float colours / a non-folded `-224`), so best.c stays w7c's.
- Cause 2 (open): at the call `func_800A78BC(…, D_801145D4[n][i] | 0x1210, 1)` retail pops two more
  temps: `lui t1; addu t1,t1,t9; lw t3,lo(t1)` (address in t1, value in t3) where ours is the as1 macro
  form `lw t1,lo(t1)`. Retail's ugen emitted the hi/lo address explicitly. 8 array-type/index forms tried
  (1-D, struct row, bounded, pointer arithmetic): none.

## audio_doppler_full — 153 words (was 167)

- **Frame:** retail 256 has `quad` at 208 and `name` at 176. Ours with the w6d declarations is 288 (each
  inlined `func_800B61A8` costs a 24-byte block: 0/1 calls 232, 2 calls 256, 3 calls 280). `char name[8]`
  (the longest string is "CNTDWN3") gives frame 256 (`best.c`), but then name lands at 200, not 176;
  `name[8]` declared after five scalars gives 264/188. The locked wrapper's `new_var` costs 16 of the
  inline blocks (a `return entity_flags_apply(...)` wrapper gives 264): if the locked wrapper source is
  revisited, that is a lever, but it is locked and matched as is.
- Still open from w6d: s7/s8 contest between `&D_80154398` (texture) and `&D_801140F4` (colour), and the
  quad `+= pos` load/store order. ~25 variants.

## func_8010D3C0 — 72 words (w7b best, unchanged)

Re-examined the as1 hoist of the default case (`li a0,1; lui v1` above `addiu t3`). Retail's jump table
(read from the image at 0x80124958) has eleven distinct targets, so default does not share case 351's
block. Address-taken `kind`/`amount` (to make them live at the exit) → memory resident (147 words).
w7b's `if(amount) {}` ×2 colour lever is still needed (without: 51 aligned rows). ~6 variants; stopped.

## func_800F7F3C — 117 words (w1c best, unchanged); oracle located the residual

Traced (proc 901). Arm 1 (mode 4): the end pointer of the first sort loop (p2 web w60) takes t4 where
retail has t5; forcing `p2:w60=c12,p2:w201=c14,p2:w202=c15,p2:w205=c12` makes arm 1 register-exact
(`&D_80152038` s0, 120 s1, 1 t5) and leaves 114 rows that are all ugen temp-ring rotation: one extra ring
pop in retail between the end of arm 1's sort loop and its tie loop (traced ugen: ours pops t6 for the
hoisted `D_80143F54[0]` load, retail t7), and similar one-step offsets in arms 2/3. Retail keeps t4 unused
during arm 1's sort loop, so some web coloured t4 and numbered before w60 is live there without
instructions. No source found; stopped (w1c already spent ~2,200 variants on this residual).

## What generalises

1. **uopt colours in two phases** (instrumented trace): webs with interference count ≥ 22 (here) are
   coloured in priority order (`p1`); the rest afterwards in **web-number order** (`p2`), each taking the
   lowest free register. Loop-invariant constants and IVs that land in s-registers (when retail has them in
   a/t registers) usually sit at 21 interferences: **one more web anywhere in the loop nest** (an index
   cursor instead of a walking pointer, a second induction variable) moves them into p1. Check `numintf`
   in `ctrace.sh` before trying variants.
2. **p2 order is first appearance**: to give a web an earlier register in p2, make its first definition
   come earlier (hoist an assignment to the enclosing loop level).
3. **Instrumented ugen** (`tools/ugt.sh`, built in `watchman2:…/w10b/ugen/`): free-list events per source
   line identify the statement that pops one temp more or less. ugen evaluates a statement's operands in
   source order, so moving a load after an address computation in the same expression rotates the ring.
4. **A coloured web for a struct field read once** (`lw v1,44(a0)` then `andi`): the source had a named
   local for it (`u32 id = blt->AnimID`).
5. **A CSE temp that is copied into a variable** (`addiu a0,v0,-8` … `move v1,a0`): write the expression
   twice (`map_width = X - 8; map_height = (X - 8) * h / w;`); the temp can push a parameter off its
   argument register.
6. Force-oracle first (`force.sh` with all suspected webs) before searching: it told us func_80109A60's
   26 words are exactly two colour ties and func_800F7F3C's rest is pure ring rotation.

Tools (all in `tools/`, w9f/w7b copies retargeted to w10b): `ctrace.sh`, `force.sh`, `pdiff.sh`,
`sum.sh`, `us.sh`, `ub.sh`, `fr.sh NAME TAG FILES…` (unit score + frame + positional/mnemonic rows),
`udiffm.py --mnem`, `ugt.sh` + `ugt_remote.sh` (traced ugen), `batch.sh`/`bscore.py` (standalone batches).
