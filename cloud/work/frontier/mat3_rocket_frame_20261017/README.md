# MAT3-informed Rocket and Frame research

Research only, with empty claims and zero new matching bytes. This packet does
not replace a canonical group or change the inventory. It preserves full real
caller/helper context while measuring two unfinished N64 functions:

- AnimateRocketFlame / entity_tick_main, 0x800930A4: **47 of 671 words differ**.
  Every difference is a stack/frame immediate. Opcodes and register operands
  agree; body length is exact. Frame: 352 bytes versus native 368.
- AnimateFrame / drone_ai_update, 0x80093B20: **537 of 820 words differ**.
  This remains a substantial partial and receives no match credit.

Both strict comparisons have zero excess nonzero words, unresolved symbols,
unverified references, or scorer errors. Source literals are independently
verified against native data; no external symbols stand in for private floats.

## What changed

The accepted MAT3 Rocket body establishes two separate position passes: first
frame position plus twice the up vector, then addition of the forward offset.
Preserving that position-plus-product order, while retaining the N64 multiply
operand order, changes the Rocket partial from 219 to 47 differing words. The
rest of the function is unchanged. This removes the persistent floating-point
temporary-register permutation; it does not repair stack allocation.

The remaining immediates comprise one frame-allocation delta of -16, 31
frame/local deltas of +16, and 15 position-array deltas of +20. Those figures
are descriptive, not instructions to add padding. The donor's unused float r
is disclosed and retained; removing it changes the frame to 344 and leaves
47 stack-only differences. No spare arrays, empty guards, added dummy
parameters, or frame-filling helpers were introduced to claim a match.

Two independent correctness repairs are retained in the Frame draft:

1. D_8011B468, the track-base-color table, contains 32-bit integers. The older
   byte-array declaration read the leading zero byte of every one of its 19
   native big-endian entries, so every track selected palette entry zero.
2. The car has four 16-bit palette samples at 0x344, current IR color at 0x34C,
   and previous IR color at 0x350. The old five-sample layout incorrectly placed
   those colors at 0x350/0x354. Later fields and the 0x3B8 car stride are retained.

The accepted SetSheenColor boundary and color helpers remain genuine context.
Substituting the already-locked direct-store reconstruction of SetSheenColor
in this group collapses Frame's body and frame, so that standalone source is
not a substitute for its caller's original nested inline structure. This was
a diagnostic negative, not a reason to modify the canonical locked helper.

## Provenance and limits

Base: 58425072b49e95f0fbe5c4c239988fb4885f7c5c.
Accepted MAT3 visuals translation-unit SHA256:
`c90126e3f37dd595bab483ae66b6bebc2933c592dfbf59e423552a2b660d5449`.
Task-11 checkpoint-60 records AnimateRocketFlame PASS / 1908 bytes,
AnimateFrame PASS / 2276 bytes, and SetSheenColor PASS / 96 bytes.
The related GetCarsIRColor is a retained **unaccepted** MAT3 partial, not an
accepted donor. Its caller establishes the boundary only. N64 palette accesses,
field layout, constants, and absence of GC color smoothing remain authoritative.

The complete group derives from the genuine visual/tire callback group and its
real Rocket/Frame caller pair. There are no fabricated keepers or substitute
callers. Existing WingFlame (already published in PR #386), tire animation, and
buffer swap remain strict matches here and are not new claims. Other context
results are explicitly recorded, including matrix_scale_apply's two excess
words and standalone O3 helper differences. This packet is not installable as
a new exact-match group.

## Verification

With installed IDO 5.3 and MIPS binutils as documented in CloudHandoff.md:

```sh
python3 cloud/work/frontier/mat3_rocket_frame_20261017/verify.py
python3 -m unittest discover -s cloud/work/frontier/mat3_rocket_frame_20261017 -p 'test*.py'
python3 -m pytest -q tests/cloud/test_mat3_rocket_frame_20261017.py
```

The replay pins complete source inputs and native target hashes, compiles all
real context, checks both partial scores, checks preserved exact callbacks,
and proves all 47 Rocket residuals use the stack register with only an
immediate changed. A deliberately wrong translucent alpha (128 to 129) produces
48 differences, exactly one outside that stack-only class. Frame and preserved
callbacks are unchanged by this negative control.

The two host tests validate all 19 native table-entry access widths, layout
assertions, and near/interpolated/far palette branches. They establish draft
semantics and native layout evidence, not N64 runtime behavior.

```sh
python3 cloud/work/frontier/mat3_rocket_frame_20261017/trace_fidelity.py \
  --traced-uopt /path/to/instrumented/uopt
```

The stock and instrumented compiler produce byte-identical complete objects;
trace_fidelity.json binds this to the frozen input hashes and strict object.
Raw listings, objects, traces, and negative-control builds remain in ignored
build storage and are excluded. No canonical build, integration, runtime,
image, cartridge, or full-ROM verification is claimed.

The observed scorer hash is informational only; replay does not pin shared tools.

### Optimized-interpreter guard check

Verification and trace-identity conditions use explicit exceptions, so `python3 -O` cannot remove them. The focused pytest additionally runs an optimized interpreter and confirms rejection of an unverified relocation, a false matching claim, a changed source receipt, and changed trace input/object bindings. Normal and optimized full replays retain the same 47/671 Rocket and 537/820 Frame partials and the same complete object hash.
