# w29d — model parser / resource loader / save / track loading (wave 29)

Status: final. Branch wave29 (main checkout, re-checked at 89bbd413); nothing committed, spliced or pushed. No
permission denials. Builder scratch watchman2:~/rush2049/scratch/frontier/w29d (toolkit reused from wtk) deleted at the
end. Flags everywhere `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3); unit = `blob_unit --tag w29d` (via `us.sh`).
No strict standalone MATCH, so nothing in cloud/matches; no group packet is installable yet (see Integration).

## Table

| Function (GC name) | Bytes | Before | After (unit) | draft_check | Residual class |
|---|---:|---|---|---|---|
| draw_ui_element (_CheckAndRepairGhost) | 240 | never drafted | **EQUAL** 60 words, 0 locked differ | not reached (needs a ghost file on the pak) | none; callers unmatched |
| func_800C7EC8 (_ReadPlayer) | 628 | never drafted | **EQUAL** 157 words, 0 locked differ | **EQUIVALENT** 1 call / player_reload | none; callee draw_speedometer unmatched |
| func_800BB9B0 (LoadTrack) | 2,104 | untouched | 40/526 words, rows 41, ops 9, **frame 168/168** | **EQUIVALENT** 41 calls / race, attract | inline-area offset (8 B), WorldBase address form, temp ring |
| func_800C813C (SaveServer) | 900 | 211/225 alone | 133/225, body length exact, frame 136/160 | **EQUIVALENT** 5,455 calls / 5 scripts (dc.c) | frame, v0 loads, lastPak narrowing |
| func_800CD104 (CreatePlayer) | 1,088 | 243/272, rows 163, frame 216/224 | 241/272, rows 142, **frame 224/224** | **EQUIVALENT** 2 calls / saves, player_finish | name-loop constant hoisting; name in s7 |
| func_80096CA8 (_SetupWaveModel) | 1,204 | 289/301, rows 134, ops 41 | best.c: structure fixed (rows 233 with the wrong IPA colours); oracle with 5 holders: rows 49, **ops 0** | **EQUIVALENT** 282 calls / race, attract | colour only: idx must outrank first |
| func_80097164 (_ReadWave) | 344 | 7 rows (w27e) | not touched | — | waits on CA8's convention |
| AdjustSpeed | 784 | 166/196, rows 99 | no new source; force oracle: callback→t0 = 65 rows, + pfs→s8 = 63 | not reached | callee-saved priorities |
| track_texture_load, props_render | | | not attempted (time) | | |

## func_800C7EC8 = GC save.c _ReadPlayer — EQUAL in the unit (`func_800C7EC8/best.c`)
```
blob_unit --tag w29d score func_800C7EC8 draw_ui_element func_800C813C func_800CD104 --with w29d/func_800C813C/best.c \
  --internal func_800C7EC8 --internal func_800CD0FC --internal func_800C7EC0 --internal draw_ui_element --internal func_800C7300 --neighbours
  EQUAL func_800C7EC8: 157 words (internal, c_best.c)
  EQUAL draw_ui_element: 60 words (internal, c_best.c)
  FAIL func_800C813C: 133 of 225 words differ ...
  FAIL func_800CD104: 241 of 272 words differ; compiled body is 267 words, target 272
  locked bodies that differ in this unit: 0
