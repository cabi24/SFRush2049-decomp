# D04 lane B preflight: rejected stale assignments

Base: d701b59592463d1e33bcee7011ed2a1e6c07b066. Owner B, branch dot/round10-b-race-setup. No matching or accepted coverage claims. No edited context members.

## race_setup_1 @ 800BD2C8, 1884 bytes

Native purpose is palette/display-list and object-frame animation, not race setup. Two 20-byte sentinel record loops, halfword palette rotations/swaps, color animation, real five-argument coordinate update, and object frame selection establish this. PR9 already contains near_miss_B104 complete source; use race_setup_1_case_mapping.c, SHA256 c66b427ff7476c5537273b6d6c135bde7ca39dc3104d50ed6e1f96a14ee3668f. The earlier native.c is explicitly rejected because cases4/6 were swapped. Canonical protected instructions expose an 11-entry switch table at80123E2C; its contents were privately audited in prior work, not newly recovered here.

Fresh IDO O3 replay reproduces381/471 differences, no extra words/errors/unresolved symbols, with2 unverified local switch-table relocations at+c4/+cc. Prior genuine codex_texture_a127 helper/caller context also records381/471; the helper has since been accepted, granting no repeat credit. Scalar/timing/O2 controls already exhausted. No new semantic/compiler explanation justifies another tuning sequence.

## assign_drones @ 800F4FEC, 1236 bytes

Native purpose is ranking/statistics update, not donor drone AI. Existing complete B62 source updates resource/global96-byte records, five rank times with handles/owners, counters/samples/distance and record refresh. Three existing controls native/O2/phases already archived. Fresh O3 phases source SHA2562f5d4d86c5d0a63b1168af58a2aa6b6cca0b69ce5636b82fb92d10095068ae8d reproduces287/309 differences without extra words/unresolved/unverified references/errors. Native/O2 archived288/309. No fresh causal lead.

## Fallback rejected

Scout-suggested func_800FC9F8 @800FC9F8,1016 bytes is also already complete in PR9 tiny_A69. A path/status search caught this despite a first full-definition grep miss. Do not reassign as fresh.

## Validation and recommendation

The accepted sound_handles_clear single and three-member resource_slot_clear group all freshly MATCH as setup controls. Source/score replay JSON is d04_replay.json. Read current locked intervals, PR inventory1–41 and PR9 fresh head before selection; these two targets are unaccepted but not fresh.

Recommendation: correct D04 native-purpose labels and mark these two frozen historical packets. Obtain a new scout-approved reconstruction target. No raw objects/instructions/ROM data, protected edits, source permutations, or new PR warranted by unchanged compiler results alone.
