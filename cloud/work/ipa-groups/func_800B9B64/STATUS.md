# func_800B9B64 (track path graph)

**4 of 4 members MATCH** (361 words), rescored 2026-09-30 with
`zbuild.py --as1=-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)).
**Spliced** (`src/blob/groups/func_800B9B64`).

| function | target words | result |
|---|---|---|
| `func_800B98D8` (`keep`) | 77 | **MATCH** |
| `func_800B9B64` | 129 | **MATCH** (needs `-r4300_mul`: without it, 100/129, one missing `nop` after `mul.s`) |
| `minimap_render` (`keep`) | 69 | **MATCH** (zbuild prints 71/69: pad nops) |
| `physics_velocity_clamp` | 86 | **MATCH** |
| `physics_friction_apply` | 126, context | 21 differ (register naming only) |

## What this is

The track path graph: 16-byte nodes, each a run of points, linked end to end.
`group.c` is hand-written from the assembly with real types.

| label | role |
|---|---|
| `func_800B98D8(node, depth)` | record `node` in walk list `D_80143A88[depth]`; 0 if already visited or dead end |
| `func_800B9B64(skip, atStart, outNode, pos, outIdx)` | nearest point to `pos`: global points first, then every node's points, skipping `skip` and the node joined to it |
| `minimap_render(node, pos, outNode, outPos, dist, stopAtTyped, depth)` | follow `prev` links summing distance (tail-recursive; IDO makes a loop) |
| `physics_velocity_clamp(node, pos, out, depth)` | section containing a point, following `next` links back (tail-recursive) |
| `physics_friction_apply()` (context) | link every node to its neighbours, then assign sections |

`D_801407F0` is one struct (`u16 numPoints; PathPoint *points; u8 numNodes;
PathNode *nodes;`): IDO addresses it through one base register, separate
externs do not match. `D_80151CE8` is an array of 0x50-byte section records
(count at element 0 +8, start +0x2E, per-node start points +0x38). `D_80123DF8`
is the initial best distance. Group `func_800D2FA8` is the same module.

## Fixes that made it match

- `func_800B98D8` is in `keep` (outside caller `time_of_day_select`, standard ABI).
- `minimap_render`'s `pos` is `s32` (`u16` adds a mask and home-slot store); the
  distance update is `*dist += numPoints - pos`.
- First loop of `func_800B9B64`: body is `best = d; *outIdx = i;`.
- Game's `-r4300_mul` assembler flag.

## physics_friction_apply (context, not a member)

Every remaining difference is `$t7`/`$t9` naming starting at the second
`func_800B9B64` call's "last point" argument. `skip`/`atStart` first in the
parameter list fixed argument order (IPA gives the same registers for any
order); several spellings of `&points[numPoints - 1]` and of the stores did not
help. Permuter is the next step.
