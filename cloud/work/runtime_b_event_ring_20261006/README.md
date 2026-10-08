# Image-B event-ring producer: 396-byte complete match

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

B:func_803914B4, [0x803914B4,0x80391640), 396 bytes / 99 words.
Natural standalone IDO 5.3 C at `-g0 -O3 -mips2 -G 0 -non_shared
-Wab,-r4300_mul` strictly matches every relocated native word.
Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

This is a source-match submission, with zero newly accepted or cartridge-
coverage bytes. Image/compression/full-ROM integration and merging remain with
the independent checker. Image B is the stream at ROM 0xB6FEC4, SHA-256
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
The overlapping image-A address is not the same function.

## Contract and source

Three signed-byte ordinary O32 arguments are row, column and delta. For negative
values, the real accepted 800F7E30 helper updates the signed-byte score matrix
first. A zero byte at 80146130 then suppresses the animation. Otherwise the
signed player byte at 80152818 + row*952 + 0x3A3 is tested; a nonpositive value
is cleared and returns. Nonnegative deltas bypass those gates entirely.

The producer reads the old signed ring index, stores its increment and rereads
it signed, resetting it at >=4. The old index selects a 24-byte record. It writes
active=1, age=0, row at +3, column at +2, delta at +20, and four floats at
+4/+8/+12/+16. Those are signed32-to-binary32 conversions from two tables with
four coordinate pairs per row. Signed16 selector 80151AD0 minus one chooses
the table row; column indexes 80115F28 and row indexes B:803940D0. The source's
ordinary multidimensional arrays preserve native -32/-28 displacements.

B:803936A8 independently witnesses every field, walks exactly four 24-byte
records from 80395E70 to 80395ED0, interpolates the coordinates and eventually
applies a positive delta through 800F7E30. The source names describe this local
behavior. Original names and arcade ancestry are not established; the arcade
reference checkout is absent. Existing service-pair research covered only the
+1 route. This packet also reconstructs the full negative path.

Accepted 800F7E30 source has SHA-256
`616d94533ad40b0e8352ad54868475739a3d7fc78080a635984e5b3e8d12dd91`
and a genuine three-signed-byte contract. Accepted effect_cleanup at 800C55E4
passes the same three arguments when its mode is 4 or 6. Six direct game call
sites are freshly enumerated: effect_cleanup at 800C562C, camera_play_script at
800C60B8, entity_update at 800C6FAC, and func_800E0B20 at
800E0FF8/800E1084/800E1120. The first caller and helper are source/lock/native-
body bound. This is not a complete runtime residency or all-caller validity
proof. Loading image B and keeping it resident is an external invariant.

The initial natural source was a broad O2 nonmatch (376-byte function,
384-byte padded text). Changing only the ordinary optimization level to O3
produced the full match. The compiler recipe is not a fabricated IPA closure:
no added caller, pressure local, artificial ABI, assembly or special compiler
option is present. O1 and the natural signed-byte index/point-struct spellings
were briefly rejected controls before the plain O3 result; no broad search was
needed. Workbench diagnosis preceded those controls.

## Verification

`verification.json` binds source, proof, host harness, tests, inputs, compiler
binaries, helper source/native bodies, consumer and all external addresses.

- Complete ELF function size: 396 bytes. Complete text: 400 bytes, including
  one zero alignment word outside the function. No owned data or literal pool.
- All relocations resolve without masking. GNU links the unmodified whole
  object at 803914B4 and agrees with the project relocator and complete native
  body. The proof enumerates every relocation and address binding.
- IDO 32-bit layout assertions establish the 952-byte player record and its
  +0x3A3 signed byte, and the complete 24-byte event layout.
- 2,560 fixtures run protected native, project-relocated and GNU-linked bodies,
  both with real native-helper register results and adversarial caller-save
  poisoning after the helper. The 64-byte accepted native helper is executed,
  rather than replaced by an abstract outcome. All 99 target instruction offsets
  and nine reachable branch outcomes are exercised.
- Every fixture checks complete mapped-state preservation, helper arguments and
  absence of global writes before helper entry, stack bounds and callee saves.
- The unchanged source also passes 2,560 C89 UBSan/bounds host cases with a
  separate helper definition. Six compiled semantic mutants are rejected;
  unknown MIPS instructions fail closed.

Domain: row, column and old ring index 0..3; selector 1..4 with all four table
rows backed; signed-byte delta and counter values (all 256 covered); stable,
nonaliasing allocated storage; usual round-to-nearest binary32 conversion.
Fixture coordinates include signed32 extremes and the 2^24 precision boundary.
The native pointer/table code has no bounds checks; invalid indices are not
invented as legal C inputs. The proof does not establish that gameplay always
satisfies the domain, universal residency, aliasing, concurrency, FCSR/exception
state equivalence, hardware behavior, or helper consumer end-to-end gameplay.

## Reproduce

From an exact base checkout with IDO and GNU MIPS binutils available:

```
python3 tools/cloud/score.py fn cloud/matches/ovl_b/func_803914B4.c func_803914B4 \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared' --targets asm/us/ovl_b
python3 cloud/work/runtime_b_event_ring_20261006/verify.py --check
python3 -m unittest discover -s cloud/work/runtime_b_event_ring_20261006 -p 'test_*.py' -v
```

`RUSH_REPO` may name the pinned checkout supplying canonical tools and protected
inputs when this packet is in a source-only directory. `IDO_DIR` and `TMPDIR`
may name installed tools and writable scratch storage. Frozen replay records
tool-hash drift rather than hiding it. Raw native instructions, objects and
private image bytes are neither packet contents nor publication artifacts.

Independent review is required before draft publication. No production,
protected target, lock, scorer or shared header changes. No CI watcher.

## Integration-portable replay (2026-10-06)

Scorer, whole-manifest and accepted-context digests are historical provenance,
not live-tree requirements. The verifier normalizes only enumerated provenance
fields on both receipt sides. Packet source and verifier bindings, compiler
identity/actual flags, selected native bodies and addresses, complete emitted
extents, relocations, owned data and behavioral checks remain binding.
The existing bare O3 header is unchanged; the canonical scorer still adds
`-Wab,-r4300_mul`, recorded as actual compiler provenance. The test wrapper
is deliberately excluded from `packet_sha256`.

### Host-tool portability

The receipt records host GCC and GNU linker binary hashes as provenance, not
frozen proof inputs. Pinned IDO hashes, packet/source hashes, native words, ELF
extents and relocations, GNU-linked text bytes, and all behavior/mutation checks
remain binding. A host tool change must pass those same checks.


### Exact GNU placement

The GNU linker script gives `.text` the explicit address `0x803914B4` and
uses `SUBALIGN(4)`. Assigning the location counter before an unaddressed
`.text` section lets GNU 2.42 advance the section to its input alignment;
this function begins four bytes past a 16-byte boundary. The linked function
and section address checks remain exact, as do all 99 native words, the
396-byte function extent, 400-byte complete text, one zero padding word,
relocations, external bindings, and absence of owned data.

Packet regressions exercise different GNU file/header layouts and scratch
input-section alignment metadata without changing the candidate source or
native target. They require identical complete linked words and reject a
rounded-up entry address, changed function/section extents, and nonzero
trailing padding. The static script regression runs without IDO; native
link tests require pinned IDO and GNU MIPS binutils and fail rather than skip
when `REQUIRE_TOOLCHAIN=1` is set.
