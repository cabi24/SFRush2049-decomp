# D71D0 menu setup: new complete native C and genuine caller context

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `drone_pathfind_main` at **0x800D71D0, 1124 bytes / 281 words**.
The historical name masks audio-options/menu resource setup. Neither the frozen
master definition index nor PR163 supplement contained a native-backed body.

For the final source, canonical local comparison improves from **276/281** in a
kept-target control to **174/281** with its real D7634 caller, then **167/281
differing words** with the genuine speed-command closure. The kept-target control
has five extra nonzero words and16 unverified own-data references. The published
context has **zero extra nonzero words**, no unresolved references or errors, but
**10 own-rodata references remain unverified** because their positions do not yet
align with the target. These are not masked away or called verified matches.

The emitted target has the correct **1124-byte ELF extent**. Its frame is80 bytes
versus the native168, so this remains a substantial contextual nonmatch.
The first direct draft was269/281 with14 extra words and an own-data mismatch;
that draft is not claimed better than the final contextual result.

The complete body preserves the twelve real64-byte menu records, actual36-byte
basis copies, three texture lookups, transforms/positions, blit creation, input
reset and audio-command dispatch. No guessed local string buffer is introduced.
Consumed angle/height/depth values are shared across their actual uses; the
record loop terminates at its native fixed endpoint. External string resources
remain named address-based references, not copied image data.

`caller.c` is the existing full D7634 body from
`src/blob/groups/credits_scroll_grp/gr3_e.c`, with only required declarations and
contract repairs: D71D0 consumes no formals; speed_set takes floats then flags;
D6160 and D6E00 take their actual single input, and the slot allocator returns a
pointer. Those repairs remove old generated extra-argument/interface mistakes,
not game behavior. `speed.c` is the real accepted
`src/blob/groups/codex_vsync_a145/group.c` closure. Other bodies are unclaimed.
Recovered field meanings and partial compiler visibility remain hypotheses.

## Reproduce

With IDO5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_menu_setup_d71d0_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `-Olimit 5000`
and `as1 -r4300_mul`. No production source or accepted lock changes.
Independent checker owns acceptance and ROM integration.
