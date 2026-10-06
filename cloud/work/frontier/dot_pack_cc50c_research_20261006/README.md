# CC50C state packer: consumed-local stack-home refinement

Observed local research candidate, not a match or ROM-coverage claim.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800CC50C`, 760 bytes / 190 words.
Prior source/recipe: `cloud/work/ipa-groups/codex_pack_a123/{group.c,group.json}`.

Reordering the declarations of its ten existing, genuinely used locals reduces
canonical positional differences from **30/190 to 14/190 words**. This aligns
the original-buffer, compressed-buffer, compressed-size, checksum and returned
handle spill homes. No local is added or removed, no initializer moves, and the
function's operations and 104-byte frame remain unchanged.

The remaining differences include a reused extent spill home, two address-add
operand orders, a queue-address/load schedule, a metadata pointer register and
the final returned-size arithmetic. The complete body remains 760 bytes. The
candidate comparison has zero unresolved/unverified references, relocation
errors and extra nonzero words; it is still explicitly NONMATCH.

All real heap/hash/caller context is copied unchanged from A123. Its fields and
record sizes describe actual stored objects, not artificial stack padding. The
original source's exact declaration order remains a reconstruction hypothesis.
No stand-ins, dummy formals, new qualifiers, extra accesses or forced registers
are introduced. Context functions are not additional claims.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_pack_cc50c_research_20261006
```

Exact source flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
The canonical group pipeline supplies `as1 -r4300_mul` and `-Olimit 5000`.
No broad tests, independent acceptance replay, image/ROM integration or CI
watching was performed. Acceptance and merging remain with the independent checker.
