# Cloud assessment: the matching loop against published practice

Written 2026-09-29 from a cloud session with only this repository (commit `5eebdbb`), IDO 5.3 from `tools/cloud/setup.sh`, and no ROM or LAN. The comparison baseline is the research collection in `cabi24/decomp-knowledge` (`synthesis/` and `research/agent-templates-deep/`), which studied about a dozen agent-driven decomp loops (Snowboard Kids 1/2 with `nigel`, Conker, tenchu, psp-autodecomp, gameDecomp and others).

**Status (rebased onto `cce96df`).** The fixes are tracked in [specs/011-scorer-hardening/FIX-PLAN.md](specs/011-scorer-hardening/FIX-PLAN.md) (F1-F11), which supersedes the "Suggested order" section below. F1 (`asm/us/blob/symbols.json`) is done and F2 (relocation resolution) is in progress. I re-ran every test in this file against the corrected ground truth from `4b44a75`; none of the functions involved (`sound_handles_clear`, the `resource_slot_clear` group) was among the 21 whose starts moved, and all results are unchanged. The game population is now 890 functions, not the 912 mentioned below.

Evidence labels: **tested** means I ran it here and the command is given. **code-observed** means I read the code but did not run the path. Nothing here touched the ROM, so none of this says anything about the maintainers' splice + SHA-1 path beyond what the code shows.

## Summary

The cartridge-side gates are sound: the image-hash gate, the fixed `make verify`, the splice lock with source hashes, and the drilled rollback. The ROM cannot be corrupted by the problems below.

The weak point is the **cloud scorer and everything that trusts it**. `tools/cloud/score.py` prints `MATCH` for C that is semantically wrong, and nothing in the repository stops an agent from editing the ground truth it compares against. That matters most if an agent loop uses `score.py` as its reward signal. The research's most repeated failure is agents learning to satisfy a verifier that doesn't verify. Masked relocation fields are exactly the kind of free space an optimizer finds.

| # | Severity | Finding | Evidence |
|---|---|---|---|
| 1 | High | `score.py` MATCHes wrong globals, wrong offsets and wrong callees | tested |
| 2 | High | CI never runs (wrong branch), and has no matching check | code-observed |
| 3 | Medium-high | Ground truth and locks are only protected by prose | code-observed |
| 4 | Medium | Compiled functions longer than the target are silently truncated | code-observed |
| 5 | Medium | Agent definitions give advice IDO rejects, plus stale paths and workflow | tested |
| 6 | Low-medium | `setup.sh` does not verify the IDO download | tested |
| 7 | Low | Group mode fails on `context` mismatches; temp dirs leak | code-observed |
| 8 | Low | Near-miss ranking uses a lenient score | code-observed |
| 9 | Decision | Provenance labeling for arcade-derived matches | policy |

## 1. `score.py` reports MATCH for semantically wrong C (high, tested)

`compare()` masks every word that carries a relocation down to its opcode and register bits (`MASKS` for `R_MIPS_26`, `R_MIPS_HI16`, `R_MIPS_LO16`). It never checks **which symbol** the relocation names or **what addend** it carries. So any mistake that lives only in the relocated fields passes.

I took the locked, matching `src/blob/sound_handles_clear.c` and made three wrong variants. All three print `MATCH`:

| Variant | Change | `score.py` |
|---|---|---|
| Wrong addend | `entry = (s8 *) &D_80116DE4 + 0x40;` (loop starts two entries late) | MATCH |
| Wrong global | `extern s8 D_WRONG;` and `entry = (s8 *) &D_WRONG;` | MATCH |
| Wrong callee | `extern void some_other_fn(s32);` and `some_other_fn(h);` in place of `sound_stop(h);` | MATCH |

Reproduce: copy the file, apply one change, then run `python3 tools/cloud/score.py fn <copy> sound_handles_clear`.

The same blind spot covers float constants and jump tables loaded through `%hi/%lo(.rodata+off)`. A wrong literal compiles to the same instruction words with a different addend.

**Why it matters.**

- `CloudHandoff.md` calls a `score.py` MATCH "strong evidence" and asks contributors to paste it into PRs. A PR can therefore claim a match that the maintainers' splice will reject. `link_function` resolves relocations against real addresses, so the image gate catches these.
- The larger risk is loop design. An agent rewarded by `score.py` is free to get data references wrong, and that is the easiest part to get wrong from m2c output.

