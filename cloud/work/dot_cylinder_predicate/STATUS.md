# Typed cylinder-proximity predicate research

**NONMATCH: 45 of 80 complete native words differ. No match or ROM coverage claim.**
`func_8010C448` spans 0x8010C448–0x8010C588 (320 bytes). The candidate
function is 304 bytes, with a 24-byte frame versus the native 32-byte frame.
The final four native word positions are missing from the candidate; this is
included in the 45 differences. There are no excess words or alignment bytes.

## Useful progress

This is a typed, alias-tested replacement research seed for the old registered-heads
raw-byte-cast variants. Those natural array variants report 67/80 differences;
the new natural source reaches 45/80 with inferred record views and a real radius
snapshot. No historical files are replaced and no accepted source is modified.
The old generated seed loaded radius after copying input coordinates. The native
function loads it before the enabled check, so the new source explicitly snapshots
it before the early return. It also avoids treating a scalar global as a large
byte array or repeatedly casting unrelated pointers to float pointers.

The record views express only evidenced offsets: enable byte at
`D_8014AA3A + index*0x808`, and XYZ floats at `player_array + index*0x3B8 + 8`.
Unknown byte arrays describe actual observed strides, not stack padding or extra
operations. Full record meanings and population count remain unestablished;
the test allocation of eight records is a bounded fixture, not a count claim.
The index is a signed halfword through a0; point, radius and optional output
pointers arrive through a1, a2 and a3. No parameters or helper calls were invented.

On a disabled record, return zero without changing output. Otherwise subtract the
query point from the selected position, extend the supplied radius by 3.5, and
optionally write `dz*dz + dx*dx - radius*radius`. Reject strictly positive radial
error, then accept only a vertical delta strictly between -2 and 18. Radial equality
is accepted; vertical equality is rejected. Ordered comparisons retain the native
NaN behavior. The output may alias any input float because required values are
captured before the output store. Accessible, properly aligned inputs and a valid
record index are preconditions, including a readable radius on the disabled path.

## Experiments and remaining blocker

Natural O1/O2 controls covered local array order, genuine scalar versus vector
locals, and placement of the selected player pointer calculation. Moving the
pointer calculation before the radius read and declaring delta before position
improved the initial typed 60/80 to 45/80 at O2. O1 was worse (79–80 differences
and excess words). Scalarized variants were worse. Workbench diagnosis reports
structural differences, four fewer candidate instructions, and 24 versus 32 bytes
of stack. Later native float scheduling/materialization and boolean-return
structure differ. No artificial stack variables, forced volatility, helper calls,
pointer laundering, or invented arguments were used to force these differences.
The complete unmasked differences are in verification.json. Native target words
are authoritative; workbench relocation warnings arise because its target object
contains resolved words rather than ELF relocation records.

No arcade equivalent is established: the reference source repository is absent
in this checkout. This is N64-assembly-grounded research with inferred names.

## Reproduction

With the pinned IDO 5.3 static-recomp compiler exposed by IDO_DIR and GNU MIPS
binutils on PATH:

```sh
python3 cloud/work/dot_cylinder_predicate/replay.py
python3 tools/cloud/score.py fn cloud/work/dot_cylinder_predicate/func_8010C448.c func_8010C448
python3 cloud/work/dot_cylinder_predicate/verify_semantics.py
cc -std=c99 -O2 -ffp-contract=off -Wall -Wextra -Werror -DHOST_MAIN \
  -fsanitize=address,undefined cloud/work/dot_cylinder_predicate/host_semantics.c -o /tmp/cylinder-test
ASAN_OPTIONS=detect_leaks=0 /tmp/cylinder-test
python3 -m pytest tests/conveyor/test_dot_cylinder_predicate.py
```

The canonical scorer exits 1 for this expected NONMATCH. replay.py exits zero only
when the documented residual is reproduced. It uses a fresh direct IDO compile,
GNU ld/objcopy full-word replay, canonical relocation agreement, and explicit
body/extent assertions. No words or relocations are masked. The linker uses
SUBALIGN(4) to place the function at its native 8-mod-16 address rather than add
an accidental eight-byte prefix.

## Validation and limits

- 7,620 native-instruction-interpreter versus host-C differential cases pass.
  Includes all eight fixture indices, disabled/positive/negative enable values,
  strict threshold neighbors, signed zero, infinities, quiet NaNs, randomized
  bit patterns, and nine output modes: null, separate, or seven input aliases.
- 20,000 ASan/UBSan host cases pass, checking the complete global-array canaries.
  Leak detection alone is disabled because this sandbox uses ptrace; no heap is used.
- Two checked-in pytest regressions rerun differential and UBSan tests in CI.
- The interpreter fails closed on unsupported opcodes, unmapped/unaligned accesses,
  nonterminating control flow, and callee-save/stack violations; it models branch
  delay slots and likely-branch annulment. Unknown-opcode/unmapped-memory negative
  controls are included.
- IEEE binary32 host round-to-nearest results and NaN classes are compared.
  N64 FCSR exceptions, signaling-NaN payload propagation, concurrency, inaccessible
  inputs and complete real-game allocation bounds are not established.
- Source, tests and verification are research artifacts only. No image splice,
  compressed-stream identity, full-ROM SHA-1 gate, accepted coverage or merge.

Independent peer review passed: fresh separate compile/GNU link and native-word
parse reproduced 45/80 and 304 versus 320 bytes. Source/ABI and all offsets were
reviewed; the reviewer independently reran all 7,620 differential and 20,000
sanitizer cases. See independent_review.json. Repository suite passed with 1,309
passes, 41 skips and 9 deselections. All 161 static locks remain intact; canonical
single and three-member group scoring controls still match. No target or gate
was changed. Green CI includes the two checked-in research regression tests;
it does not mean the C matches native instructions.
