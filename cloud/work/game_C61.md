# C61: complete native packed matrix/point transform, no claim

`camera_first_person` at `0x800BF838`, 944 bytes / 236 words. Checked archive text, filenames, accepted intervals, and overnight reservations before selection. Complete four-input `(tag, origin, first matrix, second matrix)` body. All three actual helpers were audited: `func_800AD650` expands nine signed16 matrix components to floats; `func_800BF780` performs the actual 3x3 product; `func_8009E820` transforms a Vec3 with a 3x3 matrix. No incoming custom registers are required by these helpers.

The actual 32-byte input items hold tag, matrix index, nine signed matrix halves, point index, three signed coordinates, and their packed 5-bit fractions. Output matrices have 24-byte stride and points have 8-byte stride. Source retains nine explicit 16384-scale matrix stores, packed coordinate decode at scale 1/32, subtraction from origin, both matrix transforms, addition back to origin, three actual scaled integer coordinates, and the packed fraction/coordinate stores. Retail increments an otherwise unconsumed s7 tally; no artificial use of that tally or guessed local array was invented.

Six clean singleton compiles, empty stderr, zero extra words and no unresolved/unverified/error entries. Baseline strict 8715 / 229 of 236 linked words differ. Actual lexical scopes give 8763; genuine matrix/Vec3 order 8583; O3 unchanged; direct consumed output pointer unchanged. Representing the three consumed integer coordinates as a real three-element array gives best **7134 / 227 of 236**. Its frame is 216 versus retail 240, and mixed structural/register residual dominates.

A complete real four-body O3 group also includes all three full helper implementations as kept context, with no stand-ins and no claims. Caller remains 227/236; context counts 30/57, 24/46, and 24/37, all zero extra words and no exceptions. These context sources are explicitly not claimed. Helper `func_800BF780` already has a prior native packet in `tiny_A33`; it will not be relabeled fresh work.

Best source: `game_C61/camera_first_person.packed_locals.c`, SHA-256 `83903c6f9f3fc93485d5f1b006dd0dda4cc44f0cb3027a12395071fa20af4ea9`. Target object SHA-256 `622afa571f7de6e379a7b902f65054bcba5d669b1e6b79aff3f027c120b86bf8`. Literal flags `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Group SHA-256 `566395a5bda82cc0607269089f41d7aa8bc16944d82995845e7583c7c0a178fa`, O3 flags otherwise identical.

No pressure operations, fake formals or capacities, target modifications, masked scoring, physical line reflow, or retail words committed. Full sources and sanitized JSON are frozen. Private objects and full retail disassembly stay ignored under build and Rocky scratch.

Adjacent scout: `entity_anim_texture` at `0x80091874` has real whole-program calls: `model_bounds_calc` consumes incoming s0 and `matrix_scale_apply` consumes incoming t0/s0. It was not falsely compiled as a conventional singleton.
