# w36n — MAT3 (GameCube) -> N64 FUNCTION name map (wave 36, naming lane, no matching)

**Status: complete for wave 36 (2026-10-21); re-runnable.** Read [map_functions.md](map_functions.md) (coverage, a
lane-target check table, then tables by address per population) or `map_functions.json`. Use **proven** and
**strong** rows as identities; **lead** rows are "look here" only. Overlay names are per image: `func_8038xxxx` in
image A and in image B are different functions (population column `ovl_a` / `ovl_b`).

Deliverables (this dir only): `map_functions.json`, `map_functions.md`, `renames_proposed.json`, `RESULTS.md`,
`harvest.json` (cited identities used as anchors), `tools/` (the generator). Nothing is applied: no rename, no edits
to docs/, symbol files, renames.json, the Conveyor DB or other lanes' dirs. No permission denials.

## Coverage (current run; the header of map_functions.md is regenerated with every run)

| Population | functions (bytes) | unmatched (bytes) | unmatched with a proven/strong MAT3 identity | of which proven | all functions with proven/strong identity | wrong labels found |
|---|---|---|---|---|---|---|
| game image | 1216 (561,860) | 126 (138,144) | **79 (96,956 B, 70%)** | 57 (77,568) | 558 (343,296) | 249 |
| runtime image A | 193 (152,952) | 103 (83,664) | **60 (83,320 B, 99.6%)** | 34 (67,752) | 131 (139,160) | (all func_) |
| runtime image B | 49 (38,708) | 24 (21,296) | **19 (21,256 B, 99.8%)** | 12 (16,616) | 41 (38,256) | (all func_) |
| static (boot segment, 668 functions) | 668 (159,108) | 35 (9,264) | 2 (488) | 1 | 22 (9,024) | 9 |

The unmatched image-A functions without an identity are all 8-byte deleted-static stubs (43 functions, 344 B).
"Matched" = in blob_matched.lock.json / ovl_matched.lock.json at this checkout (lane matches of wave 36 are not
in the locks yet). Static and game/overlay counts are separate denominators. Not ROM coverage.

## Method (tools/mapfn.py, re-runnable, read-only, ~20 s on the Pi)

`python3 cloud/work/frontier/w36n/tools/harvest_rows.py && python3 cloud/work/frontier/w36n/tools/mapfn.py`

Inputs decoded directly: N64 `build/game_code.bin` + `build/blob_layout.json` (game population, 1216),
`build/ovl/{a,b}/image.bin` + `asm/us/ovl_{a,b}/extents.json`, static code from `baserom.us.z64` (boot segment) with
function starts from `build/us/rush2049.us.elf` and `asm/us/boot_tail/extents.json`, names from `asm/us/blob/symbols.json`;
GC `reference/gc/disasm/**` (bl/b order per function), `reference/repos/mat3/rush_d_functions.csv` (file, source
line, scope, stripped), `reference/gc/features.json` (strings), `docs/external/gc/correlation.json`.

1. **Anchors.** names.md / renames.json GC-sourced rows (`tools/anchors.py`); identities stated in matched sources and
   lane RESULTS, harvested by hand with citations (`tools/harvest_rows.py` -> `harvest.json`: 155 rows, incl. the
   "Identities proven" sections of w36a/c/d/e/f/g/o/p/q/u and w36n's own manual checks); correlation confident pairs (lead grade alone);
   strings used by exactly one function on each side.
2. **Propagation**, iterated to a fixpoint (evidence kinds; each row lists its items):
   - `callee`: Needleman-Wunsch alignment of the N64 callee sequence (jal/j order) with the GC callee sequence
     (bl/b order) inside every accepted pair; a gap holding exactly one unknown on each side pairs them.
     Multi-unknown gaps are resolved by the candidates' own call structure (below). `confirm` = the same test
     re-deriving an already known pair (the independent check of every anchor).
   - `struct`: ordered callee structure — LCS of a function's mapped callees against the GC function's callee
     sequence (also with same-file statics expanded, since N64 inlines many of them); global search for unmapped
     functions: unique best, sim >= 0.75.
   - `sig`: callee-set Jaccard (unique best).
   - `order`: (a) file order (GC source-line order, stripped functions included) between two strong anchors with
     equal counts; (b) order consistency (between the nearest anchors of the same file in both orders);
     (c) **IPA layout** (new, see Findings): the predicted post-order of a root's subtree aligned with the N64
     functions that end at the root; (d) `stub`: a deleted inlined static's 8-byte stub right before its caller.
   - `string`: unique shared string.
3. **Safety rules.** A GC *static* whose N64 candidate sits far outside its file's anchored address span, or in a
   different population than the file's other functions, is rejected/capped (this removed e.g.
   func_8008C874 = findDamageSlot: findDamageSlot is a worlddam.c static, worlddam.c is in image B; its deleted stub
   is B:func_8038A400). Size ratio N64/GC outside [0.3, 4.5] (both > 16 B) caps at strong. Ties are reported, not
   guessed (greedy matching by evidence score).
