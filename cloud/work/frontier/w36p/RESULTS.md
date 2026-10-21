# w36p results (wave 36, game.c / race flow)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36p (from base e68aea6e1).

| function | addr | bytes | GC identity | state | best file |
|---|---|---|---|---|---|
| track_render_process | 0x800A377C | 3200 | **PS_PakServer@paksysdc.c (confirmed: `if (!gInited) return` on D_8011EAE0, changed[4] cleared at sp+304, gErrPak D_8011EAE8 skip test, gPaks D_80144030 stride 772 with inserted/formatted/... flags; N64 body osPfs/rumble-adapted)** | not attempted (IPA partners func_800A3724/func_800A3640 unmatched; 800 words) | - |
| countdown | 0x800FBF88 | 2672 | **_UpdateGame@game.c (confirmed)** | **MATCH (group, internal)**; EQUAL in the unit, no inline blockers | groups/frontier_update_game/countdown.c |
| playgame_state_change | 0x800CA3B4 | 2544 | **updateGameState@state.c with initGameState inlined (confirmed; state_change_preprocess = uninitGameState)** | ops 2 / 80 words off (638/636 w) with 3 inline blockers that break PrintScores; cluster with display_enable + object_create | playgame_state_change/best.c |
| display_list_flush | 0x800FB2C8 | 2356 | **PrePlay@game.c (confirmed: cancel arg, inited static 0x801146F4, HFOV 90)** | 55 op rows (588/589 w), frame 256/88 | display_list_flush/best.c |
| RaceStateMachine_Update | 0x800DB81C | 2148 | **updatePauseScreen@pausescr.c (confirmed: 9-case switch on gPauseState 0x801170FC)** | **13 words short (524/537): every other word identical; the 13 are retail's `li t0,2` reloads at each case exit**; frame 272/272 | RaceStateMachine_Update/best.c |
| init_state_continue | 0x800FAF6C | 712 | **_CalcPolePositions@game.c** | **MATCH (group, internal; real caller countdown)** — w16f body unchanged, provisional retired | groups/frontier_update_game/init_state_continue.c |

## Integration (frontier_update_game, new group)
- Dir: `cloud/work/frontier/w36p/groups/frontier_update_game/`; members/claims: countdown (2672 B), init_state_continue (712 B).
- Both INTERNAL (force_internal countdown, init_state_continue): retail countdown saves nothing but ra (s0-s6/$f20/$f22
  used unsaved; game_loop saves them), init_state_continue is frameless and clobbers s0-s8.
- Context (byte-identical copies, verified with cmp): ctx_input_aux.c = codex_input_aux/group.c (game_loop, kept),
  ctx_vsync_a145.c = codex_vsync_a145/group.c, ctx_game_music.c = frontier_game_music/func_800D6160.c,
  func_800B5570.c, func_8008B2E4.c (singles). keep: game_loop, speed_mode0/1_wrapper, continue_prompt, vsync_wait,
  physics_init_mode0/1, func_800B5570, func_8008B2E4.
- No inline blockers and no overrides needed in the shadow unit (`blob_unit ... --internal countdown --internal
  init_state_continue --neighbours`: both EQUAL, locked bodies that differ: 0).
- countdown's own .data: function-local `static s8 startCountdown = 0` at 0x801146F0 (score.py: "own .data verified at
  0x801146F0"). `_EndGame` is a static helper umerge inlines; its deleted-procedure stub has no symbol (in the group
  compile it lands after a context function: speed_mode0_wrapper / func_800B5570 show "1 extra words", context only).
- Retire init_state_continue's provisional entry (w16f) — its real caller countdown is now in the group.
- Proposed names: countdown -> _UpdateGame, init_state_continue -> _CalcPolePositions.

