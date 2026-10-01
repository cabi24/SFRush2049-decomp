# Codex real-caller closure, 2026-10-01

Strict `score.py group cloud/work/ipa-groups/codex_func_800B7438 --claims` on Rocky A: exit 0, MATCH for func_800B73E4 (21 words), func_800B7438 (24 words), and informational context input_init_flag_get. The exact delivered group.c/group.json were copied to Rocky before scoring.

The original group's synthetic second caller was replaced with the real input_init_flag_get body from its previously matched source. Both callers remain exported (`keep`), func_800B73E4 stays internal. Claims contain only the two unlocked members; input_init_flag_get is already locked and is context only. No stand-ins remain, and no members call stand-ins.

Source quirk: D_801551E8 is defined in this TU, as in the original group, to allow as1 to share address materialization across stores. This definition must be handled by the group integration path; do not introduce a second linked definition.

Coordinator still must independently rescore, run the image gate, check locked-context consistency, and verify the full ROM. No splice/layout/lock changes were performed by this worker.
