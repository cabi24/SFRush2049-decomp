# Fourteenth source cut: two reviewed bodies

This frozen cut adds seven distinct attempted functions: **two strict matches /
1,316 B** and **five complete nonmatches / 3,620 B**. The matching sources are
`func_80019C8C` (580 B) and `func_8001DDE0` (736 B). Each has whole relocated-word
equality, a single exact-size ELF function symbol at offset zero, and no masks,
unresolved symbols, unverified relocations or relocation errors. Exact-head
hosted CI remains pending for this cut; local/peer proof is not CI or promotion.

The cumulative union is **236 new matching bodies / 26,576 B**, plus the separate
historical 12-byte getter. Across 390 unique attempted targets, 153 complete
nonmatches / 42,924 B and one 96-byte blocked source lead receive no match credit.
No rejected control is counted as another target.

## Source and baseline provenance

`reviewed_sources.json` binds each retained C file to its immutable independently
reviewed commit and SHA-256. The spatial-control, sequence-dispatch, constructor
pair and emitter-handle packets retain their original reviewed source bytes.
Five research residuals remain nonmatches, including whole bodies whose ELF
function sizes exceed the target even if nonzero excess-word counts are small.

This integration starts on verified master
`1018ab003251d41de0671284519a91b6b07c5a95`, preserving all checker-merged work and
its strengthened generated-target guards. The previous native inputs, specs,
scorer, compiler, layouts and locks remain identical. See
`master_reconciliation.json`. No merge, history rewrite, protected edit or D10
change is performed by this cut.

The exact source/evidence from published PR #73 is inherited without C changes.
Its three matching bodies add **zero new cut14 credit**; green exact-head CI is
recorded in `../wave13/ci.json`. While PR #73 remains a separate draft, the diff
against current master contains its three inherited matches plus this cut's two
new matches. The new branch does not modify PR #73.

## Material emitter defined-path qualification

The exact `func_8001DDE0` body deliberately retains the native/source family's
five genuine uninitialized float locals. A consumed output must have been
written by the real helper or retained from a genuine earlier iteration. The
original global producer/listener invariant is **unproved**. Empty-list paths can
leave outputs unwritten; no default initialization or universal safety claim is
introduced merely because compiled bytes match.

Metadata-only definedness checks reject unwritten-output and unchecked-capacity
fixtures before executing plain C. Safe fixtures also exercise real prior-
iteration carry. Original fade data remains unknown and externally declared;
the final DC08 call is an opaque unit-test boundary. The peer receipt and tests
state finite-value, allocation, callback and capacity limits. Exact body equality
is separate from full-runtime, concurrency, hardware or cartridge safety.

## Reproduction

Use the preserved pinned IDO 5.3 toolchain via `IDO_DIR`:

- `python3 cloud/work/boot_tail/scripts/verify_wave14.py --check`
- `python3 cloud/work/boot_tail/scripts/verify_wave13.py --check` (inherited bytes)
- `python3 cloud/work/boot_tail/scripts/generate.py --check`
- `python3 -m unittest discover -s cloud/work/boot_tail/tests -q`
- `python3 tools/cloud/check_submissions.py --base 1018ab003251d41de0671284519a91b6b07c5a95 --head HEAD`
- `python3 -m tools.cloud.guard_paths --base 1018ab003251d41de0671284519a91b6b07c5a95 --head HEAD --lock-revision 1018ab003251d41de0671284519a91b6b07c5a95`

The independent checker owns merging and any future cartridge promotion. Later
sequence-event work is excluded from this frozen source cut. No ROM bytes,
raw native dumps, compiled objects, credentials or unrelated data are included.
