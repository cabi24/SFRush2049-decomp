# func_80097164: genuine resource-loader context and DMA message

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `func_80097164`, `[0x80097164,0x800972BC)`,
344 bytes / 86 words. No matching claim.

Observed local canonical score improves from the archived resource-group
scaffold's 49/86 differing words to **17/86**, with zero nonzero excess words,
unresolved symbols, unverified relocations or errors. Native frame is 112 bytes;
candidate frame is 96. Native message address is sp+56; candidate is sp+72.
The parser-private argument registers and some load scheduling also differ.

## Concrete source repairs

The target now owns the real 24-byte `OSIoMesg` object rather than the scaffold's
four-byte `M2C_UNK` placeholder. Its fields follow the repository's native
message declaration and promoted `src/rom/lib_9230.c` DMA wrapper. The actual
wrapper has seven arguments, including the message, destination, byte count
and completion queue. No guessed storage is added for the remaining frame gap.

`osInvalICache_full` takes the destination and size. `lzss_decompress` takes two
arguments; `inflate_decompress` takes three, as in `src/rom/lib_3140.c`.
The old invented cache/decompress arguments are removed. Typed resource fields
retain the native 20-byte slot, format byte, indirect/direct data selection,
ROM/size tables, three load paths, completion wait and final parser call.

## Required real context

The actual synchronous loader, loader thread, reload routine and parser wrapper
are complete in `callers.c`. `parse.c` provides the actual func_80096CA8 body
needed for its private calling convention. The two synthetic archived keepers
are absent. The unrelated entity_lod_select body with an unused padding array
is not carried; its genuine five-argument external contract is retained.

Parser context repairs follow the actual constructors: a three-word Table
replaces the old padded ParseCtx. The first-load branch no longer reads an
uninitialized prefix word as a relocation base; that base is only used on the
relocation path where it is assigned. The 88-byte model view has four 16-byte
parts after its 24-byte header, matching the current entity_render_mode layout.
It no longer declares three parts and relies on indexing into padding for the
fourth. These parser changes are unclaimed context, not a parser match.

No artificial volatile, unused stack storage, fake caller, private prototype,
asm or altered optimizer flag is introduced. Existing context scores are not
matching credit and must not replace production owners wholesale.

```sh
python3 tools/cloud/score.py group cloud/work/lean_resource_loader_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. `group.json` declares every C unit and actual root. Assumptions:
native O32 layouts, valid resource/table indices, at most four model parts,
valid queues and available resource slots on allocating paths. No target,
scorer, accepted-lock or production-source edits are included. Local research
only; independent checking, image integration and accepted coverage remain
separate.
