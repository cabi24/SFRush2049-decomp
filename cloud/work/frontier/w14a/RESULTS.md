# w14a results (matching wave 14, lane w14a)

No function reached a strict MATCH. No `cloud/matches/` files, no groups, no overrides. Nothing to integrate.

## Scorer output (this session, verbatim)

| Function | Bytes | Best draft | State | Scorer line |
|---|---|---|---|---|
| wheel_torque_apply | 140 | `wheel_torque_apply/best.c` | 6 of 35 words off, frame 64/64 matches | `wheel_torque_apply: 6 of 35 words differ \| words 8 ops 5 norm 5 \| frame 64/64` (unit, `us.sh`) |
| func_8008AD6C | 164 | `func_8008AD6C/best.c` (= q1) | 34 of 41 words off | `func_8008AD6C: FAIL 34 of 41 words differ \| words 34 ops 12 norm 12 \| frame 0/0` (unit) |
| func_800A1BB4 | 184 | `func_800A1BB4/best.c` (= b) | 6 ops mismatched, 42 words vs 46 target | `func_800A1BB4: FAIL 29 of 46 words differ; compiled body is 42 words, target 46 \| words 24 ops 6 norm 6` (unit) |

Standalone `score.py fn` (builder, `-O3`): wheel best = `want 35 words... 6 differ`, same residual as the unit.

## wheel_torque_apply (0x800AC668), about 20 variants

Recovered from the disassembly and callees:
- `func_800A473C` is strcpy (dst, src), returns dst. The key is a 20-byte-stride record `{u32 hdr; char name[28]}`; strcpy writes `name` at sp+36, and the bsearch key is the struct at sp+32. The frame is 64 with `name[28]`; `name[16]` gives 56 and `name[28]` plus a named `rec` local gives 72.
- `entity_name_copy` is the bsearch matched in `cloud/matches/entity_name_copy.c`. Its comparator is `0x8008AD48` (`pointer_offset_wrapper`), which adds 4 to both args and calls `func_8008AD04`. Element stride 20, count `*(u32*)D_801497FC`, base `D_80149818`.
- The `s16` return is `D_80149D90` (set to -1 first). `D_80149B80` is the record word, passed to `differential_output`.
- Arcade ancestor: the symbol file names `arcade.drivetra.c.drivetrain(41-56)` ("apply dwtorque to drive wheels with differential split"). Only `differential_output` (arcade `magic differential`) matches it; the strcpy/bsearch wrapper has no arcade analogue.

Residual, 6 words: (a) the `sh t6,-25200(at)` (store of -1 to D_80149D90) sits before `addiu a0,sp,36` in ours; retail has it in the strcpy call's delay slot. Reordering the source (store after the call, `(s16) -1`, `0xFFFF`, a volatile-cast store) did not change it. (b) Retail uses separate `lui at` / `lui a0` for the store and reload of D_80149B80; ours shares one `lui`. The `volatile` on D_80149B80 is needed to stop the reload being forwarded, but the address CSE persists under every spelling tried (volatile pointer, raw 0x80149B80 address, plain/volatile mixes).
Not yet tried: `force.sh` on the store/delay-slot scheduling (`as1t.sh`), and the lui-split in the ugen trace.

## func_8008AD6C (0x8008AD6C), about 12 variants and 1 force probe

Retail: word in v0 (loaded first), top mask in v1, lo in a1, `lui a2,0xffff` mask. Retail loads word 4(a0) before storing word 0 (the 0x06/0x07 path) and the shared 0x06/0x07 path ends with `li v0,2` in a jr delay slot.
- Every variant gives 34–35 of 41 words off. The partial-sum form (`q3`) and the load-after-partials form did not move it. -O2 is no better.
- `force.sh q1 func_8008AD6C "p2:w0=c1"`: the force is declined with `forbidden=0x5000000000000000` (c1 and c3). Forcing the webs that hold v0/a0 elsewhere (w2=c3 applied, w8 to c4/c5/c7/c8) does not free c1; the interferer is not identified. Residual is colour (the word web), not only structure.
- Not tried: `as1t.sh`, and a `force.sh` oracle on the word web with a set of earlier v1/a0 forces.

## func_800A1BB4 (0x800A1BB4), about 8 variants

Recovered structure: `Rec` is 772 bytes; `active` at +11; `Sub` is 40 bytes with `on` at +5 inside `sub[16]` at +128; `ModelData` has `enabled` at +72, `wheel` at +17, `next` at +0; `ModelList` is 16 bytes with `head` at +0; D_80144D68 is indexed by 16.
- The best draft (`b`: `next` named before the enabled test) gets the frame and the body except for two branch senses: the retail early `jr ra` for `!active` is a separate return (ours merges to one exit, 42 vs 46 words), and the break path `beqz; nop; b exit` (ours `bnez` to the same tail).
- Tried: early return vs `else { return; }`, empty-then `if (on == 0) {} else break;`, `for` form, `if (p == 0) break;` inside the loop (worse, 43 words), braces on the final clear. None removed the 6 branch-sense ops; IDO merges the return.

## Integration notes

Nothing to integrate: no match, no group, no unit_overrides, no supersession. The three drafts are working residuals only.

## What generalises

- Callees named by behaviour (strcpy = func_800A473C, bsearch = entity_name_copy) and a struct whose name field sits at +4 (comparator adds 4) explain a frame that otherwise looks like unexplained locals.
- Return-duplication (`jr ra` copies per exit) is not reproduced by early returns or else-return forms; it is a branch-layout residual, not a colour one.
