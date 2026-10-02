# Viewport reconstruction controls

Target `wheel_render_full` at `0x800A6BE4`: 2,072 bytes / 518 instructions.
The routine configures one, two, or four viewport records from an active player count and returns the signed selected slot byte. The historical rendering name is misleading. All seven calls have the actual nine ordinary O32 arguments; no switch table exists.

The full natural C baseline covers every native branch and operation. Important native facts: active count lives at `D_80146204`; viewport records are 72 bytes; their viewport and clip pointers occupy offsets 0 and 4. Width and height are signed 32-bit values with signed division; the single-player clip writes read their big-endian low halfwords. Four separately referenced aspect multipliers contain the same 0.95f value.

No candidate is accepted. The baseline and genuine pointer/cursor/type controls still differ broadly in pointer hoisting, constant reuse, register allocation, and instruction count. Local clip cursors recover the 72-byte frame but emit 528 words versus the native 518; pointer-address integer callee arguments give 508 words and the same frame, with broad allocation differences. Full real callee context was independently attempted by the compiler agent and did not recover the native shape. No dummy arguments, padding, fabricated operations, or coverage credit were introduced.

The machine artifacts and raw words remain in ignored `build/large_viewport/reconstruction/`. `controls.json` contains numerical strict relocation-resolved evidence only. The initial baseline score was taken before correcting the count address; subsequent controls use the corrected symbol. The final baseline file includes the correction. The input signed halfword control was supported by a caller `lh` but did not match. `s32` remains the baseline ABI.

The volatile-record control was an optimizer diagnosis, not a proven original qualifier or accepted source. The switch and early-return controls explore ordinary equivalent control flow without changing data operations.
