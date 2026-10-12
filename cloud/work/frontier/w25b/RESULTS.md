# w25b — static boot tail (IDO 7.1, trace71): voiceAllocate, synthHandle, func_80018448, func_80012D18

Builder scratch `watchman2:~/rush2049/scratch/frontier/w25b`: a copy of `base`, then rsync of src/{blob,rom}, include,
tools/{cloud,conveyor,asm-processor,workbench.py}, asm/us/{blob,boot_tail,nonmatchings/rom},
cloud/work/boot_tail_promotion, the three locks and the symbol files. `tgt/` and `t71/` (the traced passes and w24c's
PRE-oracle uopt, `t71/pre/uopt` sha256 8df3879c…) were copied from w24c's scratch, which was only read. 2 cores.
Nothing was committed, locked, promoted or spliced. No permission denials.

## Headline

No match, and no strict improvement on the scorer for any of the four functions. For each one this wave narrows the
cause to a specific mechanism, and two new tools make the next step a single experiment.

| function | before (w24c/w23a) | after (w25b) | scorer line |
|---|---|---|---|
| func_8001F13C voiceAllocate | true=605, 96 rows | unchanged (w24c best.c) | `LOCK: true=605 strict=605 reloc_blind=132 objdump-dr DIFFERS(96 rows) (436 rows)` (re-run here) |
| func_8001A658 synthHandle | true=2770 | unchanged (w23b best.c). x1 has fewer rows but scores worse (below) | best `LOCK: true=2770 strict=2770 reloc_blind=83 objdump-dr DIFFERS(319 rows) (749 rows)`; x1 `LOCK: true=2970 strict=2970 reloc_blind=66 objdump-dr DIFFERS(182 rows) (749 rows)` |
| func_80018448 | 26 rows | unchanged (w23a best.c) | obj71 `rows: want 124 got 124; differing rows 26` |
| func_80012D18 | 578 rows | unchanged (w22c/w23a best.c) | obj71 `rows: want 788 got 781; differing rows 578` |

Flags are `-g0 -O2 -mips2 -G 0 -non_shared`, with `-Wab,-r4300_mul` for voiceAllocate, on toolkit 4586d230. There is
no sources/, ctx71 jsonl, prove.log or promote_batch line because nothing matched.

## New tools (`tools/`)

- **`fifo_search.py UGT K MAP [W]`.** This replays a ugt71 trace as ugen's FIFO free list. A FREE counts as a real
  return only if it is the last FREE of that register before its next ALLOC; this stands in for usage counts. It
  checks that the model reproduces the traced pops (`model ok`). It then searches every placement of one extra
  code-free pop, held from event a to event b, and every move of one FREE event. It reports the placements whose pop
  sequence equals retail's, given as ours up to alloc K and renamed by MAP after it.
- **`udump.py` and `urun.sh`.** `cc -K` keeps uopt's output `r.O` (plus `r.T`). `udump.py` prints that binary ucode
  record by record with source lines, using the vendored workbench parser, which reads 7.1 streams unchanged.
  `urun.sh DIR FILE.O FN` runs the traced ugen and as1 on an edited `.O` and diffs the rows. On the unedited stream
  it is byte-identical to `cc`. With these, any hypothesis about ucode can be tested directly through ugen.
- `vb71.sh FN FILE...` batch-scores variants with plain 7.1 obj71 rows. The other scripts are w24c's, with the paths
  changed to w25b.

## func_8001F13C voiceAllocate: one code-free pop between the L79 reload and the L81 address (about 2 h)

- **All 96 rows are a pure register rename.** Applying σ = {t7→t9, t9→t8, t8→t6, t6→t7} to our object from row 177
  to the end reproduces retail exactly, with zero rows left. The diff -y "schedule" differences at rows 303-351 are
  alignment artefacts.
- **uopt's output, decoded.** The count-loop guard (L79) and the rotated L80 top test each carry their own
  `vreg -28; lod -28` (the maxVoices home). ugen merges them into one tree node with usage count 2 (`f_eval` →
  `f_move_to_end_gp_list` = the MOVE_END event), so t6 is reused and not reloaded.
- **The FIFO search finds only two explanations for the whole function** (`fifo_search.py ugt 24
  't7:t9,t9:t8,t8:t6,t6:t7'`, 4 + 1 hits):
  - (a) One extra temp pop at L80/L81. Either it happens while L79's t6 is still held and is freed after t6 returns,
    or it happens after t6 returns and before L81's `addu` allocation.
  - (b) The first loop's latch address `&D_800504C8[i]` (t7, L62) stays held until L81.

  No other single extra pop, delay or advance of a free anywhere in the function gives retail's sequence.
