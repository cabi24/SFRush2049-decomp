# w36w2 results (wave 36, game pool continued: gameoverscr cluster, GameLoop, _TitlePane@pausescr, GhostEnd, countdown_handler)

Lane added after launch. Repo HEAD at start: fb77b6f9e (wave35 branch). Builder scratch
~/rush2049/scratch/frontier/w36w2 (base + `git archive HEAD` of src/include/asm blob + lock files).

## Summary

Matched (game, all with blob_unit confirmation, `locked bodies that differ in this unit: 0`): 7 functions, 3,816 B
- groups/w36w2_gameover/ (NEW): func_800F857C 464, audio_update_d 76, func_800F87A0 968, finish_state_alt 556
  (needs unit_overrides force_internal func_800F84B0)
- groups/codex_input_aux/ (superseded in place): game_loop 704, countdown_handler 496 (+ refreshed copy of w36p's
  groups/frontier_update_game/ so both groups carry the same game_loop/countdown text)
- groups/frontier_pausescr_state/ (w36q's group + voice_stop): voice_stop 552
Not matched: ghost_race_setup 440 (3 words, as1 copy propagation; listing oracle exact).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| finish_state_alt | updateGameOverScreen@gameoverscr.c (NEW, confirmed) | 556 | **group MATCH + unit EQUAL** | groups/w36w2_gameover/ |
| func_800F857C | setGameOverState@gameoverscr.c (confirmed) | 464 | **group MATCH + unit EQUAL** | groups/w36w2_gameover/ |
| audio_update_d | uninitScreen@gameoverscr.c (confirmed) | 76 | **group MATCH + unit EQUAL** | groups/w36w2_gameover/ |
| func_800F87A0 | controlMain@gameoverscr.c (NEW, confirmed; 7-entry jump table = 7 menu options) | 968 | **group MATCH + unit EQUAL** | groups/w36w2_gameover/ |
| game_loop | GameLoop@game.c (confirmed) | 704 | **group MATCH + unit EQUAL** (needs w36p's frontier_update_game: countdown/init_state_continue internal) | groups/codex_input_aux/ (+ refreshed groups/frontier_update_game/) |
| voice_stop | _TitlePane@pausescr.c (confirmed) | 552 | **group MATCH + unit EQUAL** with real caller func_800B4FB0 (w36q's group; lands with it) | groups/frontier_pausescr_state/ |
| ghost_race_setup | GhostEnd@ghost.c (confirmed) | 440 | 3 words (unit; as1 copy propagation of the ghost copy; listing oracle exact) | ghost_race_setup/best.c |
| countdown_handler | _animScrollText@game.c (NEW, confirmed) | 496 | **group MATCH + unit EQUAL** (internal; real caller game_loop, same group) | groups/codex_input_aux/ |

## Notes per function

### gameoverscr cluster (finish_state_alt, func_800F857C, audio_update_d, func_800F87A0): MATCH, 2,064 B
- Identities: finish_state_alt = updateGameOverScreen (inited flag D_80114728, initScreen inlined:
  setGameOverState(1), gMainSelect loop over validOption, NewMultiBlit; BlinkResurrectingCars; gSoftReset
  shutdown; switch gGameOverState: 1 -> controlMain, 2 -> inlined controlScore (gActiveEdges & 7 ->
  PlayStartABSound(4), setGameOverState(1))). func_800F87A0 = controlMain (7-way switch on gMainSelect:
  Score -> setGameOverState(2); RaceAgain: ghost -> shutdown + gSkipToGameState 0x40000, else uninitScreen +
  gRestartGame + AudStopAllEffects + AbortGame; NewMode/NewTrack/NewCar/NextRace/Quit -> shutdown + skip
  0x40/0x80/0x100/0x4000/0x10; else PlayUDSound up/down loops over validOption). w36o's "no MAT3 counterpart"
  was wrong. func_800F84B0 = validOption (locked).
- Base: prior research bodies (PR #214 uninitScreen, #222 updateGameOverScreen, #292 setGameOverState with the
  font_set wrapper, lean_f87a0_input_20261006 controlMain 54/242).
- Lever 1 (unit, not source): **func_800F84B0 must be INTERNAL**. With the locked single kept (default),
  finish_state_alt was 7/139 (&gMainSelect in s0, retail a3) and controlMain had the constant 6 in s0 (retail
  a3): retail keeps both in a3 across validOption calls, which needs validOption's IPA register summary
  (it clobbers a1/a2 only). `--internal func_800F84B0` -> finish_state_alt EQUAL, controlMain 54 -> 54 rows
  of pure temp-ring shift. Its other caller drawMain stays EQUAL; locked bodies that differ: 0.
- Lever 2 (source, disclosed): controlMain's first words are `lui s0; addiu s0,&gActiveEdges; lw a0,0(s0)`
  (address coloured in s0, ahead of the frame setup). A compiled-out read `if (D_8015694C) {}` right AFTER
  `resource_type_select(D_8015694C)` (PlayStartABSound) keeps the address live across the call -> coloured
  -> EQUAL. Without it: folded `lui a0; lw a0,..(a0)` (241 words) or, with the old laundering
  `*(s32 *)(u32)&D_8015694C`, the address in temp t6 and every later ring temp one register off (54 words).
  Holder placements tried: before the test (241 words), right after the call (EQUAL). Written in the MAT3
  else-if form (gActiveAutoEdges tested twice, PlayUDSound(gActiveAutoEdges) takes its argument) - EQUAL;
  MAT3's `unsigned int` type for the two edge words gives 9 rows, so they stay s32.
- HAL_SetFont: the per-file non-static helper font_set_go (+ gfx_lock_go/gfx_unlock_go) as in the prior packet;
  calling the context object_create instead leaves it out of line (F857C 96 words) and breaks PrintScores.
- Group `groups/w36w2_gameover/` (new): files gameover.c (the four members + the three helpers),
  ctx_func_800F84B0.c (byte copy of src/blob/func_800F84B0.c), ctx_gr3_b.c (byte copy of
  credits_scroll_grp/gr3_b.c: slot_state_setup takes its font in s2 by IPA). members/claims = the four;
  context = func_800F84B0, slot_state_setup, object_create, func_800B669C; keep = [finish_state_alt].
  `score.py group cand/w36w2_gameover` (builder, no --claims): `func_800F857C: MATCH`, `audio_update_d: MATCH`,
  `func_800F87A0: MATCH` + `own .rodata verified at 0x8012461C..0x80124638`, `finish_state_alt: MATCH`;
  context func_800F84B0 MATCH, slot_state_setup 2/58 (sound_update_channel's t0 parameter, external here),
  object_create / func_800B669C deleted (no callers) - informational, same context as installed group
  func_80106B3C. `--claims`: all four MATCH.
- Unit: `python3 -m tools.conveyor.pipeline.blob_unit --tag w36w2 score func_800F87A0 finish_state_alt
  func_800F857C audio_update_d func_800F84B0 drawMain --internal func_800F857C --internal audio_update_d
  --internal func_800F87A0 --internal func_800F84B0 --internal gfx_lock_go --internal gfx_unlock_go
  --internal font_set_go --neighbours --with cloud/work/frontier/w36w2/groups/w36w2_gameover/gameover.c` ->
  `EQUAL func_800F87A0: 242 words (internal, c_gameover.c)`, `EQUAL finish_state_alt: 139 words (kept, ...)`,
  `EQUAL func_800F857C: 116 words (internal, ...)`, `EQUAL audio_update_d: 19 words (internal, ...)`,
  `EQUAL func_800F84B0: 51 words (internal, s_func_800F84B0.c)`, `EQUAL drawMain: 137 words`,
  `locked bodies that differ in this unit: 0`, `6/6 equal`.
- Integration: unit_overrides force_internal += func_800F84B0 (locked single; reason: retail callers keep
  values in a3 across it), func_800F857C, audio_update_d, func_800F87A0 (private IPA: s0 parameter /
  unsaved s0-s2), gfx_lock_go, gfx_unlock_go, font_set_go (inlined helpers, no image address). The locked
  stubs func_800F874C / func_800F8B68 (deleted initScreen / controlScore) stay as they are.

### ghost_race_setup = GhostEnd@ghost.c (440 B): 3 words in the unit
- Base PR #409 historical_typed.c: unit `FAIL ghost_race_setup: 16 of 110 words differ`.
- (1) DeallocateMemory = func_800960CC (the unit has its real body via prefer_definition from
  codex_heap_release_a25/wheel_params_set.c): calling it gives retail's parameter copy `lw s0,76(s2)` before the
  heap lock (explicit lock/free/unlock reads g->start after the lock).
- (2) Retail has ONE gNumPlayers reload block (J) shared by the place_locked arm and the free arm (the PurgeGhostData
  arm keeps its own). Ours put a reload at the end of each arm (+1 word). A compiled-out `do {} while (0);` after
  the inner if/else keeps J as a block, PRE then puts the reload there (disclosed shaping; plausibly an empty debug
  macro in the N64 source). Listing oracle v/o1.s proved it first.
- (3) as1 xbb pulled the free arm's osRecvMesg argument setup into the `lb t6,239(v0)` stall (weight trace
  v/dw_wt.log: X bb7 w=7, free arm bb8 w=10 -> MOVEFROM bb8). Indexing gGameCar[i] directly (MAT3 form) instead of
  a pointer local makes ugen open a `.noalias $2,..` region for the car address; its `.alias` close lands after the
  place_locked arm's `b`, which gives the free arm a pred-less directive predecessor (w36x mechanism) -> no hoist.
  53 -> 3 words.
- Residual 3 words: `move a0,v0` / `sw zero,0(s4)` order and `lw t7,0(a0)` (retail) vs `lw t7,0(v0)` (ours):
  as1 copy-propagates the ghost copy into the header load (the DAG already has 0(v0)). Listing oracle v/o7.s:
  moving the car-address `mul $25,$19,952; addu $2,$30,$25` from the place_locked block to just before
  `lw $15,0($4)` (after the modelrun store) -> `differing rows 0`. So retail's ugen emitted the car address in the
  main block, which redefines $2 between the copy and the load. Tried: pointer local at four positions (uopt
  propagates and sinks it; 53), compiled-out reads of gGameCar[i] before/after the header load (16 / 5: the
  address moves up but the handle load then takes v1), do{}while(0) boundaries around the modelrun store (21),
  MAT3 declaration orders / block-local ghost / early return (all 3). Best = MAT3 form (early return, g/i,
  block-local ghost) + direct indexing + the join do-while.
- Command: `python3 -m tools.conveyor.pipeline.blob_unit --tag w36w2 score ghost_race_setup --neighbours --with
  cloud/work/frontier/w36w2/ghost_race_setup/best.c` -> `FAIL ghost_race_setup: 3 of 110 words differ`,
  `locked bodies that differ in this unit: 0`.

### voice_stop = _TitlePane@pausescr.c (552 B): MATCH (group + unit), lands with w36q's func_800B4FB0
- Body: Astra's research PR #388 (mat3_ui_pause_title_20261017/group/voice.c, strict 138/138 there with a partial
  B4FB0 caller), unchanged except its three static helpers renamed vs_header_lock/vs_header_unlock/vs_header_font and
  made non-static (w17j rule; not on keep).
- Real caller: w36q matched func_800B4FB0 = setPauseState (groups/frontier_pausescr_state, not yet installed), which
  calls voice_stop three times. voice_stop is internal (private IPA, s0-s4 unsaved). The other retail caller,
  RaceStateMachine_Update (updatePauseScreen, w36p, unmatched), calls the byte-identical body by jal, so it needs
  nothing in the unit.
- Group `groups/frontier_pausescr_state/` = w36q's group (pause.c, ctx_gr3_b.c byte-identical; ctx_gr3_b.c also = 
  credits_scroll_grp/gr3_b.c) + voice.c; members [func_800B4FB0, voice_stop], claims [voice_stop], keep
  [func_800B4FB0]. Builder `score.py group cand/frontier_pausescr_state`: `func_800B4FB0: MATCH`, `voice_stop:
  MATCH` (context slot_state_setup 2/58, object_create/func_800B669C deleted: same context lines as w36q's group);
  `--claims`: `voice_stop: MATCH`.
- Unit: `blob_unit --tag w36w2 score voice_stop func_800B4FB0 physics_sym --internal voice_stop --internal
  vs_header_lock --internal vs_header_unlock --internal vs_header_font --internal ps_gfx_lock --internal
  ps_gfx_unlock --internal ps_font_set --internal ps_SetFont --internal ps_updatePauseStateCreate --neighbours
  --with groups/frontier_pausescr_state/pause.c --with groups/frontier_pausescr_state/voice.c` -> `EQUAL voice_stop:
  138 words (internal, c_voice.c)`, `EQUAL func_800B4FB0: 368 words (kept, c_pause.c)`, `EQUAL physics_sym`,
  `locked bodies that differ in this unit: 0`, `3/3 equal`.
- Integration: install this group instead of (or as the in-place supersession of) w36q's frontier_pausescr_state;
  unit_overrides force_internal += voice_stop, vs_header_lock, vs_header_unlock, vs_header_font (+ w36q's ps_*).

### game_loop = GameLoop@game.c (704 B): MATCH (group + unit)
- Prior: codex_input_aux's unclaimed context reconstruction (unit: 146/176, frame 32: no callee saves).
- Key fact: retail saves s0-s8/f20/f22 and keeps renderFlag at 84(sp) because its private callees countdown
  (_UpdateGame) and init_state_continue (_CalcPolePositions) use s0-s8 unsaved. w36p matched both (internal,
  group frontier_update_game, being installed by w36i in the working tree): with them in the unit the prologue
  comes out right (37 words left with a direct port).
- Levers: function-local `static s8 sInited = 0;` (0x801146EC; an extern CSEs the address into v0: 10 words);
  `elapsed` a named f32 (retail product in $f0: 10 words); `gNewGameState == gGameState` operand order at both
  tests (2 words); the elapsed assignment inside the if condition (as a separate statement line, as1 orders
  `lui at,0x4f80` before `lui t6,0x8011`: 2 words; one physical line or assignment-in-condition -> EQUAL; the
  assignment-in-condition is the delivered form). gFrames (frame_counter) must be volatile (retail lui/addiu +
  0(reg) at both accesses; non-volatile 158 words). The pause test is MAT3's two tests (0x200000 || 0x400000), not
  the codex `& 0x600000`. GVSyncCount = the volatile scheduler clock D_8002E8E8.tick (house type from
  func_80109A60.c).
- Packaging: `groups/codex_input_aux/` = codex_input_aux superseded in place: group.c = old group.c lines 1-3895
  (input_aux_handler unchanged; the trailing unclaimed game_loop reconstruction removed), new game_loop.c (member,
  kept, claimed), context = byte copies of w36p's frontier_update_game files (ctx_countdown.c,
  ctx_init_state_continue.c, ctx_func_800B5570.c, ctx_func_8008B2E4.c, ctx_game_music.c, ctx_vsync_a145.c), keep =
  game_loop + frontier_update_game's kept context. Builder `score.py group cand/codex_input_aux`:
  `input_aux_handler: MATCH`, `game_loop: MATCH` + `own .data verified at 0x801146EC (4 bytes, all zero; .data is
  laid out per translation unit)`; context countdown/init_state_continue MATCH, speed_mode0_wrapper and
  func_800B5570 `MISMATCH (1 extra words (nonzero beyond target length))` (the same context lines w36p's group
  shows). `--claims`: `game_loop: MATCH` (+ the own .data line).
- `groups/frontier_update_game/` = w36p's group refreshed so it does not carry a second (old) game_loop body:
  ctx_input_aux.c = byte copy of the new codex group.c, + ctx_game_loop.c = byte copy of game_loop.c; members,
  claims and all other files unchanged. Builder: `countdown: MATCH` (+ own .data 0x801146F0), `init_state_continue:
  MATCH`, context game_loop/input_aux_handler MATCH.
- Unit: `blob_unit --tag w36w2 score game_loop input_aux_handler countdown init_state_continue --neighbours --with
  cloud/work/frontier/w36w2/groups/codex_input_aux/game_loop.c` -> `EQUAL game_loop: 176 words (kept,
  c_game_loop.c)`, `EQUAL input_aux_handler: 121 words (internal, ...)`, `EQUAL countdown: 668 words (internal,
  g_frontier_update_game__countdown.c)`, `EQUAL init_state_continue: 178 words (internal, ...)`, `locked bodies that
  differ in this unit: 0`, `4/4 equal` (run against the working tree, where w36i's frontier_update_game install is
  in progress). countdown_handler (unmatched) stays external; game_loop does not depend on it.

### countdown_handler = _animScrollText@game.c (496 B): MATCH (group + unit)
- Identity (new): MAT3 game.c _animScrollText (GC 0x80015A24): numPlayers = gRace == kGhost ? 1 : gNumPlayers, first
  player with gControllerData[i].attached clear, none -> x = 320 / return; text from text-table pieces (N64 strings
  "%d" at 0x80120308 and "%s%s%s%s%s%d%s" at 0x8012030C = own .rodata), HAL_SetFont(11), TB_SetTextColor(22) =
  dispatch_handler, TB_DrawText(x, 8 or 205, str) = state_utility, `if (--x < -HAL_GetTextWidth(str, -1)) x = 320`.
  Statics: x = 0x801146F8 (`static int x = 320`, own .data), i = 0x801613B0. Only caller: game_loop (internal,
  s0/s1/s2/s4/s5 unsaved).
- Base w27b draft (unit 103/124). Levers: (1) `static s32 x = 320` function-local (extern: uopt CSEs &x into s1, the
  320 constant is not shared in s0): rows 67 -> 23; (2) font_set_cdh nesting gfx_lock_cdh only = frame 344 (none 336,
  lock+unlock 352); (3) **gControllerData volatile**: retail kept cfe's `bnez t7,L; b exit` around the break (listing
  oracle countdown_handler/o1.s: that listing alone gives the retail loop); a plain extern lets uopt fold it into
  `beqzl t7, exit`; seven loop spellings (continue, if/else, goto, while, do/while, == 0, pointer walk) and a
  function-local static i all compile to the folded form. A local i + the two explicit `D_801613B0 = i` stores
  reproduce what the promoted MAT3 static gives (stores in both arms after the test).
- Group packaging: member of the superseded `groups/codex_input_aux/` (file countdown_handler.c; helpers
  gfx_lock_cdh/font_set_cdh non-static, not on keep). Its allocation needs the callee IPA of the unit: context
  ctx_gr3_b.c (slot_state_setup s2 parameter), ctx_dispatch_handler.c, ctx_func_800A4770.c, ctx_sound_channel_extra.c
  + ctx_object_manager_update.c (byte copies of src/blob/dispatch_handler.c, func_800A4770.c and
  groups/codex_sound_channel_extra/*); without the object_manager_update group the group compile gives 58/124.
  Then the group compile put countdown's static _EndGame deleted-procedure stub (no symbol) right after
  countdown_handler: `MISMATCH (1 extra words (nonzero beyond target length))`. The image has exactly one stub there,
  the locked caller-less func_800FBE20, and the unit places _EndGame's stub at that address (`EQUAL func_800FBE20: 2
  words (internal, c_countdown.c)` once the static is named func_800FBE20). So ctx_countdown.c = w36p's countdown.c
  with `static void _EndGame(void)` renamed to the non-static `void func_800FBE20(void)` (code-identical; disclosed),
  and groups/frontier_update_game/countdown.c gets the same rename so both copies agree.
- Builder `score.py group cand/codex_input_aux` (no --claims): `input_aux_handler: MATCH`, `game_loop: MATCH` (+ own
  .data 0x801146EC), `countdown_handler: MATCH` + `own .data verified at 0x801146F8 (4 bytes; .data is laid out per
  translation unit)` + `own .rodata verified at 0x80120308..0x8012031C`; context: countdown, init_state_continue,
  func_800B5570 ... MATCH, speed_mode0_wrapper `MISMATCH (1 extra words ...)`, slot_state_setup 8/58, object_create
  deleted (informational). `--claims`: `game_loop: MATCH`, `countdown_handler: MATCH` (with the own-data lines).
- Unit: `blob_unit --tag w36w2 score countdown_handler game_loop countdown init_state_continue input_aux_handler
  func_800FBE20 --internal countdown_handler --internal gfx_lock_cdh --internal font_set_cdh --internal countdown
  --internal init_state_continue --internal func_800FBE20 --neighbours --with groups/codex_input_aux/game_loop.c --with
  groups/codex_input_aux/countdown_handler.c --with groups/frontier_update_game/countdown.c` -> all six `EQUAL`
  (countdown_handler 124 words internal, game_loop 176 kept, countdown 668, init_state_continue 178, input_aux_handler
  121, func_800FBE20 2 words internal), `locked bodies that differ in this unit: 0`, `6/6 equal`. (With w36p's
  unrenamed countdown.c: `EQUAL countdown_handler ...; stub tail: 2 words of deleted-procedure stubs follow (as in the
  image)`, 5/5 equal.)
- Integration (codex_input_aux superseded in place + frontier_update_game refreshed): unit_overrides force_internal +=
  countdown_handler, gfx_lock_cdh, font_set_cdh, func_800FBE20; prefer_definition func_800FBE20 ->
  src/blob/groups/frontier_update_game/countdown.c (locked as an empty stub; it is _EndGame's retail remains).

## What generalises

- **A locked single that retail treats as internal changes its callers' allocation**: func_800F84B0 (validOption)
  only clobbers a1/a2; with it kept (the default for singles) the unit gave its callers no IPA summary and they
  used s0 where retail keeps values in a3 across the calls. `--internal CALLEE` is a cheap first test when a
  caller has an s-register where retail has a3/a2/t-registers live across a jal.
- **Compiled-out read after a call = coloured address web**: `if (G) {}` right after `f(G)` keeps &G live across
  the call, so uopt colours the address (retail `lui s0; addiu s0; lw a0,0(s0)` before the frame setup).
- **Private callees decide a kept root's prologue**: game_loop's s0-s8/f20/f22 saves and its stack-resident
  renderFlag came out exactly once its private callees (countdown, init_state_continue) were in the unit; the old
  reconstruction had no saves at all. Check which callees use s-registers unsaved before hunting frame residuals.
- **Function-local statics fix address CSE**: extern globals read/written several times get a coloured address web
  (lui/addiu into an s-register); a `static` defined in the function (MAT3's statics: sInited, x = 320) gets
  retail's per-access folded `lui; lw off(lui)` and own .data the scorer verifies by address.
- **volatile also controls uopt's jump optimisation**: with gControllerData volatile, the break in a scan loop
  stays cfe's `bnez / b exit` shape (retail) instead of the folded `beqzl`; seven loop spellings could not do it.
  (gFrames also needed volatile: retail lui/addiu + 0(reg) at both accesses.)
- **ugen .noalias regions and as1 xbb**: indexing a global array directly (gGameCar[i].f) instead of through a
  pointer local makes ugen open a .noalias region for the element address; when it is still open at a block-ending
  `b`, ugen closes it after the `b`, which gives the following block a pred-less directive predecessor and stops as1's
  cross-block hoist into the previous block's load stall (w36x's mechanism, here reachable from source).
- **A join block for PRE**: an empty `do {} while (0);` after an if/else keeps the join as a block, so PRE places one
  reload there instead of one per arm (GhostEnd's gNumPlayers reload).
- **A static helper's deleted stub can land after a member in a group compile** (countdown's _EndGame after
  countdown_handler); naming it after the image's locked stub at that address (func_800FBE20) both fixes the group
  extent and matches the unit's placement.
- **Statement layout vs as1 ties**: `if ((x = expr) > c && ...)` gave the retail order of two independent `lui`s
  where `x = expr;` on its own line did not (GameLoop).

## Identities proven

- func_800F87A0 = controlMain@gameoverscr.c (7-entry jump table = kGameOverMainMenuCount options; Score ->
  setGameOverState(2); RaceAgain/NewMode/NewTrack/NewCar/NextRace/Quit as MAT3, N64 Quit sets skip 0x10). w36o said
  "no MAT3 counterpart". Byte-identical match.
- finish_state_alt = updateGameOverScreen@gameoverscr.c (initScreen and controlScore inlined; their deleted stubs
  are func_800F874C and func_800F8B68). Byte-identical match.
- func_800F857C = setGameOverState, audio_update_d = uninitScreen (w36n strong -> proven by match).
- countdown_handler = _animScrollText@game.c (D_801146F8 = its `static int x = 320`, D_801613B0 = `static int i`,
  D_80156CF0 = gControllerData, 16-byte records, attached at +0). Byte-identical match.
- game_loop = GameLoop@game.c (gstate 0x801146EC = sInited, frame_counter = gFrames (volatile), D_8002E8E8.tick =
  GVSyncCount, game_state_flags/D_801146C4 = the controller-warning blit and its multiblit). Byte-identical match.
- func_800FBE20 = the deleted-procedure stub of game.c's static _EndGame (unit places it there).
- voice_stop = _TitlePane@pausescr.c (PR #388 body; matched with its real caller setPauseState).
- ghost_race_setup = GhostEnd@ghost.c (3 words: structure and every instruction identical).

## Permission denials

none
