# Symbol Misattribution Audit (libultra timer / thread cluster)

**Audited 2026-07-14** against `reference/repos/ultralib` (decompals). Several
symbols in `symbol_addrs.us.txt` were given generic or wrong names. This blocks
`corpus` name-pairing (it pairs a target to a same-named canonical function) and,
worse, causes the canonical `__osTimer*` names to point at unrelated functions.

## Verified findings

> Update 2026-10-11 (lane w22g): `dll_get_priority` is now `osGetThreadPri` and
> `osSetTimerIntr` is now `osViSetEvent` in the inventory; see "Libultra inventory
> rename" below. The rest of this table is unchanged.

| Addr | Current label | Actually is | Evidence | Status |
|------|---------------|-------------|----------|--------|
| 0x8000C050 | `dll_remove` | **`__osDequeueThread`** | walks singly-linked list at offset-0 `next`, unlinks arg match — exact ultralib `__osDequeueThread` | **clean** (no collision, not locked) |
| 0x8000C490 | `dll_get_priority` | **`osGetThreadPri`** | `NULL → __osRunningThread`, returns offset-4 `priority` — exact ultralib `osGetThreadPri` | **clean** (no collision, not locked) |
| 0x8000C090 | `dll_init` | `__osTimerServicesInit` | inits `__osTimerList` circular list + VI accum globals | timer knot |
| 0x8000C11C | `dll_update` | `__osTimerInterrupt` | walks `__osTimerList`, `__osSetCompare`, `osGetCount`, `osJamMesg` | timer knot |
| 0x8000C294 | `dll_reschedule` | `__osSetTimerIntr` | reschedules the compare interrupt | timer knot |
| 0x8000C308 | `dll_insert` | `__osInsertTimer` | inserts into the sorted delta queue | timer knot |
| 0x80006ED0 | `osSetTimerIntr` | a VI-event setter (writes `__osViContext` +0x10/+0x14/+0x2) | **LOCKED** under wrong name | timer knot |
| 0x80009E18 | `__osTimerInterrupt` | a PFS/controller fn (`__osPfsSelectBank`, `__osContRamRead`) | — | timer knot |
| 0x8000E390 | `__osInsertTimer` | a PI-manager getter (reads `__osPiMgrState`) | **LOCKED** under wrong name | timer knot |

The real timer queue is the `dll_*` cluster in `asm/us/CC50.s`; the canonical
`__osTimer*` names were scattered onto three unrelated addresses, two of which
are locked regression anchors (`osSetTimerIntr`, `__osInsertTimer`).

## What has been done (safe, no build/link/lock risk)

`tools/conveyor/pipeline/corpus.py` `CANONICAL_ALIASES` maps the two **clean**
canonical names to their real target so corpus pairs them correctly:
`__osDequeueThread → dll_remove`, `osGetThreadPri → dll_get_priority`. Unit
tests in `tests/conveyor/unit/test_corpus.py`
(`test_submit_pairs_canonical_alias_to_misattributed_target`,
`test_alias_dedup_keys_on_target_id_not_candidate_name`).

## What is deferred (gated on watchman / SHA-1 rebuild)

A proper linker-symbol rename is **not** done here because:

1. `dll_remove` / `dll_get_priority` are called via `jal` from other asm files
   (`C990.s`, `8E10.s`, `7630.s`). Renaming means editing every `glabel` **and**
   every cross-file `jal` site — a change on the ROM link path that must be
   re-gated through the full matching build (SHA-1 must stay
   `3f99351d7bb61656614bdb2aa1a90cfe55d1922c`).
2. The timer knot's canonical names collide with two **locked** functions
   (`osSetTimerIntr` @ 0x80006ED0, `__osInsertTimer` @ 0x8000E390). Renaming
   those requires updating each lock's `target_id` + `src/rom_auto/*.c` filename
   and **re-verifying score 0 on the pool** — which needs watchman.
3. Watchman was **down** at audit time, so neither the SHA-1 gate nor lock
   re-verify could run. Blind link-path renames are not committed.

### Rename plan (run when watchman is up)

For each rename: edit `symbol_addrs.us.txt` (definition) + the `glabel` and
`nonmatching` marker in its `.s` + **every** `jal <old>` across `asm/us/*.s`,
then `make` and confirm SHA-1. For the two locked timer-knot functions, first
give 0x80006ED0 / 0x8000E390 honest names (their real VI/PI identities), update
their locks + `src/rom_auto` filenames, `lock add ... --wait` to re-verify, then
free the `__osSetTimerIntr` / `__osInsertTimer` names for the `dll_*` cluster.
Once symbols are correct, `CANONICAL_ALIASES` can be emptied.

