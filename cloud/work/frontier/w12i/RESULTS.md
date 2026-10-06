# Wave 12, lane w12i: func_800E05F0 (mode-select keystone)

Scores come from `python3 -m tools.conveyor.pipeline.blob_unit --tag w12i score ... --neighbours`, run on the Pi on
2026-10-06 (branch `wave11`, `ea15236c`). Aligned rows come from `cloud/work/frontier/tools/trace/udiff.py` (TAG=w12i).

- Builder scratch: `watchman2:~/rush2049/scratch/frontier/w12i`. It was copied from `base`, then src/blob, include,
  tools/cloud, asm/us/blob and `blob_matched.lock.json` were synced, and the toolkit was installed with `--reuse wtk`.
  A traced uopt (`bin/uopt_al`, see Tools) was built there.
- There were no permission denials.
- Nothing was committed, spliced or pushed. Nothing was written outside this lane directory.

**func_800E05F0 is still not EQUAL, so the cluster cannot land.** There is no landing group: `groups/` is not
delivered. Every residual except one is closed or explained. The one left is a single ugen `.alias` directive (lanes
b and c below). The mechanism is traced to the uopt routine that emits it, but I found no source shape that moves it.

| Function | Bytes | State | Flags | Scorer output (`sh cloud/work/frontier/w12i/comp/run.sh`) |
|---|---:|---|---|---|
| `func_800E05F0` | 1328 | near-miss: **30 word rows, 14 ops rows** (w12c: 43 / 12, 331 words). Frame and every stack home now equal retail | `-O3` unit | `FAIL func_800E05F0: 143 of 332 words differ; compiled body is 330 words, target 332; +0x408: .rodata+0x11c: retail words encode 0x00000000, outside the image` |
| `func_800E0048` (stub) | 8 | EQUAL. Defined as the camera wrapper (hypothesis, see below) | `-O3` unit | `EQUAL func_800E0048: 2 words (internal, c_mode.c)` |
| `func_800E0050` | 1440 | EQUAL. Provisional through E05F0 | `-O3` unit | `EQUAL func_800E0050: 360 words (internal, c_mode.c)` |
| `mode_select_handler` | 2976 | EQUAL. Provisional | `-O3` unit | `EQUAL mode_select_handler: 744 words (internal, c_msh.c)` |
| `func_800DEF60` | 8 | EQUAL | `-O3` unit | `EQUAL func_800DEF60: 2 words (internal, c_msh.c)` |
| `func_800D5E64` | 584 | EQUAL. Provisional | `-O3` unit | `EQUAL func_800D5E64: 146 words (kept, c_d5e64.c)` |
| `func_800DFBA0` | 1192 | EQUAL. Provisional | `-O3` unit | `EQUAL func_800DFBA0: 298 words (internal, c_mode.c)` |
| `best_times_display` | 224 | EQUAL. Provisional | `-O3` unit | `EQUAL best_times_display: 56 words (internal, c_d5e64.c)` |
| `func_800DED78` | 488 | EQUAL. Provisional | `-O3` unit | `EQUAL func_800DED78: 122 words (internal, c_ded78.c)` |
| `mode_select_input` | 152 | EQUAL. Provisional | `-O3` unit | `EQUAL mode_select_input: 38 words (internal, c_mode.c)` |

The same run prints `locked bodies that differ in this unit: 0` and `blob_unit score: 9/10 equal`.

Aligned diff of E05F0:
- `want 332 words, got 330; differing rows 30 (words); frame 224/224; unverified 4 unresolved 0`
- `differing rows 14 (ops)`

**Listing oracle (as1, `asm.sh`).** Take the current ugen listing and move the single `.alias $8,$sp` from after
`jal player_conditional_call` to just after the camera path's `b $790`. Then:
`want 332 words, got 332; differing rows 8 (words); frame 224/224; unverified 4 unresolved 0`.
- 4 of those 8 rows are the unverified own-rodata words: 0.6f at 0x8012438C and 0.1f at 0x80124390.
- The other 3 rows are loop-preheader order (lane a).
- Also moving `li $5,4` after `move $4,$19` and `li.s $f14,0.0` after `move $3,$0` gives
  `differing rows 4 (words)`, which is rodata only.

## Files

