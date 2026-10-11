# w17a results (wave 17)

Assignment: camera_play_script (0x800C5644, 3,520 B, whole-program unit, -O3). Start: w16c best.c re-scored in the
current unit: `words 624 ops 5 norm 5 | frame 608/600` (879/880 words; the ugen int ring is one register off from
+0x80 onward, so almost every t6..t9 name differs).

## Summary

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| camera_play_script | 3520 | not matched; **624 -> 27 aligned word rows, 1 mnemonic row, frame 608 = retail** (w16c: 5 norm rows, frame 600) | `-g0 -O3 -mips2 -G 0 -non_shared` (whole-program unit) | `python3 -m tools.conveyor.pipeline.blob_unit --tag w17a --jobs 2 score camera_play_script --with cloud/work/frontier/w17a/camera_play_script/best.c --neighbours` -> `FAIL camera_play_script: 793 of 880 words differ; compiled body is 879 words, target 880; +0x208: .rodata+0x498: retail words encode 0x01284282, outside the image` / `locked bodies that differ in this unit: 0`; `tools/trace/us.sh` -> `words 27 ops 1 norm 1 \| frame 608/608` |

No match, so nothing to integrate (no cloud/matches file, no group, no overrides). The strict "793 of 880" is the
missing `move a1,t0` (+0x14c) shifting every later word by one; compare drafts with `us.sh` aligned rows.
`best.c` = `prefix.c` (w15h's, unchanged) + `body.c`. No shaping devices, no unused locals, no implicit int.
The two zero spellings (`0.0f`/`0.f`) are literal choices that select uopt constant webs (w16c lesson), disclosed here.

## Levers (each traced first; all measured in the unit)

1. **idx[0] goes through an int variable** (624 -> 299 words, same 5 norm rows). Retail `lhu a2,568(sp); ...;
   sll t9,a2,3`: idx[0] is a coloured web in a2 with one use; in w16c's draft it is a ring temp, so the ugen int ring
   is shifted by one for the rest of the function. The matched siblings func_800C3AD0/input_process_controller
   write `i = idx[0]; e = i + D_8015201C;`. Measured: `k`->a3, `e`->s4, `ka`/`kb`->t3, `beste`->a2 but stored,
   `type`->a2 but merges with the line-120 `type` (v0 in retail); a NEW trailing local gives a2 and the right top
   code but grows the cfe area 448 -> 456 (temp homes 8 too low). An a0..a1-free variable whose only live block is
   the top block gets a2 (a0/a1 are the func_800AD650 arguments in that block).
2. **`c1` is not a variable; its slot (-416) is `s32 i`** (-> 121 words, 1 norm row, **frame 608**). Retail keeps the
   k-loop product `(hx - px) * ez` in caller-saved $f18 and the final clip's `pz * hx` in $f16: two registers, so
   not one variable. With `c1` gone, `i` takes its slot (named-local offsets unchanged, cfe area 448). The k-loop
   becomes `if (a < b) goto next; u = ex * dz - ez * dx; if (u != 0.0f) { u = (b - a) / u; ... } px = pv[k][0];
   pz = pv[k][1];` -- the denominator through `u` exactly as the final clip already did. That is retail's shape:
   den = coloured expression web in $f22 computed before the branch, u in $f12 in both clips, t in $f30. (w16c's
   `u` attempt failed only because c1, a variable crossing calls, used up a callee-saved FP register.) Final clip:
   `if (!(pz * hx < px * hz)) { ... u = (pz * hx - px * hz) / u; ...}`. Variant without `u` as den: 39 norm rows.
3. **`ez = pv[k][1] - pz;` before `ex = pv[k][0] - px;`** (121 -> 108): retail pv[k][1] $f24, pv[k][0] $f26.
4. **Zero spellings**: retail keeps 0.0 in $f12 for line 94 (`-px * hz > 0`) and line 97 (`u > 0`) but
   rematerialises it for line 95 (`dz != 0`). Line 94 `0.0f`, line 95 `0.f`, line 148 `0.f` (108 -> 32).
5. **`u = (hx - px) * ez; if (ex * (hz - pz) < u) goto next;`** (32 -> 29): the tie between the two k-loop
   products (save 150 each) goes to the lower PRE bit; with the product first assigned to `u` (copy-propagated),
   b is numbered first and takes $f18, a takes $f20 as in retail. (Compare spelling `b > a`, `a < b`, `b - a > 0`,
   operand orders: all inert, uopt canonicalises.)
6. **`old = col->poly;` inside the else branch** (29 -> 27): fixes the `move s1,s3` / `move s0,v0` order.

## Remaining residual (27 aligned rows), with causes

1. **Corner-loop IV** (7 rows, +0x130..+0x1a4): ours a1, retail t0 + `move a1,t0`. Colour-only:
   `force.sh o1 camera_play_script "p1:w85=c7"` removes all 7. p1cost: a1 20.0 (copy credit for being the a1
   argument), t0 21.0; forbidden v0/a0/a2/a3 (a0/a2 = the func_800A61B0 argument registers, a3 = k). Some web
   coloured a1 overlaps the corner loop in retail, or retail's IV is not the argument value. Not found; 5 call-
   argument spellings and loop-counter choices measured inert (`v/e1..e5.c`).
2. **t0/t1 swap** (8 rows, +0x340.., +0x384.., +0x7b8..): retail colours `pts[ka]` (PRE bit 156) t0 and the
   unroll remainder (bit 515, `-(n-1 & 3)`) t1; ours decides 515 first (save 15 vs 8) and gives it t0. Retail's
   `pts[kb]` is t2, so retail's order is pts[ka] > remainder > pts[kb]: pts[ka] has the higher priority in retail
   (ours: pts[ka] and pts[kb] both tot 40, nocs 5). Independent of item 1 (forcing the IV does not change it).
3. **dy temp home** (9 rows: `swc1/lwc1 $f24,148(sp)` vs 144): retail's `db - da` temp shares home -460 with the
   corner IV; ours goes to -464 because the e-loop pointer IV web (bit 146, s5, home -460) is numbered before it
   and they conflict. Homes are first-fit in web-number order (`spill.sh`). Same frame size (608) either way.
4. **One mul operand order** (+0x8f0, `mul.s $f20,$f16,$f6`): uopt puts the plain variable `ex` second
   (`(hz-pz)*ex`); retail has `ex` first. `ex * 1.0f * (hz - pz)` (an identity, would be a shaping device) fixes it
   (`v/o5.c`, 28 rows); every natural spelling measured is inert.
5. `lui at,0x8012 / lwc1 16268(at)` vs ours (+0x1ec/+0x20c): the own-rodata relocation (-0.707f), not a code
   difference (`unverified 2`).

**Best next hypothesis:** items 1-3 are all web-order / priority effects in the corner loop and e-loop top. Item 2
says retail's `pts[ka]` address web is worth more than ours: look for a source form with more `pts[ka]` uses or uses
in heavier blocks (e.g. `da`/`dx`/`hx` read through `pts[ka]` in a different statement order, or `hx`/`hz` from
`pts[ka]` inside the e-loop rather than after the conv loop). Item 1: dump the interference list of w85 with
`tools/patch_lbb.py` extended to `lr->interfere`, and check whether retail's corner loop passes a value other than
the IV in a1 (e.g. `&pts[k]` built from `k` while the IV serves only the loop test).

## Tools (lane dir)
- `camera_play_script/q.sh LABEL BODY.c` (unit score + key-web line, now prints cfe errors), `fq.sh` (w16c's).
- `tools/patch_lbb.py`: adds `[LBB] web=W bbs=...` (each live range's block list at its p1 decision; env `LBB=1`)
  on top of w16d's `patch_spl.py`; builder binary `~/rush2049/scratch/frontier/w17a/bin/uopt_lbb`.
- `tools/oracle.sh LABEL BIN "ENV=..." [udiff args]` (w16d's oracle generalised), `tools/ours.sh` (our disassembly).
- `camera_play_script/v/`: every variant body (`d1` lever 1, `j1` lever 2, `k3`, `z5`, `o1`, `p2` = best), ctrace /
  spill / pretrace outputs (`ct_*.txt`, `sp_*.txt`, `pre_*.txt`), `diff_vs_w16c.txt`.

## What generalises
- **A one-use load in a coloured argument register is a variable**, and its colour tells you which one: a variable
  whose only live block is that block takes the first argument register not used by the block's call. Use the
  sibling idiom (`i = idx[0]`), then find the slot by elimination: retail's frame and named offsets say which
  declared local is fake.
- **A variable that holds two values in two different registers in retail is two variables (or an expression).**
  `c1` ($f18 in the k-loop, $f16 in the final clip) was a fake named local that also cost a callee-saved FP
  register and broke `t`'s colouring; removing it made w16c's `u` denominator form work.
- **Ties between equal-priority expression webs go to the lower PRE bit, and text order of a compare cannot change
  it** (uopt canonicalises the compare). Assigning one operand to a dead variable first (copy-propagated, no code)
  numbers it first.

Time: about 3 h of trace work (stop rule reached). Permission denials: none.
