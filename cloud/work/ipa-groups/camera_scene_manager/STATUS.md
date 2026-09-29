# camera_scene_manager (trick/stunt scoring, not a camera)

No member is strict MATCH yet; the gap functions are close. Scored with
`-r4300_mul` (`zbuild.py`, plus `score.compare` for context functions):

| function | role | words | result |
|---|---|---|---|
| `func_800C2944` | member | 167 | 6 differ |
| `func_800C26C4` | member | 160 | 49 differ |
| `func_800C2430` | member | 165 | 81 differ (emits 164) |
| `camera_scene_manager` | member | 614 | 562 differ (emits 594) |
| `func_800C3578` | context, root (external callers) | 37 | 2 differ |
| `func_800C2004` | context | 130 | 6 differ |
| `func_800C220C` | context | 131 | 6 differ |
| `func_800C1B60` | context | 297 | 9 differ |

## What was done

- **All four closure gaps closed** with hand-written bodies:
  `func_800C1B60` (the score recorder, 297 words, two jump tables),
  `func_800C2004`, `func_800C220C`, `func_800C3578`. `func_800C3578` has
  callers outside the group, so it is in `keep`.
- **The IPA parameter is now right.** With `func_800C1B60` in the unit the
  members' player index lands in `$t1` as in the ROM; before it was `$a1`.
  `func_800C1B60(code, idx)` takes the event code in `$a0`, the player in `$t1`.
- **Seed defects fixed.** The seeded calls had scrambled arguments
  (`func_800C1B60(temp_f12, temp_f14, 5, ...)`); `D_8013FECB` is `s8`
  (`lb`), the hud flag at `+0x3F8` is `s16`; `fabsf` intrinsic kept.
  `D_80161388` is `f32[3]` (the three speeds), `player_array` a struct with
  the velocity at `+0x368`.
- All members and context are typed (`Ply`, `Stunt`, `PCar`, `HudRec`).
- `func_800C1B60` jump tables were recovered from the image data
  (`0x80123E94`: 12 entries, `0x80123EC4`: 8 entries).

## Remaining blockers

- `func_800C2944/26C4/2430`: one recurring difference. The ROM stores each
  accumulated component before loading the next car velocity
  (`swc1 f8,4(s1)` then `lwc1 f4,876(v0)`); IDO here hoists the second load
  above the store. Same source order, so the ROM's alias information must
  differ. Tried and no effect: plain/byte/absolute-address pointers, an
  `__inline` helper taking both pointers, an `f32*` car pointer, an unrolled
  loop. This is 5 of the 6 differing words in `func_800C2944`. `func_800C2430`
  also needs one extra reload of the first speed (`lwc1 $f0,28(sp)`).
- `func_800C2004/220C`: the `0.25f` constant sits in `f0` in the ROM and `f2`
  here, and the ROM's loop head is `move t1,s1; jal; li a0,N` (mine puts
  `li a0,N` first).
- `func_800C1B60`: only the register that carries `add` differs (`v1` in the
  ROM, `t7/t8/t9` here). The variable is stack-resident in both.
- `func_800C3578`: spill slot at `36(sp)` in the ROM, `32(sp)` here.
- `camera_scene_manager`: needs the per-iteration order of the `0x1F0` flag
  test, `hud->f3F8` test and the `st->f60` update to match the ROM's
  scheduling, and the hoisted float constants (`D_80123F3C` vs `D_80123F50`).
