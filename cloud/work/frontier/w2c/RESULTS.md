# Frontier wave 2 — agent w2c results

Lock at hand-over: 759 locked. Builder scratch copy `~/rush2049/scratch/frontier/w2c` refreshed from
`~/rush2049/scratch/frontier/base` on 2026-10-05 (asm, include, src, tools, cloud, `*.json`; `cand/` kept).
This agent took over from an earlier w2c agent that stopped before writing this file; all of its candidates were
re-scored against the current lock before being reported here.

| Function | Addr | Bytes | State | Where |
|---|---|---:|---|---|
| `car_angular_velocity_clamp` | 0x800AAE68 | 804 | **strict MATCH (group)** | `groups/deflate_driver/` |
| `championship_standings` | 0x800DC248 | 432 | **strict MATCH (group)** | `groups/championship_standings/` |
| `func_800BA2B8` | 0x800BA2B8 | 436 | **code identical, own rodata checked by hand** (unit EQUAL) | `cloud/matches/func_800BA2B8.c` |
| `camera_track_spline` | 0x800C04CC | 860 | provisional (MATCH with a stand-in caller; real caller `camera_update` unmatched) | `camera_track_spline/best.c`, `camera_track_spline/group_standin/` |
| `object_manager_update` | 0x800B3FA4 | 532 | 2 words off (unit), uses one fake dead read | `object_manager_update/best.c` |
| `draw_text` | 0x800C734C | 556 | 103 words off (pure colouring order) | `draw_text/best.c` |
| `entity_render_setup` | 0x8008C884 | 2,048 | 209 aligned rows off; frame now right (184) | `entity_render_setup/best.c` |

Spliceable now: 1,672 bytes (`car_angular_velocity_clamp`, `championship_standings`, `func_800BA2B8`).

Helper scripts added (all under `cloud/work/frontier/w2c/tools_w2c/`):
`ubatch.sh NAME files…` (blob_unit score, two tags in parallel), `abatch.sh` (same plus the opcode-aligned
row count and frame size, which you need when the body length is wrong), `diag.sh NAME cand.c` (builds a retail
target object from `build/game_code.bin` and runs the vendored workbench `diagnose` against the unit object),
`us.sh` (`TAG=` aware version of `../us.sh`).

---

## car_angular_velocity_clamp — strict MATCH (group)

