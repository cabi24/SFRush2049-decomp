# w26d — game (IDO 5.3), wave 26

Status: done. Nothing is installable yet. The controller-pak write cluster that func_800A1BB4 belongs to went from
1/7 to **5/7 EQUAL** in the whole-program unit (track_data_decompress and three callers now match). ghost_race_setup
and race_position_update are unchanged; their causes are narrowed below. func_800A1BB4 and func_800A1DD4 are still
open, and the group cannot land until both match. Nothing committed, spliced or pushed. No permission denials.
Builder scratch watchman2:~/rush2049/scratch/frontier/w26d (toolkit reused from wtk), deleted at the end.

| Function | Bytes | Before -> after | Scorer line (whole-program unit, `--neighbours`, 0 locked bodies differ) |
|---|---:|---|---|
| track_data_decompress | 652 | 101/163 (C96 research) -> **EQUAL** (inside the cluster; needs func_800A1DD4's mark in t3) | `EQUAL track_data_decompress: 163 words (internal, c_cluster.c)` |
| func_800A1C6C | 360 | 65/90 -> **EQUAL** (cluster) | `EQUAL func_800A1C6C: 90 words (internal, c_cluster.c)` |
| track_collision_setup | 468 | 28/117 -> **EQUAL** (cluster) | `EQUAL track_collision_setup: 117 words (kept, c_cluster.c)` |
| MaxPathZeroControls | 396 | **EQUAL** (cluster, unchanged C96 body) | `EQUAL MaxPathZeroControls: 99 words (kept, c_cluster.c)` |
| slot_state_lookup | 188 | 35/47 -> **EQUAL** (cluster) | `EQUAL slot_state_lookup: 47 words (kept, c_cluster.c)` |
| func_800A1DD4 | 192 | 42/48 -> 27 words (with mark = t3); alt 15 words (mark t2) | `FAIL func_800A1DD4: 27 of 48 words differ` |
| func_800A1BB4 | 184 | 9 rows (w25e) -> 9 rows in cluster.c; alt 7 rows (non-void, not adopted) | `FAIL func_800A1BB4: 29 of 46 words differ; compiled body is 42 words, target 46` |
| ghost_race_setup | 440 | 16 words, unchanged; as1 cross-block pull traced | `FAIL ghost_race_setup: 16 of 110 words differ` (w24f best_words.c) |
| race_position_update | 484 | 50 words, unchanged; candidates for X narrowed | not re-worked beyond the search below |
| func_8010FBE0 | 128 | not attempted (no new lever from targets 1-3) | |

## Target 1: the controller-pak write cluster (`pak_write_cluster/cluster.c`)

The structure is unusual: **track_data_decompress is itself an INTERNAL function**. The handle arrives in a2 and
mode in the caller's outgoing slot 4(sp). It uses s0-s8 without saving them; its kept callers track_collision_setup
and MaxPathZeroControls save s0-s8 for it. Its callees func_800A1C6C (a0,a1,t1,t2) and func_800A1DD4 (a0,a1,a2,t3)
are internal register-parameter functions, and func_800A1BB4 is internal (t6-t9 ring). The group therefore has
7 members: A1BB4, A1C6C, A1DD4 and track_data_decompress are forced internal; track_collision_setup,
MaxPathZeroControls and slot_state_lookup are kept.

Command (Pi, repo root), output verbatim:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w26d score func_800A1BB4 func_800A1C6C func_800A1DD4 \
  track_data_decompress track_collision_setup MaxPathZeroControls slot_state_lookup \
  --with cloud/work/frontier/w26d/pak_write_cluster/cluster.c --internal func_800A1BB4 --internal func_800A1C6C \
  --internal func_800A1DD4 --internal track_data_decompress --internal pfs_queue_init --internal pfs_lock \
  --internal pfs_unlock --neighbours
  FAIL func_800A1BB4: 29 of 46 words differ; compiled body is 42 words, target 46
  EQUAL func_800A1C6C: 90 words (internal, c_cluster.c)
  FAIL func_800A1DD4: 27 of 48 words differ
  EQUAL track_data_decompress: 163 words (internal, c_cluster.c)
  EQUAL track_collision_setup: 117 words (kept, c_cluster.c)
  EQUAL MaxPathZeroControls: 99 words (kept, c_cluster.c)
  EQUAL slot_state_lookup: 47 words (kept, c_cluster.c)
  locked bodies that differ in this unit: 0
blob_unit score: 5/7 equal
```
Start: dot_pak_write_research_20261006/group.c (C96 types), with osJamMesg -> osSendMesg and osPfsGetFileSize ->
osPfsReadWriteFile (0x8000AEC4), plus w25e's func_800A1BB4. That base scored track_data_decompress 70 rows.

The levers below are each traced from the diff and each was measured:
- **`extern volatile C96Channel D_80144030[]`**: w8b already found that the pak records are volatile. Retail re-reads
  `state` once per test in the error-code chain. Volatile keeps the other members unchanged (MaxPathZeroControls
  stays EQUAL). track_data_decompress: 70 -> 47 rows.
- **Global form instead of a channel pointer**. In track_data_decompress, `D_80144030[index].error =
  func_800A1E94(result)` lets as1 lift the mode load (`lw t8,116(sp)`) above the store; the `channel->error` pointer
  form does not (24 -> 2 rows). In track_collision_setup the same applies to every access (no channel local) and
  gives EQUAL.
- **track_data_decompress locals.** They are declared result, index, slot_index, retry/callback_value, address,
  bytes, offset, allocation, with no channel or slot locals; retail spills slot to 48(sp), which is a temp, not a
  home. The lock is `pfs_lock()` inlined, and its OSMesg local is retail's sp+60. These give every retail stack
  slot: 112-byte frame, slot_index home 100, retry 99, callback 98, address 92, bytes 88, offset 84.
- **func_800A1C6C**: scans `map[i]` with an index, not a pointer walk. uopt's strength-reduced pointer gives retail's
  `move a3,a0`. It also uses `u8 complete` (so 1 is not shared with the `*bytes = 1` web) and
  `return (*bytes == 0) ^ 1;` (retail's `sltiu; xori`). Parameter order is **(handle, &address, &bytes, &offset)**:
  the order sets ugen's argument evaluation at both call sites (t2, a1, t1); (handle, bytes, offset, address) gives
  a1, t1, t2.
- **Lock helpers are NON-static.** The names are pfs_queue_init, pfs_lock and pfs_unlock, and they must be
  `--internal`. Static copies leave two deleted-procedure stubs after track_data_decompress (retail has none). If
  they are non-static and kept, umerge does not inline the nested init.
- **slot_state_lookup**: `if (D_80144030[file->channel].slots[file->slot].flag2) {}` before the func_800A1DD4 call.
  This is a DISCLOSED compiled-out test of a volatile byte; it emits only retail's `lb t9,134(t7)`. The
  dot_pfs_flag_read research found the same read. A bare expression statement gives 33 words.

**func_800A1DD4** (open). Retail sets the cluster's convention mark = t3. It keeps `address + bytes` in v0, computed
at the join, which stops as1 hoisting it into the entry. `address - base` is in a2 (the bytes register), and its
colours are last t0, map t1, first t2, mark t3, with a3 unused.
- `last = (bytes = (u32)(address + bytes)) - (u32)*file->data - 1 >> 5;` (an embedded reassignment of the
  parameter) keeps the sum a v0 web at the join.
- Reading `*file->data` each time (no base local) gives retail's `base + size` operand order.
- With these, k1 (`first = (bytes = (u32)(address - *file->data)) >> 5`, the cluster's piece) has mark t3 and 27
  words. j4 (plain first) has 15 words but mark t2, which costs track_data_decompress 1 word.
- `force.sh` on k1 (`p2:w25=c7,p2:w38=c6,p2:w40=c8,p2:w36=c9`) gives retail's colours; what is left is the ugen temp
  ring (retail's last uses t6, ours t9).
- **So retail has (a) a codeless web numbered before `last` that takes a3, with map's web before first's, and (b) one
  codeless ugen temp pop before last's srl.** Probes that did not move either: ~40 forms (order, offset variable,
  `/ 32`, `end` locals, `do {} while (0);` at four points, range-check helpers, mark u8/s8/`== 1`, loop shapes).
  `alt/a1dd4_j4_15rows.c` is the closest body.

**func_800A1BB4** (open). The current piece gives 9 rows. Making it `s32` with an early `return -1;` and falling off
the end (alt/a1bb4_r1_7rows.c, 7 rows) produces retail's separate early `jr ra`, but `li v0,-1` sits in its delay
slot (retail nop). A pointer return of rec (`return rec` at both exits) loses v0 to the return precolour (22 rows),
as w25e found. A void wrapper around an inlined helper gets inlined into track_data_decompress; it needs
`--block` and is still 10-14 rows. The found path's `beqz; nop; b exit; nop` jump-around is the check_mpath_save
signature (an inlined helper's value return whose dead `li v0` as1 deletes). Next: a value-returning per-handle test
inlined into the loop, together with an early-return form whose dead v0 as1 can delete.

## Target 2: ghost_race_setup (440 B) — unchanged; as1 cause located
- Re-scored: w24f best_words.c `FAIL ghost_race_setup: 16 of 110 words differ | words 14 ops 11`; w24f best.c
  (inlined func_800960CC + `do {} while (0);`) `53 of 110 ... compiled body is 109 words | words 19 ops 11`.
- as1 trace (`as1t.sh g24`) shows the predecessor block [sb zero,5(s2); lb t6,239(v0); beq] rescheduled four times.
  Each pass pulls one more DAG root of the release block (move a0,s5 -> move a1,zero -> li a2,1) into the lb hole and
  delay slot. `lw s0,76(s2)` is never pulled because s0 is in the exit `.livereg 0x0000FF0E`, so it lands in the jal
  delay slot. Retail pulls nothing and copies `lw s0` into a beqzl slot, so in retail as1 judged a0/a1/a2 not dead
  on the ready arm, or the block not eligible.
- The pull is f_do_xbb_opt (wtk/src/as1.c line 55065). It calls the anonymous func_4287c8, func_428fc4,
  func_428a2c, func_429534, func_42b2e4 (only when the global at 0x10030818 is >= 4), func_42aa0c and func_42b51c.
  The legality tests are f_eligible (72656) and f_mark_unmovable (130490). The next step is to add printf
  instrumentation there (the candidate instruction and the live set at the fall-through); not done.
- Probes: inverted arms (29 rows), `goto next` ready arm (identical to best.c), `volatile s8 ready` (identical),
  `volatile s8 state` (42 rows).

## Target 3: race_position_update (484 B) — unchanged; search for X
- No locked function near 0x800BE7BC returns -1 or has a folded-compare constant parameter. Checked:
  func_800BEA30/3C/58/6C, music_fade and music_tempo_adjust (callers), and func_800BDEB0 (the spacing getter,
  already inlined).
- object_manager_update (0x800B3FA4, the text width) does its own `n < 0` test on the -1 sentinel. It narrows its
  s16 argument exactly like retail's call site (`sll/sra/move`), but it is called by jal, not inlined.
- sound_pitch_diff_calc (0x800B7170, kept) is exactly `maxw - width(str, -1)`, which is retail's `s2`. Inlining it
  is refuted: its `(s16)` return would narrow s2, and retail has no sll/sra there.
- Next: list umerge's inlinable kept functions with a parameter compared against a constant (wheel_setup_initial
  pattern), then test only those.

## What generalises
- **A non-static internal helper avoids deleted-procedure stubs.** Static inlined helpers in a candidate file leave
  `jr ra; nop` stubs after the next function in the unit; retail has none. Make them non-static and pass
  `--internal` (kept helpers break nested inlining).
- **IPA parameter order shows in the caller.** ugen evaluates register arguments in parameter order, so a caller's
  `addiu t2; addiu a1; jal; addiu t1` order gives the callee's prototype order (func_800A1C6C).
- **Pointer vs global form decides as1's store/load lifting.** Through a struct pointer local, as1 keeps sp loads
  below byte/word stores; the `D_x[index].f` form lets it lift them (track_data_decompress, track_collision_setup).
- **Embedded reassignment of a parameter keeps a value as a variable web at the join** (`(bytes = a + b) - c`),
  where a plain expression is hoisted by as1 into a free temp at the entry.
- **as1's cross-block pull is iterative**: one DAG root of the target block per reschedule of the predecessor, and
  only roots whose destination is dead on the fall-through (ghost_race_setup trace).

## Integration
Nothing is installable. When func_800A1BB4 and func_800A1DD4 match:
- Group dir with members func_800A1BB4, func_800A1C6C, func_800A1DD4, track_data_decompress,
  track_collision_setup, MaxPathZeroControls, slot_state_lookup.
- keep: track_collision_setup, MaxPathZeroControls, slot_state_lookup.
- Internal: the four others plus pfs_queue_init, pfs_lock and pfs_unlock (non-static helpers, not members, not on
  keep).
- Needs unit_overrides force_internal for func_800A1BB4, func_800A1C6C, func_800A1DD4 and track_data_decompress.
- The `volatile` D_80144030 declaration is local to the file. No data-symbol changes; no groups superseded.
- Run `score.py group` on the builder before claiming.
