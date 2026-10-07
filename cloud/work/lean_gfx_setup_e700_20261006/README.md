# gfx_setup_e700: real command-pop helper research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `gfx_setup_e700`, `[0x800954A8,0x80095528)`,
128 bytes / 32 words. No matching claim.

Observed canonical score improves from the #191 A158 body's 14/32 words off
to **9/32 words off**, with zero nonzero excess words, unresolved symbols,
unverified relocations or errors. The candidate now has the native 64-byte
frame and native command-pointer spill. The residual is confined to the initial
list-address register and register-save/load scheduling.

`take_command` factors the actual free-list operation used by the target:
load the free command, clear its retry byte, remove it through the real list
helper and return that command. The function name and original inline boundary
are hypotheses; its only caller and every operation are real. This naturally
recovers the eight missing frame bytes without padding or unused storage.

The target writes its float value before the entry-clear statement in C; IDO
schedules these independent, disjoint field stores in the native order, closing
the two-word argument-load/entry-clear residual. The other A158 callers are
preserved rather than unnecessarily refactored. No fake caller, extra formal,
artificial volatile, pressure local or asm is introduced.

`group.json` declares the complete A158 and list context. The #191 fade body and
existing exact helpers are context only, with no additional matching credit.
Nonmatching context must not replace production owners wholesale.

```sh
python3 tools/cloud/score.py group cloud/work/lean_gfx_setup_e700_20261006
```

Use IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. Native O32 node/list layouts and a valid nonempty free-command
list are assumed. No target, scorer, accepted-lock or production-source edits
are included. The local comparison is research evidence; independent checking,
image integration and accepted coverage remain separate.
