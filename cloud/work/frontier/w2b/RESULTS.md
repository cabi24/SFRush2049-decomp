# Frontier wave 2, agent w2b — results

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w2b`. All scoring with `tools/cloud/score.py` in that copy
(IDO toolkit `796ae99a…`); unit checks with `python3 -m tools.conveyor.pipeline.blob_unit --tag w2b score …` from the Pi.
Nothing was committed, spliced or edited outside `cloud/matches/` and this directory.

| # | Function | Bytes | State | Flags | Deliverable |
|---|---|---:|---|---|---|
| 1 | `random_float` | 1,424 | code identical, own rodata verified by `owndata.verify` (scorer: 14 unverified) | `-O3` only | `random_float/best.c` |
| 2 | `vector_normalize_length` | 380 | code identical, own rodata verified (scorer: 2 unverified) | `-O3` and `-O2` | `vector_normalize_length/best.c` |
| 3 | `menu_item_select` | 564 | **strict MATCH** in a real group (locked context unchanged) | `-O3` group | `groups/frontier_heap_resize/` |
| 4 | `audio_priority_find` | 432 | **strict MATCH** as internal member of a real group whose other body (its only caller, `audio_mixer_main`) is a real reconstruction that is 31/183 words off. Integrator's call: claim, or keep provisional | `-O3` group | `groups/frontier_route_cross/` |
| 5 | `track_select_handler` | 884 | code identical, own rodata verified (scorer: 2 unverified) | `-O3` only | `track_select_handler/best.c` |
| 6 | `func_80105B74` | 564 | **strict MATCH** | `-O3` and `-O2` | `cloud/matches/func_80105B74.c` |
| 7 | `camera_zoom_fov` | 444 | **strict MATCH** | `-O3` only | `cloud/matches/camera_zoom_fov.c` |
| + | `audio_mixer_main` (not assigned; written as the real caller for #4) | 732 | 31/183 words, one cause | `-O3` | `audio_mixer_main/best.c` (= the group file) |

Totals: 3 strict (1,572 bytes), 3 code-identical with literals verified against the image (2,688 bytes), 1 strict with a
real but unmatched caller (432 bytes).

Every one of the seven is `EQUAL` in the whole-program unit (`blob_unit score`, outputs below).

---

## 1. random_float — code identical

Real semantics: N64 descendant of arcade `setFBCollisionForce()` (`reference/repos/rushtherock/game/collision.c:337`);
`random_int` (already matched) is its `ForceApart`. Details and N64 differences are in the header of `best.c`.

```
$ python3 tools/cloud/score.py fn cand/random_float.c random_float --flags "-g0 -O3 -mips2 -G 0 -non_shared"
random_float:
  MATCH (14 section-relative relocations unverified: .rodata+0x0 at +0x1ac, .rodata+0x0 at +0x1b0, .rodata+0x4 at +0x20c, .rodata+0x4 at +0x214, .rodata+0x8 at +0x290, .rodata+0x8 at +0x298, .rodata+0xc at +0x2fc, .rodata+0xc at +0x300, .rodata+0x10 at +0x344, .rodata+0x10 at +0x34c, .rodata+0x14 at +0x3bc, .rodata+0x14 at +0x3c4, .rodata+0x18 at +0x50c, .rodata+0x18 at +0x510)
(-O2: 345/356 words differ)
$ python3 ownverify.py random_float.o random_float          # owndata.verify, the splice's own check
random_float ok=True references=7 sites=14
  note: own .rodata verified at 0x80124808..0x80124824
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w2b score random_float --with cloud/work/frontier/w2b/random_float/best.c
  EQUAL random_float: 356 words (kept, c_best.c)
