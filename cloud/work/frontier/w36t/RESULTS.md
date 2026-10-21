# w36t (wave 36, game audio lane) RESULTS

Status (updated after every function):

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| entity_ai_pathfind (0x800992AC) | N64 audio thread (osScAddClient + recv loop) with MAT3 _ProcessCommand inlined | 2180 | blocked (written up): ~221 rows; uopt drops store->load forwarding/PRE in the full 12-case body | thread/best.c |
| entity_process_main (0x8008D120) | AnimateShadow@visuals.c (GC 0x80059578; proven by flow) | 1424 | **MATCH** standalone + unit EQUAL | cloud/matches/entity_process_main.c |
| audio_interrupt_handler (0x800BB140) | LoadCars@carload.c (confirmed, Astra #404 source) | 1372 | **group MATCH + unit EQUAL** (with func_8009768C stub member) | groups/w36t_loadcars/ |
| audio_frame_sync (0x80097798) | LoadWave@waves.c (proven, w36n) | 520 | 4 rows: code identical except func_80097164's register param s8 (context convention, SetupWaveModel-gated) | loadwave/v7.c |
| tire_compound_set (0x800A5BB8) | InitWaves@waves.c (proven, w36n) | 372 | **MATCH** standalone + unit EQUAL | cloud/matches/tire_compound_set.c |
| suspension_setup (0x800A501C) | ReplaceWave@waves.c (strong, w36n; confirmed by flow) | 240 | 1 row: only `move s8,a3` (func_80097164's s8 param, SetupWaveModel-gated) | replacewave/v1.c |
| sound_bank_unload (0x800B10D4) | N64-only car palette builder ("C%dPALETTE"; no GC/DC donor) | 1248 | 134/312 (w3b best, unchanged); notes | palette/ |
| SetupWaveModel (0x80096CA8) | _SetupWaveModel@waves.c (conf) | 1204 | unchanged (w29d best, 233 rows); GC range-test re-run: 249 rows; gate for 3 more functions | ../w31d/SetupWaveModel/v/unit_best.c |
| audio_doppler_calc (0x800B6788) | _DrawText@HAL_text.c (GC 0x8001B410; proven by flow) | 1124 | **group MATCH + unit EQUAL** (supersedes codex_defaults_a16 in place) | groups/codex_defaults_a16/ |
| func_80098FB8 | MAT3 AudioMain (+ inlined _PeriodicMaintenance, _ProcessPriorities, update_reverb, stats stubs) | 756 | **provisional: unit EQUAL** with its real (unmatched) caller entity_ai_pathfind | thread/best.c |
| audio_bus_mix (0x800B4818) | SavePlayerOption@save.c (proven, w36n) | 720 | **group MATCH + unit EQUAL** (with partner voice_stop_2 = _SaveRaceSetting, 224 B) | groups/w36t_save_option/ |

Landable now (6 functions, 5,236 B): entity_process_main 1424 + tire_compound_set 372 (cloud/matches singles),
audio_interrupt_handler 1372 (group w36t_loadcars), audio_bus_mix 720 + voice_stop_2 224 (group w36t_save_option),
audio_doppler_calc 1124 (codex_defaults_a16 superseded in place). Provisional / gated: func_80098FB8 756 (unit EQUAL,
needs the thread), LoadWave 520 (4 rows) and ReplaceWave 240 (1 row) - both only func_80097164's s8 parameter,
i.e. SetupWaveModel's convention.

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36t (from base, master e68aea6e1).

## Identities proven

| N64 | MAT3 / role | evidence |
|---|---|---|
| entity_ai_pathfind | N64 audio-manager thread (no GC counterpart); body = audiomgr.c _ProcessCommand inlined | osCreateMesgQueue/osScAddClient/recv loop; 12-case switch = MAT3 cases 0-11 field-for-field |
| func_80098FB8 | audiomgr.c AudioMain (+ _PeriodicMaintenance, _ProcessPriorities inlined) | unit EQUAL reconstruction |
| engine_sound_sync | audiomgr.c _ProcessEmitters | called first in _PeriodicMaintenance |
| func_80098710 | _WeightedPriority; entity_state_check = AudioEffectPlaying | _ProcessPriorities calls |
| camera_update_a / camera_update_b | bstree.c _WalkBST / _InsertBSTE | WalkBST/InsertBSTE inlined, same args |
| sound_position_update (0x800951E0) | audiomgr.c _UpdateFx | WalkBST callback |
| camera_transform | AudioPeriodicMaintenance (N64 audio server) | called under the mutex in AudioMain |
| func_800979A0 / func_80096238 | AudioSongPlay / AudioSongStop | _ProcessCommand cases 0/1 |
| audio_state_save, audio_timing_sync | AudioPlayEffect, AudioStopEffect | cases 3/5/6 |
| audio_buffer_manage, audio_stream_control, sound_priority_set, audio_bus_route | AudioSetEffectVolume / LRPan / FBPan / Pitch | case 4 order |
| func_800958B8, music_track_control, gfx_setup_e700 | AudioStopAllEffects, AudioSetMasterVolume, AudioSetMono | cases 7/10/11 |
| audio_effect_setup, audio_pitch_adjust | _FreeFxTag, _InterruptFxTag (IPA s0) | bodies |
| func_8009211C / func_80091FBC | RemoveLLE / InsertLLE | _PlayFxTag |
| D_80142728 / D_801427A8 / D_80142B70 | audio mutex queue / audio command queue / its 128 messages | thread |
| D_8011023C, D_80110240, D_80110248, D_8011024C, D_80110250, D_80110278/7C/80 | gInited, gSong, gSortedFxTags, gNumStartCmds, gNumStopCmds, gNumFxPlaying/Pending/Interrupted | AudioMain/_ProcessPriorities |
| D_80143AE8 / D_80143CF0 / D_80143F58 / D_80144020 / D_80144C50 / D_80149860 / D_80152748 | gStartCmds / gStopCmds / gSortedFxTagTree / gPlayingFxTags / gInterruptedFxTags / gActiveSequences / audio clock | same |
| entity_process_main | visuals.c AnimateShadow | MATCH |
| func_8008D098 / func_8008D084 / func_8008D0B0 | zoidpoly.c ZOID_ClearPolyFlags / ZOID_SetPolyFlags / ZOID_ShowPoly | bodies; MATCH lever |
| audio_interrupt_handler / func_8009768C | carload.c LoadCars / waves.c FindWaveInstance (deleted stub) | group MATCH (Astra #404 source) |
| audio_frame_sync | waves.c LoadWave (+ _NewWaveInstance inlined; func_800972BC = its deleted stub) | 4 rows |
| func_800972C4 | waves.c _SetWaveInstanceModel (the w36n map has the labels of 800972BC/800972C4 swapped) | body: model = gWaveModel[wave]; gNumModels |
| func_80096288 | waves.c _ValidateWaveInstance (`if (ready) {}`) | ReplaceWave call (idx,1,1) |
| D_80035440 / D_80156BB0 / D_801569AC | background loader idle token / request queue / instance to read | LoadWave, InitWaves |
| tire_compound_set / func_800A5BB0 | waves.c InitWaves / deleted _BGLoadWaveCheck stub | MATCH |
| car_damage_visual / func_800A51D8 | memsys.c ClearMemoryHeap / _ConvertHeap | body |
| suspension_setup | waves.c ReplaceWave (+ _ReplaceWaveInstance inlined) | 1 row |
| audio_bus_mix / voice_stop_2 | save.c SavePlayerOption / _SaveRaceSetting (IPA t0) | group MATCH |
| func_800B4720/28/30 | deleted _SaveBattleSetting/_SaveStuntSetting/_SaveObstacleSetting stubs | group object layout |
| D_8014978C | gTrackNum (s8) | SavePlayerOption |
| audio_doppler_calc | HAL_text.c _DrawText (IPA s2 text, s0 length) | group MATCH |
| func_80087110 / func_8008A148 / func_8008A3E4 / func_800878E0 / func_8008705C | BlitDraw / BlitColors / BlitScissor / BlitSetAttribute / BlitClearAttribute | _DrawText calls |
| D_801497F0, D_80149800, D_80149820, D_80149878, D_80149B48, D_80149B60, D_80149B70, D_80149B88, D_80149B08, D_80149B28, D_80149D92/9E, D_80114748, D_8012E6D0 | gFontHeader, gFontLetters, gFontTextures[16], gCharMap[256], gFontColor, gLeading, gSpacing, gFontAttribute, font tex format, font tex size, gFontXpos/Ypos, gTrans, current blit palette | _DrawText |

## Permission denials

None so far.

## Added by coordinator (w36n map)
audio_frame_sync = LoadWave (520), tire_compound_set = InitWaves (372), func_80097164 = _ReadWave (344),
suspension_setup = ReplaceWave (240).

## Thread pair notes (entity_ai_pathfind + func_80098FB8)
- Must be scored together: func_80098FB8 is internal (unsaved s0-s8, f20/f22), only caller the thread.
  `blob_unit score entity_ai_pathfind func_80098FB8 --internal func_80098FB8 --with thread/vN.c`.
- func_80098FB8 levers found: `_SendCmd(cmd)` inline helper for the two command-send loops (arg order
  lw a1 before move a0); `gNumFxPending = gNumFxInterrupted = 0;` (one chained store fixes the t6-t9 ring
  shift and the s4/s5 address colours). Remaining: frame 184 vs 128 = 7 more inline sites (4 after
  WalkBST in program order, 3 before; each inline site = 8 bytes, sites after WalkBST push `num` up 8).
- Thread: lock/unlock inside _ProcessCommand (cmd binding before the mutex recv); `_FreeCmd`,
  `GetAudioTime`, lock/unlock inline helpers bring frame 152 -> 184 (retail 208).

### func_80098FB8: provisional unit EQUAL (thread/best.c)
`python3 -m tools.conveyor.pipeline.blob_unit --tag w36t score entity_ai_pathfind func_80098FB8 --internal func_80098FB8 --with cloud/work/frontier/w36t/thread/best.c`
-> `EQUAL func_80098FB8: 189 words (internal, c_best.c)`  /  `FAIL entity_ai_pathfind: 461 of 545 words differ`.
Provisional: func_80098FB8 is a private-IPA callee (s0-s8, f20, f22 unsaved) whose only caller is the thread;
it lands only together with entity_ai_pathfind (member, internal, not on keep).
Reconstruction (MAT3 audiomgr.c, GC AudioMain 0x800A1E60 / _PeriodicMaintenance 0x800A1EA4 / _ProcessPriorities
0x800A3208; bstree.c ClearBST/InsertBSTE/WalkBST): func_80098FB8 = AudioMain { statsUnclock(6); statsClock(4);
_PeriodicMaintenance(); update_reverb(); statsUnclock(4); statsClock(6); } with stats* empty on N64 (no code, frame
evidence: 7 inline sites, 3 before and 4 after WalkBST in program order) and update_reverb empty as in MAT3;
_PeriodicMaintenance { if (gInited) { _ProcessEmitters(); _ProcessSequences(); _ProcessPriorities(); }
AudioPeriodicMaintenance(); } where AudioPeriodicMaintenance = inline { lock; camera_transform(); unlock; };
_ProcessPriorities takes the mutex (osRecvMesg D_80142728), releases it before sending the stop/start commands
to the thread queue D_801427A8 through an inline _SendCmd. Levers: `gNumFxPending = gNumFxInterrupted = 0;`
(chained store: fixes the t6-t9 ring and the s4/s5 address colours); `int num = 0; camera_update_a(...)` on one
line in WalkBST (as1 tie by source line puts `sw zero,76(sp)` after the a3 materialisation).
Device disclosure: the four stats stubs are empty static functions (N64 build has no stats timers here); they
are frame evidence only. No unused locals, no pad.

### entity_ai_pathfind (audio thread): blocked, written up
Identity: N64-only audio manager thread (no GC counterpart, as w36n says): osCreateMesgQueue(D_801427A8, 128),
osScAddClient(scheduler D_8002E8E8), loop: recv; retrace (type 1) -> AudioMain (func_80098FB8); else
_ProcessCommand(cmd) inlined = MAT3 _ProcessCommand's twelve cases (N64 callees mapped: func_800979A0 =
AudioSongPlay, func_80096238 = AudioSongStop, audio_state_save = AudioPlayEffect, audio_timing_sync =
AudioStopEffect, audio_buffer_manage/audio_stream_control/sound_priority_set/audio_bus_route =
AudioSetEffectVolume/LRPan/FBPan/Pitch, func_800958B8 = AudioStopAllEffects, music_track_control =
AudioSetMasterVolume, gfx_setup_e700 = AudioSetMono, audio_effect_setup = _FreeFxTag, audio_pitch_adjust =
_InterruptFxTag, func_8009211C/func_80091FBC = RemoveLLE/InsertLLE). Structure, case order, every call and
offset are right; frame 192 vs 208 (retail has 2 more inline sites + the type var, or 3 sites).
Blocker: retail case 0 forwards `gSong = cmd->song` into the call (`move a0,v1`, &gSong in v0) and case 4/others
keep reloads only on call paths (PRE). Our compile of the full 12-case body reloads gSong and puts the reloads
in the join blocks; this also removes retail's separate `b end` blocks. It is a uopt threshold, not source:
the same source with any of cases 4, 5, 8+9 emptied (probe only) forwards (`move a0,v1`); emptying 1, 6, 10 or
11 does not; -Olimit 50000 does not help. Retail's regime is in between (forwarding, but &gSong not hoisted
into an s-register). Next: find the uopt limit (bit-vector/web count) with the trace toolkit, or a source form
of one case that shrinks uopt's measure. Also open: `lh v1` type compare is a coloured web in retail with the
constant first (`bne s6,v1`); an explicit `s32 type` local gives v1 but `bne v1,s6` and +8 frame.

### audio_interrupt_handler = LoadCars: group MATCH (groups/w36t_loadcars/)
Builds on Astra's research packet #404 (its LoadCars source is used unchanged; credit Astra). Astra left claims
empty because the context LoadWave did not match; this project installs groups with non-matching context
(engine_sound_sync precedent), so the group claims LoadCars. Context LoadWave replaced by my typed port.
- `score.py group cand/w36t_loadcars --claims` (builder): `audio_interrupt_handler: MATCH`, `func_8009768C: MATCH`,
  exit 0 (context: every locked body MATCH, audio_frame_sync `130/130 words differ` informational).
- `blob_unit --tag w36t score audio_interrupt_handler func_8009768C --internal func_8009768C --with root.c --with
  resource.c --with loadwave.c --neighbours`: `EQUAL audio_interrupt_handler: 343 words (kept, c_root.c)`,
  `EQUAL func_8009768C: 2 words (internal, c_root.c)`, `locked bodies that differ in this unit: 0`.
- Integration: new group `w36t_loadcars`; members/claims audio_interrupt_handler, func_8009768C (the locked 2-word
  stub single gets its real FindWaveInstance body; unit_overrides force_internal func_8009768C); keep as group.json.

### audio_frame_sync = LoadWave: 4 rows (loadwave/v7.c)
Typed port of MAT3 LoadWave with _NewWaveInstance inlined (no `return -1`: retail's exhausted-loop path returns
v0 unchanged, the frontier's `register_params: v0`) and the N64 background hand-off as an inlined
`_BGLoadWave(idx)` (msg lands at 68(sp) as retail). A used `winst` local in the found path gives frame 112.
`blob_unit --tag w36t score audio_frame_sync --internal func_80097164 --internal SetupWaveModel --with
loadwave/v7.c --with ../w31d/SetupWaveModel/v/unit_best.c`: `want 130 words, got 130; differing rows 4; frame
112/112` - the only difference is `move s8,a3` (retail) vs `move s5,a3`: func_80097164's slot parameter register,
which in retail is s8 because SetupWaveModel takes idx in s8 (and first in t0). Same blocker as the CLAIMS
resource-loader row (SetupWaveModel's idx colour): LoadWave lands as soon as SetupWaveModel/func_80097164 do.

### entity_process_main = AnimateShadow: MATCH (cloud/matches/entity_process_main.c)
- builder `score.py fn cand/entity_process_main.c entity_process_main --flags "-g0 -O3 -mips2 -G 0 -non_shared"`:
  `entity_process_main:` `MATCH` `own .rodata verified at 0x80123938..0x8012393C`.
- `blob_unit --tag w36t score entity_process_main --with cloud/matches/entity_process_main.c --neighbours`:
  `EQUAL entity_process_main: 356 words (kept, c_entity_process_main.c)`, `locked bodies that differ in this unit: 0`.
- Lever (MAT3): GC calls ZOID_ClearPolyFlags(&PolyInstanceList[v->objnum], 1<<car->camera) and ZOID_SetPolyFlags/
  ZOID_HidePoly/ZOID_ShowPoly (zoidpoly.c; N64 out-of-line copies func_8008D098/func_8008D084/func_8008D0B0). As
  inlined u16-parameter helpers with `1 << car->unk35C` (camera read twice) the clear arm evaluates shift, load,
  NOT as retail - the 2-word residual of w20e/w26e. The 3 inline sites (+24 frame) replace filler locals; the
  scale-row pointer + DEBUG_PRINT holder dropped (GC `shadow_scale[body][k]` indexing). Remaining disclosed
  unused locals: pos[3], mat[3][3], d2 (retail 280, without them 224).
- Identity proven: entity_process_main = AnimateShadow@visuals.c; func_8008D098 = ZOID_ClearPolyFlags,
  func_8008D084 = ZOID_SetPolyFlags, func_8008D0B0 = ZOID_ShowPoly (zoidpoly.c; bodies identical to GC).

### audio_bus_mix = SavePlayerOption + voice_stop_2 = _SaveRaceSetting: group MATCH (groups/w36t_save_option/)
First-draft typed port of MAT3 save.c (SavePlayerOption, _SaveRaceSetting; _SaveBattleSetting/_SaveStuntSetting/
_SaveObstacleSetting inlined; their deleted stubs are the locked singles func_800B4720/28/30, which the group
object reproduces as unnamed stubs after format_string_parse, as in retail). N64 save layout: no raceType byte
(raceSettings at 0x48, battle 0x5C, stunt 0x6C, obstacle 0x80), profile datah at +0x2C, gTrackNum = D_8014978C.
voice_stop_2 (not on any lane list) is audio_bus_mix's private IPA partner (player in t0, option a0, value a2, the
unused track argument stored in the caller's outgoing slot 12(sp)), so it is a member too.
- builder `score.py group cand/w36t_save_option` (and `--claims`, exit 0): `audio_bus_mix: MATCH own .rodata verified
  at 0x80123C64..0x80123CE4`, `voice_stop_2: MATCH own .rodata verified at 0x80123C38..0x80123C64`; context
  format_string_parse `MISMATCH (3 extra words ...)` = the three deleted-helper stubs that follow it (retail
  func_800B4720/28/30, locked singles), informational.
- `blob_unit --tag w36t score audio_bus_mix voice_stop_2 --internal voice_stop_2 --with save_option.c
  --neighbours`: `EQUAL audio_bus_mix: 180 words`, `EQUAL voice_stop_2: 56 words (internal)`, `locked bodies that
  differ in this unit: 0`.
- Integration: new group `w36t_save_option`, members/claims [audio_bus_mix, voice_stop_2], keep [audio_bus_mix],
  context format_string_parse (codex_hash_a80.c = byte-identical copy of src/blob/groups/codex_hash_a80/group.c;
  it must NOT be on keep: in the unit it is internal and its callers keep a3/t0/v1 live across it);
  unit_overrides force_internal voice_stop_2. No shaping devices.
- Identities proven: voice_stop_2 = _SaveRaceSetting@save.c; func_800B4720/func_800B4728/func_800B4730 = the
  deleted inlined statics _SaveBattleSetting/_SaveStuntSetting/_SaveObstacleSetting (order not pinned).

### tire_compound_set = InitWaves: MATCH (cloud/matches/tire_compound_set.c)
- builder `score.py fn cand/tire_compound_set.c tire_compound_set --flags "-g0 -O3 -mips2 -G 0 -non_shared"`:
  `tire_compound_set:` `MATCH`.
- `blob_unit --tag w36t score tire_compound_set --with cloud/matches/tire_compound_set.c --neighbours`:
  `EQUAL tire_compound_set: 93 words (kept, c_tire_compound_set.c)`, `locked bodies that differ in this unit: 0`.
- First draft = MAT3 InitWaves + N64 _BGLoadWaveCheck (real: NOBLOCK recv == -1 -> BLOCK recv; send back) +
  inlined ZOID_Reset (copy of locked func_800A5B3C). Frame 56 -> 64 needed one more inline site after
  _BGLoadWaveCheck: `ClearMemory()` wrapper around car_damage_visual(0) (= memsys ClearMemoryHeap, erased static
  on GC) - disclosed hypothesis; an extra local gives frame 64 but leaves msg at 44 (retail 52).
- Identities proven: car_damage_visual = ClearMemoryHeap@memsys.c (body: frees unlocked purgeable chunks via
  func_800A51D8 = _ConvertHeap, then consolidates); func_800A5BB0 = deleted _BGLoadWaveCheck stub.

### suspension_setup = ReplaceWave: 1 row (replacewave/v1.c)
MAT3 ReplaceWave with _ReplaceWaveInstance inlined (func_80096288 = _ValidateWaveInstance(idx,1,1), no texture
loop on N64). `blob_unit ... score suspension_setup --internal func_80097164 --internal SetupWaveModel --with
replacewave/v1.c --with ../w31d/SetupWaveModel/v/unit_best.c`: `want 60 words, got 60; differing rows 1` -
`move s8,a3` vs `move s5,a3` only, the same SetupWaveModel-convention gate as LoadWave and func_80097164.
**SetupWaveModel's idx-in-s8 / first-in-t0 convention now gates 4 functions: SetupWaveModel 1204 +
func_80097164 344 + LoadWave 520 + ReplaceWave 240 = 2,308 B.**

### sound_bank_unload (car palette builder): no MAT3 donor, w3b best stands (134/312, rows 198, norm 24)
- Format string at 0x80123498 is "C%dPALETTE" (not in MAT3/DC). w3b's draft calls osJamMesg for the heap-lock
  release; retail calls osSendMesg (0x800075E0; osJamMesg is 0x80007440) - fixed in palette/v3.c, no count change.
- The tail is memsys DeallocateMemory(colors) = _GetSemaphore (osRecvMesg D_80152770) + _DeallocateMemory
  (audio_reverb_update, IPA a1/a2) + _ReleaseSemaphore (osSendMesg) - as inline helpers they explain 24 of w3b's
  40-byte `pad[10]` (frame 160 without pad, 192 retail) but must be non-static (memsys.c, no stubs in retail).
- A per-element `_BrightenColor()` inline helper (w3b: retail's loop body is ~one statement bigger, unroll 2) is
  NOT inlined by umerge in the shadow unit (body 164 words, helper out of line) - possibly the unit's smaller
  umerge budget (w31d note). Not pursued further (no donor; register/ring residual).

### audio_doppler_calc = _DrawText: group MATCH (groups/codex_defaults_a16/, superseding in place)
Typed port of MAT3 HAL_text.c _DrawText; private IPA callee (text in s2, length in s0) of HAL_FinishText
(world_velocity_integrate, w36o) and HAL_DrawText (menu_input_process, w36v), both present in the unit as the
installed codex_defaults_a16 context bodies. N64 differences: _SetupFontData(0) = sound_update_channel (IPA t0),
BlitScissor(0,0,screenW-1,screenH-1) (D_8002AFC0/C4), BlitClearColors written inline (`D_8012E6D0 = 0`; as an
inline helper the frame is 8 too big), gFontColor alpha = gTrans (float -> u8 unsigned conversion),
BlitColors(&gFontColor, gFontFormat D_80149B08, gFontSize D_80149B28, 0), texture page load (object_render) for
page 0 and on each page change, BlitDraw (Astra's func_80087110, unmatched callee) has no texture argument and no +1.
Levers: newline arm order `y0 += ...; x0 = gFontXpos; ch = -1;` (as1 order); `x = (nonProp ? ... : 0) + x0;` as one
expression (the centring value is a separate v1 web, retail `addu a0,v1,s2`) - from 38 rows to EQUAL in 9 variants.
- `blob_unit --tag w36t score audio_doppler_calc --internal audio_doppler_calc --with drawtext/best.c --neighbours`:
  `EQUAL audio_doppler_calc: 281 words (internal, c_best.c)`, `locked bodies that differ in this unit: 0`.
- builder `score.py group cand/codex_defaults_a16`: `func_800B6748: MATCH`, `audio_doppler_calc: MATCH`;
  `--claims`: `audio_doppler_calc: MATCH`, exit 0. Context menu_input_process/world_trigger_check/
  world_velocity_integrate differ (unchanged non-matching context), sound_update_channel MATCH.
- Integration: supersede src/blob/groups/codex_defaults_a16 in place: group.c byte-identical, old member
  func_800B6748 kept, members += audio_doppler_calc (drawtext.c), files += drawtext.c, slot_sound.c (byte-identical
  copy of src/blob/groups/slot_sound/group.c, for sound_update_channel's IPA t0 parameter), context +=
  sound_update_channel, keep unchanged, claims [audio_doppler_calc]; unit_overrides force_internal audio_doppler_calc.
- This gives w36o (HAL_FinishText) and w36v (HAL_DrawText = menu_input_process) their callee convention.
- Identities proven: audio_doppler_calc = _DrawText; func_80087110 = BlitDraw (x0,y0,x1,y1,s,t; Astra's);
  func_8008A148 = BlitColors(colors, fmt, siz, bank); func_8008A3E4 = BlitScissor; func_800878E0 = BlitSetAttribute;
  func_8008705C = BlitClearAttribute; D_8012E6D0 = current blit palette (BlitClearColors); data: D_801497F0
  gFontHeader, D_80149800 gFontLetters, D_80149820 gFontTextures[16], D_80149878 gCharMap[256], D_80149B48
  gFontColor, D_80149B60 gLeading, D_80149B70 gSpacing, D_80149B88 gFontAttribute, D_80149D92/9E gFontXpos/Ypos,
  D_80114748 gTrans.

### Thread follow-up: the forwarding cutoff is a uopt basic-block threshold (measured)
`uopt -v` prints a per-procedure size (basic blocks): full 12-case thread = 155 (no forwarding); variants with
cases emptied: 154 (case 10 or 11), 152 (case 1), 151 (k10), 149 (case 6) -> no forwarding; 147 (case 5), 139
(k9), 135 (case 4), 134 (cases 8+9) -> forwarding (`move a0,v1`, retail-like). So uopt stops store->load
forwarding/PRE between 148 and 149 blocks. Retail's thread therefore had <= 148 blocks while producing the same
control flow (including its separate `b end` blocks) - our source has >= 7 blocks too many. Removing the four
inline helpers saves 1 block; rewriting case 6 as if/else saves 2 but changes code (one free call instead of two).
Next: find 7 code-free blocks (candidates: the retrace if/else + type variable, the case-2 search-loop shape,
inline-site return labels) - measure each with `uopt -v` (scratchpad qgv.sh pattern: single-file -O3 group with
`uopt -v`), not with the full unit.

### SetupWaveModel (no new best)
Re-ran w31d's GC in-loop range test (`if (gWaveInstance[idx].type >= 64 && <= 76) {}`) in the current unit with my
LoadWave/ReplaceWave callers: 249 rows, idx still arrives in memory, first -> s7 (ctrace swA: first w11 save 17.0
tot 323 nocs 19 -> s7; idx param piece w207 save 16.67 -> s8; idx whole web w3 tot 15 vs best 110 -> split).
Retail facts re-read from the words: idx stays in s8 for the whole body (MirrorWaveInstance gets `move a0,s8`),
`n` = idx stored at 68(sp) bounds the i/j loops via s7, first is t0 spilled at 116(sp) around every call and tested
3x per j iteration. It now gates LoadWave + ReplaceWave + func_80097164 + itself (2,308 B).

## What generalises (w36t)
- **Cross-file inlined helpers from the MAT3 tree are the first lever for "evaluation order" residuals**: GC calls
  ZOID_ClearPolyFlags(poly, 1 << camera) - as an inlined u16-parameter helper the argument is evaluated before the
  body's load, the order no single-expression form gave (entity_process_main: 2 words -> MATCH). Check the GC body
  for calls to tiny functions (zoidpoly.c, memsys.c, bstree.c) where the N64 shows open code.
- **Each inline site = 8 frame bytes; sites later in program order sit at lower addresses** (push earlier locals up).
  Use the slot of a known local (msg, num) to decide whether a missing site comes before or after (LoadWave,
  InitWaves, func_80098FB8: frame-exact with genuine helpers instead of pad/unused locals).
- **Inlined static helpers leave unnamed deleted stubs; the retail locked 8-byte stubs tell you which statics
  exist** (func_800B4720/28/30, func_800972BC, func_800A5BB0, func_8009768C).
- **A whole-program-internal callee must not be kept in a group** (format_string_parse is internal in the unit;
  keeping it in the group dropped its IPA "preserves a3/t0" fact and broke both members).
- **uopt stops store->load forwarding/PRE at ~148 basic blocks** (`uopt -v` size): a near-miss whose retail shows
  forwarding/PRE that our compile lacks may be over that threshold - measure with `uopt -v` before tuning.
- **Chained assignments are a code-free ring/colour lever**: `gNumFxPending = gNumFxInterrupted = 0;` fixed a
  t6-t9 ring shift and an s4/s5 address-colour swap at once (func_80098FB8).
- Non-matching but genuine context callers/callees installed in groups (engine_sound_sync, codex_defaults_a16)
  let IPA partners land without their partner: _DrawText and LoadCars both landed this way.
