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
- **Corpus candidates** (`pipeline.corpus`, 002): a second candidate source
  beyond the arcade tree — canonical library implementations from local git
  clones (V1: decompals/ultralib). Loop:

  ```bash
  corpus register ultralib reference/repos/ultralib \
      --repo-url https://github.com/decompals/ultralib \
      --include-dirs include,include/compiler/ido,include/PR   # once
  corpus ingest            # extract functions -> candidates (origin+provenance)
  corpus submit            # name-pair every target to a same-named candidate,
                           # compile+score under both confirmed flagsets
  corpus ingest-results    # shared matrix ingest + reloc flags + artifacts
  corpus report [--target] # roots, pairing coverage, per-origin compile rate,
                           # per-target best true/reloc_blind, flag summary
  ```

  Pairing is by **exact function name** (no size window), so a target that is
  generic library code (permanently `no_ancestry` in the arcade matrix) gets
  its canonical source compiled and scored. `submit` builds ordinary
  `compile_score` jobs (dedupe/caching apply), sources the candidate's own
  reduced TU + repo headers (comments stripped so IDO accepts `//` without
  `-Xcpluscomm`), and bundles the target `.o`. Ingest **refuses a dirty,
  missing, or moved clone** (`--allow-dirty` records a `<sha>-dirty` provenance)
  because provenance must describe the exact bytes.
- **`score_reloc_blind`**: every scored cell now also carries a
  relocation-blind score — the same word diff after masking the fields the
  candidate's relocations patch (HI16/LO16 low half-words, R_MIPS_26 targets).
  A candidate instruction-identical to its target except for unresolved
  addresses reports `reloc_blind=0` even though its true score is nonzero
  (the target `.o` carries absolute addresses, the candidate zeroes).
- **`reloc_only_diff`**: a target whose best corpus evidence is `reloc_blind=0`
  with true score > 0 is flagged `reloc_only_diff` (a review advisory in
  `function_status.human_flag`, never a promotion state) and gets a
  `work/<…>/<target>/corpus_match.c` artifact — the candidate source plus a
  provenance header (origin, source@commit, flags, both scores). It is derived
  state: regenerated every `ingest-results`, never hand-edited, and it upgrades
  to real verification automatically when relocation-aware target objects land
  (re-scoring is content-addressed). **Nothing reaches the lock, a promotion,
  or `src/` without a true score of 0** — `reloc_only_diff` is not a match.

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

## Reloc-aware targets, the gate, and supersession (003)

Static targets are assembled from their splat asm region (`asm/us/*.s`, matched
by address) so the target object carries real `%hi/%lo/jal` relocations instead
of raw ROM words with absolute addresses baked in. `n64_target.tier` records the
outcome:

- `reloc_aware` — assembled from the region and passed the round-trip gate.
- `raw_word` — dynamic game-code targets, and any static target that fell back;
  `gate_reason` says why: `no_asm_region`, `assemble_error: <first as error>`,
  `word_mismatch@<i>`, `length_mismatch <n> != <m>`.

The **round-trip gate** (`targets.gate_target`) is the safety property: the
reassembled object's instruction words, masked at its own relocation sites (the
002 mask helpers, reused verbatim), must equal the ROM words masked identically
(trailing nop padding ignored; objdump run with `-dz`). A failing target keeps
its raw-word object — the fallback is a *success* outcome, never softened.

**Supersession**: when a target's object bytes change, `populate()` deletes that
target's `matrix_entry` rows in the same transaction and prints `superseded: <n>
targets, <m> evidence rows purged` — stale evidence is scored against an object
that no longer exists. Result blobs and `work_unit` rows are never touched
(audit trail; superseded blobs GC out later). A `superseded` count after a
re-extraction that changed target objects is normal; a second identical
extraction must print `superseded: 0`. Every scored cell carries
`matrix_entry.target_o_sha` (submit → node echo → ingest), and `corpus report`
prints `attribution: <n> cells checked, <k> mismatched (expect 0)`.

**Known gaps surfaced by the first live run (2026-07-08) — not yet resolved:**

- Reloc-aware targets score 0 against a candidate only when the reloc **symbol
  names match** (or the candidate side is section-relative). Splat's asm names
  (`D_8002C3D0`) diverge from both `symbol_addrs.us.txt` and the corpus
  candidates' source names (`__osThreadTail`), so the 18 `reloc_only_diff`
  targets do not reach true 0 yet — symbol-name reconciliation is a separate,
  out-of-scope task. See `specs/003-reloc-aware-targets/quickstart.md §3`.
- Functions that reference hardware registers (MMIO / KSEG1 `#define`d
  addresses) regress: splat symbolizes the address so the target goes
  reloc_aware, but IDO emits a literal immediate, so the reloc-aware target
  scores *worse* than the raw-word one. Four locked functions (`osDpGetCounters`,
  `__osSpSetPc`, `__osSpDeviceBusy`, `__osSpSetStatus`) fail `lock verify` under
  003 for this reason. A no-regression guard (keep MMIO-absolute references
  raw-word) is the likely fix. See quickstart §4.

## Known V1 limitations

- `verify_promote` lands matched source in `work/<...>/<fn>/matched.c` and
  commits it; splicing matched functions into `src/*.c` translation units
  needs the function→TU layout map (later phase). The full-build gate still
  runs so the repo can never regress.
- Clustering runs locally on the Pi (cheap); the `cluster_score` job type is
  reserved for scaling out if it ever isn't.
- Flag sweeps take an explicit `--tu <file> --functions a,b,c`; automatic
  TU discovery is tied to the same layout map as promotion splicing.
