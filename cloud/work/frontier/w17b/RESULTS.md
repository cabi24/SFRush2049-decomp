# Wave 17, lane w17b: func_800E4B58 (MaxPathControls) + func_800E398C (avoid_areas)

**Status: MATCH, group `groups/frontier_maxpath_controls/`.** All ten members are EQUAL in the whole-program unit
with real callers. There are no stand-ins and no implicit int. `score.py group` on the builder gives MATCH for every
member, and every function's own `.rodata` is verified. The group also closes the provisional func_800E451C and
func_800E4300.

| function | bytes | state | arcade name |
|---|---:|---|---|
| func_800E4B58 | 2,268 | MATCH (kept root) | MaxPathControls |
| func_800E398C | 2,396 | MATCH | avoid_areas |
| func_800E451C | 1,588 | MATCH (was provisional) | MP_IntervalPos |
| func_800E4300 | 540 | MATCH (was provisional) | nearest track point |
| func_800E397C, func_800E3984, func_800E42E8, func_800E42F0, func_800E42F8, func_800E4B50 | 6 x 8 | MATCH (locked stubs; this group gives them bodies) | AdjustSteer, AdjustSpeed, MP_TargetSteerPos, MP_TargetSpeed, mp_interval_pos, MP_FindInterval |

New game bytes claimed: 2,268 + 2,396 + 1,588 + 540 = **6,792 B**.

## Verification (Pi repo root; tree at 534d2cbe when re-run)
```
$ cloud/work/frontier/w17b/tools/scg.sh cloud/work/frontier/w17b/groups/frontier_maxpath_controls/group.c
  # = blob_unit --tag w17b score <10 names> --with group.c --internal <all but func_800E4B58> --neighbours
  EQUAL func_800E4B58: 567 words (kept, c_group.c)
  EQUAL func_800E451C: 397 words (internal, c_group.c)
  EQUAL func_800E4300: 135 words (internal, c_group.c)
  EQUAL func_800E398C: 599 words (internal, c_group.c)
  EQUAL func_800E397C: 2 words (internal, c_group.c)
  EQUAL func_800E3984: 2 words (internal, c_group.c)
  EQUAL func_800E42E8: 2 words (internal, c_group.c)
  EQUAL func_800E42F0: 2 words (internal, c_group.c)
  EQUAL func_800E42F8: 2 words (internal, c_group.c)
  EQUAL func_800E4B50: 2 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 10/10 equal; object build/blob_unit/w17b/unit.o (7.5s)
```
Builder (`~/rush2049/scratch/frontier/w17b`, `python3 tools/cloud/score.py group cand/frontier_maxpath_controls`):
```
Members:
func_800E397C:
  MATCH
func_800E3984:
  MATCH
func_800E398C:
  MATCH
    own .rodata verified at 0x80124408..0x8012443C
func_800E42E8:
  MATCH
func_800E42F0:
  MATCH
func_800E42F8:
  MATCH
func_800E4300:
  MATCH
    own .rodata verified at 0x8012443C..0x80124440
func_800E451C:
  MATCH
    own .rodata verified at 0x80124440..0x80124454
func_800E4B50:
  MATCH
func_800E4B58:
  MATCH
    own .rodata verified at 0x80124454..0x8012447C
```
In the unit object, the members are laid out in retail order with no symbol-less stubs:
E397C, E3984, E398C, E42E8, E42F0, E42F8, E4300, E451C, E4B50, E4B58, then the locked E5434. This is the evidence for
the stub-to-helper mapping. With the helpers written as statics, the compiler leaves exactly 2 stubs before E398C,
3 after it and 1 after E451C, the same pattern as retail.

## Integration notes
- New group `src/blob/groups/frontier_maxpath_controls/` (`group.c`, `group.json`). No locked group is superseded:
  none of the four bodies was locked, and E451C/E4300 were recorded only in `provisional.json`.
- `provisional.json`: remove `func_800E451C` and `func_800E4300`.
- `unit_overrides.json`: the six stubs are locked as empty singles (`src/blob/func_800E397C.c`, `func_800E3984.c`,
  `func_800E42E8.c`, `func_800E42F0.c`, `func_800E42F8.c`, `func_800E4B50.c`). This group defines them for real
  as the inlined helpers. They therefore need `prefer_definition` entries pointing at this `group.c` and
  `force_internal` entries, following the frontier_camera_scene_manager precedent (func_800C2418/2420/2428).
  func_800E451C, func_800E4300 and func_800E398C are internal under the group rule; the unit runs above passed
  `--internal` for all of them. `func_800E5434`, the stub after E4B58, is not part of this file. Leave it as is.
- No data-symbol changes. All float literals are natural, and the `extern f32 D_801244xx` placeholders of the old
  packets are gone. E4300's `D_8012443C` is now `1e20f`, with bits 0x60ad78ec checked against the image.
- state_utility is still unmatched. It is called once (the "corrupt maxpath" path) with the include/game_types.h
  prototype. Its code does not affect any member's bytes.

## Shaping devices and disclosures
These are also listed in the group.c header. There are no stand-ins, no implicit int, no pad arrays and no
compiled-out reads.

**Unused locals.** All of them are genuine arcade declarations, and each set is the exact frame residual:
- AdjustSpeed `s32 i, slip, spddir, cur_time` (4 slots). The retail frame is 304; without them it is 288. With them,
  every home matches retail: cp 296, tgtdist 200, and the 144/120/116 spill homes.
- MP_TargetSpeed `TrackPt *mp, *nmp` (2 slots, unused). Without them the frame is 296.
- avoid_areas `tmp2, temp[3], dir_list[6][3], dir_weight[6], cur_rate[3]` (31 slots). The retail frame is 288;
  without them it is 160.
