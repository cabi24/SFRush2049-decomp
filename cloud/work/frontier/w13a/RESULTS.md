# Wave 13, lane w13a: mode-select cluster (func_800E05F0 `.alias` + loop-preheader colour)

Written by the coordinator on 2026-10-10: the lane's session ended before it reported. Its deliverable and
replay were on disk and were re-verified then.

| Function | Bytes | State | Flags | Scorer output (`TAG=w13a sh cloud/work/frontier/w13a/comp/run.sh`, Pi, 2026-10-10) |
|---|---:|---|---|---|
| func_800E05F0 | 1328 | EQUAL in the whole-program unit (w12i 30 rows -> w13b 2 words -> w13a 0) | -O3 | `EQUAL func_800E05F0: 332 words (kept, c_mode.c)` |
| func_800D5E64 | 584 | EQUAL (kept) | -O3 | `EQUAL func_800D5E64: 146 words (kept, c_d5e64.c)` |
| mode_select_handler | 2976 | EQUAL (internal) | -O3 | `EQUAL mode_select_handler: 744 words (internal, c_msh.c)` |
| func_800E0050 | 1440 | EQUAL (internal) | -O3 | `EQUAL func_800E0050: 360 words (internal, c_mode.c)` |
| func_800DFBA0 | 1192 | EQUAL (internal) | -O3 | `EQUAL func_800DFBA0: 298 words (internal, c_mode.c)` |
| func_800DED78 | 488 | EQUAL (internal) | -O3 | `EQUAL func_800DED78: 122 words (internal, c_ded78.c)` |
| best_times_display | 224 | EQUAL (internal) | -O3 | `EQUAL best_times_display: 56 words (internal, c_d5e64.c)` |
| mode_select_input | 152 | EQUAL (internal) | -O3 | `EQUAL mode_select_input: 38 words (internal, c_mode.c)` |
| func_800DEF60 (stub) | 8 | EQUAL; deleted static given a body | -O3 | `EQUAL func_800DEF60: 2 words (internal, c_msh.c)` |
| func_800E0048 (stub) | 8 | EQUAL; camera-wrapper identity hypothesis | -O3 | `EQUAL func_800E0048: 2 words (internal, c_mode.c)` |

Same run: `locked bodies that differ in this unit: 0`, `blob_unit score: 10/10 equal`. Total 8,400 B.

## Deliverable
`groups/frontier_mode_select/` (group.json with 10 members = 10 claims, keep = func_800D5E64 + func_800E05F0; files
d5e64.c, mode.c, msh.c, ded78.c; `comp/` is the same source with the replay `run.sh`). Required overrides per the
group note: force_internal func_800DEF60 and func_800E0048 (locked 2-word stubs, `--revert-single` at install);
prefer_definition entity_hierarchy_update -> mode.c (locked in codex_transform_b109; mode.c inlines it `__inline`).

## Mechanism (closed; full text in mode.c's func_800E05F0 header)
- `.alias $8,$sp`: uopt `f_base_in_reg` emits `.noalias reg,$sp` only when the FIRST access after a register-map
  change has an islda (global address) root; an access through the copy-propagated pointer variable has an isvar root
  and marks the pair aliased. Fix: index `D_80140640[slot]` directly for the first t0 access of each path.
- Loop-preheader colour tie (the w13b 2 words): `if (model->contact[i]) continue;` makes uopt number the latch's
  constant-4 web before the body's table web; plain `if (x<value) x=value;` updates keep the block count (and so the
  0.25/block callee-saved cost) low enough for `model` to stay in s3; `level=value=0.0f` in the for-init.
- Disclosed shaping: unused locals d1/d2/d3 (exact frame residual), dead `value` init, func_800E0048 identity.

## Integration status (coordinator, 2026-10-10)
`install_group.py` failed: `func_800E05F0: 2 extra words (nonzero beyond target length)`, rolled back cleanly.
`score.py group` on the builder shows the 4-file group compile is far from the unit (mode_select_input 15/38,
func_800DFBA0 13/298, func_800E0050 353/360): the match depends on whole-program IPA context that the group does
not carry. Lane w15i is packaging the needed locked callees as group `context` (w12l's method).
