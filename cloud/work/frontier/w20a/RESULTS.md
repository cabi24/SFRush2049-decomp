# w20a - static boot tail, w19a near-miss list under IDO 7.1 (wave 20)


Targets (in order): func_8001824C, func_80018448, func_80010E80, func_800252AC, func_80011A64, func_80011D74,
func_80017D38, func_80018634, func_80022CD4. Flags `-g0 -O2 -mips2 -G 0 -non_shared`, IDO 7.1 toolkit 4586d230.
Builder scratch: watchman2:~/rush2049/scratch/frontier/w20a (base copy + rsync of src/{rom,blob}, include,
tools/{cloud,conveyor,asm-processor,workbench.py}, third_party/n64-decomp-workbench, asm/us/{blob,boot_tail,
nonmatchings/rom}, locks; `tgt/` = inventory targets via w17k fetch_targets.py). Tools copied from w19a (paths renamed).

Note: the shared trace toolkit (`tools/trace`) is built on traced IDO 5.3 binaries only; there is no traced 7.1.
For 7.1 this lane uses `cc -S` (ugen output, before as1 scheduling) vs the scheduled object.

## Headline

**2 functions, 1,380 B** production-ready in `sources/` (IDO 7.1, `-g0 -O2 -mips2 -G 0 -non_shared`), both
`LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical` against the reloc-aware inventory target (toolkit 4586d230)
and TU-check identical; both spliced together (promote_batch `--prefix-dedup` replica) into lib_17dc0 build
byte-identical to the current TU (`prove.log`). Both are in segment 0x17dc0 (lib_17dc0). No TU edit, no header patch.
Static by inventory sizes: 550/668 + 2 -> 552/668 (+1,380 B; recount with `make progress`).

| function | B | state | cat | LOCK (check.py = lock add verify_entry) | whole TU |
|---|---:|---|---|---|---|
| func_8001824C | 508 | MATCH | B+C | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (133 rows)` | lib_17dc0 IDENTICAL (2598) |
| func_80017D38 | 872 | MATCH | B+C+U | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (235 rows)` | lib_17dc0 IDENTICAL (2598) |
| func_80018448 | 492 | 380 -> 325 | | `LOCK: true=325 strict=325 reloc_blind=30 objdump-dr DIFFERS(27 rows) (129 rows)` (`func_80018448/best.c`) | |
| func_80018634 | 840 | 805 -> 780, frame fixed | | `LOCK: true=780 strict=780 reloc_blind=173 objdump-dr DIFFERS(114 rows) (215 rows)` (`func_80018634/best.c`) | |
| func_80022CD4 | 592 | 980 -> 660 | | `LOCK: true=660 strict=660 reloc_blind=20 objdump-dr DIFFERS(125 rows) (152 rows)` (`func_80022CD4/best.c`) | |
| func_80010E80 | 500 | 460 (unchanged) | | `LOCK: true=460 strict=502 reloc_blind=50 objdump-dr DIFFERS(51 rows) (139 rows)` | |
| func_80011D74 | 492 | 420 (unchanged) | | `LOCK: true=420 strict=420 reloc_blind=10 objdump-dr DIFFERS(113 rows) (124 rows)` | |
| func_80011A64 | 440 | 775 (unchanged) | | `LOCK: true=775 strict=775 reloc_blind=13 objdump-dr DIFFERS(28 rows) (111 rows)` | |
| func_800252AC | 552 | 960 (unchanged) | | `LOCK: true=960 strict=960 reloc_blind=32 objdump-dr DIFFERS(50 rows) (166 rows)` | |

Categories as w19a: B type views (private views reached by casts), C shaping device (mask), U unused locals (exact
frame residual, disclosed with arithmetic in the header).

## Integration (coordinator)

