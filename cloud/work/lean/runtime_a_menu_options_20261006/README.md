# Image A projected menu options: lean research

Target: `A:func_803A6B50`, `[0x803A6B50,0x803A6F14)`, 964 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result and reproduction

Standalone IDO 5.3 O3: **59/241 words differ**, no extra words,
unresolved symbols, unverified relocations or errors with authenticated
image-A data. The initial byte-aligned color view differed in 124/241 words
plus fourteen extra. A word-aligned four-byte color aggregate reproduces the
native word copies for the two by-value color arguments and removes the extra
unaligned load/store sequences. Local offsets/allocation/scheduling still differ.

```
python3 cloud/work/lean/runtime_a_menu_options_20261006/reproduce.py
```

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The helper authenticates frozen asset and complete image A in memory and
supplies own-data to the unchanged canonical scorer. Five angle literals at
A:0x803B9820..0x803B9830 correspond to 40/140/20/160/90 degrees in radians;
source decimals reproduce their exact binary32 encodings. No raw data included.

## Source and assumptions

An ordinary one-pointer callback draws a heading and seven eligible options,
optionally projecting positions from 64-byte menu slots and changing alpha/
colors according to each slot angle. It preserves the two native player-count
formatting calls even though this body does not later draw that local text.
A:80399394 stays an external eligibility boundary with its observed integer
input/output; no low-image source is modified.

BEA3C has two genuine four-byte aggregate inputs, established by its existing
color-setter source and these native call sites. The candidate models each
aggregate as one packed word, preserving all color bits and the native
four-byte alignment. The earlier byte-only Color4 view is not asserted to be
the recovered original declaration. Shared-header reconciliation with that
callee remains a checker task; no helper implementation was changed here.

Native field views, external language strings and helper contracts are retained.
Original names, complete menu-slot semantics and local buffer declaration are
unknown; the candidate's 50-byte text buffer must hold each formatted output
including NUL. Language pointers/tables, seven slots and projected outputs must
be valid. Unknown fields represent actual record storage. No padding/pressure
locals, invented callers, extra arguments, false return values, volatile or
assembly were introduced.

Only compile/scoring was performed. This is NONMATCH research, not accepted
coverage. Independent checker owns further validation, acceptance and integration.
