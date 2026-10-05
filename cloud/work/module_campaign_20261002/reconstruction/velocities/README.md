# Velocity integration: frozen donor research

`func_800E114C` is the 948-byte native adaptation of
`reference/repos/rushtherock/game/drivsym.c:velocities`. It has an ordinary
single model-pointer ABI and three calls to the accepted magnitude helper
`func_8008B3C8`. The native frame is 40 bytes, with one genuine three-float
integration temporary and a saved return address.

The native port caps speed at 300, adds throttle-dependent near-zero linear
velocity clamps, angular damping, a contact/controller-dependent angular
drag term, and low-speed angular stopping. The typed 2056-byte model and
952-byte player record reflect actual observed fields and strides.

The new source uses the direct donor's `scalmul` and `vecadd` macro forms
with the real `temp[3]` array. All complete source controls retain the same
three genuine calls, without unused locals, padding or synthetic context.

The best complete O2 source with original named globals is
`velocities_original_add_order.c`: 200 of 237 words differ, no extra words,
and no unresolved/unverified references. The native operand spelling fixes
three lanes compared with the first typed source. Its remaining 48-byte
frame saves a floating zero across calls in f20, where the native 40-byte
frame rematerializes zero in temporary registers. Old C25/A162 genuine
magnitude context controls already retained this plateau.

The decoded .1/.025/.025 literal control remains far from a match and has
unverified local pool references. It is not accepted proof. Original
double-zero stores are inert; double-zero comparisons emit unwanted double
operations and 13 extra words, so they are rejected.

The six bounded numeric receipts pin exact source hashes and flags. All
objects, native instructions and disassembly remain ignored. This packet
earns no accepted coverage; source ownership does not justify further broad
declaration or stack-storage experiments.
