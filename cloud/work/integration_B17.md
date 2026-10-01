# B17: VI creator/worker relocation and BSS provenance

The creator's one-word strict mismatch is a legitimate equivalent-address relocation-expression issue. A private two-site target alias produces strict/raw zero without scorer changes, and both creator source and aliased target independently link to all 96 exact retail words. The worker's standalone local static counter remains unverified under the current linker. Authentic full SDK VI BSS ownership can explain it: a private complete creator+worker declaration context reproduces all 196 retail text words when linked in a derived full-layout model. That model is evidence for restoring the original storage block, not acceptance under current root state.

No root assembly, metadata, headers, link script, source, lock, farm state or match delivery was changed. This report and `integration_B17/` contain source/hash proofs only. Objects, linked images, disassembly and private compiler scratch remain ignored under `build/codex-B17/` and Rocky `~/agents/B/scratch/codex_B/`.

## Creator: genuine stack endpoint

Canonical `reference/repos/ultralib/src/io/vimgr.c` declares static OSThread viThread, a real OS_VIM_STACKSIZE stack, an aligned event queue, five message words, two OSIoMesg event objects, and the worker's function-local static u16 retrace. It passes STACK_START(viThreadStack) to osCreateThread. SDK `PR/os.h` defines OS_VIM_STACKSIZE as 4096 and `PRinternal/macros.h` defines STACK as an array of u64 and STACK_START as its byte end. These are original storage objects, not fabricated padding contexts.

The SDK thread context contains sixteen eight-byte FP pair slots, giving OSThread size 0x1B0. Thread 0x800354F0 plus 0x1B0 reaches stack start 0x800356A0; adding 0x1000 reaches 0x800366A0, the separately named queue boundary. Assembly `asm/us/7630.s` loads that exact stack endpoint into t2 at PCs 0x80006B14 and 0x80006B24, then stores t2 as the fifth argument to osCreateThread. The disassembler names the operand gViMgrMesgQueue because the stack endpoint and following object share an address.

The frozen C12 stackfield creator instead relocates `gViMgrThreadArg +0x11B0`. Against the existing trusted object this is strict 10/raw 1; the single raw difference is its genuine relocation addend. Both forms resolve to the same authoritative physical address and all 96 retail words. In private `osSetEventMesg_stackalias.s`, only the t2 HI/LO pair is expressed as `%hi(gViMgrThreadArg +0x11B0)` / `%lo(gViMgrThreadArg +0x11B0)`. No instructions, other symbol references, extent, scoring rules or masks change. The newly assembled private target scores strict/raw 0 against the identical frozen creator source. Both independently linked 96-word prefixes hash `02759e49d1d8b507ba5c5640ed2e723f8415ba028f76140cea631747c0485521`, equal to the actual ROM.

This supports a narrowly scoped authoritative target-expression normalization, if root adopts it through the normal target-refresh/extent/hash gates and proves ROM identity again. The current original trusted target remains recorded honestly as strict 10/raw 1. B did not register the alias, change DB target blobs or call it already accepted.

## Worker: local static is real, standalone binding is absent

The SDK worker genuinely declares function-local `static u16 retrace`; replacing it with global/extern/volatile declarations changes IDO allocation, as C's controls demonstrated. Independent B compilation reproduces the frozen local-static source strict/raw 0 against the current worker target. Nevertheless its candidate relocations reference object-local `.bss+0` at ten text sites. They are not an external symbol named gViMgrRetraceCounter, and the current linker does not bind them to that name automatically.

The retail assembly loads/stores the counter with HI/LO pairs for gViMgrRetraceCounter at 0x80036700 (for example PCs 0x80006BE8/0x80006BF4 and 0x80006C5C/0x80006C60). Current `rush 2049.us.ld` links original asm 7630.o text/data/rodata/BSS; the current map places its zero-sized BSS contribution at 0x8000F400. Therefore appending one standalone local counter to a new arbitrary BSS contribution would not reproduce retail storage. An assignment from arbitrary `.bss+0` to 0x80036700 would conceal missing original storage ownership and is not justified.

## Authentic full storage model

`vi_real_bss_context.c` reconstructs the full original declaration context using the two real bodies and genuine SDK objects in order. It preserves the function-local static counter. The only thread type correction is using SDK-width u64 FP pair slots privately; the actual current shared header and C12 standalone prelude both use f32 pair fields, yielding a 0x170 OSThread instead. B did not change these shared types. This is a material layout caveat for any future sizeof-based aggregate reconstruction.

Private IDO O2 compilation produces 0x310 text bytes and 0x1220 BSS bytes. Relocation addends establish this real layout:

| Object | BSS offset | Size | Derived physical address |
|---|---:|---:|---:|
| Thread |0|0x1B0|0x800354F0|
| Real 4096-byte stack |0x1B0|0x1000|0x800356A0|
| Queue |0x11B0|0x18|0x800366A0|
| Five message words |0x11C8|0x14|0x800366B8|
| First OSIoMesg |0x11E0|0x18|0x800366D0|
| Second OSIoMesg |0x11F8|0x18|0x800366E8|
| Worker local u16 retrace |0x1210|2|0x80036700|

Every named physical boundary agrees with `symbol_addrs.us.txt` and retail assembly operands. The original symbol comments saying ThreadArg size 4 and event objects size 8 are incomplete annotations; the actual thread and OSIoMesg layout, accesses and next-object boundaries demonstrate the larger objects. B changed no symbol annotations.

A read-only GNU linker model anchors the full reconstructed BSS block at the known thread start 0x800354F0, resolving every subsequent location through these authentic offsets. It uses current root's text SUBALIGN 16, resolves external symbols only through their existing authoritative image addresses, and compares the entire 196-word prefix beginning ROM 0x7630. The full prefix is exact, hash `ea7fa7d6effd8003aaf38622f1396826cd7914d9402d95e230bb2aa0b89b4cb5`. No counter-only assignment, borrowed relocation fields or masks are used. The model script is explicitly named `vi_real_bss_context.model.ld`; it is not the current root link script and does not constitute completed acceptance.

A valid future integration must own/place the entire authentic VI BSS block and preserve the existing queue/thread/event/counter symbols and all neighboring storage. Its actual ELF/map and full-ROM gates must confirm the derived placement. The current standalone worker should remain blocked until that storage integration exists. The creator's external-address alias can be assessed independently without restoring BSS.

## Frozen evidence

`proof.json` records source/target hashes, literal `-g0 -O2 -mips2 -G 0 -non_shared`, captured compiler exits and strict/raw comparisons: original creator 10/1, private alias creator 0/0, standalone worker 0/0 with the linked-binding caveat above. `linked_models.json` preserves independent actual-retail comparisons and explicit model limitations. `layout.json` freezes every genuine storage offset; `source_hashes.json` freezes SDK, authoritative assembly/symbol/linker inputs and reconstructed sources. The target object hashes in `targets.json` are unchanged authoritative DB identities. No new coverage is claimed by this audit.
