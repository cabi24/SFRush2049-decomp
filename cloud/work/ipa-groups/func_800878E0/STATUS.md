# func_800878E0 group (display-list mode helpers) - r5_f, 2026-09-30

Members: `func_8008A38C` (22w) **MATCH** (also matches alone at `-O3`, see cloud/matches/func_8008A38C.c; at `-O2` the
base register `t0` is not hoisted). Context (not claimed):
- `func_800878E0` (74w, other-mode flag setter; sibling of the matched `func_8008705C`): 6/74 words differ, only
  instruction *order*: the `0xFCFFFFFF/0xFFFDF6FB` block has `ori` order swapped, and the two `w0=0xE200....; w1=const`
  blocks store `w1` before `w0` in the target but `w0` first in ours (writing `w1` first swaps t6/t9 instead). 24 source
  variants tried (w0/w1 order per block, temporaries, pre/post increment).
- `func_8008A644` (24w): 12/24 differ. Target uses `a2` (D_8012E67A base) and `a1` (display-list pointer); ours picks
  `a3`/`a2`, i.e. the real callers (Input_ProcessGameplayPad, render_helper, func_800F6928) probably hold `a3` live across
  the call. Removing it from `keep` inlines it (25 words, worse). `sll t9` then `lui` order also differs.
- `func_80086A50` is the 387-word stand-in from the func_8008705C group.

Verify: `python3 cloud/work/tools/extscore.py cloud/work/ipa-groups/func_800878E0` (members use no `targets`; all are `.text.<name>` sections).

Note (round 5): func_8008A38C is claimed as a single function in cloud/matches/func_8008A38C.c (-O3); this group keeps it only as a reference, so claims is empty to avoid a double splice.
