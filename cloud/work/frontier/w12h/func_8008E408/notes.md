# func_8008E408 — w12h: closed (EQUAL in the unit)

The +4 spill-temp shift (w9c/w10h, 22/386) was the merged frame, not an extra coloured web.
w11b's rule decides it: umerge gives every inlined callee an area that starts 8-aligned, in call order.
Measured (`../tools/mu.sh`; cfe = caller locals, merged = `Udef Mmt` that `f_spilltemps` starts from):

| variant | caller cfe | trailing inlined call | merged | rows |
|---|---|---|---|---|
| w10h best (pF0..pF8) | 0xb0 | none | 0xd0 | 22 |
| drop 1-2 pads | 0xac/0xa8 | none | 0xc8 | 64 |
| drop 3-4 pads | 0xa4/0xa0 | none | 0xc0 | 64 |
| drop 3-4 pads | 0xa4/0xa0 | empty `void hz(void)` | 0xc8 | 64 |
| drop 3-4 pads | 0xa4/0xa0 | helper with one s32 local | **0xcc** | **EQUAL** |
| drop 1-2 pads | 0xac/0xa8 | helper with one s32 local | 0xd4 | 42 |
| drop 5-6 pads | 0x9c/0x98 | helper with one s32 local | 0xc4 | 42 |

So retail's merged frame of 204 is the two inlined func_8008B2E4 areas, then an area of one word after them.
A natural-looking helper gives the same result: the last `func_8008E26C` call inside a static helper whose named
local holds the result (`a6.c` = best.c; `a4.c`/`a7.c` are s32-returning variants that also match). A helper
whose only local is the table value (`a1.c`) is 2 words off. `return func_8008E26C(...)` with no local
(`a5.c`) does not reserve the word (64 rows). Using the helper for both objects (`a2.c`) is not inlined the
same way (346 words).

Remaining disclosure: 18 unused pad locals (down from 22) still set the cfe-local offsets.
They are the exact residual of the frame: retail's named-slot offsets need them.
