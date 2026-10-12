# w24c — static boot tail (IDO 7.1, trace71): synthHandle, voiceAllocate, func_80018448, func_80012D18

Builder scratch `watchman2:~/rush2049/scratch/frontier/w24c` (base copy + rsync of src/{blob,rom},
include, tools/{cloud,conveyor,asm-processor,workbench.py}, asm/us/{blob,boot_tail,nonmatchings/rom},
cloud/work/boot_tail_promotion, locks, symbol files; `tgt/` copied from w23b; trace71 `--reuse w22c`).

## Headline

No match this wave. One near miss moved a long way through device-free levers, and the PRE mechanism behind both
MusyX near misses is now proven with a new 7.1 PRE oracle.

| function | B | before (w23) | after (w24c) | scorer line (`tools/sc.sh` = check.py = lock add verify_entry, 7.1) |
|---|---:|---|---|---|
| func_8001F13C voiceAllocate | 1456 | true=2045 strict=2069 | **true=605 strict=605** | `LOCK: true=605 strict=605 reloc_blind=132 objdump-dr DIFFERS(96 rows) (436 rows)` |
| func_8001A658 synthHandle | 2812 | true=2770 | 2770 (best unchanged; oracle 43 rows) | `LOCK: true=2770 strict=2770 reloc_blind=83 objdump-dr DIFFERS(319 rows) (749 rows)` |
| func_80018448 | 492 | 26 rows | 26 rows | obj71/p71 `rows: want 124 got 124; differing rows 26` |
| func_80012D18 | 3148 | true=12735 | unchanged | not re-scored (w22c best) |

Flags: `-g0 -O2 -mips2 -G 0 -non_shared` (+ `-Wab,-r4300_mul` for voiceAllocate, as w23b), toolkit 4586d230.
No sources/, ctx71 jsonl, prove.log or promote_batch line (no match). Nothing committed, locked, promoted or spliced.

## New tool: a PRE / code-motion oracle for IDO 7.1 (`tools/pre71.py`, `p71.sh`, `p71rep.py`, `build_pre.sh`)

Port of the 5.3 w5d hooks to the 7.1 uopt (on top of trace71's uopt71.trace.c; builds `t71/pre/` next to `t71/ido`).
Address map (found by comparing f_printcm / f_newbit / f_isvarihash in both generated uopt.c):
node list 0x1001c8f8 -> 0x1001c358, bit count 0x1001cb38 -> 0x1001c390, bit table 0x1001cc30 -> 0x1001c3a0,
opcode names 0x10000240 -> 0x1000ff70 (found by scan), node vector offsets identical (antlocs 104 ... insert 164
delete 124), 7.1 packs var/lda addresses as one word (`blockno = w >> 11`, `mtype = (w >> 8) & 7`), node edge lists
at +20 (pred) / +24 (succ), first/last statement at +28/+32. Hooks: before codemotion (`P71_PREDUMP`,
`P71_PRECLR`/`P71_PRESET` = change local vectors), after codemotion (dump; `P71_CLR`/`P71_SET` = change
insert/delete/...), globalcolor entry (dump). `P71_FN` selects the procedure by name. With no P71_* set the object is
identical to the traced/stock one (same rows on every run here). Usage:
`tools/p71.sh SRC FN [P71_CLR=insert:95,delete:95@88 ...]` then `python3 tools/p71rep.py TRACE BIT|/regex/`.
Its 7.1 vectors for voiceAllocate reproduce w23b's 5.3 pretrace exactly (bit 95 antloc {31,75,88,101}, INSERT 60/87,
DELETE 88).

## func_8001F13C voiceAllocate: 2045 -> 605 (`func_8001F13C/best.c`)

`LOCK: true=605 strict=605 reloc_blind=132 objdump-dr DIFFERS(96 rows) (436 rows)` (w23b: `true=2045 strict=2069
reloc_blind=130 objdump-dr DIFFERS(201 rows)`). Flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

