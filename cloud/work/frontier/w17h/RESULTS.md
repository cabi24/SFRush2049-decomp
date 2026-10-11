# Wave 17, lane w17h: fresh game targets

Targets in order: physics_response, func_800D6914, engine_sound_sync, func_800C8918.
Builder scratch: watchman2:~/rush2049/scratch/frontier/w17h. Nothing committed, spliced or pushed.


## Prior state (from earlier lanes)
- physics_response (920): 13/230 in w2a's real group (caller audio_effect_apply unmatched; physics_response writes
  s0-s6 unsaved, so it is provisional until the caller matches). Residual: `cur = next` placement / v1 vs a1 colour.
- func_800D6914 (1260): w10e 282/315 provisional (real caller func_800D7634 L8 unmatched).
- engine_sound_sync (1236): lean packet 286/309; no draft in w14v (m2c failed).
- func_800C8918 (628): w14za 21/157 in the unit (busy-wait START/END address materialisation).

## Summary table (updated as I go)

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| func_800C8918 | 628 | **MATCH** (whole-program unit EQUAL; group MATCH) | -g0 -O3 -mips2 -G 0 -non_shared | unit: `EQUAL func_800C8918: 157 words (kept, c_func_800C8918.c)` / `locked bodies that differ in this unit: 0`; `score.py group groups/codex_heap_release_a25 --claims` -> `func_800C8918:\n  MATCH`, exit 0 |
| engine_sound_sync | 1236 | not matched, 107 norm rows (lean packet re-scored; no new draft) | -g0 -O3 -mips2 -G 0 -non_shared | unit: `FAIL engine_sound_sync: 286 of 309 words differ; compiled body is 315 words, target 309` (words 277, ops 107, norm 107, frame 376/368) |
| func_800D6914 | 1260 | provisional lane, not matched (caller func_800D7634 unmatched) | -g0 -O3 -mips2 -G 0 -non_shared | unit with stand-in callers: `FAIL func_800D6914: 283 of 315 words differ; compiled body is 321 words, target 315` (norm 27, frame 144/96) |
| physics_response | 920 | **provisional MATCH** in the real group (caller audio_effect_apply is unmatched context) | -g0 -O3 -mips2 -G 0 -non_shared | `score.py group cloud/work/frontier/w17h/groups/physics_response` -> `physics_response:\n  MATCH\n    own .rodata verified at 0x80123DF4..0x80123DF8` |

## physics_response (920 B) -- provisional MATCH (was 13/230)

Deliverable: `groups/physics_response/` (ctx.c = w2a's context copy, physics_response.c; claims []).
Whole-program unit (`blob_unit --tag w17h score physics_response --with groups/physics_response/physics_response.c
--neighbours`): `FAIL physics_response: 229 of 230 words differ; compiled body is 243 words` (expected: the caller is
asm in the unit, so physics_response saves s0-s6/$f20-$f24 itself) / `umerge inlined into it: func_800B930C` /
`locked bodies that differ in this unit: 0`.

Arcade ancestor: game/checkpoint.c InitCPS's optimal-time walk (get_next_checkpoint ring, per-checkpoint time).

What closed it (traced, not swept):
1. **The next-section computation is the locked kept get_next_checkpoint `func_800B930C`** (0x800B930C, two
   functions before this one; wave-9 "inlined locked helpers"). Defined verbatim in the same file, above
   physics_response, it is umerge-inlined. Its placement is load-bearing: above = MATCH, below = 2 words, in a
   separate file (byte-identical locked copy as context) = 16 words (as1 order of `move s4,v0` / the cur*80
   address). To keep the helper text verbatim D_80151CE8 is declared as a struct `Route` (record 0, the header)
   and the 80-byte section records are `(&D_80151CE8)[i]`. With the array-typed declaration (`Sect D_80151CE8[]`)
   the helper's IR differs and the unit reports race_countdown_display and world_physics_tick (which inline
   func_800B930C) as differing; with the struct type 0 locked bodies differ.
