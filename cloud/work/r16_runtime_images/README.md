# R16: runtime images and boot-segment extent (2026-10-03)

**Status:** READY-RESEARCH output for R16 (also feeds P01, S12 and S13).
Read-only analysis of `baserom.us.z64` (sha1 `3f99351d…`). Only offsets,
counts and names are published here. Reproduce with
`python3 cloud/work/r16_runtime_images/survey.py` (→ `survey.json`). Builds on
`cloud/work/r16_rom_stream_census/`.

## 1. Two runtime images share `0x8038A400`

| | Image A | Image B |
|---|---|---|
| ROM (raw deflate) | `0xB5C534`, 80,272 B | `0xB6FEC4`, 23,639 B |
| ROM pointer | `D_8002B020` (ROM `0x2BC20`) | `D_8002B024` (ROM `0x2BC24`) |
| Decompressed | 194,128 B → `0x8038A400–0x803B9A50` | 43,888 B → `0x8038A400–0x80394F70` |
| Loader (historical name) | `sync_maxpath_to_checkpoint`, flag `D_8011ED00`, BSS clear `0x803B9A50..0x803BAE20` | `InitMaxPath`, flag `D_8011ED04`, BSS clear `0x80394F70..0x8039B440` |
| Code | 195 `jr ra` | 49 `jr ra` |
| Embedded asset names | `TURNTABLEG1`, `CARSTATBARS_LG`, `BUTTON_TITLE`, `CURSORG1`, `ENGINE%02dG1` | `STNT_FLIP_F`, `STNT_HELI`, `TACHOMETER_LG`, `HEALTHBAR`, `WPR_MISSG1`, `WPR_MINE`, `WFX_SHIELDG1`, `BCOIN_*` |
| Reading | **front end / car selection** | **race HUD, stunts, battle weapons** |

**Base proof:** every internal `jal` target lands on a function start at base
`0x8038A400` (A 96/96, B 20/20). Base ±0x10 gives 1–5 hits. Each BSS clear
range from `codex_heap_load_a14` begins exactly at its image's end. The
"MaxPath" loader names are historical labels. Per that packet, these
functions are guarded image loaders (`PrevMaxPath` = reserve, inflate, clear
BSS, set flag).

## 2. Every game call into the window, resolved

`game_calls.json`: the game image has 30 distinct `jal` targets in
`0x8038xxxx`. A target is assigned to an image when it is a function start
there (prologue, or directly after `jr ra; nop`) and lands mid-function in the
other. **A: 14, B: 16.** Two needed a manual look:
`0x8038A400` is A's first function, which begins with `lui`; its only caller
is `state_exit_handler`, which otherwise calls only A. `0x8038A408` is
mid-prologue in A but a function directly after B's `jr ra` stub.

R16's named services:

| Service | Game caller | Image |
|---|---|---|
| `8038D798` | `func_8010C974` (the C974 contract) | **B** |
| `8038D3A4` | `func_8010C7F4`, `menu_options_screen` | **B** |
| `8039133C` | `engine_sound_stop` | **B** |
| `80391490` | `func_8010D3C0` | **B** (leaf start after `jr ra`; mid-function in A) |
| `8038F454` | `func_800D5A04`, `state_exit_handler` | **A** (leaf start after `jr ra`; mid-function in B) |

Contract requests should be written as `image:address`, e.g. `B:8038D798`.
Caller names are current symbol labels and are not re-verified here. Several
callers of B are race-time (`particles_spawn`, `skid_mark_render`,
`camera_*`); most callers of A are `state_exit_handler`.

## 3. Boot segment extends past the counted static code

The entry stub clears BSS from `0x8002E8E0` (`lui t0,0x8003; addiu t0,-0x1720`)
and jumps to `main` at `0x800020F0`, so the resident boot image is ROM
`0x1000–0x2F4E0` → `0x80000400–0x8002E8E0`. Coverage counts only ROM
`0x1000–0x10000` (230 functions, 61,440 B). Per-1 KB classification from
`0x10000` (`c` = CPU code, `r` = RSP vector code, `.` = data):

```
ccccccccccccc.c..cccccccccccccccccccccccccccc..cccccccccccccccccccccccccccccccccccc..ccccccccc..cccrrrrrrrrrrrr.....c.........
```

- **ROM `0x10000–~0x28C00`: CPU code, about 100 KB, uncounted.** It holds
  `symbol_addrs.us.txt` entries such as `__osContRamWrite` (`0x8000F680`),
  `osEPiRawStartDma` (`0x8000FE00`) and `__osPiGetCmdQueue` (`0x800100C0`):
  more libultra, not game code.
- **ROM `~0x28C00–0x2BC00`: RSP microcode.** There are dense `COP2`/`lwc2`/`swc2`
  words. It is not C and belongs in no matching denominator.
- **ROM `0x2BC00–0x2F4E0`: data.** It includes the image pointer table and the
  libm/fcvt constants already named (`gSinCoeffs` `0x8002D750`, …).

## Consequences

- **R16:** the same-address/different-image ambiguity is resolved for every
  game call site. C974 (`B:8038D798`), collision (`B:8038D3A4`) and engine
  sound (`B:8039133C`) now need image B's function contracts. The images'
  code is extractable from the ROM like the game image, so it is not an
  external input.
- **New population:** the two images are about 238 KB of game code outside
  both current denominators. They need their own targets (base `0x8038A400`,
  image-qualified names) before matching.
- **P01/S12:** the static population should extend to `~0x28C00` (CPU code
  only) through the layout tooling, as a reviewed denominator change. The
  numbers above are evidence, not a new metric.

## Next actions

1. Extract image-qualified targets (`A_8038xxxx`/`B_8038xxxx`) with function
   boundaries from call targets plus prologue scan. Extend the scorer for a
   second and third image base.
2. Census `0x8000F400–~0x80028000` functions against the libultra corpus
   (expected high yield for library matches).
3. Write B's contracts for `8038D798`, `8038D3A4` and `8039133C` (R03/R06).
