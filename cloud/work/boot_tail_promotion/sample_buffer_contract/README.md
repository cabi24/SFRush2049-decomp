# Shared sample, buffer, and emitter contracts

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.
This repairs six source-contract refusals, without promoting any slot or editing
locks, protected targets, `rom_tu.h`, symbol addresses, compiler flags, or scorer.
The six prepared candidates total **1,356 bytes**: registry 296; buffer/emitter
1,060. The 18 previously locked C bodies remain textually unchanged.

## What changes

`include/boot_tail_sample_contract.h` holds the real shared type definitions.
The two ROM TUs include it in place of their partial duplicate types. The six
candidate adaptations live in `cloud/work/boot_tail_promotion/sources/`:

- `func_800163A8.c`: use the established `SampleRecord` and `RegisteredSamples`
  types; retain signed storage for `D_800385A0` and an explicitly unsigned loop
  comparison. Rename the old partial resource fields to the shared names.
- `func_8001C508.c`: reuse the complete `SampleBuffer` instead of the
  promotion-only renamed partial type.
- `func_8001C7F4.c`: use the same array's `.mode` byte instead of an incompatible
  `unsigned char[][24]` external declaration.
- `func_8001D4CC.c`: accept `StateNode *` and call the real `void(StateNode *)`
  unlink interface.
- `func_8001DDE0.c`: extend the already-established `StateNode` to its native
  68-byte emitter layout; adapt only the type and the `flags08`/`identifier34`
  field names. All five output locals remain genuinely uninitialized.
- `func_8001E0C0.c`: reset `StateNode *D_8004FD50` and the separate listener-list
  `LinkNode *D_8004FD54`, rather than misdeclaring both as integer globals.

The original standalone matching submissions are unchanged. In particular, the
original emitter semantics and receipt remain available for comparison. The
new prepared sources require the normal ROM-TU `-Iinclude` search path, in
addition to their unchanged recorded code-generation flags. Their strict checks
are explicit in this packet because the ordinary changed-submission gate does
not discover `cloud/work/boot_tail_promotion/sources/`.

## Native evidence and exact layouts

All offsets below are MIPS O32 offsets, not claims about host pointer sizes.
`verify.py` compiles native assertions for every listed size and field.

### Registry

- `SampleRecord`: stride 28; identifier 0, references 2, offset 4, data pointer 8,
  descriptor bytes 12..27.
- `RegisteredSamples`: stride 12; record pointer 0, base pointer 4, count 8,
  remaining halfword 10.
- The real producer `8001605C` scans records by 28 bytes until `0xFFFF`, writes
  a registry entry at `12 * index`, stores the sample pointer at 0, base at 4,
  and halfword count at 8, and clears halfword references at record offset 2.
  Its signed count limit is eight. `800161A0` also uses signed count logic and
  compacts whole 12-byte entries.
- The existing locked `800162AC` forms the pointer `base + offset`, stores it
  at record offset 8 and gives addresses of the descriptor/data pointers to
  `80014CFC`. This resolves the misleading integer `metadata` placeholder.
- Release `800163A8` uses an unsigned registry comparison, increments record
  pointers by 28, stops on identifier `0xFFFF`, decrements a u16 reference count,
  and passes descriptor address plus the offset word to `80014D08`. The native
  `80014D08` is currently a two-input void stub; no unproved resource-freeing
  semantics are attributed to it.
- Release visits every matching record in the first matching registry. It only
  calls `800161A0` when no record in that registry has remaining references.
  Zero references decrement to 65535; no invented underflow guard is added.

The `SampleRecord.offset` name comes from the real acquire path. This packet does
not infer that the release stub uses it as a byte size merely because the old
isolated release candidate called the same word `size`.

### Sample buffer

`SampleBuffer`: stride 24; mode 0, callback pointer 4, short buffer pointer 8,
sample count 12, position 16, callback context 20. The untouched locked `8001C3CC`
uses all these fields. Native `8001C390` clears byte 0 on 24-byte boundaries.
`8001C508` supplies buffer/count to `80014C60`; that actual helper copies four
initial shorts to the buffer's sample-count end, then writes back the guard
region's cache. `8001C7F4` only changes the mode byte and calls channel stop
`8001F954`. Thus a byte-array external and a partial buffer struct cannot be
independent global objects.

### Emitters and listeners

`StateNode`: stride/size 68; next 0, previous 4, flags 8, real opaque object-field
range 12..51, identifier 52, group 56, sound id 60, counter 62, fade 64.
The native constructor `8001D1F4` supplies the group/id/counter fields in the same
object whose next/previous pointers it installs in `D_8004FD50`. The actual
`8001D084` unlinks these pointers, retains only the low 16 flag bits and calls
`8001B8C4` unless the identifier is all ones. Its original locked body and the
saved-next walker `8001D578` are unchanged. `StatePrefix` is now a typedef of
this same object, preserving the `8001D518` source body and ABI.

