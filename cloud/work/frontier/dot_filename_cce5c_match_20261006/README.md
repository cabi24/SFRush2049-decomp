# CCE5C filename update: observed local match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800CCE5C`, **160 bytes / 40 words**.
The canonical local group comparison reports **MATCH: 0/40 differing words**,
with no unresolved/unverified references, relocation errors or extra nonzero
words. This is a local match candidate, not accepted coverage or a linked-image/
ROM claim. The independent checker owns acceptance and integration.

## Source and actual context

`filename.c` is the compact filename-update source from
`cloud/work/r5_h/func_800CCE5C.best2diff.c`, with the hash's actual unsigned return
and consumed argument prototype and typed dirty-mark pointer declarations.
The logic compares eleven name bytes, copies a changed name to both records,
updates the checksum, and marks the fifteen-byte checksum/name range dirty.
Its standalone comparison remains 2/40 words off.

The real hash implementation and actual callers resolve that last temporary
register residual. `hash.c` is unchanged accepted
`src/blob/groups/codex_hash_a80/group.c`. `other_callers.c` contains real
`object_data_allocate` and `menu_dialog_close` bodies. `candidate.c` is the typed
C7578 setting writer from PR #193 (commit
`2ceb5ac356b8308cb3b0c8c04abab3c9bdf21c1c`), included only as real hash context.
In this build the hash also reports MATCH. No additional body is newly claimed.

All four real callers are kept; the hash is internal. This partial visibility is
an explicit source hypothesis, not proof of the original whole-program boundary;
other hash callers exist. No fake caller, false prototype, padding local,
volatile change, invented argument, assembly or forced register is used.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_filename_cce5c_match_20261006
```

Exact source flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
The canonical group pipeline supplies `as1 -r4300_mul` and `-Olimit 5000`.
The empty `claims` list does not imply promotion readiness; the normal command
above scores the named member. No broad tests, independent acceptance replay,
image/ROM integration or CI watching was performed before publication.
