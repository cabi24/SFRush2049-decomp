# func_8008E26C (0x8008E26C, 300 bytes) — not matched: 34/75 strict, 20 aligned

Best: `best.c` (needs `find` internal: `bscore.py --keep func_8008E26C`, or the whole-program unit).
Earlier state: 68/75 strict, 27 aligned (`cloud/work/near_miss_B47`, kept here as `v0.c`).

## What retail shows that a plain single function cannot produce
1. All four parameters are copied to `t5/t4/t3/t2` before the loop.
2. `sw v1,32(sp)` just before the `jal`: the loop index is written to a stack slot that is never read.
3. The narrowed index is kept in a word at `sp+24` across the call and returned with `lw v0,24(sp)`.
4. `i*68` is formed in `a3` (a coloured register) right at the loop exit, before the two `if`s; the array
   base is added after them (`addu v0,a3,t6`).

## What reproduces 1–3 (measured)
- The index is an **address-taken local written through the out-parameter of an inlined static**:
  `static void find(s32 *p) { loop; if (i == count) count++; if (max < count) max = count; *p = i; }`
  and `find(&i)` in the function. That alone gives the four parameter copies, the 40-byte frame and the
  dead store (`tried/b6_D3.c`, 36 words). Without an address-taken index no variant produced the store
  (about 700 variants of declarations, prototypes, return types, wrappers returning the index).
- The return type must be `s32` and the saved value a word local assigned `(s16)i`
  (`idx = (s16)i; render_mode_select(i, c); return idx;`): that gives `sw t7,24(sp)` / `lw v0,24(sp)`.
  With `s16 idx` the slot is a halfword (`sh`/`lh`); with `return i` the index is reloaded and narrowed
  after the call.
- Four word locals in the order `pad, i, e, idx` put the index at `sp+32` and the saved value at `sp+24`.

## Residual (the 34 words)
Only fact 4. Here the whole address `&D_8012E700[i]` is formed after the `if`s in the temp ring
(`t6`, `t7`), so the two sign extensions before the call use `t8/t9` where retail uses `t7/t8`, and `li -1`
lands in `a3` instead of `a2`. Workbench `diagnose` (target assembled from the retail words): verdict
`structure-mismatch`, pool lane diverges at slot 4 — retail has one more coloured web (`a3`).
Not found by: the entry pointer computed inside `find` before or after the `if`s and returned; a pointer
variable as array base (in `find`, as a `find` argument, in the caller); `e = D; e += i`; direct
`D_8012E700[i].field` stores without `e`; `while`/`goto`/pointer loop shapes; the `if`s in the caller.

## Best next hypothesis
Something between the loop exit and the first `if` uses `i * 0x44` with a base that is not the array
constant, so uopt keeps the product as its own value. Candidates not yet tried: the finder returning a
byte offset or taking the record size; a second out-parameter for the entry pointer whose store is dead;
the static being the allocator of a *generic* pool (base and element size as arguments) called here with
`D_8012E700`/`0x44`. The out-parameter itself says the real helper returns something else as its value
(a success flag or the entry pointer) and the index through the pointer.
