# Natural three-row matrix rotation research

**NONMATCH: 5 of 42 full native words differ. No match or coverage claim.**
The complete target is `func_800B5898`, 0x800B5898–0x800B5940, 168 bytes.
The candidate emits one 168-byte function and 8 zero text-alignment bytes.
Flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

## Useful change

The historical `near_miss_B7/func_800B5898_best.c` reached the same residual by
adding an unused `reserved` float before its sine temporary. This reconstruction
removes that artificial local: declaring the genuinely used cosine before sine
naturally places the saved sine at stack offset 24 in the native 32-byte frame.
All locals carry real computation. There are no forced registers, invented
arguments, helpers, volatile accesses, integer-pointer casts, or padding locals.
Historical candidates remain untouched.

The five remaining differing words are at +0x4C, +0x70, +0x78, +0x84, +0x88.
They are a consistent exchange of the roles of f14 and f16: saved sine and the
first output. Instructions, frame, control flow, arithmetic grouping, memory
accesses, and complete extent otherwise agree after all six relocations resolve.
The full-word comparison is deliberately not masked or weakened.

## Behavior and interface

The two inputs are an angle in O32 f12 and a matrix pointer in a1, confirmed in
the native function and direct caller `func_800D6914` at word 124. If angle is
below the lower threshold or above the upper threshold, call sinf and then cosf,
then rotate the Y/Z pair in each of three consecutive 3-float rows. X is retained.
Both original Y and Z are captured before either output store. Threshold equality
and a NaN angle skip both calls and all stores under normal ordered comparisons.
The function returns no value. No arcade equivalent is established because the
reference repository is absent here; this is N64-assembly-grounded reconstruction.
Native threshold data values and N64 libm outputs were not inferred or fabricated.

## Checks

- Canonical strict scorer and separate clean-directory compiler replay both
  confirm exactly 5/42 differing full words, no unverified/unresolved relocation,
  relocation errors, or nonzero excess text.
- Independent peer uses a separate direct IDO compile plus GNU ld and objcopy:
  all six relocations resolve, complete native body and the same five offsets
  verified. Independent source/ABI review approves this as NONMATCH research.
- Host ASan/UBSan test passes 107,520 cases. Tests include lower/upper equality,
  near thresholds, reversed/NaN/infinite bounds, signed zero/NaN/infinite angle,
  finite and special matrix elements/coefficient outputs, sin-before-cos and
  argument identity, all three rows, unchanged X values and surrounding canaries.
- Repository CI-style tests: **1,307 passed, 41 skipped, 9 deselected**.
  All 161 static locks and 664 blob/group source hashes remain intact.
  Initial local run lacked binutils on PATH; rerun with the existing toolchain passed.
- C89 pedantic syntax and warnings pass. Host harness uses C99 and controlled
  sin/cos mocks; it supplements assembly review, not N64 libm execution.

Reproduce the strict NONMATCH (exit 1 is expected):

```sh
python3 tools/cloud/score.py fn cloud/work/dot_matrix_rotation/func_800B5898.c func_800B5898 --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

Reproduce host semantics:

```sh
cc -std=c99 -Wall -Wextra -Werror -O2 -ffp-contract=off -fsanitize=address,undefined cloud/work/dot_matrix_rotation/host_semantics.c -o /tmp/rotation-test
ASAN_OPTIONS=detect_leaks=0 /tmp/rotation-test
```

Leak detection is disabled for sandbox ptrace; the test uses no heap allocation.
All generated native objects and complete disassemblies remain ignored build
artifacts. Only this research directory is added; accepted source, target bytes,
locks, scorer, build wiring, and coverage totals remain untouched. No ROM/image
splice, compressed-stream identity or full-ROM SHA-1 verification was performed.
