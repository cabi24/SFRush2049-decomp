# Image B HUD event consumer: lean research

Target: `B:func_803936A8`, `[0x803936A8,0x80393B34)`, 1,164 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result

Standalone IDO 5.3 O3: **2 / 291 words differ**, zero extra words,
unresolved symbols, unverified relocations or comparison errors. Both remaining
differences are the local text-buffer address: candidate frame+96 versus native
frame+112. Both complete functions have the native 144-byte frame. This is
research, not an accepted match or ROM-coverage gain.

The first reconstruction differed in 210/291 words plus two extra words.
Direct event-field expressions instead of temporary age/delta locals reduced
that to 25/291 plus one extra word. A four-iteration indexed event loop and the
native equality operand order leave only the two buffer-address differences.
No padding, unused variables or other frame-pressure devices were added.

## Reproduce

```
python3 tools/cloud/score.py fn cloud/work/lean/runtime_b_event_consumer_20261006/candidate.c func_803936A8 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

The scorer supplies `-Wab,-r4300_mul`. The result is intentionally NONMATCH.
No additional translation-unit context or owned-data artifact is needed.

## Source and assumptions

Draws each player's current counter with a shadow, then processes four 24-byte
HUD events. Events older than 40 frames apply their delta and deactivate;
younger events interpolate their coordinates, display same-player or
other-player text, and advance age unless either pause flag is set. The
already-submitted B:func_803914B4 producer independently establishes the event
layout and coordinate tables. Helper functions remain external.

Record sizes/offsets and signed byte/halfword accesses are reconstructed from
native code. Original type and variable names, whole-function arcade ancestry,
and the original local-buffer declaration are unknown. The candidate uses a
32-byte text array; valid formatted text must fit including its terminator.
Player indices must have backed 952-byte player and 76-byte information records;
coordinate-table selector is 1..4, and counts/columns must remain in bounds.
Language and player-name pointers must be valid. Unknown fields preserve real
record storage. No invented helpers, callers, artificial volatile or assembly.
Only compile/scoring was performed. Independent checker owns acceptance,
further validation and integration.
