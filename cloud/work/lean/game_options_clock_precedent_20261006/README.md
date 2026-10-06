# LEAN RESEARCH: options animation clock-declaration hypothesis

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Target `func_800DA2C0` spans 2316 bytes / 579 words. This handoff retains the prior complete ordinary-clock reconstruction and genuine `func_800DB1E0` caller, and adds one explicitly unresolved declaration control informed by existing accepted source. It is not an accepted global contract or a matching result.

## Paired observation

IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`, canonical automatic `-Wab,-r4300_mul`:

- Ordinary retained body: **505/579 differing words**, 2204-byte ELF function, 152-byte frame.
- Same body with only `D_8002EB94` declared `volatile f32`: **485/579 differing words**, 2268-byte ELF function, 152-byte frame.
- Native function: 2316 bytes and 152-byte frame.
- Both controls: zero excess words, no unresolved symbols or scorer errors, **12 unverified owned-data relocation sites**. No fully verified match is claimed.
- Unchanged real caller: **215/350 differing words**, 1396 bytes against 1400 native, frame 128 in both, two unverified owned-data sites.

These are paired results for the retained complete source, not a claim that every historical source variant scored worse. Prior ordinary-clock expression experiments included other nonmatching results. The new experiment does not repeat that expression search or add frame filler.

## Why retain this hypothesis

The same exact clock address is declared qualified in frozen accepted sources:

- [entity_collision_detect.c:50](https://github.com/cabi24/SFRush2049-decomp/blob/f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2/src/blob/entity_collision_detect.c#L50)
- [func_8008B3C8.c:111](https://github.com/cabi24/SFRush2049-decomp/blob/f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2/src/blob/func_8008B3C8.c#L111)

Conversely, [game_globals.h:51](https://github.com/cabi24/SFRush2049-decomp/blob/f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2/include/game_globals.h#L51) retains an ordinary declaration with a comment noting a volatile quirk in three sources. The accepted-source comments discuss generated load shape, not independent writer evidence.

The earlier direct-writer audit traced initialization and game-loop updates, without proving asynchronous mutation. That limitation remains. Repeated native reads do not prove the original qualifier. This control tests a **shared-declaration precedent hypothesis**; original declaration, ownership, aliasing and synchronization remain unresolved. No newly found asynchronous writer or semantic equivalence with a concurrently changing clock is asserted. No global project declaration or accepted source is changed.

## Complete bodies and genuine context

The ordinary source is the prior complete DA2C0 reconstruction, not newly recovered work. It updates fourteen primary buttons, fourteen companion labels and the arrow at index 28 using real 64-byte records; handles, matrices, angle, position and alpha offsets are preserved. All native texture/model operations and the three-argument source-backed `MBOX_FindTexture_Err` wrapper remain. The wrapper's provenance is `cloud/work/frontier/w4d/RESULTS.md`; its explicit inline spelling remains a reconstruction hypothesis.

`caller.c` is the existing complete DB1E0 caller, unchanged from the earlier handoff (SHA-256 `37da94b5178384b1e2d85bb08685f242d76e99b072608e541d8cbf0858aba8a0`). It preserves actual input selection, enabled-entry searching, scrolling, option dispatch, DA2C0 invocation and cleanup. The compiler keep root is this real caller; DA2C0 is internalized to expose the observed private ABI. This is not a synthetic call-site fixture. Both bodies were previously reconstructed against base `9651ca53bfd16b015adb63e1b3d76487a995a84f`; this packet freshly compiles them against the frozen base above. Source identities are recorded in `observed.json`.

Runtime assumptions include valid selected index 0–13, at least two enabled entries where the selected-position ratio divides by count minus one, adequate 29-record backing storage, valid texture/model handles and finite floating-point values. The source does not invent fallback behavior outside the native valid domain. Unknown record bytes express native layout; there are no padding locals, unused pressure variables, fake helpers, assembly, or changed compiler flags.

## Minimal replay

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_options_clock_precedent_20261006/replay.py --repo .
```

The script reads the frozen scorer and protected target/manifests from Git into temporary storage, builds both two-file groups and prints metadata. It performs no behavior harness or acceptance checks. The publication contains only C, exact replay code, scores and assumptions. It adds no locks, splice claims or image/ROM integration. Independent verification, declaration resolution, acceptance and merging are left to the checker.
