# BT05 conditional pitch handler

**CONDITIONAL-COMPLETE-NONMATCH. No matching-source submission, native-table
closure, production integration, or coverage claim.**

## Recovery status

The execution environment was replaced at about2026-10-04 04:32UTC before the
local commit was published or independently reviewed. This packet is recovered
from the worker's visible task/tool transcript. The retained source hash exactly
matches the pre-reset receipt. The test, verifier and documentation were
reconstructed, but their original individual hashes were not printed before
reset and are not claimed independently identical. The old commit/tree IDs
remain provenance identifiers, not recovered Git objects. See `RECOVERY.json`.

Fresh verification now reproduces the retained source scores, full ELF extents,
relocation resolution and98,816/8host-test results against restored protected
targets and all24authenticated IDO hashes. Independent review must bind the new
immutable tree. Pre-reset observations remain distinct from this fresh replay.
The README also corrects an identified pre-reset note:
the narrowed16-bit command local's promotion/left shift is defined, not overflow.

Scope: `func_80022CD4`, `[0x80022CD4,0x80022F24)`,592B. Old branch
`dot/boot-tail-bt05-conditional-pitch`, base
`c00333eb90e02d42bec33796ac8d9d2a62e30449`, lost commit
`08c4e98cbb1f5bb58e252434fa340a543b21dd15`, lost tree
`b25402d1bb71d18b40d4c4e40bdfc77c17ef629b`. Only this packet directory is owned.
The central coordinator owns claims and inventory. The worker read the local
matching skill, compiler settings, spec015 and dot_response; the root's narrower
restriction against800D1248/helper remained in force. No banned/gated body,
boot data, ROM, layout, scorer, lock or production path was changed or published.

## Binary result, separate from runtime safety

The retained `func_80022CD4_CONDITIONAL.c` implements the whole handler's closed
native ABI: state pointer in a0, command pointer in a1, zero byte return. The
known dispatcher call at0x800244D8 consumes the return's low byte. Native state
accesses are packed u32 at+0x5C and+0x24, packed u16 at+0x50, byte at+0xC0;
the command begins with an aligned u32. Layouts are minimal field evidence,
not recovered original complete types or a middleware release identification.

Prior pinned IDO5.3, C89, `-g0 -mips2 -G 0 -non_shared`, plus scorer-added
`-Wab,-r4300_mul`:

| Variant retained | Differing words | Extra nonzero words | ELF function | .text |
|---|---:|---:|---:|---:|
| O2 |125/148|0|592B|592B|
| O1 |148/148|55|820B|832B|

Both remain NONMATCH on fresh replay, with no unresolved symbols, unverified relocations, masking,
or relocation errors. Full relocated equality and exact ELF function extent
were checked independently of scorer acceptance. O2 matches the frameless leaf
shape and complete extent; O1 adds a frame and many extra instructions. Neither
result proves original runtime inputs safe. The known getter control matched.
All three protected target hashes passed and the inventory/target extents
agree on439starts/99,120B. Fresh `verification.json` reproduces these checks.

`D_8002D476` is an **undefined linker address token**, not a C storage definition,
array base or proof of a standalone halfword. Both native LUI/signed-ADDIU pairs
independently resolve to that address. The unchanged scorer's address-spelled
fallback resolved the token completely; no symbol-map addition, fake local
object, table initializer or extent is supplied to the candidate. The source
uses a u32 address cursor and exact modulo-2^32 decrement/addition. Every load
converts the actual cursor address to a halfword pointer.

This is target-specific N64/IDO address semantics. ISO C alone does not grant
pointer provenance from an integer or from naming an interior linker symbol.
Neither the token nor integer casts establish containing storage. Original
storage and producer-domain evidence are still mandatory for an unconditional
reconstruction. `literal_address_baseline.c` reconstructs the first direct-
address control, previously125/148 at O2 (592B), without relocation references.
Its original standalone hash is unavailable, so it is not claimed byte-identical.

## Conditional contract, deliberately unproved for the game

Let C=(command.word0>>8)&65535, S=state.sample5C&0xFFFFFF, P=0x8002D476.
Equal C and S bypass the table, including both zero. Otherwise:

1. State and command fields must have valid initialized backing objects,
   alignment/effective types appropriate to the packed state and aligned command,
   and access permissions. No conflicting concurrent accesses or prohibited
   type aliasing may occur. The halfword backing must remain stable across state
   writes; the tests use nonoverlapping objects.
2. min(C,S) must be nonzero. Q=((max(C,S)<<12) modulo2^32)/min(C,S).
   The exponent loop produces n in0..11; F=Q/(1<<n). The shift operations remain
   defined, and unsigned arithmetic intentionally wraps.
3. A finite first k>=0 must exist such that U16(P-2k)<F. All earlier visited
   halfwords must be>=F. There is no index-zero stop. Every visited address and
   selected adjacent address+2 must identify readable, suitably aligned halfword
   objects in valid actual backing storage under the target's pointer mapping.
4. The two selected endpoint values must differ. Their subtraction is converted
   to the native unsigned divisor. C integer promotions are safe because both
   halfwords fit int; a decreasing endpoint pair is not silently changed/clamped.
5. Target conversion behavior must hold:32-bit unsigned int,16-bit unsigned
   short,8-bit bytes, IDO's low-byte signed conversion, and the stated linker/
   integer/pointer mapping. No out-of-domain call to plain C is permitted.

