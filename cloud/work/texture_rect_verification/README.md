# Independent clipped-rectangle verification

`func_80087110` remains a **nonmatch**: four differing instruction words at
function offsets `0x4c8`, `0x4cc`, `0x4d0`, and `0x4d4`, out of 445 words.
This packet verifies the existing standalone source
`cloud/work/frontier/w4a/func_80087110/best.c`. It adds no source replacement,
matching claim, protected-target edit, lock, splice, or cartridge coverage.

## Reproduce

With stock IDO 5.3 selected by `IDO_DIR`, GNU MIPS binutils, Python, and a host C
compiler with UBSan available:

```sh
python3 cloud/work/texture_rect_verification/replay.py
python3 -m pytest -q tests/cloud/test_texture_rect_verification.py
```

To review another standalone candidate without changing the recorded baseline:

```sh
python3 cloud/work/texture_rect_verification/replay.py \
  --source path/to/candidate.c --build build/rectangle_candidate_verification
```

Generated objects, linked binaries, and host harnesses stay under ignored
`build/`. The published receipt contains hashes, counts, offsets, and scalar
outcomes only. No original instruction stream or ROM data is embedded in the
scripts, tests, or receipt.

## Byte and extent evidence

The verifier independently validates all 23 protected manifest entries and
parses the full native section. GNU `nm` establishes the candidate's complete
1,780-byte ELF symbol extent, matching native interval
`[0x80087110, 0x80087804)`. The `.text` section is 1,792 bytes; its remaining
12 bytes are separately verified zero alignment padding. Padding, a truncated
body, or a prefix match cannot satisfy exact-body acceptance.

The complete object is linked by GNU `ld` at the explicit native output-section
address. The GNU-linked function agrees with the project's relocator over
every function word. All 32 relocations are verified: 16 HI16 and 16 LO16,
referring only to the seven expected globals. There are no unresolved symbols,
unverified references, own literal/data sections, or callees. The standalone
verifier fails closed if a candidate introduces another text function, a new
data section, non-padding bytes outside the function, or unsupported relocation
kinds. Such a new context needs a separately justified verification route.

The selected source hash is
`f8355db6884b85c77a25b21c7aae746be236ed10a3dc8fee93264259cf4c6f8f`.
Its flag comment differs from the separately archived `compiler/baseline.c`;
the actual compile recipe here is explicitly
`-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The receipt records exact source, object, compiler-component, target, and
linked-body hashes, plus GNU linker and host compiler versions.

## Behavioral evidence

All 6,828 deterministic cases agree across the complete protected instruction
stream, the freshly compiled and GNU-linked instruction stream, and a separate
clipping/packet oracle. Both complete 445-instruction bodies were exercised:
there are no unexecuted instruction offsets in either stream.

- 816 clipping/flip/stretch cases, covering all four edges, simultaneous
  clipping, no clipping, equality, inversion, rejection, both mode families,
  all four flip combinations, the vertical-scale flag, and unrelated flags
- 12 full-word argument and packet-field masking cases, including values
  beyond the signed-short range
- 3,000 randomized cases inside the host source's tested arithmetic domain
- 3,000 randomized full-word native/modulo-arithmetic stress cases

All 16 mode/flip/stretch combinations emit commands. The harness observed
941 emitting and 5,887 rejected cases. Each emitting case writes exactly six
32-bit command words, advances the display-list pointer by 24 bytes in three
publications, and leaves both buffer guards intact. Rejection leaves the
pointer and command storage unchanged. External memory access order agrees
between native and candidate; stack restoration, callee-saved registers, and
unchanged input globals are checked.

The host compiler independently builds the unchanged candidate source, with
only external object definitions and a harness appended. Its 3,828 applicable
cases agree under UBSan. The host uses C99 mode deliberately for strict signed
left-shift diagnostics; the submitted source remains IDO/C89-compatible.
Separate child-process negative controls confirm that the sanitizer rejects
negative left shift and signed arithmetic overflow. These failure controls are
not failures of the bounded valid-input test corpus.

The contract includes some easily missed details:

- Equality of rectangle edges still emits commands; only inverted edges reject.
- Mode zero ignores the vertical-scale flag and uses horizontal step magnitude
  4096; a nonzero mode uses magnitude 1024 and includes the far edge plus one.
- In nonzero mode, the scale flag doubles the vertical span and selects step
  magnitude 512. The extra half-texel offset applies only to vertical flip.
- All six incoming values are consumed as full words. The shared header's
  `s16` second parameter is narrower than the implementation evidence. This
  packet does not change shared declarations or make a whole-caller ABI claim.

## Limits and promotion requirements

This is bounded integer instruction-model replay, not N64 hardware, graphics,
or gameplay validation. It assumes readable ordinary globals, an aligned valid
command buffer disjoint from those globals, and no concurrent mutation. It does
not establish buffer capacity or object lifetime at every production caller.
The parameterized clip bounds and modes do not claim every tested value occurs
in gameplay.

The emitted native arithmetic wraps at 32 bits. Modern portable-C claims need
separate bounds: negative signed left shifts and signed arithmetic overflow are
excluded from the host proof. Random full-word stress results describe the
native/linked instruction behavior and explicit modulo oracle only. The
verified caller inventory and header correction, if pursued, must preserve
these distinctions.

Behavioral agreement does not remove the four-word scheduling residual. A new
exact-match source must be freshly replayed and source-hash-bound. Production
acceptance still requires independent review, unchanged accepted neighbors,
source-backed game-image identity, exact compression, and the full-ROM gate.
The user retains the independent merge/checking step.

## Verification run

On GNU binutils 2.44 and GCC 14.2, the fresh full replay passes. All 687 focused
checks pass: nine rectangle-verifier checks, 22 existing E0B8-verifier checks,
and 656 cloud-scorer checks. Python compilation and `git diff --check` pass.
These focused checks do not stand in for whole-repository CI or cartridge gates.
