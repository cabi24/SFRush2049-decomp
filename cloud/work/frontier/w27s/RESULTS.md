# w27s - static boot tail (worktree lane): func_80018448, inflate leftovers, libultra rename leads

Branch `w27s` (worktree `.claude/worktrees/w27s`, from wave27 8a54d6d2). Static numbers only (`make progress`).
Status: IN PROGRESS.

**Static 625/667, 147,280 B -> 627/667, 149,708 B (93.15%) so far.**

| function | bytes | state | scorer / gate line |
|---|---|---|---|
| func_80018448 | 492 | **PROMOTED** ef6833a7 | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)`; `TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)` `RESULT PASS`; lock `(ido7.1): score=0`; gate `OWNED-OFFSETS: OK 19 ROM TU data sections ...`, `PROMOTED 1 (492 B) @ ef6833a7` |
| inflate_dynamic | 1936 | **PROMOTED** a1bbc86c (+ reloc_addrs prep 2 entries) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (537 rows)`; `TU: ... identical (537 rows)` `RESULT PASS`; lock `(ido5.3s): score=0`; `PROMOTED 1 (1936 B) @ a1bbc86c`, OWNED-OFFSETS OK |
| inflate_entry / inflate_entry_alt | 360/132 | left as GLOBAL_ASM: hand-edited call (evidence below) | best C `LOCK: true=25 strict=25 reloc_blind=4 objdump-dr DIFFERS(3 rows) (122 rows)` (u64 argument) |
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

## inflate_dynamic (1,936 B, ido5.3s): MATCH, promoted

Source `sources/inflate_dynamic.c` (= `inflate_dynamic/best.c`), from w26a's best.c (344/484 register-equal,
`LOCK: true=1945 strict=2313 reloc_blind=213 objdump-dr DIFFERS(521 rows) (537 rows)` against the current target).

Traced, then one lever (about 1 h):
1. Oracle: `tools/dforce.sh` = 5.3 cfe, the wave-5 TRACED uopt (`~/rush2049/scratch/frontier/wtk/bin/uopt`, CDX_LOG;
   output byte-identical to stock here), stock ugen, then the ido5.3s assembly step. The colouring summary showed the
   `nl + nd` expression web (type 4, the value retail keeps in s8) with `tot=33` against a callee-saved cost
   `best=38.5`, so it was split; `n` itself is a memory variable in both (retail reloads it from 0x84 for the loop
   test). Forcing that web to c22 (s8) took the residual from 217 rows to 126 and fixed the frame.
2. The cost is blocks/4 (a `goto`-exit variant added one block: 38.75). 154 blocks -> the 16 `do { } while (0)`
   wrappers of NEEDBITS/DUMPBITS (8 uses each) cost 2 blocks apiece (NEEDBITS-only or DUMPBITS-only variants: 34.5).
   With **gzip's original brace macros** `{ while (k < (n)) {...} }` / `{ b >>= (n); k -= (n); }` the procedure has
   122 blocks, cost 30.5, the expression web is coloured s8 (after w0 took s7, as retail), and the td reload
   disappears too: 1 row left = the end-pointer relocation.
3. `&gInflateBorder[19]` (the zero-fill loop end) is retail's split symbol `gInflateCplens`:
   `reloc_split derive inflate_dynamic --candidate ...` proposed `rom:0x6BDC/0x6BE0 ... symbol:gInflateBorder
   addend:0x4C`; `reloc_split check`: `all 131 reloc_addrs entries encode their ROM words`; `make extract` changed only
   inflate_dynamic.s's two operands (commit 26d3bb6a); `refresh_split_targets.py inflate_dynamic`:
   `reloc_aware f82aa18a6528 -> reloc_aware 735d59c0892d` (Conveyor DB backup `scratchpad/w27s/conveyor.db.w27s.bak1`).

The macros are named NEEDBITS_BR/DUMPBITS_BR only because the TU already defines NEEDBITS/DUMPBITS (do-while form)
for the other slots (a different redefinition is cfe warning 581). No shaping devices, unused locals or pad arrays.

Proof: `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (537 rows)`, `TU: true=0 strict=0 reloc_blind=0
objdump-dr identical (537 rows)`, `RESULT PASS`; `context_check.py --compiler ido5.3s` -> `inflate_dynamic ok`
(`ctx53s.jsonl`); `lock add ... --toolkit 8bdf0cae...` -> `(ido5.3s): score=0`;
```
CONVEYOR_BUILDER_REPO=~/rush2049/scratch/frontier/w27s/repo CONVEYOR_BUILDER_JOBS=2 \
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w27s/ctx53s.jsonl 0x5610 \
  --only inflate_dynamic --prefix-dedup rom/lib_5610
builder gate: OWNED-OFFSETS: OK 19 ROM TU data sections (16 owned slots), 23 named objects checked against retail addresses (18 in owned slots), 1 without a retail address
PROMOTED 1 (1936 B) @ a1bbc86c
```

## inflate_entry / inflate_entry_alt: the call was hand-edited in the .s (evidence), left as GLOBAL_ASM

Retail: `li a1,12000; jal sound_play_menu; addiu a0,zero,0` (sound_play_menu = the game heap allocator
`(Heap *heap, u32 size)`, NULL heap = default).
1. **`addiu a0,zero,0` is reachable from C** only as half of a 64-bit value: IDO 5.3 ugen writes `li $4, 0` for the high
   word of a 64-bit argument (the same encoding as __osInitialize_common's `__ll_mul(x, 3LL)` and osContInit's
   64-bit constants); 32-bit zero is always `move`. With `sound_play_menu(12000LL)` (or a `u64`/`s64` prototype) the
   result is `li a0,0; jal; li a1,12000`: `LOCK: true=25 strict=25 reloc_blind=4 objdump-dr DIFFERS(3 rows) (122 rows)`
   (inflate_entry, `inflate_entry/best_u64.c`; w26a's best 215). But ugen emits a 64-bit constant high word first in every
   spelling tried (12000LL/ULL, (u64)/(s64) casts, enum, sizeof, `+ 0ULL`, `<< 0`, a u64 local); only a non-constant
   u32->u64 conversion evaluates the low word first, and then it is a `move`, not `li 12000`. So retail's order (low
   word, then `li a0,0` in the delay slot) is not reachable.
2. **The rest of inflate_entry_alt has a ONE-argument call's register allocation.** With `sound_play_menu(12000)` (one
   s32 parameter) every other instruction matches retail: `src` moved to a3 (a0 is the call's argument in the same
   block) and `size` kept in its incoming a1 (`alt_onearg_probe.c`: the only differences are retail's `li a1,12000` +
   `li a0,0` instead of `li a0,12000`). Any two-word argument puts a1 in that block too, and then uopt cannot keep size in
   a1: every two-argument or 64-bit form homes it (`sw a1,36(sp); lw t8,36(sp)`, the 2 extra instructions w26a saw).
   inflate_entry with the one-argument call (`onearg_probe.c`) also differs only by the inserted `li a0,0` (and the
   branch offsets after it).
Conclusion: the object was compiled from a one-argument call and the .s edited afterwards (`li $4,12000` ->
`li $5,12000` + `li $4,0`), consistent with the TU's other hand-edited code (inflate_io_wait/lzss_decode non-ABI).
No C produces it; they stay GLOBAL_ASM.
