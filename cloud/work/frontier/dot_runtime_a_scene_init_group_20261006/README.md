# Runtime-A BE48: code and owned-literal candidate

`func_8039BE48`, `[0x8039BE48,0x8039C140)`: **760 bytes / 190 words MATCH**
at actual `-g0 -O3 -mips2 -G 0 -non_shared`, stock IDO 5.3 group pipeline.
No differing words, unresolved symbols, unverified relocations, relocation
errors or nonzero extra words. Candidate-owned `.rodata` matches native
`0x803B95B4..0x803B95B8`.

Update: the original draft deliberately left its scale anchor unresolved.
The authenticated image A supplies the real binary32 value **1.01f**. Using
that literal preserves all 190 code words and verifies the emitted owned data.
`group.json` now claims BE48 alone as a local candidate. This upgrades the
previously reported 760-byte text-only lead; it is not another 760 bytes of
newly discovered code and is not accepted cartridge coverage.

## Genuine source context

The complete initializer synchronizes players, normalizes configuration state,
conditionally creates two resources and applies texture/transform data, refreshes
flags, and creates UI objects. Separate real selector and resource-result locals
recover the native frame/register assignment. The selector snapshot precedes
the initialization-flag store. No unused frame filler, fake caller, dummy read,
private compiler flag or artificial inlining barrier is present.

The actual main-blob allocator wrapper stays external to runtime-A compilation.
Its returning body is documented separately as `wrapper_return.c`, listed only
in `external_sources`, never compiler `files`.

The full real C140 parent remains unclaimed at 484/496 differing words in this
minimal source context. B120/B214/A448 retain their earlier matching code scores;
none is new credit here. This extends #199/#190; do not independently install
all copies or count shared helpers twice. The other caller/body/data boundaries
are unchanged and remain for the checker.

## Authenticated in-memory data and reproduction

Fixed base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
`reproduce.py` reads that commit's existing `assets/us/data.bin` in memory,
requires length 12,418,096 and SHA-256
`f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8`, and checks
the loader pointer at asset offset 0x3850 is 0xB5C534. Raw-deflate extraction
from asset offset `(0xB5C534 - 0x283D0)` must yield 194,128 bytes, SHA-256
`0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667`, loaded at
0x8038A400. It then binds the ordinary scorer's owned-data image to those bytes.
No ROM bytes, extracted image or binary outputs are added by this draft.

With stock IDO and GNU MIPS tools configured:

```
python cloud/work/frontier/dot_runtime_a_scene_init_group_20261006/reproduce.py \
  --repo . --reference-root .
```

Use `--reference-root` for a separate authorized fixed-base object store when
working from a small source overlay. The script performs matching compilation
and scoring, not acceptance tests. Without the authenticated image binding,
the ordinary CLI correctly leaves the emitted own-data reference unverified.

No semantic packet, full tests, image/compression/ROM gate, CI wait, production
promotion, integration-safety or accepted-coverage claim is included. The
independent checker owns acceptance and merging.
