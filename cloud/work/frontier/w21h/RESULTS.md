# Wave 21, lane w21h: engine_sound_sync + audio_effect_setup; func_800D7634 (+ func_800D6914)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w21h. Nothing committed, spliced or pushed. **No matches,
no group deliverables** (no member reaches EQUAL except the already-provisional pieces below). No permission
denials. Variants are in `<fn>/v/` (ignored). Unit scores: `python3 -m tools.conveyor.pipeline.blob_unit --tag w21h
score ...` and `TAG=w21h cloud/work/frontier/tools/trace/us.sh` (rows words/ops/norm, frame want/got).

| Function | Bytes | State | Flags | Scorer output (whole-program unit) |
|---|---:|---|---|---|
| engine_sound_sync | 1236 | not matched; 14 -> 8 norm rows (words 152 -> 117), frame exact | -O3 | `FAIL engine_sound_sync: 98 of 309 words differ` |
| audio_effect_setup | 184 | EQUAL in the unit next to engine_sound_sync (provisional: lands only with it) | -O3 | `EQUAL audio_effect_setup: 46 words (internal, c_aes.c)` |
| func_80098ADC (stub) | 8 | body hypothesis (inlined gain clamp) | -O3 | `EQUAL func_80098ADC: 2 words (internal, c_best.c)` |
| func_800D7634 | 1804 | not matched; new reconstruction, 0 normalised rows, frame 256 exact, 15 words = one colour decision | -O3 | `FAIL func_800D7634: 15 of 451 words differ` |
| func_800D6914 | 1260 | unchanged (w20e best, now with its real caller) | -O3 | `FAIL func_800D6914: 46 of 315 words differ` |

## engine_sound_sync (1236 B): 14 -> 8 normalised rows (stop rule)

Commands (current tree):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w21h score engine_sound_sync audio_effect_setup func_80098ADC \
  --with cloud/work/frontier/w21h/engine_sound_sync/aes.c --with cloud/work/frontier/w21h/engine_sound_sync/best.c \
  --internal audio_effect_setup --internal func_80098ADC --neighbours
  FAIL engine_sound_sync: 98 of 309 words differ
       umerge inlined into it: func_80091AF0, func_80091AF8, func_80098ADC
  EQUAL audio_effect_setup: 46 words (internal, c_aes.c)
  EQUAL func_80098ADC: 2 words (internal, c_best.c)
  locked bodies that differ in this unit: 0
