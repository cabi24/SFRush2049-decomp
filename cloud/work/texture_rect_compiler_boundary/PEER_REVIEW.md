# Independent review of the compiler-boundary checkpoint

Reviewed publication commit `79b5dff7c80e138f3106ff7661b6e2d4216af434` against
`3dfd0a62da400716dd369a95fbbd2d6c0256e626`. **Safe to publish as a research-only
checkpoint. No blocking correctness or publication issue found.** This is not
an accepted C match or cartridge proof.

## Independently rerun

The stock-toolchain boundary helper reproduced its published
`verification.json` exactly, including source/tool/object hashes and every
comparison: historical C and identity listing 4/445; the two preceding-boundary
diagnostics 0/445; both following-boundary controls 53/445; ordinary-C
`both_body_do.c` 8/445. The zero-difference listing experiments remain explicitly
ineligible for promotion and are never used as the semantic candidate below.

The complete-source verifier was rerun directly on the checked-in
`both_body_do.c`, whose source hash is
`98a0adc8027464275b43845a6e223269c96a7cef2e1ec00395d264e0754b9c35`.
The sanitized full receipt is `independent_source_verification.json`.

- GNU-linked address `0x80087110`, exact ELF extent 1,780 bytes
- 12 separately checked zero alignment bytes, no extra executable words
- GNU linking agrees with the project's relocation implementation
- Seven expected global dependencies, no own literal/data section or callees
- Eight residual offsets: `0x3d0`, `0x3d4`, `0x3e4`, `0x3ec`, `0x464`,
  `0x47c`, `0x5e4`, and `0x5fc`; the original scheduling offsets are absent
- 6,828 native/linked/independent-oracle cases pass
- 3,828 applicable unchanged-source host C99/UBSan cases pass
- Both sanitizer failure controls are observed as expected
- All 445 instruction offsets in each complete stream execute
- Command bounds, pointer publication, rejection preservation, external access
  order, stack restoration, and callee-saved registers pass

The same domain and emulation limitations described in the independent
verification packet apply. These tests do not establish graphics hardware
behavior, every caller's storage lifetime, or portable-C semantics outside the
tested arithmetic domain.

## Inspected trace evidence

All 16 selected allocator decisions in `allocation_receipt.json` match the
retained trace logs, and all four referenced source hashes match their local
inputs. For all four allocator controls, the diagnostic optimized Ucode is
byte-identical to stock. For all five listing controls, the traced assembler
object is byte-identical to stock output. These are independent comparisons of
retained evidence, distinct from a fresh rebuild of the instrumented compiler.

The exact `height_before_do` and `setup_do` inputs remain local diagnostic
artifacts rather than checked-in source files. Their hashes and selected
allocator evidence are inspectable, but the public helper currently reproduces
only the historical baseline, five listing controls, and `both_body_do.c`.
Those two further source experiments must not be described as independently
rerunnable from this checkpoint alone.

## Publication checks

The ten-file reviewed change set contains C, Python, Markdown, and sanitized
JSON only. There are no raw instruction streams, raw listings, ROM bytes,
compiler binaries, credential material, protected-target changes, locked-source
changes, or new matching submissions. All listing zeros are labelled diagnostic
and ineligible; the ordinary-C positive remains explicitly unaccepted.

`git diff --check`, the protected-path guard, and changed-submission checks pass.
The existing 687-test verifier/scorer result remains separately documented in
the verification packet; this review freshly ran the boundary helper and the
positive-source full replay, not a second entire 687-test suite.

One nonblocking wording nit: the boundary helper's opening docstring still says
the historical candidate is its only C input, although it also compiles the
checked-in positive control. The executable code and report describe both.

## Addendum: complete controls and CI placement

Reviewed `6556e5ba218f5a3d60f6aec93c77700b2d4640a1` and its follow-up
`fe07b2d7a9a3a7b73dc79ea247da7a71d7995590`. The two historical documentation
limitations above are resolved: all three complete C controls are now included,
and the helper describes its historical baseline plus three controls correctly.
A fresh stock-helper run reproduces the updated receipt exactly, including
source/hash bindings and the three source results **8/445, 174/445, 29/445**.
Every source and listing control remains ineligible for promotion.

The corrected `nocs` wording agrees with direct inspection of `f_compute_save`
in the pinned compiler source: it is a normalized occurrence/live-IN-cardinality
divisor, not a literal live-block or source-reference count; the numerator is
net estimated saving. The prior trace values therefore do not, by themselves,
prove a one-block lifetime increase.

The receipt tests are now under `tests/cloud/`, with repository-root resolution
adjusted for that location. All **14** focused rectangle-verifier/receipt tests
pass. The previously verified positive source is unchanged. The additional
files are ordinary C source, and the new receipt contains no instruction dumps
or raw listings. **Research-only publication remains approved; no match or
cartridge acceptance is implied.**
