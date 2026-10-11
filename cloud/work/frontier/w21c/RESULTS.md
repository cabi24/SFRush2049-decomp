# w21c - static boot tail: the large functions with no D10 source (wave 21)

Targets: func_80012D18 (3148), func_8001A658 (2812), func_80023E9C (2796), func_80012730 (1512), func_8001F13C (1456),
func_800178B0 (1160), func_8001C860 (1084). IDO 7.1, `-g0 -O2 -mips2 -G 0 -non_shared`, toolkit 4586d230.
Builder scratch watchman2:~/rush2049/scratch/frontier/w21c (base + rsync). Tools: w20a's (paths renamed).

Read-only except this lane dir and builder scratch. Nothing locked, promoted, spliced into src/, or committed.
Static numbers only. Proof run against the Pi tree at master 8acd9eca (src/rom, include, tools synced to the builder).

## Headline

**1 function MATCH, 1,512 B** (`sources/func_80012730.c`), lock-style line identical and the spliced lib_11640 TU
IDENTICAL. **1 function code-identical, 1,084 B** (`ownrodata/func_8001C860.c`) that needs an owned .rodata slot for its
`1.f / 60.f` literal and a lock flagset with `-Wab,-r4300_mul`. Three near misses with traced residuals; two
identified but not attempted (time). Static by inventory sizes: +1 function / +1,512 B now; +1 / +1,084 B more after
the tooling below.

| function | B | MusyX identity | state | flags | scorer line (check.py = lock add verify_entry) |
|---|---:|---|---|---|---|
| func_80012730 | 1512 | N64 sample DMA cache fetch | **MATCH** | -g0 -O2 -mips2 -G 0 -non_shared | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (410 rows)`; `lib_11640: IDENTICAL (4276 lines: .text words+relocs, data sections)` |
| func_8001C860 | 1084 | snd3d.c CalcEmitter | code identical, owned rodata | ... -non_shared -Wab,-r4300_mul | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (280 rows)` (the 2 rows: `.rodata` vs `D_8002D900` HI16/LO16) |
| func_800178B0 | 1160 | seq.c seqStartPlay | near miss (1 store + 1 load) | -O2 plain | `LOCK: true=205 strict=205 reloc_blind=18 objdump-dr DIFFERS(277 rows) (307 rows)` |
| func_8001F13C | 1456 | synthvoice.c voiceAllocate | near miss (allocation of constants) | -O2 plain | `LOCK: true=6630 strict=6776 reloc_blind=252 objdump-dr DIFFERS(424 rows) (436 rows)` |
| func_80012D18 | 3148 | N64 per-voice command builder | near miss (structure right; variable residency) | -O2 plain | `LOCK: true=15620 strict=16995 reloc_blind=716 objdump-dr DIFFERS(674 rows) (810 rows)` |
| func_80023E9C | 2796 | synthmacros.c macHandle (114-way opcode switch) | not attempted | | needs owned jtbl slot 0x8002D964 / 0x1C8 (114 words) |
| func_8001A658 | 2812 | synth.c pre-job-queue voice handler | not attempted | | |

## Integration (coordinator)

1. **func_80012730** (no TU edit, no header patch, no owned data, nothing superseded):
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w21c/sources/func_80012730.c:func_80012730 \
  --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w21c/ctx71_O2.jsonl 0x11640 \
  --only func_80012730 --prefix-dedup rom/lib_11640 --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w21c/ctx71_O2.jsonl 0x11640 \
  --only func_80012730 --prefix-dedup rom/lib_11640
