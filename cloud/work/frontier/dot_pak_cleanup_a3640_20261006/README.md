# Controller Pak cleanup: nested allocation local

Research candidate, not a matching or ROM-coverage claim.

Branch base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Real caller source/recipe: PR #163's immutable tree
`277d344e`, `cloud/matches/pak_reset_a3724_group/{group.c,group.json}`.

For `func_800A3640` (204-byte target), declaring and initializing `buffer` inside
its nonnull branch reduces the local canonical positional comparison from
**31/51 to 29/51 differing words**. The prior form declared `buffer` outside the
loop and assigned it before the condition. The read itself is retained, with no
new memory access, qualifier, helper, argument, padding or stand-in.

Both runs report **2 extra nonzero words** after the target extent. The prior
packet identifies neighboring deleted queue-helper stubs there. This publication
preserves the canonical warning and does not use it to claim a match. Remaining
register allocation and private heap-call-interface differences are substantial.

The twelve genuine bodies and keep list are unchanged except for this local C
refinement. All other bodies are context, including A3724, which is already a
separate candidate in PR #163. The context has known accepted-source regressions;
do not replace production sources with this group. Original visibility and
translation-unit boundaries remain hypotheses. No broad tests, independent
acceptance replay, image/ROM integration or CI watching was performed.

## Reproduce

With IDO 5.3 available via `IDO_DIR`, run from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_pak_cleanup_a3640_20261006
```

Exact source flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
The canonical group pipeline adds its normal `as1 -r4300_mul` and
`-Olimit 5000`. The checker owns acceptance and integration.
