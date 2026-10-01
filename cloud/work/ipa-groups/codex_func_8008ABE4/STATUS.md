# Codex real-caller experiment, 2026-10-01

No claims. The exact real task_complete_signal and struct_init_and_call callsites replace zz_a/zz_b. Direct closure is three members, 89 target words. struct_init_and_call is already locked and remains context. There are no stand-ins.

Rocky A strict score.py group reports func_8008ABE4 6/36 words different: the D_80153F10 base is t1 instead of v1, while all eleven temp-ring positions agree. struct_init_and_call is strict MATCH. task_complete_signal differs only at the compiler-generated unreachable epilogue: O3 group emits four alignment nops before that tail, shifting five target words and leaving four extra nonzero words. Its executable loop/calls agree; this is not a claimed match.

35 guided variants after the baseline tested dead reads of five fields/two unrelated globals/address constants, initialized unused scalar, narrow/int/u32 increment types, comparison operand order, and typed struct accesses/temporary count. Best remains 6 words. Dead reads of fields rotate the base to t0 and count to t1 (8 words); widened increment types worsen to 19 or 20. No further blind budget recommended. The workbench guide's temp-pop levers cannot change this pool coloring problem.

No source was spliced. The already locked struct_init_and_call bytes still match and no actual caller callsite regressed; task_complete_signal tail needs alignment handling before any claim. Flags -g0 -O3 -mips2 -G 0 -non_shared. No invented symbols or external-context stubs are present.
