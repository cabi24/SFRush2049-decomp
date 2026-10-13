# w30b RESULTS (wave 30, menus / options / pause, IDO 5.3 -O3)

Status: FINAL (wave 30). Builder scratch `watchman2:~/rush2049/scratch/frontier/w30b` (deleted at the end); unit tag `w30b`.
Starting drafts: `w29b/<fn>/best.c`, `w29b/func_8010AEAC/ct_centertext.c`, `w28c/func_80108AB0/{best,gc_named}.c`,
`w10e/func_800DA2C0/best.c`.

| Function | Bytes | Score before -> after | draft_check | Residual | Deliverable |
|---|---:|---|---|---|---|
| func_8010DCFC (= GC _StartCone) | 660 | unit 147/165 (w29b) -> **MATCH** (`score.py fn`) + unit EQUAL | EQUIVALENT (w29b) | none | `cloud/matches/func_8010DCFC.c` (5 unused locals, disclosed) |
| func_8010B5D0 | 556 | unit 2/139 (w29b) -> **MATCH in group** + unit EQUAL | EQUIVALENT (w29b) | none | packet `w30b/groups/func_8010B5D0/` (new group) |
| func_8010AEAC / func_8010A8D0 / func_8010A7A4 (= GC animText / animButtonText / printOption) | 1,652 + 1,500 + 300 | unit 290/413, 241/375, 16/75 (w29b) -> **group**: A8D0 MATCH, A7A4 MATCH, AEAC code-identical (NOT VERIFIED = own .rodata in two regions); unit 3/3 EQUAL | EQUIVALENT (w28c) | none | packet `w30b/groups/func_8010AEAC/` (new group) |
| func_80108AB0 (= GC animWrongWay) | 760 | unit 184/190 (w28c) -> **MATCH in group** + unit EQUAL | EQUIVALENT (w28c) | none | packet `w30b/groups/credits_scroll_grp/` (supersedes the locked group) |
| func_800DB1E0 (= GC updateOptionScreen) | 1,400 | never drafted -> unit **EQUAL** with func_800DA2C0's draft internal (provisional: lands only with DA2C0) | **EQUIVALENT** 961 calls (unit pair, options_menus) | none (blocked on DA2C0) | `w30b/func_800DB1E0/best.c` (pair file) + `dc.args` |
| func_800DA2C0 (= GC updateButtons@optionscr.c) | 2,316 | 494/579 ops 82 (w10e) -> 489/579 ops 78, frame 152/160 | EQUIVALENT (inside the pair) | structure + colour; still has w10e pad arrays | same pair file |
| func_8010E8B4 | 340 | unit 3/85 -> 3/85 (32 variants + listing oracle) | EQUIVALENT (w29b) | narrowing register: y must be a web separate from h (as1 proof below) | `w30b/func_8010E8B4/best.c` (= w29b best) |
| func_8010EA08 + func_8010F218 | 2,056 + 2,444 | not attempted (time) | — | — | identity note below |

Matched bytes this lane: func_8010DCFC 660 + func_8010B5D0 556 + AEAC unit 3,452 + func_80108AB0 760 = **5,428 B** (6 functions).

