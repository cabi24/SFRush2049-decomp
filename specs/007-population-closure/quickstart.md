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

```bash
python3 -m tools.conveyor.pipeline.datasyms generate      # build/m2c_datasyms.json
python3 -m tools.conveyor.pipeline.datasyms generate      # byte-stable check
sha256sum build/m2c_datasyms.json                          # identical both runs
python3 -m tools.conveyor.pipeline.protos generate         # now emits the externs
```

Actual (T005–T007, 2026-09-10, ~4 s per run): **2086 symbols** — s32 999,
f32 731, s8 213, s16 80, u16 37, u8 26; 646 formation-only (typed `s32`,
flagged `formation_only`), 1 same-width int/FP conflict (typed integer,
recorded); 32 omitted with reason (10 `not_ram`: lui+addiu integer
immediates like `0x39B40` and lui+lwc1 float immediates like `0x3F00xxxx`;
22 `function_address`: formed pointers to known function entries); **0
hand-table collisions**. Two consecutive runs byte-identical
(`db519c9d4e8224008c061961efe9890b457060d2f2977d332f3672c937b9624c`).
Where the addresses land: 786 above the blob (.bss), 1248 in the blob's
data tail (`0x8010FD60–0x80124AF0`: the code ends at `0x8010FD60`), 44 in
the static range, 8 in code gaps. `symbol_table_sha()` changed with the
layer, so every cached derivation under `build/m2c_asm/` regenerated on the
next pass (cache-key coverage, contract §10).

`protos generate` over the enlarged population (two passes, ~22 min each):
**840 declarations** (815 own-definition, 25 fallback; was 596) + 125
omissions (hand_context/static_target) + **2086 datasyms externs**, 0
datasyms omitted; passes A and B byte-identical
(`40a46dc33520acb5319b27db705edff71ed66582371e7adaebbbe624ca99482a`).

## 3. Re-measure & flywheel

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0   # ×2
python3 -m tools.conveyor.pipeline.autodecomp clusters diff \
    specs/007-population-closure/research/baseline.json build/m2c_histogram.json
```

Actual (T008, 2026-09-10, ~19 min per histogram, population 1287):
`170 compiled / 462 blocked / 237 partial_decomp / 43 decompiler_failure /
0 no_disasm / 375 extent_conflict` — identical bucket/target/blocker content
across the two runs. **SC-004 NOT MET: 170 < 200 → stop rule fired**;
`research/t008-shortfall.md` records the diff attribution (compiled +110,
of which 70 discovered and 46 `blocked → compiled` on the surviving
inventory; 4 of the baseline's "compiled" were function suffixes) and the
residual classes — the dominant wall is m2c's struct-member syntax on
generated symbols typed as scalars (227 blocked targets touched), which
needs generated struct shapes, a contract change for a next feature.
SC-001: the unresolved-callee class is gone (0 `undeclared func_8…`); the
12 in-blob `func_` token classes left are call-arity disagreements with
own-definition signatures; 24 out-of-image + 5 static-range classes are
outside the contract. SC-002: `x<addr>` 151 → 2 (linear-tracker
control-flow blind spot, recorded). SC-003: determinism held for all
content; the stored compile `diagnostics` carried the probe's random
`/tmp/tmpXXXX.c` path, now scrubbed (`_TMP_PATH_RE`); post-fix pair below.
