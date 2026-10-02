# F56E0 type and consumed-variable compiler probes

Frozen baseline: reconstruction/tag_first.c, SHA256 560073708e8aab721b5dc6558faba218537657ee3ea5ff14a0afa193d7475509. Each variant has its own full natural source, with source and object SHA256 and canonical scorer verdict recorded in results/. Objects and resolved raw instruction words remain only on Rocky. Each compilation used a unique temporary working directory through compare.py. No production edits or coverage claims.

26 probes (13 meaningful sources at O2/O3). Baseline is best: 159/518 strict differing words, 362 aligned exact, 514 aligned opcode, 366 aligned opcode-and-register. Frame160; emitted520 includes two zero alignment words, canonical body518. All O2/O3 pairs produced the same scores.

Object enabled pointer versus integer, unknown byte gaps, original model alias, explicit complete distance cast, and explicit unsigned-float mean divide all produce the same baseline words. These do not provide a matching lever. Signed mean count loses the original unsigned conversion sequence. Unsigned genuinely consumed lap/rank/shift/phase variables worsen the result; unsigned shift variants also trigger trailing unpaired HI16 evidence and must not be counted as relocation-complete. The nested podium predicate variant worsens to166; baseline compound predicates remain better.

| Variant | Strict verdict | Aligned exact | Aligned opcode | Frame |
| --- | --- | --- | --- | --- |
| baseline_O2 | 159/518 words differ | 362 | 514 | 160 |
| baseline_O3 | 159/518 words differ | 362 | 514 | 160 |
| byte_gaps_O2 | 159/518 words differ | 362 | 514 | 160 |
| byte_gaps_O3 | 159/518 words differ | 362 | 514 | 160 |
| distance_explicit_O2 | 159/518 words differ | 362 | 514 | 160 |
| distance_explicit_O3 | 159/518 words differ | 362 | 514 | 160 |
| mean_explicit_float_O2 | 159/518 words differ | 362 | 514 | 160 |
| mean_explicit_float_O3 | 159/518 words differ | 362 | 514 | 160 |
| mean_signed_O2 | 467/518 words differ | 302 | 503 | 160 |
| mean_signed_O3 | 467/518 words differ | 302 | 503 | 160 |
| model_alias_O2 | 159/518 words differ | 362 | 514 | 160 |
| model_alias_O3 | 159/518 words differ | 362 | 514 | 160 |
| nested_podium_O2 | 166/518 words differ | 357 | 511 | 160 |
| nested_podium_O3 | 166/518 words differ | 357 | 511 | 160 |
| owner_pointer_O2 | 159/518 words differ | 362 | 514 | 160 |
| owner_pointer_O3 | 159/518 words differ | 362 | 514 | 160 |
| unsigned_lap_O2 | 281/518 words differ | 333 | 508 | 160 |
| unsigned_lap_O3 | 281/518 words differ | 333 | 508 | 160 |
| unsigned_phase_O2 | 262/518 words differ (2 extra words (nonzero beyond target length)) | 340 | 509 | 160 |
| unsigned_phase_O3 | 262/518 words differ (2 extra words (nonzero beyond target length)) | 340 | 509 | 160 |
| unsigned_rank_O2 | 507/518 words differ | 117 | 372 | 152 |
| unsigned_rank_O3 | 507/518 words differ | 117 | 372 | 152 |
| unsigned_rank_shift_O2 | 511/518 words differ (17 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80151AC0 at .text+0x804; unpaired R_MIPS_HI16 for D_80151690 at .text+0x808) | 151 | 500 | 160 |
| unsigned_rank_shift_O3 | 511/518 words differ (17 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80151AC0 at .text+0x804; unpaired R_MIPS_HI16 for D_80151690 at .text+0x808) | 151 | 500 | 160 |
| unsigned_shift_O2 | 511/518 words differ (17 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80151AC0 at .text+0x804; unpaired R_MIPS_HI16 for D_80151690 at .text+0x808) | 151 | 500 | 160 |
| unsigned_shift_O3 | 511/518 words differ (17 extra words (nonzero beyond target length); unpaired R_MIPS_HI16 for D_80151AC0 at .text+0x804; unpaired R_MIPS_HI16 for D_80151690 at .text+0x808) | 151 | 500 | 160 |
