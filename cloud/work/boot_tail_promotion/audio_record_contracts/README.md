# Audio record and buffer source contracts

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.

This packet repairs source integration for 11 existing candidates (1,388 target
bytes), and separately proves the twelfth (112 bytes) together with an explicit
accepted-cleanup adaptation that requires a maintainer relock. It does not
promote code, add match credit, edit locks, or run the full-ROM SHA-1 gate.
The original individually locked candidate files are unchanged.

## Production-compatible changes

`include/boot_tail_audio_record.h` reconciles the two accepted partial record
views into one complete, native-evidenced `AudioState`, with `AudioSlot` as an
alias. `D_80038294` is one pointer to records, not an array of pointers. The
record is 0x68 bytes on the target ABI, including a double at 0x08, the sample
data pointer at 0x2C, and the previously accepted envelope fields.

The anonymous union at 0x20 gives the identical halfword its two existing
source names, `initial_count` and `value20`. This is a supported IDO extension
(the compiler emits its usual extension warnings). It preserves both accepted
bodies without preprocessor aliases, casts between incompatible record types,
or source-lock edits. It adds no field storage. The target compiler checks all
30 accessed field offsets, 32-bit pointer size, and the full record size. It also checks the packed 25-byte sample descriptor and
all seven field offsets.
Unknown ranges preserve native-observed gaps; no instructions or data are added
to force a match.

Production `lib_11640.c` only includes this header, removes the superseded
partial declarations, and types `D_800382D0` as a pointer to halfwords. The
35 accepted function bodies and their source hashes remain unchanged. All
original assembly slots remain in place.

Adapted candidates live under `sources/`, keeping the original locked inputs:

- `80010D74`: matches the accepted byte-pointer `D_800381F0` and accesses entry
  zero of the real `D_80038228` buffer-pointer table.
- `80011894`: treats `D_8003802C` as its accepted pointer object and converts
  the loaded pointer value to a 32-bit unsigned address before alignment.
- `80011C84`, `80013C84`, `800146B4`, `800149DC`, `80014A74`, `80014AF0`,
  `80014B3C`, `80014BB0`, `80014C18`: use the shared 0x68 record, consistent
  helper signatures, and explicit field names. The sample descriptor remains
  packed, and unsigned conversion/arithmetic and unaligned accepted parameter
  loads retain their native behavior.

## Separately gated table/cleanup adaptation

`D_800382D8` and `D_800382DC` are the two cells of one pointer table. This is
not a pointer to a pointer array. Native initialization stores separate buffer
allocations at these cells; the producer and consumer index the table with a
selector that alternates between zero and one.

A coherent `unsigned short *D_800382D8[2]` declaration requires accepted
`func_8001144C` to free `D_800382D8[0]` and `[1]`, instead of using scalar D8
and DC expressions. That changes an existing source-body hash even though
its exact 116 machine-code bytes are preserved after relocation.

Accordingly, this packet leaves that production body and its lock unchanged.
`sources/func_8001144C.c` is a separately named adaptation, used only in the
verifier's temporary `twelve_with_cleanup_relock_overlay` together with
`sources/func_80013964.c`. The maintainer must explicitly migrate the cleanup
body lock and install the coherent table declaration as part of that gated
transaction. Do not splice the twelfth candidate alone. The negative control
reproduces the scalar/table declaration failure.

`D_800382D0` remains the selected buffer pointer; it is not a table.
`D_80038228` is an independent buffer-pointer table. Its accepted unsized
`void *[]` contract is retained; freeing element zero is correct and does not
convert the table address into the allocation pointer.

## Native evidence

All paths in this section are under `asm/us/nonmatchings/rom/lib_11640/`.
The references identify repository source locations without reproducing raw
assembly or ROM data.

- `func_80011104.s`, lines 23–49: allocated record base and 0x68 initialization
  stride, including bytes 0x60 and 0x61. Lines 176–205: two buffer allocations
  stored at D8/DC.
- `func_80011C84.s`, lines 4–20: 104 times the index, loaded record pointer,
  direct record-base call to the accepted reset helper, then active byte.
- `func_80011A10.s`, lines 6–14, and `func_80011A3C.s`, lines 4–13: envelope
  halfwords at 0x20/0x28/0x48, state at 0x5C, and fields 0x4C–0x58.