```
(the same two EQUAL lines with `func_800C7EC8/best.c`, which also carries draw_speedometer and the locked
frontier_menu_save_options members; those stay EQUAL: func_800CCCCC, func_800CCEFC, func_800CD058).
- IPA: fileh in s0. Levers, in order: (1) GC structure; (2) the inline _AlreadyReadPlayer loop uses the variable `player`
  (retail keeps both webs in s2; with a separate `before` fileh lost s0); (3) **_NewPlayer is the deleted static at the
  caller-less stub 0x800C7EC0**, N64 form `Handle func_800C7EC0(s32 pak, s32 slot)` (`player = &D_8013C128[pak][slot];
  *player = D_8013E5D8[pak][slot]; return player;`), inlined. Its separate return value is retail's player copy in s8 for
  the first _CheckAndRepairPlayer call (`move s8,s2` before the second). 77 rows → EQUAL.
- draft_check (`--params fileh=s0 --scripts player_reload`): `VERDICT: EQUIVALENT over 1 calls / 1 scripts (player_reload 1)`.

## draw_ui_element = GC save.c _CheckAndRepairGhost(ghosth, data, repair) — EQUAL in the unit (`draw_ui_element/best.c`)
`EQUAL draw_ui_element: 60 words (internal, c_d1.c)` / `locked bodies that differ in this unit: 0` — first draft.
IPA t0 = ghosth, t2 = data, t1 = repair: both callers (func_800C813C, func_800CBF2C) set t0, t2, t1 in that order (= the GC
prototype order). N64 differs from GC: revision test `!= 17`, on failure with repair `if (ghost->fileh)
AdjustSpeed(ghost->fileh); return 0` (pak delete), data checksum over `header + 0x58` for zippedSize.
N64 ghost header layout (GC __anon_49776 + 4 from 0x28): revision 4, flag 5, track 6, backward 7, name[13] 0x16,
playerId[2] 0x28, finish 0x38, zippedSize 0x3C, unzippedSize 0x40, zippedChecksum 0x44, unzippedChecksum 0x48,
start 0x4C, offset 0x50, size 0x54 (0x58). N64 ghost entry (44 B): fileh 4, track 8, backward 9, name 0xA,
playerId 0x18, finish 0x20, datah 0x24, header 0x28. No script reaches it (needs a ghost file on the pak).

## func_800BB9B0 = GC tracks.c LoadTrack — first draft, EQUIVALENT (`func_800BB9B0/best.c`)
`FAIL func_800BB9B0: 40 of 526 words differ | words 41 ops 9 norm 9 | frame 168/168`
(`--internal w29_FindWorld --internal w29_NewWorld`). Path: GC transcription 94 rows (frame 144) → NewWorld/FindWorld as
nested inlined helpers 71 → `extern volatile` WorldBase 65 (length now exact) → loop init order `i = 0, whdr = ...` 63 →
GC's two unused locals swapTG/swapT (**disclosed frame residual**: retail 168, without them 160, 2 slots) 41.
draft_check: `VERDICT: EQUIVALENT over 41 calls / 2 scripts (race 6, attract 35)`.
- Umerge inlines the locked player_state_get (= WaveInstanceReady) and slot_value_get (= GetWaveInstanceAddr) from other
  TUs; FindWorld is only inlined when nested in NewWorld (called directly it stays a call).
- Left: FindWorld's `whdr` struct at 48 in retail (ours 40: 8 B more inline area below it; local order/ptr in FindWorld
  and an s32 Parent change nothing); retail stores WorldBase with `lui at` and reloads with `lui a0` (ours one coloured
  address v1; defining D_80149B80 in the TU changes nothing; two symbol names would be a stand-in, not tried); temp-ring
  offsets after that block. Next: find the 8 B (another inline level in the NewWorld chain?) then the WorldBase form.
- GC names recorded in the file header (gTrackLoadInfo = D_80143A10 layout, gWorldData, WorldTable, gTargetGroups…,
  initTargetStructs = camera_lerp_position, SkyInit = camera_reset, bsearch = entity_name_copy, worldcmp =
  func_8008AD48, NewWorldObject = differential_output, InitSTree = func_800AB638, InitEnvs = func_800B2BDC,
  InitTrackDynamicObjs = tire_sound_update, LoadCars = audio_interrupt_handler, InitVisuals = sfx_position_3d,
  InitCheckpointData = audio_dsp_process, InitPathData = audio_effect_remove, InitDemoPathPoint = func_800B9284,
  InitSkids = func_800B0580, InitSmoke = physics_collision_test, InitBlast = camera_smooth_lerp, LoadWave =
  audio_frame_sync, WaveInstanceReady = player_state_get, GetWaveInstanceAddr = slot_value_get).

## func_800C813C = GC save.c SaveServer (`func_800C813C/best.c`; `dc.c` for draft_check)
`FAIL func_800C813C: 133 of 225 words differ` (no length line: 225 words), frame 136/160 (`--internal func_800C7300` added).
- The draft reads gPS_Files at D_80144D60[pak]+8 (= retail 0x80144D68), i.e. it never had w28b's +8 bug.
- Levers: sInitCountdown/sLastPak/sReadingPak are **function-local statics** (.data 0x8011742C = 3, 0x80117430 = -1,
  0x80117434 = {0,0,0,0}; only func_800C813C references them in the image): 157 → 102 rows; as file statics they still go
  through a coloured address; _AlreadyReadGhost is the deleted static at the caller-less stub **0x800C7300** (gives the
  found path's `b; nop`, body length exact).
- Open/disclose: declared `(void)` here — with GC's `(s32 forceUpdate, s32 rumbleOnly)` ugen stores a0/a1 to their homes,
  retail does not; save_settings calls `(0, 0)`. A parameter-list mismatch, not a false return type; decide or explain.
- Left: frame 24 B short (with _ReadGhostHeader as a second inline helper with GC locals: frame 152 but 2 extra words);
  retail loads the channel fields into v0 (an inlined value helper, PS_PakInserted-like?); lastPak via `lb 159(sp)`.
- draft_check needs `dc.c` (statics → externs): `VERDICT: EQUIVALENT over 5455 calls / 5 scripts (saves 1568,
  attract_pak 908, race 917, player_reload 1091, ghost_save 971)`. `--statics sInitCountdown=0x8011742C,...` did NOT bind
  initialised function-local statics (draft left 0x8011742C unchanged) — reported for w29r.

## func_800CD104 = GC CreatePlayer (in the save-unit file; `func_800CD104/best.c`)
163 → 142 rows, frame exact 224/224: _NewPlayer inlined (as in C7EC8), `_Pow(exp)` with ONE parameter (num 10 folded;
two parameters overshoot the frame by 8), attempt loop as `for (attempt = 0; ; attempt++) { ...; if (before == NULL)
break; }`. A name-filter helper (my first idea) also fixes the frame alone but is wrong once _NewPlayer is in (232).
Left: retail hoists only '=' (a1) and the '*' store (a2) in the name loop and keeps name in s7 with a walking copy in a0;
ours hoists '.', ':' into s-registers and spills name. Two stubs D0F4/D0FC precede it: _Pow and one more static (unknown).
draft_check: `VERDICT: EQUIVALENT over 2 calls / 2 scripts (saves 1, player_finish 1)`.

## func_80096CA8 (one lane-hour on the named lever)
Traced: priorities `save = tot/nocs`; first = 323/18 (17.9; 3 inner-loop tests ×100), idx = 16/19 (0.84). For retail,
idx must land between first and &wad (w117) — the 9th s-register. Code-free `if (idx) {}` holders (3 in the inner loop,
2 in the PTHD loop) put idx at 16.0, &wad 15.86, first 15.38 → first gets t0 as in retail. With that oracle the three
structural fixes below give **ops 0, 49 rows (pure permutation)** — `holders_oracle.c`, NOT a deliverable.
Structural fixes now in best.c (no devices): gPalDefPtrs D_80138670 as `Tab[]` (idx*8 CSE'd for all 3 tables);
gTargetPaths D_801392D0 as one `{ptr, count}` struct (retail uses one base + 0/4); fixups as `x = x + (first ? WADptr :
offset)`. Next: a source form that gives idx inner-loop weight without holders (none found), or force-oracle the
remaining permutation on the holder build. func_80097164 untouched (still blocked).

## AdjustSpeed (force oracle only)
`force.sh` on w28b's best: callback constant (p1 w44) → t0: 99 → 65 rows; + pfs into s8: 63. Remaining: D_8011EAE8
address (save 3.33) outranks controller/file (retail s6 below s4/s5). Retail t0 cost for the callback is 1 call
(func_800A1E94 preserves t0 by IPA); ours charges 30. No variant written (time).

## Integration notes (nothing installable yet)
- Save unit: draw_ui_element and func_800C7EC8 are unit-EQUAL but need C callers/callees to install: func_800C813C
  (133/225) and func_800CBF2C call draw_ui_element; func_800C7EC8 needs draw_speedometer (99/426, same TU — it is
  context in the locked frontier_menu_save_options group together with func_800CD104). A future group = supersede
  `frontier_menu_save_options` (keep every member) + draw_speedometer + func_800C7EC8 + func_800CD104 + draw_ui_element +
  func_800C813C; force_internal func_800C7EC8, draw_ui_element; helpers func_800C7EC0 (_NewPlayer), func_800CD0FC
  (_Pow), func_800C7300 (_AlreadyReadGhost) are non-static internal helpers named after their stubs (they are locked
  stubs: list as members, prefer_definition per WAVE_BRIEF rule). codex_func_800C7200_2 has func_800C813C as context.
- C813C data change: the three function-local statics become the TU's own .data at 0x8011742C/0x80117430/0x80117434.
- LoadTrack: a single (o32, not IPA) once it matches; w29_FindWorld/w29_NewWorld non-static internal helpers;
  `extern volatile` D_80149B80 local to the file.

## What generalises
- **Every caller-less stub was a GC static**: 0x800C7EC0 = _NewPlayer (shared by _ReadPlayer and CreatePlayer),
  0x800C7300 = _AlreadyReadGhost, 0x800CD0FC = _Pow. Look up the GC file order around the stub.
- **umerge inlines locked non-static functions from other TUs** (player_state_get/slot_value_get into LoadTrack), and a
  helper is inlined at one nesting but not another (FindWorld only via NewWorld).
- GC prototype order = IPA argument set-up order again (draw_ui_element t0, t2, t1).
- File statics behave like externs (coloured address); function-local statics load by %lo.
