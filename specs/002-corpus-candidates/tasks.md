# Tasks: Corpus Candidates — search-don't-synthesize matching for library code

**Input**: Design documents from `/specs/002-corpus-candidates/`
**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/, quickstart.md

**Tests**: included — this project's convention (49+ local tests, `pytest tests/conveyor -m "not node_required"`) and quickstart §6 require them.

**Organization**: grouped by user story. US2's *implementation* lives in Foundational (Phase 2) because the single toolkit rebuild must ship it before US1's acceptance run (research D9); US2's phase contains its end-to-end verification.

**Ground rules for the implementer** (from the constitution and 001 conventions):

- Python 3.9 syntax ceiling, stdlib only on Pi and nodes. No new dependencies.
- Never edit the vendored decomp-permuter or the true-score path.
- `tools/conveyor/jobs/` is node-side: changes there do nothing until a toolkit rebuild + `publish-toolkit` (T015). Everything else is Pi-local and live immediately.
- The live pipeline DB (`~/.conveyor/conveyor.db`) holds 281,933 scored cells — migrations must be additive and idempotent; test against a *copy* if in doubt, never wipe it.
- The pre-commit hook runs `lock check`; if it ever fails you touched a matched function — stop and revert, do not re-pin.
- After each task: `python3 -m pytest tests/conveyor -m "not node_required" -q` green before moving on.

## Phase 1: Setup

**Purpose**: shared helper extraction so US1 and the lock use one mechanism

