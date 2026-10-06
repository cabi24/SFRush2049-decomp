# func_80096CA8: bounded private flag and serialized record research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `func_80096CA8`, `[0x80096CA8,0x8009715C)`,
1,204 bytes / 301 words. No matching claim.

The repaired real-parser context submitted in #270 scores 295/301 words off.
This bounded refinement scores **287/301 differing words**, with zero nonzero
excess words, unresolved symbols, unverified relocations or errors. The match
is still poor: register allocation, private argument transport, scheduling and
frame layout remain broadly different. This is a modest source-model result.

## Source evidence and explicit hypothesis

The private first-load flag is represented as s8. Its complete actual caller
set in this group passes only 0 (fp_call_wrapper) or 1 (func_80097164). Thus the
narrowing preserves that real call graph; the original source width remains a
hypothesis, not independently established. No argument is added or ignored,
and no external/ordinary-ABI caller is asserted. A real local-copy control and
constructor-visibility controls did not improve the bounded result and are
not included.

Serialized model/texture/palette relocation fields are u32 words until their
rebased values are interpreted as addresses. This avoids expressing arithmetic
on invalid pre-relocation C pointers. The field-type repair itself does not
change the observed machine words. Native O32 wrapping-address semantics are
assumed.

The complete parser retains header-table setup, object/texture/palette lookup,
image relocation, four-part model display-list updates, optional extra texture
records, loaded-state marking and actual render-mode refresh. It preserves the
#270 real 12-byte table, four native model parts and absence of the old unused
prefix storage/uninitialized prefix read.

`group.json` declares the complete actual loader/thread/reload/wrapper context.
The real 24-byte DMA message and corrected cache/decompress APIs from #270 are
unchanged and unclaimed here. The unrelated padded display-list body and all
synthetic keepers remain absent. No artificial volatile, pressure storage,
unused operation, asm or new optimizer flag is introduced.

```sh
python3 tools/cloud/score.py group cloud/work/lean_resource_parser_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. Assumptions: native O32 layouts, valid resource/table indices,
at most four model parts, valid queues/slots and the actual 0/1 private flag
callers. Nonmatching context must not replace production owners wholesale.
No target, scorer, accepted-lock or production-source edits are included.
Only the local comparison is claimed; independent checking, image integration
and accepted coverage remain separate.
