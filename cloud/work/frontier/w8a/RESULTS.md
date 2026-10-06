# w8a results (frontier wave 8, 2026-10-05)

Builder scratch `~/rush2049/scratch/frontier/w8a` (copied from `base`; traced uopt/as1 binaries symlinked from
`w7a`). Unit tag `w8a`. Tools in `tools/` (w7a copies retargeted, plus `u.sh` = unit-score a list of files,
`area3.sh` = uopt3 area/spill/home trace on the unit snapshot, `rd.py` = read image words as floats).

| # | Function | Bytes | State | Flags | Deliverable |
|---|---|---:|---|---|---|
| 1 | `entity_render_mode` | 284 | **MATCH (group)** — member of slot_sound | -O3 | `groups/slot_sound/` (claims entity_render_mode) |
| 2 | `game_mode_handler` | 256 | **MATCH** (also -O2) | -O3 | `cloud/matches/game_mode_handler.c` |
| 2 | `func_800C885C` | 188 | **MATCH (group)** — member of codex_heap_release_a25 | -O3 | `groups/codex_heap_release_a25/` (claims func_800C885C) |
| 3 | `func_800BFD8C` | 852 | **MATCH (group)** — joins frontier_particle_knot; own .rodata verified | -O3 | `groups/frontier_particle_knot/` (claims func_800BFD8C) |
| 4 | `func_800D8078` | 220 | **provisional** (stand-in callers; real caller func_800D91A0 unmatched) | -O3 | `func_800D8078/best.c`, `func_800D8078/group/` |
| 4 | `func_800F7448` | 284 | **MATCH** | -O3 | `cloud/matches/func_800F7448.c` |
| 4 | `main_menu_render` | 236 | **MATCH** (also -O2) | -O3 | `cloud/matches/main_menu_render.c` |
| 4 | `func_8008B000` | 216 | **MATCH** | -O3 | `cloud/matches/func_8008B000.c` |

Strict: 7 of 8 (4 singles, 3 group members; 2,316 bytes). Provisional: 1 (220 bytes).

Unit cross-check (all deliverables together, no locked body broken):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w8a score game_mode_handler func_800F7448 main_menu_render func_8008B000 --with cloud/matches/{game_mode_handler,func_800F7448,main_menu_render,func_8008B000}.c --neighbours
  EQUAL game_mode_handler: 64 words (kept, c_game_mode_handler.c)
  EQUAL func_800F7448: 71 words (kept, c_func_800F7448.c)
  EQUAL main_menu_render: 59 words (kept, c_main_menu_render.c)
  EQUAL func_8008B000: 54 words (kept, c_func_8008B000.c)
  locked bodies that differ in this unit: 0
python3 -m tools.conveyor.pipeline.blob_unit --tag w8a score entity_render_mode func_800C885C audio_effect_process --with cloud/work/frontier/w8a/groups/slot_sound/entity_render_mode.c --with cloud/work/frontier/w8a/groups/codex_heap_release_a25/group.c --neighbours
  EQUAL entity_render_mode: 71 words (kept, c_entity_render_mode.c)
  EQUAL func_800C885C: 47 words (kept, c_group.c)
  EQUAL audio_effect_process: 23 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
python3 -m tools.conveyor.pipeline.blob_unit --tag w8a score func_800BFD8C func_8009C3F8 --with cloud/work/frontier/w8a/groups/frontier_particle_knot/group.c --internal func_8009C3F8 --internal render_display_list --internal func_80099B30 --internal particle_system --internal track_collision_wall --internal func_8009F058
  EQUAL func_800BFD8C: 213 words (kept, c_group.c)
  EQUAL func_8009C3F8: 113 words (internal, c_group.c)
