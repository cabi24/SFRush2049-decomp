# w38i: wave-38 integration

Branch `wave37`, integration only (src/, locks, commits). Overlay gates per batch: `ovl_layout build` (both images
byte-identical), `ovl_splice check` (ovl lock intact), `ovl_rom rom` (ROM SHA-1 EXACT); Pi pytest and rocky
`ci_equiv` (CI_FAST=16) before every push. Logs are in the coordinator's scratchpad.

| Batch | Commit | What | Image A | Image B | Pushed |
|---|---|---|---|---|---|
| 1 | 4aa2bdd04 (lane files 15d8abb49) | w38b deleted-inline stubs: 11 image-A groups superseded in place (c93c, w35d_start_screen, callers_deep, availability_refresh, menu_cleanup, seven_option_group, seven_option_init, text_helpers, menu_state, settings_values, dot_cleanup_colortrace; 28 stubs); image B singles func_8038A408/func_803925D0 reverted, re-landed as NEW groups w38b_worlddam/w38b_hud_show (2 stubs) | 127 -> 155/193 (128,284 B) | 41 -> 43/49 (36,288 B) | yes |

Note: commit 4aa2bdd04's message says "30 stubs" for image A; the split is 28 in A plus 2 in B.

## Held

- **menu_renderers** (`w38b/groups/dot_runtime_a_menu_renderers_20261011`, stubs func_803A6F14/1C/24, A -> 158/193):
  installing it breaks `tests/cloud/test_runtime_a_writer_bounds.py` (verify.py requires production group.c to equal
  the receipt's packet). Prepared fix, `held/writer_bounds_superseded_menu_renderers.patch` (handoff section 5
  superseded-copy pattern): byte copies of the current production group.c/group.json under
  `cloud/work/frontier/superseded/dot_runtime_a_menu_renderers_20261011/`; verify.py accepts that copy when production
  differs and requires production members to include the packet's; verification.json re-pins verify.py
  (3cbf3b69... -> 41a7ebf4...). Validated on rocky in a throwaway commit with the install applied: tests/conveyor 2351
  passed, tests/cloud 1109 passed (writer_bounds included). To land: owner applies the patch, then
  `python3 -m tools.conveyor.pipeline.ovl_splice group --targets asm/us/ovl_a --claims all
  cloud/work/frontier/w38b/groups/dot_runtime_a_menu_renderers_20261011` and the overlay gates.
