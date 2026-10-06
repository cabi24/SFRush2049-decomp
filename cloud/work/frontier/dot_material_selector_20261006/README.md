# Complete model-record slot setter match

Target `func_8008B000`, **[0x8008B000, 0x8008B0D8), 216 bytes / 54 words**.
Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

**Strict MATCH at ordinary O2 and O3. Accepted-byte and ROM-coverage gain: zero.**
The candidate is [the registered single](../../../matches/func_8008B000.c).
No production source, protected input, lock, shared header, scorer, or compiler
recipe is changed. No ROM bytes, instruction dumps, objects, credentials, or
unrelated private data are included.

## New source and native contract

This is a native N64 reconstruction. No arcade ancestor or recovered original
N64 typedef is claimed. The preserved near-miss seed freshly emits a 208-byte
ELF body against 216 native bytes and differs at 53 complete-body word positions.
Its nonnegative branch falls through into a second bulk loop, and it contains
an artificial empty pointer guard. The native branches are mutually exclusive.

The recovered operations are:

- Decode a 16-bit handle as a six-bit bank and ten-bit record index. The bank
  descriptor is eight bytes and each complete model record is 88 bytes.
- A nonnegative selector at least as large as the signed count returns zero
  without a record write. Otherwise, update the selected halfword at
  `record + 24 + 16*selector`, mark bit 15 of the next halfword, and return one.
- A negative selector performs a counted bulk loop. Values move through the
  payload slots, but the marked halfword stays at
  `record + 26 + 16*selector`. For selector -1, this is the **header halfword
  at +10**, not any of the four payload flag halfwords. Nonpositive counts
  perform no record writes and return one.

The straightforward four-slot C array would index outside its array for -1.
The submitted complete-record view instead has eight prefix bytes followed by
five 16-byte spans. Span zero covers header storage; its signed tail is the
count at +22. Actual payloads are spans one through four. Indexing
`spans[selector + 1]` therefore makes selector -1 legal and still compiles to
the complete native function. This representation does not identify original
field names or imply that every span's tail is a count. The opaque members are
real bytes inside the independently witnessed record extent, not compiler
pressure or invented local padding.

Independent protected consumers establish the layout:

- The accepted `car_gear_shift` decodes the same handle and uses an 88-byte
  model stride. Its scene records are separately 68 bytes.
- `entity_render_mode` traverses four pointer-bearing payload regions spaced
  16 bytes apart and then advances to the next 88-byte record.
- `string_copy_format` builds a 16-byte name key, then searches 88-byte
  records with a comparator that examines their first 15 bytes. Consequently
  +10 lies within the header/name comparison bytes. The negative path's mark is an
  observed write; its purpose is not inferred as a normal header flag.

The sole direct JAL found across the 1,216 protected game target sections is
`physics_velocity_integrate_a` at `0x8008B92C`. Its delay slot passes selector
zero. No negative-selector gameplay use is established, and indirect or
inlined users are not ruled out. That caller belongs to a separate private-ABI
closure and remains read-only here.

## Verification

`verify.py` freshly checks:

- Exact ELF `STT_FUNC` size 216 using both the project reader and GNU readelf;
  complete independent GNU linking and comparison of all 54 words.
- Both HI16/LO16 references to the real bank table resolve, at +0x18/+0x24.
  There are no owned literals, initialized data, or BSS. Eight zero section
  alignment bytes are outside the function and excluded from the claim.
- A real shared-data consumer compilation keeps the candidate and unchanged
  accepted `car_gear_shift` (208 bytes) exact. Both GNU and the production
  group reader prove their full bodies. These separately addressed bodies do
  not recover an original contiguous translation unit or a complete caller
  closure. The consumer receives no duplicate credit and is not executed by
  the runtime harness.
- Eight separate O32 layout assertions, including the bank stride, record
  extent, header halfword, count, and first/last payload positions.
- 4,480 deterministic fixtures, with two successive calls each: **17,920
  protected-native/GNU-linked executions** and **8,960 unchanged C89 host
  executions with UBSan**. The independent fixed-offset oracle checks all
  mapped memory, record/table/stack canaries, full incoming argument-home
  writes, saved registers, and both source-visible results.
- Every reachable native instruction is exercised (53 of 54), plus both
  outcomes of all four conditional branches. The unreferenced duplicate load
  at +0x78 is unreachable between an unconditional return and the taken target.
- Five compiled wrong-contract mutants are rejected: missing else, moving the
  bulk flag, wrong record mask, wrong marked bit, and treating selector zero
  as bulk. Unknown opcode, truncated body, unmapped-table, and ELF-size controls
  are rejected rather than treated as success.

The fixtures include banks 0/1/31/63, record indices 0/1/511/1023, counts
-32768/-1/0/1/2/3/4, selectors -1/0/1/2/3/4/5/32767, five halfword values,
nonzero upper argument bits, randomized remaining storage, and repeated writes.
The source's defined domain is a mapped correctly typed record, count <=4,
and selector >=-1. Larger positive invalid selectors are safe because the
bounds check returns before computing a payload address. Counts above four,
selectors below -1, invalid pointers, asynchronous access, and whole-game
behavior are excluded. The finite tests do not prove universal equivalence.

The historical full target-manifest digest is provenance. Every replay still
verifies the current complete native manifest, and the exact selected native
body hashes, consumed symbol addresses, accepted consumer source, candidate,
compiler, and proof sources remain bound in the saved receipt.

## Reproduce

From the repository root, with the pinned IDO tools in `IDO_DIR` and GNU MIPS
binutils on `PATH`:

```sh
python3 cloud/work/frontier/dot_material_selector_20261006/verify.py \
  --output /tmp/material-selector-proof.json
python3 -m pytest -q tests/conveyor/test_dot_material_selector_20261006.py
python3 tools/cloud/score.py fn cloud/matches/func_8008B000.c func_8008B000
```

All compiler objects and native byte streams stay in temporary ignored storage.
The source single is automatically discovered by the normal changed-submission
checker. Final independent source review and all production shadow/image,
compression, and full-ROM gates remain required. No CI monitoring or merging
is performed by this packet.

## Local review checkpoint

The packet's seven fresh tests plus the selected scorer, protected-path,
input-integrity, and changed-submission suites pass: **739 tests, no failures
or skips**. Both documented scorer sanity examples pass. All 402 static source
lock checks pass; this is a source-integrity check, not recompilation of every
locked function. The selected checks are not the entire repository test suite.
