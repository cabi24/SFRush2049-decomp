# w22i - owned slots for func_80023E9C + func_80022874 (lib_22300 .bss block, co-owned .rodata) (tooling lane, worktree)

Branch `w22i` (worktree `.claude/worktrees/w22i`, from wave22 cbbc6888). Not pushed. Static numbers only.

STATUS: in progress. Plan: (1) owned_data: composed companion for a co-owned slot whose owner is already C
(`c_prefix`), `prepare --replace`; promote_batch: multi-slot membership + pending members; (2) prepare the .bss
block and the replaced .rodata row, builder ROM gate; (3) locks, context rows, promote_batch; (4) tests.
