# Decoder-facing callers: recovery checkpoint

Filesystem replacement at approximately 2026-10-04 04:32 UTC removed the original
worktree, private compiler outputs, authored packet files and the pending host
harness. Nothing in this file is a recovered commit, independent review or CI claim.
The sources here were recreated from retained authored command text. They must be
placed in a fresh authorized checkout and independently recompiled/reviewed.

## Previous activation and observed checks

- Pair-only activation: BT07-decoder-facing-callers, central claim bd452b11.
- Previous clean base: eae0295b40271c4758c7a3ecb890e48192487f8c; branch
  dot/boot-tail-bt07-decoder-facing-callers. Its previous branch was preserved.
- Selected native extents: 8002574C–800259A8, 604 B;80025F74–800262BC,840 B.
- Before reset: 8002574C O2 strict0/151, no excess, actual ELF STT_FUNC604 B;
  80025F74 O2 NONMATCH2/210, no excess, actual ELF STT_FUNC840 B.
- O1 negative controls: 2574C150/151 +166 nonzero extras;
  25F74 209/210 +54 nonzero extras.
- Strict own-body relocations and the separately checked full-function relocations
  were clean. The overlong2574C O1 target-window comparison ended on an unpaired
  HI16, while relocation across its full actual extent was clean.
- Original verify.py completed successfully before the reset. Its generated
  receipt was in /tmp and is lost. No host semantic tests completed before reset;
  the authored harness is being recreated separately. No peer review or CI.

## New recreated artifacts

- func_8002574C.c: SHA256
  09afc7ba5732799e15937190a709c72f1b82b1da46a477b4eda8ab4505450a26
- func_80025F74.c: SHA256
  0f3dde0cda2d618d93193d6e37718a43e1b007156fb36abbd872185e5c78c91d
- controls/initial_func_*.c: initial complete sources, recreated from the original
  authored command. Historical initial O2/O1 residuals are encoded in verify.py.
- verify.py: reconstructed read-only replay utility. Requires normal packet path
  inside a restored repository and input_pins.json copied from unchanged committed
  BT07-stream-pair input pins; do not invent fresh pins to fit changed tools.
- recover_sources.py: local recovery generator, not needed in final packet.

The new hashes are not historical pre-reset hashes. Native replay must establish
that the intended exact source form was recreated before submission. Keep compiler
and object artifacts private. No ROM/raw disassembly/object bytes are in this set.

## Fresh verification after restoration

At 04:41 UTC, independently recompiled both initial and final source pairs against
restored PR #74 and its authenticated 24-file IDO toolchain. All expected O2/O1
residuals reproduced; both O2 function extents are exact. Fresh receipt:
recovery_replay.json. No old generated receipt was reused.

At 04:47 UTC, the rebuilt actual-source host harness passed 8,964 cases in each
of C89 O2 and ASan/UBSan O1, with complete 32-bit layout assertions. Receipt:
semantics.json. These are new test results, not pre-reset test claims.
