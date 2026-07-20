# Implementation Plan: Track B Population Closure

**Branch**: `007-population-closure` (off `006-prototype-flywheel`) | **Date**: 2026-07-19 | **Spec**: [spec.md](spec.md)
**Input**: `/specs/007-population-closure/spec.md`

## Summary

Two new deterministic passes plus a re-measure, all reusing shipped gates:

1. **Closure** (`pipeline/closure.py`, new): discover call targets by
   decoding `j`/`jal` **directly from the raw words** of every registered
   extent (opcode 2/3; absolute target = `(word & 0x3FFFFFF) << 2 |
   (pc & 0xF0000000)`) — no objdump/m2c dependency, so discovery works even
   for targets whose decompilation fails. Unknown in-blob targets go
   through the existing 005 gate (`scan_extent` → `function_words` →
   `assemble_words`) and register as `func_<addr>` extracted targets;
   iterate over the newly registered until fixpoint (caps: 10 iterations /
   2000 registrations, cap-hit is an explicit reported outcome).
   Provenance and outcomes land in a durable `build/closure_report.json`;
   `gate_reason='discovered'` marks closure-registered rows (value-level,
   no schema migration — same pattern as 005's `extent_repaired`).
2. **Data symbols** (`pipeline/datasyms.py`, new): scan derived asm for the
   already-tracked formed-address idioms whose effective address is in no
   table; emit `build/m2c_datasyms.json` (evidence: deriving accesses per
   address, widest-access width rule) consumed by `disasm.py` as a merged
   single lookup (hand `GAME_SYMBOLS` wins on collision);
   `symbol_table_sha()` covers hand+generated so caches invalidate.
   Generated externs join the generated declaration artifact (006's
   `protos generate` gains a datasyms section), never `game_types.h`.
3. **Re-measure**: regenerate the declaration layer over the enlarged
   population, run the full histogram (its population denominator and
   `population_complete` reflect the enlarged set automatically — it reads
   `n64_target`), diff against the 006 close-out baseline, let the 006
   flywheel absorb the new compiles. SC-004 retakes ≥200 with the FR-010
   stop rule.

Key design rationale (research inlined — all facts measured this session):
raw-word jal decode is exact and total (no derivation dependency, no
parser); the 005 scanner is the only registration gate that exists and its
oracle discipline transfers; the widest-access width rule is the only
deterministic choice that never under-reads (word > half > byte; integer
over float on same-width conflict — m2c tolerates integer-typed loads);
merged-lookup precedence avoids the two-lookup mid-function disagreement
hazard; caps make fixpoint runaway an explicit outcome instead of a hang.

## Technical Context

**Language/Version**: Python 3.9+ (Pi), stdlib only
**Primary Dependencies**: 005/006 modules (`targets.py` scan/assemble,
`disasm.py`, `protos.py`, `autodecomp.py`, `farm.py`); binutils unchanged
**Storage**: SQLite value-level only (`gate_reason='discovered'`); new
regenerable artifacts `build/closure_report.json`, `build/m2c_datasyms.json`
**Testing**: pytest local suite; new `test_closure.py`, `test_datasyms.py`
**Target Platform**: Pi for everything except flywheel scoring
**Performance Goals**: closure pass < 5 min (raw-word decode over ~700
extents is trivial; registration cost = assemble per new target);
datasyms pass < 5 min; full re-measure ≈ 2 histogram passes + 2 generation
passes (~25 min)
**Constraints**: idempotency (closure second run = zero); byte-stable
generated artifacts; no schema migration; existing supersession semantics
untouched; 005 SC-005 static guard green throughout
**Scale/Scope**: expect ≥135 new targets (open-ended upward: fixpoint may
find more); ~119+ generated data symbols; 2 new modules + 3 touched

## Constitution Check

| Principle | Status | Notes |
|---|---|---|
| I. Matching First | PASS | New targets enter only through the extent gate; evidence rules unchanged; no match claims beyond score 0. |
| II. Arcade Source | PASS | Untouched; placeholder naming defers semantics to matching time. |
| III. Progressive Disclosure | PASS | Placeholders now, names later; stop-rule honesty on SC-004. |
| IV. Portability | PASS | No source-tier changes. |
| V. Documentation as Artifact | PASS | closure_report + datasyms citations + diff attribution are the record. |
| 001–006 conventions | PASS | Stdlib-only, deterministic, generated-artifact precedence discipline, firewall untouched. |

No violations.

## Project Structure

```text
specs/007-population-closure/
├── spec.md, plan.md, tasks.md, checklists/requirements.md
└── contracts/closure-and-datasyms.md

tools/conveyor/pipeline/
├── closure.py       # NEW: discover (raw-word jal decode) → gate → register
│                    #   → iterate; build/closure_report.json
├── datasyms.py      # NEW: formed-address scan → widths → generated table;
│                    #   build/m2c_datasyms.json
├── disasm.py        # MODIFIED: merged symbol lookup (hand wins);
│                    #   symbol_table_sha covers generated layer
├── protos.py        # MODIFIED: emit generated data-symbol externs section
└── (autodecomp/farm untouched — enlarged population flows through 006)

tests/conveyor/test_closure.py, test_datasyms.py   # NEW
```

## Implementation Phases (for tasks)

1. **Closure** (tests → decode+gate+register → fixpoint+report → live run
   with idempotency check). Gate: SC-001 (second run zero; in-blob func_
   blockers vanish after layer regen).
2. **Data symbols** (tests → scan+emit → disasm merged lookup + cache keys
   → protos externs section). Gate: SC-002 (byte-stable; x<addr> class
   eliminated; no hand-table collision).
3. **Re-measure** (layer regen, histogram ×2, diff vs 006 baseline,
   flywheel absorption). Gate: SC-003/SC-004/SC-005; FR-010 stop rule on
   shortfall.
4. **Docs + close-out** (README, quickstart actuals, suite).
