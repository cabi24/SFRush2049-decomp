# Two remaining typed audio setters: six-word residuals

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Research members only, with no matching claims:

- `audio_stream_control`, `[0x80095B10,0x80095BFC)`, 236 bytes / 59 words.
- `audio_buffer_manage`, `[0x80095BFC,0x80095CE8)`, 236 bytes / 59 words.

Both original complete A158 bodies score 39/59 words off. Both submitted bodies
score **6/59 words off**, with zero nonzero excess words, unresolved symbols,
unverified relocations or errors, and the native 72-byte frame. Total researched
target extent is 472 bytes; it is not matching or accepted-coverage credit.

The concrete source lead is #259's actual lookup/command-pop context. Each
setter now validates the typed 24-byte entry, uses the real internal accepted
lookup, obtains the actual free command through the consumed `take_command`
helper, then assigns the entry pointer before independent sentinel fields.
The chosen value still updates only the native stream or value field; existing
queued nodes keep their direct-update path. Table reloads after callbacks remain.

The real lookup stays a full 20-word MATCH with its actual callers. Its inherited
volatile-list spelling is unchanged, not a newly introduced matching device.
The helper name and original inline boundary remain hypotheses; all operations
and dataflow are genuine, with no padding, unused locals or synthetic calls.
The six residual words in each member are the same two commuted pointer adds
and four spill references identified in #259.

`group.json` declares all sources and roots. Prior priority/bus/fade research and
existing exact helpers are context only and receive no new credit. This group
must not replace unrelated nonmatching production owners wholesale.

```sh
python3 tools/cloud/score.py group cloud/work/lean_audio_setter_pair_20261006
```

Use IDO 5.3, exact `-g0 -O3 -mips2 -G 0 -non_shared`, and canonical assembler
`-r4300_mul`. Native O32 layouts, valid 24-byte entries and command/list storage
are assumed. No target, scorer, accepted-lock or production-source edits are
included. Only these local canonical observations are claimed; independent
checking, image integration and accepted coverage remain separate.
