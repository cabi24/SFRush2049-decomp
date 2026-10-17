# StartEnvironment native reconstruction research

Research only: **18/248 differing words**, all stack/frame immediates; zero new
matching bytes. The full 992-byte function extent is exact. Native frame size
is 160 bytes; this natural reconstruction uses 112. No unused storage, padding,
register constraint, prototype change, or synthetic caller closes that gap.

The historical N64 label `music_control` at 0x800B1B48 is misleading. This is
StartEnvironment: it first creates and starts active cars, then creates their
shadow polygons and conditional multiplayer arrow polygons. Its real caller
is the PrePlay reconstruction currently labeled `display_list_flush`.

## What changed

The preserved B97 branch-call reconstruction is 172/248. One ordinary source
expression change, `flags | 0x8200` to `0x8000 | (0x200 | flags)`, reproduces the
native two-stage flag construction and leaves 18 differences. The compiler's
instruction/register sequence then agrees completely; only stack homes and
the frame displacements remain. This is a source-order finding, not a binary
patch, relocation mask, or byte-coverage claim.

An independent type audit also found an inherited C bounds problem. The old
view declared only two arrow records, but a four-window path may write more.
The corrected source exposes the genuine 21-entry visual array at offset 272
within each 952-byte car. Shadow starts at offset 512, arrows at 536, and the
unused-arrow cleanup endpoint is 584. This type repair produces a byte-identical
complete object. The genuine N64 dispatcher takes `(visual, s16 operation)`;
the different GameCube AnimateArrow declaration does not replace that contract.

## Donor evidence and limits

The accepted MAT3 reconstruction's complete visuals translation unit has SHA256
`c90126e3f37dd595bab483ae66b6bebc2933c592dfbf59e423552a2b660d5449`.
Its final checkpoint accepts StartEnvironment (220 bytes) and StartArrow
(576 bytes). StartShadow is an **unaccepted partial**, 1852 bytes with 18
scheduling differences. Its GameCube-specific strip allocation tail is not
present in the N64 function and is not transplanted.

The natural helper-decomposed N64 comparison is preserved separately in the
working research history: 33/248 after the flag correction, with a 120-byte
frame. The delivered 18-word lead is the native B97 reconstruction with the
flag and type-view corrections, not a claim that donor helper boundaries
caused the improvement. All native dimensions, constants, and the N64 depth
store are retained. Actual CreateCar, StartCar, and polygon allocation calls
remain external, with no fabricated callee implementations in the source group.

## Replay and controls

With stock IDO and MIPS binutils configured:

```sh
python3 cloud/work/frontier/mat3_environment_20261017/verify.py
python3 -O cloud/work/frontier/mat3_environment_20261017/verify.py
python3 -m pytest -q tests/cloud/test_mat3_environment_20261017.py
```

The replay pins all delivered source inputs and authenticated native target
hashes, checks the complete ELF symbol extent, and requires every residual to
change only a stack-based immediate. The old flag spelling is recompiled as a
172-word control. Explicit checks remain active under optimized Python.

IDO-emitted constants verify the real structures, array capacity, and offsets.
A wrong visual-array capacity is rejected by the emitted layout, rather than
by relying on the compiler's exit status for a negative-array declaration.

A separate field-level model agrees with both native and candidate MIPS over
512 fixtures, including all active-car masks, single/multiplayer modes, team
colors, zero through four windows, callback/owner registration, polygon flags,
cleanup, and saved-register/stack preservation. The tests cover 240 of 248
instructions. The remaining instructions are integer-division error paths.
CreateCar, StartCar, and NewPoly are deterministic boundary stubs; their own
behavior and N64 runtime execution are outside this test's claim.

Three independently compiled semantic negatives are rejected: wrong shadow
alpha, wrong arrow depth, and wrong team-arrow flags.

```sh
python3 cloud/work/frontier/mat3_environment_20261017/trace_fidelity.py \
  --traced-uopt /path/to/instrumented/uopt
```

The complete stock and traced objects are byte-identical. Raw target words,
assembly, traces, and binaries remain in ignored build storage. There is no
canonical installation, full integration, image, runtime, or ROM verification.

## Sealed-proof hardening

Protocol 2 validates the saved report before compilation and compares the entire
fresh result with it. The previous replay recomputed correct empty claims but
ignored altered saved claims, credit and comparison fields; an optimized replay
reproduced that gap before this repair. Source, compiler recipe, complete object,
18-word residual, layouts and bounded behavior results are unchanged.

The receipt now also binds the actual compiler stages (including cc’s ujoin subprocess), scorer and own-data
implementation, lock, protected manifests and symbol map, README and focused
test. The emitted inventory contains exactly one 992-byte function and no
unaccounted text. Four tests cover optimized full replay, 42 direct report/scope
refusals, a separate relocated checkout with 15 real entry-point binding/report
refusals, and the standard missing-IDO diagnostic. Ordinary missing-tool runs
skip compiler-dependent tests; REQUIRE_TOOLCHAIN=1 makes those skips failures.
The original source review is retained, with a separate review for this proof
change. These checks do not change the source or expand the stated model domain.
