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
| 4 | 0ce59e845 | w36j image B: animStuntPoints, animHealth, newWorldDamage, ShowHUDExtra (standalone) + UpdateBattlePowerUps (dot_runtime_b_resource_callbacks_20261012 superseded in place) | +6,120 | 105/193 | 25 -> 30/49 | yes (0ce59e845, with 8754d42bc) |
| 5 | cc56165ba | w36c updateSelectButtons/controlSelect@recordscr + w36b initRecordScreen (+stub) merged into dot_runtime_a_callers_deep_20261011 with the installed w36f revision; w36c controlSelect@playerscr (dot_runtime_a_availability_refresh_20261007); w36b setGhosts/updateCarComponents (dot_cleanup_colortrace_20261011) | +14,824 | 105 -> 112/193 | 30/49 | yes (cc56165ba) |
| - | 0cad8af34 | owner: research receipt re-pins for the three held items | 0 | | | with batch 6 |
| 6 | dabdcf959 | formerly held: w36f updateButtons/updateModeScreen@modescr (seven_option group), w36j RemoveHUDExtra (B), w36m animResults/animFlag/animLapTimeText/func_80106874 (game credits_scroll_grp) | +2,568 A, +380 B, +6,224 game | 112 -> 114/193 | 30 -> 31/49 | yes (dabdcf959, with 0cad8af34) |
| 7 | (this) | w36e trackscr (dot_runtime_a_c93c_closure_20261012: 11 new claims incl. w36d's BBB4/DFEC) + startscr initScreen (w35d_start_screen); w36h shots (NEW group w36h_shots, image B, 10 members) | +10,376 A, +12,360 B | 114 -> 126/193 | 31 -> 41/49 | |

## Held for owner

The owner applied the three receipt patches below himself (0cad8af34) and the items landed in batch 6.

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

### w36m: credits_scroll_grp superseded in place (animResults, func_80109F54, func_801084D4, func_80106874; 6,224 B)

- Not landed. Failing test if landed: `tests/cloud/test_slot_contract_audit.py::test_fresh_source_bound_census`
  (`cloud/work/frontier/dot_slot_contract_audit_20261006/audit.py` binds the sha256 of
  `src/blob/groups/credits_scroll_grp/group.json`, and its summary's `unaccepted_caller_bytes` /
  `bounded_dependency_bytes` move when slot_state_setup callers land: 36820 -> 30596, 4520 -> 3016).
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/slot_contract_audit_w36m_credits_scroll.patch`
  (evidence.json regenerated by the audit's own collect() against the landed state, simulated: group.json as
  install_group writes it without --provenance, the four names locked; test accepted list and counts updated).
  Land with `PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py
  cloud/work/frontier/w36m/groups/credits_scroll_grp credits_scroll_grp`, then the game gates and `blob_rom rom`.

### w36e: func_80390BC0 (LoadSelectCar@selectcarload.c, 364 B, standalone strict MATCH)

- Not spliced (`cloud/matches/ovl_a/func_80390BC0.c` stays untracked). Failing test if landed:
  `tests/cloud/test_runtime_a_resource_cache.py::RuntimeCachePacket::test_frozen_artifact_hashes_and_honest_claim`
  (asserts the file is absent; frozen NONMATCH packet dot_runtime_a_resource_cache_20261006).
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/resource_cache_allow_promoted_80390BC0.patch`
  (promoted state allowed: the file may exist once func_80390BC0 is locked from src/ovl_a/func_80390BC0.c).
- To land after the patch: `python3 -m tools.conveyor.pipeline.ovl_splice splice cloud/matches/ovl_a/func_80390BC0.c`.

## Batch 5 merge (dot_runtime_a_callers_deep_20261011)

Packet `groups/dot_runtime_a_callers_deep_20261011/`: the installed group.c (w36f: func_803923B0 member) with
w36c's 9338C region and 915E8 block and w36b's 93B3C/93B34 region taken verbatim (three-way base a14da0b24;
the only shared-declaration edit, volatile D_80140BDC / D_8002EB94, is identical in all three). Builder
(`score.py group`, no --claims): all 11 members MATCH; `--claims` rc=0 (`score_merged_callers_deep_claims.txt`).
The builder also rescored w36c's availability_refresh revision (9 members MATCH, context 9842C 2/451), w36b's
carscr revision (14 members MATCH) and the unchanged runtime_a_profile_menu_20261006 (4 members MATCH;
96FA0 is a stand-in context there): `score_batch5_groups.txt`.
