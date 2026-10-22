# Static code: the assembly ceiling

> **Update 2026-10-21.** The static denominator now also counts the 10 hand-written routines in the boot
> segment tail (section 2a, `pipeline.boot_data`): **632/677 functions, 151,176/162,284 bytes (93.16%)**.
> Assembly in the original build: **43 functions / 7,816 bytes** (sections 1, 2 and 2a). C-reachable: the
> same **2 functions / 3,196 bytes** (section 3). Ceiling: **634/677 functions, 154,372/162,284 bytes
> (95.12%)**. The wave-28 text below is unchanged apart from this note and section 2a.

> **Policy (b) adopted, 2026-10-21 (owner).** Functions that were assembly in the original build count as static
> coverage **from assembly**, reported beside C: `make progress` prints `666/677 functions, 157,476/162,284 bytes
> (97.04%: from C 633 / 93.18%, from assembly 33 / 3.85%)` (after `__osAiDeviceBusy`, below). The 33 functions of sections 1 and 2 are pinned in
> `asm_matched.lock.json` by `python3 -m tools.conveyor.pipeline.asm_lock`. Source of truth is the repo's own
> assembly (splat's disassembly, which the ROM build already assembles byte-identically); ultralib is cited as
> provenance and not vendored, because it carries no licence. Each entry records its source `.s`, the sha256 of its
> instruction words, and evidence: `ultralib:<file>` must match an IDENTICAL row of the w28s proof log covering
> its address, otherwise `handwritten:<reason>`. `add` refuses a function that is promoted to C, has a C lock, or
> whose words differ from the ROM; `check` runs in the pre-commit hook and in `tests/conveyor/test_asm_lock.py`.
> Section 2a's 10 routines joined on the same day: their `.s` (`asm/us/boot_data/`) is assembled and composed
> into their `data.bin` slots by the Makefile, so `data.bin` itself (110 receipts) is unchanged; 43 functions /
> 7,816 B are now locked and static reads `676/677 functions, 159,040/162,284 bytes (98.00%: from C 633 / 93.18%,
> from assembly 43 / 4.82%)`. The 7 ultralib name leads were applied the same day (lane ulr,
> `docs/SYMBOL_MISATTRIBUTION.md`). The C-reachable pair (section 3)
> stays C work: the lock refuses them. `__osAiDeviceBusy` landed as C on 2026-10-21 (segment 0x10760 converted
> to `rom/lib_10760`, promoted with the July score0 lock; ROM SHA-1 exact), leaving `func_80012D18`. It landed as C the same evening
> (lane z12d18, `cloud/work/frontier/z12d18/RESULTS.md`; ROM SHA-1 exact): static is `677/677 functions,
> 162,188/162,284 bytes (99.94%: from C 634 / 95.12%, from assembly 43 / 4.82%)`; no C-reachable static function remains.

Status at wave 28 (lane w28s, 2026-10-12, after #377 func_80011910 landed): `make progress` reports
**static code 632/667 functions, 151,176/160,720 bytes (94.06%)**. Of the 35 functions still in assembly,
**33 functions / 6,252 bytes are assembly in the original build too**: they cannot become C and are
the static floor. Only **2 functions / 3,196 bytes** are C-reachable. That makes the static ceiling
**634/667 functions, 154,372/160,720 bytes (96.05%)** as `make progress` counts it.

Sizes are population slot sizes from `pipeline.layout derive`, including each slot's trailing alignment
words. The 96 bytes between 160,720 and (151,176 + 9,448) are alignment padding inside promoted slots
that coverage never counts (`owned_text` boundaries). Game-code and runtime-image (overlay) coverage are
separate populations and are not part of this page.

## 1. libultra assembly: byte-identical to ultralib (27 functions, 4,816 B)

Each object was built from [ultralib](https://github.com/decompals/ultralib) (decompals `e24c836`,
`reference/repos/ultralib`, git-ignored) **VERSION_J** (`2.0J`, the version that
w24t/w25a/w27s established for this ROM), `libultra_rom` flags (`-D_FINALROM -DNDEBUG -non_shared`), using
ultralib's own IDO recipe (`makefiles/ido.mk`: `cc -c -Wab,-r4300_mul -G 0 -nostdinc`, `.s` at `-O1`,
`src/libc/*.s` at `-O2`, `-mips2 -o32`, `exceptasm.s` at `-mips3 -32`) and the project's IDO 5.3 (toolkit
`796ae99a`). w24t's `compare.py` then compared the object's **whole `.text`** with the retail ROM, checking
relocations by value. Every row printed `RESULT: IDENTICAL (..., 0 differing words)`.

Proof (builder, then Pi; log with object SHA-256s: `cloud/work/frontier/w28s/ceiling/prove.log`):

```sh
sh cloud/work/frontier/w28s/ceiling/prove.sh $SCRATCH/ul J      # builder; $SCRATCH/ul/ultralib = {include,src} copy
scp 'watchman2:$SCRATCH/ul/J/*.o' cloud/work/frontier/w28s/ceiling/obj/
sh cloud/work/frontier/w28s/ceiling/compare.sh                   # Pi, repo root; addresses in vram.txt
```

