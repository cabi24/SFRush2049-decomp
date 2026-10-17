# videoscr rectangle and initializer: retained-native-closure research

Baseline: `58425072b49e95f0fbe5c4c239988fb4885f7c5c`.
This supersedes the local five-word initializer residual documented in draft PR #392's first packet. It remains **research only, with empty claims and zero new accepted coverage credit**. No canonical source has been installed, and no full-unit or ROM verification has been performed for this candidate.


**Unresolved source dependency:** the initializer's exact local result requires the inherited `volatile unsigned char gNumModels` view in both source units. A later read of all26 original MAT3 declarations found ordinary unsigned char. Coherently applying that original type here leaves the rectangle and genuine root exact, but changes initScreen to214/253 differing words with10 unverified section references; the already-refused B59F0 becomes205/343 with its ownership failure. The MAT3 type does not establish the N64 declaration. The exact initializer therefore remains a volatile-dependent diagnostic result, not an authenticated installable source result.

## Conditional exact local result

The stock IDO 5.3 O3 group pipeline accepts:

| Native target | Source identity | Result |
| --- | --- | --- |
| particle_render, 0x800B8650 | videoscr calcMenuRect | MATCH, 112 words / 448 bytes |
| particle_collision, 0x800B8820 | videoscr initScreen, including initButtons | MATCH, 253 words / 1012 bytes |
| physics_sym | genuine retained root | MATCH, 313 words |
| func_8008E26C | native allocator, semantically ZOID_AddObject | MATCH |
| sign_extend_call | native MBOX_NewObject wrapper | MATCH, 10 words / 40 bytes |
| func_800B8810 | native SaveGlobals | MATCH, 4 words / 16 bytes |

The genuine graph is physics_sym -> initScreen -> calcMenuRect. The actual native allocator, MBOX wrapper and SaveGlobals bodies are retained by name, rather than substituted diagnostic callers. The initializer's frame is now 136 bytes; the rectangle's frame is 128 bytes. Neither uses arbitrary padding or unused local storage.

The scorer verifies the rectangle's own literals at 0x8012267C..0x8012268C. It verifies the initializer's four-byte color aggregate at 0x8011AD88 and float literals at 0x80123DE4..0x80123DF4. These are ownership checks, not merely normalized instruction comparisons.

The complete stock object, a separately staged stock rebuild, and an instrumented-uopt rebuild are byte-identical: SHA-256 `8a58d8e2cb166ef6b21ba82f7978854d6ab1624250c630e7f8458ab84c2af0c1`. Instrumentation is diagnostic; the unmodified stock compiler establishes the result.

## Complete-member disclosure

`group/group.json` lists all fourteen known native bodies as members, with an empty context list. `verification.json` records every result. Besides the six matches above, func_800B55F4, func_800B5688, func_800B59E8 and TitlePane match. The preexisting failures remain:

- func_800B59F0: instruction differences are zero, but ownership verification refuses twelve section-relative relocations. Its mixed literal references disagree about the section image address. It is **not accepted**.
- slot_state_setup: 2/58 words differ.
- object_create: 28/28 words differ.
- func_800B669C: 5/5 words differ.

Additional non-native helper symbol extents are disclosed in `fidelity.json`. Those inlining/deleted-body artifacts are not separately identified native targets and earn no match credit. The entire group must not be described as matched, accepted or integrated.

## Recovered source and coherent types

MAT3 videoscr DWARF records calcMenuRect's locals in this order: menuSize, optionWidth, valueWidth, menuIdx, then unsigned-byte str1[40]. The candidate restores that extent and ordering, with explicit casts at the character-string API boundary. validOption has its recorded signed-byte return and real signed-byte local. N64 uses font 11, unsigned width comparisons and array-only validity; the later MAT3 state/cable condition is not transplanted.

The initializer restores the real four-byte color aggregate, texture pointer, unsigned-short texture index, button index, pointer-based color setter, no-argument createBG and native float values. The MBOX source and DWARF establish a signed-short return, a real signed-short dlidx local, and int parent narrowed at the allocator boundary. SaveGlobals is the real no-local signed-byte store helper.

The candidate allocator **definition and caller declaration both return s16**. The baseline allocator source declares s32 but already returns `(s16)i`; changing the candidate declaration and definition coherently preserves its native body and allows the native wrapper/caller to match. The wrapper's signed-short result and dlidx local replace the baseline void-result reconstruction. These are explicit proposed ABI/source corrections, not canonical changes. The allocator's older commentary is inherited; its historical unidentified-ancestor note is superseded by the new MAT3 chain evidence described here.

