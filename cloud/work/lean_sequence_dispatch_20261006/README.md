# func_800988D8: typed sequence and real clock-predicate research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `func_800988D8`, `[0x800988D8,0x80098A54)`,
380 bytes / 95 words. No matching claim.

Observed local canonical comparison: the real B132 baseline carried in #187
has 80/95 differing words and one nonzero excess word. This packet has
**20/95 differing words, zero nonzero excess words, unresolved symbols,
unverified relocations or errors**. Remaining differences are two float-load
positions, one comparison operand order, and counter/register scheduling.

## Source lead and changes

The accepted `src/blob/func_80095198.c` is the actual clock-wrap predicate:
add duration to the start time, subtract 14400 when the clock has wrapped, then
compare with the current clock. Its complete body is included and inlined by
IDO at O3. This recovers the target's conditional cleanup block and float
operations instead of manually arranging a decompiler's temporary/goto chain.
Its old standalone O2 provenance does not change this packet's O3 flags.

The sequence now has native typed fields for its link, state bytes, clock pair,
byte cursor, four effect ids, three launch parameters and current handle.
A promoted integer cursor and a consumed unsigned-byte increment retain the
native truncation. Cleanup and advance use the real list/owner helpers. Lookup
returns the accepted effect pointer type rather than a raw integer address.

The indexed table view deliberately spans five words, overlapping the first
launch parameter through a union. Native code reads the cursor-indexed word
before checking cursor >= 4, including the word at +0x2C when cursor is 4.
This is an object-layout view of that actual read, not stack padding or pressure
storage. Valid entry cursor range 0..4 and native O32 layouts are assumed.
The four ids are the playable entries; the fifth word is not an extra effect.

## Required real context and reproduction

`group.json` declares the complete B132 caller, list helpers, cleanup, flag helper
and clock predicate. The target is internal to its genuine `func_80098FB8`
caller. The already submitted #187 cleanup remains context only. Existing exact
helpers do not add matching credit; the caller and inlined/deleted helper bodies
must not replace their production owners wholesale. No synthetic caller,
prototype-only private convention, asm, artificial volatile or unused local is
introduced.

```sh
python3 tools/cloud/score.py group cloud/work/lean_sequence_dispatch_20261006
```

Use IDO 5.3 with `-g0 -O3 -mips2 -G 0 -non_shared` and the canonical assembler's
mandatory `-r4300_mul`. The complete group is required for the observed result.
No target, scorer, accepted-lock or production-source changes are made. The
local score is research evidence only; independent checking, image integration
and accepted coverage remain separate.
