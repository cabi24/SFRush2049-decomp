# Image A player-count menu pair: lean research

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

- `A:func_803ADDA8`, `[0x803ADDA8,0x803AE1C0)`, 1,048 bytes:
  **24/262 words differ** in the real caller group.
- `A:func_803AE1C0`, `[0x803AE1C0,0x803AE51C)`, 860 bytes:
  **14/215 words differ**.

Both comparisons have zero extra words, unresolved symbols, unverified
relocations or errors with authenticated image-A data. The projected helper's
standalone baseline was 259/262 differences plus twenty extra words. Its real
caller provides the native interprocedural save/restore context. Neither member
is a matching claim; frame/local and allocation/scheduling differences remain.

## Reproduce

```
python3 cloud/work/lean/runtime_a_player_menu_20261006/reproduce.py
```

IDO 5.3 whole-program O3: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical
`uld`/`usplit`/`umerge`/`uopt`/`ugen`/`as1`, and `as1 -r4300_mul`.
Only the real AE1C0 callback root is kept. The two complete source bodies
are provided, without keeper stubs or invented callers; claims remain empty.
Original TU/private visibility is a hypothesis supported by the direct call
and native callee clobbers. A:8038F744 remains an external eligibility service.

The minimal helper authenticates the frozen asset/image A in memory, then
compiles/scores with the unchanged canonical tool. Five projected-view angle
literals at A:0x803B99C0..0x803B99D0 are checked by content. No bytes/dumps/
objects are included. Only compile/scoring was performed.

## Source and assumptions

The root chooses a projected menu or flat menu. Both draw ten eligible options
and add four player-count choices to the first option, with the native selected,
unavailable and shadow/color paths. The projected helper uses 60-byte slots;
its position/angle view differs from the 64-byte slots used by other screens.
Float row coordinates and the flat view's 22.5 row step are retained, including
conversion at draw boundaries. The otherwise unused height-helper call in the
flat view is a real observed external call and is preserved.

The two four-byte word-aligned by-value color aggregates retain their observed
ABI; shared-header reconciliation with the older byte-only setter view remains
a checker task. External helpers and data are unchanged. Original names,
complete record meanings, array declarations and whole-function arcade ancestry
are unknown. Each candidate's 48-byte text buffer must hold its formatted text
including NUL. Valid language tables, ten slots and projection results are
required; projected float-to-short conversions need finite representable values
for portable C behavior. No padding/unused-pressure variables, invented
prototypes, volatile or inline assembly.

Observed score improvement is not accepted coverage. Independent checker owns
further validation, acceptance and integration.
