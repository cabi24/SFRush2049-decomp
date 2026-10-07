# C7EC8 record registration: repair native handle and list dataflow

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800C7EC8`, **628 bytes / 157 words**.

This complete C reconstruction repairs the sole indexed old skeleton in
`cloud/work/ipa-groups/codex_func_800C7200/group.c`. That skeleton declared no
argument, read an uninitialized `saved_reg_s0`, omitted the second consumed
`draw_speedometer` argument and truncated a handle on its second call. It also
confused the list header with its head and advanced the option cursor before
storing the first result. These are defects in prior reconstructed C, not claims
about game bugs.

Native input/call evidence is the real `func_800C813C` caller and target dataflow:
C7EC8 consumes its slot handle; draw_speedometer consumes (created, repair=0/1);
list head is header+8; the option loop writes offsets 0..20. The two callbacks
are named symbols rather than numerical function addresses. Record-name compares
use byte offset20, and the option result is shared by byte and halfword stores.
All success/failure paths and final list insertion/cleanup calls are retained.

For the final corrected source, canonical local comparison improves from a
**157/157** kept-target control to **138/157 differing words** with the genuine
C813C caller kept and C7EC8 internal. This compares compiler contexts of the new
corrected body, not a score claimed for the invalid old skeleton. The candidate
ELF body is **588 bytes**, forty bytes short of the target. The kept-target
control is 640 bytes and has three extra nonzero words. The published caller
context has no extra nonzero words, unresolved/unverified references or
relocation errors. A bounded real draw_speedometer/hash context was worse and
is not included. Neither caller nor this target is claimed matched.

Recovered field meanings/types and partial compiler visibility remain hypotheses.
The byte-offset views describe actual fields; no artificial caller, padding,
unused formal, register binding, volatile access or assembly is introduced.
No production source or accepted lock is changed.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_registration_c7ec8_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `-Olimit 5000`
and `as1 -r4300_mul`. Caller body is extracted unchanged from
`cloud/work/ipa-groups/codex_func_800C7200_2/group.c` with necessary declarations.
Independent checker owns acceptance and ROM integration.
