# update_model state/port reconstruction: 5-word NONMATCH

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800E56F8`, `[0x800E56F8, 0x800E5C94)`, 1,436 bytes / 359 words.
Research only: no match, coverage, production edit, or integration claim.

## Reproduce and score

From the repository root with pinned IDO 5.3 available in `IDO_DIR`:

    python3 cloud/work/frontier/dot_update_model_20261006/repro.py

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical group stages including
`as1 -r4300_mul`.

Observed output:

    real-caller baseline: 115/359 words differ; emitted=359; frame=24; extra=0; unresolved=0; unverified=0; errors=0
    candidate NONMATCH: 5/359 words differ; emitted=359; frame=24; extra=0; unresolved=0; unverified=0; errors=0

Both objects have the complete native-sized extent and native 24-byte frame.
The best archived result is the E56F8 group's 115/359. No stronger later E56F8
packet was found in the current frontier/research notes. Replacing its old path
children with the newer real E4300/E451C context did not change the target score.

## Actual changes

The source lineage includes
[mdrive.c:update_model](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/mdrive.c),
with substantial N64-specific lifecycle, mode, input, and force-output additions.
This is refinement of an existing complete native-led body, not a newly found
whole-function arcade implementation.

- Read and write player state bytes +0x358/+0x359 through the existing signed
  `GC2` fields. Native uses signed byte reads. The old stores through an unsigned
  padding array prevented the native shared constant-2 register.
- Assign the actual controller-port byte loads to consumed integer locals at
  reset and force-output sites. These are real output indexes, not dummy pressure.
- Express the pending bonus calculation directly, then perform its real clear.
  Remove the now-unused m2c temporaries.
- Use this function's natural float literals. Every relocation is verified by
  the unchanged canonical scorer in the final body.

Signed fields alone changed the old 115-word residual to 122, but fixed its
original constant-2 cause. The actual port locals reduced it to 23, then the
bonus/force-output reconstruction reached five words. Merely sharing one port
local everywhere was worse. Natural gain-selector and early-return alternatives
were inert or worse; no broad permutation sweep was run.

## Real context and remaining gap

The reproduction reads the existing `ipa-groups/func_800E56F8/group.c`, removes
its historical stand-in and all artificial volatile pad arrays, and retains the
complete real E6AF8 caller with its two native call sites. E6AF8 and E4B58 are kept
ABI roots; E56F8 is internal. The actual E398C/E451C/E4300 path bodies remain
context. The candidate source replaces only E56F8. No other draft PR is needed.

All context remains unclaimed. Removing the fake pads leaves E6AF8's real-source
frame at 136 versus native 160, with 314/334 differing words; this limitation is
not hidden by retaining invented storage. Wider parent/game semantics and the
inherited external contracts are not certified by this packet.

Remaining target differences are one comparison operand order at +0x404 and a
four-word scheduling shift at +0x4BC..+0x4C8. Full extent, FP instructions, frame,
and all other words agree. Further work needs a genuine source/scheduling
explanation; no artificial volatile, padding, helper, or assembly was introduced.

Compiler/score iteration only. No independent review, behavior harness, full
tests, CI wait, or ROM gates were run before this lean research handoff.
