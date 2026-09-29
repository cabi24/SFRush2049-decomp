# func_800D2FA8 (track path search: node graph walk)

Rewritten by hand from the assembly (cloud pass 2, 2026-09-29); the m2c seed is
superseded. Builds; **1 of 3 members matches** with `-r4300_mul`
(`cloud/work/tools/zbuild.py --as1=-r4300_mul`):

```
time_of_day_select   50/50    MATCH   (was a prototype only; now a member)
split_time_display   34/60    words differ (size 60/60, frame 8/8)
func_800D2FA8       264/290   words differ (size 292/290: zbuild counts the trailing pad nop)
```

## What the code is

The same module as group `func_800B9B64` (track path graph `D_801407F0`,
sections `D_80151CE8`). `group.c` re-uses that group's types and includes its
matching `func_800B98D8` and `minimap_render` as `context` (they are called
from here and have to be in the unit for calls to link and for the callee
register information):

| function | role |
|---|---|
| `time_of_day_select(node, pos, outNode, dist, depth)` | follow `next` links from `node`, summing point counts into `*dist` (tail recursive; IDO turns it into a loop) |
| `split_time_display(node, pos, remain, outNode, outPos)` | walk `remain` points back along `next` links (tail recursive) |
| `func_800D2FA8(node, pos, outNode, outPos, stopAtTyped, depth)` | search for the node/point reached from `(node,pos)`: checks the visited list `D_80124F88`, handles nodes whose `next == prev` (a loop) inline, otherwise measures both ways round with `time_of_day_select`/`minimap_render` and finishes with `split_time_display`; recursive on the `next == prev` path |

`func_800D2FA8` has an outside caller (`func_800D3430`) and the normal ABI
(five stack/register args, `depth` on the stack): it is in `keep`.
`func_800D3430` and its callers are not in the unit; nothing depends on them.

## Findings that mattered

- **Locals layout.** IDO gives every *referenced* named scalar local a stack
  slot and lays locals out in declaration order, first declared at the highest
  address. `func_800D2FA8`'s seven `int` locals are declared
  `dist1, total, dist2, start1, node2, pos2, start2` to land at
  `sp+96, 92, 88, 84, 80, 76, 72` as in the ROM.
- **Frame padding.** The ROM frame is 8 bytes larger than the code needs. An
  unreferenced `volatile s32 padv[2];` declared first reproduces it (plain
  unused locals are dropped by the optimiser, `volatile` ones are not).
- `split_time_display` must be recursive C (`else if (pos >= remain) {...}
  else recurse`) with **no** named local for the section start: a `start`
  local adds a stack slot (frame 16 instead of 8). IDO's tail-call
  conversion produces the two dead stores at `sp+4`/`sp+0`.
- `if (pos >= remain)` first, recursion in the `else`: matches the branch
  layout (`bnezl at,recurse`).
- Reusing the `node` parameter as the loop variable
  (`node = *outPos; while (node < ...)`) moves the value into `$a0` as in the
  ROM.

## Blockers

- `split_time_display`: the ROM keeps the section start in `$v1` and the
  section/graph bases in `$t0/$t2` (`$t1 = 80` for the `mul`); ours puts the
  start in `$t0` and the bases one register higher. Same "first temp" shift as
  in `func_8008B640`'s `t` (`$v1` vs `$v0`).
- `func_800D2FA8`: `pos`/`outPos` copies are in `$t4/$t5` in the ROM, `$t3/$t4`
  here (a one-register shift again); the unrolled visited-list loop also has
  one extra `move a1,a2` in the ROM.
