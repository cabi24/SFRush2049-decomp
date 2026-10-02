# Force summation: func_800E1C30

The frozen reconstruction is `forces2_native_body_sum.c`, SHA256
`75fa8e12c16df3f7ec17b94bdf42cd7bba0adc01564ff6ddf13da971589ac36d`.

The ordinary IDO compile matches all 212 instructions (848 bytes), with no
unresolved or unverified relocations, errors, or extra words. Flags are
`-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. The native and compiled
function use a 56-byte frame and save only the return address. The receipt in
`receipts/forces2_native_body_sum.json` records the first complete match;
independent verification and image/ROM acceptance belong to the integration lane.

## Source ancestry and behavior

The direct ancestor is `reference/repos/rushtherock/game/drivsym.c:forces2`
(line 494), with the genuine `vecadd` operation from `vecmath.h`. The function
sums tire, gravity, drag, body-contact, and center forces in the vehicle's body
coordinate system. It receives one ordinary `Model2056 *` and returns void.
The two calls use the actual accepted three-pointer matrix transform
`func_800A61B0` and one-pointer vector norm `func_8008B3C8`; no helper source,
synthetic context, inline wrapper, or assembly was needed.

The N64 version uses Y as the vertical axis, conditionally blends the rear
longitudinal tire forces using road state and a vehicle tuning value, rebuilds
vertical world gravity, applies a per-car modifier under the native gear and
position predicate, and captures the combined body-contact magnitude. The
2056-byte record's arrays and named fields describe the actual native offsets;
padding describes the untouched portions of that real record.

## Grounded source choices

The direct donor vector macros improved the old opaque seed. The first fresh
compile had 112 differing words and two extra words. An explicit native row
if/else and the donor's field-first operand order for body-contact accumulation
reduced that to seven differences. Applying that same original order when
adding the body-contact sum to the total force produced the complete match.
These changes preserve the real operations and floating-point addition tree.
The consumed three-float body-contact vector supplies the native stack storage;
there are no unused locals, pressure values, artificial padding, or added work.

The mutable gravity and modifier globals retain genuine symbol relocations.
This source owns no literal-pool bytes and no switch table. The accepted
drivetrain, clutch, and torque packets were not changed.