## Identities proven
- countdown = _UpdateGame@game.c (GC 0x80014340): state-bit chain 0x40000/0x80000/0x200000/0x100000/0x400000/0x1000000/0x2000000/0x800000, SetCountdownTimer 1800/600/300/1200/CalcRaceTime, 4.0 at EndGame, WorldFogDensity loop on 0x600000.
- display_list_flush = PrePlay@game.c (useHud, cancel): `if (cancel) { inited = 0; return 0; }`, 0x2000000 branch, HFOV = 90.0.
- init_state_continue = _CalcPolePositions@game.c (called from PrePlay's high-score branch and _UpdateGame's 0x40000 state).
- playgame_state_change = updateGameState@state.c (initGameState inlined); state_change_preprocess = uninitGameState.
- RaceStateMachine_Update = updatePauseScreen@pausescr.c (switch on gPauseState D_801170FC).
- track_render_process = PS_PakServer@paksysdc.c (SaveServer calls it; same skeleton: gInited test D_8011EAE0, changed[] clear loop, gErrPak D_8011EAE8, gPaks[4] at D_80144030 stride 0x304; N64 osPfsInitPak/osPfsFreeBlocks/osMotorInit/osPfsRepairId body).
- object_create = HAL_SetFont (kept; lock D_801461D0, slot_state_setup(font) via IPA s2, unlock, returns old font). display_enable = LoadFrontEndOverlay(load) (state.c; flag D_801174C0). func_800B5570 = setGameState. func_800960CC = heap free locked (deleted helper). object_type1_create = AudKillSong body, object_type7_create/_alt = AudStopAllEffects/AudPauseAllEffects bodies, func_800D6348 = AudResumeAllEffects body (type 9).
- func_800A5B3C = ZOID_Reset, player_flag_clear_process = ReloadTrack, func_800FAF50 = InitCPS, func_800FAEE4 = InitDrones, func_800A14FC = PS_RumbleGo, effect_system_init = InitStunt, world_physics_tick = init_cars, render_post_process = init_views, music_tempo_set = SetupCar, func_800E543C = Init_MDrive, func_800EC914 = munge_gLink_data, func_800F7EB0 = ResetHighScores, camera_shake_start = InitTextureAnimation, func_800A5A40 = ZOID_ResetWindows, wheel_render_full = ZOID_SplitScreen.
- game_loop (0x800FD464) = GameLoop@game.c (calls updateGameState, updatePauseScreen, _UpdateGame).
- Callee map (N64 -> GC) used in countdown: init_state_begin=SetOptions, entity_audio_update=ClearRushControls, camera_race_setup=CarSetup, race_init_helper=Update_MDrive, race_setup_1=TextureAnimation, race_setup_2=CheckTargetCollision, viScheduleTick=SetCountdownTimer, viDeadlinePassed=CountdownTimeOut, func_800E762C=init_model_task, func_800FBF2C=CountDown, func_800FBE30=_CheckButtons, func_800FBED8=CalcRaceTime, func_800B61A8=PlayGameSound, physics_init_mode0=StartGameMusic (func_800D6160=_StartGameMusic), func_800FBE60=InitCPTime, render_viewport_init=PlayGame, func_800F7F3C=FindWinner, ghost_race_setup=GhostEnd, records_screen=RemoveHUD, func_800CF604=AreWeSolid, func_800E7DD0=are_all_cars_stopped, speed_mode0_wrapper=AudSetMusicVol, func_800D5828=End_MDrive, cpak_read=UpdateVisuals, players_frame_update=modelstop, func_800D5374=StopListeners, billboard_render=updateHighScoreScreen, finish_state_alt=updateGameOverScreen, func_800AB18C=WorldFogDensity, vsync_wait=EnableGameSounds.
- Globals: state_word_a=gGameState, state_word_b=gNewGameState, D_801174BC=gSkipToGameState, D_801146F0=startCountdown (static), D_801146F4=PrePlay inited (static), D_80114650=gRestartGame, D_80114654=gGamePointPlayed, D_8013FECB=gCoastFlag, D_80142699=gRetireGame, D_80142690=gOutOfTime, D_80142760=gDeathMode, D_801407BC=gStuntMode, D_80140804=gStuntModeMins, D_80140B08=gBattleMode, D_80140BD8=gBattleModeMins, D_80140A00=gStuntModePoints, D_80140AD8=gStuntModeDeaths, D_80142510=gBattleModePoints, D_8015256C=gTeamPoints, D_8012E67C=gCarTeam, D_80152038=gStuntPlayer (stride 120, totalPoints +20), D_80143F10=gAPlayerReallyFinished, D_801525F4=gDeathWaitEnd, D_80152734=gNumLaps, D_80152698=gGhosts, D_80152744=gNumActiveCars, D_80150F14=gUseCatchup, D_80150EFC=gCatchup, D_8002AFB4=gRetraceRate.

## Notes per function

