# Implementation Plan: Track B Prototype Layer & Seed Flywheel

**Branch**: `006-prototype-flywheel` | **Date**: 2026-07-19 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/006-prototype-flywheel/spec.md`

## Summary

Three small extensions to shipped 005 machinery, all orchestrator-local
except the scoring itself:

1. **Declaration-layer generator** (`pipeline/protos.py`, new): one m2c pass
   per derivable extracted target (the histogram already does exactly this
   pass — reuse it), capture each function's **own emitted definition line**
   as its signature, fall back to a C89 empty-parens declaration
   (`s32 name();` — call-compatible with anything) for targets m2c can't
   decompile. Emit `build/m2c_protos.h` + a JSON evidence stamp; wire it
   into `_context()` **after** hand-curated headers with an
   already-declared-names filter so hand context always wins. Because the
   signature source is the callee's own definition, there is no call-site
   conflict to resolve — the design's one subtlety (signatures depend on
   context which includes the layer) is closed by a fixed two-pass
   generation (pass 1 no-layer, pass 2 with pass-1 layer; stop), which is
   deterministic by construction.
2. **Hygiene + honest buckets** (`autodecomp.py`): extend `_clean_m2c` with
   the saved-register rule (inject missing `s32 saved_reg_sN;` locals) and
   generalize 005's byte-cursor rule to scalar-typed locals; add the
   `partial_decomp` bucket (seed text contains `M2C_ERROR` ⇒ never
   `compiled`); scoped runs write `build/m2c_probe.{json,md}` so the
   population artifact is never clobbered; add a `histogram diff` view for
   run-over-run bucket movement.
3. **Flywheel** (`pipeline/farm.py`): a daemon cycle step that reads the
   population histogram's `compiled` list, submits searches for targets
   without score evidence at **priority 60** (static work runs at 1–30 and
   the coordinator leases by ascending priority, so Track A always wins),
   and reuses the existing harvest; a coverage line in the standard report.

## Technical Context

**Language/Version**: Python 3.9+ (Pi orchestrator), stdlib only
**Primary Dependencies**: existing 005 modules (`autodecomp.py`, `disasm.py`,
`farm.py`); mips binutils + m2c unchanged; no toolkit/node changes
**Storage**: SQLite (`~/.conveyor/conveyor.db`, no schema migration —
score-evidence detection reuses `work_unit`/`matrix_entry`); generated
artifacts in `build/` (regenerable, not committed)
**Testing**: pytest `tests/conveyor -m "not node_required"`; new
`test_protos.py`, extensions to `test_autodecomp_population.py`
**Target Platform**: Pi for generation/hygiene/histogram; pool only for
flywheel scoring
**Project Type**: single project — extends `tools/conveyor/`
**Performance Goals**: full generation pass ≈ one histogram pass (~4 min
measured); two-pass generation under 10 min; flywheel cycle adds negligible
daemon overhead
**Constraints**: byte-stable generation (sorted emission, stamped inputs);
zero redefinition errors across the population; append-only evidence (no
resubmit of scored targets); 005 invariants untouched (firewall, SC-005
body identity, determinism)
**Scale/Scope**: 688 derivable targets, ~1,100 declarations expected;
~3 touched modules + 1 new; 0 schema migrations

## Constitution Check

| Principle | Status | Notes |
|---|---|---|
| I. Matching First | PASS | Generated prototypes are seed scaffolding; score 0 remains the only match claim, and the `partial_decomp` bucket *strengthens* honesty by refusing to count error-placeholder seeds as compiled. |
| II. Arcade Source as Rosetta Stone | PASS | Untouched; hand-curated arcade-provenance context keeps precedence over generated declarations by contract. |
| III. Progressive Disclosure | PASS | The whole feature is progressive: wrong-but-compilable prototypes are acceptable because the score arbitrates; evidence accumulates unattended. |
| IV. Platform-Aware Portability | PASS | No source-tier changes. |
| V. Documentation as Artifact | PASS | Generation stamp records evidence inputs; histogram diff makes context investments attributable (FR-010). |
| 001–005 conventions | PASS | Stdlib-only, no new job types, no migrations, deterministic artifacts, firewall untouched. |

No violations; Complexity Tracking not needed.

## Project Structure

### Documentation (this feature)

```text
specs/006-prototype-flywheel/
├── plan.md, research.md, data-model.md, quickstart.md
├── contracts/
│   ├── declaration-layer.md    # generation, precedence, two-pass, stamp
│   └── hygiene-and-flywheel.md # artifact rules, partial_decomp, priorities
└── tasks.md                    # /speckit.tasks output
```

### Source Code (repository root)

```text
tools/conveyor/pipeline/
├── protos.py        # NEW: generate|stamp commands; signature capture,
│                    #   precedence filter, two-pass fixpoint, emission
├── autodecomp.py    # MODIFIED: _context() consumes generated layer last;
│                    #   _clean_m2c saved-reg + scalar-cursor rules;
│                    #   partial_decomp bucket; scoped-run artifact paths
│                    #   (build/m2c_probe.*); `clusters diff` view
└── farm.py          # MODIFIED: flywheel cycle step (priority 60,
                     #   score-evidence dedupe), report coverage line

build/               # regenerated, never committed
├── m2c_protos.h     # the declaration layer
├── m2c_protos.json  # generation stamp + per-callee provenance
├── m2c_histogram.{json,md}   # population instrument (protected)
└── m2c_probe.{json,md}       # scoped probe output (new home)

tests/conveyor/
├── test_protos.py                    # NEW
└── test_autodecomp_population.py     # extended (bucket, hygiene, paths)
```

**Structure Decision**: single project, 005 conventions; the generator is a
sibling pipeline module because it is a standalone deterministic producer
consumed by `_context()`, exactly as `disasm.py` is for derived asm.

## Implementation Phases (for /speckit.tasks)

1. **Hygiene + buckets first** (autodecomp): saved-reg rule, scalar-cursor
   generalization, `partial_decomp`, probe-path separation, diff view +
   tests. Gate: histogram deterministic with new bucket; SC-005 green.
   (First because generation pass 1 should run against final hygiene —
   avoids a needless regeneration.)
2. **Generator** (protos.py): signature capture from the shared m2c pass,
   precedence filter, two-pass generation, stamped emission, `_context()`
   wiring + tests. Gate: byte-stable double run; zero redefinition errors
   population-wide; SC-001/SC-002 (compiled ≥200, func_-blockers → 0).
3. **Flywheel** (farm.py): cycle step, priority 60, dedupe, coverage
   reporting. Gate: SC-004/SC-005/SC-006 over one unattended window.
4. **Docs + close-out**: README ops note, quickstart actuals, full suite.
