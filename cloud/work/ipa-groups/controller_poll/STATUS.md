# controller_poll

**3 of 7 members MATCH** (49 of 602 words), rescored 2026-09-30
(`zbuild.py --as1=-r4300_mul`). `func_800C9590` is spliced
(`src/blob/groups/controller_poll`); `player_mode_set`/`player_state_set` match
here but are not spliced.

| function | role | target words | result |
|---|---|---|---|
| `func_800C9590` | member | 19 | **MATCH** |
| `player_mode_set` | member, `keep` | 15 | **MATCH** |
| `player_state_set` | member, `keep` | 15 | **MATCH** |
| `func_800E7038` | member | 63 | 61 differ (emits 60) |
| `func_800E7134` | member, `keep` | 169 | 165 differ |
| `process_inputs` | member, `keep` | 89 | 89 differ (emits 110) |
| `controller_poll` | member | 232 | 223 differ (emits 209) |

## Closure gap

`func_800E73D8` (the caller of `func_800E7134`) exchanges registers with the
group and is not in it.

## Changes to the seed

- `controller_poll` rewritten from the assembly: pad read under the
  `D_801497A8` message-queue lock, stick scaling, held/pressed masks, per-button
  auto-repeat timestamps (`D_80149B90[4][32]`, delay `D_80123F94 * D_8002AFB4`).
  Seed's unset `$t0..$t5/$a2/$a3` were loop-carried pointers in caller-saved
  registers. Prelude widths are wrong for this code (`D_8011195C` `s8`,
  `D_80111960` `u8`, stick tables `s8[4][2]`); `group.c` reads them through
  `#define` typed views, relocations unchanged.
- `func_800C9590(range, calib, raw)`: scale a raw stick byte to `[-range, range]`;
  `raw` in `$a0`, floats in `$f12/$f14`; clamp is
  `if (v < -range) v = -range; else if (range < v) v = range;`.
- `func_800E7134` calls `func_800C9590(1.0f, cal[k], raw[k])`; unset `$t4` is
  the constant 70 (default stick calibration).
- `player_mode_set`/`player_state_set` take an `s32` and fill all four entries
  when `player == -1`; `D_80149B64`/`D_80149B74` are `s8[4]`.

## Technique: as1 merges `lui at` only for symbols defined in the unit

Consecutive stores share one `lui at` only when the array is *defined* in the
same translation unit (`s8 D_80149B74[4];`, not `extern`); with the plain
`for (i = 0; i < 4; i++) D[i] = value;` loop this gives the ROM's order
`[1],[2],[3],[0]`. **Splicing note:** `group.c` defines `D_80149B64[4]` and
`D_80149B74[4]`; do not define them elsewhere.

## Remaining: `func_800E7038`

The target's eight `sb` of 0x46 share one `lui at,0x8015` (offsets
`-25858..-25863`): one defined array, `D_80149AF8[8]`, pairs stored high to low,
all stored unconditionally before the `D_80111968` test (the ninth `sb` is in
the `bnez` delay slot; the seed had `D_80149AF9` inside the `if`). With
`s8 D_80149AF8[8]` defined and a pair loop the stores merge, but the constants
(one register per store in the target) and the `D_80111968` load order do not
match. `controller_poll`, `process_inputs` (IDO emits 21 extra words) untuned.

## Round 3 (no change: `func_800E7038` 61/63 differ)

Defined `s8 D_80149AF8[8]` with all eight stores unconditional (orders
6,7,4,5,2,3,0,1), plain, `volatile`, `volatile s8 *p`, up/down/pair loops, named
`c0..c7` locals: the array base is then hoisted into a register (`lui v0; addiu
v0` + `sb n(v0)`), size 52-56 words, not the ROM's `lui at` + `sb -258xx(at)`
with eight separate `li 70`. Eight *defined scalars* `D_80149AF8..F` give
60 words but one `lui at` per store (no merge). Neither reaches the ROM shape.
