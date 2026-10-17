# ZOID_SetFog: genuine types, return and packed-color boundary

`func_800A7480` is the N64 fog/color setter: **34/34 words, 136 bytes, strict MATCH**. The unchanged canonical background setter (`func_800A5560`, 28 bytes) also strictly matches. The native root is frameless. Only the 136-byte root is a new candidate claim; the helper gets no additional credit. Accepted image/ROM credit is **zero**.

## What the donor resolves

The exact accessible MAT3 `ZOID_SetFog` reconstruction and its original DWARF agree on:

- Signed-short minimum and maximum parameters.
- Four unsigned-byte color parameters and an unsigned window index.
- A returned pointer to the selected 0x48-byte record, with one meaningful local pointer.
- Signed-short fog minimum at +0x40, unsigned-short fog maximum at +0x42, and four color bytes at +0x44.
- Packing from the RGB parameters, narrowing the combined masked RGB value to unsigned short, then OR-ing the final opaque-alpha bit before passing it to ZOID_SetBGColor.

The final N64 candidate uses those genuine types, local and return, and the same narrowing expression. The passed alpha is stored in the fog record, while the background packed color always uses alpha1, as both native and donor code specify.

Starting from the tracked natural unsigned-color draft (`w9d/func_800A7480/a.c`) in real helper context:

- Historical void body: 33/34 words differ.
- Genuine record-pointer return alone: 9/34.
- Narrowing-before-alpha alone: 33/34.
- Both changes: exact. Correct unsigned index/maximum-field types and the donor local pointer remain exact.

The older 17/34 `best.c` uses pointer laundering and signed-red shaping. It is not a source baseline or accepted result here. There is no pointer laundering, unused padding, dummy caller, forced register, Ucode edit or assembly replacement in this packet. Direct array indexing is an exact positive control with identical executable/data/relocation sections; the donor-style local is retained for clarity. No exact historical N64 spelling is asserted.

MAT3 provenance: `GameSource/src/zoidwindow.c`, ZOID_SetFog at 0x800631E4 (108 bytes) and ZOID_SetBGColor at 0x80063B2C (16 bytes); source SHA256 `ce9be99cc01cacae7aa684ee42b476348ea48c585c32ad6fb7d48adc59bf9d25`. Existing matching reports cover both functions. This packet does not claim a fresh MAT3 compile.

## Verification

With stock IDO 5.3 and MIPS binutils configured:

```
python3 cloud/work/frontier/dot_mat3_setfog_20261017/replay.py --check
python3 -m pytest -q tests/cloud/test_mat3_setfog.py
```

`--materialize <empty-directory>` produces the isolated group with the unchanged canonical background setter. The source verifier permits only replacing the historical duplicate helper with its real declaration, restoring the pointer return, the RGB narrowing boundary, genuine unsigned index/maximum-field types and the donor local. `group.json` claims only the root; the replay checks both functions strictly and enforces their 136/28-byte ELF symbol extents.

Two fresh builds and the direct-index positive have identical executable/data/relocation receipts. Target/context/source hashes and full scores are bound in `proof.json`. Wrong alpha, green mask, minimum or returned window all fail while preserving the helper. Semantic negative runs require the exact differential exit code and message.

The host harness checks **262,144 full-state/return cases**, covering every signed-short value for both range parameters across all four windows, with unsigned-byte color and alpha variation. It also exhausts **16,777,216 RGB triples**, comparing both packed expressions against an independent bit-slice formula. Untouched records, other fields, background color, helper-call count and returned pointer are checked. UBSan is fatal.

The host background helper deliberately uses unsigned 32-bit shifts to model the native machine operation. The unchanged canonical helper's historical C spelling has signed integer promotion on its left shift for high colors; this packet does not claim that spelling is defined ISO C for every input or modify the already-locked helper. Exact native helper comparison is independent of this bounded host model. The signed maximum is converted to its unsigned-short field with the intended modulo representation. Tests assume valid indices and storage; they do not claim invalid-pointer or out-of-range-index behavior.

This is an isolated candidate packet, not canonical integration. Whole-unit/image/ROM gates remain pending, and unrelated caller declarations are not harmonized here. No ROM bytes, raw assembly, literal target-data dumps, compiled objects, private traces or secrets are included. Independent review is recorded separately.