| Project function | Bytes | ultralib file (2.0J) | ultralib symbol | Object vs ROM |
|---|---:|---|---|---|
| osSetIntMask | 160 | `src/os/setintmask.s` | osSetIntMask | IDENTICAL @0x800071D0 (40 words, 4 relocs) |
| osWritebackDCacheAll | 48 | `src/os/writebackdcacheall.s` | osWritebackDCacheAll | IDENTICAL @0x80007810 (12 words) |
| osWritebackDCache | 128 | `src/os/writebackdcache.s` | osWritebackDCache | IDENTICAL @0x80007CA0 (32 words) |
| osInvalICache | 128 | `src/os/invalicache.s` | osInvalICache | IDENTICAL @0x80008460 (32 words) |
| osInvalDCache | 176 | `src/os/invaldcache.s` | osInvalDCache | IDENTICAL @0x800084E0 (44 words) |
| bzero | 160 | `src/libc/bzero.s` | _bzero / _blkclr | IDENTICAL @0x80008590 (40 words) |
| __osDisableInt, __osRestoreInt | 112 + 32 | `src/os/interrupt.s` | same | IDENTICAL @0x8000C4B0 (36 words, 4 relocs) |
| osGetCount | 16 | `src/os/getcount.s` | osGetCount | IDENTICAL @0x8000C970 (4 words) |
| __osException | 16 | `src/os/exceptasm.s` | **__osExceptionPreamble** | IDENTICAL @0x8000C980: whole object, 584 words, 76 relocs |
| __osExceptionPreamble | 1,356 | (same object) | **__osException** (+ internal labels) | (same) |
| __osSendInterrupt | 232 | (same object) | **send_mesg** + handle_CpU | (same) |
| __osEnqueueAndYield | 256 | (same object) | __osEnqueueAndYield | (same) |
| __osEnqueueThread | 72 | (same object) | __osEnqueueThread | (same) |
| __osDispatchThread | 380 | (same object) | __osDispatchThread | (same) |
| __osExceptionPanic | 8 | (same object) | **__osCleanupThread** | (same) |
| bcopy | 784 | `src/libc/bcopy.s` | _bcopy | IDENTICAL @0x8000D2B0 (196 words) |
| __osSetSR | 16 | `src/os/setsr.s` | __osSetSR | IDENTICAL @0x8000D770 (4 words) |
| __osGetSR | 16 | `src/os/getsr.s` | __osGetSR | IDENTICAL @0x8000D780 (4 words) |
| __osSetFpcCsr | 16 | `src/os/setfpccsr.s` | __osSetFpcCsr | IDENTICAL @0x8000D790 (4 words) |
| __osTlbFlush | 80 | `src/os/unmaptlball.s` | **osUnmapTLBAll** | IDENTICAL @0x8000D840 (20 words) |
| __osTlbInit | 96 | `src/os/maptlbrdb.s` | **osMapTLBRdb** | IDENTICAL @0x8000D890 (24 words) |
| osCauseGet | 16 | `src/os/getcause.s` | **__osGetCause** | IDENTICAL @0x8000DBB0 (4 words) |
| sqrtf | 16 | `src/gu/sqrtf.s` | sqrtf | IDENTICAL @0x8000E3C0 (4 words) |
| bcmp | 288 | `src/libc/bcmp.s` | _bcmp | IDENTICAL @0x8000F8D0 (72 words) |
| __osSetCompare | 16 | `src/os/setcompare.s` | __osSetCompare | IDENTICAL @0x8000FB90 (4 words) |
| __osProbeTLB | 192 | `src/os/probetlb.s` | __osProbeTLB | IDENTICAL @0x8000FCB0 (48 words) |

exceptasm.s's object also contains `__osPopThread` (0x8000D10C, 16 B). That function is already
C-promoted in `src/rom/lib_d580.c`; it is counted in the population as C, not here.

**Name leads** (the bold names above; **applied 2026-10-21 by lane ulr**, so the project now uses the bold names): the project's labels differ from ultralib's at
0x8000C980/0x8000C990 (the two are swapped), 0x8000CEDC, 0x8000D298, 0x8000D840, 0x8000D890 and 0x8000DBB0.
The bytes are proven identical, but nothing was renamed: a rename is a `symbol_addrs` edit with the
owned-context re-pin (see the w24t/w25a/w27s rename procedure).

## 2. Hand-written assembly with no C form (6 functions, 1,436 B)

