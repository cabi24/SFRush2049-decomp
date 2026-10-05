# Frontier wave 6 — agent w6d results

Builder scratch: `~/rush2049/scratch/frontier/w6d` (copied from `base` on 2026-10-05; `base` lock md5 equal to the
local `blob_matched.lock.json`). Unit tag: `w6d`. Nothing was committed or spliced; nothing under `src/`, `asm/`,
`tools/`, `include/`, `tests/` or another agent's directory was edited. func_80087110, stat_race_update and
func_800FE5B0 were not touched.

Tools: `tools/` holds the w5c tool set retagged for w6d, plus:
- `fd.sh SRC NAME [full.py args]`: aligned diff on the builder (w5b `full.py`, `-O3`; `--mnem`, `--all`, `--keep`).
- `gfd.sh GROUPDIR NAME`: the same for a single-file group directory (it reads `keep` from `group.json`).
- `gsw.sh GROUPDIR NAME VARIANT.c…`: scores variant `group.c` files against one `group.json`.
- `var.py BASE OUT 'old=>new'…`: literal-replacement variant generator. It fails if a pattern does not match.
- The scratch `uopt` is a symlink to `../w3a/uopt` (the traced build). `ctrace.sh`, `force.sh`, `gtrace.sh` and
  `pdiff.sh` work unchanged. The traced as1 (`w5a/as1`) is gone from the builder, so `asr_remote.sh` does not work.

## Summary

| Function | Bytes | State | Flags | Residual / lane |
|---|---:|---|---|---|
| stat_lap_split | 608 | **strict MATCH** (own .rodata verified); also EQUAL in the unit | `-O3` only (`-O2`: 144 words) | — |
| camera_target_track | 636 | 18/159 words (12 aligned rows) in the real `frontier_list_alloc_sound` group | `-O3` | One constant web's colour. `force` reproduces retail exactly (0 rows). |
| drone_throttle_calc | 1,024 | **provisional, code identical** with stand-in callers. The scorer says NOT VERIFIED: the strings and float literals sit at two separate retail addresses. | `-O3` | Real caller func_800D91A0 (3,860 B, L8) is unmatched |
| func_800B59F0 | 1,372 | **provisional**: 86/343 words with stand-in callers. The loop body is identical; the residual is the post-loop speed clamp. | `-O3` | Real caller physics_sym is unmatched |
| entity_update_callback | 2,184 | 546/546 words, 3 rows aligned by mnemonic. The remaining positional differences are v0/v1 renames. | `-O3` | Colouring (v0/v1); the source relies on one unproven empty `if` |
| audio_doppler_full | 952 | 238 words. Standalone: 40 rows aligned by mnemonic. In the unit (with the real inlined sound wrapper): frame 288 against 256. | `-O3` | Frame size from three inlined wrappers; callee-saved address-web choice |
| func_8010D680 | 476 | 49 aligned rows (86/119 positional) | `-O3` | Parameter `on` must be live past the entry block without emitting code |
| camera_play_script | 3,520 | not attempted beyond classification (see below) | — | — |

## stat_lap_split — strict MATCH

`cloud/matches/stat_lap_split.c`.

```
cloud/work/frontier/w6d/tools/sc.sh cloud/matches/stat_lap_split.c stat_lap_split --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
stat_lap_split:
  MATCH
    own .rodata verified at 0x80124824..0x80124828
python3 -m tools.conveyor.pipeline.blob_unit --tag w6d score stat_lap_split --with cloud/matches/stat_lap_split.c
  EQUAL stat_lap_split: 152 words (kept, c_stat_lap_split.c)
blob_unit score: 1/1 equal
```
Started from `cloud/work/near_miss_B8/stat_lap_split_B8_pointerfirst.c`, which was 6/152 words off.

Semantics: (mode, player, pos, u8 arg3). The function returns -1 unless the feature byte D_8010FFC0 is set and
the player's slot state (8-byte records at D_80153E88, byte +7) is 6. Otherwise:
- delta = pos − car.pos, where the car is D_80152818[player] (stride 952, pos at +8);
- func_800A61B0 rotates delta into the car frame with the matrix at +0x2C;
- if x²+z² > 1, the direction bits are classified with signed squares against 0.924f·0.924f·sum: 4/8 for ±x, 1/2
  for ±z;