```

Path: first arcade-shaped draft 270/356 -> 66 (zero literal and multiply spelling) -> 34 (named abs temp) -> 2 -> 0.
What closed it, in order:
- `force[i] = 0.0` (double literal) but `> 0.0f` / `< 0.0f` comparisons: retail has two zero webs (a temp for the store,
  `$f14` for the compares). `0.0f` everywhere costs ~180 words.
- `temp[i] *= K * fabsf(temp[i]);` The compound form keeps K as the left operand and mints the compiler temp that puts
  the UV-pointer spill at `28(sp)`. `temp = temp * (K * fabsf(temp))` flips both multiplies; a named `f = fabsf(...)`
  fixes the multiplies but leaves the spill at `32(sp)`.
- separate `scale = m2->body->mass / mass` variable (retail keeps the mean mass in its home and the ratio in `$f0`);
- `flag = (m->unk7CC == 2)` evaluated before the mode test.
- Frame: an unused three-float local between `temp` and `rvel` and exactly five scalar locals.
Frame facts recovered while doing it (generalises): a `&&`-valued expression's first copy lands in the declared variable's
home (`behind`, 48(sp)), the second stays in cfe's expression temp, which is the first slot below the declared locals;
uopt's own spill temps follow it. So "which slot does a spilled bool use" counts the declared scalars.
The frontier's `group` label for this function is a false positive (`lw a2,28(sp)` reload, nothing is preserved by callees).

Structs recovered: model `+0x124 CENTERFORCE[3]`, `+0x5C4 mass`, `+0x63C f32`, `+0x640 s8 crash flag`, `+0x788 RWV[3]`,
`+0x794 RWR[3]`, `+0x7A0 UV[3][3]`, `+0x7C6 s16 net_node`, `+0x7CC s8`; other-car record `{Body *body; f32 UV[3][3] @4;
f32 RWR[3] @0x28; f32 RWV[3] @0x34}`; `Body {f32 xmin, xmax, zmax, zmin; … f32 mass @0x1C}`; `D_8017A634 s8`,
`D_8014A110 s32`, car `+0x359 s8`.

## 2. vector_normalize_length — code identical

Real semantics: builds an orthonormal basis from a direction (`m[2] = normalize(dir)`, `m[0] = (dir.z, 0, -dir.x)`
normalised or `D_801141C8` when shorter than 0.01, `m[1] = m[2] x m[0]`, `m[0] = m[1] x m[2]`).

```
$ … score.py fn cand/vector_normalize_length.c vector_normalize_length --flags "-g0 -O3 -mips2 -G 0 -non_shared"
vector_normalize_length:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x6c, .rodata+0x0 at +0x70)
(-O2: identical line)
$ python3 ownverify.py vector_normalize_length.o vector_normalize_length
vector_normalize_length ok=True references=1 sites=2
  note: own .rodata verified at 0x8012388C..0x80123890
$ … blob_unit --tag w2b score vector_normalize_length --with …/vector_normalize_length/best.c
  EQUAL vector_normalize_length: 95 words (kept, c_best.c)
```

Three compiles. The prior source (`cloud/work/ipa-groups/codex_effect_basis_a159/basis.c`) was m2c-shaped with a fake
`D_8012388C` extern. Natural `f32 m[3][3]` source with the two cross products written out and the standard operand order
(`a1*b2 - a2*b1`) matches; a three-pointer `crossprod()` helper is not inlined by `-O3` (size -23). No quirks.
`group` label is a false positive here too (plain ABI, matches alone). It has 27 unmatched dependents.

## 3. menu_item_select — strict MATCH (real group)

Real semantics: heap block resize in place (lock, round to 32, find heap and block, split / move successor header /
absorb successor). Header of the function in `groups/frontier_heap_resize/group.c`.

```
$ python3 tools/cloud/score.py group cand/g_frontier_heap_resize
Members:
menu_item_select:
  MATCH

Context (informational; excluded from exit status):
func_80095F8C:  MATCH        func_80095EF4:  MATCH        audio_buffer_sync:  MATCH
object_counter_decrement:  MATCH   object_counter_increment:  MATCH   audio_reverb_update:  MATCH
audio_effect_process:  MATCH   synced_model_render:  MATCH   MP_TargetSpeed:  MATCH
assign_default_paths:  MATCH   stat_race_end:  MATCH
$ … blob_unit --tag w2b score menu_item_select --with cloud/work/frontier/w2b/menu_item_select/unit_cand.c
  EQUAL menu_item_select: 141 words (kept, c_unit_cand.c)
