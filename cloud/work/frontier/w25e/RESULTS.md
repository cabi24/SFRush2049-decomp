# w25e — game fresh singles and lighter near-misses (IDO 5.3), wave 25

Status: done. One standalone MATCH delivered (`cloud/matches/func_8008AD6C.c`); everything else is research.
Builder scratch watchman2:~/rush2049/scratch/frontier/w25e (toolkit reused from wtk; my st_* snapshots removed at the
end because the builder root filesystem hit 100% / "No space left on device" from other users). No permission denials.
Nothing committed, spliced or pushed. Variants are under `<fn>/v/`.

| Function | Bytes | Before -> after | Scorer line (whole-program unit unless noted) |
|---|---:|---|---|
| func_8008AD6C | 164 | 29/41 -> **MATCH** | `score.py fn`: `func_8008AD6C:\n  MATCH`; unit `EQUAL func_8008AD6C: 41 words (kept, c_func_8008AD6C.c)` / `locked bodies that differ in this unit: 0` |
| func_800A1BB4 | 184 | 32/46 (keep) -> 9 rows, provisional (internal; stand-in caller) | `FAIL func_800A1BB4: 29 of 46 words differ; compiled body is 42 words, target 46 \| words 9 ops 6 norm 6` / `locked bodies that differ in this unit: 0` |
| race_position_update | 484 | 50 words (unchanged deliverable); oracle 25-29 words, frame exact | `FAIL race_position_update: 109 of 121 words differ; compiled body is 115 words, target 121 \| words 50 ops 11 norm 11 \| frame 120/112` |
| ghost_race_setup | 440 | 16 words unchanged; as1 cause narrowed (not a label, not livereg) | `FAIL ghost_race_setup: 16 of 110 words differ \| words 14 ops 11 norm 11` (w24f best_words.c) |
| func_8010FBE0 | 128 | 6 rows unchanged | `FAIL func_8010FBE0: 15 of 32 words differ \| words 14 ops 8 norm 8` (s7 + osSendMesg) |
| entity_process_main | 1424 | 2 words unchanged | `FAIL entity_process_main: 2 of 356 words differ \| words 4 ops 0 norm 0 \| frame 280/280` |
| world_effect_update, audio_frame_update, camera_play_script | | unchanged (re-scored, one probe each at most) | 12 words / 27 rows (f2) / 27 rows |

## ghost_race_setup (440 B): 16 words unchanged; the as1 hoist is not a block-boundary or livereg effect

