# sound_priority_set: actual lookup context and command helper

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole research target: `sound_priority_set`, `[0x80095A24,0x80095B10)`,
236 bytes / 59 words. No matching claim.

Observed local canonical score: **6/59 differing words**, zero nonzero excess
words, unresolved symbols, unverified relocations or errors. The previous
complete A158 source scores 39/59. The candidate has the native 72-byte frame.
The remaining six differences are two commuted pointer-add operands and four
stack-spill references (offset at 48 rather than 52, node at 52 rather than 60).

## Source/context changes

- Validate through the typed 24-byte entry field, preserving the native low-byte
  slot calculation and post-callback table reload.
- Include the current accepted `func_800956BC` body and leave it internal with
  its actual four setter callers. It remains a full 20-word MATCH, but now
  exposes its real register-clobber contract so the caller retains offset in a2.
  No synthetic keeper or private prototype is used.
- Factor the actual free-command operation into the consumed `take_command`
  return helper, following #256's source lead. Its original name/inline boundary
  remain hypotheses; there is no extra operation or unused storage.
- Assign the actual entry pointer before the sentinel field statements. These
  disjoint stores schedule in the observed native order under IDO.

The accepted lookup's pre-existing volatile-list spelling is copied unchanged
and disclosed in its original header; no new volatile is introduced. The #191
fade refinement and #197 bus-route source are preserved as real context only.
All callers and complete list bodies are declared in `group.json`. Existing
helper matches are not additional credit. Nonmatching context must not replace
production owners wholesale.

```sh
python3 tools/cloud/score.py group cloud/work/lean_sound_priority_set_20261006
```

Use IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`, with canonical assembler
`-r4300_mul`. Native O32 layouts, valid command lists and the 24-byte entry/key
contract are assumed. No target, scorer, accepted-lock or production-source
edits are included. This is local research evidence; independent checking,
image integration and accepted coverage remain separate.
