# D10 Packet 5: C13 input-value anchor

One active claim, centrally acknowledged at `bf7b7405`: `func_80021150`,
`[0x80021150, 0x80021428)`, 728 B. Branch `dot/boot-tail-p5-c13-input-value`,
base current master `301d9e7552ad4fd7f54a38796db84671e1000d35`.
Own only this directory and the corresponding single matching filename.
Previous C13 packets are frozen; central ledger/D10 remain sole-writer work.

Fresh setup, target manifest, all 439 extents and existing-getter replay precede
candidates. Reconstruct the full two-pointer body; declare actual external
`func_80020A04` only. No helper body, fake formal, keeper, dummy call, padding
local, assembly, target/scorer/boundary/header/lock change, ROM/image/farm work
or unrelated helper investigation. Start O2 then O1; diagnose before refinements;
stop within 20 directed variants per hypothesis without justified new evidence.

Native region notes: setup and source loop at +0..+0x58; bipolar selector
classification through +0x98; packed signed LFO/helper read through +0xF4;
signed fixed-point scale and combine through +0x1EC; unipolar helper/scale and
combine through +0x290; live count reread/advance and return through +0x2D8.
The seven bipolar IDs are 128, 1, 10, 160, 161, 131 and 132. Combine modes are
0 replace, 1 add, 2 multiply. The native signed scratch result is not initialized
on an invalid first combination mode: valid input records must use supported
modes. No fabricated initialization or extra formal will replace that behavior.

## Frozen outcome: complete NONMATCH, no new matching bytes

The whole 728-byte body is reconstructed and frozen at **24/182 strict differing
words**, zero nonzero excess words, and zero unresolved symbols, unverified
references or relocation errors. O2 uses the exact native **88-byte frame**.
Aligned object text is 736 bytes, including two ordinary zero alignment words.
O1 differs in 180/182 words with 36 nonzero excess words. Neither is a match.
No source is submitted to `cloud/matches/` by this packet.

The initial O2 form had 138/182 positional differences and two excess instructions.
A natural LFO doubling expression (`tmp += tmp`) removed that size drift and
improved the strict result to 24/182. It emits addition where native uses a shift,
with remaining normalization-copy/allocation/scheduling differences elsewhere.
This improvement is research, not a masked score, match or cartridge claim.

All flags are `-g0 -O2/-O1 -mips2 -G 0 -non_shared` plus the scorer's automatic
`-Wab,-r4300_mul`. O2 is supported by the native frame, nine preserved callee-saved integer
registers and loop/branch-likely scheduling; O1 has a different spill/frame/body
shape. No unpreserved callee-save/IPA convention or alternative compiler is assumed.

## Whole-body data and arithmetic contract

The genuine inputs are a packed voice pointer and a packed input-record pointer.
The nine matched wrapper callers independently supply the latter at offsets
196, 214, ..., 340, corroborating its 18-byte extent. Four four-byte sources contain
controller byte +0, combination byte +1 and **signed** 16-bit scale +2. The byte
count is +16. Native signed high-byte loads establish the scale interpretation.
The older initializer's positive 256 store is bit-compatible with this signed
reader; its frozen source and all shared headers remain unchanged. This packet
provides read-side type evidence, not a retroactive rewrite or complete shared type.

The loop reloads source count and relevant packed fields after external calls
where the native body does. It is not replaced by a cached count. `80020A04` is
only declared with its real three-byte argument ABI and 16-bit result. Its body
is neither copied nor stubbed into the reconstruction.

For the seven observed bipolar controller IDs, raw input comes from signed LFO
halfwords at voice +368/+380 (IDs 160/161) or from the helper minus 8192. Signed
8.8 fixed-point scaling uses the product's low 32 bits and arithmetic shift by 8,
then clamps to -8192..8191. Combination modes replace, add about the 8192 center,
or multiply with an arithmetic shift by 13; the centered result becomes unsigned
`value = combined + 8192`.

Other IDs use helper output, the same signed scale and shift, and only an upper
clamp of 16383. Modes replace, add with unsigned wrapping and saturation, or
multiply with low-word wrapping and logical shift by 14 before saturation.
The return is the low 16 bits. The retained source explicitly preserves low-word
product semantics; its LFO addition is safe for every signed 16-bit stored value.
No signed-overflow assumption is needed in the tested valid-format domain.

The signed-combination scratch is deliberately not assigned a fabricated initial
value. Native loads its prior stack slot on entry and stores it at exit; valid
modes 0, 1 or 2 define it before use. An invalid first bipolar combination uses
indeterminate scratch state. The test harness explicitly demonstrates native
stack-fill dependence for that unsupported case and never passes it to host C.
This is a real algorithm local, not a keeper or padding local.

