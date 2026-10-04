# BT05 ID-register retry: bounded negative result

**One reopened 172-byte NONMATCH, no new attempted target, no matching body or verified bytes.** The single source-family control worsens the O2 residual from 2/43 to 7/43 words. The original two composition sites remain different. The better frozen `BT05-macro-five/nonmatch/func_80022324.c` stays selected; nothing is submitted under `cloud/matches/`.

- Base `f02638ed8388bd3e5256e0c31b2ae36b6be9524a`; branch `dot/boot-tail-bt05-id-register-retry`.
- Exclusive central claim covered precisely one donor-derived form with O2/O1, then stop unchanged/worse. Baseline was freshly diagnosed before the candidate was edited.
- The old macro-five packet, its 34 sources/68 compiler rows, and all frozen integration sources remain untouched. This directory alone holds the retry.

## New evidence and why it justified one control

The pinned CC0 [AxioDL MusyX `mcmdSendKeyOff`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c#L352-L363) uses an actual unsigned-byte extraction before adding the voice's note and shifting by eight. Its named helper, identifier comparison, word-sized loop index, and live voice-count bound strongly match this native role. Blob and licence provenance are in `references.json`. It remains a PC/Dolphin source-family lead, not proof of the original N64 translation unit.

The donor header declares `orgNote` as `u8`; the N64 target instead proves an unsigned halfword. This retry preserves the native halfword and its full domain rather than importing the donor layout or narrower note type.

All ten archived 22324 forms used the unsigned-word `& 255` extraction. Their sum is unsigned. The new form combines the native unsigned-halfword field with an actual unsigned-byte conversion; C integer promotions make the sum signed `int`. This is a distinct genuine width conversion, not a keeper, artificial stack/formal, narrowed sum, or new range assumption. The complete donor expression also puts the voice operand first; this is one combined source-form control, not an isolated causal test of signedness.

The high-halfword operation deliberately retains the older unsigned-word shift. Copying the donor's promoted unsigned-halfword left shift directly would make high values unrepresentable as signed `int`. No such undefined expression was introduced.

### Full-domain arithmetic proof

For every native state halfword `g` in 0..65535 and extracted byte `b` in 0..255, both promote to 32-bit signed `int`, their sum is 0..65790, and shifting it left eight gives 0..16842240, which is representable. Assignment to `u32` is exact. No carry is truncated, including `g+b > 65535`.

For every command word, `word0 >> 16` remains `u32` and its left shift is unsigned. OR composition remains defined over all 32-bit patterns, including overlap between the shifted sum and high-half tag. The source does not assume musical-note bounds, a zero high bit, or a smaller key domain.

## Native body, ABI and liveness audit

The protected entire 43-word body reads an unsigned packed halfword at voice+0x4E and an aligned command word. It creates the full-width sum, shifts it by eight, and ORs the command's high halfword. It compares `composite | index` to the unaligned identifier at +0x60 of successive 416-byte voice records.

The genuine two-pointer caller in `80023E9C` supplies voice in a0 and command in a1 at its +0x404 call; its continuation consumes the low return byte. The actual callee `80021700` consumes one full unsigned identifier, checks the sentinel and stored ID, sets flag8 and returns an integer. Its complete native body and existing independent matching C agree. No extra formal, return contract or callee body is part of this candidate.

The composite survives calls and the index advances through the live byte count. Count is reloaded after real helper calls; no snapshot or const-qualified memory assumption was added. The temporary at the two residual sites does not mean any source value remains semantically live there: whole-body observation alone does not identify which compiler pass owns the difference.

Fresh workbench diagnosis of the unchanged baseline reports 43 instructions on each side, equal 40-byte frames, zero opcode/structural distance, and exactly two register sites: shift destination at +0x40 and OR input at +0x48. Native keeps the composition in s2; baseline routes the intermediate through t9. Owning pass remains unknown with only heuristic evidence. `diagnosis_summary.json` publishes metadata only; temporary objects and raw reports stay outside the repository.

## Results and stopping decision

| Source | O2 differences | O2 ELF symbol | O1 differences | O1 ELF symbol |
|---|---:|---:|---:|---:|
| Frozen best | 2/43, 0 extras | 172 B | 43/43, 10 nonzero extras | 220 B |
| New donor-width form | 7/43, 0 extras | 172 B | 43/43, 10 nonzero extras | 220 B |

All four fresh rows have zero unresolved symbols, unverified relocations, masks and errors. Full text relocation and ELF symbol extents are checked separately; no comparison accepts trailing nonzero content. The seven O2 register sites are +0x1C, +0x28, +0x30, +0x34, +0x38, +0x40 and +0x48. The original two sites survive unchanged and five earlier sites worsen. No instruction count/frame benefit appears.

This closes only the one approved combined source form. It neither disproves the source-family identification nor establishes that every natural source is exhausted. Further retries need genuinely new N64 translation-unit evidence or authenticated compiler lifetime/reservation evidence. No operand-order, cast, storage, declaration, or flag sweep followed this negative result.

## Reproduction and checks

With the pinned IDO environment:

```sh
python3 cloud/work/boot_tail/BT05-id-register-retry/verify.py
python3 cloud/work/boot_tail/BT05-id-register-retry/test_semantics.py
```

`verify.py` binds all four rows to source/tool hashes, manifests, the inventory and native extent. The actual candidate passes C89 host execution with undefined-behavior sanitization for all 16,777,216 halfword/byte pairs (with varying full-width tags), every high-half tag at low-composition boundaries (131,072 further cases), zero/maximum voice count, selective mismatch, and live-count growth/shrink after test calls. IDO separately verifies 32-bit pointers/words, 416-byte state stride, 8-byte command size, and the two accessed field offsets. `semantics.json` records the result.

These tests do not exhaust all 2^32 command words crossed with all state values; the algebra above supplies the independent full-domain proof. Host pointers and endianness differ from N64. Count-changing callee implementations are test-only and are never compiled into scored C. They test the retained loop's behavior when an external call changes memory, not a claim that native 21700 changes the count.

The first standalone layout-check attempt lacked an IDO `stddef.h` include; the successful check uses the same direct field-address assertion idiom as the frozen packet and needs no header. No toolchain or protected file was changed to resolve it.

No targets, symbols, layout, lock, compiler/scorer, production gate, large-body implementation or restricted helper context changed. No ROM, raw disassembly, object, credential or unrelated private data is published. No merge or coverage claim is made.
