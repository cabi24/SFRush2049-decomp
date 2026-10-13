# w28g: GameCube reference tooling (RESULTS)

Lane w28g, wave 28, worktree branch `w28g` from `wave28` = e3b1f926. Tooling and docs only: no src/, asm/,
locks or root GC files touched. Entry point for lanes: [docs/external/gc/README.md](../../../../docs/external/gc/README.md).

## Deliverables

| Path | What |
|---|---|
| `tools/gcref/elf.py` | ELF32-BE reader (sections, symtab with FILE attribution, string reads); stdlib |
| `tools/gcref/dwarf1.py` | CodeWarrior DWARF v1 decoder → `types.h`, `functions.json`, `globals.json`; stdlib, <1 s |
| `tools/gcref/disasm.py` | capstone disassembly, symbolised from the ELF's own `.rela.*` → `reference/gc/disasm/**.s` (ignored), `reference/gc/features.json` (ignored), `callgraph.json`, `strings_xref.json`; ~5 s on the Pi |
| `tools/gcref/correlate.py` | N64 (game + overlay A/B) → GC candidate table → `correlation.md`/`.json`; stdlib, ~5 s |
| `docs/external/gc/` | README + the committed outputs above (3.9 MB total) |
| `tests/cloud/test_gcref.py` | 6 tests: DWARF counts, struct layout, signature/locals, disasm symbolisation (sda21, string, float, paired-single), table strings, correlation seed + string pair; skip without rush_d.elf / capstone / N64 build |
| `.gitignore`, `docs/README.md`, `docs/external/README.md` | `reference/gc/` ignored; README linked from both indexes |

## Decoded counts (vs writeup.md)

All six counts match writeup.md exactly: 2,431 structs, 26,640 members, 178 unions, 130 enums,
2,757 params, 5,591 locals (149 CUs, 980 stripped entries). After de-duplicating per-CU copies (iterated so
nested anonymous types collapse): **336 structs, 13 unions, 120 enums** in `types.h`. 1,844 linked functions
with debug info (2,368 params, 4,321 locals in linked functions; the rest belong to stripped functions and
subroutine types), 3,289 typed fixed-address variables. Findings: CodeWarrior emits **no TAG_typedef, no
TAG_pointer_type and no lexical blocks** (pointers/const are modifiers; nested-block locals are flattened),
so typedef names are lost; anonymous aggregates carry a `used by:` line (globals/members of that type)
instead. All 120 enums are 4 bytes. Modifier order verified empirically: `[pointer, const] char` is the common
pattern, i.e. `const char *` (outermost first).

Disassembly: 3,181 functions, 8,531 call edges, 1,294 strings referenced (272 directly, 1,022 only through
pointer tables). The ELF keeps `.rela.text`/`.rela.data`/`.rela.rodata`/`.rela.sdata*` (43,998 text relocs),
so symbolisation is exact (no lis/addi register tracking). **capstone needs `CS_MODE_PS`**: with plain
`CS_MODE_32|CS_MODE_BIG_ENDIAN` the Gekko `psq_st`/`psq_l` in every float-saving prologue decode as
`xsmaddmsp` etc. (the "verified" mode in the brief is wrong on those words).

## Correlation precision

Seeds: libc/libm `sprintf strlen strchr memcpy memset memchr sinf cosf modf` + inflate `huft_build
inflate_block/dynamic/fixed/stored` (14; `inflate_codes` does not exist on GC). The **same-named N64 game
symbols are not usable seeds**: `AdjustSpeed`/`AdjustSteer`/`no_catchup` are the only ones that are function
starts in asm/us/blob, and they are speculative arcade attributions from `symbol_addrs.us.txt`; N64
`AdjustSpeed` (0x800A2680) calls `osPfsDeleteFile`/`osPfsFreeBlocks`/`osRecvMesg` (a controller-pak
routine). `CheckCPs` (0x800C0000), `InitDrones` (0x8009FF00), `DoDrones` (0x800A0000), `MaxPathControls`
(0x800A1000) sit at round placeholder addresses that are not function starts.

Held-out evaluation (in `correlation.md` header): truth = 22 string-anchored pairs (strings alone score
>= 3 and twice the runner-up); two-fold, each half held out with its string channel masked:
- top-1 recovered by the other channels: **8/22** (19 had a candidate, top-3 11/22);
- confident pairs among held-out: **4/4 correct** (recall 4/22);
- with all channels: 21/22 confident.

So: confident pairs are reliable on this (small) test; non-string top-1 suggestions are right about a third
of the time and should be read as "look in this GC file". Totals: 1,458 N64 functions (1,216 game, 242
overlay), 746 with a candidate, **72 confident pairs** (17 unmatched game, 31 matched game, 24 overlay).

## Top leads for the N64 unmatched pool (by subsystem)

`*` = confident. Score and the main evidence from `correlation.md`.

Countdown / attract / title
- `*audio_doppler_full` 0x800B61FC (952) → `ShowCountdown` [countdown.c] 5.4: str "CNTDWN%d", "CNTDWN3". N64 label is wrong.
- `*func_80100B8C` 0x80100B8C (408) → `PreDrawPad` [ngcBeginMenu.c] 3.0: str "RUSH 2049".

