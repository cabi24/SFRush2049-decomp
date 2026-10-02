# A42 frozen ordinary semantic packet

Flags: `-g0 -O2 -mips2 -G 0 -non_shared`. Four genuine complete ordinary TUs; no claims. No accepted source/layout/lock changes. IDO/scorer add the canonical `-r4300_mul` erratum setting. Canonical per-function commands were run on Rocky; instruction-bearing output stays private at `~/agents/A/wt/build/codex-A42/*.score.txt`.

| Target | Strict differing / retail words | Extra | Canonical exit |
|---|---:|---:|---:|
| func_800B7360 | 33/33 | 4 | 1 |
| func_800B0EA0 | 30/48 | 0 | 1 |
| visual_objects_update | 22/35 | 0 | 1 |
| Input_ApplyPadConfig | 23/48 | 0 | 1 |

`func_800B7360` compares four unsigned bytes, updates all four on any mismatch, and clears the full dirty word. Its emitted function extends four words beyond the retail scope, leaving the cache HI16 relocation unpaired within that scope; this is an explicit scorer error, not a match.

`func_800B0EA0` preserves signed ratio endpoint comparisons and actual unsigned low-word multiplication followed by signed arithmetic shifts for the three RGB5551 components. Negative ratios follow the retail arithmetic path. No saturation or replacement arithmetic was introduced.

`visual_objects_update` traverses four real 16-byte list records, reloads the node body after the real MaxPathZeroControls call, and uses actual word72 predicate/word0 next link. The real callee returns a signed byte as a full integer; its corrected s32 return prototype made no score gain. The action is a consumed full-word input, with meaning unproved.

`Input_ApplyPadConfig` uses actual Config offsets and Pad32 stride. Seven real field snapshots before output stores improve 24 to 23 differing words. Actual callee assembly establishes full-width index/flag parameters for analog/status/button/enabled routines; status's third input is tested signed before its byte store, fourth input is u16. Correct prototypes were retained even though the score did not improve. No helper definitions or invented ABI formals were added.

Bounded controls: byte-cache snapshot/store scheduling, input seven-field snapshots, real MaxPathZeroControls return declaration, and actual input callee width declarations. All residuals remain large; stop rather than attempt allocation sweeps. Sanitized counts/errors and exact final source SHA256 values are in scores.json; prior controls are in controls.json/control_prototypes.json. Canonical command form: `python3 tools/cloud/score.py fn cloud/work/tiny_A42/NAME.c NAME --flags '-g0 -O2 -mips2 -G 0 -non_shared'`.
