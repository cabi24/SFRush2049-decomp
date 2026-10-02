# Complete configuration and mode initialization

`init_state_begin` at `0x800C9BE0`: 454 instructions / 1,816 bytes accepted into the byte-identical source-built game image and original ROM.

Two GPT-6.1-sol agents at High reasoning reconstructed ordinary C and independently verified the whole O3 compilation, genuine callee ABI, every instruction and all seven original switch entries. The routine applies signed-byte saved settings to player profiles and initializes the runtime settings for seven gameplay modes. No direct arcade equivalent was identified.

The fresh native draft reproduced the whole instruction structure on its first compile. The mode-six clear of `D_8015F72C` must precede the assignment to `D_80140B08`; that genuine store order removes the scheduling residual. The seven-entry original jump table at `0x80123F98` is verified by the existing group relocation implementation and a separate relocation replay. Table bytes and alignment earn no coverage.

Published group: `src/blob/groups/codex_large_init_state`. Final source hash and full proof: `compiler/final_independent.json`. Image and ROM gates: `image_gate.json`, `acceptance_gates.json`.

Current coverage: game **13.66%** (629/1,216 functions; 88,392/647,072 bytes), static **46.76%**.

The earlier unlock evaluator and texture rectangle investigations remain local nonmatches; their compiler allocation/scheduling blockers receive no coverage credit. Their existing source packets and new research remain preserved outside this accepted group.
