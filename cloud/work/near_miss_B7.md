# Worker B7 — closest fresh multiply-errata leads

Selection from root's current 79-function unlocked errata sweep, copied read-only from Rocky D scratch to ignored `build/codex-r4300-sweep/results.json`. Selected six smallest genuine strict residuals without extra words or compilation failure, excluding prior B packets and locked targets. Sources were fresh current-context m2c, context SHA256 `b93aa64efbe661b7aaeea40339ca5f698e1050837900c659fa958eb9696da71b`. Exact per-source/target hashes, address, flags and original verdicts are frozen in `cloud/work/near_miss_B7/provenance.json`.

Compilation used private Rocky B scratch/repo, maximum one compute process at once. Current target objects copied read-only from DB to workbench targets. Strict scorer verified protected current target assembly and relocation symbols and applied `-Wab,-r4300_mul`; masked workbench counts never determined acceptance. No integration, source, assembly, locks, state, commits or tooling changed.

| Target | Strict baseline | Best strict | Retained TU |
|---|---|---|---|
| func_80090F44 | 6/42 | 5/42 | func_80090F44_best.c |
| func_8009EA68 | 6/42 | 5/42 | func_8009EA68_best.c |
| func_800B5898 | 6/42 | 5/42 | func_800B5898_best.c |
| func_800B5940 | 6/42 | 5/42 | func_800B5940_best.c |
| func_80090E9C | 9/42 | 5/42 | func_80090E9C_best.c |
| gfx_setup_fc | 9/42 | 5/42 | gfx_setup_fc_best.c |

Every retained TU lives in `cloud/work/near_miss_B7/` and remains NONMATCH. None should be spliced. Flags are `-g0 -O2 -mips2 -G 0 -non_shared` (scorer adds multiply errata flag).

All six implement analogous three-element matrix rotations, with differing real offsets/stride/signs. Corrected m2c's missing sinf argument/prototype, then reconstructed float-array accesses instead of opaque byte-field macros. These semantic/type repairs alone do not change the score. One unused scalar declaration before the saved sine local moves its stack home from28 to target24 in the same32-byte frame; this is an explicit compiler-home quirk, not a demonstrated original declaration. Retained sources use it only as a lead.

For 90E9C/gfx_setup_fc, their first row uses sine product plus cosine product. Updating the named product with `temp_f8 += temp_f12 * temp_f0` before its store gives the target load and multiplication order, reducing8 (after home fix) to5. Simple addition commutation did not do this. This update preserves expression evaluation and does not reassociate floating-point additions.

All remaining residuals are the same f14/f16 exchange: saved sine reload and its two multiply operands use candidate f14 vs target f16, while first output operation/store use candidate f16 vs target f14. Representative 90F44 byte offsets: +4C,+70,+78,+84,+88. Instruction count, frame, integer register allocation, exact symbols and other floating-point operations agree.

Directed probes on representative bodies: named output and product temporaries, store-after-pair shape, direct inline products, typed matrix pointers, sine/cosine prototypes, register qualifier, sine copy, pointer-carried sine, formal-angle reuse for each float carrier, sine declaration position, product commutations, O3 and debug. None remove the remaining rotation; debug and pointer-escaped sine add instructions. Bounded code-free demand probes, including unsafe uninitialized-local reads, were diagnostic only, worsened scores and are discarded. No stand-in group or fake callee attempted. About40 variants on primary representative, about16 on transposed representative; structurally shared results were transferred and re-scored on all four other actual functions rather than repeating identical brute force.

Next useful investigation is authenticated IDO FP reservation/coloring for this family, or a genuinely different matrix source shape with a reason to alter the sine/output interference. Repeating local names, simple declaration permutations or already-tested arithmetic commutations is not supported by these results.
