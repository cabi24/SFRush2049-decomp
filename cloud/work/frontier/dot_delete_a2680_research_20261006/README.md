# A2680 Pak deletion: native byte address and queue-helper research

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `AdjustSpeed` at **0x800A2680, 784 bytes / 196 words**.
The historical name masks Controller Pak deletion/retry.

Canonical local comparison improves **175/196 to 173/196 differing words**.
The existing file-record view is formed as one byte displacement from the real
controller-array base: `port*772 + index*40 + 140`. This mirrors the native
address association while preserving the actual offsets and accesses.

The actual queue-initialization operations are split from the existing real lock
helper, and actual unlock calls share a helper. This changes the frame from168
to208 bytes, closer to the target232, without new message outputs, unused locals,
empty calls or padding. The helper split alone remains175/196; the byte-address
change accounts for the two-word score reduction. Original helper organization
remains a hypothesis, not recovered donor source.

The candidate ELF body is **764 bytes**, twenty bytes short. Register allocation,
frame and control/scheduling geometry remain nonmatching. There are no extra
nonzero words, unresolved/unverified references or relocation errors for the
target. No companion body is claimed matched.

Baseline and genuine context are
`cloud/work/ipa-groups/codex_pak_rename_views_a172/`, reproduced at175/196.
The older w9 AdjustSpeed packet that used three empty calls for frame size is
not used. Existing full heap, error-map and real queue code are retained.
Recovered field views, helper structure and partial compiler visibility remain
assumptions. No production source or accepted lock is changed.

## Reproduce

With IDO5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_delete_a2680_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `-Olimit 5000`
and `as1 -r4300_mul`. Independent checker owns acceptance and ROM integration.
