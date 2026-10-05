# Frontier wave 6 — agent w6b (near-miss lane) results

Builder scratch: `~/rush2049/scratch/frontier/w6b` (copied from `base` on 2026-10-05; the w5d traced uopt is
used through the symlink `w6b/uopt -> ../w5d/uopt`, and a traced as1 was rebuilt in `w6b/as1/`). Unit tag `w6b`.
Nothing was committed or spliced. func_80087110 and stat_race_update / func_800FE5B0 were not touched.

| # | Function | Bytes | Before | Now | Deliverable |
|---|---|---:|---|---|---|
| 6 | `func_8010E0FC` | 1,000 | 65/250 | **strict MATCH** (-O3), unit EQUAL, 0 neighbours broken | `cloud/matches/func_8010E0FC.c` |
| 2 | `net_state_validate` | 2,728 | 27/682 | **9/682** (one register pair; both oracles 0 rows) | `net_state_validate/best.c` |
| 4 | `music_tempo_set` | 980 | 130 aligned rows | **31/245** (unit, `--block func_80092B80`) | `music_tempo_set/best.c` |
| 1 | `audio_mixer_main` | 732 | 31/183 | 31/183; mechanism now complete (6 aligned rows with a non-natural arm) | `audio_mixer_main/best.c`, `proof_test_cm1_arm_notneg.c` |
| 5a | `sound_stop` | 160 | 12/40 | 12/40 (alternative with 8 aligned rows) | `sound_stop/best.c`, `sound_stop/perm/QN.c` |
| 3 | `func_800E95DC` | 1,684 | 2 instructions | unchanged; listing-level experiments recorded | notes below |
| 5b | `func_8008E408` | 1,544 | ~85 rows | unchanged; w5a's static-helper hypothesis tested negative | notes below |

Strict bytes this batch: 1,000 (`func_8010E0FC`, 0 dependants, a frontier `single`; spliceable as a single).

---

## func_8010E0FC — strict MATCH

Gold/silver coin object update; semantics and shaping in the file header.

```
scp cloud/matches/func_8010E0FC.c watchman2:rush2049/scratch/frontier/w6b/cand/func_8010E0FC.c
ssh watchman2 'cd ~/rush2049/scratch/frontier/w6b && IDO_DIR=…/ido python3 tools/cloud/score.py fn cand/func_8010E0FC.c func_8010E0FC --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
func_8010E0FC:
  MATCH
python3 -m tools.conveyor.pipeline.blob_unit --tag w6b score func_8010E0FC --with cloud/work/frontier/w6b/func_8010E0FC/w1.c --neighbours
  EQUAL func_8010E0FC: 250 words (kept, c_w1.c)
  locked bodies that differ in this unit: 0
```
(`cloud/matches/func_8010E0FC.c` is `func_8010E0FC/w1.c` plus the header comment; rescored after the edit: MATCH.)

Path (traced uopt, `runs/e0fc_*`):
1. Residual (2) of w5b — the by-value colour parameter of the inlined `resource_set_color` was a register web
   (a0), renumbering the temp ring. Oracle: forcing that web to split (`p1:w74=s,p1:w83=s`) left only two spill
   stores, i.e. retail has no colour value in a register at all. Passing the colour **by pointer**
   (`resource_set_color(s16 index, u32 *color)`, called with the `u32 gold[1]` / `silver[1]` arrays w5b already
   had in memory) fixed it: 65 → 27, and the remaining 27 were exactly the entity / `alone` s3↔s4 swap.
2. Swap: entity `save 28/9 = 3.11` vs `alone` `12/4 = 3.0`. `force.sh … p1:w7=c18,p1:w90=c17,p1:w158=c17` →
   0 colouring rows. A compiled-out `if (entity) {}` after the final `sound_position_set` extends entity's range
   by one block (29/10 = 2.9) → 0. The same check before `rate = …` or after `entity->flags |= 1` is worse.

## net_state_validate — 9/682 (one register pair)