4. **Confidence.** proven = a level-3 item (a matched GC port, or a closed callee gap whose caller and both bounds
   are proven), or two independent evidence groups at strong level (solid / callee / order / string / struct+sig),
   or callee evidence from >= 2 distinct callers, or correlation + one strong group; strong = one strong item;
   lead = otherwise. Never size- or float-only above lead.

## Held-out check (precision of the grades)

`python3 cloud/work/frontier/w36n/tools/evalholdout.py`: the 186 cited anchors (harvest.json + names.md rows) in two
folds; each half is removed from the anchors, the map is rebuilt from the rest, and the engine's answer is compared:

| grade given to the held-out pair | recovered | correct | different |
|---|---|---|---|
| proven | 88 | **87 (99%)** | 1 (A:func_803AD57C: engine says drawView, the cited identity is animText@recordscr.c with drawView inlined) |
| strong | 45 | **43 (96%)** | 2 (hud.c animPlace -> animCoins, animElapsedTimerText -> animPlace: the look-alike hud text callbacks) |
| lead | 16 | 11 | 5 (incl. inline shifts now demoted: func_800E543C -> multiinit, B:func_80390F60 -> UpdateBattleInvs) |
| not recovered | 37 | | |

Two guards were added after the first evaluation (precision of "strong" was 89%): the **inline-shift guard** (n's
mapped callers are callers of g's callers and never of g: n is g's caller with g inlined -> lead) and the **GC-stub
guard** (a GC body of <= 16 B against an N64 body > 256 B is position-only -> lead). Remaining failure mode: the
look-alike HUD text callbacks of hud.c; treat "strong" as ~19-in-20 and "proven" as the identity.

## Findings