`D_8004FD54` is a different list: the native listener constructor `8001D764`
links it through offsets 0/4, then writes listener-specific spatial fields.
The existing locked `8001D8B0` uses precisely this two-pointer prefix. `LinkNode`
remains only that prefix; no emitter/listener layout equivalence is invented.

## Verification, including combined translation units

Run from the repository root with the pinned IDO 5.3 and MIPS GNU binutils:

```sh
python3 cloud/work/boot_tail_promotion/sample_buffer_contract/verify.py --out /tmp/sample-contract-proof
python3 -m pytest tests/cloud/test_sample_buffer_contract.py
python3 cloud/work/boot_tail_promotion/sample_buffer_contract/replay_emitter.py
```

The verifier builds three **actual ROM-TU** versions through the existing
asm-processor: base source, repaired source, and repaired source with all six
candidates simultaneously overlaid into their existing passthrough slots.
It does not assemble one small synthetic header-only test as its match proof.
It checks exact `STT_FUNC` extents for every candidate and prior locked body,
resolves every word without masked/unverified/unresolved relocations, and
independently GNU-links each entire module at its real native address.

- `lib_16320`: 7 prior C bodies / 760 bytes; combined 8 / 1,056 bytes;
  all 26 native slots match. Full linked text: 6,816 bytes.
- `lib_1cf90`: 11 prior C bodies / 1,160 bytes; combined 16 / 2,220 bytes;
  all 31 native slots match. Full linked text: 7,504 bytes.
- The entire linked text is identical across base, repaired and combined
  stages. The inherited 12/4 trailing zero alignment bytes are identical to
  baseline; no source padding, keeper, fake argument, or asm trick is added.
- All six adapted standalone functions independently have exact extents and
  full relocated native-byte equality.
- All six original declaration-conflict overlays reject with their recorded
  conflicting symbol. Four incorrect-body controls reject: wrong sentinel,
  signed registry comparison, wrong buffer count field, and invented emitter
  volume initialization. A unit test independently rejects short and long
  function extents.
- Overlay construction handles a later maintainer promotion by retaining the
  actual C definition instead of requiring the old passthrough. Additional
  accepted bodies are checked dynamically; existing locks cannot disappear.

The focused five pytest regressions pass. The adapted host registry/buffer test
uses C89, strict warnings, ASan and UBSan; it covers empty/no-match registries,
first-registry selection, multiple matching records, sentinel stopping,
reference wrap, gated buffer stop, preserved callback context, actual
`8001D084` middle/head unlink, the actual saved-next walker, and pointer resets.

The defined-path emitter replay runs the established independent model, a
host-only consumption-tracked copy, and the untouched adapted source. Only the
host harness's type/field names are aligned to the new shared names. It repeats
**612 safe executions, 172 unwritten-output rejections, and 2 helper-capacity
rejections**; 12 safe carry cases retain 57 values written on earlier iterations,
with 1,362 total value consumptions. The original source is included directly
from disk and its hash is checked before/after. No float defaults are introduced.
The original receipt-only test is replaced by this packet's current standalone
and real combined-TU proof rather than incorrectly applying an old source hash.

A broader local sample of existing cloud guard/scorer/setup/submission/integrity
regressions plus the focused tests produced **671 passed, 15 failed**. All 15
failures are unchanged game-blob single-source checks reporting existing
section-relative `.data`/`.rodata` relocations as unverified. None involve these
six candidates, either TU, or changed scorer inputs. They were not repaired.
This packet does not claim master CI is green.

## Limits and maintainer handoff

The five float outputs of `8001DDE0` are not defined on every native path.
Every consumed output must have been written in the same invocation, possibly
on an earlier emitter iteration. The original producer/listener invariants
that would exclude unsafe paths remain unproved. Empty-list geometry may
leave x/y/z unwritten. The fade constant remains an external `const float`,
and final dispatcher internals are outside the semantic observation boundary.
This type repair neither fixes nor conceals those established qualifications.

Only source preparation and local evidence are delivered. Use the six adapted
paths above when regenerating promotion context and locking/promoting on the
maintainer's builder. Do not reuse the old cached preambles or treat the
current candidate lock metadata as refreshed by this packet. Full-ROM SHA-1,
maintainer promotion, independent review, and merging remain separate gates.
No native object, ROM bytes, or raw assembly output is committed.
