# w25f — static near-misses (IDO 7.1, trace71): voiceAllocate MATCH; func_80018448 improved (first half closed)

Builder scratch `watchman2:~/rush2049/scratch/frontier/w25f`: a copy of `base`, rsync of src/{blob,rom}, include,
tools/{cloud,conveyor,asm-processor,workbench.py}, asm/us/{blob,boot_tail,nonmatchings/rom}, the locks/symbol files,
cloud/work/boot_tail_promotion and cloud/work/frontier/tools; `tgt/` and `t71/` copied from w25b's scratch (read only
there); `tools/ido-static-recomp/build71/out` -> the 4586d230 toolkit's ido (as in w24c's scratch). Nothing committed,
locked, promoted or spliced in the repo. No permission denials except one `rm` of a relative glob in my own lane dir
(not retried; nothing needed deleting).

| function | bytes | before | after | scorer line |
|---|---|---|---|---|
| func_8001F13C voiceAllocate | 1456 (364 insns) | true=605, 96 rows (w24c best) | **MATCH** | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (436 rows)`, `TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (436 rows) RESULT PASS`, spliced `lib_1f5b0: IDENTICAL (1152 lines: .text words+relocs, data sections)` |
| func_80018448 | 492 | `LOCK: true=325 strict=325 reloc_blind=30 objdump-dr DIFFERS(27 rows) (129 rows)` (w23a best) | near miss, strict improvement | `LOCK: true=310 strict=310 reloc_blind=24 objdump-dr DIFFERS(23 rows) (129 rows)` (`func_80018448/best.c`) |

Flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` (lock flagset `-g0 -O2 -mips2 -G 0 -non_shared`), toolkit 4586d230.

## func_8001F13C voiceAllocate: the code-free pop is a CSE-breaking label, not a narrowing

