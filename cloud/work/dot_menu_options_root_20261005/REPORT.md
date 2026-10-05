# Menu options root: full control flow, explicit storage blocker

Status: **BLOCKED_ON_CAPACITY**, semantic research only; zero new matching or
ROM-coverage credit.

Base: `7487788a0ed4aae747aeb9bb0309e45fa78e1d9b` (the separately reviewed menu-row
packet), on master baseline `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`.
Owner/branch: dot menu-options reconstruction, `dot/menu-options-root-20261005`.
Scope: `func_8010AEAC`, `[0x8010AEAC, 0x8010B520)`, 1,652 bytes / 413 words.

## Result

All executable regions of the real menu-options callback are reconstructed in
`root.c`. Its actual callback input, header selection, all fourteen switch
arms, filtering/scroll limits, line spacing, queue operations, cleanup, and
both return paths are represented. This is **PARTIAL-SOURCE with complete
control flow**, because the local header buffer's original declaration is
still unresolved. It is not a complete native-ready translation unit.

Native compilation is deliberately gated. No root object, masked score,
relocated-byte proof, ELF-extent proof, literal-ownership proof, or matching
claim is made. No production source, context, symbols, targets, flags, locks,
scorer, builder, image, or ROM is changed. New verified matching bytes: **0**.

The existing A8D0/A7A4 packet remains unchanged. Its verified 300-byte helper
belongs to that earlier packet and receives no duplicate credit here. This
packet does not claim that this finite family reproduces the original TU.

## Native source contracts

- The root is a genuine callback, not a caller substitute. Static records at
  `0x80116D4C` and `0x80116D70` contain its address at +28. `replay_save_prompt`
  passes those records to `sound_control`; the latter either stores the
  callback in a new state's +8 field or invokes a callback with that state
  pointer in a0 and consumes its integer result. This agrees with the
  `SoundClearRecord` / `SoundState` declarations in `include/game_types.h`.
  `s32 func_8010AEAC(void *state)` is therefore grounded; this particular body
  does not use `state` and returns 1 on either path. The native a0 home store
  is evidence for that real formal, not a reason to add pressure parameters.
- Mode 1 calls the complete existing `func_8010A8D0` and returns immediately.
  Other modes begin a 2-D menu path. Resource sets 13 and 11 are selected
  under the real `osRecvMesg` / `osJamMesg` queue pair.
- The controller header is used only when `(D_801174B4 & 0x007C0000)` and the
  signed byte `D_80116DA8` are nonzero. The integrity-checked format is
  `CONTROLLER %d %s`, including both spaces. The integer is `D_8015698C + 1`;
  the suffix comes from `D_8017A4E0.labels[233]`. Otherwise label 50 is drawn.
  Header x is 160 minus the unsigned measured width shifted right once,
  converted to a signed halfword; y is 10.
- Initial row y uses the header resource's line height: twice that height
  plus 10. Later increments use the selected row resource's line height.
- Fourteen 32-bit visibility entries begin at `D_80116D14`. A row is rendered
  only if its entry is nonzero, its index is at least signed-halfword
  `D_80149DA2`, and the signed-halfword drawn count is less than
  `D_80149B84`. These values are read again after calls; host tests explicitly
  exercise a helper changing the next row and both scroll limits.
- Every rendered row calls A7A4 at x = 160. Its label/value choices are the
  same fourteen real choices in A8D0. `verify.py` independently checks the
  protected switch destinations, label-load offsets, and signed selector
  offsets against the new source model without publishing extracted words.
- Cleanup resets alpha to -1, restores both actual packed colors through the
  genuine setter, resets the render helper to -1, and returns 1.

The misleading historical `grip_calculate` at this address in
`src/game/game.c` is not a donor. No arcade reference checkout or original
N64 source declaration is available in this workspace.

## Header storage is an actual blocker

The native destination starts at sp+176 in a 240-byte frame. A later short
lives at sp+218. Neither the 42-byte gap nor the 64 bytes to the end of the
frame proves a C array bound; the header and later short have different
lifetimes. Choosing 40, 42, or 64 would invent a declaration.

The related `credits_scroll` uses the same format and same runtime suffix
pointer. Its `char buf[76]` is explicitly described as frame-inferred in
`cloud/work/s20261004/E/credits_scroll.json`, so it is not independent source
provenance. It cannot justify a 76-byte declaration here either.

`fcvt_wrapper` forwards to the ordinary formatter, whose `%s` branch in
`src/rom/lib_34a0.c` uses unbounded `strlen` when no precision is supplied.
The required bytes are 13 + decimal-text length + suffix length, including
the terminator. If the displayed controller number is one digit, this is
14 + suffix length. The runtime suffix's length is not bounded by the
available tracked data. Even proving a safe output bound would not by itself
prove the original declared array capacity.

