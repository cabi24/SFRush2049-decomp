# B12 — read-only C9 integration audit

All eight C9 bodies independently compile against the current root shared-header snapshot with minimal C9 additions, retaining strict stack-sensitive score zero and raw word difference zero against the current C9 trusted objects. No casts or body adaptations are needed. The private compile produced no warnings or errors. Root remains responsible for actual grouped-TU, resolved retail relocation, lock and full-ROM acceptance.

`integration_B12_proof.json` records literal flags, extracted-source and trusted-target hashes, compiled-context hashes and frozen original C9 source identities. Archive, extracted private TUs, verifier and binaries remain ignored in `build/codex-B12/` or private Rocky B scratch. Only this report/proof were added to tracked work; no shared headers, source, layout, locks or state were changed.

| Body | Target object words | Strict / raw |
|---|---:|---|
| __osCheckId |92|0 / 0|
| __osGetId |108|0 / 0|
| __osMotorAccess |68|0 / 0|
| __osPfsDeclearPage |80|0 / 0|
| osMotorInit |92|0 / 0|
| osPfsInitPak |132|0 / 0|
| osPfsReadWriteFile_pages |56|0 / 0|
| osPiStartDma |52|0 / 0|

Every flags line is `-g0 -O2 -mips2 -G 0 -non_shared`. Object word counts include standalone section alignment, or omit trailing retail zero padding as documented by C9; they do not revise the packet's 2,684 slot-byte coverage claim.

Required public correction: current `include/PR/os_pi.h:80` declares the seven-argument SDK manager API for osPiStartDma. This historical target is the four-argument raw DMA implementation, `s32 osPiStartDma(s32 direction, u32 devAddr, void *dramAddr, u32 size)`. Reconcile that existing declaration, then add the collision-free minimal context from C9. A private copy with this correction and C9 additions compiles all eight exact bodies unchanged.

The C9 report says the public osMotorInit declaration has three arguments. In the actual current `include/` and `rom_tu.h` snapshot, no declaration of osMotorInit exists. The old three-argument declaration is in standalone source preludes. The true new body is `s32 osMotorInit(OSPfs *,s32)` and can be declared additively; there is no current public declaration to replace. `void __osMotorAccess(int,OSPifRam*)` correctly describes the separately mislabeled packet builder.

The remaining additions do not collide with current shared context: __OSPackId, __OSContRamReadFormat, __osMotorPifBuf, ID read/repair/checksum helpers, address CRC, bank-selection macros, PFS ID/label block constants, and motor request constants. Existing ERRCK, ARRLEN, BLOCKSIZE, PFS_ONE_PAGE, OS_READ/WRITE and controller request macros suffice. No duplicate macros were needed. __OSPackId remains the actual 32-byte packet structure; __OSContRamReadFormat is the actual 39-byte byte-field layout. Retain their field widths/order.

Accepted `src/rom/lib_f700.c:21` defines `s32 __osIdCheckSum(u16*,u16*,u16*)`, agreeing with all new callers. Existing `src/rom/rom_tu.h` declares `osPfsReadWriteFile_pages(OSPfs*)`, matching the C6 accepted `src/rom/lib_be40.c:22` caller. Existing C8 two-argument osContStartReadData matches new osPfsInitPak. __osPfsSelectBank and __osPfsRWInode types agree. No accepted static body/prototype conflict was found, and no accepted caller body needs modification.

Concrete linked assembly callers are `asm/us/nonmatchings/rom/lib_b9f0/osPfsGetFileSize.s:47` and `lib_be40/osPfsFreeBlocks.s:22` for the one-pointer pages helper, and `lib_f700/__osGetId.s:42` for the two-pointer ID checker. The ID getter also calls the three-pointer repair helper at line49. No calls to C9 targets were found in the accepted game `src/blob` function bodies; repeated SDK-like declarations there are frozen standalone preludes and are not the shared integration context.

Legacy `src/libultra/os_pi_dma.c:38` defines a seven-argument osPiStartDma, and `src/game/game.c` contains seven-argument calls (examples lines10592 and22464). These files are not linked by the current ROM linker script; Makefile O_FILES excludes the general C_O_FILES list. They do not justify preserving the incorrect shared prototype for the real linked target. `src/libultra/os_motor.c:48` already uses the true two-argument historical motor body. No speculative edits to unlinked legacy code are proposed.