2. **Retail's `move v1,s4` (cur = next) before the `next == last` test needs a block boundary.** Written there
   plainly, cur's web shares the block where the hoisted path-point pointer web (v1, save 4700) dies, interferes
   with it (numintf 9 -> 10) and is coloured a1: 211 words. A compiled-out `do {} while (0);` before `cur = next`
   (w15d's lever) gives the exact structure; the remaining 22 words are an s3/s4 swap.
3. **s3/s4**: force.sh `p1:w227=c17,p1:w38=c18` -> 0 rows (colour-only). ctrace: &D_80151CE8 743/10 = 74.3, next
   300/4 = 75. A second compiled-out block end right after `next = ...` puts one more block into next's range only
   (300/5 = 60): MATCH. 17 of 72 holder placements matched; single holders never (b1/b2 batches).
   Disclosed devices: the two `do {} while (0);`.

Types: D_80151CE8 = 80-byte section records, header in record 0 {s16 total@0, first@2 (ring wrap), last@4,
start@6 (first section), count@8; s16 time[2]@0x2A; s16 pt_start[]@0x30 (per path)}; D_8012E5E8[] = {u16 num;
PathPt *pts} (8 bytes); PathPt = {s16 pos[3]; u8 speed; u8} (8 bytes). Literal 1.4666667f = 0x3FBBBBBC.

Integration: provisional only. Lands when audio_effect_apply (0x800BAAA0, blocked by audio_mixer_main and
func_800B9740) matches: then put audio_effect_apply's real source in place of ctx.c and claim both. physics_response.c
then also defines func_800B930C (as world_physics_tick.c does); no unit_overrides were needed in the unit run.

## func_800C8918 (628 B) -- MATCH in the whole-program unit and in the extended a25 group (was 21/157)

Deliverable: `groups/codex_heap_release_a25/` = the locked group (all 7 files byte-identical, `cmp` checked) plus
`func_80091AF0.c`, `func_80091AF8.c`, `func_800C8918.c`; members += func_800C8918, func_80091AF0, func_80091AF8;
keep += func_800C8918; claims ["func_800C8918"].

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w17h score func_800C8918 func_80091AF0 func_80091AF8 \
  --internal func_80091AF0 --internal func_80091AF8 \
  --with cloud/work/frontier/w17h/groups/codex_heap_release_a25/func_80091AF0.c \
  --with cloud/work/frontier/w17h/groups/codex_heap_release_a25/func_80091AF8.c \
  --with cloud/work/frontier/w17h/groups/codex_heap_release_a25/func_800C8918.c --neighbours
  EQUAL func_800C8918: 157 words (kept, c_func_800C8918.c)
  EQUAL func_80091AF0: 2 words (internal, c_func_80091AF0.c); stub tail: 4 words of deleted-procedure stubs follow (NOT as in the image)
  EQUAL func_80091AF8: 2 words (internal, c_func_80091AF8.c)
  locked bodies that differ in this unit: 0
blob_unit score: 3/3 equal
score.py group cand/codex_heap_release_a25 --claims   (builder)  -> func_800C8918: MATCH; exit 0
```
(Without --claims every old a25 member is MATCH, func_80091AF8 MATCH, and func_80091AF0 prints
`MISMATCH (2 extra words (nonzero beyond target length))`: in the group object the two deleted static stubs of
func_800C8918.c (post_type1/post_type7) sit right after it; score.py measures up to the next symbol. Its own 2-word
slice is the retail `jr ra; nop`. One earlier `--neighbours` run listed anim_state_update while the coordinator was
mid-edit of codex_visual_tire_module_20261002 in the working tree; rerun after that settled: 0.)

Semantics: shutdown. If D_8011023C: func_800C885C() (release the two pending effect handles, inlined), post type-1
and type-7 messages (slot from func_80091B00 under the slot-queue lock D_80142728, sent to D_801427A8), busy-wait
until no slot in D_80142DD8[128] is used, clear the flag, init_wait_completion(), func_800960CC() x3 (heap free).

What closed it (each step traced):
1. **The wrappers are not the kept functions.** In the unit umerge never inlines kept audio_effect_process or
   object_type1_create/7 into a new caller (w14za worked around it by redefining them `__inline` with four unused
   locals each, not acceptable). Retail never calls audio_effect_process by jal anywhere (25 functions inline the
   heap-free sequence); the locked precedent is the internal heap helpers func_80095CF4/func_80095CFC/func_800960CC
   (wheel_params_set.c) and func_800C885C, which umerge inlines everywhere. Calling func_800C885C() and
   func_800960CC() gives the 24-byte heap-free areas (homes 192/168 and 80/56/32).
2. **Frame 176 -> 208: the slot lock/unlock are inlined helpers too.** The message areas were 16 bytes short each;
   w2c's rule (each nested inline level adds 8 bytes per site) pointed at two more levels, and the two caller-less
   2-word stubs func_80091AF0/func_80091AF8 sit right before func_80091B00. Given bodies (osRecvMesg/osJamMesg on
   D_80142728) they give frame 208 and every home. object_type1_create written with them is also EQUAL standalone
   (`blob_unit score object_type1_create object_type7_create`: EQUAL 26 words each), so they fit there too.
   The message posts are file-static copies (func_800CB748's static release_handle precedent): a one-parameter
   static `post_msg(type)` is not inlined at two sites (118 words).
3. **Busy-wait as an index loop** `do { for (i = 0; i < 128; i++) if (D_80142DD8[i].used) break; } while (i < 128);`
   gives retail's `move a1,v1` END copy and rematerialised table start (42 pointer-loop forms: 25-78 words; this
   form: 6). D_80142DD8 is `volatile` as in the locked func_80091B00.c. With `func_800C885C()` for the first two
   releases no extra local is needed (the five-func_800960CC form needs one unused local for frame 208).
4. **as1 store order** (2 rows x 2): retail puts the unlock's `addiu a0` before `sb type`; statements keep their own
   line through umerge inlining (`.loc` checked with as1t.sh) and as1 breaks ties by line, so the store and the
   unlock call share one line (`temp_v0->type = 1; func_80091AF8();`). Disclosed as a line-layout shaping device
   (w16h precedent).

Integration (coordinator): replace src/blob/groups/codex_heap_release_a25 with this dir (same name, every old member
kept); add unit_overrides `forced_internal` + `prefer_definition` (file .../codex_heap_release_a25/func_80091AF0.c and
.../func_80091AF8.c) for func_80091AF0 and func_80091AF8, as for func_80095CF4/func_80095CFC; blob_group splice.
Unit runs above used `--internal func_80091AF0 --internal func_80091AF8`; no other override. No data-symbol changes.
Possible follow-up: retail has one deleted static stub (func_800C8784) before object_type7_create, which suggests
object_type1/7_create share a static helper in the original file.

Types: ShutdownMsg/Slot18 = {s16 id; s8 type; s8 used; u8 payload[20]} (24 bytes), D_80142DD8[128] volatile;
D_8011023C s8 flag; D_8011025C/60/44/48/70 u32 heap handles; D_80142728 slot-queue lock (OSMesgQueue),
D_801427A8 message queue, D_80152770 heap lock.

## engine_sound_sync (1236 B) -- not matched (time-boxed; nothing new landed)

Re-scored the lean packet (`cloud/work/lean_engine_sound_sync_20261006/candidate.c`, copied to `engine_sound_sync/best.c`)
in the whole-program unit: `FAIL engine_sound_sync: 286 of 309 words differ; compiled body is 315 words, target 309`,
`words 277 ops 107 norm 107 | frame 376/368`. Not a group-only case: no unsaved callee registers; audio_effect_setup
(provisional) is asm in the unit, so a unit EQUAL would be a real match (group packaging with codex_transform_b109
context for entity_transform_calc's register parameters).

Recovered from the retail listing (no arcade ancestor: arcade sounds.c drives a DSP board):
- the source list D_801461F0 is walked under the slot-queue lock D_80142728 (osRecvMesg/osJamMesg; probably the
  same inlined helpers func_80091AF0/func_80091AF8 as in func_800C8918 -- not tested here), `next` saved at 368(sp)
  before the callbacks.
- entity_transform_calc(handle, gain, pan, depth, parameter) takes its four floats in **$f22/$f24/$f26/$f28**
  (IPA register parameters); the clamped gain/pan/depth/parameter are homed at 360/356/352/348 and reloaded, and
  1.0f is rematerialised into $f26 after the call. $f30 = 0.0f, $f26 = 1.0f, $f20/$f22/$f24 = total/pan/depth sums.
- per listener: dir at 304..312, distance = func_80098A54(dir) (a0 = s6 = sp+304), basis rows at 256/268/280
  (cross product of rows 1 and 2 into row 0), then a transpose and WorldVector. 120..140(sp) are the bottom of the
  locals area = FP spill temps: six transposed off-diagonal values live there; the 256.. array is never re-read
  after the transpose. The lean draft spills a different set (it writes transposed values back to 256..).
- Next step: ctrace the FP webs of the listener loop (which six webs are spilled and why), and test whether the
  transpose is written as TransposeCopyUV into a second local or as scalar temporaries; frame is 8 bytes short.

## func_800D6914 (1260 B) -- provisional lane, not matched

Real caller func_800D7634 (L7) is blocked by five unmatched functions, and retail func_800D6914 writes s0-s8
without saving them, so any result is provisional. New draft `func_800D6914/best.c` = the now-locked sibling
func_800B59F0 (physics_sym group) with D6914's globals, no visibility branch, two model_transform_setup calls and an
unconditional `y -= 40` (283/315, norm 27 rows; w10e's best.c with a `f32 cur[12]` filler: 282, words 74).
Frame arithmetic (retail homes old 76, sel_y 68, tex 62, frame 144; draft old 68, sel_y 60, tex 54, frame 96):
retail has exactly 10 more words of named locals declared before `old` than the sibling (56 bytes above old vs 16).
A `f32 m[3][3]; f32 k;` pair there gives frame 136 (not 144) -- the identity of these locals is the open question
(the sibling's caller-less getter stub idiom: func_800D690C, a locked 2-word stub, sits right before this function
and is probably an inlined helper whose area accounts for part of it). Colour residual as w10e: retail keeps
&D_801105B8 in s7 and constant 1 in s8, never colours &tex.
Register map (retail): s0 = &D_801102B4[i], s2 = .mat, s5 = .pos, s6 = D_801102B4[i+4].mat, s3 = &D_8002EB94,
s4 = i, s7 = &D_801105B8, s8 = 1; $f20 = y, $f22 = 180, $f24 = pi, $f26 = 0, $f28 = 4pi, $f30 = pi/2.

## What generalises
1. **Wrappers that umerge will not inline are the wrong functions.** Kept locked functions with calls
   (audio_effect_process, object_type1_create) are never inlined into a new caller in the unit; retail's inlined
   copies come from internal helpers (func_800960CC, func_800C885C) or file-static copies. Check `umerge.log`
   (`build/blob_unit/<tag>/umerge.log`) for what was inlined before shaping frames with dummy locals.
2. **Frame short by 16 per inlined site = two missing nested inline levels** (w2c's 8-bytes-per-level rule). Look
   for caller-less stubs next to the callee (func_80091AF0/func_80091AF8 beside func_80091B00): they are the
   lock/unlock helpers. Probably applies to every other func_80091B00 caller and to engine_sound_sync's lock.
3. **A strength-reduced index loop gives an address-copy + rematerialised base** that no pointer loop reproduces
   (func_800C8918's `move a1,v1`).
4. **Statements keep their own line through umerge inlining, and the inlined body of a nested helper takes its
   call-site line**; as1 breaks ties by line, so two statements on one line reproduce a retail order that
   neither line order gives.
5. **A kept helper's definition placement changes the inlined code** (physics_response: helper above = MATCH, below
   = 2 words, other file = 16 words). In the unit the definition that wins is shared by every caller, so a pasted
   copy must be IR-identical to the locked one (same struct-typed access, as world_physics_tick.c does), or other
   locked inliners (race_countdown_display, world_physics_tick) change.
6. **A block boundary decides whether two webs interfere** (physics_response: `cur = next` in the block where a
   hoisted pointer dies -> interference -> a1; behind a compiled-out block end -> v1), and an extra block in a
   web's range is the cheapest priority lever (save = tot/nocs).

## Integration notes (coordinator)
- func_800C8918: install `groups/codex_heap_release_a25/` over src/blob/groups/codex_heap_release_a25 (same name,
  all 16 old members, old files byte-identical). unit_overrides: add `forced_internal` and `prefer_definition`
  entries for func_80091AF0 (file .../codex_heap_release_a25/func_80091AF0.c) and func_80091AF8
  (.../func_80091AF8.c), mirroring func_80095CF4/func_80095CFC. No data-symbol changes. Claims: func_800C8918 (+628 B).
- physics_response: provisional only (add to provisional.json with evidence groups/physics_response/); lands when
  audio_effect_apply matches. Its file defines func_800B930C verbatim (struct-typed D_80151CE8).
- Nothing in cloud/matches/ (no standalone `score.py fn` matches). No permission denials this lane.
