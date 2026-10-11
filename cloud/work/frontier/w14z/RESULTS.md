# w14z results (Haiku lane)

Flags for every score: `-g0 -O3 -mips2 -G 0 -non_shared`. Standalone scorer on watchman2
(`tools/cloud/score.py fn`, scratch `~/rush2049/scratch/frontier/w14z`). No function is a MATCH.
No group, no cloud/matches file, no splice. Whole-program unit (`blob_unit`) was not run: no candidate
reached a standalone strict 0, so there was nothing to confirm.

| function | bytes | state | best strict | exact scorer line (best file) |
|---|---|---|---|---|
| func_800A79F4 | 240 | 4 of 60 words off, stopped (3 rounds, no gain after round 1) | 4/60 | `4/60 words differ` (func_800A79F4/best.c = b4/v0000) |
| func_800EC190 | 224 | 17 of 56 words off, stopped (rounds 3-5 flat) | 17/56 | `17/56 words differ` (func_800EC190/best.c = b3/v0029) |
| func_800A7480 | 136 | 24 of 34 words off, stopped (3 rounds, no gain) | 24/34 | `24/34 words differ` (func_800A7480/best.c = b1/v0008) |

## func_800A79F4
- Prior best (w7c `best.c`): 7/60. Re-scored it first. Its misses were the hm1/wm1 register
  assignment and the w/h store offsets.
- Round 1 (`b1`, 64 variants, `t1.c`): choice points for loop form, store order of the
  b/p4/p0/hw/sf/wh blocks. Best 4 (v0004, `hw` = `s->wm1` before `s->hm1`, `bf` = b first).
- Round 2 (`b2`, 400 variants, `t2.c`): loop form, if-test form, `s = &...` placement, `D_801613AC = i + 1`
  in place of `D_801613AC++`, `(u8)255`, store placement. Best still 4.
- Round 3 (`b3`, 4 variants): position of the hm1/wm1 pair (3 positions). Best 4.
- Round 4 (`b4`, 400 variants of 384 unique, `t4.c`): wm1/hm1 and w/h position choice points, a `while`
  loop form. Best 4 (v0000 is the best; no gain).
- wbgen climb (`g1`, 183 files): best 4. No improvement; no winner to review.
- Diagnose (`diagnose one`, on best_r2): `mixed(constant:2, register:2)`, lane `temp slot 5`, lever `none-known`.
- Residual: retail computes `w-1` into t7 and `h-1` into t6. Ours uses t6 and t7 the other way round. This is
  the temp-ring order (the t6 `lb` in the loop frees first); no source-level lever found in 3 rounds.
  Not a true register-class residual, but not forced/traced either (toolkit not installed; no
  force.sh run).

## func_800EC190
- Prior drafts (`newtargets_hi/g1`, `tiny_A30`) scored 23/56 and 50/56. Re-scored them first.
- Full draft written from the retail asm (`func_800EC190/d1.c`): 50/56. The gap is the array base
  (`D_80153E88`) loaded late and `D_801543CA`/`D_8014A118`/`D_801163F4` addressed through a shared `lui`.
- Round 1 (`b1`, 6 variants): loop form (do/for/while). All 50.
- Round 2 (`b2`, 64 variants): parameter type s8/s32, statement order. All 50.
- Round 3 (`b3`, 300 variants, `t3.c`): global-address laundering. `*(s16 *)((u32)&D_801543CA) = 6;`,
  `*(s8 *)((u32)D_8014A118) = index;`, `*(s16 *)((u32)&D_801163F4) = index;`. Best **17**
  (v0029, `func_800EC190/best.c`). This was the big win, from 50 to 17.
- Round 4 (`b4`, 384 variants, `t4.c`): loop form, laundering, `D_80150F14` placement. Best 17.
- Round 5 (`b5`, 400 variants, `t5.c`): initialisation order, index load laundering, `D_80150F14` placement.
  Best 17. Stopped (three flat rounds).
- Remaining diff in best: base address `addiu a2,a2,16008` is scheduled after `move v0/v1` (retail: before);
  `sb zero,7(a0)` vs `sra`; the `lh` of `D_801163F4` is `lui v0; lh v0,25588(v0)` in retail and
  `addiu v1` in ours; `li t1,2` placement.
- Frame residual: retail has `sw a0,0(sp)` (the `mode` home). Our best has no such store. Not fixed.

## func_800A7480
- Draft first. The w9d best (17/34) is NOT a valid shape: it calls `func_800A5560` (retail makes no call)
  and so its 17 is not evidence of the function's structure. Re-scored its native and blue priors (w9-era, -O2 header):
  both 34/34 under -O3 with these flags.
