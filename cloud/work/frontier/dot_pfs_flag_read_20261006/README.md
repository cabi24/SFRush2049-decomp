# PFS dirty-bitmap caller research

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Prior source: `cloud/work/game_C96/group_bounds_assignment.c` and its complete
seven-function real-caller closure. This is an observed local research candidate,
not an accepted match or ROM-coverage claim.

## Changes and local observations

- `slot_state_lookup` (188-byte target): restore its otherwise-unused slot flag
  read before `func_800A1DD4`. The target reads the signed byte at channel-record
  offset `0x86` (slot offset 2). The previous C96 source omitted it. Representing
  that specific read with a volatile field is an inference about source semantics,
  not proof of the original qualifier. Local strict comparison improves from
  **45/47 to 40/47 differing words**; emitted body is 188 bytes.
- `func_800A1DD4` (192-byte target): express the assigned upper-bound comparison as
  `((address < (map = base + file->data_size)) == 0)`. Against the bounds-preserving
  prior source, strict comparison improves **44/48 to 42/48 differing words**.
  The emitted body remains short at 184 bytes. An older C96 spelling that loses
  the observed bounds flow scores 38/48; this does not beat that raw score.

Both reported comparisons have zero unresolved references, unverified references,
relocation errors or nonzero extra words. These are positional full-word results,
not relocation-blind or aligned-row scores. Neither function matches.

`group.c` retains all seven genuine C96 bodies, with unchanged required SDK types
copied from this base's headers. Its five context functions are not new claims.
Unknown record members describe existing storage; no padding locals, invented
callers, false prototypes or assembly shaping were introduced. Context callers
remain nonmatching. The independent checker owns acceptance and integration.

## Reproduce

From this base with IDO 5.3 available through `IDO_DIR`:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_pfs_flag_read_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The existing group scorer runs its normal cc/uld/usplit/umerge/uopt/ugen/as1
pipeline, including `-Olimit 5000` and the as1 `-r4300_mul` flag. The kept external
entry points are `track_collision_setup`, `MaxPathZeroControls` and
`slot_state_lookup`; the other genuine members remain internal. No broad tests,
ROM splicing or ROM hash check were run for this research publication.
