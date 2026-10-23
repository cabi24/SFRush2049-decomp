# w40b RESULTS (wave 40: state.c game-state machine, then _AnimateMine and controlscr animButtonText)

Branch wave37 @ dd4cf01d3. Builder scratch `watchman2:~/rush2049/scratch/frontier/w40b` (copied from `base`; trace
toolkit reused from wtk; a lane-instrumented umerge built in `w40b/umsrc`). Lane dir only; no git operations; nothing
written to `cloud/matches/` (no standalone match).

## Table

| function | state | best file | scorer (exact) | tried | next hypothesis |
|---|---|---|---|---|---|
| playgame_state_change (2544 B) = updateGameState@state.c | **MATCH** (group w40b_state, unit EQUAL) | groups/w40b_state/state.c | `score.py group cand/g/w40b_state --claims`: `playgame_state_change: MATCH` / `own .rodata verified at 0x80123FB4..0x80123FC0`, rc=0; unit `EQUAL playgame_state_change: 636 words (kept, c_state.c)`, `locked bodies that differ in this unit: 0` | DC text + N64 additions; umerge knapsack decoded; HAL_SetFont two-level form; four empty func_800C9150 calls; function-local `loaded` | - |
| object_create (112 B) = HAL_SetFont@HAL_text.c | **MATCH** (same group; was provisional in w38a) | groups/w40b_state/halfont.c | `object_create: MATCH`; unit `EQUAL object_create: 28 words (kept, c_halfont.c)` | MAT3 two-level form, helpers after it | - |
| func_800CA2F8 (8 B) = initGameState (deleted-procedure stub) | **MATCH** (same group) | state.c | `func_800CA2F8: MATCH`; unit `EQUAL func_800CA2F8: 2 words (internal, c_state.c)` | initGameState written non-static, not kept | - |
| func_800C9150 (8 B) = empty N64 helper (stub) | **MATCH** (same group) | state.c | `func_800C9150: MATCH`; unit `EQUAL ... 2 words (internal)` | 4 inlined calls | - |
| func_800B42E8 (8 B) = _GetSemaphore@HAL_text.c (stub) | **MATCH** (same group) | halfont.c | `func_800B42E8: MATCH`; unit `EQUAL ... 2 words (internal, c_halfont.c)` | - | - |
| func_800B41B8 (8 B) = _ReleaseSemaphore@HAL_text.c (stub) | **MATCH** (same group) | halfont.c | `func_800B41B8: MATCH`; unit `EQUAL ... 2 words (internal, c_halfont.c)` | - | - |
| display_enable (316 B) = LoadFrontEndOverlay@state.c | not matched; **context** in w40b_state (55/79 words; 10 with test-only local-static flags) | state.c (`display_enable`) | group context: `display_enable: 55/79 words differ (10 section-relative relocations unverified ...)`; unit `FAIL display_enable: 55 of 79 words differ`; test `pgs/de/bG.c`: `FAIL display_enable: 10 of 79 words differ` | extern / file-static / defined / volatile / struct / array flag forms (all form address webs); inlined MP_TargetSpeed/assign_default_paths; DeallocateMemory helper | overlay flags as function-local statics of a deleted overlay helper in overlay_loader.c (stub func_800A0F64), inlined into the four overlay wrappers and here; then PrevMaxPath's argument order |
| func_8010C974 (2636 B) = _AnimateMine@targets.c | 49 rows (from 66), frame exact | mine/best.c (= v2.c) | unit (us.sh): `best.c: FAIL func_8010C974: 51 of 659 words differ \| words 49 ops 5 norm 5 \| frame 392/392` | real locked ZOID/MBOX helpers (frame 320), helper nesting placements (v1 GetColor: 151 rows; v2 ObjectFlags/SetObjectFlags: 49; v3 flag wrapper: 333), loop index forms (v2a j outer: 48 rows but 661 words; v2b 50; v2c 52), force.sh on the j/pointer webs | retail j = a1 in all three loops with base a0 / walk v1: find the source form where j's webs are coloured after the pointer webs without v2a's j spill across math_utility |
| controls_animButtonText (1916 B) | 29 rows (unchanged; provisional anyway: caller unmatched) | ../w39b/ctl/best.c | unit: `best.c: FAIL controls_animButtonText: 39 of 479 words differ; compiled body is 478 words, target 479 \| words 29 ops 3 norm 3 \| frame 544/480` | locked TB_SetJust (func_800B669C) instead of ct_SetJust: `427 of 479 \| words 280 \| frame 544/488`; locked TB_DrawEllipseText (music_fade): `words 33 ops 7` | 64 B above screenPos are own-local space (inline reserves sit below a procedure's own locals, measured): an N64 local declared before screenPos |
| controls_animText (1576 B) | 151 rows (unchanged) | ../w36k/anim/best.c via w39b ctl/best.c | unit: `best.c: FAIL controls_animText: 365 of 394 words differ; compiled body is 390 words, target 394 \| words 151 ops 35 norm 35 \| frame 360/352` | rescored only | - |

Scorer setup: builder `cd ~/rush2049/scratch/frontier/w40b && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/g/w40b_state [--claims]`
(full output: `groups/score_w40b_state_claims.txt`, `groups/score_w40b_state.txt`, both rc=0). Unit (Pi):
`python3 -m tools.conveyor.pipeline.blob_unit --tag w40b score playgame_state_change object_create func_800C9150 func_800CA2F8 func_800B41B8 func_800B42E8 display_enable PrintScores --with cloud/work/frontier/w40b/groups/w40b_state/state.c --with cloud/work/frontier/w40b/groups/w40b_state/halfont.c --internal display_enable --internal func_800B41B8 --internal func_800B42E8 --keep object_create --internal func_800C9150 --internal func_800CA2F8 --neighbours`
(output `groups/unit_w40b_state.txt`): all six claims `EQUAL`, `EQUAL PrintScores: 396 words`, `FAIL display_enable`,
`locked bodies that differ in this unit: 0`. `us.sh` = `cloud/work/frontier/tools/trace/us.sh` with `TAG=w40b`.

Game code claimed: 2544 + 112 + 4 x 8 = 2,688 B.

## Integration note: group w40b_state (new group)

- Dir `cloud/work/frontier/w40b/groups/w40b_state/`. Supersedes no installed group.
- Members = claims: playgame_state_change, func_800CA2F8, func_800C9150, object_create, func_800B42E8, func_800B41B8.
- Files: `state.c` (members playgame_state_change, func_800CA2F8, func_800C9150; context display_enable), `halfont.c`
  (members object_create, func_800B42E8, func_800B41B8), and byte-identical context copies: `ctx_heap_group.c`,
  `ctx_alloc_at.c`, `ctx_overlay_loader.c` (= src/blob/groups/codex_heap_release_a25/{group,alloc_at,overlay_loader}.c:
  PrevMaxPath's IPA s0-s4 parameters, audio_reverb_update's a1/a2, sync_maxpath_to_checkpoint inlined into
  display_enable), `ctx_vsync.c` (= codex_vsync_a145/group.c: speed_set's f20/f22/s1/s2 parameters, the two AudSet*Vol
  wrappers umerge inlines), `ctx_func_800B5570.c` (= src/blob/func_800B5570.c, setGameState, inlined), `ctx_gr3_w.c`
  (= credits_scroll_grp/gr3_w.c: HAL_StartText, a second real caller of slot_state_setup; without it slot_state_setup
  has one caller in the group and umerge inlines it everywhere). One derived copy, DISCLOSED: `ctx_slot_state_setup.c`
  = credits_scroll_grp/gr3_b.c cut before its `object_create` (direct form) and func_800B669C, because object_create
  is a member here.
- Stub singles to revert: `src/blob/func_800C9150.c`, `func_800CA2F8.c`, `func_800B41B8.c`, `func_800B42E8.c`
  (locked `void f(void) {}` singles; the group gives them their real roles).
- Unit overrides:
  - prefer_definition object_create -> groups/w40b_state/halfont.c: about twenty installed groups carry the direct-form
    gr3_b.c copy as context; this is the retail kept body and the form whose umerge size gives retail's inlining.
  - force_keep object_create (if not derived from this group's keep list): retail calls it by jal from
    playgame_state_change; kept, PrintScores still inlines it (unit EQUAL, measured).
  - force_internal func_800C9150, func_800CA2F8, func_800B41B8, func_800B42E8 (if not derived once the singles are
    reverted): deleted-procedure stubs; kept they would not be inlined.
  - display_enable: internal (derived: defined by a locked group file, not kept). No inline_blockers, no align, no
    extern_data.
- Caveats: display_enable is context only (not code-identical; its body supplies the s0-s4 clobber set that
  playgame_state_change saves). state.c's own .data is display_enable's function-local `loaded` (retail 0x801174C0,
  value 0), referenced only by that context body. Retires w38a's provisional object_create entry.
- Disclosed shaping in state.c: the four func_800C9150 call positions (not determined by the bytes); the Dreamcast
  text's two compiled-out `if (p > 1) {}`.

## How playgame_state_change closed

1. **umerge's inline decisions, decoded** with a lane-instrumented umerge (`tools/patch_umerge.py` adds stderr prints to
   `pack_knapsack`; `tools/umdbg.sh` reruns it with `-debug` on the last unit stage and checks the merged output is
   byte-identical to the stock run: FIDELITY_OK). IDO 5.3 umerge, defaults bmax 500, space_time 3.0:
   - per procedure: `+32` = u-code instruction count; `+28` = size measure = max(+32 >> 5, s6), s6 counting calls and
     branch-like ops (initGameState: 829 instructions, +28 = 224).
   - per edge: benefit B = freq x (4 + param bytes / 4); size increase S = callee +32 - 4 - param bytes / 4.
   - loop 1 over all edges (within a caller in descending source line; a callee's own edges first): inline at once when
     the threshold allows and S <= 0 or the callee has a single caller.
   - loop 2 over the remaining (multi-caller) edges, same order: inline iff `C + c <= min(500, 62 * sqrt(C/c))`
     (C, c = caller/callee +28; C grows by c with every inline) and `S * max(1, C >> 7) / B <= 3.0` (S multiplied by
     the caller's inlined-copy count + 1 when the caller is inlined at some of its sites only).
   - `umerge -debug` prints `inline X in Y at line N, -B cycles, S instructions` for every accepted edge.
   The "per-caller budget" seen by earlier lanes is the `C >> 7` factor: costs double once the caller reaches 256.
2. **HAL_SetFont (object_create) is the MAT3 two-level form** with its helpers after it: S = 9, B = 5. Visiting the 13
   font sites from the last, fonts 7, 6, 5 are inlined while C < 256 (cost 1.8); the third takes C to 257 and every
   later candidate costs double (fonts 13..8: 3.6, InitMaxPath 5.6, display_enable 5.2) and stays a jal: retail's
   pattern. The direct osRecvMesg form (S = 21, cost 4.2) is inlined nowhere and, kept, made PrintScores lose its inline.
3. **C must be 232..239 before the fonts**, and the frame needs 32 more bytes: four calls of the empty helper
   func_800C9150 do both (each call adds 1 to C and the inlined callee 1 more; each reserves 8 bytes above the font
   homes). Frame 232, every home retail's (fonts 132/108/84, `i` 220). The helper's stub lies right before
   player_state_set, i.e. its last call is at the end of state 0x80.
4. **initGameState is func_800CA2F8**: a static inlined into its only caller still leaves a stub, placed by the
   post-order layout after its subtree's last new callee (init_state_begin) and before uninitGameState.
5. **LoadFrontEndOverlay's `loaded` is a function-local static.** Extern or file-static, uopt forms an address web for
   it and colours it s5 in this internal procedure (callee-saved registers cost 0 there); the caller then saves s5.

## display_enable: what blocks it
- Our body is right (Extra unload: lock / internal DeallocateMemory `audio_reverb_update((u32)D_8038A400, 0)` /
  unlock / clear; Start load: sync_maxpath_to_checkpoint inlined; else Start unload), frame 40.
- Residual 1, address webs: uopt colours &D_8011ED04 and &D_8011ED00 into s3 (tot 1.0 > cost 0). Micro tests
  (`pgs/micro/`: single-file and two-file groups) show the web forms for an extern, a defined global (with or without
  initialiser), a file-static (also of another file, inlined), a volatile, a struct member, an array element and a
  block-scope extern, and never for a function-local static, even when its address is passed to a call in the same
  procedure. With both flags as function-local statics (`pgs/de/bG.c`, test only: wrong data addresses) display_enable
  is `10 of 79 words differ`.
- Residual 2: PrevMaxPath's argument setup order (retail name, romStart, &loaded s3, end a2, rom s4, dst s0, bssStart
  s2, bssEnd s1) differs from both the out-of-line sync_maxpath_to_checkpoint and our inlined copy of it.
- Tested and rejected here: one `__inline` overlay helper with both flags as its function-local statics, called by the
  four wrappers and display_enable (`ovl/o1.c`, `ovl/state_o1.c`): umerge inlines it nowhere (`__inline` does not set
  the force-inline flag; its size fails the cost test), display_enable 30 words.
- Lead: D_8011ED00/D_8011ED04 (overlay_loader.c .data, 4 bytes apart) are function-local statics of one deleted overlay
  helper (the caller-less stub func_800A0F64 just before MP_TargetSpeed) inlined into InitMaxPath,
  sync_maxpath_to_checkpoint, MP_TargetSpeed, assign_default_paths and LoadFrontEndOverlay. Landing display_enable means
  superseding codex_heap_release_a25's overlay_loader.c with that helper (the kept wrappers stay code-identical: in
  kept procedures the webs are never coloured) and giving that group the flags' .data. Not attempted here.

## _AnimateMine (func_8010C974): 66 -> 49 rows, frame exact
- The 16-byte frame gap is two umerge inline reserves. Measured rule (micro `pgs/micro/fr2..fr4`): an inlined call
  reserves 8 bytes plus the callee's own locals rounded to 8 (parameters excluded, kept or internal alike), placed
  below the caller's own locals, the callee's reserve at the top of its area; a procedure with no local area gets none.
- With the real locked helpers (MBOX_NewObject = sign_extend_call, ZOID_SetColor = func_8008E06C, ZOID_RemoveObject =
  sound_call_minimal) the frame drops to 320: retail's helpers are the MAT3 shapes (MBOX_NewObject with its `dlidx`
  local, ZOID_RemoveObject -> ZOID_RemoveObjectTree), as w39b had them.
