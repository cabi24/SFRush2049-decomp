# Wave 16, lane w16d: camera_victory (+ func_800C3AD0), then func_800E4B58

Status: camera_victory MATCH (unit EQUAL + score.py group all members MATCH); group `groups/func_800AD4C8/`
closes provisional func_800C3AD0. func_800E4B58: not matched (stopped; write-up below).

## camera_victory (1,468 B)

Start: w15f `camera_victory/best.c`, re-scored in the current unit: `FAIL camera_victory: 250 of 367 words differ`
(150 word rows, 34 mnemonic rows, frame 584/584). `diagnose one` standalone is meaningless here (no IPA context for
func_800C3AD0's register parameters): 365 words, "mixed(constant:23, structural:130, register:181)".

### Lever 1 (measured): loop 1's counter is the same variable as loops 2/4's outer counter
`b1.c` = w15f best with loop 1 written `for (j = 0; j < 4; j++)` (j = loop-2/loop-4 outer counter, sp+264) and
loop 2's inner counter `i` (loop 1's inner search keeps `k`): 243/367, 137 word rows, 32 mnemonic rows. This gives
retail's `slti at,s2,16` loop-1 exit test (w15f's open item) and retail's s1/s2 for the pos-pointer / counter IVs.
12 sharing combinations measured (`v/s_*.c`); s_jii and s_jik best (243/137).

### Traced cause of the car residual (uopt live-range splitting) -- FIXED
Private instrumented uopt (`tools/patch_spl.py` on the toolkit's uopt.trace.c -> builder `bin/uopt_spl`; env `SPL=1`
prints every f_split start block, BFS visit and addadjacents add/reject; `SPLF=1` prints livbb creation order;
`SPL_SKIP=a,b` is an oracle that skips start blocks). Findings (uopt `f_split`, `f_addadjacents`,
`f_is_cup_affecting_regs`):
- A p1 split (decision=split) grows the new piece from a start block by BFS over successors, and does not expand
  past a block that ends in a call (every CUP ends a uopt block and every CUP counts as "affecting regs").
- The start block is the first livbb in the live range's list with a first-is-store/entry flag (f20), else the
  first with occurrences. The list is in makelivranges' block walk order = DFS preorder over successor lists.
- car (sym 7): first split {bb0..bb9} (stops at the handbrake_apply call). In w15f's source the next start is the
  loop-2 camera_play_script call block (first occurrence in DFS order), a one-block piece with -ve save, then
  C36A0 (42), C3AD0 (47), steering_sensitivity (49) ... until the remainder has -ve save: car never coloured.
- Oracle `SPL_SKIP=34` (skip the camera_play_script block as a start): splits go {0..9}, {42}, {47} and the
  remainder is coloured -- exactly retail's reload pattern (+160, +330, +3f4, +4e0).
- Source lever: write loop 2's inner test with `continue`
  (`if (col.poly == p || (p->type & 0x4000) || (p->type & 0x2000) || (p->type & 0xF) == 15) continue;`). The call
  block becomes the branch-taken successor, so the DFS reaches the C36A0/C3AD0 blocks first; car gets s4 as retail.
  `b2.c`: 222/367, 121 rows.

### Lever 3: variable sharing (loop 1 outer `k`, loop 1 search `i`, loop 2 inner `i`)
On top of b2, 12 sharing combinations: `h_kii.c` = `b3.c`: **75/367 words, 74 rows, 20 mnemonic rows**; j now
coloured (t1, retail a1). Loop 1 exit test is `li/bne` again (slti only appeared with `j`).

### Lever 4: `bound` tested through the variable (FP registers, scheduling)
`bound = pt[1]; if (bound < -5.0f || bound >= 0.0f) continue;` (instead of testing `pt[1]`): bound becomes a
coloured variable web, retail's `lwc1 $f0,180(sp)` reload is hoisted above the `w[1]` test, the `neg.s` uses it,
the final loop's FP temp ring lines up and `swc1 $f28,180(sp)` moves into the C3AD0 delay slot.
`b6.c`: **29/367 words, 32 rows, 10 mnemonic rows**, no unused locals. (A named `f32 d = bound` local also fixed the
FP code but costs a named slot: refuted by the spill-temp homes, 136/140 -> 132/136.)

### Lever 5: ADCE0's count argument is the load itself
`cnt = D_80152460[off]; func_800ADCE0(&D_80152460[off] + 1, D_80152460[off], &polys[npoly], 0);` gives retail's
`lbu a1,0(..)` + `move s0,a1` in the jal delay slot. `b7.c`: **28/367 words, 31 rows**.

### Lever 6: loop 1's counter is `j` again (with levers 2-5 in place)
`for (j = 0; j < 4; j++)` in loop 1 (search loop `i`): retail's `slti` exit test, and j's split piece now stops
before the ADCE0 call block, so j takes a1 (and the loop-4 polygon pointer a2) as retail. Traced first with the
oracle `SPL_REJ=27 SPL_SYM=93` (reject bb27 from j's piece): 9 rows left. `b8.c`: 4/367 words, 7 rows.

### Lever 7: the sibling idiom `q = D_80152460 + off; cnt = *q++; func_800ADCE0(q, cnt, ...)` with `u8 cnt`
(camera_trigger_check/entity_update use the same `n = *p++`). `u8 cnt` declared after `s16 out` fills its pad, so the
`u8 *q` pointer costs no slot. `b9.c`: **EQUAL**. `k` was then unused; it is the final copy loop's counter (unrolled,
no code), `b10.c` = deliverable, EQUAL, no unused locals (removing `s32 k` instead changes the frame 584 -> 576).

