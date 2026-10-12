# Image-A unlock panel: 1,664 fresh strict-matched candidate bytes

New body: **func_803AB848**, complete `[0x803AB848,0x803ABEC8)`,
416 words / 1,664 bytes. The seven peer-reviewed bodies from draft #344
remain exact and claimed in the same `runtime_a_stats_menu_20261006` group:
AAF0C, AB5FC, AC330, AC54C, AC7F4, ACD5C and ACF5C, **5,136 preserved
candidate bytes, no repeated credit**. Five genuine complete contexts remain
explicitly NONMATCH; only their AD57C callback root is kept. No lock migration.
Zero new accepted-image or ROM-verified bytes. Keep draft; the independent
checker owns installation and merging.

Base master: `3bec9ce30b8fbc707e2e1d6c5717826c03646232`. Live claims, overlay
locks and open PRs were checked and AB848 claimed before code changes.
The source starts from peer-reviewed draft #344, remote head
`8000d8f934045aab19d394e66c497cd8593a1849`; prior source bodies are preserved.
The verifier reverses only this continuation and the known AB5FC/numeric
changes and requires byte identity with the unchanged current-tree lean seed.

## Source and compiler evidence

Fresh complete baseline: **394/416 words differ**, emitted 1,600 bytes against
native 1,664, frame 120 against 112. Workbench diagnoses real geometry and
allocation differences; its relocation-blind counts are not acceptance evidence.

1. All six unsigned maximum comparisons must also have unsigned result arms
   before assignment to signed16 width. This restores native signed-halfword
   narrowing, including the retained-width arm. Repeated width-measure helper
   calls are genuine and remain repeated.
2. Initialize both existing scan flags after profile restoration, matching
   native lifetimes. Express the first scan's inactive-entry path as an early
   `continue`, rather than a nested positive test. Both have the same logical
   results, but the actual early-exit block layout changes the optimizer's
   constant priorities. A diagnostic-only oracle first isolated the constant1/
   constant2 and width color mismatch; the natural early-continue source then
   reproduced the complete 1,664-byte geometry with only 15 differing words.
3. Store temporary profile owner before one-player count, as in sibling AB5FC.
   This restores the four initial t7/t8 words, leaving only 11 frame/home words.
4. The old 48-byte text capacity was guessed. The sole write is the authenticated
   `%s %s` format, with NAME(44,i), i=1..3, and ordinary text index42. Native
   language loader/parser analysis and all six authentic resource tables give
   a complete bound of **24 bytes including NUL**. A real 44-byte buffer is safe
   within this supported-language contract. Move the existing first flag's
   declaration ahead of saved_owner to recover every local home: text at sp+44,
   bottom at sp+94, and frame112. No added local or smaller string truncation.

Original variable types, declaration order and capacity are not claimed to be
recovered. They are safe source/layout inferences supported by the entire native
body. No unused local, fake parameter, compiled-out read, volatile qualifier,
inline assembly, modified helper, fake caller or patched acceptance compiler.

Stock UGEN diagnostics on/off give byte-identical entire objects. Final
M80/T20/P4 produces frame112; the old48 control has M84/T20/P4 and frame120.
The nested-scan control retains frame112 but selects width in s5 instead of
native s6. These diagnostics independently replay through `trace.py`.

## Capacity and ABI scope

`capacity.py` authenticates the current asset, game image and language loader
ROM/codec tables. It parses all six table sections using the native locked
A4E58 layout, validates every string-table pointer, selects all 18 indexed
choices and the six matching label42 values, and verifies termination and
byte-mode encoding. Per-language maxima including NUL are 21, 20, 20, 24, 19,
16 bytes. No player-entered name, arbitrary integer, dynamic format, wide-string
path or other writer reaches this local text buffer. Invalid language kinds and
an out-of-range selector are rejected. The full native loader/helper targets and
formatter core are hash-bound; no resource/string/native payload is published.

The bound is native-flow and fixed-resource analysis plus a byte-count oracle,
not freshly executed native formatting. Assumes valid authentic loaded language
tables and valid backing/pointers, excluding arbitrary pointer mutation. The
native main-image helper implementations remain external contracts. AB848 is
private to genuine AD57C at call offset0x79c; its whole-group ABI is proved,
not advertised as standalone O32. No static ROM TU is rebuilt.

The inherited numeric buffers and two jump tables retain their prior
12/10/6-byte write bounds and full owned-data proof. They earn no new credit.

## Reproduction and proof

From this checkout with stock IDO 5.3 and GNU MIPS binutils:

```
python3 cloud/work/frontier/dot_runtime_a_unlock_panel_20261012/verify.py --score
python3 cloud/work/frontier/dot_runtime_a_unlock_panel_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a_unlock_panel_20261012/verify.py --deep
python3 -m pytest -q tests/cloud/test_runtime_a_unlock_panel.py
```

Canonical `score.py group` runs without `--claims` or `--allow-unverified`:
all eight members MATCH. The two inherited table owners verify their complete
rodata windows at 0x803B98E8..0x803B990C and 0x803B990C..0x803B9928.
All five NONMATCH contexts retain explicit scores and unresolved ownership.

Protocol-v2 receipt binds source/recipe/test/verifier/capacity/trace files,
selected target words and native extents, referenced symbol addresses, current
asset/image and resource identities, and all eight stock compiler passes.
It does not pin mutable scorer/tool/lock/generated assembly text.

Every member has its complete ELF extent, next-function boundary and zero
nonzero suffix checked. Independent GNU assembly/link resolves **308 text
relocations plus all 16 inherited jump-table relocations**, and compares every
complete relocated word and all64 owned table bytes. The complete GNU baseline
reproduces all394 differences and its short1,600-byte extent, counting missing
words as differences. Missing/corrupt inherited tables fail closed.

Rollback controls, each starting from final source:
- Signed result arms:348 differences
- Early flag initialization:19
- Nested first scan:395
- Profile store order:4
- Old48 capacity:11
- Old declaration order:8
- Complete original AB848 baseline:394

All seven prior exact bodies remain exact in every control. Normal and Python-O
deep proofs pass. One focused WITH-IDO test passes; WITHOUT-IDO cleanly skips
through the shared conftest. The explicit packet test provides group CI coverage;
ordinary changed-single discovery is not this packet's acceptance gate.

No full gameplay/hardware test, unrestricted memory/aliasing proof, original TU
recovery, private image/compression rebuild or cartridge SHA-1 is asserted.
