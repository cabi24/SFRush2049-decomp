# Quickstart: Track B Population Closure (007)

Everything except §3's flywheel window runs on the Pi alone. Baseline
(006 close-out, `research/baseline.json`): `60 compiled / 298 blocked /
281 partial_decomp / 49 decompiler_failure / 0 no_disasm / 197
extent_conflict` over 885; image `bf7da3fa…`; layer `93b72973…`.

Preflight (T001, 2026-09-10): `build/m2c_histogram.json` carried exactly
those buckets with `population_complete: true` and `game_code_sha` equal to
`sha256sum build/game_code.bin` (`bf7da3fa6283…`); copied to
`research/baseline.json`.

## 1. Callee closure

```bash
python3 -m tools.conveyor.pipeline.closure run            # fixpoint + report
python3 -m tools.conveyor.pipeline.closure run --report build/closure_report.run2.json
# oracle: run 2 prints registered=0 and n64_target/matrix_entry are unchanged
```

Outcomes per candidate land in `build/closure_report.json` (`candidates`,
`iterations`, `totals`, `caps`, `reclassified`; deterministic order).

**Amendment (recorded here, 2026-09-10 — the 005 rule in the other
direction).** The first live run showed that 177 of the discovered functions
*contain* an existing inventory row: the inventory's prologue scan keyed on
`addiu $sp` and started 1–322 instructions late (IDO hoists `lui/lw` global
loads ahead of the stack adjust). Every one of the 178 boundaries was
checked — none is preceded by `jr $ra` or `j`, so all are mid-function and
those inventory rows are function *suffixes*, not functions. The closure
therefore applies 005's own definition ("address strictly inside a
gate-passed extent") to them: `gate_reason` becomes
`extent_conflict:<func_id>`, object and evidence untouched (no supersession),
listed under `reclassified`. `targets._extent_plan` treats closure-registered
extents as containers too, so a later `matrix extract` agrees. Work-inventory
names are historical from here on; the population is closure-derived.

Actual (T004, 2026-09-10, image `bf7da3fa…`, run time < 2 s each):

| run | iterations | registered | inside_existing | scan_failure | invalid | cap_hit | reclassified |
|-----|-----------:|-----------:|----------------:|-------------:|--------:|--------:|-------------:|
| 1 | 5 (360 / 31 / 8 / 3 / 0) | **402** | 0 | 0 | 28 | 0 | 178 |
| 2 | 1 | **0** | 0 | 0 | 28 | 0 | 0 |

- Run 1 registered 402 ≥ 135 (contract oracle). 47,298 instructions of new
  target population; sizes 2–1307 insns, median 60; two 2-insn stubs.
- The 28 `invalid` are all `outside_image`: call targets in
  `0x8038A400–0x803A1EAC`, above the blob end (`0x80124AF0`) — a code region
  no known image covers (11 of them from `input_aux_handler`). Residual for a
  future feature; they are stable across runs and do not block anything.
- Run 2: zero registrations, zero reclassifications; `SELECT * FROM
  n64_target` hashed identical before/after; `matrix_entry` count 110,045
  before and after (no evidence change).
- Population after closure: 1287 extracted rows = 912 gate-passed functions
  (688 − 178 suffixes + 402 discovered) + 375 `extent_conflict`
  (197 from 005 + 178 suffixes). Static population untouched (246).

## 2. Generated data symbols

*(T005–T007; actuals follow.)*

## 3. Re-measure & flywheel

*(T008–T009; actuals follow.)*