High score / records / results
- `*func_800F0914` (1584) → `_PrintScores` [highscorescr.c] 6.5: default-name table x20, flt 6969, file cluster.
- `*func_800F1210` (1816) → `_SetHighScoreState` [highscorescr.c] 4.4: imm 0x138, calls the `_PrintScores` pair.
- `func_80103D28` (2516) → `DrawUnlocked` [highscorescr.c] 3.8 (runner-up 3.5): car-name table x13.
- `func_800F0F44` (464) → `_FindGhosts` [highscorescr.c] 2.2: file cluster only.
- `*game_results_input` (1732) → `animControls` [controlscr.c] 3.8: str "CONTROLS_SM".
- `func_80105EA8` (2508) → `animResults` [hud.c] 2.8: str "%s\n%s".
- `func_80100E58` (2732) → `drawRaceStandings` [gameoverscr.c] 2.0: str "DRONE", "%d:%02d.%03d" (weak).
- ovl_a `*func_803AD57C` (2092) → `drawView` [recordscr.c] 6.5: calls the `drawTotalStats`/`drawRaceStats` pairs.

Menus / options / player / pak (overlay A)
- `*func_800DA2C0` (2316) → `updateButtons` [optionscr.c] 6.1: flt 1.33, 31, 310; `*func_800DB1E0` (1400) → `updateOptionScreen` 3.4.
- `func_8010A8D0` (1500) / `func_8010AEAC` (1652) → `animButtonText`/`animText` [optionscr.c] ~3: language table (DEUTSCH, ITALIANO, NEDERLANDS).
- `*drone_collision_avoid` (852) → `calcMenuRect` [audioscr.c] 4.2: song-name table, str "10" (N64 label wrong).
- ovl_a `*func_803A1EAC` (7104) → `updateCarScreen` [carscr.c] 14.6; `*func_8039EF08` → `showCarSelect`; `*func_803A0FD8` → `_InitScreen`; `*func_8039D6A4` → `updateButtons` [carscr.c].
- ovl_a `*func_8038F79C` → `updateButtons` [startscr.c] 7.3 (str "CURSOR_B"); `*func_80390418` → `updateStartScreen`.
- ovl_a `*func_80397FFC` → `initScreen` [playerscr.c]; `*func_8039842C` → `updatePlayerScreen`; `func_803A9EEC` → `drawPakFull` [playerscr.c] 3.4 (str "PAK FULL").

Save
- `func_800CD104` (1088) → `SaveServer`/`GetGhostExtension` [save.c] 2.1: str "RUSH2049" (ambiguous).

Track / world / camera / visuals
- `*func_800BB9B0` (2104) → `LoadTrack` [tracks.c] 13.0: track-name table x19, calls the SkyInit/InitSmoke pairs.
- `*func_80096CA8` (1204) → `_SetupWaveModel` [waves.c] 9.5: FourCC immediates IMAG/OBHD/PLHD/PTHD/TXHD (a model-file parser, not audio); `func_80097164` → `_ReadWave` 2.8.
- `*func_800EB028` (1640) → `update_camera` [camera.c] 4.9; `*func_800EB90C` (2000) → `CheckCameraView` 3.8.
- `*drone_ai_update` (3280) → `AnimateFrame` [visuals.c] 6.9 (str "MINE1"); `*entity_tick_main` → `AnimateRocketFlame`; `*entity_render_setup` → `AnimateArrow`.
- ovl_b `*func_8038E114` (5196) → `UpdateShots` [shots.c] 12.8; `*func_8038FCE0` → `ShotUpdate`; `*func_8038AA8C` (7812) → `AnimateWeapon` [weapons.c]; `*func_8038A408` → `newWorldDamage`.

## Caveats

- The GC code is a divergent sibling: a pair gives names, intent, struct field meanings and constants, not
  instruction order. Arcade source stays first where it exists.
- Many N64 symbol names are speculative and wrong (examples above); the correlation is often a better name
  lead than the label.
- Short or format-only strings and single shared floats are weak evidence; check the evidence column.
- `types.h` layouts are CodeWarrior/PowerPC; check alignment and missing fields against the N64 accesses.

## Next steps

- Feed the confident pairs into lane briefs (names + `functions.json` locals for the 17 unmatched game and
  the overlay functions).
- Extra channel worth adding: struct-offset fingerprints (sets of load/store displacements off one base
  register) compared against `types.h` member offsets; would help the large screen functions with few strings.
- `offroad_d.elf`/`rock.elf` on the same disc (writeup.md) could be decoded with the same tools for a third
  reference (Rush the Rock is the arcade sibling).

## Tests

`make test-fast JOBS=4` on the Pi, in the worktree: **tests/cloud 777 passed, 196 skipped** (includes the 6
new `test_gcref.py` tests); tests/conveyor 1,511 passed, 40 failed, 2 collection errors, all environmental
(the worktree has no `build/` and no submodule checkouts: `src.scorer` from the permuter, `tools/mips_to_c`
for the m2c tests, bundled objdump/toolkit). The same tests/conveyor suite in the **main checkout** (same
commit e3b1f926, which the w28g change does not touch in any conveyor path) gives **1,623 passed, 777
skipped, 0 failed**, which covers the m2c test files there.
