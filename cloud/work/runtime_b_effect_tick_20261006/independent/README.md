# Independent runtime-B effect-tick audit

## Result

PASS for the complete, bounded semantic reconstruction of the three real bodies at B:80390F60, 80390D38 and 80390B10. NOT a strict object match, original translation-unit identification, or whole-image acceptance.

The unchanged research source is `effect_tick.c`, SHA-256 `b88b940c637c5831be3714b6dbe47f9bd3821bdb8e6501d775f46fe552274bd0`, in the owner's packet `runtime-b-effect-tick/cloud/work/runtime_b_effect_tick_20261006`. Historical context is read from base `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`, never the stale live recovery checkout. This audit made no source changes to that packet, no target compiler invocations, no protected changes, and no external writes.

The independent suite ran 2,498 paired native-versus-unchanged-host-C fixtures, 2,528 root invocations per implementation, 120 direct private-helper invocations per implementation, and 1,800 independent expected-value player-field checks. All passed. It exercised 517 of 523 native words; a conservative direct control-flow check establishes that the other six words are unreachable duplicate scheduled loads/stores. Every statically reachable native instruction was exercised.

The same 2,498 fixtures and 2,528 invocations were then replayed against the owner's existing single O3 artifact, without recompiling. Complete observable memory state, ordered boundary events, and the ordinary root's nonvolatile register preservation passed. Candidate execution used only its linked data, not the native lookup table. The reviewed packet interpreter and address adapter are reused, so this is an independent fixture/oracle and manual instruction audit, not a second MIPS emulator implementation.

## Identity and ABI

All three complete native intervals were reauthenticated directly from the historical raw-deflate B image in memory. The 988-byte F60, 552-byte D38 and 552-byte B10 identities are in `receipt.json`; the B alpha table was independently checked as exactly 16, 32, 64, 96, 112, 144, 176, 176. No native bytes, binary objects, or assembly dumps are included in this audit's deliverables.

The earlier loader/image audit in `viewport-contract-audit-20261006` establishes why these are image-B entries and why same-address image A is unsuitable. This audit does not extend its residency claim: image B and its initialized state must already be resident through the genuine game lifecycle.

F60 has no logical incoming argument. Its original 88-byte frame preserves s0, s1, f20/f21 and ra. It calls D38 then B10, which independently establish their control pointer and -2.0f sentinel and may clobber s0/s1/f20 while saving only ra. Their incoming argument/saved-register values are not logical inputs. The source includes their complete real bodies as static functions rather than pretending they are ordinary ABI external services or inventing helpers. Randomized incoming registers/FPRs and direct private-entry drills corroborate this boundary.

## Field and behavior findings

