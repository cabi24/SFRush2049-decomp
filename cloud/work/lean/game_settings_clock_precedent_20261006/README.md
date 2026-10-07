# LEAN RESEARCH: settings animation clock-declaration precedent

This is a first lean handoff of the complete `func_800D6914` settings-button animation and its ordinary/qualified clock controls. **108/315 differing words was already observed as an earlier diagnostic. This is not a newly discovered best score.** The new contribution is locating current accepted-source declaration precedents and making the unresolved shared-contract hypothesis explicit and reproducible at frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result

IDO 5.3 flags: `-g0 -O3 -mips2 -G 0 -non_shared`, with canonical automatic `-Wab,-r4300_mul`. Same complete source and genuine caller, except one target declaration:

- `ordinary.c`: `extern f32 D_8002EB94;`, **264/315 differing words**, 1216 bytes, 128-byte frame.
- `qualified_hypothesis.c`: `extern volatile f32 D_8002EB94;`, **108/315 differing words**, 1256 bytes, 120-byte frame.
- Native target: 1260 bytes, 144-byte frame. Both have zero excess and no unresolved symbols, but 12 unverified owned-data sites remain. Ordinary has two owned-data mismatches; qualified has an inconsistent owned-section address diagnostic. Neither is a match.

`observed.json` preserves full score metadata, hashes and caller results. Raw byte excerpts are redacted. No body shaping or frame-padding change accompanies the qualifier control.

## New declaration precedent, not asynchronous-write proof

At the frozen base:

- [accepted entity_collision_detect source, line 50](https://github.com/cabi24/SFRush2049-decomp/blob/f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2/src/blob/entity_collision_detect.c#L50) declares this same address `volatile f32`.
- [accepted func_8008B3C8 source, line 111](https://github.com/cabi24/SFRush2049-decomp/blob/f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2/src/blob/func_8008B3C8.c#L111) has the same qualified declaration.
- [game_globals.h, line 51](https://github.com/cabi24/SFRush2049-decomp/blob/f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2/include/game_globals.h#L51) retains ordinary `f32` and explicitly notes a volatile quirk in three sources.

The accepted-source comments discuss code generation; they do not independently prove asynchronous mutation or establish the original global declaration. The prior writer audit found initialization before game-thread creation and updates on the same game-loop path. That finding is unchanged. The qualifier is therefore a **shared-declaration precedent hypothesis only**. The original declaration, ownership and writer contract remain unresolved. No interrupt semantics, new async writer, semantic equivalence under concurrent changes, or accepted project-wide contract is asserted.

## Complete source and real context

The ordinary body is the prior complete reconstruction (SHA-256 `769251c4071f728abf9943cf3922b2d784f6069017aa450348c958ef5f3e64c0`). It animates four primary buttons and their partners, updates texture/model state, spins the ninth object and eases its position toward the selected row. The backing table has twelve 64-byte records; only records 0–8 are changed. Valid selected index 0–3, finite floating-point values and valid referenced objects are assumptions. No fallback is invented for an invalid selected index.

The genuine texture helper `MBOX_FindTexture_Err(name,index,err)` follows the source-backed three-argument arcade wrapper documented in `cloud/work/frontier/w4d/RESULTS.md`; its explicit inline spelling is still a reconstruction experiment. The existing texture-count qualifier is unchanged. No fabricated accessors, unused locals, dummy callers, assembly or padding locals are added.

The sole real caller `func_800D7634` is read unchanged from frozen `src/blob/groups/credits_scroll_grp/gr3_e.c` (SHA-256 `2cbf4ef8a30c539e1f10c185049d6b5dc2bb59d86f341221f8d07cf7074e1e29`). This is existing nonmatching context, not newly reconstructed work. It remains the keep root, with D6914 internalized to expose its private ABI. The generated caller header contains historical unused declarations/macros, including an unused integer clock declaration; this experiment does not claim to reconcile that header or use it as qualifier evidence. The full caller body stays identical in both controls and its observed score remains 424/451.

## Minimal replay

With the project's IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_settings_clock_precedent_20261006/replay.py --repo .
```

The replay reads the immutable caller, scorer and protected target/manifests from Git, materializes two temporary two-file groups, compiles and scores. It performs no behavioral or acceptance checks. Only C, replay code and metadata are published; no live source, lock, splice claim, image or ROM is modified. Independent validation, declaration resolution, acceptance and merging belong to the checker.
