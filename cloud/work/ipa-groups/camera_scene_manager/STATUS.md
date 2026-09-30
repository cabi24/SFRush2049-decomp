# camera_scene_manager (trick/stunt scoring, not a camera)

**3 of 4 members MATCH** (492 of 1,106 member words), rescored 2026-09-30
(`zbuild.py --as1=-r4300_mul`; context via `score.compare`). Not spliced.

| function | role | target words | result |
|---|---|---|---|
| `func_800C2944` | member | 167 | **MATCH** |
| `func_800C26C4` | member | 160 | **MATCH** |
| `func_800C2430` | member | 165 | **MATCH** |
| `camera_scene_manager` | member, in `keep` | 614 | 552 differ (emits 598) |
| `func_800C3578` | context, root (external callers), `keep` | 37 | 2 differ (emits 39) |
| `func_800C2004` | context | 130 | 6 differ |
| `func_800C220C` | context | 131 | 6 differ |
| `func_800C1B60` | context | 297 | 9 differ (2 jump-table relocations unverified) |

## Closure

No gap left: `func_800C1B60` (score recorder, two jump tables recovered from the
image at `0x80123E94`, 12 entries, and `0x80123EC4`, 8 entries), `func_800C2004`,
`func_800C220C` and `func_800C3578` are hand-written context. `func_800C1B60(code, idx)`
takes the event code in `$a0` and the player in `$t1`; with it in the unit the
members' player index lands in `$t1` as in the ROM.

## Techniques that worked

- **Launder the car pointer through `(u32)`**: `c = (PCar *)(u32)&player_array[idx];`.
  The ROM stores each accumulated component before loading the next car
  velocity; without the cast uopt hoists the second load above the store. This
  fixed `func_800C2944/26C4/2430`.
- Typed records (`Ply`, `Stunt`, `PCar`, `HudRec`); seeded calls had scrambled
  arguments; `D_8013FECB` is `s8`, hud flag at `+0x3F8` is `s16`, `D_80161388` is
  `f32[3]`, velocity at `+0x368` of the car struct.

## Remaining blockers

- `camera_scene_manager`: per-iteration order of the `0x1F0` flag test,
  `hud->f3F8` test and the `st->f60` update, plus the hoisted float constants
  (`D_80123F3C` vs `D_80123F50`).
- `func_800C2004/220C`: the `0.25f` constant is in `f0` in the ROM, `f2` here;
  ROM loop head is `move t1,s1; jal; li a0,N` (ours puts `li a0,N` first).
- `func_800C1B60`: only the register carrying `add` differs (`v1` in ROM,
  `t7/t8/t9` here).
- `func_800C3578`: spill slot `36(sp)` in the ROM, `32(sp)` here.
