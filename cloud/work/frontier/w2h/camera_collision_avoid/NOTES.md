# camera_collision_avoid (0x800BCEE4, 412 bytes) - 31/103 words in the unit, FP temp colouring only

Semantics (label is historical): build a camera basis.  p = (b x a) + origin (b[1]*a[2]-b[2]*a[1], ...);
row1 of `uvs` = mat * src[3..5] (func_800A61B0(in, out, mat)); row0 = (cos ang, 0, -sin ang);
row2 = row0 x row1; row0 = row1 x row2; then pos = uvs * p (func_800A61B0(p, pos, uvs)).
Eight parameters (origin, a, b, angle in a3 as f32, mat, src, uvs, pos).  No arcade ancestor identified.

State (best.c): `blob_unit --tag w2h score camera_collision_avoid --with best.c` ->
`FAIL camera_collision_avoid: 31 of 103 words differ` (size equal, frame 120 equal).
Everything from the first jal to the end is identical; the 31 words are the FP registers of the first cross
product (retail loads b1->f4, a2->f8, a1->f10, b2->f4; ours b1->f6, a2->f4, a1->f8, b2->f6).

Found:
- second product of each cross term must be written reversed (`b[2]*a[1]`, `uvs[2]*uvs[4]` ...): IDO evaluates
  the second product's operands in reverse source order; this fixed 6 rows of the basis code and the loads.
- `f32 pad[4]` (16 bytes below p) gives retail's 120-byte frame and p at sp+108 in the unit.  Not original
  names; something 16 bytes (4 floats / an unused Vec3+1) was declared before p.
- standalone `score.py fn` builds a different frame (adds s1 + a0 home); the unit is authoritative here.
Not tried / next: the starting phase of the FP temp allocator.  A two-line standalone probe shows ugen is a
pure round robin f4,f6,f8,f10,f16,f18 when nothing precedes; here ours starts at f6, retail at f4, so one
FP temp is consumed before the first statement in ours (or the IR order of the first two loads differs).
Check the pre-as1 listing (o3s.sh) with the pad declared after p / as a Vec3 / as other types.
