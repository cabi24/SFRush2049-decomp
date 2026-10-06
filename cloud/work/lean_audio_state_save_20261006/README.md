# audio_state_save: unsigned handle and allocation-output research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `audio_state_save`, `[0x80095D04,0x80095EB8)`,
436 bytes / 109 words. No matching claim.

The flattened body carried in #204/#248 scores 96/109 words off. A complete
allocation helper with an actually consumed output pointer, together with the
native unsigned voice field, improves this to **16/109 differing words**.
There are zero nonzero excess words, unresolved symbols, unverified relocations
or errors. The observed frame is 56 bytes versus native 80; the output pointer
is at 52 versus native 76. That storage gap is left unresolved.

## Source lead and change

Native code has a success result distinct from an entry pointer stored to an
address-taken local. `allocate_entry` expresses that exact allocation operation:
clear the output, scan the real 24-byte entries, initialize the native bytes and
voice sentinel, assign the generation/index handle, update the generation and
return the chosen entry through the output pointer. The name and original
inline boundary are hypotheses; every operation and parameter is consumed by
the real caller. No unused storage or synthetic caller is added.

The +0x10 voice field is unsigned, consistent with the real `func_800201D0`
contract and the recovered camera_transform command processor. Giving that
field its native unsigned type prevents inappropriate commoning of the signed
resource guard's -1 with the voice sentinel. This closes most of the residual
without adding an operation. The native generation update and all global-table
reloads after field stores are preserved.

The caller still rejects resource -1, obtains and initializes the actual free
command, submits it through the real fade helper, and returns the allocated
handle. No artificial volatile, padding, fake ABI, asm or pressure local is used.

`group.json` declares the actual A158 callers and complete list bodies. The #191
fade refinement and all other helpers are unclaimed context. Existing exact
helpers do not add credit; nonmatching context must not replace production
owners wholesale.

```sh
python3 tools/cloud/score.py group cloud/work/lean_audio_state_save_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and the canonical
assembler's `-r4300_mul`. Native O32 layouts, valid resource-table bounds and
available command/list entries are assumed. No target, scorer, accepted-lock
or production-source edits are included. Local canonical research evidence
only; independent checking, image integration and accepted coverage remain
separate.
