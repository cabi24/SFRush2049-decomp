# slot_state_setup: real bank-context refinement

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Research target only: `slot_state_setup`, `[0x800B4200,0x800B42E8)`,
232 bytes / 58 words. No matching claim is added.

The complete selector body carried in the real countdown group (#222) scores
18/58 differing words. Adding the current accepted `slot_sound` group changes
the force argument of `sound_update_channel` to its native private register and
reduces the target to **16/58 differing words**, with zero nonzero excess words,
unresolved symbols, unverified relocations or errors. The remaining differences
are the selector-global and resource-global addresses exchanging s0/s1.

This is a context improvement, not a new algorithm or an accepted match. The
selector body is unchanged apart from correcting the external resource-lookup
second parameter to the accepted signed-byte type. It preserves the native
byte reloads after calls, two resource lookups/loads, cache refresh and optional
byte-9 setter. No artificial call, local, pressure array or volatile is added.

## Real context

`group.json` declares every source, member and exported root. The five countdown
sources preserve the real callers and complete filter/clear/stop/shutdown bodies
from #222. `bank.c`, `func_800A4E58.c` and `entity_render_mode.c` are the current
accepted `src/blob/groups/slot_sound` files, with their existing root visibility.
Their full bodies and real callers establish the private bank-refresh contract.
The old synthetic callers from archived resource packets are not included.

The accepted bank debug hook's pre-existing compiled-out switch, and the accepted
stop helper's documented compiled-out index read, are carried unchanged. They
are explicit inherited compiler-affecting context, not new selector devices.
`object_byte9_set` stays an ordinary external call: adding its real small body
here caused inlining into the selector and worsened the extent. No fake keeper
was introduced. The real resource lookup body also supplied no score improvement
and is left external with its accepted signed-byte interface.

All existing matching bank/font/selector helpers, the #214 shutdown and the
#222 finish-state caller are context only. They are not additional matching
credit. The nonmatching countdown context must not replace production owners.

## Reproduce

```sh
python3 tools/cloud/score.py group cloud/work/lean_slot_state_setup_20261006
```

Use the documented IDO 5.3 toolchain and exact flags
`-g0 -O3 -mips2 -G 0 -non_shared`, with the canonical assembler's mandatory
`-r4300_mul`. The packet does not alter target data, scorer, lock or production
source. It assumes the existing native O32 resource/bank layouts and accepted
external contracts. Only the local canonical group comparison above was observed;
independent checking, integration and accepted coverage remain separate.
