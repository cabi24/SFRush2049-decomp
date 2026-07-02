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
| `cli.py` | Pi | serve, publish-toolkit, smoke, status, nodes, seed, best, attention, pin-flags, pair |

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
