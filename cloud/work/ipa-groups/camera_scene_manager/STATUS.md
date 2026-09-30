# camera_scene_manager (trick/stunt scoring, not a camera)

**3 of 4 members MATCH** (492 of 1,106 member words), rescored 2026-09-30
(`zbuild.py --as1=-r4300_mul`; context via `score.compare`). Not spliced.

| function | role | target words | result |
|---|---|---|---|
| `func_800C2944` | member | 167 | **MATCH** |
| `func_800C26C4` | member | 160 | **MATCH** |
| `func_800C2430` | member | 165 | **MATCH** |
| `camera_scene_manager` | member, in `keep` | 614 | 546 differ (emits 611; was 552 differ, emits 598) |
| `func_800C3578` | context, root (external callers), `keep` | 37 | 2 differ (emits 38) |
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

## Pass 4 (Round 3) findings

Rescored 2026-09-30 with `zbuild.py --as1=-r4300_mul`; register-blind the
`camera_scene_manager` structure now matches 518 of 614 target words.

- **The 4 hoisted globals in `$s5..$s8` need the counters written as `+= 1`.**
  The ROM keeps `&D_80157238` (`$s5`), `&D_8015B258` (`$s6`), `&D_8015B248`
  (`$s7`) and `&D_801543CC` (`$s8`) in callee-saved registers for the whole
  loop and never hoists the integer constant 1. With `D_8015B258 = 1; ...`
  (assignments after the zeroing stores) uopt promotes `li 1` to `$s6` (10 or
  more uses) and the fourth address loses its register. Writing every counter
  update in the four `hud->s61C/61E/620/622 != 8` blocks as `D_xxxx += 1;` gives
  the ROM's four hoists, no constant web, and 611 words (was 598, 3 short).
  Other constants: `n = 1`, and the six `D_80156BC8/BD8/CE4 = 1` stores stay
  as `li` per use.
- The ROM turns some of those `+= 1` after a known zero into `li 1` and some
  into `addiu t8,a0,1` with a zero register (`move a0,zero`, `move a2,zero`
  before the first block, for `D_8015B258` and `D_8015F728`). We still load
  and add for `D_8015F730`; that is 3 words plus the registers that follow.
- Tried without effect: `n++` for `n = 1`, changing the pre-loop
  `D_80152738 = 1`, extra `D_8015B258` reads, and mixed `= 1` / `+= 1` for the
  first touches (32 combinations; all `+= 1` is best, `= 1` for the first
  touches brings back the `li s6,1` hoist).

## Remaining blockers

- `camera_scene_manager` (546 differ, 611 of 614 words): per-iteration order of
  the `0x1F0` flag test, `hud->f3F8` test and the `st->f60` update, the
  hoisted float constants (`D_80123F3C` vs `D_80123F50`), the address of
  `D_8015F728/F730` (ROM: `lui/addiu` into `$a3/$t0` shared by the three zero,
  set and increment uses of one iteration; ours: separate `lui at`), the
  zero-register form above, and the order in which the loop preheader
  materialises `$s2, $s4..$s8` (`lui s4; lui s8; lui s7; lui s6; lui s5`).
- `func_800C2004/220C`: the `0.25f` constant is in `f0` in the ROM, `f2` here;
  ROM loop head is `move t1,s1; jal; li a0,N` (ours puts `li a0,N` first).
- `func_800C1B60`: only the register carrying `add` differs (`v1` in ROM,
  `t7/t8/t9` here).
- `func_800C3578`: spill slot `36(sp)` in the ROM, `32(sp)` here.
