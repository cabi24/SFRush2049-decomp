# game_results_input: independent compiler audit

Target `0x800FF724`, 433 words / 1,732 bytes. The retail frame is 120 bytes,
with the return address saved at offset 28. Input is an ordinary sprite/config
pointer; every retail exit returns integer 1. There are no hidden incoming
registers or jump tables. The counters narrow to signed shorts on increment.

## Actual callees

| Function | Words | Ordinary O32 inputs |
| --- | ---: | --- |
| Input_ApplyPadConfig @ 80094EC8 | 48 | Config pointer |
| func_800EF5B0 @ 800EF5B0 | 31 | Config pointer, name pointer, integer direct flag |
| brake_light_update @ 800A6244 | 112 | Player index, position pointer, Transform48 pointer, optional view pointer, screen pointer |
| stat_race_update @ 800FE5B0 | 99 | Config pointer, integer icon index, step, span |

Both projection calls pass a genuine fifth argument at 16(sp), pointing to the
consumed signed-short screen coordinates at retail offsets 88 and 90. Their
fourth argument is NULL. The optional view output is intentionally unused.
`D_8011407C` is a table of integer icon IDs 0–13; `D_801140E8` contains 13.
They are integer data, not pointers.

Full semantic context sources were independently audited against complete
native extents: `tiny_A42/Input_ApplyPadConfig.c`, `tiny_A51/func_800EF5B0.c`,
`tiny_A71/stat_race_update.c`, and `near_miss_B69/brake_light_update_native.c`.
The already accepted `src/blob/input_new_data_wrapper.c` is the actual Hidden
wrapper ancestor. Context definitions are not claimed as new matches.

## Independent controls

| Candidate | Frame | Emitted words | Strict differing words | Aligned opcodes |
| --- | ---: | ---: | ---: | ---: |
| Natural baseline O2 | 120 | 424 | 431 / 433 | 375 |
| Natural baseline O1 | 104 | 480 | 432 / 433, 41 extra, truncated relocation | 296 |
| Actual Hidden O3 group | 112 | 437 including padding | 413 / 433, 1 extra | 377 |
| Actual Input + Hidden group | 112 | 437 including padding | 413 / 433, 1 extra | 377 |
| Actual Input, directly expanded Hidden | 112 | 444 including padding | 408 / 433, 9 extra, truncated relocation | 393 |
| All four actual callees + Hidden | 112 | 435 | 413 / 433, 1 extra | 377 |

The O3 contexts remove the baseline saved s0 but do not inline Hidden. A genuine
wide integer return type, and placing the definition before the caller, leave
its caller identical. All four complete real callees also leave the caller
identical to Hidden alone. No additional context sweeps are justified.

Every control is accepted:false. No padding, dummy arguments, unused pressure
locals, synthetic operations, or fake callee bodies were introduced. Literal
or relocation masking is never acceptance evidence.

## Reproduction

Fresh Rocky harness: `~/agents/large-hud-compiler`. Only protected tools/asm and
comparison scripts were copied; candidates/results started empty. Toolkit
SHA256: `796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5`.
Target SHA256: `f6ad792235bfe8960509bafb6f4e5fc1773f1597de0543c59a70eb510ed2d4ff`.
Flags are recorded literally in each JSON. Whole O3 contexts use the actual
cc-j/uld/usplit/umerge/uopt/ugen/as1 pipeline. Objects, raw words, and native
assembly remain in ignored build storage. Current accepted coverage remains
13.97% game and 46.76% static.

The fresh complete m2c capture source also did not improve the structure: O2
reserved 176 bytes and emitted 428 words, with 428 / 433 differences and 326
aligned opcodes. Its actual Input context reserved 168 bytes, emitted 452
words, and differed in 432 / 433 plus 15 extra words (348 aligned opcodes).
These retain all actual captured values and no unread spill-artifact locals.
