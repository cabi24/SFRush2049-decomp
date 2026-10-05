# w2h results (frontier wave 2)

The first w2h agent hit a usage limit before it wrote this file. A second agent took over on 2026-10-05.
The takeover agent refreshed the builder copy (`~/rush2049/scratch/frontier/w2h`) from `base` (759 locked),
re-scored every candidate the first agent left, and checked each one in the whole-program unit
(`blob_unit --tag w2h`). It then continued work. All flags are `-g0 -O3 -mips2 -G 0 -non_shared`.

| # | Function | Bytes | State | Where |
|---|---|---:|---|---|
| 1 | string_copy_format | 436 | 36/109 words off (one register: `t0` vs `a3`) | `string_copy_format/best.c`, `NOTES.md` |
| 2 | camera_first_person | 944 | **group MATCH**, also 5/5 EQUAL in the unit. The locked callee `func_800AD650` must first be changed to the corrected parameter order (in, out) | `groups/camera_first_person/`, `groups/func_800AD4C8_fix/` |
| 3 | audio_frame_update | 600 | 21/150 words off in the unit (colouring of the final loop) | `audio_frame_update/best.c`, `NOTES.md` |
| 4 | main_menu_input | 224 | **group MATCH**, also 3/3 EQUAL in the unit once `func_800D52CC` is internal | `groups/main_menu_input/` |
| 5 | world_physics_tick | 1,564 | structurally right: 60 aligned rows, +3 words | `world_physics_tick/best.c`, `NOTES.md` |
| 6 | audio_channel_priority | 468 | 11/117 words off (three webs rotated) | `audio_channel_priority/best.c`, `NOTES.md` |
| 7 | camera_collision_avoid | 412 | 31/103 words off in the unit (FP temp registers of the first cross product only) | `camera_collision_avoid/best.c`, `NOTES.md` |

Totals: 2 strict group matches (1,168 bytes), both with an integration step. No provisional results.

## 2. camera_first_person: group MATCH; a locked callee must be fixed first

Semantics: for every 32-byte record of D_801525EC[0..D_8015267C) whose tag equals `tag`, the function
rotates the packed 3x3 orientation by m1 and then m2, and writes it (s16, scale 16384) to
D_801497F8[rec->matrix]. It also rotates the packed position (s16 plus a 5-bit fraction) about `origin` and
writes it back to D_8015201C[rec->point]. No arcade ancestor was found. The quirks are in the file header.

```
cloud/work/frontier/w2h/grp.sh groups/camera_first_person
Members:
camera_first_person:
  MATCH
Context (informational; excluded from exit status):
func_800AD650:
  MATCH
```

The first agent's group listed w1g's provisional roots (func_800C3AD0, input_process_controller, func_800AD4C8)
as members. That group matched, but it would have spliced provisional code. It is kept for reference in
`camera_first_person/old_group_w1g_members/`. The new group has a single member. Its file defines the
internal callee `func_800AD650` and one kept stand-in caller (`__standin_func_800AD650_a`), which keeps the
callee out of line. Without the stand-in, the callee is inlined: 225/236 words differ.

**Real unit, with the source as the first agent left it:** `FAIL camera_first_person: 2 of 236 words differ`
(+0x94/+0x98 differ: `addiu a1,s0,4` and `move a0,s2` are emitted in swapped order). Retail sets up a1 (out)
before a0 (in). The locked `func_800AD650` in `src/blob/groups/func_800AD4C8/group.c` is written
`(void *out, void *in)`. Argument set-up order is the callee's source parameter order, so the real order is
**(in, out)**: a locked callee proved wrong by its caller (plan §0 item 5).
- `groups/func_800AD4C8_fix/group.c` is the locked group.c with the parameter order swapped (in the
  definition and at both call sites). Its members still score `func_800AD650 / handbrake_apply /
  func_800C36A0: MATCH`. The context functions are unchanged or slightly better (315/362 and 306/363,
  previously 317/308).
- `blob_unit --tag w2h score camera_first_person func_800AD650 func_800C36A0 handbrake_apply func_800AD5D0
  --with …/camera_first_person/u/cfp_only.c --with …/camera_first_person/u/ad4c8_fix.c` gives
  `blob_unit score: 5/5 equal`.

**Integration order:** replace src/blob/groups/func_800AD4C8/group.c with the fixed copy and re-splice that
group. Then splice `groups/camera_first_person` (drop "claims"). Then run `blob_unit check`. Both files
define `func_800AD650` identically after the fix, so a `prefer_definition` entry may be needed if the unit
complains.

## 4. main_menu_input: group MATCH; func_800D52CC must become internal

Semantics: under the queue lock D_80142728 (osRecvMesg blocking, then osJamMesg), copy four Vec3 into a node
at +0x0C/+0x18/+0x24/+0x30. A node of -1 is skipped.

```
cloud/work/frontier/w2h/grp.sh groups/main_menu_input
Members:
main_menu_input:
  MATCH
func_800D52CC:
  MATCH
Context (informational; excluded from exit status):
object_activate:
  MATCH
```

- In the unit with the current lock (func_800D52CC is a kept -O2 single, `void (void)`):
  `FAIL main_menu_input: 29 of 56 words differ`.
- With `--internal func_800D52CC`: `blob_unit score: 3/3 equal` (main_menu_input, func_800D52CC,
  object_activate).
- The empty callee is internal, and it has a parameter that is read. Its caller therefore keeps node/a/b/c in
  t0-t2/a3 across the `jal`. Integration: `blob_splice revert func_800D52CC`, then splice the group. The
  group defines func_800D52CC and does not keep it, so the unit makes it internal. The function still needs
  its existing inline_blockers entry (retail calls it by `jal`). The `if (0)` block in the group file is that
  blocker; it is not original source.