```
   Context row: `ctx71_O2.jsonl` (1 row, `ok`, `ido7.1`). Recompute: `python3 cloud/work/boot_tail_promotion/context_check.py
   --compiler ido7.1 OUT.jsonl func_80012730=cloud/work/frontier/w21c/sources/func_80012730.c`. The source repeats the
   TU's AudioCacheNode typedef and data externs verbatim (prefix dedup removes them); private DmaEntry_80012730 /
   Fetch_80012730 views. Its callers' prototype (func_80012D18 unmatched) is not declared in the TU.
2. **func_8001C860** needs two tooling steps before it can lock (deliver as-is, `ownrodata/func_8001C860.c`):
   a. owned .rodata slot at 0x8002D900, 4-byte logical extent (the TU's D_8002D904/908/90C follow it; word 0x3C888889),
      w20c's `owned_data prepare` recipe (like func_80023BDC's 0x8002D950 slot);
   b. lock flagset: retail has the R4300 `nop` between back-to-back `mul.s`, so the function scores 0 only with
      `-Wab,-r4300_mul` (true=495 without). The ROM-TU Makefile rule already passes `-Wab,-r4300_mul` to every TU
      (Makefile ROM-TU CFLAGS), so every locked lib_1cf90 function already compiles identically with it; but
      `layout._segment_flagset` requires one flagset per segment and promote_batch refuses a lock whose flagset differs
      from the segment's. Fix: treat `-Wab,-r4300_mul` like `-Xcpluscomm` in `_segment_flagset` (it is always on in the
      ROM-TU build), or relock lib_1cf90 with it. Then `lock add ... --flags "-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul"`.
   Disclosed in its header: the `if (n) {}` holder (shaping device; colouring of n/&p/vol/doppler), the cast macro EM
   (type view), private Vec_/Emitter_/Listener_ names (the TU's `Vector` typedef follows the slot).

## Identification (MusyX family; GameCube reference = doldecomp/ttyd libs/musyx, read via `gh api`)

| function | B | TU | MusyX name | evidence |
|---|---:|---|---|---|
| func_8001C860 | 1084 | lib_1cf90 | snd3d.c `CalcEmitter(em, vol, doppler, xPan, yPan, zPan)` | TU prototype (StateNode *, 5 x float *); 3 sqrtf calls; callees func_80024BF0 = salApplyMatrix, func_80024C9C = salNormalizeVector (lib_257f0, locked); listener list D_8004FD54; 1/60 frame step; callers 8001D1F4 / 8001DDE0 feed the result to func_8001CCDC (SetFXParameters) |
| func_800178B0 | 1160 | lib_17dc0 | seq.c `seqStartPlay(norm, drum, midiSetup, song, para, studio, sgid)` | callees func_8001785C = BuildTransTab (x2), func_8001B9F8 = synthVolume (x2), func_80017720 = DoPrgChange, func_80020610 = inpSetMidiCtrl (x4: volume/pan/reverb/chorus per channel), func_80019A60 = synthSetBpm |
| func_8001F13C | 1456 | lib_1f5b0 | synthvoice.c `voiceAllocate(priority, maxVoices, allocId, fxFlag)` | TU prototype `int (u8, u8, u16, u8)`; callers pass (prio, max, alloc id, key&128 ? 1 : 0) and use the returned voice index; leaf (inlined list helpers) |
| func_80023E9C | 2796 | lib_22300 | synthmacros.c `macHandleActive` / ExecuteCommands (macro interpreter) | jump table over the macro opcode (follows func_80023BDC's owned slot 0x8002D950); ~60 distinct callees are the mcmd* handlers (func_80022CD4 = DoSetPitch, func_80023BDC, func_80022160 ...) |
| func_8001A658 | 2812 | lib_1a660 | synth.c per-voice update of the pre-job-queue revision (the work later split into LowPrecision/ZeroOffset/EventHandler) | calls func_80023E9C (macro step) and 8 consecutive 0x20-byte getters func_80021428..80021528 (inpGet* family), hw setters func_800149BC/80014A0C/80014A74 (hwStart/hwSetPitch/hwSetVolume, as in the locked func_8001C580) |
| func_80012D18 | 3148 | lib_11640 | N64 hardware layer (no GameCube counterpart): per-voice RSP audio command builder | TU call `func_80012D18(&D_80038294[i], samples, active)` per AudioState; calls func_80011A64/80011D74/80012234/80012730 |
| func_80012730 | 1512 | lib_11640 | N64 hardware layer: voice sample-fetch/decode helper of func_80012D18 | called 4x from func_80012D18; calls func_80012660 and func_8002C5E0 (x3) |

## Running log

- func_8001C860 (CalcEmitter): **code identical** under `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`
  (`sources/func_8001C860.c`): `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (280 rows)`; the two
  rows are the `1.f / 60.f` literal's HI16/LO16 (`.rodata` vs `D_8002D900`). Levers: emitter = the parameter itself
  (a local copy takes a frame slot: p moves sp+0xA0 -> 0x9C), decl order panz/pany/panx/n (pan homes sp+0x80/0x84),
  `ft = 1.f / 60.f` literal (rematerialised in the loop preheader as retail; an extern load is not: true=4805),
  `if (n) {}` holder (vol/doppler/n/&p colouring s2..s5 -> retail's n=s2, &p=s3, vol=s4, doppler=s5; true 75 -> 0).
  Without -Wab,-r4300_mul: true=495 (retail has the R4300 nop between back-to-back mul.s).
- func_800178B0 (seqStartPlay): near miss, true 1655 -> **205** (`func_800178B0/best.c`). Levers found: packed Page
  view with a u16 macro field (retail's `lbu at / sll at,8 / or` is the assembler's ulhu expansion), `entry = page;
  entry += trans[j];` (base-first addu), `base = sum; addr = base;` for the master track, `nseq->bpm = song->info;`
  as its own statement before func_80019A60, `return` -1 after the found block. Residual (1 word + 1 load): retail
  stores nseq to a home slot sp+0x74 right after the found check and reloads it for the final `free = 0` store after
  func_800175B4 (a separate memory-resident web holding the same pointer as s3); every C form tried either merges it
  into s3 (copy propagation) or stores it from inside the search loop (`Qj`/`W` variants, true 300/560). 30 forms.
- func_8001F13C (voiceAllocate, MusyX <= 1.5.3 shape: block test before allocId, `voice = -1` at _skip_alloc,
  `_update` before the prio test): near miss, `func_8001F13C/best.c`
  `LOCK: true=6630 strict=6776 reloc_blind=252 objdump-dr DIFFERS(424 rows) (436 rows)` (first draft 8520). Control
  flow, loads/stores and the packed-record accesses (ulhu allocId at +0xBA, ulw flags/age) are retail's; the TU's
  packed VoiceState + a private packed view for allocId/block. Levers: `volatile` D_8004BE94 (as lib_113e0 declares
  it: lui/addiu/lbu 0), type_alloc as `(a >= b && b < c) ? 1 : 0` (retail's branch form). Residual = register
  allocation of constants: retail keeps the D_80050548 / D_80050648 bases in s2/s3 (and the voice record in s4,
  frame 56), ours rematerialises them per use (frame 48) and PRE's `&D_8004BEB8[voice]` into _update where retail
  multiplies again. Local types (27 combos), return/goto forms: no change. Next: a 7.1 uopt trace of constant
  register candidates (no 7.1 tracer exists).
- func_80012730 (N64 sample DMA cache fetch): **MATCH** (`sources/func_80012730.c`), from m2c in 6 rounds:
  `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (410 rows)`; spliced lib_11640 `IDENTICAL (4276 lines)`
  (`prove.log`). Levers (header): parameters are the running cursor/remaining, `end`/`low` locals at entry, decl order
  for retail homes, DMA entry length typed `int` (two separate 0x600 constants), `D_80038038 += 12;` as its own
  statement, tail-link store order, tag stored before address in the fresh-node path.
- func_8001C860 moved to `ownrodata/` (context_check refuses it: `.rodata:16 outside an owned slot`, as expected).

- func_80012D18 (per-voice RSP command builder, calls func_80012730 4x): first draft from m2c true=37990 -> **15620**
  (`func_80012D18/best.c`). Levers: frames `(unsigned int)(rate * 192 + 0xFFF) >> 12` (srl), loop end computed before
  `state = 2` (the address-taken state's store kills the loads), segment start/length/padded length as two-element
  arrays (retail keeps them memory-resident at sp+0x5C/0x54/0x44), the format dispatch as `switch (format) { case 3:
  ...; case 0: ... }` inside each flags branch (retail tests 0 then 3, lays case 3 first, and cross-jumps the shared
  ADPCM tail). Residual: retail keeps offset (t5), mode (t4), mainRem (t1) and loopRem (a0) in caller-saved
  registers with ugen saves around the fetch calls (homes sp+0x50/0x4C/0x3E/0x3C), ours keeps offset/rems in memory;
  frame top slots (sp+0x64/0x70) unexplained. Types tried (offset int/u32/short x rems u16/s16/int/u32): no gain.

## What generalises

- **m2c with jump tables for boot-tail asm**: `tools/m2c.sh SEG/FN` rewrites spimdisasm FPR ABI names to $fN and reads
  `jtbl_*` tables (bound from the preceding sltiu) out of baserom, adding labels. Good first drafts for 1-3 KB functions.
- **`lbu at,N(x); lbu t,N+1(x); sll at,at,8; or` is the assembler's `ulhu`**: a u16 field of a `#pragma pack(1)` view
  (800178B0 Page.macro, 8001F13C allocId). `lwl/lwr` pairs are packed u32 fields.
