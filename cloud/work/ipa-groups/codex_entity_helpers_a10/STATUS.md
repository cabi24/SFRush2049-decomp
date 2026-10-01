# A10: authentic entity/message helpers — three strict matches

Frozen 2026-10-01. New claims: results_screen_update32 words, leaderboard_update32, camera_clip_planes55 — **119 words /476 bytes**. All three current root locks were checked and were absent at selection. No synthetic caller/callee, stand-in group, runtime side effect, integration write or locked-source change.

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`. Canonical `python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_entity_helpers_a10 --claims` exits0 with all three MATCH, exact emitted lengths, no extra/unresolved/unverified instructions. scores.json records final strict outcomes.

## Real module and source provenance

The source starts from the accepted entity_lookup group's actual bodies, removing its historical __standin_a wrapper completely. Eight actual functions total265 retail words: three claims plus lookup21, genuine empty BF01C2, message allocator42, time-display41 and receiver40. Real allocator is retained in keep because retail scheduler_recv calls it out of line; no fabricated second call site was added to inhibit inlining. Its real C body is present and emits42 words. Source extern signatures now declare actual three-argument queue protocols and one-argument entity_state_check, rather than leaving those genuine interfaces unprototyped.

Three contexts separately remain MATCH: func_80091BA8, func_800BF01C, results_time_display. Allocator func_80091B00 remains18/42 differing words, and scheduler_recv remains27/40 differing words: these are actual source bodies, not claims or new coverage. In the initial no-wrapper module umerge inlined the allocator and emitted a two-word unused shell; the delivered keep list restores the genuine out-of-line call and both retail context extents. No compiler-generated empty allocator shell is delivered as context evidence.

## Empty-callee formal mechanism

Retail BF01C is genuinely `jr ra; nop`: no reads/writes/clobbers. Actual callers pass their CamSlot pointer in a0, and camera needs a1/a3 alive across that call. The inherited unreachable `if(0) switch` prevents umerge eliminating the call. Adding **`if(c) {}`**, where c is that actual pointer formal, keeps umerge's argument use visible before the optimizer removes the empty conditional. It performs no dereference, branch, side effect or register write in final code; BF01C remains exactly the two retail words.

This code-free actual-formal guard is a source quirk, documented rather than hidden. Its purpose is retaining the real a0 argument protocol. Without it, umerge passes the unused formal via a caller stack store and both small targets score13/32. With it, results_screen_update and leaderboard_update immediately become exact32/32. The same mechanism was proven earlier for the real empty sound callee. Keeping BF01C as an ABI root instead worsens the callers and is not the delivered convention.

## Camera clamp and semantic audit

Camera's true formals are h, float-vector pointer, unused integer home32, blend float home36, extra float home40. Entity->cam is at+0x40; CamSlot XYZ are+12/+16/+20, blend+28, extra+36. Real lookup body uses 0x44 entity stride/id+12/state+16/link+60 and the actual handle mask. Msg stride24 has signed16 ID+0, type+2, used+3, entity pointer+4; actual allocator scans128 slots, marks the selected slot, sets ID=-1 and returns its address. Receiver writes type6/entity and increments the real byte counter+0x1A. None of those actual field operations were changed.

Pointer-formal repair reduces camera28→23 differing words. A genuine scalar clamp result reduces23→4. Matching retail's direct lower-branch zero store, while using the result carrier only for the upper clamp branch, closes the remaining four words: zero lives in f0 and the negative branch stores it directly, rather than moving f12 into a join result. Runtime semantics are unchanged: negative blend→positive zero, greater-than-one→one, otherwise the original float (including NaN and signed zero); all XYZ and extra writes remain in the actual order. No physical-line sweep or register-pressure declarations were used.

The claims call only real queue APIs, lookup, BF01C, scheduler_recv/time-display. BF01C and lookup preserve camera's live a1/a3 protocol; strict byte equality verifies the complete claimed caller sequences. Receiver/allocator source semantics are retained, but their allocation/scheduling differ and are honestly unclaimed. Existing accepted source/group artifacts and locks are untouched.

## Bounded verification

Fourteen directed controls are retained in controls1/2/3.json: six real keep/formal controls, five actual signature/clamp controls and three lower-store/literal controls. All compile. Final variant is the natural explicit lower zero store with float literals; integer-zero/cast spelling controls also match but were unnecessary. Canonical final claims were rerun on exactly the delivered source and keep list.

Reproduce on Rocky after copying only this new group directory:

```sh
rsync -a cloud/work/ipa-groups/codex_entity_helpers_a10/ Rocky:agents/A/wt/cloud/work/ipa-groups/codex_entity_helpers_a10/
ssh Rocky 'cd ~/agents/A/wt && python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_entity_helpers_a10 --claims'
```

Frozen group.c SHA256: `6db238b5785bde5b04904c9295d314aef3e9e4c916f8f48079c1d5448e00ac86`.
