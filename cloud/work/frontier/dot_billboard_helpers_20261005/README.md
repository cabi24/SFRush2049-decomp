# F207C: genuine name-entry caller group

Date: 2026-10-05. Base: `f88dc3cb3807b8be3246b719a9b1e008210b477c`.

**Result: strict MATCH for `func_800F207C`, all 421 words / 1,684 bytes.**
The complete ELF symbol has the exact native extent, all relocated words equal
retail, and a separate GNU MIPS link at `0x800F207C` produces the same complete
body. No unresolved or unverified references. **No splice, ROM, or coverage gain
is claimed.** An independent review and normal integration gates remain required.

## Claimed range and source provenance

- Claim: F207C `[0x800F207C, 0x800F2710)`, internal player-name entry helper.
- Genuine caller context: `billboard_render` `[0x800F64D4, 0x800F68A4)`.
  The original complete `cloud/work/ipa-groups/billboard_render/br.c` is reused,
  changing only the table/count names to current canonical symbols and removing
  its obsolete stand-in comment. The synthetic `ctx.c` is **not used**.
- Billboard remains a **NONMATCH** and is context only. Its unknown callees are
  external declarations, not guessed bodies. The pair contains one real native
  call site to F207C, in the state-3 player loop, passing the index in s0.
- The checkout has no AGENTS.md or `.agents/skills`; CLAUDE.md and the matching,
  promotion, compiler and wave guidance were read. The current frontier/claim
  evidence showed no conflicting claim; unpublished external work was not
  observable. No other worker's directory or production source was changed.

## What the helper does

F207C consumes the 76-byte player input record: unsigned player ID at +1,
pressed buttons at +4, repeated navigation buttons at +12. It moves around a
54-character, nine-column name-entry grid, with three special selections:
`-1` finish, `-2` delete and `-3` space. Names are at most 12 bytes plus terminator.
It trims trailing spaces on finish, calls the existing name filter/cache helpers,
and copies the cache index to matching entries of the 12-by-3-by-5 result table.
No arcade donor has been identified; this is a retail-grounded reconstruction.

The native finish path reads the byte immediately before the current name end
before checking any length. **Empty-name finish is reachable**, including after
deleting the last character. At length zero, the reconstruction therefore
indexes `D_80143A50[player][-1]`, preserving the native pre-buffer read but
invoking undefined behavior in standard C. Independent review reproduced this
with UBSan. No guard absent in retail has been added to alter the matching body.

The 240-case host fixture **excludes empty-name finish**. It checks deletion to
empty, but does not then finish that empty name. Its UBSan pass establishes only
the tested domain; it is not a full safety/behavior proof for every reachable
state. This inherited boundary behavior remains a review caveat, not a claim
that callers guarantee a nonempty name.

## Two source corrections, no shaping search

The first complete draft had 211/421 differing words and a 1,680-byte extent.
Two substantive corrections in the second compile produced strict MATCH:

1. Keep a real `name` pointer for the two consecutive calls to F084C and F1D04.
   This reproduces the native s1 pointer lifetime instead of keeping the table
   base alive and recalculating the pointer.
2. Store the returned cache slot in the native order: physical player-ID table
   first, current player-slot table second. The chained assignment now expresses
   that actual sequence.

No padding variables, empty reads, fake helpers, register keepers, compiler flag
changes, protected context changes, or scorer changes were introduced.

## Context conditions: exact distinction

The shipped two-file group, with only `billboard_render` kept, independently
prints strict MATCH. No blocker or synthetic context is present in that group.
Standalone compilation is deliberately recorded as a negative control: it gives
an ordinary ABI, unlike the native s0 parameter and unsaved s1–s5 working set.

The verifier also expands the group with the **unchanged accepted sources** of
all five in-image direct callees: audio_distance_atten, audio_doppler,
resource_type_select, F084C and F1D04.

- Without any shadow staging, umerge inlines the two audio wrappers. F207C then
  differs (410/421 words, 24 excess words), while all five callee bodies remain
  exact. This is reported, not hidden.
- With the **existing** shadow policy for audio_distance_atten and audio_doppler,
  F207C and all five accepted callees are again exact. The verifier invokes the
  unchanged `blob_unit.transform` for those two already-listed blockers only.
  It adds no new blocker or override and never writes production context.

This is a scoped direct-context test, **not** a complete `blob_unit check`.
The full unit, image splice and ROM SHA-1 gates have not run in this cloud task.
The separate original F2718 group contains synthetic callers and an incomplete
F1210 reconstruction; it was not imported into this packet. That existing
context should not be mistaken for a genuine complete F1210 when extending the
billboard group.

## Reproduce

With the pinned IDO/binutils environment:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_billboard_helpers_20261005/group
python3 cloud/work/frontier/dot_billboard_helpers_20261005/verify.py
python3 -m pytest -q tests/conveyor/test_billboard_name_entry_packet.py
```

The stock scorer reports F207C as MATCH and billboard as a nonmatching context;
the group exits zero because informational context is excluded from its gate.
The verification command checks the complete F207C extent and independent link,
records both expanded-context conditions, and runs the host fixture under UBSan.
The fixture covers 240 selected behavior cases, including all 228 directional
moves, button priority, character/space insertion, maximum length, both deletion
paths, confirm/timeout completion, trimming and result-table propagation.

`verification.json` binds source, tool, compiler and complete native-body hashes.
Only metadata is saved: no ROM bytes, raw assembly, ELF objects, or credentials.
