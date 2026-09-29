# F6 validation — 2026-09-29

## Implemented behavior

- `blob_tu symbols` and `blob_tu generate` write deterministic `SHA256SUMS`
  alongside the symbol table. It covers the exact bytes of all 26 assembly
  region files and `symbols.json`, using standard `sha256sum -c` format.
- `score.py` checks the region-file set and each region's bytes before using
  targets, and checks the symbol table before resolving addresses. Missing,
  extra, altered, unlisted, or unhashed inputs fail. Malformed/duplicate
  manifest entries also fail. Cached targets are rechecked, and parsing uses
  the same bytes that passed hashing. `--allow-unverified` cannot bypass this.
- `.claude/settings.json` configures the standalone standard-library hook
  `.claude/hooks/protect_files.py` for `PreToolUse` on `Edit|Write`. It denies
  `asm/us/blob/**`, `*.lock.json`, `us.sha1`, `src/blob/blob.ld`, and
  `tools/cloud/score.py`, including relative paths and symlink aliases.
  Malformed requests fail closed. Other paths return without overriding normal
  permission handling. Bash, Read, and the generation scripts are unaffected.
- Successful cloud setup configures this checkout's local Git setting
  `core.hooksPath=.githooks`, including when the compiler is already installed
  or setup is invoked from another directory.

The hook uses exit 2 with a reason on stderr, following the official
[Claude Code PreToolUse exit-code contract](https://code.claude.com/docs/en/hooks#exit-code-2).
Tests invoke the configured command with actual hook JSON; they do not require
a live Claude session. This protects the specified tool operations. The
manifest is checked against committed hashes, not an external signature;
F4 separately rejects PR changes to both the targets and their manifest.

## Validation

- `blob_tu symbols` run twice: identical manifest bytes, SHA-256
  `f0c19b8a469106ac0cf93c1a44f941d36a05b3fab818b9dfab04a529faa18e96`.
  Existing assembly targets and `symbols.json` were unchanged by F6.
- `sha256sum -c SHA256SUMS`: all **27** entries pass. The scorer reads
  **1,165** target functions and **4,678** symbol addresses in this revision.
- The full repo-only Conveyor suite, with `-m 'not node_required'`:
  - Pi: **426 passed, 132 skipped, 5 deselected**.
  - Fresh x86 checkout on watchman2: **545 passed, 13 skipped, 5 deselected**.
  The five deselected tests require live compute nodes; IDO tests run on x86.
- All locked single-function and group-member scorer regressions pass. Both
  CloudHandoff sanity commands print plain MATCH. `make check-matched` confirms
  all **26** static locks remain intact.
- Real IDO tamper experiment in an isolated temporary checkout: the original
  `sound_handles_clear` scores MATCH; changing one `.word` in
  `blob_80086a50.s` makes the CLI exit nonzero with `SHA-256 mismatch`, even
  with `--allow-unverified`.
- Hook tests deny both Edit and Write on every protected path category,
  including the specified `asm/us/blob/blob_80086a50.s`, absolute/relative
  paths, parent-directory traversal, aliases into protected directories, and
  replacement of a protected symlink. Unprotected files remain available.
- Setup tests confirm Git-hook selection from another working directory and
  paths containing spaces. Existing F5 corrupted-download checks still pass.
- `bash -n tools/cloud/setup.sh` and `git diff --check`: passed.

Validation includes the concurrent maintainer commit `d41dbb8` that regenerated
the regions after discovering additional functions. An initial run against the
older F4 scratch checkout correctly rejected its different target set. The
final run used a fresh clone of the current revision at
`watchman2:~/rush2049/tmp/f6-ci-ya7TJQfU/repo`, without private image/layout/ROM
data. The shared builder checkout and pre-existing dirty m2c submodule were
left untouched.
