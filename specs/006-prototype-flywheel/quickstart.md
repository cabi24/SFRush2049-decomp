# Quickstart: Prototype Layer & Seed Flywheel (006)

Everything except §4 runs on the Pi alone. Baseline (2026-07-19):
`42 compiled / 597 blocked / 49 decompiler_failure / 0 no_disasm /
197 extent_conflict`; 289 `func_`-shaped blockers.

## 1. Hygiene + honest buckets

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0
python3 -c "import json; d=json.load(open('build/m2c_histogram.json')); print(d['buckets']); assert sum(d['buckets'].values())==d['run']['targets']"
```

Expect the six-bucket schema with `partial_decomp` > 0 (M2C_ERROR class,
~101 at baseline priced as blockers). Run twice → identical counts.
Scoped probe check: a `--targets` run writes `build/m2c_probe.json` and
leaves `build/m2c_histogram.json` untouched (compare mtime/sha).

Actual (2026-07-19, T005): two full-population runs both produced
`37 compiled / 299 blocked / 303 partial_decomp / 49 decompiler_failure /
0 no_disasm / 197 extent_conflict` (sum 885) with
`run.population_complete: true`. The complete JSON objects were identical
after removing only `run.timestamp`; the generated Markdown was byte-identical
(`sha256 5b9c99fae2704321f8b0ba0a9a3b3d04e5a696eb19aa24f5df143d7df70b0924`).
All 101 functions in the baseline's visible `M2C_ERROR` blocker class moved to
`partial_decomp`; raw-output classification exposed another 202 placeholders
which had previously been masked by an earlier compiler diagnostic.

The diff against `research/baseline.json` reported bucket deltas
`blocked -298`, `compiled -5`, `partial_decomp +303`, with the other buckets
unchanged. Its 308 target movements were `293 blocked -> partial_decomp`,
`10 compiled -> partial_decomp`, and `5 blocked -> compiled`. A scoped probe
of `Input_AllocPadSlot` wrote `build/m2c_probe.{json,md}` with
`population_complete: false`; SHA-256 and nanosecond mtime for both population
artifacts remained unchanged.

## 2. Declaration layer

```bash
python3 -m tools.conveyor.pipeline.protos generate      # two passes, ~8 min
python3 -m tools.conveyor.pipeline.protos generate      # byte-stable check
sha256sum build/m2c_protos.h                            # identical both runs
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0
```

Gates: `func_<addr>` blockers for known targets = 0; compiled ≥ 200
(SC-001); zero redefinition-class errors in the run's diagnostics
(SC-002); SC-005 body-identity test still green
(`pytest tests/conveyor/test_autodecomp_population.py -k body_byte_identical`).

Attribution (FR-010):

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters diff <pre.json> build/m2c_histogram.json
```

## 3. Local suite

```bash
pytest tests/conveyor -m "not node_required"
```

New: `test_protos.py`; extended: `test_autodecomp_population.py` (bucket,
hygiene rules incl. no-overreach, probe routing, priority-ladder assert).

## 4. Flywheel window (coordinator + watchman)

```bash
# bring-up per specs/001-matching-pipeline/quickstart.md §1–4, then:
python3 -m tools.conveyor.pipeline.farm run          # daemon with flywheel step
python3 -m tools.conveyor.cli report                 # 'extracted: compiled N, scored M, in_search K'
```

SC-004: after one unattended window, scored M == compiled N. SC-005: no
static job displaced (queue timestamps: any static submission leases before
waiting flywheel jobs). SC-006: `promotion_record` has zero extracted rows.

## Actuals

*T005 actuals are recorded in §1; later task actuals follow their sections.*
