# Implementation handoff — 004-promotion-splicing

**For**: the implementing agent (Opus). **Spec author**: Fable, 2026-07-08
(final Fable session — the review may also fall to you or the owner; the spec
is written to be self-arbitrating).

## Assignment
Implement tasks.md T001–T014 in order on `004-promotion-splicing`. Contracts
are normative. research.md decisions D1–D9 are settled — if one fails on
contact, STOP and report with evidence (the 003 precedent: honest blockers
were worth more than forced green).

## The rules that decide review
1. **T006 is the go/no-go**: one converted, zero-promotion segment must build
   a SHA-1-exact ROM before anything else proceeds. No second conversion, no
   promotion, until it passes. Expect real friction here (asm-processor
   integration, nonmatchings includes, late_rodata) — solve it on segment
   0x8800 only.
2. **The SHA-1 gate is never weakened, mocked in live runs, or bypassed.**
   Unit tests may mock the build; quickstart runs may not.
3. **Transaction atomicity**: any failure leaves `git status` clean and the
   ROM buildable-exact. Test the rollback drill for real (SC-003).
4. **One transaction implementation**: the CLI and the conveyor job call the
   same library. Two implementations = failed review.
5. **Never edit generated files by hand** (linker script, nonmatchings asm,
   passthrough pragma lines); conversions live in splat.us.yaml.
6. All 002/003 standing rules: stdlib-only Pi tooling, additive migrations
   rehearsed on a DB copy, stage explicitly, locks are never re-pinned to
   paper over a failure.

## Environment
Coordinator (Pi :8323) + watchman agent were running as of 2026-07-08 evening;
matching builds require watchman (IDO cannot run on the Pi — 16KB pages).
Branch stacks on 003. 12 locked functions; 19 reloc_only_diff. The July-7
uncommitted files remain untouched. asm-processor is NOT yet vendored (T001).

## Stop-and-ask triggers
- T006 fails after honest effort on 0x8800.
- Segments holding locked functions derive with refusals (layout can't tile).
- The flag-conflict in research D5 resolves neither way cleanly.
- Anything approaching rules 2, 3, or 5.

Report: tasks status (honest), MEASURED numbers, deviations with reasons,
anything flagged for review.
