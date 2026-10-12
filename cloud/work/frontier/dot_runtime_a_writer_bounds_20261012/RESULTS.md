# Image-A file-menu renderer: 1,184 newly exact candidate bytes

Deliverable: `groups/dot_runtime_a_menu_renderers_20261011/`, superseding the
installed group of the same name. New member `func_803A7180` is the complete
296-word interval `[0x803A7180,0x803A7620)`. All four verified functions are
members and claims. The previous `803A6F2C` (596 bytes), `803A7620` (760 bytes)
and `803A7918` (576 bytes) retain all **1,932 bytes**, with no duplicate credit.
The real `803A7918` callback stays the sole kept root. No cross-group migration.

Base: master `3bec9ce30b8fbc707e2e1d6c5717826c03646232`. PRs #340 and #341
are prior, separate matches and are not counted. Newly accepted image bytes and
newly ROM-verified bytes remain **zero**. Keep draft; the independent checker
owns installation, image/compression/hash gates and merging.

## Trace-first cause and sole source edit

Fresh stock baseline: 2/296 differing words, only frame allocation/deallocation,
192 versus native 168. Workbench confirms the 24-byte frame gap; unlinked
relocation differences in its initial display are not matching evidence.
Stock UGEN reports M148/T28/P4. Removing scalar carriers had previously caused
replacement temporary storage and did not close it.

An independent all-writer audit below shows that the inherited `text[100]` is
larger than required for the supported native inputs. The sole executable edit
is `text[100]` to `text[76]`. It produces M124/T28/P4 and frame 168, preserving
all other instructions, registers and buffer addresses. The verifier strips
comments and checks that undoing this one declaration change reproduces the
current installed source exactly.

The complete native stack census has accesses at 16/20/24/36/52, and buffer
addresses at 52 and 68. Reconstructed suffix `[52,68)`, text `[68,144)` and
remaining scalar homes `[144,168)` do not overlap. The scalar homes are a source
layout inference, not extra observed native stack accesses. Original buffer
capacities and declarations remain unknown. Other inherited group's buffer
hypotheses and frame-shaping scalar slots are unchanged and disclosed.

A separately writer-safe `text[72]` diagnostic does **not** match: six words
change, frame 160 and buffer homes four bytes too low. The old 100-byte diagnostic
retains two differences, frame 192. Capacity safety alone therefore does not
establish matching. These are two bounded causal controls, not a sweep.

## Every writer and input domain

The current repository asset is authenticated by full SHA-256. Both the complete
game image and image A are authenticated in memory. Loader table offsets, codec,
the six packed language sections, pointer ranges, NUL termination and all relevant
formats are checked. No binary payload or assembly dump is published.

All six supported languages use byte-mode strings on every path in this function.
Their file-action index is the native halfword at resource indices+94 (entry 47).
Every writer includes its terminator in these conservative bounds:

| Destination/path | Bound | Evidence |
|---|---:|---|
| text: title choice, space, optional second choice | 36 | Both authenticated choices in all six languages |
| text: formatted row index | 12 | No-width `%d`, conservatively full signed32 |
| text: record name, period, optional extension | 45 | Fixed 16/4-character producers and mixed-encoding helpers |
| text: unused-record label149 | 16 | All six authenticated labels |
| text: available count + labels85/86 + labels91/92 | 30 | Full signed16 count, both alternatives, all languages |
| text: required-note sentence | 31 | Native `1%s %s %d%s%s`, labels88/90/86/155 and full u8 count |
| suffix: encoded extension copy | 11 | At most four wide characters, escape and paired terminator |
| suffix: page count or zero | 9 | Both producers store a logical-right-shift-by8 value, at most 0xFFFFFF |

The exact matched `func_800A1644` encoder trims its bounded 16/4-code inputs,
then emits either n+1 bytes or FF + 2*n bytes + paired NUL. The authentic
character map is checked to prevent byte-mode entries 0..65 from manufacturing
an FF wide escape. `func_800BE6A4` and `func_800BE4F0` are the actual matched
two-encoding copy/append helpers. A wide name plus period plus extension is at
most FF +2*(16+1+4)+two NUL bytes =45. A byte string promoted by concatenation
obeys the same bound. The append helper's own temporary256 buffer is sufficient
for every such input.

The full native game target call census finds only `track_process_main` and
`track_render_process` calling the encoder and the Pak-list insertion helper.
Both pass widths 16/4 to destinations record+18/+53. The verifier checks those
native argument facts, every call offset, and the native logical shifts/stores
that bound record+68 pages. The polling producer's optional zero store at
extension+1 can only shorten a byte string or alter one wide codepoint; it
cannot remove the existing paired terminator. `func_80398BF0` traverses that
port's Pak list. Thus the bound is for well-formed native nodes from these two
producer paths, including every possible source code byte, not merely a guess
from the decompiler's struct field size.

