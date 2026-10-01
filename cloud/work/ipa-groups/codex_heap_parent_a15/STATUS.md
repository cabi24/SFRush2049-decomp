# A15: authentic heap-loader parent — NONMATCH

Frozen 2026-10-01. Empty claims; no new coverage. Extends frozen A14 into a new directory; A14 and all shared source/locks/layout remain untouched. Twelve authentic bodies,1298 retail words. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`. Six directed context/ABI controls, no reflow sweep, pressure padding, fake wrapper, duplicated unused formal or new guard.

PrevMaxPath stays **5/24 strict differences, exact24 words, no extras/unresolved/unverified/errors** for every control. The five differences remain the clear-end/start/flag/ROM saved-register permutation documented in A14. Genuine parent preservation removes display_enable's own saved-register prologue but does not change the target's coloring. No accepted claim follows from this result. Empty --claims reports work in progress with exit0. All eleven context functions also remain NONMATCH; see scores.json. Final context has no relocation errors. The control that kept the real empty helper's ABI produced an unpaired sound HI16; that failed-verification control is recorded and excluded from delivery settings.

## Actual hidden bindings audited before compiling

Parent playgame_state_change800CA3B4 is636 words, with five genuine display_enable call sites. It saves s0–s4/f20/f22 and supplies the context that permits display's unsaved registers. Full body was decompiled from its own retail instructions, preserving its state-bit branches, real player/slot loops, object creation, queue operations and configuration calls. No shortened branch skeleton replaces it.

Three speed_set sites consume four real inputs: signed settings byte D_80146115 divided by10.0f with float D_80123FB4 and flags0/1; the same byte with FB8 and flags0/1; signed byte D_80146108+12 with FBC and flags1/0. These are exactly the f20/f22/s1/s2 values set at800CA55C..588,800CA63C..668 and800CA748..770. The source replaces decompiler saved_reg placeholders with genuine four-formal logical inputs and all three calls with these actual values. Historical formal order is not asserted.

Speed helper acquires real queueD_80142728, gets a real message from func_80091B00, sets type10, clamps first float to0..1 and second below at0, stores actual byte flags at12/13, releases that queue and sends the message toD_801427A8. Clamp association matches retail, including the direct zero-store lower branch. Allocator return is correctly pointer-typed. No fabricated allocator body is present.

Slot setup sites use selectors5/6/7 in incoming s2, proven by the three call delay slots800CABF4/800CAC28/800CAC5C. Source uses one consumed logical selection formal. The helper saves the old signed-byte D_80149DA0, stores the new selection, looks up/loads the two actual resource kinds selection+38 and selection+22, calls real display_list_alloc for newly loaded slots, then passes the genuine old!=selection boolean to sound_update_channel. Selector0 separately sets the real object-byte9 flag. Old selection is returned. Parent's three actual queue-protected returned-slot stores are preserved; no artificial additional arguments appear.

Sound and empty-callee bodies are copied from accepted src/blob/groups/codex_sound_channel_extra/group.c. Their runtime fields/operations are unchanged. This module is a different compilation context and does NOT preserve their accepted instruction/clobber contract: final sound120/122, emitted117; empty3/4, emitted2. Neither is claimed or eligible for integration.

## Real resource context and limits

The authentic five-input audio_frame_sync body from the historical resource-loader group replaces a fresh decompiler output containing bogus unset t0/t1/a2/v0 placeholders. It consumes resource kind, skip flag, async flag, bank flag and caller buffer. Exact20-byte resource layout has type6, flag5, allocation pointer12 and slot link16. Its actual search, allocation, buffer registration, slot finalization and sync/async paths are retained. Source search leaf is extracted from accepted resource_slot_clear/audio.c, with only field spellings ptr/id/sub adapted to equivalent p0C/type/f5 offsets12/6/5; its byte contract is unclaimed and NONMATCH29/65 here. No audio_user stand-in from that accepted module was copied.

All six actual parent/slot calls to audio_frame_sync use skip0. On the full-slot failure path its historical source returns the prior search result; that carrier is initialized for these genuine callers. An arbitrary skip-nonzero/full-pool invocation has historical uninitialized-return ambiguity and is not represented as independently certified behavior. No default return was invented to hide it. Other loader helpers remain actual named external calls, rather than stubs or zero M2C_ERROR expressions.

The copied empty helper has its existing unreachable switch/code-free condition; no new guard was added. func_80097694's different register choices and sound's context specialization mean full parent/callee ABI fidelity is not proved. This is an honest source reconstruction and measured hypothesis, not a byte-valid parent closure. The target's complete matching instruction forms but unchanged five input-register differences exhaust the proposed parent-context lever for this packet.

## Measurements

Controls cover the base actual-parent module; retaining sound alone; retaining sound+empty ABI; retaining sound+speed+slot ABI; retaining sound+display ABI; and retaining sound+free-helper ABI. The sound-only setting keeps the real sound body out of line and prevents its wholesale inclusion in slot setup; it is delivered. Retaining display's ABI returns its A14 frame/preservation shape and remains inert for PrevMaxPath. None of these settings is a synthetic runtime call or source mutation. Frozen tables contain only counts/error text, not target bytes. Objects, ROM streams and assembly remain ignored/private.

Reproduce:

```sh
rsync -a cloud/work/ipa-groups/codex_heap_parent_a15/ Rocky:agents/A/wt/cloud/work/ipa-groups/codex_heap_parent_a15/
ssh Rocky 'cd ~/agents/A/wt && python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_heap_parent_a15 --claims'
```

Frozen group.c SHA256: `5352a1756cdee4a1f28b2536a03a52ba7456c9fb21531cf69263a855bc8602f3`.