- **Address-taken locals' stores kill earlier loads under 7.1** (80012D18: `state` passed by address; compute values
  that retail loads before the store before the assignment).
- **Locals retail keeps memory-resident in adjacent slots are often arrays/structs** (80012D18's {start, count} pairs).
- **Two uses of one constant in a block share a register; a different C type splits them** (80012730: DMA length
  field `int` vs the unsigned `0x600 - offset` gives retail's two `li`).
- **A separate statement for a global increment** (`D_80038038 += 12; f(D_8003803C, D_80038038);`) gives retail's
  argument evaluation order; the in-argument `+=` form loads the other argument first.
- **`-Wab,-r4300_mul` matters for boot-tail functions with back-to-back mul.s** and is not in the 7.1 lock flagset
  (see Integration 2b). Probably affects any remaining float-heavy boot-tail function.
- **A float literal in the boot tail's global .rodata** is own data (owned slot), as w20c's jump tables; existing
  locked sources used `extern const float D_8002D9xx`, which only matches when the literal is not rematerialised.
- Retail sometimes keeps a pointer in a memory home and reloads it for one late use (800178B0's `nseq` at sp+0x74)
  where the same value is live in s3; no C form found (copy propagation merges every copy/CSE form; a def inside the
  search loop gives the home but adds a move). Candidate for a 7.1 uopt trace.

## Disclosures

- One permission denial: an `rm -rf` of a relative variant glob after `cd` (cleanup of a generated variant dir) was
  refused by the safety check; not retried; variant dirs were instead generated fresh and later moved under `*/v/`.
- MusyX GameCube sources (doldecomp/ttyd libs/musyx) read via `gh api` into the session scratchpad as reference only.
- Builder scratch `~/rush2049/scratch/frontier/w21c`: base copy + rsync of src/{blob,rom}, include, tools/{cloud,conveyor,
  asm-processor,workbench.py}, asm/us/{blob,boot_tail,nonmatchings/rom}, cloud/work/boot_tail_promotion, locks;
  `tools/ido-static-recomp/build71/out` symlinked to the 4586d230 toolkit's ido/; targets via w17k fetch_targets.py.
- Variant sweeps were small and hypothesis-driven (largest: 360 init-statement orders on 8001C860, 40 decl orders
  on 800178B0, 61 on 80012730).

## Files

`sources/func_80012730.c` (MATCH), `ctx71_O2.jsonl`, `prove.log`, `ownrodata/func_8001C860.c` (code identical),
`func_*/best.c` (near misses; func_8001C860/best.c = the ownrodata source without header), `tools/` (w20a's, renamed;
new: `m2c.sh`, `nd.sh`/`ndc.sh` non-branch diff rows per variant).
