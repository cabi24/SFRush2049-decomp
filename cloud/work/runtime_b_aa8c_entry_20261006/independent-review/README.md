# Independent AA8C entry, helpers and selector review

Verdict: PASS as bounded PARTIAL-SOURCE research. No required source correction
was found. This is neither a compiled-C validation nor a full-function MATCH.
Reviewed base: 6b2e9e506fe3d2267a710e41c85af5364ccd00c7. Review date: 2026-10-06.

## Replayed evidence

- Original entry native/reference replay: 8,160 pairs, 150 native instructions,
  18 conditional branch outcomes, and 13 rejected wrong-contract/fail-closed
  controls. The supplied receipt was reproduced exactly.
- Independent actual-source-token interpretation: 8,480 native/source-AST pairs.
  This includes the original 8,160-domain fixtures plus 320 additional descriptor
  handle and trailing-state boundary fixtures. The same 150 instructions and
  18 branch outcomes were reached. All nonstack fixture bytes, terminal states
  and ordered external-service traces agree.
- Six changed C-source contracts were rejected: narrow sentinel, wrong trailing
  byte, early sentinel store, wrong cache selector, unconditional inhibit read,
  and player-state pointer reload after cleanup.
- Source-derived layout checks verify all six struct sizes and the relevant
  member offsets; owner/handle/mode signedness and unsigned model-byte typing
  agree with observed native loads.
- Selector replay: 936 cases and 170 slice instructions. Supplied receipt agrees.
- Entry pytest: 1 pass in a source-only integration-shaped overlay, both with the
  configured IDO environment and from /tmp with an explicitly absent IDO path.
- Selector pytest: 1 pass normally and 1 pass from /tmp with absent IDO.
- Independent source replay: identical review.json and review-no-ido.json from
  ordinary versus absent-IDO/foreign-working-directory execution.
- Python syntax compilation passed. No C compiler was invoked.

## Source correspondence

The actual helper and incomplete-prefix text, rather than a manually duplicated
Python cleanup algorithm, is parsed by source_semantics.py. The parser permits
one missing outer compound terminator at end-of-file and marks that condition.
It does not append a closing brace, invent a suffix, emit a complete root, compile
private helpers with an ordinary ABI, or introduce a keeper/wrapper. Falling off
the parsed prefix means “continue into the missing source”, never a C return.
It requires the available pycparser 3.00 parser API.

The independently interpreted source agrees on:

- short narrowing of update and private-helper owner;
- update-zero short circuit and any nonzero signed inhibit value;
- signed cached-kind dispatch, separately from the reset mode field;
- retained selected-player pointer across the cleanup service;
- all-32-bit sentinel comparisons and signed-16-bit removal arguments;
- fresh later-slot/extra-handle reads and stores after service calls;
- conditional clearing of group+0x139, with group+0x138 untouched;
- late descriptor handle read followed by mode-0, mask-15 hide;
- preservation of other fixture bytes, including neighboring records.

Nonvolatile C reads are source-level evaluations here. This is not proof that an
optimizing compiler retains or orders those machine loads. Native AB18's
continuation color load belongs to the still-missing continuation behavior;
this source check compares terminal state and effects, not identical whole read
traces. The original native replay separately checks the real shared-color read
and the branch-likely load.

Signed narrowing uses the established target signed-byte/halfword behavior,
not a claim of portability to every ISO C implementation. External service
fixtures are shared with the native runner; their scene internals are not
executed. The deliberately broad service-mutation hooks probe ordering, not an
assertion that real scene removal changes descriptor/cache/player data.

## Independently reviewed context

Context was read at the recorded base using git show, never inferred from the
stale checkout HEAD and never pinned by integration-sensitive file hashes.

- B27E4 starts at player+0x110, iterates exactly 21 records of 0x18 bytes, writes
  callback+0x14 = null and handle+6 = -1. Its record 20 is player+0x2F0. The
  world_physics_tick call at EC464 receives the current player pointer from
  EC428. The descriptor-init correction is supported.
- The B1B74 setup loop visits indices 0..5; physics[index]+0x7C8 controls whether
  its two registration/update calls run. B0D1C reaches C910 only for mode 6.
  C910 receives the same narrowed index and has no 0..3 range check. Thus the
  entry/selector fixture owner domain remains explicitly 0..3; owners 4/5 are
  unresolved, not proved inactive.
- C910 embeds the descriptor at player+0x2F0, tests its signed handle, stores
  the AA8C callback and owner, then creates the scene object. The descriptor
  source view's size 0x18 does not absorb the separate later +0x18 control word.
- Selector native replay confirms mode 8 really loads from 0x80394884 through
  0x80394920 for fixture models 0..12, overlapping the separately read color
  and later vertex/color data. No eight-row or invented nine-row float object
  is justified. The entry header wisely declares no such table extent.
- Normal mode producers 0..7 plus reset mode 8 do not prove every aliased write
  or every model selector. The cache sentinel 9 is separate from current mode.
  Preserve the model-domain and complete-function source-alias prerequisites.
- Accepted sound_call_minimal invokes entity_spawn_callback(handle,0,0).
  The accepted downstream source mutates scene links/free markers and count.
  Accepted mode-0 model_data_load sets the scene hidden bit. Those facts do not
  make invalid/truncated handles safe. Both packets state this boundary.

## Scope and delivery

No production code, protected tool, context, lock, asset, branch or PR changed.
No publication, full-suite matrix, or CI monitoring was performed by this review.
The source prefix remains deliberately incomplete. The future genuine closure
must resolve the remaining root, ABI, owner/model bounds and external table
aliasing before canonical O3 compilation and full relocated equality proof.

Portable review files to preserve:

- README.md
- source_semantics.py
- verify_review.py
- review.json

review.json binds only reviewed packet inputs, these semantic review tools,
native target words, and the base commit. It does not hash a pytest file,
live/historical scorer, manifest, production source, or lock.

Reproduce from any working directory (pycparser 3.00 required):

    python3 /path/to/review/verify_review.py \
      --packet /path/to/cloud/work/runtime_b_aa8c_entry_20261006 \
      --reference-root /path/to/SFRush2049-decomp \
      --output /tmp/aa8c-independent-review.json

The entry packet must have the normal canonical tools/cloud/score.py available
at its documented repository/overlay root. No IDO or linker is needed.
