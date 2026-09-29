# 011: Scorer hardening and cloud-lane fixes (implementation brief)

Written 2026-09-29 for an implementing agent (for example Codex) working from
this repository. Source: the cloud review on branch `claude/cloud-assessment`
(`CloudAssessment.md`), with every high-severity claim re-checked by the
maintainer session. The wrong-global variant of `sound_handles_clear` printing
`MATCH` and CI triggering only on `main` were both reproduced.

## Ground rules for the implementer

- Read [CLAUDE.md](../../CLAUDE.md) and [CloudHandoff.md](../../CloudHandoff.md) first.
- Do **not** edit generated or locked files: `asm/us/blob/*.s`,
  `src/blob/blob.ld`, `*.lock.json`, `us.sha1`, `src/blob/*.c`,
  `src/blob/groups/**`. None of these fixes needs to.
- Python 3.9+, standard library only in `tools/cloud/` (it must run on a bare VM).
- Each fix lands as its own commit with tests. Run
  `python3 -m pytest tests/conveyor/ -q` before committing.
- **Environments.** IDO needs x86-64 Linux with 4 KB pages. The maintainer's
  Pi cannot run it; use Rocky, watchman2 or a cloud VM. Fixes marked
  **[Pi]** need the coordinator database/layout that exist only on the
  maintainer's Pi (`~/.conveyor/`, `build/blob_layout.json`). Everything else
  is repo-only.
- If a fix turns out to need a design change not described here, stop and
  write up what you found instead of improvising.

## Priority order

| # | Fix | Where | Blocks |
|---|---|---|---|
| F1 | Commit a symbol table for relocation resolution | [Pi] | F2 |
| F2 | `score.py`: resolve relocations instead of masking them | repo-only (x86 for tests) | F4, F6 |
| F3 | Length check: compiled function must not exceed its target | repo-only + `blob_splice` | — |
| F4 | CI workflow on `master` that rescores and guards paths | repo-only | — |
| F5 | Pin the IDO download's sha256 | repo-only | F4 |
| F6 | PreToolUse hooks + target-hash check | repo-only | after F2 |
| F7 | Group mode: judge members only; stop leaking temp dirs | repo-only | — |
| F8 | Re-rank near misses by strict `score.py` word diffs | x86 box | — |
| F9 | Clean up `.claude/agents/` | repo-only | — |
| F10 | Merge `CloudAssessment.md` | git | — |
| F11 | Provenance field per match (maintainer decision) | [Pi] | decision |

---

## F1. Commit a symbol table: `asm/us/blob/symbols.json` [Pi]

**Why.** To check relocations, `score.py` must know every symbol's real
address. The splice path gets it from `blob_splice.image_symbols(document)`:
data and static symbols from `blob_tu.data_symbols()`, plus every game
function's `vaddr` from the layout. That needs `build/`, which is not in git.

**Do.**
- Add `python3 -m tools.conveyor.pipeline.blob_tu symbols` (or a function in
  `blob_splice`) that writes `asm/us/blob/symbols.json`:
  `{"generated_by": "...", "image_sha256": <layout image sha>, "symbols": {name: "0xXXXXXXXX", ...}}`,
  sorted keys, deterministic output.
- Use exactly `blob_splice.image_symbols(blob_layout.load())`. Do not re-derive
  it; cloud and splice must resolve identically.
- Regenerate it wherever `blob_tu generate` runs (same refresh step), so it
  can't drift.

**Accept.**
- Running it twice gives byte-identical output.
- It contains `sound_stop`, `D_80116DE4`, `func_80097694` and
  `resource_slot_clear` at their layout addresses.
- It is committed.

## F2. `tools/cloud/score.py`: resolve relocations instead of masking

**Current behaviour (wrong).** `compare()` masks each relocated word to its
opcode/register bits and never checks which symbol or addend the relocation
names. Wrong globals, wrong addends and wrong callees all print `MATCH`.

**Do.**
1. Extend the built-in ELF reader to read `.rel.text` fully:
   - `r_info >> 8` is the symbol index and `r_info & 0xff` the type;
   - symbol name comes from `.symtab`/`.strtab`, symbol type from
     `st_info & 0xf` (3 = `STT_SECTION`).
