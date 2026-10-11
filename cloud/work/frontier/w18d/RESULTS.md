# w18d results (wave 18)

Assignment: camera_play_script (0x800C5644, 3,520 B, whole-program unit, -O3). Start: w17a best.c, re-scored in the
current unit and unchanged: `words 27 ops 1 norm 1 | frame 608/608`.

## Summary

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| camera_play_script | 3520 | not matched; still **27 aligned rows, frame 608/608** (best.c = w17a's, byte-identical) | `-g0 -O3 -mips2 -G 0 -non_shared` (whole-program unit) | `python3 -m tools.conveyor.pipeline.blob_unit --tag w18d --jobs 2 score camera_play_script --with cloud/work/frontier/w18d/camera_play_script/best.c --neighbours` -> `FAIL camera_play_script: 793 of 880 words differ; compiled body is 879 words, target 880; +0x208: .rodata+0x498: retail words encode 0x01284282, outside the image` / `locked bodies that differ in this unit: 0`; `us.sh` -> `words 27 ops 1 norm 1 \| frame 608/608` |

No match. Nothing to integrate (no cloud/matches file, no group, no overrides, no data-symbol changes). best.c has no
shaping devices, no unused locals and no implicit int. The two zero spellings (`0.0f` on lines 94/97, `0.f` on 95/148)
are literal choices that select uopt constant webs (w16c/w17a), disclosed here as before.

## What this lane established (oracle-proven; diagnostics only, never match evidence)

The oracle binary is `~/rush2049/scratch/frontier/w18d/bin/uopt_x`. It is w17a's `src2/u1.c` (w16d SPL + w17a LBB)
plus w17f `prio_patch.py` (PRIO, SAVELOG) plus `tools/patch_fbd.py` (this lane: `FBD=1` prints the per-block register
masks of each decided live range, `UPF=1` logs every `updateforbidden(block, excl, lr, class)` call). The SAVEV loop in
prio_patch is now bounded by the bit-vector size (`MEM_U32(lr+0xc)*128`); the 400 bound segfaulted on this function.
Run it with `tools/oracle.sh LABEL uopt_x "ENV=…" [udiff args]`; snapshots come from `vt.sh`/ctrace labels.

1. **Items (1) and (2) are exactly two decisions.** `PRIO=156:3 CDX_PROC=622 CDX_FORCE=p1:w85=c7` -> 12 rows.
   The 12 are 9 slot rows, 1 mul row and 2 reloc rows; item (3) is untouched.
2. **Item (2), the pts[ka]/remainder swap.** `PRIO=156:1.875` is enough: it raises pts[ka]'s save from 8 to 15, which
   ties w515 and wins on the lower web number. `156:1.25` is not enough. SAVELOG shows that pts[ka] (w156) and pts[kb]
   (w162) have the same structure: bb18 u1 d1 w10 plus bb25 u2 w10 gives tot 40. Card is 12 (the conv-loop blocks),
   so nocs = 12/4+2 = 5 and save = 8. w515 has bb21 u1 d1 plus bb111 u1, tot 30, card 1, nocs 2, save 15.
   Retail's colours (pts[ka] t0, remainder t1, pts[kb] t2) need **P(pts[ka]) >= P(515) > P(pts[kb])**, so retail has an
   asymmetry between pts[ka] and pts[kb]. That needs at least +35 weighted uses on pts[ka] or fewer live blocks.
   Extra textual `pts[ka][1]` reads add nothing because PRE removes the redundant load and with it the address use:
   `v/e1..e4` (e4 is identical to base, the others are worse).
3. **Item (1), corner IV a1 vs t0. The cause is the argument copy preference, not an interfering web.** The IV (w85)
   lives in blocks 2 (preheader), 3 (call) and 4 (latch). FBD/UPF show block 3's mask is a0|a1|a2 (the call
   arguments), but `updateforbidden` is called with excl=a1, from the livbb byte +19 set by `str a1 <- IV`. So a1 is
   never forbidden, and the copy credit makes a1 cost 20 against 21 for t0..t5. v1 costs 1e20 here.
   - A laundered argument `func_800A61B0(d, (f32 *) (u32) pts[k], mtx)` (`v/f1.c`) removes the preference. Then
     a1 is forbidden, v1 becomes finite, and v1/t0..t5 all cost 20. The IV (still decided first, 20.5 > 15.5) takes
     v1 and the D_801174C4 pointer (w67) takes t0: 880 words, `words 32 ops 0 norm 0`, with registers swapped
     against retail and homes in retail's order.
   - Adding `PRIO=84:0.7` (IV decided after w67) makes the corner loop exact: 20 rows. With `159:3` (pts[ka] in f1's
     numbering) it is 12 rows.
   - So retail's corner loop has (a) no direct IV-to-a1 copy and (b) the table pointer ranked at or above the IV (or v1
     closed to the IV). The cast is a shaping device, so it is not delivered. Natural forms measured inert or worse:
     k=0 before the call / do-while (`v/a1..a3`; the preheader stays a separate block 2 after the call);
     `ka/kb/e/i/type = D_801174C4[k]` holders (`v/h_*`, ring shift, 290+ words); `k != 4` and `k <= 3` (`v/h_ne`,
     `v/h_le`).
