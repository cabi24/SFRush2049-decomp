# Overlay residency: bounded static trace

## Result

The actual small-image implementation identity is established separately, but there is not yet a complete runtime-residency proof for every execution of C974 or collision callers. Native code establishes a specific load-before-dispatch route, not a universal guarantee. No emulator, instrumented gameplay or cartridge execution was performed. No compiler, build, promotion, source edit, or coverage change occurred.

Baseline: master f82c5204; source rules read in CLAUDE.md, PROJECT_PLAN.md and dot_response. Authenticated inflated game/small/large images were inspected locally; no private images or disassembly accompany this report. The owner-paused function and helper-context investigation were not inspected. Historical function labels below are locator aliases, not semantic endorsements.

## Loader lifetime facts

- Native InitMaxPath 800A1244 loads ROM pointer D_8002B024 (B6FEC4) to 8038A400, clears 80394F70..8039B440, and sets byte 8011ED04 through PrevMaxPath 800A11E4 only when that byte was zero. The flag is set after inflate and clear return.
- Large loader 800A133C uses D_8002B020 (B5C534), same destination, clear 803B9A50..803BAE20, flag 8011ED00. Both reserve through 803BB380. Overlap is real and complete over the small code range.
- Neither direct loader independently checks the other image's flag or display mode. Correct exclusion is therefore a caller-order invariant, not a property of PrevMaxPath.
- display_enable 800C8FA4: enable with mode 801174C0=0 releases small allocation if ED04 is set, clears ED04, loads large if ED00=0, sets mode=1. Disable with mode!=0 releases large if ED00 is set, clears ED00, sets mode=0. Disable does not load small. A no-op enable/disable does not repair stale flag/ownership combinations.
- Release synchronizes on queue 80152770, frees through 80095FD8, then jams the queue back. These are allocation/synchronization operations; they do not themselves prove callback quiescence or cache coherence.
- Separate small release 800A0F6C (label MP_TargetSpeed) clears ED04 after the same queue/free sequence; separate large release 800A12CC clears ED00. No direct JAL to either was found in the three inspected images. This is not proof they are unreachable through indirect calls or boot code.
- Freed bytes may remain physically readable until reuse; this is not ownership or residency. Large load overwrites small service addresses. Small BSS clear also overwrites part of the large image. Calling whichever happens to remain in RAM is not an accepted invariant.

## State transitions and reload conditions

playgame_state_change at 800CA3B4 compares current state 801174B4 against requested state 801174B8. Equal states return without reload. On change, it calls 800CA300 and then copies requested to current before state-specific work.

The ordered state-bit dispatch has enabling paths at 800CA48C, CA58C and CA66C (bits 1, 2 and 4 respectively, subject to earlier dispatch conditions). All invoke display_enable(1).

The bit-8 branch calls display_enable(0) at 800CA81C, then calls InitMaxPath at 800CA834 only if byte 80156994 is nonzero. Thus bit 8 alone does not establish small-image residency.

The bit-0x40000 branch (bit test via shift at 800CAA58, reached only after preceding state cases fail) calls display_enable(0) at 800CAA8C. At 800CAB48..CAB78 it calls InitMaxPath exactly when mode word 8014A110 is 4, 5, or 6, OR byte 80156994 is nonzero. Mode 6 is a sufficient condition for this branch's reload, not for arbitrary program points.

80156994 is set to 1 at 800E7DAC after the native memory-end comparison against 80400001, following a read of boot word 80000318 and cached-address conversion. This supports an expanded-memory flag interpretation, but the exact all-writer/initialization invariant remains unproved here. It should not be relabeled as a gameplay mode from its load guard alone.

## C974 is an indirect object callback

The pointer to 8010C974 resides at game-image 80118BBC. This is record index 120, offset 0x0C, in the 48-byte table at 80117530. Offset 0x08 of the same record contains 8010C7F4, whose body calls 8038D3A4 at 8010C8D0/C8F8/C920.

