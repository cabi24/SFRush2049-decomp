# func_800B9B64 (track path graph)

**4/4 members MATCH with `as1 -r4300_mul`** (see [../../R4300_MUL.md](../../R4300_MUL.md));
`physics_friction_apply` is context (21/126, register naming only).

```
# with the -r4300_mul assembler flag (`cloud/work/tools/zbuild.py --as1=-r4300_mul`; score.py does not pass it yet):
func_800B98D8 MATCH; func_800B9B64 MATCH; minimap_render MATCH;
physics_velocity_clamp MATCH; physics_friction_apply (context) 21/126
# with today's tools/cloud/score.py: func_800B9B64 is 100/129 (one missing
# nop after a mul.s shifts every later word); the other three MATCH.
```

## What this is

The track path graph: a table of 16-byte nodes, each a run of points,
linked end to end. `group.c` is hand-written from the assembly (the m2c
seed is superseded), with real types:

| label | role |
|---|---|
| `func_800B98D8(node, depth)` | record `node` in the walk list `D_80143A88[depth]`; 0 if already visited or a dead end |
| `func_800B9B64(skip, atStart, outNode, pos, outIdx)` | nearest point to `pos`: global points first, then every node's points, skipping `skip` and the node already joined to it |
| `minimap_render(node, pos, outNode, outPos, dist, stopAtTyped, depth)` | follow `prev` links summing the distance walked (tail-recursive; IDO turns it into a loop) |
| `physics_velocity_clamp(node, pos, out, depth)` | section containing a point, following `next` links back (tail-recursive) |
| `physics_friction_apply()` (context) | link every node to its neighbours, then assign sections |

Globals: `D_801407F0` is one struct (`u16 numPoints; PathPoint *points; u8
numNodes; PathNode *nodes;`); IDO addresses it through one base register,
so separate externs do not match. `D_80151CE8` is an array of 0x50-byte
section records (count in element 0 at +8, start at +0x2E, per-node
start points at +0x38). `D_80123DF8` is the initial best distance.

`func_800D2FA8`, `split_time_display` and `time_of_day_select` (group
`func_800D2FA8`) are the same module; `func_800D2FA8` calls
`minimap_render` and `time_of_day_select`.

## Fixes that made it match

- `func_800B98D8` is in `keep`: it has a caller outside the group
  (`time_of_day_select`) and the target uses the standard ABI.
- `minimap_render`'s `pos` is `s32` (a `u16` parameter adds a mask and a
  home-slot store). The distance update is `*dist += numPoints - pos`.
- In `func_800B9B64`'s first loop the body is `best = d; *outIdx = i;`.
- `func_800B9B64` needs the game's `-r4300_mul` assembler flag.

## physics_friction_apply (context)

21/126: every remaining difference is `$t7`/`$t9` naming, starting at the
second `func_800B9B64` call's "last point" argument. Putting `skip` and
`atStart` first in `func_800B9B64`'s parameter list fixed the argument
order (IDO's IPA assigned the same registers for every order). Tried
several spellings of `&points[numPoints - 1]` and of the stores; no change.
A permuter run is the next step.

## Relocations (for review; all resolved by the strict scorer)

`D_801407F0` (+0x0/+0x4/+0x8/+0xC), `D_80143A88`, `D_80123DF8`,
`D_80151CE8`; calls between members only.
