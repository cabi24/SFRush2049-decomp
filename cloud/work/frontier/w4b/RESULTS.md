# Frontier wave 4 — agent w4b (large near-miss lane) results

Builder scratch `~/rush2049/scratch/frontier/w4b` (copied from `base` on 2026-10-05). The traced uopt was not
rebuilt: the binary w3a built and fidelity-checked (`~/rush2049/scratch/frontier/w3a/uopt/`) was copied into
`w4b/uopt/`. Unit tag `w4b` (and `w4bb` for the second batch lane). Tools: `tools/` = w3a's scripts retargeted to
w4b, plus `tools/fl.sh` (forced uopt, then the ugen pre-as1 listing of one function). `ctrace.sh`, `us.sh` and
`ub.sh` take extra `blob_unit` arguments from `$EXTRA` (needed for `--internal`/`--keep` groups). Nothing was
committed, spliced, or copied to `cloud/matches/`.

**No new matches.** Each function's residual is now traced to specific webs, and a forced colouring
(`CDX_FORCE`) shows what retail's allocation would give. The tables below record what was proved and what was
ruled out, so the next attempt does not repeat these variants.

| # | Function | Bytes | State (unit, quoted) | Allocation reading | Deliverable |
|---|---|---:|---|---|---|
| 1 | `entity_process_main` | 1,424 | `FAIL entity_process_main: 20 of 356 words differ` | needs one more web in v1 over the tail; forced colouring leaves a ugen eval-order residual | `entity_process_main/best.c` (= w1b) |
| 2 | `net_state_validate` | 2,728 | `FAIL net_state_validate: 27 of 682 words differ` | lane A traced to `total`'s live range. The natural fix flips uopt's constant-hoisting regime | `net_state_validate/best.c` (= w1b), `t1.c` |
| 3 | `audio_mixer_main` | 732 | `FAIL audio_mixer_main: 31 of 183 words differ` (`audio_priority_find` EQUAL internal) | the `k + 1` CSE web exists in ours but not in retail: not an allocation contest | `audio_mixer_main/best.c` (= w3b) |
| 4 | `audio_frame_update` | 600 | `FAIL audio_frame_update: 21 of 150 words differ` | forced retail colouring leaves 14 rows of as1 order; v1 blocked over the last loop by an unknown web | `audio_frame_update/best.c` (= w2h) |
| 5 | `camera_collision_avoid` | 412 | `FAIL camera_collision_avoid: 31 of 103 words differ` | not uopt: ugen FP temp order, set by code *after* the first call | `camera_collision_avoid/best.c` (= w2h) |
| 6 | `AdjustSpeed` | 784 | not reworked (best 123–164/196 words: structural, not a near miss) | - | `../w2d/AdjustSpeed/` |
| 7 | `arb_rate_set` | 312 | `FAIL arb_rate_set: 25 of 78 words differ` | the inlined `color` parameter is a coloured variable (w43 → a0); retail copy-propagates it | `arb_rate_set/best.c` (= w3d c2.c) |
| 7b | `func_800B9740` | 408 | not reached | - | - |
| 8 | `entity_lod_select`, `steering_sensitivity` | | not reached | - | - |

All runs used `-g0 -O3 -mips2 -G 0 -non_shared` in the whole-program unit:
`python3 -m tools.conveyor.pipeline.blob_unit --tag w4b score NAME [--internal …] --with cloud/work/frontier/w4b/NAME/best.c`.
Proc ordinals in the current unit (needed for `ctrace.sh … PROC` and `force.sh`): entity_process_main 77,
net_state_validate 804, audio_mixer_main 480 (with `--internal audio_priority_find --keep audio_mixer_main`),
audio_frame_update 396, camera_collision_avoid 489, arb_rate_set 293.

---

## 1. entity_process_main — 20/356, two causes

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4b score entity_process_main --with …/entity_process_main/best.c
  FAIL entity_process_main: 20 of 356 words differ
