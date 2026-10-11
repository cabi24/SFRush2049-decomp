# w15h results (wave 15)

Assignment: camera_play_script (0x800C5644, 3,520 B, group, -O3); optional input_deadzone_apply.

## Summary

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| camera_play_script | 3520 | not matched; 133 -> 48 register-normalised rows (30 with the -1 web forced to t5); frame 600 vs 608 | `-g0 -O3 -mips2 -G 0 -non_shared` (whole-program unit) | `python3 -m tools.conveyor.pipeline.blob_unit --tag w15h --jobs 2 score camera_play_script --with cloud/work/frontier/w15h/camera_play_script/best.c --neighbours` -> `FAIL camera_play_script: 823 of 880 words differ; compiled body is 873 words, target 880; +0x208: .rodata+0x43c: retail words encode 0x01284282, outside the image` / `umerge inlined into it: effect_cleanup, func_800B61A8` / `locked bodies that differ in this unit: 0` / `blob_unit score: 0/1 equal` |
| input_deadzone_apply | 3580 | not attempted (no concrete idea for w12a's carried-over float free-list; time went to camera_play_script) | | |

No match, so nothing to integrate: no cloud/matches file, no group, no overrides. The strict word count is
dominated by the 8-byte frame shift (every sp offset); use `tools/trace/us.sh` norm rows to compare drafts.

Files: `camera_play_script/best.c` (= `prefix.c` + `body.c`, for `blob_unit --with`), `cps/` (all variants,
`q.sh` = unit score + key-web colouring summary, `mk.sh`), traces `ct_*.txt`, diffs `d_*.txt`, `retail.s`.
The prefix is w10h's with one fix: `func_800B61A8`'s 4th parameter is `unsigned char` (see 5).

## camera_play_script (0x800C5644, 880 words)
Start: w10h draft (`../w10h/camera_play_script/best.c`): `us.sh` 133 norm rows, frame 632.
Not a camera function: it tests the car's corner edges against one track polygon (see w9h NOTES).

### Findings so far (camera_play_script)
1. **func_803914B4 call is the inlined locked `effect_cleanup(s8,s8,s8)`** (src/blob/effect_cleanup.c, same
   wrapper entity_update uses): `effect_cleanup(car->pidx, car->pidx, -1)`. This explains retail's odd "4th arg"
   (a3 = the inlined param `a`), the `(s8)` of the -1 web, and the double sign-extension. The w9h/w10h draft's
   4-arg prototype was false. 133 -> 96 norm rows (with merge-order fix below).
2. **Final merge order**: `if (old != NULL) { if (col->k < best) {...} else {...} } else {store}` (retail `beqz v0`).
3. **Loop weights (the colouring decline).** uopt weighs a block x10 when it is in the loop region. A `return`
   inside the e-loop (type 3) leaves its blocks at weight 1 (952/&player_array/-1 tot 21 < toll 28.25, split), but
   a `goto end` to a label at the function end puts them at 10 *and* every post-loop block that flows into that
   label (col tot 18 -> 180 when all post-loop code flows there). Retail colours col, 952, &player_array but not
   bestk/poly: type 3 `goto end;`, and the two recursion paths end in explicit `return;` (so only the old==NULL
   store path and the bestk test flow into `end:`). 96 -> 62 norm rows; s2 col, s3 car, s6 12, s7 PA, s8 952 as retail.
4. **Frame 648 -> 608**: retail's named locals end at `old` (188); the draft's trailing `c2, den, a, b, i, j, pc,
   p0, p1` do not exist. Corner/conv loops reuse `k`; `D_80152818` indexed directly; `p0/p1` are the CSE'd
   `pts[ka]`/`pts[kb]`. The k-loop cross product that is reused after `px` moves is the named `c1`.
5. `func_800B61A8` must be declared with its real `unsigned char arg3` (the prefix's `s32` made the inlined
   copy store/reload a3 through the stack).
6. `den` in both clips is the named `u` (`u = a - b; if (u != 0.0f) u = (...)/u;`): IDO folds the compare to
   `a != b` but keeps the difference before the branch (retail's PRE-looking `sub.s` before `bc1t`).
7. `k = (-da / db < 1.0f) ? ka : kb;` (retail's `b`-over-`move` shape), `mtx[5] * car->mat[5]` (operand order).
8. `dy = db - da` with `da = pts[ka][1]; db = pts[kb][1];` at the loop top (first test `da * db < 0.0f`):
   dy is copy-propagated, the difference becomes a spilled expression temp (retail's `swc1 $f24,148(sp)`),
   and 244 is never stored (as in retail). 51 norm rows, but frame 600 (temp area 8 short) — open.
9. `n = poly->cnt; n &= 0xF;` (two statements): n's totalsave 122 -> 124 lifts it over `&pv` (5.64 vs 5.56),
   so n takes s0 and &pv s1 as in retail (the retail `lhu s0; andi t6,s0; move s0,t6` shape is unchanged).
10. `pts[k + 4][i] = pts[k][i] + d[i] * dz;` (operand order of the add.s; retail `add.s f22,f30,f22`).

### Where it stands (best.c = cps/body_f2.c): 48 norm rows, frame 600/608
Colouring now as retail for s0 n, s1 &pv / &pts[4], s2 col, s3 car, s4 e, s5 edge IV, s6 12, s7 &D_80152818,
s8 952. Forcing the -1 web to t5 (`tools/trace/force.sh f2 camera_play_script "p1:w610=c12" --norm`) gives
30 norm rows and reproduces the whole type-4/7/3 dispatch exactly (`bne t5,..`, `sll a2,t5,24` for the
inlined `effect_cleanup` third argument, `li t5,-1` after each call). Residual, by cause:
- **-1 web (t5)**: totalsave 30.0 == caller cost 30.0 (3 compares x weight 10; three call crossings), so it
  splits (`>` needed). The effect_cleanup CVT use is a "ctx=0 remat" occurrence and does not count (pretrace
  `const -1 ctx=0 remat blocks ...55`). Inert: literal spellings `(s8)-1`, `0xFF`, `~0`, `-1L`; unprototyped
  `effect_cleanup();`; `bestk > -1`/`!= -1`. Not tried (shaping, would need disclosure): a compiled-out
  `if (car->f6C4 == -1) {}` at loop weight. Best next hypothesis: a 4th -1 compare or a call-crossing difference
  around the type-7/A61B0 blocks (read `f_cupcosts`: it charges weight(block) per live block adjacent to a
  regs-affecting call block, via pred/succ lists).
- **Frame 600 vs 608**: the uopt temp area has one coloured expression web fewer than retail (`spill.sh`:
  d2 area 484 vs c4 492). Candidate: retail keeps `idx[0]`'s load in a2 (`lhu a2,568(sp)` right after
  func_800AD5D0) as a coloured web; ours uses a ring temp. Writing `DECODE(org, &D_8015201C[idx[0]])` did not
  give it (57 rows). c4 (dy named, frame 608) is the frame-correct but 55-row alternative.
- **FP colouring**: the 0.0f constant is a coloured FP web in ours (`mtc1 zero` once, reused) but
  rematerialised at every use in retail; px/pz/k-loop temporaries differ (retail px f2, pz f22, pv[k] f26/f24).
  The k-loop `u = ex*dz - ez*dx; if (u != 0.0f)` form (which gives the second clip exactly) blows the k-loop up
  (147 rows, e1/e2), so the k-loop den is still an inline expression there.
- `old` should be coloured s0 (retail `move s0,v0`, spilled to 188 around func_800C36A0); `col->poly = poly`
  load placement follows from it. The corner-loop IV takes a1 instead of t0 (+`move a1,t0`).

### Method notes (what generalises)
- **uopt block weights come from loop *intervals*, and `return` vs `goto` changes them.** A `return` inside a
  loop leaves its blocks at weight 1; a `goto end` to a label at the end of the function makes the target and
  every block that flows into it weight 10 (col's totalsave 18 -> 180). Use `return`/`goto` placement, not
  dead reads, when a retail constant/parameter web is coloured but ours is "tot below toll".
- **A locked wrapper can be inlined into a caller and explain "extra arguments".** A register that looks like a
  4th argument (`a3` set before a 3-arg call) was the inlined wrapper's parameter variable; the `sll/sra` of a
  constant register was the wrapper's `(s8)` parameter conversion. Grep matched siblings (entity_update) for
  the same call pattern before declaring a prototype.
- **`x = a - b; if (x != 0.0f) x = c / x;`** gives retail's "difference computed before the branch, compare
  folded to `a != b`" shape; a named `dy = db - da` whose operands are plain locals is copy-propagated and its
  value spills to a *temp* slot (not dy's home), which is how retail's 244 slot is never stored.
- A function-level named local that is completely copy-propagated still keeps its slot (frame unchanged), so
  "slot never stored" does not mean "variable absent".

Permission denials: none. (One mis-typed toolkit install created nothing outside the lane scratch.)
