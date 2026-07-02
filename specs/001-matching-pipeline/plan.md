# Implementation Plan: Deterministic Function-Matching Pipeline

**Branch**: `001-matching-pipeline` | **Date**: 2026-07-02 | **Spec**: [spec.md](./spec.md)
**Input**: Feature specification from `/specs/001-matching-pipeline/spec.md`
**Design focus** (per user direction): the distributed compute fabric (FR-013/FR-014) — pull-based coordinator, work-unit bundle format, job leasing, content-addressed caching on ephemeral nodes, minimal node agent, centralized verify/promote builder.

## Summary

Replace manual function-matching with an automated conveyor: deterministic seed generation (arcade match matrix, flag sweeper, clone clustering) feeding automated finishing (parallel permuter searches), with per-function status tracking. All heavy compute (IDO compilation, scoring, permuting) runs on a **pool of trusted x86-64 LAN nodes** that pull fully self-contained work units from a coordinator on the Pi 5. Nodes are ephemeral: they hold no project state, cache immutable toolkit bundles by content hash, and can join/leave freely. One designated builder (watchman) holds canonical repo state and performs the verify/promote step that gates every match into the codebase.

Technical approach: a small custom coordinator (Python stdlib: `http.server` + `sqlite3`) rather than reusing `pah.py`'s network stack (built for untrusted volunteers — NaCl crypto, vouching, Docker — wrong trust model, heavy deps). The decomp-permuter is reused **as a library**: its `Scorer` provides the canonical scoring function for all job types, and its permutation engine runs inside search jobs on nodes.

## Technical Context

**Language/Version**: Python 3.9+ (Pi 5 orchestrator and nodes; no syntax above 3.9 so stock distro Pythons work)
**Primary Dependencies**: Python stdlib only for coordinator and node agent (`http.server`, `sqlite3`, `tarfile`, `hashlib`, `json`, `urllib`). On compute nodes: decomp-permuter (vendored in repo, used as library), IDO via ido-static-recomp (shipped in toolkit bundle), mips binutils `objdump` (shipped in toolkit bundle). `pycparser` (already a permuter dependency) for arcade function extraction.
**Storage**: SQLite (WAL mode) on the Pi for all pipeline state — single-writer, queried by CLI/report tools. Content-addressed blob store (sha256-named files on disk, served over HTTP) for bundles, toolkits, and results.
**Testing**: pytest for unit tests (lease state machine, bundle round-trip, scoring wrapper); integration test = single-node smoke run against a known-matching function (e.g., `strlen`).
**Target Platform**: Coordinator/CLI: Linux ARM64 (Pi 5). Node agent + jobs: Linux x86-64 (IDO constraint). Builder: watchman (x86-64, canonical repo checkout).
**Project Type**: Single project — new `tools/conveyor/` package inside the existing repo.
**Performance Goals**: Full matrix (~2,500 arcade candidates × 752 targets × 3 flag sets) in <12 h on ≥20 cores (SC-002); incremental re-run <1 h; status query <10 s (SC-006); coordinator idle overhead negligible on Pi.
**Constraints**: Pi cannot execute IDO or mips objdump output paths — orchestrator never compiles or scores, only coordinates. Nodes hold no project checkout (FR-013). Trusted LAN only; shared-token auth, no TLS. All state must survive coordinator restart (FR-013); nodes may vanish mid-job (lease re-issue).
**Scale/Scope**: ~980 tracked functions; ~5.6 M matrix cells worst case (heavily pruned by fingerprint pre-filter); pool of 1–10 nodes / 4–64 cores; job throughput target ~50 compile+score ops/sec/pool at 20 cores.

## Constitution Check

*GATE: evaluated against Constitution v1.0.0 (2025-12-06). Re-checked after Phase 1 design — PASS.*

