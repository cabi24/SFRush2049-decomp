# Wave 13, lane w13b: func_800E05F0 — compiler-side trace of the `.alias $8,$sp` residual

The scores come from `TAG=w13b sh cloud/work/frontier/w13b/comp/run.sh`, run on the Pi on 2026-10-06 (master
`f2e8380d`). The aligned rows come from `cloud/work/frontier/tools/trace/udiff.py` (TAG=w13b).
- Builder scratch: `watchman2:~/rush2049/scratch/frontier/w13b`. It was copied from `base`, then src/blob, include,
  tools/cloud, asm/us/blob and `blob_matched.lock.json` were synced from the Pi. The toolkit was installed with
  `--reuse wtk`, and a traced uopt (`bin/uopt_al`, built from `tools/uopt_alias_patch.py`) was added.
- There were no permission denials.
- Nothing was committed, spliced or pushed. Nothing was written outside `cloud/work/frontier/w13b/` except
  `build/trace/w13b/` and the lane's builder scratch.

**func_800E05F0 is still not EQUAL, so the cluster cannot land yet. No `groups/` is delivered.** The `.alias`
residual (w12c lanes b and c) is now **closed**: the uopt mechanism is traced and turned into a C change. The
function goes from 30 differing word rows (143 differing words) to **2 differing words**. Those two words are the
loop-preheader colour tie (w12c lane a), which is a pure register-colour residual that `force.sh` proves.

| Function | Bytes | State | Flags | Scorer output (`comp/run.sh`) |
|---|---:|---|---|---|
| `func_800E05F0` | 1328 | near-miss: **2 differing words**, 6 aligned word rows (4 of them are the unverified 0.6f/0.1f rodata), 2 ops rows, frame 224/224 | `-O3` unit | `FAIL func_800E05F0: 2 of 332 words differ` (`+0x0b4 image 02602025 unit 24050004`, `+0x0e4 image 24050004 unit 02602025`) |
| `func_800E0048` | 8 | EQUAL. Provisional; camera-wrapper hypothesis | `-O3` unit | `EQUAL func_800E0048: 2 words (internal, c_mode.c)` |
| `func_800E0050` | 1440 | EQUAL. Provisional | `-O3` unit | `EQUAL func_800E0050: 360 words (internal, c_mode.c)` |
| `mode_select_handler` | 2976 | EQUAL. Provisional | `-O3` unit | `EQUAL mode_select_handler: 744 words (internal, c_msh.c)` |
| `func_800DEF60` | 8 | EQUAL | `-O3` unit | `EQUAL func_800DEF60: 2 words (internal, c_msh.c)` |
| `func_800D5E64` | 584 | EQUAL. Provisional | `-O3` unit | `EQUAL func_800D5E64: 146 words (kept, c_d5e64.c)` |
| `func_800DFBA0` | 1192 | EQUAL. Provisional | `-O3` unit | `EQUAL func_800DFBA0: 298 words (internal, c_mode.c)` |
| `best_times_display` | 224 | EQUAL. Provisional | `-O3` unit | `EQUAL best_times_display: 56 words (internal, c_d5e64.c)` |
| `func_800DED78` | 488 | EQUAL. Provisional | `-O3` unit | `EQUAL func_800DED78: 122 words (internal, c_ded78.c)` |
| `mode_select_input` | 152 | EQUAL. Provisional | `-O3` unit | `EQUAL mode_select_input: 38 words (internal, c_mode.c)` |

The same run prints `locked bodies that differ in this unit: 0` and `blob_unit score: 9/10 equal`.
`udiff.py func_800E05F0 --summary` gives `want 332 words, got 332; differing rows 6 (words); frame 224/224; unverified 4 unresolved 0`
(`--ops`: `differing rows 2 (ops)`).

## Files

