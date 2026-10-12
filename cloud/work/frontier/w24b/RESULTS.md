# w24b - static pre-tail boot code (36 slots) + hand asm bcmp/__osSetCompare/__osProbeTLB (wave 24)

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w24b`. Nothing locked, promoted,
spliced or committed. Static numbers only. Status: done. Proofs: `prove.log` (builder `bash lane/tools/prove.sh`, master 3bec9ce3).

NOTE on compiler: the pre-tail segments are NOT IDO 7.1. `rom_compilers.json` pins 7.1 only for 0x11050-0x26ab0;
everything below is the 5.3 default. Per-TU flags come from the layout (`opt_overrides.mk`).

## Step 1: triage (36 GLOBAL_ASM slots below 0x8000F3A4 in converted TUs, 13,180 B by layout sizes; w23c quoted 13,152)

| slot (current label) | addr | B | TU / flags | identity / origin | class |
|---|---|---:|---|---|---|
| __scMain | 0x8000063C | 324 | lib_1050_sdk, 5.3 `-g1 -O1 -Wab,-r4300_mul` | Rush-modified SDK `sched/sched.c` __scMain | C (module_campaign near-miss) |
| __scHandleRetrace | 0x80000880 | 208 | lib_1050_sdk | sched.c __scHandleRetrace (Rush variant) | C (module_campaign near-miss) |
| __scAppendList | 0x80000D6C | 180 | lib_1050_sdk | sched.c __scAppendList | C (module_campaign: 1 insn) |
| __scExec | 0x80000E20 | 360 | lib_1050_sdk | sched.c __scExec (Rush variant) | C (module_campaign: 5 diffs) |
| viTickStart, viEnableAccum, viDisableAccum, viUpdateTime, viScheduleTick, viAddTicks, viGetTimeToDeadline | 0x80001350-0x800015BC | 112+28+24+252+76+60+68 = 620 | lib_1050 (0x1f50), layout says `-g0 -O2`, really `-g1 -O1` (below) | Rush frame-timing code (no library ancestor; uses osSetIntMask, gViTickCounter) | C, Rush-specific (all 7 matched here) |
| game_init | 0x80002238 | 708 | lib_2cf0, 5.3 `-g1 -O1 -Wab,-r4300_mul` | Rush game init (arcade game.c analogue only) | C, Rush-specific (matched here) |
| inflate_io_wait .. inflate_entry_alt (12) | 0x80004A10-0x800069FC | 8,104 | lib_5610, 5.3 `-g0 -O2` | gzip/zlib-style inflate.c (Mark Adler, public, not vendored) + Rush lzss_decode; inflate_io_wait returns through t1 and clobbers unsaved s0-s4 (static_C20): hand-asm-like ABI | C (non-library source; O2 confirmed: C20/C23 drafts score worst at -O1/-g1); prior attempts C20/C21/C23 + module_campaign lib5610 far off; not attempted here |
| osSetIntMask (layout: osSetGlobalIntMask) | 0x800071D0 | 160 | lib_7c80 | ultralib `os/setintmask.s` | hand asm, stays asm |
| osInvalICache (0x80008460) | 0x80008460 | 128 | lib_8e10 | ultralib `os/invalicache.s` (w22g) | hand asm, stays asm |
| rmonPrintf | 0x8000870C | 36 | lib_92f0, 5.3 `-g0 -O2` | ultralib `libc/syncprintf.c` rmonPrintf, VERSION_J, `_FINALROM` | **ultralib C: match** |
| __osException, __osExceptionPreamble, __osSendInterrupt, __osEnqueueAndYield, __osEnqueueThread, __osDispatchThread, __osExceptionPanic | 0x8000C980-0x8000D2A0 | 16+1356+232+256+72+380+8 = 2,320 | lib_d580 | ultralib `os/exceptasm.s` (labels shifted vs ultralib: __osExceptionPreamble, __osException, send_mesg+handle_CpU (by position), __osEnqueueAndYield, __osEnqueueThread, __osPopThread+__osNop+__osDispatchThread, __osCleanupThread) | hand asm, stays asm |
| __osSetFpcCsr | 0x8000D790 | 16 | lib_e390 | ultralib `os/setfpccsr.s` | hand asm, stays asm |
| osCauseGet | 0x8000DBB0 | 16 | lib_e4f0 | ultralib `os/getcause.s` | hand asm, stays asm |

Hand asm named in the brief: bcmp (0x8000F8D0, 284; `libc/bcmp.s`), __osSetCompare (0x8000FB90, 12;
`os/setcompare.s`), __osProbeTLB (0x8000FCB0, 184; `os/probetlb.s`): ultralib has only the `.s` form (no C in any
version), so they stay asm.

Summary: ultralib C 1 slot (36 B); ultralib hand asm 11 slots (2,640 B) + the 3 brief items (480 B); Rush-modified
SDK scheduler 4 slots (1,072 B, near-misses); Rush-specific C 20 slots (vi* 620, game_init 708, inflate/lzss 8,104).

## Step 2: results (IDO 5.3, toolkit 796ae99a; check.py = `lock add` verify_entry replica)

**11 functions, 1,752 B** C-identical, each with its whole TU linked == baserom. 4 need only lock+promote
(rmonPrintf, __scAppendList) or a stale-target refresh first (__scHandleRetrace, game_init); the 7 vi* need a segment
transaction (flag switch + viStub retirement + one rom_tu.h line). Plus 2 code-identical leads (684 B) needing owned .bss.

| function | B | flags | state | scorer line (prove.log) |
|---|---:|---|---|---|
| rmonPrintf | 36 | -g0 -O2 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (7 rows)`; lib_92f0 spliced, linked == baserom |
| __scAppendList | 180 | -g1 -O1 -Wab,-r4300_mul | MATCH (1 device) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (45 rows)` |
| __scHandleRetrace | 208 | same | MATCH after target refresh (devices) | stale DB object: `LOCK: true=10 strict=10 reloc_blind=0 objdump-dr DIFFERS(2 rows) (64 rows)` (pre-w22g callee names osViSetMode/osJamMesg; retail calls osViSwapBuffer/osSendMesg); renamed target `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (64 rows)` |
| game_init | 708 | same (lib_2cf0) | MATCH after target refresh (devices) | stale: `true=10 strict=10 reloc_blind=0 DIFFERS(2 rows) (279 rows)` (osInvalICache_full/osJamMesg); renamed `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (279 rows)` |
| viTickStart | 112 | same (0x1f50) | MATCH after target refresh + segment change | stale: `true=10 strict=10 reloc_blind=0 DIFFERS(2 rows) (39 rows)` (osSetGlobalIntMask); renamed `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (39 rows)` |
| viEnableAccum | 28 | same | MATCH + segment change | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (8 rows)` |
| viDisableAccum | 24 | same | MATCH + segment change | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (7 rows)` |
| viUpdateTime | 252 | same | MATCH + segment change | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (104 rows)` |
| viScheduleTick | 76 | same | MATCH + segment change | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (28 rows)` |
| viAddTicks | 60 | same | MATCH + segment change | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (20 rows)` |
| viGetTimeToDeadline | 68 | same | MATCH + segment change | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (22 rows)` |
| __scMain | 324 | same | code identical, needs owned .bss | probe `LOCK: true=15 strict=15 reloc_blind=0 objdump-dr DIFFERS(7 rows) (100 rows)` |
| __scExec | 360 | same | code identical, needs owned .bss | probe `LOCK: true=20 strict=20 reloc_blind=0 objdump-dr DIFFERS(7 rows) (106 rows)` |

Whole-TU proofs (`tools/tuproof.sh`: asm-processor + the Makefile ROM-TU flags, GNU ld at the TU's vram, every
undefined symbol from symbol_addrs/undefined_*_auto; control = today's TU, same script):
- lib_92f0 + rmonPrintf (-O2): `linked .text == baserom: IDENTICAL`.
- lib_1050_sdk + __scAppendList + __scHandleRetrace (-g1 -O1, .rodata at 0x8002D4A4): 3840 B, sha1 7b292c1d.. == baserom.
- lib_2cf0 + game_init (-g1 -O1): 1104 B, sha1 828f9d1c.. == baserom.
- lib_1050 (0x1f50) with the 7 vi bodies, viDeadlinePassed's locked text unchanged, the "viStub" definition removed,
  rom_tu.h patched, built at -g1 -O1: 672 B, sha1 5fa164c0.. == baserom (control: today's -O2 TU, same sha1).

### Key findings / levers
1. **Segment 0x1f50 (lib_1050.c) is `-g1 -O1 -Wab,-r4300_mul`, not -g0 -O2.** Its O2 flagset came from two tiny
   locks that happen to match at O2. At -g1 -O1 every function ends with two `jr ra; nop` pairs (plus the return's
   own `jr`): viDeadlinePassed is 48 B and the 20-B "viStub" slot at 0x800015DC is just its trailing
   `jr ra; nop; jr ra; nop` (+ padding). viStub is not a function (its locked O2 body is m2c filler).
2. **`volatile` orders -O1 loads**: as1 keeps volatile loads in program order. viUpdateTime's first statement needs
   gViTickCounter AND gViLastTick volatile (one is not enough); the scheduler timestamps (u64) need volatile for
   __scMain/__scExec's hi/lo load order (module_campaign's "five scheduling differences" = this).
3. **At -g1, as1 does not move code across statements.** An empty statement after a store keeps it out of the next
   `b`'s delay slot (__scAppendList); conversely game_init needs the get_tv_offset assignment and the
   osCreateScheduler call in one (comma) statement so `move s0,v0` can sink below the next lui/addiu.
4. **Single `lui` for both halves of a u64 store = the object is defined in the TU** (extern u64 gets a second lui):
   the three scheduler timestamps (__osScRetraceTime 0x8002E8E0, __osScAudioStart 0x8002EB78, __osScFrameTime
   0x8002EB80) are .bss owned by the scheduler object; the splat "Hi/Lo" symbol pairs are one u64 each.
5. **Stale DB target objects**: w22g/w23g renamed symbols but the reloc-aware inventory objects of their callers
   still name the old callee (viTickStart, __scHandleRetrace, game_init here). `lock add` needs those refreshed
   (`targets_rename`-style object rebuild), same as w19c's refresh rows.
6. assert-style `if (!(EX)) {}` (beq/beq) vs `if (EX) {}` (beq/bne); SDK declaration lists explain -O1 frames
   (__scHandleRetrace: rspTask/i/state unused, sp/dp `= 0`).

### Not matched / stays asm
- Hand asm (ultralib `.s` only, no C form in any version): osSetIntMask, osInvalICache (0x80008460), the 7 exceptasm.s
  slots, __osSetFpcCsr, osCauseGet, and the brief's bcmp/__osSetCompare/__osProbeTLB. Stay GLOBAL_ASM (policy item
  from w15g/w16g: they assemble byte-identically from ultralib .s).
- inflate/lzss (12 slots, 8,104 B): not attempted beyond a flag check (O2 is right). lzss_decode + inflate_io_wait
  share a non-ABI register contract (t1 return, unsaved s0-s4) and are probably hand-written asm.

## Integration (coordinator)

```
L=cloud/work/frontier/w24b; F1="-g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul"
```
A. **rmonPrintf + __scAppendList** (no prerequisites; 5.3 default toolkit, no --toolkit):
```
python3 -m tools.conveyor.pipeline.lock add $L/sources/rmonPrintf.c:rmonPrintf --flags "-g0 -O2 -mips2 -G 0 -non_shared"
python3 -m tools.conveyor.pipeline.lock add $L/sources/__scAppendList.c:__scAppendList --flags "$F1"
python3 cloud/work/boot_tail_promotion/promote_batch.py $L/ctx53_O2.jsonl 0x92f0 --only rmonPrintf --prefix-dedup rom/lib_92f0 --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py $L/ctx53_sched.jsonl 0x1050 --only __scAppendList --prefix-dedup rom/lib_1050_sdk --dry-run
```
B. **__scHandleRetrace, game_init**: first refresh their inventory target objects (stale callee names; plan
`targets_rename audit`/refresh as w19c - `refresh __scHandleRetrace`, `refresh game_init`; the emulation in prove.sh
is `objcopy --redefine-sym osViSetMode=osViSwapBuffer --redefine-sym osJamMesg=osSendMesg` resp.
`osInvalICache_full=osInvalICache, osJamMesg=osSendMesg`), then `lock add ... --flags "$F1"` and promote_batch
with `$L/ctx53_sched.jsonl 0x1050 --only __scHandleRetrace` / `$L/ctx53_gi.jsonl 0x2cf0 --only game_init`
(`--prefix-dedup rom/lib_1050_sdk` / `rom/lib_2cf0`).
C. **vi* (segment 0x1f50)**, one transaction: (1) apply `$L/patch/rom_tu_volatile.diff` (src/rom/rom_tu.h:
`extern volatile s32 gViTickCounter;` + `extern volatile s32 gViLastTick;`; only lib_1050.c uses them);
(2) retire the viStub function (symbol_addrs/layout/inventory row and its lock entry; its 20 B become part of
viDeadlinePassed, extent 0x800015BC..0x800015EC = 48 B) and re-lock viDeadlinePassed with `$L/vi1050/viDeadlinePassed.c`
(locked body, flags $F1) against a refreshed 48-B target; the derived segment flagset then becomes $F1 (layout
regenerates opt_overrides.mk); (3) refresh viTickStart's target (osSetGlobalIntMask -> osSetIntMask); (4) lock add the
7 sources with $F1 and promote_batch `$L/ctx53_vi.jsonl 0x1f50 --only viTickStart,...,viGetTimeToDeadline
--prefix-dedup rom/lib_1050`. Static count: +7 functions, -1 (viStub), bytes +620. ctx53_vi rows were computed on
the patched header.
D. **__scMain/__scExec** (not deliverable yet): needs the scheduler object's .bss as an owned block (it spans
0x8002E8E0..0x8002EBA8 with gScheduler, __osScTaskCount, the vi globals between), the Hi/Lo symbol pairs merged
into three u64 objects, and names refreshed. Probes: `$L/sched/__sc{Main,Exec}_probe.c`.
Context rows carry builder-relative `lane/sources/...` paths (w23c convention). Note: context_check.py ignores -g1 (compiles -O1 -g0); the TU proofs above are at real -g1. No unit_overrides,
no groups superseded.

## Disclosures (also in each source header)
- __scAppendList: empty statement after `audioListPending = 1;` (compiled-out logging).
- __scHandleRetrace: SDK locals rspTask, i, state unused = exact frame residual (retail 48, without 40, 3 slots);
  empty `else {}`.
- game_init: SIX unused locals (retail frame 0x50, without 0x38, 6 slots; identity unknown); comma-expression
  statement; `register` local; function-pointer cast on audio_thread_entry.
- viTickStart: `register f32 t`, chained assignment. vi globals volatile (header patch).
- No pad arrays, no implicit int, no false prototypes. rmonPrintf keeps ultralib's own two unused locals.
- Builder: only ~/rush2049/scratch/frontier/w24b (rom_tu.h patched there temporarily by prove.sh and restored);
  mktemp dirs. No permission denials.
