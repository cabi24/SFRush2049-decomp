# Tasks: Deterministic Function-Matching Pipeline

**Input**: Design documents from `/specs/001-matching-pipeline/`
**Prerequisites**: plan.md, spec.md, research.md, data-model.md, contracts/coordinator-api.md, quickstart.md

**Tests**: included — plan.md specifies pytest unit tests (lease state machine, bundle round-trip, scoring wrapper), contract tests, and a strlen end-to-end smoke integration test.

**Organization**: Phase 2 builds the compute fabric (blocking prerequisite for every story). Stories then layer pipeline stages onto the fabric in priority order.

## Format: `[ID] [P?] [Story] Description`

- **[P]**: parallelizable (different files, no dependency on incomplete tasks)
- **[US#]**: user story from spec.md (US1 matrix, US2 farm, US3 sweeper, US4 clustering, US5 status)

## Phase 1: Setup

**Purpose**: package skeleton and guardrails

- [X] T001 Create package skeleton per plan.md structure: `tools/conveyor/{__init__.py,coordinator/,agent/,bundles/,jobs/,seeds/shim/,builder/,pipeline/,cli.py}` and `tests/conveyor/{unit/,contract/,integration/}` with empty `__init__.py` files
- [X] T002 [P] Add pytest config (`tests/conveyor/conftest.py`, `pytest.ini` section or `pyproject.toml`) and a CI-runnable lint test `tests/conveyor/unit/test_stdlib_only.py` that fails if `tools/conveyor/coordinator/` or `tools/conveyor/agent/` import anything outside the Python stdlib (R9)
- [X] T003 [P] Create permuter library loader `tools/conveyor/jobs/_permuter.py` that adds the vendored `tools/decomp-permuter` (or the toolkit-relative copy at runtime) to `sys.path` and re-exports `Scorer`; verify `Scorer` import works via `tests/conveyor/unit/test_scorer_import.py`

---

## Phase 2: Foundational — Compute Fabric (BLOCKS all user stories)

**Purpose**: coordinator, node agent, bundle format, and the strlen smoke test proving lease → compile → score → result end to end (FR-013/FR-014)

**⚠️ CRITICAL**: no user story work until this phase completes and the smoke test passes

- [X] T004 Implement SQLite schema + migrations in `tools/conveyor/coordinator/db.py`: all tables from data-model.md (node, blob, work_unit, arcade_candidate, n64_target, matrix_entry, flag_registry, cluster, function_status, promotion_record), WAL mode, transaction helper
- [X] T005 [P] Implement content-addressed blob store in `tools/conveyor/coordinator/store.py`: `put(bytes|path) -> sha256`, `get(sha256) -> path`, `has(sha256)`, sha256-named files under data dir, idempotent put
- [X] T006 [P] Implement bearer-token auth in `tools/conveyor/coordinator/auth.py`: token generation at init, constant-time check, `~/.conveyor/token` persistence
- [X] T007 [P] Implement work-unit manifest schema in `tools/conveyor/bundles/manifest.py`: canonical-JSON serialization (sorted keys) so `manifest_sha` is stable, validation per job type, budget fields
- [X] T008 Implement queue/lease engine in `tools/conveyor/coordinator/queue.py`: submit (with `manifest_sha` cache short-circuit per FR-006), lease (priority order, capability match, prefer cached toolkit), heartbeat extension with optional progress persistence (best_score/best_source), expiry sweep → PENDING attempt+1 or FAILED past max_attempts, idempotent first-result-wins, cancel; state machine exactly per data-model.md
- [X] T009 Write unit tests for the lease state machine in `tests/conveyor/unit/test_queue.py`: lease/heartbeat/expiry/re-issue, duplicate result discarded, cache short-circuit, capability pinning, cancel-at-heartbeat
- [X] T010 Implement coordinator HTTP server in `tools/conveyor/coordinator/server.py` (`ThreadingHTTPServer`, stdlib only) wiring all endpoints from contracts/coordinator-api.md: `POST /work/lease`, `POST /work/{id}/heartbeat`, `POST /work/{id}/result`, `GET /blobs/{sha}`, `POST /blobs`, `POST /work`, `POST /work/{id}/cancel`, `GET /work/{id}`, `GET /nodes`, `GET /healthz`, plus `GET /agent/node_agent.py` self-serve; 401/404/409/422/503 error model
- [X] T011 Write contract tests in `tests/conveyor/contract/test_coordinator_api.py`: start a real coordinator on a random port, exercise every endpoint against the documented request/response shapes including auth failures and lease-conflict 409s
- [X] T012 Implement single-file stdlib-only node agent in `tools/conveyor/agent/node_agent.py`: lease loop with 204 backoff, toolkit fetch+sha256-verify+cache at `--cache` dir, scratch-dir job execution via `python3 <toolkit>/jobs/runner.py`, background heartbeat thread forwarding progress from a `progress.json` the runner updates, result upload, cleanup, `--capabilities`/`--repo` flags for the builder role, graceful abort on `action:cancel`
- [X] T013 [P] Implement toolkit bundle builder in `tools/conveyor/bundles/build_toolkit.py`: packs IDO recomp out/ dir, `mips-linux-gnu-objdump` (+ needed shared libs via `ldd` copy), `tools/decomp-permuter` source, `tools/conveyor/jobs/` executor code, shim headers; writes manifest.json with entry point and arch; deterministic tar (sorted names, zeroed mtimes) so identical inputs → identical sha
- [X] T014 [P] Implement job bundle builder in `tools/conveyor/bundles/build_job.py`: packs manifest + `inputs/` files, returns `(bundle_path, manifest_sha)`; round-trip unit test in `tests/conveyor/unit/test_bundles.py`
- [X] T015 Implement job runner dispatch in `tools/conveyor/jobs/runner.py`: unpack job dir, dispatch by `job_type`, write `progress.json` for the agent's heartbeats, pack `results.tar.gz` with the `result.json` envelope from the contract
- [X] T016 Implement scoring wrapper in `tools/conveyor/jobs/scoring.py`: construct permuter `Scorer` against a target `.o` using toolkit-bundled objdump, `score(cand_o) -> int`; unit test with two tiny fixture objects in `tests/conveyor/unit/test_scoring.py`
- [X] T017 Implement `compile_score` executor in `tools/conveyor/jobs/compile_score.py`: for each (candidate source × flagset × target) cell — compile with toolkit IDO, score via scoring.py, record `ok|fail:<reason>` per cell; emit contract payload
- [X] T018 Implement CLI core in `tools/conveyor/cli.py`: `serve` (coordinator), `publish-toolkit` (upload + pin current toolkit sha in DB), `nodes`, `submit` (raw job JSON, for testing), `smoke --function strlen`
- [ ] T019 Implement the strlen smoke path for `cli smoke`: build a compile_score job from `src/libc/string.c` strlen + its target object at `-O2 -mips2 -G 0`, submit, wait, assert score 0; the strlen target `.o` is produced ad hoc by assembling its extracted target asm with `mips-linux-gnu-as` (no dependency on T022); integration test wrapper in `tests/conveyor/integration/test_smoke_strlen.py` (skipped unless a node is attached; documented in quickstart)

**Checkpoint**: coordinator on Pi + one node (watchman) + `cli smoke` returns score 0 → fabric proven; user stories may start

---

## Phase 3: User Story 1 — Arcade Match Matrix (P1) 🎯 MVP

**Goal**: ranked arcade-ancestor candidates for all 752 targets, deterministic and incremental (FR-001..FR-006)

**Independent Test**: matrix run completes; hand-confirmed pairings (game_loop↔`game()`, `dll_remove`, `dll_init`) rank top; re-run with unchanged inputs is byte-identical and cache-fast

- [X] T020 [P] [US1] Create initial compatibility shim `tools/conveyor/seeds/shim/conveyor_shim.h`: neutral typedefs for arcade/GUTS/3dfx types, stub externs, macro guards — enough for the portable tier (game/*.c) to compile in isolation (R8)
- [X] T021 [P] [US1] Implement arcade candidate extractor `tools/conveyor/seeds/extract_candidates.py`: pycparser walk of `reference/repos/rushtherock/` (preprocessed against the shim) with regex/brace-matching fallback for rejected files; write ArcadeCandidate rows (candidate_id, name, src_file, body_sha); arcade tree is read-only
- [X] T022 [P] [US1] Implement N64 target inventory loader `tools/conveyor/pipeline/targets.py`: populate N64Target rows for both populations (752 from `build/game_code_disasm.txt`/symbol file, 228 static from `symbol_addrs.us.txt`), and produce per-target target objects for the Scorer by assembling extracted target asm (`mips-linux-gnu-as`) into per-function `.o` blobs stored in the CAS
- [X] T023 [US1] Implement matrix orchestration `tools/conveyor/pipeline/matrix.py`: `extract` (run T021 + submit candidate compile jobs per flagset, ingest compile_status → FR-002 coverage report), `submit` (batch compile_score cells 100–1,000/job with priority, skip cells already in MatrixEntry for current toolkit_sha), `ingest` (result payloads → MatrixEntry rows in transactions)
- [X] T024 [US1] Implement rankings + no-ancestry flagging in `tools/conveyor/pipeline/matrix.py`: per-target candidate ranking with stable `(score, candidate_id)` tie-break (FR-005), `no_ancestry` flag on FunctionStatusRecord when best score exceeds configurable threshold (FR-004), status transition unmatched → candidate_identified; define "high-confidence" operationally for SC-001 (best score ≤ confidence threshold AND ≥ configurable margin below the runner-up)
- [X] T025 [P] [US1] Add `cli matrix-report`: candidate compile coverage by arcade file, per-target best candidates, high-confidence count vs SC-001 target, no-ancestry list, cells-done/remaining
- [X] T026 [US1] Write regression validation `tests/conveyor/integration/test_known_pairings.py`: after a matrix run covering the relevant files, assert hand-confirmed pairs from CLAUDE.md (game_loop↔game/game.c:game(), func_8000C050↔dll_remove, func_8000C090↔dll_init) appear in top-5 rankings (SC-003)

**Checkpoint**: `matrix extract && matrix submit` runs unattended; rankings queryable; MVP delivered

---

## Phase 4: User Story 2 — Permuter Farm (P2)

**Goal**: prioritized parallel searches with checkpointed progress, and gated promotion into the repo (FR-009/FR-010)

**Independent Test**: queue known near-matches (hand seeds acceptable — no US1 dependency for testing); jobs run in parallel, one reaches score 0, gets promoted with ROM SHA-1 intact

- [X] T027 [P] [US2] Implement `permuter_search` executor in `tools/conveyor/jobs/permuter_search.py`: drive vendored permuter as a library on the job's seed+target with `--stop-on-zero` semantics, wall/iteration budgets from manifest, PRNG seed recorded, write best-so-far source+score into `progress.json` on every improvement (≤30 s loss guarantee)
- [X] T028 [P] [US2] Implement seed builder `tools/conveyor/pipeline/seeds.py`: turn (candidate source + shim + pinned flags + target `.o`) into a permuter-format job dir (base.c, target.o, compile.sh referencing toolkit IDO) and bundle it via build_job.py
- [X] T029 [US2] Implement farm daemon `tools/conveyor/pipeline/farm.py`: maintain N queued search jobs from rankings (closest-to-match first), re-prioritize on new matrix/cluster data, transition seeded → in_search, harvest wins → status matched, budget exhaustion → seeded + human_flag=stalled, submit `verify_promote` for matched functions
- [X] T030 [US2] Implement builder executor `tools/conveyor/builder/verify_promote.py` (runs on watchman via `builder` capability, serialized queue depth 1): apply winning source to repo clone, `make COMPILER=ido` full build, SHA-1 gate, inject Constitution-V doc header (arcade equivalent, flagset, score provenance, date), commit to work branch on success, `git checkout` rollback + flag on failure; emit contract payload
- [X] T031 [US2] Wire promotion bookkeeping in `tools/conveyor/pipeline/farm.py` + `db.py`: PromotionRecord rows, matched → verified only with `outcome=promoted` (data-model validation rules), rollback → seeded + human_flag
- [X] T032 [P] [US2] Add farm CLI verbs to `tools/conveyor/cli.py`: `seed <target> <file.c>` (manual seed, FR-015 override), `best <target>` (dump best-so-far source/score), `attention` (stalled + no_ancestry queue)
- [X] T033 [US2] Write integration test `tests/conveyor/integration/test_farm_promote.py`: seed a near-match fixture, run a budgeted search on an attached node, assert progress heartbeats persisted, win harvested, verify_promote gate executed (against a scratch clone), PromotionRecord written

**Checkpoint**: conveyor runs end to end — seed → search → match → verified commit (SC-004 path live)

---

## Phase 5: User Story 3 — Compiler Flag Sweeper (P3)

**Goal**: per-TU flags pinned automatically into a registry all stages consume (FR-007)

**Independent Test**: sweeper on `src/libc/string.c` and `src/libultra/os_message.c` reproduces the hand-confirmed `-O2`/`-O1` results (SC-005)

- [X] T034 [P] [US3] Define the flag lattice in `tools/conveyor/pipeline/flags.py`: plausible variations of the confirmed baseline (`-g0 {-O1|-O2} -mips2 -G 0 -non_shared`, per docs/COMPILER_SETTINGS.md) — bounded, not open-ended
- [X] T035 [US3] Implement `flag_sweep` executor in `tools/conveyor/jobs/flag_sweep.py`: compile one TU's functions at every lattice point, aggregate score vs targets, emit contract payload
- [X] T036 [US3] Implement sweep orchestration `tools/conveyor/pipeline/sweep.py`: `submit --unmatched-only`, ingest rankings → FlagRegistryEntry (source=sweep), never overwrite `manual_override` rows (FR-015); make build_job.py/seeds.py consume pinned flags automatically
- [X] T037 [P] [US3] Add `cli pin-flags <tu> "<flags>"` manual override verb and sweep results to `cli report`; validation test `tests/conveyor/integration/test_sweep_known_files.py` asserting confirmed files reproduce (SC-005)

**Checkpoint**: flag registry populated; downstream compiles all use pinned flags

---

## Phase 6: User Story 4 — Near-Clone Clustering (P4)

**Goal**: matched functions automatically unlock their near-clone siblings (FR-008)

**Independent Test**: clustering over 752 targets groups known families; marking one member matched raises sibling priority with the matched source as seed

- [X] T038 [P] [US4] Implement opcode n-gram fingerprinting in `tools/conveyor/jobs/cluster_score.py`: deterministic fingerprint per target from its asm, Jaccard pre-filter, full Scorer scoring only for pairs above threshold (R5 — ~50× cost cut)
- [X] T039 [US4] Implement clustering orchestration `tools/conveyor/pipeline/cluster.py`: two-stage submit (fingerprint batch, then full-score batch), ingest → Cluster rows + N64Target.cluster_id, deterministic cluster assignment
- [X] T040 [US4] Add sibling-seeding hook to `tools/conveyor/pipeline/farm.py`: on transition to matched, seed unmatched cluster siblings from the winner's source and raise their priority (FR-008); unit test the hook in `tests/conveyor/unit/test_sibling_seeding.py`

**Checkpoint**: one match fans out to its family automatically

---

## Phase 7: User Story 5 — Match Status Tracking (P5)

**Goal**: authoritative per-function state + fast roll-ups steering the whole conveyor (FR-011/FR-012)

**Independent Test**: after any stage runs, `cli status` counts reconcile exactly with that stage's outputs; response <10 s (SC-006)

- [X] T041 [US5] Consolidate all state transitions into `tools/conveyor/pipeline/status.py`: single transition function enforcing the FunctionStatusRecord state machine + validation rules from data-model.md; refactor matrix/farm/sweep ingest paths to route through it in the same transaction as result ingest
- [X] T042 [US5] Implement `GET /status` roll-up in `tools/conveyor/coordinator/server.py` per the contract (per-status counts, human-attention queue, queue depths, node liveness), backed by indexed queries meeting <10 s (SC-006)
- [X] T043 [P] [US5] Finalize reporting CLI in `tools/conveyor/cli.py`: `status` (roll-up), `report` (full progress incl. match %, recent activity, human-attention ranked list), `pair <target> <candidate>` override verb (FR-015)
- [X] T044 [US5] Write reconciliation test `tests/conveyor/unit/test_status_reconciliation.py`: simulated stage events → counts always equal event-derived truth; overrides survive automated re-runs

**Checkpoint**: pipeline is fully observable and steerable

---

## Phase 8: Polish & Cross-Cutting

- [X] T045 [P] Add systemd unit examples + operational docs in `tools/conveyor/README.md` (coordinator on Pi, agent on nodes, token handling, backup of `~/.conveyor`)
- [X] T046 [P] Add determinism audit test `tests/conveyor/integration/test_determinism.py`: resubmit an identical batch job → `cached_result` short-circuit; identical inputs → identical result sha (FR-005/FR-006)
- [X] T047 Integrate conveyor stats into existing progress tooling: extend `tools/progress.py` / `docs/PROGRESS_REPORT.md` generation with verified-match percentage from FunctionStatusRecord
- [ ] T048 Walk quickstart.md end to end on real hardware (Pi + watchman + one extra node), fix drift between docs and behavior, record actual matrix throughput vs SC-002 budget, and run a 24 h unattended farm soak with zero human/model interventions as the FR-016 acceptance check

---

## Dependencies & Execution Order

- **Phase 1 → Phase 2 → everything**: the fabric (T004–T019) blocks all stories
- **Story order**: US1 (P1) → US2 (P2) → US3 (P3) → US4 (P4) → US5 (P5) is the delivery order, but:
  - US2 is *testable* without US1 (manual seeds) — only its steady-state value depends on US1 rankings
  - US3 depends on fabric only (uses compile_score machinery from T017)
  - US4 depends on fabric + farm hook (T040 needs T029)
  - US5 refactors ingest paths, so it lands after US1/US2 exist (T041 touches matrix.py/farm.py)
- Within Phase 2: T004 → T008 → T010 → T011; T005/T006/T007 parallel after T001; T012–T017 parallel after T007/T008; T018/T019 last
- `manifest_sha` caching (T008) is what makes matrix re-runs incremental — no separate incrementality work in US1

## Parallel Execution Examples

- **Phase 2 kickoff**: T005, T006, T007 in parallel after T004; then T012, T013, T014, T016 in parallel
- **US1**: T020, T021, T022 fully parallel (shim, extractor, target loader touch different files)
- **US2**: T027, T028, T032 parallel; T029–T031 serialize (same file farm.py)
- **Cross-story**: once Phase 2 lands, US3 (T034–T037) can proceed in parallel with US1 by a second contributor — no shared files except cli.py (merge-friendly verbs)

## Implementation Strategy

**MVP = Phase 1 + Phase 2 + US1** (T001–T026): a proven fabric plus the ranked match matrix — immediately valuable even before any automated searching, because it answers "which arcade function is this?" for every target. Ship that, validate SC-001/SC-003 numbers on real data, then layer US2 (the farm) to start converting rankings into verified matches, then US3–US5.

**Total**: 48 tasks — Setup 3, Foundational 16, US1 7, US2 7, US3 4, US4 3, US5 4, Polish 4.