**Fix.** Resolve relocations instead of masking them.

- For each `.rel.text` entry, read the symbol name and the in-place addend. Compute the expected word from a name→address table, and compare the full word.
- The table already exists in pieces: `blob_tu.data_symbols()` (static symbols plus `disasm.symbol_table()`) and the function addresses in `asm/us/blob/`. `blob_splice.address_named()` handles `func_XXXXXXXX`/`D_XXXXXXXX`.
- `build/blob_layout.json` is not in the repo, so commit a small generated `asm/us/blob/symbols.json` for cloud use. That way the cloud scorer and the splice path resolve the same way.
- Until that lands, have `score.py` print `MATCH (N relocations unverified)` and list `symbol+addend` for each masked word, so a reviewer can check them by eye.

## 2. CI never runs and does not check matching (high, code-observed)

- `.github/workflows/build.yml` triggers on `push`/`pull_request` to `main`. The default branch is `master` (`git ls-remote --symref origin HEAD`), so the workflow never runs on PRs.
- If it did run, it only does `make help` and `make lint || echo ...`. The `|| echo` swallows failure, the same pattern the 2026-07-11 drill found in `make verify`.
- Nothing re-runs a contributor's `score.py` claim.

**Fix.** Add a PR workflow on `master` that needs no ROM. `setup.sh` plus `score.py` finishes in under a second per function here.

1. Pin and verify IDO (see 6).
2. Run the two sanity checks from `CloudHandoff.md` and fail if they do not MATCH.
3. Re-score every changed `cloud/matches/*.c` (flags from the first line) and every changed `cloud/work/ipa-groups/*/`.
4. Fail if the diff touches generated or locked paths: `asm/us/blob/`, `src/blob/blob.ld`, `*.lock.json`, `us.sha1`, locked `src/blob/**`.
5. Run `python3 -m tools.conveyor.pipeline.lock check`.

Fix or drop the `|| echo` on lint.

## 3. Ground truth and locks are protected only by prose (medium-high, code-observed)

- `score.py` reads its targets from the working tree (`asm/us/blob/*.s`). An agent that edits a target's `.word`s to agree with its output gets MATCH. `CloudHandoff.md` says to treat these files as read-only, but nothing enforces it.
- The pre-commit lock check (`.githooks/pre-commit`) only runs if `core.hooksPath` is set, and a fresh clone does not set it. It is also documented as bypassable with `--no-verify`.
- There is no `.claude/settings.json`, so no PreToolUse hooks exist.

The loops the research found most reliable (Snowboard Kids 2 / `nigel`) block these edits mechanically. They use nine PreToolUse hooks covering asm, the SHA-1 file, raw `make` and permuter runs without a timeout, plus a verification step outside the agent's reach.

**Fix.**

- Have `score.py` check each target file against a committed hash manifest, or read targets via `git show origin/master:asm/us/blob/<file>` so local edits cannot help.
- Add a `.claude/settings.json` with PreToolUse hooks that deny edits to `asm/us/blob/**`, `*.lock.json`, `us.sha1`, `src/blob/blob.ld` and `tools/cloud/score.py`.
- Make the CI guard in 2 the backstop.
- Have `setup.sh` run `git config core.hooksPath .githooks`.

## 4. Longer compiled functions are silently truncated (medium, code-observed)

- `score.py` compares only the first `len(target)` words of the compiled function. It never checks the compiled symbol's size.
- `blob_splice.link_function` does `return data[:size]`. It rejects a *shorter* body but drops any excess from a longer one without looking at it.

IDO pads `.text` to 16 bytes, which is why the truncation exists. But the dropped bytes are never checked to be padding. The ROM stays correct, because only the retail-length prefix is spliced. The source, though, can contain code the ROM does not. That breaks later, when whole-object layout or IDO `-O3` emission order (Lane C) is needed, and it quietly weakens "this C compiles to that function".

**Fix.** In both places, assert that every word past the target extent up to the next symbol or section end is zero. Otherwise report `N extra words`.

## 5. Agent definitions give advice IDO rejects (medium, tested)

`.claude/agents/c-writer.md` recommends `register s32 temp asm("t0");` and `asm volatile("");` as matching techniques. Both are GCC extensions. IDO 5.3 rejects both with `cfe: Error: ... Syntax Error`, and no object is produced. Reproduce with `tools/cloud/ido/cc -c -g0 -O2 -mips2 -G 0 -non_shared` on a one-line function using either construct. Outside IDO, other agent loops treat register pinning as gaming the verifier.

