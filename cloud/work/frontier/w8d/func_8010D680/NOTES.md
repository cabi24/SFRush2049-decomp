# func_8010D680 — 22 aligned rows (w8d), from w6d's 49

best.c = s3/handle_pad.c. `full.sh best.c func_8010D680` -> differing rows 22 (best.diff).

Found (w8d):
- The node-flag test is an inlined helper `static s32 model_visible(s16 id) { return D_8012E700[id].flags &
  0x80000000; }`. The s16 parameter narrowing gives retail's `sll v1,t0,16; sra t2,v1,16` (v1 as the
  narrowed-parameter register) and the `sll t4,v0,0; bgez` test; with the inline `(s16)` cast and a `handle`
  local the middle is ~25 rows off. A `handle` local breaks it again (50 rows): t->handle is read directly.
- A separate `ok = 1` flag gives retail's `li a1,1` (w6d reused `on`, which loses the li).
- The frame/v[] slot (v at sp+56) needs one more word slot before v: `s32 pad` in best.c is a stand-in
  (w6d had `handle` and `flags` locals there). Helpers for unlink/hide/show/sound all grow the frame to 88.

Residual (22 rows), one cause suspected:
1. Head: retail narrows `on` in place (`sra a1,t6,16`), ours into t7. Traced uopt (st_ea vs st_eb, proc 939):
   in both, `on` (w0) is coloured a1, but nocs=1 in ours vs 7 when `on` is reused later; with a single-block
   web uopt drops the `on = (s16)on` store and ugen narrows into a temp.
2. as1 hoists `move a1,zero` / `move a0,t0` (model_data_load args) above the `bgez` and does not use
   `bgezl` + delay `move a0,t0` as retail does. Same with `lui v1` (D_8002EB94) above `lbu`.
   Both point at a1 (and possibly a0) being LIVE on the fallthrough path in retail (.livereg), i.e. the same
   cause as (1): `on` (a1) live from entry through the switch until the `ok = 1`.
Tried (~45): `on = 1` reuse (kills the web), `!on` / `on + 1` / `1 - on` / `on == 0` as the unlink argument
(no branch constant propagation), late folds `on * 0 + 1`, `on - on + 1` for the flag, `if (on) {}`, helper
splits (unlink, hide, show, sound; all 30 combinations), sound-block spellings (no vel, dt pointer, loop,
operand order).
Next hypothesis: find a read of `on` between the visible test and the loop that emits nothing but survives
dead-store elimination (e.g. an inlined helper taking `on` whose use folds only after colouring), or a source
where the flag IS `on` but is re-initialised in a way uopt cannot prove kills the old value.