```

---

## entity_render_mode — MATCH in group slot_sound

```
score.py group cand/slot_sound   (= cloud/work/frontier/w8a/groups/slot_sound)
Members: func_80096288 MATCH, slot_value_get MATCH, display_list_alloc MATCH, player_state_get MATCH,
slot_deactivate MATCH, sound_update_channel MATCH, mode_byte2_set MATCH, object_type_byte2_get MATCH,
object_type_byte3_get MATCH, mode_byte_set MATCH, func_800A4E58 MATCH, entity_render_mode: MATCH
```
Semantics: `func_80096288(index, 1, 1)` (slot validation hook), then if resource slot `D_80156D38[index]` is
loaded (s8 +3) and its mode byte (+4) differs from `(D_80140A04 != 0)`, walk the model list
`D_801161F4[index] = { Model *models; s32 count; }` (88-byte Model, four 16-byte Parts at +0x18 with a display list
at +8) and `display_list_traverse(dl, 0, 0, 2, func_8008AD6C)` each non-null part; then store the new mode byte
(D_80140A04 re-read after the calls). N64-only.

Why a group: `func_80096288` is internal in slot_sound; IPA keeps `index` in a3 across the call. Added as a kept
member in its own file (`entity_render_mode.c`), exactly as func_800A4E58 was.

Residual history: the first natural draft (`ResSlot *slot = &D_80156D38[index]`) was 2 words off — the spill of
the slot address at 64(sp) instead of 68(sp). Traced with `area3.sh` (uopt3): 5 named locals = area 20; one more
named local moved the home to 68 but grew the frame to 96. Indexing `D_80156D38[index].field` directly (no `slot`
local) gives named area 16 and the exact frame (EQUAL).

## game_mode_handler — MATCH

```
tools/sc.sh cloud/matches/game_mode_handler.c game_mode_handler
game_mode_handler:
  MATCH
```
(also MATCH with `-O2`.) Sets volatile D_80035472, jams a type-2750 message record (32-byte stack record, s16 type)
at the front of D_8002ECF8, clears D_80035471, drains D_8002ECC0 (non-blocking, blocking, then non-blocking until
-1), D_80035471 = 1, D_80035470 = 0, D_801497F4 = D_801497C8, viUpdateTime, process_inputs,
input_init_flag_get.

Shaping: record declared before `msg` (slots 48/44). Residual of the natural draft was one instruction:
`li s2,-1` after the 2nd receive instead of after the 1st. Writing the first receive as
`if (osRecvMesg(...) == -1) {}` (compiled-out check) starts the -1 web there (also `!= -1`, or comparing both of
the first two calls, matches). Loop is `while (osRecvMesg(q, &msg, 0) != -1) {}`.

## func_800C885C — MATCH in group codex_heap_release_a25

```
score.py group cand/codex_heap_release_a25   (= cloud/work/frontier/w8a/groups/codex_heap_release_a25)
all 15 members MATCH (InitMaxPath / sync_maxpath_to_checkpoint own .rodata verified), func_800C885C: MATCH;
context all MATCH
```
Frees the two cached audio heap handles: `if (D_8011025C) { audio_effect_process(D_8011025C); D_8011025C = 0; }`
and the same for D_80110260 — exactly the first two blocks of w6c's func_800C8918. audio_reverb_update is internal
and clobbers s0/s1, which is why the caller saves them unused. The frame (64, homes 56 and 32) is w6c's rule:
`audio_effect_process` defined `__inline` with four unused `s32` locals, so umerge inlines it and each instance
reserves 24 bytes. First compile matched.

Landing: group.c's `audio_effect_process` becomes `__inline` + four unused locals (its own kept body is still
MATCH); func_800C885C appended to group.c (it must be in the same file to inline the kept wrapper). The same change
is what w6c's func_800C8918 needs.

## func_800BFD8C — MATCH in group frontier_particle_knot (own .rodata verified)

```
score.py group cand/frontier_particle_knot   (= cloud/work/frontier/w8a/groups/frontier_particle_knot)
render_display_list, func_80099B30, func_8009C3F8, camera_update_c, select_screen_update, particle_system,
track_collision_wall, Input_ProcessGameplayPad: MATCH (as before)
func_800BFD8C:
  MATCH
    own .rodata verified at 0x80123E78..0x80123E84