### MATCH: group `groups/func_800AD4C8/` (supersedes the locked group of the same name)
Files: `group.c` (locked copy minus the two `__standin_func_800C3AD0_*` stand-ins), `camera_victory.c` (= b10.c with
header), `camera_trigger_check.c` (unchanged copy). Members: the four old members + camera_victory + func_800C3AD0;
claims: camera_victory, func_800C3AD0; keep: old keep list minus the C3AD0 stand-ins, plus camera_victory.

Whole-program unit (Pi repo root, tree at 0fdd8d9e). `unit_group.c` = the deliverable group.c with its context
func_800AC9BC reduced to a prototype (what the manifest does to a non-canonical definition; with it, `--with` makes the
old context body win over the locked single func_800AC9BC); `no_standins.c` (diagnostic) overrides the locked
frontier_entity_update copies of the two C3AD0 stand-ins with empty bodies, i.e. no stand-in calls C3AD0:
```
$ G=cloud/work/frontier/w16d/groups/func_800AD4C8; V=cloud/work/frontier/w16d/camera_victory
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w16d score camera_victory func_800C3AD0 camera_trigger_check \
    handbrake_apply func_800AD650 func_800C36A0 entity_update func_800AD4C8 --with $V/unit_group.c \
    --with $G/camera_trigger_check.c --with $G/camera_victory.c --with $V/no_standins.c --neighbours
  EQUAL camera_victory: 367 words (kept, c_camera_victory.c); stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)
  EQUAL func_800C3AD0: 362 words (internal, c_unit_group.c)
  EQUAL camera_trigger_check: 308 words (kept, c_camera_trigger_check.c)
  EQUAL handbrake_apply: 54 words (kept, c_unit_group.c); stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)
  EQUAL func_800AD650: 57 words (internal, c_unit_group.c)
  EQUAL func_800C36A0: 268 words (internal, c_unit_group.c)
  EQUAL entity_update: 391 words (kept, g_frontier_entity_update__entity_update.c)
  EQUAL func_800AD4C8: 66 words (internal, c_unit_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 8/8 equal; object build/blob_unit/w16d/unit.o (4.6s)
```
(The "stub tail" note is the known unit-ordering artefact: retail's next function is the unmatched entity_iterate.)
Also EQUAL with only `--with $G/camera_victory.c` (locked group and stand-ins untouched): camera_victory 367 words,
func_800C3AD0 362 words, 0 locked bodies differ.

Standalone group (builder `~/rush2049/scratch/frontier/w16d`, `python3 tools/cloud/score.py group cand/func_800AD4C8`):
```
Members:
func_800AD650:
  MATCH
handbrake_apply:
  MATCH
func_800C36A0:
  MATCH
camera_trigger_check:
  MATCH
camera_victory:
  MATCH
    own .data verified at 0x801174DC (4 bytes, all zero; .data is laid out per translation unit)
func_800C3AD0:
  MATCH

Context (informational; excluded from exit status):
func_800AD4C8:
  MATCH
input_process_controller:
  MATCH
func_800AD5D0:
  MATCH
func_800AC9BC:
  52/56 words differ
```
(func_800AC9BC 52/56 is the locked group's own context body, unchanged; the locked single func_800AC9BC is canonical
in the unit.) File order matters for the standalone group object: with `camera_victory.c` last, a deleted-procedure
stub lands after func_800C36A0 (`MISMATCH (1 extra words ...)`); `group.c, camera_victory.c, camera_trigger_check.c`
gives all MATCH.

**Integration notes**
- Replace `src/blob/groups/func_800AD4C8/` with this dir (same name, every old member kept). Splice claims
  camera_victory (1,468 B) and func_800C3AD0 (1,448 B): **+2,916 B game**.
- `provisional.json`: remove `func_800C3AD0`. `func_800AD4C8` stays provisional (input_deadzone_apply unmatched), so
  the input_process_controller stand-ins stay.
- No unit_overrides needed: func_800C3AD0's body text is identical in this group.c and frontier_entity_update's
  group.c (currently canonical); camera_victory is kept, func_800C3AD0 internal by the group rule. The locked
  frontier_entity_update group still lists the C3AD0 stand-ins in its own keep list; the unit is EQUAL with them
  (plain run) and without them (no_standins.c run). Dropping them there too is optional cleanup.
- No data-symbol changes; camera_victory's own .data (the 48-byte zero `CCol` template at 0x801174DC) verifies.
- Shaping devices: none (no unused locals, no compiled-out reads, no stand-ins, no implicit int).

### What generalises (camera_victory)
1. **uopt live-range splitting is order-dependent, and the order is source-visible.** A p1 split grows a piece by BFS
   over successors and stops at call blocks; the start block is the first occurrence in the live range's livbb list,
   which is DFS preorder over successor lists. Which branch arm is the fallthrough successor (`if (c) {call}` vs
   `if (!c) continue; call`) decides which call block is peeled first, and so whether a long-lived pointer is
   coloured at all. 250 -> 222 words from one `continue`.
2. **A private instrumented uopt is cheap**: `tools/patch_spl.py` adds prints to f_split/f_addadjacents/formlivbb plus
   two oracles (skip a start block, reject a block from a piece). The oracle proved each lever before the source
   form was found (SPL_SKIP=34 -> car coloured; SPL_REJ=27 -> j in a1).
3. Matched siblings are the best source of idioms: the `n = *p++; func_800ADCE0(p, n, ...)` / `u8 n` pattern from
   camera_trigger_check closed the last 3 words (and the u8 fills an s16 pad, so the pointer costs no slot).
4. A memory variable tested after its store (`bound = pt[1]; if (bound < ...)`) becomes a coloured variable web and
   moves the later reload above a branch; a named `f32` copy did the same but cost a slot (refuted by spill homes).

## func_800E4B58 (2,268 B; would close provisional func_800E451C + func_800E4300): NOT matched, stopped

Start (current unit, w15f's measurement reproduced with the dot_interval_position packet, `v/g0.c`):
`FAIL func_800E4B58: 488 of 567 words differ; compiled body is 534 words, target 567` (frame 216 vs 304, 106
mnemonic rows); E451C EQUAL 397, E4300 EQUAL 135, context E398C `562 of 599`.

**Identification (arcade game/maxpath.c rev 3.48):** func_800E4B58 is `MaxPathControls` with five helpers inlined in
call order -- `MP_FindInterval` (the three E451C = MP_IntervalPos calls), `MP_TargetSpeed`, `MP_TargetSteerPos`,
then the call to func_800E398C = `avoid_areas(m->net_node)` (node passed in **s7**, an IPA register parameter), the
N64 difficulty scaling (`D_801161D4[D_801543C8]` 0.8/1.0/1.25/1.5 when vy < 0, else min `D_801161E4[..]`
75/90/135/180, times `m->f7EC` drone_scale), `AdjustSpeed` (lastthrottle/lastbrake are f32 arrays D_80153F68 /
D_80153F48, -3000 -> -0.75, 0x1000 -> 1.0, 750 -> 0.183f, plus an N64 `D_801543C8 < 2 &&` on the slip test) and
`AdjustSteer` (pos = cp->tgtpos in place, 1.8 -> 108.43373f * m->f634, fatan2 -> func_8008C768(pos[2], pos[0]),
`range_it` -> 2/pi scale, D_80153F28[node] and m->f720). The arcade `range_it` macro (nested ternary) explains
retail's `mov.s $f0,$f2; mov.s $f2,$f0` pairs in all three clamps. Own .rodata 0x80124454..78 decoded
(1.4666667f speed scale, 0.1f, 0.2f, 0.00003125f, 1e-5f, 1.05f, 0.183f, 108.43373f, 3.1415927f, 0.63661975f).

`func_800E4B58/best.c` (= `v/g3.c`, whole group file: E4300/E451C/E398C from the packet + the helper-structured
MaxPathControls): `FAIL func_800E4B58: 503 of 567 words differ; compiled body is 551 words, target 567`, frame
288 vs 304, **70 mnemonic rows (from 106)**. Measured steps: helper structure with the arcade local lists minus
the never-used ones (frame 416 -> 288, 557 words, 97 rows); MP_TargetSpeed computes mp/nmp after the
NextMaxPath call and the clamp, as retail does (97 -> 70 rows).

**Traced cause of the remaining distance (best next hypothesis):** the IPA context of func_800E398C. Retail
avoid_areas uses s0-s8 and $f20-$f30 without saving them (so MaxPathControls saves all of them) and keeps its
parameter `n` in s7 for the whole body (`sw s7,288(sp)`, n*3 / n*0x3B8 indexing throughout, frame 288). The
packet's E398C takes n in **a3**, uses pointer locals instead of indexing by n, and has a 200-byte frame; so in
our unit a 0.0f constant web lives in $f20 across the E398C call (retail rematerialises `mtc1 zero` locally), the
node argument is set up in a3 instead of `lh s7,1990(..)`, and the homes differ (cp at 280 vs 296, m at 288 vs
304). Next: (1) a diagnostic stand-in E398C with retail's convention (s7 parameter, s0-s8/$f20-$f30 clobbers) to
measure how much of E4B58 is context (billboard lesson); (2) rework the E398C reconstruction to index the
D_80153F88/D_80154138/D_80153F28 arrays by `n` directly (keeps n live -> s7); (3) then the remaining E4B58 frame
(16 bytes: probably two inlined-call areas) and the vy spill (retail homes `m->vy` at 120(sp) and reuses it in
AdjustSpeed). func_800E398C itself is unmatched, so even an EQUAL E4B58 lands with E398C as nonmatching context.
No permission denials.
