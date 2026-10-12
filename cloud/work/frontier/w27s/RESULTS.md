# w27s - static boot tail (worktree lane): func_80018448, inflate leftovers, libultra rename leads

Branch `w27s` (worktree `.claude/worktrees/w27s`, from wave27 8a54d6d2). Static numbers only (`make progress`).
Status: IN PROGRESS.

**Static 625/667, 147,280 B -> 626/667, 147,772 B (91.94%) so far.**

| function | bytes | state | scorer / gate line |
|---|---|---|---|
| func_80018448 | 492 | **PROMOTED** ef6833a7 | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)`; `TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)` `RESULT PASS`; lock `(ido7.1): score=0`; gate `OWNED-OFFSETS: OK 19 ROM TU data sections ...`, `PROMOTED 1 (492 B) @ ef6833a7` |
| inflate_dynamic | 1936 | pending | |
| inflate_entry / inflate_entry_alt | 360/132 | pending | |
| 7 libultra rename leads | - | pending | |
| func_80012D18 | 3148 | only if a lever appears | |

## func_80018448 (492 B, IDO 7.1 -O2): MATCH, promoted

Source `sources/func_80018448.c` (= `func_80018448/best.c`), written in the locked sibling func_8001824C's style
(its `SequenceTrackNotes` view and `TRACK(i)` macro; `--prefix-dedup` drops the repeated lines in the TU).

The lever: both delta reads take **short operands**:
`change = ((short)(first & 127) << 8) | (short)second;` (first part) and
`change = ((short)(first & 255) << 8) | (short)second;` (15-bit delta). w25f's device
`(u8)(first & 255) & 127` is gone.

How (about 1 h):
1. Ucode first (w26b's `uedit.py`/`urun.sh`, new `tools/uscan.py` = batch uedit+urun with 2 jobs, ~0.05 s/variant).
   Single `cvtl` insertions after every record of lines 40-70 of uopt's `r.O` (744 variants): a signed 16-bit
   `cvtl` after line 45's `lod second` was the only one that moved the first differing row (52 -> 62, 24 -> 18
   rows). Its sll/sra temps (deleted by as1: the byte is already zero-extended) are retail's 2 hidden pops before
   line 54, and their free order (t8, t7, t9) is exactly the `t8 t7 t9` order retail shows at line 67 - the thing
   w26b's greedy FIFO called "different free order".
2. Chains of narrowings on line 61's `first` + a 16-bit cvtl on its `second` (774 variants): `and 255` then a 16-bit
   cvtl on first, plus the cvtl on second, gave 0 rows (3 + 2 hidden pops, retail's free order everywhere after).
3. C: `(short)` casts on int-typed operands are what make cfe emit those cvtls. `(short)second` alone and
   `(short)(first & 255)` give them; `(short)first`, `(short)(first << 8)`, `(short)((first << 8) | second)`,
   `(short)(u8)first` do not (18 rows); `(short)(first & 127)` in the first part needs no other device.

Disclosure (also in the header): `first & 255` on an unsigned char is a no-op mask, load-bearing - the same
mask the locked sibling func_8001824C carries in its 15-bit read. No unused locals, pad arrays, implicit int or
false prototypes.

Proof (builder scratch, toolkit 4586d230, `tools/check.py` w16g replay):
```
LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)
TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)
RESULT PASS
```
`context_check.py --compiler ido7.1` -> `func_80018448 ok` (`ctx71_O2.jsonl`).
`lock add ... --toolkit 4586d230...` -> `(ido7.1): score=0` / `locked ... (score0)` (commit f1eb254a).
```
CONVEYOR_BUILDER_REPO=~/rush2049/scratch/frontier/w27s/repo CONVEYOR_BUILDER_JOBS=2 \
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w27s/ctx71_O2.jsonl 0x17dc0 \
  --only func_80018448 --prefix-dedup rom/lib_17dc0
builder gate: OWNED-OFFSETS: OK 19 ROM TU data sections (16 owned slots), 23 named objects checked against retail addresses (18 in owned slots), 1 without a retail address
PROMOTED 1 (492 B) @ ef6833a7
```
No reloc_addrs, symbol, rom_tu.h or owned-data changes.