- the function then calls high_scores_display (historical label) with (mode, player, 1, arg3, 1.0f, x, y). x and y
  come from the s16 tables D_8011F020/D_8011F040 indexed by the direction bits, or are 0,0 when mode == 6.

Three changes closed it:
1. `x2 *= sign`, the compound form, fixes the `mul.s` operand order.
2. The literal is `0.924f * 0.924f`, folded in single precision to 0x3F5A9111. `0.853776f` gives …110.
3. **`car = D_80152818; car += player;`** replaces `&D_80152818[player]`. With the array form, ugen emits
   `.noalias $2,$sp` (seen in the ugen listing, `o3s.sh`) and as1 hoists the car loads above the delta stores. That
   was the B8 "load/store scheduling" residual. An integer cast (`(Car *)((s32)D_80152818 + player*952)`) also
   matches; it is the same lever w3b used in func_800F8EC8.

No same-shape sibling was found. The only other unmatched callers of func_800A61B0 have different shapes.
stat_lap_split was the sole blocker for func_8010D3C0, func_8010DBB8 and func_8010E72C, which are now unblocked.

## camera_target_track — 12 rows; one constant colour (group extension)

Group: `groups/frontier_list_alloc_sound/` holds the locked group plus camera_target_track. `claims` is empty
because this is not a match. The same source is also at `camera_target_track/best_group.c`.

```
cloud/work/frontier/w6d/tools/grp.sh cloud/work/frontier/w6d/groups/frontier_list_alloc_sound
  func_80092278 / entity_flags_apply / high_scores_display: MATCH
  camera_target_track: 18/159 words differ        (aligned: 12 rows, gfd.sh)
cloud/work/frontier/w6d/tools/force.sh ctb camera_target_track 117 "p1:w101=c8"
  want 159 words, got 159; differing rows 0
```
Semantics: `s32 camera_target_track(f32 *pos, f32 a1, f32 volume, f32 a3, f32 x, f32 y, u32 index, u32 other,
u32 value, u8 mode)`. It is a positioned variant of high_scores_display. The float parameters arrive in integer
registers and are homed.
1. If D_80110260 is 0, it returns -1. Otherwise it takes the D_80142728 lock.
2. It pops an emitter from free list D_801461B0 (func_800AED20). If none is free, it releases the lock and returns
   -1.
3. It stores pos, volume, x clamped to [0,1], y clamped to [0,1] and 1.0f in the emitter.
4. It allocates a command (func_80091B00) with kind 2 and a node (func_80092278) with p20 = 1, filling index, other,
   mode, state, live and code as in high_scores_display, with floats 0/0/−2. node->p64 points at the emitter and the
   emitter gets the node tag.
5. It posts the command, re-locks, inserts the emitter into active list D_801461E8 if `active` (s8 +8) is 0 and
   sets `queued` (+9).
6. It returns the tag.

The retail frame saves s0 and s1 because func_80092278 clobbers them.

What was found:
- Declare `object` before `cam`; that order sets the spill slots.
- Add one extra 4-byte local (`char buf[4]`) so the result spill lands at sp+32.
- `Cam.active` is s8.
- `Node68.p20` is **s32**. Retail shares one `1` web between p20 and state but not with the u8 `live`. Making p20
  signed does not change the locked members, which only store 0 to it.

Residual: the `1` constant web (w101) is coloured a0 (forbidden only v0, v1 and a3). Retail colours it t1, which
needs a0, a1, a2 and t0 all forbidden. Forcing w101 to t1 reproduces retail exactly.

Tried without movement (about 25 variants):
- the order of the p64, tag and float stores; state stored later or earlier;
- `TRUE`, s32 index/other/value, a `one` variable;
- empty `if`s after func_80092278, func_80091B00 and func_800AED20;
- `(void *)0` arguments; the nested assignment `if ((cam = …) == 0)`.

