# Dirty-bitmap run scanner: consumed boolean result

Observed local research candidate for `func_800A1C6C` (360-byte target).
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Prior source: `cloud/work/game_C96/group_bounds_assignment.c`.

Writing the consumed success result as `1 - (*bytes == 0)` instead of
`*bytes != 0` reduces canonical positional differences from **67/90 to 63/90
words**. The baseline emits 352 bytes; this candidate emits 360 bytes. Both return
zero exactly when the final byte count is zero. This is ordinary C boolean
arithmetic, not an unused expression, extra memory read, helper or storage pad.

The body still differs substantially. In particular, the compiler's arithmetic
return sequence is not the retail sequence, and its scan pointer/index register
allocation differs. Equal length does not make this a match. The candidate's
canonical comparison reports zero unresolved/unverified references, relocation
errors and nonzero extra words.

The complete seven-function genuine C96 closure is included as required context.
Only the scanner's consumed return spelling changes relative to that baseline;
SDK declarations are minimized from the base headers without changing layouts or
prototypes. No slot-flag volatility experiment is included. No function is an
accepted-match or ROM-coverage claim, and the context remains nonmatching.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_pfs_run_scan_a1c6c_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The canonical group scorer uses its normal cc/uld/usplit/umerge/uopt/ugen/as1
pipeline with `-Olimit 5000`. No broad tests, independent acceptance replay,
image/ROM integration or CI watching was performed. Acceptance and merging
remain with the independent checker.