**(1) PRE hoist of mpy(voice,416) - proven and fixed.** Oracle: clearing INSERT/DELETE of bits 95/96 (mpy, ixa)
after code motion, or only `avlocs` of 95/96 in node 75 (the skip-loop compare) before it, both give retail's
`_update` code. The graph dump then showed why retail does not hoist: **7.1 uopt does not split critical edges.**
`if (voice == -1) return -1; _update:` leaves a label-only block (node 87: `ulab; uloc`) between the test and
`_update`, and PRE inserts there and in the D_800504C0 goto block (INSERT 60,87). Every goto/label spelling keeps such
a node (`if (voice != -1) goto _update;` = a ujp block; label inside if/else; label on the same line). Lever: no
label - `if (D_800504C0 != 0xFF && type_alloc == 0) { voice = D_800504C0; } else { ...search...; if (voice == -1)
return -1; }`. The test's false edge then enters the join block directly (85 -> 87 critical), ppout(85) is false,
nothing is inserted (`v/d1.c`: true 2045 -> 605, strict 629).
**(2) maxVoices home (sp+24 vs retail sp+28) - traced and fixed.** spill71: every coloured or split expression web
gets a 4-byte home in bit order, reused only between webs sharing no block. cvt(D_8004FA1A) (w8, v0) takes home 0
and maxVoices' split cvt web (w23) shares the `D_8004FA1A <= maxVoices` block with it, so it gets home 1. CDX_FORCE
`p2:w8=s` still homes w8 (split webs are homed too), so in retail that expression web is absent (or numbered after w23). Holding the
limit in an existing v0 variable removes it: `type_alloc = (D_800504C2 >= (i = D_8004FA1A) && D_8004FA18 > i) ? 1 :
0; if (i <= maxVoices)` (same for D_8004FA19). `i` is the v0 web already, the colouring is unchanged, maxVoices goes
to home 0 = sp+28, and the embedded assignment keeps retail's load order (a separate `i = D_8004FA1A;` statement
loads it before D_800504C2: true 665). Natural C (variable reuse), no device. Using `num`/`voice` instead recolours
(147/124 rows).
**(3) Open: one ugen temp pop.** All 96 rows are a temp-ring rotation from 0x2c0 (count loop, `i = D_80050548[p]`).
FIFO replay of our DKWB-FREELIST events (`t6..t9` only; t0-t5 are coloured) gives [t7 t9 t8 t6] before that line;
retail's pops (t9, t8, t6, t7, t9, t8 ...) need [t9 t8 t6 t7], i.e. exactly one extra alloc+free of t7 in retail
after line 73's reload (t6) is freed and before line 75. In ours the line-74 while test reuses line 73's reload
through the usage count (`MOVE_END t6`, the used list; `f_free_reg` frees a temp only when its usage count reaches
0). Retail most likely evaluates a second reload request there that ugen satisfies from t6 after popping t7.
24 count-loop shapes (while/for/guard/body spellings), 11 condition spellings, index casts on `D_80050548[p]`: no
change (uopt canonicalises them all). Stop rule reached.

## func_8001A658 synthHandle: 2770 (best = w23b's, `func_8001A658/best.c`); both PRE residuals proven

Retail has `move a3,zero` (span = 0) right after `func_80014A0C` (node 84, the envelope test) AND a second
`span = 0` in the `D_8004F2F8 == 0 / midi == 0xFF` arm (node 113), nothing in the `D_8004F2F8 != 0` arm (span saved
at 104(sp) around its call), and a single `lbu a0,74(s2)` (sv->midi4A) in the volume block with `move a1,a0` in each
D_8004F2F8 arm. `func_8001A658/oracle_s1.c` = w23b best with retail's span placement.
- **span = store PRE.** PRE dump: bit 240 `str.J(span,0)` antloc {84,113} -> INSERT 10 (loop head), DELETE 84,113.
  `P71_CLR=insert:240,delete:240` (no store PRE for span): span code exactly retail's, 66 rows (w23b best: 80).
  Before code motion, `absalters:240@83` alone stops the hoist but still deletes 113 (61 rows, arm store lost);
  `absalters:240@83,absalters:240@99` = the no-PRE result. Uses (`alters`) never stop the 113 deletion. So retail's
  ucode has **another store to span on every path into the envelope block and one in the `D_8004F2F8 == 0` arm before
  the midi test**, both gone from the object (copy-propagated, dead). Source confirmation of the first half:
  `span = cpitch; func_80014A0C(i, span); span = 0;` (x1, device-like, not delivered): no insert, DELETE 113 only;
  `if ((midi = span = sv->midi4A) != 0xFF)` adds the second half (x6: no insert, no delete) but leaves span holding
  midi (111 rows). No natural form found.
- **midi = load PRE.** Bit 75/76 `ilod/cvt(sv->midi4A)`: antin covers the volume block (nodes 93-98) but it is not
  partially available there (the envelope stores at 85-92 kill it), so it is never hoisted. Oracle
  `P71_PRESET=avlocs:75@84,avlocs:76@84` (the load available after node 84) + the span oracle: INSERT 93, DELETE
  99,115 and **43 rows**: the top `lbu a0` + `move a1,a0` pattern is exactly retail's (only its position, 3 compare
  operand orders and the fader loop differ). So retail also reads sv->midi4A in node 84 (after func_80014A0C) or on
  the envelope-not-taken path. A dead `midi = sv->midi4A;` there is removed before code motion (mm1/mm2: no effect);
  a top `midi = sv->midi4A;` instead of the arm loads makes it a split variable (m1-m4: 135-239 rows).
- Retail's three midi compares put the 0xFF register first (`beq s5,a0`); ours `beq a0,s5` in every spelling
  (`0xFF != midi`, separate assignment statement) unless midi is u8/u16 (then 152-byte frame and `andi`): open.