## Libultra inventory rename (lane w22g, 2026-10-11)

Lane w21r's recompiler audit (`tools/recomp/identity.json`) found 43 static
functions whose inventory name belonged to another libultra function. Lane w22g
re-checked every row against `reference/repos/ultralib` (decompals e24c836,
`VERSION_J` branches: the Motor/PFS bodies are the >= J ones), accepted all 43
and renamed them in the inventory itself. Table with per-row evidence:
`cloud/work/frontier/w22g/renames.json`; method and results:
`cloud/work/frontier/w22g/RESULTS.md`.

| VRAM | Old inventory name | VERSION_J name | ultralib source |
|------|--------------------|----------------|-----------------|
| 0x80006A30 | `osSetEventMesg` | `osCreateViManager` | `src/io/vimgr.c` |
| 0x80006D40 | `osSetThreadPri` | `osViSetMode` | `src/io/visetmode.c` |
| 0x80006DA0 | `osSetIntMask` | `osViBlack` | `src/io/viblack.c` |
| 0x80006E10 | `osSetEventMesgAlt` | `osSetEventMesg` | `src/os/seteventmesg.c` |
| 0x80006ED0 | `osSetTimerIntr` | `osViSetEvent` | `src/io/visetevent.c` |
| 0x800071D0 | `osSetGlobalIntMask` | `osSetIntMask` | `src/os/setintmask.s` |
| 0x80007440 | `osSendMesg` | `osJamMesg` | `src/os/jammesg.c` |
| 0x80007590 | `osViSetMode` | `osViSwapBuffer` | `src/io/viswapbuf.c` |
| 0x800075E0 | `osJamMesg` | `osSendMesg` | `src/os/sendmesg.c` |
| 0x800077D0 | `osViGetFramebuffer` | `osViGetNextFramebuffer` | `src/io/vigetnextframebuf.c` |
| 0x80007810 | `osInvalICache` | `osWritebackDCacheAll` | `src/os/writebackdcacheall.s` |
| 0x8000794C | `osViModeNtscLan1` | `osSpTaskLoad` | `src/io/sptask.c` |
| 0x80007AB4 | `osViModeNtscLpn1` | `osSpTaskStartGo` | `src/io/sptask.c` |
| 0x80007B80 | `osDpWait` | `osSpTaskYield` | `src/io/sptaskyield.c` |
| 0x800081D0 | `osPiRawReadWord` | `osPiReadIo` | `src/io/piread.c` |
| 0x80008380 | `osCreateViManager` | `osSetThreadPri` | `src/os/setthreadpri.c` |
| 0x80008460 | `osInvalICache_full` | `osInvalICache` | `src/os/invalicache.s` |
| 0x80008630 | `__osPiRawStartDma` | `osPiStartDma` | `src/io/pidma.c` |
| 0x8000870C | `osWritebackDCacheAll` | `rmonPrintf` | `src/libc/syncprintf.c` |
| 0x80009450 | `__osContBuildPacket` | `osContInit` | `src/io/controller.c` |
| 0x800097D0 | `osContStartReadData2` | `osContStartReadData` | `src/io/contreaddata.c` |
| 0x80009F20 | `osMotorInit` | `__osMotorAccess` | `src/io/motor.c` |
| 0x8000A088 | `__osMotorAccess` | `__osMakeMotorData` | `src/io/motor.c` |
| 0x8000A194 | `osMotorStart` | `osMotorInit` | `src/io/motor.c` |
| 0x8000A2F0 | `osMotorStop` | `osPfsRepairId` | `src/io/pfsrepairid.c` |
| 0x8000A350 | `osPfsAllocate` | `osPfsFindFile` | `src/io/pfssearchfile.c` |
| 0x8000A520 | `osPfsDeleteFile` | `osPfsFileState` | `src/io/pfsfilestate.c` |
| 0x8000A700 | `osPfsRename` | `osPfsDeleteFile` | `src/io/pfsdeletefile.c` |
| 0x8000A8D8 | `osPfsFindFile` | `__osPfsReleasePages` | `src/io/pfsdeletefile.c` |
| 0x8000A970 | `osPfsReadWriteFile` | `osPfsAllocateFile` | `src/io/pfsallocatefile.c` |
| 0x8000ADF0 | `osPfsGetFileStat` | `__osPfsGetNextPage` | `src/io/pfsreadwritefile.c` |
| 0x8000AEC4 | `osPfsGetFileSize` | `osPfsReadWriteFile` | `src/io/pfsreadwritefile.c` |
| 0x8000BD90 | `osPfsChecker_full` | `osStopThread` | `src/os/stopthread.c` |
| 0x8000BE50 | `osPfsReAllocate` | `osGetThreadId` | `src/os/getthreadid.c` |
| 0x8000C490 | `dll_get_priority` | `osGetThreadPri` | `src/os/getthreadpri.c` |
| 0x8000D7A0 | `osPiReadIo` | `__osSiRawReadIo` | `src/io/sirawread.c` |
| 0x8000D7F0 | `osPiWriteWord` | `__osSiRawWriteIo` | `src/io/sirawwrite.c` |
| 0x8000DC10 | `osPiGetAccess` | `__osPiGetAccess` | `src/io/piacs.c` |
| 0x8000DC54 | `osPiReleaseAccess` | `__osPiRelAccess` | `src/io/piacs.c` |
| 0x8000DC80 | `osPiReadWord` | `__osPiRawReadIo` | `src/io/pirawread.c` |
| 0x8000DCD0 | `osPiStartDma` | `__osPiRawStartDma` | `src/io/pirawdma.c` |
| 0x8000DDA0 | `osPiSetDeviceTiming` | `__osEPiRawStartDma` | `src/io/epirawdma.c` |
| 0x8000E620 | `osContStartReadData` | `__osPfsGetStatus` | `src/io/pfsgetstatus.c` |

