# Implementation Plan: Relocation-Aware Target Assembly

**Branch**: `003-reloc-aware-targets` | **Date**: 2026-07-08 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/003-reloc-aware-targets/spec.md`

## Summary

Assemble static targets from their splat symbol-relative assembly regions
(`glabel`…`endlabel` in `asm/us/*.s`) instead of raw `.word` lists, so target
objects carry real R_MIPS_HI16/LO16/26 relocations and the existing permuter
scorer compares both sides symmetrically — making true score 0 reachable for
functions that reference globals. Safety comes from a per-target round-trip gate
(masked reassembled words must equal masked ROM words, reusing 002's mask logic)
with raw-word fallback. The evidence base stays honest via supersession: target
objects become attributable in evidence cells (manifest→result echo of
`target_o_sha`), and a changed target purges its derived cells so submit
reschedules instead of deduping against stale evidence.

## Technical Context

**Language/Version**: Python 3.9+ (Pi orchestrator + node agent), stdlib only
**Primary Dependencies**: `mips-linux-gnu-as` and `mips-linux-gnu-objdump`
(already installed on the Pi and already used by `targets.py`/`scoring.py`);
vendored decomp-permuter (unchanged); IDO toolkit (unchanged)
**Storage**: SQLite at `~/.conveyor/conveyor.db`; content-addressed blob store
**Testing**: pytest `tests/conveyor -m "not node_required"` (99 local tests today)
**Target Platform**: Pi 5 (extraction, gate, purge — all local) + x86-64 pool
(re-score runs)
**Project Type**: single project — extends `tools/conveyor/`
**Performance Goals**: full target re-extraction (1,131 targets, ~230 static)
completes in under 5 minutes on the Pi; gate adds two objdump calls per static
target
**Constraints**: determinism (identical inputs ⇒ identical target bytes — GNU as
output is deterministic for fixed input/flags); no network; node-side result-cell
change (`target_o_sha` echo) requires one toolkit rebuild; evidence comparable
only within (toolkit, target object)
**Scale/Scope**: 88 static asm files (56 symbol-relative), 19 flagged targets to
upgrade, 12 locked functions as regression canaries; schema deltas: 2 columns on
`n64_target`, 1 column on `matrix_entry`; ~3 touched modules + 1 node-side echo

## Constitution Check

| Principle | Status | Notes |
|---|---|---|
| I. Matching First | PASS | The whole feature exists to make byte-identity provable for non-leaf functions; the gate guarantees targets are never distorted representations of the ROM. |
| II. Arcade Source as Rosetta Stone | PASS | Untouched; this is scoring infrastructure. |
| III. Progressive Disclosure | PASS | Tiered rollout (static now, dynamic later); raw-word fallback keeps every target usable. |
| IV. Platform-Aware Portability | PASS | Not applicable to code tiers; no source changes. |
| V. Documentation as Artifact | PASS | Gate results and tier live in the DB and coverage report; upgraded matches keep 002's provenance discipline. |
| 001/002 conventions | PASS | Stdlib-only, additive idempotent migrations, content-addressed dedupe, node code ships via toolkit. |

No violations; Complexity Tracking not needed.

## Project Structure

### Documentation (this feature)

```text
specs/003-reloc-aware-targets/
├── plan.md, research.md, data-model.md, quickstart.md
├── contracts/
│   ├── target-assembly.md        # region extraction, assembly, round-trip gate
│   └── evidence-supersession.md  # attribution echo, purge, dedupe semantics
└── tasks.md                      # /speckit.tasks output
```

### Source Code (repository root)

```text
tools/conveyor/
├── pipeline/
│   ├── targets.py       # region index of asm/us/*.s; assemble_region(); gate;
│   │                    #   tier/gate_reason recording; purge-on-change
│   ├── matrix.py        # ingest stamps target_o_sha from result cells
│   ├── corpus.py        # report: SC-006 attribution spot-check line (small)
│   └── lock.py          # unchanged (verify already rebundles current target .o)
├── jobs/
│   └── compile_score.py # echo cell["target_o_sha"] from manifest into results
├── coordinator/
│   └── db.py            # migrations: n64_target.{tier,gate_reason},
│                        #   matrix_entry.target_o_sha
tests/conveyor/unit/
├── test_target_assembly.py   # NEW: region parse, gate pass/fail/fallback, determinism
└── test_supersession.py      # NEW: purge-on-change, dedupe reschedule, attribution
```

**Structure Decision**: single project; extraction/gate/purge are Pi-local in
`pipeline/targets.py`; the only node-side change is a one-line echo in
`compile_score.py` (toolkit rebuild — this feature's re-score run absorbs the
recompute cost by design).

## Rollout note

Order matters and quickstart encodes it: (1) land Pi-side code + migrations,
(2) toolkit rebuild + publish (echo change) + smoke, (3) `targets` re-extraction
with gate (purges superseded cells), (4) corpus submit/ingest-results → the 19
flagged targets re-score, (5) `lock verify` all 12 + coverage report. The arcade
matrix refills on its own next submit (shim iteration), not as part of this
feature.
