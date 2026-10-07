# LEAN RESEARCH: donor index lifetimes for 8001671C

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: boot-tail `func_8001671C`, 636 bytes / 159 words.

IDO 5.3 with requested flags `-g0 -O3 -mips2 -G 0 -non_shared`
plus the canonical wrapper's `-Wab,-r4300_mul` improves positional differing
words from **133/159 to 40/159**. Nonzero excess worsens **1 to 3**; this
tradeoff is explicit. All relocations resolve in both runs, with no unverified
references or errors. This is a research nonmatch, not accepted matching or ROM
coverage. The complete emitted extent and source hashes are in observed.json.

## Source evidence and adaptation

The pinned [MusyX synthdata.c dataInsertMacro](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthdata.c)
uses genuine main/position/base/index values, a chained assignment of the empty
bucket's first index, and a reused index for range adjustment and record
compaction. The candidate adopts that organization instead of separate range,
scan and entry pointers. The donor is CC0-1.0 and is a family lead, not the
identified N64 release or an interchangeable modern implementation.

Retained N64 details include packed table accesses, signed total, 512 buckets,
2048 records, payload-before-ID store order, actual no-argument synchronization
calls, duplicate-reference increment before release and empty-bucket metadata
write before the capacity guard. Donor-only PC argument rejection, assertions,
alternative layouts and changed capacities are omitted.

The prior reconstruction's assumptions still apply: key below 32768, a valid
0..2048 total, sorted and consistent contiguous bucket slices, and allocated
record/range tables. Stored payload pointers are never dereferenced here.
No fabricated formal, helper body, volatile, dummy local, stack padding or
assembly is introduced. The natural donor C organization remains nonmatching.

## Minimal replay

Set `IDO_DIR` to the pinned compiler, then run:

    python3 replay.py --repo /path/to/SFRush2049-decomp

Only named immutable inputs are read from the frozen Git base. Both sources
compile at the same flags and report the canonical strict score and ELF extent.
No independent review, behavior harness, full suite, CI or ROM gate was run.
Checker/Claude retains verification, acceptance and ROM integration ownership.