The agent directory is also out of date relative to the current workflow:

- **`c-writer.md`, `build-runner.md`, `n64-decomp-expert.md`** describe `GLOBAL_ASM` / `NON_MATCHING` / asm-differ. The game-code path is `score.py` / Conveyor / `blob_splice`.
- **Absolute paths:** `c-writer.md`, `arcade-comparator.md`, `asm-analyzer.md` and `n64-decomp-expert.md` use `/home/cburnes/projects/...`, which do not exist in a cloud or other checkout.

`CLAUDE.md` routes "specialized agent definitions" to this directory, so a cloud agent can reasonably follow them.

**Fix.** Delete the register/`asm volatile` block. Replace the workflow sections with a pointer to the match-function skill and `score.py`. Replace absolute paths with repo-relative ones and note that `reference/` is not in the repo. The Lane B list in `CloudHandoff.md` (pointer strides, spill locals, declaration order, the `do { } while (0)` wrap, dead-code parameter references) is the better technique list.

## 6. `setup.sh` does not verify the IDO download (low-medium, tested)

`setup.sh` downloads `ido-5.3-recomp-linux.tar.gz` from the `v1.2` release and extracts it unverified. Today's download hashes to:

`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`

That equals the pin OoT commits (`tools/compiler_archives/signatures/`), recorded in decomp-knowledge `research/docs-and-env/NOTES.md`. Add `echo "<sha>  ido.tgz" | sha256sum -c -` before `tar`. OoT also bakes archives into its CI image because GitHub release downloads occasionally fail.

## 7. Group-mode details (low, code-observed)

- `score.py group` checks `members + context`, and exit status needs all of them to MATCH. `CloudHandoff.md` says to move a stubborn member to `context` "so the rest can land", but the group still reports failure. Report context separately and exit on members only, or say in the handoff that context mismatches are expected.
- `compile_group` uses `tempfile.mkdtemp` and never removes it, so each call leaks a directory. That matters in a long agent loop.

## 8. Near-miss ranking uses a lenient score (low, code-observed)

`cloud/work/near-miss/INDEX.md` ranks by pipeline score, "stack offsets ignored", which is relocation-blind by `CLAUDE.md`'s own rule. `score.py` is strict. A function listed at 5 can be far from a strict match, and a stack-frame difference is often the expensive part. Re-rank by `score.py` word-diff count, or show both, so agents pick truly near work first. The research's loops that ordered work well used the same metric for queueing and for verification.

## 9. Decision: provenance labeling for arcade-derived matches

The constitution makes the arcade source the "Rosetta Stone" and says to "ALWAYS search for equivalent arcade code first". That is a real advantage. The research flags it as something to decide deliberately: gameDecomp reports matches "recovered from target source" as a separate tier from matches derived from the binary. Recording, per match, whether the logic came from arcade source, m2c or hand work would keep coverage claims honest and make the book-quality history easier to write later. This is a choice for the maintainer, not a bug.

## What already matches good practice

These are worth keeping as they are:

- **Two independent gates on the cartridge side:** image sha256 per splice, then built-ROM SHA-1. Failure restores the tree.
- **The `make verify` fix and the rollback drill** (2026-07-11), with a documented instruction to re-run the drill when the gate changes. The research lists this exact failure, since fixed.
- **"Relocation-blind zero is a lead, not a match"** as a written rule, and separate static/game denominators.
- **Splice locks keyed by source sha256** (`blob_matched.lock.json`) and a `matched.lock.json` drift check.
- **A wall-clock budget on permuter jobs** (`budget.wall_seconds` 14400), a 120 s timeout on m2c runs, and a flagset per function from sweep evidence.
- **A cloud lane with prepared work items and a near-miss handoff,** which is the same shape as the near-miss files in the best agent loops.

## Suggested order

1. Relocation resolution in `score.py` (1) and the length check (4). The scorer is the reward signal, so fix it before any unattended loop runs.
2. The CI workflow on `master` with the path guard (2, 3).
3. PreToolUse hooks and the target-hash check (3).
4. The agent directory cleanup (5) and the sha256 pin (6).
5. The group-mode and ranking tweaks (7, 8) and the provenance decision (9) when convenient.

I can implement 1, 4, 6 and 7 from the cloud and test them against the existing matches. Items 2 and 3 are repository policy, so they are yours to approve.