us.sh: words 117 ops 8 norm 8 | frame 376/376
```
What moved it (each measured in the unit; `best.c` header discloses all):
1. **audio_effect_setup's 6 words are closed** by the lean form already in the repo
   (`cloud/work/lean_audio_effect_setup_20261006/group.c`: consumed `Effect *effect = input;` view and the
   owner-field test before binding `owner`). `aes.c` = that body on the B132 types.
2. **Retail's s5 = &D_80146188, s8 = 1, dead s7, uncoloured array bases are one compiled-out compare** at the end
   of the listener loop: `if (i == 5 || D_80146188.count == 1) {}`. Mechanism (ctrace): a code-free use at loop
   weight lifts the list-base lda web (tot 20 -> 120) above &direction; the constants 5 and 1 become two
   save-7.14 webs coloured s7/s8 (ugen emits `li s7,5; li s8,1`, as1 keeps only the `li s8,1` that retail has);
   the three array-base webs then find no callee-saved register and split into a1..a3 as in retail. `i` in the
   compare also puts loop 2's index in v1 (retail). An uninitialised variable read (`if (pad) {}`) gives the same
   "saved, never written" register (proved with e2/e4), so s7 in retail can be either kind of web.
   Shaping device, identity unknown (assert/debug macro is the natural candidate).
3. **func_80098ADC (locked caller-less stub right before the function) = an inlined helper that writes the total
   gain clamp through a pointer** (`func_80098ADC(&out[3], total)`). The pointer store keeps loop 2's count load
   below the clamp (retail order) and reorders the a1/a2/a3 bases; its 8-byte inline area replaces one w20e pad
   (frame unchanged with `i` declared in the former pad1 slot). Three helper calls (pan/depth too) give the
   right code but frame 432, a separate 1-param pan helper is not inlined; so only out[3] uses it.
4. **`kind = effect->kind` through a named local** (former pad slot): kind in v1, int temp ring as retail.
5. **Loop 2 without `weight`**: `totalPan += panValues[i] * (gains[i] / total)` numbers pan before gains (retail
   a1 = pan, a2 = gains); `weight` stays declared (unused, frame slot at 324).
Unused locals: `weight`, `pad3` = exact frame residual (retail 376, without them 368, 2 slots).

Residual (traced, not closed):
- **FP temp ring two registers late in retail**: retail's first FP temp (out[0] = source->parameter) is $f8, ours
  $f4; every procedure in the unit starts its FP ring at $f4 (ugen trace, 303/303 procedures), so retail made two
  FP ring allocations before that statement that left no code. In the direction subtraction retail also
  allocates source before listener (ours listener first). This shifts all FP temps in loop 1 and is the whole
  cross-product block (rows 0x180-0x1bc). Statement orders, `-a + b`, two-step subtraction, chained zero inits,
  `+ 0.0f`: no change. Compiled-out FP compares generate code (not usable).
- **gains reload web $f2 (retail) vs $f12 (ours)**: tie with nearVolume at save 300, decided by web number
  (w148 near, w155 gains). `force.sh ... p1:w155=c25,p1:w148=c26` fixes the registers but the add/mul order in
  the loop tail stays (2 rows); statement order / operand spellings are canonicalised.
Best next hypothesis: the two missing FP allocations and the source-first operand order both point at the
direction/out code being a second inlined helper whose parameters are FP temps (e.g. a vector-subtract or
projection helper with an f32 argument) — but retail has only one stub (func_80098ADC) and the frame has no room
left, so it would have to replace the clamp helper hypothesis; test with `ugt.sh` on a helper variant.

## func_800D7634 (1804 B): new reconstruction, 15 words (stop rule)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w21h score func_800D7634 func_800D6914 func_800D690C \
  --with cloud/work/frontier/w21h/func_800D7634/best.c --internal func_800D6914 --internal func_800D690C \
  --internal post_msg_a --internal post_msg_b --internal row_ok --neighbours
  FAIL func_800D7634: 15 of 451 words differ
       umerge inlined into it: physics_init_mode0, post_msg_a, post_msg_b, row_ok, speed_mode0_wrapper, speed_mode1_wrapper
  FAIL func_800D6914: 46 of 315 words differ; +0x6c: .rodata+0x5c0: retail words encode 0x433402b4, outside the image
       umerge inlined into it: func_800D690C
  EQUAL func_800D690C: 2 words (internal, c_best.c)
  locked bodies that differ in this unit: 0
udiff: want 451 words, got 451; differing rows 25 (words), 0 (norm); frame 256/256; unverified 10
```
Semantics: N64 audio-options screen input handler (no arcade ancestor: the callees are N64 sound/menu code; the
direct sibling is the locked pause-menu handler `physics_sym` in src/blob/groups/physics_sym, whose idioms it
shares). Rows: 0 sfx volume (D_80146108.sfx @+13), 1 music volume (+12), 2 music track (D_80110640 0..13, 12 =
random through func_800D6160(0)), 3 stereo/mono (+15, func_800D6E00). A/B/Start leave (D_8011063C = 2); left/right
move the row (D_80110638, wrap 0..3); up/down change the value; func_800D6914 draws the 3D buttons.
Levers: every `speed_set(x, 0.05f, 1, 0)` / `(.., 0, 1)` is the locked kept wrapper speed_mode0_wrapper /
speed_mode1_wrapper and `func_800D6160(0)` is the locked physics_init_mode0(), all umerge-inlined: their 8-byte
areas are exactly retail's frame (200 -> 256, post areas at 212 and 116); the row wrap is
`do { ... } while (!row_ok(D_80110638));` with an inlined getter `v = 1; return v;` (retail's unfolded
`li v0,1; beqzl v0`; a flag variable folds the loop, `return 1;` changes the frame); btn read before the stick
copy (int ring); `f32 stick[2]` copied by floats; `Pad16 *p` function-level first (physics_sym order) and one
unused `padB` (exact frame residual: 256 vs 248 without it, 1 slot).
Residual: the D_80110640 address web at the four track-select sites is a1 (ours) / v0 (retail), 15 words.
`force.sh` shows the web (w212 for site 1) has v0, v1, a0, s1, s2 forbidden, i.e. its range includes the
speed_set call block (IPA registers of speed_set; v1 = &D_80146108 live across it); none of the phase-1 v0
webs is the interferer (forcing each away leaves v0 forbidden), so it is a precoloured/IPA register. Block
boundary devices, if-chain instead of switch, direct speed_set (no wrappers), func_800D6160 instead of
physics_init_mode0, a separate inlined sync helper (not inlined: 3 sites): no change.
Hypotheses that matter for landing:
- **row_ok has no retail stub.** The only stub is func_800D690C (before func_800D6914), which w20e uses as a
  D_80110638 getter for func_800D6914 (3 rows). Giving func_800D690C the row_ok body (`v/d2.c`) keeps
  func_800D7634 at 15 words but puts func_800D6914 back at 283 words (w17h state). One of the two uses is wrong.
