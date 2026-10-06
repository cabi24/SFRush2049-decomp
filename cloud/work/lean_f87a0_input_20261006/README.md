# F87A0 input/state handler: 54-word research residual

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800F87A0`, `[0x800F87A0,0x800F8B68)`, **968 bytes / 242 words**.

Observed canonical O3 group result: **54 / 242 words differ**, zero nonzero
excess words, unresolved symbols, unverified relocations or errors. Its own
seven-entry jump table verifies at `0x8012461C..0x80124638`. This remains
**NONMATCH research**, not independently verified or accepted code coverage.

The initial complete source, with the newly matched real finish/shutdown
context, scored 242 / 242 with two unverified table sites: one omitted input
address instruction shifted the subsequent positional comparison. The submitted
used O32 address view restores full address materialization and the native
extent/table geometry. The remaining gap is the initial address register and
scheduling, followed by a one-register phase shift through the temporary ring.

## Explicit source assumption / context

The input read is written `*(s32 *)(u32)&D_8015694C`. Pointer-to-word-to-pointer is
lossless for this native O32 target. It is a compiler-affecting address spelling,
not a claim about original C source or portable host pointers. It adds no extra
input, unused local, fake helper, new volatile qualifier or dummy runtime work.
Other natural pointer/local/operand spellings did not improve the residual.

The full source derives from the existing real `s20261004/B/g87A0` body, with
all six old stand-ins removed. The actual `finish_state_alt` caller and
`audio_update_d` helper use the source from #222/#214; they are unclaimed context
here. The real accepted filter and clear/stop bodies retain minimal necessary
O32 declarations. The shutdown helper's genuine consumed pointer snapshot and
the finish caller's corrected post-callback reload/line layout are preserved.
The stop helper's earlier documented compiled-out read remains unchanged context.

This is not a complete original translation unit. F857C/slot-state context still
differs, and no source here should replace an accepted production owner
wholesale. Exact context scores, including previously submitted bodies, receive
no duplicate new-match credit. `group.json` has empty claims.

## Reproduce

From the repository root with the documented IDO 5.3 toolchain:

```sh
python3 tools/cloud/score.py group cloud/work/lean_f87a0_input_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, plus canonical mandatory assembler
`-r4300_mul`. The required real files/roots are fully declared in `group.json`.
No target, scorer, accepted-lock or production-source edit is made.

The source assumes native O32 interfaces, valid UI/state objects and queue
contracts. Arbitrary corruption, concurrency/aliasing and gameplay behavior are
not independently tested. The next concrete question is authentic input-address
lifetime/typing; adding pressure-only uses or artificial volatile is not justified.

Only C, required real context, recipe and notes are included. No independent
verification, harness, receipts, full tests, CI wait, image/ROM integration or
merge was performed. The independent checker owns acceptance and merging.
