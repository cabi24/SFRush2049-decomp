# Phase 0 Research: Distributed Compute Fabric

**Feature**: 001-matching-pipeline | **Date**: 2026-07-02
**Scope**: resolve all design unknowns for the work-distribution fabric (FR-013/FR-014) plus supporting pipeline decisions.

## R1. Reuse decomp-permuter's pah.py (permuter@home) vs. custom coordinator

**Decision**: Do **not** reuse `pah.py`'s network stack. Build a small custom coordinator (stdlib HTTP + SQLite). Reuse the permuter **as a library** inside job executors: `src/scorer.py` for all scoring, the permutation engine for search jobs.

**Rationale** (from direct inspection of `tools/decomp-permuter/src/net/`, ~2,963 lines):
- pah.py's architecture solves *untrusted volunteer* compute: PyNaCl secret-box encryption and a cryptographic vouching system (`cmd/vouch.py`), Docker container sandboxing on servers (`net/server.py` imports `docker`), a central matchmaking controller. Our pool is trusted LAN boxes we own — every one of those mechanisms is dead weight (two non-stdlib deps, Docker required on every node, operational complexity).
- pah.py only distributes *permuter jobs*. Three of our four job types (matrix compile+score, flag sweeps, clustering) are not permuter runs; we'd need a second distribution channel anyway. One custom coordinator handles all four uniformly.
- The valuable, hard-to-rewrite part of the permuter is its scoring and mutation engine, and that part imports cleanly: `Scorer(target_o=…).score(cand_o) -> (score, hash)` with tuned penalty weights (regalloc=5, reordering=60, insertion/deletion=100). Using it everywhere makes matrix scores, sweep scores, and search scores directly comparable — one metric across the whole pipeline (spec assumption honored).

**Alternatives considered**:
- *Adopt pah.py wholesale*: rejected — wrong trust model, Docker+PyNaCl on every node, doesn't cover batch job types.
- *Third-party queue (Celery/Redis, beanstalkd, RQ)*: rejected — external broker to install/operate on the Pi, non-stdlib clients on nodes, conflicts with "ephemeral node = curl one file and run".
- *SSH push (parallel-ssh/fabric from Pi)*: rejected — push model needs node inventory management, per-node state, and rsync'd project trees; exactly what the user wants to eliminate.

## R2. Coordinator transport & state store

**Decision**: `http.server.ThreadingHTTPServer` + JSON bodies; SQLite in WAL mode as the single source of truth; blob store = flat directory of sha256-named files served over the same HTTP port.

**Rationale**: stdlib-only satisfies the Pi/ARM constraint and the "no deps on nodes" constraint symmetrically. Load is trivial for HTTP: even the full matrix is batched (one job = hundreds of cells), so the coordinator sees tens of requests/minute, not thousands/second. SQLite WAL gives crash-safe, single-writer semantics that match the one-coordinator design; every state transition is a transaction, so coordinator restart is a non-event (FR-013 durability).

**Alternatives considered**: FastAPI/uvicorn (needless deps), gRPC (deps + codegen), shared NFS spool directory (no leasing semantics, split-brain on flaky mounts, silent partial writes — fails the "no corrupt results" edge case).

## R3. Job leasing & re-issue semantics

**Decision**: pull-based lease with TTL + heartbeat extension.
- Node → `POST /work/lease` with `{capabilities, cores, max_jobs}`; coordinator returns highest-priority PENDING job matching capabilities, marks it LEASED with `lease_expires = now + 120s`.
- Node heartbeats every ~30 s (`POST /work/{id}/heartbeat`), optionally carrying progress payload (best score so far, iterations). Heartbeat extends the lease.
- Missed heartbeats → lease expires → job returns to PENDING with `attempt += 1`; `max_attempts` (default 3 for batch, unlimited for search jobs since partial progress is preserved) before FAILED.
- Results idempotent: first `POST /work/{id}/result` wins; later duplicates for the same job are acknowledged and discarded. Batch results carry `sha256(manifest)` as a cache key so identical future jobs short-circuit (FR-006).

**Rationale**: pull + lease is the simplest protocol that satisfies "nodes join/leave freely, jobs never lost/double-counted" (FR-013/014). Progress-bearing heartbeats mean a killed 12-hour permuter search loses ≤30 s of progress, which is what makes long searches safe on ephemeral nodes.

**Alternatives considered**: work-stealing without leases (double execution uncontrolled — wasteful but worse, non-idempotent promotion), ZooKeeper/etcd-style coordination (absurd overkill for one coordinator).

## R4. Bundle format & content-addressed caching

**Decision**: two-layer tar.gz bundles, addressed by sha256.
- **Toolkit** (fetched once, cached at `~/.conveyor/cache/<sha256>` on nodes, immutable): IDO recomp binaries (`cc`, `ugen`, etc.), `mips-linux-gnu-objdump` (statically linked or with bundled libs), the vendored decomp-permuter source, shim headers, job-executor code, `manifest.json` (contents, entry point, required host arch).
- **Job bundle** (small): `manifest.json` (job id/type/toolkit-sha/budgets/flagsets), `inputs/` (C sources, target `.o` files or target asm). Executor contract: `python3 <toolkit>/jobs/runner.py --job <dir> --out results.tar.gz`.

