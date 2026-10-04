# C11 numeric packet

Five exclusively assigned bodies / 844 B, central claim `ea50bee9`, stacked on
frozen registry head `f05f9eeb7028a96cf76f9d85d7c06508d3be1d61`.
The master reference remains `301d9e7552ad4fd7f54a38796db84671e1000d35`.
Earlier packets in this worktree are unchanged. Central integration owns the
shared status, claims, D10 and draft-PR publication.

## Result

| Address | Bytes | Retained O2 | O1 control | Outcome |
| --- | ---: | --- | --- | --- |
| 8001E440 | 204 | 23/51 differing, 2 nonzero excess | 47/51 | Complete nonmatch |
| 8001E688 | 172 | 0/43, no excess | 36/43, 1 excess | Strict MATCH |
| 8001E7BC | 168 | 41/42, no excess | 42/42, 7 excess, relocation error | Complete nonmatch |
| 8001E864 | 204 | 34/51, no excess | 51/51, 1 excess | Complete nonmatch |
| 8001E940 | 96 | 0/24, no excess | 23/24, 2 excess | Strict MATCH |

Two matches / 268 B; three complete nonmatches / 576 B. Each retained O2 body
has zero masked, unresolved, unverified or erroneous relocations. Both matches
have exact full relocated word equality and zero nonzero excess. E688's object
text is 176 B (one zero alignment word); E940's is 96 B. No boundary is changed
or truncated. O2 is selected per member because O1 fails; this is evidence for
the selected build, not identification of the original global flags.

Flags: `-g0 -O2 -mips2 -G 0 -non_shared`, with the canonical driver's automatic
`-Wab,-r4300_mul`; only O1 is used as a flag control. Sixteen hash-bound rows
cover five retained bodies plus three archived E864 controls. No compiler,
target, scorer, shared header, runtime image or helper body is modified.

## Actual interfaces and complete bodies

- **E440:** genuine unsigned-short input and result. Native code converts the
  input to float, multiplies by an external float at 8002D924, converts to an
  unsigned word, then returns its low halfword. The original scalar value is
  unknown and is not defined here. The native original-a0 home/in-place mask
  differs from the compiler's copy, scheduling and return-normalization forms.
  This is the already measured narrow-formal plateau; one source form, diagnosis,
  O1 control, then stop. No declaration sweep or fake word formal.
- **E688:** one real double argument in f12/f13 and a double result in f0/f1.
  `(unsigned int)value` followed by the declared double return naturally emits
  the complete FCSR-controlled conversion sequence. First O2 body matches.
- **E7BC:** genuine u16 phase, signed-short result; modulo-4096 phase split into
  four 1024-entry lookup/reflection/sign quadrants. Native signed halfword loads
  use 8002CC54 or the reflected last entry 8002D452 = base + 2046. The external
  1024-short table is declared, never defined. Original values are unknown.
  Native short home, redundant normalization and temporary allocation differ.
  One source form, diagnosis and O1 control, then stop. The O1 unpaired HI16
  at text+0xa0 for D_8002CC54 is explicitly rejected and retained in the receipt.
- **E864:** whole binary-search-shaped body with five real parameters: key,
  base, signed count, signed byte stride and an actual two-pointer comparator
  returning int. Zero count returns null; otherwise a one-based inclusive range
  selects `base + stride*(middle-1)`, invokes the callback, and narrows the range.
  Native and candidate frames are 56 B and preserve the same eight live values.
  The native midpoint shift through v0 then copy to s0 differs from the direct
  compiler s0 shift; positional differences cascade. Two natural directed
  refinements (value-bearing midpoint assignment and genuine zero-based array
  index) do not improve the original 34/51. Three forms total, then stop. The
  archived `initial` file duplicates the retained body to bind diagnosis.
  This is a semantic role identification only, not a claimed libc/version match.
  The existing caller 80016C20 uses the same five-argument comparator contract.
- **E940:** u32 output pointer and a genuine byte-state pointer. Full native
  callee 80019AA8 reads state[75], substitutes index 8 for 255, and returns a
  word from external D_8004FA20. The output word is loaded after this call, so
  helper mutation must be visible. The whole body performs wrapping 32-bit
  left shift by16, unsigned division by the returned nonzero rate, wrapping
  multiplication by1000, right shift by5, then store. First O2 body matches.
  The second formal is not an integer, and no helper implementation is supplied.

