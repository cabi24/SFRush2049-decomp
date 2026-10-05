# Wave 1, agent w1b: three large game functions

Date 2026-10-04. Builder scratch `watchman2:~/rush2049/scratch/frontier/w1b`. Nothing here is a match and nothing
is spliceable. All three have a complete natural-C body; the numbers are what `tools/cloud/score.py` printed.

| Function | Bytes | State | Flags | Best source |
|---|---:|---|---|---|
| `entity_process_main` | 1,424 | 20/356 words off (same instruction sequence; last 37 instructions) + 2 own-rodata relocations unverified | `-O3` single | `entity_process_main/best.c` |
| `net_state_validate` | 2,728 | 27/682 words off (14 aligned rows) | `-O3` single | `net_state_validate/best.c` |
| `net_session_update` | 3,568 | 858/892 positional; 880 words produced; opcode-level alignment leaves 85 of 892 unmatched | `-O3` single | `net_session_update/best.c` |

Scorer commands and output (run in the scratch copy, `IDO_DIR` as in the brief):

```
python3 tools/cloud/score.py fn cand/entity_process_main.c entity_process_main --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  20/356 words differ (2 section-relative relocations unverified: .rodata+0x0 at +0x1f0, .rodata+0x0 at +0x1f4)
python3 tools/cloud/score.py fn cand/net_state_validate.c net_state_validate --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  27/682 words differ
python3 tools/cloud/score.py fn cand/net_session_update.c net_session_update --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  858/892 words differ
```

`-O2` was probed on the first structurally right draft of `entity_process_main`: 220 aligned rows against 115 at
`-O3` (it saves `s0` and homes both arguments). All three are `-O3`.

Names are historical labels. None of the three does what its name says (see each section).

---

## What large functions needed that small ones did not

Ordered by how much it cost. Items 1 to 5 are mechanisms that were measured here and that the wave-0 notes and
`cloud/PLAYBOOK.md` do not have.

1. **`as1` is a global optimiser, and the pre-`as1` listing is the diagnostic that explains "impossible"
   residuals.** `ugen -l out.s` prints the linear code `as1` receives (`o3s.sh FILE.c NAME`). `as1` then
   (a) hoists `li` / `lui` / `addiu rX,sp,N` / `move rX,zero` upward across basic blocks into empty delay slots or
   the predecessor block, as far as the destination register is free, including speculatively;
   (b) splits `lw rX,sym` and moves the `lui` half alone; (c) deletes dead instructions and forwards a store to
   the following load. Shown on locked `func_8008A3E4` (its `lui v0` sits two diamonds above the `lw`) and on a
   hand-edited copy where `li $2,192; addu $2,$2,-1` became `move v0,zero` in the first delay slot and
   `addiu v0,v0,191` in the second. Three of the four residual groups in `entity_process_main` were this. A
   hand-edited listing can be assembled and diffed with `asmt.sh FILE.s NAME` (as0 + as1, same flags as the
   pipeline), which separates "ugen emitted a different order" from "as1 moved it".

2. **A `.noalias` region left open at an unconditional branch switches that hoisting off for the whole
   function.** ugen brackets a pointer register's life with `.noalias rA,rB` … `.alias rA,rB`. When the closing
   `.alias` lines are emitted after a `b` (the store was the last statement before a `return`/`else` jump), `as1`
   stops hoisting everywhere, before and after that point. In `entity_process_main` one statement,
   `D_8015B268[v->objnum].flags |= 0x8000; return;`, cost 27 aligned rows and one extra word. Writing it through a
   pointer local (`poly = &D_8015B268[v->objnum]; poly->flags |= 0x8000;`) gives identical instructions for that
   statement and removes the problem. Control pair: `entity_process_main/best.c` (23 rows, 356 words) against
   `entity_process_main/control_direct_hide.c` (50 rows, 357 words). Stripping every `.noalias`/`.alias` line from
   the control's listing and re-assembling gives the same 23 rows; removing only that one pair does too, and
   keeping only that pair restores the 50. `poison.py LISTING.s` finds the pattern. Stripping all directives is a
   diagnostic only: on `net_state_validate` it makes things worse (278 to 294 rows), so retail has them where
   they close properly. Compiler flags do not remove them (`-noPalias`, `-f77alias`, `-nocpalias`, `-noaliasok`
   tried; `uopt -moremotion` changes everything).

