# Pause state creator: eight-register-word research lead

Base `58425072b49e95f0fbe5c4c239988fb4885f7c5c`, 2026-10-17.
Research only. No accepted-image, integration, coverage, or ROM claim.

## Result and complete closure

`func_800B4FB0` at 0x800B4FB0 is the N64 pause state setter with its
state-creator helper inlined. The donor establishes state store, cleanup,
then creation. Its 1,472-byte native extent and 88-byte frame are reproduced.
The strict comparison differs at eight of 368 words. All eight differences
are only the s1/s2 allocation swap between the first-loop maximum and the
strength-reduced loop bound. There are no extra words, unresolved symbols,
unverified references, or owned-data errors for this target.

Every known native member remains explicit; context and claims are empty:

- func_800B4FB0: 8/368
- slot_state_setup: 2/58, plus five extra nonzero words
- object_create: 28/28, internalized/deleted body
- func_800B669C: first five words equal, one extra nonzero word; rejected
- voice_stop: first 138 words equal, one extra nonzero word; rejected

The last four results are inherited context failures, not accepted matches.
In particular, this stronger creator reconstruction does not replace the
separate exact-title closure published in draft #388. It reports its own
changed complete receipt. A further control added the real pause dispatcher
RaceStateMachine_Update and all its native audio callees; the creator stayed
8/368. That large dispatcher remains structurally partial and is not included
in this narrow packet.

## Source and layout evidence

The supplied MAT3 `pausescr.c` records original DWARF locals
`int max, width, i, x, y` for updatePauseStateCreate. setPauseState has only
its enum state parameter and calls cleanup before creation. That source is
recorded Matching/100%, but was read rather than freshly built here.
Its SHA-256 is
`69cefdd0ced15c6bdd8fdcf9880a8387f50c2a08dacaff906b0f5429f8d1296f`.

The adopted reconstruction restores the real 24-byte PlayerRect array
`[4][4]`, four-byte TitlePos, and 40-byte TextData. N64 instructions independently
confirm every referenced offset: rectangle centers +16/+20, title signed
shorts +0/+2, and text pointers +4/+12/+16. Unused fields in these records are
actual layout fields, not local frame fillers. All five creator locals are
used. The N64 eight-row menu, table index 56, and single-text indices
226/171/170/216 are retained instead of transplanting later-platform counts.

The authentic HAL_GetTextWidth contract is unsigned-int return with void
text pointer and signed-short limit. Width remains the original signed int
local. Both the creator and title translation units use that coherent
unsigned-return declaration; correcting the legacy title declaration is byte-inert.
The second width comparison uses the donor's unsigned comparison and
repeated call. Original font old-value and nested N64 semaphore boundaries
remain, with the same physical-line scheduling form already documented in
#388. No pragma, forced register, padding, extra local, or fake helper was added.

Replacing opaque byte-pointer field expressions with these real records
improved the previous donor-form creator from 348/368 to 8/368 and restored
the native frame and extent. Bounded source controls for loop spelling,
ternary spelling, state type, genuine font/cleanup closure, and the real
parent did not close the remaining allocation gap. Directly adopting the
later donor's pre-divided widths is worse and was not selected.

## Replay and negative controls

Run `python3 cloud/work/frontier/mat3_ui_pause_creator_20261017/verify.py`.
The replay pins source inventory, complete target digests, and all five member
receipts. It verifies the exact residual positions and register fields, not
merely a mismatch count. It works unchanged with `python3 -O`.

Four source mutations are rejected: wrong menu count, wrong text table,
wrong rectangle center, and wrong title font. The menu-count mutation still
has eight differing words, but adds a constant error inside one of those
words. The explicit register-content check rejects it. The other three
mutations have nine differing words and also fail that check.

Three unittest guard tests pass. Normal and optimized replay outputs are
byte-identical. Complete stock and traced compiler objects are byte-identical;
only stock results establish the receipt. Trace diagnostics report maximum
web priority 10.6 versus bound priority 9, identifying the remaining allocation
question without changing compiler behavior. Binary diagnostics are not part
of this source-only packet. No broad suite, canonical installation, image
splice, full-ROM gate, CI watcher, or new accepted byte count is claimed.
