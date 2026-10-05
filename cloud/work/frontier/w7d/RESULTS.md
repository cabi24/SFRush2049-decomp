# Frontier wave 7 — agent w7d (near-miss lane) results

Builder scratch `~/rush2049/scratch/frontier/w7d` (copied from `base` 2026-10-05; `base` lock md5 = local
`blob_matched.lock.json`; `uopt -> ../w5d/uopt` traced build, `as1 -> ../w6b/as1` traced as1). Unit tag `w7d`.
Nothing committed or spliced; nothing under `src/`, `asm/`, `tools/`, `include/`, `tests/` or another agent's
directory edited. func_80087110 and stat_race_update / func_800FE5B0 not touched.

| # | Function | Bytes | Before | Now | Deliverable |
|---|---|---:|---|---|---|
| 3 | `music_tempo_set` | 980 | 31/245 (unit) | **strict MATCH** (`score.py fn`, -O3); unit EQUAL with `--block func_80092B80`, 0 locked bodies differ | `cloud/matches/music_tempo_set.c` |
| 6 | `func_800DFBA0` (+ `mode_select_input`) | 1,192 | 24/298 | **12/298** with one unproven check (int colouring closed; one FP pair + one schedule pair left). Provisional lane (caller unmatched) | `func_800DFBA0/best_unit.c` |
| 7 | `func_80109468` | 1,528 | 12 normalised rows | **7 normalised rows** (i/6 s-register swap closed) | `func_80109468/best.c` |
| 2 | `net_state_validate` | 2,728 | 9/682 | 9/682; the tie is shown to be unbreakable by compiled-out checks in this regime (below) | `net_state_validate/best.c` (= w6b) |
| 1 | `camera_target_track` | 636 | 12 rows in the real group | unchanged; forbidden set of the constant identified | `camera_target_track/best_group/` (= w6d) |
| 4 | `func_800E95DC` | 1,684 | 2 instructions | unchanged; listing-level evidence narrows it to the as1 filler, not the scheduler | `func_800E95DC/g0/` (= w6b) |
| 5 | `entity_update_callback` | 2,184 | 3 mnemonic rows | unchanged; the v0/v1 webs identified (oracle 440 → 399 positional) | `entity_update_callback/best.c` (= w6d) |
| 7 | `func_800E847C` + `func_800E7FA0` | 2,100 + 1,244 | 21 rows / 38 words | unchanged; E847C's s-register rotation traced to E7FA0's IPA register set | `func_800E847C/best_group.c` (= w6c) |

Strict bytes this batch: **980** (`music_tempo_set`, game; a frontier `single`, spliceable as a single with one
unit note below).

Tools added (`tools/`): w6b/w6a/w6d sets retagged for w7d, plus
- `pretrace.sh` patched: the `-l` listing run segfaults the w5d uopt on some snapshots (net_state_validate,
  ordinal 835) and truncates the colouring trace; it now re-runs without `-l` for `cdx`/`w5d` (complete trace).
- `dec.sh LABEL` — colouring decisions in order (web, save, nocs, totalsave, reg, forbidden, decision).
- `area.sh` / `areas.sh NAME FILES…` — unit score + w6a's area-instrumented uopt (`../w6a/uopt2/uopt`): named-local
  area (`f_readnxtinst`) and final area after spill homes, plus aligned rows, per variant.
- `asmfn.sh FN.s NAME EDIT.py` — edit any local ugen listing of one function (e.g. from `o3s.sh`) and assemble it
  standalone (as0+as1), aligned diff; `sf.sh NAME FILES…` — standalone `score.py fn` per file.

---

## music_tempo_set — strict MATCH

```
scp cloud/matches/music_tempo_set.c watchman2:rush2049/scratch/frontier/w7d/cand/music_tempo_set.c
ssh watchman2 'cd ~/rush2049/scratch/frontier/w7d && IDO_DIR=…/ido python3 tools/cloud/score.py fn cand/music_tempo_set.c music_tempo_set --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
music_tempo_set:
  MATCH
python3 -m tools.conveyor.pipeline.blob_unit --tag w7d score music_tempo_set --block func_80092B80 --neighbours --with cloud/matches/music_tempo_set.c
  EQUAL music_tempo_set: 245 words (kept, c_music_tempo_set.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal
```
**Integrator note:** in the unit, umerge inlines the locked `func_80092B80` into music_tempo_set unless it is
blocked (w5c finding). Retail calls it by `jal`, so landing needs whatever override the integrator uses for that
(a `block`/no-inline entry for func_80092B80 in this caller), or check that the splice path builds it the way
`score.py fn` does.

Path from w6b's 31/245 (each step measured in the unit):
1. **Frame 96 vs 88 was alignment padding, not a missing variable.** The w6a area-instrumented uopt showed the
   named area at 48 and spill homes 16. Each inlined `resource_set_object` call adds 8 named bytes, and the named
   locals `model, slot, i (s16), mask, color (u16)` were laid out 4,4,2,(pad 2),4,2 — 18 bytes → 44 with the
   inlines → rounded to 48. Declaring the two 2-byte locals adjacent (`model, slot, mask, color, i`) gives 40 and
   the retail frame of 88 (`areas.sh`: named 40, final 56). 31 → 14 words.
