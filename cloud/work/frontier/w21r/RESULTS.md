# w21r: recomp Phase 1 (boot) — tooling lane, worktree branch w21r

Status: in progress (2026-10-11). Not a matching lane: no src/, asm/, lock or coverage change.

## Plan
1. Build N64ModernRuntime on rocky; decide N64Recomp pin (81213c18 vs ffb39cda).
2. tools/recomp/runtime/: CMake host project (shims, overlay/game-image hooks).
3. Boot on rocky; log in tools/recomp/BOOT_LOG.md.
