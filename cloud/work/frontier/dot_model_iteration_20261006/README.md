# Model iteration: current-contract NONMATCH reconstruction

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800E6AF8`, `[0x800E6AF8, 0x800E7030)`, 1,336 bytes / 334 words.
Research only; no matching coverage or production changes.

## Reproduce

With pinned IDO 5.3 in `IDO_DIR`, from the repository root:

    python3 cloud/work/frontier/dot_model_iteration_20261006/repro.py

Actual flags are `-g0 -O3 -mips2 -G 0 -non_shared`, including the canonical
scorer's `as1 -r4300_mul` stage.

Observed result:

    real-caller baseline: 314/334 words differ; emitted=330; frame=136; extra=0; unresolved=0; unverified=0; errors=0
    candidate NONMATCH: 125/334 words differ; emitted=332; frame=144; extra=0; unresolved=0; unverified=0; errors=0

The historical E56F8 packet records 313/334 with artificial volatile frame pads.
Removing its stand-in and pads gives the fresh 314-word baseline above. The
candidate improves against both, without restoring that fake storage. Native
frame is 160 bytes; the honest candidate remains 16 bytes short and emits two
fewer instructions. All its existing relocations are fully resolved/verified.

## Source-backed changes

Ancestry is [mdrive.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/mdrive.c),
particularly `model_iteration`, `update_drone_models`, and `update_link_cars`.
The N64 has multiplayer input records, recording/replay steps, and a different
model-clock path. This continues the complete existing native-led reconstruction.

- Restore the specifically accepted qualification for model-enable D_801525F0.
  `src/blob/func_800D60AC.c` and `players_frame_update.c` both declare it volatile;
  `include/game_globals.h` records that existing quirk. Those store-side sources
  use signed short. This read-side candidate preserves native `lhu` width with
  `volatile u16`, rather than silently changing the observed unsigned load.
- Restore the actual same-symbol `volatile s16 D_80153FD2` qualification from
  accepted `src/blob/func_800EC914.c:67`. This reproduces the native materialized
  human-count address/reloads. Qualification provenance does not prove interrupt
  behavior or justify volatile on unrelated variables.
- Read the velocity components in the donor's forward/lateral/vertical order,
  adapted to N64 coordinates. This recovers the native initial FP register lane
  while preserving the native arithmetic grouping.
- Restore the genuine drone-scheduler operation as an internal helper called by
  the real model-iteration root. Its exact N64 source boundary remains a
  hypothesis; no guessed retail stub identity is claimed. It takes no invented
  or unused argument and naturally inlines in the reported compile.

The qualifier-only result was 139/334. Coordinate order reduced that to 133.
The scheduler boundary plus coordinate order reaches 125. A broader two-helper
split left the player helper out of line and was rejected. A tick-pointer and
an alternative model alias view were worse and are not delivered.

## Context and limits

`repro.py` uses the existing `cloud/work/ipa-groups/func_800E56F8/group.c`, removing
its stand-in and artificial pad arrays. E6AF8 and E4B58 stay kept ABI roots;
E56F8 has its two actual native calls and remains internal. Complete real
E56F8/E4B58/E398C/E451C/E4300 source is preserved as context. No other new draft
is required, and no context body is claimed. The separate E56F8 five-word
candidate does not change this target's result.

Remaining differences include frame/local homes, instruction scheduling, and
control structure. No padding was added to conceal the residual. External
contracts and full gameplay semantics inherited from the archive remain
assumptions. A source boundary/layout explanation is needed before any frame
repair; further blind register sweeps are not supported.

Compiler/score iteration only: no independent review, behavior harness, full
tests, CI wait, shadow/image build, or ROM gate was added before publication.
