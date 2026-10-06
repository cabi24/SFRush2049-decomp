# Wave 12, lane w12b: residuals in assembler (as1) placement

All unit scores come from `python3 -m tools.conveyor.pipeline.blob_unit --tag w12b score ... --neighbours`, run from
the repo root on the Pi on 2026-10-06 (tree `ea15236c`, branch `wave11`). Builder scratch:
`watchman2:~/rush2049/scratch/frontier/w12b` (copied from `base`; src/blob, include, tools/cloud, asm/us/blob and
`blob_matched.lock.json` synced from the Pi; trace toolkit installed with `--reuse wtk`). There were no permission
denials. Nothing was committed, spliced or pushed. Nothing was edited outside this lane directory except the new file
`cloud/matches/race_countdown_display.c`.

| Function | Bytes | State | Flags | Exact scorer output |
|---|---:|---|---|---|
| `race_countdown_display` | 1,120 | **MATCH** (was 34/280) | `-O3` | `EQUAL race_countdown_display: 280 words (kept, c_race_countdown_display.c)` / `locked bodies that differ in this unit: 0` / `blob_unit score: 1/1 equal` |
| `func_800D5E64` | 584 | **EQUAL in the real-caller component, provisional** (was 10/146) | `-O3` | `EQUAL func_800D5E64: 146 words (kept, c_best.c)`, `EQUAL best_times_display: 56 words (internal, c_best.c)`, `locked bodies that differ in this unit: 0`, `blob_unit score: 4/7 equal` |
| `entity_process_main` | 1,424 | 2 words off (unchanged) | `-O3` | `FAIL entity_process_main: 2 of 356 words differ` / `locked bodies that differ in this unit: 0` |

Commands:
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w12b score race_countdown_display \
    --with cloud/matches/race_countdown_display.c --neighbours
C=cloud/work/frontier/w12b/comp
python3 -m tools.conveyor.pipeline.blob_unit --tag w12b score func_800D5E64 best_times_display mode_select_handler \
    func_800DED78 func_800DFBA0 mode_select_input func_800E05F0 \
    --with cloud/work/frontier/w12b/d5e64/best.c --with $C/mode.c --with $C/msh.c --with $C/ded78.c \
    --internal best_times_display --internal mode_select_input --internal func_800DFBA0 \
    --internal mode_select_handler --internal func_800DED78 --neighbours
python3 -m tools.conveyor.pipeline.blob_unit --tag w12b score entity_process_main \
    --with cloud/work/frontier/w11a/entity_process_main/best.c --neighbours