Context: func_8009F058 22/1307 (unchanged, the known 68/72 spill slot)
```
**Arcade ancestor: `LIB/fmath.c InterpQuats(frac, q1, q2, qr)`**, with the `#if 0` spherical branch live (N64
variant): `frac < .001f` copy q1, `frac > .999f` copy q2 (CopyQuat comma macro); `dp = DotQuat(q1,q2) * .999f`;
`qr = -q2, dp = -dp` if `dp < 0` else `qr = q2`; `dp < .98f` → slerp with `acosf` (= select_screen_update,
inlined into a jal func_8009C3F8 with the dot product left in **$f16** — so it does join the knot) and three
`sinf`; else the arcade linear blend with ±2 wraps. Kept (standard ABI f12/a1/a2/a3; only caller
camera_free_look, unmatched). Literals checked against the image: 0x3A83126F .001f, 0x3F7FBE77 .999f,
0x3F7AE148 .98f.

Shaping (2 fixes from the first draft, 76 rows → 0): `(1 - frac)` with int `1` (a separate 1.0f constant web
from `1.0f / sint`; retail materialises 1.0 twice), and the arcade sum order `q1[i] * X1 + qr[i] * X2`.

## func_800D8078 — provisional (stand-in callers)

```
score.py group cand/g_D8078   (= cloud/work/frontier/w8a/func_800D8078/group, keep zz_caller,zz_caller2)
Members:
func_800D8078:
  MATCH
kept in the unit (no stand-ins): FAIL func_800D8078: 25 of 55 words differ
```
Predicate (`s8 car`): `idx = D_8014A0F8[car]`, `row = D_8013C068[D_8014A118[car].row]` (10-byte s8 rows; 76-byte
car record, row number u8 at +1); returns `row[idx] != 23 && D_80114060[row[idx]] == 1 && (idx != 2 || row[2] == 25
|| row[2] == 19) && (idx < 3 || (row[idx] != 19 && row[idx] != 25))`.

Internal: the only caller func_800D91A0 (3,860 bytes, L8, unmatched) keeps t0-t5 live across both calls, and the
body uses only the t6-t9 ring. Not spliceable until func_800D91A0 is matched. Shaping: `row[idx]` written at every
use (a `c` local gives either a copy `move a2,a3` or uopt's rewrite of `c == 19` into a pointer compare against
`&D_80114060[19]`); `row` assigned before `idx` (other order: 48 rows).

## func_800F7448 — MATCH

```
tools/sc.sh cloud/matches/func_800F7448.c func_800F7448
func_800F7448:
  MATCH
```
Clears the frame buffer: on D_801497C8, PipeSync; SetColorImage(RGBA16, D_8002AFC0,
osVirtualToPhysical(D_80156C5C[D_8015F72D].buf)) (128-byte records, volatile s8 index); SetFillColor;
FillRectangle(0, 0, w-1, D_8002AFC4-1); PipeSync. SDK GBI macros. Only fix: the fill word `((u32)color << 16) |
color` — without the cast uopt gives `color` a register (a2) and the `_g` ring shifts t0 → t1 (35 words).

## main_menu_render — MATCH

```
tools/sc.sh cloud/matches/main_menu_render.c main_menu_render
main_menu_render:
  MATCH
```
(also -O2.) For each of D_8014A108 cars with a node in its 76-byte record (+0x44, -1 = none):
`main_menu_input(node, D_8014A250[rec.model].pos (+0x22C), D_801141B0, D_80150B70[i].b (+0x18), .a (+0x0C))`.
Only fix (w7c lever): `node` assigned inside the `if` after testing the field (retail loads into v1 and moves to
a0; a node local assigned before the test gets a0).

