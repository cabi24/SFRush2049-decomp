# Target orientation setup: donor-backed six-word NONMATCH

Target: `func_8010E72C`, **[0x8010E72C, 0x8010E828), 252 bytes / 63 words**.
Base: `31b2799ebb821a7ec0983a34d2611bba2cedaab9`, checked 2026-10-05.
Result: **NONMATCH, 6/63 words differ. Zero matching, accepted or ROM bytes.**

The complete natural C body has the exact native ELF extent. Every remaining
word differs only by an eight-byte stack displacement: the native frame is 96
bytes and this candidate's frame is 88. No padding locals, dead reads, extra
formals, empty checks, unsupported volatile, stand-ins, artificial keepers,
compiler recipes, scorers, targets, accepted sources or locks are changed.

## New source evidence

The pinned arcade ancestor is
[`historicalsource/rushtherock@845329d7`](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139):

- [`game/targets.c`, StartKnockdown, lines 723–757](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/targets.c#L723-L757)
  supplies the substantive allocate, node setup, orientation, matrix copy,
  list insertion and positional-sound sequence. Its consumed car pointer is
  the source-level explanation for the temporary register ordering.
- [`LIB/fmath.h`, lines 34–73](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/fmath.h#L34-L73)
  defines a real 48-byte MATRIX: a 3x3 orientation plus a position vector,
  with three equivalent views. This is the aggregate declared by StartKnockdown
  and StartCone. Its unused position subobject is part of the authentic type,
  not a new padding array added to tune this target.
- [`game/visuals.c`, PointInDir, lines 2442–2466](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/visuals.c#L2442-L2466)
  is an algorithmic ancestor of the accepted `vector_normalize_length` body.
  The latter's historical source comment says that no ancestor was identified;
  this packet records the connection without modifying that accepted file.
  The N64 threshold/control differs from the arcade version.

`claim.json` records complete donor-file hashes, source locations, prior work
and the fresh known-claim exclusion check. This is genuine source ancestry,
not a claim to have recovered the original N64 file or declaration list.

Earlier B77 sources use a 36-byte Basis. The older B5 seed instead has an
unexplained 13-float array and an unused pointer. This packet does not reuse
those artificial size devices or keep the arcade caller's unused loop indices.

The fixed two-by-two source controls separate the two useful changes:

| Local object / car expression | Different words | ELF bytes | Frame bytes |
|---|---:|---:|---:|
| 36-byte basis / direct array | 37/63 | 248 | 72 |
| 48-byte MATRIX / direct array | 37/63 | 248 | 88 |
| 36-byte basis / consumed car local | 6/63 | 252 | 80 |
| 48-byte MATRIX / consumed car local | **6/63** | **252** | **88** |
| Archived B77 resource-local source | 24/63 | 252 | 80 |
| Archived B77 direct source | 38/63 | 248 | 72 |

The car local closes the allocation/scheduling differences and restores the
complete instruction count. The MATRIX reduces the frame deficit from 16 to
8 bytes; the difference count alone does not show that improvement. Correcting
the allocator's prototype to its accepted zero-argument contract and testing a
callback-pointer view did not close the remaining frame. The final payload is
an opaque 32-bit word; its historical original typedef is not asserted.

Workbench diagnosis confirms identical register lanes and an eight-byte frame
residual. Its raw relocation warnings result from comparing the private
absolute native fixture with a relocatable candidate. The authoritative proof
resolves all 14 relocations independently and compares every complete word.

**Stop condition:** the missing native frame space needs authentic original
local/aggregate or substantive inlined-source evidence. No fake local or source
spelling/format sweep was used to fill it. Six words is not a match.

## Contract and native layout

The zero-argument accepted allocator returns a 24-byte node or null. A null
result skips all caller work. On success the caller:

1. Clears node state, takes a descriptor payload using the actor's signed
   halfword index, stores the owner and external time scalar, sets actor state
   to 4, and clears actor flag bits 1 and 2.
2. Selects the 952-byte player record using the actor's signed byte, builds a
   basis from its direction at +20, and copies exactly nine floats to the
   actor basis at +20. The matrix position tail is never read.
3. Prepends the node using the current list head after the helper calls.
4. Reloads descriptor index and player for the positional-sound request, with
   the actor's position at +56 and mode 2.

The descriptor resource and sound fields are 16 bytes apart with a 48-byte
stride. The views describe only accessed fields; gap bytes do not invent
unknown field semantics. The sound word is explicitly passed through the
accepted signed-int API. Host-wide pointers are used only for host semantics;
18 separate IDO checks establish all claimed O32 offsets and aggregate sizes.

Ten protected data slots contain this function's pointer. No direct JAL caller
appears in the verified 1,216-target population. This supports callback use;
this packet does not establish the dispatcher chain or original table ownership.
The external time scalar at 0x801249D0 is checked from the protected data and is
approximately 0.0333333015. It is not an owned candidate literal.

## Verification

- Exact 252-byte STT_FUNC body, all 63 native words, and all **14** relocations
  verified by both the project resolver and independent GNU ld/objcopy. Four
  zero text-alignment bytes are outside the claim. No owned data, literals or
  jump tables are emitted. O2 and O3 produce the same complete linked body.
- A genuine five-body O3 context preserves the four unchanged accepted direct
  dependencies: `func_80090284`, `vector_normalize_length`, `math_utility` and
  `stat_lap_split`. Their full ELF sizes and strict scores pass, including the
  two owned-literal checks. The caller's complete relocated body stays unchanged.
  All entries are kept as real existing externally visible bodies; this is a
  regression context, not proof of the original translation unit.
- **3,584** canonical-native/project-relocated/GNU-linked/oracle/unchanged-host-C89
  cases; **10,752 MIPS executions** cover all **63 instruction offsets** in each
  form. The same host corpus passes UBSan. Cases cover all 256 input flag bytes,
  allocation failure/success, all four valid fixture indices and player slots,
  arbitrary payload/time bit patterns, and callback mutation combinations.
- The dependency hooks can change the actor's selector, player, flags, list head
  and external time. Calls check arguments and ordered observable snapshots.
  Caller-save registers/FPRs are clobbered. Saved registers, restored stack,
  mapped untouched memory, model/table records and external stack canaries pass.
- Five compiled wrong-contract host mutants are rejected: state, flag mask,
  resource row, cached sound player, and cached list head. A decoder-unknown
  instruction and missing restore mutation also fail closed.

The four dependencies are explicit O32 behavioral hooks. Their real bodies
are compiled and byte-checked in context but not executed within these caller
behavioral tests. Matrix arithmetic and sound internals, invalid pointers,
out-of-bounds or negative indices, concurrency, hardware exception/FCSR behavior,
and gameplay are not tested. Finite tests are not a universal proof. No source
splice, full-game shadow, linked-image, compression or ROM SHA-1 claim is made.

## Local regression result

**721 tests pass**: 14 packet tests, 675 scorer tests, 21 protected-path guard
and 11 submission tests. No skips or failures in that scoped run. All **402
static-lock checks** pass. The complete repository suite and remote CI were not
run; no such pass or monitoring is claimed.

## Reproduction

With the pinned IDO and GNU MIPS tools available:

```sh
python3 cloud/work/frontier/dot_target_orientation_20261005/verify.py
python3 -m pytest -q tests/cloud/test_target_orientation_research.py
```

`--write` regenerates the JSON receipt. No native words, raw assembly, object
files, binaries, credentials or unrelated private data are written into this
packet. Merging and any later acceptance remain with the independent checker.