Best next hypothesis: in retail the const web interferes with the osJamMesg argument webs (the &D_80142728 address
web is a0), so retail's block order or live ranges around the post-store osJamMesg differ. Trace which webs are a0,
a1, a2 and t0 in that block (`gtrace.sh` / `ctrace.sh`, ordinal 117 in the unit).

## drone_throttle_calc — provisional, code identical (pause-menu setup)

Evidence: `drone_throttle_calc/best_group/` (group.c + group.json). The group keeps two stand-in callers,
`zz_caller` and `zz_caller2`; two callers stop umerge from inlining the function.
```
cloud/work/frontier/w6d/tools/gfd.sh drone_throttle_calc/best_group drone_throttle_calc
  want 256 words, got 256; differing rows 20; unverified 20 unresolved 0
```
All 20 rows are own-literal `lui`/`lwc1`/`addiu` relocations. `grp.sh` prints `NOT VERIFIED (… own .rodata: …
references disagree on the section's image address (0x80120254 for the strings, 0x801241E0 for the floats)`. In
retail the strings ("BUTTON_TITLE", "CURSOR", "B1") and the float literals are not contiguous, so the scorer
cannot verify them as one section. The literal values were checked by hand against the retail words:
−1.5707964f, 55.2f, 50.2f, −75.1f and −87.1f.

Semantics: the historical name is wrong. The function is the alias `pause_quit`, the pause-menu set-up for one
player. Its only caller is func_800D91A0, which stores the s8 player in its own outgoing slot 0(sp); the callee
reads it as `lb 115(sp)`. Steps:
1. It creates the missing models of the player's 17 menu items (D_80111998[player], 0x440 bytes; items are 0x40
   bytes: type, handle, mat[3][3] at +0xC, pos at +0x30):
   - copies the identity matrix with math_utility;
   - gets the name with string_copy_format(D_8011196C[type], …);
   - creates the model with func_8008E26C(name, mat, (s16)-1, type∈{0,1,2,9,10} ? 0x42000 : 0);
   - sets node colour = D_801140EC, then model_data_load(h,1,15) and model_transform_setup(handle,0,1<<player).
2. It sets the textures of items 13, 14 and 15 ("BUTTON_TITLE", "CURSOR", "B1").
3. It positions items 13, 15 and 16: identity, roll −π/2, a pos that depends on whether D_80151AD0 ≥ 2, then a
   scale of 1.0. Item 15 is also scaled by 0.5 (func_8008B32C).
4. It sets D_80154618[player] = D_80154630[player] = 1 and calls particle_velocity_set.

The IPA conventions (no callee saves, stack parameter) reproduce with stand-in callers. Levers that moved it:
- **`if (player < 0) {}`** at the top, an empty debug check. It adds a block to `color`'s live range, so `color`
  loses its s-register to the constant 10 (save 2.2 → below 2.0) and is spilled to sp+108 as in retail
  (217 → 153 rows).
- **`scale = 1.0f` held in a local.** Retail multiplies the stored positions by 1.0 (f20). A literal `*= 1.0f`
  folds in cfe, and a `static` helper with a float parameter is not inlined (IPA passes it in f12). A local
  variable is folded only after uopt has a web for it (the w4c late-fold rule).
- The `(s16)` conversion of a −1 held in a variable (`s16 none = -1`) gives retail's `sll a2,s4,16; sra`.
- The flags `?:` written inline in the call.
- `h = func_8008E26C(…)` as a local, so model_data_load gets `move a0,v0`.
- The tail uses the expressions `D_80111998[player].item[N]`, not an `e` pointer (one CSE'd base in s0, as w5b #3
  predicts).
- `if (D_80151AD0 >= 2) 105/50.2 else 115/55.2` gives the branch sense; a 4-byte `k` plus `char pad[12]` gives the
  112-byte frame.

Closes when func_800D91A0 matches. The empty `if` and the unused frame locals are unproven shaping: record them as
such.

## func_800B59F0 — provisional (pause-menu buttons), 86/343 words

