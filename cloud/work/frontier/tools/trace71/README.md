# trace71 — traced IDO 7.1 uopt / ugen / as1 (boot-tail register allocation and scheduling)

The 5.3 toolkit (`../trace/`) traces the game and overlay code. The boot tail (the 18 `lib_*` ROM TUs pinned in
`rom_compilers.json`) builds with IDO 7.1 at `-O2`. These scripts give a 7.1 function the same answers: who gets
which register and why, where its frame slots come from, what ugen's temp ring did, and how as1 scheduled it. Built
by w22c (wave 22). Diagnostics only: a forced object is never match evidence.

## Conventions

```sh
export SCR=rush2049/scratch/frontier/<lane>        # builder scratch root, relative to the builder's ~
T=cloud/work/frontier/tools/trace71
$T/install71.sh ~/rush2049/scratch/frontier/<lane> --reuse ~/rush2049/scratch/frontier/w22c   # seconds
$T/fidelity71.sh                                   # once: traced == stock on all 18 boot-tail TUs
```

- The scratch root needs `include/`, `tools/conveyor/` (for `reduced_tu`) and `src/rom/` synced from the current tree,
  and `tgt/<FN>.o` (the reloc-aware inventory target, `python3 cloud/work/frontier/w17k/fetch_targets.py DIR FN...`
  on the Pi, then rsync) for the row diffs.
- Every tool takes a **source file and a function name**: it compiles the reduced TU (`reduced_tu(SRC, FN)`, what
  `lock add` compiles) with the SRC's `/* flags: */` line (default `-g0 -O2 -mips2 -G 0 -non_shared`) plus
  `-Wab,-r4300_mul`, through the traced passes in `$SCR/t71/ido`. No unit build and no snapshot step (7.1 is
  per-function `-O2`). `FLAGS=...` overrides the flags. The source is copied to `$SCR/t71/in/`; work files are in
  `$SCR/t71/run/<FN>/` (`t.rows`/`m.rows` = target/ours `objdump -dr` rows).
- The procedure is selected **by name** (`CDX_FN`, new in this build): no ordinal lookup.
- Pi-side outputs go to `$LOUT` (default `build/trace71/<scratch>`).

## What each tool answers

