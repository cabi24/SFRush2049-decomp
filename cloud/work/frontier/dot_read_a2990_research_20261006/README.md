# A2990 Pak read/retry: genuine heap contract and queue-helper structure

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `drone_set_catchup` at **0x800A2990, 852 bytes / 213 words**.
The historical name masks a Controller Pak data read/retry path.

Canonical local comparison improves **210/213 to 208/213 differing words**.
The frame changes from **152 to the target's216 bytes** using only existing
queue operations and their actual message outputs, factored into queue-init,
lock/unlock and handle-release helpers. No extra locals, arrays, empty calls or
padding compensate for the frame. This is evidence for a source-structure
hypothesis, not proof of original helper names or organization.

The candidate also replaces the prior false three-argument heap-release
prototype with the genuine `audio_reverb_update(u32 address, s32 tag)` contract
and includes real heap source/callers so IDO derives its internal convention.
The allocation mode argument is correctly integer, and the retry callback is a
named real function pointer. The corrected contract with unfactored queue code
scored209/213; adding helper structure gives208/213 and the frame improvement.

The body remains **820 bytes**, thirty-two bytes short of the target. Saved
register allocation and source geometry still differ extensively. No extra
nonzero words, unresolved/unverified references or relocation errors are
reported. Neither the heap context nor any other function is a new match claim.

Baseline/provenance: `cloud/work/game_C88/drone_set_catchup.c`; genuine heap
context is the same reduced actual closure in
`cloud/work/ipa-groups/codex_pack_a123/group.c`. Queue operations are retained
from C88, with the helper organization also present in the genuine Pak context
published under `cloud/matches/pak_reset_a3724_group/` in PR163. Inferred field
views and partial compiler visibility remain assumptions. No production source
or accepted lock is changed.

## Reproduce

With IDO5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_read_a2990_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `-Olimit 5000`
and `as1 -r4300_mul`. Independent checker owns acceptance and ROM integration.
