# Image B HUD initializer: lean research

Target: `B:func_803925D0`, `[0x803925D0,0x80392894)`, 708 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result

IDO 5.3 standalone O3: **18 / 177 words differ**, zero extra words,
unresolved symbols, unverified relocations or comparison errors. The first
complete reconstruction differed in 33 / 177 words at both O2 and O3.
Using separate indices for the variable-player drawing loop and the fixed
four-player reset loop removed 15 register-allocation differences. The
remaining 18 differences are in the schedule around the last flag clears
and fixed-loop setup. This is a research candidate, not an accepted match
or ROM coverage.

## Build

From the repository root with the documented IDO toolchain:

```
python3 tools/cloud/score.py fn cloud/work/lean/runtime_b_hud_init_20261006/candidate.c func_803925D0 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

The scorer also supplies `-Wab,-r4300_mul`. The command is expected to report
NONMATCH. No additional translation-unit context or owned data is required.

## Source and assumptions

The body initializes mode-dependent HUD blits, selects font 10/11/12 for
one/two/more players, creates padded player panels, clears five status bytes,
and initializes four groups of scene handles. Native record sizes are 52
and 72 bytes, with ten effects in each of four groups. Unknown fields preserve
native record storage. Names describe behavior; original names, full record
definitions, and a whole-function arcade donor are not established.
Helper declarations use the existing NewMultiBlit, font-metric, panel and
color contracts. Valid table indexing requires player counts 1..4 while
building panels. Helpers remain external, without invented bodies or callers.
No artificial volatile, padding locals, pressure reads or inline assembly.
Only compile/scoring was performed. Acceptance, broader tests and integration
are left to the independent checker.
