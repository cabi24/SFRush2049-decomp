# Session notes — July 7/8 2026 (conveyor first real run)

Handoff notes for restarting Claude in tmux. As of 2026-07-08 03:02 BST.

## Daemon state: BOTH STOPPED (cleanly, ~09:30 BST, for a CC relaunch)

All state is on disk and restart-safe. To bring the pool back up:

- **Coordinator (Pi)**: `nohup python3 -m tools.conveyor.cli serve --port 8323 --data ~/.conveyor &`
  Token at `~/.conveyor/token`. State in `~/.conveyor/` (SQLite + blobs).
- **Node agent (watchman)**: `~/node_agent.py` already downloaded there; relaunch with
  `nohup python3 ~/node_agent.py --coordinator http://192.168.50.128:8323 --token $(cat ~/.conveyor/token from Pi) --cores 16 --cache ~/.conveyor-cache --capabilities x86_64,builder --repo ~/projects/rush2049-decomp &`
  Toolkit sha `23185a79...` pinned as current, cached at `~/.conveyor-cache` on watchman.

## What happened this session

1. Watchman powered on → full conveyor bring-up per quickstart §1–4: repo rsynced, toolkit
   built+published on watchman, agent joined, smoke test run.
2. **First smoke FAILED (score 35)** — two real bugs found and fixed, committed as `7e003da`
   on `001-matching-pipeline`:
   - `src/libc/string.c` strlen had drifted from the matching form (dead pre-loop load that
     IDO eliminated → wrong codegen). Restored the pipelined `do { p++; } while (*p)` form —
     byte-identical to func_80007C40.
   - Dedupe bug: `manifest_sha` (result cache key) didn't cover input file *contents*, so
     resubmitting with edited source returned the stale cached result. Fixed in
     `tools/conveyor/bundles/build_job.py` (folds `input_shas` into the manifest before
     hashing). Tests updated; 49 local tests pass.
3. **SMOKE PASS — T019 done.** Real IDO on watchman, score 0, determinism check verified
   (resubmit → `cached_result`).
4. `matrix extract` (1143 targets, 2526 candidates) + `matrix submit` → queue above.
5. Fixed files also rsynced to watchman's repo copy.

## Matrix run COMPLETE (03:55 BST) — results ingested

All 254 jobs done, 0 failed. `matrix ingest` ran: 281,933 cells scored.

**Headline: candidate compile coverage is the bottleneck.** Only 287/2510 arcade
candidates (11%) compile under the neutral shim — the other 89% never got scored.
Per quickstart §6, the fix is shim work: add missing typedefs/stubs to
`tools/conveyor/seeds/shim/`, re-run `matrix extract` + `submit` (dedupe means
only new/changed cells recompute), and watch coverage climb in
`pipeline.matrix report`. This is the highest-value next step.

The 7 `candidate_identified` results are mostly degenerate: score-0 pairs like
osWritebackDCache↔INIT_INFO are tiny stub functions trivially matching each other,
not real ancestry. 1,012 of 1,131 targets have best score >100. Also note:
libc/libultra targets will *always* be no_ancestry in the arcade matrix (they're
not arcade code) — they're covered by the per-TU flag sweep path instead
(`pipeline.sweep submit --tu src/libc/string.c ...`).

## Next steps for the new session

1. **Shim coverage**: sample compile failures for common causes, extend
   `tools/conveyor/seeds/shim/conveyor_shim.h`, re-extract/resubmit. Target >50%.
2. `pipeline.cluster run` (local) — clone clusters help once real matches appear.
3. `pipeline.sweep submit` for repo TUs (libc/libultra flag confirmation).
4. `pipeline.farm run` once seeds are meaningful (don't seed the permuter from
   the degenerate matches above).
5. T048: 24h soak + systemd units for coordinator (Pi) and node agent (watchman)
   so both survive reboots.

Note: coordinator log is in this session's scratchpad under /tmp — gone after reboot,
harmless. Restart command is at the top of this file / quickstart §1.