```
score.py fn cand/c.c net_state_validate --flags '-g0 -O3 -mips2 -G 0 -non_shared'   (c.c = net_state_validate/best.c)
  9/682 words differ
```
Lane A and lane B from w5d are both solved; what is left is a three-way priority order in the last loop.
- **Lane A + regime:** w5d's `t1.c` (`total = 0` before the zero loop) gives the right row pointer; on the t1
  snapshot `force.sh nsv_t1 net_state_validate 831 "p1:w464=s,p1:w465=s"` → **10 rows**, i.e. lane A is right
  and the regime flip is only the two low-priority ranges (76 / `D_8014A118`).
- **Lane B is a source form, not a hoist decision:** with no `flag` local — `if (D_80156994 == 0)` read directly
  in the last loop — the load lands after the data test, the address is rematerialised, and the 76/D_8014A118
  pieces stay at `save −0.167` (the good regime): **12/682** (`L2.c`). (`flag` after `base = …`, `s32 flag`, or
  the read after `D_80150E88[i][0] = 1` all stay in the 481 regime or at 12.)
- L2 residual: data4 (retail a3), the flag load (a1) and the `D_80150F7C` address (a2) are permuted. Oracle
  `force.sh nsv_L2 … "p1:w328=c6,p1:w321=c4,p1:w465=c5"` → **0 rows**. data4 has `save 30/2 = 15` and is coloured
  first; `if (data4) {}` at the end of the loop body (`best.c`) makes it 40/9 = 4.44, which frees a1 for the flag
  load (9 rows), but ties with the address range (4.44) and wins on web number. Two checks, `if (base) {}`, or a
  check after the loop (data4 then needs a home slot: frame 136, 6 rows) do not break the tie.
- **The retail-shaped source is probably one `data` variable for sections 2, 3 and 4** (`M3.c`): its register
  assignment is retail's in the last loop, and `force.sh nsv_M3 … "p1:w455=s,p1:w456=s"` → **0 rows**. Natural
  M3 colours the 76/D_8014A118 piece (13 blocks, `totalsave 8`, save +0.615) instead of leaving it at −0.167.
  A 64-variant generator over six natural choice points (init order, cup2/stunt order, `sum`/`n0..n3` placement,
  declaration order; `gen.py`, `gen/results.txt`) and a 72-variant variable-identity sweep (`gen2.py`) never
  left that regime.
- **Next:** get the split pieces' block sets. The workbench uopt only logs whole-range lineage
  (`CDX_LINEAGE_TABLES`); a hook in globalcolor's split routine printing each new piece's blocks would show
  which block M3 adds to the piece (one use with weight 10 → +10 totalsave) and therefore which statement to move.

## music_tempo_set — 31/245 (unit, `--block func_80092B80`)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w6b score music_tempo_set --block func_80092B80 --with cloud/work/frontier/w6b/music_tempo_set/best.c
  FAIL music_tempo_set: 31 of 245 words differ
       umerge inlined into it: resource_set_object
```
- The three `D_8012E700[(s16)x].object` stores (+20, u16) are the **same inlined setter func_8010E0FC uses**,
  `static void resource_set_object(s16 index, u16 object)`. Evidence: retail keeps the wheel index in v0 (an
  inlined-parameter web) and every inlined call adds 8 bytes of frame, which is what w5c's `char buf[16]` stood
  in for. 130 → 38 aligned rows with `buf` removed.
- No `vehicle` local in the first block (38 → 34).
- `if(model){}` after the second `model = &player_array[player]` ends the block, so the player reload for
  `func_80092B80`'s argument (a0) is separate from the model-address load (t7) as in retail (34 → 31). Any
  condition works (`if(player){}` too).
- w5c's "player piece in t2" is gone with these changes (ugen's temp pool now starts where retail's does).
- Residual: frame 96 vs 88 (with any one of the three calls removed the frame is 88; macro or direct-store forms
  for any one call are far worse: 68–156 rows), so every argument-home offset is 8 off (~20 rows); and in the
  first block retail reloads `model` (`lw a1,40(sp)`) before the mask selection and does both loads before both
  stores. Next: find what in retail saves the 8 bytes (an inlined parameter that is not given a home, e.g. the
  object argument as a plain variable or constant in one call site).

## audio_mixer_main — 31/183, mechanism complete

```
blob_unit --tag w6b score audio_mixer_main audio_priority_find --internal audio_priority_find --keep audio_mixer_main --with best.c
  FAIL audio_mixer_main: 31 of 183 words differ          (34 aligned rows)
  EQUAL audio_priority_find: 108 words (internal)
