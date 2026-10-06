# func_8008E408 (1544 B) — w10h: the spill-temp residual traced to its allocator

`best.c` is w9c's best unchanged (22 of 386 words, all spill-temp homes). No variant beat it. This
note records *why* the homes are where they are, measured with an instrumented uopt, so the next
attempt starts from the mechanism and not from declaration padding.

## The allocator (measured, IDO 5.3 uopt, whole-program unit)

The temp homes at sp+24..47 are not ugen's. They come from **uopt `f_spilltemps`** (recompiled
`uopt.c` 0x46d58c), which runs once per procedure after colouring:

1. Start: `tempdisp` (global 0x1001c4b4) = the procedure's `Udef Mmt` size after umerge
   (`f_readnxtinst` sets it on the Mmt `Udef`). Here 208 (0xd0).
2. Population: every web in set 0x1001cbe0. In this unit that is **every coloured expression web**
   (kind 4: ixa/ilod/add/cvt...), whether or not it ever crosses a call. 1663 of 1664 entries in the
   whole unit are kind 4 size 4 (one is size 8). Variables (kind 3) never get a temp; they spill
   to their own homes; constants (type 2) are rematerialised.
3. Order: ascending web number = itable symbol number = **first occurrence in the ucode**.
4. Slot choice: greedy. For web w, collect the temps already given to earlier webs that share a
   basic block with w (bb set at +0x15c); take the first temp of *equal size* not in that set; else
   allocate a new one at `-(align(tempdisp, min(size,8)) + size)`.
5. ugen later only references the temps it needs; temps of webs that never spill stay as holes.

Trace for best.c (tools/patch_tmp.py build, `TMPLOG=1`):
```
spill web=5   (car, ixa)        temp0 off=-212  sp44
spill web=41  (&D_801392D8[idx]) temp1 off=-216  sp40   (shared with w68 = flags)
spill web=42  (latch value)     temp2 off=-220  sp36   (shared with w83 = &o->m)
spill web=68                    temp1
spill web=83                    temp2
spill web=105 (matrix-loop ixa) temp3 off=-224  sp32   never referenced
spill web=107 (matrix-loop ixa) temp4 off=-228  sp28   never referenced
spill web=208 (wheel ptr)       temp5 off=-232  sp24
f_spilltemps disp 208->232
```
Retail has car at -208, flags -212, &o->m -216, holes -220/-224, wheel -228: **the same eight
webs, every one shifted by exactly +4**, plus one more 4-byte hole at -204 (sp52).

## What that rules out and what it requires

- umerge rounds the merged frame to 8 whenever it inlines (measured: cfe 160/164/168/172/176/180/184
  bytes of caller locals -> merged 192/192/200/200/208/208/216; inlined helpers of any shape add 8).
  So `tempdisp` can never be 204, and **no declaration edit can produce retail's homes**: removing one
  4-byte pad changes the frame by 8 (0xFF08) and leaves the temps at 24..44 (64 words, `a1.c`).
- Retail therefore has merged Udef = 200 (one 8-byte quantum fewer than best.c: about two of w9c's
  22 invented pads are not real) **and one more coloured expression web X** that is
  (a) earlier than car in itable order (car is web 5, so X must be one of the first few symbols:
      it appears in ucode before `&D_80152818[idx]`),
  (b) size 4, and
  (c) shares a basic block with car, w41, w42, w68, w83, w105, w107 and w208 (otherwise one of them
      reuses its slot and the shift is not uniform) - i.e. X is live across the whole function,
  and whose code is entirely absent from retail (no def, no save, no use).
- Simulated, X at -204 reproduces all retail homes exactly (car -208, {w41,w68} -212, {w42,w83} -216,
  w105 -220, w107 -224, w208 -228; 200+28 rounds to the same 232 / frame 256).

(c) plus "no code" is the open contradiction: a coloured web live across calls needs saves. Tested
and refuted as X: comparisons in empty-body `if`s (`idx >= 8`, `type > 3`, `(u32)idx >= 8`,
`idx < 0 || idx >= 8`) get no web; arithmetic on parameters (`idx + type`, `type - 4`,
`idx * 2 > 15`, `(idx|type) == 0`) in an entry *and* exit empty `if` does become a long-lived web
with a low number (web 2) and pushes car to the second slot - exactly the wanted temp shape - but
its def and saves stay in the code (`addiu t0,a1,-4` + `sw t0,52(sp)`, 374 words). The latch
address `D_801392D8[idx]` before car rewrites the switch (367-373 words). `car->h248` in an
empty if makes the speed load a PRE web (temp shared, 136 words). car spelt as `D_80152818 + idx`,
`(Car *)((u8 *)D_80152818 + idx * sizeof(Car))` or `[(s16)idx]` is byte-identical to best.c.
Empty inlined helpers with 0-2 parameters, a float parameter or a return value change only the
merged size (always +8).

Next hypothesis: an X whose computation is already in the retail code in a pool register, for
example a value IPA knows survives the callees (no save needed when the callees do not clobber
it), or a PRE temp of the idx sign-extension `(s16)a0` that the cfe front end materialises for an
s16 parameter. Check the callees' clobber sets with `frontier show` on each callee first, then
look for a retail register that holds the same value from entry to exit.

## Variants

All in `variants/` (score with `tools/st.sh FILE.c`, which prints unit score, sp census and the
spill-temp allocation for this proc). ~50 builds.
