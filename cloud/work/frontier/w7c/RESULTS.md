# w7c results (frontier wave 7, agent w7c, 2026-10-05)

Builder scratch `~/rush2049/scratch/frontier/w7c` (copied from `base`), unit tag `w7c` (second lane `w7cb`).
Tools in `tools/`: `sc.sh` (standalone score), `full.sh` (aligned diff), `o3s.sh` (ugen listing), `ub.sh`/`ubatch.sh`
(blob_unit score + aligned diff, two lanes), `ctrace.sh`/`force.sh`/`pdiff.sh` (w3a traced uopt, copied binary),
`ulist.sh` (unit ugen listing), `udiag.sh` (workbench diagnose), `batch.sh`.

Combined unit check of everything below (all strict members together, `--neighbours`):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w7c score func_800958B8 sfx_stop func_8010DBB8 func_8010C588 func_8010C448 func_8010E72C AdjustSteer menu_transition --with cloud/matches/func_800958B8.c --with cloud/matches/sfx_stop.c --with cloud/matches/func_8010DBB8.c --with cloud/matches/func_8010C588.c --with cloud/matches/func_8010C448.c --with cloud/matches/func_8010E72C.c --with cloud/work/frontier/w7c/AdjustSteer/AdjustSteer_unit.c --with cloud/work/frontier/w7c/menu_transition/menu_transition_unit.c --neighbours
  ... 8 x EQUAL
  locked bodies that differ in this unit: 0
blob_unit score: 8/8 equal
```

## Summary

| Function | Bytes | State | Flags | Where |
|---|---:|---|---|---|
| func_800A79F4 | 240 | 7 words off (one colour tie); 4-word variant with stores swapped | -O3 | `func_800A79F4/best.c` |
| AdjustSteer | 176 | **strict MATCH in group** (+ sibling menu_transition) | -O3 (group flags) | `groups/slot_release/` |
| menu_transition (sibling, unassigned) | 160 | **strict MATCH in group** | -O3 | `groups/slot_release/` |
| func_800958B8 | 100 | **strict MATCH** | -O3 (also -O2) | `cloud/matches/func_800958B8.c` |
| func_800D63EC | 324 | 10 words off (as1/ugen order of one spill store), unit only | -O3 | `func_800D63EC/best.c` |
| particle_lifetime_set | 732 | 59 words off, temp-ring only (pool and FP lanes identical) | -O3 | `particle_lifetime_set/best.c` |
| sfx_stop | 396 | **strict MATCH** | -O3 | `cloud/matches/sfx_stop.c` |
| func_8010DBB8 | 324 | **strict MATCH**, own .rodata verified | -O3 | `cloud/matches/func_8010DBB8.c` |
| func_8010C588 | 320 | **strict MATCH** | -O3 (also -O2) | `cloud/matches/func_8010C588.c` |
| func_8010C448 (sibling, unassigned) | 320 | **strict MATCH** | -O3 (also -O2) | `cloud/matches/func_8010C448.c` |
| func_8010E72C (sibling, unassigned) | 252 | **strict MATCH**, own .rodata verified | -O3 | `cloud/matches/func_8010E72C.c` |
| func_800E7A98 (sibling probe) | 172 | 27 words off, not pursued | -O3 | `func_800E7A98/best.c` |

Assigned: 5 of 8 strict (AdjustSteer, func_800958B8, sfx_stop, func_8010DBB8, func_8010C588), plus 3 siblings
strict (menu_transition, func_8010C448, func_8010E72C). 8 functions, 2,120 bytes.

---

## func_800958B8 — strict MATCH

```
cloud/work/frontier/w7c/tools/sc.sh cloud/matches/func_800958B8.c func_800958B8
func_800958B8:
  MATCH
