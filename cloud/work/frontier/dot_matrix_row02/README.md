# Supplied-angle row-0/2 matrix rotation

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.

`func_800C40E8`, `0x800C40E8..0x800C4180`, is a complete strict
**152-byte / 38-word object match**, independently replayed through both the
ordinary O3 and O2 compiler paths. The publication recipe is O3 plus the existing
R4300 multiply-erratum assembler option. The canonical scorer, protected target
manifest and symbol map are unchanged.

Source: `cloud/matches/func_800C40E8.c`.

## New evidence and source

This is the third supplied-sine/cosine rotation previously retained in C24,
following the row-1/2 and row-0/1 pair in draft PR #84. It rotates matrix rows 0
and 2 with the same genuine three-input ABI: two floats and a matrix pointer.
It is not one of the trig-calling B7 residuals. It makes no calls and requires
no caller stand-in, pressure formal, artificial local, assembly or new storage.

The prior C24 source explicitly named each loaded FP register and reconstructed
a stack spill as a C scalar. Fresh O3 replay gives 37/38 differing words and four
nonzero excess comparison words. Applying the pair's one-result source form
recovers the complete native extent immediately, at 18/38. Writing the consumed
first sum as `row0 * cosine + row2 * sine` then reproduces the native order of
both multiplication evaluations and every remaining instruction. Only two
natural new body forms were compiled. All actual row-0 values are preserved
until both outputs are evaluated; row 1 is untouched.

Both native direct callers are `camera_follow_path` and `stunt_combo_display`.
They are not included, changed, claimed or proven by this packet. The ABI is
ordinary; standalone equality is the scope of this proof.

## Reproduce

With the repository-pinned IDO toolchain available through `IDO_DIR`:

```
python cloud/work/frontier/dot_matrix_row02/verify.py
python -m pytest tests/conveyor/test_dot_matrix_row02.py -q -o addopts=''
```

The verifier first requires the actual ELF `STT_FUNC` extent to equal the entire
152-byte target. It then requires strict canonical acceptance and independently
relocates and compares every body word without masks or exceptions. The ELF's
160-byte text section contains eight bytes of alignment outside the function;
those bytes earn no credit. Both O2 and O3 pass with no unresolved or unverified
relocations, relocation errors, or nonzero extra instructions.

A wrong-rotation subtraction-to-addition control is rejected by both canonical
scoring and the complete-body verifier. A test also makes the extent gate refuse
a 148-byte reported function independently of the unchanged target comparison.

All four focused tests pass without skips. Native host tests cover 4,102 finite
cases, including identity, quarter-turn, negative-zero and random supplied angle
pairs, compare the single-precision arithmetic bitwise, and require the middle
row to remain unchanged. These tests are semantic checks; strict native object
comparison supplies the authoritative instruction proof. The receipt pins the
final source, target, compiler components and scorer hashes.

## Acceptance boundary

This packet adds no production splice, lock, source-built image, compressed-stream
or ROM coverage claim. Whole-program shadow, image-placement, compressed-stream
and full-ROM SHA-1 gates are unrun here and remain required for acceptance by the
independent checker. Broad CI is left to the integration batch; no unrelated
failure is repaired or waived. The packet contains source, test code and sanitized
hash/count evidence only, without ROM bytes, raw assembly, objects or credentials.
