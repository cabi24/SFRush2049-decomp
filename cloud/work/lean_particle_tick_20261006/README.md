# entity_physics_update: genuine particle-service caller context

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `entity_physics_update`, `[0x800908A0,0x80090B68)`,
712 bytes / 178 words. No matching claim.

The archived complete A167 body scores 176/178 words off. This packet scores
**66/178 differing words**, with zero nonzero excess words, unresolved symbols,
unverified relocations or errors. The native 112-byte frame and full saved
integer/FP register prologue are recovered. Broad body scheduling/register
residuals remain; no padding or saved-register pressure is fabricated.

## Actual caller/callee boundary

Native 90308 has two real callers: this callback and save_write_data's AF0E8
call site. Exporting 90308, as in the old A167 packet, hides its native private
saved-register clobbers from this callback. Making it internal without the
second caller instead inlines it and adds 297 nonzero excess words.

`service.c` supplies the complete existing AF06C caller and its 90308 body from
PR163 immutable head `1bd09c5eb3bde8f803c46d0d657e9264d4223a38`, path
`cloud/work/af06c_tagged_effect_service_20261006/candidate.c`. Only 90308's
file-local linkage is changed so both real C units share that same body;
group.json keeps the two actual outer roots. AF06C's function body is unchanged,
not refined or newly claimed. Neither AF06C nor 90308 is a match in this packet.
The allocator is the complete current accepted base source, context only.

## Target changes

The actual cleanup operation is shared by the zero-mode, inactive-model and
finished-frame paths, following the native backward branches. Attached versus
unattached timer cases are explicit, preserving the original comparisons,
including floating-point comparison behavior. The clock is read through a
consumed, lossless O32 pointer-to-word-to-pointer address expression; this
compile-affecting spelling recovers its full native address formation. It is
not a portable-host or original-source claim, and adds no extra read or volatile.

Position updates, callback-visible reloads, texture advancement, extra-effect
trigger and private particle spawn are all retained. No fake caller/prototype,
unused local, artificial array, asm or alternative optimizer flag is introduced.

```sh
python3 tools/cloud/score.py group cloud/work/lean_particle_tick_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. The entire declared real group is required. Native O32 layouts,
valid node/model/car/scene indices and the existing callback/service contracts
are assumed. Historical helper first-line recipes are provenance; this whole
packet was compiled O3. No target, scorer, accepted-lock or production-source
edits are included. Nonmatching context must not replace production owners.
Only the local comparison is claimed; independent checking, image integration
and accepted coverage remain separate.
