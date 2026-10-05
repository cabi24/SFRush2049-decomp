# Independent verification: `func_8008E0B8`

This packet checks the source in
`cloud/work/ipa-groups/dot_e0b8_vector_normalize/` without changing locks,
splices, targets, relocation tables, compiler binaries, or ROM build gates.
It is independent byte and semantic evidence for review, not a cartridge
integration or ROM identity claim.

## Reproduce

With stock IDO 5.3 selected by `IDO_DIR`, GNU MIPS binutils and a host C
compiler on `PATH`, run from the repository root:

```sh
python3 cloud/work/e0b8_verification/replay.py
python3 -m pytest -q tests/cloud/test_e0b8_verification.py
```

The replay freshly compiles the exact published O3 group through the canonical
pipeline, independently checks the protected target manifest and parses the
target section, uses GNU `nm` for complete ELF symbol extents, and links the
whole object with GNU `ld`. It compares every relocated function word against
both the target and the project's relocation implementation. Both the complete
140-byte E0B8 body and the real public 32-byte E098 helper must pass. The
published receipt is bound to source, group manifest and compiler hashes.

The linked `.text` section is 176 bytes: 32-byte helper, 140-byte normalizer,
and four bytes of section alignment. Function extent is checked independently
of the section size. Those alignment bytes cannot be mistaken for function
content. No target words or disassembly are included in the receipt or tests.
Generated objects and raw linked data stay under ignored `build/`.

For the historical admissible standalone baseline:

```sh
python3 cloud/work/e0b8_verification/replay.py \
  --source cloud/work/near_miss_B28/func_8008E0B8_vector.c \
  --build build/e0b8_standalone_verification
```

That baseline remains nonmatching: 128-byte full ELF body versus 140-byte
native target and 35/35 differing words. Behavioral equivalence alone cannot
turn it into a matching claim. The historical padding/dead-address candidate
is not used by this packet.

## Semantic coverage

The complete target instructions, freshly linked IDO instructions, a host build
of the exact candidate function body, and a separately expressed binary32
oracle agree on 7,487 cases:

- 100 boundary cases, including equality and adjacent representable magnitudes
- 3,375 Cartesian-product edge vectors
- 12 threshold/vector alias cases, one for each coordinate in four vectors
- 4,000 deterministic randomized cases

The native contract is single-precision `(x*x + y*y) + z*z`, square root,
then `length <= threshold`. The early path returns positive zero without
writing any vector component; otherwise the routine computes one reciprocal,
multiplies each component by it, and returns the original magnitude. The real
threshold has separately audited value `1e-5f`; parameterized thresholds are
also used to stress the contract. Threshold and all original components must
be read before output stores. The replay checks exact external access bounds,
write/no-write behavior, store order, input snapshot order, stack restoration,
and O32 callee-save preservation.

The host harness changes only the external threshold declaration into a pointer
binding. This permits legitimate overlap with an element of a host vector
without forming an out-of-bounds array from a scalar global. No expression or
operation in the candidate function body changes.

Twenty-two pytest checks include independently detectable negative controls:
strict `<` in place of `<=`, reciprocal instead of magnitude return, clearing
early-exit input, sum reassociation, direct division instead of reciprocal
multiplication, wrong/truncated/padded ELF extent, unsupported instructions,
and a missing final delay-slot instruction. The final matching group and the
standalone nonmatching baseline are replayed separately.

## Limits

The behavioral model uses round-to-nearest binary32 arithmetic. NaNs are
compared by classification; NaN payload propagation, signaling behavior, FCSR
exceptions/traps, and alternate rounding modes are not modeled. Signed zeros
and non-NaN finite/infinite results are compared bitwise. Arbitrary threshold
and edge tests do not imply every input occurs in gameplay. Exact native byte
identity is separately proven over both complete compiled bodies.

## GNU linker portability

The verifier explicitly addresses its output `.text` section at the verified
native base. Ubuntu GNU binutils 2.42 rounded the former implicit location-counter
form up by eight bytes to the input section's 16-byte alignment, despite the
input `SUBALIGN(4)`. The unchanged address checks correctly rejected that layout;
Debian 2.44 happened to preserve the intended placement. This was reproduced
using Ubuntu's exact `2.42-2ubuntu1cross5` package before the fix.

The explicit output-section address works on both versions. All 22 focused tests
pass on 2.42, and all 666 focused/scorer tests pass on 2.44. Both freshly linked
complete-body/7,487-case receipts remain byte-identical to the published receipt.
Subprocess failures now include command, exit status, stdout and stderr; linked
placement failures report expected and actual addresses and sizes. No byte,
extent, symbol, or semantic acceptance check was weakened.