blob_unit ... score func_800958B8 --with c6.c  ->  EQUAL func_800958B8: 25 words
```
Marks every active entry of the 24-byte table `*D_80144C48` (count `D_801460F4`) with byte +4 = 1, then sets
`D_8011028C = 1`. The whole residual of the natural draft was the last store: retail does `la t8; sb a3,0(t8)` (as1
hoists the `la` into the blez delay slot) — **`D_8011028C` is `volatile u8`**. The neighbouring stub func_8009591C
was tested as a deleted static and is unrelated (inlining it gives the same wrong store).

## sfx_stop — strict MATCH

```
cloud/work/frontier/w7c/tools/sc.sh cloud/matches/sfx_stop.c sfx_stop
sfx_stop:
  MATCH
blob_unit: EQUAL sfx_stop: 99 words
```
`sfx_stop(s16 part, s16 car, s16 tex)`: set/clear the texture of one car part. `D_80139320` is 4 cars x 13 parts of
64-byte records (the 3,328-byte block sfx_position_3d clears); `model` (s32) at +20, passed as `(s16)` to
func_8008D870 (IDO narrows that to `lh 22`). tex -1: `model_data_load(rec->model, 0, mask)`; otherwise
MBOX_FindTexture_Sub(`D_8011B438[tex]`, &index, 0, `D_80140BDC`-1, 1), `func_8008D870`, `model_transform_setup`.
Two closers: (1) declaration order `p, t, index, mask` (mask sp+44, index sp+46, frame 56); (2) **the record index
is 1-D, `D_80139320[car * 13 + part]`**: uopt distributes the *64, ugen spends one more temp than `[car][part]`,
and that single extra temp is what shifts the ring for the rest of the function (30 -> 23 -> 0 words). -O2: 99/99.

## func_8010DBB8 — strict MATCH (own .rodata verified)

```
cloud/work/frontier/w7c/tools/sc.sh cloud/matches/func_8010DBB8.c func_8010DBB8
func_8010DBB8:
  MATCH
    own .rodata verified at 0x801249C4..0x801249CC
blob_unit: EQUAL func_8010DBB8: 81 words
```
Arcade ancestor: game/targets.c `StartGeneric`/`StartKnockdown` family (Visual from `GrabEnvEntry` =
func_80090284, `AddToEnvList` inlined as the push onto `D_801391F0`, `target_sound` inlined as `stat_lap_split(
info.sound, t->slot, t->soundPos, 2)`). N64 additions: `t->state = 7` before the grab, and
`func_80090E9C(3.1415927f, t->pos)` when the hitting car's `RWV` points along `t->dir` (dot > 0).
Closers: the dot product is one named f32 in natural `x*x' + y*y' + z*z'` order (a two-step sum gave the wrong FP
colouring); declaration order **`v, car, info, dot`** gives the 40-byte frame — register-only locals still take
frame slots in declaration order (24 permutations swept; 2 match). Literals 0.0333333f (0x3D088880, not 1/30) and
3.1415927f. Prior attempt `cloud/work/heads_B10` (14/81) had m2c field order.

## func_8010C588 / func_8010C448 — strict MATCH (siblings)

```
cloud/work/frontier/w7c/tools/sc.sh cloud/matches/func_8010C588.c func_8010C588   -> MATCH
cloud/work/frontier/w7c/tools/sc.sh cloud/matches/func_8010C448.c func_8010C448   -> MATCH
blob_unit: EQUAL func_8010C588: 80 words; EQUAL func_8010C448: 80 words
```
Arcade `OverlapTarget(slot, pos, radius, *dist)` (targets.c): collidable test on `model[slot]` (MODELDAT
`D_8014A250`, 0x808, `collidable` s8 at +0x7EA), `veccopy` into a local array, `vecsub(game_car.RWR, p, vec)`,
N64: horizontal distance only, radius += 3.5f, `*dist = gap`, gap > 0 -> 0, then `-2.0f < vec[1] < LIMIT`
(4.0f for C588, 18.0f for C448; the two bodies are otherwise identical). Slot and radius arrive by pointer.
Closers: local arrays (the stack reloads are plain arrays, not `volatile` as in the prior attempt); `vec` declared
before `p`; **`gc = player_array; gc += slot;`** (with `&player_array[slot]` ugen marks the car pointer `.noalias`
against `$sp` and as1 hoists the RWR loads above the array stores — the w6d lever). Also MATCH at -O2.