w25b localised the residual to one extra code-free ugen temp pop between the L79 maxVoices reload and the L81 `addu`.
The suggested `& 0xFF` lever (w25c's byte-store narrowing) does not exist here, and the ucode experiments show why:

- **ucode oracles (`tools/uedit.py` + w25b's `urun.sh`)**. `uedit.py` splices records into uopt's `r.O` (copies of
  existing records; round-trip byte identical). A `cvtl 16` on `p` before the L81 `ixa` (eA) gives retail's ring
  everywhere (49-50 rows = only the visible `andi t7,t3,0xffff` plus branch offsets), but as1 keeps the andi.
  Every narrowing placement in the L79-L81 window (22 variants, `v/k/bf`) leaves its andi: **as1 deletes a
  redundant `andi` only when the zero-extending load is in the same block/extended block** (eJ: andi right after
  the latch `lhu` is deleted; eA/eI/K1: andi after a join is kept). A `cvtl` directly on an `lbu`/`lhu` result
  costs no temp at all (eH: ugen folds it, no ALLOC event).
- **The real mechanism (L1/L2, 0 differing rows)**: an unreferenced `lab` record inserted after the L79 `fjp`.
  The label ends ugen's tree CSE, so the L80 maxVoices reload is not merged with L79's (no MOVE_END, usage 2);
  ugen re-emits it as `lw $15,28($sp)` (the extra pop) and **as1 deletes the redundant reload** and renames its
  use to t6. Same result when only the L80 reload's dtype differs (M1/M2: `lod` Ldt instead of Jdt).
- **C spelling**: casts on maxVoices at L79/L80 are all folded by uopt (r6, 16 variants). An empty statement that
  leaves a uopt label does it: `do {} while (0);` or `if (1) {}` at the top of the `if (num < maxVoices)` arm
  (w2/w10: ring fixed, 45 rows). The remaining 45 rows were a num/voice swap (v1/a0): the extra block changes the
  p1 priorities from num 414/11 = 37.6 > voice 832/23 = 36.2 to num 414/12 = 34.50 < voice 832/24 = 34.67
  (`tk71.sh cdx`, webs 42/45). An extra block elsewhere in voice's range raises calleecost and loses s2/s3
  (352 rows, y*/z*). Writing the first search loop without `continue` (e1:
  `if (ALLOC(i).block == 0 && allocId == ALLOC(i).allocId) { ++num; ... }`) restores the order: **0 rows**.
- Without the empty statement, e1 alone is 104 rows (= best), so both levers are needed.

Disclosed shaping device: `do {} while (0);` (an empty statement, as a compiled-out debug macro would leave),
documented in the source header. No unused locals, pad arrays, implicit int or false prototypes.

### Integration (coordinator; order matters)

1. **TU edit** (layout-neutral): `python3 cloud/work/frontier/w25f/tools/tu_edits.py src/rom` from the repo root
   hoists the two lines after the func_8001F13C slot (`typedef struct ChannelLink {...} ChannelLink;` and
   `extern ChannelLink D_80050440[32];`, func_8001F6EC's context) above the slot. Without it the spliced TU fails
   with `redeclaration of 'ChannelLink'`. prove.log: `== TU edit alone` `lib_1f5b0: IDENTICAL (1152 lines ...)`.
2. **Lock**: `python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w25f/sources/func_8001F13C.c:func_8001F13C --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit 4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557`
   (the source's first line carries `-Wab,-r4300_mul`, as w24c's best did; it was not locked before).
3. **Context row**: `cloud/work/frontier/w25f/ctx71_O2.jsonl` (1 row, `ok`, `ido7.1`; computed against the
   unedited tree; prefix-dedup then drops the hoisted lines and the TU-prefix typedefs).
4. **promote_batch** (dry run first):
```
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w25f/ctx71_O2.jsonl 0x1f5b0 --only func_8001F13C --prefix-dedup rom/lib_1f5b0 --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w25f/ctx71_O2.jsonl 0x1f5b0 --only func_8001F13C --prefix-dedup rom/lib_1f5b0
```
5. No unit_overrides (static), no data-symbol changes, no owned data, nothing superseded.

`prove.log` = `tools/prove.sh` on the builder (context row, TU edit alone, LOCK + TU check with prefix-dedup, all
sources spliced into the edited TU and compared with the current TU). `tools/check.py` gained the prefix-dedup of
the ctx preamble in TU mode (promote_batch `--prefix-dedup` replica), otherwise identical to w25b's copy.

## func_80018448: first half closed, second half is not a single pop (about 1.3 h, stopped)

- w23a: retail needs two code-free pops between line 36 (`andi t3`) and line 37 (retail `andi t6; sll t7`).
- **Label scan (36 ucode variants, `v/k/lab`)**: an unreferenced `lab` at every statement boundary in lines 28-61
  changes nothing. There are no ugen CSE merges here, so the voiceAllocate mechanism does not apply.
- **Narrowing scan (52 variants, `v/k/nar`)**: a `cvtl` Jdt 8 on line 37's `lod first` (n106j) takes 26 rows to 24,
  and rows 1-52 then match. ugen allocates `and $t,$3,255` and as1 deletes it. Here as1 does know that v1 is
  zero-extended across the join at line 36, because both predecessors come from the lbu block.
- **C spelling**: `(first & 255) & 127`, `(u8)(first & 127)`, `(u8)first & 127`, `& 0xFF`, `% 128` and the other
  folds (c1-c8, f1-f7, d1-d7) all fold away in uopt or change the code. Only `((u8)(first & 255) & 127) << 8`
  (f4) keeps the cvtl, and it reproduces n106j exactly. Disclosed in the header as a shaping device.
- **Second half (lines 46-61, 23 objdump rows, all register names)**. Retail's temps by source line are
  L46 t2 / L47 t3 t4 / L48 t5 / L50 t6 / L51 t2 / L54 t5 / L57 t6 / L59 t8 t7 t9. FIFO can only produce this if
  t7-t9 are held, or popped hidden, between L50 and L51, plus two more between L51 and L54. No single
  narrowing (8/16-bit, Jdt/Ldt) or label inserted after any lod/ilod/arith record in lines 43-61 changes the
  24-row result (99 ucode variants, `v/k9/s1`). Sibling-style spellings on top of f4 (h1-h9: `& 255` on line 51,
  on `second`, and `(s8)(first & 255)` on line 56) give 28-36 rows.
- **Best next hypothesis**: ugen saves and restores its register state across the if/else arms (L50/L57) and the
  join label before L59. Retail's L59 order t8 t7 t9 is not FIFO-reachable without a reset. Trace ugen's handling of
  `ujp`/`lab` (where the free list is saved or restored) before trying more C.

## What generalises

- **A second code-free pop mechanism: a CSE-breaking label plus as1 redundant-load deletion.** ugen merges equal
  loads across statements (MOVE_END, usage count 2) until a label. A label between them makes ugen re-emit the
  load into a fresh temp, and as1 then deletes the redundant `lw` (renaming its uses) within the extended block.
  The result is one ugen temp pop and no code. In C, an empty statement that uopt keeps as a label does this
  (`do {} while (0);` or `if (1) {}`). Casts on the reloaded value are folded by uopt and do not.
- **as1's redundant-narrowing deletion is dataflow-limited.** It works when the zero-extending lbu/lhu is in the
  same block, or in single-entry predecessors (it survives a join whose predecessors all come from that block). It
  fails across a loop header with a back edge (voiceAllocate eA/K1 keep the andi).
- **uopt folds and-chains.** `(x & 255) & 127` becomes `& 127`. `(u8)(x & 255) & 127` keeps the cvtl. If a
  narrowing has to survive next to another mask, put a cast between the two masks.
- **An empty block costs priority.** It adds one to `nocs` for every web live across it, so it can flip a
  near-tie (num 37.6 > voice 36.2 became 34.50 < 34.67). It also raises calleecost: two extra blocks cost
  voiceAllocate its s2/s3 (352 rows). Removing a block elsewhere in both webs' ranges restored the order (no
  `continue`).
- **Tool**: `tools/uedit.py IN.O OUT.O ins:AFTER:SRC[:const] del:IDX dt:IDX:DTYPE` splices copies of existing
  records into uopt's binary ucode (round-trip byte-identical). Use it with w25b's `urun.sh` to test a ucode
  hypothesis in a minute before searching for C.

## Files

- `sources/func_8001F13C.c`: the match, with disclosures. Identical copy at `func_8001F13C/best.c`.
  `ctx71_O2.jsonl` and `prove.log`.
- `func_80018448/best.c`: the improved near miss (f4).
- `tools/`: w25b's set (paths changed to w25f), plus `uedit.py`, `tu_edits.py` and a new `prove.sh` (TU edit,
  then splice into the edited TU). `check.py` now does prefix-dedup in TU mode.
- Variants and ucode experiments are under `*/v/` (git-ignored).
