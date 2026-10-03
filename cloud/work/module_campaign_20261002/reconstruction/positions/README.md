# Positions: bounded donor reconstruction, unmatched

Target `func_800D0424` is 249 instructions / 996 bytes. The direct ancestor is
`rushtherock/game/drivsym.c:positions`. Fresh typed Model2056 arrays and genuine
`vecadd`, `scalmul`, and `veccopy` operations replace the older B15 opaque aliases.
The N64 version integrates normal world position, normalizes its three basis
rows periodically, caps tire penetration, and transforms all eight tire/body
corners into world coordinates. The first copy captures the previous position.

The best bounded source is `positions_native_body_cursor.c`, SHA256
`549fa36b7ef8c5305fd35e7842dd3fbad4a7b7f82240acd3232133cc3fa9a457`.
Ordinary O2 emits the complete target extent with 105 differing words and no
unresolved/unverified relocations, errors, or extra words. The native 176-byte
frame remains unexplained; the smaller source frame owns only its real used
values. This packet claims no matching coverage.

The first direct donor source had 247 differences and two extra words.
Capturing the actual current tire corner before copying its three components
gave 245 differences and one extra word. Using a consumed three-float offset
for the second corner loop recovered the target body extent and removed the
extra persistent constant/register. These are genuine address and vector
operations; no unused donor arrays, pressure locals, synthetic padding, or
new runtime work were added to reproduce the frame.

The main function has an ordinary Model2056 pointer ABI. Every callee was
audited: matrix transforms accept three pointers; `sound_position_set` accepts
the angle vector and nine-float basis; `menu_video_settings` accepts one model;
and `func_8008B424` accepts one vector and returns a float. The accepted rotation
wrapper calls the actual Y/X/Z axis routines using float f12 and pointer a1.
No stand-in helper or speculative hidden-register context was used.

All three sources and numeric receipts are preserved. Objects/disassembly are
ignored build artifacts. The packet is frozen after the bounded pointer-loop
controls; accepted forces2 and drivetrain packets remain unchanged.
