# A2D4C Controller Pak allocation: stack-object research delta

Research NONMATCH for historical `track_process_main`, protected address
`0x800A2D4C`, 1,752 bytes / 438 words. The routine allocates a Controller Pak
file, handles retry/replacement callbacks, reads its file state, and builds a
buffered request. Its historical track/geometry label is misleading.

## Actual comparison baseline

Branch base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
The stronger available source is [PR #163](https://github.com/cabi24/SFRush2049-decomp/pull/163),
commit `1bd09c5eb3bde8f803c46d0d657e9264d4223a38`, tree
`277d344e5d9a0edf6eea7732fe7d770cb04aed25`,
`cloud/matches/pak_reset_a3724_group/group.c` and `group.json`.
This packet depends on that exact real context, not the incomplete master
skeleton. It adds no claim of discovering the full source or queue helpers.

Both runs use the same complete PR #163 group and retained roots. Only the
A2D4C body is replaced. Flags are `-g0 -O3 -mips2 -G 0 -non_shared`, through
`tools.cloud.score.compile_group`; the canonical assembler adds `-r4300_mul`.

| Result | Differing words | ELF words | Frame | Extra nonzero words |
| --- | ---: | ---: | ---: | ---: |
| PR #163 baseline | 348 / 438 | 444 | 288 | 6 |
| Candidate | 335 / 438 | 444 | 280 | 6 |

Both have zero unresolved relocations, zero unverified relocations, and zero
score errors. Existing `func_800A3724` remains MATCH in both runs; that is
context preservation, not new matching credit.

## Narrow source change

Order the existing declarations to follow the observed address-taken objects:
file number at frame +272, encoded name at +256, encoded extension at +252,
retry at +251, status at +250. Remove the redundant `file_size` local, consuming
the loaded size through the actual request field. This produces the native
280-byte frame and all five object offsets. The four genuine lock helper
message slots remain +220, +196, +172, and +140. No object was enlarged and no
filler local, synthetic call, padding carrier, or qualifier was introduced.

The queue helper restoration itself was already present in PR #163. Accepted
source `cloud/matches/func_8008A704.c` and `cloud/matches/audio_queue_process.c`
at the branch base corroborates its initialization/lock/release operations.
The SDK names `osPfsReadWriteFile` and `osPfsDeleteFile` retain the repository's
historical relocation identity; `include/PR/os_pfs.h` documents their actual
allocation/file-state interfaces.

## Reproduce

With the canonical compiler environment and the pinned PR #163 tree available
in a local repository:

```sh
python3 cloud/work/frontier/dot_pak_allocate_20261006/repro.py \
  --context-repo /path/to/repository-containing-pr163-tree
```

The script reads only the two pinned source/config files, compiles baseline
and candidate, and reports canonical scores. Temporary groups and objects stay
in `build/`. It does not fetch data, alter protected targets, or perform an
acceptance test.

## Remaining uncertainty

335 words still differ and six extra nonzero words remain. This is a small
layout improvement, not a match or behavior-equivalence claim. Original local
spelling/order and the full N64 compilation-unit boundary are not proven.
PR #163's reconstructed types and other private-call interfaces, including
its stated `no_catchup` single-formal hypothesis, remain inherited context.
The original six-argument entry and the observed object offsets are retained.
The independent checker owns acceptance, integration, and any ROM validation.
