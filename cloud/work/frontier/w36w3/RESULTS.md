# w36w3 results (wave 36, game pool: unassigned small game functions, continuation of w36w)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36w3 (copied from base, master e68aea6e1).
Repo HEAD at start: fb77b6f9e (branch wave35). All scores strict.

## Summary

Matched (game; all with blob_unit confirmation, 0 locked bodies differ): 5 functions, 1,952 B
- groups/controller_poll/ (superseded in place): func_800E7134 676 (N64 pad reader)
- groups/w36w3_dust/ (new group): dust_cloud_effect 600 (N64 hud.c blit anim; 5 disclosed unused locals)
- groups/audio_options_menu/ (superseded in place): credits_screen 296 (= uninitScreen@audioscr.c)
- cloud/matches/func_800E7038.c (standalone): func_800E7038 252 (N64 controller init)
- cloud/matches/func_8010FBE0.c (standalone): func_8010FBE0 128 (scheduler RSP-task submit)
Provisional (disclosed stand-in caller): func_800A3724 88 (EQUAL internal; car_lod_select 21 words).
Near: func_800979A0 3 words (volatile-cast device), func_800E73D8 4 words (register only), world_trigger_activate
25, sound_position_update 51 (structure exact), world_collision_response 84.

Combined unit run (all five together): `blob_unit --tag w36w3 score func_800E7038 func_800E7134 dust_cloud_effect
credits_screen func_8010FBE0 func_800D7634 controller_poll --internal credits_screen --internal us_uninitButtons
--internal us_AudKillSong --internal us_ProcessCommand --neighbours --with cloud/matches/func_800E7038.c --with
cloud/work/frontier/w36w3/e7134/unit_7134.c --with .../groups/w36w3_dust/dust.c --with
.../groups/audio_options_menu/uninit_w36w3.c --with cloud/matches/func_8010FBE0.c` -> `7/7 equal`, `locked bodies
that differ in this unit: 0`.