- **post_msg_a/post_msg_b** are non-static copies of func_800C8918's post_type1 (no retail stubs either; same
  status as the locked func_800C8918 group).
- func_800D7634's own callees with IPA conventions: speed_set (locked, s1/s2/$f20/$f22), func_800D6160 (t0
  param; locked context in codex_mode_init_a18 — the unit already compiles it, `move t0,zero` matches),
  drone_pathfind_main and credits_screen (unmatched asm, clobber s-registers unsaved: harmless here because
  func_800D7634 keeps nothing in s-registers across them), func_800B4FB0 (asm). The save set s0-s8/$f20-$f30
  comes out right from the C callees in the unit.
- Integration later: func_800D7634 is `context` (m2c, kept) in the locked `credits_scroll_grp` group
  (src/blob/groups/credits_scroll_grp/gr3_e.c); a landing group needs `prefer_definition` for it.

## func_800D6914 (1260 B): unchanged
In the real-caller unit it is exactly w20e's state (3 norm rows: pi/180 constant tie $f22/$f24, D_80110638 in
the getter's v1 web instead of ring temp t7 with `bne s4,t7` operand order, tex home 4 B). The retail compare is a
direct `i == D_80110638` load into a ring temp, so w20e's getter is a stand-in for whatever produces retail's
s7/s8 there (see the engine_sound_sync finding: compiled-out compares produce exactly such s7/s8 webs — the next
thing to try on func_800D6914).

## Integration notes
Nothing to integrate. If a later lane closes these:
- engine_sound_sync group: members engine_sound_sync, audio_effect_setup, func_80098ADC (locked stub given a body:
  `--revert-single func_80098ADC`, non-static, forced_internal + prefer_definition like func_800B59E8); context:
  byte-identical copies of func_80091AF0.c/func_80091AF8.c (codex_heap_release_a25) whose inlining matters;
  unit_overrides: forced_internal audio_effect_setup, func_80098ADC.
- func_800D7634 group: members func_800D7634, func_800D6914, func_800D690C (stub, `--revert-single`), helpers
  post_msg_a/post_msg_b/row_ok non-static off keep; context copies of speed_mode0/1_wrapper and
  physics_init_mode0 (codex_mode_init_a18) and func_80091AF0/AF8; prefer_definition func_800D7634
  (credits_scroll_grp context); forced_internal func_800D6914, func_800D690C.

## What generalises
1. **A saved-but-never-written s-register and a hoisted `li sX,1` with no use are compiled-out compare webs.**
   `if (a == K1 || b == K2) {}` gives code-free uses that lift the address/variable webs and colour the
   constants; ugen emits the constant loads and as1 drops the unused ones. An uninitialised variable in an empty
   `if` gives the same dead saved register.
2. **A pointer-store helper blocks PRE hoisting of a global load** above it (count load stayed below the clamp).
3. **Kept locked one-line wrappers are inlined across files and set the frame**: 8 bytes per inlined call site;
   count call sites between two known inline areas (post areas at 212/116) to identify them.
4. **An unfolded `li v0,1; beqz v0` is an inlined getter returning a constant through a variable**; plain flags and
   `return 1;` fold or change the frame.
5. Every procedure's FP temp ring starts at $f4 (ugen trace): a retail ring offset at the first FP statement means
   extra FP allocations before it that left no code.