Start: w24f `best_words.c` re-scored (osSendMesg): `FAIL ghost_race_setup: 16 of 110 words differ | words 14 ops 11 norm 11`;
w24f `best.c` (inlined func_800960CC + `do {} while (0);`): `53 of 110 ... compiled body is 109 words | words 19 ops 11`.
Both residuals are as1-level (ugen listing already has retail's shape):
1. `move a0,v0` / `sw zero,0(s4)` order and `lw t7,0(a0)` (retail) vs `lw t7,0(v0)` (ours): the listing reads
   through `$4`; as1 renames the base to `$2` and moves the store up.
2. Release arm: as1 fills the `lb t6` load hole and the beqz delay slot with `move a0,s5; move a1,zero; li a2,1`
   from the release block; retail leaves the hole and uses `beqzl` + `lw s0,76(s2)`.
Listing oracles on our own listing (`asm.sh b0`, build/trace/w25e/o*.s):
- o1 label after `lw $16` (w24f's q3 idea): beqzl + lw s0 appears, but the two moves are STILL hoisted into the
  lb hole (8 rows). So as1's hole fill scans past the block boundary; a boundary alone cannot explain retail.
- o2 label after `move $4,$2`: worse (18 rows); o3 both: 12 rows.
- o4 `.livereg 0x0E00000E` before the ready arm's `b`: no change (as1 ignores it for this decision).
Source forms measured, all identical to best.c (53 words / 19 rows): `handle` holders in the ready arm / after the
if-else (h1-h3), a named `addr` with `do {} while (0)` / `if (addr) {}` before the call (a1-a3: the `do {}` emits no
label there), inlined getter/release helpers standing for the stub func_800D52C4 (g1-g4), eight line layouts (L1-L8,
line numbers do not move either residual).
Next: as1's hole-fill legality test for target-block instructions (dest registers dead on the fall-through path):
retail must have a0/a1 live (or the instructions unmovable) along the ready arm. Needs an as1 that logs the hole-fill
candidate checks (the stock as1 trace only logs per-block picks).

## race_position_update (484 B): 50 words (w20d best, no device) unchanged as a deliverable; oracle frame now exact

w20d best.c re-scored: `FAIL race_position_update: 109 of 121 words differ; compiled body is 115 words, target 121 | words 50 ops 11 norm 11 | frame 120/112`.
Measured (all with w20d's inline-return lim oracle, which has no caller-less stub and so is NOT a proposal):
- V2 inline call as the loop argument (`object_manager_update(dst, all_chars())`, no named lim): folds to `li a1,-1`
  (25 words / 11 ops, frame 120/120): the inline return is propagated when it is used directly as an argument;
  only `lim = all_chars();` assigned to a variable before the loop keeps the CVTL web (w20d Y2, 29 words / 4 ops).
- `for (;;) { if (... >= w) break; ...}` single-test loop (V1/V3/V4): frame 112 (one getter inline area fewer:
  the front end duplicates the while condition, so each getter call site costs 8 B), 86 words.
- lim in p's slot with p removed (W1/W2, `(&dst[n * 2])[k]`): identical to Y2 (29 words / 4 ops, frame 120).
- helper shapes (local r, `n < 0 ? -1 : n` with a folded compare, s16 return, s16 lim) X1-X6: identical.
ctrace (W1): n s0 (save 8.8), dst s1 (4.6), maxw-avail s2 (4.0), lim s3 (2.75, tot 11 / 4 blocks), &D_80149B70 s4
(2.5, tot 10 / 4), wide s5 (2.0). Retail order is spacing s3 > lim s4 > X s5 (codeless) > wide s6, so retail needs
a web X with 2.0 < save < lim's and spacing above lim. lim's def must stay after the avail call (retail's
`li s4,-1` is in the loop pretest's delay slot), so its save is fixed near 2.75; the lever must raise spacing or
add X. No locked game function returns -1 (searched every target of <= 4 words), so the -1 is not a kept getter.
Next: identify X (a codeless coloured web: per w24f, a constant in a folded compare inside an inlined helper keeps
a web) — the helper would need a caller-less stub, and there is none next to this function.

## func_8010FBE0 (128 B): 6 rows unchanged (w16e/w20e best); no new lever

w20e s7.c (defined `OSScTask D_80155238;`) with osJamMesg -> osSendMesg (0x800075E0): `FAIL func_8010FBE0: 15 of 32
words differ | words 14 ops 8 norm 8 | frame 24/24`. ctrace: the address web w6 (type 1) has tot 3.0 over 2 blocks
(the jam argument after memcpy extends it) vs best cost 2.1 (a3) -> coloured; w15d's force to split gives 0 rows.
Compiled-out block ends (`do {} while (0);` after next / after msg / after flags / before memcpy / two of them,
F1-F5): all identical (uopt drops them here; no block boundary appears). A web confined to block 0 would cost 0 and
still be coloured, so the lever has to lower tot below the cost, i.e. one istr use fewer through D_80155238, which
every spelling tried by w14za/w15d/w20e/w25e merges back into the same ldaS web. Note: w20e's best.c still calls
osJamMesg; any future draft must use osSendMesg.

## func_800A1BB4 (184 B): 32/46 -> 9 rows (provisional lane: the function is INTERNAL); `func_800A1BB4/best.c`

Finding: retail's loop uses the four-wide t6-t9 temp ring, the plan's internal-function signature. In keep mode every
draft (w24f base/r3 included) gets t1-t5 temps; with `--internal func_800A1BB4 --block func_800A1BB4` and a stand-in
caller (`zz_caller_a1bb4`, two calls) the ring is retail's. Its only real caller, track_data_decompress (two `jal`
sites at +0x224/+0x270, v0 ignored), is unmatched, so this is provisional at best.
Levers (each measured): (1) the list walk written inline instead of w24f's static `find_enabled` (there is no
caller-less stub near 0x800A1BB4, so a static helper cannot be retail's): rec in v0 and list/p in v1 as in retail
(n2, keep mode 24 rows); (2) `next = p = D_80144D68[index].head;` (p2 web numbers p < next < data; plain order gives
data a1 / next a2, `next = 0;` first gives next v1): retail p v1 / next a1 / data a2.
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w25e score func_800A1BB4 --with cloud/work/frontier/w25e/func_800A1BB4/best.c \
  --internal func_800A1BB4 --keep zz_caller_a1bb4 --block func_800A1BB4 --neighbours
  locked bodies that differ in this unit: 0
us.sh: best.c: FAIL func_800A1BB4: 29 of 46 words differ; compiled body is 42 words, target 46 | words 9 ops 6 norm 6 | frame 0/0
```
Left (4 words of control flow, 9 aligned rows): retail's separate early `bnez t7; jr ra; nop` (ours branches to the
shared exit) and the found path `beqz t8,next; nop; b exit; nop` (ours `bnez t6,exit`). A holder `if (p) {}` before
`break` gives the `b exit` and body length 46 but emits a real `beqz v1` (k6, also 9 rows): not adopted.
**Why the non-void form is not adopted:** the callers ignore v0, and every value-returning form is worse with the
right ring: `s32` return 0 / 1 (j3) 10 rows / 7 ops, `return rec` / `return &D_80144030[index]` (j1/j2) 22 rows
(rec loses v0: the return's v0 forbids it, force.sh `p2:w5=c1` declined `forbidden=0x50000000`), and `return 0`
emits `move v0,zero` where retail has `nop`. The duplicated epilogue must come from a void form whose early-return
block survives to ugen empty (the plain `return;` is threaded to the exit).
Next: find the empty-but-surviving return block (e.g. a structure where uopt keeps `bne; b exit` and as1 replaces
the `b` by a copy of `jr ra; nop`), then package with track_data_decompress.

## func_8008AD6C (164 B): 29/41 -> **MATCH** (standalone + unit EQUAL), `cloud/matches/func_8008AD6C.c`

F3DEX2 winding flip: G_TRI1 (0x05) swaps the last two vertex-index bytes of word 0, G_TRI2/G_QUAD (0x06/0x07) of
both words; returns 2. Traced lever: w24f's diagnosis (retail keeps hi/lo/mask as coloured webs, ours propagates
hi/lo into ugen temps) holds for any form with a word local `w`: `x = w & 0xFF00` is substituted into its single use.
Measured: pieces of word 0 taken before `w = gfx[1]` (A1, 24 words: word-0 pieces coloured, the rest not); reading
memory instead (`x = *gfx & 0xFF00; lo = *gfx & 0xFF; *gfx = (*gfx & 0xFFFF0000) | ...`, C2): ops 0, only v0/v1
swapped (the opcode variable `x` is web 0 and takes v0, the load expression v1). Testing the opcode as
`(*gfx & 0xFF000000)` three times (no opcode variable, D4): the load web is first and takes v0 -> EQUAL.
`score.py fn` (builder, -O3 flags): `func_8008AD6C:\n  MATCH`
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w25e score func_8008AD6C --with cloud/matches/func_8008AD6C.c --neighbours
  EQUAL func_8008AD6C: 41 words (kept, c_func_8008AD6C.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal
```
Integration: plain single (`splice_singles.py func_8008AD6C`), no own rodata, no overrides, no data symbols, no
shaping devices, no unused locals.


## entity_process_main (1424 B): 2 words unchanged
Retail order is shl (t9), load (t6), not (t7); ours shl, not, load. A block-local bit variable (`pad1 = 1 << i;` —
pad1 is the disclosed unused frame local — then `~pad1 & poly->flags`, u2) is propagated back into the tree: same 2
words; `poly->flags & ~pad1` / `&=` / bit computed before `poly` (u1/u3/u4): 21 words; `i = 1 << i` (s16 i): 20-34.
Every form reaches ugen as one tree, so w14o/w20e's conclusion stands.

## world_effect_update, audio_frame_update, camera_play_script
- world_effect_update: w19e best re-scored (12 words). Probe: an inlined empty KEPT function (func_800D52C4) at the
  top or after the 90.0f store (umerge inlines it) gives no block boundary (s1/s2, identical). w20d's ~40 forms stand.
- audio_frame_update: w17i f2 re-scored (`words 27 ops 8`); no new hypothesis tested (the v1 web is still unknown).
- camera_play_script: w17a best re-scored (`words 27 ops 1 norm 1 | frame 608/608`). Retail's corner pass keeps the
  `&pts[k]` IV in t0 and copies it into a1 (`move a1,t0`, spilled at 148); ours colours the IV a1. Not worked further.

## What generalises
- **Read through memory to keep single-use pieces coloured** (func_8008AD6C): `x = w & 0xFF00` from a word local is
  substituted into its only use (ugen ring temps); `x = *gfx & 0xFF00` keeps x a coloured variable because the load
  expression is not propagated. Retail register pairs like `andi v1,v0,..` / `andi a1,v0,..` feeding `srl/sll` temps
  are the signature.
- **Expression webs are numbered after variables in p2** — an opcode variable (`x = *gfx & 0xFF000000`) took v0
  ahead of the load; writing the test as the expression itself let the load web take v0.
- **Check the temp ring before tuning colours**: a four-wide t6-t9 ring in retail with t0-t5 temps in ours means the
  function is internal (func_800A1BB4); score it with `--internal NAME --block NAME` and a stand-in caller.
- **`next = p = head;`** fixes p2 web order (p < next < data) without code: a chained assignment's leftmost target
  gets the later web number.
- A value-returning form forbids v0 to webs live at the return; the return-value precolour is the reason `return rec`
  loses rec's v0 (force declined, `forbidden=0x50000000`).
- as1 fills a load hole from the branch target past a label (ghost_race_setup o1): a block boundary alone does not
  stop the cross-block hoist.

## Integration
- `splice_singles.py func_8008AD6C` (plain single, no own rodata, no unit_overrides, no groups superseded, no data
  symbols, no shaping devices, no unused locals).
- Nothing else is installable. func_800A1BB4 is internal: when track_data_decompress matches, put both in one group
  (func_800A1BB4 member, forced internal).
- Note: `cloud/matches/func_800EC190.c` is untracked in the tree but is not from this lane.
