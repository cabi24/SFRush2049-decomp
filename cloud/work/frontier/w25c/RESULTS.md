# w25c RESULTS (wave 25, game near-misses, IDO 5.3)

Status: done. Three targets, three matches (two groups, one standalone single). No permission denials. Builder scratch watchman2:~/rush2049/scratch/frontier/w25c (toolkit reused from wtk).

| Function | Bytes | Before -> after | Flags | Scorer line |
|---|---:|---|---|---|
| audio_fade_control unit (12 members incl. gfx_setup_e700) | 1904 + stubs | 7/8 (gfx_setup_e700 4 rows) -> **13/13 EQUAL in unit, group MATCH** | -O3 group | `blob_unit score: 13/13 equal` / `locked bodies that differ in this unit: 0`; `score.py group`: all 12 members `MATCH` |
| wheel_setup_initial (+ stub func_800A4B40) | 124 | 4 words -> **EQUAL, group MATCH** | -O3 group | `EQUAL wheel_setup_initial: 31 words` / `locked bodies that differ in this unit: 0`; `score.py group`: both `MATCH` |
| func_800EC190 | 224 | 2 words -> **MATCH** (`cloud/matches/func_800EC190.c`) | -g0 -O3 -mips2 -G 0 -non_shared | `score.py fn`: `MATCH` / `own .data verified at 0x801163F4 (4 bytes, all zero; ...)`; unit `EQUAL func_800EC190: 56 words`, `locked bodies that differ in this unit: 0` |

## audio_fade_control unit: 7/8 -> 8/8, packaged as group `audio_fade_control` (12 members, all MATCH)

gfx_setup_e700 lever (traced, one edit): w24f proved the residual colour-only (`&D_80146138` web w21 must be a2).
ctrace of w24f best: w21 save 1.0, nocs 1, a0 cost 0.0 (its call-argument register), a2 0.1, forbidden only a1 ->
a0. a0 has to be FORBIDDEN in retail. A block boundary inside node_alloc between the head load and the unlink call
does it: `do {} while (0);` after `node = D_80146138.head;` gives w21 nocs 2 (two blocks), forbidden0=0x18000000
(a0 and a1: a0 is precoloured for the call argument in the second block) -> a2 (cost 0.1), and as1 then folds the
argument move into `lui a2; addiu a0,a2,lo` exactly as retail. The device sits in the shared helper node_alloc and is
inert for the five other inlined copies (they stay EQUAL). `if (node) {}` at the same place and `do {} while (0);`
before or after `node->busy = 0` all give EQUAL (r3/s1-s3); a gfx-only copy of the helper does too (r2/q1-q3, not
used: it would need a fifth stub). The value-holder forms (w24f a1/a2/k1/k2) make `value` a register web instead.

Whole-program unit (Pi, repo root):
`python3 -m tools.conveyor.pipeline.blob_unit --tag w25c score audio_fade_control gfx_setup_e700 audio_bus_route
audio_timing_sync sound_priority_set audio_stream_control audio_buffer_manage audio_state_save func_800956BC
func_800953B4 func_800953BC func_800954A0 func_800953C4 --with cloud/work/frontier/w25c/groups/audio_fade_control/group.c
--internal audio_fade_control --internal func_800956BC --internal func_800953B4 --internal func_800953BC
--internal func_800954A0 --neighbours`
```
  EQUAL audio_fade_control: 53 words (internal, c_group.c)
  EQUAL gfx_setup_e700: 32 words (kept, c_group.c)
  EQUAL audio_bus_route: 59 words (kept, c_group.c)
  EQUAL audio_timing_sync: 46 words (kept, c_group.c)
  EQUAL sound_priority_set: 59 words (kept, c_group.c)
  EQUAL audio_stream_control: 59 words (kept, c_group.c)
  EQUAL audio_buffer_manage: 59 words (kept, c_group.c)
  EQUAL audio_state_save: 109 words (kept, c_group.c)
  EQUAL func_800956BC: 20 words (internal, s_func_800956BC.c)
  EQUAL func_800953B4: 2 words (internal, c_group.c)
  EQUAL func_800953BC: 2 words (internal, c_group.c)
  EQUAL func_800954A0: 2 words (internal, c_group.c)
  EQUAL func_800953C4: 2 words (kept, s_func_800953C4.c)
  locked bodies that differ in this unit: 0
blob_unit score: 13/13 equal
```
(The w24f "stub tail: 4 words of deleted-procedure stubs follow (NOT as in the image)" note is gone: the helpers are
now non-static and named after their stubs.)

`score.py group cand/groups/audio_fade_control` (watchman2, IDO 5.3), with and without `--claims`: every one of the
12 members `MATCH`.

Integration (coordinator):
- Group dir `cloud/work/frontier/w25c/groups/audio_fade_control/` (group.c, func_800956BC.c = byte-identical copy of
  the locked single, group.json with members = claims, keep = the seven callers).
- Members: func_800953B4, func_800953BC, audio_fade_control, func_800954A0, gfx_setup_e700, func_800956BC,
  audio_bus_route, audio_timing_sync, sound_priority_set, audio_stream_control, audio_buffer_manage, audio_state_save.
  New group (no locked group superseded). Locked singles absorbed: `--revert-single func_800956BC --revert-single
  func_800953B4 --revert-single func_800953BC --revert-single func_800954A0`.
- unit_overrides (add BEFORE installing): `force_internal` + `prefer_definition` (file
  `src/blob/groups/audio_fade_control/group.c`) for func_800953B4, func_800953BC, func_800954A0 (deleted helpers
  list_append / entry_alloc / node_alloc inlined into the members; only the retail stubs remain). audio_fade_control
  and func_800956BC need nothing: as non-keep group definitions they are internal automatically.
