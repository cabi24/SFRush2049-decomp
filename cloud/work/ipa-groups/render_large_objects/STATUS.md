# render_large_objects group (hail mary, 2026-09-30)

Group `{func_800F92C8, func_800DE860, render_large_objects}`; only the two callees
are drafted. Scored with `zbuild.py --as1=-r4300_mul`. Stand-in callers
(`__standin_render_large_objects`, `__standin_b`) call each callee twice, otherwise
-O3 inlines the single-call-site `func_800DE860` and `func_800F92C8` (2/52 words
emitted).

| function | words | result |
|---|---|---|
| `func_800F92C8` | 52 | **MATCH** (strict, `claims`); signature `f32 f(f32 a, f32 b, f32 x, f32 outA, f32 outB)` |
| `func_800DE860` | 211 | 203 emitted, structure right, **not matched** (prologue/reg allocation) |
| `render_large_objects` | 1413 | not started as C (analysis only, below) |

## func_800F92C8 (clamped linear map)
Args are (a=$f12, b=$f14, x=$f16, outA=$f18, outB=$f20): x is the THIRD arg (the
IPA float regs f16/f18/f20 are x, outA, outB, not range/out as first guessed).
`if (a<b) {if (x<a) return outA; if (b<x) return outB;} else {if (x<b) return outB;
if (a<x) return outA;}` then `d=a-b; if (fabsf(d) < eps) return (outA+outB)*0.5f;
t=(outA-outB)/d; return t*x + (outA - t*a);`. Needs `float fabsf(float);
#pragma intrinsic (fabsf)`. Note the interpolation is at x, so a and b are the
x-range ends; outA at a, outB at b (the odd-looking clamps are that way round).
Constant eps is `D_80124638` (extern f32; ROM data not available here).

## func_800DE860 (per-car scale reset / rubber band)
Reads: `(active_player_count == 1 && D_80152030 < 5) || D_80150F14 == 0` reset path:
for j in 0..5 `if (D_80153E88[j].flag == 0 || == 6) car[j].scale = 1.0f`.
**The loop variable must be `s32 j`** (s16 gives a rolled loop); with s32 IDO peels
2 + unrolls 4 exactly like the target (`li v0,2`, `lbu 7(v1),15(v1),...`).
Else path: pick `best` (s16, -1 start) = max `player_array[i].dist` among cars with
`car.active != 0 && rec.state < 2 && (car.kind == 2 || D_80152030 == 5)`; then per such
car `d = bd - rec[i].dist; s = d > K2 ? K1 + 1.0 : d*K1/K2 + 1.0; if (D_80150F14 == 1)
s = (1-s)*0.5+1; scale = scale*K3 + K4*s` (K1..K4 = f32 at 0x80124310..1C).
`bd` must be a local (target hoists `rec[best].dist` into $f2). NB the `== 1` test
is on `D_80150F14` (v0 kept from the top), not active_player_count.
Remaining diff (source at `group.c`): target saves only s0 + $f20 (frame 16) and
keeps K1/K2/1.0 hoisted in f18/f16/f20 (+ a second 1.0 web in f14) while K3, K4 and 0.5
are loaded in the loop; mine hoists 0.5 and 1.0 into $f22/$f24 (bigger frame), so
every word after the prologue is shifted. Tried: local k1/k2, literals for K1..K4
(hoisted and constant-folded, worse), K array (worse), operand order swaps.

## render_large_objects (not drafted; notes from reading 0x800F93A0..0x800F9E88)
1. call `func_800DE860()`; n = D_80152744-ish s8 at 0x80150 +10052 (num cars, `t5`).
2. `s16 order[]` (sp+376): `order[car[i].order] = rec[car[i].order].place` (car stride
   0x808, +0x7C6; rec stride 0x3B8, +0xEE).
3. selection by rank into `sp+392` (all), `sp+416` (kind != 2, count s3), `sp+428`
   (kind == 2, count s6) with mark table `sp+404`.
4. writes `rec[list[k]].slot(+0x356) = k`.
5. flags `state_word_a & 8`: set `car.f7EC = 1.0` for kind 1 cars and return; or if
   D_801543CC-ish timer (0x8015+17356?) < 5.0 use linear spread `k*K+K0`; else the
   pairwise squared-distance table `s32[6][6]` at sp+188 (stride 24/row) from
   `rec.pos[3]` (0x8015 0x2818 + 8), ranks, +10/+1 counters.