**Rationale**: content addressing makes node caches trivially correct (immutable objects, no invalidation problem) and makes toolchain versioning explicit — every result records which toolkit produced it, which is required for the determinism guarantee (FR-005): scores are only comparable within a toolkit hash. Updating node behavior = publishing a new toolkit hash; nodes are never "deployed to".
- Note: `objdump` is a real node-side dependency of the Scorer (it shells out). Bundling it in the toolkit (not requiring apt install) keeps nodes zero-provision. Verify static linkage or ship needed shared libs (task for implementation).

**Alternatives considered**: Docker images (heavier, requires Docker on nodes, and IDO recomp runs fine as plain binaries), rsync'ing the repo (the model being eliminated), pip-installable package (nodes shouldn't need pip or venvs).

## R5. Job taxonomy

**Decision**: four job types, two execution shapes.

| Type | Shape | Contents | Typical size |
|---|---|---|---|
| `compile_score` | batch (pure) | N candidates × M flagsets × K targets; compile each, score each pair | 100–1,000 cells/job |
| `flag_sweep` | batch (pure) | one TU × full flag lattice | 10–50 compiles/job |
| `cluster_score` | batch (pure) | fingerprint or full-score a block of the target×target matrix | 1,000s of comparisons/job |
| `permuter_search` | long-running (budgeted) | one seed + target, iteration/time budget, `--stop-on-zero` | hours |

Batch jobs are deterministic pure functions → cacheable, retry-safe, any node. Search jobs are stochastic but checkpointed via heartbeat progress; seed for the PRNG recorded in results for reproducibility of *reported* wins (a winning source always re-verifies deterministically by recompilation, so search nondeterminism never leaks into pipeline state).

**Cluster pre-filter**: full 752×752 scoring is ~283 k scorings — feasible but wasteful; use a cheap opcode-n-gram fingerprint (computed once per target, on nodes) and only full-score pairs above a Jaccard threshold. Deterministic, and cuts clustering cost ~50×.

## R6. Builder (verify/promote) integration

**Decision**: watchman runs the same node agent plus capability tag `builder`, and holds a canonical repo clone. `verify_promote` is a fifth job type pinned to `builder` capability, but it is **not** bundle-self-contained — it operates on the repo clone by design (it *is* the centralized stateful step). Sequence: apply promoted source → `make COMPILER=ido` full build → SHA-1 gate → inject Constitution-V doc header (arcade equivalent, flags, date, score history) → commit on a work branch → report. Rollback = `git checkout` of touched files on any failure. Promotions are serialized (queue depth 1) to keep repo state linear.

**Rationale**: keeps the fabric's clean stateless model for 99% of work while acknowledging the one inherently stateful operation. Serialization avoids merge races; git provides rollback for free and an audit trail (PromotionRecord ↔ commit hash).

## R7. Security posture

**Decision**: single shared bearer token (generated at coordinator init, placed on nodes at bootstrap) over plain HTTP on the trusted LAN. No TLS, no per-node identity, no sandboxing of job payloads.

**Rationale**: matches the spec's trust assumption (maintainer-owned LAN nodes; internet-scale distribution explicitly out of scope). The token exists to prevent *accidents* (a stray process poking the API), not adversaries. Revisit only if the pool ever leaves the LAN — the bundle/lease design doesn't preclude adding TLS + per-node tokens later.

## R8. Arcade candidate extraction & shim strategy

**Decision**: `pycparser`-based extraction (already a permuter dependency) with a preprocessor pass using the shim headers; fall back to a regex/brace-matching splitter for files pycparser rejects (K&R-isms, arcade compiler extensions). Shim = one curated `conveyor_shim.h` mapping arcade types/macros (3dfx, GUTS types) onto neutral typedefs + stub externs, iterated until candidate-compilation coverage plateaus. Coverage is a first-class report (FR-002) — expected: high coverage for the portable tier (game logic), low for 3dfx rendering (which has no N64 counterpart anyway).

**Rationale**: the shim is the highest-leverage *human/LLM* touchpoint in the whole system (fix one header, hundreds of candidates start compiling), which is exactly where the spec wants intervention concentrated (FR-016).

## R9. Python version & style

**Decision**: Python 3.9 floor, stdlib-only for `coordinator/` and `agent/node_agent.py` (enforced by a CI grep for imports); job executors may use permuter deps since they run inside the toolkit. pytest for tests (dev-only dependency, never needed on nodes).

## Resolved unknowns summary

| Unknown | Resolution |
|---|---|
| pah.py reuse | Engine yes, network no (R1) |
| Transport/state | stdlib HTTP + SQLite WAL + CAS blobs (R2) |
| Lease semantics | TTL 120 s, 30 s heartbeats w/ progress, idempotent results (R3) |
| Bundle format | 2-layer tar.gz, sha256-addressed, toolkit cached forever (R4) |
| Long jobs on ephemeral nodes | progress-bearing heartbeats; ≤30 s loss on node death (R3/R5) |
| Clustering cost | fingerprint pre-filter, then full scoring (R5) |
| Builder integration | node agent + `builder` capability, serialized promotions, git rollback (R6) |
| Security | shared token, plain HTTP, trusted LAN (R7) |
| Candidate extraction | pycparser + shim header, coverage-reported (R8) |