## func_8010E72C — strict MATCH (sibling of func_8010DBB8)

```
cloud/work/frontier/w7c/tools/sc.sh cloud/matches/func_8010E72C.c func_8010E72C
func_8010E72C:
  MATCH
    own .rodata verified at 0x801249D0..0x801249D4
blob_unit: EQUAL func_8010E72C: 63 words
```
Arcade `StartKnockdown`: `t->state = 4`, `PointInDir(car->RWV, &tmat)` (vector_normalize_length) and
`math_utility(tmat.uvs, t->uv)` (the arcade uv copy loop), then AddToEnvList/target_sound as above. Frame
96 = v + MATRIX(48) + **8 bytes of an inlined static wrapper's two parameters**: `static void PointInDir(f32 *dir,
MATRIX *m) { vector_normalize_length(dir, m->uvs); }`. The same 8 bytes appear with the wrapper taking the GameCar *
and no car local (also matches). 348 declaration layouts (with/without arcade `lv[3]`/`pos[3]`, MATRIX 36/48) never
reached 96 without it.

## AdjustSteer + menu_transition — strict MATCH in group `groups/slot_release`

```
ssh watchman2 'cd ~/rush2049/scratch/frontier/w7c && IDO_DIR=... python3 tools/cloud/score.py group cand/slot_release'
Members:
AdjustSteer:
  MATCH
menu_transition:
  MATCH
menu_item_value_get:
  MATCH
