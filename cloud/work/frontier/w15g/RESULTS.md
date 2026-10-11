# w15g - static (boot/library) side investigation

Read-only except this lane dir and builder scratch (`~/rush2049/scratch/frontier/w15g`). Nothing locked, promoted,
spliced or committed. Nothing written to `cloud/matches/` (see "Scorer caveat").

## Method

- Inventory: conveyor DB `n64_target` (population=static, 668 rows) minus the 402 `target_id`s in `matched.lock.json`
  = 266 unmatched static targets, 87,536 B of real code (`inv.json`, `static_rows.json`; 86 named = 20,272 B,
  179 `func_*` = 67,264 B). `make progress` = `blob_rom coverage` (400/668 fn, 73,596/160,720 B).
- Target objects: the reloc-aware `.o` blobs conveyor already built (`tgt/*.o`, copied from `~/.conveyor/blobs`).
- Scorer: `sc.py` = IDO from toolkit d4c39cbc (the one the locks were verified with) + `tools/conveyor/jobs/scoring.py`
  (`score()` true score and `reloc_blind_score()`), the same functions the farm uses. Sources are the vendored
  `reference/repos/ultralib` (HEAD) compiled with `-DBUILD_VERSION=VERSION_L -D_FINALROM -DNDEBUG -Xcpluscomm`.
- `sweep.py`: compiled every ultralib `.c/.s` (702 funcs) at {-O1,-O2} x {-mips2, -mips3 -32} and compared each
  function's relocation-masked words against all 266 targets by CONTENT (not name). Output `sweep_L.json`.
  Version H (pre-J) found nothing new (`sweep_H.log`).

## Scorer caveat

`tools/cloud/score.py fn` cannot score static targets: it reads `asm/us/blob/*.s` (game image only), and has no IDO
on the Pi (`tools/cloud/ido/cc missing`). Static targets are scored through the conveyor target `.o` + `scoring.py`,
which is what I used. Because `score.py fn` can never print MATCH for a static name, I did NOT write anything to
`cloud/matches/`. Candidate sources are in `cand/`. "true=0" below is the conveyor true score vs the reloc-aware target.

## Key findings

1. **The static symbol labels are partly wrong (shifted), so name-pairing against ultralib misses.** Examples:
   `osEPiRawStartDma`@0x8000fe00 is really `__osEPiRawWriteIo`; `osEPiRawWriteIo`@0x8000fda0 is
   `__osResetGlobalIntMask`; `__osPiGetCmdQueue`@0x800100c0 is `__osSetGlobalIntMask`; `osMotorStop`@0x8000a2f0 is
   `osPfsRepairId`; `__osPiDeviceBusy`@0x8000fd70 is `__osSiDeviceBusy` (reads SI_STATUS 0xA4800018);
   `osSetGlobalIntMask`@0x800071d0 is the asm `osSetIntMask`; `__osPfsDataChecksum`@0x8000fac0 is ultralib's
   `__osContDataCrc`; `__osExceptionPreamble`@0x8000c990 (1360 B) is the whole asm `__osException`.
2. **`__osContDataCrc`@0x8000fa4c (128 B) is not a function**: it is the unrolled tail of `__osContAddressCrc`
   (0x8000f9f0, 208 B, which matches whole). Extent/inventory bug: that target can never match alone.
3. **`read_controllers` target has a bogus ~470 KB extent** (`target .o` text = 470,976 B). Needs extent repair.
4. **libgcc ll/llcvt (`__muldi3`, `__divdi3`, `__fixdfdi` ... 18 fns) match with `-mips3 -32`** (`-O1` for the float
   conversions and `__moddi3`/`__qdivrem`; any of -O1/-O2/-O3 for mul/div/shifts). The farm seeds use `-mips2`
   (`-mips3` alone fails: "implies -64bit"), which is why every one of them sat at score 600-3105 as "seeded".
5. **Library version matters**: ultralib HEAD has `#if BUILD_VERSION >= VERSION_J` branches. Rush 2049 code is the
   J-or-later variant (`__osContRamWrite`, `__osPfsRWInode`, `osPfsRepairId`, `__osContAddressCrc` all match J+).
   The corpus-submit TUs did not define `BUILD_VERSION`, so the pre-J branch was compiled and nothing scored.
