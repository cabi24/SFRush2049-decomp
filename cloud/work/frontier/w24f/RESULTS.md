# w24f RESULTS (wave 24, game fresh singles)

Status: done (session end). No MATCH delivered: no cloud/matches files, no groups, nothing to integrate.
Builder scratch watchman2:~/rush2049/scratch/frontier/w24f (toolkit reused from wtk). No permission denials.

Targets (fewest prior attempts first): audio_fade_control unit (w20f 6/8), ghost_race_setup (w20f/w8b 16/110),
wheel_setup_initial (w20f/w21g s4 save), func_8008AD6C (w21g 29/41), func_800EC190 (w21g-w23e 2 words),
func_800A1BB4 (w21g 35/46).

| Function | Bytes | State before -> after | Flags | Scorer line (unit, `--neighbours`) |
|---|---:|---|---|---|
| audio_fade_control unit (8 fns) | 1904 | 6/8 -> **7/8 EQUAL**; audio_fade_control 4 words -> EQUAL (1 disclosed holder); gfx_setup_e700 4 rows, proven colour-only | -O3 unit | `blob_unit score: 7/8 equal`, `FAIL gfx_setup_e700: 9 of 32 words differ`, `locked bodies that differ in this unit: 0` |
| wheel_setup_initial | 124 | 27/31 (frame 40 vs 48, missing s4) -> **4/31, frame exact, colour-only** (force -> 0 rows) | -O3 unit | `FAIL wheel_setup_initial: 4 of 31 words differ` / `locked bodies that differ in this unit: 0` |
| ghost_race_setup | 440 | 17 -> 16 words (osSendMesg rename); shared-reload form found, as1 hoist traced | -O3 unit | `FAIL ghost_race_setup: 16 of 110 words differ` (best_words.c) |
| func_8008AD6C | 164 | 29/41 unchanged; cause narrowed | -O3 unit | `FAIL func_8008AD6C: 29 of 41 words differ` |
| func_800A1BB4 | 184 | 35/46 -> 32/46 (body 46 = target) with a non-void form; not adopted | -O3 unit | `FAIL func_800A1BB4: 32 of 46 words differ` (v/r3.c) |
| func_800EC190 | 224 | not attempted (three lanes on the ugen ring already) | | |


## audio_fade_control unit (w20f best.c start)
Command (Pi, repo root):
`python3 -m tools.conveyor.pipeline.blob_unit --tag w24f score audio_fade_control gfx_setup_e700 audio_bus_route
audio_timing_sync sound_priority_set audio_stream_control audio_buffer_manage audio_state_save --with
cloud/work/frontier/w24f/audio_fade_control/best.c --internal audio_fade_control --internal func_800956BC --neighbours`
```
  EQUAL audio_fade_control: 53 words (internal, c_e2.c); stub tail: 4 words of deleted-procedure stubs follow (NOT as in the image)
  FAIL gfx_setup_e700: 9 of 32 words differ
  EQUAL audio_bus_route: 59 words (kept, c_e2.c)
  EQUAL audio_timing_sync: 46 words (kept, c_e2.c)
  EQUAL sound_priority_set: 59 words (kept, c_e2.c)
  EQUAL audio_stream_control: 59 words (kept, c_e2.c)
  EQUAL audio_buffer_manage: 59 words (kept, c_e2.c)
  EQUAL audio_state_save: 109 words (kept, c_e2.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/8 equal
```
audio_fade_control (4 words -> EQUAL). Two levers, both traced:
1. `entry` is assigned inside the condition, `(entry = node->entry)->flags[1] == 0`: retail loads entry after the two
   kind tests (block 2), not at the top. (A no-local `node->entry` form gives the same load placement but moves the IPA
   registers: node s3, 31 rows.)
2. The constant 2 is a coloured web (a0) in retail; ours had one use (save 0, never a candidate, `li at,2`). A
   compiled-out `if (node->kind == 2) {}` as the first statement of the then-arm gives it the second use with no code
   (DISCLOSED shaping device). In the else arm or before the loop it is inert; at function top or in block 3 with the
   old top-of-function entry load it breaks the block layout (38-39 words).
