# Cloud handoff: contributing to the Rush 2049 N64 decompilation

This file is for an agent joining with no other context, working only from
this GitHub repository (no ROM, no LAN build farm). It says what the project
is, what you can verify from here, and where your work helps most.

## 1. The project in one paragraph

We are writing C source that compiles, with SGI's IDO compiler, to the
byte-identical **San Francisco Rush 2049 (N64, US)** ROM. Game logic comes from
the arcade source (`reference/repos/rushtherock/` on the maintainer's machine;
**not** in this repo) and from decompiling the N64 assembly. The ROM has two parts:
- **static code** (boot, libultra, libc): 23/230 functions from C;
- the **game code**, a zlib-compressed image that inflates to
  `0x80086A50` (647,072 bytes): **125/912 functions from C** (11.7 KB).

Every function we "match" is spliced into the rebuilt image, and the full ROM
must still hash SHA-1 exact. Read [CLAUDE.md](CLAUDE.md) for the shared rules.

## 2. What you can and cannot do from the cloud

**You can:** compile C with the real compiler (IDO 5.3, static recompilation)
and compare any game function word for word against its exact retail bytes.
Those bytes are committed: every game-code function is a `.text.<name>` section
of `.word`s in [asm/us/blob/](asm/us/blob/). Treat those files as read-only
ground truth. They are generated, and the words are the ROM's.

**You cannot:** build or hash the ROM (it is not in the repo), run the
Conveyor pipeline (it needs the coordinator database on the maintainer's
LAN), or splice into the image. The maintainers do the final
splice + ROM SHA-1 check from your pull request. So your deliverable is
**C source plus `tools/cloud/score.py` output showing MATCH.**

### Setup (x86-64 Linux)

```bash
bash tools/cloud/setup.sh                 # downloads IDO 5.3 into tools/cloud/ido/ (git-ignored)
# optional, for disassembled diffs:  sudo apt-get install -y binutils-mips-linux-gnu

# one function compiled alone (the -O2 pipeline):
python3 tools/cloud/score.py fn src/blob/sound_handles_clear.c sound_handles_clear     # -> MATCH
# an IDO -O3 interprocedural call group:
python3 tools/cloud/score.py group src/blob/groups/resource_slot_clear                   # -> 3x MATCH
```

Setup also selects `.githooks` as this checkout's Git hooks directory, enabling
the existing matched-source commit check.

Submodules (m2c decompiler, decomp-permuter) are only needed for tooling work:
`git submodule update --init`. m2c needs our local patches applied, and
`tools/conveyor/pipeline/autodecomp.ensure_m2c_patched()` applies them.

Run those two first. If they do not print MATCH, stop and investigate the
setup and reported differences before trusting any other result. `score.py`
resolves call targets and `%hi`/`%lo` symbol addresses and addends using the
committed `asm/us/blob/symbols.json` table (with address-spelled `func_XXXXXXXX`
and `D_XXXXXXXX` names as a fallback). Calls within a group resolve to the
callee's target address, including calls encoded as offsets into `.text`.
Resolved instructions must match in full, **including stack offsets and frame
sizes**. Unknown symbols, unsupported relocations, and unpaired HI16 records
fail verification.

The compiled function ends at the next function symbol or the end of `.text`.
Words beyond the target length must be zero padding; any nonzero excess
reports `N extra words` and fails, including with `--allow-unverified`.
A shorter function cannot borrow instructions from its neighbor. The splice
paths apply the same length rule to the bodies entering the image.

Local data-section relocations (`.rodata`, `.data`, and similar sections)
cannot be fully checked from the repository. Only their relocation fields
are masked, and the output lists the unverified references, for example
`MATCH (2 section-relative relocations unverified: ...)`. This is not a plain
`MATCH` and exits nonzero by default. `--allow-unverified` permits that partial
comparison in either mode, but never permits unresolved symbols, relocation
errors, or differing words. The image and ROM hash gates remain required
before splicing a contribution.

Before using target words or symbol addresses, the scorer verifies their exact
file bytes against `asm/us/blob/SHA256SUMS`. A changed, missing, or unlisted
region fails verification; `--allow-unverified` cannot bypass this check.
The coordinator refreshes the manifest with `blob_tu symbols` and
`blob_tu generate`. Cloud contributors should restore altered protected files
from the trusted checkout instead of regenerating their hashes.

