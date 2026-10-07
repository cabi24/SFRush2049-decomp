# D7E88 binding refresh: actual handle lifetime and output cursor

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `drone_target_update` at **0x800D7E88, 496 bytes / 124 words**.
The historical name masks controller binding synchronization.
Baseline: `cloud/work/near_miss_B79/drone_target_update_do.c`.

Canonical local comparison improves **115/124 to 113/124 differing words**.
The candidate ELF body is **484 bytes**, twelve short; frame64 matches the
native frame. There are no extra nonzero words, unresolved/unverified references
or relocation errors. This remains an extensive allocation/geometry nonmatch.

The target explicitly loads the real record handle before clearing the selector
flag, then reloads it after the actual filename-update call. The candidate names
that consumed handle and retains the post-call reload. It also uses an actual
output cursor for the ten binding stores. Each change alone gives114/124;
together113/124. No unused variable, artificial caller, volatile access, padding
or assembly is added.

The complete genuine CCE5C/hash closure from PR194 was tried first: it leaves the
baseline at115/124 and the final source at113/124. Since it is unnecessary for
this observed result, it is referenced rather than duplicated. The published
candidate uses the same real two-argument filename-update contract. Existing
signed configuration indices, actual ten-byte table stride and original helper
calls remain unchanged; no new table-capacity or behavior-safety claim is made.

## Reproduce

With IDO5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py fn cloud/work/frontier/dot_binding_d7e88_research_20261006/candidate.c drone_target_update --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

The canonical driver adds mandatory `-Wab,-r4300_mul`.
No production source, accepted lock or accepted coverage changes.
Independent checker owns acceptance and ROM integration.
