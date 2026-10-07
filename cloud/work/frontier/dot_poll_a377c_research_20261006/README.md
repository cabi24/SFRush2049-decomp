# A377C Controller Pak polling: real loop and bitmap-predicate structure

Observed local research candidate, explicitly NONMATCH.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `track_render_process` at **0x800A377C, 3200 bytes / 800 words**.
Source baseline is the complete real Pak group published in PR163,
`cloud/matches/pak_reset_a3724_group/group.c`, tree
`277d344e5d9a0edf6eea7732fe7d770cb04aed25`; it is not a new-body claim.

Canonical local comparison improves **786/800 to 768/800 differing words**.
The target frame is reproduced: **416 →432 bytes**. The candidate ELF body is
**3188 bytes**, twelve short. This remains an extensive nonmatch, with no extra
nonzero words, unresolved/unverified references or relocation errors.

Two narrowly grounded changes:
- Clear the actual four-byte changed array with a pointer/end loop, retaining
  the target's native loop instead of the previous indexed-loop unrolling.
- Factor the existing bitmap scan into a consumed predicate that returns0 for
  a missing/clean buffer and1 when a dirty byte is found. The native block has
  exactly this separate boolean result before the caller sets dirty flags.
  The bitmap cursor and count remain actual used locals, not frame filler.

The clear-loop control scored769/800, bitmap-predicate control771/800; together
768/800 and the exact432-byte frame. The helper name/identity is a source
hypothesis. All original reads, loop bounds, dirty-flag writes, callbacks and
other control paths are retained. No artificial caller, extra formal, padding,
volatile access or assembly is introduced. Context functions are unchanged and
unclaimed, including the previously published reset body. No production source,
accepted lock or accepted coverage is changed.

## Reproduce

With IDO5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_poll_a377c_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `-Olimit 5000`
and `as1 -r4300_mul`. Independent checker owns acceptance and ROM integration.
