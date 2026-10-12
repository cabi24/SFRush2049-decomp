# Image B position initializer: inherited exact delivery

## Result

The installation-ready one-member kept group in this directory delivers
B:func_8038D054, [0x8038D054, 0x8038D1A8), 340 bytes / 85 words.
The complete body was already exact in
`cloud/work/lean/runtime_b_position_init_20261006/candidate.c`.
This packet retains that executable source verbatim, adds the group contract,
and hardens its proof against current master and the actual installer.

- Newly discovered matching bytes: **0**.
- Inherited exact candidate bytes made installation-ready: **340**.
- Preserved installed bytes: **0**. No group migration or supersession.
- Newly accepted image/ROM bytes: **0**. Keep the PR draft; installation,
  cartridge verification and merging remain with the independent checker.

Current master b37643eec, all ten B locks and open PRs were checked before
reservation. D054 is unlocked and has no competing matching PR. Already
installed B908D0/B936A8 and separately submitted D3A4 are excluded.

## Deliverable and replay

`group.c` plus `group.json` is the source-first deliverable. Every verified
member is in members, claims and keep; there is no unclaimed context.

```
python3 cloud/work/frontier/dot_runtime_b_position_initializer_20261012/verify.py --score
python3 cloud/work/frontier/dot_runtime_b_position_initializer_20261012/verify.py
python3 -O cloud/work/frontier/dot_runtime_b_position_initializer_20261012/verify.py
python3 -m pytest tests/cloud/test_dot_runtime_b_position_initializer.py -q
```

The score entry point authenticates the current-tree image B in memory, then
calls the unchanged canonical scorer in group mode, without `--claims` or
`--allow-unverified`:

```
Members:
func_8038D054:
  MATCH
    own .rodata verified at 0x80394DAC..0x80394DB8
```

The group route supplies real owned-data context rather than substituting
external floats. Current master has no standalone image-B data artifact.
No protected data, image bytes, assembly dump or compiled object is submitted.

## Source and ABI

An ordinary-o32 allocator call requests a record from the pool at 0x80395EE8.
A null result returns null. Modes 0..3 select a kind bit, lifetime and separation
offset. The input position is read and copied into four triples, in source
order, then the second coordinate of the last two is separated.

The accepted allocator at 0x8008E3C0 takes one pool pointer and returns the
removed free-list node; the source uses this genuine contract. The real FCE0
caller constructs a1 as its position pointer at caller offset 0x8E4, copies it
to a2, and calls D054 at offset 0xB00. D054 genuinely homes the unused third
argument. The receipt binds the complete caller target and checks those setup
sites and the target home. This is a native ABI observation, not execution of
the complete caller. No static ROM TU is rebuilt; stock IDO 5.3 is identified
through the shared compiler registry for this runtime-image build.

Original names, record spellings and whole-function arcade ancestry are
unknown. Valid C inputs require mode 0..3, three accessible finite input floats
and a returned record with the accessed 64-byte target prefix. Other native
modes use an uninitialized local and are not claimed valid C inputs. No new
unused locals, pressure reads, padding arrays, volatile, fabricated helpers,
false prototypes or compiler patches were introduced.

## Verification

- Complete STT_FUNC: 340 bytes, exact entry. Complete object and linked text:
  352 bytes, with 12 separately checked zero alignment bytes.
- All nine relocations resolve. Independent GNU links the whole object at
  explicit text 0x8038D054 and rodata 0x80394DAC, verifies every word, external
  binding, section and symbol extent, and rejects retained relocations.
- Complete owned literal payload: 12 bytes (three binary32 values). The full
  16-byte rodata section and four zero padding bytes are checked. Padding is
  not claimed to overwrite adjacent native data.
- The actual `ovl_splice.packet_spec` and `blob_group.member_slices/relocate`
  paths preserve the single claim and reproduce the complete native body.
  No splice, lock, production-source or image write is performed.
- Missing data fails closed. Independent corruption of each of the three
  literals is rejected by both the scorer and actual installer relocation.
  A compiled wrong-kind source is also rejected.
- 240 actual-source C89 fixtures run with strict aliasing, UBSan/bounds and
  warnings-as-errors. They cover four modes, six finite position seeds, five
  source/destination overlap arrangements and allocation success/failure;
  complete records, external positions, return pointers and allocator calls
  are compared with an independent sequential oracle. A separate 32-bit
  compile checks every accessed record offset and the 64-byte target size.
- Receipts bind source, verifier, harness, inherited source, selected target,
  genuine caller and image identities. Mutable tools, locks and assembly-text
  hashes are not pinned. Normal and optimized-Python full replay pass.
- One cheap collected test supplies the group gate. The shared conftest turns
  absent IDO into a clean skip and REQUIRE_TOOLCHAIN makes that skip fail.

The host allocator is a hook; the actual allocator, full native caller and
whole game do not execute in these behavioral fixtures. No pool lifetime,
unrestricted aliasing/safety, image/compression, hardware or ROM proof is
claimed. GNU and installer relocation are independent of the host fixtures.

## Bounded scout, not a second claim

Fresh B:C910 was examined first. Explicit O3 initially gave 58/69 words;
naming its observed player pointer restored the 64-byte frame but left 55/69.
The native setup consumes save_slot_valid's returned scene index and passes
its third argument at full width, whereas its historical accepted source
has a void return and narrow formal. Accepted save_load_data also consumes
that result through an old-style explicit-s32 declaration. The callback
record and this boundary need an authentic audit. No invented prototype,
matching claim or source submission is made for C910. Further mechanical
variants were not pursued.