… --with t_cm1.c                       59 of 183 (184 words); 28 aligned rows
… --with proof_test_cm1_arm_notneg.c   48 of 183 (184 words);  6 aligned rows (only the arm's encoding)
```
w5d proved retail keeps all three `k + 1` (test, arm, increment). New:
1. **The test is `k == count - 1`.** ugen re-forms `equ(k, add(count,-1))` as `addiu t9,s0,1; bne t9,v0`, i.e.
   retail's exact test, but in uopt it is a different expression from `add(k,1)`, so the test is no longer a
   source of availability (trace `runs/amm_cm1`: bit 81 `equ.J(k, add.J(count,-1))`).
2. With that test, the arm and the increment still CSE: PRE **inserts** `k + 1` into the then-arm (bit 88
   `antloc 31,40 | DELETE 40 | INSERT 30`) and the increment becomes `move s0,s2`. So the arm and the
   increment must also be different ucode expressions. `next = -~k` (not natural) proves it: with
   the count-1 test the whole function is right except the arm's two instructions (`nor; negu` for `addiu`).
3. So retail's ucode has three distinct expressions with k in a register throughout. An address-taken k
   (`x_deadptr.c`, count-1 test) does give three separate computations, but the call then kills `mpy(k,80)`
   (post-call `multu` recompute), adds home stores and grows the frame — not retail.
4. Not distinct (all canonicalised to `add.J(k,1)`, 59 rows): `k + 1U`, `(unsigned)k + 1`, `1 + k`,
   `k - -1`, `next = k; next++`/`++next`/`+= 1`, `++k`/`k += 1`, while-loop, `register`, ternary (statement and
   in the call argument), inlined `next_index(n)` helpers (s32/s16 parameter, with either test form), `s16`/`u32`
   `next`, a parameter as the counter, `next = first; if (k != count-1) next = k+1;` and other if/else shapes.
- **Next:** find a source construct whose ucode for `k + 1` differs from `add.J(k,1)` but which ugen emits as
  `addiu` (an address-typed `ixa`/`lda` form, or an `inc`); the unit-wide scan in w5d's `redundancy_scan.py` can
  be pointed at `add.A`/`ixa` occurrences of matched functions to find one in house style.

## sound_stop — 12/40 (unchanged; one alternative at 8 aligned rows)

`score.py fn … sound_stop`: `best.c` (w5a) 12/40; `perm/QN.c` 13/40 but 8 aligned rows (no compiled-out check,
statement order pad / count / swap). Findings:
- Retail's join block starts with `i*4` (`sll t7,v0,2` at both loop exits is the join's first instruction plus
  as1's beql copy, not a PRE insertion) and the slot is a **coloured web in v0** (after i dies).
- A slot that is an inlined-helper parameter or a named local becomes a web (`k1.c`: helper
  `func_800B3584(s32 slot) { pad_config[slot].state = 2; }` — the stub before sound_stop) but it is loaded first
  and coloured a1 because i is still live. Putting a `D_80149450[i]` use first (dead check, named pointer,
  helper with `&D_80149450[i]` or `i` parameters) either copy-propagates or breaks the count promotion.
- 23 statement-order permutations (`perm/`), 5 helper-parameter forms (`k*.c`), 4 helper splits (`m*.c`,
  `h*.c` in the unit with `--internal func_800B3584`).
- Next: an ordering in which the first ucode in the join is `mpy(i,4)` (an expression that ugen keeps in a temp
  because it occurs twice in the block) while the slot stays a separate web.

## func_800E95DC — 2 instructions (unchanged)

Built a traced as1 (`~/rush2049/scratch/frontier/w6b/as1/as1`, w4a recipe) and two scripts:
`tools/ulist.sh NAME CAND LABEL` (unit score + ugen listing + as1 trace) and `tools/asmed.sh LABEL NAME EDIT.py`
(edit the function's ugen listing with a Python snippet, assemble it standalone with stock as0/as1, aligned diff;
the standalone build reproduces the residual, plus 2 prologue rows).
- Root cause in as1 terms: case 0's `move s0,t3; sll t0` are hoisted above the dispatch by as1 in both builds;
  for `beq a0,1` retail fills the slot from the **target** (case 1's `move s0,t3`, retargeted +4) where ours
  takes the fall-through `li at,2`. In our case-1 block `move s0,t3` has aftercycles 0 and is scheduled after
  `sw t0,140(sp)`; the as1 trace prints scheduling only, not the slot-filling decision.
- Listing edits with no effect: an extra label on the `beq $2,2` block, `.loc` changes on the dispatch or case 1,
  `move $16,$11` placement (case 1 start/after the loads; case 0's move into the dispatch block), removing the
  case-1 `mul $8`, swapping `li.s`/`li $31` in the default. Swapping the case-1/case-2 tests changes the fill.
- Next: instrument as1's delay-slot filler (`f_do_branch_opt` / `f_do_forward_branches` / `f_eligible` in the
  recompiled `as1.c`) the way w4a instrumented the scheduler, then search listing edits against that rule.

## func_8008E408 — unchanged

w5a's hypothesis (matrix scale = inlined `func_8008B32C`): calling the real `func_8008B32C(o->m, o->m, scale)`
is not inlined by umerge (it is kept; 326 aligned rows), and a static `mscale` copy with two call sites (int or
u32 counters, `src/dst` or in-place form) also misses badly (343-word bodies, 294–347 aligned rows). The inlined
loops therefore come from two separate static bodies or from plain loops; not pursued further (breadth).

---

## Tools added (`tools/`)

- copies of w5d's `pretrace.sh`, `nopre.sh`, `force.sh`, `prereport.py`, `precmp.py` re-pointed at w6b;
- `uv.sh NAME FILES…` / `uva.sh NAME FILES…` (unit score of variants, the second with aligned rows; `EXTRA=`
  passes `--internal/--keep/--block`), `ammv.sh` (audio_mixer_main shortcut);
- `ulist.sh`, `asmed.sh` (traced as1 / listing editing, above).

## What generalises

1. **Pass a stored value by pointer to an inlined setter when retail never holds it in a register.** A
   by-value parameter of an inlined static becomes a register web; a pointer to the caller's memory copy (the
   `u32 gold[1]` idiom) leaves only a load into a temp (func_8010E0FC, 65 → 27).
2. **The same inlined setter appears in several functions.** `resource_set_object(s16 index, u16 object)`
   (`D_8012E700[index].object`, +20) is used by func_8010E0FC and music_tempo_set; look for `(s16)` casts on
   `D_8012E700` indices. Its fingerprints: the index in v0/v1 and 8 bytes of frame per inlined call — an
   unexplained `char buf[N]` filler is usually N/8 inlined calls.
3. **"Fewer named variables" is a lane-B lever.** Reading a global directly in the `if` instead of through a
   local moved a hoisted load back into the loop and kept an address range uncoloured (net_state_validate
   27 → 12). Try deleting the local before moving its assignment.
4. **ugen re-forms `x == y - c` as `x + c == y`**, so a retail `addiu tX,k,1; bne tX,count` can come from
   `k == count - 1`, which uopt treats as a different expression from `k + 1`. When PRE has merged a
   recomputation retail keeps, try the comparison spelled against the other operand.
5. **PRE insertion is the other half of "retail recomputes".** Removing availability from the test is not
   enough: a partially available expression is inserted into the other arm (`INSERT` in the trace) and the later
   occurrence deleted. Read both DELETE and INSERT before concluding.
6. **Ties in `save` go to the lower web number** (variables before constants/addresses). A compiled-out check
   that lands exactly on a tie (net_state_validate data4 at 40/9 vs 4.44) does nothing; aim for a strict
   inequality.
