# State/pad callback: real-context probe

Status: **COMPLETE-NONMATCH**, no new match or coverage. Baseline `cf10b339`.
Target `state_update_global`, `[0x8010B560,0x8010B5D0)`, **112 bytes**.

The specific new hypothesis was that the now-accepted `Input_ApplyPadConfig`
and its real inlined helper would remove the archived 3-word residual.
It does not: the group has **3/28 fully relocated word differences**, exactly
112 emitted ELF bytes, no masks/unverified/unresolved relocations, and both
accepted context bodies remain exact strict matches. The difference is the
register carrying the raw global value before boolean normalization.

Four bounded controls:

- Archived `cloud/work/near_miss_B/state_update_global_best.c`, O2: 3/28.
- The same archived full source, O3: 3/28.
- Its assignment-in-condition body with exact accepted Sprite context and
  real callee definitions, O3 group: 3/28, 112 bytes.
- Natural cached-boolean typed body in that real group: 7/28, 112 bytes.

The earlier `near_miss_B.md` and `workbench_pilot_C1.md` already cover condition,
local-type, dead-cost, and global-definition variants. Those searches were
not repeated. The genuine callee-context hypothesis is now exhausted for
these source forms. New source or measured allocator evidence is needed.

`group.c` starts with the unchanged accepted group source. Its added callback
uses the exact Sprite view; the observed 32-bit store at offset 0x28 is an
explicit offset access because the accepted Sprite declaration leaves that
part of the object opaque. The address stored in field 4 remains a 32-bit N64
value. No shared type/prototype/source edit is proposed.

The native contract is: normalize `D_80149D98` to a boolean; on a mismatch,
update signed byte 0x1A, apply configuration, then reload that byte. If it is
zero, replace word 4 with the address of `D_80117358`, apply configuration
again, and zero word 0x28 after the call. Always return one. Calls may affect
the byte through external hooks, so the post-call reload is preserved.

Reproduce with the pinned local IDO/toolchain environment:

```sh
python3 cloud/work/dot_state_update_context_20261005/verify.py
python3 tools/cloud/score.py group cloud/work/dot_state_update_context_20261005
python3 -m pytest -q tests/cloud/test_dot_pad_channel_reset.py
```

The canonical scorer exits 1 for this intentionally unclaimed nonmatch.
The verifier checks all actual ELF extents and every relocated word, requires
the unchanged context prefix and both strict context matches, and expects
exactly the recorded three-word residual. No target, lock, accepted source,
flag, symbol, or scoring change; no image or ROM gate or coverage claim.

Host behavior tests cover 64 combinations of global/prior values and callee-side
flag mutations, including both callback paths and the required post-call
reload and zero-store order. These are behavior tests, not matching evidence.
