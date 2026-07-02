# Data Model: Deterministic Function-Matching Pipeline

**Feature**: 001-matching-pipeline | **Date**: 2026-07-02
All tables live in one SQLite database (WAL) on the coordinator (Pi). Blobs live in the content-addressed store; tables reference them by sha256.

## Fabric entities

### Node
Ephemeral pool member. Rows are informational (for `conveyor nodes`), never load-bearing — a node that never registers again simply stops appearing.

| Field | Type | Notes |
|---|---|---|
| node_id | TEXT PK | self-generated UUID, persisted in node's cache dir |
| hostname | TEXT | advertisory |
| capabilities | TEXT (JSON list) | e.g. `["x86_64"]`, watchman: `["x86_64","builder"]` |
| cores | INTEGER | advertised parallelism |
| last_seen | TIMESTAMP | updated on any API call |

### Blob
Content-addressed object (toolkit bundle, job bundle, result bundle).

| Field | Type | Notes |
|---|---|---|
| sha256 | TEXT PK | file name in store |
| kind | TEXT | `toolkit` \| `job` \| `result` |
| size_bytes | INTEGER | |
| created_at | TIMESTAMP | |

### WorkUnit
One leasable unit of work. (The spec's "Search Job" entity is not a separate table — it is a WorkUnit with `job_type = permuter_search`; its search-specific fields are `budget`, `best_score`, `best_source_sha`.)

| Field | Type | Notes |
|---|---|---|
| job_id | TEXT PK | UUID |
| job_type | TEXT | `compile_score` \| `flag_sweep` \| `cluster_score` \| `permuter_search` \| `verify_promote` |
| manifest_sha | TEXT | sha256 of canonical manifest JSON — **result cache key** |
| bundle_sha | TEXT FK→Blob | job bundle |
| toolkit_sha | TEXT FK→Blob | required toolkit |
| required_capability | TEXT | `x86_64` (default) or `builder` |
| priority | INTEGER | lower = sooner; farm recomputes for search jobs |
| state | TEXT | see state machine below |
| attempt | INTEGER | increments on re-issue |
| max_attempts | INTEGER | 3 batch / NULL (unlimited) search |
| leased_by | TEXT FK→Node | nullable |
| lease_expires | TIMESTAMP | nullable |
| budget | TEXT (JSON) | `{wall_seconds, iterations}` for search jobs |
| best_score | INTEGER | live, from heartbeat progress (search jobs) |
| best_source_sha | TEXT FK→Blob | best-so-far source snapshot (search jobs) |
| result_sha | TEXT FK→Blob | final result bundle |
| created_at / updated_at | TIMESTAMP | |

**State machine**:
```
PENDING ──lease──► LEASED ──result──► DONE
   ▲                 │ heartbeat (loops, extends lease, may carry progress)
   │                 ├─ lease expired ─► PENDING (attempt+1)   [attempt ≤ max]
   │                 └─ lease expired & attempt > max ─► FAILED
   └── requeue (manual or farm re-prioritization)
CANCELLED reachable from PENDING/LEASED via CLI.
```
Invariants: only one non-expired lease per job; first result wins (idempotent); every DONE row has `result_sha`; a DONE row for the same `manifest_sha` short-circuits new identical submissions (incrementality, FR-006).

## Pipeline entities

### ArcadeCandidate

| Field | Type | Notes |
|---|---|---|
| candidate_id | TEXT PK | `file.c:function_name` |
| name / src_file | TEXT | from extraction |
| body_sha | TEXT | content hash of extracted body — drives incremental re-extraction |
| compile_status | TEXT (JSON) | per flagset: `ok` \| `fail:<reason>` (FR-002 coverage report) |
| object_shas | TEXT (JSON) | per flagset: sha256 of compiled `.o` blob |

### N64Target

| Field | Type | Notes |
|---|---|---|
| target_id | TEXT PK | e.g. `func_800FD464` or named symbol |
| address | INTEGER | RAM address |
| population | TEXT | `extracted` (752) \| `static` (228) |
| target_asm_sha / target_o_sha | TEXT | scoring inputs |
| fingerprint | TEXT | opcode n-gram fingerprint (clustering pre-filter) |
| cluster_id | TEXT FK→Cluster | nullable |

### MatrixEntry
One scored (candidate × target × flagset) cell. ~millions of rows; indexed `(target_id, score)`.

| Field | Type | Notes |
|---|---|---|
| target_id | TEXT FK | |
| candidate_id | TEXT FK | |
| flagset | TEXT | e.g. `-O2 -mips2 -G0` |
| toolkit_sha | TEXT | scores comparable only within a toolkit (FR-005) |
| score | INTEGER | permuter Scorer score; 0 = byte-identical |
| PK | (target_id, candidate_id, flagset, toolkit_sha) | |

Derived view `target_rankings`: per target, candidates ordered by `(score, candidate_id)` — stable tie-break (FR-005). Threshold rule: `best_score > no_ancestry_threshold` ⇒ flag target `no_ancestry` (FR-004).

### FlagRegistryEntry

| Field | Type | Notes |
|---|---|---|
| translation_unit | TEXT PK | repo-relative path |
| pinned_flagset | TEXT | winner |
| evidence | TEXT (JSON) | ranked `[{flagset, aggregate_score}]` |
| source | TEXT | `sweep` \| `manual_override` — overrides survive re-runs (FR-015) |

### Cluster

| Field | Type | Notes |
|---|---|---|
| cluster_id | TEXT PK | |
| member_ids | TEXT (JSON) | N64Target ids |
| similarity | TEXT (JSON) | pairwise scores for members |

Trigger (farm logic, not SQL): member reaches `matched` ⇒ siblings seeded from its source, priority raised (FR-008).

### FunctionStatusRecord
Authoritative per-function pipeline state (FR-011). One row per tracked function (980).

| Field | Type | Notes |
|---|---|---|
| target_id | TEXT PK FK→N64Target | |
| status | TEXT | state machine below |
| best_score | INTEGER | lowest ever achieved |
| best_candidate_id | TEXT | current seed ancestry |
| flagset | TEXT | from registry or override |
| human_flag | TEXT | NULL \| `no_ancestry` \| `stalled` \| `data_layout` |
| override | TEXT (JSON) | manual pairing/flag override (FR-015) |
| updated_at | TIMESTAMP | |

**State machine** (FR-011):
```
unmatched ─matrix─► candidate_identified ─seed built─► seeded ─farm─► in_search
    │                     │ (score below threshold)                     │
    │                     └────────► human_flag=no_ancestry             ├─ score 0 ─► matched ─verify_promote OK─► verified
    └ (manual seed) ─► seeded                                           └─ budget exhausted ─► seeded + human_flag=stalled
matched ─verify fails─► seeded + human_flag (rollback, FR-010)
```

### PromotionRecord
Audit trail for every promotion attempt (FR-010, Constitution V).

| Field | Type | Notes |
|---|---|---|
| promotion_id | TEXT PK | |
| target_id | TEXT FK | |
| source_sha | TEXT FK→Blob | winning source |
| search_job_id | TEXT FK→WorkUnit | provenance |
| build_ok / sha1_ok | BOOLEAN | gate results |
| commit_hash | TEXT | nullable — set on success |
| doc_header_injected | BOOLEAN | Constitution V compliance |
| outcome | TEXT | `promoted` \| `rolled_back:<reason>` |
| created_at | TIMESTAMP | |

## Relationships (summary)

```
Node 1──N WorkUnit (lease)          ArcadeCandidate N──M N64Target (via MatrixEntry)
Blob 1──N WorkUnit (bundle/toolkit/result)
N64Target 1──1 FunctionStatusRecord 1──N PromotionRecord
Cluster 1──N N64Target              FlagRegistryEntry 1──N WorkUnit (compiles use pin)
```

## Validation rules

- A `verify_promote` WorkUnit may only be created from a FunctionStatusRecord in `matched`.
- `FunctionStatusRecord.status = verified` requires a PromotionRecord with `outcome = promoted`.
- MatrixEntry insertions rejected if `toolkit_sha` ≠ current pinned toolkit (stale results logged, not merged).
- Manual overrides (`FlagRegistryEntry.source = manual_override`, `FunctionStatusRecord.override`) are never overwritten by automated stages (FR-015).
- Every state transition is written in the same SQLite transaction as the event that caused it (result ingest, heartbeat, expiry sweep) — crash-consistent by construction (FR-013).
