# F4 preparation and policy decision — 2026-09-29

Historical preparation notes. The maintainer approved the policy below and F4
is now implemented. See [F4-RESULTS.md](F4-RESULTS.md) for final validation;
the counts here describe the earlier preparation run.

## Implemented preparation

`tools/cloud/check_submissions.py --base <commit> --head <commit>`:

- Uses a NUL-delimited Git diff with rename detection disabled so both sides
  of a move are considered. Requires the checkout to be at the head revision.
  An all-zero base checks every tracked path, for a branch's first push.
- Scores changed `cloud/matches/<function>.c` using the required first-line
  `/* flags: ... */` comment. Flags are passed as an argument, never shell code.
- Scores each changed `cloud/work/ipa-groups/<group>/` once, including changes
  to metadata. Fully deleted submissions are skipped; a remaining group
  without `group.json` fails.
- Runs strict `score.py` without `--allow-unverified`, preserves any failure
  even if a later submission matches, and fails malformed submission headers.

Tests cover rename/deletion selection, flags, group deduplication and incomplete
groups, failure propagation, and real IDO acceptance/rejection. The real-compiler
case copies `sound_handles_clear` into an isolated submission: the original
passes and the unresolved wrong-global variant fails.

Repository-test preparation repairs dependencies on coordinator-local state:

- Synthetic all-passthrough blob-build tests now explicitly use `spliced={}`.
  Their real assembler/linker and altered-word image-gate assertions remain.
- The live-layout test skips only when its generated layout is absent; the
  synthetic layout tests still run everywhere.
- Static supersession fixtures supply an empty extracted image rather than
  requiring `build/game_code.bin`.
- The closure regression seeds the historical DB state directly. Current
  `populate` repairs the stranded head before closure can discover it, which
  invalidated the old setup. The test now verifies closure discovers the
  container and that a later population pass preserves the suffix conflict.
  No production closure behavior was changed or failure marked expected.

## Validation

Test command on both hosts:

```sh
python3 -m pytest tests/conveyor -q -m 'not node_required' --tb=short -o addopts=''
```

- Pi: **362 passed, 132 skipped, 5 deselected**.
- watchman2: **481 passed, 13 skipped, 5 deselected**, in a fresh Git-bundle
  clone with initialized submodules, no ROM, no extracted game image, no
  generated layout, and a fresh Python environment. IDO is the pinned download
  validated during F5. The five deselected tests require live farm nodes.
- `make check-matched`: all **26** static locks intact in the clean checkout.
- `sound_handles_clear` and the three `resource_slot_clear` group members:
  plain **MATCH** using the CloudHandoff sanity commands.
- `git diff --check`: passed.

The initial clean-checkout run had ten failures. Nine were the fixture/data
issues above; the remaining integration test needed the toolkit's Python
dependencies installed. Required Python test dependencies are `pytest`,
`graphviz`, `pycparser`, and `toml`. CI also needs MIPS binutils and cross GCC,
native build tools/cpp, curl, and initialized m2c/decomp-permuter submodules.

Scratch checkout: `watchman2:~/rush2049/tmp/f4-ci-i2TDI1cu/repo`.
No changes were made to the shared builder checkout or locked game sources.

## Policy decision: verifying a regenerated symbols.json

F4 requires rejecting PR changes to `asm/us/blob/**`, with an exception for
`symbols.json` regenerated using F1. F1 uses the coordinator's private layout
and symbol context. GitHub CI cannot regenerate that output from this repo.
The `generated_by` field alone proves nothing: a PR can retain it while
changing a symbol address, causing strict scoring to trust the wrong address.

The brief's ground rule says: "If a fix turns out to need a design change not
described here, stop and write up what you found instead of improvising."
The missing verification mechanism makes this such a design decision.

Approved policy: reject all protected-path edits in PRs, including
`symbols.json`. Maintainers regenerate protected files on the Pi and commit
them directly to master. Push CI still runs scoring, sanity checks, lock checks,
and repository tests; the protected-path gate applies to PRs.

Alternative: define a maintainer-approved PR exception with explicit scope and
approval bound to the exact head commit. That requires an approval mechanism
not specified in the brief; do not treat a PR-controlled marker as approval.

The maintainer accepted the recommended policy on 2026-09-29. The implemented
workflow rejects all protected-path PR edits, with no symbol-table exception.

## Implementation checklist (now completed)

1. Add the protected-path checker, deriving locked sources from the **base**
   lock file. Protect group manifests and their locked C files too. Reject
   deletions and moves just like edits; test an altered target `.word`.
2. Replace `build.yml` with `verify.yml`, triggered by pushes and PRs to master.
   Fetch history/submodules with a read-only token and no persisted credentials.
   Compute PR changes from the merge base and push changes from the before SHA.
3. Install the dependencies above; cache IDO using the setup script hash and
   run F5's checksum-validating setup on cache misses. Run the two sanity checks,
   changed-submission scorer, `make check-matched`, and the repo-only pytest
   command above. Fail commands normally, without the old `|| echo` suppression.
4. Test the complete workflow's selection/path gates, record results, and commit
   all F4 changes together as required by the implementation brief.
