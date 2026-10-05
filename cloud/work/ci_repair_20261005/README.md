# CI repair, 2026-10-05

GitHub Actions `Verify` had failed on every master push since 5c98cecc (except
52904b37). These research packets and their tests were written before later,
legitimate project changes: boot-tail functions promoted into `src/rom` TUs
(waves 3-4), shared declarations that gained real field names, game splices that
rewrite `asm/us/blob/SHA256SUMS`, and owndata gaining `.bss` placement. No
production source, lock, protected target or `tools/cloud/score.py` was edited.

Reproduced on `watchman2` in a clean clone of `integrate-prs2` with the CI IDO
(`tools/cloud/setup.sh`), `REQUIRE_TOOLCHAIN=1`, and every workflow step.

## Changes

| Failure | Cause | Fix |
|---|---|---|
| `tests/conveyor/test_voice_list_promotion.py` (4 errors: `wrong_stride: mutation did not apply`) | 9b7d51a0 named the tail of `VoiceState_80019C8C` in `src/rom/lib_1a660.c`; `unknownC4[220]` no longer exists. | `cloud/work/boot_tail_promotion/voice_lists/verify.py`: the stride negative control now pads the current final field (`unknown17E[34]` -> `[38]`, 416 -> 420 bytes). It still must be rejected by the full-body comparison. |
| `tests/conveyor/test_frontier_resource_initializer.py::test_fresh_full_body_link_and_differential_replay` | `func_8010D85C` was later accepted independently (3eb2e97f); the verifier asserted it was still unlocked. | `cloud/work/frontier/dot_resource_initializer/verify.py`: dropped the "still unlocked" preflight assertion (true at the packet's base commit, recorded in a comment). The NONMATCH replay, link, differential and sanitizer checks are unchanged. |
| `tests/conveyor/test_dot_route_tracker.py::test_fresh_pinned_compiler_replay` | `verify.py` compared the full receipt, including `owndata_sha256` and `target_manifest_sha256`. | `cloud/work/frontier/dot_route_tracker/verify.py`: check mode excludes `scorer_sha256`, `owndata_sha256` and `target_manifest_sha256` as frozen provenance (the test already documented this). All compiled results and source hashes are still compared. |
| `tests/conveyor/test_dot_fresh_small_20261005.py::test_fresh_source_extent_relocation_and_semantic_replay` | Receipt pins `target_manifest_sha256` (`asm/us/blob/SHA256SUMS`), which changes with every splice. | The test drops `target_manifest_sha256` from both sides before comparing (frozen provenance). |
| `tests/cloud/test_dot_fresh_masked_rng.py::test_compiled_source_replays` | Same manifest pin (plus a scorer hash). | The test drops `target_manifest_sha256` and `scorer_sha256` before comparing. |
| `tests/cloud/test_task_enqueue_research.py::test_ido_replay_and_native_abi_when_compiler_is_available` | Same manifest pin. | The test drops `target_manifest_sha256` before comparing. |
| `tests/cloud/test_macro_stream_contracts.py` (3 tests: `full-TU extent mismatch: func_80025DC0`) | `func_80025DC0`, the last function of `lib_25bb0`, was promoted after the packet's base. The verifier checks every current lock in the BASE object, where the function was assembly and its label covers the 4-byte `.text` alignment pad. In the current object the C symbol is 236 bytes, but the span reaches the 16-byte section end. | `cloud/work/boot_tail_promotion/macro_stream_contracts/verify.py`: (1) bodies promoted after BASE that are outside this packet (not candidates, not `EXISTING`) are recorded as `promoted_after_base` and skip the BASE-object check. Their accepted C is still checked exactly in the current and combined objects, and `untouched()` still compares the BASE words. (2) In full-TU mode, the TU's final function may be followed only by zero words that end on a 16-byte section boundary and are shorter than 16 bytes. The ELF symbol-size check still requires the exact extent, and the existing negative test (`[1,2,0]`, unaligned padding) is still rejected. |
| `tests/cloud/test_sequence_context_contract.py::test_actual_tu_and_every_adapted_standalone_source` | The stride negative control inlined the research header copy. Since ddf9ec6b the TU uses the canonical `include/sequence_context.h`, and 62d984bb named `disabledFC5`/`valueFC6`, so promoted bodies did not compile against the old copy. | `cloud/work/boot_tail_promotion/sequence_context_contract/verify.py`: the control mutates the header the TU actually compiles against (`include/sequence_context.h` when present). The mutation (`unknownFF4[4]` -> `[8]`) is unchanged and is still rejected by the GNU-linked full-TU comparison. |

## Not changed (needs a maintainer)

- `cloud/matches/func_800F0674.c` (added in 5c98cecc) has line 1
  `/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (also code-identical at -O2) */`.
  `check_submissions.py` passes the parenthetical to IDO
  (`cc: Error: malformed or unknown option: -O2)`), which is why the 5c98cecc
  run failed "Rescore changed cloud submissions". Later pushes do not touch the
  file, so CI does not rescore it. It will fail again if the file is changed or
  if an all-zero base is checked. Fix: move the parenthetical to line 2.
- The Guard step runs only for pull requests. Integration commits that splice
  (for example 3dfd0a62) change protected paths by design and are pushed by a
  maintainer.
