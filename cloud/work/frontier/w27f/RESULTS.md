# w27f — credits_scroll_grp reconciliation (w27b + w27c)

Packet: `cloud/work/frontier/w27f/credits_scroll_grp/` (members credits_scroll, func_80100B8C, slot_state_setup,
world_trigger_check; claims slot_state_setup, world_trigger_check).

## Cause of `slot_state_setup: 2 extra words`
The naive merge (`w27b/groups/credits_scroll_grp_merged/`) took w27c's group.json `keep` list, which lacks
w27b's two additions `world_trigger_check` and `__standin_func_800B6748`. Without `keep`, world_trigger_check has no
caller in the group, so umerge deletes it and leaves a 2-word deleted-procedure stub (`jr ra; nop`). That stub lands
straight after slot_state_setup, which then shows "2 extra words (nonzero beyond target length)". world_trigger_check
itself scored `54/54 words differ` (`got 03e00008 jr ra / 00000000 nop`). The stand-in caller was not kept either, so
it was dropped too. It was not caused by D_80118E20, file order, `__unit_dead` or func_800B669C. The only fix is
`keep += ["world_trigger_check", "__standin_func_800B6748"]`. All six .c files are unchanged from the merged packet
(installed w27c files + w27b gr3.c/gr3_b.c/gr3_w.c).

## Devices (all kept, all still needed)
- w27c gr3_d.c: `Pair D_80118E20;` definition, unused `char buf[40]` (exact frame residual 144 vs 104), `== 1U`, and
  `dispatch_handler(1)` inside the if. These are unchanged from the installed lock.
- w27b gr3_b.c `do {} while (0);` in slot_state_setup: needed. Without it: `slot_state_setup: +0x000 want lui s1 got lui s0`.
- w27b gr3.c `if (0) { __unit_dead[...] }` in the object_byte9_set context copy: needed. Without it:
  `slot_state_setup: +0x0c4 want bnezl s2,0x14 got bnezl s2,0x24` (inlined).
- w27b gr3_w.c: byte-identical context copy of func_800B6748 plus the kept stand-in caller `__standin_func_800B6748`.

## Verification
Builder (watchman2 scratch w27f, IDO 5.3), `python3 tools/cloud/score.py group g/final --claims` (rc=0):
```
Members:
slot_state_setup:
  MATCH
world_trigger_check:
  MATCH
Context (informational; excluded from exit status):
credits_scroll:
  MATCH
func_80100B8C:
  MATCH
func_800B6748:
  MATCH
```
(The other context rows print the same as the installed group's run, e.g. object_create 28/28, which is informational.)

Pi, `python3 -m tools.conveyor.pipeline.blob_unit --tag w27f score credits_scroll func_80100B8C slot_state_setup
world_trigger_check --with cloud/work/frontier/w27b/slot_state_setup/slot_state_setup.c --with
cloud/work/frontier/w27b/world_trigger_check/world_trigger_check.c --neighbours`:
```
  EQUAL credits_scroll: 83 words (internal, g_credits_scroll_grp__gr3_c.c)
  EQUAL func_80100B8C: 102 words (kept, g_credits_scroll_grp__gr3_d.c)
  EQUAL slot_state_setup: 58 words (internal, c_slot_state_setup.c)
  EQUAL world_trigger_check: 54 words (kept, c_world_trigger_check.c)
  locked bodies that differ in this unit: 0
blob_unit score: 4/4 equal
```

## Integration
`PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w27f/credits_scroll_grp credits_scroll_grp`
- No `--revert-single`: neither slot_state_setup nor world_trigger_check is locked as a single.
- No unit_overrides change: slot_state_setup is already internal in the unit (w27b).
- After landing, codex_defaults_a16's context copy of world_trigger_check (`= 0x8000`) is stale but harmless (w27b note).
- Builder scratch deleted.