| Principle | Compliance |
|---|---|
| I. Matching First | Reinforced: the pipeline's promotion gate (FR-010) mechanizes "byte-identical or it doesn't merge". Permuter score 0 = identical assembly; full-build SHA-1 verification before promotion. |
| II. Arcade Source as Rosetta Stone | Operationalized: the match matrix is Principle II turned into a batch job. Arcade tree is read-only input (spec assumption); candidates are extracted copies. |
| III. Progressive Disclosure | Preserved: pipeline runs alongside `NON_MATCHING` builds; stalled searches return functions to the human queue rather than blocking; progress tracked continuously (FR-011/012). |
| IV. Platform-Aware Portability | Respected: no-ancestry flagging (FR-004) is the mechanical detection of rewrite-tier code; matrix coverage report documents which modules have arcade ancestry. |
| V. Documentation as Artifact | **Design obligation**: `promote.py` MUST inject the required doc header (arcade equivalent, flags used for match) into promoted source — a match without documentation fails the quality gate. Carried into Phase 1 design. |
| Infrastructure PR gates | This is an infrastructure change: no modification to existing build workflow (`make VERSION=us` untouched); all new code under `tools/conveyor/`; tool changes documented in quickstart + this plan. |

**Violations**: none. Complexity Tracking table not required.

## Project Structure

### Documentation (this feature)

```text
specs/001-matching-pipeline/
├── plan.md              # This file
├── research.md          # Phase 0 output — fabric design decisions
├── data-model.md        # Phase 1 output — entities & state machines
├── quickstart.md        # Phase 1 output — bring-up guide
├── contracts/
│   └── coordinator-api.md   # Phase 1 output — HTTP API contract
└── tasks.md             # Phase 2 output (/speckit.tasks — NOT created by /speckit.plan)
```

### Source Code (repository root)

```text
tools/conveyor/
├── __init__.py
├── coordinator/               # Runs on Pi 5 (ARM-safe: stdlib only)
│   ├── server.py              # ThreadingHTTPServer, JSON API (see contracts/)
│   ├── db.py                  # SQLite schema, migrations, WAL setup
│   ├── queue.py               # Job queue: priority, leasing, expiry, re-issue
│   ├── store.py               # Content-addressed blob store (sha256 → file)
│   └── auth.py                # Shared-token check
├── agent/
│   └── node_agent.py          # SINGLE FILE, stdlib-only — copy to any x86-64 box and run
├── bundles/
│   ├── manifest.py            # Work-unit manifest schema + validation
│   ├── build_toolkit.py       # Packs IDO + mips objdump + permuter lib + shims → toolkit bundle
│   └── build_job.py           # Packs per-job bundle (references toolkit by sha256)
├── jobs/                      # Job executors — shipped inside toolkit, run on nodes
│   ├── runner.py              # Entry point: unpack, dispatch by job type, pack results
│   ├── compile_score.py       # Batch: compile candidate(s) at flagset(s), score vs target(s)
│   ├── flag_sweep.py          # Batch: one TU × all flag combos
│   ├── cluster_score.py       # Batch: target×target similarity fingerprints/scores
│   └── permuter_search.py     # Long-running: drives decomp-permuter as library w/ budget
├── seeds/
│   ├── extract_candidates.py  # pycparser walk of arcade tree → candidate inventory
│   └── shim/                  # Compatibility headers so arcade functions compile in isolation
├── builder/
│   └── verify_promote.py      # Runs on watchman: apply source, full build, SHA-1 gate,
│                              #   inject doc header (Constitution V), commit or rollback
├── pipeline/
│   ├── matrix.py              # Submits matrix job batches, ingests results → DB
│   ├── sweep.py               # Submits flag-sweep batches → flag registry
│   ├── cluster.py             # Submits clustering batches → cluster table
│   ├── farm.py                # Maintains search-job queue from matrix + cluster state
│   └── status.py              # FunctionStatus state machine transitions
└── cli.py                     # `conveyor` command: serve, submit, status, nodes, report

tests/conveyor/
├── unit/                      # queue/lease logic, manifest round-trip, db transitions
├── contract/                  # API contract tests against a live local coordinator
└── integration/               # single-node smoke: strlen end-to-end (compile→score→0)
```

**Structure Decision**: single new package `tools/conveyor/` in the existing repo, mirroring the existing convention that tooling lives under `tools/`. The node agent is deliberately one dependency-free file so ephemeral nodes bootstrap with `curl … | python3` and nothing else. Job executors live in the toolkit bundle (versioned by content hash), so node behavior updates by publishing a new toolkit — never by touching nodes.

## Fabric Design (core of this plan)

### Roles

