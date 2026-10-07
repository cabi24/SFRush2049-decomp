# Complete twelve-command effect dispatcher research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `entity_ai_pathfind`, `[0x800992AC, 0x80099B30)`, **2,180 bytes / 545 words**.
The historical name does not describe the recovered effect-message thread.

## Observed compilation

The complete standalone source scores **536 / 545 differing words**, 13 nonzero
excess words and two unverified jump-table relocation sites. The submitted real
context group scores **514 / 545**, **19 excess words**, **two unverified sites**,
and **one unpaired R_MIPS_HI16 for D_80142728**. No unresolved symbols were reported.
The source-level goto-loop control removed the unpaired-relocation diagnostic
but worsened the result to 521 / 545 with 28 excess; it is not submitted.

These are broad **NONMATCH research observations**, not complete relocation proof,
independent verification or accepted coverage. The canonical scorer's error is
reported, not masked. Its positional count does not establish semantic correctness.
The submitted frame is 128 bytes versus native 208; no filler is supplied.

## Recovered source / corrected interfaces

The protected own-data artifact supplies all twelve switch cases. The source
initializes the real message queue and eight-byte scheduler-client object, waits
for scheduler ticks or commands, locks the effect queue, and dispatches:

- sequence start/stop;
- duplicate-aware effect activation and voice allocation;
- four independent float-field updates with the native -2 sentinel;
- stop/dispose commands and whole-list stop/pause/resume;
- two-channel music adjustment and a signed-byte global setting.

Used-message release and queue unlock occur after command dispatch. Saved next
pointers protect traversal across cleanup callbacks. The case-3 returned voice
is captured before the message's target pointer is re-read for the store.

The union represents actual message field overlaps instead of the decompiler's
incorrect byte/float reinterpretations. Pointer-consuming cleanup calls receive
the actual effect carried in the native private register. The music conversion
is ordinary IDO unsigned float-to-word conversion, not a fake cfc1 macro. The
thread's one real incoming argument is homed but unused in the native body;
it is not an invented pressure argument.

`func_800979A0` remains an external, unchanged observed two-word call contract;
its frozen source is not investigated here. `func_80096238` is explicitly declared
without a fixed parameter prototype: this native caller supplies its handle,
while the currently accepted body ignores incoming arguments. Its original C
formal list is not established. No body or false return type is invented for it.

## Actual context and reproduction

`group.json` names the exact files/roots and has empty claims. The context contains:

- real typed B132 callers/list routines and the cleanup spelling from PR #187;
- the actual A158 setter family with the #191/#197 research changes;
- unchanged accepted lookup, mark-all and music-control implementations;
- the real B113 allocation-success source, with its duplicate old wrappers omitted.

No stand-in caller or empty substitute callee is supplied. All non-target bodies
are unclaimed context, whether their local score is exact or different. They
must not be installed wholesale over their accepted production owners. The O2
header on unchanged music context is its historical standalone recipe; this
entire group was actually compiled O3.

```sh
python3 tools/cloud/score.py group cloud/work/lean_effect_dispatch_20261006
```

Run from the repository root with the documented IDO 5.3 toolchain. Exact group
flags are `-g0 -O3 -mips2 -G 0 -non_shared`, plus mandatory canonical assembler
`-r4300_mul`. The source alone can also be scored with `score.py fn`, the same
flags, and target `entity_ai_pathfind`. No scorer/target changes are made.

## Limits / next useful lead

The source assumes native O32 layouts, valid queue messages and effect/list
storage, and float values within the intended command domains. Native narrowing,
message field overlap and post-callback rereads are explicit; arbitrary aliasing,
corruption, scheduler behavior and unrestricted floating-point conversion inputs
have not been tested. Descriptive type/field names are reconstruction hypotheses.

The missing original local/inlined-source layout and exact module visibility
remain important: register-save count, frame and scheduling are still broad.
The one unpaired relocation must be resolved before any complete match claim.
This packet contains only complete C, real context, recipe and these notes. No
independent verification, harness, receipts, full tests, CI wait, image/ROM
integration or merge was performed. The independent checker owns acceptance.
