# func_800D2FA8 (track path search: node graph walk)

**1 of 3 members MATCH** (`time_of_day_select`, 50 of 400 words), rescored
2026-09-30 (`zbuild.py --as1=-r4300_mul`). Hand-written from the assembly. Not spliced.

| function | role | target words | result |
|---|---|---|---|
| `time_of_day_select` | member | 50 | **MATCH** |
| `split_time_display` | member | 60 | 14 differ (size 60/60, frame 8/8) |
| `func_800D2FA8` | member, `keep` | 290 | 264 differ (zbuild size 292: trailing pad nop) |
| `func_800B98D8` | context, `keep` | 77 | **MATCH** |
| `minimap_render` | context, `keep` | 69 | **MATCH** |

Closure: the two context functions come from group `func_800B9B64` (same
module: path graph `D_801407F0`, sections `D_80151CE8`) and must be in the unit
for calls to link. `func_800D3430` (outside caller of `func_800D2FA8`) and its
callers are not needed.

## What the code is

| function | role |
|---|---|
| `time_of_day_select(node, pos, outNode, dist, depth)` | follow `next` links summing point counts into `*dist` (tail recursive; IDO makes a loop) |
| `split_time_display(node, pos, remain, outNode, outPos)` | walk `remain` points back along `next` links (tail recursive) |
| `func_800D2FA8(node, pos, outNode, outPos, stopAtTyped, depth)` | find the node/point reached from `(node,pos)`: checks visited list `D_80124F88`, handles `next == prev` loops inline, else measures both ways round with `time_of_day_select`/`minimap_render`, finishes with `split_time_display`; recursive on the `next == prev` path |

## Techniques that worked

- **Locals layout.** IDO gives every referenced named scalar local a stack slot,
  first declared at the highest address. `func_800D2FA8`'s seven `int` locals
  are declared `dist1, total, dist2, start1, node2, pos2, start2` to land at
  `sp+96..72`.
- **Frame padding.** ROM frame is 8 bytes larger than needed; an unreferenced
  `volatile s32 padv[2];` declared first reproduces it (plain unused locals are dropped).
- `split_time_display`: recursive C (`if (pos >= remain) {...} else recurse`)
  with **no** named local for the section start (a `start` local makes frame 16
  not 8); tail-call conversion produces the two dead stores at `sp+4/sp+0`.
  Test `if (*outPos < 0)` after `*outPos = pos - remain;` (not `pos - remain < 0`):
  uopt forwards the stored value, so it is no longer a cross-block CSE temp
  competing for registers (34 -> 14 differing words).
- Reusing the `node` parameter as loop variable (`node = *outPos; while (...)`)
  moves the value into `$a0` as in the ROM.

## Blockers

- `split_time_display`: ROM section start in `$v1`, section/graph bases in
  `$t0/$t2` (`$t1 = 80` for the `mul`); ours puts start in `$t0`, bases one
  higher. Left: `t`-temps of the loop recompute and the two arms (`t6,t7,t8` vs
  `t7,t8,t9`, `t3/t4` rotation). Same "first temp" shift as `func_8008B640`.
- `func_800D2FA8`: `pos`/`outPos` copies in `$t4/$t5` in the ROM, `$t3/$t4` here;
  the unrolled visited-list loop has one extra `move a1,a2` in the ROM. A random
  search found only semantically wrong edits.

## Round 3 (no change: `split_time_display` 14/60)

All 14 differences are one-step temp register rotation (`t6` vs `t7` chain in
the loop, `t9` vs `t3` for the second `pos - remain`). Tried named `d`/`e`
locals for `pos - remain`, `+=` form, `remain <= pos`, one-line ifs, store
order swaps, `t = *outPos`: none better (14-53 words).