| Question | Tool | Notes |
|---|---|---|
| How far off is it? | `obj71.sh SRC FN` | Plain compile, `rows: want W got G; differing rows N` against `tgt/FN.o`. `DIFF=1` prints them. |
| Who gets which register, and why? | `ctrace71.sh SRC FN` | One line per colouring decision (`sum71.sh`): phase, web, `save`, `nocs`, `tot` (net benefit), `best` (cheapest register cost), `numintf`, decision, final register, and what the web is: `var@OFF/SIZE` (a declared variable: OFF is cfe's virtual-frame offset, locals laid out downward in declaration order with natural alignment, the first `int` at -4; parameters positive), `expr op=0xNN` (an expression web), `addr` (an address kept in a register, e.g. `&D_x`), `const V` (a constant kept in a register). Raw records: `RAW=1`; the `[CDX] calleecost` line gives the procedure's callee-saved cost. `XENV="CUPLOG=1"` adds `[CUP]` lines (below). `XENV="CDX_DETAIL_WEB=N"` lists web N's interference neighbours (`intf … other= assigned=`, the colour each had when N was decided). |
| Would it match with these colours? | `force71.sh SRC FN SPEC` | `CDX_FORCE` oracle (`p1:w59=c12,p1:w3=c2,p1:w9=s`), then the row diff. Declined forces print `force_declined … forbidden=` (same decode as 5.3). |
| Why is this web caller-saved / callee-saved / split? | `XENV=CUPLOG=1 RAW=1 ctrace71.sh` | `[CUP] unit web= reg= bb= run=` per live unit, `[CUP] transit … bb=` for blocks the web only passes through, `[CUP] total … cost=`: the caller-saved cost f_cupcosts computes for each register. |
| Where do the frame slots come from? | `spill71.sh SRC FN` | `AREA f_readnxtinst … area=N` = cfe's local area; `SPILLTEMP`/`HOME`/`CONFL`/`SPILLWEB web= temp= off=` = the 4-byte home f_spilltemps gives every coloured expression web, reused only between webs that share no block; the `AREA f_spilltemps` steps are the growth. |
| Which statement pops which ugen temp? | `[SCHED=1] ugt71.sh SRC FN` | `DKWB-FREELIST ADD/REMOVE/FREE/ALLOC_GP_RESULT reg=N … line=L` (ring = least-recently-freed; REMOVE = registers coloured somewhere in the procedure, never temps), `SCHED=1` adds `DKWB-EMIT-V1`; then FN's ugen listing (`cc -S`). |
| Why did as1 schedule it this way? | `as1t71.sh SRC FN` | The real compile with `as1 -R` (`-Wb,-R`): per block `Node N: inst, relocation, lineno` (source lines), `before/aftercycles/maxhazard`, and every `Picking node`. Checks the `-R` object equals the plain one. The reduced TU's other procedures come first in the log (no delimiters). Also fetches FN's listing for `asm71.sh`. |
| Is the residual as1's? | `asm71.sh EDITED.s FN` | Assemble an edited listing (stock 7.1 as0/as1, `-r4300_mul`), row diff. A `cc -S` listing has no `.loc` source lines, so as1 tie-breaks can differ from the real compile: calibrate with the unedited listing first. |

## What the 7.1 allocator does (measured with this build, w22c)

- **Decision**: p1 colours a web when `tot > best`, else splits it (to memory). `best` = the cheapest register:
  caller-saved registers first (c1 v0 … c13), then callee-saved (s0–s8); a later register replaces the best only if
  strictly cheaper, so **a caller/callee tie goes to the caller-saved register**.
- **Callee-saved cost** (`[CDX] calleecost`) = `max(4, min(60, nblocks × 0.25))` per procedure (path `opt=0`;
  nblocks = basic blocks at globalcolor time), i.e. 4.0 for any procedure under 17 blocks. **Adding blocks raises it**:
  func_80010E80 needed 20 blocks (cost 5) so its rounded length stayed in v1 like retail; a natural 4-trip loop
  supplied the 4 blocks.
- **Caller-saved cost** (`[CUP]`): per live unit, about one per call the web lives across plus one per reload after
  a call; transit blocks (live through, no occurrence) that end in a call add one each.
- **One web per variable**: disjoint uses of one C variable (e.g. a temporary reused for two values) form ONE web
  with the summed priority (5.3 split them). Reusing a variable to save a frame slot therefore changes the colouring
  (func_8001A270); a CSE'd expression (`keymap[(u8)(key & 127)]` written at each use) takes no cfe slot but keeps the
  same web.
- **Frame**: cfe area = the declared locals (declaration order, natural alignment) plus cfe's own temporaries below
  them (e.g. a `switch` selector temp: a `var@` web below the last declared local); then one 4-byte home per coloured
  expression web (f_spilltemps). A frame 8 bytes off is usually one 4-byte local too many — `spill71.sh` shows which.
- Colour codes as 5.3: c1 v0, c2 v1, c3–c6 a0–a3, c7–c12 t0–t5, c14–c22 s0–s8 (forced and checked on 7.1 objects:
  c2, c4, c5, c6, c7, c9, c12, c3/c4 swaps). FP c24–c29 = $f0 $f2 $f12 $f14 $f16 $f18 (A71 table; not exercised).

## Fidelity (the identity gate)

`fidelity71.sh` compiles all 18 boot-tail TUs (ROM TU flags) with four compilers and compares the objects byte for
byte: `rel` = the pinned release 7.1 (toolkit 4586d230 `ido/`, decompals v1.2 release binaries), `reb` = our unpatched
rebuild of uopt/ugen/as1 from the v1.2 generated C (gcc 15 -Os), `off` = traced passes with tracing off, `on` = every
trace on (CDX_LOG all procedures + CDX_DETAIL_WEB=all, SPLOG, CUPLOG, DKWB_UGEN_TRACE + SCHED, as1 -R):

```
fidelity71: 54 identical, 0 different (18 TUs x 3 comparisons against the release 7.1)
  lib_11640 trace volume: cdx 9602 ugen 263335 spill … as1 -R 41131
```

LOCK-line identity (w19a `check.py`, `--flags "-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul"`) on 13 locked boot-tail
sources, stock toolkit vs a toolkit whose `ido/` is the traced dir with every trace on: 13/13 identical
(`cloud/work/frontier/w22c/logs/lock_identity.log`).

## Binaries and patches (`install71.sh SCRATCH [--reuse OTHER]`)

| file | from | patch chain | switches (all off by default) |
|---|---|---|---|
| `t71/ido/uopt` | v1.2 `build/7.1/uopt.c` sha256 `0f2c66d2…` | `patches/instrument71_uopt.py` (the workbench 5.3 globalcolor profile ported to the 7.1 labels: same frame slots and live-range offsets, globals 0x1001eb70→0x1001c4d0, 0x10021c74→0x1001c530; plus procedure names, `CDX_FN`, `calleecost`, `CUPLOG`) + `patches/spill71.py` (patch_spill.py ported: area 0x1001c4b4→0x1001c31c, name 0x1001eb00/len 0x1001c330) | `CDX_LOG CDX_OUT CDX_FN CDX_PROC CDX_DETAIL_WEB CDX_FORCE CUPLOG SPLOG` |
| `t71/ido/ugen` | v1.2 `ugen.c` `44c24d9d…` | `workbench instrument-ugen --emit-provenance` + `patches/ugen71_addrs.py` (cursor/line globals 0x10018e70/78/00 → 0x10021238/40/20, read from f_emit_*/f_warning) | `DKWB_UGEN_TRACE DKWB_UGEN_SCHED` |
| `t71/ido/as1` | v1.2 `as1.c` `19494fb0…`, unchanged | none: v1.2's libc already implements printf, so `-R` prints | `-Wb,-R` |
| libc | v1.2 `libc_impl.c` | `../trace/patches/ecvt.patch.py` | — |
| `t71/reb/` | the same three passes unpatched (control) | — | — |

`isr71/` is a shallow clone of decompals/ido-static-recomp v1.2 (with its IRIX 7.1 binaries); `make setup` +
`recomp.elf` regenerate the C, pinned by sha256. `--reuse` copies `t71/{ido,reb,MANIFEST}` and checks the binary
hashes. Not ported (5.3-only addresses): W5D PRE listing/nopre oracle, GETTEMP flags, [TMP] wrappers. The webdetail
`line` field reads a 5.3 offset and prints -1/2 under 7.1 (ignore it; `bb` is right).

## Files

`install71.sh`, `fidelity71.sh`, `obj71.sh`, `ctrace71.sh`, `sum71.sh`, `force71.sh`, `spill71.sh`, `ugt71.sh`,
`as1t71.sh`, `asm71.sh`; `lib/env71.sh` (Pi side); `remote/tk71.sh` (every mode, re-copied each call),
`remote/build71.sh`, `remote/fidelity71.sh`; `patches/` as above. Tests: `tests/conveyor/test_trace71.py`.