2. For each relocation in the compared slice, compute the expected
   resolved word, as `tools/conveyor/pipeline/blob_group.relocate` does:
   - `R_MIPS_26`: target = S + ((insn & 0x03FFFFFF) << 2); word = (insn & 0xFC000000) | ((target >> 2) & 0x03FFFFFF).
   - `R_MIPS_HI16` / `R_MIPS_LO16` (REL): pair each HI16 with the next LO16 of
     the same symbol. Value = S + (hi_imm << 16) + sign_extend(lo_imm). HI =
     ((value + 0x8000) >> 16) & 0xFFFF; LO = value & 0xFFFF.
   - Where S comes from:
     - a named global: from `asm/us/blob/symbols.json` (F1). If the name is
       missing, fall back to `func_XXXXXXXX` / `D_XXXXXXXX` spelled addresses
       (the `blob_splice.address_named` rule); otherwise the symbol is
       unresolved.
     - a function defined in the same object (a group member): its target
       address, from symbols.json.
3. Compare **full words** for resolved relocations.
4. For `STT_SECTION` relocations (local `.rodata`/`.data`: float constants,
   jump tables, strings) the target address cannot be known from the repo.
   Keep masking those, but count them.
5. Output `MATCH` only when there are 0 differing words, 0 unresolved
   symbols and 0 unverified relocations. Otherwise print, for example,
   `MATCH (2 section-relative relocations unverified: .rodata+0x10, .rodata+0x18)`
   or the differing words. The exit code is non-zero unless it is a plain
   `MATCH`, unless `--allow-unverified` is given.
6. Update the explanation in `CloudHandoff.md` §2 to match.

**Accept.** Add `tests/conveyor/test_cloud_score.py`, skipped when
`tools/cloud/ido/cc` is absent.
- Every single-function lock entry in `blob_matched.lock.json` (non-group)
  scores `MATCH` with its recorded `flagset`.
- Groups `src/blob/groups/resource_slot_clear` and `entity_flag_check` match
  on their members.
- These three mutants of `src/blob/sound_handles_clear.c` must **fail**:
  1. `entry = (s8 *) &D_80116DE4 + 0x40;`
  2. an `extern s8 D_WRONG;` used in place of `D_80116DE4`
     (unresolved → fail)
  3. `extern void some_other_fn(s32);` called instead of `sound_stop(h)`.

## F3. Length check: no silent truncation

**Why.** `score.py` compares only the first `len(target)` words, and
`blob_splice.link_function` does `return data[:size]`. A compiled function
longer than its target is truncated unseen. IDO pads `.text` to 16 bytes, so
some excess is expected, but only if it is zero.

**Do.**
- In `score.py`, take the compiled function's extent as the distance to the
  next function symbol or the end of `.text`. Any word past the target
  length that is not `0x00000000` fails, reported as `N extra words`.
- Apply the same rule in `blob_splice.link_function` (raise `BuildError`)
  and `blob_group.relocate` (raise `GroupError`).

**Accept.**
- A test with an object whose function has one extra non-zero word fails.
- Pure zero padding passes.
- All 125 current locked bodies still pass `blob_splice check` and the
  image gate. That's [Pi]; the maintainer runs `blob_rom rom`.

## F4. CI on `master`

**Why.** `.github/workflows/build.yml` triggers on `main`, a branch that
doesn't exist here, so it never runs. Its lint step is `|| echo`, so it
can't fail.

