# w23r: recomp Phase 3 (per-call capture, input scripts, static binding, OSThread word mask) — tooling lane, worktree branch w23r

Status: in progress (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w23r` (copy of this branch's tracked files; nothing ROM-derived committed).

## Plan
1. Per-call capture at a replaced function's entry/exit in both builds (stock build gets transparent capture hooks).
2. Input scripts under tools/recomp/scripts/*.press (audio options, second track, two players).
3. Static binding of replaced bodies' local statics to retail addresses.
4. OSThread host-pointer word mask instead of unjudged pages.
5. Tests, README, plan section 11, WAVE_BRIEF "Behavioural check".
