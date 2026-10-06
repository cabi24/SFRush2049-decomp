# Runtime A timed prompt: local standalone match

`func_8038F454`, image A `[0x8038F454,0x8038F648)`, 500 bytes / 125 words.
**Strict MATCH, 125/125**, at normal standalone O3:
`-g0 -O3 -mips2 -G 0 -non_shared` plus the scorer's `-Wab,-r4300_mul`.
Zero differing, unresolved, unverified or extra words. Fixed research base:
`f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

The complete routine carries a timed three-descriptor prompt through initial
state checks, a handle/retry path, first-entry initialization, confirm input,
and deadline expiry. Confirm clears the first player state and sets the other
three to 5 before requesting state 0x10; expiry requests state 8. Fields and
constants are native-derived, and no exact arcade donor or original name is
claimed.

The first natural draft scored119/125 plus one extra word. An early-return
form, unsigned state-word declarations and an indexed player loop reduced the
residual to six register-only words. The locked entity_flags_apply definition
in `src/blob/groups/frontier_camera_target_track/group.c` proves its return and
arguments are `u32 (u32, u32, u32, u8)`. Correcting that real contract, rather
than declaring the ignored result void, fixes all six. The other update helper
is the locked void entity_audio_update. No group context is required.

Source caveat: repeated handle=-1 stores and the subsequent redundant
handle!=-1 test are intentionally retained because the native contains them;
IDO emits the native same-register comparison. This does not establish the
original source or any missing inlined helper identity. PlayerStatus is an
accessed-byte/stride view, not a full original type recovery.

Reproduce from repository root with documented IDO:

```
python3 tools/cloud/score.py fn cloud/matches/ovl_a/func_8038F454.c func_8038F454 --targets asm/us/ovl_a --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

This handoff contains source, flags/context and the observed local score only.
No full tests, semantic proof packet, image/ROM gates, or cartridge-coverage
acceptance is claimed. Independent validation, integration and merging remain
with the checker.
