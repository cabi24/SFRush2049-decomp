# w36y — MAT3 (GameCube) -> N64 DATA symbol and struct-type map (wave 36, naming lane, no matching)

**Status: v2 complete for wave 36 (2026-10-21 01:00; lane harvest as of then); re-runnable** (`sh cloud/work/frontier/w36y/tools/run_all.sh`,
~3 min on the Pi, read-only on the repo). Nothing applied: no edits to include/, tools/, docs/, symbol files,
renames.json or other lanes' dirs. No permission denials.

| deliverable | content |
|---|---|
| [map_data.md](map_data.md) / `map_data.json` | 1,061 GC data objects placed in the N64 address spaces (rows: population, n64_address, current N64 symbol, mat3 name/key/file/type/size, N64 access kinds and derived type, volatile-like flag, GC->N64 field offsets, confidence, support, users/unmatched users, evidence, alternatives) |
| [types.md](types.md) / `types_n64.h` | 67 MAT3 struct/union types with N64 offsets per field (same / moved / interp / ambiguous / unseen), N64 sizes, evidence; C89 draft of 43 well-evidenced structs (NOT installed) |
| `renames_proposed.json` | 351 proven game/static-data renames in the w29n `renames.json` row format (`status: proposed`, `batch: w36y`), 158 informational overlay rows, 5 held (generic names) |
| `tools/` | refs.py (reference decoders), mapdata.py (code channel), content.py + contentscan.py (content channel), harvest.py (lane channel), output.py (merge + grading), fields.py + types_out.py (struct layouts), renames.py, evaluate.py, run_all.sh |

## Coverage

| population | proven | strong | lead |
|---|---|---|---|
| static-data (0x8000_0400..0x8008_6A50, boot segment data/bss) | 10 | 3 | 7 |
| game image (0x8008_6A50.., incl. .bss to 0x8017_A6xx) | 359 | 179 | 196 |
| runtime image A (per image, 0x8038_A400..) | 128 | 86 | 43 |
| runtime image B (per image) | 30 | 14 | 6 |

Distinct N64 data addresses referenced by code (field addresses counted separately; float literals and switch
tables excluded) that fall inside a proven/strong object: game+static 1,726 of 3,207 (54%), image A 671/1,065 (63%),
image B 183/241 (76%); of the addresses referenced by UNMATCHED functions: 770/1,498, 283/413, 106/139. Not ROM
coverage.

Struct layouts: 67 types with N64 field evidence; the big ones: `tModelData` (model[], 80/133 fields seen, N64 =
GC layout, 0x808), `tCarData` (gGameCar, 47/54 fields: identical through `visuals` at 0x110, everything after the
visuals array 0x18 lower = one tVisual fewer, N64 stride 0x3B8), `gPlayers` (__anon_3d1b: 0x0..0x17 identical, a
4-byte field gone inside `controls[11]` -> numCrashes 0x40, save 0x48, stride 0x4C), `CarParts` (N64 drops hsheen,
tsheen, brakes, trim, headlight[4]: frame 0, extraparts 4, hood 0x10 ... bSpark 0x3C, size 0x40), `gLink` (8-byte
records, owner at +7), `_tBlit`, `tTarget`, `tVisual`, `tBlastStruct`, `tStuntPlayer`, `tCMuzzleObj`, buttonInfo,
gCheckpoints, gResPath, gText, gCamera, the pak/save list heads, the stats records.

## Method (three placement channels + layout pass)

