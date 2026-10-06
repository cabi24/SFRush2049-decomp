# Wave 11 — lane w11c (camera unit, continuing w10d)

Assignment: camera_free_look, camera_process_input, camera_look_at_point, camera_update. All flags
`-g0 -O3 -mips2 -G 0 -non_shared`. Builder scratch `watchman2:~/rush2049/scratch/frontier/w11c`, which has its own
traced uopt in `uopt/`.

All four functions live in one cluster file, `cu/best.c`. The same text is
`groups/camera_aspect_ratio/group.c`. The `<fn>/best.c` files hold each function's text on its own, for reading.

| Function | Bytes | State | Flags | Exact scorer output |
|---|---:|---|---|---|
| `camera_process_input` | 1,020 | **strict MATCH (group)**. Frame needs disclosed unused locals (see below). Owner decides whether that is acceptable | -O3 | `score.py group cand/camera_aspect_ratio` → `camera_process_input:  MATCH` / `own .data verified at 0x8011750C (4 bytes; ...)` / `own .rodata verified at 0x80123E90..0x80123E94`; unit: `EQUAL camera_process_input: 255 words (kept, c_group.c)` |
| `func_800C15FC` (stub, 8 B) | 8 | **strict MATCH (group)**. It is the deleted slot setter, defined for real | -O3 | `func_800C15FC:  MATCH`; unit: `EQUAL func_800C15FC: 2 words (internal, c_group.c)` |
| `camera_free_look` | 436 | **code identical, provisional**: its only caller, camera_update, is unmatched (was 24 words) | -O3 | unit: `EQUAL camera_free_look: 109 words (internal, c_group.c)`; group context: `camera_free_look:  MATCH` |
| `camera_look_at_point` | 796 | near-miss, **38 words** (was 160), 2 opcode rows | -O3 | `FAIL camera_look_at_point: 38 of 199 words differ` |
| `camera_update` | 2,876 | near-miss, **244 words** (was 255), 13 opcode rows, **no pad arrays** (the frame is now explained) | -O3 | `FAIL camera_update: 244 of 719 words differ; compiled body is 718 words, target 719` |
| `camera_track_spline` | 860 | still EQUAL, still provisional (its caller camera_update is unmatched) | -O3 | `EQUAL camera_track_spline: 215 words (internal, c_group.c)` |

Commands, from the repo root on the Pi:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w11c score camera_process_input func_800C15FC camera_update \
    camera_free_look camera_look_at_point camera_track_spline camera_aspect_ratio camera_fov_control \
    camera_build_view_matrix --with cloud/work/frontier/w11c/groups/camera_aspect_ratio/group.c \
    --internal func_800C15FC --neighbours
  EQUAL camera_process_input: 255 words (kept, c_group.c)
  EQUAL func_800C15FC: 2 words (internal, c_group.c)
  FAIL camera_update: 244 of 719 words differ; compiled body is 718 words, target 719
  EQUAL camera_free_look: 109 words (internal, c_group.c)
  FAIL camera_look_at_point: 38 of 199 words differ
  EQUAL camera_track_spline: 215 words (internal, c_group.c)
  EQUAL camera_aspect_ratio: 50 words (internal, c_group.c)
  EQUAL camera_fov_control: 60 words (internal, c_group.c)
  EQUAL camera_build_view_matrix: 153 words (internal, c_group.c)
  locked bodies that differ in this unit: 0
  blob_unit score: 7/9 equal

# builder scratch (copy of groups/camera_aspect_ratio):
python3 tools/cloud/score.py group cand/camera_aspect_ratio
  camera_aspect_ratio:  MATCH
  camera_fov_control:  MATCH
  camera_build_view_matrix:  MATCH
  func_800C15FC:  MATCH
  camera_process_input:  MATCH
    own .data verified at 0x8011750C (4 bytes; .data is laid out per translation unit)
    own .rodata verified at 0x80123E90..0x80123E94
  Context: camera_free_look MATCH, camera_look_at_point 38/199, camera_update 245/719, camera_track_spline MATCH
