# Wave 14, lane w14i: place_cars_in_order (continued from w14c), assign_drones (draft)

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w14i` (copied from base; src/blob, include, tools/cloud,
asm/us/blob and blob_matched.lock.json synced from the Pi). Trace toolkit installed there (`install.sh`, fidelity 9/9 identical).
Nothing spliced, committed, pushed or written to cloud/matches (no strict standalone match was reached).

| Function | Bytes | State | Best candidate | Scorer / unit output |
|---|---:|---|---|---|
| `place_cars_in_order` | 544 | 9 of 136 words differ (standalone and in unit); NOT a match | `pc/place_cars_in_order_best.c` (= `pc/b6/v0005.c`) | see below |
| `assign_drones` | 1236 | draft only: 299/309 standalone; 283/309 in unit (frame 136/144) | `ad/assign_drones_v3.c` | see below |

Flags for all: `-g0 -O3 -mips2 -G 0 -non_shared`.

## place_cars_in_order

Best (`pc/place_cars_in_order_best.c`), scorer output (`score.sh`, standalone):
```
python3 tools/cloud/score.py fn cand/place_cars_in_order.c place_cars_in_order --flags '-g0 -O3 -mips2 -G 0 -non_shared'
  +0x140 want 46103480 add.s $f18,$f6,$f16     got 46103000 add.s $f0,$f6,$f16
  +0x150 want 46009124 cvt.w.s $f4,$f18        got 460004a4 cvt.w.s $f18,$f0
  ... (FP sum in f0 instead of f18; 5 more rows of the same residual)
  +0x1d0 want 30c500ff andi a1,a2,0xff         got 00c02825 move a1,a2
  9/136 words differ
```
Unit (`blob_unit --tag w14i score place_cars_in_order --with pc/place_cars_in_order_best.c --neighbours`):
```
  locked bodies that differ in this unit: 0
