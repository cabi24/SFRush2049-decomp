# CD8EC record checksum: observed local match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800CD8EC`, **372 bytes / 93 words**.
The canonical local group comparison reports **MATCH: 0/93 differing words**,
with no unresolved/unverified references, relocation errors or extra nonzero
words. This is a local match candidate, not accepted coverage or a linked-image/
ROM claim. The independent checker owns acceptance and integration.

`candidate.c` refines `cloud/work/ipa-groups/codex_records_a90/group.c`, whose
recorded and reproduced residual is 14/93. The changes are ordinary consumed
pointer operations:

- Split each existing record-pointer adjustment into assignments/additions,
  retaining its original stride, offset and mode tests.
- Use a named checksum pointer for the three non-96-byte branches. Its assignment
  precedes the checksum store and the following dirty-range call.

The local progression is 14/93, then 5/93 with the checksum pointer, then 0/93
with both changes. The frame remains 24 bytes. No fake guard, extra field access,
unused local, padding, volatile change, invented argument or forced register is
introduced. This is compiled-source evidence, not a behavior/ownership proof for
arbitrary invalid input pointers or out-of-domain selector values.

Only the real accepted `src/blob/groups/codex_hash_a80/group.c` is needed as
local compiler context and is copied unchanged to `hash.c`. The target stays
kept while the hash is internal. Other hash callers exist; this partial visibility
is explicitly a hypothesis about compiler context, not an established original
translation-unit boundary. No additional context body is newly claimed.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_records_cd8ec_match_20261006
```

Exact source flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
The canonical group pipeline supplies `as1 -r4300_mul` and `-Olimit 5000`.
The empty `claims` list does not assert promotion readiness; the normal command
above scores the member. No broad tests, independent acceptance replay,
image/ROM integration or CI watching was performed before publication.