Notes:
- `rmonPrintf` (0x8000870C) is identified by position: its body (an empty
  `_FINALROM` varargs function) is identical to its neighbour's; segment
  `lib_92f0` holds exactly the two functions of `syncprintf.o`, osSyncPrintf
  then rmonPrintf. 0x800086F0 (`osWritebackDCache_full`) is therefore
  `osSyncPrintf`; not renamed here (not in the w21r table).
- VERSION_J renamed the PI raw functions (`include/PRinternal/piint.h`): the
  inventory uses `__osPiRawReadIo`, `__osPiRawStartDma`, `__osEPiRawStartDma`;
  `identity.json` keeps the pre-J names N64Recomp knows. The historical alias
  `osEPiRawStartDma = __osEPiRawWriteIo` in `rom_symbol_aliases.ld` (0x8000FE00,
  S12 census) is unrelated and unchanged.
- The names form cycles (`osSendMesg`<->`osJamMesg`, `__osPiRawStartDma`<->
  `osPiStartDma`, `osSetEventMesg`->`osCreateViManager`->`osSetThreadPri`->
  `osViSetMode`->`osViSwapBuffer`, ...), so they were applied as one
  permutation: `python3 -m tools.conveyor.pipeline.static_rename` (symbol_addrs,
  C code, locks with a pure-rename proof per body, rom_owned_data.json pins) and
  `python3 -m tools.conveyor.pipeline.targets_rename rename` (Conveyor rows in
  place, history kept).

### Game side: legacy names

The game image and the runtime images still call these functions by their old
names (src/blob C and preludes, `asm/us/blob/symbols.json`,
`asm/us/ovl_*/symbols.json`). Because the names were permuted, an alias cannot
express both meanings, so `game_static_legacy.us.txt` maps each old name to its
address for the game link only (`pipeline.blob_tu.static_symbols`); the static
ROM link never reads it. Generated game files are byte-identical. To finish the
rename on the game side: rename the callers in src/blob (and its preludes),
regenerate blob.ld/symbols.json, re-prove the image, then drop the entries.

### Other misnamed static functions seen during the check (leads, not verified)

Call order in the 43 bodies matched ultralib only after mapping these callees,
which keep their current names: `dll_remove` = `__osDequeueThread` (verified
2026-07-14, above), `__osCleanupThread` = `__osEnqueueAndYield`, `osPiInit` =
`__osPiCreateAccessQueue`, `__osInsertTimer` (0x8000E390) = `osPiGetCmdQueue`,
`osViModeTableGet` = `_VirtualToPhysicalTask`, `__osSpDma` = `__osSpRawStartDma`,
`__osContRamReset` = `__osPackRequestData`, `__osContGetStatus` =
`__osContGetInitData`, `osSiInit` = `__osSiCreateAccessQueue`,
`__osContBuildRequest` = `__osPfsRequestOneChannel`, `__osContParseResponse` =
`__osPfsGetOneChannelData`, `osPfsReadWriteFile_pages` = `__osCheckId`, and
`osWritebackDCache_full` = `osSyncPrintf`. Each needs its own body check before
a rename.
