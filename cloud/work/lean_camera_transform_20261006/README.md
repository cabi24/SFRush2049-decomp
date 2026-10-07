# camera_transform: complete effect-command queue research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `camera_transform`, `[0x80097CA0,0x80098554)`,
2,228 bytes / 557 words. No matching claim.

The old `src/blob/groups/MP_TargetSteerPos` camera scaffold scores 549/557
words off, but contains `M2C_ERROR` FCSR placeholders, incorrect call arguments
and a synthetic helper keeper. It is not a valid semantic baseline. This
complete C reconstruction scores **533/557 differing words**, with zero nonzero
excess words, unresolved symbols, unverified relocations or errors. The residual
is broad. The frame is 80 bytes versus native 88.

## Source and contracts

The body rotates the native five-word completion history, retires selected or
finished entries, processes the real command list, retries temporarily blocked
commands and clears the per-entry blocked markers after the pass. Typed views
retain 24-byte entries, 32-byte commands and the actual list offsets. All
post-callback table/entry reloads follow the native control flow.

The old FCSR decompiler placeholders are replaced with ordinary unsigned-float
conversions, byte/halfword narrowing and the native rate lower bound. The helper
API arities and widths come from the real `cloud/matches/boot_tail` bodies:
FE58 takes one u32; FEA4/FFF4/FF4C take u32/u8; FFA0 takes u32/u16;
20174 takes u16/u8/u8 and returns s32; 201D0 returns u32; 20518 takes u8 and
returns void. In particular, no old fabricated third argument survives.

The actual `MP_TargetSteerPos` algorithm is included. The five small static
stop/parameter helpers express native inline operation blocks; their names and
original source boundaries are reconstruction hypotheses. There is no new
runtime algorithm, pressure storage, artificial volatile, asm or fake caller.

## Explicit helper-visibility limitation

With the sole real caller and MP_TargetSteerPos internal, IDO inlines it, yielding
537/557 differences and 56 nonzero excess words. The submitted group exports
both real bodies to preserve the call, producing the 533-word residual above.
This gives MP_TargetSteerPos the ordinary ABI rather than its native private
register contract. Its own body is nonmatching in this packet and carries no
new credit. The archived synthetic keeper is deliberately absent. Recovering
the genuine visibility/source context is still open; this packet cannot replace
the accepted MP_TargetSteerPos owner or be treated as a spliceable match.

The real outer caller was reconstructed in #248, but its body is unnecessary for
this explicitly exported target recipe and is not duplicated here.

```sh
python3 tools/cloud/score.py group cloud/work/lean_camera_transform_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. The declared group is the observed recipe. Native O32 layouts,
valid lists/table bounds and float values within the unsigned-conversion domain
are assumed; no extra guards alter native behavior. No target, scorer, accepted
lock or production-source edits are included. Independent checking, image
integration and accepted coverage remain separate from this local research.
