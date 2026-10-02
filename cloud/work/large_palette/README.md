# Display command coordinate updater

`camera_smooth_follow` at its existing protected game target: 113 instructions / 452 bytes accepted. The historical function name is misleading: this routine walks graphics commands, adjusts texture coordinates and scales coordinates when texture dimensions shrink.

The native five-argument ABI was reconstructed during large-function scouting. The complete source reproduces all 113 instructions with no unresolved or unverified references. Four ordinary statement line breaks changed IDO scheduling to the native order; no extra runtime work or fabricated register pressure was introduced.

Independent root compilation through `blob_splice` passed the full 647,072-byte image hash. The source-built compressed image passed the complete ROM SHA-1 gate (`MAKE=0 TEST=0`). All standalone, group and static lock checks passed.

Flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
Coverage: game 630/1,216 functions, 88,844/647,072 bytes (**13.73%**); static **46.76%**.

Large viewport and palette investigations remain preserved unaccepted research. The two GPT-6.1-sol High agents are continuing the separate 1,580-byte crash-state routine.
