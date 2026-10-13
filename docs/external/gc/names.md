# N64 game symbols named from the GameCube build (and wrong labels replaced)

Lane w29n (wave 29) renamed these game symbols. Machine-readable table with evidence:
[`cloud/work/frontier/w29n/renames.json`](../../../cloud/work/frontier/w29n/renames.json); procedure and gates:
[`cloud/work/frontier/w29n/RESULTS.md`](../../../cloud/work/frontier/w29n/RESULTS.md). Style: GC names keep
their spelling without the leading underscore of CodeWarrior file statics (`_PrintScores` → `PrintScores`);
labels with no GC/arcade counterpart get a neutral snake_case description of what the function does.
A name is only as good as its evidence: "matched" means the N64 function is locked C written from the GC
function (statement order, locals, layout); "call" means the N64 call sites sit where the GC function's
calls do; "corr" means a confident pair in [correlation.md](correlation.md).

## Functions (inventory targets)

| Address | Old N64 name | New N64 name | GC source | Evidence |
|---|---|---|---|---|
| 0x800DC88C | attract_mode_handler | control_cell_text | — (N64 only) | wrong label: one menu cell's text, centred (w28d); only caller control_settings (overlay A) |
| 0x800DCDF4 | attract_video_handler | control_cell_text2 | — (N64 only) | wrong label: cell text + two option strings (w28d) |
| 0x800DC99C | attract_demo_handler | control_cell_text3 | — (N64 only) | wrong label: cell text + three option strings (w28d) |
| 0x800A2680 | AdjustSpeed | pak_delete_file_retry | lead: paksysdc.c PS_DeleteFile/_DeleteFile (not scored) | wrong label: osPfsDeleteFile with retry, osPfsFreeBlocks, node to free list (w28b) |
| 0x800EDDC0 | attract_or_transition | gfx_frame_begin | — (N64 only) | wrong label: per-frame display-list prologue (w28d, locked) |
| 0x800B61FC | audio_doppler_full | ShowCountdown | countdown.c ShowCountdown | matched (w28c), corr 5.35 (str CNTDWN%d) |
| 0x800F0914 | func_800F0914 | PrintScores | highscorescr.c _PrintScores | matched (w28a), corr 6.51 |
| 0x800F1210 | func_800F1210 | SetHighScoreState | highscorescr.c _SetHighScoreState | matched (w28a), corr 4.4 |
| 0x80108154 | func_80108154 | animLapTime | hud.c animLapTime | matched (w28c; GC locals close the frame) |
| 0x800CD104 | func_800CD104 | CreatePlayer | save.c CreatePlayer | call-for-call + homes = GC locals (w28b draft) |
| 0x800C813C | func_800C813C | SaveServer | save.c SaveServer | structure + save_settings' (0, 0) call (w28b draft) |
| 0x80096CA8 | func_80096CA8 | SetupWaveModel | waves.c _SetupWaveModel | FourCC chunk tags, homes = GC locals (w28d), corr 9.48 |
| 0x800B574C | sound_loop_set | TitlePane | _TitlePane | unit-EQUAL draft from GC (w28c) |
| 0x800A2D4C | track_process_main | PS_CreateFile | paksysdc.c PS_CreateFile | call: CreatePlayer's (pak, file, "P", 0x810, 0x3544, 'NRUE') (w28b) |
| 0x800A2504 | slot_state_lookup | PS_WriteFile | paksysdc.c PS_WriteFile | call: CreatePlayer (w28b); locked (pak_write_cluster) |
| 0x800A2378 | MaxPathZeroControls | PS_FlushFile | paksysdc.c PS_FlushFile | call order in CreatePlayer (w28b); locked |
| 0x800A21A4 | track_collision_setup | PS_FlushPak | paksysdc.c PS_FlushPak | call order in CreatePlayer (w28b); locked |

## Data (GAME_SYMBOLS in tools/conveyor/pipeline/disasm.py)

| Address | Old | New | GC source | Evidence |
|---|---|---|---|---|
| 0x80140B20 | D_80140B20 | gHighScoreState | highscorescr.c | SetHighScoreState stores/switches it (w28a) |
| 0x80149418 | D_80149418 | namePanes | highscorescr.c | SetHighScoreState pane array (w28a) |
| 0x80146208 | D_80146208 | codePane | highscorescr.c | SetHighScoreState (w28a) |
| 0x8002E870 | D_8002E870 | gDefStartTimes | highscorescr.c | PrintScores default times, f32[] (w28a) |
| 0x80156D38 | D_80156D38 | gWaveInstance | waves.c | SetupWaveModel 20-byte records (w28d) |