```

The group file is `src/blob/groups/codex_heap_release_a25/group.c` **unchanged** plus the new function (same pattern as
`codex_counter_guard_a30`); `group.json` has `"claims": ["menu_item_select"]`. It is a genuine whole-program member:
`func_80095F8C` preserves a1/a3 and `func_80095EF4` preserves a3/t0 for it.
Prior state (`cloud/work/ipa-groups/codex_heap_resize_a41`): 48-56/141 with opaque `pad8[n]` fields. One rewrite closed it:
- typed header `{magic, next, prev, size, owner, s8 used, s8 tag, u8 count}`, heap `tail` at +0xC;
- `next` is its own variable (a1) but the null test and the first `moved->next =` use the field expression
  `block->next` (v1): `next = block->next; if (block->next == NULL || next->used != 0)`, `moved->next = block->next;`
- cached `size = block->size` for the tests, re-read `block->size` in the size arithmetic;
- grow path: `nsize = next->size` used for the test only, operand orders `block->size + next->size + 32` and
  `block->size + next->size - desired`.
`unit_cand.c` is the same function with prototypes only, for the unit check.

## 4. audio_priority_find — strict MATCH with its real (unmatched) caller

Real semantics: route-crossing test (see the group header). Arguments are `(who, route)`.

```
$ python3 tools/cloud/score.py group cand/g_frontier_route_cross
Members:
audio_priority_find:
  MATCH

Context (informational; excluded from exit status):
audio_mixer_main:
    … 31/183 words differ
$ … blob_unit --tag w2b score audio_priority_find audio_mixer_main --with …/groups/frontier_route_cross/group.c --internal audio_priority_find --keep audio_mixer_main
blob_unit score: 1/2 equal        (audio_priority_find EQUAL, audio_mixer_main 31 words)
$ python3 tools/cloud/score.py group cand/g_standin_group          # audio_priority_find/standin_group, stand-in caller
Members:
audio_priority_find:
  MATCH
```

State, stated plainly: the body is strict MATCH both with a stand-in caller and with a real reconstruction of its only
caller. The real caller does not match yet, so under the brief's wording this is at least provisional; it is *not* a
stand-in proof any more. `group.json` lists it under `claims` with a note; drop the claim if the policy is "every member
of the group must match".

Findings:
- **Internal functions do not get ABI-position argument registers.** The entry stores are `sw a0,28(sp)` /
  `sw a1,24(sp)`: homes follow *source* parameter order, registers are chosen by uopt. Source `(s16 who, s16 route)`
  with `route` arriving in a0 reproduces it; `(route, who)` gives un-swapped homes (2 words off) whatever the caller
  does. A scan of the image finds three functions with permuted homes: this one, `voice_stop_2` and the locked
  `func_800E7B44` (both of those also have a t-register parameter). Useful as a detector: permuted or offset home stores
  in the prologue give the real parameter list of an internal function.
- Kept (standalone) compile: 92/108. Internal with two call sites: 10/108 on the first try.
- Tracker fields must be read through the global array (`D_80151CE8[who].x`) so uopt hoists them into the preheader;
  route fields through a pointer (re-read each iteration). A `Tracker *` local keeps the loads inside the loop.
- `d[0] * dir[0] + d[1] * dir[1]` (cfe canonicalises the operand order; of 8 spellings only this one is right).
- `(Route *)((u8 *)routes + route * sizeof(Route))` loads the base before the shift; `&routes[route]`,
  `routes + route` and a `u8 *` field give the reverse temp order (2 words).

Layout recovered: `D_801407F0 {…; u8 count @8; Route *routes @0xC}`, `Route {u8 type; …; u16 num @0xA; RoutePoint *pts
@0xC}` (16 bytes), `RoutePoint {s16 x, y, z}`; trackers are 0x50-byte records starting 12 bytes into `D_80151CE8`
(`s16 first @2, last @4, count @8`; record: `f32 pos[3] @0xC, dir[3] @0x18, s32 range @0x24, s16 idx[20] @0x2E,
f32 len @0x58` in the prefixed view). `w2c` describes the same records in `func_800BA2B8`.

### audio_mixer_main (bonus, not assigned) — 31/183

Real reconstruction, structurally exact (183 words, frame 80, every call site right). Single residual: in the second
loop retail computes `k + 1` three times (`addiu t9,s0,1` for the test, `addiu v0,s0,1` for `next`, `addiu s0,s0,1`
for the increment); my source lets uopt keep one copy in `s2` and reuse it as the increment (`move s1,s2`), which
shifts `done`, the two global pointers and the `80` constant up one s-register. Tried (about 35 variants, no movement):
if/else, ternary, `next = k + 1` first, inline ternary in the subscript, `while`/`do`/`+= 1` loops, reusing `i`/`j`,
a separate `k`, `s16`/`u16`/`s8`/`u8` counters and `s16 next` (all add narrowing code), compiling it internal with a
stand-in grand-caller (then it stops saving s-registers, so retail's copy is kept). Best next hypothesis: the index in
the test/`next` and the loop counter are different C objects with the same value (for example a counter plus an
explicit record pointer whose index is derived), so that uopt has no common subexpression. Frame needs 8 bytes of
unused locals above `s16 pos[3]` (written as `s32 pad[2]`); `pos` is written from the tracker position and never read.

## 5. track_select_handler — code identical

Real semantics: N64 port of arcade `update_resurrecting_car()` (`game/resurrect.c:1303`). `func_800CFDEC` is arcade
`interpolate`, `menu_vibration_test` is `make_uvs_from_quat`, `math_utility` a nine-float copy.

```
$ … score.py fn cand/track_select_handler.c track_select_handler --flags "-g0 -O3 -mips2 -G 0 -non_shared"
track_select_handler:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x2bc, .rodata+0x0 at +0x2c0)
(-O2: 60/221 words differ)
$ python3 ownverify.py track_select_handler.o track_select_handler
track_select_handler ok=True references=1 sites=2
  note: own .rodata verified at 0x80124128..0x8012412C
