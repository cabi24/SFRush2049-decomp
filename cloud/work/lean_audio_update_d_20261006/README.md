# audio_update_d: 76-byte real-caller group match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole target/claim: `audio_update_d`, `[0x800F8754,0x800F87A0)`, **76 bytes / 19 words**.

Observed canonical group output:

```text
Members:
audio_update_d:
  MATCH
```

All 19 words match, with zero nonzero excess words, unresolved symbols,
unverified relocations or errors. This is a local group match candidate, not
standalone compilation, a whole-group replacement, independent verification or
accepted cartridge coverage.

## Genuine source / required context

The new helper selects mode zero through the real F857C routine, clears the
ordinary handles, stops/clears the saved UI object if non-null, then clears its
active byte. A consumed local pointer snapshots the object actually loaded for
the stop call. Without that natural local, IDO inlines away the body in this
context. With it, the native out-of-line body and unsaved-register behavior are
reproduced. This compile-affecting spelling is disclosed; it adds no unused
storage, fake input, dummy work or synthetic caller.

`caller.c` carries the complete genuine F87A0/F857C/slot-state source from
`cloud/work/s20261004/B/g87A0/group.c`, with all six stand-ins removed. Its old
approximate F84B0 substitute is removed in favor of the actual accepted body.
It also adds the complete real second caller `finish_state_alt` (556 bytes),
which did not previously have a C definition in the robust base inventory.
That caller is unclaimed and remains NONMATCH. Both real native callers are
present; the target itself is internal and is not forced into the exported roots.

The second caller preserves queue-message byte +2, the actual loop-carried
selection global, the fixed object snapshot across its repeated callback, and
the true F857C mode-1 argument. Pointer-valued UI-object handles/returns have
explicit pointer declarations rather than the old signed-integer guesses.
The two queue states both use the real D_80138870 symbol; the raw decompiler's
spurious D_80140002 alias is not used.

`filter.c` and `clear.c` retain complete accepted bodies with only their used
O32 declarations, avoiding thousands of unrelated preprocessed declarations.
The raw-word handle passed by the clear body is explicitly cast to the actual
stop pointer interface. `stop.c` is the unchanged accepted source, including
its already-disclosed historical compiled-out index read. No new such device is
added here. Existing exact context earns no duplicate claim.

## Reproduce

With the documented IDO 5.3 toolchain, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/lean_audio_update_d_20261006
```

`group.json` records every file, genuine caller/helper, exported root and sole
claim. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`; the unchanged backend supplies
mandatory `-r4300_mul` to the assembler. The historical standalone O2 recipe of
the accepted filter is not used for this group.

## Limits

Caller/helper context is not all exact and is not a complete original module.
In particular, F87A0/F857C/slot-state and the new second caller remain unclaimed
NONMATCHs. No production source or accepted lock is changed. Valid O32
objects/lists, queue contracts and the observed callback interfaces are assumed;
arbitrary corruption/aliasing and whole-game behavior have not been tested.

This lean packet contains only C, required real context, recipe and these notes.
No independent verification, test harness, receipts, full tests, CI wait,
image/ROM integration or merge was performed. The independent checker owns
acceptance and merging.