Evidence: `func_800B59F0/best_group/`, with stand-in callers. The real caller is physics_sym; another historical
label.
```
cloud/work/frontier/w6d/tools/grp.sh cloud/work/frontier/w6d/func_800B59F0/best_group
  func_800B59F0: 86/343 words differ (1 extra words …)          gfd.sh: 69 aligned rows
```
Semantics: buttons D_8011A994[0..3], with shadow copies at [i+4] and a spinning cursor at [8]; 0x40-byte items
as in drone_throttle_calc.
1. If D_8011AC94 is set, the selected button (D_8011AD44) rotates towards π and the others towards 0, at 4π·dt
   (frame time D_8002EB94, volatile). Angles snap when D_8011AC98 == 1.
2. Each button: identity matrix; pos = {−90, y, 300}, with y starting at 100 and dropping 40 per visible item;
   y is eased at 180/s unless snapping. The shadow copy gets x = 150. The matrix is rolled by angle − π/2 and
   copied to the shadow.
3. Texture "BUTTON_SELECT" if angle > π/2, else "BUTTON".
4. Visibility from D_8011AD40[i]; the shadow of the last item is hidden.
5. The cursor spins at 2π·dt and is placed at {−160, sel_y·225/300, 225}. Unless snapping, its y is eased with
   speed D_80152678, clamped between 180 and 540·D_8011AC9C. Then WRollUV, RollUV(π/2), scale 0.4, and the snap
   flag is cleared.

The loop body is identical. Found: `#pragma intrinsic(fabsf)` (abs.s), D_80152678 volatile, and the frame
(a 16-byte unused array above `old[3]`; one extra f32 between `sel_y` and `tex`). Residual (lane: PRE/colouring):
- retail recomputes `d*60/10` after the store `D_80152678 = d*60/10` (two mul/div pairs); ours CSEs it;
- retail keeps 60 and 10 in f24/f26, reads the speed once into f12 for the final easing, and keeps the target in
  f20.

Best next hypothesis: the clamp is written with a MIN/MAX macro, or the speed is copied into a local after the
clamp. Use the w5d PRE oracle (`nopre.sh`) on the `d*60/10` bit to see what kills it in retail.

## entity_update_callback — 546/546 words, 3 rows by mnemonic

`entity_update_callback/best.c` (`sweep/` has the steps).
```
cloud/work/frontier/w6d/tools/fd.sh entity_update_callback/best.c entity_update_callback --mnem
  want 546 words, got 546; differing rows 3; unverified 8 unresolved 0
tools/sc.sh … → 440 aligned rows positional (v0/v1 renames throughout)
```
Semantics: callback (obj, s16 mode) for a per-car debris/flare effect. Records: D_80154660[car], 0x190 bytes:
- 4 Piece of 0x54 bytes: handle, mat[3][3], pos, scale/dscale, yaw/dyaw, pitch/dpitch, alpha/fade, height;
- glow handle at +0x150, pos at +0x178, alpha/phase at +0x184/185, life at +0x188, timer at +0x18C.

Steps:
1. `life -= dt`. If `life <= 0 || D_8014A250[car].s6C4 >= 0` (car record 0x808 bytes), free the pieces
   (entity_spawn_callback).
2. If the car's +0x640 byte is 0: free the glow when D_80156994 || D_8014978C >= 6, free the pieces, and unlink
   the object. The same unlink happens when mode == 0 (one shared call; a `goto done` reproduces retail's block
   order).
3. Otherwise copy the car position. Each 1/30 s (`timer`, reset to 0.0333333f), the phase steps alpha by ±31 or
   restarts it with ANSI rand(): `seed*1103515245+12345`, `(s32)(r*3.0f/32768)*-1` (the multu by the −1 constant
   web), alpha 95.
4. If active_player_count < 4 it sets the glow node colour to {255,255,255,alpha} and grows node +0xC by 0.05
   up to 1.
