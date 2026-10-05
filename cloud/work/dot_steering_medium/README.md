# Complete steering caller reconstruction: NONMATCH

Base and independently rechecked remote: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.
Research only. No coverage, splice, whole-image, or full-ROM claim.

## Useful outcome

`steering_sensitivity` at `0x800AD128` is 928 canonical bytes. A complete,
natural reconstruction in its genuine existing call group now emits exactly
928 ELF function bytes with the original 104-byte stack frame. Strict comparison
is **106/232 words different**, with zero excess words, unresolved symbols,
unverified relocations, or scorer errors. This is a substantial nonmatch, not
an allocation-only near-match and not an acceptance candidate.

The existing A12/w2g context source independently reproduced 230/232 differences
and 19 nonzero excess words. Its actual ELF extent is 1,008 bytes (252 words,
including its final zero instruction), with a 176-byte frame. Historical reports
of 251 emitted words described the nonzero tail rather than the full ELF extent.

All four real context bodies are preserved byte-for-byte as source from
`src/blob/groups/frontier_traction_control/group.c` at the pinned base. Fresh
strict results are:

- `vector_diff_process`: 0/20
- `traction_control`: 0/248
- `func_800A61B0`: 0/37
- `math_utility`: 0/19

Their existing source bodies and acceptance state were not edited. The new
rotation proofs for `func_800ACFF8` and `func_800AD090` establish their ordinary
float/float/matrix contracts; those functions remain external here and receive
no duplicate credit. No synthetic caller or callee is present.

## Why this is new work

The committed w2g result explicitly says steering was retained as unchanged
unmatched A12 context while traction was rewritten. The accepted traction source
now establishes the real 132-byte path-record layout and the actual internal
`vector_diff_process(origin, basis, position, out)` formal order. The separate
rotation work removes both remaining source-contract blockers. The old steering
body was still m2c-shaped, with spill assignments and numerous pass-through
floating variables, so this pass reconstructed the actual full caller instead
of repeating its old controls.

The authentic retail callers are `input_deadzone_apply`, `camera_trigger_check`,
`camera_victory`, and `entity_update`; call offsets are in `selection.json`.
The six-input interface, including the existing unused first carrier, is
retained from its actual entry homes and calls. No new formal was added.

## Source and evidence

`group.c` contains the complete new steering body plus the four unchanged real
context bodies. The `PathRec` field names are inherited interpretations by use,
not established semantics for every field in this different routine. No arcade
ancestor is asserted. The local arcade reference checkout was unavailable; the
existing w2g source records this subsystem as N64-specific with no found donor.

The routine selects the next 132-byte record with wrap, copies its matrix,
transforms the incoming position, clamps the forward coordinate, interpolates
its actual scalar fields, and computes side/vertical displacements. It preserves
the actual matrix-flip branch, early output writes, square roots, threshold test,
and the later conditional row rotations. All arithmetic and memory operations
come from the complete native body. The final source does not add pressure
operations, unused locals, guessed arrays, alignment helpers, volatile fields,
inline assembly, or external context stand-ins.

Twenty complete controls, including the old baseline, were replayed. They cover:

1. A native typed body replacing m2c field casts and spill assignments.
2. Reuse of actually consumed geometric values across their existing phases.
3. Capture of the current segment length before its real clamp.
4. Correct retention of the same current length in sine/cosine arguments.
5. Declaration order anchored to observed consumed homes: local vector at 52,
   clamp/slope value at 68, side at 76, width at 80, radial distance at 84, and
   next-record pointer at 96. This changes the placement of real locals only.
6. Placement of the genuine side/vertical calculations relative to the actual
   interpolations, branch-local vertical calculation, and field snapshots.
7. A shared consumed quotient in the two interpolation expressions; the
   compiler performs its ordinary common-subexpression elimination.

These are a bounded source reconstruction, not an automated permutation sweep.
The controls and their exact body snippets are retained in `controls.json` and
`variants/`. `replay.py --all` freshly compiles each against the same real context
and reproduces every recorded canonical result. The final context source hashes
are checked before any compile. Raw assembly and objects stay in ignored build
scratch, never in this packet.

## Remaining blocker and next evidence needed

The first integer setup and real call sequence are restored, and the emitted
length/frame now agree. The FP values still occupy different webs and some loads,
spills, and comparisons are scheduled differently. Fresh workbench diagnosis is
`structure-mismatch`, with 25 aligned structural and 82 aligned register rows;
its geometry comparison is explanatory only. Its relocation-symbol warnings
come from comparing a raw target object to a relocatable object and are not used
as acceptance evidence. The canonical strict result resolves every relocation
and remains 106/232.

Do not start a line/declaration/register sweep. A useful continuation needs
specific evidence for the original scalar value lifetimes or source expression
structure, especially the initial length/fraction, height/vertical pairing,
and later slope denominators. Alternatively, a genuine neighboring inlined
source body would need independent evidence before being introduced. No such
missing helper was established here. Current bounded controls are exhausted.

## Reproduce

On an IDO-capable host with the pinned local toolchain configured:

```sh
python3 cloud/work/dot_steering_medium/replay.py --all
python3 tools/cloud/score.py group cloud/work/dot_steering_medium
```

The first command succeeds only when it reproduces the explicitly expected
nonmatch and all four matching context functions. It is not an acceptance gate.
The second command reports the research member as a nonmatch. `claims` is empty.
`verification.json` records full function extents, compiler/source/manifest
hashes, strict results, and the verified limitations.

## Other selection result

The separate 1,248-byte `sound_bank_unload` body is really palette construction.
Its three real calls to the 192-byte RGB5551 interpolator `func_800B0EA0` offer a
credible real-caller closure, unlike that helper's old isolated A42 attempt.
This was handed to the coordinator and reserved by another worker; no palette
source is reconstructed here.

`camera_track_spline` was rejected as a fresh singleton despite the frontier's
hint: it receives its real pointer through `a3`, and its existing camera group
already documents the unresolved IPA parameter allocation. Repeating that old
packet without new caller evidence would not be new work.
