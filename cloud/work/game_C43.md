# C43 — real aligned allocator, bounded nonmatch

`sound_play_menu` at `0x800CC3C0` is the actual complete332-byte/83-word canonical ordinary two-argument allocator. Report text/artifact filenames, current blob locks and accepted address intervals were checked; no prior packet or accepted overlap. No accepted files changed.

Source reconstructs the real32-byte block header (previous+4,next+8,u32size+12,value+16,signed used byte+20), optional arena header first pointer+12,32-byte allocation alignment,64-byte split threshold, original FEDCBA98 header marker and real queue calls. Original unchecked end-of-list dereference is preserved. The original unused total/largest metrics naturally remain in ordinary IDO output; no artificial consumers or volatility were introduced.

Exact flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Five meaningful source controls: natural baseline strict1873/69 linked differences; distinct selected arena strict1478/60 plus2extra; genuine cached previous pointer strict883/62/noextras; genuine cached block size strict940/62; declaration order strict990/65. Best strict source is `game_C43/sound_play_menu.previous.c`, SHA256 `74dc927130ea3943d9530de3de28e1f0c878db316894118eb842c2f4b47b3461`. No zero, no coverage.

Target SHA256 `d72f24ecabfd2f914c01536b8663e7f223ae59f49d3182b66d37c4ee161e47a6`, reloc_aware/extent_repaired. All comparisons have no unresolved/unverified/errors. Residual is allocation plus constant/address scheduling. Complete sources and canonical strict/full linked records are frozen; objects remain Rocky private `~/agents/C/scratch/game-C43/`. No pending jobs or broad register/line sweep.
