# w16g - static (boot/library) promotion prep from w15g

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w16g`. Nothing locked, promoted,
converted, spliced or committed; no edits to src/, asm/, tools/, include/, locks, symbol_addrs or the conveyor DB.
Nothing in `cloud/matches/` (score.py fn cannot score static targets, w15g "Scorer caveat").

## Result

34 sources in `sources/`, one per static slot, each named by the slot's CURRENT layout/symbol_addrs label (promote.py
splices by slot name). First line `/* flags: ... */`; header names the ultralib file and version branch (decompals
e24c836, `BUILD_VERSION >= VERSION_J` where the file has a split) and every project-vs-ultralib name mapping.

| group | functions | bytes (layout) | dry-run |
|---|---|---:|---|
| ultralib + Rush, C as-is | 15 | 3,332 | LOCK and TU true=0 strict=0 rb=0, `objdump -dr` (words + reloc type/symbol) identical |
| libgcc = ultralib `src/libc/ll.c` + `llcvt.c` | 16 | 916 | same, at `-g0 -O1 -mips3 -32 -G 0 -non_shared` |
| needs owned data | 3 | 460 | see "Data-bound" |
| **total** | 34 | 4,708 | 31 clean (4,248 B) + 3 data-bound |

Static coverage would move 400/668, 73,596 B to 431/668, 77,844 B (to 434/668, 78,304 B with the data-bound three).
Static bytes only, never merged with game coverage.

### Per function (LOCK line = exactly what `lock add` scores; full output in `dryrun.log`)

| slot (segment) | B | flags | inventory target `lock add --target` must name | LOCK / TU |
|---|---:|---|---|---|
| __osPfsRWInode (0xf700, converted) | 732 | O2 | __osPfsRWInode | true=0 strict=0 rb=0 identical (204 rows) / same |
| __osContRamWrite (0x10280) | 592 | O2 | __osContRamWrite | 0/0/0 identical (163) / same |
| __osEPiRawWriteIo (0x10a00) | 352 | O2 | **osEPiRawStartDma** (stale) | 0/0/0 identical (88) / same |
| __osEPiRawReadIo (0x10b60) | 352 | O2 | **osEPiRawReadIo** (stale) | 0/0/0 identical (89) / same |
| __osContAddressCrc (0x105f0) | 208 | O2 | __osContAddressCrc | 0/0/0 identical (52) / same |
| __osContDataCrc (0x105f0, real @0x8000FAC0) | 160 | O2 | **__osPfsDataChecksum** (stale) | 0/0/0 identical (37) / same |
| osMotorStop = osPfsRepairId (0xab20, conv.) | 96 | O2 | osMotorStop | 0/0/0 identical (22) / same |
| __osSiDeviceBusy (0x10970) | 48 | O2 | **__osPiDeviceBusy** (stale) | 0/0/0 identical (8) / same |
| func_800103A0 = __osPfsGetInitData (0x10d60) | 176 | O2 | func_800103A0 | 0/0/0 identical (47) / same |
| func_80014650 (0x11640, converted) | 44 | O2 | func_80014650 | 0/0/0 identical (13) / same |
| func_8002517C (0x25bb0, converted) | 44 | O2 | func_8002517C | 0/0/0 identical (13) / same |
| __osResetGlobalIntMask (0x109a0) | 96 | O1 | **osEPiRawWriteIo** (stale) | 0/0/0 identical (27) / same |
| __osSetGlobalIntMask (0x10cc0) | 80 | O1 | **__osPiGetCmdQueue** (stale) | 0/0/0 identical (24) / same |
| osYieldThread (0x10d10) | 80 | O1 | osYieldThread | 0/0/0 identical (25) / same |
| osDestroyThread (0x107a0) | 272 | O1 | **__osEnqueueAndYield** (stale) | 0/0/0 identical (79) / same |
| __lshrdi3 __umoddi3 __udivdi3 __ashldi3 __umoddi3_alt __divdi3 __muldi3 __qdivrem __moddi3 __ashrdi3 (0xe4f0) | 704 | O1 -mips3 -32 | same names | all 0/0/0 identical / same |
| __fixdfdi __fixsfdi __floatdidf __floatdisf __floatundidf __floatundisf (0xa5f0) | 212 | O1 -mips3 -32 | same names | all 0/0/0 identical / same |
| osAiSetNextBuffer (0xca70, converted) | 144 | O2 | osAiSetNextBuffer | true=0 strict=0 rb=0, objdump DIFFERS 6 rows: relocs `.data` vs `__osAiNeedsAlign` |
| __fixunsdfdi / __fixunssfdi (0xa5f0) | 160+156 | O1 -mips3 -32 | same | true=10 strict=10 rb=0: the `.rodata` 2^63 literal |

O1 = `-g0 -O1 -mips2 -G 0 -non_shared`, O2 = `-g0 -O2 -mips2 -G 0 -non_shared`. Note the default `scoring.score`
does not tell `.data` from `__osAiNeedsAlign` (true=0 above); check.py's objdump compare does, which is why
osAiSetNextBuffer is listed as data-bound and not as a match.

Corrections to w15g:
- libgcc is not "trivial C": the 18 functions are ultralib `src/libc/ll.c` (10, ROM order identical:
  `__ull_rshift __ull_rem __ull_div __ll_lshift __ll_rem __ll_div __ll_mul __ull_divremi __ll_mod __ll_rshift`)
  and `llcvt.c` (8, `__d_to_ll __f_to_ll __d_to_ull __f_to_ull __ll_to_d __ll_to_f __ull_to_d __ull_to_f`), verbatim,
  at ultralib's own `ido.mk` flags (`-O1`, `-mips3 -32`). No s64 shift-count or u16/s16 guesswork is needed.
- func_80014650 / func_8002517C call `osJamMesg` (0x800075E0) on the Rush-owned queues D_80038368 / D_800586A8,
  i.e. the release half of the access-queue pairs already promoted in lib_11640.c / lib_25bb0.c. The SDK identity
  (`__osPi/SiRelAccess`, which call osSendMesg) is unproven; the C is exact anyway.
- osAiSetNextBuffer: w15g's "true=0" is the scorer ignoring the reloc symbol; it needs owned data.

## How it was checked (what "dry-run" means here)

`promote.py` has no dry-run or check mode (`run` and `batch` only; `run` splices, builds, commits). So:

1. `check.py` (builder, `run_all.sh` -> `dryrun.log`) replays both compile steps without writing anything:
   - LOCK: `pipeline.lock add`'s `verify_entry`: `reduced_tu(SRC, FN)` compiled `cc -c <flags> -I include
     -I include/PR -I <toolkit>/shim` (jobs/compile_score.compile_one) with the pinned toolkit d4c39cbc, scored with
     `jobs/scoring.score` against the conveyor inventory object `BlobStore[n64_target.target_o_sha]`.
   - TU: what the splice produces: the real `src/rom/<tu>.c` text before the slot (GLOBAL_ASM lines dropped,
     earlier promoted bodies reduced to their prototypes) or bare `#include "rom_tu.h"` for an unconverted segment,
     plus the context row preamble, plus the body; compiled with the Makefile ROM-TU CFLAGS for that TU (incl.
     `-Wab,-r4300_mul -Xcpluscomm -D_LANGUAGE_C -I src/rom`, opt/ISA as the TU would need).
   Each reports `scoring.score`, strict (`stack_differences=True`), reloc-blind, and an exact `objdump -dr` compare.
