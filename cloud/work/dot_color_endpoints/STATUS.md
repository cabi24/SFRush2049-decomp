# Complete RGBA endpoint setter: strict match

Base: `cabi24/SFRush2049-decomp` master
`2f1f30c508495db08d61170c4a9b2409461ae326` (2026-10-02).

## Result

`func_800BEA3C`, `0x800BEA3C..0x800BEA58`, is a complete **7-word / 28-byte**
strict match. The canonical scorer and a separate clean-directory compilation
both resolve every instruction exactly, without masks, unresolved or unverified
relocations, or relocation errors. The ELF function is 28 bytes; one trailing
zero word aligns the 32-byte text section and receives no matching credit.

The submitted source is `cloud/matches/func_800BEA3C.c`. It is compiled alone:
no helper implementations, caller context, group recipe, or other functions.
Its literal recipe is `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` with
the approved, project-pinned IDO 5.3 static-recomp v1.2 installation.

## Source and type evidence

The preserved `cloud/work/game_C32/func_800BEA3C.c` research candidate models
both arguments and globals as `u32`. Its freshly reproduced result remains
**4/7 differing words**: its scalar interface omits both native argument-home
stores. The historical source, documentation, and copy in draft research PR #9
(head `433611408270ede7fe6acdb027f1b82179209511`) remain unchanged.

The accepted `src/blob/dispatch_handler.c` supplies the missing data context.
Its source hash agrees with the accepted image-gated lock, and it independently
recompiles to strict MATCH. Its complete 257-word native body was reviewed:
mode 22 reads all four unsigned bytes of `D_80118E28` and `D_80118E2C`, blends
corresponding components using `D_8017A630`, and passes the resulting RGBA
channels to `func_800B7360`. It already calls the four-byte type `Color4`.

The setter is therefore reconstructed with two `Color4` values passed by value
and two aggregate assignments. IDO naturally emits both native homes at entry
`sp+0` and `sp+4`, alongside the exact full-word global stores. There is no
frame, call, return-value computation, narrowing operation, or invented input.
The four unsigned-byte fields are at offsets 0, 1, 2, and 3; the type is exactly
four bytes. A host C89 check verifies that layout and 4,096 paired color copies.

A direct jump/call/branch scan covers all 1,216 protected functions / 140,252
words and all protected region words. No direct caller of this setter was
found. This does not rule out indirect or opaque-region references, and the
original source-level typedef spelling is not recovered. The aggregate
interpretation is supported by the complete setter, independently accepted
consumer, and ordinary compiler ABI, rather than a fabricated caller.
Future generated declarations must preserve this aggregate interface.

No arcade equivalent has been established: the primary arcade reference tree
is unavailable in this checkout. The visible purpose is N64 rendering state.
There are no volatile accesses, artificial stack locals, helper stand-ins,
extra parameters, dead operations, target edits, scorer changes, or raw
instruction insertion.

## Reproduce

```sh
python3 tools/cloud/score.py fn cloud/matches/func_800BEA3C.c func_800BEA3C \
  --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

Expected output:

```text
func_800BEA3C:
  MATCH
```

`verification.json` records the native/source/compiler/scorer hashes,
independent replay, type/consumer audit, provenance, and final checks.

## Integration limits

This is a draft source contribution, **not accepted cartridge coverage**.
No accepted C, lock, target, generated context, build wiring, scorer, or
coverage total is changed. Current master has no accepted lock for this
function and no other current cloud claim was found. Open matching PRs
#10–12 cover different functions.

The original ROM, full extracted game image, and derived `build/blob_layout.json`
are unavailable. Source-image splicing, compressed-stream identity, full-ROM
SHA-1, and `make test` have not run. Read-only source-hash checks do not replace
those gates. Live LAN coordinator ownership must be rechecked by the integrator
before the normal image/ROM/lock promotion gates.

## Final validation

- Cloud/scorer suite: **840 passed, 27 subtests passed**, exit 0 (235.18 s)
- CI-style repository suite: **1299 passed, 41 skipped, 9 deselected**,
  exit 0 (57.17 s)
- All **160 static locked functions** intact
- Existing blob/group source-hash checks: **zero problems**
- C89 syntax, four-byte layout, and **4096 paired RGBA copies**: pass
- All **23 protected manifest files**: exact hashes
- Changed-submission strict scorer and protected-path guard: pass
- Fresh accepted `dispatch_handler` strict replay: **257/257 words exact**

Commands for the aggregate suites:

```sh
python -m pytest tests/cloud tests/conveyor/test_cloud_score.py -rs
python -m pytest tests/conveyor -q -o addopts='' -m 'not node_required'
make check-matched
```

The pinned repository submodules and already-approved test/compiler environment
were reused. No unrecognized executable or compiler change was introduced.