3. **One C variable is one register, so variable identity has to be recovered, not just types.** uopt colours a
   named variable as one candidate over all its uses. Reusing or splitting a local changes registers far from
   the edit:
   - `entity_process_main`: the four airdist differences are held in `d1`, `offset`, `fscale`, `bscale` (the
     variables used later for other things). Giving them their own names costs 20 rows.
   - `net_state_validate`: a separate accumulator for section 2 (`sum`) and section 3 (`total`) 173 to 143 rows;
     `data` shared by sections 2 and 3 but not 4, 45 to 33; where `flag = D_80156994` is assigned, 33 to 14.
   Registers in the target are the evidence (the same register in two places is a hint of one variable, different
   registers for the same role prove two variables), but they do not determine the answer; a generator plus
   batch scoring was needed (`net_state_validate/gen.py`: 16 choice points, 0.3 s per variant, about 1,700
   variants scored, 348 words off down to 27).

4. **A loop whose control variable is reused by a later loop is not unrolled.** Tiny test: `for (k…<4)` then
   `for (k…<8)` leaves the first loop rolled; with two variables both unroll (the 4-trip one disappears into
   four loads). The target's straight-line four-term sum in `net_state_validate` is such a loop. The converse
   is how to keep a loop rolled without an `s16` counter. Related: a variable-trip loop unrolls only when its
   bound is a register value. `for (j = 0; j < max; j++)` unrolls by 4; the same loop against the `s8` global
   `D_80154640` stays rolled, which is what retail has (`net_session_update`, opcode-level distance 114 to 99).

5. **`if (x == 0) continue;` and `if (x != 0) { … }` are different to uopt.** With `continue` the false edges of
   the last inner `if` and the loop latch are separate blocks, so the reload of a call-killed global
   (`D_8014A108`) is inserted at the end of the body and only on the path that made calls. With the block form
   every edge lands on the latch and the reload sits at the loop test on both paths. Retail has the first.

6. **Register pressure has two stable regimes, and a large function flips between them on one-line edits.**
   Low-priority candidates are a multiplier constant (`li rX,76` + `multu`), the address of an array
   (`D_8014A118`) and the address of a scalar read under a condition in a loop (`la rX,D_80156994`). When a
   register is free they are hoisted and the code changes shape in every loop (about 30 more opcode-level
   mismatches in `net_state_validate`, constants into `s0`-`s2`, and then the global loop bound lands in `t2`/`t4` instead of
   retail's `s1`). Retail is in the starved regime in all three functions: every caller-saved register and `ra`
   hold a candidate, constants are shift-add, loop-invariant `nplayers * 76` is recomputed per iteration.
   `regprobe.py DIR FN IMM` histograms which register a global's value received across a batch (in a
   120-variant sample no variant with an explicit four-term sum put the count in `s1`, 37 with loop forms did;
   126 of 160 variants with two pointer loops on separate variables did).

7. **Inlined statics.** `net_session_update` has no `jal`; its eight random draws are an inlined helper, and the
   empty stub `func_800F34D0` directly before it is most likely that helper's remains (inferred). umerge only
   inlines it when written as two nested statics, a two-statement `rand` and a one-statement
   `% (max + 1)` wrapper; a single two-statement function with the modulo inside is not inlined at any of the
   eight sites (8 `jal` remain). The inlined return variable is visible in retail as `move v0,tN; mfhi v0;
   andi tM,v0,0xffff; move v0,tM`. Scorer `fn` mode at `-O3` and the group pipeline inline identically.
   `entity_process_main` showed the opposite: making `ZOID_HidePoly`/`ShowPoly` real inlined functions
   materialises the argument in `v1` and is wrong; they are macros or direct statements.

