# Wave 20, lane w20e: game mid near-misses

Targets: engine_sound_sync, func_8010FBE0, func_800D6914, entity_process_main.
Builder scratch: watchman2:~/rush2049/scratch/frontier/w20e. Nothing committed, spliced or pushed. No matches.
No permission denials. Variants are in `<fn>/v/` (ignored).
Unit scores: `TAG=w20e cloud/work/frontier/tools/trace/us.sh NAME FILE` (rows words/ops/norm, frame want/got)
and `python3 -m tools.conveyor.pipeline.blob_unit --tag w20e score ...`.

| Function | Bytes | State | Flags | Scorer output (whole-program unit) |
|---|---:|---|---|---|
| engine_sound_sync | 1236 | not matched; 107 -> 14 norm rows, frame and every sp home exact | -O3 | `FAIL engine_sound_sync: 270 of 309 words differ; compiled body is 308 words, target 309` / `FAIL audio_effect_setup: 6 of 46 words differ` / `locked bodies that differ in this unit: 0` (best.c + aes_ctx.c, `--internal audio_effect_setup --neighbours`) |
| func_8010FBE0 | 128 | not matched; 6 rows (unchanged); mechanism narrowed | -O3 | `FAIL func_8010FBE0: 30 of 32 words differ \| words 6 ops 6 norm 6 \| frame 24/24` (w16e best.c) |
| func_800D6914 | 1260 | provisional lane; 27 -> 3 norm rows | -O3 | `FAIL func_800D6914: 46 of 315 words differ` / `EQUAL func_800D690C: 2 words (internal, c_best.c)` / `locked bodies that differ in this unit: 0` (us.sh: `words 57 ops 3 norm 3 \| frame 144/144`) |
| entity_process_main | 1424 | 2 words (unchanged), 1-hour cap | -O3 | `FAIL entity_process_main: 2 of 356 words differ \| words 4 ops 0 norm 0 \| frame 280/280` |

## engine_sound_sync (1236 B): 107 -> 14 norm rows, frame exact (not matched; stop rule)

Start: w17h best (= lean packet) `words 277 ops 107 norm 107 | frame 376/368`. Best: `engine_sound_sync/best.c`
(header discloses everything) with context `engine_sound_sync/aes_ctx.c`.
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w20e score engine_sound_sync audio_effect_setup \
  --with cloud/work/frontier/w20e/engine_sound_sync/aes_ctx.c --with cloud/work/frontier/w20e/engine_sound_sync/best.c \
  --internal audio_effect_setup --neighbours
  FAIL engine_sound_sync: 270 of 309 words differ; compiled body is 308 words, target 309
  FAIL audio_effect_setup: 6 of 46 words differ
  locked bodies that differ in this unit: 0
```
(udiff --norm: 14 rows; without aes_ctx.c the same source is `words 153 ops 15 norm 15 | frame 376/376`.)

Levers, in the order found (each measured in the unit):
1. **Lock/unlock are the locked inlined helpers func_80091AF0/func_80091AF8** (w17h lesson; umerge.log shows both
   inlined): frame 368 -> 384, then right once the rest was right.
2. **No transpose.** Retail computes arcade `BodyVector` (each basis row dotted with the direction); it never
   stores transposed values, ours (TransposeUV + WorldVector) did. ops 107 -> 70, frame 376.
3. **Indexed first listener loop** (`panValues[i] = ...; i++`, the same `i` as the weighting loop) instead of three
   walking pointers: uopt strength-reduces it to retail's per-source `addiu s2,sp,232` initialisation and hoists
   `&direction`. ops 70 -> 65.
4. **The four call arguments live in memory**: retail stores gain/pan/depth/parameter at 360/356/352/348 and
   reloads them into $f22..$f28 right before entity_transform_calc (IPA register parameters). A local array
   `f32 out[4]` (param, depth, pan, gain ascending) does exactly that, and frees $f26 for the hoisted 1.0f as in
   retail. ops 65 -> 15. (`volatile` scalars give 17; the array is the better and the more natural form.)
5. **audio_effect_setup takes `effect` in s0** (IPA register parameter, `frontier show`: register_params s0,
   unsaved s1/s2). With its provisional source as context (`aes_ctx.c`, from
   `cloud/work/ipa-groups/codex_effect_typed_b132/group.c`, `--internal audio_effect_setup`) the call becomes
   retail's `jal; nop` (15 -> 14). So engine_sound_sync can only land in a group with audio_effect_setup
   (itself 6/46, frozen nonmatch).
6. **Frame layout**: with the projection written inline (an inlined static adds a 16 B area) plus three named
   slots (2 between `weight` and `direction`, 1 between `basis` and `panValues`) every sp home equals retail
   (348..360, 304, 292, 256, 232/212/192, next at 368). The three slots are unused placeholders `pad1..pad3` in
   best.c, disclosed in its header (retail 376, without them 360, 3 slots); their identity is unknown.

Residual (traced, not closed): retail colours `&D_80146188` (s5, used as `lw s0,8(s5)` / `lw a0,4(s5)`), the
integer constant 1 (s8, **never used** in the final code) and something in s7 (saved, never used), and leaves
the three array bases uncoloured (re-materialised per source in a1..a3). Ours colours the array bases s5/s6/s8
(w241-243, tot 100, best 13). A force (`p1:w241=s,...`) makes their pieces a1-like, but the list base web
(ours `ldaS(+4)`, tot 20) cannot take an s-register then. Retail's two dead s-register webs (s7, constant 1 in
s8) are what pushes the array webs out of the callee-saved registers; their source is unknown (best next
hypothesis: an index/counter variable of loop 1 whose uses were all strength-reduced away, plus an extra use
of the list base). Pointer local `list = &D_80146188`, a separate loop-1 index `n`: both worse (v13, v14, v21, v22).

## func_8010FBE0 (128 B): 6 rows, mechanism narrowed (not matched)

Re-confirmed w15d/w16e: the definition `OSScTask D_80155238;` + all stores through it gives retail exactly when
the address web is forced split (w15d oracle). New measurements (`func_8010FBE0/v/`):
- `-O1` standalone (`score.py fn s7.c --flags "-g0 -O1 ..."`) reproduces retail's three `lui at` exactly
  (next alone, msgQ/msg sharing one, flags alone): as1 shares `lui at` only between *consecutive* macro stores to
  the same defined symbol (the `la t6` between next and msgQ breaks it). So retail = one defined symbol, plain
  stores, address never in a register.
- uopt canonicalises every spelling to `istr@off(ldaS(sym+0))`, i.e. one web: `*&D.f`, `(&D)[0].f`,
  `((u32 *)&D)[k]`, `(char *)&D + 80`, pointer locals (`q = &D.msgQ`), array-typed definition `[1]`
  (constant or propagated index), `u32 D[22]`, file-static definition: all 14-15 words. Partial volatile casts on
  the msgQ/msg stores: 12 words (`f2`), still coloured.
- Defined *static* scalars for msgQ/msg (`k1`): two `lui at` (as1 does not share across two .bss statics either).
- Conclusion: with two uses of one lda in the store block uopt always finds net 1 > cost 0. The retail form must
  reach the stores without an lda web (e.g. from loop unrolling, as in func_800B2BDC where unrolled copies became
  `str varS(sym+off)`), or the stores are not in one uopt block. Not found; stopped.

## func_800D6914 (1260 B): provisional, 27 -> 3 norm rows

Provisional lane (stand-in callers `zz_caller`/`zz_caller2`; real caller func_800D7634 unmatched; retail writes
s0-s8 unsaved). Score with `--internal func_800D6914` (otherwise the function saves every callee register):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w20e score func_800D6914 func_800D690C \
  --with cloud/work/frontier/w20e/func_800D6914/best.c --internal func_800D6914 --internal func_800D690C --neighbours
  FAIL func_800D6914: 46 of 315 words differ; +0x6c: .rodata+0x578: retail words encode 0x433402b4, outside the image
       umerge inlined into it: func_800D690C
  EQUAL func_800D690C: 2 words (internal, c_best.c)
  locked bodies that differ in this unit: 0
```
- **Lever: the caller-less locked stub func_800D690C (right before the function) is an inlined getter**, exactly
  the sibling func_800B59F0 / func_800B59E8 idiom: `s32 func_800D690C(void) { s32 v; v = D_80110638; return v; }`
  used for `if (i == func_800D690C())`. w17h best: `words 82 ops 27 norm 27 | frame 144/96`; with the getter
  (`best_nofill.c`): `words 59 ops 3 norm 3 | frame 144/104`. The getter fixed retail's s7 = &D_801105B8 and
  s8 = 1 (the w10e residual).