```
The unit result is the same without the four `__standin_*` callers: the unit keeps the callees out of line by
itself. The stand-ins stay in the file only for the standalone `score.py group` compile, as in the locked group.

## Integration notes

- **Group `groups/camera_aspect_ratio/` supersedes the locked group of the same name.** It has the same name and
  `members` keeps all three locked members (camera_aspect_ratio, camera_fov_control, camera_build_view_matrix, all
  with unchanged text). It adds `func_800C15FC` and `camera_process_input`. `"claims": ["func_800C15FC",
  "camera_process_input"]`. The context is camera_free_look, camera_look_at_point, camera_update and
  camera_track_spline (near-miss and provisional bodies, not claimed).
- `func_800C15FC` is currently locked as an empty `-O2` single (`src/blob/func_800C15FC.c`). Run
  `blob_splice revert func_800C15FC` first. It also needs a `prefer_definition` entry in
  `src/blob/unit_overrides.json`: `func_800C15FC` → `src/blob/groups/camera_aspect_ratio/group.c`. Reason: the
  locked stub is what remains of the inlined slot setter, and its real definition gives the callers their frame
  slots and the v0/v1 pair. It must stay internal (not in `keep`); the group spec already leaves it out.
- Own data, both verified by the scorer:
  - the `col` initialiser in anonymous .data at 0x8011750C (`FF 00 00 FF`);
  - the literal `0.0425f` in .rodata at 0x80123E90.

  camera_free_look's `22500.0f` (0x80123E84) and camera_update's `0.0425f` (0x80123E8C) are now natural literals.
  They replace the old `extern D_80123E8x`.
- **camera_process_input's frame relies on unused locals.** This is disclosed in the source header:
  - `s32 unused[11]`, the top 44 bytes (sp+204..247);
  - two unused scalars `unused1` and `unused2`, the two slots between the function-level locals and the mode-2
    block.

  They are the exact frame residual. The allocation model below says only function-level declarations can sit
  there. Every natural split I tried changed the code: a separate second loop counter, an `f3C` temporary, an
  `id`/`fl` temporary (that one flips the compare operand order), or a `CamCtl *c` in the mode-2 block. The
  unused `col[4]` red colour is direct evidence of compiled-out debug code, which is the likely owner of these
  locals. If the owner does not accept unused locals in a final match, hold this group. Nothing else in it
  depends on them.

## Camera unit structure recovered this wave

- **Retail has exactly two deleted helpers in the camera TU**: the stubs func_800BF778 (after look_at_point) and
  func_800C15FC (between camera_update and camera_process_input). I measured that every inlined static leaves a
  `jr ra; nop` stub at -O3, `static __inline` included. So at most two helpers exist. This rules out the earlier
  3-helper designs (w4d's mode-2 helper plus `cam_slot_set` plus a scene-start helper). Those also produced
  score.py "1 extra words" failures from the static stubs that followed members.
- **func_800C15FC is the slot setter** `D_8012E714[slot * 0x44] = v`, i.e. D_8012E700 records, field +0x14. It is
  inlined at camera_update's four flag sites and at camera_process_input's one. It has 2 parameters (retail's
  v0/v1 pair) and 2 locals (`base`, `p`). Four slots per call site are exactly what **both** callers' frames need:
  - process_input: 248;
  - camera_update: 248 with no pad. Its 9 words between `cam` (244) and `moved` (207) are function-level scalars
    declared there, and 2 more scalars come from splitting `v` into `v`/`id`/`cnt` (same code).

  The fifth store (the s50 update, `D_8012E714[cam->slot] = (&D_801427C0)[na]`) is a direct store. A setter
  there makes the frame 8 bytes too big.
- **The mode-2 block of camera_process_input is inline code with block-scoped locals.** `k` sits in its own
  loop-body block, so it is allocated after `mat`. It is not the stub.
- Hypothesis, not tested in the unit: func_800BF778 is `camera_track_entry` (the static `__inline` helper of the
  locked members), defined after camera_look_at_point.

## camera_free_look — 24 → 0 words (provisional)

Traced uopt, proc 585:
1. A `CamKey *keys = sc->keys;` local, as in camera_track_spline, used for `keys[next].rot`. It makes the next
   index (v1) colour before `k`, so `k` falls to t0, idx gets a0, keys t1 and the stride t2. All integer rows
   close, and retail's preloaded `a1 = k->rot` follows by itself.
2. `t = k->dur; t -= ctl->t;` instead of `t = k->dur - ctl->t;`. The local def and use raise t's savings from
   3/3 to 5/3, above dur's 3/2, so t takes f2 and dur f14 (retail). The generated code is unchanged.
3. `22500.0f` is a natural literal. It replaces `D_80123E84`.

## camera_look_at_point — 160 → 38 words

1. **The degenerate `bgez v1,+8`** comes from `tt = 0; if (b < 0) { tt = 1; } if (tt) {}`: a flag set under a
   condition and only tested in an empty if. The dead store is removed after CFG cleanup, so the branch stays.
   This is the same family as func_80096288's empty nested ifs. A plain empty `if`, or a dead store whose only
   reader is gone, is removed entirely.
2. No `CamTbl *t` local in the else part (index `D_80117530[cam->tbl]` each time). The table expression then
   gets v1, `id` gets v0 (it merges with the result), `-1` gets a0 and `s28` gets t0, all as in retail.

Residual:
- f/r colour: retail f→f0, r→f2. The copy-propagated r expression has save 2.5 and beats f's 2.0. Nine
  source forms tried, none moved it. `force p1:w60=c24,p1:w58=c25` fixes it.
- the t6–t9 ring is one step behind from +0x208, and the FP temp ring from +0x174: `lui at` placement and
  `mtc1 at,$f8` vs `$f4`.

Neither residual moved with expression-order variants.

## camera_update — 255 → 244 words, frame explained (no padA)

- `v = ++node->s04;` replaces `v = node->s04 + 1; node->s04 = v;`. Retail loads into a temp (t9) and then adds
  (11 words).
- Frame: see above (4-slot setter, 9 scalars declared between `cam` and `moved`, `id`/`cnt` split from `v`).
- Residual (13 opcode rows):
  - The s50 site: retail reloads `cam` (`lw t8,244(sp)`) for `cam->slot`. Ours reuses t6 from the `cam->s58`
    load, so retail's `andi t6` clobbers t6 and ours takes t8. From there the t6–t9 ring is one step off for
    the rest of the function, about 120 words (block_99, the dv/sv loops). Tried:
    - a setter at the site (2- and 4-slot);
    - a `Camera *` parameter;
    - `slot` read inside the helper;
    - a nested `cam_slot_put(v, cam, sc)`.

    The cached t6 survives all of them, and a `Camera *` setter breaks the v0/v1 pair at the other sites.
  - The build_view_matrix calls: retail evaluates `lw s3,244(sp)` (cam) before `move a0,zero`. A variable
    first argument (`idx`/`i`) gives the order but costs 230 words.
  - The loop-top FP load order, and the `div.s` position in the sv loop.

## Tools (this directory)

- `tools/fl.sh FN BODY.c [BASE]`: splices one function body into a cluster file and unit-scores it (env `EXTRA`
  for more blob_unit args).
- `tools/flt.sh FN BODY.c PROC`: the same, plus the traced colour summary of PROC.
- `tools/fsum.sh LABEL PROC SPEC`: traced colour summary with `CDX_FORCE`; shows `DECLINED(reg)`.
- `tools/fnset.py`: replaces a whole function definition.
- Everything else (ctrace/force/pdiff/udiff/run/vrun) is w10d's, retargeted to w11c.
- `tools/build_uopt.sh` builds the traced uopt into the w11c scratch.

uopt proc ordinals in this unit: free_look 585, look_at_point 580, camera_update 594.

Permission denials: none.

## What generalises

1. **Frame-slot allocation model (measured with offset probes, IDO 5.3 -O3, whole-program):**
   - Every named function-level local takes a slot, used or not, in declaration order from the top of the frame.
   - Block-scoped locals come after all function-level ones; a nested block's after its parent's.
   - Each inlined call site's parameters and locals come after all of the caller's, in call order, as an
     8-aligned block.
   - The frame is rounded to 8.
   - Consequence: offsets relative to the frame top are fixed by the declarations; offsets from sp also depend
     on the inline totals. A residual above the first known local can only be function-level declarations. A
     uniform shift of every sp offset means slots are missing at the bottom (inline sites).
2. **Inlined statics leave stubs.** Every deleted static, `static __inline` included, leaves `jr ra; nop`. Count
   the TU's caller-less stubs: that is an upper bound on its deleted helpers. Use it to reject designs with too
   many helpers. In `score.py group`, a static stub right after a member shows as "N extra words".
3. **Raise a web's priority without changing code**: `t = a; t -= b;` instead of `t = a - b;`. The local def/use
   adds savings and fixes an FP colour swap between two tied-looking variables.
4. **A word copied from `.data` to the stack at entry and never read is an initialised local aggregate.** The
   image word gives the initialiser (`FF0000FF` → `u8 col[4] = {255, 0, 0, 255}`); the scorer verifies it as own
   .data.
5. **Degenerate conditional branch** (`bxx r, +8`): use a flag assigned under the condition and only tested in an
   empty if.
6. **Splitting a multi-purpose variable** into purpose-named ones (camera_update `v` → `v`/`id`/`cnt`) can
   supply frame slots with byte-identical code. Try it before declaring unused scalars.
7. When two functions share an inlined helper, solve their frames together. The helper's slot count is one
   unknown that both frames constrain.