```
**Trace (`ctrace.sh … 77`).** `car->unk35C` is expression web w208 (save 1.5, nocs 2), forbidden {v0, t1} → v1.
`car->unk35D` is w211 → v0. Forcing `w208=c3` (a0) frees v1, `&D_8011AD8C` (w247) moves from t0 to v1, and the
result gets worse (130 rows). So in retail, **v1 is held over the whole tail** (from the `&D_8011AD8C` computation
to the call) by a web that emits no instruction there. The only v1 web retail shows is the scale-row pointer
`D_8011F914[body]` (w142, `addu v1,t6,t7`).
- A compiled-out check `if (scale == 0) DEBUG_PRINT(…)` just before the `func_8008C074` call (`dF.c`) does extend
  w142 to the tail, but its priority drops to 6/7 = 0.86 < 1.5, so it is coloured after w208 and lands in a1
  (25 words). Placed after the call, w142 crosses the call (spill, 357 words). Placed before an `if` or an
  assignment, the empty `if` disappears before uopt runs (no change at all). Only "before a call" positions survive.
- `force.sh dF … "p1:w142=c2,p1:w208=c3"` (scale v1, unk35C a0) gives **every register right**. What remains is
  21 rows of ugen temp ring. Retail's clear arm evaluates `li 1; sllv` **before** `lhu flags`
  (ring t6 objnum, t7 mflo, t8 1, t9 sllv, t6 lhu, t7 nor, t8 and). Ours always loads first. Pre-as1 listings
  (`o3s.sh`) of 15 spellings show the same `lhu, li, sll, not, and` order for all of them: `&= ~(1<<x)`,
  `= ~(1<<x) & f`, u16/u8/s16/s32 casts on the mask or the count, `0xFFFF ^`, `(x & 31)`, `1U`, `poly->` pointer,
  `| 0x8000`, `& 0xFFFF`. So the clear statement's tree in retail is not `and(load, not(shl))`. `x` or the mask
  probably comes from somewhere else, such as an inlined helper's parameter or a value computed in an earlier
  statement.
- Ruled out: a named local for unk35C (s8/s16/s32/u8/int) at five positions × two debug-check forms (50 variants,
  `g1/`); `i` reuse (128 words, op leaves a1: i's nocs 6→7 reorders the pool).

**Next.** Find a source form where both of these hold at once: (a) a web with priority > 1.5 is held in v1 over
the tail, and (b) the shift is evaluated before the load. Both point to the flag code being something other than
`D_8015B268[v->objnum].flags &= ~(1 << car->unk35C)`. The most likely candidate is an inlined ZOID-style helper
whose mask or lane parameter is live in v1. w1b's three helper signatures did not test it with the
`scale`/debug-check combination.

## 2. net_state_validate — 27/682, lane A explained

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4b score net_state_validate --with …/net_state_validate/best.c
  FAIL net_state_validate: 27 of 682 words differ
```
**Lane A (4 words, `&D_80150DD8[i]` v1 vs a1).** The row-pointer expression web is **w237** (save 110, forbidden
{v0, a0}). `force.sh n0 … "p1:w237=c4"` removes lane A completely (the rows at +678/+704 disappear). For v1 to be
forbidden in retail, `total` (w262, v1, save 250) must interfere with the zeroing loop, so `total = 0` comes before
the 19-wide zero loop, as `total2 = 0` already does. `t1.c` (`total = 0` right after `total2 = 0`) does colour
w237 → a1 in the trace. It also **flips the regime**: two new low-priority candidates appear that b0 does not
have (constant 76 → s2, an address constant → t1), and the result is 474 words. Every position before the
`data == 0` test flips it the same way (positions a–d in `g1/`, 474 each). Putting `total2 = 0` later instead is
neutral or worse (27/29/160). So moving `total` is the right fix, and something else must keep uopt in the starved
regime.

