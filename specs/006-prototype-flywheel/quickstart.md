# Quickstart: Prototype Layer & Seed Flywheel (006)

Everything except §4 runs on the Pi alone. Baseline (2026-07-19):
`42 compiled / 597 blocked / 49 decompiler_failure / 0 no_disasm /
197 extent_conflict`; 289 `func_`-shaped blockers.

## 1. Hygiene + honest buckets

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0
python3 -c "import json; d=json.load(open('build/m2c_histogram.json')); print(d['buckets']); assert sum(d['buckets'].values())==d['run']['targets']"
```

Expect the six-bucket schema with `partial_decomp` > 0 (M2C_ERROR class,
~101 at baseline priced as blockers). Run twice → identical counts.
Scoped probe check: a `--targets` run writes `build/m2c_probe.json` and
leaves `build/m2c_histogram.json` untouched (compare mtime/sha).

Actual (2026-07-19, T005): two full-population runs both produced
`37 compiled / 299 blocked / 303 partial_decomp / 49 decompiler_failure /
0 no_disasm / 197 extent_conflict` (sum 885) with
`run.population_complete: true`. The complete JSON objects were identical
after removing only `run.timestamp`; the generated Markdown was byte-identical
(`sha256 5b9c99fae2704321f8b0ba0a9a3b3d04e5a696eb19aa24f5df143d7df70b0924`).
All 101 functions in the baseline's visible `M2C_ERROR` blocker class moved to
`partial_decomp`; raw-output classification exposed another 202 placeholders
which had previously been masked by an earlier compiler diagnostic.

The diff against `research/baseline.json` reported bucket deltas
`blocked -298`, `compiled -5`, `partial_decomp +303`, with the other buckets
unchanged. Its 308 target movements were `293 blocked -> partial_decomp`,
`10 compiled -> partial_decomp`, and `5 blocked -> compiled`. A scoped probe
of `Input_AllocPadSlot` wrote `build/m2c_probe.{json,md}` with
`population_complete: false`; SHA-256 and nanosecond mtime for both population
artifacts remained unchanged.

## 2. Declaration layer

```bash
python3 -m tools.conveyor.pipeline.protos generate      # two passes, ~8 min
python3 -m tools.conveyor.pipeline.protos generate      # byte-stable check
sha256sum build/m2c_protos.h                            # identical both runs
python3 -m tools.conveyor.pipeline.autodecomp clusters --population extracted --limit 0
```

Gates: `func_<addr>` blockers for known targets = 0; compiled ≥ 200
(SC-001); zero redefinition-class errors in the run's diagnostics
(SC-002); SC-005 body-identity test still green
(`pytest tests/conveyor/test_autodecomp_population.py -k body_byte_identical`).

Attribution (FR-010):

```bash
python3 -m tools.conveyor.pipeline.autodecomp clusters diff <pre.json> build/m2c_histogram.json
```

Actual (2026-07-19, T009 stop rule): two corrected full `generate` runs
produced byte-identical `build/m2c_protos.h` files with SHA-256
`c95bffd469a910c7d9cf02ad1672e43d7881577fa9622914d2ca70819211aec9`.
The layer covers 718 symbols as 596 declarations (586 pass-2 own definitions,
10 fallbacks) and 122 omissions (58 `hand_context`, 64 `static_target`), with
no declaration/omission overlap and no hand-context leaks.

The full histogram produced `58 compiled / 252 blocked / 329 partial_decomp /
49 decompiler_failure / 0 no_disasm / 197 extent_conflict` (sum 885).
Known-target `func_<addr>` blockers were 0, the `math_utility` spot blocker was
0, and no per-target diagnostic matched `redeclar|conflicting types` (SC-002
passed). SC-001 failed: 58 compiled is below 200, so work stopped without
hand-typing context and the residual analysis was recorded in
`research/t009-shortfall.md`.

Against the committed `research/baseline.json`, `clusters diff` attributed
`compiled +16`, `blocked -345`, `partial_decomp +329`, with all other buckets
unchanged. Relative to the post-hygiene T005 actuals in §1, the count movement
was `compiled +21`, `blocked -47`, `partial_decomp +26`.

## 3. Local suite

```bash
pytest tests/conveyor -m "not node_required"
```

New: `test_protos.py`; extended: `test_autodecomp_population.py` (bucket,
hygiene rules incl. no-overreach, probe routing, priority-ladder assert).

## 4. Flywheel window (coordinator + watchman)

```bash
# bring-up per specs/001-matching-pipeline/quickstart.md §1–4, then:
python3 -m tools.conveyor.pipeline.farm run          # daemon with flywheel step
python3 -m tools.conveyor.cli report                 # 'extracted: compiled N, scored M, in_search K'
```

SC-004: after one unattended window, scored M == compiled N. SC-005: no
static job displaced (queue timestamps: any static submission leases before
waiting flywheel jobs). SC-006: `promotion_record` has zero extracted rows.

Actual (window 2026-07-19 20:00 UTC → 2026-07-27 23:35 UTC; recorded at
close-out 2026-09-10, T012): the flywheel population was the post-separator-fix
histogram (`60 compiled`, `population_complete: true`, timestamp
2026-07-19T13:43Z). 37 of the 60 already carried score evidence from 005 /
earlier searches; the cycle selected the remaining **23** and submitted all
23 at priority 60 within 19 s. Outcome per `farm.flywheel_selection`:
**compiled 60, scored 58, in_search 0** — SC-004's scored == compiled was
**not reached: 58/60**. The two gaps are a pipeline bug, not a seed problem:

- `display_list_traverse` and `entity_name_copy` FAILED after 5 attempts
  each with `Exception: base.c does not contain any function!` from the
  permuter. Both seeds take a function-pointer parameter
  (`s32 (*arg4)(s32, u32)`), which the permuter's fallback regex
  `(\w+)\([^()\n]*\)\s*?{` cannot match; the job never told the permuter
  the function name. Fixed at close-out: `jobs/permuter_search.py` now
  writes `function.txt` from `manifest.target_id`. The fix lives in the
  toolkit (`jobs/` ships inside it), so it takes effect on the next toolkit
  rebuild/publish; the two targets stay `seeded`/`job_error` until then and
  are resubmitted by the next flywheel cycle only if their evidence rows are
  cleared (they currently count as "searched").
- Of the 21 DONE: 18 scored > 0 (5 … 3005), `audio_engine_update` scored
  **base 0** (it is a 2-instruction `jr $ra; nop` stub — trivially
  identical, evidence only, firewalled), and 2 (`stat_lap_complete`,
  `state_change_preprocess`) returned `seed does not compile` on the node
  although the Pi histogram had them `compiled` — the node compiles under
  the toolkit's shim headers, the histogram under the Pi's probe context;
  they count as scored (a DONE search with a recorded outcome) but carry no
  score. That context skew is a residual for 007's re-measure.

History (honest, as the HANDOFF required): the daemon's first two flywheel
cycles died on a transient coordinator `RemoteDisconnected` and submitted
nothing; the cycle was primed manually on 2026-07-19 through the same code
path (23/23). The agent then lost the coordinator path for ~10 h overnight
(watchman flakiness), retry-looped as designed and self-healed ~23:51 UTC.
Three `sqlite3 database is locked` tracebacks appeared in the coordinator
log, all in `_record_blob` under `PUT /api/v1/blobs` from a local script
while another local script held a write transaction past the connection's
30 s busy timeout. The last two searches (`stat_lap_complete`,
`state_change_preprocess`) completed 2026-07-27 after the builder was
retargeted to watchman2 (`349e1a6`). Both faults are hardened at close-out
(farm `with_transient_retry`: one retry then log-and-skip the step, never
the loop; coordinator `_record_blob`: 5 short retries on a locked write —
`tests/conveyor/unit/test_hardening_006.py`).

**Builder finding (2026-09-10, close-out):** a `cli smoke --fresh` probe
(new flag; the cached smoke result had been masking this) FAILED on
watchman2: the toolkit's bundled `bin/objdump` died with SIGABRT ("stack
smashing detected"). Root cause: `build_toolkit._ldd_libs` copied the build
host's `libc.so.6` into `lib/`, and `scoring._objdump_path` puts `lib/` on
`LD_LIBRARY_PATH`; on watchman2 (Ubuntu 26.04, glibc 2.43) the 22.04 libc was
loaded under the native ld.so and aborted. IDO itself ran fine. Consequence:
**no job could score on watchman2 between the 2026-07-28 retarget and
2026-09-10** — the two 07-27/28 flywheel completions above ("seed does not
compile") are this fault, not a context skew, and the smoke fixture itself
had gone stale after the 004 re-split (`asm/us/8800.s` → `nonmatchings/rom/
lib_8800/strlen.s`, fixed). Fixes: builder never bundles glibc libraries;
node-side objdump health-check with fallback to the system mips objdump;
toolkit rebuilt on watchman2 from the pinned blob's IDO + its binutils
2.45.90 objdump → **`796ae99a5cb7…` pinned as current**; `smoke --fresh`
then PASSED (strlen score 0 on watchman2). The four unscored targets
(`display_list_traverse`, `entity_name_copy`, `stat_lap_complete`,
`state_change_preprocess`) were resubmitted at priority 60 on the new
toolkit at close-out; their outcomes land in the queue as usual and do not
change this section's verdict (58/60 for the window as run).

SC-005: no static search or promotion job was created or leased between
2026-07-19T19:00Z and the window's end (queue query: zero `priority < 60`
rows with `created_at`/`updated_at` in range), so nothing could be
displaced; the priority ladder itself is asserted by
`test_flywheel.py`. SC-006: `promotion_record` holds 28 rows, **0** with
`population = 'extracted'`.

Note: 526 static priority-30 searches plus the 23 flywheel rows have
`ingested_at IS NULL` — the farm's ingest step has not been the banking
path since 005; `autodecomp harvest` is (run at close-out: 0 new score-0;
it skips extracted base-0 stubs because its asm index is static-only —
`audio_engine_update` is therefore recorded here, not in `work/auto`).

## Actuals

T005 actuals are in §1, T009 in §2, T012 in §4. Scorecard: `CLOSEOUT.md`.