- **Image-A/B (IDO -O3, uld IPA) function layout is a post-order DFS of the unit's call graph from its root, callees
  visited from the LAST call site to the first, each function once; a static that was inlined still occupies its
  slot as an 8-byte `jr ra; nop` stub.** Verified exactly on recordscr (root updateRecordScreen: updateViewButtons
  stub, updateSelectButtons, updateButtons, updateViewInfo, updateSelectInfo, updatePakInfo, setRecordState,
  uninitButtons stubs, uninitRecordScreen, controlSelect, initButtons stub, initRecordScreen, updateRecordScreen),
  playerscr, weapons.c (_RemoveCannMFlash, _RemoveGattMFlash, AnimateWeapon, StartWeapon, InitBattlePlayer),
  worlddam.c, powerups.c (Shield, Health, UpdateBattlePowerUps with Invs inlined). Use: the function right
  before a known overlay function is its last-called not-yet-emitted callee (or that callee's stub). The game image
  (-O2 units) keeps GC source order within runs of a file instead (with N64 file splits).
- **Lane-table corrections** (details in map_functions.md "Wave-36 lane targets"):
  A:func_8039C910 = **updateCheatScreen** (not updateCircuitScreen: that is A:func_8039C140, matched; the N64 body is
  GC updateCheatScreen with initScreen/uninitScreen@cheatscr inlined); A:func_8038FFD8 = **initScreen@startscr.c**
  with initButtons@startscr inlined (AudSetMusicVol, AudPlaySong, createBG, [initButtons body], NewMultiBlit,
  ClearRushControls); B:func_80391B00 = **UpdateHUDExtraPostCamera** with animBattleCoins and animStuntCoins inlined
  (it calls B:func_80391864 = animWeaponIcons and B:func_80391650 = animPowerupIcons first; correlation had those two
  swapped/mislabelled); music_control = **StartEnvironment** (StartShadow/StartArrow inlined: the three ZOID_NewPoly);
  A:func_80393B3C = initRecordScreen (w36f), not an initButtons.
- **Identity of the "TitlePane" family:** TitlePane (0x800B574C) = _TitlePane@**videoscr.c** (its callers are
  physics_sym = updateVideoScreen and func_8010E8B4 = animPanes@videoscr.c); time_result_display =
  _TitlePane@optionscr.c, voice_stop = _TitlePane@pausescr.c. This corrects slp.c's header ("optionscr") and w31b
  ("pausescr"); the four GC copies have identical bodies, so only the callers discriminate.
- **w28 leads re-checked:** entity_flags_apply = **AudPlayEffect** (not PlayGameSound: PlayGameSound is
  func_800B61A8; N64 ShowCountdown calls AudPlayEffect directly), func_800B24EC = MBOX_FindTexture_WORK,
  slot_state_setup = _HAL_SetFont and object_create = HAL_SetFont (w28a/w28c disagreement resolved, see next item),
  object_manager_update = HAL_GetTextWidth, render_helper/func_800ED66C/dispatch_handler/state_utility =
  HAL_SetTextDepth/HAL_SetTextTranslucency/TB_SetTextColor/TB_DrawText, ambient_sound_set/crowd_cheer_play/
  sound_handles_array_clear = InitPane/ChangePaneSize/RemovePane, sound_stop/sound_control = RemoveBlit/NewMultiBlit,
  Input_ApplyPadConfig/func_800EF5B0/brake_light_update = UpdateBlit/RenameBlit/ZOID_ProjectWindow, func_800CCCCC =
  _InitPlayer, format_string_parse = _CalcChecksum, viScheduleTick/viGetTimeToDeadline = SetCountdownTimer/
  GetCountdownTimeRemaining (static), func_80091FBC = InsertLLE — all confirmed. ShowCountdown's world-vector callee
  func_8008C544 is U64WorldVector (lead; WorldVector proper is func_8009E820, proven via update_camera and four more).
- **N64 inlines the HAL_SetFont wrapper**: most callers show `osRecvMesg; slot_state_setup; osSendMesg` (the font
  semaphore around the static `_HAL_SetFont`), a few call the wrapper object_create = HAL_SetFont (w36o/w36g). The
  aligner now treats a GC call to a small single-callee wrapper as matching an N64 call to its callee.
- **More big identities** (w36n manual checks, cited in harvest.json): stunt_combo_display = **Resurrect** (CopyUV/
  syminit frame, FindSurfaceUvsInPlace inlined as RoadPosition+sin/cos, then FindHumanResPt = func_800D348C,
  FindSurfaceUvsFromResPoint, 3x CountdownTimeOut + PassedCP = race_countdown_display; caller Init_MDrive);
  playgame_state_change = **updateGameState** (initGameState inlined; callers game_loop = GameLoop and
  func_800D5A04 = AbortGame; w36p agrees); emitter_update = **animCountdownTimer** (not animRadarBlip = func_80108F40);
  func_80109F54 = **animFlag**; entity_update_callback = AnimateNewBlast (lead).
- Image B is now almost fully identified: worlddam.c, weapons.c, streaks.c (UpdateStreaks, NewStreak, InitStreaks),
  shots.c (UpdateShots, ShotUpdate, ShotInit, UpdateDefense = func_8038F938, ElevateGun = func_8038F568,
  ShotToCarCollision, SonicCarCollision, MissileLock, ProximityDamage, BattleDamage, ShotDone, _UpdateShotObject,
  _CreateShotObject), powerups.c, hudextra.c (ShowHUDExtra, RemoveHUDExtra, UpdateHUDExtraPostCamera, the anim*
  callbacks incl. animStuntPoints = func_80392894, animHealth = func_80392FE4, animAmmoCounts = func_80393518,
  animBattlePoints, _AddBattlePoint).

## Rename batch (renames_proposed.json)

- `renames`: game functions, exact `cloud/work/frontier/w29n/renames.json` row format (`vram, old, new, kind,
  source, evidence, status, batch`), `status: "proposed"`, `batch: "w36n"`; proven/strong only; only functions whose
  label is `func_XXXXXXXX` or wrong. Naming rule of names.md: GC spelling without the CodeWarrior underscore
  (kept when GC also has the bare name: `_ReadGhostData` vs `ReadGhostData`, `_PS_FlushFile` vs `PS_FlushFile`); a
  file-static name that recurs in several GC files gets the N64 screen-file prefix (`controls_`, `options_`,
  `audio_`, `video_`, `pause_`, `highscore_`, `gameover_`, `track_`, `car_`, `start_`, `player_`, `circuit_`,
  `records_`, `mode_`, `cheat_`, `hud_` ...). Rows with `note` = include/ already declares that name as an unbound
  arcade-derived prototype: check the signature before applying.
- `held`: a C library name (`entity_name_copy` -> `bsearch`) / an existing symbol: `func_800F8EC8` = **CheckCPs** and
  `func_800FAEE4` = **InitDrones** are the real functions, but symbols.json carries arcade placeholders `CheckCPs` /
  `InitDrones` at non-function addresses (0x800C0000 ...; correlate.py notes this) -- retire the placeholders first /
  static functions (need the static rename proof, not the game batch: viScheduleTick = SetCountdownTimer, viAddTicks =
  AddCountdownTime, viGetTimeToDeadline = GetCountdownTimeRemaining, viDeadlinePassed = CountdownTimeOut,
  inflate_entry_alt = unzipdata, inflate_free_window = inflate_codes, huft_alloc = alloc_zip_mem ...).
- **How to apply** (established procedure, w29n/w30n/w32n): the coordinator copies the accepted rows into
  `cloud/work/frontier/w29n/renames.json` with `status: "accepted"` (batch name of its choice), adds the reader's table
  to docs/external/gc/names.md, then on a clean checkout runs `RENAME_BATCH=<batch> sh cloud/work/frontier/w29n/replay.sh`
  (inventory on a DB copy, GAME_SYMBOLS, symbol_addrs, sources via users.py/static_rename, lane drafts, group.json,
  lock re-key with pure-rename proofs + recompile on watchman2 + image gate, receipts/rebind, CLAIMS, gate chain to
  ROM SHA-1). With ~500 rows the batch is 20x the earlier ones: split it (e.g. proven rows first — the `evidence`
  field starts with `w36n proven` / `w36n strong` — or first the functions wave-36/37 lanes touch), and expect the
  rebind/packet steps to grow with the number of matched sources that use the old names.
- **Overlay names: no rename tooling exists.** `tools/conveyor/pipeline/ovl_targets.py` derives every runtime-image
  name as `func_%08X` (`name(address)`) and writes `asm/us/ovl_{a,b}/symbols.json` from the game symbols plus those
  generated names; `ovl_matched.lock.json`, `src/ovl_{a,b}` group.json members/claims/keep and `ovl_splice`/`ovl_layout`
  are keyed by those names. Applying `overlay_renames` would need a per-image name table read by ovl_targets plus
  re-keying of the overlay lock and groups (not built, as instructed). Until then use the map as a comment-level
  reference (`/* = GC updateCheatScreen */`).

## Lane harvest (cited in harvest.json; last pass 2026-10-21 00:05)
Identities proven by w36a (createGridMap stub, showGridMap, carscr confirmations), w36c/w36f (recordscr unit, drawSelect,
playerscr animText/drawDeletePlayer), w36d (the whole trackscr IPA unit), w36e (LoadSelectCar, InitCarDynamicObjs,
updateWarningScreen, SetFogIntensity, initScreen@startscr), w36g (animText@circuitscr + inlined stubs, updateCheatScreen,
animText@cheatscr, circuitscr string helpers, text-layer names), w36o (ShowHUD, animSpeedText, FindWinner, the
HAL_SetFont/_HAL_SetFont split, HAL_GetFontAscent/Descent), w36p (game.c/state.c flow), w36q (createBG), w36u
(WorldCullObjects + WorldDistance stub). Lanes w36b/h/j/k/l/m/s/t/v had no "Identities proven" entries yet. Re-run
`tools/harvest_rows.py` (add rows) + `tools/mapfn.py` to refresh after later lane results.

## For matching lanes
- Callee and global identities for your function: grep its callees' N64 names in map_functions.md (game helpers such
  as model_data_load = MBOX_HideObject, sign_extend_call = MBOX_NewObject, func_8008E06C = ZOID_SetColor ...).
