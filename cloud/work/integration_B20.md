B20 freezes a guarded target-assembly normalization patch for two C15 canonical SDK sources. No source/scorer/header/target inventory was changed by this worker; coordinator supported refresh and independent acceptance remain pending.

The historical labels designate real SDK subfields:

| Target | Historical alias | Canonical field expression | Guard |
|---|---|---|---|
| osGetTime | gViTimeAccumLo | gViTimeAccumHi + 4 | Both authoritative addresses; live and SDK OSTime=u64, unsigned long long u64 |
| osViModeTableGet | gViModePtr0..6 | gViModeTempBuffer + {0x10,0x18,0x20,0x28,0x2c,0x30,0x38} | All seven authoritative addresses; complete matching ordered SDK/live OSTask_t fields; both OSTask wrappers expose t at zero |

The implementation accepts only the exact documented o32 scalar and pointer declarations. SDK unsigned long u32 and live unsigned int u32 are both four-byte o32 types. It checks complete task declaration order/widths and the canonical alignment union plus live struct wrapper. Missing SDK files, scalar/type/layout drift, absent symbols or any address-family mismatch leave original target operands intact. Replacements affect only exact bare %hi/%lo operands and only the two named targets; unrelated symbols and expressions with pre-existing addends remain unchanged. Existing PIF normalization and float register alias prelude are preserved. No scorer settings, masks, penalties or branch behavior changed.

Fresh independent private target assembly passed existing round-trip gates. Linking all referenced symbols at authoritative addresses reproduces every unmasked retail byte: osGetTime 144 bytes, osViModeTableGet 268 bytes. The task target object contains 68 words due to assembler alignment; its 67-word retail extent is exact and the sole extra emitted word is zero, matching C15's existing declared alignment observation.

Fresh Rocky IDO compilation of C15's unchanged complete canonical sources against these private targets gives:

| Target | Literal flags | Before strict/raw | After strict/raw | Object words |
|---|---|---|---|---|
| osGetTime | -g0 -O1 -mips2 -G 0 -non_shared | 10 / 1 | 0 / 0 | 36 |
| osViModeTableGet | -g0 -O2 -mips2 -G 0 -non_shared | 140 / 14 | 0 / 0 | 68 |

Canonical scorer used stack_differences=True, difflib, ign_branch_targets=True and unchanged toolkit objdump. Source hashes remain acf650fe... and 8065f19e... respectively. New private target hashes are 003f0366f146328fdd098524974f78db29815c628a0ca0892e4ebc4a63c5b53e and 6a0055529638e63974886853916a7c7087106788dd8d52248708ecf9da4412c1. Exact hashes/counts are in strict_verification.json and linked_targets.json; no raw byte streams are delivered.

`integration_B20/guarded_aliases.patch` contains the small targets.py extension and a new focused test file. `targets.patch` is implementation-only for manual review. Patch apply check passes against current root. All 24 tests pass, including the entire existing target-assembly suite and new missing/drifted SDK/type/wrapper/address, atomic-family, unrelated-operand and unmasked full-link checks. guard_provenance.json pins the authority files; manifest.json pins deliverables.

Root should apply the reviewed patch, run target tests, refresh only osGetTime and osViModeTableGet through the supported scoped regeneration path, and independently re-score C15 before normal locks/promotions. C15 owns canonical source integration. Do not copy private .o files into inventory or alter genuine source to match historical alias spelling.

B18 follow-up review found no new storage-ownership blocker. Root already improved the local gate to explicitly run make test. Its standard body-lock route still needs B19's reviewed whole-module evidence recorder because ordinary reduced-TU staging does not retain the complete VI context. Current B18 promotion pins whole source/proposed header, refuses single-function integration, gates complete source/header/storage as a tuple, and preserves startup placement evidence. No additional lifecycle mutations were made by worker B20.