- Full draft from retail asm (`func_800A7480/d1.c`, 33/34). Structure: 72-byte record, fields
  `x,y` (s16 at 0x40, 0x42), `r,g,b,a` (u8 at 0x44-0x47). Packed RGBA5551 word:
  `px = ((r<<8)&0xf800)|((g<<3)&0x7c0)|((b>>2)&0x3e)|1`, stored as `(px<<16)|px` to `D_80124FC8`.
  Params: x,y s16; r,g,b,a u8 (`lbu` from the stack for b and a); index s32 (7th arg at 24(sp)).
  Best = `func_800A7480/b1/v0008.c` (choice options not decoded in this log; see manifest.tsv).
- Round 1 (`b1`, 32 variants): r/g type, pointer form, store order, packing position. Best **24**
  (`func_800A7480/best.c`).
- Round 2 (`b2`, 48 variants): decl/address forms (`(u32)D_8017A510 + index*72`), packing position. Best 24.
- Round 3 (`b3`, 16 variants, `t3.c`): direct global indexing `D_8017A510[index].x = ...`. Best 33 (worse).
- `-O2` on the best: 34/34 (worse than -O3).
- Residual: retail computes the base address (`lui t6; addiu t6`) before the index multiply; ours
  schedules the index first, and the `lbu t0,19(sp)` load early. The register names then cascade.

## Log of batch files
- func_800A79F4/b1 (64, t1.c), b2 (400, t2.c), b3 (4, t3.c), b4 (384, t4.c), g1 (183, wbgen on best)
- func_800EC190/b1 (6), b2 (64), b3 (300, t3.c), b4 (384, t4.c), b5 (400, t5.c)
- func_800A7480/b1 (32), b2 (48), b3 (16, t3.c)
Helper: `sc.sh LOCAL.c FN` (scores one file on the builder; `TAILN=` sets the tail length).

## Notes for integration
- Nothing to integrate: no match.
- Two tool issues seen: (1) `wbgen.py` takes its output path relative to the repo root, so `<lane>/...`
  wrote into a stray `w14z/` at the repo root (I moved it back and deleted the stray dir). Use
  `cloud/work/frontier/<lane>/...` paths. (2) `vgen.py` choice points cannot nest; a nested
  `/*@{x*/` inside another choice point's option silently produces raw markers in the variant.

## Continuation (coordinator instruction: new choice-point kinds, alternate targets)

### func_800A79F4 (still 4/60; best file func_800A79F4/best.c)
- Round 5 (`b5`, 192 variants, `t5.c`): `w + -1`, `-1 + w`, `(s16)(w - 1)` (same for h), `D_801613AC += 1`,
  `D_801613AC = D_801613AC + 1`, `D_801613AC > D_8013C234` form, `i > 199`. Best **4**.
- Round 6 (`b6`, 32 variants, `t6.c`): evaluation-order / local forms: `s32 wv, hv` declared and assigned
  before the stores (the coordinator's "assign to a local first"), store order of hm1/wm1 and w/h. Best **4**.
- wbgen climb (`g1`, rerun with an absolute output path, 183 files): best **4**. Control v0000 = 4.
- Not reached: nested-expression and comma forms were not written separately; the local-variable round
  covers the "computed first" axis. Still 4/60, stopped (3 rounds with no strict improvement).

### func_800EC190 (15/56 now; best file func_800EC190/best.c, disclosed header)
- Round 9 (`b9`, 32 variants, `t9.c`): store order of `value`/`ordinal`/`flag` in the else branch, `flag = 176`
  before/after, `i = 0; j = 0;` order, `D_80150F14` placement. **Improved 17 -> 15** (v0020 = `best_r9.c`).
- Round 10 (`b10`, 400 variants, `t10.c`): l2/l3/l4 launder choices, base-pointer location. Best 15.
- Round 11 (`b11`, 400 variants, `t11.c`): `Entry *bp` declared at top (not yet used in the loop; a
  frame-only choice). Best 15.
- Round 12 (`b12`, 384 variants, `t12.c`): `s16` vs `s32` for i/j/index, `i = i + 1`, `(u8)j` store form.
  Best 17. No improvement over 15: three rounds (10, 11, 12) with no strict improvement.
- Shaping pricing on the best (laundering per global, others plain):
  - `D_801543CA` laundered: 15/56; plain: **51/56** (kept).
  - `D_8014A118` laundered: 21/56 (**+6**, dropped).
  - `D_801163F4` load laundered: 15/56 (neutral, dropped); store laundered: 15/56 (neutral, dropped).
- Missing `sw a0,0(sp)` (mode home): three address-taken forms tried (`s8 *pm = &mode;` unused,
  `D_8014978C = *pm;`, and combinations): 50 to 60 words, with frame size +1 to +4. Not kept. The
  store is not reproduced by any source form tried. A call-based source would be a fake caller (not allowed).
- Residual: array base `addiu a2` scheduled after the zeroing moves; `lui v0 / lh v0` for D_801163F4 read.
