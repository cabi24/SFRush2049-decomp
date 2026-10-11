# w16h RESULTS (wave 16, fresh targets)

Targets: func_80108F40 (1320), entity_render_setup (2048), anim_state_update (1840), func_800F45F8 (900).

| Function | Bytes | State | Deliverable |
|---|---:|---|---|
| func_80108F40 | 1320 | **MATCH** standalone + unit EQUAL | `cloud/matches/func_80108F40.c` |
| entity_render_setup | 2048 | near-miss: 60 ops rows, 171 words, frame 184/200, body 517/512 words | `w16h/entity_render_setup/best.c` |
| anim_state_update | 1840 | near-miss: 21 words (8 are own-rodata relocation rows), ops 2, frame 224/224, length 460/460 | `w16h/anim_state_update/best.c` |
| func_800F45F8 | 900 | **MATCH** standalone + unit EQUAL | `cloud/matches/func_800F45F8.c` |

## func_80108F40 — MATCH (radar-car Blit AnimFunc, arcade hud.c AnimateTraffic)

Flags `-g0 -O3 -mips2 -G 0 -non_shared`. Callees all locked (Input_ApplyPadConfig = UpdateBlit,
func_800CF604 = are_we_solid, func_800A61B0 = frwtobod, stat_race_update = SelectBlit at 0x800FE5B0).
The "group" recipe label came only from the `preserved: a1, a2` signature fact; no group is needed.

```
$ score.py fn cand/func_80108F40.c func_80108F40 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_80108F40:
  MATCH
    own .rodata verified at 0x801248CC..0x801248D4
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w16h score func_80108F40 --with cloud/matches/func_80108F40.c --neighbours
  EQUAL func_80108F40: 330 words (kept, c_func_80108F40.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w16h/unit.o (5.6s)
```

Integration: plain single (`splice_singles.py func_80108F40`); no unit_overrides, no groups superseded, no
data-symbol changes. Own .rodata: two copies of 1/480 (0x3B088889) at 0x801248CC/0x801248D0.

Disclosures (also in the file header):
- Unused locals `f32 ldir[3]; s16 angle, flip;` (arcade AnimateTraffic names) are the exact frame residual:
  retail frame 168 has a 16-byte hole at sp+104..119 between `lpos` (120) and `hw` (100); without them the
  frame is 152 with hw/hh at 116/112. 12 + 2 + 2 = 16 bytes.