- Rows marked "Label wrong" have a speculative arcade label that is not what the function does.
- If you prove an identity, write it in your RESULTS.md "Identities proven" section; w36n harvests it.

## Open questions
- Unmapped big unmatched game functions, probably N64-only or rewritten (leads in parentheses, not graded):
  func_8009F058 (N64 world render: guOrtho/guPerspective/guLookAtF), entity_ai_pathfind and func_80098FB8 (N64 audio
  thread; _ProcessCommand / _ProcessPriorities-like command handling), car_shadow_render (audio thread start-up +
  front-end wave loading, _LoadFrontEndWaves-like), audio_doppler_calc / menu_input_process (N64 text renderer,
  _DrawText / HAL_DrawText-like), func_800F87A0 (controlMain@gameoverscr.c-like, caller finish_state_alt),
  particles_update (animCoins-like), track_render_process (PS_PakServer, w36p/w28b lead), game_late_init,
  sound_bank_unload, entity_process_main, func_80106874.
- Static code: the boot segment (668 functions: built ELF FUNC symbols + asm/us/boot_tail extents, bytes from
  baserom 0x1000-0x2F4E0) is decoded and aligned, but almost all of it is libultra/N64 audio with no GC counterpart;
  the GC GameSource functions found there are timer.c, unzip.c, a few memsys.c and the libc seeds.
- object_create / slot_state_setup: settled by w36o (HAL_SetFont / _HAL_SetFont). No unresolved ties remain at the
  time of the last run (`conflicts` in map_functions.json).
