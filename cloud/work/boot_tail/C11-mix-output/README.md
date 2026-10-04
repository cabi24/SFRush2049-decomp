# C11 mix-output: complete bounded nonmatch

Sole assigned target **8001E0E0 / 864 B**, ending at 8001E440. Central activation
`3e91ca50`; branch `dot/boot-tail-c11-mix-output` explicitly stacks on frozen
numeric head `42c094cb7fef0a8d08d3ed51f333ddacfbd287d2`. The master anchor is
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Only this packet's research directory
is changed; all earlier source/receipt files stay frozen.

## Frozen result and stop

The retained complete body is **NONMATCH: 207/216 differing words plus three
nonzero excess words** at O2. Object text is 880 B, compared against the complete
fixed 864-byte native extent. There is no masked, unresolved, unverified or
erroneous relocation. The native and retained candidate frames are both 16 B.
O1 is 216/216 with 50 nonzero excess words. There is **zero matching credit**
and no changed matching submission for this packet.

Four natural source forms were compiled O2 then O1, using the fixed canonical
`-g0 -mips2 -G 0 -non_shared` flags and automatic `-Wab,-r4300_mul`:

| Form | O2 differing /216 | O2 excess | O1 differing /216 | O1 excess |
| --- | ---: | ---: | ---: | ---: |
| Initial whole expression body | 214 | 7 | 213 | 59 |
| Genuine reused external-scale value | 210 | 3 | 215 | 56 |
| Genuine interpolation-entry pointer | 210 | 3 | 216 | 50 |
| Read reused scale at first use (retained) | 207 | 3 | 216 | 50 |

The native single scale read survives all later output stores, so the cached
local represents a real value, not a keeper. It restores the 16-byte frame from
the initial candidate's eight bytes. The pointer local is the actual pair of
adjacent interpolation entries. Reading scale at its first use is the final
bounded refinement. FP allocation, persistent table-base allocation and an
extra home/reload of the second output pointer still differ. No unsupported
local/formal/register/flag sweeps follow this plateau. `diagnosis.json` binds
initial and retained workbench summaries to source hashes. Suggestions from
its heuristic classifier are not treated as measured compiler ownership.

## Whole-body ABI and operations

The complete canonical caller 80014A74 was previously independently approved
as a strict match. It has five genuine inputs and passes eight actual slots:

`void(u16 *left, u16 *right, u32 volume, u32 pan, u32 span,
      u16 *span_output, u32 aux, u16 *aux_output)`

The pointers address record halfwords +66, +64, +68 and +70 respectively; the
four value inputs are full words. Names describe roles without asserting an
original library identity. This function has no callees or callbacks. No
unused formal, stub, inline assembly, fake stack object or dummy argument is
introduced.

The reconstructed whole body:

1. Clamps volume above 0x7F0000, then linearly interpolates adjacent floats at
   D_8002CA40 using its high16 index and low16 fraction /65536.
2. Interpolates D_8002CC44 using span's high10 index and low22 fraction /4194304,
   multiplies by gain, D_8002D914 and the shared D_8002D910 scale, truncates to
   signed32 and stores a halfword through span_output.
3. Replaces span by unsigned `0x800000-span`, clamping results at/above
   0x800000 to 0x7F0000, interpolates again, and scales the live gain.
4. For pan exactly 0x800000, uses separate external multipliers D_8002D918 and
   D_8002D91C. Otherwise it interpolates pan for the first output and the same
   unsigned-reflected/clamped pan for the second. Stores occur left then right.
5. Independently clamps/interpolates aux using D_8002CA40 and multiplies by
   D_8002D920 and the same shared scale, then stores through aux_output.

Each floating multiply/add is single precision. Halfword conversion is a real
signed32 truncation followed by low16 storage; there is no invented saturation.
The exact store order is span_output, left, right, aux_output. Output pointers
may alias one another, and their last-writer behavior is preserved.

## External data: addresses proved, values unknown

The candidate only declares two external float-array views and five external
float scalars; none is defined or replaced:

- D_8002CA40: 129-element view, last element8002CC40, sufficient for the clamped
  index0..127 and next entry. Its end abuts the independently addressed next table.
