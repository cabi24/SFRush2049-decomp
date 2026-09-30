# func_8010A7A4 -> 35/75 words aligned (real function, extent 75 words, not 242)

## Current status (rescored 2026-09-30, master d0891f3)

**Not a MATCH. Builds; 0/75 words strict; 35/75 words match after alignment, structure 0.671.** Scored with
`python3 cloud/work/ipa-groups/func_8010A7A4/extscore.py --norm cloud/work/ipa-groups/func_8010A7A4`:

```
func_8010A7A4  size 71/75  50/75 words differ (position-based)
```

Blockers: stack-local shape (retail frame 56 vs ours 48), the hoisted `li a0,1`, register naming.
Closure gap (open): the 14 call sites in unregistered-head functions `func_8010A8D0` and `func_8010AEAC`.

Real head 0x8010A7A4 (`lui t6,0x8011; lw ...; addiu sp,-56`) in the opaque run
`blob_80107edc.s`; the registered `.text.func_8010A7A4` section is 75 words, which agrees with the
epilogue at 0x8010A8C8. INDEX's 242 insns are wrong. Callers: `func_8010A8D0` (13 call sites
via a jump table, `a0` = 1..13 and 0) and `func_8010AEAC` (`s1` = 160). Params (IPA):
`a0` = index, `s1` = x, `s2` = y (s16), `s3`, `s4` = values forwarded as `a2` to `state_utility`.

Draft: `f.c`. Verify: `python3 cloud/work/ipa-groups/func_8010A7A4/extscore.py cloud/work/ipa-groups/func_8010A7A4 --norm`

Logic: if `D_80116D0C == 1` { copy two words `D_801146BC/C0` into `D_80118E28/2C` and into
dead stack locals, `dispatch_handler(D_80116514[idx].f[0] < D_801248D4 ? 1 : 22)` } else
`dispatch_handler(idx == D_80116D9C ? 22 : 1)`; then `state_utility(x-105, y, s3)` and
`state_utility(x+40, y, s4)` with s16 casts.

Differences left (71 vs 75 words):
- retail keeps the two copied words in a 4-word stack area (frame 56, stores at 32/36 and 40/44
  of two different locals, one per branch); mine (`volatile` locals) gives frame 48 and a
  different order. A plain struct copy gives lwl/lwr-style code, worse.
- retail emits `li a0,1` before the compare (hoisted), mine does not; and the s16 sign-extension
  chain (`sll/sra` then `move`) is one step shorter.
- register naming of the temporaries.
Callee `state_utility` and `dispatch_handler` are plain externs.

Blockers: the stack locals shape, and the IPA context (callers are 14 call sites in two
unregistered-head functions, not in the group).

## Spliceability

Unspliceable as is: not a match, and callers are stand-ins (`caller_a`/`caller_b`). The stand-ins only reproduce the IPA register/frame context; they are not
the retail callers, so this C cannot go through `blob_splice` until the real callers are in the group
(or the maintainers' whole-module IPA is used). Scores above were reproduced on 2026-09-30 with `extscore.py --norm`.
