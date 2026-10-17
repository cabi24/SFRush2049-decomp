# ShotToWorldCollision: natural C exact candidate

`input_deadzone_apply` at `0x800ADD58` is the N64 ShotToWorldCollision routine: **895/895 words, 3,580 bytes**, strict stock-IDO comparison. The repeat has identical executable and relocation sections. There are no unresolved relocations, unverified own-data references, differing words, or extra nonzero words.

This is an isolated source/research candidate. Accepted image/ROM credit is **zero**. The whole-unit, image and ROM gates have not been run. No canonical source is edited by this packet.

## Source changes and evidence

The token-bounded reconstruction starts with tracked `w12a/ida/hdr_ipcorder.h` plus `best.c` (the historical 122/895-word partial), with precisely three changes:

1. Replace unused `s16 W` with a meaningful `s16 state`, assigning and testing the signed-short CylinderInSurf result. The historical and meaningful-state objects have identical executable/data sections and relocations. This is not an unused padding variable. The MAT3 partial uses an `int state`; the N64 signed-short choice is reconstructed from its callee contract and target output, not a recovered historical spelling.
2. Evaluate the two z-coordinate floor assignments in their respective quadtree lookup arguments. The x assignment still precedes each call. The assignments do not interact with the other arguments, and the local z value has no external visibility. This stage gives 115 differing words.
3. Write all eight traversal interpolations as `cur[i] + d[i] * t`, rather than `d[i] * t + cur[i]`. This ordinary source evaluation-order change closes the remaining differences. Interpolation alone leaves 12 words. No identity multiplication, volatile access, dummy operation, inline assembly, compiler patch, Ucode edit, register oracle or target-byte substitution is used.

The exact result was found after comparing the two compiler phases: the earlier z/end-z persistent-register conflict and the floating temporary ring were separate effects. Earlier Ucode-only diagnostic experiments are not part of this packet or its proof.

MAT3's ShotToWorldCollision reconstruction is itself a rejected partial, useful for structure but not an exact source donor. The match claim here comes solely from the N64 source replay. The host check covers finite inputs; it makes no statement about arbitrary NaN payload behavior or portable equivalence on every floating-point implementation.

## Complete preserved group

`group.json` preserves all six existing `func_800AD4C8` members:

- `func_800AD650`, `handbrake_apply`, `func_800C36A0`
- `camera_trigger_check`, `camera_victory`, `func_800C3AD0`

It adds `input_deadzone_apply`. All seven member claims and all 17 strict context functions pass. The inherited `func_800AC9BC` remains 52/56 words different and is not claimed. The inherited unrelated C36A0 stand-ins remain explicitly visible.

`replay.py` materializes context from canonical tracked sources. The only changes inside the existing group are the authentic CylinderInSurf parameter order, deletion of the three obsolete AD4C8/CylinderInSurf stand-in bodies, and giving the existing unkept `lsq` helper external linkage to retain its own deleted-procedure symbol boundary. The six member bodies are unchanged. Restoring static linkage leaves one extra nonzero word after `effect_cleanup`, which strict closure rejects. It is not permitted to slice this residue away.

Additional caller/callee files are copied or reduced to their real bodies with the repository's normal blob-unit transformer. No new stand-in caller is added. The recipe does not depend on the separate carbody PR.

## Replay and controls

With the repository's stock IDO and MIPS binutils configured:

```
python3 cloud/work/frontier/dot_mat3_shot_match_20261017/replay.py --check
python3 -m pytest -q tests/cloud/test_mat3_shot_match.py
```

The materialized group can be inspected separately with `replay.py --materialize <empty-directory>` and scored with `tools/cloud/score.py group <directory> --claims`.

Fresh compiler controls:

| Source | Differing root words |
| --- | ---: |
| historical partial | 122 |
| meaningful signed-short state only | 122 |
| z argument evaluation only, with state | 115 |
| interpolation order only, with state | 12 |
| complete candidate | 0 |
| 32-bit state in complete candidate | 276 |
| unsquared radius in complete candidate | 29 |

The separate static-helper control fails closure even though the root remains exact. All normal controls retain exact context. The proof includes both successful full-closure passes and every failed control, not merely the best score.

The host harness runs 70,898 finite differential cases against the historical body, exhausts all 65,536 signed-short returns, exercises traversal faces/corner ties, filtering, extended list offsets, both snap branches, and normalization. Undefined-behavior sanitization is fatal. An intentionally unsquared radius must also fail the same semantic comparison. These are bounded supporting checks, not substitutes for the strict N64 proof.

The receipt contains source/recipe/prepared-context/target hashes, full scores, output-section hashes and function boundaries. It contains no ROM bytes or raw assembly. Independent review is recorded separately once complete.
