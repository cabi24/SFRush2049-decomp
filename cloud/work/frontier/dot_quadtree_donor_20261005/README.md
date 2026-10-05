# Quadtree donor boundary: 19-word nonmatch

`func_800AC9BC`, **0x800AC9BC–0x800ACA9C**, is a complete 224-byte / 56-word
quadtree downward search. This packet is **research only**: no matching claim,
accepted bytes, image, compression, ROM, or gameplay coverage.

The existing kept O3 group reproduces **52/56 differing words**. The new complete
candidate improves this to **19/56**, with an exact 224-byte ELF function, all
relocations resolved, no owned data and no extra or missing words. The remaining
19 words concern integer midpoint calculation and its control scheduling. This
is not a strict match.

## Source evidence and bounded experiment

The algorithm donor is [Rush The Rock `stree.c:downleaf`, lines 1211–1236](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/stree.c#L1211-L1236).
Its pinned file hash is in `claim.json`. The matching quadrant convention,
bounds, midpoint locals, descent and output establish an algorithmic relationship;
they do not establish the original N64 declarations or compiler boundary.

The N64 body has 20-byte records, signed-half bounds at 4/6/8/10, unsigned-half
children at 12/14/16/18 and a child-selection mask at 3. Its scalar x/y formals are
signed halves. Unlike the donor's arithmetic shift, N64 midpoint division rounds
toward zero. Unlike the donor's positive-child test, N64 first checks the mask;
a selected child index zero returns NULL without writing the output quadrant.
A clear mask bit writes the selected quadrant and returns the current node.
The accepted `handbrake_apply` source independently corroborates the record view.

Historical Round 5 already exhausted roughly 60 iterative allocation controls.
This packet tests a distinct donor boundary rather than repeating that search:

- Literal recursive donor adaptation: O3 45/56, 244-byte ELF function. This fixes
  the former y-to-t0 input-register problem but repeats argument narrowing on
  descent and grows the body. O2 is also a nonmatch.
- Genuine iterative descent retaining consumed midpoint locals: 41/56, 228 bytes.
- Keep the midpoint sums and perform `/ 2` at their actual comparisons: 25/56,
  224 bytes. Every sum is consumed; no extra runtime work is added.
- Read the child directly at descent instead of retaining the donor's separate
  child local: **19/56, 224 bytes**. This fixes the final pointer-register block.

The fixed explicit-quadrant, inline-midpoint, guarded-child and compound-division
controls are retained under `controls/`. No blind parameter/allocator/reflow
sweep, dead read, pressure variable, fake padding, unsupported volatile, keeper,
stand-in, inline assembly, changed compiler recipe or scorer exception is used.
Source exploration is frozen. Reopening needs a concrete original integer
midpoint/source-boundary explanation for the remaining scheduling block.

## Verification

`verification.json` records all 14 single-function compiler rows and three
actual two-body O3 context controls. All controls use the stated O3 flags except
the two explicitly named `_o2` rows. The target never acquires a MATCH label.

- Exact ELF STT_FUNC extents are read independently with GNU readelf and nm.
  All body relocations are resolved with GNU ld and checked against the project
  relocator. GNU objcopy independently checks the complete linked text bytes.
  The candidate has two HI16/LO16 references to `D_80124EEC` and no owned literals.
- Full-extent accounting includes zero words inside a function. The 228-byte
  iterative control has one excess zero word that the canonical scorer's
  nonzero-excess counter does not report. Recursive controls similarly have
  larger true extents than their nonzero-excess counts. Nothing is cropped.
- The exact existing `handbrake_apply` body is extracted without edits and
  compiled with the archived, donor-recursive and final target sources. The
  wrapper remains a **216-byte MATCH** in all three controls. No existing group,
  protected source, flags or keep policy is edited. This is a real two-body
  regression, not full-game shadow compilation.
- **9,391 cases** agree among a separate tuple oracle, manifest-protected native
  words, the complete GNU-linked candidate and unchanged host C89 + UBSan.
  There are 18,782 interpreted runs and 9,391 primary host runs. All 56 native and
  candidate instructions execute; both outcomes of all six conditional branches
  execute. The unconditional branch is separately recorded.
- Cases cover all low mask combinations, ignored high mask bits, all quadrants,
  negative odd midpoint sums, signed-half extremes, noisy upper argument bits,
  zero-child failure/output preservation, deterministic forward trees, and
  chains through 512 nodes. Nodes and the table pointer are preserved, as are
  O32 saved registers and stack canaries; both real argument-home writes are
  checked. The host wrapper verifies complete node and table-pointer preservation.
- Four separately compiled wrong-contract host sources are rejected: floor
  rounding, inverted y quadrant, wrong mask selection and writing on failure.
  Unknown instructions and redirected argument-home writes fail closed.
- **740 selected tests pass**: five packet tests including a fresh compiler and
  complete behavioral replay, plus 735 scorer, target-integrity, submission,
  protected-path, setup and lock tests. All 402 static source locks are intact.
  The two scorer sanity examples pass.

## Reproduce

From the repository root, with the ordinary IDO/GNU toolchain available:

```sh
python3 cloud/work/frontier/dot_quadtree_donor_20261005/verify.py --output /tmp/quadtree-replay.json
python3 -m pytest -q tests/cloud/test_dot_quadtree_donor.py
python3 tools/cloud/score.py fn cloud/work/frontier/dot_quadtree_donor_20261005/candidate.c func_800AC9BC --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

The final command deliberately exits nonzero with `19/56 words differ`.
The proof command succeeds only when all asserted research outcomes and behavioral
checks pass. Whole-object hashes are build-path provenance: IDO `.mdebug` paths
can differ. Compare stable receipt fields, source hashes, complete linked body
hashes, all relocations and extents; do not mask instruction differences.

## Limits and ownership

Inputs require stable, fully accessible initialized records and finite descent
paths. Cyclic/corrupt graphs, arbitrary invalid pointers, concurrent mutation,
full caller execution, exact original N64 source declarations, full-game unit
compilation, image/compression/ROM gates and gameplay are not proved. Host-width
pointer transport is not an independent O32 ABI proof; the native executions
check that ABI. UBSan is enabled; no ASan result is claimed.

Only additive research C, proof/test scripts and notes are submitted. Native
bytes, raw assembly dumps and compiled objects remain outside this packet.
No matching submission is registered, because all claims are empty. Merging and
integration remain with the independent checker. No CI monitoring is requested.
