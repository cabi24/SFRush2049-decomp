# Tire forces independent compiler audit

Target `func_800E23A4` is 1,688 bytes / 422 executable words. Native frame is 152 bytes, saving only s0 and ra. All actual pointer arguments and float stack arguments follow ordinary O32; no hidden register parameters were found.

## Genuine source ancestry

`reference/repos/rushtherock/game/drivsym.c:390` supplies `forces1`: four calctireuv calls followed by four dotireforce calls, front turning drag correction, then rolling/aerodynamic resistance. The N64 body adds traction, wind, and drag-table logic. The current reconstruction retains the native rear condition that reads wheel_mode[0] in its final disjunct.

The native calctireuv helper is historically named camera_collision_avoid: eight arguments, with steering float bits in a3. The native dotireforce helper is historically named camera_follow_target: fourteen arguments. Original `reference/repos/rushtherock/game/tires.c:123` proves the last airfact formal, although its native callee body does not use it. Main native callers store 1.0f at outgoing stack offset 52. The consumed poortract and airfact locals are donor-owned; no unused donor i/temp/gameoverdrag declarations are included.

## Actual structural corrections

Interleaved basis/velocity capture order reproduces all eleven pointer homes. Direct member addresses plus consumed poortract/airfact declarations reproduce the 152-byte frame. A converted original double literal `(float)1.0` in the front throttle expression reproduces native float allocation and rear 1.0 rematerialization. Integer zero in the last terrain arm reproduces native zero materialization. Explicit row if/else reproduces native ordinary branch, NOP, LH, branch and increment.

`row_explicit_else.c` and the actual O3 group at `donor_consumed_group/` now reproduce all 422 instruction opcodes. Both O2 standalone and the full real O3 pipeline emit the same body shape. Two natural row statement-line controls did not change the residual.

## Protected literal and complete-body proof

`row_explicit_else_linked.json` records the full protected `blob_group.relocate` result. All twelve source-built literal bytes (5000.0f, 0.1f, 0.1f) match the native pool at 0x801243CC through 0x801243D7. Every HI16/LO16 site is resolved using the existing protected resolver. Full 1,688-byte comparison leaves exactly eight word differences: 179 and 190 are poortract stack home 120 instead of 124; 329, 332, 334, 335, 337 and 339 are row v0 instead of native v1.

No relocation masking, padding, stand-in callee, extra source operation, image mutation, lock, integration gate, test, or commit was used. This candidate remains unaccepted pending a strict zero-word result and root integration gates.

## Current two-word result

The genuine original donor front `{ float tsc; ... }` block, with a separate consumed later factor, restores frame 152 after the row index is expressed directly in the table access. This source is `reconstruction/native_expression_scoped_tsc.c`, SHA256 a78bf705049cb5d0db9fc8bcb09efa0ac819ba1e11c4d65c6daf377a3f68e8bb. All 422 opcode and register lanes are exact. Protected whole O3 proof `native_expression_scoped_tsc_linked.json` leaves only word indices179 and190, both poortract stack displacement120 versus124.

Full genuine native calctireuv and dotireforce context at `actual_tire_helpers_group/`, preserving the donor airfact formal, is inert. Bounded g3/O2 worsens the body to36 word differences. Capturing the actual wind conversion in a consumed float local preserves all opcode/register lanes but shifts pointer homes, worsening to30 differences; declaration-first form is inert. None of those controls is accepted.

## Real type and generated-expression controls

The accepted `src/blob/camera_dolly.c` proves Tire's compact 92-byte layout: eleven initial floats, a 24-byte preserved gap, five further floats and signed-byte slip diagnostic, with natural 4-byte alignment. Replacing the opaque four records and dotireforce formal with this actual Tire type is inert. Full native helper context with typed Tire is also inert. Naming real parameter spring/damping/drag and tire-position fields preserves the same two-word result.

Three natural physical source layouts of the rear boolean and real call arguments are inert. Source operations, argument counts, native model offsets and all 422 instruction/register lanes remain intact in every best control. No arbitrary whitespace search was used.

The actual donor unsigned-byte BOOL definition was checked once: explicit unsigned-byte poortract introduces conversions and regresses to273 differing words. The wide native integer spill/reload remains the authority. G1/O2 (and reconstruction's G1/O3) disables the desired optimization shape, producing512 padded words,104-byte frame and81 nonzero words past the native extent. These are rejected.
