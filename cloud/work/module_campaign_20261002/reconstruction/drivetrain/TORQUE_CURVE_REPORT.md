# Torque curve lookup: bounded nonmatching research

`func_800E2D18` is a complete 488-byte, four-argument curve lookup. Its direct
ancestor is the interpolation tail of `drivetra.c:enginetorque`; the native
port uses fixed 1150-rpm spacing, 14-unit throttle spacing and a final tuning
scale. Input rpm and throttle and all three interpolation results are
genuine signed shorts. The table has the native 12-column row stride.

Two new source forms were evaluated after reading frozen B38 research:

| Source | Flags | Different native words | Extra words |
| --- | --- | ---: | ---: |
| `torque_curve_native_float_spacing.c` | O2 | 112 / 122 | 29 |
| `torque_curve_donor_register.c` | O1 | 121 / 122 | 25 |

The float-to-short spacing cast follows the donor's actual float
`rpmperent` conversion. The second form retains genuine donor flat table
pointers and register declarations for actual consumed interpolation
locals. Neither form explains the native division guards and register
allocation, and neither earns coverage. No fake division helper, runtime
pressure, unused storage or invented global was added. Sources and numeric
receipts are frozen; raw compilation artifacts remain ignored.