(context func_800CC040 29/224, func_800CBF2C 58/69, menu_back 3/33 differ: unchanged from the locked group)
blob_unit: EQUAL AdjustSteer: 44 words; EQUAL menu_transition: 40 words
```
The group is the locked `src/blob/groups/menu_cc040_20261004` group.c with two edits: AdjustSteer defined, and
menu_transition rewritten; `claims: [AdjustSteer, menu_transition]` (menu_item_value_get is already locked; drop it
from members when installing, or keep the file as a superseding group — integrator's choice). Flags are the group's
(`-Wab,-r4300_mul`).
AdjustSteer (historical label) releases a slot's data block: `if (s->data) { lock; audio_reverb_update(data, 0)
(= heap free, IPA args in a1/a2, callee clobbers s0/s1 so the caller saves them); unlock; s->data = NULL;
D_80144030[s->id].seat[s->seat].busy = 0; }`. `Slot` gained `u8 seat` at +17; `D_80144030` is 0x304-byte records
with 40-byte entries from +0x64 (busy at +0x22; offsets inferred, only +0x86 + 40*j is evidenced).
**Lever (both functions, same 4-word residual):** the released pointer must be a block-scoped local initialised
*inside* the `if` (`if (o->h36 != NULL) { void **old = o->h36; ...`). Initialised before the test, the local's web
is denied v1 (`available0` lacks v1 in the trace; colour a3) and its spill store is emitted after the other one.

## func_800A79F4 — 7 words (best.c), lane: allocation tie

`best.c` (7/60) and `best_4words_storeswap.c` (4/60, stores at +28/+30 swapped, registers right).
```
cloud/work/frontier/w7c/tools/sc.sh best.c func_800A79F4       -> 7/60 words differ
force.sh c2 func_800A79F4 322 "p2:w53=c9,p2:w57=c8"           -> differing rows 0 (oracle)
```
Allocates a slot in the 200-entry, 32-byte pool `D_80140BF0` (historical `pad_config`; first free entry with
`state == 2`, else append; count `D_801613AC`, high-water `D_8013C234`), returns the index. Record: p0, p4, id(s16
+8), +0xA, +0xC, +0xE, w +0x10, h +0x12, alpha u8 +0x14 = 255, +0x15, state s8 +0x16, clip x18/x1A = 0, x1C = h-1,
x1E = w-1 (the clip block that `Input_SetAnalogBounds` edits; inlining it as a setter gives reloads, not retail).
Recovered: s32 params, `for` loop on the global count, the b store first in source (as1 sinks it; that also bars v0
from w/h). Residual: w and h param webs tie (save 0.5, nocs 2) and the lower web number (first occurrence) wins t1;
retail needs w first while ugen order (temps t6/t7) proves h-1 is computed first. Forcing the two colours gives the
exact retail object. Tried (~2,400 compiles, mostly sweeps under `sweeps/`): every placement of the w/h/b stores
(1,296 + 676 + 338 variants), field read-backs, chained assignments, dead reads `if (w) {}` at every top-level point,
locals for w-1/h-1, array vs pointer forms, count local. Next hypothesis: something in source references w (not h)
before the clip stores without emitting code — e.g. an inlined helper whose parameter list is (…, w, h) evaluated in
order, or a compiled-out debug check on w only in a position that does not split the final block.

## func_800D63EC — 10 words, lane: as1/ugen order of one spill store (unit only)

`best.c`; `ub.sh best.c func_800D63EC` -> `FAIL func_800D63EC: 10 of 81 words differ`.
Packet allocate: `if (!D_8011025C) return -1; lock D_80142728; p = pop free list D_80146170 (func_8009211C);
p->type = 0; copy four Vec3 a..d; push on D_80146188 (func_80091FBC); p->ready = 1; unlock; return p`.
Closers found: the alloc and post are inlined static helpers (`packet_alloc`, `packet_post`) plus static
`packet_lock`/`packet_unlock` wrappers (frame 48 and the separate homes 28/24/44), and one compiled-out check
(`if (D_80146170.count == 0) {}` or `if (packet) {}` — any of four spellings) before the head load, which splits the
block so the four parameter webs are reloaded and re-spilled around func_8009211C as in retail (70 -> 58 -> 22 -> 10).
Residual: retail `lw a1,8(a0)` comes after the four reloads and `sw a1,28(sp)` sits in the jal delay slot; ours
loads first and stores before the parameter spills (ugen emits the home store at the definition). 50 placements of
one/two empty checks (5 points x 2 conditions) all stop at 10; List-pointer parameters make it worse. Next: trace
as1 (`w6a/tools/as1t_remote.sh`) to see whether the order is ugen's or as1's; if ugen's, the p web must be defined
in the call's block after the parameter spills (e.g. head loaded inside the call argument list).

## particle_lifetime_set — 59 words, lane: temp ring only

`best.c`; `sc.sh best.c particle_lifetime_set` -> `59/183 words differ`. Workbench diagnose (`tools/udiag.sh`):
`verdict=allocation-mismatch ... opcodes=0 gaps=0`, pool lane identical 57/57, fp lanes identical, temp/shared lanes
diverge at the first temp after the first call (retail t8 for the `D_801391E4` reload, ours t5), `lever: none-known`.
Builds the four HUD quads per player count: `D_801391E4 = 1025`; per quad i: name copy `D_8011464C`, texture
lookup `D_80114638 = func_800B24EC(D_801202E8, &tex, 0, D_80140BDC-1, 1)`, vertices from
`D_80114264[nump-1].slot[i].rect * scale` (the same MotionFrame the locked func_800FC9F8 uses),
`D_80114628[i] = func_800A78BC(4, verts, tex, &name, D_801145D4[nump][i] | 0x1210, 1)`, flag clear, and if
`D_8011463C[nump-1] == 1` copy the saved xs/ys into the list points; then `sound_control` on `state_word_a & 0x80`.
Recovered: frame (decl order `i, verts[12], tex, name, k, …`), four named corner floats, `list` local, the
`[nump - 1]` indexing (explains the 0x80114264/-224 address split), volatile `D_80140BDC`. Tried: line layouts,
do-while, 1-D indexing, casts, scale spellings, an inlined helper for the stub func_800B836C (worse).
Next: ugen ring trace (`DKWB_UGEN_TRACE`, workbench capture) to find which statement consumes the three extra temps.

## func_800E7A98 (sibling probe of AdjustSteer, not assigned) — 27 words, stopped

Heap-list unlink + free under the D_80152770 lock. `best.c` is structurally close; retail evaluates
`heap ? heap : head` with the head load duplicated into both arms. Six spellings, no convergence; left for later.

## What generalises

1. **A block-scoped local initialised inside the `if` is a register lever.** A pointer assigned before
   `if (x)` and tested there is denied v1 (traced `available0`); the same value read inside the block
   (`if (o->f) { T *p = o->f; …`) gets v1 and retail's spill order. Closed AdjustSteer and menu_transition.
2. **1-D vs 2-D indexing is a temp-ring lever.** `A[car * 13 + part]` and `A[car][part]` give the same
   address but uopt distributes the *64 in the 1-D form, which costs ugen one more temp and shifts every later
   temp. When the ring is off by one from a multiply-by-constant onward, try the other indexing (sfx_stop).
3. **Inlined static wrappers explain "8 bytes of frame with nothing in it"** (func_8010E72C): a one-line static
   wrapper with two pointer parameters, inlined, takes 8 bytes below the arrays. Two more confirmations of the
   w5b/w6b rule; prefer this over `char pad[8]`.
4. **Register-only locals take frame slots in declaration order** (func_8010DBB8, sfx_stop, particle): sweeping
   the permutations of the declaration list is cheap (24 compiles) and decisive when the frame or the spill
   homes are off.
5. **Same-shape siblings are common in the target code.** func_8010C448 is func_8010C588 with one literal;
   func_8010E72C is the StartKnockdown sibling of func_8010DBB8. Search by callee set (`callers.py`) and by
   identical disassembly (`diff` of `tdis.py` output with addresses cut).
6. **A pointer global stored through `la; s? 0(reg)` once is `volatile`** (func_800958B8) — the existing quirk
   applies to stores, not only loads.
7. The arcade targets.c names fit the N64 target code: Visual = {next, s16 index, s16 objnum, s16 slot, data,
   f32 timeStamp, func}, the push onto `D_801391F0` is `AddToEnvList`, `stat_lap_split` is `target_sound`'s
   positional sound, `func_80090284` is `GrabEnvEntry`.

## Types recovered

- `Visual` (func_80090284 result): next +0, s16 index +4, s16 objnum +6, s16 slot +8, data +0xC, f32 timeStamp +0x10,
  func +0x14. `D_801391F0` = gEnvList.
- `TargInfo D_80117530[]` (48 bytes): VisFunc +0xC, sound (s32) +0x1C.
- `Target`: flags u8 +4, type s16 +0x10, uv[3][3]/pos at +0x14, dir[3] +0x2C, soundPos[3] +0x38, state s16 +0x5A,
  slot s8 +0x5C.
- `player_array` (GameCar, 0x3B8): RWR[3] +8, RWV[3] +0x14. `D_8014A250` MODELDAT (0x808): collidable s8 +0x7EA.
- `D_80139320`: [4][13] car-part records of 64 bytes, model s32 +20.
- `D_80140BF0`: 200 x 32-byte 2-D slot pool (see func_800A79F4); `D_801613AC` count, `D_8013C234` high-water.
- `D_80114264` MotionFrame[ ] (0xE0 = 4 x 56: rect[4], dx, dy, xs[4], ys[4]), indexed by player count - 1.
- `Slot` (menu group) gains `u8 seat` +17; `D_80144030` 0x304 records with 40-byte entries (busy at +0x86 + 40*j).
