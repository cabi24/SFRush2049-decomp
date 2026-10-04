# Bounded negative-research checkpoint

This branch preserves completed research and explicit stop conditions after
PR77. It adds **zero matching functions, zero matching bytes and zero unique
attempts**. The accepted-source baseline remains PR77 commit
`12ae34a64dd3a9004e1b949c9acf4557542df2f2`, tree
`7c37dbb0cfebf4a9152c2be7f66cd85d9b1be747`: 240 new matching bodies / 28,372 B,
with the historical getter separate. Its exact-head Verify run 37184548717
succeeded. No canonical matching file, current source classification or
historical manifest is changed by this checkpoint.

## Preserved evidence and review limits

- `loop_scheduling_scout.json`: source-free read-only scout. The remaining
  same-line scalar loops already have their native store schedule. The packed
  record copies already have separate header/body lines. No new source form
  was justified or attempted by the scout.
- `record_copy_causal_audit.md`: source-free causal handoff for 15720 and 158D8.
  The first divergence is a fixed assembler-temporary carrier selected before
  scheduling. The upstream compiler implementation is a version-limited lead,
  not verified correspondence to the pinned stock5.3 lowering.
- `BT07-stream-schedule-retry`: author-recorded source-free negative audit of
  the two-word 25F74 residual. No candidate was edited. Existing forms already
  cover the proposed scheduling spellings. No independent retry review is claimed.
- `BT03-spatial-threshold-retry`: independently reviewed complete nonmatch.
  One source-line change improves DC08 from three to two words; two further
  justified forms do not close it. The original published archive and its central
  classification are retained. Its packet-local status delta is not applied here.
- `BT05-id-register-retry`: **unreviewed research checkpoint**. One donor-width
  control worsens 22324 from two to seven words; the original source is retained.
  Author tests and the fresh central replay are evidence, not an independent
  source/ABI acceptance review.
- `BT03-voice-priority-retry`: **unreviewed research checkpoint**. One donor
  assignment-test control reproduces the local bucket load/test/store/reread
  topology but worsens 1EF8C from88/108 to92/108 and expands the frame48 to56 bytes.
  The original source is retained. Author tests and central replay do not confer
  independent acceptance or matching credit.

The manifest binds each imported packet to its immutable author/reviewer commit
and file hashes. Source-free scout artifacts are separately hashed. Fresh
checkpoint verification is recorded in `checkpoint_verification.json`. These
checks protect reproducibility and scope; this branch is not a new matching cut
or a request to promote the rejected candidate sources.

## Concrete reopen conditions

1. For packed-record copies, obtain a read-only trace or verified source-level
   correspondence for the approved stock5.3 copy lowering. It must expose copy
   opcode/alignment/length, expression references, live/reserved/free registers,
   carrier allocation/free events and the actual condition selecting the
   assembler temporary. Only then consider a genuine C construction or original
   type/macro evidence that changes that condition while preserving the real ABI,
   eight-byte geometry, packed-field contract, loop direction and semantic domain.
   A7.1 implementation or a register-name guess alone does not close this gate.
2. For stream/DC08 scheduling residuals, require new measured stock scheduler or
   original source/translation-unit evidence beyond the preserved line and loop
   forms. Unknown external values, volatile qualifiers, padding, directives and
   arbitrary line sweeps are not substitute evidence.
3. For identifier/priority lowering, require an authenticated conversion,
   coalescing or liveness explanation that supports a natural new source form
   without widening genuine byte/halfword inputs, changing live reads or repeating
   the retained negative controls.
4. Remaining untouched functions still require their recorded table mappings,
   boundary/decoder interface evidence, corpus ownership or spec-T050 gates.
   Exhausting a nonmatch does not satisfy a requirement that it be matched.

No further candidate is authorized by this checkpoint. Protected source,
targets/scorer, ABI/layout definitions, locks and D10 remain unchanged. Only
reconstructed C/tests and source-free project evidence are included; no ROM,
raw assembly listings, objects, credentials or unrelated private material.
Merging and cartridge integration remain with the independent checker.
