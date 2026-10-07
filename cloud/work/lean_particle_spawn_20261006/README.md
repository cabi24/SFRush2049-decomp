# func_80090308: native Random and color-object research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `func_80090308`, `[0x80090308,0x80090770)`,
1,128 bytes / 282 words. No matching claim.

The genuine two-caller body from #291 scores 279/282 words off. Restoring the
actual accepted Random/rand body and the consumed native four-byte color
object improves this to **263/282 differing words**, with zero nonzero excess
words, unresolved symbols, unverified relocations or errors. The residual is
still broad, including frame/stack placement and loop scheduling.

## Source lead

`random.c` is the complete accepted base `src/blob/func_8008B2E4.c`, compiled
inside this O3 group. Its ancestry is arcade LIB/fmath.c Random(max), with the
native 32768 divisor and actual seed recurrence. The four real draws, their
order, limits, float arithmetic and byte lifetime conversion are preserved.
No new random call or read is introduced. This is the native compiler's
32-bit signed-seed/wrapping arithmetic model, not portable-host C evidence.

The color has the actual four-byte union/byte view seen in the earlier A167
source and native accesses: capture D_8011B550 before allocation, set the three
RGB bytes for the debris, set alpha in the loop, then reuse it for the optional
white extra effect. All assignments feed actual scene color stores. There is
no unused array, artificial volatile or stack padding. Flattening this object
to scalar literals had removed its source-level lifetime.

The target still performs actual allocation, callback/state setup, four debris
updates, conditional extra effect, list insertion and conditional secondary
spawn. Pointer-loop and alternate handle-view controls did not improve the
submitted result and are omitted.

## Required real callers and reproduction

`effects.c` supplies the complete #291 entity_physics_update callback.
`service.c` retains the complete existing AF06C caller from PR163 immutable
head `1bd09c5eb3bde8f803c46d0d657e9264d4223a38`; its function body is unchanged.
As in #291, 90308 is shared by these two actual callers rather than held out of
line by a synthetic keeper. The current accepted allocator is context only.
All roots/files are declared in group.json. Caller/helper bodies are not new
matching credit or wholesale production replacements.

```sh
python3 tools/cloud/score.py group cloud/work/lean_particle_spawn_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. Native O32/big-endian layouts, valid scene/player/effect indices,
the actual four-debris capacity and the native RNG domain are assumed. No
target, scorer, accepted-lock or production-source edits are included. Only
the local comparison is claimed; independent checking, image integration and
accepted coverage remain separate.
