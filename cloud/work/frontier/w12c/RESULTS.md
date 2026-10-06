# Wave 12, lane w12c: the mode-select cluster

Scores come from `python3 -m tools.conveyor.pipeline.blob_unit --tag w12c score ... --neighbours`, run from the
repo root on the Pi on 2026-10-06 (branch `wave11`, `ea15236c`). Aligned rows come from
`cloud/work/frontier/tools/trace/udiff.py` (TAG=w12c).

- Builder scratch: `watchman2:~/rush2049/scratch/frontier/w12c`. It was copied from `base`, then src/blob, include,
  tools/cloud, asm/us/blob and the locks were synced, and the toolkit was installed with `--reuse wtk`.
- There were no permission denials.
- Nothing was committed, spliced or pushed. Nothing was written outside this lane directory.

**The cluster is now 8 of 9 EQUAL with no stand-ins: every caller in it is the real function.** The one body left
is `func_800E05F0`. It is the keystone: `mode_select_handler` takes its model pointer in `s1` from E05F0,
`func_800E0050` is called only by E05F0, and `func_800DFBA0` is called only by E05F0. So **nothing can land until
E05F0 matches.** Every row below other than E05F0 is therefore *provisional (code identical, real callers)*, not a
match.

| Function | Bytes | State | Flags | Scorer output (`sh cloud/work/frontier/w12c/comp/run.sh`) |
|---|---:|---|---|---|
| `mode_select_handler` | 2976 | **EQUAL** (was 149 ops rows). Provisional only through its caller E05F0. Own rodata unverified | `-O3` unit | `EQUAL mode_select_handler: 744 words (internal, c_msh.c)` |
| `func_800DEF60` (stub) | 8 | EQUAL. This is the deleted arcade `check_forces_on_car` | `-O3` unit | `EQUAL func_800DEF60: 2 words (internal, c_msh.c)` |
| `func_800E0050` | 1440 | **EQUAL** (was 12/360) with its real caller E05F0. Own rodata unverified | `-O3` unit | `EQUAL func_800E0050: 360 words (internal, c_mode.c)` |
| `func_800E05F0` | 1328 | near-miss: **12 ops rows, 43 word rows** (was 60 ops rows) | `-O3` unit | `FAIL func_800E05F0: 152 of 332 words differ; compiled body is 331 words, target 332; ...` |
| `func_800D5E64` | 584 | EQUAL. w12b's `d5e64/best.c`, pulled in unchanged | `-O3` unit | `EQUAL func_800D5E64: 146 words (kept, c_d5e64.c)` |
| `func_800DFBA0` | 1192 | EQUAL. w12a's `if (count0\|count1\|count2\|i) {}`, merged in | `-O3` unit | `EQUAL func_800DFBA0: 298 words (internal, c_mode.c)` |
| `best_times_display` | 224 | EQUAL | `-O3` unit | `EQUAL best_times_display: 56 words (internal, c_d5e64.c)` |
| `func_800DED78` | 488 | EQUAL | `-O3` unit | `EQUAL func_800DED78: 122 words (internal, c_ded78.c)` |
| `mode_select_input` | 152 | EQUAL | `-O3` unit | `EQUAL mode_select_input: 38 words (internal, c_mode.c)` |

The same run prints `locked bodies that differ in this unit: 0` and
`blob_unit score: 8/9 equal; object build/blob_unit/w12c/unit.o (4.7s)`.

The full E05F0 line is:
`FAIL func_800E05F0: 152 of 332 words differ; compiled body is 331 words, target 332; +0x40c: .rodata+0x104: retail words encode 0x-0007fee, outside the image`.
The scorer counts shifted words after the one missing instruction. The aligned diff is
`want 332 words, got 331; differing rows 43 (words)` and `differing rows 12 (ops); frame 224/224`.

## Files