- **Mechanism for a code-free pop, found.** The pop must cost no instruction. The matched sibling func_8001824C shows
  how one arises. ugen emits `and $13,$3,255` for `(first & 255)`, where `first` is a u8 register loaded by `lbu`.
  That allocates a temp, and **as1 then deletes the redundant `andi`**. The ugen listing has the instruction but the
  object does not. The boot-tail TUs show no other code-free allocation path: for t6-t9, temp definitions are never
  fewer than ALLOC_GP_RESULT events.

  Specific-register takes (`f_get_one_reg`, for example t9 for `jalr`) are not traced and reorder the ring too.
  voiceAllocate has none.
- **Oracle showing where the pop belongs** (`func_8001F13C/c7_held_pop.c`, oracle only). With
  `if (num < (maxVoices & 255))` at L79, the andi temp is held across the L80 test, and L81's `addu` becomes retail's
  t9. The andi survives here, because maxVoices is a word reload that as1 cannot prove zero-extended. Also, t6 is
  then freed after t7, the reverse of what retail needs, so L83 onwards is off again. Retail needs a temp popped in
  that window whose instruction as1 deletes, and which is freed after t6.
- **Variants tried, none closing the window (all under `v/`).**
  - r1: count-loop shapes, operand order, a do-while guard, a break form, and `& 0xFF` / `& 0xFFFF` on the index,
    `i` and `p`. In a05, `D_800504C8[i & 0xFF].next` realigns the ring from L88 on, at the cost of a visible andi.
  - r3: masks at L79, L80 and L81.
  - r2: the real MusyX `voiceAllocate` loop bodies, from synthvoice.c in the w21c scratch copy. The nested
    `if (!type_alloc || …) { if (cFlags & 2) continue; …}` form gives 135 rows: it swaps num and voice (v1/a0, a
    priority tie) and leaves the ring unchanged.
- **Best next hypothesis.** The missing pop is a no-op narrowing that ugen materialises and as1 deletes. It sits on a
  value that as1 can see is zero-extended, inside L80-L81. The candidates are `p` (u16, last set by `lhu` in the L76
  latch, so as1 must see across blocks) and the `lbu` result of `D_80050548[p]`. Test it at ucode level with
  `urun.sh`: insert an `and 255` (or `cvt`) record on L81's `ilod` or L80's `lod reg11` in `r.O`. If retail's rows
  come out, search for the C spelling of that record. uopt folds `& 255` on a u8 *load* (c3), so the C must apply it
  to a register variable, as the sibling does with `first`.

## func_8001A658 synthHandle: the store-PRE halves separated, and three equivalent midi oracles (about 1.3 h)

- **First half of span, solved naturally.** w24c's x1 is `span = cpitch; func_80014A0C(i, span); span = 0;` (span
  reused for the call argument). It removes the span hoist entirely: 61 obj rows against best's 80, saved as
  `func_8001A658/x1_span_reuse.c`. On the scorer, though, it is true=2970 against 2770 (two fewer instructions, and
  the insert/delete penalties). It is therefore not a strict improvement, and best.c stays w23b's.
- **Second half.** x1 still deletes the `span = 0` in the `D_8004F2F8 == 0` / midi == 0xFF arm, because that store is
  redundant after node 84. Variant n3 (`span = sv->fxFlag4C ? 22 : 21` used as the fader index) shows that any span
  store between node 84 and that arm keeps it, exactly as in retail. n3 breaks the `!= 0` arm, so it is only a
  probe. Retail's kill must therefore lie on the `== 0` path only, before the midi test.
