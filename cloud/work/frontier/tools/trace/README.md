# Compiler-tracing toolkit (traced IDO 5.3 uopt / ugen / as1)

These scripts merge the per-lane tracing tools from w3a through w10h into one set. Each script prints the
compiler's decision behind a residual, so you do not have to guess it by building variants. Everything is
parameterised by **`TAG`** (your lane tag) and **`SCR`** (your builder scratch dir). No paths are hardcoded to a lane.
The scripts run on the Pi from any directory. The builder is `watchman2`. These are diagnostics only: forced or
oracle objects are never match evidence.

## Conventions

```sh
export TAG=w11x                              # your own tag: blob_unit --tag $TAG, builder unit/$TAG/stage
export SCR=rush2049/scratch/frontier/w11x    # optional; default rush2049/scratch/frontier/$TAG (relative to builder ~)
T=cloud/work/frontier/tools/trace
$T/install.sh ~/rush2049/scratch/frontier/w11x            # once per scratch dir (~2 min, 2 gcc jobs, niced)
$T/fidelity.sh NAME CAND.c                                # once: traced == stock, on your unit build
```

- Use your own `TAG` and your own `SCR`. Do not point either at another lane's scratch. `blob_unit` runs with
  `--jobs 2` (`JOBS=`).
- **Snapshots.** Every tracer works on a snapshot `$SCR/st_<LABEL>` (`merged` + `st`). `snap.sh LABEL` copies
  the last unit build of `$TAG`. `snap.sh LABEL FILE.c KEEP,LIST` stages a single-file `-O3` group instead
  (`cc -j`, `uld -kp`, `usplit`, `umerge`). A later `us.sh` rebuilds the unit but does not disturb a snapshot,
  so take one snapshot per variant (e.g. `LABEL=v3`).
- Procedures are named, not numbered. The tools look up the globalcolor ordinal (`CDX_PROC`) themselves (W5D
  `ordinal` record), and the ugen ordinal from the listing. You no longer need `pdiff.sh` to find an ordinal.
- Pi-side outputs go to `$LOUT` (default `build/trace/$TAG/`). Builder-side outputs stay in `st_<LABEL>/`.

## What each tool answers

| Question | Tool | Notes |
|---|---|---|
| How far off is it, and what kind of difference? | `us.sh NAME FILE.c [FILE.c…]` | One line per file: unit verdict, aligned rows at three strictness levels (`words` / `ops` = mnemonic only / `norm` = registers, offsets and immediates normalised), frame want/got. `UARGS="--internal X --keep Y"`; `DIFF=1 UD=--all` adds the diff. |
| Aligned diff of any object | `udiff.py NAME [--obj O] [--all] [--ops\|--norm\|--nosp] [--summary]` | Default object `build/blob_unit/$TAG/unit.o`. `--nosp` hides home-slot shifts. |
| Who gets which register, and why? (colouring) | `ctrace.sh NAME LABEL [CAND.c]` | One line per web: `save` (priority), `nocs`, `tot` (totalsave = net benefit), `best` (bestcost), `numintf`, decision, final register, web type/bb. Raw `[CDX]` records (`p1cand`, `p1cost`, `p1dec`, `p1color`, `webdetail`) are saved under `$LOUT`. `RAW=1` prints them. |
| Would the code match with these colours? (oracle) | `force.sh LABEL NAME SPEC [udiff args]` | `CDX_FORCE` on NAME, then stock ugen + as1 and the aligned diff. Prints the applied (`forced=`) and declined records. |
| Which procedures did my change touch? | `pdiff.sh LABEL1 LABEL2` | Ordinals whose decisions differ (for IPA/caller effects). |
| Which expressions did PRE/CSE delete or insert, and which constants/addresses got live ranges? | `pretrace.sh LABEL NAME` | uopt level-3 listing + W5D trace, formatted by `lib/prereport.py`. The bit number is the web number. `lib/precmp.py A B NAME` compares two runs by expression text. |
| What if PRE had not removed these occurrences? | `[FORCE=SPEC] [VEC=d] nopre.sh LABEL NAME BITS [udiff args]` | Oracle. Clears the bits from the delete vector. |
| Where do spill-temp homes and local-area size come from? | `spill.sh LABEL NAME`, `spcensus.sh NAME [OBJ]` | `f_spilltemps` slot per coloured web, home reuse, block conflicts, area growth per uopt function. The census compares retail and ours sp offsets. |
| Which statement pops which ugen temp (evaluation order)? | `[SCHED=1] ugt.sh LABEL NAME` | Traced ugen: `DKWB-FREELIST ADD/REMOVE reg=N emitted=I line=L`. `SCHED=1` adds `DKWB-EMIT-V1` (every instruction in ugen emission order, with its line). Then NAME's ugen `-l` listing. |
| Why did as1 schedule it this way? | `as1t.sh LABEL NAME`, then `asm.sh LABEL EDITED.s NAME` | Traced `as1 -R`: per block the DAG (`Node N: inst, lineno`, `before/aftercycles/maxhazard`) and every `Picking node` with the ready list. Edit the listing and reassemble to test whether a residual comes from as1 or from earlier passes. |

