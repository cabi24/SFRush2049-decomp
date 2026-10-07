# LEAN RESEARCH: time_result_display with both real callers

Frozen tool/target base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `time_result_display`, 800DA174..800DA2C0, 332 bytes / 83 words.

## Observed result

The published display source at standalone O3 gives **67/83 differing words**,
1 nonzero excess word and a 340-byte ELF function. With both genuine callers
and the actual slot-selection body, it gives **4/83 differing words** and an
exact **332-byte ELF function**, with no unresolved/unverified/error fields for
this target. The canonical scorer still reports **3 nonzero excess words** in
its next-symbol span; that diagnostic is retained without slicing or masking.
This remains a research nonmatch, not accepted matching or ROM coverage.

Flags: IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`, canonical R4300 erratum
handling. This is explicitly a standalone-versus-real-group comparison.

## Minimal real context and provenance

- `display.c` retains the complete existing body from
  `src/blob/groups/credits_scroll_grp/gr3_c.c`, with the real font wrappers and
  current pointer interfaces for the panel factory/update. It formats the actual
  controller header when needed, measures text, then updates or creates its panel.
- `slot_context.c` is the genuine slot_state_setup definition from that group's
  `gr3_b.c`; unrelated definitions after it are omitted. Its callees stay external.
- `options_caller.c` is unchanged complete DB1E0 from the earlier DA2C0 options
  reconstruction (original base 9651ca53bfd16b015adb63e1b3d76487a995a84f).
- `replay_caller.c` is byte-identical to the complete newly published
  [replay setup source, PR 212](https://github.com/cabi24/SFRush2049-decomp/pull/212).
  DB1E0 and replay_save_prompt are the two native direct callers.

All context source hashes are in observed.json. There is no stand-in caller,
invented callee, empty pressure condition, filler local, assembly or new volatile.
The obsolete synthetic context used by earlier provisional attempts is absent.
The existing 76-byte meaningful format buffer is inherited from the prior display
body; its original declaration is not claimed recovered, and formatted output
must fit it. No buffer-size sweep or stack-padding substitution is made.

## Unresolved context and assumptions

This is a bounded compilation group, not the complete original unit. The three
context bodies remain nonmatching and receive no new matching claim:
- replay_save_prompt: 373/385, 1592 versus 1540 bytes, 12 excess; owned-data
  relocation diagnostics remain.
- DB1E0: 344/350, 1316 versus 1400 bytes, two unverified owned-table sites.
- slot_state_setup: 18/58, exact 232-byte ELF function, two nonzero excess words
  in the canonical scorer's next-symbol span.

Other real callees remain external. Valid label tables, selected player index,
queue/panel contracts, N64 O32 pointer widths, bounded formatted text and
representable signed coordinate arithmetic are assumed. The function preserves
native live panel-pointer reads after the line-height call. It does not invent
an allocation-failure fallback or assert malformed-input safety.

## Minimal replay

Set `IDO_DIR` to the pinned compiler and run:

    python3 replay.py --repo /path/to/SFRush2049-decomp

The replay reads immutable named Git inputs, compiles the same display source
alone and in the actual four-function group, and reports canonical results,
source hashes and ELF extents. Raw diagnostic byte excerpts are redacted; all
failure/unverified states remain. No scorer, target or comparison boundary changes.
No independent review, behavior harness, full suite, CI wait or ROM gate was
run. Checker/Claude owns verification, acceptance and ROM integration.
