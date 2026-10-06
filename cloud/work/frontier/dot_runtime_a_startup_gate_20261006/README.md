# Runtime A startup/input gate match candidate

Function: `func_8039A2A0`, runtime image A, `[0x8039A2A0, 0x8039A440)`.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Source: `cloud/matches/ovl_a/func_8039A2A0.c`.

Observed local scorer result: **MATCH**, all 104 words / 416 bytes, no extra,
unresolved, or unverified words. Source uses IDO 5.3 with the first-line flags
`-g0 -O3 -mips2 -G 0 -non_shared`; the canonical scorer adds its mandatory
`-Wab,-r4300_mul`. No caller bodies, inlined helpers, deleted-static stubs,
owned data, or group context are required.

## Reproduce the observed score

From the repository root with the documented IDO and MIPS toolchain installed:

```sh
python3 tools/cloud/score.py fn cloud/matches/ovl_a/func_8039A2A0.c func_8039A2A0 --targets asm/us/ovl_a --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

The routine snapshots controller input before initialization, then takes an
immediate state transition or manages a timed two-descriptor prompt. Natural
O3 loop unrolling and declaration order reproduce the target. Historical
helper names are address identities; no exact arcade donor or original name
is claimed. Extern declarations and the O32 MultiBlit layout are in the source.

The carried callback uses an explicit seven-argument outer ABI. The nested
integer predicate's argument ABI remains unresolved and is non-prototyped;
its observed address `0x8009A6A4` is inside the protected `render_display_list`
extent rather than a separately proved function entry. This candidate invokes
neither callback. Full helper behavior and reachable engine states remain
assumptions for independent review.

This is an observed local matching candidate, not independently accepted image,
compression, ROM, hardware, or cartridge coverage. Acceptance tests and CI
review are left to the independent checker. Only source and these notes are
submitted; local experimental proof/test artifacts are intentionally omitted.
