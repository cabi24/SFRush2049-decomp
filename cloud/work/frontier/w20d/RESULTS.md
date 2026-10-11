# w20d RESULTS (wave 20, game near-misses with small residuals)

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| assign_drones | 1236 | **MATCH** standalone + unit EQUAL (`cloud/matches/assign_drones.c`) | -g0 -O3 -mips2 -G 0 -non_shared | `assign_drones:\n  MATCH`; unit `EQUAL assign_drones: 309 words (kept, c_assign_drones.c)` / `locked bodies that differ in this unit: 0` |
| race_position_update | 484 | 50 words / 11 ops (was 76 / 27), frame 112 vs 120 | same | `FAIL race_position_update: 109 of 121 words differ; compiled body is 115 words, target 121 \| words 50 ops 11 norm 11 \| frame 120/112` |
| world_effect_update | 456 | 12 words (unchanged; boundary cause narrowed to a jump-to-next or label) | same | `FAIL world_effect_update: 106 of 114 words differ; compiled body is 112 words, target 114 \| words 12 ops 11 norm 11 \| frame 24/24` |
| audio_frame_update | 600 | 22 words (unchanged, w17i best.c) | same | `FAIL audio_frame_update: 22 of 150 words differ \| words 21 ops 12 norm 12 \| frame 40/40` |

Targets: world_effect_update, audio_frame_update, assign_drones, race_position_update.
Builder scratch: watchman2:~/rush2049/scratch/frontier/w20d (toolkit `--reuse wtk`). Nothing committed, spliced or pushed.
Helper: `tools/lst.sh NAME FILE LABEL` = `us.sh` + `snap.sh` + `as1t.sh` to fetch the ugen listing per variant.

## world_effect_update (456 B) -- not matched (stop rule), 12 words (w19e best.c), unchanged

