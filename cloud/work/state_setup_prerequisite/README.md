# Setup-state reconstruction prerequisite

Base: `53d0fba0c5f44e91fe921f722fee7ff63a2dd0fe`, refreshed against master
2026-10-03. Status: **partial behavioral source, not a matching candidate**.
Zero new accepted bytes. No compile-score claim, ROM claim, or IPA closure.

## Concrete result

`fast_prepare.inc.c` reconstructs the genuine `800FB378`–`800FB5BC` region
of `setup_state_main` (`800FB2C8`, historical alias `display_list_flush`).
It is the first-entry path selected by flags bit `0x02000000`, through the
call to `init_state_continue` (`800FAF6C`). It is an include fragment,
not an invented helper or a replacement for the full function. The original
entry guards, earlier calls and later completion are intentionally outside
this region. Never use the host harness as a candidate translation unit.

The region resets three counters, resets the selected players' model/vehicle
fields, builds two ordinal lists, handles remaining slots, publishes counts
and sentinel fields, and calls the unsaved-s-register leaf. Selected physical
slots come from byte zero of 76-byte rows; list values are loop ordinals.
Remaining slots are indexed directly. Those loops differ: the second does
not clear model `+0x380` or set vehicle `+0xA`, `+0x7CA`, `+0x7CC`.

The call to `music_tempo_set` is a real resource-setup service, despite its
historical name. B99 already recovered the complete body and its consumed
signed-half player, unsigned-byte mode and integer apply parameters. That
body is reused as evidence, not rediscovered or claimed as new source.
The layout header defines only native-evidenced fields and unknown byte ranges;
no artificial local arrays, register pressure or padding instructions exist.

## Source audit and ABI boundary

The nominal tracked `setup_state_main/target.s` stops after 500 words, at
`800FBA94`; it alone is incomplete. The overlapping
`pause_toggle_handler/target.s` supplies the rest through `800FBBF8`.
297 shared words agree. Their union supplies all 589 words, zero gaps.
Independent audit also checked the redundant `hud_render_main/target.s`.
SHA256 of the reconstructed big-endian word stream:
`1d92bbdf5225a65d93ebbcb0f421526cfad83630387c27ecf223ddca197d479a`.
No raw native dump or ROM is included in this packet.

The full body consumes two incoming values: a1 is a reset gate clearing
byte `801146F4` and returning zero; a0 gates a later `800F0100` call.
These observations establish consumed input behavior, not the source types
or the entire external ABI. Its 256-byte frame saves s0–s7/fp/ra. The real
FAF6C call saves a live zero loop index at sp+0xFC, then reloads it afterward;
this is useful evidence for the future genuine compiler-context experiment.
There is no justification to encode that spill manually in C.

The general path has unresolved services `8038CA24` and `803908D0`.
The first receives a sign-extended loop index when mode is six; the second
has no fresh explicit arguments after memset calls. No fresh arguments is
not proof of void(void). Full source cannot be called ABI-complete until
these contracts and remaining callees are established.

Known archive assets:
- B99: `../near_miss_B99/music_tempo_set_native.c`, complete 800B200C source;
  frozen nonmatch, not acceptance.
- `../ipa-groups/codex_vsync_a145/group.c`: complete FB234 logical source,
  one integer flag, updates 8010FFC0 and invokes the genuine speed helper.
- `../../../src/blob/func_800FAEE4.c`: accepted zero-argument source,
  initializes two six-float arrays to 1.0.
- `../near_miss_B93/func_800E543C_branches.c`: complete source, two signed-half
  parameters (historical third pointer is false), frozen nonmatch. Use the
  corrected branches/address version: the initial `_native.c` omitted a drift
  clear for action zero and must not be reused as the semantic baseline.
- F0100 has no demonstrated faithful full C here. Do not import the historical
  `game.c` pointer-argument sketch solely for its convenient name.
- C9BE0 needs full source/side-effect audit; it writes configuration later
  consumed by FAF6C, including 80142724, 80152734 and 801543C8.

No existing complete FB2C8 C was found by address/name archive search. The
historical generic graphics submission sketch is not this state setup body.
This audit does not establish source absence under all unrecognized names.

## Validation

Host strict C89 compilation and execution passed with
`cc -std=c89 -pedantic -Wall -Wextra -Werror` with the packet include directory
and `tests/conveyor/fixtures/state_setup_prerequisite.c`. The harness lives
under tests so its callbacks cannot be mistaken for recovered game source.
The CI-discovered wrapper is `tests/conveyor/test_state_setup_prerequisite.py`.
The pytest wrapper passed (1 test). Embedded cases exercise mapped versus
ordinal indices, unsigned-byte wrapping, zero and negative signed counts,
untouched model/vehicle fields in the second loop, callback mutation of list
counts and both loop bounds, and final publication before FAF6C.
Host ASan/UBSan run passed with leak detection disabled: the initial default
LeakSanitizer run was blocked by the container's ptrace environment. This is
not full native differential execution or an IDO matching experiment.

## Work package for the shared project plan

1. Reserve this exact interval and audit the fragment independently. Verify
   each memory width/offset against the authenticated text, especially the
   selected-slot versus ordinal distinction and post-call counter reloads.
   Done when the source review and host cases agree; no matching credit.
2. Recover the complete genuine C9BE0 prelude from existing native evidence,
   and establish F0100's incoming register and memory contract. Reuse B99,
   B93, FB234 and accepted FAEE4; do not repeat their frozen compile sweeps.
3. Recover FB2C8's entry/prelude and fast-path completion around this fragment.
   Test reset, initialized flag, both configuration bits, list mutations and
   completion order. Preserve symbolic constant 801247F0 until its value has
   authenticated evidence. Do not guess a float from a name.
4. Resolve `8038CA24` / `803908D0` with a maintainer-authenticated runtime
   module/address map plus typed interface or implementation and exact US
   revision. Ask for a text contract, not raw ROM upload. In parallel reconstruct
   the general path's known regions; keep the unresolved calls explicit.
5. Recover/test the entire state machine. Cover first versus resumed entry,
   asynchronous false return, mode-six/mode-four paths, signed count handling,
   queue interactions, all final resets and a0-controlled completion. Use
   proven callable contracts; no guessed standard O32 signatures.
6. Only then reserve a full natural context experiment for FAF6C. List genuine
   TU members, externally kept roots, visibility and source hashes. Include
   actual countdown/game_loop call contracts, with source order derived from
   evidence. Explain the native s-register clobbers and sp+0xFC live value.
7. Compile one bounded evidence-derived baseline with the pinned IDO recipe,
   then diagnose predicted versus observed allocation. If prediction fails,
   stop and record causal evidence; do not start parameter/pressure sweeps.
8. Any eventual match needs resolved full-body equality and accepted-neighbor,
   image, compression, full-ROM and reporting gates. Partial host behavior,
   masked scores, merged research and source line counts give no byte credit.

Suggested ownership: one state/source reconstructor, independent ABI reviewer,
maintainer for external runtime contracts, and integration owner for private
acceptance. Steps 1–3 are locally actionable source work. Step 4's known
regions can run concurrently, but unknown service closure needs external
proof. Step 6 is blocked by complete body/context readiness, not lack of a ROM.

Stop conditions: missing service evidence; unproved incoming register; conflicting
native revisions; a source archive that already supplies a stronger candidate;
or no new allocation explanation after the single baseline. Report the precise
boundary, preserve rejected attempts, and switch to another evidenced lane.
The paused 800D1248 target and its helper-context investigation are excluded.