### Reading the colouring trace (lane findings, with provenance)

- **Phase 1** (`p1…`) takes webs by priority. `p1cand` lines show the highest-`save` search, and `p1cost` gives
  the cost of each register (`kind=caller|callee`). The decision is `split` when `totalsave` (net) does not
  beat `bestcost` (workbench law L28). Webs left for **phase 2** (`p2…`, when present) are coloured in
  **web-number (first appearance) order**, each taking the lowest free register. Source: w10b draw_text. In
  that procedure webs with `numintf` ≥ 22 were in p1 and the rest in p2, so one extra web in the loop nest (an
  index cursor instead of a walking pointer) moved loop invariants from s-registers to a/t registers. Many
  procedures have only p1 decisions (func_80109A60, steering_sensitivity).
- **Ties on `save`** go to the lower web number (first appearance). To move a web's first appearance without
  moving its code, split the assignment (`x = a; x += b;`). Source: w9a, w10b.
- **Callee-saved cost is a per-procedure constant** (`p1cost kind=callee cost=`, e.g. 28.0 in
  camera_play_script and 13.75 in func_8008E408). A web whose `totalsave` is below it is split whatever its
  rank. Compare `tot` with `best` before you hunt for priority inversions. Source: w10h.
- `p1` and `p2` web numbers are **disjoint spaces**: `p1:w9` and `p2:w9` are different webs.

### Forcing colours and reading a declined force

`SPEC` is comma-separated with no spaces: `p1:w74=c3` (phase 1 web 74 → colour 3), `p2:w60=c12`, or `p1:w80=s`
(force the split path). `force.sh` prints one `p1color … forced=N` per applied colour. A force the pass cannot
apply is **declined**. The natural decision then stands, and the pass prints (with or without `CDX_LOG`):

```
[CDX] force_declined phase=p1 site=dec proc=965 web=2 color=1 reg=v0 forbidden=0x7e00000000000000
```

`forbidden` is two 32-bit masks. Colour c is bit `31-c` of the first word, so `0x7e000000` means c1–c6
(v0, v1, a0–a3) are already forbidden to this web. Some web that interferes with it already holds that colour.
To find that web, force each earlier web holding that colour somewhere else (`pN:wM=c9`); the one that frees
the colour is the interferer (w10d found `idx + 1` and the loop index this way). A force on a *split* web is
silently declined (w10e). Once forced colours give `differing rows 0`, the whole residual is colouring. If rows
remain, the rest is structure, the temp ring or as1 (w10b: "force-oracle first").

### Colour codes (globalcolor colour → register)

| colour | int register | provenance |
|---|---|---|
| c1, c2 | v0, v1 | force-and-diff (workbench) |
| c3–c6 | a0–a3 | force-and-diff (c3–c5 workbench; c3/c2 swap reproduced here, smoke test) |
| c7–c12 | t0–t5 | coloroffset decode; c10/c11 = t3/t4 reproduced here (smoke test) |
| c13 | **unconfirmed**: occurs naturally (func_80109A60 w270, type 1). Forcing an int web to c13 emitted `ra`, the same as c23 | wtk probe, 2026-10-06 |
| c14–c22 | s0–s8 (c22 = s8/fp) | decode; c14–c22 forced in w10e/w10h |
| c23 | ra | decode; forcing an int web to c23 emitted `ra` (wtk probe) |

