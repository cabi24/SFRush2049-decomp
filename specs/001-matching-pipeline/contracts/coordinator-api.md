# Contract: Coordinator HTTP API

**Feature**: 001-matching-pipeline | **Version**: v1 | **Date**: 2026-07-02
The only network contract in the system. JSON over plain HTTP on the trusted LAN. All endpoints (except `GET /healthz`) require `Authorization: Bearer <token>`. Timestamps are UTC ISO-8601. Binary payloads are tar.gz bundles, `Content-Type: application/gzip`.

Base URL: `http://<coordinator>:8323/api/v1`

## Node-facing endpoints (consumed by node_agent.py)

### POST /work/lease
Node requests work. Body:
```json
{
  "node_id": "uuid", "hostname": "nodeA", "cores": 8,
  "capabilities": ["x86_64"], "cached_toolkits": ["<sha256>", "..."]
}
```
Responses:
- `200` — a job:
```json
{
  "job_id": "uuid", "job_type": "compile_score",
  "bundle_sha": "<sha256>", "toolkit_sha": "<sha256>",
  "lease_expires": "2026-07-02T12:00:00Z",
  "heartbeat_seconds": 30,
  "budget": {"wall_seconds": 14400, "iterations": null}
}
```
- `204` — no matching work (node sleeps `Retry-After` seconds, default 15).

Semantics: coordinator picks the highest-priority PENDING job whose `required_capability ⊆ capabilities`, preferring jobs whose toolkit the node already caches. Registers/updates the Node row as a side effect (no separate registration call).

### POST /work/{job_id}/heartbeat
Extends the lease; optionally reports progress. Body:
```json
{ "node_id": "uuid",
  "progress": { "best_score": 143, "iterations": 52100, "best_source": "<inline gz+b64, optional>" } }
```
Responses: `200 {"lease_expires": "...", "action": "continue"}` — `action` may be `"cancel"` (job cancelled or superseded; node aborts and discards). `409` if the lease is no longer held by this node (node aborts silently).

Progress rule: for `permuter_search`, a `best_score` improvement SHOULD include `best_source`; coordinator persists it (≤30 s progress loss on node death).

### POST /work/{job_id}/result
Body: result bundle (tar.gz) containing `result.json` + artifacts. Query param `?node_id=uuid`.
Responses: `200 {"accepted": true}`; `200 {"accepted": false, "reason": "duplicate"}` (idempotent — first result won); `409` lease mismatch after expiry ⇒ result still accepted if job is PENDING-again or discarded if DONE (never errors the node).

`result.json` common envelope:
```json
{ "job_id": "uuid", "job_type": "...", "manifest_sha": "<sha256>",
  "toolkit_sha": "<sha256>", "exit": "ok|error", "error": null,
  "wall_seconds": 812.4, "payload": { } }
```
Per-type `payload`:
- `compile_score`: `{"cells": [{"candidate_id","flagset","target_id","score","compile":"ok|fail:<reason>"}]}`
- `flag_sweep`: `{"tu": "src/game/game.c", "rankings": [{"flagset","aggregate_score"}]}`
- `cluster_score`: `{"pairs": [{"a","b","score"}], "fingerprints": [{"target_id","fp"}]}`
- `permuter_search`: `{"final_best_score": 0, "iterations": 90210, "seed": 1337, "source_file": "best.c"}` (source in bundle)
- `verify_promote`: `{"build_ok": true, "sha1_ok": true, "commit_hash": "...", "doc_header_injected": true}`

### GET /blobs/{sha256}
Content-addressed fetch (toolkit and job bundles). `200` gzip stream; `404` unknown. Nodes verify sha256 after download; mismatch = discard and re-fetch.

### GET /agent/node_agent.py
Serves the current single-file node agent (auth required). Used at node bootstrap (`curl … -o node_agent.py`) and for manual self-update after a `426` response. `Content-Type: text/x-python`.

### GET /healthz
`200 {"status":"ok","pending":123,"leased":8}` — no auth; for node backoff logic and monitoring.

## Operator-facing endpoints (consumed by cli.py / pipeline modules on the Pi)

### POST /blobs
Upload a bundle. `201 {"sha256": "..."}` (idempotent by content).

### POST /work
Submit jobs (single or batch). Body: list of `{job_type, bundle_sha, toolkit_sha, required_capability, priority, budget, manifest_sha, max_attempts}`.
`201` with per-job: `{"job_id": "..."}` or `{"cached_result": "<result_sha>"}` when an identical `manifest_sha` already has a DONE result (FR-006 short-circuit).

### POST /work/{job_id}/cancel
`200`; running node is told `"action":"cancel"` at next heartbeat.

### GET /work/{job_id}
Full WorkUnit row incl. state, attempt, best_score, result_sha.

### GET /status
Pipeline roll-up (SC-006, must return <10 s):
```json
{ "functions": {"unmatched": 0, "candidate_identified": 0, "seeded": 0,
                "in_search": 0, "matched": 0, "verified": 0},
  "human_attention": [{"target_id","reason","best_score"}],
  "queue": {"pending": 0, "leased": 0, "done": 0, "failed": 0},
  "nodes": [{"node_id","hostname","cores","last_seen","running": 2}] }
```

### GET /nodes
Node roster with liveness (last_seen within 2× heartbeat = healthy).

### GET /meta/{key} · PUT /meta/{key}
Small key/value registry for pipeline-wide settings (e.g. `toolkit_sha` = the currently pinned toolkit, set by `publish-toolkit`). PUT body: `{"value": "..."}`. GET returns `{"key","value"}` or `404`.

## Error model

`401` bad/missing token · `404` unknown job/blob · `409` lease conflicts (documented above) · `422` malformed body (message in `{"error": "..."}`) · `503` coordinator draining (nodes back off exponentially, max 5 min).

## Compatibility rules

- Additive changes only within v1 (new optional fields ignored by old agents).
- `node_agent.py` sends its `agent_version`; coordinator MAY refuse (`426`) with a URL to the current agent file — self-update is manual by design (curl the file again).
- Job execution contract is versioned by the **toolkit hash**, not the API: behavior changes ship as new toolkits.