```text
┌────────────── Pi 5 (ARM) ───────────────┐
│ coordinator: queue + SQLite + blob store │◄────── cli.py / reports (also on Pi)
└───────┬──────────────────────────▲───────┘
        │ HTTP pull (lease)        │ HTTP push (results, heartbeats)
┌───────▼───────┐  ┌───────────────┴┐  ┌──────────────┐
│ node (x86-64) │  │ node (x86-64)  │  │ watchman     │
│ agent+toolkit │  │ agent+toolkit  │  │ agent+toolkit│
│ scratch only  │  │ scratch only   │  │ + repo clone │
└───────────────┘  └────────────────┘  │ capability:  │
   ephemeral OK       ephemeral OK     │  "builder"   │
                                       └──────────────┘
```

- **Coordinator (Pi)**: owns all state. Never runs IDO or scoring (the x86-only toolchain); light cross-binutils use (e.g., assembling target asm with `mips-linux-gnu-as`, which runs fine on ARM) is permitted. Serves blobs, leases jobs, ingests results, drives status transitions.
- **Compute node**: stateless. Pulls work matching its capabilities (`x86_64`), executes in a scratch dir, posts a result bundle, deletes scratch. Caches toolkit bundles by sha256 (immutable → cache never invalidates).
- **Builder (watchman)**: an ordinary node that additionally advertises capability `builder` and keeps a canonical repo clone. Verify/promote jobs are pinned to `builder`. It is the only place repo state and pipeline meet.

### Work-unit lifecycle (leasing)

`PENDING → LEASED → (RUNNING via heartbeats) → DONE | FAILED | EXPIRED→PENDING`

- Lease TTL default 120 s, extended by heartbeat (every ~30 s). Long-running permuter jobs heartbeat with progress payloads (current best score, iteration count) — the coordinator persists best-so-far, so even a node that dies mid-search loses at most one heartbeat interval of progress.
- Expired lease → job re-issued with `attempt+1`. Results are idempotent: keyed by `(job_id)`, first completed result wins, duplicates discarded (identical by determinism, verified by content hash).
- Batch jobs (matrix cells, sweeps, clustering) are pure functions of their inputs: result cache key = `sha256(manifest)` — a re-submitted identical job returns the cached result without executing (FR-006 incrementality falls out of this for free).

### Bundle format

Two layers, both tar.gz with `manifest.json`:

1. **Toolkit bundle** (~50–100 MB, fetched once per node, cached forever): IDO recomp binaries, mips binutils objdump, the decomp-permuter checkout, shim headers, `jobs/` executor code, pinned Python entry point. Identified by sha256; a job states `"toolkit": "<sha256>"`.
2. **Job bundle** (KBs): job type, inputs (C source(s), target object(s)/asm, flag sets, budgets), scoring config, toolkit reference. Self-contained given the toolkit — **no project paths, no repo access** (FR-013).

### pah.py decision (research summary — full analysis in research.md)

Reuse the permuter **engine**, not its network. `src/net/` is ~3 k lines designed for untrusted volunteers (PyNaCl encryption/vouching, Docker sandboxing) — wrong trust model, heavyweight deps, and it only handles permuter jobs, not our batch compile+score/sweep/cluster types. `src/scorer.py` (`Scorer.score(cand_o) → (int, str)`) and the permutation engine are imported directly by job executors. One scoring implementation everywhere = comparable scores across matrix, sweeps, clusters, and searches.

### Failure & determinism posture

- Coordinator restart: SQLite WAL + journaled queue state; leases resume or expire naturally.
- Node vanishes: lease expiry re-issues; no double-count (idempotent results).
- Builder unreachable: verify/promote jobs queue; searches/scoring continue (edge case per spec).
- Determinism: every result records `(toolkit_sha, manifest_sha)`; identical inputs → identical outputs; ranking ties broken by candidate name then file (stable, FR-005).
- Trust: shared bearer token on all API calls; LAN only; no TLS (documented assumption).

## Phase 0 → research.md

All Technical Context unknowns resolved; decisions with rationale and alternatives recorded in [research.md](./research.md).

## Phase 1 → data-model.md, contracts/, quickstart.md

- [data-model.md](./data-model.md): entities (Node, WorkUnit, Lease, Blob/Toolkit, ArcadeCandidate, N64Target, MatrixEntry, FlagRegistryEntry, Cluster, SearchJob, FunctionStatusRecord, PromotionRecord) with fields, relations, and state machines.
- [contracts/coordinator-api.md](./contracts/coordinator-api.md): the coordinator HTTP API (lease, heartbeat, result, blobs, status, nodes) — the only network contract in the system.
- [quickstart.md](./quickstart.md): bring-up on Pi + first node + smoke test.