```
The rest of the component run is unchanged from w11f: `FAIL mode_select_handler: 684 of 744`,
`EQUAL func_800DED78: 122 words`, `FAIL func_800DFBA0: 21 of 298`, `EQUAL mode_select_input: 38 words`,
`FAIL func_800E05F0: 295 of 332`.

## Integration notes

- **race_countdown_display**: `cloud/matches/race_countdown_display.c`. It is a kept function, EQUAL in the
  whole-program unit with the current `unit_overrides.json` (no new entries) and it supersedes no group.
  - Standalone `score.py fn` does not apply: it prints `156/280 words differ`, because the function depends on
    whole-program IPA (`frontier show`: `preserved: a2`, `signatures: preserved`). Splice it through the
    whole-program path, not as a standalone single.
  - It defines a **file-static** direct-return copy of the sound wrapper `func_800B61A8`. umerge inlines all five
    calls and no static body is emitted: the unit object has one `func_800B61A8` symbol, the locked 8-byte one.
  - The locked kept `src/blob/func_800B61A8.c` (the `new_var` form) is untouched. Calling it instead of the
    static copy gives `FAIL race_countdown_display: 39 of 280`.
  - No own rodata: 2.0f becomes `lui a1,0x4000` and 0.0f becomes `mtc1 zero`.
- **func_800D5E64**: `d5e64/best.c` is a drop-in replacement for `cloud/work/frontier/w11f/comp/d5e64.c` (my copy
  is `comp/d5e64.c`). best_times_display in it is unchanged. Lane w12c owns the cluster landing.
  - It is not spliceable alone. best_times_display is internal, and its other real caller, mode_select_handler,
    is unmatched.
  - The function body alone is in `d5e64/best_body.c`.
  - Two other spellings are also EQUAL in the component (`d5e64/v23.py`): declaration initialisers
    `s32 object=(s32)&D_8010FFC4[player]; Player84 *state=&D_80140420[player];`, and the dead store to `i`
    instead of `object` (`v22.py` c4).

## race_countdown_display: MATCH (34 → 0)

**Mechanism.** It was not xbb candidate selection in the abstract. I built a debug as1 (`tools/build_as1dbg.sh`, see
Tools) and bisected the listing, which found the cause:

- **The trigger.** Our ugen listing closed the `.noalias $3,…` region of the `&D_80152818[player]` register with
  `.alias $3,$sp` *between* the then-arm's `b $2549` and the `$2546:` label.
- **What as1 does with it.** as1 makes that directive a block of its own (bb 1) with no predecessors that falls
  into `$2546` (the `old_remaining != D_80152014` block, bb 36).
  - bb 36's xbb weight (`func_429534`: the minimum over lower-numbered predecessors of their weight + 3/+1/-3)
    becomes **65534**.
  - Its successors restart at weight 1, so the bne block (weight 25) sorts *before* it.
  - The xbb transform only hoists from higher-weight blocks within 3·k+3 of the current block, so `li at,1` is
    never hoisted. The beq then takes `li at,1` as its delay slot instead of becoming `li at,1; beql …; lw ra`.
- **Proof** (`rcd/listing/`): deleting that one directive (`ours_noalias3.s`), or moving it below the label
  (`ours_alias_after_label.s`), reassembles to `want 280 words, got 280; differing rows 0 (words)` (`asm.sh`).
  - ugen never emits `.alias` after a label: there are 0 cases in the 119k-line unit listing, against 53 of
    `b; .alias; label`.
  - So retail had no noalias region open at that point.
- **Source fix.** The working shape is `gc = &D_80152818[player];` assigned once, right after `link = …`, with both
  place_locked tests reading `gc->place_locked`.
  - The address computation then reaches the deep block through PRE (sunk to the use) rather than being computed
    there from source. ugen gives that register no `.noalias`.
  - ctrace shows gc is still an expression web (w54, type 4, v1). So the condition is not "variable vs expression".
    What differs is where the computation came from.
  - With only the assignment moved and the old `D_80152818[player].place_locked` test left in place, the result
    is still 34.

## func_800D5E64: provisional EQUAL (10 → 0)

w11f's listing proof was confirmed on the current tree. Removing `.noalias $21,$sp` and putting ready's home store
at the end of the loop preheader gives 0 rows (`d5e64/listing`). Source shapes, in the order that moved it:

1. **`state=&D_80140420[player];` before the early-return test** (`v13.py` s_pre).
   - uopt sinks the computation past the return, and the `.noalias $21,$sp` disappears. This is the same mechanism
     as in race_countdown_display.
   - Only ready's store placement remained (9 rows; `asm.sh` with the store moved gives 0).
2. **A discarded read of ready right after the test** (`v15.py`, an L37-style read):
   `ready=&D_8010FFC4[player]; if(ready!=0);`.
   - ready's web now has a definition-adjacent occurrence. The split piece holding the definition is coloured v1,
     and its home store lands at the preheader exit, as in retail.
   - The ready variable itself is copy-propagated away, and the home is a **spill temp** of the address web.
   - The rdy() `if(1)` helper and w11f's `f32 vec[3]` frame pad are no longer needed. Without the pad: frame
     144/136 (`p_ne0_novec`), 2 rows.
3. **Frame and home slot.**
   - Retail's home 116 means cfe area 16 with ready's web as spill temp 0. Spill temps are numbered in itable
     (first occurrence) order, so ready's address expression must occur before state's.
   - The ready variable is dropped. A dead store `object=(s32)&D_8010FFC4[player];` before `state=…` carries the
     first occurrence, and `if(&D_8010FFC4[player]!=0);` after the test plus `D_8010FFC4[player]=1` at the end
     give the rest.
   - Result: EQUAL (`v22.py` c1).
4. `object` must stay a named variable. As a CSE expression it takes v0, where retail has t0 (`v21.py` A_noobj;
   `force.sh … p1:w68=c7` gives 0 rows).

About 75 variants this wave (`d5e64/v1.py`–`v23.py`).

## entity_process_main: 2 words (unchanged)

The listing hypothesis tested clean. In `epm/listing`, both `li; sll; lhu $14; not $15` and `li; sll; not $15;
lhu $14` reassemble to the 2 unverified-rodata rows only, so the residual is purely which ring temp each node gets.
Retail allocates in the order li(t8), sll(t9), lhu(t6), nor(t7): the shift is evaluated before the load and the not
after it.

- ugen evaluates the and/not tree strictly operand-first, and source operand order gives only shl,not,load (2
  words) or load,li,shl,not (18 words).
- A uopt-CSE'd shift is coloured (v1), not a ring temp (`epm/v3.py` dup).

About 20 variants (`epm/v1.py`–`v4.py`), all without movement:

- `(u16)`/`(s16)` casts on the not or the whole expression (17/18/19/13 words, other ring phases);
- mask carriers `pad1`, `xlu`, `body` (copy-propagated);
- dead or discarded early `1 << i` occurrences (the d5e64 lever), before the condition and inside it.

**Next hypothesis:** retail's shift reaches ugen as a separately evaluated (inserted) computation that is *not* a
coloured web. Find which construct gives a ring-temp expression evaluated ahead of its statement (`ugt.sh SCHED=1`
on candidates).

## Tools (lane `tools/`)

| Tool | What it does |
|---|---|
| `build_as1dbg.sh [SCRATCH]` | Builds a debug as1 on the builder; the object is identical to stock as1. Two switches: `AS1DBG=N` sets as1's internal debug word (0x10030818); level 3 prints the xbb pass (`current bb, weight, stalls`, MOVETO/MOVEFROM), level 4 per-bb weights, level 8 dominator/postdominator sets and block dumps. `AS1BB=1` (`as1_patch_bb.py`) prints every bb's id, weight, stalls, flags and predecessor list at xbb entry. |
| `rcd/dbg.sh FILE.s [LEVEL]` | Runs as0 + the debug as1 on a listing and prints the log. |
| `asmdis.sh LABEL FILE.s` | Assembles an edited listing and prints the objdump. |
| `rcd/bis.py RANGES…` | Bisects a listing by deleting instruction line ranges. `rcd/ins*.py` insert directives or labels. |
| `fvar.py FUNC variants.py BASE` | Component variant runner: whole-file replacement from a marker. |
| `svar.py FUNC BASE.c variants.py` | Single-file OLD→new variant runner (`PRE`, `POSTMAP` hooks). |
| `lst.sh LABEL FUNC` | Snapshots the last unit build and prints FUNC's ugen listing. |

## What generalises

1. **A `.alias`/`.noalias` directive between an unconditional `b` and the next label blocks as1's cross-block
   hoisting into that labelled block.**
   - as1 turns the directive into a predecessor-less block, and the labelled block gets xbb weight 65534.
   - The symptom is a load-use stall in a branch-target block: retail has `lw; li at,K; beql …; <target insn>`, we
     have `beq …; li at,K`.
   - To check, find `b`, then `.alias`, then a label in the listing. If deleting the directive closes the
     residual, the source fix is to stop that register from being noalias-marked.
2. **PRE-placed address computations carry no `.noalias`.**
   - A pointer to a global, assigned early in a block that is not its use block (before an early return, or before
     a nested `if`), reaches its uses through PRE, and ugen then emits no `.noalias $r,$sp` for it.
   - This closed both race_countdown_display and func_800D5E64.
   - Try it whenever as1 keeps a store above (or moves it past) a stack store where retail does the opposite.
3. **A discarded read right after a definition turns a split variable's definition piece into a coloured piece, and
   its home store moves to the block exit (the loop preheader).** This is L37 used for placement rather than for
   killing a web.
4. **Spill-temp homes follow itable first-occurrence order.** A dead early assignment of the address expression
   moves it to temp 0 at no instruction cost. Watch which coloured expression web occurs first.
5. as1's internal debug level gives the xbb pass's decisions directly. Use `build_as1dbg.sh` before mutating
   listings blind.