| Path | Contents |
|---|---|
| `comp/` | w12i's component. `mode.c` is the new best. `run.sh` is the replay (`TAG`; `MODE=file.c` swaps in a different mode.c). |
| `func_800E05F0/best.c` | A copy of `comp/mode.c`. |
| `var/*.c` | Whole mode.c variants. They are named in the "Tried" section below. |
| `tools/uopt_alias_patch.py` | w12i's patch, extended. A `BIR` line now also prints the base expression's kind (`a1k`), the root node's sym/addr and the pair state `S`. `ALDUMP=1` dumps the root and base nodes. |
| `tools/altrace_remote.sh` | Runs on the builder: traced uopt on `st_LABEL` → `al_NAME.log` (finds the procedure by its globalcolor ordinal). |
| `tools/alu.py` | Annotates the trace's `U op=` records with ucode opcode names. |
| `tools/udump.py` | Compact dump of one procedure's pre-uopt ucode from a snapshot's `merged`, by source line. |
| `tools/ora.sh`, `ora/*.s` | Listing oracle: delete or insert lines in a ugen listing, reassemble with stock as1, print rows. |
| `tools/v.sh`, `tools/mk.py` | Score one mode.c variant (`AL=1` prints every `$8` alias directive); exact-substring variant maker. |

## The `.alias` mechanism (closed)

**Trace.** `f_base_in_reg(reg, base_expr, root)` runs each time a coloured base register is used.
- After a register-map change, the first call decides the `$sp` pair state `S`:
  - `S=2` and `.noalias reg,$sp` when `f_base_sp_noalias(root)` is true;
  - otherwise `S=1`, which emits nothing.
- Later calls do not change `S`.
- At each block start, `func_4247a4` clears the state when the register map no longer holds the recorded value.
  If `S` was 2 at that point, it emits `.alias reg,$sp`.

`root` is the **root of the access's address operand as written in the ucode** (the third argument of
`func_424ddc`, saved at sp+136). It is not the coloured expression:
- `entry->x` in a block where `entry` was not assigned has the **isvar** root `varM(-12)`, the variable `entry`
  itself. `base_sp_noalias(isvar)` goes through `pointtoheap`, which is false, so the pair is aliased.
- `D_80140640[slot].x` (and PRE-inserted reloads, site 6) have the **islda** root `ldaS(D_80140640)` with
  memtype byte 4. `base_sp_noalias` is true, so `.noalias`.
- In both cases the base register is the same coloured web, `ixa(lda, slot*20)` in t0.

**What retail needs (listing oracle `tools/ora.sh`, base listing `ora/base.s`).**
- `e5`: `.noalias $8` at the set path's first access, `.alias` after `osRecvMesg`, `.noalias` at the camera
  path's first access and `.alias` after `b $790`. This gives 8 rows, the same as w12i's single move.
- `e6`: the set path half alone gives 30 rows.
- `e7`: the camera half alone gives 10 rows.
- The `.alias` after `player_conditional_call` can stay. What retail has is **re-opened noalias** in both paths.

**C change.** The first `entry` access of the set path (`level != D_80140640[slot].level`) and of the camera
path (`style != D_80140640[slot].style`) index the array directly. uopt then emits exactly e5's directive
pattern by itself (v.sh `AL=1` on `var/v1.c` shows `.noalias` at 210, `.alias` at 239, `.noalias` at 259 and
`.alias` after `b $790`). Result: 30 rows → 8, and `4 of 332 words differ` (only the preheader words).
- `var/k1.c` writes every set/camera access directly and `var/k3.c` writes every access directly (`entry` is
  kept only for `player_conditional_call`). Both give the same result.
- Forms that are copy-propagated `entry` everywhere cannot work. This is why w12i's `entry`-assignment variants
  did not move the directive.

`level=value=0.0f` was then moved into the for-init after `i=0` (`var/a1.c` = the best). This puts
`mtc1 zero,$f14` in retail's order: 2 words.

## The remaining residual: a1/a2 colour tie (w12c lane a), not closed

The constant-bound form `for(i=0,level=value=0.0f;i!=4;i++)` is `var/a3.c` (5 words, 9 rows, 0 ops rows). The
only remaining difference is that `&D_8011F060` (w174) and the constant `4` (w178) tie at save 3.333, nocs 3.
- `force.sh a3 func_800E05F0 p1:w174=c5,p1:w178=c4` gives `differing rows 4 (words)`, which is the rodata only.
  So **the constant form plus the right colour equals retail**.
- The current best uses `n=4` (a var web, save 3.67, wins). Its `li a1,4` is emitted at the `n=4` statement,
  but retail's comes after the strength-reduced `move a0,s3`, which uopt inserts at the end of the preheader.
  No statement order puts it there, so the n form keeps the 2-word swap.