| Path | Contents |
|---|---|
| `comp/run.sh` | The replay. No stand-ins. Each file is its own translation unit. |
| `comp/d5e64.c` | w12b `d5e64/best.c`: `func_800D5E64` and `best_times_display`. |
| `comp/mode.c` | `mode_select_input`, `func_800DFBA0` (w12a, with natural literals 0.7f / 0.33f), `func_800E0050`, `func_800E05F0`. The headers inside describe the shaping. |
| `comp/msh.c` | `mode_select_handler` and `func_800DEF60`, rewritten from the arcade source. |
| `comp/ded78.c` | `func_800DED78` (dot_audio_force, unchanged). |
| `mode_select_handler/best.c` | Copy of `comp/msh.c`. |
| `func_800E0050/best.c`, `func_800E05F0/best.c` | Copies of `comp/mode.c`. |
| `tools/rv.py BASE.c FUNC variants.py [FUNC…]` | Replacement-list variant runner over the whole component. `SLOT=` picks which component file BASE replaces; `ONLY=` runs one variant. |
| `var/` | All variant sources and variant lists, `v*.py`. |

## mode_select_handler: 149 ops rows → EQUAL (`comp/msh.c`)

**Arcade ancestor:** `do_bump_sounds()` and `check_forces_on_car()` in
`reference/repos/rushtherock/game/carsnd.c`. The structure and every constant match:
- `range(high_force*.01,150,235)`;
- `bump_index |= 1<<i`;
- car-to-car `abs(peak_vec[i]) > high_force*.5`;
- `scrape_state` switch 0 / 1-3 / 4-6 with `+= 3`;
- `scrape_snd` switch 1..6;
- the `lastthump` / `thumpflag` switch 1..3.

The campaign m2c draft was replaced by a natural rewrite. These are the forms the words prove:
1. **Literals are this function's own `.rodata`, not globals:** 5000.0f, 0.01f, 0.65f, 0.001f, jump table, 0.1f,
   jump table, 0.16666667f, at 0x80124324..0x8012436C. Bit patterns were checked against the image.
2. **Random:** `(s32)(func_8008B2E4(13.0f) + 48.0f)`, the locked arcade Random, inlined. The `(u32)` conversion
   explodes the code.
3. **Counters:** `s32 p = m->player`; this gives the narrowing to the `best_times_display(s16)` register parameter.
   The impact loops use an `s32 i`, which is strength-reduced (`v1 += 24 < 96`). Only the DED78 loop has an
   `s16` counter.
4. **`check_forces_on_car` is a separate function, defined after the caller** as in carsnd.c, and internal. It is
   named `func_800DEF60` because its deleted body is that retail stub, and it is EQUAL as 2 words.
   - With the loop written inline in the caller, 2 words differ: an as1 tie, where `mtc1 $f20` is scheduled
     ahead of the bump_time load. Proven by listing edit: `asm.sh` with the `li.s $f20` moved after the load
     gives 0 rows.
   - Inlined from a later definition, the DED78 calls carry later line numbers, and the tie goes the retail way.
   - Defined *before* the caller, umerge inlines DED78 into it instead (DED78 becomes 2 words).
5. **`kill_scrape_sound()` is written out at all three sites.** A macro (one line number) or an `__inline`
   function schedules the `D_80140B10` address `addu` differently. The arcade `if (scrape_state||scrape_time)`
   guard is gone on N64.
6. **Scrape pan arguments are int literals `1` / `-1`.** These are their own constant webs; retail re-materialises
   them per case, and `0.0f` stays shared with `$f20`. Writing `1.0f` makes them share `$f30` (8 ops rows).
7. **Volume is two statements,** `volume = volume / 256.0f; volume = volume * 0.65f;`. One expression puts the
   product in a temporary, and the FP temp ring then shifts for the rest of the function (137 → 10 words).

## func_800E0050: 12 → EQUAL (`comp/mode.c`)

- **It has a real caller.** w10d's premise that "retail has no reference" is wrong. `func_800E05F0` calls it at
  0x800E0688 (`sw s3,0(sp)` / `jal` / `sw s3,224(sp)`: a memory parameter in the out-arg slot, with s0–s8 and
  f20–f30 unsaved). With E05F0 in the unit as the real caller, the 4 as1 words of w10d's residual disappear.
  The `func_800E0048` dead-caller stand-in is no longer needed; the locked stub stays as it is.