The semitone counter begins11 and wraps unsigned on enough decrements. The
note assignment truncates to16bits. The upward fraction truncates to a signed
byte; the downward path performs the native signed-byte conversion, negation,
and final byte conversion. No donor downward clamp or new guard is inserted.
The final flag update ORs0x100 into the original packed flags word.

None of a13-entry extent, base0x8002D460, monotonicity, endpoint values, or a
producer guarantee follows from the observed anchor/walk. A finite walk beyond
11entries is supported conditionally and was tested with synthetic backing;
it is not proof that the actual game table has that backing.

## Source family and falsified donor-domain assumption

Public reference: [AxioDL/musyx, synthmacros.c, DoSetPitch lines597–663](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c#L597),
revision `78d2e16e4905fc675952162d331c24d5198b2687`, source blob
`a3203f9e0032fa5e9d08fa5e51da80acf4cab0c0`, [CC0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE),
license blob `0e259d42c996742e9e3cba14c677129b2c1b6311`.
Ratio normalization, descending strict search, semitone offset and interpolation
identify the family. The native command is16-bit and lacks the newer donor's
note clamp; donor layout/table/domain are not adopted.

- S22050/C44100 and the reverse produce Q8192,n1,F4096. The donor's lowest
  halfword is4096, so strict<F continues below its nominal index0. This
  disproves importing the donor13-entry domain, not actual N64 storage safety.
- S0x100000/C1 wraps the left shift to0, so F0. No unsigned halfword can be
  strictly less than0. Arbitrary nonzero24-bit rates cannot guarantee termination.
- One unequal zero rate causes a zero divisor. Equality at both zero remains
  an intentionally valid no-table path.

**Missing evidence:** native table-object extent/interior-anchor relation and
initialization properties, plus actual producer/input invariants excluding
zero-divisor/nontermination cases. Owned middleware source/header evidence or
maintainer-only verification may close these facts without publishing raw data.

## Bounded source work and stopping point

Ten justified spellings, O2 then O1 each, are recorded in recovered
`variants.json`: cast-rate and masked-rate extraction, external address token,
explicit index-to-address calculation, split load/extraction, while-loop cursor,
narrowed command local, unsigned integer cursor, cursor update in loop body,
direct indexed integer-address expression. These cover the closed16-bit command
extraction and genuine backward-walk structures. They retain strict comparison
and wrapping arithmetic on their defined domains. The narrowed command local
promotes to int, but maximum65535 shifted12 remains below INT_MAX; that valid
control was rejected because its residual worsened. It was compiled, not run or
retained as the final body.

Workbench diagnosis ran on the baseline before refinement and after every
form, using fresh target/candidate objects only in temporary storage. Its
structural/cfe-spelling classification and unsupported-lever result for the
retained spelling were heuristics. Manual comparison showed an extra command
extraction move, changed scalar allocation and loop rotation; no grounded
further source hypothesis improved125/148. Stop here rather than fabricate a
table base/extent, change ABI/flags, add keepers/padding or sweep declaration
orders. Reopen on actual storage/source evidence or a justified lowering lead.
Original raw diagnostic files/objects were temporary, are lost, and are not
published. The recovered variant table contains observed numeric receipts only.

## Reproduce and tests

After independently restoring pinned compiler provenance and protected targets,
install this packet under cloud/work/boot_tail/BT05-conditional-pitch and run:

```
IDO_DIR=/path/to/pinned/ido python3 cloud/work/boot_tail/BT05-conditional-pitch/verify.py
```

The original pitch preflight compiler hash dictionary was written but not printed
before reset, so that old file is not claimed recovered. The current preflight
uses independently authenticated24-file IDO pins from the published
cloud/work/boot_tail/packet2/preflight.json at
496a72b0edd683bd7d46e8c21f8323ab43629494. The restorer compared all24actual
compiler files against those original pins. Fresh `verification.json` records
the new replay; it is not a reused old receipt.

Before reset and again on fresh replay,98,816 validated actual-source calls
passed strict-C89 compilation and UBSan: both directions, all256original-key bytes, rate16/24-bit edges and
wrapping, exponent cap, equal/zero-equal inputs, traversal lengths0..20, positive/
decreasing adjacent endpoints, byte conversions, modulo note underflow, flag
preservation, zero return, untouched state bytes and unchanged command.
A separate prevalidator establishes finite readable traversal and nonzero
divisors before every source call. Eight unsafe fixtures were rejected before
execution: two zero-rate divisors,F0, donor-lowest-bound counterexample in both
directions, equal endpoints, missing adjacent endpoint and missing anchor.

The verifier compiles the actual retained C without rewriting its body. A
synthetic mapping at the proven address and test-linker absolute token are host
test fixtures only. They are absent from the IDO object and supply no native
data, original extent or matching shortcut. The synthetic donor-shaped fixture
preserves the lowest4096 property; it is not a copy/assertion of N64 contents.

The fixed address conflicts with ASan shadow placement, so no ASan run or claim
is made. UBSan tests are conditional host checks, not native table proof or
universal runtime safety. No sanitizer establishes pointer provenance.
Independent review must bind the restored materials to a new immutable source/
tree identity and keep binary matching separate from domain safety.