2. **The first block reads `player_array[player]` directly** (no `model` local there). The listing edit first
   (`asmfn`/`asmed`: moving `lw $5,40($sp)` into the test block reproduced retail's model reload position), then
   the source: with the expression, the spilled address is a uopt temp whose base ugen knows, so as1 may load
   `model->flags` before the vehicle store as retail does. 14 → 2.
3. **Line layout of `if (D_80156994 || D_8014978C >= 6) … else …`:** the calls on their own lines (any of four
   layouts) change as1's tie-break so the `lui` of D_8014978C goes before the branch and the 2056-multiply `addu`
   into the delay slot. 2 → 0.
Kept from w6b: the inlined static setter, no `vehicle` local, the compiled-out `if (model) {}` (still needed:
without it 174 words).

## func_800DFBA0 — 24 → 12/298 (provisional lane)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w7d score mode_select_input func_800DFBA0 --internal mode_select_input --internal func_800DFBA0 --with cloud/work/frontier/w7d/func_800DFBA0/best_unit.c
  EQUAL mode_select_input: 38 words (internal, c_best_unit.c)
  FAIL func_800DFBA0: 12 of 298 words differ
```
Trace (`runs/dfb0`): constants 1 and 2 have 40/15 = 2.667; the four variables (count0..2, i) 41/16 = 2.5625 and
tie among themselves, so web-number order (count0, count1, count2, i) already gives retail's v1/a0/a1/a2 once the
constants come after them. The variables have one block more than the constants (the initialising entry block;
a post-loop use only adds weight 1, nocs unchanged). A compiled-out `if (count0 == 1 || count0 == 2) {}` after the
loop gives both constants that block (41/16, ties lost on web number) and closes every integer row (24 → 12).
**Unproven shaping** — labelled in the file. Remaining 12 words: the FP constant 80.0 (retail f28) and one sum
(retail f24) swapped — the same pattern (80.0's 20/15 = 1.333 must drop below the sums' ~1.28), plus one
`addu`/`addiu` schedule pair. Tried without effect: empty `if`s before the loop (on `model`, `slot`), chained or
declaration initialisers, late `slot`. Best next hypothesis: one source structure that gives the *loop constants*
one more block (all three — 1, 2, 80.0 — need exactly that), e.g. a different loop shape or a guard block between
the initialisers and the loop.

## func_80109468 — 12 → 7 normalised rows

`func_80109468/best.c`; `tools/fd.sh func_80109468/best.c func_80109468 --mnem` → `differing rows 7`
(positional `score.py fn` stays ~240/382 because of a one-word shift at +0x4c).
- Retail's clear loop counter is a1 (a separate web) and the main-loop `i` is s2; w6c's source reused `i` for the
  clear loop, so `i` was one 212/14 range that beat the hoisted constant 6 to s1. With the clear loop on `j`/`y`
  (12-variant sweep of loop-variable identity, `v1/`), `i` = s2 and 6 = s1 as in retail.
- Residual: in the dot loop retail has dx a2, dz a3, size (D_801161C4) v1, px v0, pz a1; ours dx v1, dz a0,
  size a1, px a0, pz v1; and the `move a0,s0` hoists above the two UpdateBlit branches. Forcing size to v1 is
  declined (row interferes), so it is an order problem among the inner-loop webs (row 2200, col 1667, x 1300,
  size 1264) — not attempted further.

## net_state_validate — 9/682 (unchanged; why the check lever cannot work here)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w7d score net_state_validate --with cloud/work/frontier/w7d/net_state_validate/best.c
  FAIL net_state_validate: 9 of 682 words differ
```
Current numbering (ordinal 835): flag load w321 5.0 → a1, data4 w328 40/9 = 4.44 → a2, D_80150F7C address
w467 40/10 = 4.0 → a3; retail needs address a2, data4 a3, i.e. data4 strictly below the address.
- The address is hoisted to the preheader, so its range is every loop block; data4's range is a subset of the loop
  unless it is live outside it. Extra in-loop checks only add weight (50/9, `v1/a..h`), and with the
  `if (data4 != 0) {…}` form (`v2/t2`) both are 40/9 — the exact w6b tie, lost on web number. **Within the loop
  the best possible is a tie.**
- A post-loop use makes data4 39/10 < 4.0 and gives exactly retail's colours (`v1/f`, `v2/t4`: 6 aligned rows),
  but data4 is then undefined on the zero-trip path and uopt adds a home store at the loop exit (683 words,
  frame 136).
- A fifth address use is folded before range building (`v4/`); recomputing the test from the expression
  (`v5/`), M3 (shared `data`) with end checks (`v3/`) are worse.
- Next: a structure in which data4 is defined before the loop on the zero-trip path at no cost, or the M3 regime
  with the 76/D_8014A118 piece back at negative save (w6b's lane).

## camera_target_track — 12 rows (unchanged)

`camera_target_track/best_group/` (= w6d's group). Oracle `gforce.sh ctt0 camera_target_track "p1:w101=c8"` →
0 rows; `=s` → 50 rows. New facts from the trace (`runs/ctt0`): calls end uopt blocks, so the constant `1`
(w101) lives only in block 21 (between the func_80092278 call and the osJamMesg call). Retail's colour t1 means
v0, v1, a0, a1, a2, a3 and t0 were all unavailable — exactly func_80092278's register set (v0, v1, a0–a2,
t6–t9, s0, s1; the IPA clobber set) plus a3 (object) and t0 (the result web). So in retail the constant's range
most likely includes the func_80092278 call (or its result web's), i.e. the constant is in a register across
that call. Variants without movement: empty `if`s at five points × three conditions (`v1/`), result/tag/p64/state
orders and declaration order (`v2/`), an inlined `emitter_queue(cam)` helper (frame right without `buf[4]`, result
slot 36 instead of 32; colour unchanged, `v3/H1`).

## func_800E95DC — 2 instructions (unchanged)

Group `func_800E95DC/g0/` (src group + E95DC as member); listing `runs/e95_g0_fn.s`. Traced as1 on the standalone
listing (`af/r.log` on the builder): case 0 and case 1 are scheduled identically (move s0,t3 is node 13, picked
after `sll t0`); retail fills `beq a0,1`'s delay slot with case 1's `move s0,t3`, ours with the fall-through
`li at,2`. Listing edits with **no** effect: moving `move $16,$11` to any of 6 positions in case 1, a label before
the `beq $2,2` block, `.loc` changes on the dispatch. So the scheduler is not the cause; the slot filler's choice
between target and fall-through is. Next: instrument `f_do_branch_opt`/`f_eligible` in `../w6b/as1/as1.c`
(lines ~6870 and ~72656) to print the candidate list.

## entity_update_callback — 3 mnemonic rows (unchanged)

Standalone trace via `gt.sh … euc1 entity_update_callback entity_update_callback`. The swapped pair is w5
(`obj->car` load, 56/6 = 9.33 → v0) and w7 (`&D_80154660[car]`, 26/5 = 5.2 → v1): forcing `p1:w5=c2,p1:w7=c1`
fixes the first region (440 → 399 positional rows; w6d's note that these webs are not the pair is superseded).
Further regions differ independently (e.g. the glow-handle load `lw v0,336(t6)` uses a temp address in retail
where ours uses w7), and the phase step uses `bnezl … addiu t9,v1,1` (branch-likely into the join + 8) in retail.
Not pursued (breadth).

## func_800E847C + func_800E7FA0 (unchanged)

Trace `runs/e84`: in E847C the callee-saved cost is 22.75 for s5–s8 and 32.75 for s2–s4 — the registers the
internal callee func_800E7FA0 uses are the expensive ones. Ours E7FA0 uses s0–s4, retail E7FA0 uses s0–s5. With
s5 also expensive, E847C's colouring order (m, gc, then the 4.545 tie snd/12/&same_count/8) gives retail's
s6/s7/s8/s3/s4/s5 exactly. **So E847C's rotation is E7FA0's residual**: retail puts the segment loop's `lo` /
`lo[1]` in s0/s1 and the level IV pointer in t5 (ours s0, t2/t4). Fix E7FA0 first.

---

## What generalises

1. **Frame filler can be alignment padding of 2-byte locals.** Named locals are laid out in declaration order
   with natural alignment; an `s16`/`u16` between two 4-byte locals costs 2 bytes of padding and can push the
   rounded area up by 8. Before inventing pads or homes, measure the named area (`areas.sh`) and try putting the
   short locals together (music_tempo_set: the whole "frame 96 vs 88" residual).
2. **IPA register sets drive colouring in callers and inside blocks.** A caller's callee-saved cost depends on
   which s-registers its internal callees use (E847C rotation = E7FA0's s5), and a constant live across an
   internal call is forbidden that callee's clobber set (camera_target_track's t1 = func_80092278's set + a3 + t0).
   When a retail colour needs a strange forbidden set, compare it with the callees' register usage first.
3. **Loop-variable identity again:** a counter reused by an earlier, independent loop joins the two loops into one
   long range and changes priorities (func_80109468's i vs 6). Retail using a different register for the first
   loop's counter is the sign.
4. **A tie between a variable and a hoisted constant/address inside one loop cannot be broken by in-loop checks**:
   the constant's range is the whole loop, so the variable can at best tie, and ties go to the variable. Look for
   a use outside the loop that costs no code, or for a different regime.
5. **One extra block for several loop constants at once** (DFBA0's 1, 2 and 80.0) is a single structural fact,
   not three checks: find the source shape that gives all of them the block.
6. Tooling: the w5d uopt segfaults with `-l` on some snapshots and silently truncates the colouring trace — check
   that `p1dec` covers the procedure (`dec.sh`) before trusting a report.