## Directed controls and stop

`tools/workbench.py diagnose` ran before the first refinement and again at the
improved 24-word checkpoint. Both frames agree at 88 bytes; the initial diagnostic
found two inserted instructions, while the improved body has no instruction-count
delta. No proven ownership/source lever was returned. `diagnosis.json` stores
metadata only; raw instructions, diagnostic listings and objects are not committed.

Ten natural source forms total are archived, each with both prescribed flag levels:

1. initial direct LFO multiplication (138/182 + 2 excess at O2);
2. split load/multiply statements (same residual);
3. defined unsigned LFO shift representation (same residual);
4. SDK-style 32-bit `long` typedef spelling (same residual under IDO);
5. additive LFO doubling (best, 24/182, no excess);
6. split defined shift (initial residual);
7. common LFO scaling path (138/182 + 1 excess);
8. direct signed-product reference spelling (initial residual);
9. signed-product spelling plus additive doubling (same best residual);
10. legacy signed shift-assignment spelling (initial residual).

The retained version uses explicit wrapping products and safe additive doubling;
rejected signed-product/shift controls receive no portable behavioral guarantee.
No fake formal, callee body, unused live variable, arbitrary padding, dummy call,
inline assembly, forced register, altered extent or tool/scorer edit was tried.
The hypothesis froze below the twenty-variant bound after equivalent controls
ceased producing movement. Do not repeat these forms without new native/compiler
context. The next useful input is authenticated conversion/coalescing context for
LFO doubling and post-multiply normalization, not a score-masking exception.

## Source-family and type-reference limits

Pinned [AxioDL/musyx `_GetInputValue` in snd_midictrl.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_midictrl.c)
([CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE))
is a family lead. Native here additionally treats controller 132 as bipolar,
uses signed 16-bit/8.8 scale records, supports only the three observed combination
modes, and has no demonstrated modern dirty-cache/oldValue or variable-source
logic. The public PC/Dolphin implementation is not claimed as the exact N64 source.
No reference translation unit, header or table is vendored.

The primitive typedef spelling control was grounded in pinned
[ultralib ultratypes.h](https://github.com/decompals/ultralib/blob/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd/include/PR/ultratypes.h).
That header carries an SGI proprietary notice and is not copied here. Only the
ordinary fact that its 32-bit SDK aliases use `long` was tested on IDO's 32-bit ABI;
it did not change this residual. No licence compatibility or source-identity
claim is inferred from that comparison.

## Bounded semantic verification

Six tests compare the retained C89 body, an independent integer model, and the
canonical native body in a fail-closed integer MIPS-II replay over synthetic
memory. **2,427 valid fixtures** yield **4,854 native comparisons** with two
different initial stack fills. Cases include all controller classes, every
supported combination, signed scale/LFO extremes, random four-source sequences,
empty inputs, wrapping products, live count shrink/growth and helper-driven
field changes. Return values, memory effects and external argument sequences
agree. Native replay checks ordinary O32 integer caller-save clobbering, restoration
of the real callee-save registers/stack, delay slots and likely-branch annulment.

The external helper is an explicit test contract with synthetic outcomes; these
tests do not reconstruct that helper. They validate supported source counts
0..4 and combination modes 0..2. Unknown opcodes, unmapped access and step limits
fail closed. Invalid first bipolar combination is documented as stack-dependent,
not silently initialized. No ROM/image or game runtime is invoked.

Reproduce:

- `python3 cloud/work/boot_tail/C13-input-value/verify.py --check`
- `python3 -m unittest discover -s cloud/work/boot_tail/C13-input-value -p 'test_*.py' -v`

`scores.json` binds all 22 compiler comparisons to source, target, compiler and
tool hashes. `semantic_verification.json` separately binds the semantic fixture
result to the retained source, canonical body and replay/test code. The tests
compile retained C under host C89/pedantic/Werror and award zero matching credit.
Independent BT03-high review passed source commit
`80a70911dd839502111541b2e2e4973aeb7bc024`, tree
`90a241de6bb0cb4765ee3a264a8f105a5960110c`; `REVIEW.json` records the exact
source/score/semantic hashes and bounded scope. The reviewer independently
reproduced all 22 compiler rows and six semantic tests, then added a separate
exhaustive 256-controller zero-scale classification check: 512 further native
comparisons and 256 host/model comparisons passed. These additional review cases
are reported separately and are not added to the committed suite's fixture count.
The 631 existing cloud regression tests also pass with zero skips, and the
protected-path, 161 static-lock and whitespace checks pass.

There are zero matching submissions or matching bytes in this packet. Aggregate
publication remains lead-owned. This branch changes only its owned research
directory; central state and frozen sources stay untouched. Merging and production
integration remain checker-owned.