blob_unit score: 0/1 equal; object build/blob_unit/w14i/unit.o (4.2s)
```
`tools/trace/us.sh`: `FAIL place_cars_in_order: 9 of 136 words differ | words 9 ops 1 norm 1 | frame 72/72`.
Only the FP sum colour and the (u8)s7 call argument remain. The `ops`/`norm` levels are 1 row each.

Trace evidence (`ctrace.sh`, `force.sh`):
- `p1:w55=c28` (sum to $f16): 5 word rows, but the quotient moves to $f18.
- `p1:w55=c29` (sum to $f18, the retail register): sum is right, but the quotient `dist/528` moves to $f4 (ugen FP temp), not $f16.
- So the FP residual is not a pure colour problem: the quotient is an ugen FP temp chosen from the free list after the sum's colour is fixed. Next hypothesis: make the quotient a named FP local (or drop the sum's name) so it takes $f16 while the sum takes $f18.
- Key/base colours (earlier residual): with `Car *cp` local the car base and the key agree with retail on the int side (`lw`, `bgez`, `addu`); the remaining int differences are gone at 9 words.

Round log (template, variants, best three strict counts; all standalone via vbatch/bscore):
| Round | Template (choice points) | Variants | Best strict (top 3) |
|---|---|---:|---|
| b1 | `pc/t1.c`: decl order, `D_80152570` cond, 96*s6 form, 4 key-dist forms (v26 form, mixed) | 48 | 71, 71, 71 |
| b2 | `pc/t2.c`: 7 key/dist forms incl. two-branch forms (dist read in each branch) | 56 | 71, 71, 71 (two-branch forms 118-151) |
| b3 | `pc/t3.c`: + off hoist, load-order forms | 80 | 71, 71, 71 |
| b4 | `pc/t4.c`: uk-form fixed, `cp` pointer choice, hoist | 64 | 71, 71, 71 |
| b5 | `pc/t5.c`: pointer-based forms (cp->dist) | 96 | 33, 33, 33 (K=cp forms; frame 72/80 with the unused cp slot) |
| b6 | `pc/t6.c`: cp placement before/after the p block (CP/CQ), off/96*s6/s6*96, 5 key forms | 480 | 9, 9, 9 (`b6/v0005.c` best) |
| b7 | `pc/t7.c`: + u8 call-arg temp `sb`, store forms, q-form | 800 (of 6912, seed 3) | 9, 9, 9 |
| b8 | `pc/t8.c`: single-statement store form (Z), CA/ST/CP choices | 800 (of 2304, seed 5) | 9, 9, 9 |
Total place_cars variants scored: 2424. Stopped: 3 rounds (b6-b8) with no movement below 9.

Fidelity of the trace tools for this lane: `fidelity.sh` gives `fidelity: 9 identical, 0 different`.

## assign_drones (draft, not iterated to a match)

`ad/assign_drones_v3.c` (= w14c `ad/v2.c` plus the pass-1 side-array shifts in the unrolled insert):
- the pass-1 shifts are `D_80151AC0[10+j] = D_80151AC0[9+j]` (bytes) and `D_80151690[60*s6+40+4j] = D_80151690[60*s6+36+4j]` (words),
  both guarded by `pass == 1`, read off the retail insert at 0x800F5184-0x800F52B0 (word side array index e at `60*s6+40+4e`, same
  element index as the float array `p+0x2C+4e`);
- unit (`us.sh assign_drones`): `FAIL assign_drones: 283 of 309 words differ; compiled body is 293 words, target 309 | words 255 ops 118 norm 118 | frame 136/144`;
- the frame is 8 bytes too large (144 vs 136); retail uses `ra` as a data register (`move ra,v0`, `sw ra,60(sp)`), which the draft does not reproduce.
- A 32-variant frame round (`ad/t1.c`, toggling `t0`, `cur/incoming` scope, `pass`/`kk`, `car` decl) gave 299 standalone at best, no movement.

Next step for assign_drones: fix the frame first (the `ra` data-register use and the unused `t0`, which is a frame residual candidate), then
run the unit score on each batch round (standalone ranking does not track the unit: v3 is 299 standalone and 283 in unit).

## What generalises
- For place_cars_in_order the big win was a named `Car *cp` local for the car base (plus placing its assignment after the p block): it took the walk from 71 to 9 words. The same record-walk idiom should be used in graphics_chunk-family functions.
- Standalone bscore and in-unit `us.sh` disagree on large functions (assign_drones 299 vs 283). Confirm top candidates in the unit before trusting a ranking.
- ugen FP temps are not colours: forcing an FP web moves the un-named quotient temp too, so an FP residual needs a named quotient local to test.

## place_cars_in_order, round 2 (coordinator follow-up): call argument and named quotient

Best now `pc/place_cars_in_order_best.c` (= `pc/clean.c`, no unused locals, the `cp` win kept):
- Standalone: `python3 tools/cloud/score.py fn cand/place_cars_in_order.c place_cars_in_order --flags '-g0 -O3 -mips2 -G 0 -non_shared'` gives `8/136 words differ`.
- Unit: `blob_unit --tag w14i score place_cars_in_order --with pc/place_cars_in_order_best.c --neighbours` gives `locked bodies that differ in this unit: 0` and `blob_unit score: 0/1 equal`.
- `us.sh`: `FAIL place_cars_in_order: 8 of 136 words differ | words 8 ops 0 norm 0 | frame 72/72`.
- Still NOT a match. Strict is 8, not 0, so nothing goes to cloud/matches.

Call argument (item 2): solved. `func_800CD8EC` is prototyped `void func_800CD8EC(void **arg0, u8 arg1);` (as in graphics_chunk.c, line 25) and the call passes `(u8) s7`. The `andi a1,a2,0xff` row is gone. Results: the s32 prototype with `(u8) s7` stays at the 9-word level, the u8 prototype with `s7`, `(u8) sb`, `sb` (u8 or s32 `& 0xFF`) are all 8 to 14 words.

Round log (all standalone bscore, 800 sampled from each product):
| Round | Template | Variants | Best strict |
|---|---|---:|---|
| b9 | `pc/t9.c`: prototype u8/s32, call-arg forms (u8 s7, s7, u8 sb, sb, (u8)(s7&0xFF)), sb typed u8/s32/s16, q-local on/off, 6 key-dist forms incl. q-named | 800 of 57600 | 8, 8, 8 (`b9/v0012.c`, mnem-missing 0) |
| b10 | `pc/t10.c`: as b9, q declared always, q-named after key cvt (`g` temp forms) | 800 of 57600 | 8, 8, 8 |
| b11 | `pc/t11.c`: two-branch named-quotient forms (`if ((s32)*(u32*)(p+88) < 0)` with dist in each branch) | 800 of 57600 | 14, 14, 14 |

Per-option results: q-named `f = (f32)uk + q` (q computed before the key load) gives 20-38; the form `f=(f32)uk; q=dist; f=f+q` gives 14 and puts the cvt into the sum (wrong). Inline `cp->dist/528` (no q) gives 8.

Trace evidence (`ctrace.sh`, `force.sh`, fidelity 9/9 identical):
- Inline best (`b9v12`, FP sum web `w53`): `p1:w53=c29` gives `differing rows 0 (norm)` but `9 (words)`. The sum moves to $f18 and the quotient `dist/528` moves to $f4 (not $f16). The quotient is an ugen FP temp, so it cannot be forced.
- Named quotient (`b9v44`, `b10` forms): forcing quotient to c28 and sum to c29 gives 0 norm rows only in the `g`-temp form (`b10k0`), and then `cvt` lands in $f16 (the temp becomes named), so 9 word rows remain (FP temp ring $f4 vs $f8).
- Conclusion for item 1: the named-quotient hypothesis is not confirmed. Naming the quotient is only correct when the key cvt is a ring temp and the dist load sits after the key sign test (retail `bgezl` in the delay slot), and no C form I tried gives that load order with a named quotient. The next hypothesis is the two-branch form with the named quotient where the cvt lands in a ring temp; the b11 round did not reach it cleanly.

Stopped after 3 rounds with no movement below 8 (per the 3-round rule). Total place_cars variants this session: 2424 (w14i round 1) + 2400 (rounds 9-11 sampled) = 4824 standalone.