The project's `.claude/settings.json` adds a PreToolUse hook that denies direct
Edit/Write calls on `asm/us/blob/**`, `*.lock.json`, `us.sha1`, `src/blob/blob.ld`,
and `tools/cloud/score.py`. It leaves the maintainer's generation scripts
available. This hook covers Claude's Edit/Write tools; the manifest and F4 PR
checks provide separate verification of the protected data.

## 3. Where to contribute, in priority order

### Lane A: IDO -O3 interprocedural call groups (highest value, newest)

About a third of the game (354 of 912 functions) was shaped by **IDO -O3
interprocedural register allocation (IPA)**:
- static callees take parameters in non-ABI registers (`$t0`, `$s0`, `$f16` ...);
- callers keep values in caller-save registers across calls to callees
  known not to clobber them;
- leaf functions get their temporaries squeezed into `$t6`-`$t9`.

These can only be matched by compiling the function **together with its
callers/callees** as one whole program. We proved the method and put the first
two groups in the ROM. Background (read in this order):
1. [specs/010-ipa-call-groups/planning.md](specs/010-ipa-call-groups/planning.md): problem, phases, status;
2. [research/s1-poc.md](specs/010-ipa-call-groups/research/s1-poc.md) and
   [research/s2-s5-spikes.md](specs/010-ipa-call-groups/research/s2-s5-spikes.md): how the original was built;
3. worked examples: [src/blob/groups/resource_slot_clear/](src/blob/groups/resource_slot_clear/)
   (hand-written, 3 members) and [src/blob/groups/entity_flag_check/](src/blob/groups/entity_flag_check/)
   (generated; one member plus one `context` function).

**Work items:** [cloud/work/ipa-groups/](cloud/work/ipa-groups/INDEX.md): 40
machine-generated groups, each `group.json` + `group.c` + `STATUS.md`. Most
**do not compile yet** (35/40, mostly IDO type/prototype errors from the
generated context). Tasks, in order:
1. Make a group compile under IDO (fix prototypes, casts, `M2C_FIELD`
   types). `score.py group` reports per-function word diffs once it builds.
2. Get members to MATCH. Rules of the game:
   - `group.json`: `members` are spliced when they match; `context` are
     compiled in the unit but not spliced. Move a stubborn member to
     `context` so the rest can land. The scorer reports context separately;
     only members determine its exit status. `keep` is the `uld -kp` list of
     functions that stay externally visible: roots and the `__standin_*` callers.
   - Parameters named `ipa_t0`, `ipa_s0`, ... are the IPA register
     parameters. Write them as ordinary C parameters; `-O3` reassigns the
     registers itself.
   - A `__standin_X` function exists only to keep X from being inlined; its
     body does not matter.
   - Some single-member groups are suspicious (a callee whose callers were not
     found). Flag them rather than force them.

### Lane B: single-function near misses (steady, parallel)

[cloud/work/near-miss/](cloud/work/near-miss/INDEX.md): 103 candidates ranked by
strict word differences, retaining their historical pipeline scores (5-500).
All 104 original candidates were rescored at their recorded `-O2`/`-O1` flags;
`func_800C54F0` already matches and is reported separately (it is already locked).
The ranking includes nonzero excess instructions and retains any verification
warnings. Rebuild it on x86 with `python3 tools/cloud/rank_near_miss.py`. Each has
`base.c`, a whole translation unit; edit only the target function (and
prototypes it needs).
`python3 tools/cloud/score.py fn cloud/work/near-miss/<f>/base.c <f> --flags "<flags from INDEX>"`.

What worked on the 32 hand matches of 2026-09-28/29 (details in the commit log
from `2f2f5bc` onward):
- **Pointer strides:** m2c byte offsets through typed pointers (`s32 *p; p += 4`
  steps 16 bytes). Use `s8 *` arithmetic or divide by the element size.
- **m2c spill locals** (`void *sp1C; sp1C = x;`) move the frame. Drop them, or
  inline the value.