Not changed with the rename (by design): group directory and file names (`groups/audio_doppler_full/`,
`frontier_billboard_render/func_800F0914.c` ...: they are ids, not symbols), the generated m2c prelude
declarations in sources that do not use a renamed name, `cloud/matches/` submissions and lane drafts
(history), the overlay images' frozen `asm/us/ovl_{a,b}/symbols.json` and overlay sources (Astra's), and
`asm/us/boot_tail/symbols.json`. Drafts written against the old names: rename them with
`python3 -m tools.conveyor.pipeline.static_rename source cloud/work/frontier/w29n/renames.json FILE.c`.

Deferred (evidence as good, but a test or a research packet uses the old label as a fixture; rename each
together with its fixture, see `deferred` in renames.json):

| Address | Old | New | GC source | Blocking fixture |
|---|---|---|---|---|
| 0x800FF724 | game_results_input | animControls | controlscr.c animControls (corr 3.76, str CONTROLS_SM) | `tests/conveyor/test_d05_hud_context_audit.py` parametrises on the name |
| 0x800BB9B0 | func_800BB9B0 | LoadTrack | tracks.c LoadTrack (corr 12.96: track-name table x19, SkyInit/InitSmoke pairs) | `tests/cloud/test_node_pool_init_match.py` records the native caller as func_800BB9B0 |
| 0x80151690 | D_80151690 | gTotalNames | gamerecords.c/highscorescr.c (PrintScores `[track][3][5]`) | packet `cloud/work/dot_name_cache` replays sources using D_80151690 |
| 0x801161F4 | D_801161F4 | gRomObjDefPtrs | waves.c/mbox.c (SetupWaveModel object defs) | `dot_material_selector_20261006` pins car_gear_shift.c and binds the name |
| 0x80151AE8 | D_80151AE8 | gTexDefPtrs | waves.c (SetupWaveModel {ptr, count}) | src/blob/func_800B24EC.c is pinned by `dot_runtime_a_availability_root_20261012` and the multiblit/blit_bar contracts |
| 0x80138670 | D_80138670 | gPalDefPtrs | waves.c (SetupWaveModel {ptr, count}) | `tests/conveyor/test_disasm.py` fixture symbol |
| 0x80150B70 | D_80150B70 | gCamera | camera.c (ShowCountdown, stride 0x98) | `dot_steady_camera_20261005` looks the name up |
| 0x80151AD0 | D_80151AD0 | gNumWindows | global s16 (SetHighScoreState, ShowCountdown) | sources pinned by `dot_runtime_b_radius_effect_20261012`, `dot_frame_dispatch_20261005` |
| 0x80152032 | D_80152032 | gCountDown | countdown.c (ShowCountdown, s16) | `dot_frame_dispatch_20261005` pins Effects_UpdateEmitters.c |
| 0x80140BDC | D_80140BDC | gNumModels | global u8 (ShowCountdown, volatile) | `dot_runtime_a_availability_root_20261012` checks `extern volatile u8 D_80140BDC;`; sources pinned by `dot_slot_setter_boundary_20261006`, multiblit/blit_bar contracts |

## GC names recorded by wave-28 lanes, not applied yet

Weaker evidence, or the N64 label is in active use by an open lane; apply with the same procedure when a
match or a second anchor confirms them.

