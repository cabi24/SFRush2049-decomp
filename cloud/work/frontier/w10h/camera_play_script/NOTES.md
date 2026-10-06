# camera_play_script (3520 B) — w10h

Start: w9h draft (`../../w9h/camera_play_script/body.c`). `body.c` here is that draft plus one
edit; `best.c` = w9h `cps/prefix.c` + `cps/types.c` + `body.c` for `blob_unit --with`.

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10h --jobs 2 score camera_play_script \
    --with cloud/work/frontier/w10h/camera_play_script/best.c --neighbours
  FAIL camera_play_script: 840 of 880 words differ; compiled body is 892 words, target 880; ...
  locked bodies that differ in this unit: 0
```
Strict word counts mean little at this distance (registers differ everywhere). Use the
register-normalised opcode diff (`tools/cops.sh`, w9h `ops.py` normalisation): w9h draft 151 rows,
`body.c` **132 rows**, and with the retail colouring forced on body.c **96 rows**.

## Edit that moved it

In the clip loop, `px = pv[k][0]` moves above `if (b != a)` (retail `mov.s $f2,$f26` precedes the
`bc1t`; px and b share `$f2`), with `den = a - b` named before it: 151 -> 132.

## Colouring (traced uopt in the unit, CDX proc 612 for this unit build)

Retail -> our web (body.c numbering): s0 n=w3, s1 &pv=w665, s2 col=w447 (param a2), s3 car=w62,
s4 e=w138, s5 edge-table IV=w143, s6 12=w657, s7 &D_80152818=w668, s8 952=w669, t5 -1=w670.
Ours colours s0..s5 only and **splits** col, &D_80152818, 952, -1, conv (w0/w692) and others.
The decline is arithmetic, not pressure: every unused callee-saved register costs 28.0 in this
procedure (`p1cost kind=callee cost=28`), and those webs have totalsave 18-21 (`p1dec ... totalsave=21
bestcost=28 decision=split`). Retail must give each of them > 28 (more weighted references, e.g. in
loops), or a cheaper first use of s6..s8. Forcing the ten colours
(`tools/force.sh P1 612 "p1:w3=c14,p1:w665=c15,p1:w447=c16,p1:w62=c17,p1:w138=c18,p1:w143=c19,p1:w657=c20,p1:w668=c21,p1:w669=c22,p1:w670=c12"`)
gives `multu a0,s8`, `addu v0,s7,..`, `li t5,-1` as in retail and drops to 96 rows.

## Structural residue left after forcing (each a source-shape question)

1. Both loops keep `<`: retail `sltu at,a3,s0` (k < n) and `slti at,s4,8` (e < 8); ours turns the
   e loop into `bne s4,8` (LFTR). `s32 k` vs `u32 k` made no difference.
2. `a - b` is computed *before* the `bc1t` in both clip tests (`sub.s $f22,$f0,$f2`, a coloured
   PRE temp in `$f22`), ours computes it in the taken block. `den = a - b; if (b != a)`,
   `if (a - b != 0.0f)`, `if ((den = a - b) != 0.0f)` and `den = a - b; if (den != 0.0f)` all compile
   identically (IDO folds `x - y != 0` to `x != y` and sinks the single-use difference).
3. Type dispatch: retail emits the type 4, 7, 3 blocks in that order and passes func_803914B4
   `(s8)(s8)pidx, (s8)pidx, (s8)<-1 web in t5>, (s8)pidx` - the -1 argument is converted at run time
   from the -1 web, so it is not a literal in the source (a variable holding -1, or `car->f6C4`
   after the `== -1` test), and arg 4 is a separate s8 variable (`sll a3,a0,24; sra; move a3`).
4. The final merge (`old == NULL` / `col->k < best`) is laid out in a different order
   (retail `beqz v0` first) and retail has `li a3,2` / `move a1,zero` where ours reloads a home.
5. Stores of dz into its home at 236 twice and dx/dz/da/db/dy kept in memory: FP colouring.

Frame: 632 vs 608. w9h attributed the +24 to extra uopt temps (not re-measured here). The
func_8008E408 notes describe how uopt sizes that area: every coloured expression web gets a slot,
so the count follows the colouring, and the frame should be re-checked after the colouring is fixed.
