# Image B nearest-target steering: lean research

Target: `B:func_8038DDDC`, `[0x8038DDDC,0x8038E088)`, 684 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result and reproduction

Standalone IDO 5.3 O3: **14/171 words differ**, no extra words,
unresolved symbols, unverified relocations or errors with authenticated data.
The initial complete source differed in 158/171 words plus 22 extra.
Nesting the real eligibility/cone/nearest checks instead of continuing from
each rejection preserves one common loop-end count read and removes the
extra duplicated end-pointer arithmetic. Residuals remain in local offsets
and register/scheduling choices; this is NONMATCH research.

```
python3 cloud/work/lean/runtime_b_nearest_steering_20261006/reproduce.py
```

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The helper authenticates the frozen native asset/image B in memory and
supplies required own-data to the unchanged canonical scorer. Float literals
at B:0x80394DE4..0x80394DF0 decode to -0.04, 0.04, 0.03 and -0.03.
No raw bytes, assembly dump or object is included.

## Source and assumptions

Walks the live player array, rejects the owner/same-team and disabled or
inactive players, transforms each displacement into the object's frame, and
selects the nearest candidate within the forward cone and a 2,000-unit limit.
A selected target's horizontal/vertical signs choose small yaw/pitch steps.
Next loop bounds read the live signed-halfword player count, as in native.

Existing transform and rotation contracts remain external. Player and vehicle
record strides are the native 952 and 2,056 bytes, with explicit accessed field
views; unknown bytes represent real record storage. Original names, full record
semantics, local declarations and whole-function arcade ancestry are unknown.

Valid C inputs require nonnegative backed player counts, valid owner/player
indices and team/vehicle tables, valid object transform/position and finite
coordinates. The best vector is consumed only when a candidate was selected.
The comparisons preserve the native rejection predicates; no arithmetic
reassociation or nearest-distance shortcut was introduced. No padding/pressure
locals, fake helpers/callers, artificial volatile or assembly.

Only compile/scoring was performed. Observed scores are not accepted coverage;
independent checker owns further validation, acceptance and integration.
