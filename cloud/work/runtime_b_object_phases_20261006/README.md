# Runtime B object selection and phase initialization

**Strict MATCH: `func_8039133C`, [0x8039133C,0x80391490), 340 bytes / 85 words.**
The first natural typed reconstruction matches ordinary IDO 5.3 O2. This is
new matching-candidate evidence, with **zero accepted-byte or ROM-coverage gain**.
No allocation/pressure controls, altered ABI, artificial reads, volatile, assembly,
keeper roots, or protected-source changes were needed.

## Identity and source admission

Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`. The protected extent identifies
runtime image **B**, ROM stream `0xB6FEC4`, image SHA-256
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
Runtime A overlaps this address space and is not interchangeable with B.
Open PRs were checked on 2026-10-06; this target had no submitted match. The
adjacent accepted B:91490/914A8 count mutators, draft B:A8CC and B:CA24 remain
untouched. Native game `engine_sound_update` contains the direct call at its
+0x558 offset; the old R16 report used historical caller labels. Image lifetime
and a complete native game caller execution are not proved here.

This is an N64-native reconstruction. Whole-function arcade ancestry and original
source spelling are not established. A pinned arcade search in
[historicalsource/rushtherock](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139)
for the distinctive 45.0 constant found `game/game.c` and `game/sgame.c`, whose
only occurrences are unrelated camera-angle tables. It did not supply this
initializer. This bounded search is not proof that no ancestor exists.
No upstream source is redistributed.

The record and field names are descriptive hypotheses. The native access model
is concrete: a linked object prefix has next pointer at +0 and flag byte +4;
selected records have bit 0x20 set. Output slots are eight bytes (pointer, signed
halfword, signed halfword). State at 0x80399AE0 has signed count byte +0, float
fields +4/+8/+12, and slot pointer +16. Ten IDO layout assertions verify every
consumed offset and native pointer width. Opaque padding represents actual
unaccessed storage, not compiler pressure.

## Complete contract

The sole input is a standard 32-bit O32 reuse flag. If zero, call the genuine
accepted `audio_dma_sync(Heap *, u32)` with null heap and signed count times eight
converted to u32, then store its result as the slots pointer. Its historical name
is misleading: the accepted `audio_heap` group implements a heap allocator.
Its source hash and accepted lock provenance are recorded. No accepted allocator
source is changed, recompiled, or executed by this packet.

Read the object-list head after allocation. Traverse next links; append only
objects with flag bit 0x20 to successive slots, storing both halfwords as one.
The count is never used as a loop bound or capacity clamp. Accepted
`src/blob/func_800BEA6C.c` independently witnesses this list head and next-pointer
location. Its old generated record typedef is not treated as authoritative for
this prefix's other fields. Accepted B:91490 and B:914A8 witness signed count
increment and reset, respectively.

Make four real RNG calls, in order, with float ranges 10,45,45,1. The first three
results initialize +4 to random+5, +8 and +12 to random+15. If the fourth result
is strictly greater than 0.5, add 45 to +12; otherwise add 45 to +8. Equality
therefore selects +8. The concrete helper is `func_8008B2E4`; its submitted
source/contract is [draft PR #132](https://github.com/cabi24/SFRush2049-decomp/pull/132),
head `67a1764330852867a3d0cee2fa3acf0ddae60716`. It is not accepted on this base.
The four direct `jal` instructions are B:0x803913D8, B:0x803913F4,
B:0x80391410 and B:0x8039142C (target offsets +0x9c,+0xb8,+0xd4,+0xf0),
all in the B image identified above. They target 0x8008B2E4 in the main game.
PR #132's earlier statement that its census found no direct native `jal` caller
must be read as limited to the **main-game target census**, not the runtime
images. This packet establishes these four image-B callsites without changing
PR #132 or any accepted source.

This packet executes all 18 protected native helper instructions, including its
LCG seed write and binary32 operations. It neither depends on an invented RNG
implementation nor republishes the helper's source or bytes.

## Verification

- Complete ELF function: exactly 340 bytes. Complete `.text`: 352 bytes, with
  exactly 12 zero alignment bytes outside the function. No owned data, literals,
  jump tables, masks, unresolved relocations or ignored function words.
- All 11 relocations are checked. Independent GNU ld places the complete,
  unmodified object at its native address; nm verifies the 340-byte symbol,
  and all 85 function words equal the protected B target.
- 18,432 fixtures and 55,296 protected-native/project-relocated/GNU-linked
  executions agree with an independently written field-level oracle.
- The unchanged C89 source passes the same 18,432 host fixtures under UBSan.
  Logical host pointers are serialized to native fixture addresses. Native ABI
  and offsets are proved separately; LP64 layout is not confused with O32.
- All 84 reachable initializer offsets and all 18 RNG offsets execute. Native
  +0x12c is a duplicate float load unreachable after branch-likely scheduling;
  it remains included in full-byte comparison.
- Full mapped memory is compared at allocator/RNG call boundaries and return,
  including unselected slots, object records, globals and canaries. The actual
  native RNG runs; a separate run also poisons its permitted caller-save
  registers. Stack, callee-save GPRs and callee-save FPRs are checked.
- Inputs cover list lengths 0..8, all 256 raw count/flag byte values, four reuse
  inputs (0,1,-1,0x12345678), and fourth-RNG results immediately below, exactly
  at and immediately above 0.5. Five compiled wrong-contract mutants fail both
  matching and behavior checks. Unknown instructions fail closed.

The allocator is explicitly a boundary hook returning sufficient mapped storage.
Allocate-mode cases with negative counts or fewer requested slots than selected
objects are **argument/ordering stress cases with synthetic storage**, not proof
that the real heap allocator successfully handles those inputs. Actual execution
of that allocator, allocation failure, insufficient real capacity, invalid/cyclic
lists, pointer aliasing, concurrent mutation, and gameplay are outside the proof.
There is no graceful allocation-failure claim. Finite round-to-nearest RNG
arithmetic is exercised; NaNs, FCSR exception effects and other rounding modes
are not covered.

## Reproduce

From a repository with the standard IDO and GNU MIPS toolchain configured:

```sh
python3 tools/cloud/score.py fn cloud/matches/ovl_b/func_8039133C.c func_8039133C --targets asm/us/ovl_b
python3 cloud/work/runtime_b_object_phases_20261006/verify.py --check
python3 -m unittest discover -s cloud/work/runtime_b_object_phases_20261006 -p test_packet.py
```

For a minimal packet beside an existing repository, set `RUSH_REPO` to that
repository's absolute path. Compiler binaries and native inputs stay local;
only source, tests, hashes and metadata are published. The packet-specific tests
are a focused check, not a full-suite or CI claim. Source-built runtime image,
compression, cartridge SHA-1, acceptance and merging remain with the independent
checker. No CI watcher is created.