$ … blob_unit --tag w2b score track_select_handler --with …/track_select_handler/best.c
  EQUAL track_select_handler: 221 words (kept, c_best.c)
```

Three compiles: arcade body with the arcade declaration list gave 133/221 with the right frame at once; writing the
final `RWR` copy as three statements and sweeping the zero literal of the three `interpolate()` calls closed it:
the first call passes `0.0f` (`li a3,0`), the two in the first-half branch pass `0.0` (`mtc1 zero,$f14; mfc1 a3,$f14`,
sharing the zero with `RWV[i] = 0`). Prior attempt `cloud/work/near_miss_B98.md` was 163-198/221 without the arcade
source. Needs an unused pointer local (arcade `gc`) for the frame.

Model fields recovered (same record as #1): `+0x220 RWV[3]`, `+0x22C RWR[3]`, `+0x2EC uvs[3][3]`, `+0x660 resurrect
pos[3]`, `+0x69C quat_start[4]`, `+0x6AC quat_end[4]`, `+0x6C4 s16 moving_state`, `+0x6C8 f32 resurrect_time`,
`+0x6D4 initin pos[3]`, `+0x714 f32 time`, `+0x7CA s16 we_control`, `+0x7D4 s32 appearance`, `+0x7DF s8 hide_car`;
`D_80154390 f32` = RESURRECT_TIME, `D_80161450 f32[][3]` = save_pos, `D_80153E88` 8-byte records with a state byte
at +7, `D_801427A1 s8`.

## 6. func_80105B74 — strict MATCH

Real semantics: builds the end-of-race results tables (formats four times, orders slots by place). See file header.

```
$ … score.py fn cand/func_80105B74.c func_80105B74 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_80105B74:
  MATCH
(-O2: MATCH)
$ … blob_unit --tag w2b score func_80105B74 --with cloud/matches/func_80105B74.c
  EQUAL func_80105B74: 141 words (kept, c_func_80105B74.c)
```

Four compiles (139 -> 96 -> 2 -> 0). Quirks: `D_80153FD2` declared `volatile s16` (retail re-reads it through a hoisted
address after every byte store; original declaration unknown), `D_80142DB4[num - j++ - 1] = idx` for the
store-after-increment order, an `s32` index for the first loop and `s16` counters for the rest, car count copied to a
local. Layout: `D_8014A118` is an array of 0x4C-byte records with the slot byte at +0 (existing sources index
`D_8014A114` + 4), `D_801439D8 u8[4][8]`, `D_80142DB4 s8[6]`, model `+0x7C6 s16 slot`, `+0x7CC s8`, car `+0xEE s8
place`, `+0xEF s8 finished`, `+0xF0 f32 time`.

## 7. camera_zoom_fov — strict MATCH

Real semantics: counts the lines a string occupies when word-wrapped to a pixel width (8-bit or 0xFF-prefixed 16-bit
text). `camera_shake_update` is the glyph-width lookup, `camera_auto_follow` (next function, unmatched) is very likely
the drawing sibling with the same loop.

```
$ … score.py fn cand/camera_zoom_fov.c camera_zoom_fov --flags "-g0 -O3 -mips2 -G 0 -non_shared"
camera_zoom_fov:
  MATCH
