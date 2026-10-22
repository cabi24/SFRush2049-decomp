# z12d18 — func_80012D18 (static, lib_11640, IDO 7.1 `-g0 -O2`), 2026-10-21

**MATCH.** `best.c` is byte-identical to retail: `obj71.sh` (trace71 toolkit, scratch
`watchman2:~/rush2049/scratch/frontier/z12d18`, fidelity 54/54) gives 788 = 788 `objdump -dr` rows, 0 differing,
relocations included; the pool proof `lock add ... --toolkit 4586d230…` (IDO 7.1) scored 0 and locked it.
The w23a baseline was 578 differing rows; session 1 took it to 187, session 2 to 0.

DISCLOSED: `probe` (an `unsigned short` local, never used) is still needed. Without it only the first expression
home moves (retail `44(sp)`, ours `48(sp)`: 4 rows), i.e. retail's local area is 4 bytes larger below `loopRem`.
Its real identity is unknown.

## Levers (each measured; rows after)

| Step | Lever | Rows |
|---|---|---:|
| base | w23a `best.c` | 578 |
| 1 | `cmd->volume[i] = (V->level[i] = V->target[i] * V->gain) << 16` (and the delta branch): retail sign-extends the stored short (`sll/sra` x8) instead of reloading `lh` | 542 |
| 2 | `loopRem / 32`, `mainRem / 32` in the **non-flags** ADPCM case only (retail: `bgez/sra/addiu` signed divide there, plain `sra` in the flags case) | 548 (+8 words, now aligned) |
| 3 | the `flags & 1` cases end with `goto adpcm_tail` / `goto pcm_tail` into the non-flags cases' shared tails (retail `b 0x6c0` / `b 0x780` with the tail's registers preloaded); IDO does not merge tails itself | 491 (788 = 788) |
| 4 | `unsigned int position = func_8001E688(V->position)` (no double local; uopt still places the conversion per branch) | 487 |
| 5 | `unsigned int excess` declared after `state`, assigned in the third condition: cmd/state/address/length/padded/mainRem/loopRem homes all at retail offsets, frame 120 | 412 |
| 6 | range planning statement order: third branch computes `offset` before storing `address[0]` (retail reuses the test's `loop_start` load: no reload); second and fourth branch order | 230 |
| 7 | more statement order (third/fourth branch, else-path sub-branch) | 221 |
| 8 | 2 bytes of local area below `loopRem` (`probe`, disclosed): first expression home at -76 like retail's `44(sp)` | 217 |
| 9 | non-flags ADPCM loop/main statement order; `cmd->unknown44 = 0` after `position4C` | 205 |
| 10 | `frames = (V->rate * 192 + 0xFFF) >> 12; frames += 6;`: the second definition raises frames' priority (tot 6 -> 8, save 1.5 -> 2.0, ties the excess expression and is decided first), so frames a1 / excess a2 like retail; equals the force71 oracle | 187 |
| 11 | `unsigned int mode` (`mode_u`): the end markers' `mainRem + mode` used an int conversion of mainRem and `length[0] + mainRem` an unsigned one, two webs (two copies `move v0,t1` / `move a0,t1`); unsigned mode makes them one web (retail's single `move v0,t1`) and drops the int one out of the ADPCM `(int)mainRem` web (save 4.5 -> 3.0) | 197 (-2 words) |
| 12 | non-flags PCM main: `mainRem = address[0] & 3;` before the `main.first` store (`p_m1`; retail reads `address[0]` once: `move t1,t7` / `andi t1,t1,3`, no reload, no `andi 0xffff`) | 194 |
| 13 | flags PCM calls: `func_80012730(&cmd->loop, (unsigned int)((address[1] & 0xFFFC) * 2 + V->data), ..., (unsigned int)V->data)` (`pc_ptr2`; `pc_int4`, `pc_ptr2i`, `pc_int2` are byte-identical). Mechanism below | 137 (788 = 788) |
| 14 | non-flags PCM order: `main.first_length` before `main.second_length` (`p_m3`), `main_count` first in `pcm_tail` (`pt1`), `loop.first_length` before `loop.second_length` (`p_l1`) | 130 |
| 15 | else entry: `mode = -1` inside the else sub-branch instead of at its top (`e_d4`; retail sets offset (t5) before mode (t4)) | 127 (opcodes 788/788) |
| 16 | `loopRem = address[1] & 3 & 0xFFFF;` in the non-flags PCM loop (`ph2`; `ph1`, `ph3`, `ph6` identical): a phantom ring pop (law L65) | 20 |
| 17 | `cmd->position4C = (unsigned short)(padded[0] * (V->rate * 16)) + cmd->fraction;` (`r5`; `r1` `(... & 0xFFFF)` identical): the second phantom pop | **0** |

