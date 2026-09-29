# F4 validation — 2026-09-29

Replaced the inactive `main`-branch workflow with `.github/workflows/verify.yml`
for pushes and pull requests to `master`. The old suppressed lint command is
gone. Command failures now fail verification.

## Approved protected-path policy

PRs reject changes to all of `asm/us/blob/**`, including `symbols.json`, plus
`src/blob/blob.ld`, every `*.lock.json`, `us.sha1`, and locked blob sources.
Locked groups protect their manifest and every file in its `files` list,
including context sources. The guard reads protection metadata from the PR's
base-branch commit, so editing the proposed lock or manifest cannot remove
protection. Deletions and moves count as changes.

The maintainer explicitly approved removing the proposed symbol-table exception:
CI cannot prove regeneration without the private Pi inputs. Maintainers
regenerate/verify protected files on the coordinator and commit directly to
master. Push CI still performs all verification checks; only the protected-path
gate is PR-only. This amendment is recorded in FIX-PLAN.md and CloudHandoff.md.

## Workflow and change selection

- Checkout the submitted head, with full history and recursive submodules,
  `contents: read`, and no persisted checkout credentials. The workflow uses
  ordinary `pull_request`, never `pull_request_target`.
- For PRs, compare merge base to submitted head. For pushes, compare the before
  SHA to head. A zero before SHA selects every tracked file. Invalid revisions
  fail rather than silently skipping checks.
- Install system and Python test dependencies in an Ubuntu 24.04 runner and
  a Python virtual environment. Cache IDO by OS, architecture, and setup-script
  hash; run F5's checksum-validating setup.
- Run both CloudHandoff scorer sanity checks, strict rescoring of changed
  singles/groups, `make check-matched`, and the repository-only Conveyor suite.
  Submission flags must be recorded on line 1; no `--allow-unverified` is used.

Checkout/cache options follow the official
[checkout v4 documentation](https://github.com/actions/checkout/blob/v4/README.md)
and [cache v4 documentation](https://github.com/actions/cache/blob/v4/README.md).

## Results

Full test command: `python3 -m pytest tests/conveyor -q -m 'not node_required'
--tb=short -o addopts=''`.

- Fresh repository-only checkout on watchman2: **499 passed, 13 skipped,
  5 deselected**. No ROM, extracted game image, or generated layout was copied.
  The five deselected tests require live compute nodes.
- Pi: **380 passed, 132 skipped, 5 deselected**; IDO cannot execute on this host.
- New CI helpers: **28 tests** on x86, including a real matching single-function
  submission that passes and a wrong-global mutant that fails.
- The protected-path CLI rejects a committed altered target `.word`. Tests also
  exercise every protected path category, base-lock/manifest tampering,
  deletions/moves, newer base-branch protection, and missing lock data.
- Both CloudHandoff sanity commands: `sound_handles_clear` and all three
  `resource_slot_clear` members print plain **MATCH**.
- `make check-matched`: all **26** static locks intact.
- `actionlint` 1.7.7 on `verify.yml`: passed. `git diff --check`: passed.

The repository-test fixture corrections are explained in
[F4-PREPARATION.md](F4-PREPARATION.md). Production closure behavior was unchanged;
the previously failing regression now explicitly constructs the historical DB
state it is intended to protect.

Remote validation used `watchman2:~/rush2049/tmp/f4-ci-i2TDI1cu/repo`; the shared
builder checkout was not modified. GitHub-hosted execution and cache service
behavior await the first push; this validation ran the commands on watchman2
and linted the workflow locally.