1. **Code channel** (tools/refs.py, mapdata.py). Every function of the game image and runtime images A/B is decoded
   from `build/game_code.bin` / `build/ovl/{a,b}/image.bin` (w36n's loaders, imported read-only): `lui` halves carried
   through `addiu`/`addu`/moves, every load/store/address formation resolving to a fixed address is recorded with its
   kind (lb/lbu/lh/lhu/lw/lwc1/ldc1/sb/sh/sw/swc1/sdc1/addr), mode (direct / indexed / field off a formed base) and
   use count. GC side: the symbolised disassembly (`reference/gc/disasm/**`, exact relocations: `@sda21`, `@ha/@l`)
   with the same register tracking; CodeWarrior's "neighbouring static off one base register" references
   (`namePanes@l` + 0x1B0) are re-attributed to the symbol containing the address. For each of the 730 w36n
   proven/strong function pairs, the two first-use element lists are aligned (Needleman-Wunsch: same access class,
   mode, use-count similarity) and then each GC symbol is given ONE N64 base per pair (greedy, one-to-one), scored
   by offset-set agreement (its accessed offsets land on N64 accesses at base+off), agreement with the NW order,
   and object identity (N64 accesses off the same formed base). Iterated 3x with the accepted map as a prior.
   Votes = distinct N64 functions per (GC symbol, N64 base). Base selection prefers the address aligned with GC
   offset 0, and snaps a base that is misaligned for the object or owned by unrelated N64 code to the lowest aligned
   access (gCarParts 0x80139300 -> 0x80139320, gLink 0x80153E87 -> 0x80153E88).
2. **Content channel** (content.py, contentscan.py). Every initialised GC object (rush_d.elf .data/.rodata/.sdata/
   .sdata2, GameSource/NGC units only) is searched in the N64 images byte for byte; GC pointer words are wildcards but
   a pointer to a string must point to the same text on the N64. 124 objects hit, 107 uniquely; restricted to the
   image(s) their GC file lives in (w36n's file -> population), 116 rows carry content evidence (car tables, unzip/zip
   tables, colour tables, string tables such as powerupIconNames, buttonNames, componentToName, gCarOrder, trackMap).
3. **Lane channel** (harvest.py). "D_X = name" / "name = D_X" / "D_X name[...]" statements in lane RESULTS, notes and
   matched-source comments (235 claims with MAT3 names, each with its files); used to confirm (strong -> proven),
   to fill objects the code channel could not place, and as the evaluation set.
4. **Grading** (output.py). proven = >= 2 distinct N64 functions with more than an address formation and >= 2 GC
   functions (objects > 8 B also need >= 2 aligned offsets or >= 3 functions; address-only evidence needs >= 4
   functions), 2x the runner-up, no stronger rival for the same N64 base; or code + content agreement; or exact
   content of >= 6 non-zero words / 2 strings; or code/content + a lane statement. strong = one clean function of a
   PROVEN w36n pair, a weaker majority, a short content hit, or a lane statement alone. lead = the rest, including
   any placement inside another symbol's proven object.
5. **Layouts** (fields.py, types_out.py). Channel P: every GC parameter typed as a pointer to a struct is followed
   on both sides (GC r3.. by argument order; N64 o32 a-register slot of the same argument; copies, stack spills,
   `addi` adjustments); channel G: accesses of a placed global inside its N64 object. Per pair the field accesses are
   aligned twice (identical-offset bonus / pure order) and the alignment with the more consistent delta is kept;
   pass 2 replaces the identical-offset bonus with the pass-1 layout prior (expected delta of the nearest voted
   field). Per struct the votes are filtered to the max-weight chain in which GC and N64 offsets both increase.

## Precision (tools/evaluate.py; measured BEFORE the lane channel was merged, i.e. independent)

Against the 235 identities other lanes / GAME_SYMBOLS state (147 comparable rows):

| grade | agree | disagree | notes |
|---|---|---|---|
| proven | 135 | 12 | none real: 5 are field claims (gPlayers = D_8014A160 is "gPlayers[i]+0x4C", model[0].fastin at D_8014A96C, gPlayerFiles.head), 6 are same-name statics of other screens (buttonInfo/buttonsInited/gButtonScale per screen file), 1 a harvest mis-parse ("path") |
| strong | 32 | 12 | ~80%; real misses: compLight (D_803B2858, mine +0x10), ties (w36o D_80150B60), gGhostPlayers, gPathPtList, coinStuntPos image |
| lead | 14 | 11 | hints only |

The first iteration had gPathMin/gPathMax swapped and namePanes/codePane swapped (same-type neighbours accessed in
interleaved order); access-count similarity and the object-consistent assignment fixed both, which matches w33c's
matched CalcResPathBounds (D_801407D4 = gPathMin, D_801407B4 = gPathMax) and w28a's namePanes/codePane.

## High-value identities (referenced by the most unmatched N64 functions; all proven)

gRace = 0x8014A110 (label `gameplay_mode`), gGameState = 0x801174B4 (`state_word_a`), gNumPlayers = 0x8014A108
(`active_player_count`), gNumWindows 0x80151AD0, gGameCar = 0x80152818 (`player_array`, tCarData stride 0x3B8),
model = 0x8014A250 (tModelData 0x808), unitmatrix = 0x8011418C, gTrackNum = 0x8014978C (s8, volatile-like),
ObjInstanceList = 0x8012E700, gText = 0x8017A4E0 (`countdown_state`!), gPlayers = 0x8014A118 (`input_rec0`, stride
0x4C), gNumModels 0x80140BDC, gFrameSeconds = 0x8002EB94 (volatile f32), gJust = 0x80118E20, gPauseState =
0x801170FC, gOptions = 0x80146108 (s8[41]), gPaks = 0x80144030, gCamera 0x80150B70, gControllerData = 0x80156CF0,
gWind = 0x80142726, gModelTime = 0x801543CC, gSkipToGameState = 0x801174BC, gCarTeam = 0x8012E67C, gObjList =
0x801427C0, gWaveInstance 0x80156D38, gNewGameState = 0x801174B8 (`state_word_b`), gFogColors = 0x80114658
(`playgame_settings`; content-proven), gSeconds = 0x8002EB90, gNumCars = 0x801543CA.

**Wrong existing labels** (hand GAME_SYMBOLS names that denote something else): playgame_settings (gFogColors),
state_word_a/b (gGameState/gNewGameState), active_player_count (gNumPlayers), gameplay_mode (gRace), input_rec0
(gPlayers; input_rec1 0x8014A164 = gPlayers[1]), player_array (gGameCar), countdown_state (gText). Speculative
symbol_addrs.us.txt labels contradicted: __osScElapsedTime/__osScDeltaTime (gSeconds/gFrameSeconds),
race_active (gPauseState), gstate/gstate_requested, gRaceControlByte (gRetireGame), gRaceTimer (gInvincible),
gStartTime (gCarModelNums), player_slots (gPaks), gTrackDataA (gRace), gTrackDataB (model), gPlayerCarState1
(gGameCar), gPlayerRacePositionFlags (controlScrActive), gShadowTextureId (gCircuitCars), gPlayerStatusArray
(gControllerData), gRaceResetFlag (gSoftReset), gScheduler (sc).

## How it would be applied (not done)

- Game and static-data names: hand-authored homes are `tools/conveyor/pipeline/disasm.py` GAME_SYMBOLS (and
  `include/game_types.h` for declarations/types), per CLAUDE.md. The established procedure is the w29n replay
  (`cloud/work/frontier/w29n/replay.sh`, w30n/w32n RESULTS): copy accepted rows of `renames_proposed.json` into
  `cloud/work/frontier/w29n/renames.json` with `status: accepted` and a batch name, add the reader table to
  docs/external/gc/names.md, then `RENAME_BATCH=<batch> sh cloud/work/frontier/w29n/replay.sh` on a clean checkout
  (gamesyms.py writes GAME_SYMBOLS' generated block; symaddr_rename for symbol_addrs labels; users.py/static_rename
  for every source that uses an old name; lock re-key with pure-rename proofs + recompile + image gate; gate chain to
  the ROM SHA-1). 351 rows is ~13x the earlier data batches: split it (e.g. the 30 high-value names first).
- `collision_files` on 133 rows: the new name already exists as an identifier in src/ (a parameter/local/field
  named like the global, e.g. `model`, `path`, `save`). The replay's token renamer must not rewrite those; 5 names
  that are too generic for a mechanical rename (sc, model, path, save, forward) are in `held` with a suggestion.
- Static-data addresses (0x8002xxxx: gSeconds, gFrameSeconds, sc, the unzip/zip tables) live in the boot segment;
  their current labels sit in symbol_addrs.us.txt, so they need the static rename path (symaddr_rename +
  static_rename proofs), not only GAME_SYMBOLS.
- Overlay data names (`overlay_renames`, 158 rows): no rename tooling exists for runtime-image names (w36n RESULTS:
  ovl_targets.py derives every name as func_/D_%08X); use them as comments / local `#define`s in matching sources.
- Types: `types_n64.h` is a draft for `include/game_types.h` additions; take only the fields marked same/moved with
  >= 2 votes as given, the rest as hypotheses to confirm against the target assembly.

## Open questions / caveats

- gObjList: proven at 0x801427C0 (19 functions index it at offset 0 with a variable index); w35e's "u16 array at
  0x8014254C" looks like the GC constant index 531 applied to an N64 access (0x80142972 - 2*531); the N64 object
  numbering differs (N64 index 217 there), so constant indices into gObjList must come from the N64 code.
- "volatile-like" is a heuristic (scalar read through a freshly formed address + 0(reg) at least as often as via a
  folded %lo); confirmed for gSeconds, gFrameSeconds, gNumCars, gNumModels; unverified elsewhere.
- Same-name GC statics (buttonInfo, buttonsInited, gButtonScale, blit, mblit ... one per screen file): placements are
  per GC object (key `name@0xGCADDR`); lane claims for them could not always be tied to a GC object (`name@lane:pop`).
- Overlay `n64_symbol` names come from the frozen per-image symbols.json; overlay rows are per image.
- Struct votes favour identical offsets; single-vote rows are leads. Unions are listed in types.md but not drafted.
- ties/tied (w36o FindWinner: D_80150B60/D_80150B68) disagree with the code channel (0x80143F50, strong); trust w36o.

## Identities proven (harvest for w36n and other lanes)

Content-proven tables (exact bytes, unique): stdtorquecurve 0x8011F724, frontTire 0x8011F814, backTire 0x8011F82C,
basic/forward/backward/shortie/stretch/truck 0x801108D0 + 0xB4 each, collsize 0x8011F844, shadow_scale 0x8011F914,
gControlTable 0x801118E4, gravityArray 0x801202F8, drone_sched 0x80120E74, pole_pos_offset 0x801210E8, gMineRingColors
0x80121D54, ammoCnts 0x80121D60, gDefStartTimes 0x8002E870, gBumpLRTab/gBumpFBTab 0x8011F020/40, FSCLS 0x801106C4,
gCollisionMass 0x80110E0C, carTransGearRatio 0x80110EBC, carTransTraction 0x80110F7C, carEngineTorque 0x801110C4,
carEngineFwdForce 0x80111130, gTireSideForce 0x8011121C, gCarFrame 0x80111230, carDurability 0x80111274, gWingSizes
0x801112D0, gFront/RearTireScale 0x801112DC/0x801113E0, suspensionLoose/Normal/Tight 0x801114F4/18/3C, gCarSuspensions
0x8011156C, gTireRims 0x8011157C, gCarHorn 0x801115C0, gTune 0x801116D0, ghostRaceTimes 0x80111754, gCircuitPoints
0x8011176C, gNumCircuitTracks 0x80111784, gCircuitFog 0x80111794, gWingDrag/Lift 0x8011416C/78, gWingAngVDrag
0x80114184, gBackRects 0x80114264, gBackCamera 0x801145E4, gFogColors 0x80114658, playerTextColor 0x801146AC,
flashColors 0x801146BC, gCarMiddles 0x8011744C, BodyVerts 0x801174CC, gTargetStructSizes 0x80117510, BBoxCollInfo
0x80118C10, flag_pals 0x80118E08, gFontColors 0x80118E34, gDummyVtx 0x8011AD90, wingOffsets 0x8011B4B4, arrowColors
0x8011B558, dm5Parts 0x8011E468, obstacle1Parts 0x8011E5B8, trackParts 0x8011E748, zip.c tables 0x8011E838..0x8011E9FC,
unzip.c border/cplens/cplext/cpdist/cpdext/mask_bits 0x8002B040..0x8002B184, gPlayerRects (controlscr 0x80113EE0,
highscorescr 0x801148E8, pausescr 0x80117100). Image B: BattleMultiBlit 0x80393DE8, stuntCoinTexNames 0x80393B40,
powerupIconNames 0x80393B68, whichCam 0x80393B74, playerPaneColors 0x80394250, playerColor 0x80394270, gShieldSizes
0x80394358, gStreakColors 0x80394394, weaponOffsets 0x803943A4, damColor 0x803948B8, CaseOffsets/ShotOffsets
0x80394AFC/0x80394B08. Image A: buttonNames (carscr 0x803AF984, startscr 0x803B30A0), componentToName 0x803B2600,
gTransStats 0x803B28C8, gHandlingStats 0x803B2948, gTireStats 0x803B2A08, gFrameStats 0x803B2A58, gCarOrder 0x803B3010,
buttonInfo@recordscr 0x803B5F28, gPlayerRects@playerscr 0x803B5D50, gViewStatValid 0x803B6554, mblit@trackscr
0x803B7A6C, trackMap 0x803B7CE4, gScreenRect@recordscr 0x803B8ACC.
Graded strong, not proven (unique content but fewer than 6 non-zero words): FSCLS, carDurability, gBumpFBTab,
gCarOrder, gCircuitFog, gStreakColors, gTargetStructSizes, gTireSideForce, gravityArray, trackParts.