6. per-car tuning (0x800F9AE0..0x800FA96C): builds the 12 nested clamped-interp
   blocks (`func_800F92C8` args f12/f14/f16 from literals, f18/f20 computed as
   `K*f22 + K` forms, f22 = speed fraction, f28 = ratio) with a tail at 0x800FA844.
Blocks branch to a common tail through `mov.s` copies to f2/f12 and re-load of the
three lb values (v1, v0, t5) after each call: those are the hallmarks of IPA callee
convention (caller-save regs reloaded), so the source is straight-line calls, not a
table loop. No attempt at compile yet.

## Round 5 (cloud/work/r5_c scratch; helpers rb.py, sc.py, rlo_sh.py there)

Group layout changed: `keep` is now `render_large_objects` + `func_800DE860`, no stand-ins.
`func_800F92C8` still MATCH (52/52) and gets its f16/f18/f20 IPA registers from the 12
real call sites in `render_large_objects`.

### func_800DE860 is NOT an IPA function
The target saves `s0` and `$f20` itself (frame 16). Compiled alone at `-O2`
(`score.py fn`, flags `-g0 -O2 -mips2 -G 0 -non_shared`) it emits 212 words (target 211) with
the same control flow, and the same in the group once it is in `keep`. So it can be claimed
as a single function once it matches. Remaining difference is only float register allocation:
- target: d=f0 bd=f2 s=f12, hoisted 1.0 in TWO webs (f14 and f20), K2=f16, K1=f18,
  K3/K4 (D_80124318/1C) loaded late in the join block into f6/f10 (not hoisted), frame 16.
- mine: K1=f2 K2=f12 bd=f14 s=f16, ONE 1.0 web in f22, K3/K4 hoisted to the loop-body top
  into f18/f20 (uopt anticipation hoist), frame 24 (f20 + f22 saved).
Tried (all compile, none changes the allocation): k1/k2/bd/d/s as locals or inline, all
declaration orders, `register`, `one`/`half` locals at several places, int/double/cast
spellings of 1.0 (each un-hoists that use instead of splitting the web), operand/compare
orders, ternary/else-if/`s=1.0; s+=` forms, `scale += ...` forms, struct/array wrapping of K1..K4,
absolute-address casts for K3/K4 (un-hoists them but hoists the `lui` into an integer
register), `volatile` K3/K4 (same), `(u32)`-laundered store pointer (un-hoists K3/K4 but breaks
the integer side: reloads D_80150F14), -O1/-O3, group with keep. ~500 variants. Declaring `bd`
first makes bd=f2 like the target (174 vs 173 aligned words). This looks like the
"IDO-internal, stop" case from the playbook.

### render_large_objects (1413 words): first full draft compiles
`group.c` now holds a complete typed draft (no goto, no m2c spill locals): rank tables
(`place`, `byrank`, `clist`, `nlist`, `cnt` as `s16[]`), `dist2[6][6]`, and the 12 interpolation
blocks written with four macros over the literal pool (FA = `1 - (ka*s+kb)*m`,
FB = `(kc*s+1) - m*(kd*s+ke)`, FC = `(ka*s+kb) - m*(kc*s+kd)`, FD = `ka - (0.5*s+kb)*m`;
block arguments read from the asm by symbolic execution, `cloud/work/r5_c/sym.py`).
Each literal is an `extern f32 D_8012xxxx` (an array base made the compiler share one `lui`
and was worse). Result: 1321 words emitted vs 1413, opcode-LCS 946/1413, strict 1361/1413
words differ, so NOT a match and NOT claimed. The integer half (first ~470 words) is close in
shape (85-99 % per 100-word chunk); differences are register assignment (target keeps the
car count in t5, loop counters t3/s3/s6, tables via s8/s4/s7 base pointers) and the
missing ~90 words in the float half (target re-materialises 1.0 with `lui at,0x3f80; mtc1`
after each call rather than keeping it in f30, and the blocks' scheduling differs).
Structure notes for whoever continues: per-car f22 = `1 - level/5` (times a table factor
`D_80154484[car][lvl + (D_80152570 ? 6 : 0)] * 0.125 + 0.75` when `D_8014A110 == 3`), f28 = rank
fraction (`0.5/(n-1)` for first place, `P/(n-1)` else, 0 when `D_80152030 == 5`); blocks pick
(outA target f2, outB target f12) from `func_800F92C8(a, b, x=gap, outA, outB)` depending on
place (first / last / middle with neighbour-window and kind-2 counts), then both are rate
limited by `D_801247E0/E4/E8` (outA) and `D_801247E8` (outB) toward the car's 0x7EC/0x7F0 fields.