| Function | Bytes | Segment | Evidence |
|---|---:|---|---|
| lzss_decode | 596 | lib_5610 (converted) | non-ABI: reads `t1` after each call and spills `s0` itself (w25a RESULTS, `cloud/work/frontier/w25a/RESULTS.md`; w26a) |
| inflate_io_wait | 236 | lib_5610 (converted) | non-ABI: returns in `t1`, clobbers `s0`-`s4` (w25a; w26a) |
| inflate_entry | 360 | lib_5610 (converted) | hand-edited call: `li a1,12000` + `addiu a0,zero,0` after a one-argument compile; no C order reaches it (w27s RESULTS, best `LOCK: true=25`) |
| inflate_entry_alt | 132 | lib_5610 (converted) | same hand edit (w27s) |
| entrypoint | 80 | 0x1000 (whole-asm) | boot stub: clears .bss from 0x8002E8E0, sets `sp`, jumps to `main` (`/* Handwritten function */`) |
| __setfpcsr | 32 | 0x55F0 (whole-asm) | `ctc1 a0,$31; nop; nop; jr ra` + padding; not an ultralib routine (ultralib's `__osSetFpcCsr` also returns the old value) |

## 2a. Boot segment tail: hand-written, still in the ROM data bin (10 functions, 1,564 B)

Added 2026-10-21. These sit past splat's static code end (ROM 0x283D0) inside the `data` bin, so they are
not in `layout derive`; `pipeline.boot_data` labels them and `blob_rom`'s cartridge report adds them to the
static denominator. `tests/conveyor/test_boot_data.py` proves the extents and callers.

| Function | Bytes | Evidence |
|---|---:|---|
| func_800277D0, func_800278C4 | 244, 188 | libgsm RPE decoding and long-term synthesis, called by `func_80026558`; each reads an inline table PC-relatively with a never-taken `bltzal $zero` |
| func_80027B44, func_80027980, func_80027C98 | 340, 452, 164 | short-term synthesis: `func_80026360` calls B44, which `bal`s into 980/C98 with no frames; C98 calls a coefficient routine through `jalr t3` eight times |
| func_80027D3C, func_80027D54, func_80027D64, func_80027D7C | 24, 16, 24, 84 | the coefficient routines: pointers formed as `ra + imm` after `bltzal $zero`; three branch into D7C (`LARp_to_rp`, constants 11059/20070/26112) |
| func_8002C5E0 | 28 | D-cache `cache 0xd` loop over [a0, a1), placed among the segment's .data; jal'd by six static functions |

None of this has a C form: PC-relative self-addressing, frameless `bal` calls and computed callbacks into
code fragments are hand-written assembly. They are built from `asm/us/boot_data/*.s` (`pipeline.boot_data asm`
generates them from splat's disassembly, with the two inline tables as `.word` data), composed into their
`data.bin` slots by the Makefile, and locked in `asm_matched.lock.json` (2026-10-21). A builder drill changed one
instruction in `kernels.s` and the ROM SHA-1 failed, so the composition is what the gate judges.

## 3. C-reachable, still open (2 functions, 3,196 B)

| Function | Bytes | State |
|---|---:|---|
| func_80012D18 | 3,148 | **Landed 2026-10-21** as IDO 7.1 C (lane z12d18, `cloud/work/frontier/z12d18/RESULTS.md`; ROM SHA-1 exact). Was a near miss: 674/787 words differ at baseline, body 6 words short (#376, `cloud/work/frontier/dot_static_12d18_20261013/RESULTS.md`) |
| __osAiDeviceBusy | 48 | **C in ultralib** (`src/io/ai.c`): compiled at `-O2` with IDO 5.3, it is IDENTICAL @0x8000FB60 (12 words, prove.log above). Its segment 0x10760 is whole-asm, so landing it needs a segment conversion (`pipeline.layout convert`) with an `-O2` flagset, not a new match |

## 4. Whole-assembly segments (not converted to ROM TUs)

These segments are single splat `asm` files (`asm/us/<ROM>.s`) linked as they are. Their functions are
counted in sections 1-3 above.

| ROM segment | Bytes | Contents |
|---|---:|---|
| 0x1000 | 80 | entrypoint |
| 0x55F0 | 32 | __setfpcsr |
| 0x8410 | 48 | osWritebackDCacheAll |
| 0x88A0 | 128 | osWritebackDCache |
| 0x90E0 | 176 | osInvalDCache |
| 0x9190 | 160 | bzero |
| 0xD0B0 | 144 | __osDisableInt, __osRestoreInt |
| 0xD570 | 16 | osGetCount |
| 0xDEB0 | 784 | bcopy |
| 0xE370 | 32 | __osSetSR, __osGetSR |
| 0xE440 | 80 | __osTlbFlush (osUnmapTLBAll) |
| 0xE490 | 96 | __osTlbInit (osMapTLBRdb) |
| 0xEFC0 | 16 | sqrtf |
| 0x104D0 | 288 | bcmp |
| 0x10760 | 48 | __osAiDeviceBusy (C, section 3) |
| 0x10790 | 16 | __osSetCompare |
| 0x108B0 | 192 | __osProbeTLB |

These segments total 2,336 B. The rest of the assembly sits as `GLOBAL_ASM` slots in converted TUs:
lib_5610 (the inflate set), lib_7c80 (osSetIntMask), lib_8e10 (osInvalICache), lib_d580 (exceptasm),
lib_e390 (__osSetFpcCsr) and lib_e4f0 (osCauseGet); lib_11640's last slot (func_80012D18) is C since 2026-10-21. The ROM header and IPL3
(0x0-0xFFF) are not in the static population.
