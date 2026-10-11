# w21r: recomp Phase 1 (boot) — tooling lane, worktree branch w21r

Status: in progress (2026-10-11). Not a matching lane: no src/, asm/, lock or coverage change.

## So far
- N64ModernRuntime cdf5abbd builds on rocky as a subproject of `tools/recomp/runtime`
  (CMake/Ninja, clang 21); pin decision in tools/recomp/VERSIONS.md (81213c18..ffb39cda changes
  only `--dump-context` ordering: same emitted C contract).
- Host binary links: 2136 recompiled functions + librecomp/ultramodern + null renderer + shims.
- First run: entrypoint -> main_recomp runs, crashes in osPiReadWord (a hardware PI status read):
  the static inventory's libultra names are partly wrong, and N64Recomp replaces libultra BY NAME
  (identity audit in progress -> tools/recomp/identity.json).