| colour | FP register | provenance |
|---|---|---|
| c24, c25 | $f0, $f2 | workbench L27 (T1); w9d `p2:w27=c25` → $f2; w10d `c25` → $f2 |
| c26, c27 | $f12, $f14 | L27; w10d `c26` → $f12 |
| c28, c29 | $f16, $f18 | L27 |
| c30, c31 | $f20, $f22 (callee-saved) | w9a forced height=c30, y=c31 to retail's $f20/$f22 (not yet in the workbench table) |
| — | $f4–$f10 | ugen's local FP temp ring; never a colour (L38) |

The pass prints `reg=?` for FP colours. `sum.sh` fills in the names from this table. Forcing a colour of the
wrong class gives meaningless output (an int web forced to c30 emitted `s4`).

Integer temp ring (ugen, not coloured): least-recently-freed order, starting `t6 t7 t8 t9 t0 … t5`. The
`DKWB-FREELIST` `reg=` value is the MIPS register number (8–15 = t0–t7, 24/25 = t8/t9, 2/3 = v0/v1).

### Spill-temp layout (w10h, w7a)

`f_spilltemps` (uopt, not ugen) gives a 4-byte slot to **every coloured expression web** (`kind=4`), spilled
or not. It takes them in itable (first-occurrence) order and reuses a same-size slot only when the webs share no
basic block (`CONFL`). The area starts at the merged `Udef` size, and umerge rounds the frame to 8 after
inlining. So a family of homes off by a constant means one more or one fewer coloured expression web earlier,
or a different merged-frame start. It does not mean a missing declaration. In `spill.sh`, `AREA
f_readnxtinst … area=N` is the cfe locals and the `AREA f_spilltemps` steps are the homes.

## Binaries and patches (`install.sh SCRATCH [--reuse OTHER]`)

Instrumentation runs on the Pi (it needs the vendored workbench). gcc runs on the builder
(`gcc -std=c11 -Os -fno-strict-aliasing`, two jobs, `nice`). The sources are the recompiled C in
`~/rush2049/scratch/ci/tools/ido-static-recomp/build`, pinned by sha256. Every lane since w3a used this same
`uopt.c`. The stock passes compared against are in the toolkit IDO `~/rush2049/cache/toolkits/796ae99a…/ido`
(linked as `SCRATCH/bin/ido`).

| binary | built from | patch chain | env switches (all off by default) |
|---|---|---|---|
| `bin/uopt` | `uopt.c` sha256 `627eff8f…` | `patches/instrument_w5d.py` = workbench `instrument-uopt --profile globalcolor` (w3a) **+** w5d PRE hooks; then `patches/patch_spill.py` = w6a `patch_area2`/`patch_sp` + w7a `patch_home2`/`patch_confl`/`patch_gt2` + w10h `patch_tmp` + w10h's hand-made `spill web` hook, now gated | `CDX_LOG CDX_OUT CDX_PROC CDX_DETAIL_WEB CDX_FORCE`; `W5D_LEVEL W5D_PROC W5D_OUT W5D_NOPRE W5D_NOPRE_VEC` (+ `-l FILE`); `SPLOG`; `TMPLOG` |
| `bin/ugen` | `ugen.c` sha256 `4079660f…` | `workbench instrument-ugen --emit-provenance` (w10b used the same without `--emit-provenance`) | `DKWB_UGEN_TRACE=1` (call/FREELIST/PROC), `DKWB_UGEN_SCHED=1` (EMIT-V1) |
| `bin/as1` | `as1.c` sha256 `4d7a9714…`, unchanged | none. The libc's `printf` is made real so that `-R` prints instead of aborting (w4a) | `-R` command-line flag |
| `libc_impl.o` (all three) | `libc_impl.c` `7dd4bac0…` | `patches/ecvt.patch.py` (real ecvt/fcvt for the uopt listing, w5d) + `patches/as1_printf.patch.py` (w4a) | — |

