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

### Game side: legacy names (retired by lane w23g)

Until lane w23g the game image still called these functions by their old
names, so `game_static_legacy.us.txt` gave the game link the old names at their
addresses. w23g renamed every game caller (`src/blob/**` including the groups'
context copies, and the 86 `cloud/matches` submissions) with the same
permutation (`static_rename source`), dropped the prelude prototypes the
permutation turned into conflicting redeclarations
(`cloud/work/frontier/w23g/drop_renamed_dups.py`), re-pinned the blob lock with
a pure-rename proof per entry and re-proved the image
(`cloud/work/frontier/w23g/rename_resplice.py`), then emptied the table. The
runtime images keep resolving against their frozen `asm/us/ovl_*/symbols.json`
(old spellings; not regenerated).

## ll.c / llcvt.c helpers: libgcc names -> ultralib names (lane w23g, 2026-10-11)

The boot segment's 64-bit helpers were labelled with libgcc names. They are
ultralib `src/libc/ll.c` and `llcvt.c` (no version branches), compiled
`-g0 -O1 -mips3 -32` (w16g). Evidence per row: the locked definition in
`src/rom/lib_e4f0.c` / `lib_a5f0.c` (score 0) is the ultralib definition token
for token after the rename, and the ROM order of each TU equals the definition
order of its ultralib file (`cloud/work/frontier/w23g/ll_check.py`, table
`cloud/work/frontier/w23g/ll_renames.json`). IDO lowers u64 arithmetic in C to
calls of exactly these names (w22d: cpak_read; w2i: object_render's `__ll_lshift`).

| vram | old (libgcc label) | ultralib name | file |
|---|---|---|---|
| 0x8000D8F0 | `__lshrdi3` | `__ull_rshift` | ll.c |
| 0x8000D91C | `__umoddi3` | `__ull_rem` | ll.c |
| 0x8000D958 | `__udivdi3` | `__ull_div` | ll.c |
| 0x8000D994 | `__ashldi3` | `__ll_lshift` | ll.c |
| 0x8000D9C0 | `__umoddi3_alt` | `__ll_rem` | ll.c |
| 0x8000D9FC | `__divdi3` | `__ll_div` | ll.c |
| 0x8000DA58 | `__muldi3` | `__ll_mul` | ll.c |
| 0x8000DA88 | `__qdivrem` | `__ull_divremi` | ll.c |
| 0x8000DAE8 | `__moddi3` | `__ll_mod` | ll.c |
| 0x8000DB84 | `__ashrdi3` | `__ll_rshift` | ll.c |
| 0x800099F0 | `__fixdfdi` | `__d_to_ll` | llcvt.c |
| 0x80009A0C | `__fixsfdi` | `__f_to_ll` | llcvt.c |
| 0x80009A28 | `__fixunsdfdi` | `__d_to_ull` | llcvt.c |
| 0x80009AC8 | `__fixunssfdi` | `__f_to_ull` | llcvt.c |
| 0x80009B64 | `__floatdidf` | `__ll_to_d` | llcvt.c |
| 0x80009B7C | `__floatdisf` | `__ll_to_f` | llcvt.c |
| 0x80009B94 | `__floatundidf` | `__ull_to_d` | llcvt.c |
| 0x80009BC8 | `__floatundisf` | `__ull_to_f` | llcvt.c |

Not a permutation (no old name is a new name), so the game side needed no
legacy entries: the game link and `asm/us/blob/symbols.json` provide the new
names, and game C now writes u64 arithmetic naturally (`src/blob/cpak_read.c`).
`disasm.SYMBOL_ALIASES` no longer needs its `__ll_lshift` alias. The owned
.rodata slot of `__fixunsdfdi` is now `__d_to_ull` (co-owner `__f_to_ull`).
`tools/recomp/identity.json` maps the 18 back to their libgcc names for the
recompiler only, so recompiled output is unchanged (N64Recomp lists the ultralib
names as ignored/reimplemented: dropping those entries is a recomp decision).
`asm/us/ovl_{a,b}/symbols.json` (frozen target snapshots) still spell the
libgcc names; nothing in the runtime images calls them.

## Leads verified and renamed (lane w23g, 2026-10-11)

Table `cloud/work/frontier/w23g/suspect_renames.json` (evidence per row), applied
to the static inventory and the game callers at once:

| vram | old | ultralib name | file | confidence |
|---|---|---|---|---|
| 0x800086F0 | `osWritebackDCache_full` | `osSyncPrintf` | libc/syncprintf.c | medium (FINALROM empty varargs; segment position, as rmonPrintf) |
| 0x8000C050 | `dll_remove` | `__osDequeueThread` | os/thread.c | high |
| 0x8000C308 | `dll_insert` | `__osInsertTimer` | os/timerintr.c | high |
| 0x8000CFC4 | `__osCleanupThread` | `__osEnqueueAndYield` | os/exceptasm.s | high |
| 0x8000DBC0 | `osPiInit` | `__osPiCreateAccessQueue` | io/piacs.c | high |
| 0x8000E390 | `__osInsertTimer` | `osPiGetCmdQueue` | io/pigetcmdq.c | high |

`asm/us/boot_tail/symbols.json` also lost a stale `__osEnqueueAndYield =
0x8000FBA0` key (the census name of osDestroyThread). `corpus.CANONICAL_ALIASES`
is empty now. Still open below: the remaining leads. (`dll_init`, `dll_update`,
`dll_reschedule`: done by lane w24t, next section.)

## Timer services renamed (lane w24t, 2026-10-12)

Proof by compilation, not by reading: ultralib (decompals) `src/os/timerintr.c` and
`src/io/pfsinitpak.c`, VERSION_J, `-D_FINALROM`, compiled with the project's IDO 5.3 at the
locked flagsets of their ROM TUs (`-g0 -O1` for lib_cc50, `-g0 -O2` for lib_a810, `-mips2 -G 0
-non_shared -Wab,-r4300_mul`). Each object's whole `.text` equals the ROM words, relocations
compared by value (`cloud/work/frontier/w24t/rename/{prove.sh,compare.py,prove.log}`):
timerintr.o 256 words at 0x8000C090, pfsinitpak.o 196 words at 0x80009C10, 0 differing.
Table `cloud/work/frontier/w24t/rename/renames.json` (a permutation, static side only: no game
source calls these names):

| vram | old | ultralib name | file |
|---|---|---|---|
| 0x80009E18 | `__osTimerInterrupt` | `__osPfsCheckRamArea` (static) | io/pfsinitpak.c |
| 0x8000C090 | `dll_init` | `__osTimerServicesInit` | os/timerintr.c |
| 0x8000C11C | `dll_update` | `__osTimerInterrupt` | os/timerintr.c |
| 0x8000C294 | `dll_reschedule` | `__osSetTimerIntr` | os/timerintr.c |

The same compile also names, by address (renamed by lane w25a, next section): the timerintr.c globals
`__osCurrentTime` = `gViTimeAccumHi` (0x80037C50), `__osBaseCounter` = `gViLastCount`,
`__osViIntrCount` = `gViRetraceCount`; and pfsinitpak.c's callees `__osCheckPackId` =
`__osCheckId` (0x8000EFC0), `__osRepairPackId` = `__osRepairId` (0x8000EC70).
`tools/recomp/identity.json` maps the four back to their pre-w24t names for the recompiler.

### Other misnamed static functions seen during the check (leads, not verified)

Call order in the 43 bodies matched ultralib only after mapping these callees,
which keep their current names (the w23g renames above are done):
`osViModeTableGet` = `_VirtualToPhysicalTask`, `__osSpDma` = `__osSpRawStartDma`,
`__osContRamReset` = `__osPackRequestData`, `__osContGetStatus` =
`__osContGetInitData`, `osSiInit` = `__osSiCreateAccessQueue`,
`__osContBuildRequest` = `__osPfsRequestOneChannel`, `__osContParseResponse` =
`__osPfsGetOneChannelData`, and `osPfsReadWriteFile_pages` = `__osCheckId` (proven and renamed
by lane w25a, next section). All seven others were proven and renamed by lane w27s (last section).

## contpfs.c and timerintr.c data renamed (lane w25a, 2026-10-12)

Same method (`cloud/work/frontier/w25a/rename/{prove.sh,prove.log}`, w24t's `compare.py`):
ultralib VERSION_J `-D_FINALROM` with the project IDO 5.3. `io/contpfs.c` at lib_f700's `-g0 -O2`:
`RESULT: IDENTICAL (736 words, 52 relocated fields compared by value, 0 differing words)` at
0x8000EB00 (`__osSumcalc`, `__osIdCheckSum`, `__osRepairPackId`, `__osCheckPackId`, `__osGetId`,
`__osCheckId`, `__osPfsRWInode` in ultralib order); `os/timerintr.c` at `-g0 -O1`: IDENTICAL
(256 words), its relocations naming the globals; `io/pfsinitpak.c` IDENTICAL (196 words), its jal
targets naming the two pack-ID callees. Independent confirmation: the legacy `src/libultra/os_pfs_*.c`
already declare `__osCheckId(OSPfs *)` at 0x8000F2D0. Table `cloud/work/frontier/w25a/rename/renames.json`
(static side only; no game source uses these names):

| vram | old | ultralib name | file |
|---|---|---|---|
| 0x8000EC70 | `__osRepairId` | `__osRepairPackId` | io/contpfs.c |
| 0x8000EFC0 | `__osCheckId` | `__osCheckPackId` | io/contpfs.c |
| 0x8000F2D0 | `osPfsReadWriteFile_pages` | `__osCheckId` | io/contpfs.c |
| 0x80037C50 | `gViTimeAccumHi` | `__osCurrentTime` (OSTime, 8 B) | os/timerintr.c |
| 0x80037C58 | `gViLastCount` | `__osBaseCounter` | os/timerintr.c |
| 0x80037C5C | `gViRetraceCount` | `__osViIntrCount` | os/timerintr.c |

`gViTimeAccumLo` (0x80037C54) was kept by w25a as the asm label of `__osCurrentTime`'s low word;
**w26a retired it**: four reloc_addrs.us.txt entries (osGetTime 0x7FDC/0x7FE0, __osTimerServicesInit
0xCC90/0xCC9C) name the sites `%hi/%lo(__osCurrentTime + 0x4)` as ultralib's 64-bit accesses do, the
symbol_addrs line is gone (`make extract`: undefined_syms_auto lists splat's anonymous `D_80037C54`),
the two leftover nonmatchings files were rewritten to the same operands, and the ROM is unchanged
(both functions are C). The game/overlay symbol maps (asm/us/{blob,ovl_a,ovl_b,boot_tail}/symbols.json,
src/blob/blob.ld PROVIDE) still list the name but nothing references it; `targets._typed_data_field_aliases`
still folds it for pre-w26a asm text. Applied with `static_rename
symbols|source|lock|owned` (src/rom TUs and rom_tu.h only; the legacy src/libultra and src/os sources
are not built and keep their spellings), nonmatchings git-mv'd and permuted, `make extract`
(undefined_syms_auto), boot_tail/blob `symbols.json` + SHA256SUMS and `src/blob/blob.ld` permuted in
place, timer_services profile revision, Conveyor DB `targets_rename rename` (functions only).
`tools/recomp/identity.json` maps the three functions back to their pre-w25a names.

## sptask.c, sprawdma.c, controller.c, siacs.c, pfsgetstatus.c functions renamed (lane w27s, 2026-10-12)

Same method (`cloud/work/frontier/w27s/rename/{prove.sh,prove.log}`, w24t's `compare.py`): ultralib
(decompals e24c836) VERSION_J `-D_FINALROM` compiled with the project IDO 5.3 at the holding TUs'
flagset `-g0 -O2 -mips2 -G 0 -non_shared` (lib_8440, lib_e280, lib_a050, lib_f160; no opt override).
Each object's whole `.text` equals the ROM, relocations compared by value:
`io/sptask.c` IDENTICAL (176 words, 54 relocated fields) at 0x80007840, `io/sprawdma.c` (36 words) at
0x8000D680, `io/controller.c` (184 words) at 0x80009450, `io/siacs.c` (48 words) at 0x8000E560,
`io/pfsgetstatus.c` (140 words) at 0x8000E620. Table `cloud/work/frontier/w27s/rename/renames.json`
(static side only; no game source calls these names; the blob/boot_tail `symbols.json` maps and
`src/blob/blob.ld` PROVIDEs were permuted in place):

| vram | old | ultralib name | file |
|---|---|---|---|
| 0x80007840 | `osViModeTableGet` | `_VirtualToPhysicalTask` (static) | io/sptask.c |
| 0x8000D680 | `__osSpDma` | `__osSpRawStartDma` | io/sprawdma.c |
| 0x800095AC | `__osContGetStatus` | `__osContGetInitData` | io/controller.c |
| 0x80009658 | `__osContRamReset` | `__osPackRequestData` | io/controller.c |
| 0x8000E560 | `osSiInit` | `__osSiCreateAccessQueue` | io/siacs.c |
| 0x8000E6F0 | `__osContBuildRequest` | `__osPfsRequestOneChannel` | io/pfsgetstatus.c |
| 0x8000E7B4 | `__osContParseResponse` | `__osPfsGetOneChannelData` | io/pfsgetstatus.c |

Applied with `static_rename symbols|source|lock|owned` (src/rom TUs and rom_tu.h; the unbuilt legacy
src/libultra keeps its spellings), PROMOTED headers retitled, nonmatchings git-mv'd and permuted,
Conveyor DB `targets_rename rename` (functions), `targets._typed_data_field_aliases` accepts the new
name for the task-field family, `tools/recomp/identity.json` maps the seven back to their pre-w27s
names for the recompiler.

## exceptasm, TLB and cause accessors renamed (lane ulr, 2026-10-21)

Proof by assembly: the wave-28 ceiling proof (`cloud/work/frontier/w28s/ceiling/{prove.sh,prove.log}`) built
ultralib (decompals `e24c836`) VERSION_J `exceptasm.s`, `unmaptlball.s`, `maptlbrdb.s` and `getcause.s` with the
project's IDO 5.3 and found each object byte-identical to the ROM (`docs/static-asm-ceiling.md` §1). Table
`cloud/work/frontier/ulr/renames.json` (a permutation; static side only, no game source calls these names):

| vram | old | ultralib name | file |
|---|---|---|---|
| 0x8000C980 | `__osException` | `__osExceptionPreamble` (the 16 B vector stub) | os/exceptasm.s |
| 0x8000C990 | `__osExceptionPreamble` | `__osException` | os/exceptasm.s |
| 0x8000CEDC | `__osSendInterrupt` | `send_mesg` (the slot also holds `handle_CpU`) | os/exceptasm.s |
| 0x8000D298 | `__osExceptionPanic` | `__osCleanupThread` | os/exceptasm.s |
| 0x8000D840 | `__osTlbFlush` | `osUnmapTLBAll` | os/unmaptlball.s |
| 0x8000D890 | `__osTlbInit` | `osMapTLBRdb` | os/maptlbrdb.s |
| 0x8000DBB0 | `osCauseGet` | `__osGetCause` | os/getcause.s |

Applied with the same tools as w22g: `static_rename symbols|source|lock|owned` (symbol_addrs; src/rom
lib_7b30/lib_8a80/lib_d580/lib_e4f0 and rom_tu.h; 2 lock bodies re-pinned with pure-rename proofs; both storage
owners' context proofs and sdk_initialize's full module rebound into `cloud/work/frontier/ulr/rebind/`, with a
reviewed `renamed` revision in `owned_existing_storage.PROFILES`), `make extract` (splat fed a sanitized copy of
symbol_addrs for the split only) regenerating the permuted nonmatchings and `asm/us/E440.s`/`E490.s`,
`asm/us/boot_tail/symbols.json` keys renamed in place, `asm_matched.lock.json` re-keyed (words unchanged).
The game and runtime-image links keep the old names through 7 rows in `game_static_legacy.us.txt`, so
`src/blob/blob.ld` and `asm/us/blob/symbols.json` regenerate byte-identically. `tools/recomp/identity.json` maps
the seven back to their old names for the recompiler (patches.json and the runtime shims key on them). Conveyor
DB: `targets_rename rename` (two rows, 0x8000C990 and 0x8000CEDC, already failed the object round-trip gate;
`rename` now carries such a recorded failure under the new name instead of refusing).
