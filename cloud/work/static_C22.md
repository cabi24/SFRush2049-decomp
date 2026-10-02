# C22 — two canonical SDK trigonometric matches

Two current static passthroughs from unconverted9330 pass strict stack-sensitive scoring and raw unlinked word comparison, both independently and together in the genuine complete TU. Final ordinary sources include types.h only, with the exact flags line first. Parent already independently D replayed the earlier header-inclusive form and performed the supported two-target refresh; final types.h-only files were recompiled and rehashed by this worker and await parent final replay/normal gates. Worker touched no shared source/header/layout/flags/locks/state/target metadata.

| Function | Actual body | Original tiled slot | Strict / raw |
| --- | --- | --- | --- |
| sinf | 112 words /448 bytes | 448 bytes | 0 / 0 |
| cosf | 90 words /360 bytes | 368 bytes including8 original zero alignment bytes | 0 / 0 |

Potential static gain is two functions /816 slot bytes. Both bodies together emit exactly816 original linked text bytes, including final8-byte alignment, with no .data/.rodata/.rdata/.bss or COMMON storage. They reference existing real readonly symbols exclusively, so there is no new constant ownership or source allocation proposal.

Source flags first line: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul. The exact verified complete production invocation also includes existing -Xcpluscomm. Explicit R4300 multiply errata handling is mandatory for these multiply-heavy bodies. The unconverted segment has no previous accepted neighbor flag pin; the eventual converter must derive the actual O2+Wab pin from accepted lock evidence. No arbitrary override is proposed.

Final frozen source SHA256:

- sinf.c: 17e4db8bc83e2690379172fe33449d833dbcf70599705c57fa2c0354d9c025e5
- cosf.c: 548afebb05933e26f02c51db02bc0b1b5d92393e3de499b7052ec527b65e7196
- full_module.c: 2eb4b0088a662efa3f41db73c557a28bd34484752a5a54f1c560af625acd4b29

Final private objects are Rocky ~/agents/C/scratch/static-C22/{sinf,cosf,full_module}.final.o. final_verification.json pins their exact SHA256, source/target identities and all four standalone/whole-module strict/raw zeros. verification.json separately records an independent fresh rebuild with explicit full commands. Object .mdebug filenames depend on source invocation paths, so different replay paths can change object hash while every scored instruction remains exact; use each recorded command/source hash for its respective object.

## Canonical source and readonly proof

Local SDK gu/sinf.c and cosf.c bodies were adapted only from SDK local constant names to authoritative original symbols. Original polynomial coefficients, argument reduction, ROUND/ABS expressions, exponent extraction, sign selection, NaN return and zero fallback are retained. The ROM constants exactly equal ALL SDK initializer bytes, recorded in sdk_constant_identity.json. Original libm_vals.s provides the exact7F810000 NaN word, also verified against gNaNf. Source/SDK/current-header/symbol hashes are in context_hashes.json.

Historical labels are misleading but preserved: gTwoOverPi and gCosAngleScale actually contain1/pi; the gPiOver2* and gCosPiOver2* pairs contain the SDK split pi; gNaN and gCosOne are the SDK float zero fallback, not NaN or one. Named gSinCoeffs/gCosCoeffs each contain the genuine five-double coefficient array. The source uses their actual correct f64/f32 types rather than relying on misleading comments. constants.json records values, sizes, locations and original-data hashes; no binary ROM bytes or objects are committed.

rom_tu_declarations.txt is the minimal integration context: two const f64[5] arrays, six const f64 scalars, three const f32 scalars, plus the exact simple ROUND/ABS macro definitions. The source files compile with types.h only; complete-TU proof uses current rom_tu.h. Root can place the local macros before the first target pragma as reviewed. There are no existing ROUND/ABS definitions in the shared header (ROUND_UP_DIVIDE is unrelated). No accepted caller prototypes or existing storage declarations require changes.

## Target and linked verification

DB initially retained obsolete raw-word targets with floating ABI alias assembler failures. Parent performed supported scoped refresh through already-reviewed alias-ready assembly; db_targets_current.json shows both reloc-aware/no fallback and exact new objects:

- sinf target: 51e5aa6c1edc07702c4f92838e1ced47594202b30d3ce5eec621e7e060de086f
- cosf target: 4d85fcdef870a78c63152eb7e1cc79e5880358461f691835aa1c2ff1a4d8dce0

Current genuine target assembly round-trip gates pass. full_module.ld assigns only actual referenced readonly symbols to their authoritative original addresses and links source text at80008730, ROM9330. Entire816-byte text equals original, with SHA2565696e5d1339e307b1038e087fa9d6dac0a84b367a64ee466cb445b9d6b514201 on both sides. linked_full.json and full_module.readelf.txt prove exact whole-module placement, actual function extents and absence of new data. No masks or substituted linked fields were used for this comparison.

Independent D replay:

```sh
python3 PACKET/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target-dir TARGET_DIR --output D-proof.json
```

Optional --context-dir CURRENT_ROOT_SNAPSHOT compiles against the exact current header tree while using the verifier tools from --repo. The script serially compiles both final standalone files and full_module.c, uses unchanged canonical stack-sensitive scorer with function-scoped objdump, and requires every raw object word including alignment to match. All four checks passed in the worker replay. Targets stay private in Rocky C scratch or can be read-only exported from supported current DB. Parent normal lock/prep/convert/promotion transactions and forced-ROM SHA1 remain required before any coverage credit.

This packet needed only four initial O2/O1 baselines and final standalone/full-TU exact replays. O1 failed honestly and is excluded; no source variation sweeps or scoring relaxation were necessary.