| Path | Contents |
|---|---|
| `comp/run.sh` | The replay: the w12c component plus `func_800E0048`, with no stand-ins. Set `TAG`. |
| `comp/mode.c` | The best file: msi, DFBA0, **func_800E0048**, E0050, E05F0. Its headers describe the shaping. |
| `comp/{d5e64,msh,ded78}.c` | Unchanged copies of w12c's component files. |
| `func_800E05F0/best.c` | Copy of `comp/mode.c`. |
| `var/head*.c`, `var/t_*.c` | Variant sources. `tools/tv.sh TAIL.c` builds `mode.c` as `var/head.c` + TAIL and scores the component. |
| `var/*.py` | Variant lists for `tools/mkv.py BASE_TAIL.c LIST.py` (`TOOL=al.sh` also prints the `$8` alias directives). |
| `tools/uopt_alias_patch.py` | Patch for the traced uopt described below; usage is in its docstring. |
| `tools/{rv.py,lst.sh,asmdis.sh,al.sh}` | `rv.py` (from w12c) adds `SPC=1` to print the stack-offset census and `XINT=` for extra internals. `lst.sh` and `asmdis.sh` are w12b's tools with the w12i tag. |

## What moved, in order

1. **Frame (w12c lane d): solved. All stack homes equal retail: 140, 144, 148, 168, 180, 220.**
   - Retail's homes are measured from the frame top: `level` -44 and `h` -56 (the inlined entity_hierarchy_update
     parameter).
   - A probe of inline-area layout (`var/pr.py`) gives this rule. Each inlined call's block is placed below the
     previous one, with its bottom 8-aligned. Parameters ascend from the block bottom and the callee's locals sit
     below them. This adds to the w11c/w12h rules.
   - So `h` at -56 means:
     - the own area ends at -44, so there are 11 function-level slots and `level` is the last one;
     - there is no inline block before ehu, so the stop and set paths are written out;
     - there is exactly one 12- to 16-byte inline block after ehu.
   - That block is `func_800E0048(LayerState *e, f32 *pos, f32 level, f32 style)`, a wrapper around the
     camera_clip_planes call. The image's only deleted-static stub in this file is E0048, right before E0050.
   - The wrapper must not modify `style`. The `else style=-2.0f` stays in E05F0, otherwise style loses f28 (32 ops
     rows).
   - Two of the 11 slots (`d2`, `d3`) are still unexplained. They are the exact frame residual and are disclosed.
2. **`(s32)D_801141B0` → `f32 *` third parameter of camera_clip_planes (w12c lane c, first half).**
   - With the cast, the camera_clip_planes argument is the same uopt expression as camera_target_track's (web
     `cvt.J(ldaS(1963))`, `antloc 23,44 | DELETE 44 | INSERT 41`). PRE then hoisted the `la` into the compare block.
   - As a pointer it stays at the join, as in retail.
   - func_800E0050's two calls now pass `ref` uncast and it stays EQUAL.
3. **`extern s8 D_801115CD[][13]` and `D_801115CD[original_slot][model->u8_8]`.** This fixes the t7/t8 temp-ring swap
   in the tail (5 rows). It is ugen evaluation order: `mul os,13` is now evaluated before the `lbu`.
4. **Loop bound in a variable, `for(i=0,n=4;i!=n;i++)` (w12c lane a).**
   - The tie was `&D_8011F060` against the constant 4, both at save 3.333. The candidate scan takes the lower web
     number, and the constant's web is numbered at its first occurrence in the ucode.
   - `n=4` before the body makes the 4-web precede the table web. 4 then takes a1 and the table takes a2, as in
     retail.
   - This is a shaping quirk (disclosed) and uses one of the three former dummy slots.
   - Cost: `li a1,4` now sits at the `n=4` statement. That accounts for 2 preheader rows plus the `mtc1 zero,$f14`
     row.
   - None of these moved the tie: `while`/`do` forms, `4>i`, pointer or `tbl` locals (copy-propagated), and
     compiled-out uses (each moves 50 or more ops).

## The open residual: where `.alias $8,$sp` lands (w12c lanes b and c are one cause)

Retail needs `entry` (t0) to stay `.noalias $8,$sp` through the set path's `s.s $f14,4($8)`. That lets as1 put
`mfc1 a1` first (lane b). Retail also needs an alias or noalias directive between the camera path's `b $790` and
`$789:` (lane c). That directive becomes a predecessor-less block, which blocks as1's cross-block hoisting into the
compare block (w12b mechanism).

Oracles on the current listing (`asm.sh`):
- moving the directive there gives 8 rows;
- deleting it gives 30;
- any other register's `.alias` or `.noalias` at that point also works;
- `.set`, `.livereg` and `.loc` there do not work;
- swapping `mfc1`/`s.s` in the listing fixes lane b alone.

**Mechanism (traced uopt, `tools/uopt_alias_patch.py`).** These directives are uopt's `Uunal` records. ugen only
copies them.
- `f_base_in_reg(reg, LR, parent)` runs each time a coloured temp is used as an address base.
  - It records `rec[reg]=LR`.
  - The first time `base_sp_noalias(parent)` is true, it emits `.noalias reg,$sp`. Otherwise it marks the pair as
    aliased.
  - The test is true only when the parent is the evaluation of a coloured load temp. Here that is the handle load
    `ilod.J@0(entry)`, colour a0.
  - For ordinary field loads and stores the test is false.