## func_8010DCFC = GC _StartCone — MATCH (660 B), `cloud/matches/func_8010DCFC.c`
- Builder: `score.py fn cand/func_8010DCFC.c func_8010DCFC --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `func_8010DCFC:` / `MATCH`.
- Unit: `blob_unit --tag w30b score func_8010DCFC --with cloud/matches/func_8010DCFC.c --neighbours` -> `EQUAL func_8010DCFC: 165 words (kept, c_func_8010DCFC.c)`, `locked bodies that differ in this unit: 0`, `1/1 equal`.
- Integration: single (splice_singles); no unit_overrides, no data-symbol changes.
- Path (unit rows): 88 (w29b) -> 48 `vel = t->motion->vel;` computed from the target BEFORE `angV = t->motion;` (with vel
  derived from angV uopt folds the vel stores into angV+12 and tInfo loses t0; this fixed tInfo t0 / car a3 and the
  second id-236 branch shape, which were the colouring the brief named) -> 28 lap-flag helper takes the record
  pointer `Flag1600(&D_8014A250[car])` -> 19 tVisual stores in the order index, func, data, timeStamp -> 11 angular
  velocity stores in GC order 0, 12, 15 (FP ring) -> 9 GrabEnvEntry/AddToEnvList NOT helpers (one inline area left)
  -> EQUAL with the named-slot layout measured by spcensus.
- The three "frame helpers" of w29b were checked against GC: GC calls GrabEnvEntry/AddToEnvList as functions and
  has the lap test-and-set inline; on N64 only the lap-flag helper survives (8-byte inline area at 32..39).
- Shaping (in the header): Flag1600 inlined static of unknown identity; FIVE unused locals u1..u5 = exact frame
  residual (retail 136, 112 without them, 17 words differ; homes vis 132, tmat 84..131, u1..u3 72..80, vel 68,
  angV 64, car 60, u4/u5 52..56, tInfo 48 = retail's spill stores). Decl-order tests (o1..o5, k0..k7) show the homes
  follow declaration order once vel/angV are real locals.
- GC names (from _StartCone, targets.c): tTarget (Obj), tVisual (Node: next, index +4, data +12, timeStamp +16,
  func +20), targetInfo = D_80117530 (stride 48: f12 = func, f28 = sound, id +20), gCarMines = D_8013FECD,
  gMineCones = D_8013FECC, MBOX_HideObject = model_data_load, StartBlast = save_write_data, GrabEnvEntry =
  func_80090284, LookInDir = vector_normalize_length, CopyUV = math_utility, PlayTargetSound = stat_lap_split,
  env list head D_801391F0, GC model[] stride 0x808 = D_8014A250 (byte +1600 = 0x640).

## func_8010B5D0 — MATCH in group (556 B), packet `w30b/groups/func_8010B5D0/`
- Files: `fb5d0.c` (helpers non-static `*_B5D0`), `ctx_gr3_b.c` (byte-identical copy of credits_scroll_grp/gr3_b.c);
  members/claims/keep = func_8010B5D0; context slot_state_setup, object_create, func_800B669C. Draft with static
  helpers: `w30b/func_8010B5D0/best.c`.
- Builder: `score.py group cand/groups/func_8010B5D0` -> `func_8010B5D0:` / `MATCH`, exit 0; `--claims` -> MATCH, exit 0.
- Unit: `blob_unit --tag w30b score func_8010B5D0 --with cloud/work/frontier/w30b/groups/func_8010B5D0/fb5d0.c
  --internal gfx_lock_B5D0 --internal gfx_unlock_B5D0 --internal font_set_B5D0 --internal item_refresh_B5D0
  --internal TB_CenterText_B5D0 --neighbours` -> `EQUAL func_8010B5D0: 139 words (kept, c_fb5d0.c)`, `locked bodies
  that differ in this unit: 0`; best.c (static helpers, no --internal) -> `EQUAL ... (kept, c_best.c)`, 0 differ.
- **unit_overrides**: force_internal += gfx_lock_B5D0, gfx_unlock_B5D0, font_set_B5D0, item_refresh_B5D0, TB_CenterText_B5D0.
- Lever: the lock/font helpers written on ONE source line. With one inlined font_set site umerge keeps the helpers'
  own `.loc` lines and as1 orders font_set's `sw v0,68(sp)` against gfx_unlock's argument moves by line (helpers
  after the function: sw first; before: sw last, in the jal delay slot). On one line the tie falls back to ugen order
  (`lui/addiu a0`, `sw v0,68(sp)`, `move a1`, jal, `move a2` = retail). Nested font_select instead: frame +8 and wrong.
- Shaping: unused `s32 i` = frame residual (152; 144 without it, 16 words differ). GC identity: none confident.

## func_8010AEAC / func_8010A8D0 / func_8010A7A4 — group `w30b/groups/func_8010AEAC/`
- Files: `faeac.c` (helpers non-static `*_AEAC`), `ctx_gr3_b.c` (byte-identical credits_scroll_grp/gr3_b.c);
  members = claims = the three; keep = func_8010AEAC only (A8D0/A7A4 internal); context slot_state_setup,
  object_create, func_800B669C. Draft with static helpers: `w30b/func_8010AEAC/best.c` (+ `dc.args`).
- Builder `score.py group cand/groups/func_8010AEAC` (exit 1 only because of AEAC's owndata line):
  `func_8010AEAC: NOT VERIFIED (4 section-relative relocations unverified: .rodata+0x0 at +0xd0, .rodata+0x0 at +0xe8,
  .rodata+0x5c at +0x208, .rodata+0x5c at +0x210; own .rodata: this function's references disagree on the section's
  image address (0x80121018 for +0x0, 0x801248C0 for +0x5c): the literals are not laid out as in retail)`,
  `func_8010A8D0: MATCH` (`own .rodata verified at 0x801248D8..0x8012491C`), `func_8010A7A4: MATCH`
  (`own .rodata verified at 0x801248D4..0x801248D8`). AEAC has no word diffs: the string "CONTROLLER %d %s"
  (0x80121018) and its jump table (0x8012491C) are two retail .rodata regions = the known owndata one-base case ->
  install via blob_group.
- Unit: `blob_unit --tag w30b score func_8010AEAC func_8010A8D0 func_8010A7A4 --with .../groups/func_8010AEAC/faeac.c
  --internal func_8010A7A4 --internal func_8010A8D0 --internal gfx_lock_AEAC --internal gfx_unlock_AEAC --internal
  font_set_AEAC --internal TB_FlashTextColors_AEAC --internal TB_SetJust_AEAC --internal TB_CenterText_AEAC
  --neighbours` -> `EQUAL func_8010AEAC: 413 words (kept, c_faeac.c)`, `EQUAL func_8010A8D0: 375 words (internal,
  c_faeac.c)`, `EQUAL func_8010A7A4: 75 words (internal, c_faeac.c)`, `locked bodies that differ in this unit: 0`,
  `3/3 equal`. Same with best.c + the two `--internal` (A7A4 then prints the static-helper stub tail, as expected).
- **unit_overrides**: force_internal += func_8010A7A4, func_8010A8D0 (without them both stay kept/o32 and the unit
  fails: `0/3 equal`), gfx_lock_AEAC, gfx_unlock_AEAC, font_set_AEAC, TB_FlashTextColors_AEAC, TB_SetJust_AEAC,
  TB_CenterText_AEAC. Data: the file declares D_801146BC as `Color D_801146BC[2]` (GC flashColors[2]); no definition.
- Levers: (1) **TB_FlashTextColors takes the GC colour union by value** (`union { u8 c[4]; u32 rgba; }`, GC
  __anon_21da; body copies `.rgba`, callers pass flashColors[0]/[1]): by-value aggregates have stack homes, so
  retail's `sw t8,40(sp)/sw t9,44(sp)` are the inlined parameter homes and the values stay in ring temps
  (A7A4 16 -> 0; A8D0 frame 160 -> 152). Union-to-union assignment compiles to an `at` block copy instead.
  (2) **`case 0:` written last** in both switches (retail code order): A8D0 -> EQUAL, AEAC ops 0. (3) AEAC reads
  gGameState into a named local `gs` (retail v0); an inlined getter gives v0 too but puts its 8 B under str1.
- Shaping: AEAC `s32 u0` unused = frame-layout residual (frame 240 either way; without it str1/itemsDrawn sit at
  180/222 instead of 176/218, 5 words). Helpers as listed in the header.
- GC names (optionscr.c / TB_text.c): animText, animButtonText, printOption(x, y, item, name, value),
  TB_FlashTextColors(color1, color2), flashColors = D_801146BC[2], gFlashColor1/2 = D_80118E28/2C.

## func_80108AB0 = GC animWrongWay — MATCH in group (760 B), packet `w30b/groups/credits_scroll_grp/` (supersede)
- The packet is the locked credits_scroll_grp with every file byte-identical (checked with cmp) and every old member
  kept, plus `gr3_8ab0.c`; members/keep += func_80108AB0; `claims` = func_80108AB0; provenance appended. Same flags
  (`-Wab,-r4300_mul`). A single-member group failed (`165/190`): TB_SetJust's two stores share one `lui at` only
  when D_80118E20 is defined in the compile, and gr3_d.c defines it (w27c) — hud.c siblings live in this group anyway.
- Builder: `score.py group cand/groups/credits_scroll_grp` -> exit 0; Members: `credits_scroll: MATCH`,
  `func_80100B8C: MATCH`, `slot_state_setup: MATCH`, `world_trigger_check: MATCH`, `func_8010A53C: MATCH`,
  `func_80107EDC: MATCH`, `animLapTime: MATCH`, `animElapsedTimerText: MATCH`, `func_80108AB0: MATCH` (`own .bss
  placed at 0x8013F2F8 (1 bytes; zero-initialised: verified by address only, nothing to compare)`, `own .rodata
  verified at 0x801248C4..0x801248CC`). `--claims` -> `func_80108AB0: MATCH`, exit 0.
- Unit: `blob_unit --tag w30b score func_80108AB0 --with .../groups/credits_scroll_grp/gr3_8ab0.c --internal
  gfx_lock_8AB0 --internal gfx_unlock_8AB0 --internal font_select_8AB0 --internal font_set_8AB0 --internal
  TB_SetJust_8AB0 --internal TB_GetFlashTextCycle_8AB0 --internal HAL_SetFont_8AB0 --neighbours` -> `EQUAL
  func_80108AB0: 190 words (kept, c_gr3_8ab0.c); own zero-initialised data (addresses consistent, inside game .bss,
  no overlaps; nothing to compare)`, `locked bodies that differ in this unit: 0`. best.c (static helpers): EQUAL, 0 differ.
- **unit_overrides**: force_internal += gfx_lock_8AB0, gfx_unlock_8AB0, font_select_8AB0, font_set_8AB0,
  TB_SetJust_8AB0, TB_GetFlashTextCycle_8AB0, HAL_SetFont_8AB0. Data: wasStrayed (function-local static, GC
  wasStrayed$1766) becomes own .bss at 0x8013F2F8 (drop any extern-data definition of D_8013F2F8);
  `extern volatile f32 D_8002EB90` (gSeconds) in this file.
- Path (rows): 99 -> 41 nesting font_select inside font_set (this is the "callee cost" lever w29b named: with the
  deeper helper nest the FP constants +-pi/2, 2.0, 255.0, -1.0 are rematerialised per use instead of hoisted into
  $f20-$f26) -> 17 `volatile` gSeconds (retail reloads it in each arm through an address register) -> 9 reset arm
  stores wrongWayTime before wasStrayed (s6/s7 web order) -> EQUAL with one more nesting level, `HAL_SetFont(font) {
  font_set(font); }` (void; 8 B per site, closes the last 16 B of frame; an s32-returning wrapper gives 126 words).

## func_800DB1E0 = GC updateOptionScreen (+ func_800DA2C0 = updateButtons) — pair draft
`w30b/func_800DB1E0/best.c` defines both; `dc.args` = `--unit func_800DB1E0,func_800DA2C0 --scripts options_menus`.
- Unit (`--internal func_800DA2C0`): `EQUAL func_800DB1E0: 350 words (kept, c_q1.c)`; `FAIL func_800DA2C0: 489 of 579
  words differ; compiled body is 572 words, target 579 | ops 78 | frame 152/160`.
  DB1E0 saves s0-s8/f20-f30 only because DA2C0 is an internal callee that clobbers them: DB1E0 can only land
  together with a matching DA2C0 (provisional until then).
- draft_check: `VERDICT: EQUIVALENT over 961 calls / 1 scripts (options_menus 961)` (unit members func_800DB1E0 961,
  func_800DA2C0 0 = only called inside the unit); register shims for func_800DA0BC, replay_save_prompt,
  time_result_display.
- DB1E0 levers: physics_sym template (`volatile s8 b0`), `case 0` last, the third first-option clamp written
  `14 - D_80149B84` inline (a reused `x` swaps v0/a0: 13 words).
- DA2C0: union Color copied by `.rgba` (retail `sw v0,60(t8)`; the struct version gives swl/swr); still w10e's body
  incl. pad arrays (`f32 pad0[4]`, `pad1[3]`: frame fillers, NOT acceptable for a match). Next: restructure along
  the matched twin func_800B59F0 (physics_sym), D_80116D10/D_8015273C reloads (retail `lui at; lw/lwc1` per use).
- GC names: see the best.c header (gActiveEdges/gActiveAutoEdges, inited -> initScreen = replay_save_prompt
  [wrong label], PlayStartABSound = resource_type_select, PlayUDSound = audio_distance_atten, PlayLRSound =
  audio_doppler, gSelectedOption D_80116D9C, validOption D_80116D14, gOptionMenuSize D_80149B84, gFirstOption
  D_80149DA2, gOptions D_80146108, gLang D_8013FEC8, LoadTextWave = func_800A510C, SetOptions = init_state_begin
  [wrong label], gSoftReset D_80157244, uninitScreen = func_800DA0BC, setPauseState = func_800B4FB0, exitOptionScreen
  D_80116DA0, the N64 _TitlePane of this screen = time_result_display [wrong label]).

## func_8010E8B4 (no change, 3/85)
32 source variants (`w30b/func_8010E8B4/v/`): split/one-line assignments, `(s16)` casts at the uses, s32 vs s16 y,
inlined helpers returning s16 (lineY/sh: 20-23 words), optPane(s16 y) parameter (6), holders `if (h) {}`/`if (y) {}`,
operand order of `y + D_8011AD70[2]`. None below 3.
**Listing oracle (asm.sh)**: retail's shape needs y in a coloured register DIFFERENT from h. Editing our ugen listing
so the narrowing goes `sll $24,$8; move $10,$24; sra $25,$10; move $10,$25` with the uses reading $10
(`w30b/func_8010E8B4/e6_listing_proof.s`) leaves 1 word (`addu a3,t9,t7` operand order): as1 then forwards $25
into the uses as retail. With y and h in one register ($8) as1 renames the sra destination instead (`sra t0`).
A `.loc` change, `sll/sra` without the first move, and reordering loads (e1-e5) change nothing. So the lever is a
source form where the unnarrowed value stays live past y's definition (or y is a parameter/return web that cannot
share h's register) and `y + D2` is emitted y-first. physics_sym packet: not superseded (E8B4 not matched); the
`inline_blockers` TitlePane entry stays.

## func_8010EA08 + func_8010F218 (not attempted)
Identity correction: correlation and the GC disassembly put them in **videoscr.c**, not carscr.c: EA08 =
`animButtonText@videoscr.c` (TV centring "%s%d" lines with gVertCenter/gHorzCenter, gTestFrequency, the test pane,
function-local statics width$636/height$637 = the two `sh 160` at entry, D_801569B0/D_80156988), F218 = its caller
`animText@videoscr.c` ("CONTROLLER %d %s"). EA08 is F218's internal callee (unsaved s0-s8): pair packet. EA08's
TB_FlashTextColors sites copy the first colour through a stack temp with an `at` word copy and the second by
value: a different helper idiom than optionscr.c's.

## What generalises
- By-value colour union = the stack homes of inlined colour helpers (any `sw tN,40(sp)` beside a global store of
  the same value in the TB_FlashTextColors family).
- `case 0:` (or whichever case retail emits last) placed last in the source switch fixes block order wholesale.
- Helper nesting depth is a callee-cost lever: one more inlined level changed FP constant hoisting (80108AB0) — and
  an extra void wrapper level is 8 B of frame per site.
- One-line helper definitions settle as1 ties between a helper's spill store and the next helper's argument moves.
- A pointer derived from another pointer local (`vel = v->vel`) is folded to base+offset; compute it from the
  original object to keep it a separate web (func_8010DCFC).

## Permission denials
None.
