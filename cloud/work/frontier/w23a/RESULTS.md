# w23a — static boot tail (IDO 7.1, trace71): func_80018634, func_80018448, func_80012D18

Builder scratch `watchman2:~/rush2049/scratch/frontier/w23a` (base copy + rsync of src/{blob,rom}, include,
tools/{cloud,conveyor,asm-processor,workbench.py}, asm/us/{blob,boot_tail,nonmatchings/rom},
cloud/work/boot_tail_promotion, the three lock files; `tgt/` = w17k fetch_targets.py; `lane` -> this dir;
`tools/ido-static-recomp/build71/out` -> the 4586d230 toolkit's ido/). trace71 installed with
`install71.sh ~/rush2049/scratch/frontier/w23a --reuse ~/rush2049/scratch/frontier/w22c` (all 6 binary hashes OK).
Nothing committed, locked, promoted or spliced. 2 cores, own scratch only.

## Headline

| function | B | state | scorer line |
|---|---:|---|---|
| func_80018634 | 840 | **MATCH** | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (215 rows)` (same with `-Wab,-r4300_mul`); TU (with ctx) `true=0 ... identical (215 rows) RESULT PASS`; spliced `lib_17dc0: IDENTICAL (2598 lines: .text words+relocs, data sections)` |
| func_80018448 | 492 | not closed (26 rows, unchanged) | obj71: `rows: want 124 got 124; differing rows 26` (w21a best.c) |
| func_80012D18 | 3148 | not closed (unchanged, w22c best.c) | obj71: `rows: want 788 got 781; differing rows 578` |

Static by inventory: +1 function, +840 B once integrated (recount with `make progress`).

## func_80018634 — MATCH (`sources/func_80018634.c`)

Trace: w22c's force71 oracle needs patterns a1 / pattern a2 / code a3 / optional04 t0, i.e. v1 forbidden in the
default arm (bb14). CUPLOG units: the jump web (w40, v1) lives only in bb10 (the 0xFFFE arm), patterns (w61) only in
bb14, so nothing with v1 interferes. Lever: **one variable for the 0xFFFE jump index and the default arm's pattern
number** (`index = event->value.jump;` ... `index = event->code; pattern = patterns[index] + sequence`). IDO 7.1 makes
one web per C variable, so the v1 web now spans bb14 and every default-arm web shifts exactly as retail; uopt still
reads the CSE'd `event->code` register (a3) for the index, so the default-arm copy costs no instruction.
- `u16 index` gives 18 rows (7.1 narrows the copy: `andi v1,a3,0xffff`); `u32`/`int` give 0.
- Rewritten against the TU's `sequence_context.h` (SequenceContext, D_8004BE80, disabledFC5, valueFC6, half120,
  fraction118, whole11C) with suffixed private views: `SequenceContextHead_80018634` (pointer field at +0x10C),
  `SequenceEventTrack_80018634` over `unknown128` (+0x128, 64 x 0x10), `SequencePlayback_80018634` over `tracks568`.
  The header pointer must be a struct-field view: a cast of `unknown004 + 0x108` swaps the operands of the two
  `optional0x + header` adds (2 rows; source order does not fix it).
- **Unused locals (disclosed, exact frame residual):** `int u1, u2, u3, u4;` (from w21a). Retail frame 88; without
  them 72 (2 rows: the sp adjusts only); 4 slots of 4 bytes. Natural alternatives tried: a `u8 *channels` local
  (46 rows). No other devices, no pad arrays, no false prototypes (func_80018B3C is `void (unsigned int)` as defined
  later in lib_17dc0; func_80017720 is declared exactly as the TU's two existing externs).
- No libultra names referenced (static_rename not needed). No header patch, no data, no TU edit.

### Integration (coordinator)
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w23a/sources/func_80018634.c:func_80018634 \
  --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w23a/ctx71_O2.jsonl 0x17dc0 \
  --only func_80018634 --prefix-dedup rom/lib_17dc0 --dry-run      # then without --dry-run
```
LOCK lines are identical with and without `-Wab,-r4300_mul`, so the default lock flagset works. Context row:
`ctx71_O2.jsonl` (1 row, `ok`, `--compiler ido7.1`), recompute with
`python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl func_80018634=cloud/work/frontier/w23a/sources/func_80018634.c`.
Full proof: `sh lane/tools/prove.sh` on the builder -> `prove.log` (context row, check.py LOCK/TU with and without
`-Wab`, splice.py + tusplice.sh: lib_17dc0 IDENTICAL).

## func_80018448 — not closed (~45 min)

All colours already match (w22c). Retail's temp sequence in the second part, by source line of the best.c:
46 t2, 47 t3 t4, 48 t5, 50 t6, 51 t2, 54 t5 (+2, hoisted by as1), 57 t6 (+1), 59 t8 t7 t9, 60 t3, 61 t2. Ours (ugt71,
pure FIFO, verified event by event): t6 / t7 t8 / t9 / t2 / t3 / t4 / t5 / t6 t7 t8 / t9 / t2. **No FIFO pop sequence
produces retail's 50-57 run (t6 t2 t5 t6) unless t3/t4/t7/t8/t9 are held (allocated, not freed) across lines 47-57**,
and then the 59 run (t8 t7 t9 at the queue head) is again impossible without a reset at the join. So retail's ugen
input differs in more than extra code-free pops: a temp held across statements, or a different emission order of
the two arms. The first part is consistent with two code-free pops between line 36 (t3) and line 37 (retail t6 t7).
Tried (40 variants, `v/g1`, all 26 or 36 rows): narrowing casts/masks on line 37 and line 40 (`(u8)`, `& 255`,
`(u16)`, `(s16)`); the matched sibling func_8001824C's source adapted to the second stream (58 rows).
Next: find a source form whose ugen output keeps a value live in a temp across lines 47-57 (e.g. a multi-use
expression ugen does not re-evaluate), checked with `SCHED=1 ugt71.sh`; the matched sibling's shaping device
(`(first & 255) << 8`) shows 7.1 does produce code-free pops here.

## func_80012D18 — not closed (~30 min), w22c best.c unchanged

spill71 on w22c's best: cfe area 68 (frames -4, end -8, state -9, address -20, length -28, offset -32, mode -36,
padded -44, cmd -48, mainRem -50, loopRem -52, pos -64, a cfe temp -68) + 4 expression homes (temp0 w39/153/182/184,
temp1 w40/197, temp2 **w348 = CVT `(unsigned int)pos`, v0, bb6-19**, temp3 **w358 = `frames - end + (u32)pos`, a1**)
= 84, frame 120. Retail (w22c offsets: state -13, address -28 ... loopRem -60) = area 76 (pos -72, temp -76) + 2 homes
= 84. The two extra retail locals sit at -12 and -20, and the two homes retail lacks are exactly temps 2 and 3 —
consistent with **w348 and w358 being named u32 variables in retail** (`position`, `excess`). Probe
(`probe_named_position.c`): declaring them there gives area 76 like retail, but uopt copy-propagates both
single-definition variables back into expressions, so the homes remain (frame 128, 582 rows); assigning `position`
once at the top changes nothing either. Next: a form in which those two variables survive copy propagation (more
than one reaching definition at a use, e.g. assigned on both paths and read after a merge), then ctrace71 for the
upstream colours (frames a1 vs a2, cmd home).

## What generalises
- **7.1 one-web-per-variable as a lever**: when force71 says a register must be forbidden in a block where no
  value of the needed register lives, reuse the variable that owns that register (here the jump index) for a value
  of the block (the pattern number). The copy disappears into the CSE'd expression and the web's live range grows.
  Keep the variable's type wide enough that 7.1 does not narrow the copy (`u32`, not `u16`).
- Private views of unnamed header ranges: use a pointer-field struct view, not a cast of a byte-array member plus an
  offset; the latter is an "expression" operand for uopt's commutative ordering (w22f) and swaps adds.
- A ring residual that no FIFO pop sequence can produce (same register twice in a row with other temps free) is not
  a missing code-free pop: look for held temps or a different emission order instead.

Permission denials: none (one `./install71.sh` not executable; ran it with `sh`).