5. Each piece: diagonal matrix = scale; offsets {0,0}, {+h/2,+h/2}, {−h/2,−h/2} or {−h/2,+h/2} by index (dx/dz are
   uninitialised locals, which gives the 228/232 stack homes); pos = car + offset with y + height − 3; height
   += 0.2. Then PitchUV(pitch) (func_80090F44) and YawUV(yaw) (func_80090E9C); scale += dscale; dscale −= 0.002;
   yaw and pitch integrate. When life < 0.2 the piece fades and its node colour becomes {244,205,20,alpha}.

Found:
- `Piece *p` for the free loops;
- `timer <= 0` as the body test;
- a signed rand seed;
- the frame: `char buf[16]; u8 color[4]; f32 dx, dz; char buf2[80];` — unused locals, which matches a 256-byte
  frame, color at sp+236 and dx/dz at 232/228;
- an empty `if (obj == 0) {}` at the start of the main path. It gives the 952 stride constant one more block, so
  &D_80154660 takes s7 and the car base takes s8 as in retail (552 → 546 words). The `if` is unproven: probably a
  compiled-out check.

Residual: in retail the car-index reload is in v1 and the record pointer in v0; ours is the reverse. Forcing the two
type-4 webs did not reproduce retail, so they are probably not those webs. Next: trace with `CDX_DETAIL_WEB` and map
webs to expressions with the w5d `prereport.py`.

## audio_doppler_full — countdown "3-2-1" display

`audio_doppler_full/best_standalone.c` (standalone form) and `best_unit.c` (form for the unit).
```
tools/fd.sh audio_doppler_full/best_standalone.c audio_doppler_full --mnem   → 238 vs 232 words, 40 rows
python3 -m tools.conveyor.pipeline.blob_unit --tag w6d score audio_doppler_full --with audio_doppler_full/best_unit.c
  FAIL audio_doppler_full: 167 of 238 words differ; compiled body is 240 words, target 238
```
Semantics: per player, when `show` is set:
1. A quad of 4 corners (template D_801140F8, size = frac(clock − 0.001)·12, at least 5) is rotated by the view
   matrix (func_8008C544) and offset by the view position. Views are D_80150B70, 152 bytes.
2. The first time, it loads texture "CNTDWN3" (func_800B24EC = MBOX_FindTexture_Sub) and creates a poly with
   func_800A78BC: flags (1<<player)|0x82C0, colour D_801140F4.
3. While the countdown value D_80152032 is below the shown value D_801543A4, the last player reloads
   "CNTDWN%d", plays sound 75, 76 or 77 for 3, 2 or 1, and clears the poly's hidden bit.
4. When `show` is clear, the poly is freed and D_80152032 is set to −1.

N64-only; no arcade ancestor.

Found:
- the three sounds are the locked wrapper **func_800B61A8(id, 0, 1, 0)** (arcade SOUND), inlined by umerge in the
  unit. It gives the inverted `bnez → jal; b; nop` shape;
- D_80140BDC is volatile;
- in the unit the wrapper inlines cost 72 bytes of frame (3 × 24), so retail's 80 bytes below `name[32]` are the
  inlines plus about 8 bytes of named locals. Ours is 288 against 256 even with only the needed locals.

Residual:
- the frame (named-local slots);
- in the standalone form, &D_80154398 (texture) takes s7 where retail keeps &D_801140F4 (colour) in s8 and
  rematerialises the texture address;
- the quad[i] += pos load and store order.

Next: reduce named locals (player/view/t/size: pointer-loop forms), then trace the address webs in the unit.

## func_8010D680 — 49 rows

`func_8010D680/best.c`.
```
tools/sc.sh func_8010D680/best.c func_8010D680 --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'
  86/119 words differ (2 section-relative relocations unverified …)    aligned (fd.sh): 49 rows
```
Semantics: callback (holder, s16 on).
- If on == 0: unlink (entity_transform_apply(h, 1)).
- Else, unless D_801170FC is set: if the thing's flag bit 2 is set, play its sound at vel·D_8002EB94
  (volatile; sound_position_set).
