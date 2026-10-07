# F857C menu-panel switch: lean match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800F857C`, `[0x800F857C,0x800F874C)`, **464 bytes / 116 words**.
Observed canonical O3 result: **0 / 116 differing words**, zero nonzero excess,
unresolved symbols, unverified relocations or errors. This is a matching source
candidate for the independent checker, not accepted ROM/image coverage.

## Source-backed change

The old two direct lock/slot/unlock sequences discard the previous font return.
The existing real `font_set` wrapper returns that previous selection after
unlocking. IDO inlining retains the native two return-value copies without a
pressure variable, dummy operation, new volatile qualifier or fake caller.
The wrapper is copied from frozen `cloud/work/s20261004/E/src/helper.h`.

The source/caller context is the already-published PR229 group, commit
`38e202bd4ace1d2844c160cf1bc67ac9586e135c`, with its complete genuine
`finish_state_alt` and `audio_update_d` bodies from PR222/214. The accepted full
F84B0 filter and clear/stop helpers are retained. No archived stand-ins remain.
The genuine HUD countdown callback from PR220 is also present: without that
additional actual slot-state caller, IDO inlines slot_state_setup into the font
wrapper and loses the native out-of-line boundary. All group roots are real.

The countdown C comes from the corrected PR220 body, SHA256
`d41ecea76cddbccd58af80410a26666fa283e45beb5bd0a6d21cd712b1b43837`.
Its slot/font helper calls are ordinary gameplay/UI context, not synthesized
calls. Its O32 object-manager length contract is signed 16-bit. This packet also
normalizes the existing caller declaration to that length contract and models
the fifth audio_frame_sync argument / D_80114740 as a pointer. These declaration
normalizations do not change the baseline score.

## Observed comparisons

- PR229 context: F857C 95/116, 456 bytes, zero excess.
- Same context plus the genuine countdown caller and corrected declarations:
  F857C 95/116, 456 bytes, zero excess.
- Same context with only the two font operations replaced by font_set(13) and
  font_set(10): F857C 0/116, 464 bytes, zero excess, native 40-byte frame.
- Wrapper without countdown context: 103/116, 816 bytes, 85 nonzero excess
  words; the slot body inlines. This unsuccessful control is not the candidate.

The candidate retains the prior exact audio_update_d 0/19 and finish_state_alt
0/139 results. F87A0 stays 54/242. slot_state_setup stays 18/58 with two canonical
nonzero excess words in its next-symbol span (its own ELF extent is 232 bytes).
These are contextual observations, not new matching claims. Only F857C is
claimed by group.json. The group is not a recovered original whole translation
unit, and its unclaimed bodies must not replace accepted owners wholesale.
The inherited sound_stop context retains its earlier documented compiled-out
read; no new such operation was introduced for this target.

## Reproduce

From the repository root, with the documented IDO 5.3 toolchain:

```sh
python3 tools/cloud/score.py group cloud/work/lean/game_menu_panel_switch_20261007
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, plus canonical mandatory assembler
`-r4300_mul`. To recover the 95/116 paired control, replace the two font_set calls
inside F857C with the original osRecvMesg / slot_state_setup / osJamMesg sequences
from the pinned PR229 caller, leaving the genuine countdown context in place.
The canonical scorer and protected target manifests are unchanged.

Assumptions: native O32 interfaces, valid UI/state objects and queue contracts;
the inferred shared compilation context is not proof of the original source
layout. No independent behavioral, corruption, concurrency or aliasing claim is
made. Only source, real context, build recipe and observed research notes are
included. No independent review, harness, full suite, CI wait or integration was
performed; acceptance and merging remain with the independent checker.
No ROM bytes, raw assembly dumps, binaries or credentials are included.
