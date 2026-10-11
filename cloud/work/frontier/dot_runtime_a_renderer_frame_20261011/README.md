# Two overlay-A renderer matches: 1,356 new candidate bytes

## Source and claims first

`groups/menu_renderers/` is a self-contained IDO 5.3 group with two new strict
claims, both complete native extents:

- `func_803A6F2C`: 0x803A6F2C..0x803A7180, **596 bytes**
- `func_803A7620`: 0x803A7620..0x803A7918, **760 bytes**

The complete real `803A7180` and sole kept caller `803A7918` remain compiled,
unclaimed context. These helpers have private-register conventions and must
not be installed as independent O32 functions. No dummy caller, stand-in,
false return prototype, forced inline or compiler patch is used.

Base: master `eae0c6466`. This packet contains all its source; the historical
PR #311 reconstruction is provenance, never a replay dependency. The runtime
image landing path exists, but these candidates have not been spliced or
accepted. **No new accepted-image or cartridge bytes** are claimed here.
Existing PR #309/#310 results and the separate availability-root match receive
no duplicate credit. No production source, lock or protected target is modified.

## Trace-first changes and exact limits

Workbench `diagnose` first confirmed identical instruction/register lanes and
frame/home residuals in all four complete bodies. Stock `umerge -v` records no
inlining, excluding missing nested inline levels. Ordinary and diagnostic
complete objects agree byte for byte, with the same kept-root/private-helper
classification. Parsed metadata is in `frame_trace.json`; raw IR and assembly
listings are not published.

- **6F2C:** native frame 320, previous 312. Two disclosed unused `s32` scalar
  declarations before the real X local contribute exactly 8 bytes. UGEN M area
  increases 288→296; T=16/P=4 and all existing buffer addresses are unchanged.
- **7620:** native frame 144, previous 128. Four disclosed unused `s32` scalars
  contribute exactly 16 bytes: one before the arrays and three after them.
  UGEN M increases 100→116, T=16/P=4 unchanged. Title/status bases shift upward
  12 bytes to the native sp+80/sp+120. Every other instruction stays exact.
- **7180, still NONMATCH:** declaring the existing scalar locals before its
  arrays fixes all four home-reference differences, reducing 6/296 to **2/296**.
  Only frame 192 versus native 168 remains. T=28/P=4/M=148 is unchanged.
  A register qualifier and replacing the row-Y carrier with its expression did
  not close the gap. Its 100-byte text capacity is an unproven hypothesis; it
  has not been shrunk to force a match.
- **7918, still code-identical/unverified:** the real text-X return now has a
  named `s16` carrier, rather than a nested argument temporary. Three unused
  `s32` slots give a 528-byte named area and native frame 608. With the carrier
  but without those 12 declared bytes, the area is 516/frame 600; alignment makes
  the frame increase 8. Every native buffer home, saved-register slot and code
  word now agrees. Its owned switch table remains unverified by the default
  scorer because the overlay data artifact is absent. No claim is made.

All unused locals are the current Wave 16 exact-frame-residual technique,
explicitly disclosed in the C header. They reconstruct frame layout; they do
not establish the original source's variable names, types, number or purpose.
No padding array or artificial read/write was added.

All live buffer capacities remain unchanged from PR #311: title 128/name 128/
suffix 16 in 6F2C, text 100/suffix 16 in 7180, status 20/title 40 in 7620, and two 256-byte
buffers in 7918. These are hypotheses, not authenticated capacities or guarantees
of termination or unbounded string safety. Passing a matching test does not
establish arbitrary-input C safety or rendered/gameplay behavior.

## Strict canonical command and output

Compiler: canonical stock IDO 5.3, identified through the existing compiler
registry. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`, with canonical
`-r4300_mul` assembly handling.

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_renderer_frame_20261011/groups/menu_renderers --targets asm/us/ovl_a
```

Command exits zero without `--claims` or `--allow-unverified`. Exact member output:

```text
Members:
func_803A6F2C:
  MATCH
func_803A7620:
  MATCH
```

Its informational context summaries are:

```text
func_803A7180:
  2/296 words differ
func_803A7918:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0xa0, .rodata+0x0 at +0xa8)
```

The last line is the scorer's literal output, **not strict acceptance**.
`Comparison.accepted()` is false for 7918 without authenticated table data.
The prior research separately authenticated and GNU-resolved the complete
five-entry 20-byte switch table at 803B9834, but that proof is not substituted for
the default canonical refusal, and no raw table bytes are republished here.
The two claimed bodies have no owned-data references.

## One cheap routine check

```sh
python3 cloud/work/frontier/dot_runtime_a_renderer_frame_20261011/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_renderer_frame_20261011/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_renderer_frame.py -o addopts='' -q
```

**1 passed in 0.14s** with IDO, including full-body strict scoring, all four
STT_FUNC sizes, fixed member/claim/context/keep sets, and an independent compile
with just the claimed functions' unused scalars removed. That control restores
exactly the original 2/149 and 4/190 frame/home discrepancies. Source, verifier,
diagnostic helper, image-qualified target words/extents and relevant manifest
metadata are input-bound. Tools, locks, Makefile and assembly text are not pins.
Ordinary and optimized-Python replay pass. Missing IDO uses the shared conftest
skip policy; `REQUIRE_TOOLCHAIN=1` makes a missing-tool skip fail.

Optional deeper stock-stage metadata, outside the routine suite:

```sh
python3 cloud/work/frontier/dot_runtime_a_renderer_frame_20261011/trace_frame.py
```

No ROM, raw assembly dump, extracted image, compiled object, credential or
unrelated private data is part of the deliverable. Actual image splicing,
recompression, full-ROM SHA-1, hardware and gameplay validation belong to the
independent coordinator's acceptance path.

## Independent review

Independent source review of frozen C
`7f1a6acbd82b4e44b69884d057be553b1d571accb612f1cb4986ff685ae018dd`
relinked both complete claimed bodies through GNU: **29 relocations in 6F2C**
and **47 in 7620**, with every word exact. It checked full four-function ELF
tiling, next-function boundaries and zero alignment, and independently removed
only the six declared unused scalars to restore the original residuals.

The native sole caller 7918 preserves s0–s8/ra and reaches the two claimed
helpers at its witnessed call sites. The independent review also resolves all
223 code relocations and five table entries across the four-function family,
including the authenticated 20-byte table at 803B9834. This corroborates the
research but does not convert 7918's default canonical unverified status into
an accepted claim. No arbitrary-text safety or gameplay proof is implied.

The single test's no-IDO run is **1 skipped in 0.07s**. A focused compatibility
run of overlay layout/splice tests plus this packet is **21 passed, 2 skipped
in 0.46s**; private-input skips are distinct from completed acceptance gates.
