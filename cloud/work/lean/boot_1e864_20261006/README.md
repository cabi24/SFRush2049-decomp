# LEAN RESEARCH: MusyX nested binary-search expression

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: boot-tail `func_8001E864`, 204 bytes / 51 words.

IDO 5.3 at `-g0 -O3 -mips2 -G 0 -non_shared` (plus the canonical wrapper's
`-Wab,-r4300_mul`) improves differing words **34/51 to 9/51**, with no nonzero
excess, unresolved symbols, unverified relocations or errors. This is an
observed research improvement, not a match, verified behavior or ROM credit.

## Source and assumptions

Pinned [MusyX snd_service.c sndBSearch](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_service.c)
provides the nested midpoint/element/comparator assignment and long-valued
low/high/middle/result locals, using an integer-address base calculation.
This CC0-1.0 source is an authentic family lead, not proof of N64 version identity.
The earlier reconstruction already described the same one-based binary search;
the new expression organization improves its generated code.

The real five-parameter callback signature is retained: two pointers, signed
count and byte stride, and a comparator returning int for two const pointers.
No helper definition, fake context, padding, volatile, assembly or forced
register is supplied. Zero count returns null; other counts enter the existing
inclusive one-based midpoint search and preserve callback order.

`unsigned long` is used for the donor's address-sized unsigned type under N64
O32, where it and pointers are 32 bits. This is target-specific integer-address
arithmetic, not a portable C89 pointer guarantee. The valid search domain still
requires an allocated sorted array, positive stride/count when nonempty,
representable midpoint/offset arithmetic and a compatible comparator. No new
bounds check or behavior is invented.

## Minimal replay

Set `IDO_DIR` to the pinned IDO compiler and run:

    python3 replay.py --repo /path/to/SFRush2049-decomp

The script reads only named frozen Git inputs, compiles the baseline and
candidate at identical flags, and reports the canonical strict score and ELF
extent. Source hashes and observed results are in observed.json.
No independent review, behavior harness, full suite, CI wait or ROM gate was
run. Checker/Claude owns verification, acceptance and ROM integration.