`root.c` requires a test-only enabling macro and an explicit
`MENU_OPTIONS_HEADER_CAPACITY`. The host fixture's 128 bytes exist solely for
its known short test strings. They are not a native declaration, inferred
stack allocation, compiler experiment, or frame-padding search. The verifier
checks that compiling the root without the test fixture fails closed.

Reopen native matching only with an original declaration or other independent
source evidence establishing local storage. Remove the fixture-only gate as
part of that reviewed change; do not compile a sequence of guessed capacities.

## Genuine resource selection, without synthetic summaries

`slot_state_setup.c` independently preserves the whole 232-byte native
routine's semantics. It saves and returns the signed previous slot byte,
stores the new low byte, skips resource work only for full input -1, looks up
or loads the two resources using the current signed slot byte plus 38/22,
activates newly loaded handles, refreshes the bank when the previous signed
byte differs from the full input, and applies object byte 9 only for full
input 0. The last two conditions are deliberately not comparisons against
the truncated new byte.

Native slot-state input arrives privately in s2; the routine uses s0/s1/s3
without saving them and returns the sign-extended previous byte. Its call to
`sound_update_channel` passes the force value privately in t0. Supplying a
plain external declaration does not teach IDO those summaries. Standalone
O32 compilation here checks source syntax only and has no matching status.

Older groups contain artificial dead-switch inline blockers and/or substitute
empty-debug-hook bodies. None is imported. The existing `game_C104` report
already records that truthful empty-hook source leaves nonmatching helper
context. Accepted `slot_sound` context also documents a dead switch added to
its empty hook. This packet changes neither accepted context nor those older
experiments, and does not use their exact scores to claim a genuine closure.
The new root will need actual downstream context validation after its local
storage is resolved. Native dead copies of slot return values are not
recreated with fake consumers or volatile locals.

## Checks

With the pinned IDO/MIPS tools available:

```
python3 cloud/work/dot_menu_options_root_20261005/verify.py
python3 -m pytest -q tests/conveyor/test_dot_menu_options_root.py tests/conveyor/test_dot_menu_row_caller.py
```

- 410,370 root-model cases under ASan/UBSan: all 16,384 visibility masks,
  five starting indices, five count limits, signed selectors, all 32 flag
  bits, three enabled-byte values, eight width edge cases, mode-1 delegation,
  and callback mutation of the next row/scroll limits
- 24,576 slot-model cases under ASan/UBSan: all 256 previous signed byte
  values, twelve full-width selections, four lookup-failure combinations,
  and mutation of the current slot byte during lookup callbacks
- Six O32 field/size assertions with pinned IDO
- Slot-source IDO syntax check, explicitly not its private-ABI match
- Integrity-checked native extent, callback pointer slots, format content,
  and all fourteen switch cases
- Fail-closed root compilation without fixture declarations
- Previous menu-row source hash unchanged

Host tests verify the reconstructed source against an explicit behavioral
oracle. They are not native execution, instruction-level equivalence, or a
proof of unrestricted formatter safety. Only source, semantic evidence,
hashes, and counts are published; generated objects and private native
inspection output stay under ignored `build/`.

## Draft integration and dependency

This root-only draft is stacked on menu-row PR #88, branch
`dot/menu-row-caller-20261005`, verified remote head
`6627928714f423d3a88aaf811a3b62bc46d00be7`, tree
`5c1513c910646e9ee39fcad8dbfd1c6d74be7133`. Its underlying master baseline is
still `cf10b3392d7f00ae42d75c008b79fdc2541aab6b`. The row packet is a dependency,
not additional root credit. After #88 merges, the independent checker can
retarget this draft to master. No merge or retarget is performed here.

`peer_review.json` records independent review of the exact root intake. The
integration adds a normal pytest replay of the full root verifier, including
the ASan/UBSan fixtures and O32 layout/syntax checks. It compares the fresh
receipt byte for byte and checks the subprocess exit status. Missing compiler
tools fail under the existing `REQUIRE_TOOLCHAIN=1` test policy.

The existing Verify workflow only runs pull requests targeting master. A
stacked draft therefore has no eligible CI run until it is retargeted; this is
**NOT_TRIGGERED**, never a CI pass. PR #88 separately has a strict submission
gate failure for two ordinary-scorer own-literal fixups; its complete helper
proof passes locally but does not waive that gate. This draft changes no
scorer, workflow, accepted claim, or production file. Its root-only changed
submission check does not contain any native match submission.

See `integration.json` for local focused/aggregate results, exact command exit
codes, unchanged-baseline failure comparison, and the source hash manifest.
