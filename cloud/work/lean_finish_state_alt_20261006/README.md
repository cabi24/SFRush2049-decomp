# finish_state_alt: 556-byte real-group match candidate

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Sole target/claim: `finish_state_alt`, `[0x800F8B70,0x800F8D9C)`,
**556 bytes / 139 words**.

Observed canonical group output:

```text
Members:
finish_state_alt:
  MATCH
```

All 139 words match with zero nonzero excess words, unresolved symbols,
unverified relocations or errors. This is a locally observed match candidate,
not standalone compilation, independent verification, integrated image/ROM bytes
or accepted cartridge coverage.

## Source and causal changes

This complete real caller was first reconstructed as unclaimed context in
PR #214. It initializes/selects the actual UI state, submits the native queue
message, processes the current object chain, performs the observed transition
calls, and dispatches the existing mode handlers.

The original 30-word residual closes through these source/context changes:

- Read/update the real selection global directly in the native loop instead of
  carrying a pass-through scalar local.
- Reload the current object after each `entity_transform_apply` call before
  deciding whether to repeat. The first #214 draft incorrectly retained the
  old object pointer; #214 was separately corrected before this publication.
- Compile the complete accepted F84B0 filter as an internal member. Its own
  full 51-word body stays exact, while the caller can retain its real selection
  address in the native caller-save register. It is not newly claimed.
- Keep message allocation, type assignment and queue unlock on one physical
  source line. This changes IDO's two remaining scheduled words without
  changing C operations. The formatting dependency is explicit in the source.

The message view has its native halfword id and 24-byte allocation shape.
The accepted allocator established that shape; its body is not required for the
observed match and is not duplicated here. No unused locals, artificial arrays,
extra parameters, dummy reads or new volatile qualifiers are added.

## Required real context and reproduction

`group.json` records all files and exported roots. `caller.c` contains this target
and the complete real F87A0/F857C/slot-state bodies; the other C files carry the
new shutdown helper and the real accepted filter/clear/stop implementations.
The original six stand-ins from the archived caller packet are absent. Helper
sources with formerly huge preprocessed headers retain only the used O32
interfaces and their complete bodies. The old raw handle in the clear helper
is explicitly cast to the real stop pointer argument.

`audio_update_d` remains a full 19-word MATCH in this group, but was already
submitted in #214 and is context only here. Likewise, exact accepted helper
scores are not new claims. F87A0/F857C/slot-state remain nonmatching context;
this group must not replace their production owners wholesale. The stop helper's
old documented compiled-out index read is unchanged, not a new target device.

```sh
python3 tools/cloud/score.py group cloud/work/lean_finish_state_alt_20261006
```

Run from the repository root with the documented IDO 5.3 toolchain. Exact flags
are `-g0 -O3 -mips2 -G 0 -non_shared`, plus mandatory canonical assembler
`-r4300_mul`. There are no target, scorer, accepted-lock or production-source edits.

The source assumes native O32 object/message layouts, valid queues and the
observed external service contracts. Full original-module identity, arbitrary
aliasing/corruption and gameplay behavior have not been independently tested.
Only C, necessary real context, the recipe and these notes are published: no
harness, receipts, independent verification, full tests, CI wait, image/ROM
integration or merge. The independent checker owns acceptance and merging.
