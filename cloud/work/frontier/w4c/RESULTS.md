# Frontier wave 4 — agent w4c

Builder scratch `~/rush2049/scratch/frontier/w4c` (copied from `base`), unit tag `w4c`/`w4cb`. Nothing
committed or spliced. Flags for every result: `-g0 -O3 -mips2 -G 0 -non_shared`.

| Function | Bytes | State | Where |
|---|---:|---|---|
| func_8010D85C | 368 | **MATCH** (own .rodata verified by the scorer) | `cloud/matches/func_8010D85C.c` |
| battle_mode_setup | 348 | **MATCH** (also at -O2) | `cloud/matches/battle_mode_setup.c` |
| camera_lerp_position | 380 | **MATCH** (-O3 only) | `cloud/matches/camera_lerp_position.c` |
| func_8008E0B8 | 140 | 23/35 words off | `func_8008E0B8/best.c`, `NOTES.md` |
| race_position_update | 484 | structurally right; register/frame residual (116/121 positional, size -9) | `race_position_update/best.c`, `NOTES.md` |
| engine_torque_calc unit | 4,132 | not matched: 215/224, 19/452, 273/357 in the unit | `engine_torque_calc/best.c`, `NOTES.md` |

All three matches are also EQUAL in the whole-program unit:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4c score NAME --with cloud/matches/NAME.c
  EQUAL func_8010D85C: 92 words (kept, c_func_8010D85C.c)
  EQUAL battle_mode_setup: 87 words (kept, c_battle_mode_setup.c)
  EQUAL camera_lerp_position: 95 words (kept, c_camera_lerp_position.c)
```
Scorer command (builder, own scratch):
`IDO_DIR=…/ido python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"`

## func_8010D85C — MATCH
```
func_8010D85C:
  MATCH
    own .rodata verified at 0x80121DA0..0x80121DAC
```
Capture-the-flag flag object setup. The model name picks "REDFLAG"/"GRNFLAG"/"BLUFLAG" (table D_80118E08) by
whether it contains "BLU" or "GRN". That name goes through sound_bank_load (historical label), which retail
calls with **5 arguments**: an implicit declaration resolves the old "fifth-argument conflict". The result
is stored in the entity record D_8012E700[id]+0x38. The team digit comes from "_OFFn". At -O2 it does not
match. Quirks, each tested as necessary:
- unused `char buf[16]` plus one unused `s32` give the 88-byte frame;
- `handle` is a word (a u16 lands at sp+70);
- D_80140BDC is read through `volatile`;
- one `s32 i` holds the table index, the entity id and the team digit (all in v1).

The old PR #85 blocker (frame 88 vs 64, 12/92) was the frame plus the shared variable.

## battle_mode_setup — MATCH
```
battle_mode_setup:
  MATCH
```
Its ancestor is the "we control" branch of arcade `game_reckon_all()` (game/reckon.c), pasted and then
adapted. The arcade `scalmul`/`vecadd` calls are written per component. The add must be `RWR + temp`; writing
`temp + RWR` reorders the schedule. It has no quirks. MODELDAT field offsets used: W 76, RWV 544, RWR 556,
UV 748, lasttime 1812, reckon.RWR 1940, reckon.UV 1952, in_game 1992, time_fudge 2036. CAR_DATA state byte
is at +857.

## camera_lerp_position — MATCH
```
camera_lerp_position:
  MATCH
```
Resets the level-object system: the node pool (130×112), per-category counts, records, the mode-6
secondary pool, and the counters that transmission_ratio_get increments. It has no arcade ancestor.
Shaping:
- the two pool set-ups are one inlined static `pool_init(p, mem, count, size)`. The argument order sets
  t6/t7/t8, and the pointer parameter puts the second pool's memory in v0;
- **D_80151688 and D_80151964 are cleared with `*= 0`.** uopt folds the read-modify-write late, after it
  has given the address a register web (retail `lui v0; addiu v0; sw zero,0(v0)`). `= 0`, `&= 0`, `-= x`,
  `volatile`, one-trip loops, and dead `&x` uses all fail.

## func_8008E0B8 — 23/35
`23/35 words differ (2 section-relative relocations unverified …)` (`best.c`). This function normalises a
vector in place and returns its length (0 when the length is ≤ 1e-5f). Retail homes `z` on the stack in a
40-byte frame, and I could not reproduce that naturally. About 190 variants are recorded in NOTES.md.

## race_position_update — structurally right
Fits a string into a pixel width with a "..." tail; NOTES.md has the semantics. The loop's maxlen is a
register variable holding -1. A late-folded `n = len * 0 - 1` reproduces it exactly: the same mechanism as
the camera `*= 0`. Remaining residual:
- `width` lives in a2, saved and reloaded around every call;
- s5 is saved but never used;
- the frame is 120.

## engine_torque_calc + transmission_ratio_get + engine_sound_update
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w4c score engine_torque_calc transmission_ratio_get engine_sound_update --with cloud/work/frontier/w4c/engine_torque_calc/best.c --internal engine_torque_calc
  FAIL engine_torque_calc: 215 of 224 words differ; compiled body is 223 words, target 224
  FAIL transmission_ratio_get: 19 of 452 words differ
  FAIL engine_sound_update: 273 of 357 words differ; compiled body is 374 words, target 357
```
This is the level-object instantiation system; the names are wrong. Starting from the B129 group, I wrote
transmission_ratio_get from scratch (452 → 19 words). The 19 are 18 spill-slot offsets plus the s0/s1
parameter register, which is decided in engine_torque_calc.

In engine_torque_calc, node and matrix tie at priority 2.0 in the uopt trace. Forcing the swap removes
about 70 rows. The rest probably comes from deleted inlined helpers: there is a stub `func_800AB7D0` just
before it, and retail has a-register webs plus a 96-byte frame.

Layouts, the closers and the open residuals are in `engine_torque_calc/NOTES.md`.

## What generalises
1. **Late-folded expressions are source structure.** `x *= 0`, `len * 0 - 1` and similar forms fold in uopt
   *after* the variable or address has a register web. Two signs point to one:
   - retail keeps an address in v0/v1 for a single store;
   - retail keeps a constant in a callee-saved register and converts it to s16 at every call.
   This closed camera_lerp_position and explains race_position_update's s4. (The original was probably a
   macro or constant that expanded to 0.)
2. **A function called with more arguments than its definition takes** (func_8010D85C → sound_bank_load)
   is an implicit declaration in the caller's file; write the call without a prototype.
3. **Inlined s16 getters.** A `lw; sll v1; sra` pair with a-register webs around an entity-table access was
   an inlined `static u32 get(s16 id)` (transmission_ratio_get, −34 words).
4. **Loop counters.** A loop counter whose register is reused later for another value is the same C
   variable. Reusing `index` turned `bne const` into `slti` with pointer strength reduction.
5. **Arcade paste first.** It again matched on the second compile (battle_mode_setup from reckon.c).

Tools added under `tools/`: copies of the w3a and w1f tools retagged for w4c, plus:
- `eu.sh`: scores the engine unit in two lanes;
- `etrace.sh`: traced uopt for the unit;
- `xref.py`: finds which functions reference a data address, from build/game_code.bin.