## 5. world_physics_tick: structurally right

This is the N64 descendant of arcade `init_cars()` (game/mdrive.c). Full semantics, recovered layouts and
the list of source-form levers are in `world_physics_tick/NOTES.md`.

`blob_unit --tag w2h score world_physics_tick --with best.c` gives `FAIL world_physics_tick: 305 of 391 words
differ; compiled body is 394 words, target 391`. The aligned count, with relocation addends masked
(`udis.py`), is 60 rows. The C52 -O2 draft this started from was at 363/391.

Residual lanes:
- the delay-slot pick in both byte-clear loops;
- retail hoists &D_80151CE8 into a0;
- as1 placement of `li 6`;
- loads of the three config bytes before the byte15 store;
- one shared `lui at` in the tail.

The `next_cp` helper is a stand-in for the inlined `get_next_checkpoint`.

Structures recovered (offsets):
- **model** (2056 B): +0 object ptr, +8..15 s8 bytes, +244..288 BODYR (12 f32), +1020 s32, +1024 f32,
  +1620 colrad, +1732 s16, +1816 f32 (preserved), +1988..1994 s16 (slot @1990, in_game @1992,
  we_control @1994), +1996..1999, +2004 s32, +2013, +2018/+2020 s16 checkpoint (last/next), +2022, +2026/2027,
  +2028/2032 f32 = 1.0.
- **game_car** (952 B): +232 s32, +236, +238 place, +256/+260 f32 distance/last_distance (-100.0),
  +777, +785, +856, +859..862 s8, +896 pointer into D_8014A118 (76-byte records, flags byte at +66).
- **link record** D_80153E88 (8 B): +1 body, +2..4 three bytes, +5 place, +6 flags (0x80 = active), +7 owner
  (0 = this node, < 6 human, 6 = drone).
- **collsize** D_8011F844 (16 B): front, rear, side, height.

## 1. string_copy_format: 36 words off, unchanged

`score.py fn` gives `36/109 words differ (2 section-relative relocations unverified: .rodata+0x0 at +0x64,
.rodata+0x0 at +0x68)`. The unit gives `FAIL string_copy_format: 36 of 109 words differ`. The residual is a
single register: the name copy is coloured a3, and retail uses t0. The rest is the temp-ring shift that
follows from it. About 110 variants (first agent) moved nothing. Next step: an instrumented-uopt colouring
trace. See `string_copy_format/NOTES.md`.

## 3. audio_frame_update: 21 words off in the unit

`blob_unit … --with audio_frame_update/best.c` gives `FAIL audio_frame_update: 21 of 150 words differ` (size
equal). best.c is the first agent's `alt_handle_first.c`; its old `best.c` (28 words) is kept as
`prev_best_28w.c`. All 30 of the first agent's v*/ variants were re-scored in the unit: 21 to 142. In the
last loop, retail never uses v1: i is in a0, 24 in a1, car in a2, anim in a3, ctl in t0. Ours puts i in v1,
24 in a0, car in a1, ctl in a2, anim in a3. So in retail, one more web interferes with i. Details are in
NOTES.md.

## 6. audio_channel_priority: 11 words off

`score.py fn` and the unit both give `11/117 words differ`. Retail colours outer→v0, weights→v1, id→t0. Ours
colours id→v0, outer→v1, weights→t0. About 280 variants (272 generated by the first agent, 7 discarded-read
penalties on id by the takeover agent) all score 11 or worse. The `if (id) {}` filler changes nothing at
-O3.

## 7. camera_collision_avoid: 31 words off in the unit, new this session

Semantics: build a camera basis from cos/sin of an angle and two cross products, and transform two points
with func_800A61B0(in, out, mat). The prologue, frame (120), the code after the first `jal` and the epilogue
are identical. The 31 words are the FP temp registers of the first cross product: retail starts the ring at
f4, ours at f6.

Levers that worked:
- write the second product of each cross term reversed;
- a 16-byte local before `p`. Its form is unknown: `pad[4]` is a stand-in, not an original name.

The standalone `score.py fn` frame differs (s1 plus an a0 home), so trust the unit for this function.

## What generalises

- **Re-score group "matches" in the unit before reporting them.** Both group matches here pass `score.py
  group` but fail in the unit until a locked neighbour is changed: a callee's parameter order in one case,
  a single's kept/internal status in the other. `blob_unit score NAME --with cand.c` (plus `--internal`, or a
  second `--with` holding a corrected locked file) is a 4-second test that names the exact integration step.
- **A group whose only reason to exist is an internal callee** can be reduced to the claimed member, the
  callee, and one kept stand-in caller. This keeps provisional neighbours out of the claim.
- **IDO -O3 float products:** in `x*y - z*w`, the second product's operands are evaluated in reverse source
  order. Operand order inside a product does not matter (it is canonicalised); which side of the `-` it is
  on does.
- **`(u8) s->s8field` re-read after storing a u8 into it** reproduces retail's `lbu …; sb …; andi …,0xff`
  index pattern. 2-D `T[][13]` tables reproduce the uncached `idx*13` recomputation that 1-D `T[(idx+1)*13+b]`
  does not.
- **Two equal constants competing for a callee-saved register** (`112` twice, `6` twice) can be split by
  giving one occurrence a different type (`6U`), or by writing the dispatch as a `switch`. This freed s8 for
  the global address retail keeps there.
