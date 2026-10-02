# Complete tire friction-circle reconstruction

Historical symbol: `camera_dolly`, address `0x800BC2BC`.
Authoritative scanner/closure extent: **575 words / 2,300 bytes**.
Direct arcade counterpart: `reference/repos/rushtherock/game/tires.c:248`,
`frictioncircle`; companion `func_800BC21C` corresponds to `calcalpha`.

## Source and ABI

`camera_dolly.c` is the full ordinary C89 candidate. It retains the arcade
scalar locals and coefficient arithmetic while applying the native N64
branches documented in its header comment. Seven genuine O32 arguments are
model pointer, three-float velocity vector, normal force, torque, tire pointer,
lateral force output pointer, and longitudinal force output pointer. The
historical camera naming is misleading.

The N64 tire record fields are radius0, lateral spring4, lateral damping8,
friction coefficient16, inverse inertia20, alpha limit28, cubic coefficients
32/36/40, contact-patch displacement68, wheel angular velocity72, slip torque76,
lateral force80, traction84, and signed-byte slip diagnostic88. Unknown layout
padding preserves the native field offsets; no values are invented.

## Compiler controls

All controls preserve the complete function and behavior:

| Candidate | Strict differing / 575 | Extra nonzero words | Explanation |
|---|---:|---:|---|
| arcade_port.c | 569 | 36 | Missing genuine fabsf intrinsic contract; unresolved fabsf call |
| arcade_intrinsic.c | 6 | 0 | sqrtf/fabsf declarations plus genuine IDO intrinsic pragmas |
| patchspeed_reuse.c | 4 | 0 | Reuse the real patchspeed scalar in softened side-force branch |
| patchspeed_sum_order.c | 0 | 0 | Restore arcade longitudinal-square then lateral-square order |
| camera_dolly.c | 0 | 0 | Publication comments only; separately recompiled |

The exact flags are the first source line. All 575 retail instructions match
with strict relocation resolution and no unresolved or unverified symbols.
The compiler emits one additional zero word for alignment; that word is not
function coverage. Frame size matches the native 120 bytes.

A workbench diagnosis is retained privately under ignored build. With raw
retail-word target export, its relocation/layout and padded-instruction-count
warnings are expected; its commutative operand observations agreed with the
native softened-speed branch. The canonical resolved positional comparison is
the matching authority. No masks, dummy operations, pressure variables,
assembly source, or target byte arrays appear in the C candidate.

`manifest.json` records the source, retail extent, flags, and toolkit hashes.
`control_results.json` records all canonical results. Raw target assembly,
objects, emitted-word arrays, and workbench output remain under ignored
`build/large_camera_dolly/reconstruction/`.

Image/ROM integration and coverage reporting belong to the root agent.
