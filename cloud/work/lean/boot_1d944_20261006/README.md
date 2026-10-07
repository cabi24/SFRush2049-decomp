# LEAN MATCH-CANDIDATE: running-emitter insertion

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: boot-tail `func_8001D944`, 304 bytes / 76 words.

IDO 5.3 at `-g0 -O3 -mips2 -G 0 -non_shared` (plus canonical automatic
`-Wab,-r4300_mul`) improves strict differing words **53/76 to 0/76** with no
nonzero excess, unresolved symbols, unverified relocations or errors.
The candidate's true ELF function extent is recorded in observed.json.
This is a full-score match candidate for the checker, not accepted ROM coverage.

## Authentic source context and N64 adaptation

Pinned [MusyX snd3d.c AddRunningEmitter](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd3d.c)
provides the direct group indexing, genuine long-valued group index, for-loop
list cursor, and final-field count postincrement. Applying that natural source
organization closes the retained reconstructed body's score. The donor is
CC0-1.0 and a source-family lead, not proof of original N64 release identity.

The actual N64 object/group/node layouts and capacities are retained. No newer
numRunning field/counter is imported. The genuine signature remains one object
pointer and one float. No callback, fabricated parameter, helper body, volatile,
stack padding, forced register or assembly is introduced. No data is defined.

As in the native body, capacity is not checked here: valid group count is 0..32,
new groups require room, and every insertion needs a small-node index below 32.
Lists must be valid allocated acyclic lists. Equal or unordered distances stay
after existing entries because the break comparison is strictly ordered.
The source is for N64 O32 pointer sizes and the ordinary engine FP state.

## Minimal replay

With the pinned compiler selected by `IDO_DIR`:

    python3 replay.py --repo /path/to/SFRush2049-decomp

Only named immutable source/tool/target inputs are read from the frozen base.
Both sources are compiled at identical flags; canonical score and true ELF
extent are reported. No independent review, behavior harness, full suite, CI
wait or ROM gate was run before publication. Checker/Claude owns verification,
acceptance and ROM integration. No matching lock or production source is changed.