- D_8002CC44: four-element view through8002CC50, sufficient for tested pan/span
  0..800000 and next-entry reads. This is a bounded view, not proof of the
  original declaration's array length.
- D_8002D910, D_8002D914, D_8002D918, D_8002D91C and D_8002D920: real native
  scalar-load addresses, with all original values unknown.

`abi_proof.py` explicitly selects the boot-tail symbol map and records all
external relocation metadata. The unchanged scorer resolves the address-named
externals and fully relocates each body. O2 objects have no local data/rodata
sections, so no unknown literal or local-table relocation is masked away.
Original table/scalar contents remain outside this proof. The only literal
float constants in the body are immediate, exactly represented1, 1/65536 and
1/4194304 already present in the native arithmetic. No guessed waveform,
external scalar value, ROM byte, native dump, assembly listing or object is
published.

## Semantic validation and explicit limits

Five test groups pass, using three **synthetic** external-data profiles. Each
profile has11,337 fixtures:9,801 boundary-grid inputs, all256 output-alias
partitions at four input sets (1,024), and512 deterministic random inputs.

- **34,011** whole-native instruction runs agree with an independent high-level
  binary32 model, including ordered writes and bounded external reads.
- **34,011** host-C89 candidate calls agree with those native outcomes under
  AddressSanitizer, UndefinedBehaviorSanitizer and float-cast-overflow checks.
- **19,608** freshly fully relocated IDO O2 candidate/control-body runs agree
  with the native results (four forms, three profiles,1,634 selected fixtures
  each), including every alias partition and the random set.
- Temporary IDO assertions prove native uint4/ushort2/float4 widths. Host tests
  require IEEE binary32 and disable fused-operation contraction. Only leak
  detection is disabled because of executor ptrace compatibility; no candidate
  or C harness heap allocation is present.
- The instruction interpreter loads the existing hash-verified target or freshly
  relocated candidate, embeds no native words, checks delay/likely slots, rounds
  every floating operation to binary32 and fails on unsupported instructions.
  Model self-tests reject an unsupported opcode and an external-data read beyond
  the declared object. It is a limited test interpreter, not a complete emulator.

Volume and aux may be any u32 values and are clamped. Tested pan/span lie in
0..800000 inclusive; this packet does not establish wider caller invariants.
Outputs must be valid aligned halfwords and may mutually alias, but must not
alias the read-only external data. Synthetic tables/scalars are finite and keep
all float-to-int conversions in the signed32 representable range. Native floating
rounding is modeled as round-to-nearest binary32; nondefault FCSR behavior is
not tested. NaNs, infinities, invalid cast ranges, malformed pointers, concurrency,
original external numeric values, whole-game behavior and cartridge coverage
are not proved. Original data are not prerequisites for this parameterized
body-semantic comparison, but are required to claim actual game output values.

## Receipts and reproduction

Fresh setup, manifest hashes and all439 canonical extents /99,120 B agree;
existing getter80010A00 strictly matches. All631 existing cloud setup, guard,
submission, integrity and scorer regressions pass. `verification.json` binds all eight
O2/O1 rows, native extent/hash, compiler/input/source hashes, excess and relocation
fields. `host_verification.json` binds the tests/model and their stated domains.

```sh
python3 cloud/work/boot_tail/C11-mix-output/preflight.py
python3 cloud/work/boot_tail/C11-mix-output/verify.py --check
python3 cloud/work/boot_tail/C11-mix-output/abi_proof.py --check
python3 cloud/work/boot_tail/C11-mix-output/test_semantics.py
```

This is source-complete research with external declarations, not a strict match
or an identified original source. Central integration alone updates STATUS/D10
and publishes draft PRs. No runtime-image or farm work is included.

## Independent review and final gates

The parent independently reviewed source commit
`5040204a3526811f0807de3f5a68bef72c2866d0`, tree
`f510dc018faa599fb94c709d3f721774c71a880c`, inspected the entire native body,
actual caller, source and test models, and reproduced all eight compiler rows,
address proof and five semantic tests. PASS is for complete bounded NONMATCH
research only; see `REVIEW.json`. This final receipt update changes no C/model
source. The protected guard, all161 static locks and whitespace pass; the
changed-submission checker correctly finds zero new submissions.
