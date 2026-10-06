# LEAN RESEARCH: starting-emitter insertion

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: boot-tail `func_8001DA74`, 404 bytes / 101 words.

IDO 5.3 at `-g0 -O3 -mips2 -G 0 -non_shared` (plus canonical automatic
`-Wab,-r4300_mul`) improves strict differing words **59/101 to 10/101** with
no nonzero excess, unresolved symbols, unverified relocations or errors.
True ELF extents and source hashes are in observed.json. This remains a
research nonmatch, not verified behavior, accepted matching or ROM coverage.

## Real source organization

Pinned [MusyX snd3d.c AddStartingEmitter](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd3d.c)
provides the genuine long-valued index, direct group/node indexing, one list
cursor, and final distance-field count postincrement. These replace the prior
extra group/head/next/node pointer locals. The CC0-1.0 donor is a family lead,
not identified as the original N64 release.

The N64 function still has its real object pointer and five float arguments.
Native 32-group and 32-node capacity checks and existing-head anchor are retained.
A newly created group's side effect survives a subsequent full-node-pool failure.
No newer donor numRunning/lpf fields or larger capacities are imported.
The node's five float members and real object pointer retain their native order.
No helper definition, fabricated argument, stack padding, forced register,
volatile or assembly is introduced.

Assumptions are valid allocated acyclic lists, pool counts in 0..32, N64 O32
pointer width and ordinary engine FP state. Nonempty lists insert after a visited
anchor, never before their original head. No arbitrary malformed-list or special
FCSR exception-state guarantee is added.

## Minimal replay

Set `IDO_DIR` to the pinned compiler and run:

    python3 replay.py --repo /path/to/SFRush2049-decomp

The script reads only named immutable Git inputs from the frozen base and
compiles baseline/candidate at identical flags. Canonical strict score and ELF
function extent are reported. No independent review, behavior harness, full
suite, CI wait or ROM gate was run. Checker/Claude owns verification, acceptance
and ROM integration. No lock or production source is changed.
