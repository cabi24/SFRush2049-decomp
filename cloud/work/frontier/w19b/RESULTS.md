# w19b - static boot tail under IDO 7.1: the w18c not-matched list (wave 19)

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w19b`. Nothing locked, promoted,
spliced into src/, or committed; no edits to src/, asm/, include/, tools/, locks, rom_owned_data.json,
reloc_addrs.us.txt. Static numbers only. Toolkit 4586d230 (the 7.1 bundle `lock add --toolkit` uses), flagset
`-g0 -O2 -mips2 -G 0 -non_shared`, targets = reloc-aware inventory objects (w17k fetch_targets.py).

## Headline

| function | B | state | LOCK (check.py = lock add verify_entry) | whole TU |
|---|---:|---|---|---|
| func_800217E4 | 96 | MATCH (`sources/`) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (27 rows)` | lib_22300 IDENTICAL |
| func_80021764 | 128 | MATCH (`sources/`, type view) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (35 rows)` | lib_22300 IDENTICAL |
| func_80021844 | 136 | MATCH (`sources/`) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (37 rows)` | lib_22300 IDENTICAL |
| func_80023190 | 140 | MATCH (`sources/`) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (35 rows)` | lib_22300 IDENTICAL |
| func_80015720 | 440 | code identical, needs reloc_addrs | current target `LOCK: true=20 strict=20 reloc_blind=0 objdump-dr DIFFERS(6 rows) (128 rows)`; re-split target `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (128 rows)` | lib_16320 vs re-split TU IDENTICAL |
| func_800158D8 | 308 | same | `true=20 ... DIFFERS(6 rows) (91 rows)` / re-split `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (91 rows)` | same |
| func_80015C0C | 348 | same | `true=20 ... DIFFERS(6 rows) (101 rows)` / re-split `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (101 rows)` | same |
| func_80015F28 | 308 | same | `true=20 ... DIFFERS(6 rows) (91 rows)` / re-split `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (91 rows)` | same |
| func_800161A0 | 268 | same | `true=10 ... DIFFERS(3 rows) (80 rows)` / re-split `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (80 rows)` | same |
| func_8001661C | 256 | same | `true=10 ... DIFFERS(3 rows) (77 rows)` / re-split `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (77 rows)` | same |
| func_80021548 | 96 | code identical, needs owned .rodata (`owned/`) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (26 rows)` (the 2 rows = jtbl HI16/LO16 symbol `.rodata` vs `jtbl_8002D930_main`) | - |
| func_800177EC, 80017824 | 56+56 | near miss, scheduling only | `LOCK: true=120 strict=120 reloc_blind=3 objdump-dr DIFFERS(4 rows) (16 rows)` | |
| func_80016E68, 80016F80 | 120/152 | near miss | best 225 (w18c p5) | |
| func_800154A4, 8001536C | 208/312 | near miss | 50 (w18c g1) | |
| func_800230B0 | 224 | near miss, frame | `true=120 strict=162` | |
| func_8001DC08 | 472 | not reworked (time) | w18c 140 | |

Matched now: 4 functions, 500 B (sources/, ready for lock + promote_batch). Plus 1,928 B (six) that land with
reloc_addrs entries (data + one tooling fix), and 96 B with an owned-rodata slot. Recount with `make progress`.

Full logs: `prove.log` (sources/: ctx rows, LOCK, TU), `prove_relocname.log` (relocname/: LOCK vs current and
re-split targets, TU vs re-split passthrough asm). The `TU: COMPILE FAIL ... redeclaration` lines are check.py
appending the preamble without promote_batch's prefix dedup (same as w17g/w18c); the spliced TUs
(`tools/splice.py` = promote_batch splice with `--prefix-dedup`) compile and are identical:
```
== all sources spliced (into the current TUs)
lib_22300: IDENTICAL (3382 lines: .text words+relocs, data sections)
== six spliced vs re-split passthrough TU
lib_16320: IDENTICAL (1701 lines: .text words+relocs, data sections)
```