- v2 (`mine/best.c`): ZOID_ObjectFlags / ZOID_SetObjectFlags (the N64-only 0x2000 flag line) one level deeper ->
  `words 49 ops 5 | frame 392/392`. Which chain carries the 16 bytes is not determined (ZOID_GetColor nested: 151 rows).
- Residual: retail j = a1 in all three loops (oldest-mine search, uvs copy, scale loop outer index) with base a0 and
  walk pointer v1; ours j v1. v2a (scale loop with j outer, k inner) puts base/walk in a0/v1 as retail but j in a2 and
  j's init is spilled across math_utility (661 words). force.sh: j cannot take a1 there (forbidden 0x7a000000).
  A block boundary between math_utility and the loop (`do {} while (0);`, `if (i) {}`, `j = 0;` split: v2d/v2e/v2f)
  leaves the spill: `words 48 ops 8 | frame 392/392`, 661 words, all three.

## controls_animButtonText / controls_animText
- Unchanged from w39b (29 / 151 rows). New fact for the 64 B: inline reserves sit below a procedure's own locals
  (micro fr4), so retail's 64 bytes above screenPos (top-88) are own-local space declared before screenPos, not an
  inline chain. MAT3/DC declare nothing there; an unused 64-byte local stays a device (w39b probe64), not delivered.

