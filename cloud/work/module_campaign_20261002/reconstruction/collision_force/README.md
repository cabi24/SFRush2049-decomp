# Collision force: complete reconstruction, unmatched

The actual target is `menu_options_screen` at 800CE358, 589 instructions /
2356 bytes. Its name is a historical label. The direct ancestor is
`rushtherock/game/collision.c:setFBCollisionForce` (line 341), with N64 additions
for car item effects, contact-force clamping, and distributing opposite forces
between both cars using their inverse masses.

The frozen best full source is `collision_force_native_value_groups.c`, SHA256
`64ec79504daad7ab4daed141b3a5d50b6d3b998cc30c40854855757c388e871e`.
Ordinary IDO O3 reproduces the native 152-byte frame and saves only the return
address, but still differs at 449 of 589 words. It emits 588 true instructions,
one fewer than native, with no extra words or unresolved/unverified references.
It claims no matching bytes. Independent compiler replay confirms the result;
the opcode edit distance is 33, so register and frame similarity do not establish
a nearly complete match.

## ABI and source ownership

Five consumed formals are the three Model2056 pointers, direction vector, and
contact point. An accepted caller supplies a sixth height value that the native
body never reads; no unused formal was added to this source and that caller was
not changed. The overlap helper at CE1EC consumes four ordinary pointer arguments.
Both matrix transforms and the vector norm have their actual accepted ABIs.
The external `func_8038d3a4` receives two Car952 pointers and one integer; its
identity is not established, and its real address resolves through the protected
function-address mechanism.

All body, reckon, car-control, and force fields are actual native accesses. The
used local arrays hold force, intermediate world force, relative velocity, and
the two transformed relative centers. Named basis addresses are consumed in
real transforms, the two mass ratios drive the two real scaling operations, and
the norm-result scalar is consumed in the actual collision-threshold predicate.
There are no unused arrays, synthetic storage, dummy values, or added runtime
operations. These used source values account for the emitted frame.

## Bounded progression

The first ordinary O2 source emitted a 144-byte frame and two persistent saved
registers, unlike native. O3 recovered the native policy of spilling the actual
model pointers around calls. The two cooldown reads were corrected to their
distinct native symbols, D_80124108 and D_8012410C; both pool values are preserved
as real external references. Existing used-value captures and declaration groups
recovered the 152-byte frame. Genuine pointer lifetimes, callback operand order,
integer/float constant spellings, and zero-value forms provided no further lift.

The independent full source context includes a genuine ForceApart body and the
unchanged accepted transform/norm bodies. Those accepted helpers remain exact,
but the caller remains unmatched. No accepted source, lock, image, or ROM was
changed by this research. Raw objects and disassembly remain in ignored build
storage. The main packet is frozen after these bounded source controls.