2. `ctx/*.jsonl`: `cloud/work/boot_tail_promotion/context_check.py` (unchanged; `ctx.py` only swaps its hardcoded
   `-O2 -mips2` for `-O1` / `-mips3 -32`) = the promote_batch.py context rows. All ok except the 3 data-bound
   ("object has data/rodata"). func_80014650's row has its `extern OSMesgQueue D_80038368;` line removed (the
   TU declares it as `MessageQueue` before the slot; TU check passes without it).
3. `precheck.py` (Pi, read-only; `precheck.log`) replays promote.py's preconditions in order with the lock entries
   `lock add` would write injected in memory. Output summary:
   - PRECONDITIONS OK today: __osPfsRWInode (0xf700), osMotorStop (0xab20), func_80014650, func_8002517C,
     osAiSetNextBuffer (passes preconditions, would fail the ROM gate without owned data).
   - `REFUSE: <seg> is not converted - run pipeline.layout convert <seg> first` for every other segment; after
     conversion the derived flagset is `None` (=> `refusing: segment ... has no pinned flagset`) for 0x10a00, 0x10b60,
     0x109a0, 0x10cc0, 0x107a0, 0x10970: the lock's target_id is the stale inventory name, and
     `layout.load_flagsets` keys flagsets by target_id, so it never lands on the segment's (renamed) slot.

