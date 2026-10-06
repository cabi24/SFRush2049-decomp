# w12d results (wave 12): callers unblocked by wave 11's engine group

Assignment: `tire_sound_update` and `differential_output` (newly ready after w11e's `frontier_level_objects`
group was locked), plus any other ready function that calls `engine_sound_update` / `transmission_ratio_get`
/ `engine_torque_calc`. `frontier next --limit 1000` (67 ready functions) has no other such caller, so the
assignment is exactly these two functions.

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| `tire_sound_update` | 504 | **MATCH** (single, kept), own .rodata verified | `-g0 -O3 -mips2 -G 0 -non_shared` | `tire_sound_update:` / `  MATCH` / `    own .rodata verified at 0x8012325C..0x80123264` |
| `differential_output` | 648 | 13 words off, **colour-only** (proven with force.sh) | same | `FAIL differential_output: 13 of 162 words differ` |

New strict coverage: 1 function, **504 bytes**. No group is extended or superseded.

## tire_sound_update — MATCH

Deliverable: `cloud/matches/tire_sound_update.c` (first draft, no shaping quirks).

Strict scorer (builder, own scratch `~/rush2049/scratch/frontier/w12d`):
```
IDO_DIR=…/ido python3 tools/cloud/score.py fn cand/tire_sound_update.c tire_sound_update --flags "-g0 -O3 -mips2 -G 0 -non_shared"
tire_sound_update:
  MATCH
    own .rodata verified at 0x8012325C..0x80123264
exit=0
```
Whole-program unit (Pi):
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w12d score tire_sound_update --with cloud/matches/tire_sound_update.c --neighbours
  EQUAL tire_sound_update: 126 words (kept, c_tire_sound_update.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w12d/unit.o (4.7s)
```
Semantics: it fills the name-id table `D_801427C0[157..385]` with `string_copy_format(name, 0, lang - 1, 0)`
from four string-pointer tables. These are `D_8011B02C[59]` (LODs/explosion spheres), `D_8011AFDC[8]` (weapons),
`D_8011AFFC[12]` (wing parts), then an extra `"MINEG1"` entry when `D_8013FECC || D_8013FECD`, then
`D_8011B118[i - 236]` (track props) up to index 386. After that it calls `engine_sound_update`. One running
index `i` gives retail's `s2 = 4*i` shared induction variable (the old m2c draft's offset accumulator hid it).
The `"MINEG1"` literal at 0x8012325C is this function's own .rodata, and the scorer verified it. Integration:
a plain single. It needs no unit_overrides entries.

## differential_output — 13 words off, colour-only

Best: `cloud/work/frontier/w12d/differential_output/best.c` (variants in `dout/`; the start was B84's
`differential_output_word.c`, at 153/162 in the unit).
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w12d score differential_output --with cloud/work/frontier/w12d/differential_output/best.c --neighbours
  FAIL differential_output: 13 of 162 words differ
       defined by c_best.c (kept)
       umerge inlined into it: node_flags
  locked bodies that differ in this unit: 0
```
Colour oracle (`tools/trace/force.sh`, snapshot of the same source):
```
force.sh e1 differential_output p1:w11=c18,p1:w89=c17 --summary
[CDX] p1color … web=11 … reg=s4 forced=18
[CDX] p1color … web=89 … reg=s3 forced=17
want 162 words, got 162; differing rows 0 (words); frame 80/80; unverified 0 unresolved 0
```
So the whole residual is the `parent` (w11) / constant `-1` (w89) s3/s4 swap.

What closed the structure (from 78 → 13 aligned rows):
1. **`s16 parent` parameter** (B84 used `int` + a local). This gives retail's entry cvt, the `sw a1,84(sp)` home
   store and the redundant cvt on the recursive call's argument.
2. **`int result` with `(s16)` casts on both `func_8008E26C` returns.** With `s16 result`, uopt copy-propagates
   `result = parent` into the call argument. Retail keeps the per-iteration copy in s1.
3. **An inlined flags getter in the 0xE00 branch only** (`node_flags(current) | 0xE00`). Retail has
   `lw v0,64(s0)` … `jal; ori a3,v0,0xe00`. Every plain spelling (`|`, operand order, `u`-suffix, casts)
   gives `lw a3; ori t7,a3; move a3,t7`. Using the getter in both branches changes the frame (72). A static
   getter defined *after* this function leaves its stub at 0x800AC660. That is exactly the locked caller-less
   `func_800AC660`. **Conflict to resolve:** w11e attributed `func_800AC660` to the entity-flags getter that
   `transmission_ratio_get` inlines (a hypothesis, not proved). The position evidence here (a stub
   directly after its only inliner) favours a node-flags getter for differential_output. Nothing is changed in
   the locked group.
4. `volatile u8 D_80140BDC` (as in the locked group), and `first = -1` before `current = node` (scheduling of
   `li s2,-1`).
5. `if(parent){}` at the end of the function (compiled-out read, the w11a lever): 22 → 13 rows. It ties parent with
   `first` (5.1 = 51/10), and `first` wins the tie by web number.

Why the last swap resists (≈70 variants, with ctrace numbers): retail needs save(first) ≥ save(-1) >
save(parent). The constant -1 is 30/7 = 4.2857. Each extra end-of-function `if(parent){}` adds +1 tot but at most
+1 nocs (two reads give 52/11, four give 54/12), and each one adds 0.25–0.5 to the per-procedure callee-saved cost.
With one read the cost is 9.75. That is just under the tot (10) of the `&D_80149D94` constant web, so any second read
splits that web: s8 is lost and the frame becomes 72. Keeping it alive with `if(D_80149D94){}` in the loop (tot 20) ties it with
`&D_80149D90` (20/7), and the tie goes the wrong way (s7/s8 swapped, 19 rows, parent still 4.5). Things that did not move it:
expression-statement reads (`parent;`, `(void)`, self-assign, `|=0`), dead compares against -1, a
separate loop-2 index, ternary/do-while loop forms, a merged `&&` bounds test, real callee prototypes, a K&R
definition, and extern declaration order.
Best next hypothesis: the original has fewer basic blocks somewhere (so more dead reads fit under the callee
cost), or a different shape for the parent==-1 / result=parent uses. Read `pretrace.sh` for parent's web
and check retail's block count against ours before trying more variants.

Layouts (best.c): Node104 {name[16]; f32 transform[12] @16; u32 flags @64; s16 sibling @68, child @70;
… ; f32 minimum[3] @80, maximum[3] @92}. Globals: pool pointer `D_80149B80`, current `D_80149D94`, first id
`D_80149D90` (s16), `state_word_a` (0x801174B4).

## What generalises

1. **A `v0` temp feeding an immediate op in a delay slot (`lw v0,…; jal; ori a3,v0,k`) marks an inlined
   getter's return value.** The plain expression gives `lw a3; ori t; move a3,t`. A static getter defined after
   the caller puts its stub at the caller-less `jr ra; nop` that follows.
2. **`s16` result vs `int` result decides whether uopt copy-propagates `r = param`.** With `int r` and `(s16)` on
   the assigned calls, the copy stays as a separate s-register web (retail's `sll s1,s4; sra; move s1`).
3. **The callee-saved cost grows with dead-read blocks** (+0.5 for each `if(x){}` and +0.25 for each `&&`). The
   `if(x){}` lever stops working when the lowest-priority s-register constant's tot is within that margin, because
   that web then splits and the frame shrinks. Check the lowest coloured constant's `tot` against `best` before
   stacking dead reads.
4. A single that waits only on a locked callee (tire_sound_update) can match at the first draft once that
   callee is locked. Re-derive arrays from the strength-reduced induction bases (`T - 4*k`) and do not reuse
   m2c offset accumulators.

## Files

- `../../matches/tire_sound_update.c` — the strict match (also `tsu/t0.c`).
- `differential_output/best.c` — best near-miss. `dout/` holds the variant history (b84 → d2 → v3/w1 → y1 getter → e1
  dead read → c*/f*/h*/k*/m*/n*/p*/q*/r*/s*/t*/u*/v*/z*/a*/kr* priority attempts).
- Builder scratch `~/rush2049/scratch/frontier/w12d` (trace toolkit installed with `--reuse wtk`).

No permission denials. Nothing committed, spliced or edited outside `cloud/work/frontier/w12d/`,
`cloud/matches/tire_sound_update.c` and my builder scratch.
