# draw_number -> 110/110 MATCH (real function, extent 110 words)

## Current status (rescored 2026-09-30, master d0891f3)

**110/110 MATCH** (strict, `extscore.py --norm`: exact after alignment 110/110, structure 1.000; extent 110
words). Caveat: matches with stand-in callers, not spliceable as is. Closure (approximate `closure.py`): real
callers `draw_speedometer`, `func_800CCCCC` and the menu cluster are absent; `func_800C7578` is an extern.

Real head 0x800C760C (`addiu sp,-24`), IPA params `$s1` (object pointer) and `$s2` (index),
callers `draw_speedometer`, `func_800CCCCC`. It makes twelve calls to `func_800C7578(p, (u8)idx,
slot, s8 value)`: ten byte-table lookups indexed by `idx` and two float scalings
(`value = (s8)(f*100 - 75)`). INDEX's 973 insns (`menu_confirm_render`) is a caller.

Verify: `python3 cloud/work/tools/extscore.py cloud/work/ipa-groups/draw_number`

- Each table needs its own extern symbol (`D_8011103C[]`, ...). One shared base makes IDO hoist
  the base into a saved register.
- Not in `keep`, two stand-in callers (`caller_a`, `caller_b`) give IPA registers.
- `func_800C7578` (37 words) is a plain extern.

Blockers: none for the function; real callers not in the group.

## Spliceability

Unspliceable as is: MATCH, but callers are stand-ins (`caller_a`/`caller_b`). The stand-ins only reproduce the IPA register/frame context; they are not
the retail callers, so this C cannot go through `blob_splice` until the real callers are in the group
(or the maintainers' whole-module IPA is used). Scores above were reproduced on 2026-09-30 with `extscore.py --norm`.
