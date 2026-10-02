# Large crash state updater reconstruction

## Target and behavior

`func_800E0B20` is a complete 1,580 byte game function, 395 native instructions. It has one ordinary model pointer argument, returns no value, and uses no switch table.

The function skips players in an inactive state, then examines four suspension compression values to produce a thump severity and wheel bitmask. It computes the total force magnitude and discounts simultaneous contact forces before checking whether the car should enter its crash state. Finally it checks body contact and wheel distances to detect a car resting on its roof, sets crash and top scrape state, and requests the existing cleanup effect in the relevant gameplay modes.

The model accesses cover force and contact vectors, suspension compression, wheel distances, crash threshold, crash and scrape flags, and the player index. The player table has a native 952 byte stride and the state byte is at offset 857. Field accesses in the chosen source preserve the native offsets directly.

## Arcade ancestry and N64 differences

The arcade ancestor is `reference/repos/rushtherock/game/drivsym.c`, `checkok()`. Its four wheel compression loop establishes the genuine signed halfword wheel index and bitmask, the severity comparisons, and the source order assigning severity before updating the bitmask. Its crash and roof damage logic also explains the surrounding purpose.

The N64 function differs substantially outside that loop. It skips inactive players, uses force magnitude and four contact force magnitude calls, applies gameplay mode guards, and uses body force and wheel distance checks for roof contact. The N64 severity thresholds are 1.2, 0.9 and 0.6 rather than the arcade 1.8, 1.3 and 0.9. The native image and disassembly govern every N64 offset, constant, condition, callback and store; the arcade source is supporting provenance rather than a replacement implementation.

## Genuine compiler context

The chosen complete source is `cloud/work/large_damage/compiler/actual_group_native_locals.c`, with its flags comment corrected to O3 for publication. It includes the real `effect_cleanup` wrapper from the already matched `src/blob/effect_cleanup.c`: three signed byte arguments, a gameplay mode 6 or 4 check, and the static callback. Compiling this actual body with the documented whole program IDO O3 pipeline explains the native callback arguments being constructed before the mode guard.

The external vector magnitude helper is the already matched `src/blob/func_8008B3C8.c`, with the genuine signature `float func_8008B3C8(float *)`. No fabricated callee or register pressure context is included.

All eight emitted read only floats are genuine native constants. In pool order they are 0.6, 0.9, 1.2, 0.15, 0.4, 0.2, -0.1 and 0.707. Root independently rebuilt the complete group and verified the whole 32 byte literal pool against the protected native image, then resolved every reference to its actual native address.

## Match development and final evidence

The first complete reconstruction already had the correct 56 byte frame. Mutable named float globals prevented the native constant preloads, while standalone compilation omitted the actual callback wrapper context. Genuine native literals and the real wrapper reduced the result to first loop scheduling and float stack homes.

Two source corrections completed the loop: the arcade severity-before-bitmask statement order, and ordinary suspension array indexing instead of flattening the element access into a byte offset expression. Both preserve the real operations. The final local declaration order places the magnitude before severity, index and bitmask locals, with threshold, sample, sum, count and roof contact count following; every local has a real use. This reproduces the native stack homes without additional locals or work.

Root independently compiled the chosen whole group and obtained zero differences across all 395 relocation resolved instruction words, plus exact equality of all 32 literal pool bytes. Image and ROM promotion gates, locking and publication are owned by root.

Earlier controls remain here to document the source decisions. Raw compiler objects and diagnostic artifacts live only in ignored `build/large_damage/` or the isolated Rocky harness. No protected target bytes, disassembly, or opcodes are published in this packet.