- func_800953C4 stays a locked empty stub (only three helpers are known; the stub identity among 3B4/3BC/3C4 is a
  hypothesis, all three are `jr ra; nop`).
- Disclosed shaping devices: `if (node->kind == 2) {}` in audio_fade_control (w24f) and `do {} while (0);` in
  node_alloc (w25c). No data-symbol changes.

## wheel_setup_initial: 4 words -> EQUAL, packaged as group `wheel_setup_initial` (2 members, MATCH)

Start: w24f best (inlined `release_if(slot, i, type, s8 sub)` with sub = -1; frame 48 exact, colour-only).
ctrace (w0): `type` w13, -1 w46, 64 w47 all at save 3.333 / nocs 3 / tot 10; ties go to the lower web number, so
the order was type s2, -1 s3, 64 s4 (retail: 64 s3, -1 s4). pretrace: the constant webs follow the expression
bits, and -1's only use (`equ.J(-1, f5)`, block 4) precedes 64's (loop test, block 7) in block order, so moving
64's appearance cannot work without changing the loop shape. Lever: give 64 (and `type`, which must stay first)
one more use that emits no code: `if (type == 64) {}` in the loop body. t1/t2/t3/t5 (before or after the call,
inside the helper, `!=`) all EQUAL; t4 (before the loop) 10 words. No wrapper form (the helper is called from the
function itself; the locked callers billboard_render / state_change_preprocess are unchanged: 0 locked differ).

Unit (Pi): `blob_unit --tag w25c score wheel_setup_initial func_800A4B40 --with
cloud/work/frontier/w25c/groups/wheel_setup_initial/group.c --internal func_800A4B40 --neighbours`
```
  EQUAL wheel_setup_initial: 31 words (kept, c_group.c)
  EQUAL func_800A4B40: 2 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 2/2 equal
```
`score.py group cand/groups/wheel_setup_initial` (watchman2), with and without `--claims`: wheel_setup_initial
`MATCH`, func_800A4B40 `MATCH`.

Integration: group dir `cloud/work/frontier/w25c/groups/wheel_setup_initial/`, members wheel_setup_initial +
func_800A4B40 (locked empty stub given its body), keep wheel_setup_initial. `--revert-single func_800A4B40`;
unit_overrides `force_internal` + `prefer_definition` (file `src/blob/groups/wheel_setup_initial/group.c`) for
func_800A4B40. Disclosed device: `if (type == 64) {}`.

## func_800EC190: 2 words -> MATCH, `cloud/matches/func_800EC190.c` (standalone single)

Start: w21g best (= w23e base; function-local static D_801163F4, holder `if (D_8014A118) {}`), 2 words: `li t1,2`
retail vs `li t0,2`. ugen trace (`SCHED=1 ugt.sh`): the temp ring is strictly round-robin here (every temp freed
right after use), and every visible temp matches retail (prologue s8-narrowing t6/t7 hidden, t8/t9 for the
D_801543CA store, loop t0..t5, t6/t7, tail sll t8 / hidden sra t9), so retail allocates exactly one more hidden temp
between the counter's sign extension and the constant 2. as1 deletes a narrowing of a value that only feeds a byte
store (it already deletes the owner param's sll/sra before `sb a0`). Lever: `D_8014A118 = D_801163F4 & 0xFF;`:
ugen emits `and $8,$2,255; sb $8,0($3)` (t0), as1 drops the `and` and stores v0, and `li $9,2` gets t1. Casts
`(s8)/(u8)/(int)/(short)` on either store are folded by uopt (c1, 24 variants, all 2 words); typed value locals
(c3) colour the value; counter locals (c4) and ternary/chained forms (c2) are worse. Masking the D_80150C04 store is
equivalent (a2); both masks: 5 words. The holder is still required (a1n/a2n without it: 10 words).
```
score.py fn cand/func_800EC190.c func_800EC190 --flags "-g0 -O3 -mips2 -G 0 -non_shared"   (watchman2)
func_800EC190:
  MATCH
    own .data verified at 0x801163F4 (4 bytes, all zero; .data is laid out per translation unit)
blob_unit --tag w25c score func_800EC190 --with cloud/matches/func_800EC190.c --neighbours
  EQUAL func_800EC190: 56 words (kept, c_func_800EC190.c)
  locked bodies that differ in this unit: 0
```
Integration: plain game single (`splice_singles.py func_800EC190`); no overrides. Data: D_801163F4 becomes a
function-local static (own .data, 4 zero bytes at 0x801163F4; other `extern s16 D_801163F4;` lines are generated
declarations only, w21g). Disclosed devices: the w21g holder and the `& 0xFF` mask.

## What generalises
- **A colour-only "wrong argument register" for an address can be a block-boundary question**: a web that ends in
  the call's own block may take the argument register (cost 0); one block earlier, the precoloured argument in the
  call block forbids it. `do {} while (0);` (or a holder) between the load and the call in an inlined helper moves
  the boundary; check the helper's other inlined copies stay EQUAL.
- **Three-way callee-saved priority ties** (equal save/nocs): a codeless compare between the two webs that must
  rise (`if (type == 64) {}`) adds one use to each and leaves the third behind; constant web numbers follow block
  order of their uses, so moving appearance rarely works.
- **One-register ugen temp-ring shifts**: as1 deletes a narrowing (`and 0xff`, `sll/sra`) whose value only feeds a
  byte store, so `x & 0xFF` on a byte store costs one hidden ugen temp and no code; uopt folds the equivalent casts.
