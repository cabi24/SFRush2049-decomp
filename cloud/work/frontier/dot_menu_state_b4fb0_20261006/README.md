# B4FB0 menu-state setup and its real B438C heading helper

New source-context research, **NONMATCH**. Base:
`f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
No complete definition of either body was found in the master source index
or the supplemental PR #163 tree `277d344e5d9a0edf6eea7732fe7d770cb04aed25`.
Historical audio-like names describe menu UI operations here.

## Canonical comparison

`func_800B4FB0`, `0x800B4FB0`, 1,472 bytes / 368 native words:

| Same real context | Differing words | Emitted words | Frame |
| --- | ---: | ---: | ---: |
| Initial complete source, baseline.c | 332 / 368 | 356 | 112 |
| Shared state-local coordinates/widths, candidate.c | 316 / 368 | 363 | 88 |

Both have zero extra nonzero words, unresolved/unverified relocations, and
score errors. The candidate restores the native 88-byte frame without filler
locals or size tuning. Five words are still missing from its ELF extent.
This is development-stage progress on new source, not accepted-byte credit.

The source reuses the actual x/y/width/half-width variables across mutually
exclusive states instead of retaining separate seed names for each branch.
It preserves state 1, state 6, state 7/8, font selections 13/10/11, signed
coordinate divisions, repeated width calls, localized-label reads, panel
creation, and sprite-handle stores.

## Real helper context

- `voice.c` is the actual B438C `voice_stop` heading routine, 552 bytes / 138
  native words. It formats or measures a localized heading and creates/resizes
  its panel. Current score: 130/138, 137 emitted words, frame 144 versus native
  152, zero extra words, unresolved/unverified relocations, or score errors.
- The real formatter destination starts at native frame +72, leaving an
  80-byte region before the caller frame. `char text[80]` represents this
  observed available span. **Original capacity is inferred, not proven.** It
  is consumed by the real formatter and width routine; no size sweep was used.
  Formatting bounds and unrestricted behavior equivalence remain unproven.
- `context.c` is the clean complete C104 context from base path
  `cloud/work/dot_selector_d9058_20261006/context.c`, also preserved in PR #163's
  gear-label packet. It contains the real selector, refresh, empty hook,
  object-create and world-trigger operations. D_80149B70's declaration alone
  is harmonized to the accepted signed-byte width contract; byte stores are
  unchanged. The older artificial dead-switch/read inline blocker is absent.
- `font_metrics.c` retains the exact accepted width body from base path
  `src/blob/groups/codex_sound_channel_extra/object_manager_update.c` and
  `object_bytes_sum_global` from that directory's `group.c`. The documented
  inherited monospace `prev = ch` convention is unchanged.

This clean context does not reproduce the full original private-register
composition. Other residuals are slot_state_setup 30/58 (three extra nonzero
words), sound_update_channel 120/122, object_byte9_set 6/16, func_80096288 3/4,
object_create 1/28, world_trigger_check 39/54 (three extra), object_manager_update
23/133, and object_bytes_sum_global 14/21. All have zero unresolved/unverified
relocations and score errors. They remain unclaimed context, not promoted
replacements or new matching credit.

B4FB0 has an ordinary saved-register entry. B438C is private; its other real
caller, RaceStateMachine_Update, is outside this bounded group. The known
B4E68 cleanup remains an external declaration with its accepted interface.
Original source names/TU, full helper ABI, and behavior equivalence are not
claimed. The independent checker owns acceptance and integration.

## Reproduce

Flags are `-g0 -O3 -mips2 -G 0 -non_shared`, through canonical `compile_group`,
including its normal as1 `-r4300_mul` workaround. All real files and retained
roots are in `group.json`; claims remain empty.

```sh
python3 cloud/work/frontier/dot_menu_state_b4fb0_20261006/repro.py
```

The script compiles and scores baseline/candidate with identical context,
then reports complete ELF extents, frames and relocation counts. Both results
were freshly reproduced after recovery from an executor reset. No behavior
harness, independent review, full tests, CI wait, or ROM gate precedes this
research publication. No raw assembly or binary artifacts are included.
