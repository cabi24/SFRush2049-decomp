# D5A04 transition: mark the allocated message before handoff

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800D5A04`, **428 bytes / 107 words**.
Baseline: `cloud/work/ipa-groups/codex_transition_a175/`.

Canonical local comparison improves **14/107 to 13/107 differing words**.
The candidate retains the exact428-byte ELF extent and native40-byte frame.
No extra nonzero words, unresolved/unverified references or relocation errors
are reported for the target. This is a small source-dataflow improvement.

After allocating the real command message, the candidate writes operation7
through the existing node pointer, then hands that pointer to the message
variable consumed by the queue send. Native code likewise writes the command
before its saved-handle move. Both pointers have real consumers; reusing the
existing node local adds no variable, storage or capacity. A separate consumed
allocation local gave the same13/107, so the smaller form is published.

The remaining mismatch is primarily temporary-register allocation and schedule.
A bounded genuine second-caller/release-context experiment was worse and is not
included. The existing A175 helper bodies and keep recipe are unchanged and
unclaimed. No new volatile qualification, empty guard, synthetic caller, padding,
assembly or production/lock change is introduced.

## Reproduce

With IDO5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_transition_d5a04_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `-Olimit 5000`
and `as1 -r4300_mul`. No accepted coverage claim. Independent checker owns
acceptance and ROM integration.