Restoring the real MBOX boundary accounts for 24 bytes of initializer storage; restoring SaveGlobals accounts for the remaining 8 bytes. Earlier exploratory groups with deleted helper bodies also had scheduling sensitivity to helper definition placement. That observation does **not** carry over to this stronger retained-native closure: both definition-before-use equivalence controls preserve the complete fourteen-member result receipt. No lexical-order trick is necessary here.

### Donor provenance and limitations

The supplied MAT3 source and DWARF excerpts identify videoscr initScreen/initButtons/calcMenuRect, validOption, SaveGlobals, color/font helpers and the MBOX_NewObject -> ZOID_AddObject -> ZOID_InsertObject chain. Individual donor statuses are provenance, not proof of N64 identity. videoscr initScreen and initButtons report 100% individually; the MAT3 videoscr TU is incomplete and calcMenuRect is partial (13/103 differences). The MBOX chain reports individually matching units. Reconstructed MBOX lookup flag formals are not asserted to originate in original DWARF; the N64 callsites independently establish their use.

Source fingerprints supplied with the donor evidence:

- videoscr: `d0dd51c36ef1c26d8886500427147373012a8cd56b3760e90020354c8e230bfa`
- save: `b3dbeb247cd8db6746b1d5bf9ce73176c0277cc5debe6e42affb763d74f4e153`
- zoidtexture: `3c6c46b38ab700bf405e32bbbce3c5fabcb5ea7ed3e47799b2c293b883cebe43`
- HAL_text: `01edf586c784fbb0ed8da894559957b058e4da80de3af1c6f564731891a3a4fb`
- mbox: `8cbe7ecbc8b1ff1a373c73aeef46b3f6e80a8e6b01ad520858970084d238690e`
- zoid: `9d95dd5f92f5aaae64e5f29d9c9779a317db2b44fe8b4477eaac8ae79167699b`

## Reproduction and fail-closed tests

From a checkout containing the unchanged repository scorer, configure stock IDO_DIR and the MIPS toolchain, then run:

    python3 PATH/TO/THIS/DIRECTORY/verify.py
    python3 -O PATH/TO/THIS/DIRECTORY/verify.py
    python3 tools/cloud/score.py group PATH/TO/THIS/DIRECTORY/group

The verifier pins source inventories, every source file, group configuration and native target fingerprints. It requires the entire member receipt and stock-object hash to reproduce, and uses explicit exceptions rather than removable Python assertions. Normal and optimized-Python outputs are identical.

Eight current-source mutations must be rejected:

1. Remove validOption's recorded local: rectangle 5/112.
2. Expand the real 40-byte rectangle buffer to 56: rectangle 2/112.
3. Change rectangle font 11 to 10: rectangle 1/112.
4. Change the rectangle's literal spacing: ownership refusal despite zero instruction differences.
5. Remove MBOX_NewObject's recorded dlidx local: initializer 5/253.
6. Flatten SaveGlobals into the initializer: initializer 2/253.
7. Change the initializer's -75.1f literal: ownership refusal despite zero instruction differences.
8. Coherently widen allocator return to s32: allocator still matches, but initializer differs at 184/253 plus two extra words, and the native wrapper fails.

The two definition-order controls must reproduce the entire baseline member receipt. `test_results.json` contains all fourteen baseline results, eight negative outcomes and both equivalence controls. Raw compared literal byte values are omitted from publication receipts.

This package contains source, verification logic and research receipts only. It contains no target objects, ROM bytes, raw assembly dumps, toolchain binaries or private trace streams. Independent review and any subsequent acceptance gates remain separate from this local result.

## Model-count source-evidence control

The full ordinary-type fourteen-member receipt is `ordinary_model_count.json` and is replayed under `source_evidence_controls`. This is not labeled a wrong-source negative: its declaration has stronger original MAT3 evidence, while the native version remains unresolved. No qualifier is inferred merely because it improves the N64 score. Both declarations change together; the allocator, signed-short MBOX wrapper, SaveGlobals, rectangle and real control root remain strict. The baseline source and earlier result are retained transparently for comparison, with empty claims and no new credit. Native syntax or an independently established N64 access boundary is needed before adopting the initializer.