`bin/MANIFEST` records the sha256 of every source, generated source and binary. `--reuse OTHER` copies an
existing install and checks it against OTHER's MANIFEST, which takes seconds. Two differences from the lane
copies: the AREA/SPILLTEMP/HOME/CONFL/GETTEMP prints are now gated by `SPLOG` (w7a's `uopt3` always printed
them), and AREA lines name the original uopt function.

**Fidelity** (`fidelity.sh`, run 2026-10-06 on the whole `wtk` unit with func_80109A60 = w10b
`best_named_msize.c`). Each pass ran stock, traced with tracing off, and traced with everything on:

```
IDENTICAL uopt opt: stock vs traced (tracing off)
IDENTICAL uopt opt: stock vs traced (CDX_LOG all procs + W5D_LEVEL=3 listing + TMPLOG + SPLOG)
IDENTICAL uopt symbol table: stock vs traced (all on)
  trace volume: cdx 297548 lines, w5d 130171, listing 325854, spill/tmp 48558
IDENTICAL ugen gen: stock vs traced (tracing off)
IDENTICAL ugen gen: stock vs traced (DKWB_UGEN_TRACE=1 DKWB_UGEN_SCHED=1)
IDENTICAL ugen symbol table: stock vs traced (on)
  trace volume: 241888 FREELIST/EMIT/PROC records
IDENTICAL as1 object: stock vs traced (no -R)
IDENTICAL as1 object: stock vs traced (-R scheduler trace)
  trace volume: as1 -R 1329389 lines
IDENTICAL stock replay of the snapshot vs the unit's unit.o
fidelity: 9 identical, 0 different
```

## Smoke test (fresh scratch `~/rush2049/scratch/frontier/wtk`, 2026-10-06)

This reproduces w10b's result for func_80109A60: four forced colours turn the `best_named_msize.c` residual
into zero rows.

```
$ T=cloud/work/frontier/tools/trace; export TAG=wtk
$ $T/install.sh ~/rush2049/scratch/frontier/wtk          # MANIFEST: bin uopt 835893a1…, ugen f82b9510…, as1 98398910…
$ $T/fidelity.sh func_80109A60 cloud/work/frontier/w10b/func_80109A60/best_named_msize.c
  FAIL func_80109A60: 59 of 317 words differ                # (fidelity block above)
$ $T/snap.sh msize
$ $T/ctrace.sh func_80109A60 msize
func_80109A60: ordinal 965, 28 p1 + 0 p2 decisions; raw build/trace/wtk/msize.func_80109A60.cdx
p1 w74   save=3.000000  nocs=2   tot=6.000000  best=0.000000 numintf=12  color    -> v1   type=3 raw10=0xffffffe8 bb=9
p1 w79   save=3.000000  nocs=2   tot=6.000000  best=0.000000 numintf=16  color    -> a0   type=3 raw10=0xffffffdc bb=9
p1 w14   save=1.000000  nocs=2   tot=2.000000  best=0.000000 numintf=16  color    -> t3   type=4 raw10=0x36000610 bb=0
p1 w67   save=1.000000  nocs=7   tot=7.000000  best=0.200000 numintf=23  color    -> t4   type=3 raw10=0xfffffff0 bb=8
  … (28 lines)
$ $T/force.sh msize func_80109A60 "p1:w74=c3,p1:w79=c2,p1:w14=c11,p1:w67=c10"
proc func_80109A60 ordinal 965
[CDX] p1color phase=p1 proc=965 web=74 sym=74 color=3 reg=a0 forced=3
[CDX] p1color phase=p1 proc=965 web=79 sym=79 color=2 reg=v1 forced=2
[CDX] p1color phase=p1 proc=965 web=14 sym=14 color=11 reg=t4 forced=11
[CDX] p1color phase=p1 proc=965 web=67 sym=67 color=10 reg=t3 forced=10
want 317 words, got 317; differing rows 0 (words); frame 40/40; unverified 0 unresolved 0
```

The web numbers are the ones w10b recorded (its proc ordinal was 955 in that unit; here it is 965, found by
name). The unforced object has `differing rows 58`. Other checks run on the same install:

