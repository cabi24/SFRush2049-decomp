# Image A: 692 new verified candidate bytes

## Deliverable

`groups/dot_runtime_a_menu_state_20261006/` supersedes the installed group of
the same name. All four verified bodies are both members and claims. The new
body is `func_803990D0`, native extent `0x803990D0..0x80399384`, 692 bytes.
The old `func_80398B40`, `func_80398C3C`, and `func_80398E70` remain exact;
their 1,332 bytes earn no new credit. There is no cross-group lock migration.

Base: master `262e9f0f368914320da53df08102da9682187381`.
This is matching candidate evidence only: no image/recompression, ROM hash,
gameplay or original-source claim. The independent checker owns installation
and merging. Publication remains a draft.

## Trace-first closure

The installed genuine four-body group reproduced at 8/173 differing root
words. Workbench `diagnose` found identical opcodes, register choices and
instruction count, with frame 96 versus retail 112. Resolved stock comparison
located only two frame-size words and six live-local addresses. All live
addresses were 12 bytes below retail: descriptor +56 instead of +68, and
status bytes +93/+94/+95 instead of +105/+106/+107. The three preserved
callee-save homes already agreed. The diagnostic's unlinked relocation
caution was not used as acceptance evidence.

Only four bounded local-layout forms were compiled:

- Original declarations: 8/173, frame 96, descriptor +56.
- Three unused s32 slots after the live declarations: 8/173, frame 104,
  descriptor +64.
- Four unused s32 slots after the live declarations: 6/173, frame 112,
  descriptor +72.
- One unused s32 before the live status bytes and three after the descriptor
  and index: MATCH, frame 112, descriptor +68, all three status homes exact.

The final four slots exactly account for the 16-byte frame residual. Their
placement also accounts for the 12-byte live-home shift. They are explicitly
disclosed compiler-shaping declarations, not recovered original locals. No
live capacity, descriptor size, executable statement, prototype or ABI was
changed. No fake caller, padding array, volatile access, implicit-int callee,
compiler patch or instruction substitution was used.

## Complete acceptance evidence

`verify.py --score` authenticates the current-tree asset and full image A in
memory, then calls the unmodified canonical group command without `--claims`
or `--allow-unverified`:

```text
Members:
func_80398B40:
  MATCH
func_80398C3C:
  MATCH
func_80398E70:
  MATCH
func_803990D0:
  MATCH
    own .rodata verified at 0x803B9508..0x803B951C
```

The new root's complete five-entry, 20-byte jump table is separately checked:
all five R_MIPS_32 relocations resolve to in-root addresses and all twelve
alignment bytes are zero. Corrupting each entry, truncating the table, or
omitting the image is rejected. Without authenticated data, ordinary scoring
correctly reports two unverified own-section references.

`verify.py --deep` additionally performs an independent ELF32 reader/GNU
assembler-linker replay of all four complete members (171 code relocation
records and five table relocations), including the table data. Every word and
full ELF extent equals its native target. It also exercises the actual
installer's packet conversion and complete member/data relocation path in
temporary storage, including a corrupted-table refusal. It never calls
splice or changes production state.

The receipt pins source/recipe/verifier and parsed image-qualified target
words/rows. It does not pin mutable tools, lock files, assembler text or a
historical checkout. The current-tree asset's size/hash, loader pointer and
full decompressed-image identity are checked before use. Native bytes, raw
listings, compiler intermediates and link products stay temporary and are
not included in the packet.

## Reproduction

- `python3 cloud/work/frontier/dot_runtime_a_menu_state_root_20261012/verify.py --score`
- `python3 cloud/work/frontier/dot_runtime_a_menu_state_root_20261012/verify.py --deep`
- `REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_menu_state_root.py -o addopts='' -q`

The one routine test recompiles the real group and the three measured frame
controls, preserves all old matches, checks complete extents and exercises
all seven owned-data failure controls. Shared conftest handles missing IDO.
GNU tooling is only needed for `--deep`; no static ROM TU is rebuilt.

Recorded local checks: one routine test passed in 0.24 s; the shared missing-
IDO path skipped in 0.07 s; optimized-Python `--deep` and canonical `--score`
passed. The packet, renderer owned-data, overlay installer and target-manifest
suite passed 18/18 in 0.63 s after materializing the sparse checkout's image-B
and boot-tail metadata. The protected-path guard passed. No production files
were changed by these checks.
