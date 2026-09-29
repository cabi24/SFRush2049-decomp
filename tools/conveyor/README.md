# conveyor — deterministic function-matching pipeline

Distributed compute fabric for the Rush 2049 matching phase. Design docs:
`specs/001-matching-pipeline/` (spec, plan, research, data-model, API
contract, quickstart). Bring-up: see `specs/001-matching-pipeline/quickstart.md`.

## Navigation and current build paths

This is the detailed operations reference. For a task-sized procedure, use the
[matching](../../.claude/skills/match-function/SKILL.md),
[promotion](../../.claude/skills/promote-match/SKILL.md), or
[game-context](../../.claude/skills/refresh-game-context/SKILL.md) skill.
Builder access and current host paths are maintained in [the build environment guide](../../docs/BUILDING.md#watchman2-builder).

- [Queue, toolkit, corpus, and locks](#operating-notes)
- [Managed services](#systemd-units)
- [Target relocations and supersession](#reloc-aware-targets-the-gate-and-supersession-003)
- [Static ROM promotion](#promotion-splicing-matches-become-rom-004)
- [Extracted context](#game-code-context-bootstrap-005) and [histograms](#prototype-layer-and-extracted-flywheel-006)
- [Population closure and data symbols](#population-closure-and-generated-data-symbols-007)
- [Game-image splicing](#game-code-image-rebuild-008-stage-1) and [cartridge composition](#game-code-blob-into-the-cartridge-009-stage-2)

Static `lock`/`promote` commands retain their extracted-target firewall. Features
008/009 provide a separate game-image path into the cartridge, gated by image
byte identity and the full-ROM SHA-1. Historical first-run numbers below are
dated evidence; see [milestones](../../docs/history/project-milestones.md) for
the chronology and use reports for current status.

## Layout

| Path | Runs on | Purpose |
|---|---|---|
| `coordinator/` | Pi | queue + leases, SQLite state, blob store, HTTP API (stdlib only) |
| `agent/node_agent.py` | any x86-64 box | single-file pull agent (stdlib only) |
| `bundles/` | Pi / x86 builder | deterministic job + toolkit bundle builders |
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

Farm daemon (Pi), `conveyor-farm.service` — **installed and enabled
2026-09-24.** It was documented here from 001 onward but never installed, so
the farm ran as a hand-started `nohup` and silently stopped twice (a pause
for lock-verify jobs that was never undone, and a power outage); finished
searches then sat un-ingested until someone noticed. The unit:

```ini
[Unit]
After=network-online.target conveyor-coordinator.service
Wants=network-online.target conveyor-coordinator.service

[Service]
User=cburnes
WorkingDirectory=/home/cburnes/projects/rush2049-decomp
Environment=PYTHONUNBUFFERED=1
ExecStart=/usr/bin/python3 -m tools.conveyor.pipeline.farm run --interval 300
Restart=always
RestartSec=30
```

Operating it:

- **Restart after merging pipeline code** — a running daemon does not pick up
  new code: `sudo systemctl restart conveyor-farm`.
- **Pause it with systemctl, never by killing the process** —
  `sudo systemctl stop conveyor-farm` / `start`. `Restart=always` brings a
  killed process straight back. While it is stopped, finished results wait
  in the queue un-ingested; nothing is lost, but nothing is recorded either.
- Logs: `journalctl -u conveyor-farm -f` (cycle summaries are the
  `farm: {...}` lines).

watchman2's node agent additionally carries a drop-in,
`/etc/systemd/system/conveyor-node.service.d/cores.conf`, capping it at
`--cores 4` and `CPUQuota=400%` so Plex on the same box stays responsive.
Remove the drop-in and `daemon-reload` to give it the whole machine.

## Reloc-aware targets, the gate, and supersession (003)

Static targets are assembled from their splat asm region (`asm/us/*.s`, matched
by address) so the target object carries real `%hi/%lo/jal` relocations instead
of raw ROM words with absolute addresses baked in. `n64_target.tier` records the
outcome:

- `reloc_aware` — assembled from the region and passed the round-trip gate.
- `raw_word` — targets not converted to reloc-aware objects, or targets that fell back;
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

**Gaps reported by the first live run (2026-07-08; historical snapshot):**

Check current reports before treating these as open blockers. Extracted targets
also gained a [relocation-aware path](#reloc-aware-extracted-targets-2026-09-24).

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

## Promotion splicing: matches become ROM (004)

This is how a verified match turns into compiled C linked into the SHA-1-exact
ROM. The full-ROM hash is the *only* promotion authority.

**Layout map** (`pipeline/layout.py`, derived — never hand-maintained): per
splat code subsegment, the ordered functions (name/vaddr/size) with
padding-aware tiling (alignment nops ride with the preceding function; non-zero
data gaps refuse), the flagset joined from the lockfile, and a deterministic
structural map hash embedded in each generated TU header.

```bash
python3 -m tools.conveyor.pipeline.layout derive     # build/layout.us.json
python3 -m tools.conveyor.pipeline.layout report     # clean vs refused segments
python3 -m tools.conveyor.pipeline.layout coverage   # linked-C functions/bytes
```

**Convert** a segment (`asm` → `c` in splat.us.yaml, re-split, generate an
all-passthrough ROM-aligned TU under `src/rom/`). A zero-promotion TU is
byte-identical to pure assembly (asm-processor + IDO), so `make test` stays
SHA-1-exact — the hash-neutral scaffolding. `make extract` (splat re-split)
requires the sanitized symbols produced by `tools/sanitize_symbol_addrs.py`
and `SPLAT_PYTHON` (splat 0.41 in `~/.splat-venv`); rule: never hand-edit the
generated linker script or nonmatchings asm — conversions live in
`splat.us.yaml`.

```bash
python3 -m tools.conveyor.pipeline.layout convert 0x8800   # [--revert]
```

**Promote** one function: splice verified C over its passthrough, full matching
build + SHA-1 gate, commit on pass / clean rollback on any failure, migrate the
lock to the ROM-TU path, record provenance. The CLI and the conveyor job call
the same `run_promotion()` library.

```bash
python3 -m tools.conveyor.pipeline.promote run 0x8800:strlen \
    --from src/libc/string.c --via-builder     # matching build on configured x86 builder
python3 -m tools.conveyor.pipeline.promote batch --locked --via-builder
```

Refusal remedies: *not converted* → `layout convert <seg>`; *no evidence* →
`lock add` (verify) or `--override --reason`; *no pinned flagset* → per-TU flag
sweep; *dirty tree* → commit/stash first; *no IDO here* → `--via-builder`. A
missing MMIO `#define` a promoted body needs goes in `src/rom/rom_tu.h` (the
designed extension point). `make progress` prints derived linked-C coverage.

> **The gate must never be vacuous.** `make verify` hashes the *built* ROM
> (`build/us/…z64`), not `baserom`, and a failing hash exits nonzero — a bug
> where it hashed baserom with failure swallowed by `|| echo` made every
> "ROM matches!" meaningless for months (caught by the SC-003 rollback drill,
> commit 17c70f5). On the builder, `--via-builder` touches the TU and asserts
> the rebuilt object is newer, because rsync-preserved mtimes let `make` skip
> the rebuild and verify a stale ROM.

## Game-code context bootstrap (005)

The `extracted` population is carved from `build/game_code.bin`. Its function
extents are scan-derived: the scanner follows forward branch reach and ends at
the first return outside that reach, including the delay slot. The `info.txt`
sizes are repair inputs, not trusted extents. Running `python3 -m
tools.conveyor.pipeline.matrix extract` reports `extents: N agree, N repaired,
N conflict`; a healthy immediate second run reports `repaired 0`. Targets
nested inside a repaired extent are marked `extent_conflict:<container_id>`.

Autodecomp commands accept `--population {static,extracted}` (default
`static`) and `--targets id1,id2,...|@file`. For example:

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters \
    --population extracted --targets @tools/conveyor/clusters/game_loop.txt
python3 -m tools.conveyor.pipeline.autodecomp clusters \
    --population extracted --limit 0
```

The full-population command writes `build/m2c_histogram.json` (run metadata,
exclusive buckets, and per-target details) and `build/m2c_histogram.md`
(ranked blockers with arcade hints). Extracted assembly is normalized using
the committed game symbol table in `pipeline/disasm.py`; `include/game_types.h`
supplies shared game types, typed globals, and declarations to compile probes.

Extracted targets remain evidence-only for the **static promotion path**. Both
`pipeline.lock add` and `pipeline.promote run|batch` resolve
`n64_target.population` and refuse extracted targets with the 005/FR-010
error. Static targets retain the score-zero lock and full-ROM SHA-1 gate.
Game targets now reach the cartridge through the separate
[008/009 blob path](#game-code-blob-into-the-cartridge-009-stage-2).

## Prototype layer and extracted flywheel (006)

Generate the declaration layer before a full histogram. The generator makes
two deterministic passes, omits declarations owned by the hand-written
context, and writes `build/m2c_protos.h` plus its evidence file:

```bash
python3 -m tools.conveyor.pipeline.protos generate
python3 -m tools.conveyor.pipeline.autodecomp clusters \
    --population extracted --limit 0
```

The histogram has six exclusive buckets: `compiled`, `blocked`,
`partial_decomp`, `decompiler_failure`, `no_disasm`, and `extent_conflict`.
`partial_decomp` is an honesty rule: if raw mips_to_c output contains an
`M2C_ERROR` placeholder, it stays partial even if cleanup would make the seed
compile. Never treat it as `compiled` or rewrite the placeholder away.

Only the unfiltered, untruncated command above is an instrument run. It writes
`build/m2c_histogram.{json,md}` with `run.population_complete=true`. Any
`--targets` filter or truncating `--limit` is a probe and writes
`build/m2c_probe.{json,md}` instead, leaving the population instrument intact.
The farm refuses a probe or incomplete histogram. Compare population runs with:

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters diff \
    specs/006-prototype-flywheel/research/baseline.json \
    build/m2c_histogram.json
```

`clusters diff` reports sorted target movements, bucket-count deltas, and
blocker-class deltas. On each normal `farm run` cycle, the extracted flywheel
submits every compiled target that has neither a historical `permuter_search`
work unit nor a scored matrix cell. It uses the existing autodecomp seed path,
the standard four-hour budget, and never resubmits append-only score evidence.
`cli report` shows `extracted: compiled N, scored M, in_search K`.

Queue priority is ascending: farm verify `1`, farm promote/static search `10`,
autodecomp static seed `30`, extracted flywheel `60`, coordinator default
`100`. Thus all static work has strict precedence over flywheel work. Manual
re-scoring remains an explicit `autodecomp seed` operation; the flywheel does
not override evidence, and extracted targets remain behind the static promotion
firewall. Use the 008/009 blob path for game-image integration.

### Reloc-aware extracted targets (2026-09-24)

```bash
python3 -m tools.conveyor.pipeline.targets --relocate-extracted
```

Extracted target objects used to be assembled raw-word, with absolute
addresses baked into the instruction words. A compiled candidate emits a
relocation with a zeroed field instead, so **every call and every global
reference read as a permanent mismatch** — any extracted function that
referenced anything could not score 0 no matter how correct its C. The only
two that ever did were the only two referencing nothing. Rescoring the 104
targets with stored results against reloc-aware objects turned 2 matches
into 17. Evidence:
`specs/007-population-closure/research/reloc-scoring-finding.md`.

Targets are now assembled from their own derived asm (which already
symbolizes `jal`/`%hi`/`%lo`) behind feature 003's round-trip gate: masked
at the new object's relocation sites, the words must equal the ROM's. A
failure keeps the raw-word object. Live: **891 of 912** gate-passed targets
are `reloc_aware`; the rest are 8 word-mismatches and 13 length-mismatches,
left raw-word and reported. The pass is idempotent and supersedes evidence
when an object changes — the first run purged 61,848 stale matrix scores,
all of them measured against unreachable targets.

Two derivation details this depends on: m2c's synthetic `lui` re-emissions
are dropped (the real instruction is the last in each `.L` label block), and
`c1_fcsr` is renumbered to `$31`, which objdump prints by name and GNU as
refuses.

**After running it, clear queued searches.** Job bundles embed the target
object, so anything already queued would still be scored against the old
one; delete those work units and let the flywheel re-submit.

### Triage before commitment (2026-09-23)

One node runs searches serially, so the flywheel spends node-days, not
node-hours. New seeds therefore get a **20-minute triage pass, smallest
first** (`TRIAGE_BUDGET_SECONDS`); only a target whose triage score lands at
or under `TRIAGE_PROMOTE_MAX_SCORE` (200, nonzero — zero is already a match)
earns a full four-hour search, submitted by the next cycle ahead of new
triage work. `work_unit.budget` records which pass a job was, and a job with
no recorded budget counts as a full search already spent, so pre-triage
history is never redone.

Why these numbers: over the first window, targets of <=20 instructions scored
a median of 30 (best 5) and produced both byte-exact game-code matches — one
in 5 seconds — while targets over 50 instructions have a median of 2095 and
have never converged. `cli report` and the cycle's `flywheel_started` /
`flywheel_promoted` counters show the two streams separately.

### 006 close-out hardening (2026-09-10)

- The farm daemon wraps each step (`ingest`, `flywheel_cycle`, `top_up`) in
  `with_transient_retry`: one retry after 5 s on a dropped/refused/timed-out
  coordinator connection, then log-and-skip that step for the cycle. The
  loop itself never dies on a flaky coordinator path.
- The coordinator retries a locked `blob` insert (`_record_blob`, 5 × 0.5 s)
  on top of the connection's 30 s busy timeout, so a local script holding a
  long write transaction no longer fails a node's upload.
- `permuter_search` writes `function.txt` (the manifest's `target_id`), so
  the permuter never falls back to its regex, which cannot see definitions
  with function-pointer parameters ("does not contain any function!").
- **Toolkit portability rule:** `build_toolkit` never bundles glibc's own
  libraries (`libc`, `libm`, `libpthread`, …). A bundled `libc.so.6` under
  `LD_LIBRARY_PATH` aborts objdump on any node whose glibc differs from the
  build host's — this silently killed all scoring on watchman2 (Ubuntu
  26.04) from the 2026-07-28 retarget until 2026-09-10. Node-side,
  `scoring._objdump_path` health-checks the bundled objdump once and falls
  back to the system `mips-linux-gnu-objdump`. Current toolkit:
  `796ae99a5cb7…`, built on watchman2 (`PYTHONPATH=<old toolkit dir>` supplies
  `pycparser`/`toml` there; IDO comes from the pinned blob's `ido/`).
- `cli smoke --fresh` appends a nonce so the compile really runs on a node
  instead of returning the cached result — use it whenever a node or toolkit
  changes. The strlen fixture now points at
  `asm/us/nonmatchings/rom/lib_8800/strlen.s` (post-004 layout).

## Population closure and generated data symbols (007)

The extracted population is now **closure-derived**: work-inventory names
are historical labels, and the population's truth is `n64_target` after
`closure run`.

```bash
python3 -m tools.conveyor.pipeline.closure run          # j/jal closure to fixpoint
python3 -m tools.conveyor.pipeline.datasyms generate    # build/m2c_datasyms.json
python3 -m tools.conveyor.pipeline.protos generate      # now also emits the externs
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0
```

- `closure run` decodes `j`/`jal` from the raw words of every gate-passed
  extracted extent, gates each unknown in-blob target through the 005 extent
  scanner, and registers survivors as `func_<ADDR8>` (`gate_reason=
  'discovered'`, raw-word object) — iterating until an iteration registers
  nothing (caps 10 iterations / 2000 registrations surface as `cap_hit`).
  `build/closure_report.json` records every candidate's outcome
  (`registered`, `inside_existing_extent`, `scan_failure`, `invalid`,
  `cap_hit`) with discovery provenance. A second run registers zero.
- **Suffix rule** (amendment found live): an inventory row whose address is
  strictly inside a discovered extent is a function *suffix* (the inventory's
  prologue scan started late) and is marked `extent_conflict:<func_id>` —
  object/evidence untouched. `matrix extract` honours discovered extents as
  containers, so re-extraction agrees.
- `datasyms generate` scans the derived asm with the hand table only and
  emits a typed entry for every formed data address in RDRAM that the hand
  table does not name (widest access wins; FP-only `f32`/`f64`; same-width
  int/FP conflicts recorded and typed integer; formation-only `s32`).
  Non-RAM constants and function-pointer formations are omitted with reason.
  Byte-stable; never hand-edit it — put judgement in `include/game_types.h`
  and `disasm.GAME_SYMBOLS`, which win on collision.
- `disasm.symbol_table()` is the one merged lookup (hand > generated) and
  `symbol_table_sha()` covers both, so every cached derivation under
  `build/m2c_asm/` regenerates when either table changes. `protos generate`
  emits `extern <type> D_<ADDR8>;` for the generated layer ahead of the
  function declarations, omitting names the hand context already declares.

Order matters: closure → datasyms → protos (twice, byte-stable) → histogram.
The histogram's denominator reflects the enlarged population automatically.

## Game-code image rebuild (008, stage 1)

The game code is stored as a raw DEFLATE stream at ROM `0xB0CB10` and inflates
at boot to `0x80086A50`. Stage 1 links the 647,072-byte **image** from sources.
[Stage 2](#game-code-blob-into-the-cartridge-009-stage-2), now implemented,
reproduces the compressed stream and puts it into the cartridge. Early 008 notes
predate the identification of zlib 1.0.4 and should not be read as current blockers.

```bash
python3 -m tools.conveyor.pipeline.blob_layout derive     # build/blob_layout.json
python3 -m tools.conveyor.pipeline.blob_tu generate       # assembly, linker script, symbols.json
python3 -m tools.conveyor.pipeline.blob_build build       # the gate
python3 -m tools.conveyor.pipeline.blob_splice splice --all-matched
python3 -m tools.conveyor.pipeline.blob_splice coverage|check|revert <target>
```

`blob_tu generate` also refreshes `asm/us/blob/symbols.json`, the deterministic
name-to-address table for cloud relocation checks. To refresh only that table,
run `python3 -m tools.conveyor.pipeline.blob_tu symbols` on the Pi. It uses
`blob_splice.image_symbols()` with the current layout and symbol context,
records the layout's image SHA-256, and needs no compiled objects or image
bytes. Commit the regenerated table when those inputs change.

Everything except the IDO compile runs on the Pi. The initial map described the image
as an ordered, complete list of entries — 912 gate-passed functions (79.5%)
and 214 opaque runs (20.5%, mostly the 85 KB data tail). Overlaps and
coverage holes are hard errors: either would corrupt the image silently.

**The gate**: the linked image must equal `build/game_code.bin` byte for
byte. A failure names the first differing offset, its vram, and the region
and function that own it. Drilled on purpose (SC-003) before being trusted,
because 004's ROM hash gate had been vacuous for months.

**Spliced C contributes bytes, not sections.** IDO pads `.text` to 16 bytes,
so a 72-byte function occupies 80 in its object and the padding overwrites
its neighbour; truncating the section loses its relocations. Each function is
linked ALONE at its image address — where `PROVIDE` resolves its data globals
(inside opaque runs), its calls out to the cartridge, and its sibling game
functions, 4,346 symbols in all — and the extent decides how many bytes it
contributes. Splice at the flagset the sweep recorded as scoring 0, not the
default: a body matches under one optimization level and not the other.

Stage 1 verifies image coverage. Stage 2 makes those splices cartridge coverage;
`make progress` reports static and game code separately under one cartridge
heading. Never merge their denominators. The 005 firewall still protects the
static promotion commands; blob splicing uses its own path and locks.

## Game-code blob into the cartridge (009, stage 2)

The cartridge's compressed game-code blob is now **built from sources**. The
ROM build composes its `data` segment around `build/blob/game_code.deflate`,
so every function spliced into the game-code image (008) is cartridge
coverage, gated by the full-ROM SHA-1.

```bash
python3 -m tools.conveyor.pipeline.blob_rom produce      # Pi: image -> blob, pre-flight
python3 -m tools.conveyor.pipeline.blob_rom rom          # + sync + builder ROM build + make test
python3 -m tools.conveyor.pipeline.blob_rom rom --drill  # also prove the gate bites
python3 -m tools.conveyor.pipeline.blob_rom coverage     # what `make progress` prints
```

- **Compressor:** vendored zlib 1.0.4, deflate half only
  (`tools/zlib-1.0.4/`, see its `PROVENANCE.md`), driven by
  `tools/deflate104` with the cartridge's exact parameters — level 9,
  windowBits −15, memLevel 8, default strategy. Other tested zlib versions produced
  streams 140 bytes too long; see the [compressor research](../../specs/008-blob-image-rebuild/research/compressor-identified.md).
  It is a 1996 release with known CVEs: a build-time oracle for our own
  image, never for untrusted input.
- **What gets compressed** is the image *linked from sources*, never
  `build/game_code.bin` — compressing the extracted image would make the
  pipeline decorative.
- **No fallback.** `data.o` depends on `GAME_BLOB` and no rule makes it, so a
  missing blob stops the build (`No rule to make target
  'build/blob/game_code.deflate'`). `tools/compose_data.py` never reads the
  original slot bytes and refuses any blob that is not exactly 326,180 bytes.
- **Where it runs:** the image needs the layout map and cached spliced
  objects, which live on the Pi, so `produce` runs there and `rom` rsyncs the
  blob, the Makefile and `compose_data.py` to the builder before `make`.
- **Drilled:** flipping one bit of the blob makes `make test` fail with `ROM
  does NOT match` — a hash mismatch, not a build error — and restoring it
  passes again. The drill checks for that message specifically, because a
  broken build also "fails" and would prove nothing.

## Known V1 limitations

- `verify_promote` still lands matched source in `work/<...>/<fn>/matched.c`
  (the pre-004 stopgap); upgrading it to call `pipeline.promote.run_promotion`
  on the builder so pipeline-verified matches splice automatically is tracked
  as 004 T012 (needs a toolkit rebuild). The full-build gate already runs.
- Clustering runs locally on the Pi (cheap); the `cluster_score` job type is
  reserved for scaling out if it ever isn't.
- Flag sweeps take an explicit `--tu <file> --functions a,b,c`; automatic
  TU discovery is tied to the same layout map as promotion splicing.