- Fader loop (w23b): unchanged.
- span's type, zero spelling, `register`, block scope: no effect (all the same bit; cfe never shares slots between
  block scopes, k2).
- Next: find the natural statements that put a span store and an sv->midi4A load right after `func_80014A0C`
  (node 84) - both point at the same spot, so a single retail statement or helper there may supply both (e.g. an
  inlined helper taking `sv` whose body was copy-propagated away; look for a caller-less stub in lib_1a660).

## func_80018448: 26 rows (unchanged), about 30 min

Re-derived w23a's ring with ugt71 + FIFO replay. ugen's free list is a FIFO, but `f_free_reg` (7.1 ugen.c) only
returns a temp when its usage count drops to 0, and `MOVE_END` reorders the separate used list: a held temp is one
whose ucode node has more than one use. Retail's line-37 pops (t6 t7) need two extra pops after line 36, and the
second part (t6 t2 t5 t6, then t8 t7 t9) is not FIFO-consistent without temps held across lines 47-57.
Nine line-37 spellings (`(u8)`, `& 255`, `(u16)` on each operand): no change (uopt folds them before ugen).
Next: find ucode with multi-use nodes (usage count > 1) in the second part; the PRE dump shows none of the
second14 increments (bits 66/71) are moved.

## func_80012D18: not worked beyond one check

PRE dump on w23a's `probe_named_position.c`: neither `position` (var -12) nor `excess` (-20) has a store bit even
before code motion, so uopt's copy propagation removes them before PRE. A form that keeps them must defeat copy
propagation itself (two reaching definitions at a use, or a right-hand side copypropagate will not duplicate).
Not tried further (578 rows; lowest expected value of the four).

## What generalises (the oracle)

- **pre71 = the 7.1 counterpart of the 5.3 W5D/nopre oracle**, and more: it dumps every node's 15 PRE vectors, edges
  and first/last ucode op, and it can clear or set any vector bit both before code motion (local facts:
  antlocs/avlocs/alters/absalters) and after it (insert/delete). "Which local fact must differ for retail's code?" is
  now one run. With no `P71_CLR/SET`, objects are byte-identical to the traced and the release 7.1 (checked on both
  MusyX reduced TUs, with the dump on).
- **7.1 uopt does not split critical edges.** A label right after an `if (...) return` makes a label-only block
  where PRE can insert; an if/else join (no label) does not. Use this to stop or allow a PRE insertion.
- **Store PRE:** a constant store in a loop body is hoisted to the loop head unless another store to the same
  variable (`absalters`) precedes it on some path. Uses do not block it, and they do not stop a later identical store
  from being deleted as redundant.
- **Load PRE needs partial availability:** an anticipated `p->field` load is hoisted into a block (giving retail's
  "load once into a0, `move a1,a0` per arm") only if some path into that block already has the load with no
  intervening store or call.
- **Spill homes are per expression web, coloured or split, in bit order.** A home that is 4 B low behind an early
  expression web means retail has no such web: hold that value in an existing variable of the same register
  (voiceAllocate `i = D_8004FA1A`).
- ugen: `f_free_reg` frees on usage count 0; MOVE_END is the used list. A FIFO replay that needs one extra code-free
  pop points at a second ucode reference that ugen satisfied from a register it already held.

## Disclosures

- voiceAllocate best.c: no devices, no unused locals, no pad arrays, no false prototypes, no implicit int. Levers:
  if/else in place of the `_update` goto (control flow equivalent), `i` reused to hold the per-type voice limit (an
  embedded assignment inside the type_alloc condition). w23b's `volatile D_8004BE94` and the private packed views
  are kept.
- Oracle-only (in no delivered source): every P71_CLR/P71_PRESET run, CDX_FORCE runs, `span = cpitch` (x1/z1/x7),
  `midi = span = ...` (x6), dead midi loads (mm1/mm2).
- Builder: only `~/rush2049/scratch/frontier/w24c` (copied `tgt/` from w23b's scratch, which was read only;
  `include/rom_tu.h` copied from src/rom so the sibling TU compiles under tk71). 2 cores. No permission denials.

## Files

`func_8001F13C/best.c` (near miss), `func_8001A658/best.c` (= w23b best), `func_8001A658/oracle_s1.c` (the
oracle source). `tools/`: `pre71.py` (uopt patch), `build_pre.sh` (build `t71/pre/` from w22c's uopt71.trace.c),
`pre71r.sh` (builder runner), `p71.sh` (Pi wrapper), `p71x.sh` (resolve a bit regex, then clear it), `p71rep.py`
(per-bit vector report), plus w23b's `check.py`/`sc.sh`/`ct.sh`/`prove.sh`/`splice.py`/`tusplice.sh`. Variants are in
`*/v/` (git-ignored). PRE dumps go to `build/trace71/w24c/FN.p71.txt`.
