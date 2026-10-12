# w22r: recomp Phase 2 (function replacement + frame oracle) — tooling lane, worktree branch w22r

Status: in progress (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w22r` (copy of the Phase 1 tree; nothing ROM-derived committed).

## Plan
1. Frame oracle in the host: per-frame rdram page hashes + display-list hashes, frame-clocked input.
2. Replacement build: `tools/recomp/replacements.json` -> replaced functions linked into the host.
3. Proof: ten matched functions EQUIVALENT, injected bug flagged, one draft verdict.
