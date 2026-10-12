# Extra-car statistics panel: 588 new exact candidate bytes

Base: `3bec9ce30b8fbc707e2e1d6c5717826c03646232`.
Source-first deliverable: `groups/runtime_a_stats_menu_20261006/`.
New member: image A `func_803AB5FC`, `[0x803AB5FC,0x803AB848)`, 588 bytes,
all 147 words. **Zero accepted-image or ROM-verified bytes claimed.**

The group keeps its existing identity and all six exact candidates from
[draft #341](https://github.com/cabi24/SFRush2049-decomp/pull/341), head
`7cf0ea411a257c90a7224510063a114410ea353c`. Those six total 4,548 bytes and
receive no new credit. Every verified function is both a member and a claim:
AAF0C, AB5FC, AC330, AC54C, AC7F4, ACD5C, ACF5C. No accepted lock is moved
between groups. The six other complete genuine bodies remain NONMATCH context;
only the real AD57C callback root is kept. This is a whole-group private-ABI
result, not a standalone ordinary-ABI submission.

## Source result and causal controls

AB5FC temporarily installs the inspected profile as player zero and selects
one-player mode, invokes the existing unlock-table refresh, then restores both
settings. It renders unlocked extra-car names, or the real localized empty-list
message, and sizes the surrounding panel from the measured maximum width.
The observed second width-measure call in the selected maximum arm is retained.

The complete previous body had **124/147 differing words** and a true 560-byte
function extent. Seven target words were missing, not zero padding. Two local,
native-grounded source corrections recover the complete 588-byte extent:

1. Both result arms of the maximum-width conditional are explicitly converted
   to unsigned 32-bit. Native uses an unsigned comparison and sign-narrows the
   selected result to 16 bits on both branches. The former mixed signed-result
   expression let uopt discard one narrowing branch, create a separate carried
   comparison value, and displace subsequent allocation. With both result arms
   unsigned, the complete instruction geometry returns: four words differ.
2. Install the temporary profile pointer before the one-player count. The two
   distinct global stores precede the same helper call. This recovers the native
   first appearance of the pointer-load and constant temporaries, closing the
   four t7/t8 words. Restoration order already matched and is unchanged.

No declaration, buffer, function signature, callee implementation, keep list,
compiler, target or production source changed. No padding, dummy read, fake
caller, volatile qualifier or hand-edited object is retained. The unsigned
conversion is a source-level type hypothesis supported by the native lowering,
not recovery of the original macro or declaration.

Recompiled causal controls, all retaining the six previous exact members:

- Original signed-result arms with final store order: 121/147 differences
- Original store order with unsigned arms: 4/147 differences
- Both original forms: 124/147 differences; true extent 560 bytes
- Final source: MATCH, complete extent 588 bytes, no extra nonzero words,
  unresolved symbols, unverified references or owned-data errors

## Verification

```
python3 cloud/work/frontier/dot_runtime_a_extra_cars_20261012/verify.py --score
python3 cloud/work/frontier/dot_runtime_a_extra_cars_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a_extra_cars_20261012/verify.py --deep
python3 -m pytest tests/cloud/test_runtime_a_extra_cars.py -q
```

Canonical scorer, without `--claims` or `--allow-unverified`:

```
Members:
func_803AAF0C:
  MATCH
func_803AB5FC:
  MATCH
func_803AC330:
  MATCH
func_803AC54C:
  MATCH
    own .rodata verified at 0x803B98E8..0x803B990C
func_803AC7F4:
  MATCH
    own .rodata verified at 0x803B990C..0x803B9928
func_803ACD5C:
  MATCH
func_803ACF5C:
  MATCH
```

The input-bound receipt authenticates the current-tree image, selected native
extents/words, complete genuine caller inventory, source, verifier, test, recipe,
referenced symbol addresses and all eight stock compiler-pass identities. It
reconstructs the prior master source by undoing only the two AB5FC corrections
and the three inherited numeric-layout corrections, rejecting unrelated drift.
It never reads an unmerged commit during replay and does not pin mutable tools,
locks, build files or assembly text. No static ROM TU is rebuilt.

Independent GNU assembly/linking uses every original instruction and relocation
record from each complete claimed function, reanchoring original section-relative
calls to their owners. All seven native-address extents are exact: **250 text
relocations** resolve, together with **16 relocations in both full inherited
jump tables**, a 64-byte native interval. The short baseline is also linked in
full; absent words remain absent and count as differences, including a missing
native NOP. No comparison window, relocation masking or patched binary is used.
Stock ugen listing diagnostics on/off produce byte-identical complete objects.
Normal and optimized-Python deep replays pass. The focused WITH-IDO test passes
and the WITHOUT-IDO arm cleanly skips through shared conftest.
Missing or corrupt inherited jump tables are independently rejected for both
owners. The unrelated NONMATCH contexts retain their recorded data uncertainty.

The inherited 40-byte numeric buffers and their source are unchanged. Their
12/10/6-byte maximum-write analysis remains bound to the exact format strings
and formatter/copy helper native identities. This is a source/native-flow bound,
not an executed formatter proof. AB5FC introduces no text buffer or write.

## Limits

The full bodies and genuine AD57C call edge supply IPA context. The private
helper clobbers the s-register set observed in native; standalone code is not
claimed. Remaining context bodies are not installation claims.

Language/name pointers, profiles and selectors must be valid, backed and
terminated for the external services they reach. Helper implementations are not
executed by this packet; in particular the unlock refresh remains a called-service
contract. The source preserves native unsigned comparison and IDO/MIPS signed
halfword narrowing; out-of-range signed narrowing is implementation-defined in
portable C. Existing finite, representable float-to-u32 preconditions for the
preserved time formatter remain unchanged.

No gameplay/hardware run, unrestricted aliasing or memory-safety proof, original
TU/type recovery, private image rebuild, compression gate or full-ROM hash is
asserted. Publication stays draft; the independent checker owns installation and
merging. ROM bytes, raw assembly, binary objects and credentials are excluded.

## Independent review

The paired lane independently recompiled the complete group and reproduced all
seven strict matches, then passed normal and Python `-O` deep replays. It audited
the native unsigned/narrowing behavior, unchanged helper call multiplicity, both
setup/restoration sequences, complete GNU symbol/suffix/relocation handling, the
short baseline and missing-word accounting, source drift checks, current-image
inputs, inherited capacity limits and missing/corrupt table rejection. No blocking
finding remained. Its defensive suggestion is included: the sole native private
call is required explicitly at AD57C+0x7ac.