6. Hand asm in ultralib (`*.s`) assembles byte-identically with `cc -O2 -mips2` (or `-mips3 -32` for exceptasm.s,
   which uses `sd/ld` on odd regs): bcopy, bcmp, bzero, osInvalDCache, osWritebackDCache, __osDisableInt,
   __osRestoreInt, __osEnqueueThread, __osDispatchThread, TLB/SR/cause/compare/count/fpcsr accessors, sqrtf,
   osSetIntMask, __osException. They cannot become "from C" coverage; they need a hand-asm-source policy
   (audit doc line 96: "retained handwritten assembly needs separate policy").

## Results table (best score line; `true`/`rb` = relocation-blind)

C-source known (ultralib HEAD, version J+), code identical:

| target (label) | actual fn | bytes | segment/addr | source | best score | residual |
|---|---|---|---|---|---|---|
| __osPfsRWInode | same | 736 | boot 0x8000f3a4 | io/contpfs.c | true=0 (-g0 -O2 -mips2, VERSION_L; data syms renamed to `__osSiChannelMask/__osSiLastChannel/__osContPifInode`) | none, needs the project's data names |
| __osContRamWrite | same | 592 | 0x8000f680 | io/contramwrite.c | true=5 rb=0 -O2 -mips2 (only my renamed callee `osContStartReadData_` differs) | symbol name of `__osPfsGetStatus` in project is `osContStartReadData` |
| osEPiRawReadIo | __osEPiRawReadIo | 352 | 0x8000ff60 | io/epirawread.c | true=0 -O2 -mips2 `-D__osCurrentHandle=__osPiDevList` | none |
| osEPiRawStartDma | __osEPiRawWriteIo | 352 | 0x8000fe00 | io/epirawwrite.c | true=0 -O2 -mips2 same -D | none |
| __osContAddressCrc | same | 208 | 0x8000f9f0 | io/crc.c (J+ body) | true=0 -O2 -mips2 | none |
| __osContDataCrc | (fragment) | 128 | 0x8000fa4c | - | n/a | tail of AddressCrc (bad extent) |
| __osPfsDataChecksum | __osContDataCrc | 160 | 0x8000fac0 | io/crc.c | true=0 -O2 -mips2 | none |
| osAiSetNextBuffer | same | 144 | 0x8000be70 | io/aisetnextbuf.c | true=0 -O2 -mips2 | none |
| osMotorStop | osPfsRepairId | 96 | 0x8000a2f0 | io/pfsrepairid.c | true=0 -O2 -mips2 | none |
| osEPiRawWriteIo | __osResetGlobalIntMask | 96 | 0x8000fda0 | os/resetglobalintmask.c | true=0 -O1 -mips2 (`-D__OSGlobalIntMask=__osGlobalIntMask`) | none |
| __osPiGetCmdQueue | __osSetGlobalIntMask | 80 | 0x800100c0 | os/setglobalintmask.c | true=0 -O1 -mips2 | none |
| osYieldThread | same | 80 | 0x80010110 | os/yieldthread.c | true=5 rb=0 -O1 -mips2 | project data symbol `__osActiveQueue` vs `__osRunQueue`, callee `__osCleanupThread` vs `__osEnqueueAndYield` |
| __osPiDeviceBusy | __osSiDeviceBusy | 48 | 0x8000fd70 | io/si.c | true=0 -O2 -mips2 | none |
| func_800103A0 | __osContGetInitData (or __osPfsGetInitData) | 172 | 0x800103a0 | io/controller.c | true=20 rb=0 -O2 -mips2 | symbol names |
| __osEnqueueAndYield | osDestroyThread | 272 | 0x8000fba0 | os/destroythread.c | true=35 rb=0 -O1 -mips2 | symbol names (label shifted) |
| func_80014650, func_8002517C | __osPiRelAccess / __osSiRelAccess | 44+44 | 0x80014650, 0x8002517c | io/piacs.c, siacs.c | sweep: content-identical at -O1 and -O2 -mips2 | (not individually scored; multi-function files) |

Subtotal C, ultralib: ~3,476 B across 16 targets (func_ rows included).

libgcc (flags `-g0 -O1 -mips3 -32 -G 0 -non_shared`; source = trivial C on `long long`, `cand/libgcc.c`, `cand/libgcc2.c`):

