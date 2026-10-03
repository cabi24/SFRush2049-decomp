# Trailing resource directories and a loader-selected boundary

Research/tooling only. Zero accepted C bytes, no matching, original-TU,
recompression, whole-ROM identity or completeness claim. Independent from the
active V01/census and runtime-overlay target/scorer work. No raw assets or native
instruction arrays are included. Only this packet and its synthetic test file
are changed.

## Result

The previously observed output-minus-first-word differences **48, 72 and 96 are
trailing-directory sizes, not header sizes**. The output begins with two
big-endian 32-bit words: directory offset and entry count. At that offset are
`count` records of 12 bytes: four-character tag, payload-relative offset, and a
**tag-dependent quantity**. In all 64 original candidates the directory ends
exactly at EOF. The preceding payload intervals begin at offset 8 and are
8-byte aligned.

- 38 six-entry directories: `IMAG TXLD OBHD PLHD TXHD OBJS`
- 7 eight-entry directories: those six followed by `PATH PTHD`
- 19 four-entry directories: `WHDR WOBJ GTLD GDAT`

Those are literal tags, not a claim that complete image/model/audio formats
have been reconstructed. A conservative parser validates their directory
bounds, tag uniqueness, monotone offsets and half-open payload intervals.
It deliberately rejects layouts outside that profile; rejection means
unsupported by this probe, not impossible for the native loader.

## Native consumer evidence

Independent inspection of the pinned game image and tracked targets establishes:

- `func_80096C28`: reads words +0/+4 as directory offset/count, rebases directory
  and each entry's +4 offset with the payload base at stride 12. A high-bit
  already-relocated pointer follows a separate path. The offline parser accepts
  only unrelocated offsets. Native routines themselves do not enforce bounds.
- `func_80096B00`: searches +0 tags at stride 12 using the signed count.
- `lookup_with_output` (`0x80096B5C`): returns entry +4 and optionally writes entry
  +8; absence returns zero and writes zero to the optional output.
- `func_80096CA8`: requests `OBHD/TXHD/PLHD/PTHD/TXLD/IMAG`; loop advances establish
  element sizes OBHD=88, TXHD=36, PLHD=24 and PTHD=36 bytes. TXLD's quantity is
  used as a byte length. These are consumer-backed units, not generic sizes for
  every tag. Existing source work under `ipa-groups/audio_frame_sync` predates
  this packet; it is not a newly reconstructed consumer.

For all 64 candidates, the next offset equals the current offset plus the
quantity times the tested stride, rounded up to 8. The other tested strides
(IMAG/OBJS/PATH/WHDR=1, WOBJ=104, GTLD=4, GDAT=28) are **empirical only** here.
The report calls them empirical for every tag to avoid over-promising schema
certainty. Inferred padding is a length, not proof that its bytes are zero.
The parser reports stride disagreement rather than interpreting an unchecked
quantity as a byte length. It does not validate pointers nested inside records.

## Boundary resolved against an actual loader input

At the ambiguous census neighborhood, all three starts terminate at `0x399363`:

- `0x36BCA5` produces 390,231 bytes, a 167-byte prefix plus aligned output
- `0x36BCAB` produces 390,083 bytes, a 19-byte prefix plus aligned output
- `0x36BCB0` produces 390,064 bytes and a valid six-tag directory

The first two fail this directory profile. More decisively, the authenticated
game image has `D_8011B5BC[60] = 0x36BCB0` at VRAM `0x8011B6AC`, and codec table
`D_80123564[60] = 2` at `0x80123654`. Native `func_80097164` indexes these tables
using the resource-slot byte at +6: codec 0 takes DMA, 1 the other decompressor,
and other values call `0x800026C0` with that ROM pointer and argument 1. It then
calls the resource consumer with first-load flag 1. Thus **the loader's type-60
input selects the aligned candidate**, adding a 65th structurally validated
container to this research set. This establishes a specific loader-table mapping;
it is not an exhaustive table extent, proof every index is live, or a claim
that the misleading prefix bytes are padding or independently meaningful assets.
The CLI refuses table interpretation if the decompressed game's SHA-256 differs.

`boundary_evidence` preserves all three starts, hashes and overlaps. The original
119-row census input is unchanged, so its summary remains 64 directories. Do not
add their compressed sizes together: the alternatives overlap almost entirely.

## Interval ledger and remaining scope

All 119 supplied candidates reach EOF at exactly their recorded compressed size
and produce the recorded output size. The 116 pre-game rows cover 7,775,811 bytes
in their union, leaving 471,773 bytes unexplained in `[0x32F1F0, 0xB0CB10)`.
The ledger partitions this scope without double counting, labels gaps unknown,
and retains all source candidates. It is not a new scanner. The separate
boundary ledger includes the earlier 0x36BCA5 start; it does not silently change
the original census's main ledger.

The complete outputs at `0x333B10` and `0x334180` remain hash-identical. They do
not pass this directory profile; duplicate identity is independent of parsing.
Unknown candidates and gaps remain research leads. Directory/consumer recovery
now justifies the next narrow work: validate internal record fields and sparse
loader associations, then investigate unexplained intervals. Rendering or codec
labels would still require independent evidence.

## Provenance and reproduction

Base: master `f82c5204edc4ead0f9056f323ff71122209ff259`.
Census source: `cloud/work/r16_rom_stream_census/streams.json` at
`4f2514d804a4a1bd93cd0329c2568752df43b6a4`; metadata copied without changes.
Existing native target/consumer source references:

- `asm/us/blob/blob_800966d8.s` (lookup/init/resource-consumer/loader targets)
- `cloud/work/ipa-groups/audio_frame_sync/group.c` and `STATUS.md`
- `symbol_addrs.us.txt` (address labels, not independent semantic proof)
- `tools/compose_data.py` (segment mapping)

Local input: `assets/us/data.bin`, 12,517,376 bytes at ROM base `0x10000`, SHA-256
`c348ea04768321ca2f864195ad6013a8fb15b59f0295ab74b8b16317cfa77154`.
Game SHA-256: `bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d`.
This authenticates supplied segment/image identities, not a complete retail ROM.
`consumer-evidence.json` records six independently inspected native-region hashes
with end-exclusive ranges; no native bytes are exported.

From repository root, with an existing authorized local input:

```sh
python3 cloud/work/asset_container_structure/probe.py assets/us/data.bin \
  cloud/work/asset_container_structure/streams.json --out /tmp/asset-manifest.json
cmp /tmp/asset-manifest.json cloud/work/asset_container_structure/manifest.json
python3 -m pytest tests/conveyor/test_asset_container_structure.py
```

The manifest is deterministic metadata only. No payload extraction command is
provided. Python standard library, version 3.9+, suffices for the probe; pytest
is used by the synthetic tests. Decompression has a fixed 8 MiB output ceiling.
Tests cover malformed/truncated headers/directories, relocated pointers, negative
count representation, huge quantities, zero-length entries, unknown tags,
alignment, profile mismatch, exact EOF/trailing/truncated/bomb inputs, overlapping
accepted candidates, gaps, duplicate/out-of-range stream rows, and manifest
invariants. A synthetic filter-order model preserves the prior rejected-stored-hit
scanner finding without importing/editing Claude's scanner or claiming it fixed.
