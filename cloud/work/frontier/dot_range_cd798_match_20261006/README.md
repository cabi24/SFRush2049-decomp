# CD798 checksum/range updater: observed local match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800CD798`, **340 bytes / 85 words**.
The canonical local group comparison reports **MATCH: 0/85 differing words**,
with no unresolved/unverified references, relocation errors or extra nonzero
words. This is an observed local match candidate, not accepted coverage or an
image/ROM claim. The independent checker owns acceptance and integration.

## Source and actual context

`range_update.c` starts from `cloud/work/tiny_A80/func_800CD798.c`. In the current
real hash context that prior body scores 58/85 with an extra-word warning.
Two ordinary C refinements close it:

- Assign the already-used checksum pointer before writing the hash through it.
  This lets the store and following dirty-range calls share the correct address.
- Spell record adjustment as `record = data; record += mode * stride;`, retaining
  the same 96-byte or 64-byte stride. The combined pointer expression was two
  words off after the first change.

The actual mode tests, checksummed regions, writes and six possible dirty-range
calls are unchanged. There are no new reads, padding locals, fake arguments,
qualifiers, helpers or forced registers.

`hash.c` is unchanged accepted `src/blob/groups/codex_hash_a80/group.c`.
The other four real checksum callers are the context carried in PRs #193 and
#194; they are not additional claims in this packet. All five callers are kept,
and the hash is internal. This is a partial-visibility hypothesis: other hash
callers exist, and the original whole-program partition is not established.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_range_cd798_match_20261006
```

Exact source flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
The canonical group pipeline supplies `as1 -r4300_mul` and `-Olimit 5000`.
The empty `claims` list does not assert promotion readiness; the normal command
above scores the member. No broad tests, independent acceptance replay,
image/ROM integration or CI watching was performed before publication.