8. **Literal types and unused locals, at scale.** Same rules as the playbook, more of them per function:
   `fabsf(d) > 20` (int) keeps the constant in one callee-saved FP register across four tests, `20.0f` splits
   it; `rng(d1 * 8.0f, 20.0f, 255.0f)` must stay float or the two 20s merge into one web. Frame size needed
   eight unused locals in `entity_process_main` (the arcade's own unused `pos`/`mat` account for 48 bytes).

9. **Struct recovery was not the obstacle.** Ad-hoc structs with only the touched fields were enough for all
   three (layouts below). The arcade source gave the whole skeleton of `entity_process_main` in minutes and
   nothing for the other two.

10. **Rodata / jump tables.** None of the three has a jump table. `entity_process_main` owns one float literal
    (0.1f at `0x80123938`), reported as unverified; nothing was contorted to hide it.

11. **Time.** About 75 minutes wall-clock for the three, interleaved. The first structurally right draft took
    5 to 10 minutes each; everything after was allocation and scheduling. About 2,000 compiles in total.
    Without items 1 and 2 `entity_process_main` would have stopped at 45 rows with three unexplained residuals.

Tooling gaps this exposed: `agentC/full.py` crashes on functions with consecutive `nop`s (objdump folds them;
`full.py` here passes `-z`); positional `words differ` is useless above about 300 words (27 real words read as
128 to 640 while one instruction was displaced), so `bscore.py` here also reports an opcode-level alignment
(`mnem-missing`) that tracks structure independently of registers; `cc -S` aborts at `-O3` in the recompiled
driver and `as1 -R` (scheduler trace) is not implemented in it.

---

## 1. `entity_process_main` (0x8008D120): car shadow visual

**What it is.** N64 `AnimateShadow(Visual *v, S16 op)` from arcade `game/visuals.c` (ancestor proven by
structure, see the header of `best.c`). `op == 0` frees the poly; otherwise hide tests, four corner points from
tyre positions, width and length scaling from a per-body table, alpha from mean air distance, then
`func_8008C074(poly, 4, xyz, 0, colour, 0, 0)`.

**State.** 20/356 words, 356 words produced, mnemonic sequence identical. Residual is confined to +0x4d8..+0x568.

**Residual lane.** One pool register and the temp-ring phase that follows from it:
`car->unk35C` is `a0` in retail and `v1` here, and retail evaluates `1 << x` before loading the flags word
(`li t8,1 … lhu t6 … sllv t9,t8,a0; nor; and`), here the load comes first. Everything after is the same ring
shifted.

**Tried on that residual (about 55 variants, no movement).** Named local of five types for `unk35C`, both
fields in locals, reuse of `i` / `whl` / `body` / `xlu` (reuse of `i` does give `a0` but demotes `op` out of
`a1` and widens the ring: 130 rows), commuted and recast forms of `&= ~(1 << x)` (uopt canonicalises: 9 forms,
identical output), mask in a local, clear/set through pointer locals, inlined `ZOID_ClearFlags(objnum, mask)`
in three signatures, condition reordering, switch form, statement joins.

**Best next hypothesis.** `v1` is not free at that point in retail, i.e. one more variable is live across the
tail. The arcade passes `xlu` to `ZOID_UpdatePoly`; a variable that is assigned before the flag code and consumed
by the call (or a block-local `Poly *` per macro expansion, which would also account for the four padding
slots) is the candidate to test. The evaluation order should follow once the pool matches.

**What fixed the other 325 rows** (in order): int literal 20; variable reuse for the differences; frame slots;
pointer local on the hide path (item 2).

**Layouts recovered** (only touched fields):

```c
Visual   : +0x06 s16 objnum; +0x08 s16 slot; +0x14 func
Poly     : D_8015B268[], 0x58 each: +0x00 s16 active; +0x02 u16 flags (0x8000 hidden, low 4 bits per-lane)
CarData  : D_80152818[], 0x3B8: +0x74 f32 dr_tirepos[4][3]; +0xE8 u32 flags (8: hide shadow, 0x10: body row 13);
           +0x35C s8; +0x35D s8; +0x35F s8
ModelDat : D_8014A250[], 0x808: +0x08 u8 body_type; +0x6C4 s16 (resurrect.moving_state); +0x76C f32 airdist[4];
           +0x7DF s8 hide_car
D_8011F914 f32 [][4]  shadow scale per body (front width, rear width, front length, back length)
D_8011AD8C u8 [4]     poly colour, [3] alpha
D_80140418 s8         view/cockpit flag
```

## 2. `net_state_validate` (0x800F2A28): unlock tables

**What it is.** Four independent blocks, each `if (cheat == 1) set everything; else derive from the save record`.
Callee `func_800B78A4(u32, u8)` is a bit count.

**State.** 27/682 words; 14 aligned rows; three mnemonic rows.

**Residual lanes.**
- (A, 4 words) `&D_80150DD8[i]` for the two 19-wide loops is `a1` in retail, `v1` here. Something occupies `v1`
  across those loops in retail. Tried: named row pointer in either or both branches, walking pointer, shared
  with the cup pointer, moving `total = 0` / `total2 = 0`.
- (B, 23 words) last loop. Retail: `lw a3,44(..); beqzl; lw a0,0(a3); lb a1,D_80156994` inside the loop on the
  path past the data test. Here `flag = D_80156994` as the first statement is hoisted out of the loop
  (`best.c`); placed after the test it becomes `la rX,D_80156994` hoisted plus `lb 0(rX)` (`j5.c`, 30 rows).
  Tried 25 variants: flag position, types, `volatile`, struct/array declarations, inlined getter, direct use
  without a local (flips to the over-hoisting regime, 308 rows), block instead of `continue`.

**Best next hypothesis.** Both are item 6: one more candidate is live in retail's sections 3 and 4, so the
address of `D_80156994` gets no register and the scalar is loaded directly. Extend `gen.py` with section-4
choice points (who holds `&D_80150E88[i]`: retail computes it twice, `v1` before the inner loop and `v0`
after) rather than hand edits.

**Layouts recovered.**

```c
Player76 : D_8014A118[], 0x4C: +0x01 u8 index; +0x48 Ref *ref     (ref->car at +0, car->data at +0x2C, data->base at +0)
SaveBase : +0x08C TrackRec tracks[12] (0x60 each: +0x58 s32, +0x5C u16 medal bits)
           +0x50C CupRec   cups[4]    (0x40 each: +0x0C s32, +0x3C u16 medal bits)
           +0x60C StuntRec stunts[8]  (0x0C each: +0x08 u16)
           +0x66C ModeRec  modes[4]   (0x1C each: +0x02, +0x04, +0x06 u16)
           +0x6F4 Tourney             (see function 3)
s16 D_801164C0, D_801164C2, D_801164C4   cheat words (== 1: unlock all)
s16 D_8014A108                           player count
s8  D_80150E30[][13], D_80150EB8[][8], D_80150ED8[][9], D_80150F00[][5], D_80150F40[][6],
    D_80150DD8[][19], D_80150E88[][4], D_80150F7C[]; s8 D_80156994
```

Quirk reproduced as written: after the last cheat loop the code tests `D_80156994` and clears
`D_80150E88[i][2]` with `i` equal to the player count (one row past the last player).

## 3. `net_session_update` (0x800F34D8): tournament setup

**What it is.** See the header of `best.c`. Leaf after inlining; frame 312 with a `s32 order[6]` at `sp+264`.

**State.** Complete body, 880 words against 892. Structure: 85 instructions unmatched by opcode-level alignment
(`bscore.py`), mostly the consequence of allocation.

**Residual lane.** Allocation regime (item 6). Retail: `s8` player count, `s6` drone count, `s7` track count,
`ra` the tournament pointer with a stack home, `s3` the `i + 1` companion of `t0`, `s5` the LCG multiplier, `t5`
the cached seed, and `p` / the `D_80154640` value spilled at `sp+92` / `sp+84`. Here constants 76 and 12 take
callee-saved registers and the counts are spilled or reloaded.

**What moved it** (mnemonic distance 123 at the first inlined draft): `goto`-retry form of the car pick (-6),
`D_80154640` used directly instead of a local (-15, stops the fill loops unrolling), pointer local in the
column-swap loop (unrolls by 2 as retail), `D_8014A108` used directly (-13), pointer local for the
`D_80154FD0[track]++` before the `else` jump (removes the item-2 pattern).

**Best next hypothesis.** Same method as function 2: a generator over (local or global for the three counts,
pointer locals `race` / `ent` that retail clearly has as walking pointers `a3 += 5`, `t1 += 76`, loop-variable
sharing). Retail's swap loop has no `s3` companion, so its control variable is not `i`.

**Layouts recovered.**

```c
Tourney  : SaveBase+0x6F4: +0x06 u8 resume; +0x08 s8 type; +0x09 s8 nraces; +0x0C u32 seed; +0x14 u8 results[6][9]
Entrant  : D_80154450[6], 0x4C: +0x00 u8 car; +0x01 u8 place; +0x02 u16 points; +0x04 u8 finish[24];
           +0x1C u8 pts[24]; +0x34 s16 skill[12]
Race     : D_801543D8[], 5 bytes: track, mirror, reverse, unk3, unk4
u8  D_801543D4 (current player); s8 D_80154628 (last race index); s8 D_80154640 (race count); u32 D_80154658 (seed)
u8  D_80154FD0[] (uses per track); u8 D_80155140[]; u16 D_80155148[][6]
s32 D_8011176C[] (points per finish); s32 D_80111784[] (tracks per type); {u16,u16} D_80111794[], D_801117A4[];
s32 D_801117C4[][12]
```

---

## Files

| Path | What |
|---|---|
| `*/best.c` | best source, header states what it is and which shaping it depends on |
| `*/v1.c` | first complete draft, for comparison |
| `*/residual*.txt` | aligned diff of `best.c` against retail |
| `entity_process_main/control_direct_hide.c` | control for the `.noalias` finding |
| `net_state_validate/j5.c` | alternative for residual B (flag loaded after the data test) |
| `net_state_validate/gen.py` | variant generator (choice points for variable sharing and loop forms) |
| `sc.sh`, `t.sh`, `tg.sh`, `full.sh`, `mn.sh`, `batch.sh`, `grp.sh` | score / aligned diff / group-mode diff / mnemonic-only diff / batch |
| `o3s.sh` | print the pre-`as1` ugen listing for the `-O3` pipeline |
| `asmt.sh` | assemble a (hand-edited) listing with as0 + as1 and diff against retail |
| `poison.py` | find `.noalias` regions left open at an unconditional branch |
| `regprobe.py` | histogram of the register a global's value received over a batch |
| `pipe_remote.sh` | run the pipeline with extra per-stage flags |
| `full.py`, `bscore.py`, `objdiff.py` | builder-side helpers (`full.py` fixed for `nop` runs, `bscore.py` adds the opcode-level metric) |
