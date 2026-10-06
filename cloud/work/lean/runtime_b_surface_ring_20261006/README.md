# Image B projected surface-object ring: lean research

Target: `B:func_8038A408`, `[0x8038A408,0x8038A8CC)`, 1,220 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result

Standalone IDO 5.3 O3: **268/305 words differ**, zero extra words,
unresolved symbols, unverified relocations or comparison errors. This remains
a broad NONMATCH. The first complete source differed in 276/305 words.
Spelling out the four native edge checks reduced that to 273; using separate
corner and ring indices reduced it to 268. Frame/allocation and instruction
layout remain substantially different (candidate frame 200, native 248 bytes).
No unused pressure locals or artificial padding were added to close that gap.

## Reproduce

```
python3 tools/cloud/score.py fn cloud/work/lean/runtime_b_surface_ring_20261006/candidate.c func_8038A408 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

The canonical scorer adds `-Wab,-r4300_mul`. No translation-unit or own-data
context is needed. The command intentionally reports NONMATCH.

## Source and assumptions

The two-input function checks four floats in a 2,056-byte vehicle record,
projects four 12-unit-square corners onto terrain, rejects excessive edge
height/distance, and updates a 25-object surface ring. Once the ring is full,
its next object is retired before advancing the index. Otherwise a quad is
created. The next four present objects receive successive colors, followed
by a position/color update of the current object.

The already-submitted B:8038A8CC initializer establishes the ring's signed-byte
wrapped flag, unsigned-halfword index and 25 pointer slots. Existing terrain
query and polygon helpers establish the 3-vector/3x3-matrix output and seven-
argument polygon update contracts. Helpers stay external and unmodified.
Unknown vehicle fields preserve actual record storage. Names describe native
behavior; original names, local declarations and whole-function arcade ancestry
are not established. The second player input is read in native a1 even though
some older game-side declarations show only the position argument.

Inputs require three accessible position floats, a backed vehicle index,
ring index 0..24 and valid ring handles when wrapped. The terrain helper's
successful output must provide the three hit coordinates. Finite values and
valid helper/asset lifetimes are assumed; full-game callers are not validated.
No fake prototypes/callers, artificial volatile or assembly were used.
Only compile/scoring was performed. Observed source improvement is not accepted
coverage; independent checker owns further tests, acceptance and integration.