- `func_80013C84.s`, lines 12–26 and 60–65: double at 0x08, unsigned words at
  0x10/0x14, active byte and explicit 0x68 traversal.
- `func_800146B4.s`, lines 18–125: independent record stride, packed descriptor
  loads, unsigned offset conversion, data and loop fields, format byte and
  flags halfword. `func_800148F8.s`, lines 5–53, corroborates 0x20–0x28.
- `func_80011C1C.s`, lines 17–29, and `func_80014A74.s`, lines 21–30:
  flags/rate and halfword fields at 0x18–0x1E and 0x40–0x46.
- `func_800149DC.s`, `func_80014AF0.s`, `func_80014B3C.s`,
  `func_80014BB0.s`, and `func_80014C18.s`: record stride and byte fields
  0x00/0x01/0x61, release count 0x28, and returned word 0x14.
- `func_80013964.s`, lines 4–31: table selection, scalar D0 assignment and
  halfword stores. `func_80013DEC.s`, lines 170–192: selector XOR 1 and the
  same two-cell table access.
- `func_80010A40.s`, lines 19–28 and 49–67: up to 24 entries in D38228,
  initialized from one allocation at 0x300-byte intervals. Lines 129–133 and
  accepted `func_80013D70` index the table; `func_80010D74.s`, lines 13–18,
  frees the loaded entry-zero value.

## Fresh complete-TU verification

`verify.py` uses the pinned IDO toolchain and the real asm-processor build path,
with the Makefile's ROM-TU code-generation flags. It never strips passthroughs
or compiles a synthetic C-only surrogate. It rebuilds these complete stages:

1. Historical baseline: all 35 accepted C bodies.
2. Current repaired production source: every currently locked C body.
3. Current source with the 11 independent pending candidates overlaid.
4. All 12 candidates plus the separately gated cleanup adaptation.

Each C function must have exactly the target's ELF function-symbol extent and
full relocated words, with zero masks, unresolved symbols, unverified
relocations or relocation errors. Independently, GNU ld resolves the complete
TU and checks all 75 native slots, totaling 17,132 target bytes. All four
stages have identical 17,136-byte linked text, including four final alignment
zero bytes. No nonempty data sections are introduced.

All 12 adapted candidates and the auxiliary cleanup are also freshly compiled
standalone and in `rom_tu.h` context; the context cannot change instructions or
relocations. Original declarations, wrong selected buffer and wrong 0x68 stride
are explicit failing controls. The JSON report contains counts and hashes,
never native bytes or assembly dumps.

## CI and later promotion

`tests/cloud/test_audio_record_contracts.py` is selected by the existing normal
Cloud tests job. This is necessary because adapted paths are outside the normal
changed-submission selector. It runs the actual-TU verifier and tests partial,
11-body and 12-body promotion fixtures, missing migrated locks, changed locked
bodies (including a forged fresh fixture lock), and undersized/oversized/overlapping symbol extents.

Current locks authorize already promoted C; only still-pending slots are
overlaid. A legitimate later promotion therefore keeps this regression useful.
Lifecycle fixtures modify only strings and temporary lock dictionaries, never
production files or actual locks. The historical baseline and negative controls
remain fixed. `REQUIRE_TOOLCHAIN=1` turns missing-tool skips into CI failures.

Reproduce from the repository root:

```sh
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_audio_record_contracts.py -q
python3 cloud/work/boot_tail_promotion/audio_record_contracts/verify.py > /tmp/audio-record-verification.json
make check-matched
python3 -m tools.cloud.guard_paths --base cf10b339 --head HEAD --lock-revision cf10b339
git diff --check
```

The maintainer still owns refreshed candidate/context evidence, any required
source-lock migrations, fresh full-ROM build/SHA-1 transaction and promotion.
Existing master CI issues are outside this repair. No remote builder, legacy
promotion driver, protected targets, symbol addresses, context records, lock
records, compiler flags or scoring implementation were changed.

## Final local checks

The packet's 11 CI tests pass. Together with the existing probe and static-lock
unit tests, the focused run passes all 37 cases. `make check-matched` reports
all 383 source-body locks intact. An additional broader scorer run found 15
unrelated existing blob functions rejected for unverified non-text relocations;
those inputs and the scorer are unchanged, and no broader-CI pass is claimed.
