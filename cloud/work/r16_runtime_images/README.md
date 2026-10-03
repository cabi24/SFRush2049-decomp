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

## 4. Scoreable targets (added)

`python3 -m tools.conveyor.pipeline.ovl_targets generate` writes
`asm/us/ovl_a/` and `asm/us/ovl_b/` (region `.s`, `symbols.json`,
`extents.json`, `SHA256SUMS`). Output is deterministic: a regenerate is
byte-identical. Score with `tools/cloud/score.py fn SRC NAME --targets asm/us/ovl_b`.
CI rescores `cloud/matches/ovl_a/*.c` and `cloud/matches/ovl_b/*.c` against
their own image.

| | A | B |
|---|---:|---:|
| Functions | 192 | 49 |
| Text bytes | 152,952 | 38,708 |
| Empty functions (`jr ra; nop`) | 63 | 10 |
| Switch tables recognised | 25 | 6 |
| Unproven merges | 1 (`0x8038FF90`; see `extents.json`) | 0 |
| Call targets landing inside a function | 0 | 0 |

Every function records its start evidence (`jal`, `game_jal`, `prologue`,
`hilo_ref`, `data_ref`, `empty_function`, `image_base`). No switch destination
is a function start. `0x8038A400` is a tile start in both images. The game
calls it from `state_exit_handler` (image A per section 2), so treat B's entry
in `game_calls_here` as non-evidence.

**First true matches** (strict relocated equality through the scorer, IDO 5.3,
`-g0 -O2 -mips2 -G 0 -non_shared`; `cloud/matches/ovl_*`):

| Image | Function | Bytes | What it does |
|---|---|---:|---|
| B | `func_8038D1A8` | 88 | two `struct_fields_init` pool setups in B's BSS |
| B | `func_80391490` | 24 | `D_80399AE0++` (s8); R16 service called by `func_8010D3C0` |
| B | `func_803914A8` | 12 | `D_80399AE0 = 0`; called by `camera_lerp_position` |
| A | `func_80398BF0` | 68 | linked-list find by byte id in slot `D_80144D68[D_803B9BBA]` |
| A | `func_8039A24C` | 84 | `func_800A361C(x) || (D_803B46B4 == 2 && func_800A35F8(x))` |

**Compiler shape:** 76 of 192 functions in A, and 13 of 49 in B, write a
callee-saved register (`s0`–`s7`) that they never save. Examples are
`func_8038AE50` and `func_80399394`, which use `s0` with no `sw s0` anywhere.
Plain `-O2` never does this; it is IDO `-O3` interprocedural register
allocation within a file. Most of each image is therefore matchable only
through the call-group route (`cloud/work/ipa-groups/`, `score.py group`) with
the real callers in the same file. Single-function `-O2` scoring fits only the
self-contained leaves. A batch of 50 raw m2c seeds (functions of 400 bytes or
less) scored as expected for unedited seeds: none matched raw, and the five
matches above were hand-finished.

The 73 empty functions also match `void f(void) {}`. They are not submitted
individually. None of this is cartridge coverage yet: integrating C into the
runtime images needs an image build plus exact recompression of each ROM
stream, the analogue of `blob_rom`.

## 5. Integration prototype: C → image → exact ROM stream

`python3 -m tools.conveyor.pipeline.ovl_rom verify` compiles every
`cloud/matches/ovl_<x>/*.c` on the builder (private remote and local
directories, using the flags on line 1). It links each body alone at its
image address (`blob_splice.link_function`; excess non-zero words are
refused) and splices it over the protected target words. The data tail is
carried through unchanged. Then it gates in two steps:

1. **Image gate:** the composed image must be byte-identical to the
   decompressed original.
2. **Stream gate:** `deflate104` (zlib 1.0.4, level 9, raw) must reproduce
   the cartridge's stream at the image's ROM offset exactly.

Result on 2026-10-03: **A, 2 bodies → stream 80,272 B == ROM @ `0xB5C534`; B,
3 bodies → stream 23,639 B == ROM @ `0xB6FEC4`.** Negative control: changing
one body (`+= 2`) fails both gates and names the function.

**Stream reproducibility, all 119 deflate streams in the cartridge:**
`deflate104` reproduces **117** exactly from their decompressed bytes,
including the game image and both runtime images. Two asset streams do not
reproduce at any level 1–9: `0x360230` (47,736 B) and `0x36BCAB`
(186,040 B). They were made with other settings or another tool and must stay
opaque bytes for now.

**Remaining production step:** compose the source-built runtime-image
streams into the cartridge, as `tools/compose_data.py` does for the game
blob, then run the full-ROM SHA-1 gate on the builder. Add a lock for
runtime-image bodies and protect `asm/us/ovl_*`. These are maintainer-owned
production changes and are deliberately not made here.

## Next actions

1. Production integration (section 5): compose the runtime-image streams in
   the cartridge build, add a lock, and protect `asm/us/ovl_*` in
   `guard_paths.py`.
2. Census `0x8000F400–~0x80028000` functions against the libultra corpus.
3. Write B's contracts for `8038D798`, `8038D3A4` and `8039133C` (see #53).