## func_8008B000 — MATCH (first compile)

```
tools/sc.sh cloud/matches/func_8008B000.c func_8008B000
func_8008B000:
  MATCH
```
`(u16 id, s16 part, u16 value)`: model = `&D_801161F4[id >> 10].models[id & 0x3FF]`; part ≥ 0: range-check
against the s16 count at +0x16 (return 0), set `part[part].value`, `flags |= 0x8000`; part < 0: set every part's
value and — an original bug reproduced by retail — OR the dirty bit into `part[part]` (negative index) each
iteration. Returns 1.

---

## Types recovered

- `ModelList D_801161F4[]` = `{ Model *models; s32 count; }`, indexed by list number (`id >> 10`).
- `Model` (88 bytes): s16 part count +0x16, `Part part[4]` at +0x18. `Part` (16): u16 value +0, u16 flags +2
  (0x8000 = dirty), display list `u32 *dl` +8.
- `ResSlot D_80156D38[]` (0x14, slot_sound): s8 loaded +3, u8 mode +4 (cached `D_80140A04 != 0`), data +0xC.
- `D_8014A118[]` 76-byte car records: u8 model +0, u8 row +1, render node +0x44 ((void *)-1 = none).
  `D_8014A108` s16 car count, `D_8014A0F8[]` s16 per car. `D_8013C068[][10]` s8 rows; `D_80114060[]` s8.
- `D_80150B70[]` 152-byte view records: two Vec3 at +0x0C and +0x18. `D_80156C5C[]` 128-byte frame-buffer records
  (pointer at +0). `D_8002AFC0` / `D_8002AFC4` screen width / height.

## Same-shape siblings found (not assigned to me)

- `D_801161F4` model-list users still unmatched: **func_80096130** (264) and **func_80096CA8** (1,204); the
  Model/Part layout above should fit them.
- Other unmatched callers of `func_80096288` (slot_sound group candidates; they keep their index in a3 across it):
  func_800979A0, suspension_setup, car_shadow_render, func_800BB9B0.
- Other callers of `func_8009C3F8` (asin/acos kernel, $f16): camera_follow_path (and camera_free_look calls
  func_800BFD8C) — both must join frontier_particle_knot.
- func_800C8918 (w6c near-miss) begins with func_800C885C's two blocks verbatim.

## What generalises

1. **Arcade LIB math survives in N64 as kept leaf functions** — InterpQuats was found by its literals' shape
   (`<.01 / >.99` copy, `* .999` dot) even though the N64 edition re-enables the `#if 0` slerp branch and changes
   the thresholds to .001/.999/.98. Grep `LIB/fmath.c` for quaternion/vector helpers before writing FP code.
2. **Int literal `1` in `(1 - frac)`** is again a separate constant web from `1.0f` (w6c item 6): retail
   materialising 1.0 twice (`mtc1 at,$f18` and `mtc1 at,$f10`) is the sign.
3. **A named pointer local can cost exactly one spill slot offset.** When a spill home is 4 bytes low and the frame
   is right, try deleting a pointer local and indexing the global array directly (entity_render_mode); adding a
   named local instead grows the frame. `area3.sh` shows the named area vs home layout in one run.
4. **A constant's first materialisation follows its first comparison.** `li sN,-1` earlier than natural C puts it
   is an earlier compiled-out `if (call() == -1) {}` (game_mode_handler).
5. **`(u32)x << 16 | x` vs `x << 16 | x` for a u16 parameter** decides whether uopt gives the parameter a register
   web; when retail reloads a narrow parameter from its home into a ring temp, try the cast.
6. **Writing an array element at every use instead of a local** avoids both copies and uopt's
   strength-reduction of a compare into an address compare (func_800D8078).
7. Kept `__inline` wrapper with N unused locals (w6c) reproduced a second caller on the first compile — the rule is
   reliable; whoever lands func_800C885C should land it as the group form here.