Native formatter source `src/rom/lib_34a0.c` is reviewed, and its `fcvt`/`sprintf`
function bodies are input-bound: no-width `%d` consumes signed32 and emits at
most sign+10 digits; `%s` copies bytes to NUL. Its unsigned-magnitude handling
includes INT_MIN. The record-page domain is always int-representable; host tests
do not pass out-of-domain unsigned values to a signed variadic conversion.
This static ROM TU is not rebuilt. Any future rebuild must use `tu_cc`.

Width/draw helpers are source consumers: the width path reads encoded characters;
the draw wrapper passes the text onward, and the native queue/draw paths copy it
to their own stack/queue or read glyphs. They do not append to caller buffers.
Their selected native target hashes are included as review inputs. No pointer
into either local buffer survives as an owned record string. Full lifetime and
pointer-mutation behavior under arbitrary corrupted state is not claimed.

Limits: valid loaded language tables and genuine Pak-node lineage, no arbitrary
pointer/character-map mutation, normal control-flow and integer preconditions.
Other inherited renderers' string capacities are not proven by this audit.

## Canonical proof and reproduction

`python3 cloud/work/frontier/dot_runtime_a_writer_bounds_20261012/verify.py --score`
authenticates current-tree image A, then calls the unchanged ordinary group
scorer without `--claims` or `--allow-unverified`:

```text
Members:
func_803A6F2C:
  MATCH
func_803A7180:
  MATCH
func_803A7620:
  MATCH
func_803A7918:
  MATCH
    own .rodata verified at 0x803B9834..0x803B9848
```

- `verify.py` checks input-bound receipts, all four complete extents and next
  boundaries, zero alignment, source-delta contract, every writer bound, full
  native stack census, both causal capacity controls, and five independent
  corrupt-table-entry plus absent-data controls. Missing or invalid language
  resources/selectors are refused.
- `verify.py --deep` additionally GNU-reassembles/relinks all four complete
  members: **223 text relocations and five switch-table relocations**, every
  external binding, function STT_FUNC address/size and full 20-byte table. A
  separate GNU baseline run confirms exactly the two frame words and no other
  differences in the full family. The actual installer relocation/data path
  reproduces all 3,116 bytes and rejects a corrupt table, without installing.
- Deep proof stock-rescores the real encoder/copy/append helpers; checks native
  ABI widths and all accessed Record/Language/index offsets; and replays stock
  UGEN diagnostic-on/off whole-object identity for 100/76/72 capacities.
- Host-only `host.c` compiles the actual candidate with one buffer observer,
  and the actual matched encoder/copy/append source. Authentic strings and map
  live only in temporary inputs. **457,224 calls and 3,583,224 writer invocations**
  cover all signed16 availability values x six languages, all u8 required counts,
  record/no-record and selected/unselected paths, every code byte in every one
  of 20 name/extension positions, all bounded name/extension lengths, polling
  truncation and valid page-count boundaries. Measured maxima are text 45 and
  suffix 11. Format conversion and drawing are hooks; this is a capacity test,
  not native formatter execution or gameplay equivalence.
- Normal and optimized-Python deep replay pass. Focused routine test is
  `REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_writer_bounds.py -o addopts='' -q`:
  **1 passed in 0.55 s**. The inherited packet plus new test also pass together.
  Missing IDO cleanly skips in 0.03 s through shared conftest handling.

Receipts bind sources, selected native inputs, authentic resources and this
packet's verifier. No shared-tool, lock, Makefile or assembly-text pins; no
historical non-ancestor replay. No protected-source edits, ROM data, raw assembly,
objects, credentials or unrelated private data. No hardware, replacement-oracle,
installation, recompression or full-ROM claim.

## Independent review

The paired image-B lane reran normal/optimized deep proof, canonical full-group
score, the focused test (0.56 s), guards and discovery. It independently audited
all buffer writers, formats and integer domains, every supported language input,
both native string encodings, encoder trimming and the full getter/list-producer
lineage. The getter reads the selected port's head at D_80144D68; insertion uses
the same D_80144D60 + port*16 list header. Consumer native paths were checked as
reads or copies into their own queue/stack storage. No blocking finding.

The review requested one narrow strengthening: bind the getter's complete native
target and matched C source, and freshly stock-score it in deep replay. Those
checks were added and independently rerun. Source/group/test logic is unchanged.
An additional full host matrix with ASan and UBSan also passed with the same
457,224 calls and measured bounds; only ptrace-incompatible leak detection was
disabled. This adds no native gameplay or leak-safety claim.

Protected-path guard passes. Ordinary changed-submission discovery reports zero
frontier groups, so the explicit bounded packet test supplies their CI coverage.
Publication must use the final independently reviewed tree; exact-head hosted CI
is separate from these local results and is not claimed before it completes.