- The frame still needs 10 more named words above `old` (w17h); `best.c` uses `f32 m[3][3]; f32 k;` (unused,
  disclosed: retail 144, without 104, 10 slots): `words 57 ops 3 norm 3 | frame 144/144`. A getter of
  D_801105B8 instead (4 sites, 32 B of inline areas) gives the same norm 3 but puts the bytes below the named
  locals, where retail has none (old/sel_y must sit at 76/68), so it is the selected-index getter.
- Left: pi vs 180 constant webs tie at save 3.0 (tot 30/10 vs 42/14) and go by web number ($f22/$f24 swapped);
  the getter's value is a v1 web, which shifts the t6..t9 ring by one; `tex` at 66 vs 62.
- Integration: none (provisional). When func_800D7634 matches, put both in one group; func_800D690C becomes a
  member (locked stub given a body, non-static, forced_internal + prefer_definition like func_800B59E8).

## entity_process_main (1424 B): 2 words, 1-hour cap reached

Only forms not in w11a/w12b/w14o/w15c/w16e were tried (20 files, `entity_process_main/v/f01..f20.c`), all on the
clear arm `poly->flags = ~(1 << i) & poly->flags;`:
- no-code identities on either operand or the NOT (`^ 0`, `* 1`, `| 0`, `+ 0`, `<< 0`, `- 0`, `& ~0`, `0 |`):
  all 2 words (folded before ugen);
- `((1 << i) ^ ~0)`: 39 words; `~(u8)(...)`: 38; `(u16)~(u32)(...)`: 17; `*(u16 *)&poly->flags`: 18;
- `~(1 << (u32)i)`, `~(s32)(...)`, `& (u32)poly->flags`: 2, unchanged.
Conclusion stands: ugen's postfix order cannot give shl, load, not from a single tree; no new hypothesis.

## What generalises
1. **An IPA register-parameter callee forces memory homes for argument variables only if they are not register
   candidates**: retail's stores/reloads around an `$f22..$f28` call were reproduced by an array local holding the
   arguments, not by scalars (which uopt colours). Look for `swc1 X(sp)` ... `lwc1 $f2x,X(sp)` before such calls.
2. **Check every callee's convention in `frontier show`**: a `jal` with no argument set-up (`jal; nop`) means a
   register parameter in an s-register (audio_effect_setup: s0); the caller is then group-only.
3. **Transpose-then-WorldVector and BodyVector are not interchangeable**: uopt keeps the transposed stores to a
   local array; if retail has none, the source multiplied by the rows directly.
4. **The sibling's getter idiom transfers**: a caller-less stub right before a function whose matched sibling has
   an inlined getter is the same getter (func_800D690C/func_800B59E8); it moved 24 norm rows.
5. **as1 shares `lui at` only between consecutive macro stores to one defined symbol** (`-O1` shows it directly).
