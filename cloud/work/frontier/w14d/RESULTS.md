# w14d RESULTS (group-recipe small functions)

Builder scratch: `~/rush2049/scratch/frontier/w14d` (toolkit installed with `--reuse wtk`).
Scoring: `blob_unit --tag w14d score NAME --with FILE --internal NAME` and `TAG=w14d tools/trace/us.sh`.
Nothing committed, spliced or edited outside this lane. No `cloud/matches` files written.

## Caller finding (all four targets)
Retail call sites (scan of `score.targets()` words, `callers.py` in this dir):
- func_800DA0BC: called by func_800DB1E0 (+0x4a8, +0x4f8). func_800DB1E0 is an unlocked group (1400 B, blockers
  func_800B4FB0, func_800DA2C0, replay_save_prompt, time_result_display).
- credits_screen: called by func_800D7634 (+0x63c, +0x68c). Unlocked group (1804 B, blockers include credits_screen).
- func_800B5688: called by physics_sym (+0x414, +0x464). Unlocked (blockers func_800B59F0, particle_collision, ...).
- func_800A3724: called by track_render_process (+0x1a0, +0x214) and car_lod_select (+0xb8).

Retail does not save s0-s2 for func_800DA0BC and func_800B5688 / credits_screen: this only reproduces when the
callee is internal (`--internal`) with a caller in the unit. Without a caller the function is absent from the unit.
Every result below therefore uses a **stand-in caller** (a two-call wrapper in the same file, named standin_caller_*,
not retail). Any result from this is **provisional** (not spliceable, not a match).

## Results

| Function | Bytes | State | Flags | Scorer line (this session) | Best file |
|---|---|---|---|---|---|
| func_800DA0BC | 184 | provisional, 2 words off (addiu s0/s2 order, as1 scheduling) | -O3 default, internal, stand-in caller | `cand4d.c: FAIL func_800DA0BC: 2 of 46 words differ \| words 2 ops 0 norm 0 \| frame 24/24` | `func_800DA0BC/best.c` |
| credits_screen | 296 | provisional, 6 words off (same addiu order x2, move s0,v0 and lui-at ordering) | -O3 default, internal, stand-in caller | `cand5s.c: FAIL credits_screen: 6 of 74 words differ \| words 6 ops 2 norm 2 \| frame 24/24` | `credits_screen/best.c` |
| func_800B5688 | 196 | provisional, 4 words off (sw in jal delay slot, addiu order) | -O3 default, internal, stand-in caller | `cand1s.c: FAIL func_800B5688: 4 of 49 words differ \| words 6 ops 4 norm 4 \| frame 24/24` | `func_800B5688/best.c` |
| func_800A3724 | 88 | provisional EQUAL in the unit with stand-in callers (port arrives in a3) | -O3 default, internal, stand-in callers | `cand1s.c: EQUAL func_800A3724: 22 words (internal, c_cand1s.c) \| words 0 ops 0 norm 0 \| frame 24/24` | `func_800A3724/best.c` |

Notes:
- func_800DA0BC: about 14 variants. Frame, saves, and all but two words match. The loop is a do-while with a
  `(s32)slot != (s32)&D_80116D0C` compare (the plain `&` compare gives a `beql` pre-check; `(u32)...` gives `ori`
  instead of `addiu`). The residual is the order of the two `addiu` (s2 before s0 in retail). Variants that moved
  the slot init (cand5h, cand5i, cand6k, cand6l) were worse (19-20 words).
- credits_screen: about 8 variants. Same loop lever as above. Residual: `move s0,v0` placed before the `sb` in
  ours (retail stores through v0 first), `addiu s0` order, and `lui at` before `lui t9` (retail: lui t9 first).
  Variants e2-e5 (slot init moved, `(s8*)[2]` store, truthy condition, D_801105B4 store moved) did not move it.
- func_800B5688: about 3 variants. Residual is only as1 placement: retail puts `sw zero,D_8011AC94` in the delay
  slot of `jal sound_handles_clear`. Reordering the two statements (cand2a) did not change it.
- func_800A3724: `andi t6,a3,0xff` in retail means the port arrives in a3, which only happens with the internal
  calling convention. The function is EQUAL in the unit with two stand-in callers. The real caller
  (car_lod_select, and track_render_process for the second caller) is not verified. The earlier group notes
  (cloud/work/near_miss_B131.md) say the full car_lod_select caller does not match, so this stays provisional and
  car_lod_select remains the blocker. Do not add to cloud/matches (not standalone).

## Integration notes (for the integrator)
- All four are **provisional**. None is a strict match or a group claim, so no `group.json` / `claims` were written.
  The stand-in callers are context only: do not splice from these files.
- Prerequisite for any real match: unlocked callers must be in the unit (func_800DB1E0, func_800D7634, physics_sym,
  track_render_process/car_lod_select). Each is a multi-hundred-byte group in its own lane.
- Scorer flags for every candidate: `-g0 -O3 -mips2 -G 0 -non_shared` (line 1 of each file). `--internal NAME` is needed.
- Own rodata: no float literals or jump tables in these four; no own-rodata caveat.
- func_800DA0B4 (locked neighbour) and func_800D6690 (locked neighbour) were not touched.

## What generalises
- Retail functions that use s0-s2 without saving them are internal-convention bodies. The score only reproduces with
  `--internal` and a caller in the unit. Probing with a stand-in caller is the cheapest way to see the frame.
- A do-while with a pointer compare against the end global reproduces the retail loop exactly. A plain for loop
  adds a pre-check (`beql`) and must be avoided.
- Global address order (`addiu` for the end pointer before the base pointer) is an as1 scheduling residual that
  source spelling did not change in about 25 variants.

## Permission denials
None.

## Iteration status
Variant counts are below the ~50-per-function target: func_800DA0BC about 14, credits_screen about 8,
func_800B5688 about 3, func_800A3724 1 (EQUAL with stand-ins). The remaining residuals are as1 scheduling
(addiu order, delay-slot placement). Further work would need the real unlocked callers in the unit, not more spellings.
