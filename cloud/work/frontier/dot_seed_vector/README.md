# Collision callback: func_8010C02C

Strict source match at `0x8010C02C`: **174 / 174 full relocated words, actual ELF function size 696 bytes**. Standalone `-O2`, standalone `-O3`, and an `-O3` group with all three real direct callee definitions each pass. No relocation masks, unresolved references, unverified data references, or nonzero extra instructions.

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`, verified as `origin/master` at the start of this work on 2026-10-05. The candidate is not in the accepted lock. This packet does not splice anything, change context or protected files, or claim cartridge coverage. Full-program shadow, image, compressed-stream and ROM-hash gates remain integration work.

## Source evidence

The native body selects one model using `*index` (signed 16-bit; stride `0x808`) and a dead-reckoned collision record using the signed byte at object offset `0x65` (stride `0x40`). It returns immediately when model byte `0x7EA` is zero. Otherwise it tests the squared center distance against the square of the sum of radii, transforms up to four corners, and calls the collision-force routine on the first point inside all six inclusive bounds. All paths return zero.

The three callees are accepted sources, read without alteration:

- `src/blob/func_8009E820.c`: body-to-world three-vector transform
- `src/blob/func_800A61B0.c`: world-to-body three-vector transform
- `src/blob/random_float.c`: the historical name of the corner-collision-force routine, with `(s32 unused, model, reckon, direction, point)` parameters

The force call forwards the model address twice, followed by the selected record and the actual delta/local vectors. Those five outgoing arguments are native behavior, not synthetic formals. The callback also homes incoming `a2` and `a3` without using them. Their slot presence is proven; their precise high-level types are not. No direct `jal` callers were found in the integrity-checked target set, so this packet does not invent a callback registration or rename the function.

The broad algorithm is related to [arcade collision.c](https://github.com/historicalsource/rushtherock/blob/master/game/collision.c), but that source is not an exact donor: this N64 callback handles one selected pair, uses reordered XYZ bounds, and tests only the selected model's four corners. The arcade source was inspected privately and is not copied into this packet.

## What closed the prior lead

The old `heads_B14/func_8010C02C_seedvectors.c` was an m2c-shaped, 157-word-different lead with an unprototyped force call. A compact typed rewrite recovered the entire integer register assignment and ordinary loop structure. A plain `float world[3]` still caused three stack reloads to disappear and changed the FP register sequence. Named components in a real three-float union, with an array view for the genuine transform calls, reproduce those reloads naturally. This is not a volatile workaround; a volatile diagnostic changed address bases and register assignment and was rejected.

A named, used `radius_squared` calculation and placement of the existing model-index declaration before the vectors reproduce the 168-byte frame and vector homes. Every local is read; there are no unused locals, extra buffers, fake arguments, inline assembly, or helper keepers. The unknown byte arrays in record types describe native field offsets and record extents, not stack-frame padding.

## Reproduce

From the repository root with the pinned IDO 5.3 and MIPS tools configured:

```sh
python3 tools/cloud/score.py fn cloud/work/frontier/dot_seed_vector/func_8010C02C.c func_8010C02C --flags '-g0 -O3 -mips2 -G 0 -non_shared'
python3 cloud/work/frontier/dot_seed_vector/verify.py
python3 -m pytest -q tests/conveyor/test_seed_vector_match.py
```

`verify.py` validates target-manifest integrity via the unmodified scorer, checks the exact ELF `STT_FUNC` extent independently, resolves every relocation over that full extent, and requires word-for-word identity without masks. It then copies the three entire locked callee source files, validates their hashes against the lock, and rebuilds a genuine direct-callee group. This is a target-only group check, not a claim that the complete game unit or all callee data has been reverified. Native-layout compile assertions confirm the `0x808` model, `0x40` record and pointer-dependent field offsets. The standalone text section has two zero alignment words beyond the 696-byte symbol; they are reported separately.

The host harness runs 1,033 contract cases: disabled and negative-nonzero flags, signed record indices `-128/-1/0/127`, all four first-hit positions, all six inclusive faces and points just outside them, sphere equality and rejection, NaN comparison behavior, translations and nonidentity bases, plus 1,000 deterministic varied cases. It verifies transform counts, early stopping, zero return, all five force-call arguments and both vector payloads. Host tests check semantics; native offsets and instruction identity come from the IDO checks. Negative controls prove that the verifier rejects a wrong relocation target and a wrong actual ELF function size.

`verification.json` is bound to the exact candidate source hash. Only source, tests and hash/count metadata are retained here. ROM bytes, disassembly, compiled objects and private diagnostic variants are excluded.