### countdown (_UpdateGame) + init_state_continue (_CalcPolePositions): GROUP MATCH
Group `cloud/work/frontier/w36p/groups/frontier_update_game/` (members countdown, init_state_continue; claims both).
```
$ IDO_DIR=... python3 tools/cloud/score.py group --claims cand/frontier_update_game
Members:
countdown:
  MATCH
    own .data verified at 0x801146F0 (4 bytes, all zero; .data is laid out per translation unit)
init_state_continue:
  MATCH
(context: game_loop 172/176 differ = the unclaimed codex_input_aux reconstruction; speed_mode0_wrapper and
 func_800B5570 "MISMATCH (1 extra words (nonzero beyond target length))" = context only; the rest MATCH)
exit=0
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w36p score countdown init_state_continue --with combo/c1.c --internal countdown --internal init_state_continue --neighbours
  EQUAL countdown: 668 words (internal, c_c1.c)
  EQUAL init_state_continue: 178 words (internal, c_c1.c)
  locked bodies that differ in this unit: 0
```
(blob_unit check of the final countdown.c alone: `EQUAL countdown: 668 words (internal, c_v12.c)`, locked bodies that differ: 0, no --block.)
Integration: both must be INTERNAL (force_internal countdown, init_state_continue); retire init_state_continue's provisional entry;
countdown's static startCountdown is the group's own .data at 0x801146F0; _EndGame is a static helper inlined by umerge.
History:
Unit run: `blob_unit --tag w36p score countdown --with countdown/v/vN.c --internal countdown --block func_800B61A8 --block func_800FBE30 --block func_800FBF2C --block func_800FBE60 --block func_800FBED8 --block func_800E762C`
(the six small callees are called by jal in retail; without blockers umerge inlines them: they need unit inline_blockers on install).
- v1 direct port: ops 80 -> 39 with blockers. v2: startCountdown as a function-local static (address web gone): 27. v3: `viScheduleTick(3.5)` vs `3.5f * gRetraceRate` (two spellings = two constants, retail rematerialises): 24. v4: gRetireGame as the head of the else-if chain (retail jumps to the block's common exit): 22.
- Remaining: the &gNumPlayers address web (pretrace ldaS(1083) = p1:w306 -> s3); force.sh "p1:w306=s" gives 668/668 words, 10 op rows.

### display_list_flush (PrePlay) — in progress
Unit run (with the countdown group sources so init_state_continue is internal and clobbers s0-s8 as in retail):
`UARGS="--internal InitBattlePoints --with groups/frontier_update_game/countdown.c --with groups/frontier_update_game/init_state_continue.c --internal countdown --internal init_state_continue" us.sh display_list_flush display_list_flush/best.c`
-> (q4) `ops 63`; with the audio commands composed from internal AudLock/AudUnlock/AudSend (p7 = best.c): `FAIL display_list_flush: 499 of 589 words differ; ... | words 395 ops 55 norm 55 | frame 256/88`.
The frame gap is a consequence of colouring: retail spills two expression webs (gGameCar pointer at 84, InitStunt loop pointer at 92) into homes deep in the inline area; here they get s-registers (callee cost 29.75), so no deep homes and ugen keeps the frame small.
Found so far:
- N64 callee map: vsync_wait=EnableGameSounds, music_tempo_set=SetupCar, func_800FAF50=InitCPS (2 stmts, inlined), func_800FAEE4=InitDrones, func_800E543C=Init_MDrive, skid_mark_render=ShowHUD, object_type1_create=AudKillSong body, object_type7_create(_alt)=AudStopAllEffects body, func_800A5B3C=ZOID_Reset (kept, but INLINED in PrePlay), func_800A5488=InitBlits, tire_compound_set=InitWaves, func_800EC914=munge_gLink_data, func_800F7EB0=ResetHighScores, func_800A5A40=ZOID_ResetWindows, wheel_render_full=ZOID_SplitScreen, player_flag_clear_process=ReloadTrack (inlined), camera_shake_start=InitTextureAnimation, func_800A7480=ZOID_SetFog(1000, 999, r, g, b, 255, win), world_physics_tick=init_cars, music_control=StartEnvironment, render_post_process=init_views, 0x8038CA24=InitBattlePlayer (image B), func_800D6530=StartListeners, 0x803908D0=ShotInit (image B), effect_system_init=InitStunt (inlined), func_800A14FC=PS_RumbleGo (inlined).
- **effect_system_init (locked) in the MAT3 shape** (`memset; ResetStunts(); eTime = 0.0f; gStuntsDone = 0;` with a static ResetStunts loop) is EQUAL as its own kept body (`EQUAL effect_system_init: 35 words (kept, c_p4.c)`) AND umerge inlines it into PrePlay as retail does; the locked do{}while(0) form is not inlined. (Integration: PrePlay needs that definition — supersede/prefer_definition for effect_system_init.)
- ZOID_Reset and the two audio-message bodies are inlined in retail although their kept copies (func_800A5B3C, object_type1_create, object_type7_create) are 5-10 statements: here as static copies (`static void ZOID_Reset`, `AudKillSong`, `AudStopAllEffects`).
- num_our_drones = 0 then a reload stored to num_drones/num_humans through volatile stores (`*(volatile s16 *)&num_drones = num_our_drones;`): retail's lui/addiu + 0(reg) form and single reload.
- Remaining: callee-saved cost 29.25 here (constant 1 web tot=30.0 coloured into s3, gLink and gNumActiveCars addresses coloured); retail colours none of them (cost >= 30 => more uopt blocks in retail), and retail's frame is 256 vs 88 (local area ~168 B larger: more spill homes / inlined-call area). Unknown source of the extra blocks/frame.

### RaceStateMachine_Update (updatePauseScreen) — in progress
`us.sh RaceStateMachine_Update RaceStateMachine_Update/best.c` -> `FAIL ... words 232 ops 35 norm 35 | frame 272/96`.
Map: func_800B4FB0=setPauseState (w36q), viEnableAccum/viDisableAccum=PauseTimer/ResumeTimer, continue_prompt=PauseGameMusic
(speed_set(0, 0.05, 1, 0), inlined), physics_init_mode1/0 = ResumeGameMusic/StartGameMusic (_StartGameMusic t0=1/0),
func_800D5374=StopListeners, players_frame_update=modelstop, func_800D6530=StartListeners, func_800D60AC=modelgo,
player_mode_set=SetControllerStickEdges, visual_objects_update(1)=SaveFlush, entity_flags_apply=AudPlayEffect (N64 ids 46/37),
func_800DB1E0/func_800D91A0/func_800D7634/physics_sym = update{Option,Control,Audio,Video}Screen, audio_distance_atten=PlayUDSound,
audio_doppler=PlayLRSound, resource_type_select=PlayStartABSound, func_800D5A04=AbortGame, voice_stop (end-of-frame
controller-removed path). Globals: D_801170FC gPauseState, D_80149D98 pauseMenuSelect (8 items on N64), D_8015698C
playerInPause, D_80156CF0 gControllerData (stride 16, attached +0, VOLATILE: retail rereads it), D_80117350
gControllerRemoved, D_80117354 gYes, D_801543CC gModelTime, D_80157244 gSoftReset, D_80143FD8 target list (PauseTargets inlined).
N64 changes: SaveServer(0,0) first when paused; pause also when a controller is unplugged; resume path returns; soft reset
moved to the end; controller-removed re-check calls voice_stop.
Levers found: the five audio-command copies (types 7/9/7/7/7) are a macro with a block-scoped `msg` (retail spills each to
its own slot 244/180/132/100/68; a static helper with 3 call sites is not inlined); `pressed = (edges & 1) ||
!attached || 0;` reproduces retail's materialised boolean exactly (the 2-operand form loses `bnez t7; move v0,zero`).
Later levers (r20..r32): audio commands as kept functions built from kept AudLock/AudUnlock/AudSend wrappers (inlined at
all 5 sites; spill homes 244/180/132/100/68 = retail); PauseGameMusic kept wrapper (speed_mode0_wrapper(0, 0.05));
PlaySelectSfx kept wrapper for AudPlayEffect 37; SetPausePlayer(p) kept wrapper `playerInPause = p; gControllerRemoved =
!gControllerData[playerInPause].attached;` (gives retail's materialised-a1 store and load-first order); AudSend as
`if (osSendMesg(...)) {}` (gives retail's stray `beqz v0,L; b L` in the retire path; disclosed compiled-out debug test).
Frame now 272/272 with all five msg homes at retail offsets. Which wrappers carry the three extra 8-byte inline slots is
not unique (PauseTimer, PlayPauseSfx, PlaySelectSfx, SetPausePlayer all fit by position): the chosen set is the one that
also fixes code.
Residual (r32 = best.c): `ops 26`, all from the constant-2 web: retail keeps 2 live over the whole switch (t0 set in the
dispatch delay slot and re-set after the last call of every case: 13 `li t0,2`), here the web (ctrace p1 w177, nocs 4)
covers only PauseTargets and the end chain, so t0 is set once at the end-block entry. Tried: inline loop instead of
static PauseTargets, switch/|| forms of the voice_stop test, an early `if (gPauseState == 2) {}` (oracle; worse).
`blob_unit ... score RaceStateMachine_Update --with RaceStateMachine_Update/best.c --neighbours` ->
`FAIL RaceStateMachine_Update: 498 of 537 words differ; compiled body is 524 words, target 537` (offset shift after the
first missing li), locked bodies that differ: 0.

### umerge inlining and frame-area findings (w36p, generalise)
- M3's "kept callee with 3+ statements is never inlined" is WRONG. Probe (`umerge_probe/probe1.c`, read from the unit's
  umerge.log): kept callees with 5 global stores, 3 no-argument calls, or 3-5 statements of calls to tiny kept helpers
  ARE inlined at all 3 call sites; 3 calls with 3 arguments each, a 10-store+call body, or a body with a 3-argument
  external call are NOT. The limit is a size budget on the callee body before its own inlining (cheap: no-arg calls,
  stores of constants; expensive: argument lists).
- The retail audio-command bodies (object_type1_create/object_type7_create/object_type7_create_alt: no jal callers in
  retail, bodies inlined everywhere) are kept functions built from tiny kept helpers: `AudLock(); msg = func_80091B00();
  msg->type = T; AudUnlock(); AudSend(msg);` with AudLock/AudUnlock/AudSend one-statement osRecvMesg/osSendMesg
  wrappers. Written that way umerge inlines them at every call site (RaceStateMachine_Update: 5 sites).
- Frame: every inlined KEPT call reserves an 8-byte slot in the caller's local area (in umerge inline order); an inlinee's
  own frame area (its locals + its own nested inline slots) comes with it. An AUD command = 32 bytes (msg + 3 nested
  slots + pad): with this RaceStateMachine_Update's msg spill homes moved from 84/80/76/72/68 to retail's
  -/180/132/100/68 and the frame from 96 to 256 (retail 272). Static/internal inlinees without locals add nothing.
  This explains the large retail frames of PrePlay (256) and playgame_state_change (232, slot spills 132/108/84): they
  inline many kept wrappers.

### playgame_state_change (updateGameState + initGameState) — in progress
Map: state_change_preprocess=uninitGameState, tire_compound_set=InitWaves, display_enable=LoadFrontEndOverlay(load) (unmatched,
internal, IPA: clobbers s0-s4 via PrevMaxPath's register params; my reconstruction: unload "Extra" via func_800960CC
(heap free, locked helper), inline sync_maxpath_to_checkpoint (load "Start"), flag D_801174C0; 21 ops off),
init_state_begin=SetOptions, game_mode_handler x3, func_800C9480=InitRecords, player_cleanup_slots=_UnloadFontWaves,
scene_cleanup_slots=_UnloadFrontEndWaves, resource_slots_clear_multiple (jal), player_state_set=SetControllerActive,
player_mode_set=SetControllerStickEdges, func_800A3424=PS_LockFile, sync_entry_register(6,1)=AudPlaySong,
audio_frame_sync=LoadWave (N64 ids 57/56/60/55), visual_objects_update(1)=SaveFlush, InitMaxPath (load "Extra"),
**object_create = HAL_SetFont (13 calls in the 0x40000 state in GC PrePlay's font order 13,10,11,12,4,1,2,3,9,8,5,6,7;
retail calls the first 10 and umerge inlined the LAST 3)**, hud_setup/hud_speed_display heap setup on !D_80156994,
speed_set via speed_mode1_wrapper(sfx/10, 0.1) and speed_mode0_wrapper(music/10, 0.05) + AudPlaySong(6,1).
Findings: umerge processes a caller's call sites from the LAST to the first under a size budget (limit minus caller
size; probe: shrinking the caller raises the count); tiny callees still fit. object_create must be KEPT (retail body:
a0 home, `lw s2,48(sp)`, old spilled) and inlinable: the direct form `osRecvMesg; old = slot_state_setup(font);
osSendMesg; return old;` is EQUAL as its own body but never inlined; composed forms (internal FontLock/FontUnlock) are
inlined (9-11 sites) but differ by 4-13 words (oc3: 4 words, spill of `old` lands in the osSendMesg delay slot).
With kept object_create, `PrintScores` (frontier_billboard_render) stays EQUAL. resource_slots_clear_multiple,
InitMaxPath and func_800C885C need suppressing; --block on them breaks PrintScores/func_800C8918, so the real cause
is budget (retail's caller budget leaves room for only 3 object_create inlines).
Remaining in best.c (g9, with blockers, --keep object_create, display_enable internal): gGameState web v0 vs retail v1
(force.sh p1:w0=c2 fixes it), frame 224/232, s5 saved because my display_enable colours two address webs retail folds.

## Permission denials
none