- `func_4247a4` runs at every basic-block start in reemission order, and uopt starts a new block after every
  `cup`.
  - If the block's register map for reg is not `rec[reg]`, it clears the state.
  - If the pair was noalias at that point, it emits `.alias reg,$sp` before the block's label. This is why ugen
    never puts `.alias` after a label.
- For E05F0, the trace shows:
  - noalias at bb 18, from the original handle load;
  - the first block outside entry's live range is bb 32, the MODE reload right after `jal player_conditional_call`.
    So `.alias` lands there.
  - after that, the set path's accesses are ordinary loads and stores, so they mark the pair aliased and nothing
    reopens it;
  - bb 38, after `osRecvMesg`, clears the state again;
  - the camera path's first access (`l.s 8($8)`) marks it aliased before the inserted handle reload in bb 42 could
    reopen it. So nothing is emitted at `$789`.
- Under this model, retail's listing needs entry's live range to cover the block after the stop path's `pcc` call
  and the blocks after the set path's `osRecvMesg`. Alternatively, the set and camera paths need a coloured-temp
  evaluation of `entry->x` before their first plain access.
  - Retail code shows neither. There is no restore of t0 after `osRecvMesg`, and the loads are into ring temps
    f4/f8.
  - So some assumption about retail's source is still wrong.
  - Shapes tried, with no movement on the directive:
    - stop path via `player_conditional_check`, `&D_80140640[slot]` and per-branch pcc;
    - compiled-out uses after pcc (these move the directive but force t0 live to the join, about 58 ops);
    - a volatile MODE read at the join (this removes the per-path MODE reloads; the alias still follows the pcc
      call);
    - `entry` built as `D_80140640+slot`, `entry=D_80140640; entry+=slot` (this removes noalias entirely),
      `&entry[slot]` and early or late assignment;
    - `client_sync(entry->handle, entry->level=level)`;
    - inverting the `level` test (which changes the layout).

**Next hypotheses.**
- A source form in which the stop path's last use of t0 is not followed by a new block. For example, a different
  call or argument form for `player_conditional_call`: retail IPA shows pcc touches only t6/f0/a0, so t0 survives
  it.
- `entry` as a real, non-propagated register variable. Its base node may give a different `base_sp_noalias` result.
- Check `func_424ddc` (the site that passes `parent`): when is `parent` 0? An access with a 0 parent records nothing.

## Integration notes (for when E05F0 matches)

- Use w12c's landing recipe with these changes:
  - **add `func_800E0048` to members (internal)**. It gives the locked empty stub a body, so it needs
    `force_internal: func_800E0048` (shaping wrapper inlined into E05F0; only the retail stub remains);
  - `mode.c` now defines E0048 before E0050, so the stub keeps its position.
- Group name: `frontier_mode_select`. Members: w12c's nine plus `func_800E0048`. Keep: `func_800D5E64`,
  `func_800E05F0`.
- `prefer_definition` for `entity_hierarchy_update` → `mode.c`, as in w12c.
- E05F0's own rodata, 0.6f and 0.1f at 0x8012438C and 0x80124390, is unverified (the 4 `lui at,0x0` rows).
- Disclosed quirks added by this lane:
  - the E0048 wrapper's identity;
  - `n` as the loop bound;
  - unused `d2` and `d3`;
  - the pointer-typed camera_clip_planes parameter (an m2c prototype fix rather than a quirk);
  - the `[13]` table shape.

## What generalises

- **The inlined-area layout rule.** Blocks are bottom-8-aligned in call order. Within a block, parameters ascend from
  the bottom and the callee's locals sit below them. A nested inline follows its parent. Retail home offsets
  therefore pin how many inline calls come before and after a given helper. One spilled variable's home plus one
  inlined parameter's home was enough to fix the whole layout here.
- **An int cast on a global address passed to two calls makes them the same PRE expression.** PRE may then insert
  the `la` early. A pointer-typed parameter keeps the occurrences separate.
- **Colour ties between a constant web and an address web follow first ucode appearance.** A bound variable assigned
  before the loop flips the tie, but its `li` placement follows the assignment statement.
- **The `.noalias`/`.alias` rules are uopt's (`f_base_in_reg`, `func_4247a4`), not ugen's.**
  - An alias is emitted at the first basic block in reemission order, and every `cup` starts a block, whose register
    map lacks the live range.
  - noalias is reopened only through the evaluation of a coloured load temp on that base.
  - Use `tools/uopt_alias_patch.py` to see the decisions. The `BB/CHK/BIR` lines name each block.