- Player stride is 952 bytes. Status is the 32-bit word at +908; countdown and phase are signed bytes at +928/+930; alpha is unsigned at +929; timer is binary32 at +936. All source offsets agree with native. The owner separately verified all 28 target O32 size/offset facts before the sole baseline.
- The live player count is signed halfword 8014A108. A nonpositive value skips players. The countdown decrement is independent of status bit 0 and happens only for a positive signed byte.
- Phase 3 indexes the eight-byte alpha table with the signed countdown, then decrements positive countdown. Its valid lookup domain is 0..7. Phase 1 clears countdown; with positive timer, alpha subtracts eight modulo 256 before the <=16 clamp and phase transition. Thus alpha 0..7 wraps upward rather than clamping. Other phases widen alpha+8 and saturate at 255. A nonpositive phase-1 timer changes phase to 3 without changing alpha; a nonpositive other-phase timer clears only status bit 0 and phase.
- Player timer positivity is checked before subtraction. A subtraction that crosses zero changes the timer now; the nonpositive-timer phase action occurs on a later invocation. All relevant additions/subtractions round to binary32, without fused operations. The audit includes signed zero, exact equality, adjacent representable values around -2 and 0.25, small half-ULP-scale deltas, and large values where subtraction rounds back to the original timer.
- The 20-byte control block at 80399AE0 has signed count +0, signed selected indices +1/+2/+3, timers +4/+8/+12, and an entry-table pointer at +16. Entries are eight target bytes: pointer, signed state, signed eligibility. The source's descriptive timer names correspond to these offsets correctly.
- For each control path, sentinel -2 with object flag bit 2 still set does nothing. When that flag clears, RNG supplies the delay, existing matching-state entries reset to state 1, and the selected entry becomes the path's state with eligibility 1. The fast path uses range 10 plus 5 and state 4; the other paths use range 60 plus 90 and states 3 and 2.
- Outside the sentinel, subtraction precedes the strict <0 test. Expiration obtains an RNG index, cycles with wrap until eligibility==1 and state differs from the path's own state, stores the selected index, sets state 1 / eligibility to the path kind, sets object flag bit 2 and animation, issues both external services, then stores -2.0f. A missing qualifying entry can make the native search infinite; the audit's invalid-search control fails closed at the interpreter step limit.
- Animations are 353/359/354 in actual D38/B10/F60 order. Resource halfwords are 80142A82/80142A8E/80142A84. The setter consumes the signed low half of the object's +12 scene word, matching native +14 on big-endian N64; the following helper consumes the full signed word at +12 with arguments 0 and 15.
- The saved selection offset survives the first service, but table and object pointers must be reloaded before the second service. The tests switch to reversed object pointers and change global selected indices at the setter boundary. RNG-boundary probes also mutate count, table, delta, resources and players. These are deliberately widened memory-effect tests, not claims that the actual services perform those particular mutations.
- No allocation, free-list/pool insertion/removal, indirect callback invocation, or callback registration occurs in these three bodies. The lifecycle here is entry-state/eligibility, object flags/animation and player fade state. Pointee allocation/ownership and the implementations of the external services remain outside this closure.

## Existing O3 artifact inspection

The owner ran the unchanged canonical group route with the required bare header `/* flags: -g0 -O3 -mips2 -G 0 -non_shared */` and recorded the mandatory backend `as1 -r4300_mul`. This reviewer ran no target compiler, no alternate flags, and no tuning.

ECOFF procedure/end/PDR evidence shows:

- D38: 548 bytes versus native 552; 24-byte ra-only frame.
- B10: an 8-byte compiler-created deleted-static stub. Its complete source logic is inlined into F60.
- F60: 1,560 bytes versus native 988; an 80-byte frame preserving s0/s1 and f20/f21.
- All procedures own 2,116 text bytes, with 12 additional zero-alignment bytes. These complete extents matter; a truncated scorer view is not an exact-body proof.

All 53 object relocations independently reproduce the GNU-linked words, and all 1,932 remaining allocated text/data bytes are unchanged by linking. The own alpha initializer is in `.data`; there is no requirement that this compiler put it in `.rodata`.

The canonical comparison remains a failure: 245/247 target words differ, 141 extra nonzero words in the scorer view, an unresolved private `.text+0x8` reference, and two unverified own-data references. Independent semantic replay and relocation correctness do not repair those acceptance failures. The three-body source supplies a genuine private-helper visibility hypothesis; the differing inlining decision is evidence that it does not establish the original translation unit. There is no whole-image acceptance claim or promotion recommendation.

## Bounds and reproduction

Tests assume finite normal-or-zero binary32 values with default rounding, initialized aligned nonaliasing storage, valid player/entry/object selections, the bounded fixture counts, legal external-service indices, and at least one qualifying entry when searching. They do not prove NaN/subnormal/overflow behavior, invalid conversions, corrupt pointers, asynchronous mutations, unbounded gameplay, or every possible image transition.

Set TMPDIR to a writable workspace directory. `audit.py --packet PACKET --reference-root REPO --output receipt.json` compiles only a host adapter that includes unchanged source, then runs the semantic suite. `object_audit.py --packet PACKET --reference-root REPO --score-root SCORE_WORKSPACE --work-dir EXISTING_BASELINE --output object-receipt.json` never compiles and replays the existing object. Compiler-produced objects stay local and are not audit deliverables. The JSON receipts pin only reviewed packet inputs/native target words and the existing artifact evidence, not live manifests, production source or acceptance locks.
