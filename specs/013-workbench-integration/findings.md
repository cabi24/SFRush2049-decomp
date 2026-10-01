# Workbench integration findings

2026-10-01, base checkout `417b1251`. Shared pilot outputs were left alone.

## A: pipeline diagnosis

Implemented `tools/conveyor/pipeline/diagnose.py` with `one` and `triage`.
Coordinator access is SQLite `mode=ro`; blob reads verify SHA-256. Source selection
prefers near-miss `base.c`, then `cloud_worklist._best_source`, with explicit source
and flags overrides. The exact first-line flags comment overrides coordinator
flags. Shim includes are expanded before sending the source.

Compilation batches mixed flagsets into one SSH request per batch. Each request
uses a private temporary directory, invokes the toolkit's IDO compiler directly,
and returns ordinary objects plus per-function compiler errors. No section
renaming, shared splice staging, function-status updates or lock writes occur.
Workbench runs as a subprocess. JSON reports record source/target/candidate hashes,
flags, source origin, diagnosis and compact summary. Failed builds produce explicit
`diagnosis-failed` reports and a nonzero command exit code; stale objects are never
used as successful results.

The summary keeps positional relocation-aware and raw word differences separate.
The latter is an object-word count, **not** proof from the project's strict linked
image scorer. Ownership evidence remains labelled heuristic when that is what the
workbench supplies. Frame delta is candidate minus target. Actual vendored verdict
names are preserved: a synthetic constant residual is `constant`, whereas the
prompt's illustrative list says `constant-mismatch`.

`VERDICTS.md` rendering is deterministic: closest positional difference first,
function-name ties, errors last, class counts, and tool revision/date in the header.
An uncommitted tool is explicitly labelled uncommitted. The current file is a
**three-function failed trial, not an accepted classification campaign**.

### Validation and blockers

- Twelve new tests cover JSON mapping, missing frame information, batching,
  mixed flags, compiler/transport failures, stable Markdown, source selection,
  read-only DB access, content verification, real synthetic GNU MIPS objects,
  and executing the actual remote batch program against a synthetic compiler.
- Combined new diagnosis and pending switch-head tests: 32 passed, exit 0,
  captured separately in `/tmp/diagnose-tests.exit`; log `/tmp/diagnose-tests.log`.
- Python 3.9 syntax parsing passed; module uses stdlib only.
- `git diff --check` passed.
- Real command `python3 -m tools.conveyor.pipeline.diagnose triage --limit 3`
  reached source/target selection and the batched builder invocation. All three
  failed with `ssh: Could not resolve hostname watchman2: Temporary failure in
  name resolution`. Exit 1 captured separately in `/tmp/diagnose-real.exit`;
  log `/tmp/diagnose-real.log`. No real classifications were invented.
- The broader `tests/conveyor` run exited 1 (separate capture
  `/tmp/diagnose-suite.exit`). Socket-dependent tests fail with `Operation not
  permitted`; integration tests that mutate the live coordinator fail because
  its database is read-only. One known-pairing ranking assertion also fails on
  the current coordinator data. This is not a passing full-suite gate.
- This session permits reading `.git` but not writing it, so no commits can be
  made. The builder/network restrictions also prevent real IDO acceptance.

Task A is **not accepted**: the complete real-data table and the three reference
verdict comparisons remain outstanding, as does its commit. Once a permitted
builder is available, run:

```sh
python3 -m tools.conveyor.pipeline.diagnose one func_8008A704
python3 -m tools.conveyor.pipeline.diagnose one func_800CCE5C
python3 -m tools.conveyor.pipeline.diagnose one func_800966D8
python3 -m tools.conveyor.pipeline.diagnose triage
```

Global options precede the subcommand, for example `--builder Rocky` or
`--data /path/to/private/coordinator`. Expected pilot classifications are schedule,
allocation/frame (delta 8), and structure respectively. Compare the generated
reports to independent manual workbench runs before accepting the table. After
committing the tool, regenerate the table to give its header a real tool revision.

## B and C sequencing

B has not been started: the prompt requires finishing and committing A first.
No ROM rebuild, splice, live registration or lock mutation was performed here.

Before the new A/B/C prompt arrived, the next-head lane produced pending changes
in `targets.py` and `tests/conveyor/test_heads_switches.py`. These remain separate
from A. A read-only extracted-image audit proved these six switch extents:

| Head | Bytes | Entries | Table |
|---|---:|---:|---|
| 0x8010221C | 548 | 7 | 0x80124848 |
| 0x80102F30 | 3560 | 7 | 0x80124864 |
| 0x80104B14 | 2404 | 8 | 0x80124880 |
| 0x80105480 | 1780 | 5 | 0x801248A0 |
| 0x8010D3C0 | 704 | 11 | 0x80124958 |
| 0x8010D680 | 476 | 11 | 0x80124990 |

These are dry-run proofs only: **none was registered**. The seventh, 0x80104704,
still refuses because its branch at 0x80104A48 reaches 0x80104A58, the existing
`highscore_entry_anim` head. No shared-tail extent has been registered.
ROM/lock/group acceptance gates for B/C have not been run in this session.

## Acceptance of Task A (coordinating session, 2026-10-01, outside the sandbox)

Run from a shell that can reach the builders: `python3 -m tools.conveyor.pipeline.diagnose --builder Rocky one <fn>`.
- `func_8008A704` -> `schedule`, owning pass `g0-scheduler`, lever `line-order`, frame delta 0 (manual run: schedule).
- `func_800CCE5C` -> mixed constant/register, frame delta -8, lever `drop-a-declared-local` (manual: allocation + frame -8).
- `func_800966D8` -> mixed structural/register, no known lever (manual: structure-mismatch + register).
- `triage` over the near-miss candidates: exit 0 in about 1 minute; `cloud/work/near-miss/VERDICTS.md` is now a real table.
- Tests: `test_diagnose.py`, `test_heads_switches.py`, `test_heads.py`: 49 passed, exit 0. Full `tests/conveyor` run before commit.
The header of VERDICTS.md still says "uncommitted": regenerate after this commit. Tasks B and C remain open (see specs/014).
