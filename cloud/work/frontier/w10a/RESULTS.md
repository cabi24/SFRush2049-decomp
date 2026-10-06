# Wave 10, lane w10a — results

Assignment: `entity_update_callback` (2,184 B), `func_800E847C` (2,100 B), `func_80109468` (1,528 B),
`entity_process_main` (1,424 B). None was untouched: all four had prior drafts (w1b/w4b, w6c/w7d, w6d/w7d),
which were the starting points.

All scores are `-g0 -O3 -mips2 -G 0 -non_shared`, from the Pi with
`python3 -m tools.conveyor.pipeline.blob_unit --tag w10a score …` (whole-program unit) unless marked as the
builder scorer.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| `func_80109468` | 1,528 | **MATCH** (strict; own .rodata verified) | `-O3` single, kept | `MATCH` / `own .rodata verified at 0x80120E40..0x80120E48`; unit: `EQUAL func_80109468: 382 words (kept, c_func_80109468.c)`, `locked bodies that differ in this unit: 0` |
| `entity_process_main` | 1,424 | 3 words off (was 20) | `-O3` single | `FAIL entity_process_main: 3 of 356 words differ` |
| `func_800E847C` (+ `func_800E7FA0`) | 2,100 (+1,244) | 232 / 35 words off (E7FA0 was 38) | `-O3` group, E7FA0 internal | `FAIL func_800E847C: 232 of 525 words differ` / `FAIL func_800E7FA0: 35 of 311 words differ` |
| `entity_update_callback` | 2,184 | unchanged (w6d/w7d: 3 mnemonic rows) | `-O3` single | `FAIL entity_update_callback: 423 of 546 words differ; compiled body is 545 words, target 546` (unit) |

## func_80109468 — MATCH

Deliverable: `cloud/matches/func_80109468.c` (line 1 `/* flags: -g0 -O3 -mips2 -G 0 -non_shared */`).

