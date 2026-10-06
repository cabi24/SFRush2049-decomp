# w8a tools

Copies of the w5b/w4a/w3a scripts retargeted to the w8a builder scratch (`~/rush2049/scratch/frontier/w8a`,
unit tag `w8a`), plus group-level variants:

| script | use |
|---|---|
| `gd.sh SRC.c FN [--mnem|--all]` | aligned diff of FN compiled as a single-file `-O3` group (keep = the stand-in callers; `KEEP=` overrides) |
| `grp.sh DIR` | `score.py group DIR` on the builder |
| `gt.sh SRC.c LABEL PROC [KEEP]` (+ `gt_remote.sh`) | stage the group's merged ucode into `st_LABEL`, run the w5d-traced uopt (level-3 PRE listing, regcand, colouring trace) for PROC; report in `runs/LABEL/report.txt` |
| `gforce.sh LABEL NAME SPEC` | colouring oracle (`CDX_FORCE`) on a `gt.sh` snapshot, then ugen + as1 and the aligned diff |
| `as1t_remote.sh LABEL` | on the builder: stock uopt/ugen on `st_LABEL`, stock and traced as1 (`-R` scheduler trace in `st_LABEL/as1r.log`); checks the objects are identical. The traced as1 is `~/rush2049/scratch/frontier/w8a/as1/as1` (built from the recompiled as1.c + w4a's `patch_as1_printf.py`) |
| `asmt.sh FILE.s NAME` (+ `asm_remote.sh`) | assemble a hand-edited ugen listing (from `o3s.sh`) and diff against retail |
| `patch_area2.py SRC DST` | instruments the recompiled `uopt.c`: prints every change of the procedure's local-area size (`0x1001c4b4`) with the writing uopt function (`f_readnxtinst` = cfe locals, `f_spilltemps` = spill-temp homes) and every `f_gettemp` call |
| `patch_sp.py SRC DST` | adds a print of every expression bit `f_spilltemps` considers (class 1 int, 2 fp) |

Build of the instrumented uopt (builder): copy `header.h libc_impl.[ch] helpers.h` from
`~/rush2049/scratch/ci/tools/ido-static-recomp`, `python3 patch_area2.py …/build/uopt.c uopt.c`, then
`gcc -c libc_impl.c; gcc -o uopt uopt.c libc_impl.o -lm` (output is byte-identical to the stock uopt; the
prints go to stderr).

## Added by w8a

| script | use |
|---|---|
| `vrun.sh NAME...` | build `runs/NAME/group.c` from `runs/NAME.c` (as the Input_ProcessGameplayPad part, via `dev/mk.sh`) and print the aligned diff count (`gd.sh` is now parallel-safe: per-PID candidate names) |
| `frun.sh NAME...` | same for a `runs/f/NAME.c` variant of `dev/part_f058.c`: func_8009F058's score, our frame size and our `t0` slot |
| `sp.sh SRC.c LABEL` (+ `sp_remote.sh`) | area / spill-temp / home trace of a group with `uopt3` (below); log in `runs/LABEL/sp.log` |
| `patch_home2.py` | adds `HOME proc= bit= home= reused= size=` (one line per temp homed by `f_spilltemps` phase 2) |
| `patch_confl.py` | adds `CONFL proc= bit= other= blk=` (temp `bit` meets lower-numbered homed temp `other` in block `blk`'s set at +0x15c) |
| `patch_gt2.py` | extends `GETTEMP` with the chosen home index and the free/busy flags of the home list |

`uopt3` on the builder (`~/rush2049/scratch/frontier/w8a/uopt3/uopt`) = w6a's `patch_area2.py` + `patch_sp.py` +
the three patches above, built with `gcc -std=c11 -O1 -fno-strict-aliasing -I. -DIDO53 uopt_g.c libc_impl.o -lm`
(prints to stderr only). Bit numbers match the `report.txt` of `gt.sh` for the same source.