## What generalises
1. **umerge's knapsack is computable.** Per multi-caller edge: inline iff `S * max(1, C >> 7) / B <= 3.0` and
   `C + c <= min(500, 62 * sqrt(C/c))`; S = callee instructions - 4 - param words, B = 4 + param words, C/c = the +28
   size measure (instructions/32 or the call/branch count, whichever is larger), edges visited from the last source line.
   A "last N calls inlined" pattern is the caller's +28 crossing 256 (or 384, ...) during the font-like run; tune C
   before that run, not the callee. `w40b/tools/patch_umerge.py` + `umdbg.sh` print every number.
2. **Smaller kept wrappers inline, direct ones do not**: MAT3's two-level HAL_SetFont (helpers defined after it) is
   both the retail kept body and the only form umerge inlines; the same likely holds for other helper pairs.
3. **Empty inlined calls are frame and budget**: each costs 8 bytes of frame (when the caller has locals) and +2 of the
   caller's umerge size. Caller-less stubs whose layout position is a loading-state hook are such helpers.
4. **A static inlined into its only caller is a stub at the end of its own post-order subtree** (initGameState =
   func_800CA2F8, just before uninitGameState).
5. **Function-local statics never get uopt address webs**; every other global form does in an internal procedure
   (callee-saved registers cost 0 there). A flag a retail internal procedure accesses with plain lui/lb ... lui at/sb is a
   function-local static (or that of an inlined helper).
6. **Inline reserves sit below the procedure's own locals**; unexplained bytes ABOVE the first named local are own
   locals, not inline chains.

## Tools (lane)
- `tools/patch_umerge.py IN.c OUT.c`: instruments the recompiled umerge.c (ido-static-recomp) `pack_knapsack`.
  Built on the builder: `w40b/umsrc/umerge` (gcc -Os, libc_impl.o from wtk). `tools/umdbg.sh OUT.log` reruns it with
  `-debug` on the last `--tag w40b` unit stage and checks fidelity.
- `tools/de.sh BODY.c`: display_enable variant harness. `tools/{gs,gd,sc,tc}.sh`: w39c-style builder wrappers.

## Permission denials
None.