1. **Locks** (7.1 toolkit; nothing superseded, neither function was locked):
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in func_80017D38 func_8001824C; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w20a/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
```
2. **Context rows**: `cloud/work/frontier/w20a/ctx71_O2.jsonl` (2 rows, both `ok`, `"compiler": "ido7.1"`, computed on the
   builder against master 81d91115). Recompute: `python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1
   OUT.jsonl func_80017D38=cloud/work/frontier/w20a/sources/func_80017D38.c func_8001824C=cloud/work/frontier/w20a/sources/func_8001824C.c`
3. **promote_batch** (dry run first; `--prefix-dedup` required):
```
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w20a/ctx71_O2.jsonl 0x17dc0 \
  --only func_80017D38,func_8001824C --prefix-dedup rom/lib_17dc0 --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w20a/ctx71_O2.jsonl 0x17dc0 \
  --only func_80017D38,func_8001824C --prefix-dedup rom/lib_17dc0
```
4. No unit_overrides (static), no data-symbol changes, no owned data, no TU edit, no header patch. Both files include
   `sequence_context.h` and read SequenceTrack +0x10..+0x27 (and the context's +0x110/+0xF80/+0xFC8 ranges) through
   private views; a header patch is NOT needed. If the owner prefers real names over views, the names are (offsets in
   the 0x28-byte SequenceTrack): +0x08 event_time (u32), +0x0C events (u8 *, header active0C), +0x10 pitch cursor
   (u8 *, header active10), +0x14 modulation cursor (u8 *, header active14), +0x18 pitch value (u16), +0x1A modulation
   value (u16), +0x1C pitch due time (u32), +0x20 modulation due time (u32), +0x24 channel (u8), +0x25 velocity offset
   (s8), +0x26 transpose (s8), +0x27 group number (u8). Changing active0C/10/14 to pointers would need func_800180A0's
   `||` test re-proved (pointer and int tests compile alike, untested).
5. `D_8004BE7B` (func_80017D38) is the low byte of D_8004BE78 under its own relocation symbol (the target relocates to
   D_8004BE7B); declared `extern unsigned char D_8004BE7B;` in the source (relocation-label view as w18c).

## Shaping devices, views and unused locals (all in the file headers)

- func_8001824C: `(first & 255) << 8` in the 15-bit delta (C). `first` is already unsigned char; under 7.1 ugen copies
  it into a fresh temp for the and (`and $13,$3,255`), the object keeps no instruction of it, and the temp ring advances
  by one - exactly retail's one-register shift from that point (w19a residual true=80 -> 0). Private 0x28-byte track
  view `SequenceTrackNotes` reached by casting the header's `tracks568` (B).
- func_80017D38: `number & 255` for func_8001A270's u16 group argument (C; retail `andi t2,s3,0xff`); the context read
  through a private view `SequenceContextEvents` cast once (macro CTX) (B); the node clock pair copied as one 8-byte
  struct through `SequenceTime_17D38` views (two field stores give true=405) (B); five unused int locals = exact frame
  residual (U): retail frame 120 (`active` sp+102, `advance` sp+88), without them 104 (`advance` sp+92); four before
  `active`, one between `active` and `advance`, 5 slots, 20 bytes (two doubles + one int is also exact; disclosed).
- No pad arrays, no false prototypes, no implicit declarations, no compiled-out blocks.

## What generalises (IDO 7.1, boot tail)

- **`cc -S` works as a 7.1 ugen trace.** The listing is ugen's output before as1; comparing it with the object shows
  which temps as1 deleted (8001824C's and-255), how the temp ring advances and which instructions as1 moved.
  `tools/S.sh SRC FN` (reduced TU, `FULL=1` for the whole file). There is no traced 7.1 uopt/ugen/as1 yet.
- **A no-op mask on a u8 is a "hidden temp".** `x & 255` (or `(u16)(x & 255)`, two temps) on an unsigned char costs no
  instruction but advances ugen's temp ring; retail temp-register offsets of one or two from a point on can be this.
- **Unused and dead-store locals take stack slots under 7.1** (measured on 80017D38): a retail frame larger than ours
  with matching relative offsets is declared-but-dead locals; position in the declaration list sets the slot.
- **7.1's temp ring excludes registers coloured anywhere in the function** (80018634: retail never uses t0 as a temp
  because a coloured web got t0; ours uses t0 as a temp). A temp-ring shift from the very first temp is a colouring
  difference elsewhere, not a temp-order one.
- **Operand order of address sums follows integer arithmetic**: retail `x + (u32)base` (80018634 patterns/pattern/
  optional offsets: `sequence->patterns + (u32)sequence`), and `delta + event_time` (80017D38 `due`).
- **One variable for two values** (80018448: part-1 delta time and the value delta share t0 = one s16 variable).
- **as1 decisions are the residual class left** (80011D74 delay slot vs likely slot, 800252AC/80011A64 hoisting from
  successor blocks, 80018448 which successor's cursor step is hoisted). Reassembling an edited `-S` listing
  (`tools/asm.sh`) is NOT faithful in general (the binary ugen->as1 path carries more information: moves dropped,
  `bge`/`li.d` pairs differ), so it can only localise; 80011D74's first block was reproduced exactly by a label after
  the loop_length load.

## MusyX reference used

`doldecomp/ttyd` libs/musyx (GameCube MusyX decomp, read via `gh api`, not vendored): `seq.c` GetStreamValue (the
controller-stream format of 8001824C/80018448: 0x80,0x00 end; 15-bit/7-bit delta time; `v <<= 1; v >>= 1` /
`v <<= 9; v >>= 9` sign extension in the pre-1.5.4 version) and `synthmacros.c` DoSetPitch (80022CD4: `static u16
kf[13]` pitch table, D_8002D476 = &kf[11]; the N64 version's else path differs from the GC one).

## Running log

- func_8001824C: **MATCH** (see table). Lever: `(first & 255) << 8` in the 15-bit delta (7.1 copy-then-narrow:
  ugen emits `and $13,$3,255` into a fresh temp, as1 deletes it (byte-known value), so the temp ring advances by one).
  `cc -S` (ugen, pre-as1) showed the deleted temp directly. Types: sequence_context.h + private 0x28-byte track view.
- func_80018448: 380 -> 325 natural (`best.c`: ONE s16 variable for the part-1 delta time and the value delta;
  retail keeps both in t0 = one variable; this fixes the a0/t0 swap). Remaining 325 = temp-register ring only
  (no colour/opcode diffs). 290 with unnatural mask chains (`best_masks290.c`, NOT proposed). Retail's join block
  (`lw t8; lhu t7; addu t9; lw t3`) is impossible under a plain ring, so the remaining difference is ugen
  temp allocation state (free-list order), not something one more hidden temp fixes.
- func_80010E80: stays 460 (w19a best). Traced with `cc -S`: in ours uopt splits `count` (piece in v1 to the `* 2`,
  ugen caller-save spill temp 36 around the D_80038018 jalr, the `length` store reads count's memory home 48);
  retail keeps ONE v1 web for X = duration*rate/1000 and count across all three calls (ugen saves it once at 44 and
  reloads after the calls), with no separate home slot. One variable for X and count (`count = X; count = 192 -
  count % 192 + count;`) gives count s1 (callee-saved, frame 56, true=755); one variable for X, count and the loop
  index gives frame 48 but s0 (true=1590). Decl order (48 permutations) is irrelevant. Next: needs a 7.1 uopt
  priority trace (no 7.1 tracer exists); hypothesis: retail count's benefit is just under the callee-save cost, so
  7.1 gives it a caller-saved register with ugen spills instead of a split.
- func_80011D74: stays 420 (D10). Traced with `cc -S` + reassembly (`tools/asm.sh`, as0+as1 7.1 on an edited
  listing): as1 ignores instruction order inside the first block; inserting a LABEL between `lw $24,56($4)`
  (loop_length) and `beq $24,0` reproduces retail's first block exactly (mul.d stays before the branch, lw hoisted,
  `beqzl` with the else path's `lw v0,48(a0)` in the likely slot) - `listing/d74_lbl2.s`. So retail's test of
  loop_length starts a new basic block after the load. C forms tried (goto, do/while(0), nested if, compiled-out
  `if (loop_length) {}`, 7 spellings of the increment): none moved it. (Reassembly is not fully faithful for
  `bge; li.d` pairs, so only the first block is evidence.)
- func_80017D38: **MATCH** (see table). From D10 (715): `due = delta + event_time` operand order, `channel, active`
  declared before the ints, `number & 255` at the func_8001A270 call -> true=0 strict=94; five unused ints fix the
  frame (strict 0); TU port: context view + 8-byte clock copy (two field stores: 405).
- func_80018634: 805 -> 780, frame now exact (four unused ints declared first: 72 -> 88, `active` stays sp+66;
  `best.c`, NOT a match, unused locals would need disclosure). Levers: integer address sums (`sequence->patterns +
  (u32)sequence` etc.), `sum >>= 16` as in the TU's func_800180A0. Residual is allocation-only (workbench: 0 opcodes,
  114 register rows). Traced cause: retail colours four webs into a1/a2/a3/t0 (patterns, pattern, event code,
  optional04) and the jump value into v1, so t0 is never a temp; ours colours one web fewer into the argument
  registers (code a2, pattern a1, optional04 a3, patterns v1) and uses t0 as a temp from the first while-test on.
  `code`/`jump`/`offset` locals are copy-propagated away (no change). Next: needs a 7.1 colouring trace.
- func_80022CD4: 980 -> 660 (`best.c`). The table scan as `i = 11; cursor = &D_8002D476; for (;;) { if (f > *cursor)
  break; i--; cursor -= 2; }` gives retail's i=v1/cursor=a3 and loop shape (720); cursor before i in the else copy
  (660). Left: in the `ofrq < frq` copy uopt pulls the post-loop note/detune code into the loop's break path (loop
  step out of line), retail keeps the step inline (`bnezl` to the exit); plus `move t0,zero` delay-slot fills.
  MusyX DoSetPitch (ttyd decomp) confirms the algorithm and the kf[13] table (D_8002D476 = &kf[11]).
- func_800252AC, func_80011A64: unchanged (as1 hoisting from successor blocks: 252AC hoists the target block's
  `lbu option` where retail hoists the fall-through's `lui`; 11A64 retail hoists `mtc1 step` from the join block and
  keeps 65536.0f in $f8, ours keeps it in place and as1 renames to $f14). Listing reassembly is not faithful there.

## Disclosures

- Builder scratch `~/rush2049/scratch/frontier/w20a` (base copy + rsync from the Pi tree at master 81d91115; IDO 7.1 via a
  `tools/ido-static-recomp/build71/out` symlink to the 4586d230 toolkit's ido/). Targets fetched with
  w17k/fetch_targets.py into the session scratchpad and copied to `tgt/`.
- MusyX GameCube sources read from GitHub (doldecomp/ttyd, via `gh api`) into the session scratchpad as reference
  only; nothing vendored or copied into sources.
- Variant sweeps were small and hypothesis-driven (largest: 120 mask forms on 80018448 to count hidden temps, 48
  declaration orders on 80010E80). No permission denials. Nothing committed, locked, promoted or spliced.

## Files

`sources/` (2), `ctx71_O2.jsonl`, `prove.log`, `func_*/best.c` (near misses; `best0.c` = starting source),
`func_80018448/best_masks290.c` (unnatural, not proposed), `func_80011D74/listing/d74_lbl2.s` (edited listing that
reproduces retail's first block). Tools: `tools/S.sh` (7.1 `cc -S`), `tools/asm.sh`/`asmvs.sh` (reassemble a listing,
diff vs target / vs direct compile), `tools/gen.py` (choice-point expander, ` ## ` separator), `tools/vb.sh`,
`tools/regs.sh` (print selected instructions per variant), `tools/prove.sh`, plus w19a's check.py/splice.py/
tusplice.sh/sc.sh/dif.sh/ct.sh (paths renamed).
