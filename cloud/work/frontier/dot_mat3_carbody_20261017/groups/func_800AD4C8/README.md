# Car-body surface collision: strict cloud candidate

Population: game blob. Historical symbol `camera_play_script`, address `0x800C5644`, 3,520 bytes. The name is misleading; this is car-body versus surface collision.

## Deliverable and result

The same-named `func_800AD4C8` group supersedes the existing group in place. All seven members strictly match, including the six preserved locked members. Six additional locked callee definitions are byte-identical copies used as context. No canonical source, lock, target, pipeline or linker file is edited.

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_mat3_carbody_20261017/groups/func_800AD4C8
func_800AD650: MATCH
handbrake_apply: MATCH
func_800C36A0: MATCH
camera_trigger_check: MATCH
camera_victory: MATCH (own .data verified at 0x801174DC)
func_800C3AD0: MATCH
camera_play_script: MATCH (own .rodata verified at 0x80123F8C..0x80123F90)
```

The packet's `proof.json` contains the full source-bound replay, not only a claims-only score. `independent_review.json` independently binds every C input and group recipe. New individually strict candidate: 3,520 bytes. Accepted image/ROM coverage: **0 new bytes** until coordinator gates pass.

## Provenance and source-shaping disclosure

The N64 draft derives from `cloud/work/frontier/w18d/camera_play_script/{prefix,body}.c`. The 2026-10-17 MAT3 read-only lookup identified the matched reconstruction `stree.c::CollideCarBodyAgainstSurface`, `0x800472A0`, 2,280 bytes. Its `stree.c` SHA-256 is `2d82b2803044fcde7b8a0f36be1b44620bdd869575e851ab1f89f2bec66e8c26`. This is matched reconstructed source, **not recovered historical original source**. No donor binary or raw assembly is included.

The donor reuses its first edge endpoint index in both four-corner loops. Applying that supported reuse to the N64 draft closes the former allocation residual. The direct donor cross-product spelling and its `dy, dx, dz` order do not reproduce the final N64 word order and are not asserted as N64 original spelling.

Two uses of `ex * 1.0f` make `ex` an expression before stock IDO folds the identity. Removing both leaves one commuted `mul.s` word out of 880; the natural one-word variant is generated and preserved as a rejected control. The exact candidate emits no extra multiplication. This is an explicit source-shaping device, not donor evidence or a general IEEE-754 identity assertion. No universal equivalence is claimed for signaling NaNs, exception flags, non-default rounding/flush modes, or a different compiler. The actual stock-IDO target output is word-identical. Mixed `0.f`/`0.0f` spellings and named locals are inherited from the w18d reconstruction. The native frame remains 608 bytes. No padding locals, false prototypes, new fake callers or compiled-out reads were added.

The existing `lsq` helper is made non-static and left unkept, preserving its body and all member bytes. This gives its 8-byte deleted-procedure residue a named boundary instead of appending one nonzero extra word to `effect_cleanup`. Restoring `static` is a packaging-negative control: the seven members still match but the locked callee's extent fails. This follows the existing Wave 17 group-packaging convention. The coordinator should keep `lsq` internal if the whole-unit process requires an explicit override; it is not a retail match claim.

## Closure limits

Preserved members: `func_800AD650`, `handbrake_apply`, `func_800C36A0`, `camera_trigger_check`, `camera_victory`, `func_800C3AD0`.

Unchanged copied callees: `effect_cleanup`, `func_800C54F0`, `func_800A61B0`, `math_utility`, `func_800B61A8`, `func_8009E820`. All six strictly match in this group.

Existing context `func_800AD4C8`, `input_process_controller`, and `func_800AD5D0` also strictly match. Historical `func_800AC9BC` remains 52/56 words different and is not claimed. The five inherited stand-in callers are unchanged; the unresolved wider historical closure is not presented as newly solved. Full-unit compatibility, integration, image identity and ROM hash are **not run**. This packet does not merge or start a CI watcher.

## Reproduce

Use the normal stock IDO 5.3 setup, including the scorer's `-r4300_mul` assembler flag. No private donor checkout is needed.

```
python3 cloud/work/frontier/dot_mat3_carbody_20261017/groups/func_800AD4C8/replay.py --check
python3 -m pytest -q tests/cloud/test_mat3_carbody.py
```

Replay verifies pinned source/recipe/target-word hashes and symbol references; it does not pin changing tools, lock files or assembly-file text. It compiles the positive twice, checks all preserved and added strict functions, and rejects five controls: natural one-word source, separate corner index, donor-first `dy`, wrong surface depth, and unbounded static-helper residue. A host differential test includes the exact root body and compares it with the natural control on 1,024 finite geometries, checking full car/contact state and callback ordering: 180 contacts, 274 callbacks. This is bounded supporting evidence, not an emulator test or floating-point exception proof.

One packet test runs in under five seconds here. No ROM bytes, raw disassembly, object files, credentials or scratch experiments are published.
