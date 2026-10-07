# Follow-up: actual queue context for existing bus/timing targets

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Research members, both already investigated in earlier PRs:

- `audio_bus_route`, `[0x8009570C,0x800957F8)`, 236 bytes / 59 words:
  **35/59 (#197) to 6/59 differing words**; native 72-byte frame.
- `audio_timing_sync`, `[0x80095924,0x800959DC)`, 184 bytes / 46 words:
  **7/46 (#243) to 2/46 differing words**. The only differences are the entry
  and exit frame adjustments: candidate 72 bytes versus native 64.

Both observations have zero nonzero excess words, unresolved symbols,
unverified relocations or errors. These are follow-up research results, with
no additional unique target extent, matching or accepted-coverage credit.

## Concrete new source lead

The #256 command-pop helper and #259 real internal lookup context recover the
native queue-call relationships. The accepted lookup still matches with its
actual setter callers; its historical volatile-list spelling is unchanged.
The bus target uses that context, the consumed command-pop return, typed entry
validation and independent field ordering. Its six residuals are two commuted
pointer adds and four spill references, as in the other three setters.

The timing target keeps only the real u32 slot index and obtains its command
through the same helper. Typed indexed entry access lets IDO retain the native
scaled offset, register and spill layout. All 44 non-frame words now match.
Directly flattening the command helper loses that correspondence; adding an
output-pointer or different input helper does not close the frame gap. No
unused local, dummy operation, artificial volatile, padding or altered flagset
is introduced to force the remaining two words.

The helper's name and original inline boundary remain explicit hypotheses.
`group.json` supplies all actual A158/list/lookup bodies. Prior priority/stream/
buffer/fade refinements are unclaimed context. Some context scores change in
this combined unit, so it must not replace their production owners wholesale.
Existing exact helpers do not add credit.

```sh
python3 tools/cloud/score.py group cloud/work/lean_queue_context_followup_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. Native O32 layouts, valid 24-byte entries and command/list storage
are assumed. No target, scorer, accepted-lock or production-source edits are
included. Independent checking, image integration and accepted coverage remain
separate from the local canonical observations above.