New evidence (listing oracles on best.c's own ugen listing, `asm.sh b0 ed.s`):
- After `.frame`, these reproduce retail's head (114 words, 6 rows left = the msgq reload only): a label `$L:`;
  `b $L` / `j $L` / `beq $0,$0,$L` immediately followed by `$L:` (as1 deletes the jump-to-next but keeps the
  block boundary); `.set noreorder` + `.set reorder`.
- These do NOT: `.set volatile`/`.set novolatile` (empty, or around the direct-form `sw $0, D_8015F738`), `.bgnb`,
  `.loc 2 1`, `.livereg`, `.alias`/`.noalias`, `.set noat`/`.set at`, extra `.loc` lines.
- So retail's ugen listing had a branch target (or a jump-to-next as1 removed) right after the prologue and another
  between the msgq store and its reload.
Source forms measured (listing labels counted per variant; none adds a label at either point): folded ifs
(`i = 0; if (i)`, inlined `st(0)` with `if (x)`), `for (i = 0; i < 0; i++)`, `while (i)` after `i = 1`, `if/else`
on a constant, `switch` (default only / one empty case), `goto L; L:`, `if (c) goto L; L:`, `while (0)`,
inlined statics with explicit `return;` (empty, at the top; holding everything up to the msgq store; at the msgq
point with folded `if (x) return;`), identical-arm `if/else` around the msgq store (no tail merge: real branches),
long straight blocks (80 extra stores: uopt's own block splits emit no label). Loops around the head stores
(`do {stores} while (osTvType == 0)`, `while (osTvType == 0) {stores}`, outer `for(;;)` + `break`, `goto top`
after the wait loop) all hoist the stores out of the loop (label after the stores, not before).
`-O2`/`-O1`/`-g -O2` standalone: worse (108-113/114). Also no label or surviving jump from `do { continue; } while (0)`,
`do { break; } while (0)`, `for (;;) { break; }`, `while (1) { break; }`, `switch (0) { case 0: break; }`, a
conditional `continue` in `do/while (0)`, or the wait loop placed before the stores (the stores then follow it).
uopt removes every jump-to-next it creates in these forms; retail's must come from a branch uopt cannot see is to
the next block (block layout), which none of ~40 forms produced. Stopped (with w19e's ~3 h, well past the rule).

## assign_drones (1236 B) -- MATCH, `cloud/matches/assign_drones.c`

From w19e best.c (29 words / 7 ops rows). Two levers, both "a loaded value / address is its own web, copied into
the variable" (w19e's diagnosis), found by writing the expression where retail reads the web:
1. track init: `track = D_8014978C; track2 = D_8014978C; if (D_80152570) { track = D_8014978C + 6; track2 += 19; }`.
   The byte is loaded once (PRE) into v0, copied to track (ra) and track2's home; `+6` is formed from v0 (retail
   `addiu ra,v0,6`). `track2 += 19` (reading the variable) rather than `D_8014978C + 19` puts the sum in t8 (the
   variable read costs one ugen ring temp) and fixes the temp ring for the rest of the function (B7: 168 words/1 op
   -> C6: 20 words/1 op).
2. car: `car = &D_80152818[D_8014A118[i].car]; if (D_80152818[D_8014A118[i].car].finished) { time = car->time;`.
   The direct-index test makes the address an expression web (v0) that car copies (retail `move s5,v0`), which also
   gives the player/car pointer webs retail's s4/s5. Equivalent EQUAL forms: both reads direct (D1), both via
   `(&D_80152818[...])->` (D4). Pointer-arithmetic `D_80152818 + idx` (D5) is a different expression (11 ops).
No shaping devices, no unused locals, frame 136 = retail.

```
$ score.py fn cand/assign_drones.c assign_drones --flags "-g0 -O3 -mips2 -G 0 -non_shared"   (builder)
assign_drones:
  MATCH
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w20d score assign_drones --with cloud/matches/assign_drones.c --neighbours
  EQUAL assign_drones: 309 words (kept, c_assign_drones.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w20d/unit.o (5.3s)
```
Integration: plain single (`splice_singles.py assign_drones`); no own rodata (528.0f via lui/mtc1, 0.0f via
mtc1 zero), no unit_overrides, no groups superseded, no data-symbol changes. Callee func_800CD8EC is locked.

## race_position_update (484 B) -- not matched: 50 words / 11 ops rows (from 76 / 27), frame 112 vs 120

`race_position_update/best.c`. Levers so far (each from the retail listing):
1. The letter-spacing read is the locked getter `s8 func_800BDEB0(void) { return D_80149B70; }` (0x800BDEB0, a kept
   function in the same text module), inlined by umerge: the inlined return value is retail's `lb v0,0(s3)` web and
   the inline areas (loop rotation puts the condition at both the pretest and the bottom test) account for 16 of
   the frame bytes. G1: 76/27 -> 53/14.
2. The tail width is a variable and the loop condition is `maxw - tw - func_800BDEB0() < w`: retail's `move v1,v0`
   (width result live across the getter's v0) and the hoisted invariant `maxw - tw` in s2. -> 51/11.
3. Loop truncation as direct indexing `dst[n * 2 + 1] = 0; dst[n * 2 + 2] = 0;` (retail `addu v1,s1,t6`, dst first;
   `&dst[n * 2]` / `dst + n * 2` / `p = ...` give `addu v1,t6,s1`). -> 50/11.
4. `u8 *p;` declared before `tail[20]` puts tail 4 bytes below the top of the local area as in retail (96..115 with
   116 unused once the save area is retail's size).
Remaining (all one cause): retail saves s0-s6; s4 holds -1 (`li s4,-1` in the loop preheader, then
`sll a1,s4,16; sra t8,a1,16; move a1,t8` at the loop call), s5 is saved but never written, wide is s6. Every
constant/variable form of the -1 argument folds to `li a1,-1` (literal spellings x12, s16/s32 `lim` variables assigned
once, before the loop, before the loop and again in the loop, `register`, an inlined `text_width(u8 *, s16|s32)`
wrapper for the loop call). uopt's makelivranges lists the call-argument -1 as `ctx=0 remat`; retail must have the
-1 as a LIVE-RANGE operand of an unfolded CVTL (coloured s4), i.e. a value uopt does not know is constant at
copy-propagation time.

Trace of the -1 (decoded `f_constinreg`, uopt.trace.c 193816): a constant gets a register live range only for
ctx 3 (indirect store / compare operand), ctx 2 with a negative value (logical op), ctx 5 when not a power of two
(multiply), or values outside 16 bits; ctx 0 (call argument / direct store) and ctx 1 (add) are always remat, and 0
never. So retail's `li s4,-1` + `sll/sra s4` is not a constant web: it is a coloured *variable* (def `li s4,-1` in
the loop preheader, use CVTL(var) at the call) that copy propagation did not replace. Measured:
- every variable assigned a constant (once, twice, equal constants on several paths, a copy chain `k = -1; lim = k;`)
  is propagated (C1/C2/S1-3/U1-3/U6-7: 11 ops);
- an uninitialised path blocks propagation (U4: `lim = -1;` after the loop call gives the sll/sra but adds a home load);
- **the value returned by an inlined function is not propagated** (`oracle_inline_lim.c`, = Y2: `lim = all_chars();`
  right before the loop, `static s32 all_chars(void) { return -1; }`):
  `FAIL race_position_update: 114 of 121 words differ; compiled body is 119 words, target 121 | words 29 ops 4 norm 4 | frame 120/120`.
  The loop is then retail's except register numbering: ours lim s3 / spacing address s4 / wide s5 and 6 saved
  registers; retail spacing s3 / lim s4 / (unwritten) s5 / wide s6 and 7 saved registers. NOT a proposal: there is
  no caller-less stub for such a helper, and its inline area makes the frame 8 bytes too big once the seventh
  register is saved (retail's local area is 64 bytes = p 4 + tail 20 + w/avail/n/wide 16 + two getter inline areas
  16 + two temps 8; Y2's is 72).
Best next hypothesis: the -1 is the return value of an inlined *locked* helper that already accounts for one of the
two inline areas (i.e. only one of the two areas is the spacing getter), and whatever else it returns/defines is the
unwritten s5 web. Candidates: a kept text-module function returning a length/limit; check umerge.log of the unit for
what umerge can inline into race_position_update.

## audio_frame_update (600 B) -- not matched (stop rule), 22 words (w17i best.c), unchanged

Re-scored in the current unit: w17i best.c 22 words / 12 ops, f2.c 25 / 8, w16f b0.c 21 / 16 (same as w17i).
New measurements (all no better):
- second counters alongside i (`j = 6`/`j++` s32 and s16, `for (i = 0, j = 6; ...)`, `j = i + 6`, loop over
  `j = 6..9` with `i = j - 6`): 30-34 words; `j = i` copy is propagated (= best).
- holders `if (x) {}` at the loop bottom of f2 for dst / t / car / m / slot: 79 / 44 (frame 64) / 27 / 24 / 27 words;
  the m holder colours m (a0) and shifts the loop webs the wrong way.
- What the v1 web cannot be (decoded `f_constinreg`): a constant. 0 is never a register candidate; the loop's 1 and
  4 are add-immediate context (always rematerialised); 24 is already the a0/a1 web. It cannot be an inlined
  helper's return value either (the 40-byte frame has no inline area). So it is a variable or expression web live in
  blocks 16-17 only (it must not interfere with the v1 webs of the copy block), decided before i (save > 30.5).
Best next hypothesis unchanged from w17i (a loop-carried value with ~6 weighted loop occurrences that ugen never
materialises); `boost.sh` from w17i is the oracle.

## What generalises
- **The loaded value / address as its own web, copied into a variable** (assign_drones, two places): when retail
  computes into v0 and copies (`move ra,v0`, `move s5,v0`), write one use as the expression (`D_8014978C + 6`,
  `D_80152818[x].finished`) and keep the variable for the rest. `x += N` reading the variable vs `D + N` re-reading the
  global costs/saves one ugen ring temp and shifts every later temp.
- **Kept one-line getters are inlined by umerge** (func_800BDEB0 = `return D_80149B70;` inlined twice into
  race_position_update): look for them when retail reads a global into v0 next to an unexplained 8-byte-per-call
  frame residual. `umerge.log` in the unit stage dir lists every inline per caller.
- **A value returned by an inlined function is not copy-propagated** even when it is a literal constant: it becomes a
  coloured variable (li sN in the preheader, CVTL at an s16 use). Variables assigned constants on every path (once,
  twice, through a copy chain) are always propagated.
- **`f_constinreg` decoded** (uopt.trace.c 193816): constant register candidates only in compare / indirect-store
  context (ctx 3), logical ops with negative values (ctx 2), non-power-of-two multiplies (ctx 5) or out-of-16-bit
  values; call arguments, direct stores and add-immediates (ctx 0/1) are rematerialised; 0 never.
- **as1 deletes a jump-to-next but keeps the block boundary** (world_effect_update listing oracle), so a residual that
  needs an as1 block boundary can come from a branch uopt leaves pointing at the next block, not only from a label.
- Direct indexing `dst[n * 2 + 1]` gives `addu rd, dst, n*2` (dst first); `&dst[n * 2]`, `dst + n * 2` and a
  pointer local give the mpy operand first.

No permission denials. Builder: only `~/rush2049/scratch/frontier/w20d` was used (2 jobs).
