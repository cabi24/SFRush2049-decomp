# Eight macro/stream source-contract repairs

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.

This packet repairs eight existing source conflicts, covering 1,840 protected
function bytes. It establishes no new match or cartridge coverage and performs
no promotion, lock migration, remote builder operation, or CI repair.

## Source changes

### Macro handlers, `lib_22300.c`

New adapted sources cover `func_80021BF0`, `func_800225FC`, `func_80022678`, and
`func_80023520`. The original individually locked sources remain untouched.

These handlers access packed partial views of the same runtime state. Their
shared calls now declare the already accepted `MacroState *`/`MacroCommand *`
contract for `func_80021BC0`, `func_8002193C`, `func_80021764`, and
`func_800233B0`. Explicit pointer conversions at those call sites preserve the
observed packed accesses; they do not change offsets, argument count, argument
register class, or return ABI. This is not a blind global struct rename.

The sole production edit moves the existing `func_800233B0` prototype before the
`func_80023520` passthrough. The promotion driver's global declaration deduplication
would otherwise skip the earlier declaration and leave an implicit-int call
before the later u8 declaration. All 15 existing function bodies stay unchanged.
The separate `func_80023754` wrapper contract remains exactly as accepted.

### Stream handlers, `lib_25bb0.c`

New adapted sources cover `func_800254D4`, `func_80025670`, `func_8002574C`, and
`func_800259A8`. They share the actual `StreamState_80025264` record already used
by the accepted token lookup and `D_80056230[2]` declaration.

Only previously opaque ranges are expanded into observed fields in that
production declaration. The record retains its 0x1228 stride, signed state and
scale bytes, volatile busy byte, the four-argument callback, and SDK queue
layout. Named fields include the request queue/messages, controls, rate, handle,
buffer, request state, processed count and duration. Native target-ABI compile
assertions check the complete size and 16 important offsets. No fake object,
keeper, assembly, artificial stack padding or compiler-flag change is used.

The new sources use the real `OSMesgQueue_s` tag and `OSThread_s` pointers, the
same SDK contract repaired by PR #82. `D_800586A8` and its accepted queue
initialization remain untouched. All five existing function bodies stay
unchanged. Scalar mappings from the original service view include
`remaining_blocks -> read_count`, `duration -> field_1220`, and
`volume/pan/span -> value1/value2/value3`, at their original offsets.

## Fresh verification

`verification.json` is from a fresh run of `verify.py` using the pinned IDO 5.3
files, unchanged Makefile ROM-TU flags, real asm-processor and MIPS assembler.
It records hashes of sources, tools, SDK/context headers and protected targets.

- All eight candidates pass standalone flags, Makefile flags, and actual
  `rom_tu.h` context, with exact ELF function sizes and full relocated words.
- The baseline, declaration-repaired current source and combined candidate
  overlays pass all 28 C function checks: 8 candidates and every one of the
  20 existing locked functions. No relocation masks, unresolved symbols,
  unverified relocations or extra words are allowed.
- All 66 macro-TU and 21 stream-TU function positions remain unchanged. Entire
  raw text sections are equal: 13,552 macro bytes and 3,840 stream bytes,
  including assembly passthroughs and terminal padding. The stream section has
  four alignment bytes beyond its 3,836 function bytes.
- Allocated C data payload sections remain empty and unchanged. The stream
  overlay changes the `.reginfo` register-mask metadata hash. asm-processor
  combines compiler placeholder and assembler register-use masks by OR;
  these masks need not be byte-identical when assembly slots become C.
  The verifier reports this difference rather than claiming all non-text
  object bytes are identical. Its 24-byte size and GP value are preserved.
  The full ROM transaction is still required.
- The historical macro/helper and stream/global redeclaration failures are
  reproduced using original source in fresh real TUs. A wrong macro field
  selection and wrong stream record offset are rejected (15 and 44 differing
  words respectively). Exact-extent tests reject borrowing a neighbor, added
  padding, and wrong ELF sizes.
- Native host harnesses exercise 96 macro and 50 stream cases. These cover
  signed note adjustments/clamping, conditional audio calls, forwarded pointers,
  busy-state transitions, callbacks, release ownership, queue acquisition and
  release, missing tokens, both stream indices and progress ratios. Host record
  sizes are not used as evidence of the 32-bit MIPS layout.
- `tests/cloud/test_macro_stream_contracts.py` explicitly selects these adapted
  paths in standard CI: all 14 tests pass. Partial and complete temporary
  promotion fixtures retain the checks. A wrong promoted body remains rejected
  even after its temporary body lock hash is recomputed. Missing accepted locks
  and missing/duplicate candidate slots are rejected. No real lock is edited.

The macro raw-byte preservation check does not claim new relocated-target proof
for unrelated assembly-only jump-table slots. Every candidate and current C
lock is fully proved individually; assembly/padding outside those verified
spans must be byte-identical to the baseline.

## PR #82 coexistence and dependencies

These eight repairs do not require any PR #82 file or commit. They deliberately
leave its eight macro wrappers, three queue wrappers and identifier repair to
that separate promotion track.

`verify_pr82.py CHECKOUT` optionally checks coexistence against the exact
published PR #82 source tree, `13eb95cbe593611c823dfa3e05a9226cc9c0d1bd`.
The local source-equivalent commit is `0591c0728d37c3db16ab94bbba578dc65dd8abec`;
the published commit is `93e3bfa`. `pr82_coexistence.json` records a fresh replay
with all eight new handlers plus the eleven PR #82 candidates in these two
TUs overlaid together: 39 complete C functions pass. PR #82 files are read and
hashed, never copied or modified. The unrelated identifier TU is outside this
coexistence check.

## Reproduction and remaining maintainer gates

From the repository root, with pinned IDO selected through `IDO_DIR` and MIPS
binutils on `PATH`:

```sh
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_macro_stream_contracts.py -q -rs
python3 cloud/work/boot_tail_promotion/macro_stream_contracts/verify.py > /tmp/macro-stream.json
python3 cloud/work/boot_tail_promotion/macro_stream_contracts/verify_pr82.py /path/to/pr82-checkout > /tmp/macro-stream-pr82.json
make check-matched
git diff --check
```

`check_submissions.py` does not automatically select these new adapted paths;
the explicit CI regression above is required. The proof must remain active
following later promotion, and tests exercise that lifecycle.

The maintainer must re-prove and replace each candidate's existing source lock
with its adapted path through the authorized gate, regenerate fresh context
rows, and then perform the normal narrowly scoped promotion transaction with
fresh actual objects and the complete cartridge SHA-1. Do not add duplicate
function locks. Existing locks, context records, protected targets, symbol
addresses, layouts, scoring logic and flags are unchanged in this packet.
Merging remains with the independent checker.