KEY LEVER (new, generalises): a global referenced by ONE function only is often a FUNCTION-LOCAL static. As an
extern/defined global, uopt builds an lda web for its address and shares constants between its stores; as a local
static it is a direct variable reference. `tools/xref.py ADDR...` lists the functions that build %hi/%lo of an
address. It closed func_800E7038 and func_8010FBE0 (w15d/w16e/w20e had proven "no lda web" but found no spelling)
and moved world_trigger_activate/func_800E73D8.

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_800E7134 | N64-only pad reader (no MAT3 ancestor) | 676 | **group MATCH + unit EQUAL** (controller_poll superseded in place) | groups/controller_poll/ |
| dust_cloud_effect | N64 hud.c blit anim (MAT3 name open) | 600 | **group MATCH + unit EQUAL** (new group w36w3_dust; 5 disclosed unused locals) | groups/w36w3_dust/ |
| func_800E73D8 | N64-only pad read (osContStartReadData; no MAT3 ancestor) | 596 | 4 words: only the local static counter's register (v1 vs retail v0); own .data verified (init 29) | func_800E73D8/best.c |
| world_collision_response | N64 gfx task submit (OSScTask; no MAT3 ancestor) | 540 | 84 words (unit), ops 17: head/GBI right; register choices (a3/t1, v0/v1 webs) + as1 order | world_collision_response/best.c |
| world_trigger_activate | N64 gfx-thread message handler (no ancestor) | 464 | 25 words (one-file group; frame exact): &D_80114730 web a0 vs v1, counter value web | world_trigger_activate/best.c |
| world_effect_update | InitGraphics@Init.c (N64; = game_late_init's inlined body) | 456 | not advanced: w19e best 12 words stands (block boundary after the prologue); InitGraphics helper form 35 rows | ../w19e/world_effect_update/best.c |
| sound_position_update | _UpdateFx@audiomgr.c (proven: body, constants 0.75/0.2/0.1, AudioTimeout inline) | 444 | 51 words (unit), ops 2: structure exact; colour + constant-1 web | sound_position_update/best.c |
| car_lod_select | PS_InitPakSys@paksysdc.c (N64 form; NEW, strong) | 324 | 21 words (one-file group w/ disclosed stand-in): list/end address webs swapped (s3/s4) | car_lod_select/best_standin.c |
| display_enable | (private IPA, s0-s4 unsaved) | 316 | not worked: only caller playgame_state_change (w36p) unmatched -> provisional at best | - |
| credits_screen | uninitScreen@audioscr.c (NEW, proven) | 296 | **group MATCH + unit EQUAL** (audio_options_menu superseded in place) | groups/audio_options_menu/ |
| func_800979A0 | N64 sequence start (audio thread; no ancestor) | 292 | 3 words (unit + group) with a volatile-cast final store (device); plain form 11 | func_800979A0/group_volatile_store/ |
| render_thread_entry | N64 model-thread entry (no ancestor) | 268 | w27a 65/67 rescored; s1=const 1 / s2 saved-unused residual not found; needs `align` override | ../w27a/render_thread_entry/best.c |
| func_800E7038 | N64-only controller init (no MAT3 ancestor) | 252 | **standalone MATCH + unit EQUAL** (own .data verified) | cloud/matches/func_800E7038.c |
| func_800A3640 | paksys (caller track_render_process = PS_PakServer lead) | 204 | not worked: provisional (w8b); only caller track_render_process (w36p) unmatched | - |
| brake_force_apply | N64 wave-loader thread entry | 196 | blocked: 2 aligned rows only via _ReadWave's s8 param (func_80097164, w36x5) + needs `align` override | (installed audio_frame_sync context body) |
| func_800B0EA0 | RGBA5551 lerp | 192 | not worked: already provisional MATCH as internal callee (PR #87); closes with its caller sound_bank_unload (w36t) | - |
| func_800B66B0 | _TextLen (w36v, provisional) | 152 | not worked: provisional MATCH exists (agentA); closes with its caller menu_input_process (w36v) | - |
| func_8010FBE0 | N64 scheduler RSP-task submit (no ancestor) | 128 | **standalone MATCH + unit EQUAL** (local static OSScTask) | cloud/matches/func_8010FBE0.c |
| func_800A3724 | _ClearPak@paksysdc.c (N64 form) | 88 | provisional EQUAL (unit, internal) with a disclosed stand-in caller (retail 2nd caller track_render_process unmatched) | car_lod_select/best_standin.c |
| object_bytes_sum_global | TB_GetFontLineHeight | 84 | not advanced: w36w 4 words colour-only (&gFontHeader t2 vs t4) | ../w36w/text/best_object_bytes_sum_global.c |
| object_bytes23_sum | TB_GetFontHeight | 72 | not advanced (same t4 cause) | - |

## Identities proven

- credits_screen (0x800D6698) = uninitScreen@audioscr.c: sole caller updateAudioScreen (func_800D7634) at two
  sites like GC; body = AudKillSong + uninitButtons (12 buttons, objIdx -1, buttonsInited 0) + RemovePanes(0) /
  gTitlePane / RemoveBlit / removeBG / panesCreated / inited; group + unit MATCH.
- sound_position_update (0x800951E0) = _UpdateFx@audiomgr.c: 0.75f*gMaxSfx, AudioTimeout(startTime, 0.2f)
  (func_80095198 inlined), stop cmd 5 / start cmd 3 queues, 0.1f vol, type == 1; structure exact in the unit.
- car_lod_select (0x800A4508) = PS_InitPakSys@paksysdc.c (N64 form: errorFunc, free-file list, per-pak InitLL /
  damaged = 0 / _ClearPak, pak thread, gInited = 1, _CompleteCallback slot func_800A43FC).
- func_800A3724 (0x800A3724) = _ClearPak@paksysdc.c (memset the 772-byte pak keeping byte 5).
- world_effect_update (0x800EE8B4) = InitGraphics@Init.c (N64): the same _InitGfx / ZOID_Init / InitBlits /
  InitCameras body that the matched game_late_init (InitSystem, w36o initsystem.c) inlines.
- func_800E7134 / func_800E7038 / func_800E73D8 / render_thread_entry: the N64 model-thread controller layer (pad
  reader / controller init / per-frame read / thread entry), N64-only; func_800E7710 (matched) is its init.
- dust_cloud_effect (0x800EFEA8): N64 hud.c blit anim right before ShowHUD (MAT3 name open).
- object_type_byte2/3 note: w36n's map row func_800A473C = InitLL@ll.c is wrong (it is strcpy).

## Permission denials

none

## Notes per function


### func_800E7038 (252 B): MATCH (standalone + unit)
- Body: `if (!D_80111954) { player_state_set(-1,1); player_mode_set(-1,1); } D_80111954 = 1;` reset of the
  per-controller stick calibration gStickCal (D_80149AF8 s8[4][2]) to 70 in a 4-trip loop that IDO unrolls fully,
  the once-only SI lock queue init (osCreateMesgQueue(&D_801497A8,&D_80152730,1); osSendMesg) and a busy wait while
  the osContInit bit-pattern byte D_80111950 == 0x80, then controller_poll().
- KEY LEVER (generalises): the queue's created-once flag 0x80111968 is a FUNCTION-LOCAL static. xref scan
  (tools/xref.py): only func_800E7038 references 0x80111968 (and only func_800E73D8 references 0x80111964). As a
  global, uopt makes two 2-occurrence webs (its address, and the constant 1 shared by both flag stores; save 0.5,
  tot 1.0, best 0 -> coloured v0/v1, 46 words); retail splits both (force.sh `p1:w59=s,p1:w60=s` -> 0 rows). A
  local static is a direct variable reference in uopt (no lda web; its constant store is remat), giving retail.
- Other levers: unrolled loop (8 separate `li tN,70`; straight statements share one 70 web); store order [1] then
  [0] (ugen ring t8,t9,t0..t5); gStickCal DEFINED in the file (as1 shares one `lui at` over the 8 stores only for a
  symbol defined in the unit; extern gives one lui per store); the two stores on ONE line (two lines: as1 swaps the
  last pair, 2 words).
- Scorer: `score.py fn cand/func_800E7038.c func_800E7038 --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `MATCH` /
  `own .data verified at 0x80111968 (4 bytes, all zero; .data is laid out per translation unit)`.
  Unit: `blob_unit --tag w36w3 score func_800E7038 --neighbours --with cloud/matches/func_800E7038.c` ->
  `EQUAL func_800E7038: 63 words (kept, c_func_800E7038.c)`, `locked bodies that differ in this unit: 0`.
- Integration note: the file defines `s8 D_80149AF8[4][2];` (tentative definition, as the controller_poll prelude
  does for D_80149B74); the unit accepted it. The installed controller_poll group keeps its old func_800E7038 text as
  context; nothing there needs to change.

### func_800E7134 (676 B): MATCH (group + unit), controller_poll superseded in place
- N64-only (the GC/arcade input layers differ): under the SI lock (osRecvMesg/osSendMesg on D_801497A8), init via
  func_800E7038 if D_80111954 is clear, then per controller: record D_80156CF0[i] (16 B: s8 connected, u32 buttons
  @4, s8 stick[2] @8; written by func_800E73D8 from osContGetReadData); disconnected -> raw stick 0, calibration 70;
  connected -> copy buttons/stick, stick-as-dpad bits (D_80149B74[i]: x < -56 -> 0x1000, x > 56 -> 0x2000, y < -56
  -> 0x800, y > 56 -> 0x400), button 0x40000 resets calibration, calibration grows to 127; then gButtons
  (D_8013FED0) = buttons, pressed = (gHeld ^ buttons) & buttons, gPrevPressed (D_80149B10) |= pressed, gHeld
  (D_80149B30) = buttons, D_801403C0 = pressed, stick floats D_80140620[i][0/1] = func_800C9590(1.0f, cal, raw).
- Levers (in order, 79 -> 0 words): (1) tail statement order (gButtons store last in source: the IV web order then
  gives retail's s0/t5); (2) `pressed = held ^ b; pressed = pressed & b;` (xor+and into v0); (3) the stick value
  for func_800C9590 read into a local first (rawX, rawY: two variables; one reused `raw` costs 2 words); (4)
  volatile views of gHeld, gPrevPressed and the record array D_80156CF0 (retail keeps all those accesses in
  source order: the volatile record is what gives the `bnezl ...; lw v1,4(a3)` head); gStickRaw/gStickCal were
  already volatile views in the locked controller_poll. D_8013FED0/D_801403C0/D_80140620 stay non-volatile.
- Scorer (builder, `score.py group cand/g_controller_poll`, no --claims): controller_poll MATCH (own .rodata
  verified at 0x80123F94..0x80123F98), func_800C9590 MATCH, player_mode_set MATCH, player_state_set MATCH,
  func_800E7134 MATCH; `--claims`: `Members: func_800E7134: MATCH`.
- Unit (Pi): `blob_unit --tag w36w3 score func_800E7134 controller_poll func_800C9590 --neighbours --with
  cloud/work/frontier/w36w3/e7134/unit_7134.c` -> `EQUAL func_800E7134: 169 words (kept, c_unit_7134.c)`,
  `EQUAL controller_poll`, `EQUAL func_800C9590: 19 words (internal, ...)`, `locked bodies that differ in this
  unit: 0`, `blob_unit score: 3/3 equal`. (unit_7134.c = the same body with plain extern declarations; the group.c
  itself defines D_80149B64/D_80149B74/D_80156958, which the unit already defines.)
- Integration: group `controller_poll` superseded in place: group.c changed ONLY in func_800E7134's body (+ the
  typed record/views just above it), group.json members += func_800E7134, context -= func_800E7134 (it stays on
  keep: retail calls it by jal from func_800E73D8), claims [func_800E7134]. No overrides needed.

### func_800E73D8 (596 B): 4 words, register only
- N64-only: under the SI/pak lock (pak_queue_init/pak_lock/pak_unlock statics, as in the matched func_800E7710 and
  func_800A1A60), drain the SI event queue D_80035458, every 30th call (counter 0x80111964) re-query the pads
  (osContStartQuery, wait for the SI message (s16 5), osContGetQuery(D_80149440), func_800A43FC()), start a read
  (osContStartReadData; on failure unlock and return 0), wait, osContGetReadData(pads), unlock; then per controller:
  connected flag from the status/pad errno bit 8 (CONT_NO_RESPONSE_ERROR), disconnected pads read as zero, the 19-entry
  button map D_8011FAD4 builds the game button mask, stick copied, record bytes 10..13 cleared; func_800E7134();
  return 1.
- Levers: the 30-frame counter is a FUNCTION-LOCAL static (0x80111964, referenced only here) initialised to 29
  (retail .data byte 0x1D: the first call queries); record array D_80156CF0 volatile (as in func_800E7134); locals
  declared msg, i, j, pads (pads at sp+84); `pads[i].button & D_8011FAD4[j]` operand order.
- `score.py fn func_800E73D8/best.c` -> `4/149 words differ` / `own .data verified at 0x80111964 (4 bytes ...)`.
  The 4 words: the counter's register web (lbu/addiu/andi source) is v1, retail v0. ctrace: the static's web
  (w5, nocs 2) has v0 forbidden (v0, a0, s0); forcing j (w44, the first v0 holder) elsewhere still leaves v0
  forbidden (another web takes it): retail's counter web must interfere with less. Spellings tried (t = q + 1 temps,
  global + temp, reset forms, compare forms): no gain.
- Not yet run in the unit (needs the 4 words first).

### render_thread_entry (268 B): not advanced
- w27a best rescored: `65/67 words differ` standalone (frame 56 vs 64). Retail has six callee-saved webs: s0 &msg,
  s1 = constant 1 (`move a2,s1` for OS_MESG_BLOCK), s2 saved but unused, s3 -1, s4 &queue, s5 &D_801525F0; ours four.
  Tried: inlined receive helper with a flag parameter (3 spellings): no change (parameters copy-propagated).
  Hypothesis left: two webs that die after colouring (s2) and a non-remat use of constant 1. Also needs an `align`
  unit override (`.align 5` epilogue after the endless loop), like dma_wait_complete/audio_queue_process.

### dust_cloud_effect (600 B): MATCH (group + unit), new group w36w3_dust
- N64 hud.c blit animation (immediately before ShowHUD = skid_mark_render), sibling of the matched animLapTime /
  animPlace: data word = window | mode << 4; hidden on mode/window-count mismatch (state = 0) or when D_8015F72C is
  clear or gGameCar[model[win].slot].place_locked == 1; font 4 / 1 under the font lock; digit width =
  camera_shake_update(56) (HAL_GetCharWidth('8')); x = pos.x - 2*w - 2, y = pos.y + (one window ? 1 : 0),
  alpha = D_80116187, half18 = 100, UpdateBlit. MAT3 name open (GC hud.c has no per-window blit for this).
- Base: w27a best (19 words, 13 with a 40-byte unused-local probe). Levers: (1) matched-sibling style: Hidden /
  gfx_lock / gfx_unlock / font_set as non-static inlined helpers (gives the inlined font_set homes 76/52);
  (2) the car index is model[win].slot (D_8014A250, stride 0x808, s16 at +1990; the old draft's 0x8014AA16 * 36
  view was the same address with the wrong stride); (3) `((u32)data & 0xF0) >> 4` (srl); (4) `?:` for the y step;
  (5) **alpha stored before half18** (ugen temp order t7/t8: the last 13 words; the matched animLapTime has the same
  order); (6) frame: five disclosed unused locals (x before win; y, yoff, w, h after digitW), see the file header.
- Scorer: `score.py group cand/g_w36w3_dust` -> `Members: dust_cloud_effect: MATCH` (also with --claims; context
  slot_state_setup 2/58 there, informational: it is matched in its home group).
  Unit: `blob_unit --tag w36w3 score dust_cloud_effect --neighbours --with
  cloud/work/frontier/w36w3/groups/w36w3_dust/dust.c` -> `EQUAL dust_cloud_effect: 150 words (kept, c_dust.c)`,
  `locked bodies that differ in this unit: 0`, `1/1 equal` (umerge inlined Hidden_dust, font_set_dust).
- Integration: NEW group `w36w3_dust`: files dust.c + ctx_gr3_b.c (byte copy of credits_scroll_grp/gr3_b.c, the
  codex_control_settings_a4 pattern), members = claims = [dust_cloud_effect], context [slot_state_setup,
  object_create, func_800B669C], keep [dust_cloud_effect]. The four helpers (gfx_lock_dust, gfx_unlock_dust,
  font_set_dust, Hidden_dust) are non-static and not on keep; the unit may need force_internal entries for them
  like HAL_InitText_Init (w36o) if the installer does not drop caller-less non-image functions itself.

### sound_position_update = _UpdateFx@audiomgr.c (444 B): 51 words in the unit, structure exact
- Identity (new, proven): MAT3 audiomgr.c `_UpdateFx(void *element, int elNum)` (the ordered-list callback of
  _ProcessPriorities = func_80098FB8): `elNum >= 0.75f * gMaxSfx (D_80146200)`, the loop/state/vol test with
  AudioTimeout(startTime, 0.2f) = the locked func_80095198 inlined (it sits right before this function), stop
  command 5 into gStopCmds D_80143CF0[gNumStopCmds D_80110250++], start command 3 into gStartCmds
  D_80143AE8[gNumStartCmds D_8011024C++], 0.1f vol threshold, `type == 1`; _GetCmd = func_80091B00 (private IPA,
  preserves a2/a3). N64 FxTag offsets: state +0x10, type +0x14, loop s8 +0x19, numCmds u8 +0x1A, startTime +0x20,
  vol +0x24; stag->tag at +8; cmd: u8 cmd +2, tag +4.
- Levers: no `tag` local (the GC DWARF has only stag and cmd; every test reads `stag->tag->...`, CSE'd);
  `if (stag->tag->state == 2 || stag->tag->state != 1)` gives retail's `beq t0,v1,next` + `beql t1,v1,exit` head.
- Unit (us.sh): `FAIL sound_position_update: 51 of 111 words differ | words 33 ops 2 norm 2 | frame 24/24`.
  Residual: (1) stag->tag (w5) and stag->tag->state (w6) swapped (v1/a3; retail a3/v1); (2) constant 1 is not a
  live range (w104 save 0.0, best 3.0, never coloured; retail keeps 1 in t1 like 2 in t0). force.sh
  `p1:w5=c6,p1:w6=c2` -> 25 rows (the w104 force is declined: not a candidate). Next: find the source form that
  gives constant 1 a positive save (retail uses it at the head `state == 1` and at `type == 1`).

### world_collision_response (540 B): 84 words in the unit (WIP)
- N64-only frame submit: frame record f = &D_80156BE0[volatile s8 D_8015F72D] (128 B: OSScTask at 0, buffer at
  +0x7C, done message at +0x5C), D_80156BA8 = display-list cursor D_801497C8, D_80156BCC = D_80149438,
  world_velocity_integrate(1) (HAL_FinishText lead), gSPDisplayList(audio_start()), gDPFullSync,
  gSPEndDisplayList (SDK macro forms), OSTask fields (type 1, rspboot 0x80027DD0..0x80027EA0, ucode 0x80028CC0,
  ucode data 0x8002DDF0 / 2048, dram stack 0x8007BA50 / 1024, output 0x8007CA50..0x80084A50, data =
  D_8015B250 + 0x9CC0 / Gfx count * 8, yield 0x8007BE50 / 3072, flags = D_80114730 ? 2 : 0), next 0, sc flags
  99, msgQ D_8002ECF8, msg &f->doneMsg, framebuffer = f->buffer; osStopThread/osStartThread(D_8002E998) around
  D_8002EB70++, osSendMesg(D_8002E960, f, BLOCK), D_8002EB98 = sched frame count (+636).
- Levers so far: frame pointer computed first (ugen ring: retail t6 for &D_8015F72D), volatile D_8015F72D
  (materialised address), GBI macros. Residual: &D_801497C8 web a1 (retail a3), &D_8015B250 t0 (retail t1),
  retail colours rspboot (v0) and the constant -0x9CC0 (v1, hoisted lui/ori) as webs, as1 order of the ucode /
  ucode_data / framebuffer stores. Caller world_trigger_activate (also in this pool, 464 B, the gfx-thread message
  switch: 1 retrace, 2, 4 pre-NMI, 2750 task done) is unmatched; frontier lists a1/a2 as preserved here.

### credits_screen = uninitScreen@audioscr.c (296 B): MATCH (group + unit), audio_options_menu superseded
- NEW identity (proven): only caller updateAudioScreen = func_800D7634 (two sites, as GC updateAudioScreen's two
  uninitScreen() calls); body = GC uninitScreen: AudKillSong() (N64: slot-queue lock func_80091AF0, _GetCmd
  func_80091B00, cmd 1, unlock func_80091AF8, osSendMesg to D_801427A8), then if inited (D_8011062C):
  uninitButtons() (12 x 64-B buttons D_801102B4: ZOID_RemoveObject = entity_spawn_callback(objIdx, 0, 0), objIdx =
  -1; buttonsInited D_801105B4 = 0), `!(gGameState & 0x7C03FFFE)` -> RemovePanes(0) = sound_handles_clear(0),
  gTitlePane D_80110648 = 0; blit D_80110630 -> RemoveBlit = sound_stop, 0; removeBG = ambient_sounds_clear();
  panesCreated D_80110634 = 0; inited = 0. Private IPA (s0-s3 unsaved): internal, needs its real caller.
- Base: w14d provisional draft (6 words with a stand-in caller). Levers: real caller (group audio_options_menu,
  where func_800D7634 is a member); AudKillSong as w36q's helper pair (task->type = 1 on the same line as the
  _GetCmd call); uninitButtons defined AFTER credits_screen, as in the GC file order (as1 line ties: the last 2
  words, gGameState's `lui t9` before the inlined `D_801105B4 = 0` store's `lui at`).
- Scorer (builder): `score.py group cand/g_audio_options_menu` -> func_800D690C MATCH, func_800D6914 NOT VERIFIED
  (unchanged, expected: its own .rodata sits in two regions, see the group note), func_800D7634 MATCH (own .rodata
  verified 0x801241D4..0x801241E8), drone_pathfind_main MATCH (own .data/.rodata verified), `credits_screen:
  MATCH`; `--claims`: `Members: credits_screen: MATCH`.
- Unit: `blob_unit --tag w36w3 score credits_screen func_800D7634 --internal credits_screen --internal
  us_uninitButtons --internal us_AudKillSong --internal us_ProcessCommand --neighbours --with
  cloud/work/frontier/w36w3/groups/audio_options_menu/uninit_w36w3.c` -> `EQUAL credits_screen: 74 words
  (internal, c_uninit_w36w3.c)`, `EQUAL func_800D7634: 451 words (kept, ...)`, `locked bodies that differ in this
  unit: 0`, `2/2 equal`. Without the helper --internal flags the unit keeps us_uninitButtons out of line (34 words).
- Integration: group audio_options_menu superseded in place (HEAD version as base): every old file byte-identical,
  new file uninit_w36w3.c, members += credits_screen, files += uninit_w36w3.c, claims [credits_screen]; credits_screen
  NOT on keep. unit_overrides force_internal += credits_screen, us_uninitButtons, us_AudKillSong, us_ProcessCommand.

### func_8010FBE0 (128 B): MATCH (standalone + unit)
- Copies the caller's OSTask into an OSScTask (next 0, flags 2, msgQ D_80152750, msg 0), sends it to the scheduler
  task queue D_8002E960 (BLOCK) and posts 670 to D_8002E928.
- The function-local-static lever (found on func_800E7038): 0x80155238..0x8015528F are referenced only here
  (tools/xref.py), so the OSScTask is `static OSScTask scTask;` inside the function. w15d/w16e/w20e had proven
  "retail = one defined symbol, the address never in a register" and could not find a spelling: every global form
  gives uopt an lda web. A local static is a variable reference (no lda web).
- `score.py fn ... func_8010FBE0` -> `MATCH` (own .bss placed at 0x80155238/40/48/88/8C, zero-initialised,
  verified by address). Unit: `blob_unit --tag w36w3 score func_8010FBE0 --neighbours --with
  cloud/matches/func_8010FBE0.c` -> `EQUAL func_8010FBE0: 32 words (kept, c_func_8010FBE0.c); own
  zero-initialised data (addresses consistent, inside game .bss, no overlaps)`, `locked bodies that differ: 0`.

### world_trigger_activate (464 B): 25 words (WIP)
- N64 gfx thread step: osRecvMesg(D_8002ECF8, &msg, BLOCK); switch (*(s16 *)msg) in source order case 4 (pre-NMI:
  func_800205E4, apply_display_mode, osViBlack(1), D_801525F0 = 0, check_mpath_save, D_8011EAE0 = 0, hang), case 1
  (retrace: if the next framebuffer is the current one, no task pending (D_80035471/70/72), the scheduler frame count
  moved on (D_8002E8E8+636 != D_8002EB98) and fewer than 2 tasks queued (D_80114730 < 2): count = 0,
  gfx_frame_begin(), D_80035470 = 1, osSendMesg(D_8002ECC0, D_80142D90, BLOCK); count++), case 2750 (task done:
  D_80035472 = 0, world_collision_response(), D_8015F72D ^= 1, D_80114730++), case 2 (D_801161CC ^= 1,
  D_80114730--).
- Levers: case order 4, 1, 2750, 2 (layout); volatile flags/counters (materialised addresses); retrace counter
  0x80151AE0 and the case-2 toggle 0x801161CC are function-local statics (only referenced here); frame 56 = one
  inlined helper level (the 2750 body as a non-static helper) + one disclosed unused local `x` above msg (cur's home
  at 44 as retail); `D_8002EB98 != sched.frameCount` operand order (load order); a compiled-out holder `if (cur) {}`
  (disclosed) makes cur a coloured web (retail reloads it into v1; without it tot 2.0 = best 2.0 -> split).
- Residual (25 words, one-file group with keep [world_trigger_activate]): the case-1 &D_80114730 web is a0 (retail
  v1: our web also spans the case 2750/2 accesses and interferes with the switch value in v1; force declined), and
  the retrace counter's loaded value is a ring temp (retail a v1 web), which shifts later temps by one.

### func_800979A0 (292 B): 3 words (WIP)
- Base: the research packet cloud/work/frontier/dot_sequence_start_979a0_20261006 (12/73: everything but the final
  `D_8011EAA0 = func_800156E8(...)` store, which retail writes through a materialised address in v1).
- Writing that store as `*(volatile s32 *)&D_8011EAA0 = ...` (disclosed device; a volatile declaration also
  materialises the top read, 65 words) gives the shape: 3 words, `lui/addiu/sw` through t9 (ring temp) where retail
  uses v1 (a coloured web). Same 3 words in the unit (us.sh). Pointer-local spellings cost 8 frame bytes.
- D_80151ADC is referenced only here (local-static candidate) but the residual is not there.

### car_lod_select = PS_InitPakSys (N64 form) + func_800A3724 = _ClearPak: provisional
- Identity (new): GC paksysdc.c PS_InitPakSys: gErrorFunc (D_80144008) = arg; InitLL(&freeFiles D_801460E0, 1, 1)
  and the 64 76-byte file records D_80144DC0 pushed through their handle slots D_80144C60 (InsertLLE =
  func_80091FBC); per pak InitLL(&gPS_Files[i] D_80144D60, 1, 1), gPaks[i].damaged (+5) = 0, _ClearPak(i) =
  func_800A3724 (memset the 772-byte pak keeping byte 5; port in a3 = private IPA); N64 pak thread
  osCreateThread/osStartThread; gInited D_8011EAE0 = 1 (B131 had the wrong address 0x8012EAE0); _CompleteCallback
  slot = func_800A43FC.
- InitLL as a non-static inlined helper (`cl_InitLL`, head/tail/count order) for the free list: the list pointer
  then stays a register (retail `lw a2,8(s4)`); 21 words left, all one register permutation (list address s3/end
  address s4 swapped vs retail s4/s3) plus one delay-slot order.
- func_800A3724 is EQUAL in the unit as internal only with a kept stand-in caller (without one umerge inlines it
  into car_lod_select: its retail second caller track_render_process is unmatched) -> provisional.

### world_effect_update = InitGraphics@Init.c (456 B): not advanced
- The matched game_late_init (w36o initsystem.c, InitSystem) inlines exactly this body (InitGraphics_Init: one-line
  _InitGfx + ZOID_Init + InitBlits + InitCameras), so world_effect_update is the out-of-line InitGraphics.
  Re-measured: that helper form standalone = 35 rows (the w36o one-line _InitGfx is tuned for the inlined copy);
  static vs non-static helpers: no change; the prologue still loses its block boundary. w19e best (12 words) stands.
  The globals are all shared (xref: gfx_frame_begin, game_late_init), so the local-static lever does not apply.

## What generalises

- **Single-function globals are function-local statics.** Run `tools/xref.py` on every global of a stuck function:
  if only that function references it, declare it `static` inside the function (initialiser from the retail .data
  word; the scorer prints the retail bytes when they differ: 0x80111964 = 29). uopt then uses direct variable
  references: no address web, constant stores rematerialised, as1 `lui at` per store. This closed func_800E7038
  (46 -> 0) and func_8010FBE0 (after three earlier lanes), and fixed counters in world_trigger_activate.
- **Volatile shared state**: a function that reads/writes pad/controller/thread state shared with another thread
  keeps every load/store in source order in retail; `volatile` record/array views (casts like controller_poll's)
  fixed the head schedule of func_800E7134 and func_800E73D8 and the materialised addresses of the gfx-thread
  globals (world_trigger_activate, world_collision_response).
- **Matched siblings' idioms transfer**: dust_cloud_effect closed with animLapTime's helper set and its store order
  (alpha before half18, ugen temp order); credits_screen with w36q's AudKillSong helper pair; GC file order for
  helper definitions (uninitButtons after uninitScreen) decides as1 line ties.
- **IDO unrolls short constant loops** and the unrolled constants are rematerialised (one `li` per store); straight
  stores share one constant register. Store order inside the body shows in the ugen temp ring.
- **Register-only residuals in thread code** (func_800E73D8's static counter, world_trigger_activate's &D_80114730)
  come from webs that live across the switch/return structure; a static's web is live to the function exit in our
  build. Not solved; force.sh is declined when the competing register is forbidden by a precoloured constraint.

### object_bytes_sum_global / object_bytes23_sum: observation (not advanced)
- Using the real leading getter func_800B3D0C (locked, `return D_80149B60`) instead of w36w's local static: same
  4 words. In the unit the &gFontHeader web sees t2..t5 at cost 0 (p1cost) because sound_update_channel's colouring
  avoids t1..t5 (it even uses ra, c13, as a GPR, as retail does): IDO's IPA makes the callee avoid registers its
  callers keep across the call. Retail chose t4, so in retail t2/t3 were not free across that call: the residual
  looks like IPA state from the procedure order / caller set of sound_update_channel (callers TB_GetFontHeight,
  HAL_GetTextWidth, mode_byte*_set ...), not this function's source. force.sh p1:w21=c11 -> 0 rows (w36w).
