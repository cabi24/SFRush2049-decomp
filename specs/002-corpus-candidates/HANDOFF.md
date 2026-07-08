# Implementation handoff — 002-corpus-candidates

**For**: the implementing agent (Opus). **Spec author**: Fable, 2026-07-08.
**Review**: Fable reviews the finished diff against this spec; deviations need a
written reason in the PR/commit message, not silent judgment calls.

## Your assignment

Implement `specs/002-corpus-candidates/tasks.md` — all 19 tasks, in order, on
branch `002-corpus-candidates`. The spec set is complete and self-contained:

1. `spec.md` — what and why (3 user stories, FR-001..013, SC-001..007)
2. `plan.md` — structure, constitution gates, rollout note
3. `research.md` — every design decision with rationale; **do not re-litigate
   these**; if one proves wrong in practice, stop and say so rather than
   substituting a different design
4. `data-model.md` — schema deltas, id grammar, artifact format (normative)
5. `contracts/` — CLI surface and the reloc-blind algorithm (normative;
   `scoring-reloc-blind.md` says "implement exactly this" and means it)
6. `quickstart.md` — the acceptance run; you will execute it and record real
   numbers into it (T012, T013, T016)

## Definition of done

- [ ] T001–T019 checked off in tasks.md (update the file as you go)
- [ ] Full local suite green: `python3 -m pytest tests/conveyor -m "not node_required" -q`
- [ ] Quickstart executed for real; measured numbers recorded in quickstart.md
- [ ] SC-001..SC-007 each verifiably true (quickstart §§3–5 map to them)
- [ ] Committed on `002-corpus-candidates` (project commit conventions; the
      pre-commit hook must pass — if `lock check` fails you edited a matched
      function: revert, never re-pin)
- [ ] tasks.md updated with any deviations, each with a one-line reason

## Hard rules (violating any of these fails review)

1. **Stdlib only, Python 3.9 ceiling**, on Pi and nodes. No new dependencies.
2. **Never touch** the vendored decomp-permuter, the true-score path, or
   `src/` game code. This feature adds evidence and artifacts; it changes no
   matched source.
3. **The live DB is production**: `~/.conveyor/conveyor.db` holds 281,933 scored
   cells and the function-status state machine. Migrations are additive +
   idempotent only. Rehearse against a copy (`cp` it to /tmp) before first
   connect if you changed db.py.
4. **Nothing promotes, locks, or lands in the repo without true score 0**
   (SC-007). `reloc_only_diff` is a review flag, not a match.
5. **Node-side code (`tools/conveyor/jobs/`) is inert until the toolkit rebuild**
   (T012). Don't debug "why didn't my scoring change take effect" — rebuild and
   republish per quickstart §1. One rebuild total; it also ships the pending
   head-capture change from commit 8b51c69.
6. **Dedupe is sacred** (FR-012): corpus jobs are ordinary compile_score
   manifests; if you find yourself adding a new job type or bypassing
   manifest-sha caching, you've left the design — stop.

## Environment (live right now)

- Coordinator running on the Pi (port 8323, log at `~/.conveyor/coordinator.log`);
  watchman node agent attached (16 cores, builder). If either died:
  restart commands in `tools/conveyor/README.md` and july7notes.md.
- ultralib cloned at `reference/repos/ultralib` @ `e24c8367` (git-ignored, as all
  reference repos are).
- Pinned toolkit `23185a79...` predates this feature — T012 replaces it.
- The working tree carries unrelated uncommitted changes from an earlier session
  (CLAUDE.md, src/game/game.c, tools/conveyor/{cli,pipeline/flags,pipeline/sweep}.py,
  july7notes.md, two test files, docs/WIKI.md). **Leave them out of your commits**
  — stage your files explicitly; never `git add -A`.

## When to stop and ask instead of pushing through

- A research.md decision doesn't survive contact with reality (e.g. objdump output
  format differs from the contract's parsing notes).
- T013 fails: the reloc-blind contract fixture and the live pipeline disagree.
- Anything requires touching rule-2 territory or a non-additive migration.
- The smoke test fails after the toolkit rebuild — do not proceed to T012's
  corpus run on a broken toolkit.

Report at the end: tasks completed, deviations + reasons, measured quickstart
numbers, and anything you flagged for review.
