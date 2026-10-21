# w37b — wave 37 menu near-misses (game code, whole-program unit)

Status: FINAL (short wave). New coverage ready to install: **replay_save_prompt 1,540 B** (group packet
`groups/frontier_optionscr_buttons/`, superseding in place). Near-miss progress: particle_lifetime_set 65 -> 51 words.

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w37b` (from base c12837976). Pi blob_unit tag: `w37b`.

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| replay_save_prompt | initScreen@optionscr.c (proven) | 1540 | **code-identical in group** (superseding `frontier_optionscr_buttons`); unit EQUAL; splice-proof replay 4/4 image-identical | groups/frontier_optionscr_buttons/option_init.c |
| drone_collision_avoid | calcMenuRect@audioscr.c (proven) | 852 | 5 words (unchanged; frame/area order analysed, see below) | cloud/work/frontier/w36q/audio_init/rect_best.c (= w37b rect/v/a0.c without the stale drone_pathfind_main copy) |
| particle_lifetime_set | createBG@front.c (proven) | 732 | 51 words (from 65): D_801391E4 as a function-local static | cloud/work/frontier/w37b/cbg/best.c |
| controls_animText | animText@controlscr.c | 1576 | not reworked (w36k 151 rows, frame 352/360) | cloud/work/frontier/w36k/anim/best.c |
| controls_animButtonText | animButtonText@controlscr.c | 1916 | not reworked (w36k 49 rows, frame 440/544); retail stack census below | cloud/work/frontier/w36k/anim/v/b3.c |
| func_8010F218 / func_8010EA08 | videoscr animText/animButtonText | 4500 | not reworked (time) | cloud/work/frontier/w33b/videoscr/best.c |

## Log

### replay_save_prompt — DONE (group packet `groups/frontier_optionscr_buttons/`, supersedes in place)
- Unit (Pi): `blob_unit --tag w37b score replay_save_prompt func_800DB1E0 func_800DA2C0 func_800DA0BC --with
  cloud/work/frontier/w37b/groups/frontier_optionscr_buttons/option_init.c --internal replay_save_prompt --internal op_gfx_lock
  --internal op_gfx_unlock --internal op_HAL_SetFont --internal op_setButtonColor --internal op_ZOID_SetColorP --internal
  op_calcOptionMenuMaxSize --internal op_initButtons --neighbours` -> `EQUAL replay_save_prompt: 385 words (internal,
  c_option_init.c)`, `EQUAL func_800DB1E0: 350 words (kept ...)`, `EQUAL func_800DA2C0: 579 words (internal ...)`, `EQUAL
  func_800DA0BC: 46 words`, `locked bodies that differ in this unit: 0`, `blob_unit score: 4/4 equal`.
- Builder `score.py group cand/frontier_optionscr_buttons`: func_800DB1E0 `MATCH` (own .rodata verified at
  0x80124248..0x80124280), func_800DA0BC `MATCH`, func_800DA2C0 its known two-region `NOT VERIFIED (32 section-relative
  relocations unverified ...)` line (same text as the installed group except the section-relative offsets: .rodata+0x34
  vs +0x18, because option_init.c's literals now share the merged .rodata), replay_save_prompt `NOT VERIFIED (16
  section-relative relocations unverified: .data+0x0 at +0x1fc, .data+0x0 at +0x200, .rodata+0x18 at +0x300, ... own
  .rodata: this function's references disagree on the section's image address (0x80121014 for +0x18/+0x20/+0x24,
  0x801241F0 for +0x48/+0x4c/+0x50/+0x54): the literals are not laid out as in retail)` + `own .data verified at
  0x80116DAC (4 bytes ...)` — strings CURSOR/B1/BUTTON_TITLE and the floats are two retail regions (particle_collision /
  func_800B59F0 precedent). Context: sign_extend_call MATCH, func_800B8810 MATCH, slot_state_setup 2/58, object_create
  28/28, func_800B669C 5/5 (the ctx_gr3_b.c lines every group using that copy shows).
- Splice-proof replay (`tools/gproof.py`, read-only copy of w36q's; `groups/proof_frontier_optionscr_buttons.log`):
  `group_bodies OK: func_800DA0BC, func_800DA2C0, func_800DB1E0, replay_save_prompt`; every member `0 differ from the
  image`; `4/4 members image-identical`.
- Integration: install_group.py cloud/work/frontier/w37b/groups/frontier_optionscr_buttons frontier_optionscr_buttons
  (supersede in place). members += replay_save_prompt; claims = [replay_save_prompt]; files += option_init.c,
  ctx_sign_extend.c (byte copy of dot_sign_extend/group.c), ctx_func_800B8810.c (byte copy of src/blob/func_800B8810.c),
  ctx_gr3_b.c (byte copy of frontier_pausescr_state/ctx_gr3_b.c); context += sign_extend_call, func_800B8810,
  slot_state_setup, object_create, func_800B669C; keep += sign_extend_call, func_800B8810. unit_overrides force_internal
  (if not derived): replay_save_prompt, op_gfx_lock, op_gfx_unlock, op_HAL_SetFont, op_setButtonColor, op_ZOID_SetColorP,
  op_calcOptionMenuMaxSize, op_initButtons.
- Device (disclosed in the header): one unused local `u0` = exact frame residual (248 -> 256; every sp offset already
  retail's). w36q's `volatile s8 attached` device is no longer needed (dropped).
- Path: w36q best 7 words -> helpers as real functions (e1/e2/f1: umerge refuses both) -> f2 (real object_create as
  HAL_SetFont: both helpers inline, object_create not, PrintScores breaks) -> f5 (local object_create copy: same, no
  neighbour break) -> f6 (MAT3 HAL_SetFont shape with nested _GetSemaphore/_ReleaseSemaphore helpers: knapsack-inlined
  after the helpers) 5 words, frame 240 -> f7 (+ZOID_SetColor nested level as updateButtons) 2 words (frame 248) -> g1
  EQUAL (+u0) -> g4 (volatile attached dropped) EQUAL.
- umerge facts measured here (`umerge -v -debug` on the stage `split`): single-caller helpers are decided per edge by
  size; a multi-caller helper of zero growth (w36q op_SetFont = a call to op_font_set) is inlined immediately, growing
  the caller/callee before the single-caller decisions; a multi-caller helper with positive growth waits for the
  knapsack pass. So the font-helper SHAPE decides whether calcOptionMenuMaxSize/initButtons inline.

### drone_collision_avoid — 5 words (unchanged), area analysis
- `rect/v/a0.c` (w36q rect_best.c minus its stale drone_pathfind_main/di_ copy; drone_pathfind_main is installed):
  `FAIL drone_collision_avoid: 5 of 213 words differ` (the menuIdx IV home sw/lw at sp+28 vs retail sp+32).
- spill.sh: merged Udef (`AREA f_readnxtinst`) is 112 in a0; retail's IV home at -112 means retail Udef 108. Measured:
  a0 (3-level du_SetFont, `du_validOption(s32)`, u0) 112; a1 (MAT3 HAL_SetFont shape: 2 levels) 104; a2 (a1 without u0)
  96; a4 (a0 with K&R `du_validOption()`) 104; a7 (shared frontier_audioscr `au_SetFont`) = a0, 0 neighbours broken;
  a8 (`au_font_set` directly, knapsack-inlined) 104 / frame 136; a6 (real object_create) not inlined + PrintScores
  breaks. Every lever moves Udef by 8; retail's 108 needs a 4-byte step (an unaligned last area), not found.
- The replay_save_prompt lever (font helper shape/knapsack order) does not transfer: here every helper is
  single-caller (direct) except the shared ones, and the area order stays SetFont-first.

### particle_lifetime_set (createBG) — 51 words (from 65)
- `cbg/best.c` (= cbg/v/s1.c): w36q best.c with D_801391E4 (referenced only by this function: w36w3 `xref.py
  0x801391E4` -> `particle_lifetime_set`) as a function-local `static s32` set to 1025 (N64 BG_DIST source; GC
  `#define BG_DIST 16777000.0f`). Unit: `FAIL particle_lifetime_set: 51 of 183 words differ; .bss+0x3c78 is placed at
  different image addresses: 0x801391e4 ..., 0x80140044 ...` (the second address is the misaligned row, not a layout
  problem: the static's own words line up as 0x801391E4 wherever the rows align).
- Residual, two causes only (everything else identical, s-registers now as retail): (1) as1 places the static store
  `sw t6,-28188(at)` before `sw ra,68(sp)` in retail, after it in ours (2 rows + shifted addiu rows); (2) from +0x104 the
  ugen temp ring is shifted by two (retail `lw t3,17876(t1); li t5,1; ori t4,t3,0x1210`, ours `lw t1 ...; li t3,1; ori
  t2,t1 ...`) through the rest of the loop: in retail t1/t2 are still busy at the gBackCamera load. ugt.sh trace saved
  in build/trace/w37b/ugt_s1.txt (not in repo).
- Tried without change: `(u16)` cast of the flags argument, u32/u16 gBackCamera, `0x1210 | x` order, s32 flags
  parameter (all 51). MAT3/Dreamcast function-scope backPoly/vertIdx: 52 (frame slots move); keep the block locals.

### controls_animButtonText — not reworked; retail stack census (for the next lane)
- Retail (`tdis.py controls_animButtonText`) frame 544: the only memory slots are screenPos at sp+456 (top-88), spill
  homes sp+56/60/68, and the two TB_FlashTextColors parameter-home pairs sp+88/92 and sp+104/108. Nothing is stored in
  the 68 B above screenPos (top-20..top-88), so they are cfe slots of never-stored locals or inline areas. Hypothesis
  (untested, from the replay_save_prompt result that a helper's locals sit below the areas of helpers inlined before
  it): the 68 B are inline areas above the locals of an inlined helper that owns screenPos, not a dead char[64].

## Research receipts
- Landing replay_save_prompt changes the slot-contract audit (`cloud/work/frontier/dot_slot_contract_audit_20261006`,
  tests/cloud/test_slot_contract_audit.py): replay_save_prompt is a selector caller listed `"accepted": false` (4
  sites 0x800DAC08/0x800DAC48/0x800DB0F8/0x800DB144). Once locked, `accepted_callers` gains replay_save_prompt and
  `unaccepted_caller_bytes` drops by 1,540 (17,664 -> 16,124), so test_counts_are_bounded_dependency_surface_not_matches
  and the summary comparison in test_fresh_source_bound_census need the integrator's re-pin patch (as for the
  w36w2/w36w3 selector callers). Not edited here.

## Integration notes
- Group `frontier_optionscr_buttons` superseded in place (`cloud/work/frontier/w37b/groups/frontier_optionscr_buttons/`):
  old members func_800DB1E0, func_800DA2C0, func_800DA0BC unchanged (buttons.c byte copy); new member/claim
  replay_save_prompt (option_init.c); new context files are byte copies (ctx_sign_extend.c = dot_sign_extend/group.c,
  ctx_func_800B8810.c = src/blob/func_800B8810.c, ctx_gr3_b.c = frontier_pausescr_state/ctx_gr3_b.c); keep +=
  sign_extend_call, func_800B8810; context += sign_extend_call, func_800B8810, slot_state_setup, object_create,
  func_800B669C. unit_overrides force_internal (if not derived): replay_save_prompt, op_gfx_lock, op_gfx_unlock,
  op_HAL_SetFont, op_setButtonColor, op_ZOID_SetColorP, op_calcOptionMenuMaxSize, op_initButtons.

## Identities proven
- replay_save_prompt = initScreen@optionscr.c with calcOptionMenuMaxSize and initButtons umerge-inlined (EQUAL);
  N64 HAL_SetFont (object_create) inlines with the MAT3 HAL_text.c shape: _GetSemaphore/_ReleaseSemaphore nested.
- D_801391E4 is createBG's function-local static (sole reference; N64 BG_DIST source = (f32)1025-int).

## What generalises
- **The font helper's SHAPE decides which other helpers umerge inlines.** A multi-caller helper with zero growth (a
  void wrapper whose body is one call, e.g. w36q's op_SetFont -> op_font_set) is inlined immediately, enlarging the
  caller and the helpers before umerge's per-edge size test for single-caller helpers (calcOptionMenuMaxSize,
  initButtons) runs, so those stay calls. A multi-caller helper with positive growth (MAT3 HAL_SetFont: lock; call;
  unlock with the lock/unlock as nested helpers) waits for the knapsack pass, after the single-caller helpers are in.
  `umerge -v -debug` on the unit stage's `split` prints every procedure's size and each inline decision in order
  (run it in your own scratch: copy `split` and `st` from `~/rush2049/scratch/frontier/unit/<tag>/stage`).
- **Inline areas of helpers inlined earlier sit above the locals of helpers inlined later**: when retail has a local
  of an inlined helper deep in the frame (buttonColor 100 B below the top), write that helper as a real function and
  get umerge to inline it, instead of writing it in place (in-place block locals are cfe locals at the top).
- **Nested inline levels are 8 B each**; the same GC file's matched siblings tell you how deep a helper is
  (optionscr updateButtons: ZOID_SetColor with one nested level; the same shape closed initScreen's frame).
- **A real kept multi-caller helper (object_create) added at new sites is usually refused by the shadow unit's
  knapsack and breaks other functions' inlining (PrintScores)**; a local copy of the same shape is not refused.
- **Single-function globals are function-local statics** (w36w3 rule) held again: createBG 65 -> 51 words.

## Permission denials
None.