**Lane B (23 words, last loop).** Retail has **no address web at all** for `D_80156994`. Its cheat-branch read is
`lui; lb` too. In `j5.c` (`flag = D_80156994` after the data test) the address becomes type-1 web w468
(save 1.0, nocs 10). It is coloured t1 because t1–t5 are free in section 4: retail uses t1–t5 only in sections 2
and 3. So w1b's "starved regime" explanation is ruled out for lane B. Retail must avoid creating the address
candidate, not lose it in colouring. `volatile` on the read changes nothing (the cheat-branch read shares the web).
A row-pointer store in the loop (aliasing) breaks the D_8014A108 hoist instead (163/490).

**Next.** Both lanes come down to which loop-invariant constants and addresses uopt turns into candidates before
colouring. The `globalcolor` trace only shows webs after that point. A uopt profile for candidate creation
(codemotion / constant-hoisting decisions) would answer it. The concrete test: what in `t1.c` makes the
`li 76`/`&D_8014A118` candidates appear.

## 3. audio_mixer_main — 31/183

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4b score audio_mixer_main audio_priority_find \
    --with …/audio_mixer_main/best.c --internal audio_priority_find --keep audio_mixer_main
  FAIL audio_mixer_main: 31 of 183 words differ
  EQUAL audio_priority_find: 108 words (internal, c_b0.c)
```
Trace (proc 480), loop 2: k w76 s0, record pointer w90 s1, **`k + 1` w80 (type 4, save 10, tot 40) s2**, done w73 s3,
&D_80152800 w123 s4, &D_801543AC w125 s5, 80 w126 s6. Retail is the same list **without w80**, with every later web
one register lower (done s2, s3, s4, 80 in s5), and it computes `k + 1` three times. With s6/s7 free in retail's
loop 2, w80 would be coloured if it existed. So the CSE candidate is never created in retail; it is not a lost
colouring. Spellings tried in the unit (all still CSE): `next = k + 1; if (next == …)`, `!=` with swapped arms, `?:`,
`(next = k + 1) == …`, `next = k; next += 1`, `k + 1L`, unsigned casts on the compare, `(s16)` casts, s16/u16 `k`,
s16 `next`, volatile k (31–147). Like lane B above, this is candidate creation (PRE), which needs a uopt PRE trace.
`audio_priority_find` stays provisional.

## 4. audio_frame_update — 21/150

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4b score audio_frame_update --with …/audio_frame_update/best.c
  FAIL audio_frame_update: 21 of 150 words differ
```
Trace (proc 396), last loop: i w78 (save 30.5) v1, 24 w120 a0, car w64 a1, ctl `&D_80139320[slot]` w83 a2,
anim cb w89 a3 (w83 and w89 both save 5.5, coloured in that order). Forcing the retail colouring
(`"p1:w78=c3,p1:w120=c4,p1:w64=c5,p1:w89=c6,p1:w83=c7"`) gives **14 rows, all as1 order** of the `m->cb`/`m->slot`
stores against the `lui`/`addiu`/`sll` of the loop invariants. So retail has (a) a web in v1 over the whole loop
that is coloured before i (priority > 30.5 or i lower), (b) ctl with lower priority than anim (a longer range,
≥ 3 blocks), and (c) one statement-order difference before the loop. A named `ctl` pointer at six positions is
copy-propagated (no change). Loop-base variants (`m[6+i]`, `m->snd`, walking pointer) score 21–40. Compiled-out
checks after the loop on dst/t/m/car/ft/i/mode score 21–94, none better.

## 5. camera_collision_avoid — 31/103, ugen FP temp order (not uopt)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4b score camera_collision_avoid --with …/camera_collision_avoid/best.c
  FAIL camera_collision_avoid: 31 of 103 words differ
