# C17 ordinary libc packet — five improved leads, no matches

Selected from current authoritative layout: memset3390 and modf/modff/__isinf/__isnan34a0, all genuine passthroughs with no existing segment flag pin. All five frozen sources were rebuilt on Rocky and scored strictly with stack differences enabled; final_verification.json records source/target/object hashes, exact flags and honest nonzero residuals. No locks, shared declarations, source/layout/state or target metadata were written by this worker.

| Function | Final flags | Strict / raw-word residual |
| --- | --- | --- |
| memset | -g0 -O0 -mips2 -G 0 -non_shared | 540 / 21 |
| modf | above plus -Wab,-r4300_mul | 780 / 108 |
| modff | above plus -Wab,-r4300_mul | 1180 / 96 |
| __isinf | above plus -Wab,-r4300_mul | 400 / 15 |
| __isnan | above plus -Wab,-r4300_mul | 400 / 15 |

The O0 bodies preserve the original redundant return structure and natural register-ring behavior. memset's observed implementation includes a zero-only word-store optimization after initial byte alignment; unsigned `while(n-- > 0)` reproduces its genuine unsigned nonzero result rather than an arbitrary temporary. Four real source forms were tested for each modf variant: direct input, volatile input, a reduced-sign-branch form, and an ordinary local copy of a register input. The local copy fixes the exact original stack frame/home pattern without guessed storage. The bodies preserve the observed large-integer shortcut, add/subtract large-power rounding step, sign correction and fractional return. No readonly constant mapping is needed; all original constants are immediate.

For the predicates, a local IEEE union initialized from a register FP argument reproduces the genuine8-byte local storage and direct word/halfword bitfield accesses. Pointer-casting the argument itself instead emits extra address temporaries and no original local frame. Actual historical symbol labels appear reversed relative to conventional meanings: the original function labelled __isinf returns nonzero when the exponent is all ones and the remaining fraction is nonzero (NaN), while __isnan returns nonzero when that fraction is zero (infinity). The delivered bodies preserve exact observed behavior and labels.

Residuals are genuine instruction scheduling and control-flow differences. Normal O0 leaves a move before an unconditional branch, leaves the stack restoration before return, and retains extra return branches. Retail schedules those moves/restorations into delay slots. Normal O1 schedules but removes the redundant branches and changes other allocation. Extra assembler optimization flags are inserted before cc's final global optimization argument and did not produce a useful override. Olimit controls also did not change the relevant behavior. Three source optimization-pragma forms had no effect and are excluded from final sources. No large physical-line sweeps or production pipeline changes were attempted.

The initial and directed compiler/source controls are recorded separately in initial_results.json, flag_controls.json, structural_controls.json, local_controls.json and pragma_controls.json. Final sources are truthful unmatched leads; there is no candidate lock/promotion command or full-ROM matching claim. Private staged compiler investigations also failed to produce a usable exact ordinary compiler route; no synthetic assembly edits or altered compiler behavior are proposed for acceptance.

Parent performed the supported four-target scoped assembly refresh before final scoring. Old raw-word targets (modf adb816..., modff5c8c9e..., __isinf55b4b3..., __isnand857f8...) had stale floating-register alias assembly failures. Current targets in targets.json are all reloc-aware/no fallback: modf5fb70a53..., modffa2b7b9e4..., __isinf393c4b19..., __isnan92386af7.... memset remains11786464.... Parent reported4 old targets superseded and1370 stale evidence rows retired. No target objects or ROM bytes are committed; current objects remain private in Rocky agents/C/scratch/static-C17.