| target | bytes | best score |
|---|---|---|
| __muldi3 / __divdi3 / __udivdi3 / __umoddi3 / __umoddi3_alt | 48/96/64/64/64 | true=0 (-O1/-O2/-O3) |
| __lshrdi3 / __ashldi3 / __ashrdi3 | 48 each | true=0, with a `long long` (s64) shift COUNT parameter |
| __moddi3 | 160 | true=0 -O1 only, body `r=a%b; if((r<0&&b>0)||(r>0&&b<0)) r+=b;` |
| __qdivrem | 96 | true=0 -O1, `void f(u64*q,u64*r,u64 a,u16 b){*q=a/b;*r=a%b;}` (s16 b gives 275) |
| __fixdfdi / __fixsfdi / __floatdidf / __floatdisf | 32 each | true=0 -O1 |
| __floatundidf / __floatundisf | 64 each | true=0 -O1 |
| __fixunsdfdi / __fixunssfdi | 160 each | true=10 rb=0 -O1: only the `.rodata` float constant address differs (needs data layout) |

Subtotal libgcc: ~1,312 B, 18 targets.

Hand asm from ultralib `.s` (identical bytes with IDO `as`, NOT C coverage):
bcopy 784 (true=0, -O2 -mips2), bcmp 288 (true=0), bzero 160, osInvalDCache 176, osWritebackDCache 128,
__osDisableInt 112 (rb 0), __osRestoreInt 32, __osEnqueueThread 80, __osDispatchThread 384 (rb 0, true 55 names),
__osExceptionPreamble 1360 (= `__osException`, content-identical), osSetGlobalIntMask 160 (= osSetIntMask),
__osTlbFlush 80, __osTlbInit 96, __osTLBLookup 192, osGetCount/__osGetSR/__osSetSR/__osSetFpcCsr/__osGetCause/
__osSetCompare/sqrtf 16 each (+ osInvalICache 48 = writebackdcacheall). Subtotal ~4.2 KB.

Known source but does not match: `__scMain` 336, `__scHandleRetrace` 208, `__scAppendList` 192, `__scExec` 368
(ultralib sched/sched.c; best `-g1 -O1 -Wab,-r4300_mul` with `assert(x)` -> `if(!(x)){}`: AppendList true=645 rb=16,
Exec 2815/74, Main 4245/67, HandleRetrace 5205/62). The retail code is an older/debug libultra build
(branch delay slots left as `nop`, locals spilled to the frame, assert expressions evaluated into empty `if`s),
and the sibling functions already promoted from src/rom/lib_1050_sdk.c came from a different published source.

No vendored source: inflate/huft/lzss (`huft_build` 1744, `inflate_dynamic` 1936, `inflate_free_window` 1280,
`lzss_decode` 608, `inflate_stored` 544, `inflate_fixed` 464, `inflate_block` 432, `inflate_entry` 368,
`inflate_read_bits` 320, `inflate_io_wait` 240, `inflate_entry_alt` 128, `inflate_loop` 96 = ~8.2 KB; gzip/zlib
style, public but not in `reference/`), `game_init` 720, `vi*` (~0.8 KB), `__osSendInterrupt` 240 (no ultralib
function matched), `__osTLBLookup`-adjacent TLB helpers, and the 179 `func_*` rows (67 KB, 0x80010450-0x80026xxx).
The `func_*` population is Rush-specific engine code (table-indexed structs at D_8003xxxx, no jal into libultra),
not library code; only the 5 listed (800103A0, 80014650, 8002517C, 80014BD8/80014BF8/800205E4 trivial stubs
ambiguous) matched ultralib content.

## Estimate (question 3)

- Recoverable as true C with known source at correct flags/version: ~3.5 KB (ultralib, 16 targets) + ~1.3 KB
  (libgcc, 18 targets) = ~4.8 KB, i.e. static 73.6 -> ~78.4 KB (45.8% -> ~48.8%). Of that, `__osPfsRWInode`,
  `__osContRamWrite` need project data/callee symbol names only.
- Another ~4.2 KB of byte-identical hand asm (only a coverage-policy matter, plus __osExceptionPreamble's 1.4 KB).
- Blocked: sched x4 (1.1 KB; source version), inflate/lzss (~8.2 KB; source not vendored), 179 func_ (67 KB; no
  public source, game engine), flag/extent problems (`read_controllers`, `__osContDataCrc`).

## Blockers on the farm side (why the attention list never closed these)

- `-mips3 -32` is not in any farm flagset; libgcc seeds compile at `-mips2`.
- Corpus submit pairs by exact name and compiles without `BUILD_VERSION=VERSION_J+`; labels are shifted anyway.
- Hand-asm functions get C seeds that cannot work (cache ops, exceptasm).
- `osWritebackDCacheAll` 'seed_does_not_compile': the real asm at 0x8000870c (32 B) is not that function's code;
  the writebackdcacheall.s bytes sit at the `osInvalICache` label (0x80007810).
