# Complete suspension and crash-state updater

`func_800E0B20` at `0x800E0B20`: **1,580 bytes / 395 instructions** accepted. Two GPT-6.1-sol High agents reconstructed the full function and independently checked the native ABI, arcade ancestry and exact whole-unit compilation.

Arcade ancestor: `rushtherock/game/drivsym.c:checkok`. The N64 routine computes suspension thump severity and wheel masks, discounts contact-force magnitudes, and checks crash and roof-contact state. Its changed thresholds and conditions follow the native image.

The genuine already-matched `effect_cleanup` wrapper must be present as O3 context. It explains argument construction before the mode guard. Arcade severity-before-mask statement order and actual suspension array indexing recover native loop scheduling. Ordering the existing live locals by their native stack homes recovers the remaining 21 instructions without extra work or padding.

Published source: `src/blob/groups/codex_large_crash_state/group.c`. Root independently compiled the final source through every documented O3 stage, resolved all references and verified the complete 32-byte eight-float pool. Proof: `compiler/root_final_independent.json`. The context body supplies no additional coverage; its existing matched provider remains in the ROM.

Full source-built game image and original ROM hashes match. `MAKE=0 TEST=0`; all standalone, group and static locks pass. Coverage: **631/1,216 functions, 90,424/647,072 bytes (13.97%) game**, **46.76% static**.

This round also accepted the separate 452-byte graphics coordinate helper (`cloud/work/large_palette`), totaling 2,032 new game bytes. Earlier viewport and palette candidates remain preserved local research without coverage credit.
