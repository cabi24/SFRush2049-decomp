# cpak_init (tyre marks, not Controller Pak)

**BUILDS** (cloud pass 2026-09-29), hand-written from the assembly; not
matching yet. Scores with the game's `-r4300_mul` assembler flag
([../../R4300_MUL.md](../../R4300_MUL.md)):

```
cpak_init       244/265
func_800AF8C0    39/113
save_validate   129/135
```

## What this is

Per-wheel tyre marks: four `WheelSlot`s (0x5C bytes) per player at
`D_80155290`, each holding a pooled display object from `D_80155220`.

| label | role |
|---|---|
| `cpak_init(s16 player)` | per wheel: start, extend or end a mark depending on contact, terrain and surface flags |
| `func_800AF8C0(slot, player, wheel, color)` | place the mark's leading edge across the axle (wheels `wheel\|1` and `wheel&2`), push it to the display list |
| `save_validate(player, wheel, color, slot)` | take an object from the pool, stealing the one farthest from the camera (`D_80150B94`) if empty; start the mark |

IPA registers in the ROM: `func_800AF8C0` takes `slot` in `$s0`, `player`
in `$t1` (an `s16` stored to the second parameter's home slot, so
`player` is the second parameter), `wheel` in `$s4`, `color` in `$s5`;
`save_validate` takes `player` in `$s3`, `wheel` in `$s4`, `color` in
`$s5`, `slot` in `$s6` and passes them on.

## Known remaining differences in func_800AF8C0

- The frame is 104 bytes with the two vectors at `sp+64`/`sp+76` and 16
  unused bytes above them; ours puts them at the top. Unreferenced padding
  locals are optimized out, so they cannot fake it.
- `D_8002EB90` is read through a materialized address (`lui/addiu` then
  `lwc1 0(reg)`); `D_80123C08` is read once for both adds (now a local,
  which fixed that part but spills).

## Relocations (for review; resolved by the strict scorer)

`player_array`, `D_8014A250`, `D_80155290`, `active_player_count`,
`state_word_a`, `D_8011743C`, `D_80117438`, `D_8011AD8C`, `D_8011AD90`,
`D_801497F8`, `D_80123C08`, `D_80123C0C`, `D_8002EB90`, `D_80155220`,
`D_80150B94`, `D_80161378`; calls `func_8008E3C0`, `func_800AFA84`,
`func_8008D0C0`, `func_800A78BC`, `func_8008C074`, `vector_copy_scale`,
`func_800AF844`.
