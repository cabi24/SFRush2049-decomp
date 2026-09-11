# 006 Prototype Flywheel — close-out scorecard (2026-09-10)

Branch `006-prototype-flywheel`, closed out per `HANDOFF.md` §1. Evidence
lives in `quickstart.md` (§1 T005, §2 T009, §4 T012 actuals) and
`research/t009-shortfall.md` (both sections). Nothing below is massaged.

| SC | Verdict | Evidence (short) |
|----|---------|------------------|
| SC-001 compiled ≥ 200, known-target `func_` blockers = 0 | **NOT MET** — 60 < 200 (structural) | Post-separator-fix histogram `60/298/281/49/0/197`; known-target `func_` blockers 0. Shortfall is inventory closure (≥135 unregistered blob callees) + raw data addresses, not the layer — `t009-shortfall.md`. Handed to 007. |
| SC-002 layer byte-stable, no redefinitions, no static regression | MET | Double `generate` identical (`93b72973…` post-fix); zero `redeclar\|conflicting types`; body-identity test green. |
| SC-003 histogram coverage/exclusivity/determinism; probes don't clobber | MET | Two full runs identical modulo timestamp, sum 885; `--targets` probe wrote `m2c_probe.*` and left the instrument's sha/mtime unchanged. |
| SC-004 after one unattended window, scored == compiled | **NOT MET — 58/60** | 23 submitted, 21 DONE, 2 FAILED on a permuter function-detection bug (function-pointer params) — fixed (`function.txt`) and resubmitted on the rebuilt toolkit; see §4. |
| SC-005 no static job displaced | MET (vacuous + asserted) | Zero `priority < 60` jobs created/leased during the window; ladder asserted in `test_flywheel.py`. |
| SC-006 zero extracted rows in `promotion_record` | MET | 28 rows, 0 extracted. |

## What the close-out changed

- Hardening (HANDOFF §1.2): farm `with_transient_retry`; coordinator
  `_record_blob` locked-write retry; permuter `function.txt`.
  Tests: `tests/conveyor/unit/test_hardening_006.py`.
- **Builder repair (found here, not in the HANDOFF):** the toolkit bundled the
  22.04 build host's `libc.so.6`; on watchman2 (26.04) that aborted objdump
  and every scoring job since the 2026-07-28 retarget. Builder now excludes
  glibc, nodes health-check the bundled objdump and fall back to the system
  one, toolkit rebuilt on watchman2 and pinned (`796ae99a5cb7…`), smoke
  fixture repointed to the post-004 asm path, `smoke --fresh` added and
  PASSED end to end.
- T014: local suite green (`pytest tests/conveyor -m "not node_required"`,
  189 passed, 5 deselected).

## Open after 006 (routed, not chased here)

- 007 population closure (branch already spec'd) — the SC-001 fix.
- `autodecomp harvest` skips extracted base-0 stubs (static-only asm index):
  `audio_engine_update` (2-insn `jr $ra; nop`) is recorded in §4 only.
- Farm `ingest` has not been the banking path since 005 (549 DONE/FAILED
  search rows with `ingested_at IS NULL`); `harvest` is. Reconcile or retire
  in a later feature.
- Node-side "seed does not compile" hides the compiler's stderr; surface it
  in the result payload so a broken node is distinguishable from a bad seed.
- Track A backlog per HANDOFF §3.