## External-data proof and limits

`abi_proof.py` reproduces metadata-only relocation/section evidence. All five
O2 objects have no local data/rodata sections. Named external references are
resolved by the unchanged scorer's canonical address-name rule, then the full
relocated comparison is performed. E440 and E7BC have proven native external
addresses and no masked/unverified fields; no local literal/table mapping has
been substituted. This proves reference addresses, not external contents or an
original source file. Neither nonmatch receives matching credit.

Host tests define formula-generated synthetic data only in temporary harnesses.
The six E440 coefficients are test fixtures; the E7BC formula is not a recovered
waveform. No original scalar, table bytes, ROM, native dump, object or assembly
listing is published. `diagnosis.json` contains summaries only, bound to initial
source hashes before the directed controls.

## Validation and semantic domain

- Fresh setup, checksum manifest and all 439 canonical extents / 99,120 B agree;
  existing getter 80010A00 strictly matches (12 B). See `preflight.json`.
- Six C89 sanitizer/width/receipt groups pass: **532,958 retained-source calls**.
  E440: all 65,536 inputs for six synthetic coefficients (393,216 calls).
  E688: 65,536 word-plus-fraction values and nine boundaries (65,545).
  E7BC: all 65,536 phases (65,536). E864: 8,517 search cases including empty,
  missing keys and duplicate-midpoint behavior. E940: 144 word/rate/helper-
  mutation cases. `host_verification.json` binds source and harness hashes.
- C89 pedantic-errors, Wall/Wextra/Werror, AddressSanitizer,
  UndefinedBehaviorSanitizer and float-cast-overflow checks pass. Only leak
  detection is disabled for executor ptrace compatibility; these harnesses and
  candidates do not allocate heap storage.
- Temporary IDO assertions verify u32=4, u16=2, float=4 and double=8 bytes.
  Host tests require the corresponding IEEE binary32/binary64 representation;
  the E940 independent wider-word model additionally requires unsigned long
  at least 64 bits on the test host, not on N64.
- All **631** existing cloud setup/guard/submission/integrity/scorer regressions
  pass. Canonical two-submission gate, protected guard, 161 static locks and
  whitespace pass at the immutable source checkpoint.

Float conversion tests are restricted to finite values whose truncated result
is representable as u32; E440's rounded products are in [0,2^32). They do not
claim portable behavior for NaN, infinities or out-of-range conversions. E7BC's
signed-short wrap for negating -32768 follows the N64/compiler two's-complement
conversion (host tested likewise), not implementation-independent C89 behavior.
Search requires valid sorted objects, nonnegative counts, positive stride,
nonoverflowing midpoint/byte-offset arithmetic and a consistent comparator.
E940 requires a valid output word/state object and nonzero rate. Synthetic tests
establish these bounded body semantics, not original external data, malformed-
input safety, concurrency behavior, whole-game correctness or cartridge coverage.

## Reproduction

From repository root with the pinned IDO setup available:

```sh
python3 cloud/work/boot_tail/C11-numeric/preflight.py
python3 cloud/work/boot_tail/C11-numeric/verify.py --check
python3 cloud/work/boot_tail/C11-numeric/abi_proof.py --check
python3 cloud/work/boot_tail/C11-numeric/test_semantics.py
```

Only the two strict C files belong in the changed-submission checker. Complete
nonmatches remain in this research directory. Further allocation experiments
are stopped absent new authentic compiler/source context.

## Independent review

BT03-high independently approved source commit
`53adec19a3a734a7abdf9acc22aff3a34392c18d`, tree
`fa209b5e85d469d90e60a5b8c826898a2b7bdf5f`: all sixteen compiler rows, six
sanitizer groups, actual native bodies/ABIs and unknown-data qualifications
reproduce from immutable Git copies. See `REVIEW.json`. The receipt-only
handoff preserves every C hash. The address-metadata script now explicitly
selects the boot-tail symbol map rather than the scorer's default blob map;
this changes only the reported provenance of helper 80019AA8 from address-name
fallback to its existing canonical symbol entry, with all addresses unchanged.
The metadata correction was separately replayed and approved; see
`REVIEW_SUPPLEMENT.json`, which binds both changed metadata-file hashes.
