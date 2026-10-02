# Complete three-component vector length: strict O3 match

Base: `cabi24/SFRush2049-decomp` master
`2f1f30c508495db08d61170c4a9b2409461ae326` (2026-10-02).

## Result

`func_8008E098`, `0x8008E098..0x8008E0B8`, is a complete **8-word / 32-byte**
strict match using the unchanged canonical cloud scorer. Its ELF function and
entire emitted text are both exactly 32 bytes, with no excess alignment words.
It is the only function and the only member/keep/claim in this translation unit.
No helper implementation or caller context is supplied.

Literal recipe: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, through the
standard whole-program group compiler pipeline. No masks, unresolved or
unverified relocations, relocation errors, or excess instructions are allowed.

## Source and provenance

The source is the faithful three-float norm previously preserved in
`cloud/work/tiny_A36/func_8008E098.c`. Its O2 build has seven differing words
out of eight and one nonzero excess instruction; that outcome was independently
reproduced. The unchanged old source matches through the standard O3 group
pipeline. This contribution removes unused typedefs, formats the same expression
readably, and records the correct build recipe. The final source also matches.
The A36 and A38 research packets remain unchanged, including their archive in
draft PR #9 at commit `433611408270ede7fe6acdb027f1b82179209511`.
The old packet lists this function as a target, not a matching claim.

The native function consumes three single-precision values in the standard
hard-float O32 locations: `f12`, `f14`, and the raw float bits in `a2`. It computes
`(x*x + y*y) + z*z` with the native operation order and applies `sqrt.s`, returning
in `f0`. IDO's genuine `sqrtf` intrinsic produces that square-root instruction;
there is no external math call or stand-in implementation. The O3 pipeline
eliminates the O2 argument-home round trip and reproduces the native direct
register transfer. There is no frame or memory access in the matching body.

No direct caller was found in the protected function-word inventory. This does
not establish that the function is unused: indirect references and opaque
regions are outside that scan. Several generated context files contain stale
`s32 func_8008E098()` declarations; the native float operations and float result
support this candidate's actual three-float/float-return prototype. No unrelated
or accepted context source is changed. The arcade primary reference is not
present in this checkout, so no original arcade equivalent is asserted.

This preserves native floating-point instruction behavior; it does not replace
the expression with a rescaled or overflow-resistant norm. There are no invented
arguments, operations, helpers, guards, volatile accesses, artificial stack
locals, assembly, target changes, or scorer changes.

## Reproduce

With the project-pinned IDO 5.3 static-recomp v1.2 installed:

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_vector_length
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_vector_length --claims
```

Expected output:

```text
Members:
func_8008E098:
  MATCH
```

The approved compiler archive is pinned by `tools/cloud/setup.sh` to SHA-256
`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
The existing approved installation is reused. Fresh `sound_handles_clear` and
three-member `resource_slot_clear` scorer sanity checks both pass. Independent
clean-directory verification records source/spec/compiler/scorer and body hashes
in `verification.json`.

## Limits and integration

This is a draft source contribution, **not accepted cartridge coverage**. The
local `claims` field requests strict changed-submission CI rescoring only.
Accepted C, locks, coverage totals, build wiring, protected retail targets, and
the scorer are unchanged. The target is absent from accepted locks and other
current claims at preflight. Open draft PRs #10 and #11 cover different functions.

No original ROM, extracted full game image, or derived `build/blob_layout.json`
is available here. Source-image splice, compressed-stream identity, full-ROM
SHA-1, and `make test` have not run. Read-only source-hash checks do not substitute
for those gates. Live LAN coordinator ownership is inaccessible and must be
rechecked by the integrator before normal image/ROM/lock promotion gates.

## Additional validation

- Cloud/scorer suite: **840 passed, 27 subtests passed**, exit 0
  (213.50 seconds): `python -m pytest tests/cloud tests/conveyor/test_cloud_score.py`
- CI-style repository suite: **1299 passed, 41 skipped, 9 deselected**, exit 0
  (62.32 seconds):
  `python -m pytest tests/conveyor -q -o addopts='' -m 'not node_required'`
- `make check-matched`: all **160 static locked functions** intact
- Existing blob/group source-hash checks: **zero problems**, via read-only
  `blob_splice.check()` / `blob_group.check()`; this is not an image gate
- Host `cc -std=c89 -pedantic -Wall -Wextra -Wno-unknown-pragmas -fsyntax-only group.c`:
  exit 0; IDO independently checks the actual intrinsic pragma
- Fresh compiler sanity controls pass; all 23 protected manifest files verify
- Changed-submission strict scorer and protected-path guard: both pass
