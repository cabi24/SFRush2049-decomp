# Independent renderer-closure review

Reviewed exact commit `69a5a9f7ab7abfd70d4ecef4625ea6b57581fc9e` in a separate
worktree. **Research evidence passes; all four bodies remain NONMATCH.**
No matching-byte, accepted-byte, production-replacement or ROM claim follows.

Fresh unmodified canonical IDO compilation, independent GNU link and stock
UGEN diagnostic replay reproduce the frozen receipt (excluding elapsed time).
Nine focused tests for the wave-one and sibling-closure packets pass, including
optimized-Python checks and cross-packet import-state preservation. The source
is C89-compatible. `git diff --check` passes and the reviewed worktree is clean.

The exact whole-function results are 6F2C 2/149, 7180 6/296, 7620 4/190 and
7918 7/144 differing words. All 3,116 function bytes are covered, with contiguous
ELF STT_FUNC boundaries and only four final zero alignment bytes. All 223 code
relocations and five switch-table relocations independently resolve. The
20-byte table is checked against the authenticated image; the 12 compiled
rodata alignment bytes receive no owned-data credit. The final receipt contains
hashes and ownership metadata, not an ordered dump of the table's targets.

Independent inspection confirms each residual changes only an actual stack
frame/home immediate. Branch geometry, external addresses, ordered calls and
all other instruction fields agree. Private renderer register clobbers require
the genuine 7918 caller; the caller's native save/restore set is recovered.
The callback argument home follows its changed frame size. This is static
body/ABI evidence, not native gameplay execution or a standalone O32 replacement
proof. The source capacities, text-field extents and string termination remain
explicit hypotheses. Native address gaps do not establish exact capacities.
No capacity changes, padding or invented helper bodies were used in this review.

Two verifier findings were repaired before approval: temporary Python module
aliases and search paths now restore their prior state, and GNU re-sectioning
explicitly rejects overlapping/gapped/incomplete function ownership. Fresh
replay of the final repair and metadata-scrub commits passes. No remaining
blocker prevents publication as bounded NONMATCH research. Authentic source
or compiler-home ownership evidence remains necessary to reopen the frame
residuals without inventing capacities.

Reproduce from the reviewed repository root with IDO_DIR and GNU MIPS tools:

```
python3 cloud/work/frontier/dot_runtime_a_menu_renderer_closure_20261011/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_menu_renderer.py tests/conveyor/test_runtime_a_menu_renderer_closure.py -q
```
