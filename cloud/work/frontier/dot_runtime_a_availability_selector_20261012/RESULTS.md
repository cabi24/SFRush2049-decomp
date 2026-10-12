# Availability producer: new 720-byte strict match

Deliverable: `groups/dot_runtime_a_availability_refresh_20261007/`.
New image-A candidate: `func_80394564`, 720 bytes. Existing `func_80394834`,
400 bytes, is preserved and excluded from new credit. No installed-image or
ROM coverage is claimed. Both verified bodies are members and claims; the
same group name preserves ownership. The genuine 97FFC and 94B00 callers
remain NONMATCH context.

Fresh stock baseline was 3/180 words off, entirely the selector's entry a0
instead of native a1. Stock-faithful uopt trace identified p2:w2=a0; the
single diagnostic force p2:w2=c4 made all 180 words exact. Reusing the consumed
selector as the outer loop index naturally supplies the native a1 allocation.
The diagnostic compiler is never used for acceptance. No dummy formal,
padding, extra declaration, narrowed live buffer, or fake helper is present.


## Verified result

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_availability_selector_20261012/groups/dot_runtime_a_availability_refresh_20261007 --targets asm/us/ovl_a
Members:
func_80394564:
  MATCH
func_80394834:
  MATCH
```

No `--claims` or `--allow-unverified` switch is used. The claimed members own
no data. The caller contexts still report 97FFC at 260/268 words (12 excess
words, four unverified own-data sites and an unpaired HI16), and 94B00 at
162/182. Those contexts are not spliced or claimed. A successful group exit
accepts only the two listed members, not the surrounding callers.

The native producer is frameless and uses private IPA registers, including
unsaved callee-saved registers. It is not a standalone o32 function. The genuine
native callers pass the selector in a1: 94B00 at +0x2c0 takes it from player s0;
97FFC at +0x1c8 passes -1. Both native call-site windows are input-bound and
the argument-setup encodings are checked. The source keeps its genuine one-
argument signature and uses real complete callers. No invented ABI slot.

`verify.py --deep` and optimized-Python `-O` replay pass:
- 720-byte producer and preserved 400-byte refresh have complete exact STT_FUNC
  extents; neither has trailing padding or borrowed neighboring instructions.
- All four object-function boundaries and the complete 2,976-byte text section
  are checked. The final nonmatching caller has 12 zero alignment bytes.
- GNU as/ld independently resolves all 42 member relocations, at explicitly
  pinned native section and symbol addresses; all 280 words equal native.
- A corrupt instruction and an overlong function symbol are rejected.
- Restoring a separate index reproduces exactly three differences while keeping
  the preserved refresh exact. No uncertainty or excess exists in that control.
- 25,000 actual-source C89 host cases pass with undefined-behavior/bounds
  sanitizers, strict aliasing and warnings-as-errors. Cases cover selectors -1
  and 0..3, active counts 0..4, mapped status -4..9, busy/mode flags, all 14
  options, final counts, and untouched nonselected rows. Explicit coverage masks
  require every mapping ID, status -4..9, mode 0..5 and busy value 0..2. These are bounded
  host fixtures, not native/gameplay equivalence or arbitrary-input safety.

`trace.py --tracer PATH_TO_CDX_UOPT` optionally repeats the causal trace. Both
unforced complete objects equal stock output. Separate index gives p2:w2=a0;
reused selector gives p2:w2=a1. The single diagnostic force p2:w2=c4 on the
baseline produces MATCH. Acceptance never uses that diagnostic object.

One routine test, `tests/cloud/test_runtime_a_availability_selector.py`, passes
in approximately 0.15 seconds with stock IDO/GNU. Missing IDO cleanly skips
through the shared conftest. Receipts pin sources, this verifier, target rows,
parsed native words and actual native call windows, not mutable tools, locks,
or assembly file text. Only digest and summarized proof data are committed.

No arcade ancestor is asserted. Installation, image/compression gates, ROM
SHA-1 validation and merging remain the independent checker's responsibility.
No protected source, lock, target manifest, raw assembly dump, ROM bytes,
compiler object, credential or unrelated private data is changed or included.

Independent peer review caught low-bit PRNG correlations in the first fixture.
The corrected fixture consumes high bits and checks all advertised value sets;
all 25,000 cases were rerun. The matching source/proof outcome was unchanged.
