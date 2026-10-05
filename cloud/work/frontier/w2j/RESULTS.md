# Wave 2, agent w2j: `render_large_objects` (0x800F93A0, 5,652 bytes / 1,413 words)

Date 2026-10-04. Builder scratch `watchman2:~/rush2049/scratch/frontier/w2j`. Nothing spliced, nothing committed.

| Function | Words | State | Flags | Source |
|---|---:|---|---|---|
| `render_large_objects` | 1,413 | code identical, own rodata unverified by the scorer (verified by hand: 109 of 109 words) | `-O3` group | `cloud/matches/render_large_objects.c` = `render_large_objects/best.c` = `groups/render_large_objects/group.c` |
| `func_800F92C8` | 52 | code identical with a natural literal (`1e-3f`) instead of the locked `extern f32 D_80124638` | same group, internal | same file |

Not a strict `MATCH`: the unpatched scorer cannot check own literals. It is spliceable by the plan's rule
(the splice verifies literal bytes); the integrator has to replace the old group (see "Integration").

## Scorer output

```
python3 tools/cloud/score.py group cand/render_large_objects_group      (in the w2j scratch copy, IDO_DIR as in the brief)
Members:
func_800F92C8:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x74, .rodata+0x0 at +0x78)
render_large_objects:
  MATCH (214 section-relative relocations unverified: .rodata+0x4 at +0x29c, .rodata+0x4 at +0x2a0, ... , .rodata+0x1b0 at +0x1560)

python3 -m tools.conveyor.pipeline.blob_unit --tag w2j score render_large_objects func_800F92C8 --with cloud/matches/render_large_objects.c --neighbours
  EQUAL render_large_objects: 1413 words (kept, c_render_large_objects.c)
  EQUAL func_800F92C8: 52 words (internal, c_render_large_objects.c)
  locked bodies that differ in this unit: 0
blob_unit score: 2/2 equal; object build/blob_unit/w2j/unit.o (4.3s)

python3 cloud/work/frontier/w2j/tools/rodata_check.py <objdump -s -j .rodata of the group object>
object .rodata 448 bytes, retail window 436 bytes (109 words), mismatching words: [], object tail: 000000000000000000000000
```

The 214 unverified relocations are 107 `lui`/`lwc1` pairs on the root's own float literals (`.rodata+0x4` to
`+0x1b0`); the object's `.rodata` equals retail `0x80124638..0x801247E8` word for word, including
`func_800F92C8`'s `1e-3f` at `+0x0` and one literal no instruction references (`0.01f` at `0x8012464C`,
`.rodata+0x14`), which the compiler leaves behind in the same place.

## What the function is

Not a renderer. It is arcade `DoDrones()` with `assign_drones()` in it
(`reference/repos/rushtherock/game/drones.c`), with `place_cars_in_order()` (`maxpath.c`) written into the body:

- `func_800DE860` = `set_catchup()` (model.c), `func_800F92C8` = `linear_interp()` (drones.c, N64 parameter order
  `in1, in2, input, out1, out2`).
- `D_8014A250[]` = `model[]` (stride 0x808: `slot` +0x7C6, `we_control` +0x7CA, `drone_type` s8 +0x7CC with
  DRONE = 1 and HUMAN = 2, `drone_target` +0x7E6, `drone_scale` +0x7EC, `time_boost` +0x7F0, `catchup` +0x400).
- `player_array[]` = `game_car[]` (stride 0x3B8: `dr_pos[3]` +8, `place` s8 +0xEE, `distance` +0x100,
  `weight_index` s16 +0x356).
- `D_80152744` = `num_active_cars` (s8), `D_8013FECB` = `coast_flag`, `D_80152718` = `end_game_flag`,
  `D_80152015` = `lap_flag`, `D_80152030` = difficulty 0..5 (s8), `D_801174B4 & 8` = the training/demo test,
  `D_801543CC` = elapsed race time in seconds (f32).
- N64 changes against the arcade: no `assign_default_paths`, no `num_to_assign` difficulty hand-out, no
  `win_opts` pacer, no second `trackno == 0` pass, no `okay_to_cheat`/`max_boost *= 1.2`. `diff_scale` is
  `1 - difficulty/5`, scaled by a per-car table (`D_80154484[slot][D_8014978C + (D_80152570 ? 6 : 0)] * .125 + .75`)
  when `D_8014A110 == 3`; `place_scale` is 0 at difficulty 5 and `.5/(n-1)` for the leader. A local `place[]`
  caches `game_car[].place` at entry.

## What explained the old residual (1,361 of 1,413 words, "structural frame deficit" 448 vs 472)

The old source (`src/blob/groups/render_large_objects/group.c`) was hand-written from the assembly with its own
variable set. Rewriting the body as the arcade function, with the arcade declaration list, gave 1,413 words on
the first compile. The steps, each measured (`full.py` aligned rows; 214 of the rows in every figure are the
literal relocations, which that tool shows as differences):