## Session 2 mechanisms (2026-10-21)

- **`V->data` in v0 (step 13).** Webs are p1-coloured in save order; ties go to the lower web number, and a split
  piece gets a new, higher number. The `V->data` expression web (four flags-case units) is split: its ADPCM-main/PCM
  piece lost v0 to the `(int)mainRem` web. Forcing that piece to v0 (`force71 p1:w420=c1,p1:w185=c7`) reproduced
  retail (-20 rows), so the question was why the piece could not take a caller-saved register by itself. Every
  caller-saved register except a3 cost the ~1e20 "ineligible" sentinel there. In `f_cupcosts` (recompiled 7.1 uopt),
  a live unit whose second flag byte is 6 makes every register in the mask at `0x1001c7d0` cost 1e20. Only the two
  flags-PCM units carried flag 6: the units where `V->data` is passed **directly** as an outgoing argument (the 4th).
  Writing the address argument as one cast expression (or the 4th as `(int)V->data`) ends the direct-argument use of
  the shared web, v0 becomes eligible, and ugen emits retail's `move a3,v0`. Rule: a web that is itself a direct
  call argument is restricted to that argument's register in that unit.
- **The ugen temp ring (steps 16-17).** After step 15 every opcode matched and all 127 rows were t6-t9 names from
  row 443 on. ugen's integer ring here holds only t6 t7 t8 t9 (t0-t5 are coloured somewhere; `ugt71.sh` shows the
  REMOVEs) and is strictly FIFO (pop head, free to tail; checked event by event in our trace). The registers of line
  188 (non-flags ADPCM tail) fully determine the queue entering the PCM case, and working the target's
  allocations backward shows no starting queue reproduces them: the target spends one extra pop between the
  `address[1]` load and its `& 3`. A redundant mask (`& 0xFFFF` folded into the u16 store) is exactly one
  instruction-free pop (compiler law L65 in `third_party/n64-decomp-workbench/docs/compiler-laws/ido-5.3.md`). The same
  analysis at `position4C` needed one pop between `V->rate * 16` and the product; the `(unsigned short)` cast of the
  product supplies it (masking the whole sum pops one step too late: 5 rows).
- Not levers (measured, kept in `variants.py` EDITS): truncating casts `(unsigned short)(address[1] & 3)` (cfe drops
  it), `(unsigned short)address[1] & 3` (narrows the load to `lhu 98(sp)`), `% 4`, `3U`, condition and
  `loop.first` spellings (24 combinations, all identical bytes); chained `cmd->loop_offset = loopRem = ...`;
  `(loopRem &= 0x1F)` / `(mainRem &= 0x1F)` as values; a `format` local; casts on either switch selector.

## Method and tools

`variants.py BASE SPEC...` applies named edit sets and scores each (`EDITS` holds every edit tried);
`rows.py SRC...` scores and prints the differing row ranges (`ROWS_DIR` sets where row files go). Analysis was an
opcode-level alignment of target vs ours (`difflib` on mnemonics), then reading the target around each misaligned
block; colouring with `ctrace71.sh`/`force71.sh` (`CDX_DETAIL_WEB`, `CUPLOG`), the ring with `ugt71.sh`.

Behaviour: every lever is semantics-preserving (`/ 32` equals `>> 5` for the 6-bit remainders; the goto tails run the
same statements; reorders move independent stores only; `mode` holds -1/-2/0/`loop_start - position` and is only
compared with -1/-2 and added to a u16; the masks and casts truncate values that are stored to u16 fields anyway).