**Do.** Replace it with `.github/workflows/verify.yml` on `push`/`pull_request`
to `master`:
1. Install IDO via `tools/cloud/setup.sh` (with F5's sha check), and cache it.
2. Sanity: the two `CloudHandoff.md` MATCH checks.
3. Path guard: fail if the PR diff touches `asm/us/blob/**` (including
   `symbols.json`), `src/blob/blob.ld`,
   `*.lock.json`, `us.sha1`, or any path under `src/blob/` listed in
   `blob_matched.lock.json`.
   **Maintainer-approved amendment, 2026-09-29:** CI cannot verify F1
   regeneration without the private Pi inputs, so there is no PR exception.
   Maintainers regenerate/verify protected files on the coordinator and commit
   directly to `master`. Push CI still runs all verification checks; only the
   protected-path gate is PR-only. Group source files referenced by locked
   manifests are protected along with the manifests themselves.
4. Rescore every changed `cloud/matches/*.c` (flags from line 1
   `/* flags: ... */`) and every changed `cloud/work/ipa-groups/*/`.
5. `make check-matched` (the `matched.lock.json` drift check), and
   `python3 -m pytest tests/conveyor -q` for the repo-only tests.
6. Remove the `|| echo`.

**Accept.** A PR that edits one `.word` in `asm/us/blob/` fails. A PR adding a
correct `cloud/matches/<f>.c` passes. A PR with a wrong-global variant fails.

## F5. Pin the IDO download

In `tools/cloud/setup.sh`, before extracting:
`echo "ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506  ido.tgz" | sha256sum -c -`
(the published v1.2 `ido-5.3-recomp-linux.tar.gz`; the same pin OoT uses).
Fail loudly on mismatch.

**Accept.** A corrupted archive aborts setup. The normal run still succeeds.

## F6. Hooks and a target-hash check (after F2)

- Add `asm/us/blob/SHA256SUMS`, generated alongside F1, and have `score.py`
  verify each target file it reads against it. On mismatch, refuse to score.
- Add `.claude/settings.json` with PreToolUse hooks denying Edit/Write on
  `asm/us/blob/**`, `*.lock.json`, `us.sha1`, `src/blob/blob.ld` and
  `tools/cloud/score.py`. The pipeline scripts write these files through
  Python, not through Claude's tools, so they are unaffected.
- `setup.sh` runs `git config core.hooksPath .githooks`.

**Accept.** Editing a target `.word` makes `score.py` refuse to score. The
hooks deny an Edit to `asm/us/blob/blob_80086a50.s`.

## F7. Group-mode details

- `score.py group`: judge `members` only. Print `context` functions under a
  separate heading. The exit status depends on members alone.
- `compile_group`: use `tempfile.TemporaryDirectory()` so nothing leaks.

**Accept.** `src/blob/groups/entity_flag_check` exits 0: its member matches and
its context (`func_800988D8`) doesn't.

## F8. Re-rank the near misses (x86 box)

`cloud/work/near-miss/INDEX.md` ranks by the pipeline score, which ignores
stack offsets.
- Add `tools/cloud/rank_near_miss.py`. It runs `score.py fn` on each
  `cloud/work/near-miss/<f>/base.c` with the INDEX flags, then rewrites the
  table with a `strict words differing` column, sorted by it.
- Drop entries that already MATCH strictly (report them; they are free wins
  for the maintainer to splice).

**Accept.** INDEX.md has both columns and is sorted by the strict one.

## F9. Clean up `.claude/agents/`

- **`c-writer.md`:** remove the `register ... asm("t0")` and `asm volatile("")`
  advice. They are GCC-only, and IDO 5.3 rejects both.
- **`c-writer.md`, `build-runner.md`, `n64-decomp-expert.md`:** replace the
  GLOBAL_ASM / NON_MATCHING / asm-differ workflow with pointers to
  `.claude/skills/match-function/SKILL.md`, `tools/cloud/score.py` and
  `CloudHandoff.md` Lane B.
- **All agent files:** replace absolute `/home/cburnes/...` paths with
  repo-relative ones, and note that `reference/` is not in the repo.

**Accept.** `grep -r "/home/cburnes\|asm volatile\|asm(\"" .claude/agents` is empty.

## F10. Merge the assessment

Fast-forward or merge `origin/claude/cloud-assessment` (one file,
`CloudAssessment.md`) into `master`, then delete the branch.

## F11. Provenance per match (maintainer decision; [Pi])

Proposal: add `"origin": "m2c" | "m2c+permuter" | "hand" | "arcade" | "group-seed"`
to each `blob_matched.lock.json` entry. Backfill from commit history where it
can be decided, otherwise `"unknown"`. `blob_splice`/`blob_group` get a
`--origin` option. Coverage reports can then split by origin.
**Do not implement until the maintainer confirms.**

---

## Not in scope here (tracked elsewhere)

- Making the 32 non-building IPA group seeds compile. That's matching work:
  CloudHandoff Lane A, `cloud/work/ipa-groups/`. Remaining error classes are
  m2c struct-field typing, `spXX`/`unkspXX` stack artifacts, and functions
  m2c cannot seed.
- The IPA roadmap (group node job, group permuter):
  [specs/010-ipa-call-groups/planning.md](../010-ipa-call-groups/planning.md).
