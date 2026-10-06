# Pak initializer: explicit request-pool endpoint

Observed local research candidate for `car_lod_select` (324-byte target).
Despite its historical label, this routine initializes the Controller Pak
request lists and worker thread.

Branch base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Baseline source/recipe: PR #163 tree
`277d344e5d9a0edf6eea7732fe7d770cb04aed25`,
`cloud/matches/pak_reset_a3724_group/{group.c,group.json}`.

Giving the existing 64-request loop a named, consumed endpoint and initializing
that endpoint first reduces canonical positional differences from **19/81 to
16/81 words**. The endpoint still equals `&D_80144DC0[64]`; no storage bound or
memory access changes. Both complete bodies are 324 bytes. The new local is
used by the real loop condition, not a padding or pressure-only variable.

Register allocation and scheduling in the first initialization loop remain
nonmatching. This is not a match or accepted ROM coverage. The local candidate
comparison has no unresolved/unverified references, relocation errors or extra
nonzero words.

All twelve genuine bodies and the keep list are retained from the baseline;
only this initializer's C changes. A3724 is already a separate candidate in
PR #163, and all other bodies are context with known regressions. Do not replace
production sources with this group. Original visibility and translation-unit
boundaries remain hypotheses.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_pak_init_endpoint_20261006
```

Exact source flags: `-g0 -O3 -mips2 -G 0 -non_shared`.
The canonical group pipeline supplies `as1 -r4300_mul` and `-Olimit 5000`.
No broad tests, independent acceptance replay, image/ROM integration or CI
watching was performed. The independent checker owns acceptance and merging.