(-O2: 90/111 words differ (2 extra words (nonzero beyond target length)))
$ … blob_unit --tag w2b score camera_zoom_fov --with cloud/matches/camera_zoom_fov.c
  EQUAL camera_zoom_fov: 111 words (kept, c_camera_zoom_fov.c)
```

About 1,700 scripted variants (mostly three placement sweeps), each step with a named cause (89 -> 52 -> 14 -> 2 -> 12 -> 0):
- **inlined getter**: `lb v0; sll s8,v0,16; sra; move` is the locked 3-word `func_800BDEB0` (no jal callers) inlined;
  it must be defined in the same file (a single file with a helper is fine for the single splice path).
- `n++` is the *last* statement of the inner loop, after the break test (retail finishes the truncation of `n` only on
  the loop-back path); three separate `break` tests keep 13 and 10 out of s-registers; `(p[0] << 8) | p[1]`.
- initial statements all after the `if (wide)` block in the order gap, w, n, lines, space (sets n/step/32 colouring).
- **as1 follows source line order across a branch**: the last 2 words were `addu s4,s6,s1` against `li at,32767`.
  A `#line` experiment proved the exit test's line number must be lower than the assignment's, i.e. `start = step + p`
  is written *after* `if (ch == 0 || lines == 0x7FFF) break;`. as1 then hoists the `addu` above both branches (its
  destination is dead on the taken path). Token-identical line joins had no effect; statement order did.
- QUIRK: an empty `if (start) {}` after the inner loop. Code-free; it lengthens `start`'s live range across the exit
  tests so uopt colours `space` (s3) before `start` (s4). Probably the remains of the line-start use in the drawing
  sibling; not proven original. Without it: 12 words (s3/s4 swapped). Declaration order has no effect on colouring.

---

## What generalises

1. **Arcade ancestors by constants, not names**: `random_float` = `setFBCollisionForce`, `track_select_handler` =
   `update_resurrecting_car`. Both reached the right frame on the first compile from the arcade declaration list.
2. **Internal functions: registers by uopt, homes by source order.** Permuted argument-home stores in a prologue are the
   source parameter list (#4). Worth adding to the frontier's signatures (three hits in the image).
3. **A real but unmatched caller is enough context for an internal callee.** `audio_mixer_main` at 31 words off gives
   the same `audio_priority_find` bytes as the stand-in. A first-draft reconstruction of the caller took one compile to
   be structurally right; it is cheaper than it looks and removes the "stand-in" objection.
4. **as1 hoists an instruction above conditional branches** when its destination is dead on the taken path (`li at`,
   and an `addu` into an s-register before the epilogue), and orders the hoisted instructions by source line. If a
   statement appears *before* a test in retail, it may be written after it. `#line` is a quick oracle for this.
5. **Literal spelling again**: `0.0` vs `0.0f` vs `0` selected the zero web in two functions; `x *= K * f(x)` vs
   `x = x * (K * f(x))` selects operand order *and* a compiler temp slot.
6. **Own rodata can be verified now without the scorer patch**: `objget.sh` + `ownverify.py` in this directory fetch the
   object and run `tools/cloud/owndata.verify` against `build/game_code.bin`, the same check the splice runs.
7. Frontier label corrections: `random_float` and `vector_normalize_length` are `group` false positives (reloads from the
   function's own frame); `audio_mixer_main` saves all its s-registers and has ABI entry (kept-shaped) although labelled ring.

## Files

- `sc.sh`, `full.sh`, `batch.sh`, `bq.sh`, `grp.sh`, `dump.sh`: w1f helpers retargeted to this scratch copy;
  `full.py`/`bscore.py` default to `-O3` here (the w1f copies default to `-O2`, which cost me the first half hour).
- `o3s.sh` / `o3s_remote.sh`: pre-as1 listing without `rm` in the remote script.
- `objget.sh`, `ownverify.py`: own-rodata verification (item 6).
- per function: `best.c`, generators (`gen*.py`) and the surviving variant directories. The large sweep directories
  were deleted; the generators recreate them.
