# Implementation Plan: Corpus Candidates — search-don't-synthesize matching for library code

**Branch**: `002-corpus-candidates` | **Date**: 2026-07-08 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/002-corpus-candidates/spec.md`

## Summary

Add a second candidate corpus (local git clones of canonical library source, first
root: `reference/repos/ultralib`) to the conveyor pipeline, matched to targets by
exact function name instead of the size-window matrix, and add a relocation-blind
secondary score so instruction-identical-modulo-relocations pairings are machine
detectable. Everything reuses the existing job/result/dedupe fabric: corpus pairings
are ordinary `compile_score` jobs whose sources are reduced TUs with the corpus
repo's own headers (the mechanism proven by `pipeline.lock verify`); results land in
`matrix_entry` with one new column. True score-0 keeps the existing promotion/lock
path; reloc-blind-0 produces a provenance-stamped `corpus_match.c` work artifact and
a `reloc_only_diff` flag that ordinary re-scoring upgrades later.

## Technical Context

**Language/Version**: Python 3.9+ (Pi orchestrator and node agent; no syntax above 3.9)
**Primary Dependencies**: Python stdlib only on Pi and nodes (`sqlite3`, `tarfile`,
`hashlib`, `json`, `urllib`, `subprocess`, `difflib`); IDO via ido-static-recomp and
mips binutils `objdump` shipped in the toolkit bundle; vendored decomp-permuter for
the existing true score (unchanged)
**Storage**: SQLite (WAL) at `~/.conveyor/conveyor.db`; content-addressed blob store
**Testing**: pytest, `tests/conveyor` (local marker discipline: `-m "not node_required"`)
**Target Platform**: Pi 5 (coordinator/CLI) + x86-64 LAN nodes (compile/score)
**Project Type**: single project — extends `tools/conveyor/`
**Performance Goals**: full corpus cycle (85 pairings × 2 flagsets) completes in
under 15 minutes of pool time; ingest and report are sub-second local operations
**Constraints**: no network in any pipeline stage; deterministic dedupe preserved
(manifest-sha result cache); results comparable only within one toolkit sha; node-side
code changes ship via toolkit rebuild (new sha)
**Scale/Scope**: +686 corpus candidates (first root), 85 name pairings today; schema
deltas: 1 new table, 2 new columns, 1 new `human_flag` value; 1 new pipeline module;
~4 touched existing modules

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

| Principle | Status | Notes |
|---|---|---|
| I. Matching First | PASS | SC-007/FR-010: nothing lacking true score 0 is promoted, locked, or committed. `reloc_only_diff` is explicitly a non-promotion state. |
| II. Arcade Source as Rosetta Stone | PASS (justified extension) | Applies to game logic. Library targets have no arcade equivalent by definition (permanently `no_ancestry`); canonical SDK source is the correct Rosetta stone for that tier and gets the same provenance discipline. |
| III. Progressive Disclosure | PASS | `reloc_only_diff` is a tracked intermediate state with an automatic upgrade path (FR-011); no work is blocked on the future reloc-assembly feature. |
| IV. Platform-Aware Portability | PASS | Feature targets the rewrite/SDK tier explicitly; arcade-tier flow untouched (SC-006). |
| V. Documentation as Artifact | PASS | Match artifacts carry provenance headers (origin repo, path, commit, flags, both scores) — same convention as verify_promote's doc header. |
| Tech standards (001 conventions) | PASS | Stdlib-only, single-writer SQLite, content-addressed blobs, toolkit-bundled executors all preserved. |

No violations; Complexity Tracking not needed.

## Project Structure

### Documentation (this feature)

```text
specs/002-corpus-candidates/
├── plan.md              # This file
├── research.md          # Phase 0: decisions with rationale
├── data-model.md        # Phase 1: schema deltas, id grammar, artifact format
├── quickstart.md        # Phase 1: executable end-to-end walkthrough
├── contracts/
│   ├── cli-corpus.md    # CLI surface: register/ingest/submit/report
│   └── scoring-reloc-blind.md  # exact algorithm + result schema delta
└── tasks.md             # Phase 2 (/speckit.tasks — not created here)
```

### Source Code (repository root)

```text
tools/conveyor/
├── pipeline/
│   ├── corpus.py        # NEW: register | ingest | submit | ingest-results | report
│   ├── matrix.py        # ingest gains score_reloc_blind passthrough; report gains origin split
│   ├── lock.py          # reduced-TU/header helpers move out (imported back)
│   └── status.py        # unchanged (human_flag is an existing free-text column)
├── seeds/
│   ├── extract_candidates.py  # extractor reused as-is for corpus roots
│   └── context.py       # NEW: reduced_tu() + resolve_headers() shared by lock + corpus
├── jobs/
│   ├── compile_score.py # cells gain score_reloc_blind (node-side, toolkit rebuild)
│   └── scoring.py       # NEW reloc_blind_score(target_o, cand_o) using toolkit objdump
├── coordinator/
│   └── db.py            # migrations: corpus_root table; origin/provenance columns;
│                        #             matrix_entry.score_reloc_blind
tests/conveyor/unit/
├── test_corpus.py       # NEW: ingest idempotence, id grammar, pairing, dirty-clone refusal
├── test_reloc_blind.py  # NEW: masking algorithm on fixture objdump text
└── (existing suites unchanged and green)
work/<...>/<target>/corpus_match.c   # artifacts (generated, per reloc_only_diff target)
```

**Structure Decision**: single project; all changes inside `tools/conveyor/` plus
tests, mirroring 001's layout. Node-visible changes are confined to `jobs/` (ships in
the toolkit); everything else is Pi-local.

## Rollout note (toolkit rebuild)

`jobs/scoring.py` and `jobs/compile_score.py` changes require one toolkit rebuild +
`publish-toolkit` on the builder node. This is batched with the already-pending
stderr head-capture change from commit `8b51c69`. Consequence (existing FR-005/001
rule): the new toolkit sha starts a fresh comparison set; corpus jobs run under it
from day one, and the arcade matrix re-fills on its next submit (planned anyway for
the shim-coverage iteration). SC-006 is evaluated per-toolkit.