## Integration notes (for whoever promotes)

Order by readiness; each step uses existing gates.
1. Now (converted, names agree): `lock add` then `promote_batch.py ctx/ctx_O2.jsonl 0xf700 0xab20 0x11640 0x25bb0
   --only __osPfsRWInode,osMotorStop,func_80014650,func_8002517C` (916 B). Use promote_batch, not `promote run`:
   `run` splices the body only, and rom_tu.h lacks `__osContPifInode`, `__osSiLastChannel`,
   `PFS_INODE_SIZE_PER_PAGE` (__osPfsRWInode). The other three also pass the TU check body-only (no preamble), so `promote run` works for them.
   Rows are in `ctx/` (redundant identical `#define`s in them are harmless; TU check passes with them).
2. After `layout convert` (names agree): 0x10280 __osContRamWrite, 0x105f0 __osContAddressCrc (+ __osContDataCrc,
   see 3), 0x10d10 osYieldThread (-O1; `write_opt_overrides` emits the -O1 rule), 0x10d60 func_800103A0
   (osPfsIsPlug/__osPfsRequestData stay passthrough) = 1,056 B (+160).
3. After re-registering inventory targets under current names (see labels): 0x10a00, 0x10b60, 0x109a0, 0x10cc0,
   0x107a0, 0x10970 = 1,200 B, and __osContDataCrc cleanly (today `lock add --target __osPfsDataChecksum` would score
   0, but promote migrates the lock to `target_id: __osContDataCrc`, which is the 0x8000FA4C fragment row, so the
   next `lock verify` fails).
4. libgcc (916 B) after the -mips3 tooling change below; whole segment 0xe4f0 is proven (next section).
5. Data-bound (460 B) after owned-data registration (below).

### `-mips3 -32`: where the tooling refuses, smallest change (described, not made)

- `lock add --flags "-g0 -O1 -mips3 -32 -G 0 -non_shared"`: accepted (free-form flagset, `shlex.split` in
  compile_one). `layout._segment_flagset` pins it (precheck: 0xe4f0/0xa5f0 derive exactly that flagset).
- `layout.write_opt_overrides` only rewrites `-O`/`-g`: for 0xe4f0 it would emit `$(subst -O2,-O1,$(CFLAGS))`,
  the TU builds at -mips2, and the ROM gate fails (safely). Change: also emit `$(subst -mips2,-mips3 -32,...)`
  when the segment flagset contains `-mips3` (and do not skip such a segment as "default").
- The object then fails to link: IDO `-mips3 -32` writes e_flags 0x20000001 (arch3, no EF_MIPS_32BITMODE) and
  GNU ld refuses it next to -mips2 objects ("linking 32-bit code with 64-bit code"; reproduced, `tu_e4f0.log`).
  Setting EF_MIPS_32BITMODE (0x100) makes `ld -r` succeed (0x20000101). Change: a post-compile e_flags fix for such
  TUs, in the existing ROM-TU rule next to `owned_data object` (OoT's `set_o32abi_bit.py` does the same).
- asm-processor needs nothing: `tu_e4f0.sh` builds 0xe4f0 as a ROM TU (10 C bodies + the osCauseGet GLOBAL_ASM
  passthrough) through `tools/asm-processor/build.py` with the Makefile command line at `-mips3 -32 -O1`:
  `segment 0xe4f0 words 180 object 180 IDENTICAL`. (`cc: Warning: -mips3 should not be used for ucode 32-bit
  compiles` is printed; harmless.)
- No `-D` is needed: every source spells the project's names directly (w15g's `-D__osCurrentHandle=__osPiDevList`
  etc. are gone). The pool path would accept `-D` in a flagset; the Makefile path would not (no per-TU define hook).

### Data-bound