| Step | Frame | Words | Aligned rows |
|---|---:|---:|---:|
| old group source | 448 | 1,321 | 1,433 |
| arcade body + arcade declaration list verbatim (`v/a01.c`) | 480 | 1,413 | 673 |
| `place[]` after `ttype`, drop `rel_drone`/`old_index`/`num_to_assign`/`okay_to_cheat` (`v/a02.c`) | 480 | 1,413 | 673 |
| table index in two existing S16 locals instead of two new S32 (`v/a03.c`) | 472 | 1,413 | 609 |
| own variable for the loop that fills `place[]` (`v/a04.c`) | 472 | 1,415 | 269 |
| `if (x) v = 6; else v = 0;` (`v/a06.c`, no `-D`) | 472 | 1,416 | 266 |
| `diff_scale` assigned after the `place_scale` if/else (`v/a06.c -DDM`) | 472 | 1,413 | 214 (= literals only) |

1. **The frame deficit was the declaration list, not an inlined static.** Every declared local owns a slot
   whether or not it is used (S16 scalars pack at 2 bytes, arrays and floats in declaration order from the top
   of the frame down). The retail frame is the arcade list minus the locals of the removed code, plus `place[6]`.
   The old source declared only what it used, so it was 24 bytes short. An inlined `assign_drones` was tested
   (`-DAD_FN`): it adds 8 bytes (488), as the plan says, so retail has either no inlined call here or 8 fewer
   bytes of locals; the two cannot be told apart from this function alone. What stub `func_800F9398` is stays
   open (a deleted `assign_drones`, or the arcade's empty `EndDrones`).
2. **The 95 missing instructions were source shape**: the arcade's `max_boost`/`min_boost`/`max_brake`/`min_brake`
   as variables assigned once (uopt re-evaluates them at each use, hence one literal set per branch; macros give
   the same code), `num_active_cars` read as a global everywhere, the arcade loop forms.
3. **One variable = one register**: the first loop's index is not `index` (609 to 269 rows).
4. **Statement order decides which loop invariant gets the last saved register**: with `diff_scale` before the
   `difficulty == 5` test uopt hoists `&D_80152030` into `s3`; after it, the constant `5` (retail). About ten
   spellings of the read and the test (`-DDA` … `-DDL` in `v/a06.c`) did not move it; moving the statement did, and the three-literal
   pattern in the rate-limit tail came right with it.

Literal spellings: float throughout (`.02f`, `(1.0f - 1.1f)` = `0xBDCCCCD0`, `(.96f - 1.0f)` = `0xBD23D710`,
`(.15f - .05f)` = `0x3DCCCCCE`, `(.05f - .02f)` = `0x3CF5C290`), `99999999` → `0x4CBEBC20`, `70 * 70` → 4900.0.

## Arbitrary choices (same bytes, not evidence of the original)

- Which unused arcade scalars survive: the frame needs ten S16 scalars above `total_drones[]`, two S16 between
  `cars_in_order[]` and `place[]`, 36 bytes below `car_distance`. I kept `i..high_index, high_place`,
  `my_place, ttype`, and `max_brake..diff_scale, m, gc`.
- Names of the reused locals: `high_place` (first loop), `temp`/`high_index` (table index).
- Variables or macros for the four boost/brake expressions.

## Integration (for the owner; not done here)

`groups/render_large_objects/` is a complete group (`members` and `claims`: `func_800F92C8`,
`render_large_objects`; `keep`: `render_large_objects`). It supersedes `src/blob/groups/render_large_objects`
(which locks `func_800F92C8` with an extern literal and carries the old root as context): `blob_group revert
render_large_objects`, copy the new directory in without `"claims"`, splice, then the usual checks. The splice
must verify 109 literal words at `0x80124638`; `func_800DE860` stays a locked single (kept, called by `jal`).
`tests/conveyor/test_dot_d08_scout.py` pins lock state for this function and will need its
`accepted_lock_present` field updated.

## Tools (in `tools/`)

`f.sh FILE.c [lines]` (-O3 group build, aligned diff, full diff left in the scratchpad), `pp.sh FILE.c -D…`
(same through `cpp`, for a choice-point file such as `render_large_objects/v/a06.c`), `o3s.sh` (pre-`as1`
listing), `rodata_check.py` (object `.rodata` against the retail words). Copies of w1b's `full.py`, `bscore.py`,
`objdiff.py`, `poison.py` with paths set to `w2j`; the w1b scratch directory no longer exists on the builder.

## Generalises

- A "frame deficit" has two causes: a missing inlined static (8 bytes each) or missing unused declarations.
  Compare stack offsets of the arrays against the frame top, not the frame size: the gaps between arrays count
  the scalars declared between them.
- When a function has an arcade ancestor, start from the arcade text and delete, do not start from the
  assembly: 1,361 words off to code-identical took about twenty compiles, none of them a blind variant.
- When uopt hoists the wrong loop invariant into the last saved register, move the statement, do not respell it.