Traced facts about the tie:
- Constant/lda register webs are numbered by **first surviving occurrence**. The loop body puts the table (174)
  before the bottom test's 4 (178).
- cfe does emit a guard `i!=4` before the body (`udump.py`, ucode 31135). It is folded at read time because
  `i=0` is in the same block, so it allocates no number.
- If `i=0` sits in an earlier block (`e1`) or the bound is a var set earlier (`e2`), the guard survives, and uopt
  then **unrolls** the loop (376 and 413 words). Retail is not unrolled, so its guard folded at read time.
- A compiled-out branch on 4 does give 4 the lower number, but:
  - in the preheader (`g1`, `i1`, `i2`) it adds a block to 4's live range (save 2.5): 352 words, model loses s3;
  - inside the loop (`h1`, `j1`, `j2`) it adds a loop block, which reorders the FP colours: 358 words. In `h1`,
    4 does take a1.
- `(void)(i==4)` and a dead store `d2=(i==4)` (`h3`, `h4`) are removed before numbering and do nothing.
- Also unchanged: `4!=i` (`d4`), `i<4` (`f1`), `continue` form (`d1`), `(D_8011F060+i)[0]` (`d2`).
  `do/while` (`d3`) breaks.
- Not a frame fit: an inlined `mode_select_input(entry,level,style)` set path that replaces n/d2/d3
  (`m1`, `m2`) gives a 200-byte frame.

**Next hypothesis for lane a:** something in retail's source must give the constant 4 its register-web number
before `&D_8011F060` without adding a block to the loop or the preheader. One option is a surviving occurrence
of 4 earlier in the procedure, in a block that 4's live range already covers. Another is for the table's first
use to follow the latch in numbering order. A uopt-side next step is to trace where those numbers (bit
positions) are assigned for constants (`f_loopunroll` and the pass after DCE), to find which surviving
occurrences count.

## Integration notes (for when E05F0 is EQUAL)

These are unchanged from w12i, with the w13b source:
- Group `frontier_mode_select`, files `comp/{d5e64,mode,msh,ded78}.c`.
- members: `func_800D5E64` and `func_800E05F0` (kept); `best_times_display`, `mode_select_handler`,
  `func_800DED78`, `func_800DEF60`, `mode_select_input`, `func_800DFBA0`, `func_800E0050` and `func_800E0048`
  (internal).
- Overrides:
  - `force_internal: func_800DEF60` (deleted `check_forces_on_car`);
  - `force_internal: func_800E0048` (inlined wrapper; only the retail stub remains);
  - `prefer_definition entity_hierarchy_update → mode.c`.
- No existing locked group is superseded.
- Own rodata to verify at splice:
  - E05F0 0.6f/0.1f at 0x8012438C/0x80124390;
  - the msh, E0050 and DFBA0 literals listed in w12c.
- Disclosures:
  - the unused `d2`, `d3` (exact frame residual);
  - the `for(i=0,...,n=4;i!=n;i++)` bound;
  - the func_800E0048 camera-wrapper identity hypothesis;
  - the mixed `entry->` / `D_80140640[slot].` access (alias shaping, see above);
  - the w12c quirks (E0050 `unused[2]` and `if (rpm < t->b1) {}`, DFBA0's `if (count0|…) {}`, D5E64's).

## What generalises

- **`.noalias`/`.alias` depend on the access's ucode address root, not on the register.** uopt reopens noalias
  for a coloured base only if the first access after a register-map change is written through the global array
  (islda root). An access through a copy-propagated pointer local (isvar root) is aliased. So a stray `.alias`
  or a missing `.noalias` can be fixed by writing that block's first access as `G[idx].f` instead of `p->f`. It
  costs no code, because both use the same coloured register.
- The listing oracle can test **directive sets** (e5/e6/e7), not just single moves. That tells you which blocks
  need their first access changed.
- A guard that cfe emits for a for-loop and that uopt does not fold at read time leads to unrolling. Keep `i=0` in
  the guard's block. Constant register webs are numbered at their first surviving occurrence, after DCE.
  Compiled-out branches count; dead stores and `(void)` expressions do not.
