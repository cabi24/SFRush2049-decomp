# Codex second real DMA closure experiment, 2026-10-01

No claims. Included the actual task_complete_signal and struct_init_and_call bodies from the first real experiment, plus B's real matched func_8008AD04 body. All callers/context remain real, with no stand-ins. Flags -g0 -O3 -mips2 -G 0 -non_shared.

The real four-function group reproduces func_8008ABE4's same 6/36 strict residual: its D_80153F10 base is t1 rather than v1. The temp ring and every other target word agree. struct_init_and_call and func_8008AD04 informational context both match; task_complete_signal has an unclaimed unreachable-tail alignment difference in this combined emission order.

Nineteen bounded new controls investigated actual declaration/prototype and source structure rather than repeating the first packet's 35 dead-read variations: defined-vs-extern descriptor, unsigned count/limit fields, void-vs-s32 and K&R DMA prototypes, three return types, direct struct fields/pointer, preincrement, named current count with three types, and inverted early-return guard. None improved below 6; widened/named or guard forms were worse. Two prototype/body layout attempts failed C89 declaration placement and were corrected where meaningful. No source was submitted as a match.

Stopped this residual and moved to C7200's actual return-tail mechanism, which produced a separate strict match. No locked source, tooling, splice or state files were changed here.
