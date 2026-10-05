# func_8008E0B8 — not matched (23/35 strict at -O3, best.c)

Normalises the 3-float vector at a0 in place and returns its length; returns 0 when length <= 1e-5f
(own literal 0x3727C5AC at 0x8012394C; 1.0f is lui 0x3F80). Only jal caller: func_8008E408.

Residual (one lane): retail homes z = v[2] in a 40-byte frame (store to sp+4 right after the load, reload
for z*z and for z*inv) while x, y stay in f12/f16; length goes to f2 first. No natural spelling tried
reproduces a memory-homed scalar: volatile z reads twice; `&z` taken in dead code homes it at sp+4 (frame
16; six unused locals give the 40-byte frame) but the FP colouring is then x=f2, y=f12, len=f16 instead of
len=f2, x=f12, y=f16 (traced: proc 79 colours webs in web order at equal tiny priority). Local arrays,
union, struct copy, inlined helpers, v[0]/v[1] read directly, late-folded forms (`+0.0f`, `*1.0f`): all
worse or same. About 190 variants (most generated). Not arcade: arcade NormalVector/direction differ.
Next hypothesis: an inlined static (deleted) helper taking `&z` or a K&R float parameter; check
`frontier stubs` near 0x8008E0xx and the arcade `scalmul` address-taken pattern (`bp = &bval`).
