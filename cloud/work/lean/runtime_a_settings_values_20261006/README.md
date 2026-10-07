# Image A projected settings values: lean research

Target: `A:func_803AECE4`, `[0x803AECE4,0x803AF744)`, 2,656 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result and reproduction

Standalone IDO 5.3 O3: **270/664 words differ**, no extra words,
unresolved symbols, unverified relocations or errors with authenticated
image-A data. The first complete reconstruction differed in 537/664 words
and its shifted switch layout failed owned jump-table comparison. Explicit
zero/nonzero selections and the native three-way text selection recover the
switch geometry; the full 21-entry jump table now verifies. Allocation,
local-offset and scheduling differences remain. This is NONMATCH research.

```
python3 cloud/work/lean/runtime_a_settings_values_20261006/reproduce.py
```

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The minimal helper authenticates frozen asset and image A in memory and
provides required own-data to the unchanged scorer. Six float references at
A:0x803B99E0..0x803B99F4 are the paired 40/140/90-degree thresholds. The switch
table at A:0x803B99F8 is checked by relocated entry content, not ignored.
No native bytes, assembly dump or object is included.

## Source and assumptions

An ordinary callback draws the settings title and an optional warning, then
walks the live selected-option list. Each 64-byte slot supplies its label
position/angle/alpha; the matching value uses slot+21. Visible labels and
values use the original projection, alpha, blend-color and text calls.
The value switch preserves cases 0..13, 17 and 20, including mode-dependent
course flags, signed numeric options, and language-table choices. The other
switch entries intentionally do no text work.

Packed word-aligned four-byte colors retain the native two-aggregate by-value
ABI, as in the separate 6B50 research. Shared-header reconciliation with the
earlier byte-only color helper view remains a checker task. External helpers
and data stay unchanged. Source names and partial record views are descriptive;
original declarations and whole-function arcade ancestry are unknown.

Valid inputs require a backed selected list, slot indices 0..20, accessible
slot+21 records, valid language/course tables and finite representable projected
coordinates. The candidate's 80-byte text buffer must hold each formatted
result including NUL; the original declaration is not established. Unknown
fields preserve actual record storage. No pressure locals, fake prototypes/
callers, artificial volatile or inline assembly. Only compile/scoring was run.
Observed scores are not accepted coverage; checker owns further validation,
acceptance and integration.
