# Runtime A scene initializer: code match, data boundary open

`func_8039BE48`, `[0x8039BE48,0x8039C140)`, 760 bytes / 190 words, now reports
**local MATCH, 190/190**, at normal whole-program O3 plus `r4300_mul`.
The observed comparison has zero differing, unresolved, unverified or extra
words. This is explicitly code/relocation evidence, not owned-data or acceptance
proof: the native float storage at D_803B95B4 remains an external anchor whose
value and original ownership are unresolved. Accordingly group.json has empty
claims; this is a lean research/matching candidate for the independent checker.

## What changed

This continues [PR #199](https://github.com/cabi24/SFRush2049-decomp/pull/199)
with the actual C140 parent and the genuine main-blob wrapper kept external.
Removing cross-image wrapper inlining had reduced BE48 to 34/190 differences.
Two genuine locals, a separate signed selector snapshot and a reused resource
lookup result, recover the native register assignment and exact stack layout.
Placing the selector snapshot before the initialization-flag store closes the
two remaining scheduling words. Both locals are used; no unused frame filler,
dummy reads, inlining barrier, fabricated callers or private flags are added.

The complete initializer synchronizes current players, normalizes configuration
state, conditionally creates two resources and binds texture/transform data,
refreshes configuration flags, and creates the screen's UI objects. Source
views encode native offsets and strides rather than claiming original types.
The source snapshots the selector before helpers can mutate globals. The actual
main-blob allocator wrapper has an explicitly returning source sidecar, rather
than a false returning declaration backed by a void implementation.

B120, B214 and A448 retain their existing local matching scores, with no new byte
credit here. C140 remains unclaimed at 484/496 differences. This context extends
#199 and #190; do not independently install all three groups or count shared
bodies multiple times. Existing PRs stay available for the checker's decision.

## Reproduce and limits

From repository root with documented IDO:

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_scene_init_group_20261006 --targets asm/us/ovl_a
```

This prints the local BE48 match and all other body scores; the nonmatching
parent means the all-members command exits nonzero. --claims intentionally
reports research with no acceptance claims. external_sources identifies the
separately compiled main-blob wrapper sidecar; it is not runtime compiler input.

The D_803B95B4 data value/ownership must be resolved before treating this as a
complete source/data match. The parent's own unknown float anchor and private
ABI gaps also remain unclaimed. No semantic test packet, full tests,
image/compression/ROM gates, accepted coverage, or integration safety is claimed.
Only source, compiler/context information and local scoring are supplied.
Fixed base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