- **Declaration order** decides stack spill slots; permute the declarations.
- **Swapped `lui/addiu` pairs** before a loop: wrap the setup as
  `do { p = &D; do { ... } while (...); } while (0);`.
- **Unused parameters** the target does not spill: reference them in dead code
  (`if (arg0) {}`). A `u8`/`s16` parameter shows up as `andi 0xff` / `sll`+`sra`.
- **Varargs** wrappers: `f(a, b, ...)` with `(s8 *) &b + 4` as the `va_list`.
- **Two-load float adds** in the wrong order: swap the operands.
- **Ternaries and plain `for` loops** often beat m2c's `goto` shapes.
- A function whose diff shows a register it never sets (`$t0`-`$t3`, `$s*`
  on entry) belongs to Lane A, not here.

### Lane C: tooling (useful, needs care)

Only take these with tests (`python3 -m pytest tests/conveyor/...`; most tests
run without the LAN):
- **Group permuter:** mutate one member of a group while scoring the whole
  group with `score.py`-style comparison.
- **Unregistered tiny functions:** several 3-4 instruction setters
  (`lui $at; jr $ra; sw $a0,%lo(D)($at)`) sit in small opaque runs between
  functions and were never registered (see `targets.stranded_head` and commit
  89a6a12). A finder + report is welcome.
- **IDO -O3 emission order:** the rule that orders functions in a whole-program
  object. Splicing does not need it; faithful source layout will.

## 4. How to hand work back

- Work on a branch; open a PR against `master`. One PR per group or per
  batch of single functions is fine.
- **Single function:** put the matching whole-TU source at
  `cloud/matches/<function>.c`, with the flags in a first-line comment
  (`/* flags: -g0 -O2 -mips2 -G 0 -non_shared */`).
- **Group:** update its directory under `cloud/work/ipa-groups/<id>/`
  (`group.c`, `group.json` members/context, `STATUS.md`).
- Paste the `score.py` output (MATCH lines) into the PR description.
  Maintainers splice it (`blob_splice` / `blob_group`) behind the image gate and
  the ROM SHA-1, then move it into `src/blob/`.

CI runs on PRs and pushes to `master`. It strictly rescores changed single
submissions and groups, checks the static locks, and runs repository tests.
PRs cannot change `asm/us/blob/**` (including `symbols.json`), `*.lock.json`,
`us.sha1`, `src/blob/blob.ld`, or locked blob sources and group inputs.
Maintainers regenerate and verify protected files on the coordinator and
commit them directly to `master`; push CI still runs the verification checks.
The symbol-table exception is deliberately closed because CI lacks the private
inputs needed to verify regeneration.

## 5. Please do not

- Commit ROM data, extracted images, or compiled objects (`build/` is ignored;
  keep it that way).
- Edit generated files: `asm/us/blob/*.s`, `src/blob/blob.ld`,
  `blob_matched.lock.json`, `matched.lock.json`, anything under `build/`.
- Edit locked sources in `src/blob/*.c` or `src/blob/groups/*` that already
  match. A change there breaks the splice lock.
- Change `tools/m2c_patches/` or the Conveyor pipeline without tests. These run
  unattended on the maintainer's farm.
- Claim a match from a pipeline "score 0" alone. Stack offsets are masked
  there; `score.py` is strict.
- Merge the static and game-code coverage numbers; report them separately.

## 6. Map of the repo (for orientation)

| Path | What |
|---|---|
| [CLAUDE.md](CLAUDE.md) | shared rules and routing |
| [docs/README.md](docs/README.md) | documentation index |
| [asm/us/blob/](asm/us/blob/) | game-code image as per-function `.word` sections (ground truth) |
| [src/blob/](src/blob/) | matched game functions (one TU each) and `groups/` |
| [tools/conveyor/](tools/conveyor/README.md) | the matching pipeline (runs on the maintainer's LAN) |
| [tools/m2c_patches/](tools/m2c_patches/) | local fixes to m2c (decompiler), applied automatically |
| [specs/](specs/) | feature specs 001-010 with research and decisions |
| [cloud/work/](cloud/work/) | work items prepared for you |
| [tools/cloud/](tools/cloud/) | setup + scorer that need only this repo |