- From the earlier packets: mp_interval_pos's unused `invdist`, and E451C's unused nx/ny and volatile D_80152768.

**Float literal spellings.** uopt keys constants by their spelling: `0`, `0.0f` and `0.f` are three different
constants (verified with a 3-line test). So each site uses the arcade value, and only the spelling was chosen.

**Arcade integer literals** were kept where the float form changes the code:
- `/ 4` in AdjustSpeed. IDO strength-reduces `/ 4.0f` to `* 0.25f`; retail has div.s.
- `interp = 1` in MP_TargetSteerPos.

**One-line vector macros.** `veccopy`, `vecsub` and `scalmul` (the arcade function names) are one-line macros.

**`m` in avoid_areas.** `m` is reassigned in the traffic loop, as in the arcade. So the N64-only `m->s6C4 >= 0`
test reads the last car scanned (retail reloads 60(sp)).

## How it closed (traced, in order)
Measurements are unit scores with E451C, E4300 and E398C internal. The variants are in the ignored
`func_800E4B58/e9/`, `func_800E398C/e9/` and the `b*` sweep dirs.

**func_800E4B58**, starting from w16d's best: 503/567 words and 70 mnemonic rows, with the frame at 288 against 304.
1. **The callee convention was not the blocker.** Substituting the dot_avoid_areas E398C (it indexes by
   drone_index, so the parameter goes in s7 and it clobbers s0-s8 and $f20-$f30) left E4B58 unchanged at 503. The
   real blocker was constant webs: a 0.0f web was coloured in $f20 for the whole procedure.
2. AdjustSpeed with the arcade's full local list gives frame 304 with every home matching (499 words).
3. Per-helper zero spellings (`0` in AdjustSpeed/AdjustSteer, `0.0f` in the MP_* helpers and the vy test) give 342
   words and 40 rows. Removing the `dif` local (retail loads D_801543C8 in each arm) and making the vy test `0.0f`
   lets AdjustSpeed's zero take $f16, as in retail.
4. `interp = 1`, `cp->spd = 28` (a constant separate from the compare) and the arcade brake/throttle store order
   bring it to 17 rows.
5. `s16 nmpi` in MP_TargetSteerPos (retail converts each call result once) gives 148 words and 4 rows, and the size
   now equals retail.
6. In MP_TargetSteerPos the lerp is written `(nmp - mp) * interp + mp`; MP_TargetSpeed keeps `base + diff * interp`.
   TargetSpeed indexes D_8012E5E8 directly, because retail keeps the mp/nmp addresses in ring temps. This reaches
   6 words, all in the prologue.
7. **as1 breaks scheduling ties by source line.** The entry copy `move a1,a0` must sort before the inlined
   FindInterval's `move s5,a0`, so func_800E4B58 is defined before its helpers. That is also the arcade order
   (maxpath.c:745 versus :873). Result: EQUAL.

**func_800E398C**, starting from the dot_avoid_areas candidate at 569/599:
1. A fresh arcade transcription gives 542/599: `m` reassigned in the loop, `s16 i, index, hint, can_we_cheat` (the
   order follows the 280(sp) home), and the extra locals listed above.
2. Spelling sweeps (vbatch standalone, which agrees with the unit within a few words), then ctrace. The six
   single-use loop constants that go to $f20-$f30 tie at save 1.666667, so the lowest web number wins. A constant
   that is also used after the loop under the same spelling is split first; its loop piece gets a late web number
   and is not hoisted. That is retail's pattern:
   - `scale2 = 1` shares the spelling of `scale1 = 1`;
   - `.9f` is shared with the post-loop `scale1 < .9f`;
   - the loop `8` is shared with the post-loop `* 8`;
   - branch 1's `cp->spd = 100.0f` differs from the loop's `100`.

   This brought it to 26 words.
3. One-line `veccopy`/`vecsub`/`scalmul` macros reproduce retail's [1],[0],[2] and [1],[2],[0] store orders, because
   as1 orders stores by line: 4 words.
4. `dist = rpos[0] * rpos[0] + rpos[2] * rpos[2]` (arcade operand order): EQUAL.
5. Packaging. The helpers are named after the locked stubs they leave, and the unit layout confirms the mapping.

## What generalises
1. **Literal spelling is a register-allocation lever.** uopt keys constants by spelling (`0`/`0.0f`/`0.f`,
   `1`/`1.0f`). The spelling decides which uses form one web, and so what is coloured, what is hoisted and what is
   rematerialised. Use it when retail rematerialises a constant (`mtc1 zero`, `lui at`) that our build keeps in a
   register, or the other way round. The arcade source's mix of `0`, `0.0` and `1` is usually the right guide.
2. **Loop-constant hoisting ties** (equal save) go to the lowest web number. Sharing a spelling with a use outside
   the loop makes the web split first and lose the tie.
3. **as1 breaks ties by source line.** That gives two levers:
   - function definition order: a caller defined before its inlined helpers changes the prologue order;
   - one-line macros: three stores on one line come out in a different order than on three lines.
4. **An integer literal beside a float** (`/ 4`, `interp = 1`) keeps arcade semantics and differs in code from the
   `.0f` form: `/ 4.0f` becomes `* 0.25f`.
5. **Named pointer variables that retail keeps in ring temps** should be expressions; retail does not keep them in
   registers.
6. **When a blocker is blamed on a callee's convention, measure that first** with a convention-correct stand-in.
   Here the real cause was local (constant webs).
7. **Map inlined helpers to stubs** by making them internal non-static functions and reading the unit object's
   symbol order.

Tools: `tools/sc.sh`, `sc3.sh` and `scg.sh` (unit scoring), `tools/dis.sh` (disassemble a function from unit.o),
`tools/mk.py`. No permission denials.