- `scale = 1.0f / 480.0f` assigned in both mirror arms (two retail literals; Y reuses the arm's `$f0`, negated).

Path (trace, 6 steps from 195 words to MATCH):
1. Hidden() as the inlined static helper (5 call sites; each inlined call reserves 8 bytes) — frame 104 -> 144.
2. Viewer state byte via `D_80152818[me].state` (+0xEF) instead of `D_801528F0[me].state`: with the second
   symbol the car base address was a coloured type-1 web in t0 (ctrace w252) and shifted the whole temp
   ring by one — 186 -> 73 words.
3. `+=` instead of `x = x + E` for X/Y (add.s operand order) — 46 words.
4. `blt->X = ...` before `blt->Y = 16` (ugen temp order) — ops 14 -> 8.
5. Slot-car pointer `gc = &D_80152818[slot]` assigned early (beside `m`): uopt then emits no `.noalias $3,$sp`,
   so as1 keeps the slot-car loads behind the lpos stores as in retail — ops 0.
6. Frame: m/gc/mc/scale declared between `me` and `slot`; 16-byte unused hole (above).

Recovered layouts: see the struct block in the match file. Blit (+0x0E X, +0x10 Y, +0x14 Width, +0x16 Height,
+0x1A Hide, +0x28 AnimFunc, +0x2C AnimID); MODELDAT stride 0x808 (+0x7C6 s16 icon index passed to SelectBlit,
+0x7C8 in_game, +0x7CC s8 drone_type: 2 human / 1 drone); CAR_DATA stride 0x3B8 (+8 dr_pos[3], +0x2C
dr_uvs[3][3], +0xEF s8 state). Globals: D_8011617C/D_80116180 radar frame size (s32), D_80140A04 mirror flag
(s8), D_80116028 Pos[][4] split-screen offsets.

## entity_render_setup — near-miss (60 ops rows; frame 200 vs 184)

N64-only effect callback (no arcade ancestor found): a per-player marker quad for split screen. Callees all
locked (func_8008C544 = vector x 3x3 transform, func_8008C768 = atan2, func_8008B3C8 = magnitude,
func_8008C074 = mesh quad update, group codex_vertices_b88). Single in the unit; no group needed.
Started from the B116 draft (`cloud/work/near_miss_B116/*_visibility.c`, 481/512 words) and rewrote:

```
$ TAG=w16h us.sh entity_render_setup cloud/work/frontier/w16h/entity_render_setup/best.c
best.c: FAIL entity_render_setup: 394 of 512 words differ; compiled body is 517 words, target 512 | words 171 ops 60 norm 60 | frame 184/200
```

What the trace established (each measured):
- Colour is a 4-byte struct copied from `D_8011B578` (`Color4 {u8 r,g,b,a}`), not a union: retail stores
  `sb ..,115(sp)` directly; the union form walks it through a pointer register (481 -> 306 words).
- The HUD arm layout is goto-shaped: A (`angle < -r*0.48 && angle > -pi/2`, side 3) jumps to the shared `y`
  tail; B (`angle < -pi/2` -> yaw += pi, jumps into the `pi/2 < angle` body: yaw -= pi; side 0) jumps past the
  tail to the draw loop; C (`r*0.48 < angle && angle < pi/2`, side 1) is a plain `if` whose body holds the
  tail, the 4x unrolled draw loop, the func_8008C074 call and `return`, so a failing C test falls into the
  world-space path (retail `bc1fl` straight to it). IDO does not tail-merge (tested: duplicated bodies give
  549 words). rodata order (10pi, 1.1, 0.48, -pi/2, 2/pi, -pi/2, pi, pi/2, pi, 2/pi, pi/2, 2/pi, pi/2) fixes the
  source order of the literals; all spelled naturally (`31.415927f`, `1.5707964f`, `0.63661975f`, `3.1415927f`).
- `19.0f` is a variable (`len = 19.0f`): retail's C arm does `neg.s` of the constant, which uopt only leaves
  unfolded for a constant-propagated variable (`-19.0f` and `-(r*g*19.0f)` both fold) — confirmed, rows 61->60.
- `size` (view->scale), `slope`, `threshold` live in memory (homes 52/76/72); arm 2 multiplies `view->scale`
  before assigning `size` (no reload), arm 1 assigns first (store + reload as in retail).
- Retail frame (184): named area 132 = record@180, len?@176, quad[4]@128, scale@124, type@120, i@116,
  color@112, delta@100, then 12 slots 96..52 (angle, yaw, depth, gain, x, slope@76, threshold@72, y, side, view,
  camera@56, size@52); exactly one spill temp (camera->basis @48). Retail spills the camera pointer to its
  NAMED home 56, i.e. `camera` is a variable web; ours copy-propagates `&D_80150B70[type]` into an
  expression web (spill temp), and has 4-5 coloured expression webs that overlap (ctrace e15: w25 a2 =
  D_8014A110, w105 a3 = camera expr, w39/w139 v1), so frame 200.
- a1 is homed at entry (`sw a1,188(sp)`) in retail: the callback receives (effect, 0) (B116 caller audit);
  neither an unused second formal nor `vehicle` as the reused second parameter reproduces the store.

Open residuals (best next hypotheses): (1) make `camera` a variable web (the operand `type` is itself a
memory-resident variable in retail: `lw t4,120(sp)`); (2) `&D_8011B558` is defined before the `mode == 2` test
in retail (an earlier def of a palette pointer?); (3) a0/a1 (`&delta`, `quad`) set before the D_8014A110 test;
(4) world loop: retail bumps the quad pointer before the second/third loads (`-8(s0)`); (5) the a1 home store.
Not tried: pretrace on the camera web; force.sh (residual is not colour-only — frame and structure differ).

## anim_state_update — near-miss, 11 real words (lucent colour), ops 2, frame and length exact

Arcade ancestor: game/visuals.c AnimateTire (proven by structure: suscomp spring, DIRT bounce with Random,
spring_save, wheel object swap at vel 64, rpy, CheckXlu). Started from the locked context copy
`src/blob/groups/codex_visual_tire_module_20261002/anim_tire_donor.c` (433/460 words, 336 differ).

```
$ TAG=w16h us.sh anim_state_update cloud/work/frontier/w16h/anim_state_update/best.c
best.c: FAIL anim_state_update: 39 of 460 words differ | words 21 ops 2 norm 2 | frame 224/224
```
(8 of the 21 rows are `lui at,0x0`/`lwc1 ..(at)` own-rodata relocations in the unit diff: 0.6, 0.05, 0.002, 3.7.)

Levers found (each measured, in order):
1. `Random(t)` is the locked `func_8008B2E4` (inlined by umerge), not `func_8008B2B4()*t/32768`: the macro
   lets uopt turn `* 2.0f` into `x+x`; retail keeps `mul.s` by the propagated 2.0 (67 -> 24 ops rows).
2. Arcade `model[slot].sviscode[tire]` -> `D_8014A250[slot].visual_code[tire]` (retail recomputes the address
   from slot/tire after matrix_scale_apply instead of using `m`).
3. Two `func_8008B32C` calls (one per `tire >= 2` arm), no `scale` local; no `appearance` local (frame 232 -> 224).
4. **Wrong global in the donor**: the race-state byte is `D_8014978C` (`lui 0x8015; lb -26740`), not
   `D_8015978C`.
5. Arcade statement order `slot, car, tire, m` (spill-temp order car@52, m@48).
6. Wheel object index as two ternaries: `index = cond ? 0 : D_80123264[index];` /
   `index = cond ? 16 : index + 3;` — gives retail's `b next; li v1,16` and the un-filled delay slots
   (36 -> 23 rows, length 459 -> 460).
7. `pos[1] = tire_position[1] + *radius + spring` operand order.

Residual (traced): `lucent` (`D_80140418 || appearance & 8`) — retail colours its first split piece a0 and
stores it to its home (124) at the end of the third block; ours colours the piece c13 (ra), because our piece
spans up to the model_bounds_calc call block and every caller-saved colour is forbidden for it
(ctrace a10: w33 `split` then piece `color -> c13`, forbidden0=0x7ffa0000; force.sh to c3 is declined even with
w71/w94/w384 moved off a0, so it is not a simple colour tie). Since c13 costs 0 and a0 0.1, retail's piece
must have had c13 forbidden or a smaller region. Tried: four `if (lucent) {}` holder placements (worse),
`||` forms (frame +8), arcade `player_array[slot]` form, assignment-in-condition (no change).
Disclosure for best.c: `s16 i, j` are unused (arcade AnimateTire names) and are part of the exact frame.
Best next hypothesis: find what ends retail's piece at the row-test block (a use or def of `lucent` there, or
`row` computed via a helper) — trace with `RAW=1 ctrace.sh` webdetail of the piece's block list.
Packaging when it lands: anim_state_update is `keep` context in locked group
codex_visual_tire_module_20261002 (file anim_tire_donor.c) — the group must be superseded with the same name,
moving anim_state_update to members and replacing the donor body; callees model_bounds_calc/matrix_scale_apply
use IPA register params (a3 = v), so it needs the whole-program unit context (WAVE_BRIEF "Group packaging").

