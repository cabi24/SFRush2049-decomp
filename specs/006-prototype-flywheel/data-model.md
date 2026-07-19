# Data Model: Prototype Layer & Seed Flywheel

No schema migration. Two new regenerable `build/` artifacts, one bucket
added to the histogram schema, one probe artifact pair, and value-level
reuse of existing tables.

## Generated declaration layer (`build/m2c_protos.h` + `.json`)

`m2c_protos.h` — one declaration per line, sorted by symbol name, emitted
only for names that pass the precedence filter. Header comment carries the
generation stamp. Never hand-edited (regenerating overwrites).

`m2c_protos.json` — the evidence stamp and per-callee provenance:

```json
{
  "stamp": {"timestamp": "...", "image_sha": "...", "symbol_table_sha": "...",
             "context_sha": "...", "derivation_version": 2, "passes": 2},
  "declarations": {"<name>": {"signature": "s32 f(void *arg0);",
                                "source": "own_definition|fallback",
                                "pass": 2}},
  "omitted": {"<name>": "hand_context|static_target|underivable"}
}
```

Invariants: byte-stable across runs with identical stamp inputs; every
known-target callee referenced by extracted code appears in exactly one of
`declarations`/`omitted`; `declarations` ∩ hand-context names = ∅.

## Histogram schema (extended)

Buckets become: `compiled` | `blocked` | `partial_decomp` (**new**: seed
text contains a decompiler error placeholder — checked before the compile
probe; overrides `compiled`/`blocked`) | `decompiler_failure` | `no_disasm`
| `extent_conflict`. All 005 invariants (exclusivity, 100% coverage,
determinism, counts sum) extend to the new bucket.

Artifact routing: full-population runs (no target filter, no truncating
limit) → `build/m2c_histogram.{json,md}`; scoped/limited runs →
`build/m2c_probe.{json,md}`. The population artifact is only ever written
by a run that covers the whole population.

`clusters diff <old.json> [<new.json>]` view: per-target bucket movements
(`target: blocked → compiled`), bucket-count deltas, blocker-class deltas
(count and functions), deterministic ordering.

## Hygiene rules (behavioral entities, in `_clean_m2c`)

| Rule | Trigger | Rewrite | No-overreach bound |
|---|---|---|---|
| saved-register locals | body references `saved_reg_s[0-7]`, undeclared in seed | inject `s32 saved_reg_sN;` at function top | only the exact `saved_reg_s\d` names, only when undeclared |
| scalar-cursor members (generalizes 005 rule) | `x->unkNN` where `x` is a *declared local* of scalar or scalar-pointer type | explicit offset load `(*(s32 *)(x ± 0xNN))` | keyed on the local's declaration in the same seed; never fires on globals/typed externs |
| error placeholder | `M2C_ERROR` present in seed text | **no rewrite** — classification only (`partial_decomp`) | n/a (explicitly excluded from hygiene) |

## Flywheel cycle (farm daemon step)

Input: `build/m2c_histogram.json` (population artifact only — a stale or
probe file is refused by checking the stamp covers the population).
Per `compiled` target:

```
has score evidence?  = EXISTS work_unit(job_type='permuter_search', target_id)
                     OR EXISTS matrix_entry(target_id, score NOT NULL)
if not → submit_one(...) with priority=60, standard budget
```

Queue-priority ladder (measured): farm verify 1 < promote 10 < static
autodecomp seeds 30 < **flywheel 60** < default 100. Coordinator leases
`ORDER BY priority, created_at` ⇒ Track A always precedes flywheel work.

Report addition: `extracted: compiled N, scored M, in_search K` in the
standard report path.

## Existing tables (reused, unchanged)

- `work_unit`: flywheel submissions are ordinary `permuter_search` rows;
  `best_score`/`best_source_sha` remain the evidence of record.
- `matrix_entry` / `function_status`: unchanged lifecycles.
- `n64_target`: read-only for this feature (population, gate_reason
  filters as in 005).
- Promotion firewall (005): untouched; SC-006 re-verified after flywheel
  volume.