Real semantics: the game's one-shot zlib 1.0.4 compressor (`deflate_mem`). Full description and shaping quirks
are in the header of `groups/deflate_driver/deflate_mem.c` (three nested one-parameter `__inline` free helpers
give retail's 24-byte spacing of the parked pointers and the 200-byte frame).

The group was **rebased** onto the current locked `src/blob/groups/codex_heap_release_a25` (which gained
`car_damage_visual.c` and `func_800A51D8` after the first w2c agent built it): `group.c`, `alloc_at.c`,
`car_damage_visual.c` are unchanged copies of the locked files, plus `deflate_mem.c`. Only
`car_angular_velocity_clamp` is claimed. No stand-ins.

```
cloud/work/frontier/w2c/grp.sh groups/deflate_driver      # = score.py group on the builder
Members:  audio_reverb_update MATCH, audio_effect_process MATCH, synced_model_render MATCH, MP_TargetSpeed MATCH,
          assign_default_paths MATCH, stat_race_end MATCH, NextMaxPath MATCH, menu_item_select MATCH,
          car_damage_visual MATCH, car_angular_velocity_clamp MATCH
Context:  func_80095F8C MATCH, func_80095EF4 MATCH, audio_buffer_sync MATCH, object_counter_decrement MATCH,
          object_counter_increment MATCH, func_800A51D8 MATCH

python3 -m tools.conveyor.pipeline.blob_unit --tag w2c score car_angular_velocity_clamp \
    --with cloud/work/frontier/w2c/groups/deflate_driver/deflate_mem.c --neighbours
  EQUAL car_angular_velocity_clamp: 201 words (kept, c_deflate_mem.c)
  locked bodies that differ in this unit: 0
```
Integration: this group supersedes `codex_heap_release_a25` (same members plus the new claim). Keep the existing
`prefer_definition` entry for `func_800A51D8` (→ `car_damage_visual.c`).

## championship_standings — strict MATCH (group)

Real semantics: decoder of a 5-bit password (character table → bit buffer, then checksum compare); the whole
cluster is a password codec, not championship logic. Header comments in `groups/championship_standings/group.c`
give the globals and the shaping (one `u16 i` for both loops set to 0 before the `func_800DC120` call and again
in the for-init; `s32 v` for the table value; `pos += 1` in the for-header).

Changed on take-over: the group's context copy of `func_800DC1AC` was an unmatched rewrite; it is now the
**locked source** `src/blob/func_800DC1AC.c` verbatim (it matches in the group as context).
`tournament_trophy_award` (unmatched) stays as context; `__standin_func_800DC120` is the stand-in that the
already-locked group uses for `func_800DC120` (unchanged from the lock; it is not a claim).

```
cloud/work/frontier/w2c/grp.sh groups/championship_standings
Members:  func_800DC120 MATCH, championship_standings MATCH
Context:  func_800DC1AC MATCH, tournament_trophy_award "40/97 words differ" (informational, unmatched)

python3 -m tools.conveyor.pipeline.blob_unit --tag w2c score championship_standings --with <group.c without the
    three context/partner bodies> --neighbours
  EQUAL championship_standings: 108 words (kept, c_cs.c)
  locked bodies that differ in this unit: 0
```
(Passing the whole `group.c` as `--with` replaces the locked `func_800DC1AC` with the group's copy; with the
original unmatched copy that showed `locked bodies that differ: func_800DC1AC`, which is why it was replaced.)
Integration: supersedes `src/blob/groups/championship_standings` (claims `championship_standings`).

## func_800BA2B8 — code identical, own rodata verified by hand

`cloud/matches/func_800BA2B8.c` (left by the first w2c agent, re-scored): index of the point where a path's
closed point list crosses a gate line; header comment has the semantics and quirks.
```
cloud/work/frontier/w2c/sc.sh ../../../matches/func_800BA2B8.c func_800BA2B8 --flags '-g0 -O3 -mips2 -G 0 -non_shared'
func_800BA2B8:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x48, .rodata+0x0 at +0x58)
blob_unit --tag w2c score func_800BA2B8 --with cloud/matches/func_800BA2B8.c --neighbours
  EQUAL func_800BA2B8: 109 words (kept, c_func_800BA2B8.c)   locked bodies that differ in this unit: 0
```
Literal `1e20f` = 0x60AD78EC = retail word at 0x80123DFC (checked with `rw.py`). Splice with
`splice_singles.py func_800BA2B8`. Unlocks `audio_mixer_main` (sole blocker).

## camera_track_spline — provisional

Unchanged from the first w2c agent: `best.c` is MATCH with a stand-in caller.
```
cloud/work/frontier/w2c/grp.sh camera_track_spline/group_standin
camera_track_spline:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x2d8, .rodata+0x0 at +0x320)
```
It is internal in retail (takes `cam` in a3, homes no argument). Its only caller is `jal` at 0x800C1134 inside
`camera_update` (0x800C0AC0, 2,876 bytes, unmatched, blocked by `camera_free_look`, `camera_look_at_point`,
`entity_spawn_callback`). Own literal 202500.0f = 0x4845C100 = retail word at 0x80123E88. Not spliceable until
`camera_update` is matched; then add it to that group unchanged. Struct layouts (`CamKey` 0x44, `CamScene`,
`CamCtl`, `Camera`) are in `best.c`.

## object_manager_update — 2 words off (highest priority, 89 dependants)

Real semantics: pixel width of a text string in the current font (header of `best.c`). Scored only in the
whole-program unit (callee `sound_update_channel` preserves t1/t4 by IPA):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w2c score object_manager_update \
    --with cloud/work/frontier/w2c/object_manager_update/best.c
  FAIL object_manager_update: 2 of 133 words differ
   +118  lbu t7,7(a0)        lbu t7,7(a0)
|                            li a3,-1
   +11c  li v1,-1            li v1,-1
|  +120  li a3,-1
```
Progress this session: 16 → 2 words. `diagnose` on the old best: `allocation-mismatch`, pool lane, webs
`a1<->a2` (width vs the cached `s[pos]` byte), temp ring identical. Lever 9/39 (raise a web's priority by an
extra read) closed it: one empty `if` with an rvalue that reads `width`, `D_80149B70`, `pos` and the counter,
placed in the space arm (`if (width + D_80149B70 + pos + (s32)str) {}`). `if (width) {}` alone fixed width but
left `wide`/`&D_80149B70` (t2/t3) and then `pos`/`&D_80149B70` (t0/t2) swapped; only the four-term read at that
one position gets all four webs right (546 subset×position variants, 600 random expressions).
**This dead read is a fake**, so even at zero words the source would need a natural equivalent.

Constraint discovered: any *second* empty `if` (anywhere, even before the loop) or any extra named local
(`u8 c = s[pos]`) drops 120-ish words: uopt stops hoisting the constants 32/10/12 into s0/s1/s4. The function
is exactly at the register-pressure edge, so the original source has no extra variable there either.

Residual lane: ugen emission order of the two `li -1` (prev before ch) in the space arm. Not moved by:
statement order (all 24 permutations of the four statements in that arm), `prev = ch = -1` / `ch = prev = -1` /
`prev = ch`, physical line joins (8 layouts × 2 orders), declaration order of `ch`/`prev` (41 orders), extra
`ch`/`prev` terms in the dead read, a separate counter variable instead of reusing `str` (13–19 words).

Best next hypothesis: the space arm originally assigned through a construct that also reads those four webs
(an inlined helper taking width/pos/count, e.g. a deleted static "advance" or a macro), which would both supply
the priority and change the order of the two constants. Look for a caller-less stub near 0x800B3FA4 other than
`func_800B41B8` (already locked, the neighbour after it) and at the 107 callers' common helpers.

## draw_text — 103 words off

Real semantics: merge a save slot's best times into the global top-5 tables: for 12 tracks × 3 categories × 5
ranks, insert each positive time from `entry->owner->slot->data->tracks[i].times[j]` into
`D_80150F88[i].times[j]` (and the owner pointer into `D_80151690[i][j]`) when the slot is empty or slower,
shifting the tail down. Structure is identical (139 words, every opcode, the 4× unrolled shift loop).
```
blob_unit --tag w2c score draw_text --with cloud/work/frontier/w2c/draw_text/best.c
  FAIL draw_text: 103 of 139 words differ
```
`diagnose`: pool lane diverges at slot 0; temp ring and all FP lanes identical. Retail colours the *outer*
values first (i v1, j*20 v0, &D_80150F88 a1, constants 4/5/60/12 in a2/a3/t1/t3, data/src bases t0/t2) and gives
the inner-loop values s-registers (k s0, src s5, &times[k] s4, shift-loop pointers s1-s3/s6-s8); ours does the
reverse (constants in s5-s8). The hoisted constants also appear in reverse order (retail 12, 60, 5, 4; ours 4,
5, 60, 12). Same result (103) from -O2 and -O3, `register`, `while` loops, an inlined `merge()` helper, `times`
hoisted/inside, `&&` forms (≈45 variants by both agents); goto loops lose the unrolling (81 words).
Best next hypothesis: the loop nest is written so that uopt's loop-depth weighting does not favour the inner
webs — e.g. the k loop walks a pointer/limit pair that is set up in the j loop, or the outer two loops are one
loop over 36 entries; the reversed constant order is the cheapest signal to test against.

## entity_render_setup — 209 aligned rows off (draft)

Semantics (from the draft and retail): draws the player-position marker quad for one viewport. Early outs set
`D_8015B268[marker->mesh].flags |= 0x8000`. Colour from `D_8011B558[8]`, alpha by `car->alphaMode`, RGB from the
model's slot table (`D_8012E67C`) in mode 6 or the slot itself, alpha capped at 112 / forced 128 in mode 2. In
mode 6 the quad is drawn on screen at the edge the car is off (angle from `func_8008C768(-v.x, v.z)` of the
camera-space delta; sides 3/1 use a vertical offset `scale*2/pi*slope*(yaw-limit)/(pi/2-limit)`, behind uses
`x = r*2/pi*19*yaw*2/pi`), else in world space at the car scaled by `|delta|/25`. Struct layouts (`Car` 0x3B8,
`Model` 0x808, `ViewCam` 0x98, `ViewInfo` 0x48, `Mesh` 0x58) are in `head.h`.

This session: removing the `scale` and `cam` named locals (both are expression temps in retail) and the `pad`
gives retail's 184-byte frame (`best.c`, 237 → 209 aligned rows, `tools_w2c/abatch.sh`). Frame facts:
retail homes car 180, quad 128–175, size 124, view 120 (memory-resident), ? 116, colour 112, delta 100–111,
slope 76, limit 72, ugen temps a3 56 / scale 52 / a2 48. Ours (best.c) is the same from the top down to view;
from colour down everything is 4 bytes higher, because retail has one more 4-byte local between `view` and
`color`, but adding one (`pad`) makes ours spill into 8-byte temp slots and grows the frame to 192/200.
Remaining structural items: (1) retail shares the "vertical" y-offset block between side 3 and side 1 (side 1
falls through into it; ours emits an extra `b`); (2) FP colouring radius f18 / gain f16 / yaw f14 (ours f16/f14/
f18); (3) the draw loop's store order (x, depth, y in retail); (4) `-19.0f` is `neg.s` of a 19.0 held in a
variable in retail (no folding); (5) literal order in .rodata: 31.415927, 1.1, 0.48, -pi/2, 2/pi, -pi/2, pi,
pi/2, pi, 2/pi, pi/2, 2/pi, pi/2 (`rw.py 0x80123904 14`) — that is the order the source spells them.

## Generalises

1. **One dead read with several terms can fix a whole pool lane where single-term reads cannot.**
   `object_manager_update`: four webs out of order; `if (w) {}` fixes one and breaks another; the sum of the four
   variables in one `if`, at one position, fixes all four. A second empty `if` anywhere is catastrophic in a
   function at the register-pressure edge, so put every extra read into one expression.
2. **Diagnose works on game functions with a hand-built target object**: assemble the retail words from
   `build/game_code.bin` as `.word`s under the function's symbol and compare with the `blob_unit` object
   (`tools_w2c/diag.sh`). The relocation caution it prints is expected (the target has none).
3. **When the body length differs, positional word counts are noise** (339–355 of 512 for drafts that differ
   in ~5 places); use an opcode-aligned row count and the frame size together (`tools_w2c/abatch.sh`).
4. **ugen spill temps around calls sit below the declared locals and here behave as 8-byte slots in our
   builds**, whereas retail `entity_render_setup` has three 4-byte temps (56/52/48). Unresolved; worth checking on
   a matched function with several call-spanning expression temps before relying on frame arithmetic.
5. A context function inside a claimed group must be the locked source verbatim (`func_800DC1AC`), otherwise
   `blob_unit score --with group.c --neighbours` reports the locked body as broken.