4. **Item (3), the dy temp home, is a home-order effect.** Temp homes go first-fit in web-number order over block
   conflicts (spill.sh CONFL/HOME). The order is: w25 (D_8015201C) home 0, w67 home 0, w85 (corner IV) home 1,
   **w146 (`&D_801174CC[e]`, s5) home 1** (it conflicts with w25 only), w156 home 2, ..., w201 (`db - da`, the dy
   value) home 2. w201 conflicts with w192 (home 0) and w146 (home 1). Retail puts w201 in home 1 (148), so in
   retail w146 is not in home 1. Two candidate causes:
   - w146 conflicts with a home-1 web, for example the IV live into the e-loop region. This is unlikely because the
     IV and pts[ka] share t0 in retail.
   - w25 is numbered after w146, so w146 takes home 0. w25 is numbered from the top-of-function use
     `&D_8015201C[i]`; any read of D_8015201C is the same `ilod.A@0(ldaS)` node, so no spelling was found.
   Declaration order cannot move temp homes; it only sets named-local offsets.
5. Item (4) (`ex * (hz - pz)` operand order) and item (5) (the -0.707f relocation) are unchanged from w17a.

## Best next hypothesis

Items (1) and (2) both say that retail's corner loop and e-loop top differ in a way that is visible only to the
colourer, so look for one source shape that does both of these:
- passes the corner pointer to func_800A61B0 as something other than the bare IV, e.g. through an inlined helper's
  parameter, which is the wave-15 "inlined static's return value is a separate value" lever, while keeping the
  D_801174C4 pointer at or above the IV;
- gives pts[ka] about 2x the weight of pts[kb].

Checked: `frontier stubs` lists no caller-less stub next to 0x800C5644. The neighbours are func_800C54F0 and
effect_cleanup at 0x800C55E4, which umerge already inlines, and there are no stubs at 0x800A6xxx. A helper of this
kind would have to be a non-static inlined function with a body elsewhere. An inlined `(d, pts[k], mtx)` wrapper would
remove the direct a1 copy exactly as the cast does. For item (3), dump w146's and w85's bit vectors (SAVEV) in a variant
where the IV is t0. If retail's w146 must conflict with w85, that pins the IV's extent.

## Files
- `camera_play_script/best.c` (= w17a best.c), `body.c`, `prefix.c`, `retail.s`.
- `camera_play_script/vt.sh LABEL BODY.c`: unit rows plus key-web priorities and colours. `q.sh`: w17a's, TAG=w18d.
- `tools/patch_fbd.py` (FBD/UPF instrumentation) and `tools/oracle.sh` (w17a's oracle runner, TAG=w18d).
- `camera_play_script/v/` (ignored): variant bodies (a1-a3, e1-e4, f1-f2, h_*), ctrace (`ct_*`), spill trace
  (`sp_base.txt`), pretrace (`pre_base.txt`), SAVELOG (`savelog_base.txt`).

## What generalises
- **An argument register copy is a colour preference that also un-forbids that register.** uopt's livbb byte +19
  records "this value is stored into physical register R in this block". `updateforbidden` then excludes R from that
  block's argument mask, and the cost loop credits 1 for the saved move. If retail has `move aN,tX` where yours puts
  the value straight in aN, the retail value is not a bare copy of the argument (a cast, helper return or different
  expression). Proving it takes a laundered-argument variant plus the PRIO oracle.
- **Temp homes are assigned in web-number order, not priority order.** A spill-slot residual between two equal-sized
  frames is decided by which earlier-numbered expression web already holds the home, even when that web is callee-saved
  and never spilled: every coloured type-4 (expression) web gets a home. `spill.sh` CONFL lines name the blocker.
- uopt nocs = card/4 + 2 (card = live blocks without occurrences), so a web spanning a loop of 8-11 blocks needs 4x the
  weighted uses of a web confined to its own block to outrank it.

Time: about 2 h of trace work, plus 3 rounds of variants without strict improvement (stop rule). Permission denials: none.
