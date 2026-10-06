# audio_timing_sync: unsigned-byte slot research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `audio_timing_sync`, `[0x80095924,0x800959DC)`,
184 bytes / 46 words. No matching claim.

The genuine A158 group carried with the #191 fade refinement scores 8/46
words off. Explicitly extracting the low handle byte into a consumed `u8 slot`
before the 24-byte offset multiplication improves this to **7/46 words off**.
There are zero nonzero excess words, unresolved symbols, unverified relocations
or errors. This is a small, locally observed source refinement.

The offset spill now uses the native stack location. The remaining residual is
v0/v1 selection, the node spill location and final pointer-add operand order.
The handle validation, post-callback table reload, state/handle stores and real
fade call are unchanged. The byte local has an actual dataflow use, not an
unused read, filler or artificial volatile qualifier.

`group.json` supplies all real A158 callers and the complete current list bodies.
The #191 fade-control body is context only. Existing exact helpers and other
nonmatching A158 setters are not new matching credit; this packet must not
replace their production owners wholesale. No synthetic caller is present.

```sh
python3 tools/cloud/score.py group cloud/work/lean_audio_timing_sync_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. Native O32 node/24-byte entry layouts and the existing list/handle
contracts are assumed. No target, scorer, accepted-lock or production-source
changes are included. Independent checking and integrated coverage remain
separate from this local comparison.