- osAiSetNextBuffer: matches only with ultralib's function-local `static u8 hdwrBugFlag = FALSE;` (extern u8,
  extern volatile u8, tentative and initialised file-scope definitions all CSE the address: 26 objdump lines
  differ). lib_ca70.o then owns a 0x10 `.data` section. ROM bytes at 0x8002C3C0..CF (data.bin offset 0x4BF0) are
  16 zero bytes, followed by ultralib thread.c's `__osThreadTail` at 0x8002C3D0, consistent with aisetnextbuf.o's
  own 16-byte .data. Route: an `rom_owned_data.json` slot (owner_section .data, size 0x10, offset 0x4BF0) like
  `timer_services_pointer`, with `__osAiNeedsAlign` kept as an alias at the slot address.
- __fixunsdfdi / __fixunssfdi: one 16-byte `.rodata` each (the 2^63 bias). Their inventory targets are `raw_word`
  tier (no relocations), so the lock can never reach true 0 until the targets are regenerated reloc-aware and the
  literals get owned `.rodata` slots.

## Labels

Most w15g "wrong labels" are already fixed in `symbol_addrs.us.txt` by the S12 census
(`cloud/work/s12_boot_tail_census`), with `rom_symbol_aliases.ld` keeping the old names as exact aliases for locked
C and asm callers. What is still stale is the **conveyor inventory** (`~/.conveyor/conveyor.db` `n64_target`,
populated from `work/**/info.txt`, which CLAUDE.md calls historical labels).

A. Inventory rows under pre-S12 names (symbol_addrs and layout already correct):

| address | inventory target_id | current name | touch to rename |
|---|---|---|---|
| 0x8000FE00 | osEPiRawStartDma | __osEPiRawWriteIo | n64_target row only (alias exists) |
| 0x8000FF60 | osEPiRawReadIo | __osEPiRawReadIo | same |
| 0x8000FDA0 | osEPiRawWriteIo | __osResetGlobalIntMask | same |
| 0x800100C0 | __osPiGetCmdQueue | __osSetGlobalIntMask | same |
| 0x8000FD70 | __osPiDeviceBusy | __osSiDeviceBusy | same |
| 0x8000FBA0 | __osEnqueueAndYield | osDestroyThread | same (no alias needed: no locked caller) |
| 0x8000FAC0 | __osPfsDataChecksum | __osContDataCrc | same |
| 0x8000FCB0 | __osTLBLookup | __osProbeTLB | same (asm-only today) |

Can a promotion land under the old inventory name? The lock can (`lock add --target <old>` scores 0), but promote
refuses (no pinned flagset, precheck) and the migrated lock would point at a name the inventory lacks. So these
need the rename first. Repair tool: `targets.populate` (the `matrix extract` path) over a temporary inventory
built from `layout.derive()` names/addresses/extents, i.e. `cloud/work/boot_tail_promotion/refresh_split_targets.py`
generalised to register new names instead of refreshing existing rows; then retire the old rows. Locks: none of the
404 static locks has `target_id != function name`, so nothing else moves.

B. Extent problems (inventory only; layout and symbol_addrs are already right):
- `__osContDataCrc` @0x8000FA4C, 29 words, raw_word: a fragment (the unrolled tail of `__osContAddressCrc`,
  0x8000F9F0..0x8000FAC0). From `work/libultra/cont/__osContDataCrc/info.txt`. Retire the row; the real function is
  the 0x8000FAC0 row (today `__osPfsDataChecksum`, see A).
- `read_controllers` @0x800140B0, raw_word, 117,742 words (target .o .text 470,976 B): from
  `work/boot/read_controllers/info.txt`; 0x800140B0 is inside func_80013DEC (S12 extent 0x80013DEC+780), and
  symbol_addrs already comments it out as "mid-function". Retire the row (no function starts there). Same tool.
- Also stale: `n64_target.insn_count` for osYieldThread is 4072 (object is correct, 80 B).

C. Wrong labels still in symbol_addrs (code identity from the matches above):

