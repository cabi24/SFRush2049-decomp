# Finite hypothesis batches

`tools/cloud/hypothesis_batch.py` is a thin adapter around the pinned Workbench
campaign engine and Rush's canonical `score.compare`. It runs a finite reviewed
set and never adopts sources, promotes matches, or starts another target.

## Calibration quick start

Prerequisites: Python 3.10+, existing IDO 5.3, GNU MIPS assembler and objdump.
Set `IDO_DIR`, `PATH`, `MIPS_OBJDUMP` and (when needed) `LD_LIBRARY_PATH` for those
installed tools. This command does not install or fetch a compiler.

```sh
python3 tools/cloud/prepare_car_select_batch.py --out build/hypothesis/car-select-plan
python3 tools/cloud/hypothesis_batch.py validate build/hypothesis/car-select-plan/batch.json
python3 tools/cloud/hypothesis_batch.py prepare build/hypothesis/car-select-plan/batch.json --out build/hypothesis/readiness-01
# Run only when ready to execute the bounded experiment:
python3 tools/cloud/hypothesis_batch.py run build/hypothesis/car-select-plan/batch.json --jobs 2 --out build/hypothesis/pilot-01
python3 tools/cloud/hypothesis_batch.py report build/hypothesis/pilot-01
```

Output directories must be new. `prepare` runs two baseline builds and a deliberately
nonmatching ELF canary; it does **not** compile the 20 variants. `run` repeats those
controls, then runs all declared variants with at most two compiler/scorer jobs.
For a machine-overhead comparison, replay the same plan with `--jobs 1` in another
new directory. This does not measure LLM savings or authorize another experiment.

The generated calibration is pinned to commit
`c35728addbae1626aabc89b7d837e28774f333a0` and the actual complete production
`car_select_handler` TU. The generator rejects baseline drift and records complete
C files, hashes, exact patches, semantic justifications and native-effect predictions.
All 20 proposals alter existing local declarations, complete case ordering, guard
structure or floating comparison spelling. No callers or context are fabricated.
This already-integrated function cannot yield new matched-byte coverage.

## Contract and current boundaries

- The Rush sidecar references an ordinary Workbench experiment-v2 manifest.
  Unknown sidecar keys, duplicate JSON keys, changed inputs, outside-root paths,
  symlinks, unknown signals and excessive budgets fail closed.
- Version 1 supports self-contained, standalone O2 production TUs only. Include
  directives, altered signatures/TU context, other flags, IPA/group builds and
  custom Workbench controls are rejected. Semantic equivalence still requires
  source review; the adapter is not a C equivalence prover.
- The private snapshot comes from the declared Git commit, not working-tree
  headers. The adapter, canonical scorer, own-data verifier, Workbench, protected
  target manifests, source bytes, compiler stages, assembler, objdump and supplied
  runtime libraries are hashed. The compiler uses copied tools and sealed
  environment; the Python executable/version is recorded. System libc/kernel
  behavior is not a hermetic virtual machine.
- Baseline builds must both be strict exact with repeatable strict fields before
  any variants start. Each source is compiled at its production-relative filename
  in a private directory/TMPDIR. Workbench's `stop_on_exact` is false.
- Canonical scoring occurs in fresh processes with declared deadlines. A private
  target ELF is assembled only from verified target words and checked for padding.
  Private objects and complete streams survive Workbench diagnosis failures.
- Exact source duplicates compile once. Complete ELF hash matches are confirmed
  by byte comparison; equal `.text`, diagnostic basins or scores never justify
  collapsing relocation/own-data differences. Every candidate ID remains visible.
- A missing expected-effect measurement is `UNKNOWN`. Only signals named by that
  candidate's prediction can confirm its effect. Workbench frame/ownership labels
  are heuristic; Workbench can diagnose a relocation-bearing canonical-exact object
  as a residual because its private target is a raw-word diagnostic object.
- POSIX process groups enforce timeout/cancellation. SIGINT/SIGTERM stop queued
  scoring work and terminate active stage/compiler groups. Windows is unsupported.
- Receipts are atomic and hash-checked. This first version intentionally has no
  resume or persistent cache: choose a fresh output directory each time.

## Reports, privacy and acceptance

`summary.json` is the complete **private** report, including all failures, strict
fields (including explicit notes), artifact references, effect checks, source/object
hashes and diagnosis summaries. `summary.md` has at most six detailed rows. Ranking
uses differing + extra words as a disclosed convenience, with unresolved/unverified
or error results visibly blocked. A tie does not imply identical emitted programs.

Only `public-summary.json` and `summary.md` are allowlisted review exports. The
former contains counts, statuses, hashes and numeric score fields, never arbitrary
compiler/diagnostic text. Do not publish the snapshot, target ELF, generated target
assembly, candidate objects, compiler logs, Workbench JSON or raw diagnosis. C
sources, tests and these sanitized summaries can accompany a tooling draft PR.

A zero is a **strict exact candidate**. Independent checker review, production-base
replay and image/ROM gates still control integration and coverage. Tooling tests and
calibration controls do not pass those gates. Useful findings and LLM time/token
accounting remain unknown until reviewed; no speedup claim is inferred.

## Tests

```sh
python3 -m pytest tests/cloud/test_hypothesis_batch.py tests/cloud/test_car_select_batch.py tests/conveyor/test_diagnose.py
```

The source-structure tests preserve the full TU, check every signed-byte guard
combination and ordered effects, cover floating boundaries/NaN, and perform C89
syntax checks when a host compiler exists. Adapter regressions cover invalid plans,
frozen identity/receipts, failure status separation, timeout descendants,
whole-object deduplication and public-export leakage. None is a ROM acceptance test.

## Implementation-readiness verification (2026-10-07)

On the pinned base in an isolated x86-64 Linux cloud checkout (4096-byte pages),
Python 3.12.14 and the existing IDO/GNU MIPS tools were executed successfully.

- The final control-only preparation accepted all 20 materialized source proposals.
- Two fresh baseline builds both returned canonical `0/90` differing words, zero
  extra words, and no unresolved, unverified or error evidence.
- Both complete ELF hashes were
  `25e1a8ed8c19fe15449772f5076681e00868131d5aea556b85aa5e158282080e`.
- The deliberately altered private ELF canary was rejected with `1/90` differing.
- A separate adapter smoke test used two **identical baseline-source controls**:
  both IDs were retained, compiled once, strict-exact, and grouped into one ELF.
  Retained Workbench diagnosis completed and was kept diagnostic-only.
- 38 focused adapter, source-structure and existing diagnosis tests passed. A
  separate focused review also exercised resistant descendant cancellation and
  cancellation of queued two-worker scoring jobs.
- The full 20-variant calibration has **not** run at this readiness checkpoint.
  No production source, image or ROM gate was changed or claimed.

Control preparation took 3.07 seconds and the separate bridge smoke took 5.06
seconds in this environment. These are setup/smoke measurements, not pilot costs
or a speedup claim.
