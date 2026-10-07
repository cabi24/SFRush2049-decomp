# func_80098FB8: real thread caller and typed effect traversal

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `func_80098FB8`, `[0x80098FB8,0x800992AC)`,
756 bytes / 189 words. No matching claim.

The exported B132 body observed in #187 differs in all 189 words and has 25
nonzero excess words. Supplying the complete real `entity_ai_pathfind` thread
from #204 and making this callback internal produces 166/189 differing words
with exact extent. Replacing the raw effect-offset view with the actual field
layout and expressing both traversals with shared loop advancement reduces
this to **42/189 differing words**, zero nonzero excess words, unresolved
symbols, unverified relocations or errors.

The native frame is still 184 bytes; the candidate frame is 48. The address-taken
counter is correspondingly at a different offset. Other residuals are primarily
temporary-register scheduling in the second traversal and message loops. No
unused array, local, artificial volatile or padding is added to fill the gap.

## Source and required context

The typed effect retains the native 68-byte field layout: linked-list header,
list pointer, handle, kind/mode, state/delay/persistent/priority bytes, score,
five float fields, resource/group/voice words and owner pointer. The successor
is captured before callbacks, and each loop's common advancement uses that
snapshot. The second traversal retains the real time-field reload after its
store; a value-only local replacement changed code generation substantially
and is not submitted.

`group.json` declares all seven C sources and the real thread root. They carry
the #204 dispatcher and its actual setter/allocation/music helpers, the #187
cleanup and the #240 sequence/clock-predicate refinement. No synthetic caller
or prototype-only private calling convention is present. The supplied thread
and other nonmatching context remain research; their earlier scores are not
new claims and this group must not replace production owners wholesale.
Existing accepted tree-insertion, tree-walk and score bodies were tried as real
context without changing the target score; they are left external in this
minimal packet. Their interfaces remain the existing native contracts.

```sh
python3 tools/cloud/score.py group cloud/work/lean_effect_tick_20261006
```

Compile the complete group using IDO 5.3, exact
`-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler `-r4300_mul`.
Native O32 layouts, valid effect/tree/list pointers and the existing queue
contracts are assumed. The sequence context retains #240's cursor range 0..4
and five-word overlapping read-window assumption. The thread's existing
`func_80096238` caller/body argument ambiguity remains unaltered context.

No target, scorer, accepted lock or production-source edits are included.
Only the local canonical comparison above is claimed. Independent checking,
image integration and accepted cartridge coverage remain separate.
