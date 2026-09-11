# 007 Population Closure — close-out scorecard (2026-09-10/11)

Branch `007-population-closure`. Executed in one Opus session against the
contract (routing deviation from HANDOFF §2 noted: no Codex pass; the
contract oracles were run as the gates). Evidence: `quickstart.md`
(§1–§3 actuals), `research/t008-shortfall.md`, `build/closure_report.json`,
`build/m2c_datasyms.json`, `build/m2c_histogram.{json,md}`.

| SC | Verdict | Evidence (short) |
|----|---------|------------------|
| SC-001 second pass registers zero; no in-blob `func_` blocker | **MET with a nuance** | Run 1: 402 registered (≥135), run 2: 0, DB/evidence byte-identical. The unresolved-callee class is gone (0 `undeclared func_8…`). 12 in-blob `func_` *tokens* remain in the blocker histogram, all call-arity disagreements with own-definition signatures (a 006 precedence rule), not unresolved callees — recorded in the shortfall. |
| SC-002 layer byte-stable, no hand collisions, `x<addr>` eliminated | **MET, 2 stragglers** | 2086 symbols, `db519c9d…` twice, 0 collisions; `x<addr>` 151 classes / 115 targets → 2 classes / 3 targets (linear-tracker control-flow blind spot). |
| SC-003 100% coverage, exclusive, deterministic, diff attributes | MET | 1287/1287 in one bucket; two post-fix runs byte-identical modulo timestamp (a `/tmp` path in stored diagnostics was the only nondeterminism — scrubbed); `clusters diff` attributes 654 movements. |
| SC-004 compiled ≥ 200 or stop-rule report | **NOT MET — 170 < 200**, stop rule honoured | +110 over baseline (60 → 170); `research/t008-shortfall.md` names the walls: struct-shaped generated symbols typed as scalars (227 blocked targets), locals/args typing (167), call arity (23). |
| SC-005 flywheel scores 100% of compiled unattended; 0 extracted promotions; static unchanged | **OPEN — window running** | 94 seeds submitted unattended at priority 60 in one cycle, dedupe verified (0 on later cycles); `promotion_record` extracted rows = 0; static body-identity guard green. 100%-scored is checked at drain (~94 × 4 h on watchman2): `cli report`. |

## Amendments made live (recorded in quickstart §1 / README 007)

- **Suffix rule**: 178 inventory rows start strictly inside a discovered
  extent (prologue scan started late) → `extent_conflict:<func_id>`;
  `targets._extent_plan` treats discovered extents as containers.
- Datasyms omission rules beyond the contract text: non-RDRAM constants
  (`not_ram`) and function-pointer formations (`function_address`).

## Farm defects found and fixed while opening the window

Ingest/coordinator SQLite deadlock (since 005), stale-result demotion of
matched targets (17 restored), flywheel queue starving static top-up —
see quickstart §3 and `bee3781`.

## Next

- **Feature 008 — generated struct shapes** (the 227-target wall) + arity-
  tolerant declarations where callers disagree (23 targets). Contract
  change; own spec.
- Close SC-005 when the window drains; then merge to master.
- Look at the 28 out-of-image callees in `0x8038A400–0x803A1EAC`.
