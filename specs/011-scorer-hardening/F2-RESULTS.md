# F2 validation — 2026-09-29

`tools/cloud/score.py` resolves R_MIPS_26 and REL HI16/LO16 pairs using
the committed symbol table, with the splice resolver's address-name fallback.
Calls encoded as `.text` offsets map through the owning function to its target
address. Resolved instructions are compared in full.

Local data-section references remain explicitly unverified. They exit nonzero
unless `--allow-unverified` is supplied. That flag never permits differing
words, unresolved symbols, unsupported relocations, or unpaired HI16 records.
`CloudHandoff.md` documents the new behavior.

## Acceptance

On watchman2, in an isolated scratch checkout of the committed targets at
`cce96df`, with the F2 scorer and tests overlaid:

```sh
IDO_DIR=/home/cburnes/rush2049/repo/tools/ido-static-recomp/build/out \
  python -m pytest tests/conveyor/test_cloud_score.py -q
```

**146 passed**: 17 compiler-independent cases, 123 locked single functions
with their recorded flags, both groups' members, and four mutants of
`sound_handles_clear` (wrong addend, unknown global, unknown callee, and known
but incorrect global). The group tests judge members directly; F7's CLI
context handling and temporary-directory cleanup remain separate work.

On the Pi:

```sh
python3 -m pytest tests/conveyor/ -q -m 'not node_required' --tb=short -o addopts=''
```

**324 passed, 129 skipped, 5 deselected, 1 pre-existing failure**:
`test_populate_keeps_suffix_row_conflicted_against_discovered_extent` in
`test_closure.py`. IDO cases are skipped on the Pi and passed on watchman2.
Live-node tests were excluded; they do not exercise the cloud scorer.

## Target issue encountered and resolved separately

The initial target snapshot contained seven duplicate function sections in
`blob_8010a534.s` and `blob_8010a7a4.s`; six copies were identical, but
`steering_apply` differed. The old scorer concatenated the two copies of
`struct_callback_init`, causing the same failure with and without F2.

Concurrent commit `4b44a75` regenerated the target regions and removed these
files. F2 was then tested in a fresh scratch directory against the corrected
snapshot. F2 makes no changes to generated targets, locks, linked C, or the
target reader's duplicate handling.
