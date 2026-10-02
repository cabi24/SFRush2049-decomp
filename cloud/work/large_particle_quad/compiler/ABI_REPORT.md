# Independent native audit: `entity_update_callback`

Target `0x80090FEC`, complete scanner extent 546 executable words / 2,184 bytes. Ordinary two-argument ABI: pointer to a node with signed halfword index at offset 8, then signed halfword update flag. Retail saves every used integer and floating callee-saved register and homes its actual second argument. Native frame is 256 bytes. There are six genuine call sites and no switch jump table.

The historical function name is generic. Native logic updates four 84-byte particle matrix records plus their shared 400-byte state record, releases objects on expired timers or changed mode/car state, cycles byte color/opacity, applies matrix rotations, and updates particle size, position and angular/scale velocities. Global object records have 68-byte stride. Several field purposes remain inferred from their operations; retain exact native offsets.

`entity_spawn_callback(0x80090088)` is also historically misnamed: its complete 104-word body recursively releases linked object handles. It consumes the actual signed halfword handle plus two ordinary integer flags; the target always calls it with both flags zero. Native entry homes actual first/second arguments and does not consume hidden registers. `entity_transform_apply(0x8009079C)` is already matched: actual node cleanup/release function taking one node pointer and integer unlink flag.

`func_80090F44` and `func_80090E9C` are each 42 words. Both take one ordinary float in `$f12` and a matrix pointer in `$a1`, call standard `sinf`/`cosf`, and update three columns of a 3x3 float matrix. The first rotates matrix rows one/two; the second rotates rows zero/two. No extra incoming or preserved caller registers are required. Their real small-angle bounds are separate global constants, not hidden ABI state.

The actual ANSI random generator `func_8008B2B4` is already matched, with real seed `D_8011735C`, multiplier 1103515245, increment 12345 and signed shift/mask to 15 bits. The target's random block is its complete native inline expansion. If needed, include this real body as context; do not model a fake clobber helper.

Constants independently decoded from the extracted native image: `D_801239E0 = 0.05f`, `D_801239E4 = 0.2f`, `D_801239E8 = 0.002f`, `D_801239EC = 0.0333333f`. Native quadrant offsets use exact ±0.5f. The matrix loop handles exactly indices zero through three and each active index defines the two consumed offset locals before use. Early native stack-home reloads do not justify adding initialization operations absent from the retail code.

Arcade search: searched `reference/repos/rushtherock/game/visuals.c`, `svisuals.c`, cars sound code, and particle/smoke/blast/spark names. Related arcade routines include `AnimateSmoke`, `AnimateBlast`, and `HandleASpark`; none has this native four-record matrix/color update structure. Treat them as subsystem clues, not an established direct equivalent.

Protected target SHA256: `e170584e78457822aec246af20b566fc833c536822e3404e5566387044857453`. Isolated IDO harness: Rocky `~/agents/large-particle-compiler`, using toolkit `796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`. All raw artifacts remain under ignored `build/large_particle_quad/compiler/`.

## Frozen independent controls

No candidate accepted. Baseline O2 and O3: 144-byte frame versus retail256,
547 executable words versus546,525 fullworddifferences. Real wholeO3
contexts containing the complete accepted `entity_transform_apply` body or both
complete native rotation helper bodies left the caller identical to baseline.

Ordinary O1 control:64-byte frame,684 words,545 differences; rejected.
Root complete m2c source, with the undeclared alias corrected to `D_8014A108`,
naturally reserved248bytes, but produced only500words/392alignedopcodes and
529differences under both O2/O3. This confirms real captured temporaries can
reserve source homes but is no matching structural recovery.

Natural literal-source scalar captures (scale/radius/velocity/opacity/alpha)
reserved176bytes,548words,530alignedopcodes,506differences. Capturing the
actually computed lifetime/timer/position/animation arithmetic results reserved
216bytes,548words,528alignedopcodes,510differences. Fourliteralpool relocations
are unverified by the standalone harness; no float-pool or ROM match is claimed.
Neither control contains unused pressure locals, dummy arguments, padding,
synthetic operations, or fake callee context. Current coverage remains13.97%
game and46.76% static, unchanged by this packet.