- Otherwise, if node D_8012E700[(s16)handle].flags & 0x80000000 (written as a mask: retail `sll 0; bgez`), map
  type 350..360 to slot 0..7. The jump-table order is 350, 351, 352, 355, 356, 357, 358, 360; 353, 354 and 359
  go to default. If no car's +0x384 byte equals the slot, call model_transform_setup(handle, 0, 15) and set flag 2.
- Otherwise call model_data_load(handle, 0, 15).
- The model functions are called unprototyped (the raw int handle).

Found:
- the frame: v[3] declared after t/handle/index;
- `vel` as a local;
- the `flags` local gives v0.

Residual: retail narrows `on` in place (`sll t6,a1,16; sra a1,t6,16`). That happens only when `on`'s uopt web
spans more than one block (traced: nocs 1 in ours, more than 1 when `on` is reused later). Reusing `on` as the
`ok` flag fixes the head but drops retail's `li a1,1`. A real `|| on == 0` emits code. Tried: empty `if (on)` in
five places, switch on `on`, K&R definition, `if (h == 0) {}` at entry.

Next: find a use of `on` that survives to colouring but emits nothing (late-folded, as in w4c), or a `do {} while`
or loop around the body.

## camera_play_script — not attempted

Classified only. It is 880 words with a 608-byte frame and saves all s and FP callee-saved registers. It is a
car-against-collision-polygon routine: packed 5-bit normals × 1/32, func_800AD5D0/func_800AD650 lookups, and a
call to the register-parameter internal func_800C36A0. func_800C36A0 is currently in `src/blob/groups/func_800AD4C8`
with stand-in callers `__standin_c36a0_a/b`; camera_play_script is one of its real callers (the other is
camera_victory). No time was left for a 3.5 KB body after the other seven.

## What generalises

1. **`.noalias reg,$sp` comes from typed array indexing.** `&ARRAY[i]` held in a pointer lets ugen mark it as not
   aliasing the stack, and as1 then hoists its loads above local-array stores. Two spellings of the same address
   stop it: `p = ARRAY; p += i;` and an integer cast. Check the ugen listing (`o3s.sh`) for `.noalias` whenever
   loads come too early relative to stack stores (stat_lap_split; same lever as w3b's func_800F8EC8).
2. **Folded single-precision literal products:** retail 0x3F5A9111 is `0.924f * 0.924f`, not `0.853776f`. When the
   nearest decimal is one ulp off, try a product of two short literals.
3. **An "x · 1.0" multiply in retail is a variable holding 1.0f.** cfe folds the literal form and a static helper
   with a float parameter is not inlined. A local assigned 1.0f survives because uopt propagates the constant
   after building webs (drone_throttle_calc; also w4c/w5a).
4. **The empty-`if` priority lever works on callee-saved contention.** When a constant web and a variable or
   address web compete for the last s-register, one compiled-out `if` in the shared region lowers the
   wider-ranged web's save more than the narrower one's and swaps them. Read the traced uopt `save`/`nocs` first,
   then place the `if` (drone_throttle_calc: color against constant 10; entity_update_callback: &D_80154660
   against the 952 stride).
5. **An IPA stack-passed parameter reproduces with stand-ins.** An internal callee that reads `lb 115(sp)`
   (= caller 0(sp)) and saves nothing reproduces with two trivial stand-in callers. Two callers are needed, or
   umerge inlines it.
6. **SOUND(...) in N64 code is the locked wrapper func_800B61A8.** Its fingerprint at a call site is argument
   set-up before the D_8010FFC0 test, then `bnez → jal; b; nop`. It is inlined only in the unit, so score such
   callers with `blob_unit`. Each inline costs 24 bytes of frame.
7. **`(x & 0x80000000)` gives `sll t,x,0; bltz/bgez`**, not a signed bitfield and not `< 0`. The same idiom is in
   locked func_80096C28.
8. **Strings and float literals in one function** are not contiguous in retail (strings at 0x8012xxxx, floats in the
   0x80123870 .rodata). The scorer reports NOT VERIFIED, not MATCH, even when the code is identical. The
   integrator's per-reference own-data check is needed for such functions.
