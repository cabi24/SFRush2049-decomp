# Whole-channel Pak flush: observed source improvement

Research candidate, explicitly NONMATCH; no accepted coverage or ROM claim.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `track_collision_setup`, **468 bytes / 117 words**.

The canonical local comparison improves from the C96 baseline's **88/117 to
28/117 differing words**. The refined ELF body is the full 468 bytes, with the
104-byte target frame. Its comparison reports zero unresolved/unverified
references, relocation errors or extra nonzero words.

Changes relative to `cloud/work/game_C96/group_bounds_assignment.c`:

- Factor the existing queue init/receive/send operations through the genuine
  init/lock/unlock bodies used in PR #210. No operation or artificial caller is
  added; helper boundaries remain a source-structure hypothesis.
- Express the dirty-slot test through `D_80144030[index].slots[...]`, as the
  target's indexed access shows, while retaining the channel pointer for its
  busy/result flags.
- Read the existing `node->file` member directly instead of keeping a redundant
  function-local file alias. Reorder the remaining consumed pointer declarations
  to recover node/next spill homes. No padding or unused local is introduced.

The remaining differences are scheduling/control-flow placement and some spill
traffic, especially the initial result clear and queue/list transitions. The
helper's same-line flag/create scheduling sensitivity is inherited and disclosed
in PR #210. All seven real C96 functions remain; the other bodies are context,
not additional claims. Original helper identities/visibility are not established.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_pak_channel_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The canonical pipeline includes `-Olimit 5000` and `as1 -r4300_mul`.
No broad tests, independent acceptance replay, image/ROM integration or CI
watching was performed. The independent checker owns acceptance and merging.
