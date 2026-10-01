# B11 — read-only C8 integration audit

All eight C8 bodies compile against a snapshot of the current shared `rom_tu.h` and `include/` context with strict stack-sensitive score zero and zero raw word differences against C8 trusted objects. These are extracted single-body context checks; grouped TU, resolved retail relocation, lock and full-ROM acceptance remain the coordinator's responsibility. No shared source, layout, locks, or state was changed by this worker.

Proof: `integration_B11_proof.json` records source and target identities, literal flags, per-header snapshot hashes and the ignored context archive identity. Builder: Rocky B scratch, one compiler/scorer at a time. Scripts/archive/binaries remain under ignored `build/codex-B11/` and private `~/agents/B/scratch/codex_B/`.

| Body | Words including target section padding | Flags | Strict / raw |
|---|---:|---|---|
| osPiReadWord |20|O2|0 / 0|
| osContStartReadData |52|O2|0 / 0|
| __osPfsSelectBank |32|O2|0 / 0|
| __osContBuildPacket |88|O2|0 / 0|
| __osPiRawStartDma |48|O2|0 / 0|
| osPfsGetFileStat |56|O2|0 / 0|
| osPfsFindFile |36|O2|0 / 0|
| osCreateViManager |56|O1|0 / 0|

All flags are `-g0 -O2 -mips2 -G 0 -non_shared` except the stated O1 thread body. Section padding is not new function coverage.

Required correction: preserve the accepted `s32 __osInsertTimer(void)` declaration/body in `src/rom/lib_e9a0.c`. Cast its returned pointer bits at both new DMA queue uses, `osSendMesg((OSMesgQueue *)__osInsertTimer(), ...)` and `osJamMesg((OSMesgQueue *)__osInsertTimer(), ...)`. Private full corrected TU `build/codex-B11/__osPiRawStartDma.c` SHA256 `a4a4edf9458948988e28c4fa26f9b67282fa35b5d812e89b7e8c9ca1f45acc28` independently retains strict/raw0. Original frozen C8 file is untouched.

Preserve the existing accepted C4 `OSThread *__osActiveQueue` shared declaration. C8 standalone preambles contain the old double-pointer type, but the new thread body already casts the loaded head and its address correctly. Private full preamble-corrected TU `build/codex-B11/osCreateViManager.c` SHA256 `2a5246d519b7a0c4a583620e1b169b7aed39b5dacd1bfdb20baa65f1995a354e` retains strict/raw0. Thread helpers `dll_remove(OSThread**,OSThread*)`, `__osEnqueueThread(OSThread**,OSThread*)` and `__osCleanupThread(OSThread**)` match existing accepted callers.

The C3 accepted timer body and header agree with the five-argument `osSetTimer(OSTimer*,OSTime,OSTime,OSMesgQueue*,OSMesg)` used by new controller initialization. Controller builder/parser, SI DMA, inode read/write, bank select, and controller status helper declarations match their new C8 call uses. No additional conflicting accepted C definitions were found.

`rom_auto.h` already includes controller and PI headers. Five new definitions caused IDO redefinition warnings in the snapshot: CONT_CARD_ON/PULL/ADDR_CRC_ER and OS_MESG_TYPE_DMAREAD/DMAWRITE. OS_MESG_PRI_HIGH is also redundant. Root removed these duplicates; their values agree with existing definitions. BLOCKSIZE is already present from C6. Root also corrected the semantic parameter labels in the five-argument osPfsFindFile prototype to start_page then bank; the two u8 parameter types and ABI do not change.

Real caller evidence agrees with the reconstructed signatures: `asm/us/nonmatchings/rom/lib_b300/osPfsRename.s:79` passes start_page in a2, bank in a3 and last_page at sp+0x10; `lib_b9f0/osPfsGetFileSize.s:250` supplies queue and channel to osContStartReadData; `lib_5610/lzss_decode.s:39` supplies seven PI DMA arguments; `lib_8e10/osCreatePiManager.s:46` uses the two-argument thread function.

Existing promoted `src/rom/lib_8dd0.c:osPiRawReadWord` calls typed osPiReadWord with a historical s32 second formal. That produces an integer-to-pointer warning, but private disassembly is instruction-identical except the three ordinary unresolved jal relocation fields. Its literal retail target has resolved calls, so this separate caller check is raw3/strict15 before relocation and is not a new score-zero claim. Root preserves its accepted body. No actual C8 calls were found in accepted `src/blob` bodies; the numerous old signatures there are standalone preambles. `src/game/save.c:13` retains a six-argument SDK-style osPfsFindFile declaration and uses, but that legacy file is not linked by the ROM linker script or Makefile O_FILES; it is outside this integration packet.
