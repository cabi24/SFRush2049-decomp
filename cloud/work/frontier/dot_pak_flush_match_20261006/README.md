# Pak flush wrapper: observed local match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `MaxPathZeroControls`, **396 bytes / 99 words**. The historical name
covers a Controller Pak per-handle flush wrapper.

The canonical local group comparison reports **MATCH: 0/99 differing words**,
with no unresolved/unverified references, relocation errors or extra nonzero
words. This is a local match candidate, not accepted coverage or an image/ROM
claim. The independent checker owns acceptance and integration.

## Source change and compiler assumptions

Baseline: the genuine C96 group, specifically
`cloud/work/game_C96/group_bounds_assignment.c`, with a 12/99 residual and
72-byte frame. Its operations are retained exactly, but the queue operations
are factored into the same real init/lock/unlock sequence reconstructed in
PR #163's Pak source:

- `pak_queue_init`: conditionally create and seed the queue.
- `pak_lock`: initialize if needed, then receive into its actual `OSMesg` local.
- `pak_unlock`: send the release message.

Those helpers inline and naturally restore the target's 88-byte frame and
message home, reducing the residual to 3/99. No padding, dummy locals, extra
accesses or artificial call is introduced. Helper boundaries/names are an
explicit source-structure hypothesis; they are not proven original identities.

The final three words depend on source-line scheduling: placing the existing
`D_8011194C = 1;` assignment and `osCreateMesgQueue(...)` statement on the same
source line preserves the observed assembler order. Their execution order and
arguments are unchanged. Reformatting that line may change the generated code.
This sensitivity is disclosed rather than presented as a semantic discovery.

All seven real C96 bodies remain present; only the flush wrapper and these
factorings change. The other bodies are nonmatching context, not new claims.
The existing keep list and private-call visibility remain hypotheses about the
original program. No forced register, false prototype or stand-in is used.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_pak_flush_match_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The canonical group pipeline includes `-Olimit 5000` and `as1 -r4300_mul`.
The empty `claims` list does not assert promotion readiness; the normal command
above scores the member. No broad tests, independent acceptance replay,
image/ROM integration or CI watching was performed before publication.