Builder scorer, in the w10a scratch copy:
```
python3 tools/cloud/score.py fn cand/func_80109468.c func_80109468 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_80109468:
  MATCH
    own .rodata verified at 0x80120E40..0x80120E48
```
Whole-program unit with neighbours:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10a score func_80109468 --with cloud/matches/func_80109468.c --neighbours
  EQUAL func_80109468: 382 words (kept, c_func_80109468.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w10a/unit.o (4.0s)
```
**Integration:** a plain single (`splice_singles.py func_80109468`). It needs no unit_overrides entry and supersedes
no group. The only own .rodata is the `"MAP_MD"` string, and the scorer verifies it.

Semantics: the split-screen minimap Blit AnimFunc. It is N64-only and has no arcade ancestor. w6c/w7d have the
details. Starting from w7d's best (7 normalised rows), it closed in three steps. A trace and a forced colouring
checked each one:
1. **Row pointer:** the row pointer is a single statement after the column choice,
   `row = data + (size - y - 1) * size / 2 + col / 2`. In retail the base (a1) and the final pointer (v1) are
   different webs. This also removed the `move a0,s0` hoists that w7d had attributed to as1.
2. **Arm temporary:** the `size - 8` term goes through a temporary in each arm (`t = size - 8; w = t; h = dz * t / dx;`).
   Without it, w's live-range split leaves its arm-1 piece at save 0, and it gets no register: ours wrote
   `addiu t6; sw; move t8,t6`, while retail has `addiu v0`. One extra word shifted the whole temp ring.
3. **Counter reuse:** the clear loop reuses **pz/px** as its row/column counters. One variable is one live range,
   so px and pz inherit the clear loop's weight. They are then coloured before the point-pointer expression,
   which gives px v0, pointer v1 and pz a1 as in retail. Forcing those three colours first
   (`force.sh … p1:w160=c1,p1:w163=c2,p1:w170=c4`) gave 2 rows, which only the string relocation explained. The
   counter-identity sweep then found the source form: `g1/` has 72 pairs and `g7/` tests them on the fixed base.

## entity_process_main — 3 words (best: `entity_process_main/best.c`)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10a score entity_process_main --with cloud/work/frontier/w10a/entity_process_main/best.c
  FAIL entity_process_main: 3 of 356 words differ
       +0x510  image 944e0002  unit 944f0002
       +0x514  image 0098c804  unit 00982004
       +0x518  image 03207827  unit 00807027
```
Changes from w1b's best (20 words), in order:
- **`i` reused for `car->unk35C`** (`i = car->unk35C; if (i >= 0 && …)`). This puts unk35C in a0, which is
  retail's register (`g1/i.c`). Alone it costs 128 words, because i's longer range reorders the pool.
- **w4b's compiled-out check** `if (scale == 0) DEBUG_PRINT(("x"));` before the final call. Together with the
  `i` reuse, every register is retail's (18 words; `g2/sc_before_call.c`). The remainder was ugen evaluation
  order: retail emits li, sllv, lhu, nor, and.
- **Clear-arm form:** the clear arm is `poly = &D_8015B268[v->objnum]; i = ~(1 << i); poly->flags = i & poly->flags;`
  (`g11/c_i.c`). This gives retail's order (address, then shift, then load) and its `and` operand order.
  - Residual: the shift result lands in i's register a0 (`sllv a0,t8,a0`), where retail has a ring temp (t9).
    The ring then gives lhu t7 / nor t6, where retail has t6 / t7.
  - Tried without movement:
    - about 60 spellings: casts, `1U`, s16/u16/s8/u8/s32 holder variables, `whl`/`xlu`/`pad1`/`body` as the
      holder, inlined `bit()` helpers (an inlined call adds 8 frame bytes, so it is wrong here);
    - `if ((1 << i) == 0)` checks before or after (the CSE web gets coloured v1, not a ring temp);
    - ternary forms.
- Next hypothesis: retail's shift is a block-local expression temp that ugen evaluates before the load. Find a tree
  where `shl` is a local CSE that is not coloured. The tiny-harness `o3s.sh` runs in the scratchpad show the
  forms: an expression shared within one statement or block.

## func_800E847C + func_800E7FA0 — 232 / 35 words (best: `func_800E847C/best_group.c`)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10a score func_800E847C func_800E7FA0 --internal func_800E7FA0 \
    --with cloud/work/frontier/w10a/func_800E847C/best_group.c
  FAIL func_800E847C: 232 of 525 words differ
  FAIL func_800E7FA0: 35 of 311 words differ
```
w7d showed that E847C's s-register rotation follows from E7FA0 (retail E7FA0 uses s0–s5), so I worked on E7FA0:
- **Phase-2 colouring order (new finding):** the trace (`runs/e7*`) shows that E7FA0's segment-loop webs are
  coloured in phase 2 in ascending web (expression-bit) order, not by priority. Each takes the lowest free
  t-register. Retail's assignment needs this order: segment (t2), lo[4] (t3), lo[0] (t4), the `car->level[i]`
  induction pointer (t5), then lo (s0) and lo[1] (s1).
- **Forced colours:** forcing exactly that on the w6c source
  (`force.sh e7 func_800E7FA0 0 "p2:w162=c9,p2:w296=c10,p2:w177=c11,p2:w268=c12,p2:w153=c14,p2:w165=c15"`)
  leaves **2 rows**. These are the order of `move t2,zero` / `move s0,t0`: retail initialises segment before lo.
  So the whole E7FA0 residual is web numbering, plus that one statement order.
- **Named bound:** a named bound `hi = lo[4]` placed after `lo = curve->level` gives lo[4] t3 (38 → 35).
- **What does not work:** lo and lo[1] cannot take numbers after the induction pointer while lo is a source
  variable assigned before the loop. The induction pointer gets number 268 only after `car` is
  copy-propagated.
  - These all change the structure (312–313 words): the indexed loop (`curve->level[segment]`), moving the
    `lo =` / `segment = 0` initialisations, and `for (; …)`.
  - Reusing `blocked`/`alternate` for segment or hi is much worse (67–135).
- Next hypothesis: make `car->level[i]` an early expression, for example through a `car` that is not
  copy-propagated or a level pointer that is a real variable (w6c tried the latter alone: 218). Alternatively make
  lo a late web while keeping its initialisation before the `hi` test.

## entity_update_callback — unchanged

I traced it (`runs/e0`) but stopped there for breadth. In phase 1, the `obj->car` load web (w5, 56/6 = 9.33)
beats the record address (w7, 26/5 = 5.2), so v0 and v1 are swapped throughout. Forcing `p1:w5=c2,p1:w7=c1`
gives 399 rows (as w7d found). The glow-handle region is also off: retail does not keep `&D_80154660[car]` in a
web there.

In the unit, the w7d source compiles to 545 words, where standalone it gave 546 (`full.py`). The next agent should
check whether an `align` entry or a trailing-nop difference explains this before trusting unit word counts.

## What generalises

1. **One variable = one live range, across loops.** Reusing a later variable as an earlier loop's counter
   raises its priority, because the range inherits the loop's weight. This is how retail made px/pz beat a
   pointer expression. In a colouring residual, try counter identities that include the variables that
   lose (`g1/gen1.py`).
2. **Arm temporaries:** a value assigned to a spilled variable and used in the same arm gets no register piece
   when the split pieces have save 0. Retail's `addiu v0; …; sw v0` is a temporary (`t = …; w = t;`). The
   symptom is `sw tN; move tM,tN` in ours.
3. **Phase-2 colouring follows web-number order.** In uopt phase 2 the decisions come in ascending web
   (expression-bit) order, and each web takes the lowest free register. First appearance in the source
   therefore decides who gets t2 versus s0. This is a different lever from priority, which governs only phase 1.
4. **ugen evaluation order for `and(load, not(shl))`:** splitting the mask into its own statement on a coloured
   variable (`i = ~(1 << i)`), with the address in a pointer statement first, gives address → shl → load → not.
5. **Tooling:** `tools/gu.sh` is unit score plus aligned rows for any member of a group. `tools/fdb.sh` gives
   batch aligned diffs on the builder (2 jobs). `force.sh` accepts `p2:` webs.

## Notes

- No permission denials.
- Builder: own scratch copy only (`~/rush2049/scratch/frontier/w10a`, `uopt` symlinked to the w5d build), at most 2 jobs.
- Nothing committed, spliced or edited outside `cloud/matches/func_80109468.c` and this lane directory.
