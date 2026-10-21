# w36i: wave-36 integration

Branch `wave35`, integration lane only (src/, locks, commits). Every batch: `ovl_layout build`,
`ovl_splice check`, `ovl_rom rom` (log `gates_batchN.log`: image gates byte-identical, ovl lock intact,
ROM SHA-1 EXACT); tests on rocky (clean clone, authoritative) and the Pi before any push.

## Batches

| Batch | Commit | What | Bytes | Image A | Image B | Pushed |
|---|---|---|---|---|---|---|
| 1 | 2bc8a8558 | w36a updateCarScreen, showCarSelect, _InitScreen, createGridMap (dot_cleanup_colortrace_20261011) | +15,000 | 90 -> 94/193 | 25/49 | yes (a14da0b24) |
| 2 | ecd144bde | w36g circuitscr + cheat screen, w36f recordscr/modescr/stats | +18,208 | 94 -> 107/193 | 25/49 | no (see batch 3) |
| 3 | a5a4ccc1a | back out w36f's dot_runtime_a_seven_option_group_20261007 supersession (func_803993EC, func_80399D10) | -2,568 | 107 -> 105/193 | 25/49 | yes (a5a4ccc1a) |
| - | 8754d42bc | w36y research commit (MAT3 -> N64 data-symbol map, struct layouts; not applied) | 0 | | | |
| 4 | (this) | w36j image B: animStuntPoints, animHealth, newWorldDamage, ShowHUDExtra (standalone) + UpdateBattlePowerUps (dot_runtime_b_resource_callbacks_20261012 superseded in place) | +6,120 | 105/193 | 25 -> 30/49 | |

## Held for owner

### w36f: func_803993EC (updateButtons@modescr.c) + func_80399D10 (updateModeScreen)

- Failing test (rocky): `tests/cloud/test_runtime_a_capacity_evidence.py::test_runtime_a_capacity_evidence`
  -> `ValueError: inherited genuine group source differs from production`
  (`cloud/work/frontier/dot_runtime_a_capacity_evidence_20261012/verify.py:262-265`). The receipt compares
  its inherited eligibility.c with `src/ovl_a/groups/dot_runtime_a_seven_option_group_20261007/eligibility.c`;
  w36f's revision extends that file in place.
- Batch 3 re-installed the pre-batch-2 group from
  `cloud/work/frontier/superseded/dot_runtime_a_seven_option_group_20261007/` (byte-identical to a14da0b24;
  README added), so production matches the receipt again.
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/capacity_evidence_superseded_seven_option.patch`.
  verify.py accepts the superseded copy when production differs (eligibility.c and renderer.c must equal
  the packet's, and production must still lock every member the packet claims); verification.json
  re-pins verify.py (0c95de4b... -> 1074fae3...). Handoff section 5 "superseded context group" pattern.
- To land after the patch: `git apply cloud/work/frontier/w36i/held/capacity_evidence_superseded_seven_option.patch`,
  then `python3 -m tools.conveyor.pipeline.ovl_splice group --targets asm/us/ovl_a --claims all
  cloud/work/frontier/w36f/groups/dot_runtime_a_seven_option_group_20261007`, gates, tests (+2,568 B, A +2).

### w36j: func_8039244C (RemoveHUDExtra@hudextra.c, 380 B, standalone strict MATCH)

- Failing test if landed: `tests/cloud/test_runtime_b_teardown_packet.py::test_packet_is_research_only`
  asserts `cloud/matches/ovl_b/func_8039244C.c` does not exist (frozen NONMATCH research packet
  cloud/work/runtime_b_teardown_20261006, 34 words).
- Not spliced; `cloud/matches/ovl_b/func_8039244C.c` stays untracked where w36j left it (this is also why the Pi
  run fails that test; rocky's clean clone does not see it).
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/teardown_packet_allow_promoted_8039244C.patch`
  (handoff section 5 "function not yet locked" pattern: the file may exist once func_8039244C is locked from
  src/ovl_b/func_8039244C.c).
- To land after the patch: `python3 -m tools.conveyor.pipeline.ovl_splice splice cloud/matches/ovl_b/func_8039244C.c`,
  gates, tests (+380 B, B +1).