```
The residual is ugen's FP temp choice in the first cross product. uopt colours f0/f2/f12–f18/f20–f24 to webs
(trace, proc 489), which leaves ugen four temps, f4–f10. Measured with pre-as1 listings (`o3s.sh`):
- the FP ring is **reset per function** (two copies of a probe in one file both start at f4);
- with no uopt FP webs in f16/f18, the ring is plain round-robin f4,f6,f8,f10,f16,f18;
- with only f4–f10 free, the order depends on code **after** the first call. Deleting the
  `uvs[0..2] = cos/0/-sin` lines (`x3.c`) gives exactly retail's first-statement pattern (b1→f4, a2→f8, a1→f10,
  product→f6, b2→f4). Removing `pad`, removing the first call, or making `angle` unused (`x1`, `x4`, `x5`) does not
  change it. The `mtc1 a3,$f20` is not the cause.

- 72 generated forms of the `uvs[0..2]` statements (`camera_collision_avoid/g1/`: store order, `0`/`0.0`/`0.0f`,
  `-sinf` vs `0.0f - sinf`, cos/sin through locals) give 10 distinct first-statement orders, all permutations of
  f4/f6/f8/f10. None is retail's (pre-as1 `l.s f4; l.s f8; mul.s f6,f4,f8; l.s f10; …`). The closest is `v061`
  (`f4 f8` then product in f10). Declaring the 16-byte pad before p, as s32, or as named floats changes nothing.
- The ordering matches "most-used temp first" over the whole function (b0 counts f6 30, f4 26, f10 21, f8 20 →
  b0 order f6, f4, f10, f8). This is a hypothesis about ugen, not verified.

So the later code, which is already identical to retail's after the first `jal`, shapes ugen's FP temp order
before it. The difference must be in how ugen sees the later webs (their order or their types), not in the
emitted instructions. **Next:** keep the later statements' instructions fixed and vary their ucode, for example
`uvs[1] = 0` (int) against `0.0f`, `-sinf` against `0.0f - sinf`, or cosf/sinf results through locals. Check each
with `o3s.sh`, comparing the first six FP registers.

## 7. arb_rate_set — 25/78

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4b score arb_rate_set --with …/arb_rate_set/best.c
  FAIL arb_rate_set: 25 of 78 words differ
```
Trace (proc 293): the inlined `func_800A5560` parameter is variable web **w43** (frame −6, u16, save 2) → a0. In
retail the u16 value is a ugen temp used twice (`andi t5,…; sll t8,t5,16; or t2,t8,t5`), so uopt forward-substituted
the parameter. A u16 or u32 local at the call site changes nothing (25 words). An inlined `setfill(r,g,b)` wrapper
(u8 or int parameters) scores 42, and the record's alpha as the GPACK argument scores 36. The v0/v1 swap
(record address vs `&D_8011EA30`) involves w3/w62/w72: the record address w3 (save 7) takes v0 first in ours.

## What generalises

1. **Force first, then look for source.** `force.sh` with the retail colouring separates allocation from
   ugen/as1 effects in one run. In three of these functions, retail's colouring left a non-allocation residual
   (entity_process_main: ugen eval order; audio_frame_update: as1 order; camera_collision_avoid: ugen FP temps).
   A colouring-only search would never have closed them.
2. **"Retail has no such web" is a separate residual class** from "the web lost". net_state_validate lane B
   (`&D_80156994` address) and audio_mixer_main (`k + 1` CSE) both come from candidate creation in
   PRE/codemotion, which happens before `globalcolor`. These need a uopt trace profile for candidate creation; the
   workbench offers only `alias` and `globalcolor`.
3. **uopt's starved/rich regime is a whole-function switch.** Moving one initialisation in section 3 of
   net_state_validate created hoisted-constant candidates in section 1. Read the `webdetail` count (70 vs 72) to
   tell a regime flip from a local change.
4. **Empty `if (x) DEBUG_PRINT(...)` survives to uopt only in some positions.** In entity_process_main it survived
   directly before a call and was dropped before another `if` or an assignment. Check that the trace changed before
   concluding a placement "does nothing".
5. **ugen's FP temp order depends on later code, even though the ring is reset per function.** A first-statement
   FP-register residual can therefore be fixed by statements after it.