## IDO 7.1 idioms established (what generalises)

1. **Copy-then-narrow** (`move a1,v0; andi a1,a1,0xff`): a narrow local assigned a MASKED expression of a load that
   uopt CSEs with an earlier test, and NO intermediate local: `index = voice->id60 & 0xFF;` after
   `voice->id60 != 0xFFFFFFFFU`. With `u32 id = voice->id60; ... index = id;` 7.1 substitutes `andi v1,v0` and sinks
   it into one arm. Closed 217E4/21764/21844 at once. (Sibling func_80017D38 shows the same shape:
   `or a3,s2,zero; andi a3,a3,0x7f`.)
2. **Narrowed variable in a coloured register** (s8 gain, `srl a2,v0,8; sll a2,a2,24; sra a2,a2,24`): same root
   cause. Do not hold the command word in a local; write `command->word[0]` each time (one `lw v0`, CSE'd) and
   `gain = command->word[0] >> 8;` into an `s8`. A `parameter` local turns it into ring temps (w18c true=160).
3. **Clamp with a redundant branch** (`bnez at,L; move v0,v1; b L; lui v0,0x7f; L:`): one nested conditional
   expression `v = v < 0 ? 0 : (v > MAX ? MAX : v);`; every if/else form loses the `b`.
4. **Switch returning a mapped value with one shared tail** (`jr ra; move v0,a0`): every case assigns the
   parameter and `break`s to a single `return controller;` (returns inside cases: true=60..165).
5. **u8 parameter "reload from home" is not one cause**: 7.1 narrows a single-use u8 parameter in its own
   register at entry (`sw a0,24(sp); andi a3,a0,0xff`) when the parameter is used once; it reloads `lbu 27(sp)`
   when (a) the parameter is used twice across a call, (b) it is an `int` parameter cast `(u8)` at the use, or
   (c) it is `s8` (probes p/a.c, p/f.c on the builder). w18c's 177EC/17824 residual is NOT a reload: see below.
6. **Relocation-name-only = data, not source**: when the split label sits on a compiler-made address (loop end
   pointer `&a[count-1]`, base-8/-12, or a base+4 field the compiler folded), no C names it. The project mechanism
   already exists: `reloc_addrs.us.txt` + `tools/conveyor/pipeline/reloc_split.py derive/check` (WAVE4). It had one
   gap: `derive` prints negative addends as `addend:0x-8`, which `check` and splat (`int(v, 0)`) reject.

## Integration (coordinator; order matters)

### A. The four matches (sources/, ctx71_O2.jsonl), lib_22300 only
No TU edits, no header patch, no owned data, no unit_overrides.
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in func_800217E4 func_80021764 func_80021844 func_80023190; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w19b/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w19b/ctx71_O2.jsonl 0x22300 \
  --only func_800217E4,func_80021764,func_80021844,func_80023190 --prefix-dedup rom/lib_22300 --dry-run
# then without --dry-run; ROM gate (make + make test)
```
`--prefix-dedup` is required (the sources repeat the TU's VoiceState / MacroCommand / MacroState_800223D0
typedefs verbatim; dedup drops them). func_80021764 takes `struct MacroState *` (its source declares the tag
first) so lib_22300's later `extern int func_80021764(MacroState *)` (func_800225FC block) stays compatible;
the body reads through `((VoiceState *)state)->...` casts (a `VoiceState *` local costs code: true=60).

### B. Relocation-name six (relocname/, ctx71_relocname.jsonl), lib_16320
1. Apply `reloc_split_negative_addend.diff` (tools/conveyor/pipeline/reloc_split.py one-line format fix +
   a test in tests/conveyor/test_reloc_split.py; `git apply --check` clean; the new test fails without the
   fix and the file's 9 tests pass with it, the current 8 pass either way).
2. Append `reloc_addrs_w19b.txt` (20 lines, produced by `reloc_split derive <fn> --candidate <7.1 LOCK object>`)
   to `reloc_addrs.us.txt`; `python3 -m tools.conveyor.pipeline.reloc_split check` -> on the Pi with the
   appended file: `all 78 reloc_addrs entries encode their ROM words`.
3. Re-split (`make extract`, regenerates the GLOBAL_ASM slots; expected operands e.g.
   `%hi(D_80042230 - 0x8)`, `%lo(D_8003C618 + 0x4)`), ROM gate, then refresh the six inventory targets
   (as for func_8001144C in 7c49d8ff). Emulated here: `tools/resplit_target.py` (targets.assemble_region +
   gate_target on the region with the entries applied: all six `gate_target OK`) and `tools/resplit_asm.py`.
   Not run: splat itself (no repo edits); spimdisasm renders negative addends as `sym - 0xN`, which is what
   the emulation assembled.
4. Lock + promote like A with `relocname/` and `ctx71_relocname.jsonl`, segment of lib_16320 (0x16320),
   `--prefix-dedup rom/lib_16320`. Typedefs were aligned with lib_16320: 8001661C uses the TU's own
   Descriptor/RecordFields/RecordStorage (func_800164D0 block) verbatim; 800161A0 `#include
   "boot_tail_sample_contract.h"` (the TU's header); the four others use per-table names
   RecordFields_8003C618 (15720 + 158D8, identical statements: 158D8's `payload` made `void *` like 15720's,
   unused by 158D8's code), RecordFields_8003CE20 (15C0C), RecordFields_80038610 (15F28).

### C. func_80021548 (owned/func_80021548.c) - for w19c / the coordinator
The 7.1 object's `.rodata` is exactly the jump table: size 0x20, align 16, 6 words relocated against .text
(+0x2C..+0x54) + 8 zero bytes; lib_21210 has no other .rodata/.data/.bss today (TU built on the builder). Retail
bytes at data.bin 0x6160..0x617F = `80021574 8002157c 80021584 8002158c 80021594 8002159c 00000000 00000000`
(jtbl_8002D930_main at 0x8002D930; no overlap with the 5 existing rom_slots). Proposed `rom_slots` entry:
```
{"owner": "func_80021548", "tu": "src/rom/lib_21210.c", "source": "assets/us/data.bin",
 "container_section": ".data", "owner_section": ".rodata", "offset": "0x6160", "size": "0x20",
 "container_vram": "0x800277d0",
 "sha256": "626e96a8b7ad0e519ca7a368ddf278d6fd0e693412113f6e7e61eb1c7fe845de",
 "passthrough_asm": "asm/us/nonmatchings/rom/lib_21210/func_80021548.table.s",
 "composed_source": "build/us/assets/us/data.composed.bin"}
```
Companion `func_80021548.table.s` (as osSpTaskLoad_full.table.s): `.section .rodata` / `.balign 4` /
`glabel jtbl_8002D930_main` / `.incbin "assets/us/data.bin", 0x6160, 0x20`. `context_check.py` refuses the row
(`func_80021548 refused object has data/rodata (.rodata:32)`), so it is a manual splice behind the gate (like
osAiSetNextBuffer in w17g). Its LOCK needs the reloc-aware target to name the table by section or the
owned-data scoring path; not run here (w19c owns the tooling).

## Near misses (stop rule reached)

- **func_800177EC / func_80017824** (56+56): w18c's h5.c (`func_80020610(130, value, D_8004BE7B, channel)`,
  u8 params) already narrows both params in registers; the only residual is the schedule: retail
  `andi a3; lui a2; andi a1,a1; lbu a2; jal; li a0` vs ours `andi a3; lui a2; lbu a2; li a0; jal; andi a1`.
  Traced: ugen (`cc -S`) emits `and $7; and $5; li $4; lbu $6`; feeding as1 every dependency-respecting order
  of those 6 instructions (bf.py, 35+19+40+26 orders) never yields retail; a basic-block boundary after the
  entry narrowing gets `li a0` into the delay slot but moves `sw a1/andi a1` above `sw ra`. So retail's ugen
  input had a different dependency set, not a different order. Tried ~40 C forms (locals, casts, register,
  K&R, unprototyped/int callee, s8/int params, empty blocks, flags -O1/-O3/-mips1/-g1/-Wab,-r4300_mul): none
  moves it. Note D_8004BE7B is the low byte of D_8004BE78 (u32 in sequence_context.h): `(u8)D_8004BE78` gives
  the same code with relocation D_8004BE78+3 (would also need a reloc_addrs entry). Next: as1's scheduler
  heuristics (priority on `andi` with src==dst?), or an inlined-helper shape.
- **func_80016E68 / func_80016F80**: retail stores the result with `lui at; sw v0,%lo(D_80042678)(at)` in the
  branch delay slot, then re-reads it with a separate `lui t8; lw t8` in the next block. Non-volatile: the
  re-read is copy-propagated and the store's address materialised (`lui a0; addiu a0; sw v0,0(a0)`);
  volatile (whole, store-only, load-only casts): the address is materialised in v1 and shared (225..240).
  A struct view of 0x80042670..7B (key/found fields of one search state passed by address) and -O1/-O0: worse.
  Next: find what blocks address CSE across the two blocks without volatile.
- **func_800154A4 / func_8001536C**: `addu a0,s0,tN` order. Tried pointer-local running form (E64 lever: frame
  grows, 400), sum orders, `&((u8 *)group)[...]`, `u8 *group` with casts, `(u32)` laundering, no-code identities
  on either side (`^0`, `*1`, `>>0`, `|0` on the pointer: worse, `+0`: same): 50 throughout. Next: trace why
  E64's parameter base goes first but a coloured local does not (ctrace on the canonicalisation).
- **func_800230B0**: frame 40 vs 32; ours already spills `time` to 0x1C like retail. Declared types
  (u16/s32 time, s32/s16 offset), masks, a `target`/`delta` local, chained assignment, a pointer to
  panTimeA8: frame never moves (probe p/fr.c: 1-3 values live across a call all stay at 24). Next: the
  two-slot object is likely address-taken or 8-byte (look for a struct/double temp in the arcade ancestor).
- **func_8001DC08**: not reworked this wave (time).

## Files

`sources/` (4 matches), `ctx71_O2.jsonl`; `relocname/` (6) + `ctx71_relocname.jsonl` + `reloc_addrs_w19b.txt` +
`reloc_split_negative_addend.diff`; `owned/func_80021548.c` (`ctx71_owned.jsonl` = its "refused" row, see C);
`prove.log`, `prove_relocname.log`; `tools/`: `check.py`/`sc.sh`/`dif.sh`/`rdiff.sh`/`tusplice.sh` (= w18c),
`run.sh` and `splice.py` (w18c's, parametrised by `S`/`CTX`), `prove.sh`, `prove_relocname.sh`,
`resplit_target.py` (Pi: re-split targets), `resplit_asm.py` (scratch asm copy only). Builder scratch:
`~/rush2049/scratch/frontier/w19b` (base copy + rsync of src, include, tools/{cloud,conveyor,asm-processor},
asm, boot_tail_promotion scripts; `tgt/` = inventory targets + `*_rs.o`; probes in `p/`, variants in `v/`).

## Disclosures

- Builder: w18c's `dif.sh` and check.py create and remove temp dirs under the builder's `/tmp` (mktemp /
  tempfile), as in w18c. `prove_relocname.sh` rewrites my scratch's `asm/` copy temporarily and restores it
  (verified: no `D_80042230 - ` left). Nothing outside `~/rush2049/scratch/frontier/w19b` otherwise.
- Pi: scratchpad holds targets, re-split objects and a symlinked tree copy used to test the reloc_split patch
  (tools/ and tests/ copied, not edited in place). DB read-only (fetch_targets.py, reloc_split derive).
- CLAIMS.md not edited (static boot tail row is "open"). No permission denials.
- Devices: func_80021764's `struct MacroState *` parameter + casts (type view, no code). No unused locals, no
  pad arrays, no implicit int, no compiled-out blocks in any delivered source.
