# func_800F0F44 -> 46/116 words aligned (real function, extent 116 words)

Real head 0x800F0F44, IPA parameter `$s8` = player index, callers `func_800F1210` and
`func_800F1930`. INDEX's 815 insns (`func_800F1210`) is the caller. It looks up a player's
network/slot state: `D_80146198[idx] = 0`; result code `D_801461C0[idx]` = -4 (inactive,
`D_80156CF0[idx][0] == 0`), -1 (slot byte 1 zero), -2 (byte 5 set), -3 (byte 6 set), else 0;
if `D_8014A110 == 2` it walks the list `D_80152028` (every node `o`: `o->q->id == idx`,
matching `D_8014978C`/`D_80152570` bytes with `o->b8/b9`, `func_800950AC(name, ref name, 14) == 0`)
stores the node in `D_80146198[idx]`, and sets the result to 1 if `func_8008AD04(o->pos, r->pos)==0`
and two words match, else 2. Slot stride is 772 bytes at `D_80144030`, `r = *D_8014A160`.

Draft: `f.c`. Verify: `python3 cloud/work/ipa-groups/func_800F0F44/extscore.py cloud/work/ipa-groups/func_800F0F44 --norm`

Control flow and word count are near (113 vs 116). Differences are register allocation:
- retail puts the index in `$s8`, constants 1 and 2 in `$s6`/`$s7` (the `== 2` compare uses `s7`)
  and the `&D_80146198[idx]` pointer in a stack slot (`sw t8,24(sp)`), frame 72;
  mine puts the index in `$s7`, hoists three global addresses into `s6-s8`, no spill, frame 24.
- The IPA parameter register does not follow the caller stand-ins (tried 8 live values in the
  callers: unchanged). Source spelling of the two array stores (pointer variable or direct) does
  not change the output.
The list walk is written `for (n = head; n; n = o->next) { o = *n; ... }`, which reproduces the
retail alternating `s1`/`s0` loads.

Blockers: register pressure/allocation (needs the real `func_800F1210`/`func_800F1930` context).
