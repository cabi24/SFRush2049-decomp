# Drivetrain module member

`func_800E32CC` is the native adaptation of `drivetrain` in
`reference/repos/rushtherock/game/drivetra.c`.

The complete source calls automatic shifting, engine update, transmission
update and clutch integration. It computes rear wheel angular velocity,
distributes differential torque using rear tire loading, and updates rear
wheel inverse inertia. The native vertical axis changes the donor's rear
load sign; the native branches and accumulating torque stores are preserved.

`Tire92` follows the compact record already established by the accepted
`src/blob/camera_dolly.c` friction-circle implementation. The vehicle record
retains native observed offsets and its genuine 2056-byte stride. Its torque
array begins at 948, so rear torques are at 956 and 960.

`drivetrain_donor.c` compiles with ordinary IDO O2 flags to all 89 native
instructions (356 bytes), with exact 32-byte frame, ordinary one-pointer ABI,
and all four resolved calls. `results.json` records zero differing words,
zero extra words, and no unresolved, unverified or erroneous references.
The independent compiler lane also reproduced the complete O2 and O3 match.

The direct donor's array expressions, compound assignments and integer zero
constants explain the native expression lifetimes. No extra runtime work,
unused local, synthetic callee context or manufactured stack storage was
introduced. The parent coordinator owns image/ROM acceptance and publication.

## Automatic shifting

`autoshift_donor.c` reconstructs the direct `drivetra.c:autoshift` ancestor.
The native tuning multiplier scales both shift thresholds. The complete
source preserves the donor's neutral/reverse `if/else` and three genuine
consumed float locals. It takes one model pointer; `func_800E313C` receives
that pointer and both float thresholds, while the up/down shift helpers
receive the same model pointer. The physical native calls confirm these
ordinary ABIs. IDO O2 reproduces all 62 instructions (248 bytes), the exact
32-byte frame and every resolved call. `autoshift_results.json` and the
independent compiler receipt show zero differences and extra words.

Frozen source identities ready for coordinator acceptance:

| Member | Source SHA256 | Native bytes |
| --- | --- | ---: |
| Drivetrain | `aacf977fa7add74f6564c60cd6b7bbd4f1521fa1a56cf671070c02b7d6613b2e` | 356 |
| Automatic shifting | `8f9610a71af5af6eaf35f39d52ad128244e1c18ebfeaf4b7dd8ec0d43b1717b7` | 248 |

Both use `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
No helper coverage is credited merely from these declarations or context.

## Clutch integration research

`whatslips_donor.c` and `whatslips_native_order.c` are complete native
adaptations of `drivetra.c:whatslips`. They use the original squared ratio
capture and native single crossing test rather than the arcade's duplicated
clutch integration branches. The native minimum engine angular velocity and
clutch threshold remain genuine named global reads. The best complete O2
control currently differs in 34 of 107 instructions, with no extra words or
unresolved references; it earns no accepted coverage. Research continues
only on concrete operand ownership and actual consumed float captures.
