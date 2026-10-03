# Native navigation subsystem audit

Frozen research; no new matching credit or acceptance claim.

## Selection and reservations

The three proposed historical names were checked against native purpose and complete prior reports. `drone_ai_update@80093B20` is a palette/model renderer paired with a 671-word IPA callee, already frozen with a roughly 300-byte unexplained frame region. `Input_ProcessGameplayPad@800A04C4` is a display-list renderer with unresolved tail-callee context. Neither is a fresh drone controller. C96's nominal maxpath family actually processes controller-pack files. Existing E398C/E4300/E451C path closures are archived plateaus.

The chosen actual navigation family searches six-byte road points and sixteen-byte branch records, updates each car's path state, and accumulates lap/section/projected distance. `world_gravity_apply@800ECC18` is the historical name of its 661-word main routine. `audio_channel_alloc@800BA00C` is its 169-word road-distance helper. The active B125 `func_800F8EC8` caller was not edited or compiled in this lane. Physics/drivetrain reservations were avoided.

## Source and ancestry

`native_counter_types.c` is a complete fresh structured native reconstruction from the archived m2c seed, with actual Model952, Vehicle2056, Point6, Branch16 and Section80 fields. It uses a real signed-halfword position[3] and two consumed float[3] vectors. The path successor ABI is four integer/pointer arguments, not the seed's stale floating arguments; the eligibility predicate has one consumed signed-halfword input. The final successor call passes -1, confirmed by the native LI at800ED35C. Native word loop counters, short first-window bound, signed far-scan count and unsigned final completion count are distinct.

`road_distance.c` reconstructs both wrapped distance loops with the actual three-float scratch vector and sqrtf intrinsic. The y-component difference is written before the genuine y=0 assignment, as observed in the native source form; no volatile or pressure workaround is used to retain it.

Arcade drones.c, maxpath.c, checkpoint.c and mdrive.c were searched. `mdrive.c:get_demo_path_point` and `maxpath.c:find_maxpath_intervals` establish related nearest-point and race-distance algorithms. An exact branch-aware whole-function ancestor was not found, so this packet does not claim a verbatim donor port. Existing accepted path-step and normalization bodies are copied unchanged only into ignored compiler context. Their source hashes are recorded. No notices were removed from copied files; no new license assertion is made for the native reconstruction.

## Independent compile outcome

Final main SHA256: `74187fb86262b7091e1fd94faf0bd80a229abd507ba726590daa589fa44b94ec`.

| Routine | Native words | Generated words | Aligned opcodes | Strict differing words | Frame |
|---|---:|---:|---:|---:|---|
| world_gravity_apply |661|648|611|632|248 vs240|
| audio_channel_alloc |169|155|142|169|128 vs native120|
| func_800B9F60 (already accepted) |43|43|43|0|0|
| func_80098A54 (already accepted) |34|34|34|0|0|

The archived main seed aligned only517 of661 opcodes; the corrected structured source aligns611. This is structural research progress and earns zero coverage. The real four-function O3 closure is inert for the main compared with ordinary O2. Both existing helpers remain strictly exact. All functions have empty unresolved, unverified and error lists; no relocation masks are accepted. The full context emits3,520text bytes and no owned initialized data or literal pool.

`actual_callers.json` proves real outside callers for all four retained functions, avoiding fabricated keepers. `compile_recipe.json` records the actual whole O3 stages and toolkit; `full_context_proof.json` contains the strict results and source/object hashes. `baseline_comparisons.json` preserves the superseded initial measurement with explicit semantic errata, then the final corrected receipt. An actual register-float declaration control was inert and is not retained as another source variant.

The native frame remains eight bytes smaller, and thirteen executable words remain absent. Native FP and integer register webs differ extensively. This packet has no evidence for another helper or unused buffer occupying those homes; padding and synthetic helper controls were not attempted. No accepted source, lock, target/scorer, image/build infrastructure, test or commit was changed.
