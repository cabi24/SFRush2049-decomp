# conveyor — deterministic function-matching pipeline

Distributed compute fabric for the Rush 2049 matching phase. Design docs:
`specs/001-matching-pipeline/` (spec, plan, research, data-model, API
contract, quickstart). Bring-up: see `specs/001-matching-pipeline/quickstart.md`.

## Layout

| Path | Runs on | Purpose |
|---|---|---|
| `coordinator/` | Pi | queue + leases, SQLite state, blob store, HTTP API (stdlib only) |
| `agent/node_agent.py` | any x86-64 box | single-file pull agent (stdlib only) |
| `bundles/` | Pi / watchman | deterministic job + toolkit bundle builders |
| `jobs/` | nodes (in toolkit) | executors: compile_score, flag_sweep, permuter_search, verify_promote |
| `seeds/` | Pi | arcade candidate extractor + compatibility shim |
| `pipeline/` | Pi | matrix, farm, sweep, cluster, targets, status |
| `cli.py` | Pi | serve, publish-toolkit, smoke, status, nodes, seed, best, attention, pin-flags, pair, report, bootstrap-flags, gc |

## Operating notes

- **Token**: created at first `serve`, stored at `~/.conveyor/token` (mode 600).
  Nodes receive it at bootstrap; treat like a LAN-local password.
- **State**: everything lives in `~/.conveyor/` (SQLite WAL + `blobs/`).
  Back it up by copying the directory; the blob store is content-addressed so
  partial copies are safe.
- **Node lifecycle**: kill agents freely — leases expire (120 s) and jobs
  re-issue. A node is "removed" by stopping its agent; nothing to deregister.
- **Toolkit updates**: any change to `jobs/` or the shim requires rebuilding
  and re-publishing the toolkit (`build_toolkit` + `publish-toolkit`); nodes
  fetch the new sha on their next lease. Never mutate node caches by hand.
- **Builder**: exactly one agent should run with `--capabilities
  x86_64,builder --repo <clone>`; promotions serialize through it.
- **Flag pins**: `serve` auto-seeds `flag_registry` from the proven pins in
  `docs/COMPILER_SETTINGS.md` (source `confirmed`), so the sweeper never
  re-discovers them; `bootstrap-flags` does the same against an already-running
  coordinator. Both are idempotent and never clobber a `manual_override`.
- **Blob GC**: `gc` reclaims job/result blobs unreferenced by any live state
  and older than `--days` (default 7). Dry-run by default; pass `--apply` to
  delete. Toolkit blobs and anything still reachable are never touched.
- **Regression lock**: `matched.lock.json` (repo root) pins the normalized
  source-body hash of every function proven byte-identical. `make
  check-matched` (or the `.githooks/pre-commit` hook — enable once with
  `git config core.hooksPath .githooks`) re-hashes locally in milliseconds
  and fails on drift. Pin new matches with `python3 -m
  tools.conveyor.pipeline.lock add <file>:<fn> --flags "..."` — add compiles
  the function through the pool (a reduced TU: real repo headers, all other
  functions stripped) and refuses to lock without score 0. `lock verify`
  re-proves existing entries end to end.
- **Shim iteration loop**: after every `matrix ingest`, run
  `python3 -m tools.conveyor.pipeline.matrix failures` — it clusters candidate
  compile failures by signature (`undefined: gstate`, `unknown type? BLIT`)
  and ranks them by how many candidates each blocks. `--locate` greps the
  arcade headers for likely definitions of the top undefined identifiers;
  `--grep <substr>` lists the candidates behind one cluster. Fix the top
  clusters in `seeds/shim/conveyor_shim.h`, rebuild + re-publish the toolkit,
  resubmit, and watch coverage climb in `matrix report`. Note a shim change
  is a new toolkit sha, so all cells recompute (correct: the shim can change
  codegen).

## systemd units

Coordinator (Pi), `/etc/systemd/system/conveyor-coordinator.service`:

```ini
[Unit]
Description=conveyor coordinator
After=network-online.target

[Service]
User=cburnes
WorkingDirectory=/home/cburnes/projects/rush2049-decomp
ExecStart=/usr/bin/python3 -m tools.conveyor.cli serve
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

Node agent (any x86-64 box), `conveyor-node.service`:

```ini
[Service]
User=cburnes
ExecStart=/usr/bin/python3 /opt/conveyor/node_agent.py \
    --coordinator http://PI:8323 --token TOKEN --cores 20
Restart=always
RestartSec=15
```

Farm daemon (Pi), `conveyor-farm.service`: same pattern with
`ExecStart=/usr/bin/python3 -m tools.conveyor.pipeline.farm run`.

## Known V1 limitations

- `verify_promote` lands matched source in `work/<...>/<fn>/matched.c` and
  commits it; splicing matched functions into `src/*.c` translation units
  needs the function→TU layout map (later phase). The full-build gate still
  runs so the repo can never regress.
- Clustering runs locally on the Pi (cheap); the `cluster_score` job type is
  reserved for scaling out if it ever isn't.
- Flag sweeps take an explicit `--tu <file> --functions a,b,c`; automatic
  TU discovery is tied to the same layout map as promotion splicing.
