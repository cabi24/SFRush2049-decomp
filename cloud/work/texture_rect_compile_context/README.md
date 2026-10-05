# 80087110: authentic neighboring-group control

## Result

The unchanged rectangle source emits **identical fully relocated bytes** when
compiled alone and when added as a retained external root to the current,
complete accepted graphics group. Both outputs have exactly 1,780 bytes and
four differing words, at offsets 0x4c8, 0x4cc, 0x4d0, and 0x4d4. No additional
words, unresolved relocations, unverified data, or errors are present.

All seven existing group bodies remain strict zero, with exact symbol extents:
mode (387 words), clear-mask (45), set-mask (74), palette (145), solid rectangle
(118), initializer (100), and object-render/texture selector (2,512). The mode
function's existing five-case table is verified against the protected data
artifact by the unchanged scorer. Total existing body coverage is 3,381 words.
No original source, lock, protected target, compiler stage, or recipe changes.

This rejects the narrow hypothesis in `PLAN.md`: this authentic family context
does not change the leaf's output. It does not rule out every possible original
source or whole-program context. The leaf does not call another function, so
missing callee closure is not an explanation of its current residual.

`verification.json` preserves source and tool hashes, exact group recipes,
full comparison results, and the fully relocated leaf hash. The mode function's
raw-relocator hash is deliberately omitted: that lower-level helper does not
finish own-data relocation, whereas `score.compare` separately verifies its
table by content. No hash computed with unresolved relocation fields is
presented as a linked-body hash.

## Actual compilation and source-boundary evidence

The existing group at `src/blob/groups/gfx_modes/group.json` contains the real
mode/state helpers, palette, solid rectangle, initializer, and texture selector.
The October 5 acceptance added complete texture-selector and initializer source.
The group input has no real call to 80087110. Its current public roots are kept
unchanged; the experimental copy adds only the exact rectangle source, its
member name, and its retained external root. This preserves the independently
established ordinary O32 interface rather than inventing IPA register inputs.

Physical order near the target is:

- 80086A50: mode, 1,548 bytes, accepted O3 group
- 8008705C: clear-mask, 180 bytes, accepted O3 group
- 80087110: this 1,780-byte unaccepted leaf
- 80087804: logarithmic texture-mask helper, 220 bytes, accepted standalone O2
- 800878E0: set-mask, 296 bytes, accepted O3 group
- 80087A08: texture selector, 10,048 bytes, accepted standalone O3 and included
  unchanged as context in the current group

The different accepted flags are reconstruction recipes, not proof of original
source-file boundaries. Adjacency and common graphics purpose likewise do not
prove that these functions shared an original C file. The whole-image
bottom-up-call-order evidence in
`specs/010-ipa-call-groups/research/s2-s5-spikes.md` strongly supports whole-program
O3 optimization; it does not recover the original source partition or exact
export-root list. Its older allowance for stand-ins was not used here.

The current accepted initializer genuinely writes all four clip globals,
D_8012E608 flags, and D_8014A248 mode; the group shares Gfx *D_80149438. These
are extern declarations in the accepted family inputs. The authentic definition
that mattered to the initializer/texture-selector pair is the separate s64
D_8012E688 cache, which 80087110 does not access. That is real evidence for a
whole-unit effect elsewhere, not evidence for inventing definitions of the
rectangle's seven globals. No such definitions were introduced.

The frozen rectangle declares the flag word as signed int, while accepted
family source uses u32. This known difference was preserved, not swept. The
prior isolated unsigned-global and full-SDK controls already failed to explain
the four-word residual. The current unchanged-input group experiment likewise
does not validate either spelling as the unique historical declaration.

## Real callers and limits

A fresh scan of the manifest-verified complete native bodies finds exactly
three direct calls to 80087110:

- Input_ProcessGameplayPad, 800A04C4, at +0x914 / 800A0DD8
- Input_ProcessGameplayPad, at +0x9e8 / 800A0EAC
- audio_doppler_calc, 800B6788, at +0x3f8 / 800B6B80

The function names are historical labels. The existing behavior-first packet
establishes six O32 words at these calls and no used return value. This audit
does not strengthen that into a claim about original source parameter widths.
A scan for ordinary typed C definitions across src, cloud/work, and work finds
only the two empty base.c stubs for those caller names. No such stub was added
to the experiment. The search is a reconstruction inventory, not proof that
unknown original source or every possible C declaration syntax is absent.

Thus the actual caller source is not newly available context. Reconstructing
those large callers would be a distinct source-development task; it is not
necessary merely to reproduce the leaf's ordinary external entry, and cannot
be substituted by synthetic callers. No requested maintainer action follows
from this negative bounded result.

## Compiler-settings provenance

- The executed baseline uses stock IDO 5.3 and the already-protected
  `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul` recipe.
- The existing group runner uses its normal cc-to-Ucode, uld -kp, usplit,
  umerge, uopt, ugen, and as1 sequence, including its existing Olimit 5000.
  These are recorded executed settings, not a newly inferred exact retail
  command line. The standalone and group equality shows those existing two
  pipelines agree for this source.
- `-r4300_mul` has independent image-wide evidence in
  `cloud/work/R4300_MUL.md`. This integer leaf provides no new discrimination
  for that floating-point multiply workaround.
- IDO 5.3/O3 is the successful reconstruction environment for the neighboring
  accepted source and the four-word baseline. A near-match alone cannot prove
  the game's exact compiler revision or all original flags.
- The resolved prior cause remains uopt alias-boundary metadata influencing
  as1 cross-basic-block predecessor weighting, as documented in
  `texture_rect_compiler_boundary/REPORT.md`. Older w4a speculation that the
  branch target needed an extra real v1 use is superseded. This group test adds
  no dead reads and does not reopen that rejected hypothesis.

## Reproduce and scope

Source the pinned IDO/binutils environment, then run:

    python cloud/work/texture_rect_compile_context/verify.py
    python -m pytest -q tests/cloud/test_texture_rect_compile_context.py

The verifier writes generated sources, objects, and scalar receipts under
ignored build/87110_compile_context. Compare its verification.json to the
checked-in receipt. Only the receipt, script, tests, and analysis are published.
No protected bytes, raw disassembly, compiler listings, downloaded SDK headers,
or binary objects are included. This is research only, with no promotion,
image/compression/ROM verification claim or additional coverage credit.

## Verification status

Stock baseline and both group builds completed. Twenty-two focused tests pass
(seven new context checks, nine existing rectangle-verifier checks, and six
compiler-boundary receipt checks). Python compilation and diff whitespace
checks pass. The existing native target object was independently checked against
all 445 protected words before running workbench diagnosis. Comparing the raw
relocatable candidate initially introduced 32 misleading relocation-symbol
metadata warnings because the native object has no relocations. A full GNU
link agrees with the project relocator on every word, and the linked diagnostic
removes that ambiguity. Diagnostic files remain in ignored build storage. No
complete ROM build was run.