- **midi.** On top of x1, three PRE oracles give the same 58 rows, with INSERT 93 and DELETE 99 and 115:
  - `P71_PRESET=avlocs:75@84,76@84` (w24c's);
  - `avlocs@92`, the load available at the end of the envelope block;
  - `P71_PRECLR=alters/absalters:75,76@83`, meaning the hwSetPitch call (node 83) does not kill `sv->midi4A`.

  So retail's ucode needs `sv->midi4A` partially available at the volume block. Any one of those three facts is
  enough.
- **Compare order.** Retail compares `beq s5,X`, with the constant first, which is what our code produces for a *load*
  compared against 0xFF (the cpitch test at row 395 matches). Our midi *variable* compares come out `beq X,s5`. Seven
  no-code identities on the aux test (`* 1`, `^ 0`, `>> 0`, `| 0`, `+ 0`, casts) do not flip it; `(u32)` changes the
  constant to `li at,255`. This points to retail's three midi tests being of the load/PRE temp, as in
  `if (sv->midi4A != 0xFF)` with `midi` copied from it, rather than of a variable.
- Also tried: m1-m5 (a top `midi =` in the volume block, or at node 84, with the arm loads kept or removed, giving
  a split variable: 162-195 rows; a dead load has no effect), and s1-s3 (span store order in the `!= 0` arm).
- **Best next hypothesis.** Start from x1. Find the one `== 0`-arm statement that stores span (or reuses span)
  before the midi test, and the midi test written on the load expression. The `udump.py`/`urun.sh` route lets you
  write retail's PRE result into `r.O` and confirm what remains before searching for C.

## func_80018448: 26 rows (about 25 min)

Retail's first part needs two code-free pops between line 36 and line 37 (w23a). By the voiceAllocate finding, those
are as1-deleted narrowing temps. Tried (`v/r1`, `v/r2`):
- `(first & 255)` in the second stream: no change.
- The sibling's `change = first; change <<= 9; change >>= 9`: 39-43 rows.
- Masks on `first` in the tests at lines 32, 36 and 37 (`& 255`): 26-32 rows.
- Arm order, the increment spelled `+= 1` / `++`, and width: 26-45 rows.

Rows 63-82 also show as1 hoisting the other arm's cursor increment above the `first & 128` branch. Retail hoists
`+2`, ours hoists `+1`. That follows from the ring, not from emission order: f1, with the arms swapped, gives 30 rows.

Next: run `fifo_search.py` with a per-event retail map. Build the map from row alignment, since this ring has 8
members (t2-t9) and is not a single rotation. That locates the two pops; then look for a u8 register variable masked
there.

## func_80012D18: retail keeps `double pos` and two copy-propagation-proof u32 locals (about 15 min)

- g1 (no `double pos`, `position = func_8001E688(V->position)`): cfe area 64 with 4 homes, frame 112, 581 rows.
  Retail's area of 76 = 68 + 8 therefore includes the double *and* two 4-byte named locals (`position` at -12,
  `excess` at -20).
- g2 (`position = pos;` once at the top): 582 rows, copy-propagated again.

What is needed is a second reaching definition, or a redefinition of `pos` between `position = pos` and its uses, so
that uopt cannot substitute `cvt(pos)`. No natural candidate was found in the loop or no-loop code.

## What generalises

- **A code-free ugen pop is an as1-deleted redundant narrowing.** ugen allocates a temp for `x & 255` on a u8
  register variable (`and rT,rX,255`), and as1 drops it when rX is known zero-extended (loaded by `lbu`). This is the
  mechanism behind the sibling's `(first & 255)` lever, and the only code-free path in the 7.1 boot tail besides
  untraced specific-register takes (`f_get_one_reg`, for example t9 for `jalr`).
- `fifo_search.py` turns "the ring is rotated" into the exact event window where the extra pop or held temp must sit.
  For voiceAllocate it rules out everything outside L79-L81.
- ucode-level oracles: `cc -K` keeps `r.O` and `r.T`; `ugen … r.O -t r.T` plus as1 reproduces cc byte for byte, and
  the workbench parser decodes 7.1 ucode. Edit the ucode, then look for the C.
- Store PRE splits into independent kills. A kill before the store on every path stops the hoist; a kill between two
  equal stores on one path stops the second from being deleted. Each kill can come from reusing the variable.
- A "fewer rows" variant can still score worse (`true=` counts inserted and deleted instructions). Check with
  `LOCK true=` before calling it an improvement.

## Disclosures

- No source here is a candidate. `c7_held_pop.c` is an oracle (it has a visible andi). `x1_span_reuse.c` is a
  variable reuse with no device, but scores worse than best.
- The PRE-oracle runs (`P71_PRESET` / `P71_PRECLR`) and the ucode tools are diagnostics only.
- The builder was used only in `~/rush2049/scratch/frontier/w25b`, plus the compiler's own temp files there.
  w22c's and w24c's scratch dirs were only read (the ugen trace source and the `tgt/`/`t71/` copies).

## Files

- `tools/`: `fifo_search.py`, `udump.py`, `urun.sh`, `vb71.sh`, plus w24c's set (sc.sh, p71.sh, pre71r.sh, p71rep.py,
  check.py and others, with paths changed to w25b).
- `func_8001F13C/c7_held_pop.c` and `func_8001A658/x1_span_reuse.c`, as above.
- Variants are under `*/v/` (git-ignored).
- The best sources are still the previous lanes': `w24c/func_8001F13C/best.c`, `w23b`/`w24c/func_8001A658/best.c`,
  `w23a/func_80018448/best.c` and `w23a/func_80012D18/best.c`.
