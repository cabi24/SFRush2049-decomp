# LEAN RESEARCH: donor index lifetimes for 80016998

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: boot-tail `func_80016998`, 608 bytes / 152 words.

With IDO 5.3 and requested flags `-g0 -O3 -mips2 -G 0 -non_shared`
(the canonical compiler wrapper also adds `-Wab,-r4300_mul`), the retained
baseline scores 106/152 differing words plus 4 nonzero excess words. This
candidate scores **76/152 differing words plus 3 nonzero excess words**.
Both runs have no unresolved symbols, unverified relocations or errors.
This is an observed improvement, not a match, verified behavior or ROM credit.

## Source evidence and adaptation

[AxioDL MusyX synthdata.c, dataRemoveMacro](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthdata.c)
uses real main/base/index locals, reuses the search index for the record
compaction loop, and updates the main table through that index. The candidate
adopts that organization instead of retaining range/entry pointers and a
separate compaction index. The donor is CC0-1.0; it is a later MusyX source
family, not established as the original N64 release.

N64 storage, capacity, packed accesses, signed counts, callback signatures and
return behavior are retained from the base reconstruction. No donor PC-only
argument rejection, new-version assertions or alternate data layout is added.
The genuine no-argument synchronization calls remain external.

Assumptions remain the prior reconstruction's valid consistent bucket slices,
0..2048 active records and key below 32768, so the bucket index is below 512.
Payload pointers are stored/copied but never dereferenced. No new argument,
artificial local/volatile, padding, inline assembly or helper replacement is
used. The complete emitted body still differs and contains excess instructions.

## Minimal replay

With the pinned IDO available through `IDO_DIR`:

    python3 replay.py --repo /path/to/SFRush2049-decomp

The replay reads only named immutable inputs from the frozen Git base, compiles
the old and new sources at the same flags, and reports the strict canonical
score and true ELF function size. No protected target or tool is changed.
Independent checking, acceptance and ROM integration remain with the checker.
No independent review, behavior harness, full suite, CI or ROM gate was run for
this lean research submission.
