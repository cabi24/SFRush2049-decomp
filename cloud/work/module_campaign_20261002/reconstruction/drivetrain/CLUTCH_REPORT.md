# Clutch integration

`func_800E2AC4` is the 428-byte native adaptation of
`reference/repos/rushtherock/game/drivetra.c:whatslips`.
It takes one 2056-byte model pointer through the ordinary ABI and has no
calls or stack frame. Its actual parameter/tuning fields are shared with
the matched drivetrain and automatic-shifting members.

The N64 version clamps engine angular velocity to 1000 rpm, computes the
available clutch torque, integrates engine angular velocity once, and snaps
to wheel angular velocity when the clutch crosses from one slip direction
to the other. It then updates effective inertia and driveshaft torque. The
native gain at tuning offset 16 augments the original donor calculation.

The winning source is `whatslips_native_crossing.c`, SHA256
`3984d787b9648243b82aa428e52c358dd528c80c6c72e29e2e4a6568ebdeac5b`.
Its flags are `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.

Concrete source choices that recovered the original code:

- The donor's squared total-ratio capture executes before the initial clamp.
- Original `rpmtordps` and float conversions produce the native 1000-rpm
  threshold, `104.71966552734375f`, and clutch threshold, `.8f`.
- Integer zero matches native floating zero materialization.
- The `angvel` local holds the actual angular increment. The crossing check
  predicts the new velocity, and the normal branch applies that increment
  with a compound assignment. IDO shares the actual addition; no extra
  runtime calculation is emitted.
- XOR between the two Boolean slip directions expresses the genuine
  crossing predicate. Both inputs are zero or one; IDO lowers it directly
  to the native comparison branch without an extra XOR instruction.

The initial standalone receipt shows all 107 runtime instructions exact,
no extra words, and four local constant-pool references awaiting the full
protected resolver. The compiler lane owns the independent complete-body
and 8-byte constant-pool proof. The coordinator owns image/ROM acceptance
and publication. No separate coverage is claimed from source declarations,
unproved local relocations, or the older nonmatching controls.

No pressure variable, padding, unused storage, synthetic helper, additional
runtime operation or target instruction stream was introduced.
