# Four particle quad updater research

Target `entity_update_callback`, 0x80090FEC, 546 words / 2,184 bytes. Complete ordinary pointer and signed halfword input ABI. The historical generic name conceals four 84 byte particle records in a 400 byte per player effect block. The routine releases expired handles, updates crash particles, cycles alpha, constructs scaled and rotated matrices, and changes per object color. Graphics object records are 68 bytes.

`baseline.c` is a full natural reconstruction. `native_literals.c` replaces four native read only constants with their decoded values and reproduces the native six floating register saves; it has 548 emitted words including alignment, a 152 byte frame, and 536 of 546 aligned opcodes. It is not an accepted match. The native frame is 256 bytes, leaving a genuine local layout and allocation question unresolved. Literal references require a complete native pool mapping before acceptance.

Actual source contexts for the existing cleanup and matrix rotation callees did not improve the complete caller. `actual_rand_group/` uses the genuine already matched integer random generator. `actual_frand_group/` uses the complete native float random helper preserved in tiny_A23. These are genuine context controls, not fabricated register pressure functions. Neither produced a near match.

Natural loop carrier, local pointer, model type and shared cleanup controls are retained. `hybrid_native_head.c` combines genuine captured values from a fresh typed m2c seed with the complete indexed particle loop; it approaches the native frame at 224 bytes but has worse instruction structure. Independent compiler controls with real scalar and result captures also failed to close the frame and allocation gap. No dummy locals, unused arrays, padding operations, invented arguments or additional runtime work were added to bridge that gap.

The native timer predicate includes its NaN behavior: updates proceed only when the timer is less than or equal to zero. Each legal particle index defines both offset floats before use; early reloads of their stack homes do not justify artificial initialization.

Arcade visuals.c contains related smoke, blast and spark subsystems, but no exact ancestor for this four record matrix routine was established. Protected bytes and raw diagnostics remain only in ignored build directories or isolated compiler harnesses. This research is frozen without a match claim while the team pursues another fresh large target.
