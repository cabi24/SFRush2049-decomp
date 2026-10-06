# w10h results: last mile on the two closest wave-9 misses

Neither function closed. Both are scored in the whole-program unit (all callees locked, real
callers). No match was produced, so there is nothing to integrate: no splice, no group, and no
unit_overrides changes.

| Function | Bytes | State | Flags | Scorer output (exact) |
|---|---:|---|---|---|
| func_8008E408 | 1544 | 22 of 386 words differ: only spill-temp homes (unchanged from w9c best) | `-g0 -O3 -mips2 -G 0 -non_shared` (unit) | `python3 -m tools.conveyor.pipeline.blob_unit --tag w10h --jobs 2 score func_8008E408 --with cloud/work/frontier/w10h/func_8008E408/best.c --neighbours` -> `FAIL func_8008E408: 22 of 386 words differ` / `locked bodies that differ in this unit: 0` / `blob_unit score: 0/1 equal` |
| camera_play_script | 3520 | 840 of 880 words differ (892 compiled); normalised opcode diff 151 -> 132 (96 with the retail colouring forced) | same (unit) | `... score camera_play_script --with cloud/work/frontier/w10h/camera_play_script/best.c --neighbours` -> `FAIL camera_play_script: 840 of 880 words differ; compiled body is 892 words, target 880; +0x208: .rodata+0x35c: retail words encode 0x01284282, outside the i…` / `locked bodies that differ in this unit: 0` |

Files: `func_8008E408/{best.c,notes.md,variants/}`, `camera_play_script/{body.c,best.c,NOTES.md}`,
`cps/` (variants P1-P5, K2 and traces), `tools/`.

## func_8008E408: the residual is traced to uopt `f_spilltemps`, and it is not a declaration problem

Details are in `func_8008E408/notes.md`. In brief:
- The sp+24.. temp homes are allocated by **uopt** (`f_spilltemps`), not ugen. Every *coloured
  expression web* gets a 4-byte slot, whether or not it ever spills. Webs are taken in itable
  (first-occurrence) order, with greedy reuse of a same-size slot that has no shared basic block.
  The area starts at the merged `Udef` size.
- **umerge always rounds the merged frame to 8** when it inlines. So retail's homes (all eight
  webs exactly +4) cannot come from padding locals. They need merged 200 (one 8-byte quantum
  fewer than w9c's padding) **plus one extra coloured expression web** that comes before car in
  itable order and is live everywhere. Simulating that web gives every retail home exactly.
- A long-lived parameter expression in entry and exit empty-body `if`s produces exactly that temp
  shape (web 2, car pushed to the second slot), but it keeps its def and its saves in the code. No
  zero-code way to make such a web was found (~50 builds). The next hypothesis is in the notes.

## camera_play_script: one structural fix, the colouring decline explained, and the residue listed

Details are in `camera_play_script/NOTES.md`. Moving `px = pv[k][0]` above `if (b != a)` (with
`den = a - b`) is worth 19 normalised rows. The callee-saved shortfall is arithmetic, not
pressure: every unused s-register costs 28.0 in this procedure, and col / &D_80152818 / 952 / -1
have totalsave 18-21, so they split. Forcing the ten retail colours lands `multu s8`, `s7`, `t5=-1`
as in retail. The remaining structural items are listed with retail evidence: `<` loop tests kept,
`a - b` as a coloured PRE temp before the branch, dispatch block order with a run-time -1 argument,
and the order of the final merge block.

## What generalises

1. **uopt owns the "spill temp" area, and it is sized by colouring, not by spilling.**
   `f_spilltemps` gives a slot to every coloured expression web (kind 4) in itable order. Slots are
   reused only between same-size webs with no common basic block. Loop-local address webs that
   never spill still take slots (the holes in both retail and ours). A family of temp homes off by a
   constant therefore means "one more or one fewer coloured expression web earlier in itable order",
   or a different merged-frame start. It does not mean a missing declaration.
2. **umerge rounds the frame to 8 after inlining** (`align8(locals + inlined areas)`; every inlined
   helper adds 8 here). A function that inlines anything can never have its temp area start at
   4 mod 8. Removing one 4-byte local moves the frame by 8 or not at all.
3. **The callee-saved threshold is a per-procedure constant.** Taking an unused s-register costs
   the same amount for every web (28.0 here, 13.75 in func_8008E408). A web whose
   `totalsave` is below it is split however its priority ranks. Compare `totalsave` against
   `bestcost` in `p1dec` before searching for priority inversions.
4. **Empty-body `if` on an arithmetic expression at entry and exit** creates a low-numbered,
   function-long PRE web (useful as a temp-slot or priority lever). Its def and its call-crossing
   saves stay in the code. A comparison (`x >= 8`) creates no web at all.
5. IDO folds `x - y != 0.0f` to `x != y` and sinks a single-use difference into the taken block.
   A difference computed before the branch in retail is a coloured PRE temp, not a source temp.

## Tools (tools/, replay against the current tree; no lock or manifest pins)

- `sc.sh`, `sp.sh`: unit score (tag w10h) and the sp-offset census, retail vs unit.
- `patch_tmp.py`: patches the w3a traced `uopt.c` to print `f_spilltemps` per-web slot choices and
  tempdisp changes (`TMPLOG=1`; output is byte-identical to the toolkit uopt). Built on the builder
  at `~/rush2049/scratch/frontier/w10h/uopt2/uopt`.
- `st.sh FILE.c`: score, census and spill-slot trace for func_8008E408 in one line.
- `udump.py`, `getu.sh`, `udef.sh`: dump a procedure's merged/opt ucode (frame `Udef`, Mmt refs) with
  the workbench ucode parser.
- `cu.sh`, `cops.sh`, `ctr.sh`, `force.sh`: camera_play_script unit score, register-normalised
  opcode diff, CDX colouring trace, and forced colouring on a unit snapshot.

Permission denials: none.
