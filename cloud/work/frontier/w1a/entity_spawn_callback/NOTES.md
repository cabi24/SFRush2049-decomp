# entity_spawn_callback (0x80090088, 104 words) - 9 words off, single function, -O3 or -O2

What it is: recursive removal of a node from the 0x44-byte scene-node table `D_8012E700`
(`u16 id @0x14`, `s16 child @0x16`, `s16 sibling @0x18`): `(s16 idx, s32 freeChildren, s32 freeSiblings)`.
Optionally frees the sibling chain and the child chain (recursive `(link, 1, 1)`), unlinks `idx` from whoever
references it (`func_8009002C` = find node whose child is idx, list head `D_8015B254`, `func_8008FFD0` = find node
whose sibling is idx), marks the node free (`id = 0xFFFF`, links -1) and trims the high-water count `D_80156990`.
No arcade ancestor found (N64 scene graph). It is plain ABI: nothing is kept in a register across any call, so it
does not need a group (the frontier `group` flag is a false positive from `li a1,1; li a2,1` before `bltzl`).

Best: `best.c`. Scorer (`tools/cloud/score.py fn cand/entity_spawn_callback.c entity_spawn_callback --flags "-g0 -O3 -mips2 -G 0 -non_shared"`):
`9/104 words differ`, all in +0xdc..+0x100 (same at -O2).

Levers found (each verified by removing it):
- `if (0) {}` anywhere in the body (or a `do { } while (0)` around most single statements): without it uopt gives
  `idx` s0 (frame 48, 61 rows off); with it `idx` stays in its home slot (`lh 34(sp)` at every use, no `sll/sra`)
  and `n` lives in a3 with spills around the calls, as retail. 23 of 24 placements behave the same, so this is a
  procedure-wide effect of a constant-false branch, not of block count (`if (0)` with 0, 1, 2 ... 40 dead calls
  is identical). The original probably had a compiled-out debug block or a `do {} while (0)` macro.
- no named local for the two search results: reusing the `freeChildren` parameter gives the 32-byte frame
  (a named `s32 r` costs 8 bytes). Quirk; an unnamed-result spelling would be better if one exists.
- `volatile s16 idx` also keeps idx in memory but orders the final loads/stores wrongly (28 rows): rejected.
- `if (0) { dbg(&idx); }` (address-exposure probe) keeps idx in memory but hands s0 to `n`: rejected.

Residual (lane: uopt web/CSE, then as1 follows):
1. retail materialises `&D_8015B254` once in v0 (`lui/addiu`, then `lh t9,0(v0)` ... `sh t2,0(v0)`); mine uses two
   symbolic accesses. Array / struct-field / `*(&x)` spellings do not create the address web. A pointer variable
   does (`alt_address_web_5rows.c` parks it in the dead `freeSiblings` parameter: 5 rows), but a named `s16 *`
   local costs a frame slot (28 rows).
2. retail loads the `func_8008FFD0(idx)` argument in the else block (`bnel t8,t9; lh a0,34(sp)` plus the dead copy
   at +0x100); mine loads `a0` before the branch (`lh t8; lh a0; bne; nop`), i.e. uopt coloured a two-block
   fragment of idx. Unchanged by operand order, casts, K&R or s32 callee prototypes, goto/nested/else-if forms.

About 80 variants (`variants/`), no movement on these two for the last ~50. Tested last and rejected: the head
test/store (or all three unlink stores) as an inlined `static void relink(s16 *link, Node *n)` - the inlined
parameters cost a frame slot (frame 40, 28 rows). Best next hypothesis: the address web must come from an
expression, not a variable - look for a spelling in which `&D_8015B254` is loop-invariant inside a real loop
(uopt registers loop-invariant addresses), e.g. the head test living inside a `do { } while (0)` that also
contains a store uopt cannot hoist; or revisit once the whole-program unit exists (C) in case the constant-false
block is really an inlined empty debug callee.