| address | current label | is | refs (src/rom, src/blob C, asm non-blob) | lands under current label? |
|---|---|---|---|---|
| 0x8000A2F0 | osMotorStop | ultralib osPfsRepairId (J+; in J+ osMotorStop is a macro over __osMotorAccess) | 1, 1, 4 | yes (source is named osMotorStop) |
| 0x8002C3D8 / 0x8002C3DC | __osActiveQueue / __osRunQueue | ultralib __osRunQueue / __osActiveQueue (swapped; thread.c order) | 3/2, 430/0 (shared blob prelude), 9/5 | yes (sources use current names) |
| 0x8000CFC4 | __osCleanupThread | __osEnqueueAndYield (exceptasm.s) | 7, 430, 11 | yes |
| 0x8000C050 | dll_remove | __osDequeueThread (already [MISATTR]) | 3, 430, 7 | yes |
| 0x8000E620 | osContStartReadData | __osPfsGetStatus | rom_tu.h + promoted bodies | yes |
| 0x800071D0 | osSetGlobalIntMask | osSetIntMask (asm, w15g) | 4, 0, 7 | asm only |
| 0x8000C990 | __osExceptionPreamble | __osException (asm, w15g) | 1, 0, 5 | asm only |
| 0x8000D8F0.. | __lshrdi3 .. __ashrdi3 (10) | ll.c `__ull_rshift` .. `__ll_rshift` | asm/blob callers | yes |
| 0x800099F0.. | __fixdfdi .. __floatundisf (8) | llcvt.c `__d_to_ll` .. `__ull_to_f` | asm/blob callers | yes |
| data | __osPfsBuffer, __osPfsRequestType, __osPfsRequestType2, __osContLastChannel, __osSiChannelMask, __osSiLastChannel, __osContPifInode, __osPiDevList, __osAiNeedsAlign | __osPfsPifRam, __osContLastCmd, __osMaxControllers, __osPfsLastChannel, __osPfsInodeCacheBank, __osPfsInodeCacheChannel, __osPfsInodeCache, __osCurrentHandle, aisetnextbuf.c `hdwrBugFlag` | many | yes |
| 0x800103A0 | func_800103A0 | __osPfsGetInitData | - | yes |

Every promotion above can land under the current label without breaking anything: the C uses the label, so the
body hash, relocation names and callers all agree. A later rename should follow S12: change symbol_addrs, add an
alias line in `rom_symbol_aliases.ld` for each old name still used by locked C or asm (no lock churn), re-run
`make extract` for nonmatchings file names (a converted TU's `#pragma GLOBAL_ASM` slot name and the promoted
function name both change, which IS a lock-key change: `src/rom/lib_X.c:<old>` -> `<new>`, re-lock), and
re-register the inventory row. The __osActiveQueue/__osRunQueue swap cannot use aliases (both names exist with
opposite meanings); it needs a coordinated rename of 430 blob prelude declarations and every body that uses them.

## Policy question for the owner (no action taken)

About 4.2 KB of static code is ultralib hand-written assembly (`bcopy`, `bcmp`, `bzero`, cache ops,
`__osDisableInt`/`__osRestoreInt`, `__osEnqueueThread`/`__osDispatchThread`, `__osException` (1,360 B), TLB, SR,
cause, compare, count and fpcsr accessors, `sqrtf`, `osSetIntMask`). w15g showed it assembles byte-identically from
`reference/repos/ultralib/src/**/*.s` with IDO `cc` (`-O2 -mips2`, or `-mips3 -32` for exceptasm.s). Should it count
as static coverage? If yes, it would need: (1) a separate "assembled-source" category in `make progress`, reported
apart from C, so "from C" stays meaningful; (2) a source-of-truth rule, either vendoring the ultralib `.s` files
(license and provenance header) or treating the splat `asm/us/nonmatchings` output as final; (3) a promotion path
that builds `.s` with IDO `cc` (not GNU as) behind the same full-ROM SHA-1 gate, with its own lock kind (source
SHA-256 of the `.s`, no C body hash); and (4) the label fixes above first, since several of these sit under shifted
labels (osInvalICache holds writebackdcacheall.s; osWritebackDCacheAll's 32 B at 0x8000870C is not that function).
Alternatively they stay GLOBAL_ASM permanently and leave the denominator, which changes the static total instead.

## Files

`sources/` (34 .c), `ctx/` (context rows for promote_batch; `osAiSetNextBuffer.h` TU additions), `check.py`,
`ctx.py`, `run_all.sh` -> `dryrun.log` (builder), `precheck.py` -> `precheck.log` (Pi), `tu_e4f0.sh` ->
`tu_e4f0.log` (builder). Builder scratch layout: see `run_all.sh` header.