- **`player_conditional_check` is not redefined.** Its locked kept body is inlined cross-file by umerge (prototype
  only), and it stays EQUAL.
- **The b0/b1 conversion colour tie** (w10d's 8 words; save 15 each, w76/w85) is broken by a compiled-out
  `if (rpm < t->b1) {}` in the final `else` arm, the w11a lever. This is a shaping quirk and is disclosed.
  - Placed inside the second segment, the same lever also extends the int b1 web (nocs 2→3), and v1/a0 swap.
  - In the `else` arm only the b1 conversion web gains a use, and the result is EQUAL.
- `f32 unused[2]` is still the exact frame residual (retail frame 72; 19 words without it). It is disclosed.

## func_800E05F0: 60 → 12 ops rows (`comp/mode.c`)

What moved it, in order:
1. `D_8012438C` / `D_80124390` are E05F0's own literals, **0.6f / 0.1f**. They were coded as global loads.
   Coloured as a constant, 0.6 changed the whole FP allocation.
2. `D_8002EB90` is `volatile`, as msh.c already has it. Retail reloads it three times.
3. `frame_sync(s32,s32,s32,s32)` in this file: retail passes `t1` and the original slot with no `sll/sra`.
4. Ternary max updates, `style = style<value ? value : style`. The if-form gives `style` a caller-saved register,
   and then f28 is lost.
5. `D_8011F060[i]/2.0f + 1`. The int `1` makes a separate constant web, and `/2.0f` becomes a separate 0.5 web,
   so style's 0.5 init is a fresh `mtc1` into f28 as in retail.
6. `weighted = model->power[i]*D_8011F060[i]`. uopt swaps the commutative operands (pretrace: `mpy(D,power)`), and
   this order gives retail's `mul.s f12,f0,f4`.
7. `level = value = 0.0f`. This dead init makes `value`'s web precede `weighted`'s; they tie at save 15, and
   retail's value takes f2.
8. `entry->handle` is read directly everywhere, with no `handle` local. The else path then reuses `a0`, as retail
   does.
9. `& 0x10` flag test with the `w4 & w56` operand order.

**Open residual, every piece traced:**
- **(a) a1/a2 tie:** `&D_8011F060` w196 against the constant `4` w200, both at save 3.333. The force
  `p1:w196=c5,p1:w200=c4` gives a loop identical to retail. Loop-form variants (while, do, `!=4`, `<=3`) do not
  move it. An in-loop `if (i<4){}` breaks the loop.
- **(b) `swc1 $f14,4(t0)` / `mfc1 a1` order before `client_sync`:**
  - ugen emits `.alias $8,$sp` right after `jal player_conditional_call` (the level<=0 path). That makes the
    `entry->level` store depend on the stack spill.
  - Deleting that one directive and reassembling (`asm.sh`, `var` e6) fixes the order.
  - Not yet found: the source shape that keeps entry `noalias` there.
- **(c) camera_clip_planes argument setup:** as1 hoists `lui/addiu a2` (`&D_801141B0`), `mfc1 a3` and the
  `lui at,0xc000` into the compare block's FP-hazard slot. Retail leaves that `nop` empty and keeps them at the
  join.
  - Listing edits (moving `la $6` / `s.s 16($sp)` into the join) do not stop as1 from hoisting.
  - So the difference is upstream of the listing order of this block. The likely cause is block structure, or an
    alias or live-register fact like (b).
  - This is also the one missing word (retail duplicates `lui at` into the `bc1tl` delay slot).
- **(d) Frame 224:** this is reached only with the `layer_set` / `layer_stop` static inlines plus the block-local
  `pos`. Two stack homes still differ: level at 204 against retail 180, and the inlined `h` at 152 against 168.
  - Treat the helpers as a hypothesis. The image has no deleted-procedure stubs for two statics near E05F0; only
    `func_800E0048` (before E0050) and `func_800DEF60` exist.
  - A shared `layer_place` helper for E0050 and E05F0 was tested and is worse (25–32 ops rows).
  - Without the helpers the frame is 200 and the ops rows are unchanged.

**Next hypotheses:**
- Find what ends `entry`'s noalias range in ugen. The candidate is the level<=0 path layout or the
  `player_conditional_call(entry)` argument; (b) and maybe (c) follow from it.
- Then size the frame with the E0048 stub as the one deleted static.

## Integration recipe (when E05F0 matches)

Nothing is spliceable now. Splicing any subset would need E05F0 as a stand-in, because `mode_select_handler`'s
s1 parameter, `func_800E0050`'s memory parameter and `func_800DFBA0` all come from it. No sub-cluster has only real
callers without E05F0:
- `best_times_display` ← D5E64 and msh;
- msh ← E05F0;
- DED78 ← msh;
- `mode_select_input` ← DFBA0 ← E05F0.

There is no existing locked group with any of these names, so the landing group is new; suggested name
`frontier_mode_select`.
- **Files:**
  - `d5e64.c`, `mode.c`, `msh.c`, `ded78.c`, from `comp/`;
  - each compiled as its own file in one `-O3` group (`-g0 -O3 -mips2 -G 0 -non_shared`).
- **members:**
  - `func_800D5E64`, `func_800E05F0` (kept);
  - `best_times_display`, `mode_select_handler`, `func_800DED78`, `func_800DEF60`, `mode_select_input`,
    `func_800DFBA0`, `func_800E0050` (internal).
  - `func_800DEF60` is a locked empty stub that this group replaces with the deleted body of
    `check_forces_on_car`. List it, and add `force_internal: func_800DEF60` ("deleted check_forces_on_car;
    inlined into mode_select_handler, only the retail stub remains").
- **keep:** `func_800D5E64`, `func_800E05F0`.
- **unit_overrides:**
  - `prefer_definition` for `entity_hierarchy_update` → `src/blob/groups/frontier_mode_select/mode.c`. The
    `__inline` definition must be visible to inline into mode_select_input, E0050 and E05F0, as w10g noted.
  - The `force_internal` entry for `func_800DEF60` above.
  - `player_conditional_check` needs nothing: the locked body is inlined cross-file.
- `func_800E0048` stays the locked stub. It is not a member, unless E05F0's final form defines a static there.
- **Own `.rodata` to verify at splice:**
  - msh: 0x80124324–0x8012436C, including two jump tables;
  - E0050: 890/0.15/0.85/0.05/0.8 at 0x80124378–0x80124388;
  - E05F0: 0.6/0.1 at 0x8012438C/0x80124390;
  - DFBA0: 0.7f / 0.33f at 0x80124370 / 0x80124374. These were w12a's `extern D_80124370` / `D_80124374`;
    `comp/mode.c` now uses the natural literals and DFBA0 stays EQUAL.
- **Disclosed shaping quirks in the group:**
  - E0050's `f32 unused[2]` (frame) and compiled-out `if (rpm < t->b1) {}`;
  - DFBA0's `if (count0|count1|count2|i) {}` (w12a);
  - D5E64's quirks (w12b);
  - E05F0's dead `value` init and whatever closes (a)–(d).

## What generalises

- **as1 tie-breaks follow line numbers, and an inlined function contributes *its own* line numbers.** Defining a
  helper after its caller (arcade file order) moved an `mtc1` behind a load and closed mode_select_handler. If an
  inline helper's position in the file matters, check the arcade file order.
- **An internal "dead" function may simply have a caller the call scan missed.** Check `tdis` of the neighbours for
  `jal` before building dead-caller stand-ins (E0050 is called from E05F0).
- **Global-looking `D_801243xx` float loads in a draft are usually the function's own literals.** Read the
  image word; there were five such loads in this cluster. As literals they become constant webs and change the
  colouring.
- **uopt canonicalises commutative operands.** Swapping `a*b` in the source changes the order inside uopt's `mpy`
  (`pretrace.sh` shows it), and ugen emits the operands in uopt's order. If retail has the other operand order,
  swap the source.
- **A dead `x = y = 0` init moves a variable's web ahead in numbering, which decides colour ties.** It emits no
  code.
- **The `.alias rN,$sp` that ugen emits after a call taking the pointer** makes later stores through that register
  depend on stack spills, which reorders `mfc1` against `swc1`. `asm.sh` with the directive deleted is a quick
  proof.
