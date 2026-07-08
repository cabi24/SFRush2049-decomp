# Implementation handoff — 003-reloc-aware-targets

**For**: the implementing agent (Opus). **Spec author**: Fable, 2026-07-08.
**Review**: Fable reviews the finished diff against this spec. Deviations need a
written one-line reason in tasks.md/quickstart.md — recorded, not silently
substituted (the 002 handoff discipline; it worked).

## Your assignment

Implement `specs/003-reloc-aware-targets/tasks.md` — T001–T018 in order, on
branch `003-reloc-aware-targets`. The spec set is complete: spec.md (stories,
FR-001..010, SC-001..007), plan.md, research.md (D1–D8 decisions — do not
re-litigate; if one breaks on contact with reality, stop and report),
data-model.md and the two contracts (normative), quickstart.md (the acceptance
run — you execute it and record measured numbers in place).

## What this feature is, in one paragraph

Target objects are currently raw ROM words with absolute addresses baked in, so
compiled code that references globals can never score 0. You will assemble
static targets from their splat asm regions (which carry `%hi/%lo/jal` symbol
operands) so they get real relocations, guarded by a per-target round-trip gate
(masked-word equality against the ROM; raw-word fallback, reason recorded), and
you will make the evidence base supersede correctly when target objects change
(attribution echo + derived-row purge). Then the 19 `reloc_only_diff` targets
re-score to true 0 through 002's existing machinery, untouched.

## Definition of done

- [ ] T001–T018 checked off in tasks.md, deviations noted with reasons
- [ ] Full local suite green (`pytest tests/conveyor -m "not node_required" -q`)
- [ ] Quickstart executed; every **MEASURED** blank filled with real numbers
- [ ] SC-001..SC-007 verifiably true; specifically: osCreateMesgQueue true=0
      through the ordinary corpus cycle, `lock verify` green for all 12,
      attribution check 0 mismatched, second extract run changes nothing
- [ ] Committed on `003-reloc-aware-targets`, only your files staged

## Hard rules (002's all apply; these are the 003-specific ones)

1. **The gate is not optional and not softenable.** A target that fails the
   round-trip gate keeps its raw-word object, full stop. No "close enough",
   no masking-rule widening to make a stubborn target pass. Fallback + reason
   is a *success* outcome of the gate.
2. **Reuse `jobs/scoring.py`'s mask helpers for the gate** — one mask logic in
   the codebase. If the gate and the reloc-blind score ever disagree about
   masking, that IS the bug you report.
3. **Purge only `matrix_entry` rows.** Result blobs, `work_unit`, and
   `function_status` are never deleted by supersession. The 002 flag recompute
   handles flag consequences on its own.
4. **No special-casing the acceptance targets.** The 19 flagged and the 12
   locked functions go through ordinary paths. Hardcoding any target_id
   anywhere (outside test fixtures) fails review.
5. **Scorer and permuter untouched** (research D6). If osCreateMesgQueue does
   not reach true 0 at T015, stop and debug that one pairing on watchman —
   do not patch the scorer, do not lower the bar, do not proceed to scale.
6. **Migration discipline**: additive, PRAGMA-guarded, rehearsed on a copy of
   the live DB (281,933+ evidence rows) before first real connect.

## Environment (live right now)

- Coordinator on the Pi (port 8323, log `~/.conveyor/coordinator.log`);
  watchman agent attached (builder, 16 cores). Restart commands in
  `tools/conveyor/README.md`.
- Pinned toolkit `b613fc5d…` (002's) — T013 replaces it with yours.
- `mips-linux-gnu-as`/`objdump` confirmed present on the Pi.
- 19 targets currently flagged `reloc_only_diff`; 12 functions locked in
  `matched.lock.json` (the pre-commit hook verifies them on every commit).
- Branch `003-reloc-aware-targets` is stacked on `002-corpus-candidates`.
  The working tree still carries unrelated uncommitted changes from an older
  session (src/game/game.c, tools/conveyor/cli.py, pipeline/flags.py,
  pipeline/sweep.py, july7notes.md, two test files, docs/WIKI.md) — stage your
  files explicitly, never `git add -A`.

## When to stop and ask

- A research decision (D1–D8) fails on contact (e.g. splat region text won't
  assemble for a large class of functions — a few fallbacks are expected and
  fine; hundreds mean D2 needs rethinking).
- T015's single-function gate: osCreateMesgQueue ≠ 0 after reloc-aware targets.
- Anything tempts you toward rules 1, 3, or 5.
- The purge would delete more than ~40% of all evidence rows (expected: purges
  touch only static targets' rows; the dynamic population's evidence survives).

Report at the end: tasks completed, deviations + reasons, the measured
quickstart numbers (tier counts, flag 19→after, attribution check), and
anything flagged for review.
