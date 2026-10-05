# Camera/path helper contracts, 2026-10-05

## Result

**NONMATCH. No matching, splice, cartridge, or coverage claim.** This packet
reopens `func_800D348C` with its genuine parent `stunt_combo_display` and preserves
a useful typed source baseline, tests, and complete-object rejection evidence.
The source unit is not yet a complete IPA closure. It must not be promoted.

Base: `f88dc3cb`, freshly fetched `origin/master`. No checkout `AGENTS.md` was
present; the supplied owner instructions and repository matching/frontier rules
were followed. The parent assigned the two principal ranges and confirmed that
no other current worker owns the additional camera context. Unpublished work on
other machines cannot be observed here. No protected target, context generator,
scorer, accepted source, lock, or compiler recipe was changed.

## New contract evidence

- `func_800D348C`: **0x800D348C–0x800D3B20, 1,684 bytes**. Its only native direct
  caller is `stunt_combo_display`, at **0x800D40A4**. The three actual source
  arguments are a car pointer and two output pointers. The caller installs them
  in **s0 / s2 / s3**. The helper consumes the car and writes node/point results
  through s2/s3. It uses a **200-byte frame**, saves only ra, and clobbers s1 and
  f20–f26 without local saves. It is not an ordinary-ABI standalone candidate.
- `stunt_combo_display`: **0x800D3B28–0x800D4D84, 4,700 bytes**. Its ordinary
  entry and call-site spills do not establish ordinary contracts for its callees.
  It has **29 direct calls to 17 distinct destinations**. The genuine helper call
  writes parent locals at sp+356 and sp+348; these are real outputs, not invented
  pressure variables.
- `camera_follow_path`: **0x800C46D0–0x800C4C9C, 1,484 bytes**. Both parent
  call sites pass the output matrix in **s0**, in addition to a0/a1/a2. The old
  three-argument scout is incomplete. A full source draft exists in
  `cloud/work/ipa-groups/codex_camera_reset_b135/group.c`; this packet imports only
  that function's body, with its provenance retained here.
- That camera draft is still insufficient matching context: its own
  `func_8009C3F8` call consumes the input float in **f16** and selector in **a0**.
  An ordinary external C prototype cannot reproduce this. The incidental f12
  value at the call is overwritten by the callee before use, so it is not a third
  source argument. The real trig helper and its relevant caller context remain
  required. This packet does not claim its range or alter any existing source.
- The embedded random-range calculation in D348C corresponds to the real
  `func_8008B2E4`, **0x8008B2E4–0x8008B32C, 72 bytes**. Expressing it as that real
  helper restores source-level evaluation of the float path count before the
  RNG update. Its body still differs at four words in this unit; earlier leaf
  and rand-context near-misses are not superseded by an exact result.

The old `cloud/work/bigfish/stunt_combo_display.md` ordinary-ABI assessment is
therefore obsolete. The historical `work/game/ai/stunt_combo_display/base.c` is
only an empty stub, not a usable current source baseline.

## Source and semantics

`group.c` contains a natural typed D348C implementation, the genuine parent
reconstructed from native control flow, a read-only-derived camera context, and
the actual random-range helper. All function definitions perform their real
work; there are no fabricated callers, empty callees, register-pressure keepers,
inline assembly, or stack padding. Opaque bytes in `CameraCar` describe observed
record offsets and the native 0x808 record stride.

D348C behavior:

- Mode 6 rotates the point search order by one RNG sample, then picks the point
  maximizing its nearest other-car XZ distance. It uses the other car's camera
  position or ordinary position according to its state.
- Mode 5 returns the car's saved point.
- Other modes search nearest XYZ points. Modes 1 and 4 retain the main-path
  answer. Remaining modes can search graph nodes, dispatch a node of type 0,
  retain a type-2 result, or restrict the answer to current/next section bounds.
- Both node and point are returned through the real output pointers.

Parent repairs are supported by actual callee bodies, not neutralized arity
errors: `math_utility` is a two-pointer matrix copy; `func_800D14F4` takes two
pointers; `viDeadlinePassed` has no arguments. There are no zeroed float-to-pointer
casts. Point X reads use signed halfwords, sequential induction variables retain
native integer width, and the two true three-float output temporaries are arrays.
Own float constants were checked against the manifest-verified data artifact and
written as natural literals. The parent draft has not received host/gameplay
semantic equivalence testing; it is research context, not proven implementation.

`helper_seed.c` preserves an alternate typed decompiler-shaped body. Its nearly
equal byte count is intentionally shown alongside its wrong frame and broad
instruction mismatch. It is not the chosen semantic implementation or a near
match suitable for allocation-only tuning.

## Bounded experiments and full-extent evidence

The fixed recipe is IDO 5.3 whole-program `-g0 -O3 -mips2 -G 0 -non_shared`,
with the existing scorer's mandatory `as1 -r4300_mul`. The genuine parent and the
real independently emitted random-range helper are kept. Experiments changed
only used C computations: signed RNG conversion, separate XZ distance values,
sequential point pointers, separate distance-expression locals, and the real RNG
function boundary. No flag or scorer sweep was used. Nine meaningful source
forms were compiled, followed by the final parent width corrections.

| Function | Native bytes / frame | Natural group ELF bytes / frame | Differing native words | Nonzero words past native end |
|---|---:|---:|---:|---:|
| D348C | 1,684 / 200 | 2,088 / 160 | 369 / 421 | 96 |
| parent | 4,700 / 400 | 4,704 / 424 | 1,101 / 1,175 | 1 |
| camera context | 1,484 / 104 | 1,336 / 112 | 250 / 371 | 0 |
| random-range context | 72 / 0 | 72 / 0 | 4 / 18 | 0 |

The alternate helper emits **1,672 bytes with a 304-byte frame**, and differs at
**378 / 421** native words. Neither its shortfall nor the camera context's
shortfall is supplied by another function. The parent's final ELF body has
**8 additional zero alignment bytes**, accounted for separately from its 4,704
function bytes. The natural helper's 404-byte excess includes 96 nonzero words
and five zero instructions; all remain part of its real function extent.

`verification.json` records native hashes, call topology, ELF STT_FUNC extents,
zero alignment, full-body relocation status, and full-body resolved hashes.
Section-relative references remain unverified where code positions differ;
position-based own-data errors in a broad mismatch do not prove that the chosen
literal values themselves are wrong. No function is accepted. Workbench diagnosis
also confirms a structural/frame residual, not a register-only finish. Its raw
assembly outputs are retained only in ignored local build files.

## Validation and stopping point

```sh
python3 cloud/work/frontier/dot_camera_path_contracts/audit.py --replay
python3 -m pytest -q tests/conveyor/test_dot_camera_path_contracts.py
```

The **four tests pass**. They check live manifest-backed extents/topology and the
three-register native call contract; honest complete-size accounting and empty
claims; and the natural helper against an independent Python reference in
**1,400 deterministic randomized cases plus eight tie/empty/single-car cases**.
The host helper is compiled with UBSan and signed-wrap semantics matching the
MIPS RNG. Modes, nearest/farthest choices, inclusive/exclusive section bounds,
RNG state and ties are covered. Test-only external callbacks record the overlay
argument and provide a controlled D3430 result; they are not matching context.
These tests do not validate those external functions or the parent/camera body.

Stop here rather than tuning declarations against incomplete contracts. Next:
close the actual camera/trig context using its genuine callers, audit the full
parent's dataflow and stack temporaries, then replay the same complete extents.
Only after those source questions are resolved would helper register/frame work
be useful. No ROM, compressed-stream, image-splice, or full-ROM hash test was run.