## func_800F45F8 — MATCH (end-of-round result record; N64-only)

Callees all locked (net_session_update, func_800F43B8, object_data_allocate in group codex_menu_records_a88).
Started from B80's native draft (113/225 words, frame 56 vs 80).

```
$ score.py fn cand/func_800F45F8.c func_800F45F8 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800F45F8:
  MATCH
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w16h score func_800F45F8 --with cloud/matches/func_800F45F8.c --neighbours
  EQUAL func_800F45F8: 225 words (kept, c_func_800F45F8.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w16h/unit.o (4.9s)
```
Integration: plain single (`splice_singles.py func_800F45F8`); no overrides, no groups, no data-symbol
changes; no own .rodata.

Levers (64 words -> 0): `level = session->level` read once (36); `data` and `session = data + 1780` as two
locals so the record base stays unfolded (12 -> 3, frame 80); declaration order `data, session, count, i, j,
bit, best, first, second, difference, level` (homes first@48 second@44); the xor swap on ONE source line
(SWAP macro) — three lines give as1 store order [0],[1],[0], one line gives retail's [0],[0],[1] (3 -> 0).
Nested-assignment swaps (`a ^= b ^= a ^= b`) get the store order but copy `values` into a2.

Recovered layouts: Player76 input_rec0 (0x8014A118, stride 76, +0x48 handle -> object -> +44 resource ->
data); session record = data + 1780 (Session76: +7 u8 level, +8 s8 mode, +9 s8 length, +16 f32 score,
+20 packed 3-bit results, 9 bytes per car); Ranking76 D_80154450 (+1 u8 place, +2 u16 score);
D_80154628 s8 round/bit cursor; D_80154640 s8 round limit; D_801543D4 u8 current player;
active_player_count s16 (0x8014A108).

## What generalises
- **Line layout decides as1 store order** (func_800F45F8): a 3-statement xor swap on one line (macro) gives a
  different store schedule than on three lines. Try one-line/macro forms when only store order differs.
- **One extern symbol per base**: reading a byte of the car table through a second symbol (`D_801528F0`
  = player_array + 0xD8, as locked func_80108DA8 does) turns the shared base into a coloured address web and
  shifts the whole temp ring (func_80108F40: 186 -> 73 words). Prefer the table's own base symbol.
- **A pointer local assigned in an earlier block loses `.noalias`**, so as1 keeps its loads behind stack stores
  (func_80108F40 `gc`). Confirms handoff §7.6 from the other side: use it to *create* the retail ordering.
- **Neg of a constant survives only via a constant-propagated variable** (entity_render_setup `-len`):
  `-19.0f` and `-(x*19.0f)` both fold; `len = 19.0f; ... * -len` gives retail's `neg.s`.
- **Ternaries give `b next` + unfilled delay slots** where if/else gives branch-likely fills
  (anim_state_update index selection, 36 -> 23 rows, length fixed).
- **Use the locked Random (`func_8008B2E4`) rather than `rand()*r/32768` macros**: the macro lets uopt rewrite
  `*2.0f` as an add.
- **Check prior drafts' globals**: the anim_state_update donor had `D_8015978C` for retail's `D_8014978C`.
- IDO does not tail-merge identical source tails; retail code shared between arms means gotos into a body.