Object setup at 800AB7D8 selects the table record using signed object+0x10 times 48. For record flag bit 0x2, 800ABA80 allocates a node; 800ABA94..ABAB4 copies table+0x0C to node+0x14, object pointer to node+0x0C and links node into list head 801391F0. Thus this is a concrete route from the table pointer to the runtime dispatch slot, not merely an address-looking word.

Updater 800B0868 walks that list. At 800B0898..08A8 it reads node+0x14 and invokes it with a0=node, a1=1. Neither this dispatch nor C974's own service call tests ED04, ED00 or display mode.

C974's own entry has two important safe skips: signed-low16(a1)==0 takes immediate cleanup via 8009079C and returns; nonzero word 801170FC returns before the main body. The service call 8010D2C4 is further conditional on object state and float threshold (state+0x68 > 120.0 at 8010D290..D2A4); those object conditions do not encode image ownership.

The direct callers of 800B0868 found in the game image are 800F7420, 800FD64C and 800FD6B4. Wrapper 800F73FC calls it when its incoming a1!=0; no direct JAL to that wrapper was found in these images, leaving indirect exposure unclosed.

## Main-frame ordering: useful but insufficient

800FD464 calls playgame_state_change at 800FD5E4 before later updater calls. The 800FD64C route requires state bits 0x200000 or 0x400000 and 801170FC!=0 after 800DB81C, then 800F733C. If the guard remains unchanged, C974's matching entry guard skips its service. Callee effects and asynchronous writes must be checked before promoting that observation to an invariant.

The 800FD6B4 route checks current==requested state at 800FD69C, after optional 800FD238 and 800FBF88, and calls 800FBC30 then 800F733C before updater. Equality ensures no pending state change at that comparison; it does not imply small residency. In particular the equal-state fast return in playgame_state_change relies on the image invariant from earlier transitions.

No complete summary has been proved for intervening callees, callbacks, mode writers, object creation/destruction, allocator aliases, or scheduling. To close C974 residency statically, prove that every live record-120 update occurs only after an applicable small-image reload and before any release/overwrite, or that its entry guards block all other states. Current evidence supplies the exact nodes at which to prove that property.

## Other direct collision calls

Two game calls at 800CE69C and 800CE778 are in native body 800CE358 (historical label menu_options_screen). Both are dominated within that body by mode==6 at 800CE544..CE550, plus target/team/object predicates. That establishes a local mode gate, not a loader gate. Mode 6 must still be tied to completed state transition and no subsequent overlay invalidation.

Small-image internal call sites exist for both service addresses. Once the small image is correctly owned and remains stable, their address binding is straightforward; no internal call site alone establishes entry residency.

## Minimal model

`residency_model.py` tests the abstract loader transitions and reload predicate, including disable-without-reload and nonexclusive direct Init ordering. It is explicitly not a MIPS interpreter, actual reachable-state exploration, or proof that malformed orderings occur in the game. Its assertions establish only consequences of the documented transition assumptions. Run: `python3 cloud/work/small_overlay_service_pair/residency_model.py`.

## Narrow admissible invariant

Conditional static invariant: starting from a coherent exclusive loader state (flags accurately describe allocation/image ownership, and display mode agrees with the active large-image state), the bit-0x40000 reload path with mode in {4,5,6} or 80156994!=0 returns with the small image loaded, assuming allocator/inflate/cache operations succeed as intended. The invariant is invalidated by small release, overlapping load/write, allocator reuse, or unclosed concurrent activity. It cannot yet be lifted to all C974 calls.

Concrete remaining proof obligation: record-120 list lifetime and dispatch versus state/mode transitions, including summaries for 800FBC30, 800F733C, 800FBF88 and 800FD238; complete direct/indirect loader and flag-writer census across boot and other assets; serialization/cache behavior. Runtime instrumentation at the two loaders, releases and 8010D2C4/8038D3A4 could alternatively supply trace evidence but would not be a static universal proof.