gfx_setup_e700 (4 rows, unchanged): **colour-only**. `force.sh e2 gfx_setup_e700 "p1:w21=c5"` -> `differing rows 0`.
w21 is the address web `&D_80146138` (type 1, save 1.0); ours takes a0 (cost 0.0: it is the call's a0 argument),
retail a2 (cost 0.1, i.e. a0 must be unavailable: some web live in a0 across the address's live range in retail).
The `lui a2; addiu a0,a2` and the missing `.noalias $4,$sp` (w20f) both follow from the a2 colour (as1 folds the
argument move; the lw keeps its place after the saves). Inert/worse: value holders before/after the inlined pop
(19-26 words: value becomes a register web), getter `free_list()` / `pop(List *)` helpers (4-9 rows), block-end
devices (b1-b4). Next: find the a0 interferer (an inlined helper parameter that stays in a0 over the address load?).
Integration: not installable yet (gfx_setup_e700 is a real caller of the internal audio_fade_control). When it
matches: group per w20f (members audio_fade_control + 7 callers + func_800956BC with `--revert-single func_800956BC`,
helpers renamed to stub names func_800953B4/BC/C4 and func_800954A0, non-static, not on keep).

## wheel_setup_initial (0x800A4AC4, 124 B): 27 -> 4 words, frame 48 exact, colour-only
Lever (traced from the frame arithmetic, not swept): the 20-byte slot table D_80156D38 is the `ResSlot` table of the
locked sibling **func_80097694** (codex_state_audio_a146: `p0C != 0 && arg0 == type && (arg1 < 0 || arg1 == f5)`).
wheel_setup_initial = "release every slot of this type": the per-slot test is an inlined static helper (the caller-less
stub func_800A4B40 right after the function) taking `(slot, i, type, s8 sub)` called with `sub = -1`. Its parameter
homes are retail's 8-byte local area (16..23, frame 48) and the constant -1 is retail's codeless 5th callee-saved web
(the s4 save that w20f/w21g could not explain: `-1 < 0` folds, so it emits no code but keeps a live range).
`best.c` (= v/R5.c, no shaping devices):
```
blob_unit --tag w24f score wheel_setup_initial --with cloud/work/frontier/w24f/wheel_setup_initial/best.c --neighbours
  FAIL wheel_setup_initial: 4 of 31 words differ
  locked bodies that differ in this unit: 0
force.sh wR5 wheel_setup_initial "p1:w47=c17,p1:w46=c18"
  want 31 words, got 31; differing rows 0 (words); frame 48/48
```
Residual: constants 64 (w47) and -1 (w46) tie (save 3.33, tot 10, nocs 3); the lower web number (-1) takes s3, retail
gives 64 s3 and -1 s4. The constant web numbering is NOT first appearance: -1 is numbered first whether it appears at
the call before the loop (G2) or inside the loop body (R1/R5); -2/-100/-1609, `s32 sub`, `64U`, `u32 i`, operand and
clause orders, holders on `sub`/`type`, `if (i == 64) {}` (worse: i/pointer swap) all leave the tie. Do NOT use the
wrapper form (G2: `wheel_setup_initial(type) { release_type(type, -1); }`, also 4 words): umerge then inlines
wheel_setup_initial into billboard_render and state_change_preprocess (`locked bodies that differ: 2`). Next: read how
uopt numbers constant webs (makelivranges order) or try the w17f prio_patch oracle to find a 1-4 block change on -1.

## ghost_race_setup (440 B): 17 -> 16 words; structure traced
- **Renamed libultra**: the release path's unlock is `osSendMesg` (0x800075E0); w8b/w20f's `osJamMesg` now binds to
  0x80007440 (1 word). `best_words.c` = w8b best + rename: `FAIL ghost_race_setup: 16 of 110 words differ`.
- With w20f's inlined `func_800960CC(state->allocation)` (retail's `lw s0,76(s2)`), a compiled-out `do {} while (0);`
  after the ready/release if-else gives retail's shared count reload (`b 0x158`) (`best.c`, 53 words / ops 11).
- Listing oracles on the g1 listing (`build/trace/w24f/q*.s`): shared reload alone = q2; shared reload + a block
  boundary after `lw $16` = q3, 8 rows. as1 trace: the release block is scheduled retail-like first (lw s0 first), then
  as1's cross-block pass hoists `move a0,s5; move a1,zero` above `beqz` to fill the `lb t6` load delay; retail has the
  stall and `beqzl` with lw s0. Next: why as1 may not hoist into that predecessor in retail (liveness of a0/a1 on the
  ready arm, or a different predecessor shape).

## func_8008AD6C (164 B): 29/41 unchanged
Retail facts: each arm computes its own `lo = w & 0xFF` (a1), `x = w & 0xFF00` (v1) and mask 0xFFFF0000 (a2); the
`andi a1` and `lui a2` in the entry block are as1 delay/hoist fills from arm 1, and arm 2's `lui a2` sits in the beq
delay slot. So x, lo and the mask are coloured webs in retail; ours (pretrace) has no web for x/lo at all (propagated
to ugen temps) and the mask only for arm 2. Unrolled 2-iteration loop for arm 2 (L1-L3): not unrolled, worse.
Next: a form where x/lo live across a block boundary or a w redefinition (all pieces of word 0 taken before
`w = gfx[1]`).

## func_800A1BB4 (184 B): 35 -> 32/46 in a non-void form (not adopted)
The retail early return (`bnez t7,cont; jr ra; nop`) appears only when the early return carries a value distinct from
the end: `s32` with `return 0;` early and falling off the end (v/r3.c) gives retail's block shape and body length 46
(`FAIL func_800A1BB4: 32 of 46 words differ`), but `move v0,zero` sits where retail has `nop` and rec moves to v1
(retail v0). Void forms with codeless statements in the return block (holder, `do {} while (0);`, `index;`) and
`return rec;` forms are inert or worse. Callers are not in C, so the return type is open. Next: a value-returning
form whose early return value is already in v0 (retail v0 = rec at that point).

## What generalises
- **Read the locked siblings' struct and predicate**: wheel_setup_initial's missing frame area and codeless s4 came from
  func_80097694's `(type, sub)` predicate inlined with `sub = -1`; a folded constant parameter keeps a coloured web.
- **A wrapper made small by an inlined helper gets inlined into its callers** by umerge; check `--neighbours`
  (locked callers change) before trusting a wrapper form.
- **Assigning in the condition** (`(entry = node->entry)->flags`) moves a load below the earlier tests without losing
  the variable (the no-local form changed the IPA registers).
- **A constant compared once can need a second, codeless use** to be coloured (`if (node->kind == 2) {}` in the arm).
- **Colour-only address residual**: when an address that is a call's a0 argument lands in a2 in retail
  (`lui a2; addiu a0,a2`), the missing `.noalias` and the load placement follow from the colour; prove it with force.
- After the libultra renames, re-check every libultra call target in old drafts (osJamMesg/osSendMesg swapped).