| N64 | GC name | Source of the lead |
|---|---|---|
| func_800F0F44 | _FindGhosts (highscorescr.c) | called by SetHighScoreState where GC calls _FindGhosts (w28a); corr 2.2 |
| func_800F1114 | _UninitScreen (highscorescr.c) | same (w28a); the GC name exists in several files |
| func_8010AEAC / func_8010A8D0 / func_8010A7A4 | animText / animButtonText / printOption (optionscr.c) | w28c (register-parameter order = GC prototype order); corr ~3 |
| func_80108AB0 | animWrongWay (hud.c) | w28c names only |
| func_80105EA8 | animResults (hud.c) | corr 2.8 (str "%s\n%s") |
| func_80103D28 | DrawUnlocked (highscorescr.c) | corr 3.8, runner-up 3.5 |
| func_800CCCCC | _InitPlayer (save.c) | CreatePlayer's call (w28b) |
| func_800CD0FC | _Pow (inlined; caller-less stub) | w28b |
| track_render_process | PS_PakServer | SaveServer's call (w28b) |
| drone_set_catchup | PS_ReadFile | w28b |
| func_80091FBC | InsertLLE | CreatePlayer's call (w28b) |
| format_string_parse | _CalcChecksum | CreatePlayer's call (w28b) |
| func_8008C544 / func_800A78BC / func_8008C074 / func_8008D0C0 | WorldVector / ZOID_NewPoly / ZOID_UpdatePoly / ZOID_RemovePoly | ShowCountdown's calls (w28c) |
| func_800B24EC | MBOX_FindTexture_Sub | ShowCountdown (w28c); corr pairs it with MBOX_FindTexture_WORK |
| entity_flags_apply | PlayGameSound | ShowCountdown (w28c) |
| object_manager_update / object_bytes_sum_global | HAL_GetTextWidth / TB_GetFontLineHeight | w28a, w28c |
| ambient_sound_set / crowd_cheer_play / sound_handles_array_clear | InitPane / ChangePaneSize / RemovePane | w28a, w28c |
| sound_stop / sound_control | RemoveBlit / NewMultiBlit | w28a |
| Input_ApplyPadConfig / func_800EF5B0 / brake_light_update | UpdateBlit / RenameBlit / ZOID_ProjectWindow | w28c |
| render_helper / func_800ED66C / dispatch_handler / state_utility | HAL_SetTextDepth / HAL_SetTextTranslucency / TB_SetTextColor / TB_DrawText | w28c |
| slot_state_setup vs object_create | HAL_SetFont | w28a and w28c disagree: resolve first |
| func_800D50E4 | rand_r (libc) | w28a; a libc name in game code, check header collisions first |
| viScheduleTick, viGetTimeToDeadline (static) | SetCountdownTimer, GetCountdownTimeRemaining | w28a, w28c; static side, needs the static rename proof |
| D_80152570, D_8014978C, D_801148D0, D_80142528, D_80150F88, D_801147C8, D_80151968 | gBackwards, gTrackNum, gTable, gThisTrack, gTotalRaceStats, gDefaultNames, gTempNames | PrintScores (w28a) |
| D_801148CC, D_801148A8, D_80149798, D_801148E8, D_80144DA0, D_801460C0 | blit, mblit, ghost panes, gPlayerRects, gGhostMenuSelect, gGhostPak | SetHighScoreState (w28a) |
| D_8014A110 (`gameplay_mode`), D_801174B4 (`state_word_a`) | gRace, gGameState | w28a, w28c (existing GAME_SYMBOLS names would change meaning) |
| D_8012E6D8, D_80144D68, D_8011742C, D_80117434 | gPlayerFiles, gPS_Files, sInitCountdown, sReadingPak | CreatePlayer/SaveServer (w28b) |
| D_80118E20, D_8015698C, D_8011AD6C, D_8011AD68, D_80116D0C, D_80116D9C, D_80116D14, D_80146108, D_801146BC | TB_SetJust pair, playerInPause, gControllerRemoved, gTitlePane, buttonsInited, gSelectedOption, validOption, gOptions, flashColors | w28c |

Other confident correlation pairs whose N64 label looks wrong (unreviewed; see correlation.md): e.g.
`entity_spawn_init` → StartSmoke, `buffer_swap` → AnimateWing, `cpak_read` → UpdateVisuals,
`save_load_data` → StartFrame, `transmission_ratio_get` → SetupTarget, `render_large_objects` →
_AssignDrones, `mode_select_handler` → _UpdateBumps, `drone_collision_avoid` → calcMenuRect, and the libm
pairs func_8008C5E0/C680/C720/C768 → fxatan/fsatan/fatan/fatan2, func_8009C3F8 → farcsine,
func_800A557C → ftan.

## Later wave-29 lanes (recorded for the replay; candidates are not applied)

`renames.json` rows with status `candidate` (accept by setting `accepted` before `replay.sh`):
func_800F0F44 → FindGhosts (w29a matched it from GC _FindGhosts), func_8010221C → drawMain (GC gameoverscr.c,
w29a), func_800E6AF8 → model_iteration (arcade mdrive.c, w29c), lap_complete → animElapsedTimerText (GC hud.c,
w29c), race_finish → animPlace (GC hud.c, w29c draft), and the wrong-label lead game_results_update /
game_results_render = the audio-options screen (GC audioscr.c animButtonText/animText; w29a; needs an N64 name).
Helper identifiers the later packets introduce (rt_gfx_lock, rt_gfx_unlock, rt_font_set, rt_font_set_b;
update_drone_models, update_model, gfx_lock_761C, gfx_unlock_761C, font_set_761C, pair_set_761C) are not
inventory targets and collide with no new name; the old names those packets use (PrintScores/SetHighScoreState/
SaveServer and the highscore data in w29a's frontier_billboard_render, animLapTime in w29c's gr3_8154.c) are
found and renamed by the replay once the packets are installed in src/blob.
