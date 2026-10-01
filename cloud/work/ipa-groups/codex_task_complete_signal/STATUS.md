# task_complete_signal — strict MATCH in genuine compile context

2026-10-01 worker B. Claim only task_complete_signal (36 words, 144 bytes).
Flags -g0 -O3 -mips2 -G 0 -non_shared, strict score.py group --claims: MATCH.
Context func_8008AD04 also reports MATCH; it is already locked and is not claimed.

The target has a dead epilogue after an infinite loop, and as1 aligns that region
against absolute module position. Compiling task alone leaves one extra nop;
invented PG predecessors reproduce it but are not acceptable integration context.

This group instead includes the **real adjacent string-comparison function
func_8008AD04**, using its already verified full body from cloud/matches, with
both functions kept external. No stand-ins, invented helpers, or synthetic calls.
The compiler emits func_8008AD04 before task, placing task at the module phase that
reproduces its exact tail. The group claims task only; caller/callee symbols remain
external and do not point into unclaimed invented code. Group image/ROM gates are
pending coordinator acceptance. Existing locked func_8008AD04 remains context only.

Six permutations of the earlier real DMA group did not close the tail. A nearby
real queue-init function produced the wrong phase. Identifying that task needs
module start congruent to 4 bytes mod32 led to the real adjacent 17-word comparator.
