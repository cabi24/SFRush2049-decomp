# w22b - static boot tail: func_80023E9C (macro interpreter, owned jtbl) + func_80022874 (owned .bss) + func_8001A658

Read-only except this lane dir and builder scratch `watchman2:~/rush2049/scratch/frontier/w22b` (base copy + rsync of
src/{blob,rom}, include, tools/{cloud,conveyor,asm-processor,workbench.py}, asm/us/{blob,boot_tail,nonmatchings/rom},
cloud/work/boot_tail_promotion, locks, symbol files, baserom, assets/us/data.bin; `tools/ido-static-recomp/build71/out`
symlinked to the 4586d230 toolkit's ido/). Targets = reloc-aware inventory objects (w17k fetch_targets.py). Nothing
locked, promoted, spliced into src/, or committed. Static numbers only. IDO 7.1, `-g0 -O2 -mips2 -G 0 -non_shared`.

## Headline

| function | B | TU | state | flags | scorer line (check.py = lock add verify_entry) |
|---|---:|---|---|---|---|
| func_80023E9C | 2796 | lib_22300 | **code identical**, needs owned .rodata (co-owned with func_80023BDC) + owned .bss | -g0 -O2 -mips2 -G 0 -non_shared | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(127 rows) (896 rows)` |
| func_80022874 | 460 | lib_22300 | **code identical**, owned .bss co-owner (lands only with func_80023E9C) | same | `LOCK: true=0 strict=0 reloc_blind=6 objdump-dr DIFFERS(33 rows) (135 rows)` |
| func_8001A658 | 2812 | lib_1a660 | near miss (register allocation) | same | `LOCK: true=7725 strict=7983 reloc_blind=270 objdump-dr DIFFERS(364 rows) (749 rows)` |

Static by inventory sizes after the slot tooling: +2 functions / +3,256 B (lib_22300 then has no GLOBAL_ASM left
except func_800233B0). Linked lib_22300 with both spliced: .text sha1 72e105bf... == baserom, .rodata/.data == ROM.

## func_80023E9C + func_80022874 (lib_22300, 2,796 + 460 = 3,256 B): code identical, need owned .rodata + .bss

| function | B | state | scorer line (check.py = lock add verify_entry) |
|---|---:|---|---|
| func_80023E9C | 2796 | code identical (owned .rodata + .bss) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(127 rows) (896 rows)` |
| func_80022874 | 460 | code identical (owned .bss, co-owner) | `LOCK: true=0 strict=0 reloc_blind=6 objdump-dr DIFFERS(33 rows) (135 rows)` |

Differing rows are relocation symbols/in-place addends only (`tools/rows.sh`, `prove.log`): func_80023E9C 61+61
`D_80056200 -> .bss`, 1+1 `D_80056228 -> .bss`, 1+1 `jtbl_8002D964_main -> .rodata` (the w20c case), 1 in-place addend
word (`addiu s0,s0,0` vs `+8`: cstep sits at .bss+8 in the standalone object); func_80022874 22 `D_800562xx -> .bss` +
11 in-place addends (its record is at .bss+8 behind DebugMacroSteps). No C can produce the splat label names.

**Linked whole-TU proof** (`prove.log`, `tools/prove.sh`): both sources spliced into a copy of the current
src/rom/lib_22300.c (prefix-dedup semantics), built as the ROM TU (IDO 7.1, asm-processor, Makefile CFLAGS), linked
with .text 0x80021700, .data 0x8002D460 (func_80022CD4's slot), .rodata 0x8002D950, .bss 0x80056200:
```
spl: .text 13552 B, sha1 72e105bf4429fbbcc73ef9697f8ff0ebbf03a859; .bss .bss NOBITS 80056200 016200 000030
rom:  .text 13552 B, sha1 72e105bf4429fbbcc73ef9697f8ff0ebbf03a859
linked spliced == baserom 0x22300..+13552: IDENTICAL
linked .rodata 0x1e0 B == data.bin[0x6180:] (0x8002D950): True
linked .data 0x20 B == data.bin[0x5c90:] (0x8002D460): True
80056200 b debugMacroSteps_80056200 / 80056208 b sample_80056208 / 80056228 b cstep_80056228
```
Context rows (`context_check.py --compiler ido7.1`, verbatim; refused until the slots exist, as expected):
```
func_80022874 refused ido7.1 object has data/rodata (.bss:48) outside an owned slot of func_80022874
func_80023E9C refused ido7.1 object has data/rodata (.rodata:464, .bss:16) outside an owned slot of func_80023E9C
```

### Source (sources/func_80023E9C.c, from m2c via w21c tools/m2c.sh: true 855 -> 95 -> 35 -> 10 -> 0, 4 rounds, 31 variants)
MusyX synthmacros.c macHandleActive of the pre-1.5 revision (per-voice wait/time handling still inside it).
Levers (header): the 0x40 test as `if (!(cFlags & 0x40)) { init } else { free; return; }`; `default: ex = 0;` (no
`ex = 0;` before the switch); as1 store order: `field68 = 0;` after the revVol stores, `waitTimeA0 = timeA4;` before
`wait9C = 0;` in the key-off block, trap clear chained `trapAddr1C = trapCurAddr20 = 0;`. Handlers declared by
their lib_22300 struct tags with the TU's parameter types and called through casts (type views). No devices, no
unused locals, no pad arrays, no implicit int.

### The .bss order (why func_80022874's source declares DebugMacroSteps)
lib_22300's whole retail .bss is 0x30 at 0x80056200: D_80056200 (u8 DebugMacroSteps, synthmacros.c file scope),
D_80056208.. (func_80022874's 28-byte sample record, 8-aligned), D_80056228 (func_80023E9C's static cstep). IDO 7.1
allocates .bss in definition order; with `static u8 debugMacroSteps_80056200;` only in func_80023E9C's preamble
(after func_80022874 in the TU) the record lands at 0x80056200 and the counter at 0x8005621C (measured). Declared
in func_80022874's preamble (the TU position of synthmacros.c's file-scope declaration relative to these two),
the layout is retail's. func_80023E9C repeats the line (tentative definition, deduplicated by the splice). So the two
land together, as one owned .bss block. func_80022874's source = w21b `ownbss/func_80022874.c` + that line.

### Proposed rom_owned_data rows (data/)
- `data/owned_lib_22300_bss.json`: .bss block, owner func_80023E9C, co-owner func_80022874, 0x80056200, size 0x30
  (w21d's slot kind). No overlap with storage_blocks (vi_manager/timer/sdk) or lib_16320's block. D_80056200..
  D_80056228 are referenced only by these two functions; they stay absolute in undefined_syms_auto.
- `data/owned_lib_22300_rodata.json`: SUPERSEDES func_80023BDC's row: owner func_80023BDC, co-owner func_80023E9C,
  offset 0x6180, size 0x1E0 (both tables + IDO's 4-byte zero pad, ends 16-aligned at 0x8002DB30 where the next
  data starts), sha256 8840692c..., no logical extent needed once both are C. The prepared (passthrough) state needs
  a companion carrying func_80023E9C's table after func_80023BDC's C table (tooling: lane w22c / coordinator).

### Integration (coordinator / w22c; order matters)
1. owned_data: register `data/owned_lib_22300_bss.json` (`python3 -m tools.conveyor.pipeline.owned_data prepare ...`,
   w21d kind) and replace func_80023BDC's .rodata row by `data/owned_lib_22300_rodata.json` (co-owned 0x1E0 slot; its
   prepared state needs a companion with func_80023E9C's passthrough table words after func_80023BDC's C table, or
   prepare + promote in one transaction). Builder gate `make COMPILER=ido && make test` -> ROM matches.
2. Locks (7.1 toolkit):
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in func_80022874 func_80023E9C; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w22b/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
```
3. Context rows once the slots exist (now refused, verbatim above):
   `python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 cloud/work/frontier/w22b/ctx71_w22b.jsonl
   func_80022874=cloud/work/frontier/w22b/sources/func_80022874.c func_80023E9C=cloud/work/frontier/w22b/sources/func_80023E9C.c`
4. promote_batch, ONE unit (they co-own the .bss block; func_80022874's preamble carries DebugMacroSteps):
```
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w22b/ctx71_w22b.jsonl 0x22300 \
  --only func_80022874,func_80023E9C --prefix-dedup rom/lib_22300 --dry-run      # then without --dry-run
```
5. No TU edit, no header patch, no reloc_addrs, nothing superseded (w21b's ownbss/func_80022874.c is replaced by
   sources/func_80022874.c = same body + the DebugMacroSteps line). D_80056200..D_80056228 / jtbl_8002D964 stay
   in undefined_syms_auto / the splat asm (only these two functions use them). Packet verifiers that assume
   lib_22300 has no .bss / only the 0x14 table (macro_stream_contracts, macro_wrapper_contracts) will need the
   w20c/w21d treatment (admit registered owned slots).
6. Recompute the proof: `tools/prove.sh` from the builder scratch root (needs w21b's check.py/sc.sh, copied here,
   `lane/` = this dir, baserom + assets/us/data.bin in the scratch root).

## func_8001A658 (lib_1a660, 2,812 B): near miss, `func_8001A658/best.c`

`LOCK: true=7725 strict=7983 reloc_blind=270 objdump-dr DIFFERS(364 rows) (749 rows)` (m2c draft: true=24405; mnemonic
diff 243 -> 84 lines). Identification (w21c) confirmed against doldecomp/ttyd synth.c: synthHandle's voice loop of
the pre-job-queue MusyX (LowPrecision/ZeroOffset/EventHandler work inline: LFOs via sndSin func_8001E7BC, vibrato,
sweeps, pedal/portamento controllers, hwStart/hwKeyOff, age, pan ramp, pitch via apply_portamento func_80019AD4 +
convert_cents func_8001A5D8 + doppler -> hwSetPitch, volume envelope, master-fader/track/input volume, pan, surround,
reverb -> hwSetVolume), then the 32 master faders (HandleFaderTermination inline: seqStop/seqPause/seqMute =
func_80018D40/80018C2C/800190AC) and hwFrameDone (func_80014D14).
Levers found (each from the diff): the fader view is NOT packed (aligned lw/sw) while the voice view is; the fader
loop as an index loop `for (j = 0; j < 32; j++) smf = &D_8004F300[j]` (LFTR end pointer in s3, retail's
`D_8004F800` label = `D_8004F300 + 0x500` -> needs a reloc_addrs entry like func_8001B9F8's); `time >> 8` signed;
the volume as three statements (pauseVolume, volume, fx fader); `cpitch = convert(...) + sweepOff[0] + sweepOff[1]`
in one expression; clamps `x = x < 0 ? 0 : (u32)x > 0x7F0000U ? 0x7F0000U : x` for the pan/env ramps (unsigned, `at`
rematerialised as retail) and signed `> 0x7F0000` for pan/span/aux; `aux` assigned 0 only in an else arm (its
range no longer crosses the getter call: 0x7F0001 then gets s8); `midi = sv->midi` once at the top of the volume
block; 16 declared locals so span/vrange/midi sit at retail's homes (-32/-48/-64; modulation/portamento are real
getter-result locals).
**Residual (traced to one cause):** register allocation in the volume/pan block. Retail's nine s-registers are vol,
pan, sv, &D_8004BE90, 2, 255, i, 0x7F0001, 0x7F0000; midi lives as a split range (a0 at the load, a1 in both
D_8004F2F8 arms, spilled to its home sp+0x48 around each inpGet call), vrange in a1 (sp+0x58), span in a3 (sp+0x68).
Ours gives midi s3 and rematerialises 0x7F0000 (`lui`), so the frame stays 120 (retail 136 = deepest spilled home
-64) and every later register name shifts. Not variable placement/type (u8/s32/u32, 5 positions tried). Next: a 7.1
uopt priority trace of midi vs the 0x7F0000 constant (w22c's instrumented 7.1 build) to see which use/block count
flips it; candidate levers are the number of `midi != 0xFF` tests (retail may test sv->midi directly in one arm)
and the aux/span clamp forms. Integration needs (for when it closes): TU patch moving lib_1a660's `ChannelState`
typedef + `extern ChannelState D_8004F300[32];` above the func_8001A658 slot (the source declares them verbatim), the
D_8004F800 reloc_addrs entry, no owned data.

## What generalises
- **IDO 7.1 allocates a TU's .bss in definition order, file-scope statics included**: a file-scope static that
  retail places before a function-local static of an EARLIER function must be declared in that earlier function's
  source preamble (the original file declared it at the top). Measure with a linked TU (`tools/linkproof22.sh`).
- **A 114-way MusyX dispatcher from m2c in 4 rounds**: m2c (w21c tools/m2c.sh) + a generator for the case list
  (handlers declared by their TU struct tags with the TU's parameter types: `struct Tag;` forward declarations keep
  prototypes compatible with the TU's typedef'd definitions without copying the bodies). Then only control-flow
  placement (`if (!(f & 0x40)) {...} else {free; return;}`, `default: ex = 0;`) and as1 store order were left.
- **as1 store order levers (7.1)**: a chained assignment `a = b = 0;` reorders the swr halves of two packed u32
  stores; moving one store statement past three others moved its sb by exactly that.
- **A value assigned on both arms of an `if` around a call keeps its range off the call**: `aux = 0;` before
  `if (...) { call; aux = ...; }` made aux callee-saved; `else aux = 0;` let a constant win the s-register.
- **Retail spill homes give the declaration order**: under the w21b slot model (local k at -4k), retail's spill
  offsets (span -32, vrange -48, midi -64) fix the positions of those locals among 16.

## Disclosures
- No unused locals, no pad arrays, no false prototypes, no implicit int, no compiled-out devices in the delivered
  sources. Type views: SynthVoice_80023E9C (private packed), handler struct tags, the TU's MacroState/MacroCommand
  verbatim; func_80022874 as w21b's. func_8001A658/best.c (not a match) uses private SynthVoice_/SynthFader_/
  SynthLfo_8001A658 views and the TU's ChannelState lines.
- The DebugMacroSteps declaration sits in func_80022874's source although only func_80023E9C uses it (explained
  above; it is a tentative definition repeated in func_80023E9C's source).
- Builder: only `~/rush2049/scratch/frontier/w22b` (plus mktemp under the builder /tmp by check.py/dif.sh as earlier
  lanes); `tools/ido-static-recomp/build71/out` there is a symlink to the 4586d230 toolkit's ido/. Pi: target objects
  fetched with w17k fetch_targets.py into the session scratchpad (read-only DB/blob reads); MusyX GameCube sources
  (doldecomp/ttyd synthmacros.c, synth.c, synth.h, sal.h) fetched with `gh api` into the scratchpad, reference only.
- No permission denials. `workbench diagnose` was not run on func_8001A658 (the residual was traced from the diff
  to the colouring decision directly; it is a 7.1 function and the diagnose oracle is 5.3-oriented).

## Files
`sources/func_80023E9C.c`, `sources/func_80022874.c`, `data/owned_lib_22300_{bss,rodata}.json`, `prove.log`,
`func_8001A658/best.c` (near miss), `func_*/m2c.c` (drafts), `tools/` (`prove.sh`, `rows.sh` row classes,
`ops.sh`/`vbo.sh` mnemonic diff, `gen23e9c.py` switch generator, `linkproof22.sh`, `ownbss_splice.py`, w21b's
`check.py`/`sc.sh`/`dif.sh`/`md.sh`/`mdlocals.py`, `t.sh`/`vb.sh`). Variants under `func_*/v/` (ignored).