```
$ $T/us.sh func_80109A60 …/w10b/func_80109A60/best.c …/best_named_msize.c
best.c: FAIL func_80109A60: 26 of 317 words differ | words 26 ops 0 norm 0 | frame 40/40          (= w10b's 26)
best_named_msize.c: FAIL func_80109A60: 59 of 317 words differ | words 58 ops 3 norm 3 | frame 40/40
$ $T/force.sh msize func_80109A60 "p1:w2=c1,p1:w6=c14" --summary                       # a declined force
[CDX] force_declined phase=p1 site=dec proc=965 web=2 color=1 reg=v0 forbidden=0x7e00000000000000
[CDX] force_declined phase=p1 site=color proc=965 web=2 color=1 reg=v0 forbidden=0x7e00000000000000
[CDX] p1color phase=p1 proc=965 web=6 sym=6 color=14 reg=s0 forced=14
$ $T/snap.sh gsteer cloud/work/frontier/w9a/groups/steering_sensitivity/group.c steering_sensitivity,traction_control,func_800A61B0,math_utility
$ $T/ctrace.sh steering_sensitivity gsteer      # w9a's matched group: height/y webs land in retail's $f20/$f22
p1 w52   save=2.428571  nocs=7   tot=17.000000 best=9.000000 numintf=14  color    -> $f20 …
p1 w72   save=2.000000  nocs=5   tot=10.000000 best=9.000000 numintf=14  color    -> $f22 …
$ $T/us.sh func_8008E408 cloud/work/frontier/w10h/func_8008E408/best.c; $T/snap.sh e408; $T/spill.sh e408 func_8008E408
best.c: FAIL func_8008E408: 22 of 386 words differ | words 30 ops 0 norm 0 | frame 256/256        (= w10h's 22)
[TMP] spill web=5 kind=4 size=4 temp=0 off=-212   … web=41 temp=1 off=-216 … web=208 temp=5 off=-232
AREA f_readnxtinst:93590 proc=func_8008E408 area=208   then f_spilltemps 212 … 232
$ $T/ugt.sh msize func_80109A60     # ugen proc ordinal 1030 (procs in listing: 1077, PROC BEGIN records: 1077)
$ $T/as1t.sh msize func_80109A60    # as1-identical (traced vs stock); 9857-line trace
$ $T/asm.sh msize build/trace/wtk/fn_msize.func_80109A60.s func_80109A60 --summary
want 317 words, got 317; differing rows 58 …    (the listing alone reassembles to the unit's rows)
$ $T/pretrace.sh msize func_80109A60 ; $T/nopre.sh msize func_80109A60 33 --summary   # both ran
```

## Files

`install.sh`, `fidelity.sh`, `us.sh`, `udiff.py`, `snap.sh`, `ctrace.sh`, `sum.sh`, `force.sh`, `pdiff.sh`,
`pretrace.sh`, `nopre.sh`, `spill.sh`, `spcensus.sh`, `ugt.sh`, `as1t.sh`, `asm.sh`. The Pi side shares
`lib/env.sh`. On the builder, `remote/tk.sh` runs every snapshot mode and is re-copied to `$SCR/tk/` on each
call; `remote/build.sh` and `remote/fidelity.sh` are the other builder scripts. `lib/prereport.py`,
`lib/cmparse.py` and `lib/precmp.py` are w5d's report code, unchanged. `patches/` holds the source patches
listed above.

**Not consolidated** (still in the lane dirs): standalone `score.py fn/group` wrappers (`sc.sh`, `grp.sh`,
`fd.sh`, `gd.sh`, `full.py`, `batch.sh`/`bscore.py`, `fdb.sh`). They are scoring, not tracing, and they need a
lane copy of `tools/cloud` on the builder. Lane-specific drivers are left out as well (`vrun.sh`/`frun.sh`,
w10h `cu.sh`/`st.sh` for camera_play_script/func_8008E408, w9h `trq.sh`). w10h's ucode `Udef` dumpers
(`udump.py`/`getu.sh`/`udef.sh`) are not ported: they read the unit stage directly and are a different
question (frame `Udef`, not tracing). Also left out: `udiag.sh` (use `tools/workbench.py diagnose`), w5d
`redundancy_scan.py`, w10b `poison.py`, `rd.py` (a retail data dump). w4a's whole-unit `as1 -R` log is replaced
by the per-function `as1t.sh`, because the trace has no procedure delimiters.
