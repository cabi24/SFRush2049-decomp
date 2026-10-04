# C11 rate conversion: one bounded natural attempt

Sole assigned body **8001E50C /380 B**, ending at8001E688, exclusively activated
at central `48ce763d`. Branch `dot/boot-tail-c11-rate-convert` stacks on frozen
`a356fe876ed3518c338b233634758e123f12dcf0`; master reference remains
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Only this research directory changes.

## Outcome and required stop

Exactly **one natural source** was compiled O2 and O1, diagnosed, and frozen.
No refinement or renewed declaration/allocation sweep was attempted.

- O2: **94/95 differing words, one nonzero excess word**, object text400 B.
- O1: **88/95 differing words, two nonzero excess words**, object text400 B.
- Complete fixed native extent:380 B. Both rows have zero masks, unresolved
  symbols, unverified references or relocation errors. Neither is accepted.
- **Zero matching credit.** No matching submission is added.

Flags are the fixed `-g0 -O2 -mips2 -G 0 -non_shared`, followed by O1, with the
canonical automatic `-Wab,-r4300_mul`. O2 is the retained research build because
it preserves the native leaf/no-frame form; O1 introduces a16-byte local frame.
O1's lower positional difference count is disclosed and is not a match. The
native byte home/in-place argument normalization differs from the compiler's
extra copy/normalization and shifts later allocation. This is the previously
observed narrow-formal plateau. `diagnosis.json` contains metadata-only evidence
bound to the sole source hash; its ownership labels are heuristic.

## Closed body and actual ABI

The whole body is `u32(u8 note, u32 encoded_value)`. Its only canonical caller,
8001A5D8, passes an explicitly masked byte from its second argument and the
packed voice word at+92. The callee has no calls and returns the full unsigned
conversion word; the caller's later low-halfword use does not change that ABI.

The body performs every native operation:

1. Replace all-ones encoded input by immediate0x40005622.
2. Extract its top byte. If it differs from note, load a float ratio from
   D_8002C640 indexed by `note-reference`, or from D_8002C840 indexed by
   `reference-note`, and multiply by the low24-bit value. Equal bytes bypass
   table lookup and convert the low24 bits directly to float.
3. Multiply the float by immediate4096, divide by the **signed** word global
   D_8004F800 converted to float, then convert the result to unsigned32.

All intermediate arithmetic is binary32. The natural u32 cast emits the IDO
FCSR-controlled truncation and high-bit fallback; it is not replaced by a
handwritten approximation or unsigned denominator. Real input widths are
preserved. No extra formal, keeper, padding, assembly, helper stub or scorer
change is present.

## External data: unknown contents and lengths

The candidate declares two **unsized external const-float arrays** at8002C640
and8002C840, and an external signed-int rate at8004F800. Their original lengths
and values are unknown and are not inferred. The two array bases are512 bytes
apart; a source-level array-length claim would be unjustified.

`abi_proof.py` explicitly uses the canonical boot-tail symbol map and records
all selected-object relocation addresses. There are no local data/rodata
sections. Address-name resolution and full relocation prove the references,
not any external data value. No masked local table, guessed ratio, literal-pool
substitution or fetched data is used. The only semantic literals are those
already encoded in native instructions.

Tests create a synthetic384-float pool and two overlapping-value views at
float offsets0 and128. This makes every tested byte-difference lookup coherent
with the native512-byte base displacement, including overlap. Host arrays hold
the corresponding read-only values separately; pointer identity is unobserved.
These lengths and values are fixtures only, never claims about original arrays
or evidence that every possible byte input is valid in the game.

## Bounded FP tests

Four groups pass:

- **133,433** canonical native/reference comparisons across two coherent
  synthetic profiles. Each profile includes all65,536 byte pairs, encoded-value
  boundaries, sentinel substitution and positive-rate boundaries.
- **6,591** retained host-C89 calls match native results under ASan, UBSan and
  float-cast-overflow checks.
- **6,591** freshly fully relocated IDO O2 executions match native results.
- There are **66 valid high-u32 cases** exercising the native signed-conversion
  invalid flag followed by the high-bit fallback. **383 out-of-u32-range cases
  are explicitly excluded**, rather than invoking an undefined portable C cast.
  Temporary native width assertions and the source hash in both rows pass.

The limited interpreter loads existing hash-verified canonical words or freshly
relocated candidate words; it embeds no native instructions. It models the
actual delay/likely slots, binary32 operations, signed word-to-float conversion,
temporary truncate-toward-zero FCSR mode, the invalid flag needed for valid
unsigned fallback, and restoration of the initial FCSR/ABI state. It rejects
unsupported instructions. An independent high-level binary32 model and the host
C implementation check its results; it is not a general FPU emulator.

Required test domain: valid supplied external indexed objects, positive signed
rates, finite synthetic ratios and arithmetic, round-to-nearest incoming
binary32 arithmetic, and final results in[0,2^32). Original lengths/contents,
zero or negative rates, NaNs/infinities, out-of-range casts, nondefault incoming
FCSR modes, malformed memory, concurrency, whole-game and cartridge behavior
remain outside this proof. Actual native invalid-conversion behavior outside
the C-defined domain is not claimed portable.

Host tests compile C89 with pedantic-errors, Wall/Wextra/Werror and disabled
FMA contraction. Only leak detection is disabled for executor ptrace
compatibility; no candidate/C-harness heap allocation is present. Native and
host checks require u8=1, u32/int=4, float=4 and IEEE binary32 on the host.

## Reproduction and receipts

Fresh setup, all checksum manifests and439 extents /99,120 B agree; the
existing12-byte getter strictly matches. All631 existing cloud setup, guard,
submission, integrity and scorer regressions pass.

```sh
python3 cloud/work/boot_tail/C11-rate-convert/preflight.py
python3 cloud/work/boot_tail/C11-rate-convert/verify.py --check
python3 cloud/work/boot_tail/C11-rate-convert/abi_proof.py --check
python3 cloud/work/boot_tail/C11-rate-convert/test_semantics.py
```

Use the pinned IDO5.3 setup and a host C compiler with ASan/UBSan. Receipts bind
target/compiler/input/source hashes, all results and test domains. This is a
complete NONMATCH with symbolic external inputs, not original-source or
original-data identification. Central integration owns STATUS/D10 and draft
publication. No ROM, raw assembly, object, credentials, runtime-image or farm
output is included.

## Independent review

The parent approved source commit
`8f5aa1312092cf767cbeb45fe5f6d9ed08a77ce5`, tree
`aacfccf486a927b2fb5f6cb4b83a6db2c367646f`, after reading the entire native/C
body, data/FP domain and fixtures, and independently replaying both compiler
rows, ABI proof and all four semantic tests. See `REVIEW.json`. No source change
was requested or made in this final receipt update. Protected guard, all161
static locks and whitespace pass; zero matching submissions is the correct
canonical-gate result.