- [x] T001 Create `tools/conveyor/seeds/context.py` by moving `reduced_tu()` and `resolve_headers()` out of `tools/conveyor/pipeline/lock.py`, parameterizing them on `(repo_root, include_dirs)` instead of the hardcoded game-repo constants (`resolve_headers(tu_path, repo, include_dirs)` — search order: includer's dir, then each include dir). Update `lock.py` to import from the new module with the game repo's `("include", "include/PR")` as its arguments. Files: `tools/conveyor/seeds/context.py` (new), `tools/conveyor/pipeline/lock.py`. Acceptance: `pytest tests/conveyor/unit/test_lock.py -q` green unchanged.

---

## Phase 2: Foundational (blocking prerequisites)

**Purpose**: schema + reloc-blind scoring, which US1's toolkit rebuild must ship and US3's flags depend on

- [x] T002 Add idempotent migrations to `tools/conveyor/coordinator/db.py`: `CREATE TABLE IF NOT EXISTS corpus_root (...)` per data-model.md; `PRAGMA table_info`-guarded `ALTER TABLE arcade_candidate ADD COLUMN origin TEXT NOT NULL DEFAULT 'arcade'` and `ADD COLUMN provenance TEXT NOT NULL DEFAULT '{}'`; same-guarded `ALTER TABLE matrix_entry ADD COLUMN score_reloc_blind INTEGER`. Acceptance: connecting twice to a fresh DB and to a copy of a pre-002 DB both succeed and expose the columns.
- [x] T003 [P] Implement `reloc_blind_score(target_o, cand_o)` in `tools/conveyor/jobs/scoring.py` exactly per `contracts/scoring-reloc-blind.md`: split pure functions `_parse_text_words(objdump_stdout) -> list[int]`, `_parse_relocs(objdump_stdout) -> list[(index, mask)]`, and `_masked_diff(t_words, c_words, sites) -> int` so tests need no objdump; the public function shells out to the toolkit objdump (`-d`, `-r`) and composes them. Masks: HI16/LO16 → `0xFFFF0000`, R_MIPS_26 → `0xFC000000`, unknown types → `0xFFFFFFFF` (no normalization creep).
- [x] T004 [P] Write `tests/conveyor/unit/test_reloc_blind.py` covering contract invariants I1–I6 using fixture objdump text (build the osCreateMesgQueue I2 fixture from the real 12-word disassembly recorded in research D4: HI16 at indices 0,1; LO16 at 2,3; words differing only in low 16 bits there ⇒ score 0). Files: `tests/conveyor/unit/test_reloc_blind.py` (new).
- [x] T005 Wire the score into `tools/conveyor/jobs/compile_score.py`: each result cell gains `"score_reloc_blind": reloc_blind_score(...)` iff `compile == "ok"` (same rule as `score`); failed cells carry neither. File: `tools/conveyor/jobs/compile_score.py`.
- [x] T006 Teach `cmd_ingest` in `tools/conveyor/pipeline/matrix.py` to read `cell.get("score_reloc_blind")` into the new `matrix_entry` column (NULL when absent — old blobs must still ingest). Extend an existing ingest-path test in `tests/conveyor/unit/` with a cell carrying the field and one without.

**Checkpoint**: full local suite green; no behavior change visible to arcade flows (SC-006 groundwork).

---

## Phase 3: User Story 1 — Name-paired canonical matching (P1) 🎯 MVP

**Goal**: ingest ultralib with provenance; submit name-paired compile_score jobs; evidence lands in the shared rankings/report path.

**Independent test**: quickstart §§2–4 — register + ingest (idempotent, dirty-refusal), submit ~85 pairings, results queryable; strlen/guMtxIdentF reach true score 0 from corpus source.

- [x] T007 [US1] Implement `register` and `ingest` in new `tools/conveyor/pipeline/corpus.py` per `contracts/cli-corpus.md`: registry upserts into `corpus_root` (reject origin `arcade`, `/`, `.c`; re-register identical = no-op); ingest re-checks `git -C <path> rev-parse HEAD` + `status --porcelain` (refuse dirty/missing/moved unless `--allow-dirty` → record `<sha>-dirty`), extracts via `seeds/extract_candidates.extract_functions`, upserts candidates with the 3-part id grammar, origin, and provenance JSON from data-model.md.
- [x] T008 [P] [US1] Write register/ingest tests in `tests/conveyor/unit/test_corpus.py`: build a throwaway git repo in tmp_path (`git init` + commit one C file with 2 functions); assert id grammar (`<origin>:<rel>:<name>`), provenance fields, ingest idempotence (`0 new` on rerun), dirty-clone refusal exit, origin-name validation, and that arcade rows are untouched (count + a sampled candidate_id unchanged).
- [x] T009 [US1] Implement `submit` in `tools/conveyor/pipeline/corpus.py`: pair every `n64_target.target_id` equal to a corpus candidate's name; per pairing × flagset (default: the two confirmed sets) not already in `matrix_entry` under the pinned toolkit, build a `compile_score` job — source = `seeds/context.reduced_tu()` of the candidate's TU + `resolve_headers()` against the root's registered `include_dirs`, bundled with the target .o from the blob store — priority 50, batched like `matrix submit`, `--dry-run` supported, skip counters (`no_target_o`, `unextractable`) printed.
- [x] T010 [P] [US1] Write pairing/submit tests in `tests/conveyor/unit/test_corpus.py`: tmp DB with fabricated `n64_target` rows + corpus candidates; assert exact-name pairing set (no size window), dedupe against pre-existing `matrix_entry` rows, skip-reason counts, and that `--dry-run` uploads nothing (mirror `test_bundles`/matrix dry-run patterns).
- [x] T011 [US1] Implement `report` in `tools/conveyor/pipeline/corpus.py` (sections 1–5 of `contracts/cli-corpus.md`, including per-target best true/reloc-blind table and `--target` detail) and split `matrix report`'s compile-coverage line by origin in `tools/conveyor/pipeline/matrix.py`.
- [x] T012 [US1] **Toolkit rebuild + acceptance run (node_required, watchman is on)**: follow quickstart §§0–3 exactly — rsync repo to watchman, rebuild + `publish-toolkit` (ships T003/T005 plus the pending 8b51c69 head-capture change), `cli smoke` must PASS with the result cell carrying `score_reloc_blind: 0`; then `corpus register/ingest/submit` for real. Record actual counts (pairings, jobs, minutes) in `specs/002-corpus-candidates/quickstart.md` as measured values. Acceptance: SC-001 (≥80 targets with evidence) and SC-004 (immediate resubmit = 100% cache hits).

**Checkpoint**: US1 delivers standalone value — scored canonical evidence in rankings.

---

## Phase 4: User Story 2 — Relocation-blind score verification (P2)

**Goal**: prove the metric end to end (implementation shipped in Phase 2/T012).

**Independent test**: SC-002 + SC-003 through the real pipeline, no hand disassembly.

- [x] T013 [US2] Run `corpus report --target osCreateMesgQueue` after T012's ingest of results: assert best_true > 0 and best_reloc_blind = 0 (SC-002). Run it for `strlen` and `guMtxIdentF`: both scores 0 (SC-003). Paste the three report excerpts into `specs/002-corpus-candidates/quickstart.md` §4 as the recorded acceptance evidence. If any assertion fails, STOP — the contract fixture (I2) and the live pipeline disagree; debug `scoring.py` parsing against real toolkit objdump output on watchman before proceeding to US3.

---

## Phase 5: User Story 3 — Match artifacts and honest promotion gates (P3)

**Goal**: turn evidence into artifacts + flags without weakening any gate.

**Independent test**: quickstart §§4–5 — flagged targets have provenance-stamped artifacts; nothing non-byte-identical is promoted, locked, or committed.

- [x] T014 [US3] Implement `ingest-results` in `tools/conveyor/pipeline/corpus.py`: run the shared matrix ingest, then recompute `reloc_only_diff` per data-model rules (set only for `unmatched`+non-override targets whose best cell has `score_reloc_blind == 0 AND score > 0`; clear when the condition stops holding; superseded by the normal `candidate_identified` path when true 0 exists), then write/refresh `work/<...>/<target_id>/corpus_match.c` artifacts (locate dir by the `verify_promote` rglob rule, fallback `work/corpus/<target_id>/`) with the exact doc-header format from data-model.md, then run `update_rankings`.
- [x] T015 [P] [US3] Write flag/artifact tests in `tests/conveyor/unit/test_corpus.py`: fabricated cells → flag set + artifact content matches the header contract; true-0 cell → no flag, normal promotion path invoked; flag clears on changed evidence; overridden/already-locked targets never flagged; artifact regeneration overwrites (derived state).
- [x] T016 [US3] **Honesty-gate acceptance (node_required)**: quickstart §5 — after the real ingest-results: `cli report` shows no target moved to matched/verified by this feature alone (SC-007); `pipeline.lock check` green and `matched.lock.json` unchanged; `git status` shows only `work/**/corpus_match.c` artifacts, nothing under `src/`. Record the `reloc_only_diff` count and the flag-summary line in quickstart §5.

---

## Phase 6: Polish & cross-cutting

- [x] T017 [P] Document the corpus loop in `tools/conveyor/README.md` (new "Corpus candidates" operating note: register → ingest → submit → ingest-results → report; dirty-clone rule; artifact semantics; reloc_only_diff meaning) and add the feature summary + commands to the conveyor section of `CLAUDE.md`.
- [x] T018 Verify the failure-clustering path: `python3 -m tools.conveyor.pipeline.matrix failures` distinguishes corpus-origin compile failures (edge case from spec); if ultralib candidates fail to compile, the report must show them without polluting arcade shim-gap analysis — split the histogram by origin if mixed. File: `tools/conveyor/pipeline/matrix.py` (only if a change is needed).
- [x] T019 Full local suite green (`pytest tests/conveyor -m "not node_required" -q`), then commit on `002-corpus-candidates` with the project's commit conventions. Do NOT update the wiki status page — the reviewer handles milestone comms.

---

## Dependencies

```text
T001 ─→ T009 (context helpers)
T002 ─→ T006, T007, T009, T014 (schema)
T003 ─→ T004, T005          T005, T006 ─→ T012 (toolkit must ship them)
T007 ─→ T008, T009 ─→ T010, T011 ─→ T012 ─→ T013 ─→ T014 ─→ T015, T016 ─→ T017–T019
US1 ⊃ T007–T012   US2 ⊃ T013 (impl in Phase 2)   US3 ⊃ T014–T016
```

Parallel opportunities: T003+T004 alongside T002; T008/T010/T015 alongside their implementation siblings; T017 anytime after T014.

## Implementation strategy

MVP = Phases 1–3 (T001–T012): delivers SC-001/SC-004 alone. Phase 4 is a
verification gate, not new code — if T013 fails, fix before US3. Phases 5–6
complete the artifact/flag flow and docs. Node-required tasks (T012, T013, T016)
assume the pool from quickstart §0; everything else is Pi-local and testable
offline.
