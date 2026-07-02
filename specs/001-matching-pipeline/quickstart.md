# Quickstart: Conveyor Bring-Up

**Feature**: 001-matching-pipeline | **Date**: 2026-07-02
From zero to a working pool and a first end-to-end match verification.

## 1. Coordinator (Pi 5)

```bash
cd ~/projects/rush2049-decomp
python3 -m tools.conveyor.cli serve --port 8323 --data ~/.conveyor
# First run prints the bearer token; it's stored at ~/.conveyor/token
```

State lives in `~/.conveyor/` (SQLite DB + blob store). Restart-safe; run under systemd for real use.
The coordinator and node agent are stdlib-only — no pip installs needed to *serve* or to join nodes. (One exception on the Pi: `matrix extract` uses `pycparser` for arcade source parsing — `pip install pycparser` once, or reuse the copy vendored with decomp-permuter.)

## 2. Build and publish the toolkit bundle (one-time, on watchman)

The toolkit needs x86-64 binaries (IDO, mips objdump), so build it where they run:

```bash
ssh watchman
cd ~/projects/rush2049-decomp
python3 -m tools.conveyor.bundles.build_toolkit \
    --ido tools/ido-static-recomp/build/out \
    --objdump "$(command -v mips-linux-gnu-objdump)" \
    --permuter tools/decomp-permuter \
    --shim tools/conveyor/seeds/shim \
    --out /tmp/toolkit.tar.gz
python3 -m tools.conveyor.cli publish-toolkit /tmp/toolkit.tar.gz \
    --coordinator http://<pi>:8323 --token <token>
# Prints: toolkit sha256 <SHA> pinned as current
```

## 3. Join compute nodes (any x86-64 Linux box)

One file, no dependencies:

```bash
curl -H "Authorization: Bearer <token>" \
  http://<pi>:8323/agent/node_agent.py -o node_agent.py
python3 node_agent.py --coordinator http://<pi>:8323 --token <token> \
    --cores $(nproc) --cache ~/.conveyor-cache
```

The agent pulls work, fetches+caches the toolkit by sha256 on first job, and runs until killed. Killing it (or the whole box) at any moment is safe — leases expire and jobs re-issue. Watchman joins the same way, plus the builder flag and a repo clone:

```bash
python3 node_agent.py ... --capabilities x86_64,builder --repo ~/projects/rush2049-decomp
```

## 4. Smoke test (single known function)

Proves the whole loop: bundle → lease → compile on node → score → result ingest.

```bash
# On the Pi
python3 -m tools.conveyor.cli smoke --function strlen
# Submits one compile_score job for strlen (known perfect match at -O2).
# PASS = score 0 comes back and status shows strlen: matched
python3 -m tools.conveyor.cli status
```

## 5. Run the pipeline stages

```bash
# Extract arcade candidates + report compile coverage (runs extraction on Pi,
# compilation as pool jobs):
python3 -m tools.conveyor.pipeline.matrix extract
python3 -m tools.conveyor.pipeline.matrix submit           # full matrix (batched)
python3 -m tools.conveyor.pipeline.cluster submit          # target clustering
python3 -m tools.conveyor.pipeline.sweep submit --unmatched-only
python3 -m tools.conveyor.pipeline.farm run                # keeps search queue full

# Watch it
python3 -m tools.conveyor.cli status          # <10 s roll-up (SC-006)
python3 -m tools.conveyor.cli report          # full progress + human-attention queue
python3 -m tools.conveyor.cli nodes           # pool health
```

`farm run` is the steady-state daemon: it turns matrix rankings into seeds, keeps N search jobs queued (closest-to-matching first), promotes cluster siblings when a member matches, and submits `verify_promote` jobs to the builder when searches hit zero. It contains no model calls and needs no attention (FR-016).

## 6. Where humans (or Claude) plug in

```bash
python3 -m tools.conveyor.cli attention      # ranked human-attention queue
```
- `no_ancestry` functions → write a seed by hand/m2c, then `cli seed <target> <file.c>`
- `stalled` searches → inspect best-so-far (`cli best <target>`), fix structure, re-seed
- shim gaps → edit `tools/conveyor/seeds/shim/`, re-run `matrix extract` (coverage report shows the win)
- overrides: `cli pin-flags <tu> "<flags>"`, `cli pair <target> <candidate>` — never clobbered by re-runs

## Verify it's healthy

| Check | Command | Expect |
|---|---|---|
| Coordinator up | `curl http://<pi>:8323/healthz` | `{"status":"ok",...}` |
| Nodes pulling | `cli nodes` | last_seen < 60 s |
| Determinism | resubmit any batch job | `cached_result` short-circuit |
| Gate integrity | `cli report` | every `verified` has a PromotionRecord + commit hash |
