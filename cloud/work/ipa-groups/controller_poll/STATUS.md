# controller_poll

**BUILDS** (cloud pass 2026-09-29). `func_800C9590` MATCHES. With
`-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)):

```
controller_poll    223/232
func_800C9590      MATCH
func_800E7038       61/63
func_800E7134      165/169
player_mode_set     13/15
player_state_set    13/15
process_inputs      89/89
```

## Changes to the seed

- `controller_poll` rewritten from the assembly: pad read under the
  `D_801497A8` message-queue lock, stick scaling, held/pressed masks,
  and per-button auto-repeat timestamps (`D_80149B90[4][32]`, delay
  `D_80123F94 * D_8002AFB4`). The seed's unset `$t0..$t5`/`$a2`/`$a3` were
  loop-carried pointers kept in caller-saved registers across calls.
  Several prelude globals have the wrong width for this code
  (`D_8011195C` is `s8`, `D_80111960` is `u8`, the stick tables are
  `s8[4][2]`); `group.c` reads them through typed views (`#define`s over
  the same symbols, so relocations are unchanged).
- `func_800C9590(range, calib, raw)` (19 words, **MATCH**): scale a raw
  stick byte to `[-range, range]`. IPA passes `raw` in `$a0` and the two
  floats in `$f12`/`$f14`. The clamp is `if (v < -range) v = -range; else
  if (range < v) v = range; return v;`.
- `func_800E7134`: its two `func_800C9590` calls now pass
  `(1.0f, cal[k], raw[k])`; the unset `$t4` is the constant 70 (default
  stick calibration written to both axes).
- `player_mode_set` / `player_state_set` take an `s32` value and fill all
  four entries (`[0]` included; the seed dropped it) when `player == -1`.
  Both are in `keep` (many callers outside the group). Remaining diff: the
  ROM stores the four bytes under one `lui`; our build re-emits `lui` per
  store. `D_80149B64`/`D_80149B74` are now declared `s8[4]`.
- The IPA closure check found one more function that exchanges registers
  with this group: `func_800E73D8` (the caller of `func_800E7134`).

## Relocations (for review; resolved by the strict scorer)

`D_801497A8`, `D_8011195C`, `D_80111958`, `game_loop_tick`,
`D_80111960`, `D_80156944`, `D_80149784`, `D_8015694C`, `D_80149B10`,
`D_80156958`, `D_80143A00`, `D_80156978`, `D_80156998`, `D_80123F94`,
`D_80149AF8`, `D_80149B50`, `D_80149B30`, `D_80149B64`, `D_80149B74`,
`D_80149B90`, `D_8002AFB4`; calls `osRecvMesg`, `osJamMesg`.
