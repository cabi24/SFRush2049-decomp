# Cloud handoff: contributing to the Rush 2049 N64 decompilation

This file is for an agent joining with no other context, working only from this GitHub repository
(no ROM, no LAN build farm). It says what the project is, what you can verify from here, where your
work lands, and how to hand it back so it lands quickly. Rewritten 2026-10-11; the companion plan is
[docs/plans/2026-10-11-astra-cloud-plan.md](docs/plans/2026-10-11-astra-cloud-plan.md). Claims live in
[cloud/work/CLAIMS.md](cloud/work/CLAIMS.md): add a line before you start on a function.

## 1. The project in one paragraph

We are writing C source that compiles, with SGI's IDO compiler, to the byte-identical **San Francisco
Rush 2049 (N64, US)** ROM. Game logic comes from the arcade source (`reference/repos/rushtherock/` on the
maintainer's machine; **not** in this repo) and from decompiling the N64 assembly. The cartridge holds
four code populations, reported separately and never merged:

| Population | Targets in this repo | Compiler | 2026-10-11 |
|---|---|---|---|
| Static boot/library code | `asm/us/boot_tail/` (the unmatched Rush boot tail) | IDO 5.3 (libultra), **IDO 7.1** (Rush boot tail, `rom_compilers.json`) | 528/668 functions, 94,836 / 160,720 B (59.01%) |
| Game code (zlib stream at ROM `0xB0CB10`, inflates to `0x80086A50`) | `asm/us/blob/` | IDO 5.3, `-O2`/`-O3 -mips2` | 972/1216, 310,380 / 647,072 B (47.97%) |
| Runtime image A (stream `0xB5C534`, loads at `0x8038A400`) | `asm/us/ovl_a/` | IDO 5.3 | 36/193 functions, 12,856 / 152,952 B; lands via `ovl_splice` + `ovl_rom` (2026-10-11) |
| Runtime image B (stream `0xB6FEC4`, loads at `0x8038A400`) | `asm/us/ovl_b/` | IDO 5.3 | 8/49 functions, 1,640 / 38,708 B; same |

Every function we "match" is spliced into its rebuilt image, and the full ROM must still hash SHA-1
exact. Read [CLAUDE.md](CLAUDE.md) for the shared rules. The live state of the game-code frontier
(coverage, the near-miss table with traced causes) is
[docs/plans/2026-10-06-frontier-handoff.md](docs/plans/2026-10-06-frontier-handoff.md) §0; the matching
method that closed most of the recent functions is
[cloud/work/frontier/WAVE_BRIEF.md](cloud/work/frontier/WAVE_BRIEF.md) ("Wave 15", "Wave 16", "Wave 17").

## 2. What you can and cannot do from the cloud

**You can:** compile C with the real compilers and compare any function word for word against its
exact retail bytes. The target words are committed (`.text.<name>` sections of `.word`s plus
`extents.json`, `symbols.json`, `SHA256SUMS` in each population directory). Treat them as read-only
ground truth. Pick the population with `--targets`:

```bash
bash tools/cloud/setup.sh      # IDO 5.3 -> tools/cloud/ido, IDO 7.1 -> tools/cloud/ido71 (git-ignored)
# game code (default targets):
python3 tools/cloud/score.py fn src/blob/sound_handles_clear.c sound_handles_clear          # -> MATCH
python3 tools/cloud/score.py group src/blob/groups/resource_slot_clear                        # -> 3x MATCH
# runtime images:
python3 tools/cloud/score.py fn cloud/matches/ovl_b/func_8038D1A8.c func_8038D1A8 --targets asm/us/ovl_b
# Rush boot tail (IDO 7.1 segments 0x11050..0x26ab0; libultra segments stay 5.3):
IDO_DIR=tools/cloud/ido71 python3 tools/cloud/score.py fn FILE func_80014CF4 --targets asm/us/boot_tail
```

Run the first two before anything else; if they do not print MATCH, fix the setup first.

**You cannot:** build or hash the ROM (not in the repo), run the Conveyor pipeline (needs the
coordinator database on the maintainer's LAN), lock static functions (`lock add` goes through the pool),
or splice. The coordinator does splice, lock, promote and the ROM SHA-1 check from your PR, usually
within a day of a ready PR with a non-empty claims list, and comments on the PR with what landed or
which gate refused and why.

**Scorer rules you will hit.** Resolved instructions must match in full, including stack offsets and
frame sizes. The compiled function ends at the next function symbol or the end of `.text`; words
beyond the target length must be zero padding, otherwise `N extra words` fails (also with
`--allow-unverified`). Local data-section relocations (`.rodata`, `.data`, `.bss`) are verified against
the image where the words permit (`own .rodata verified at …`); what cannot be verified is listed and
exits nonzero unless `--allow-unverified`, which never permits differing words or unresolved symbols.
Target words and symbols are checked against `SHA256SUMS` before use; never regenerate those files.
A whole-program-unit match (IPA, `-O3`) is proven with `score.py group` on a group directory, not with
`fn`; a function that is EQUAL only inside a group does **not** go in `cloud/matches/`.

**Per-TU compilers.** Static ROM TUs are compiled by the compiler `rom_compilers.json` pins for their
segment. Anything that rebuilds a ROM TU (a research packet, a test) must take its `cc` from
`tools.conveyor.compilers.tu_cc(tu, default_cc)`, never from `score.ido("cc")` directly.

Submodules (m2c, decomp-permuter) are only needed for tooling work: `git submodule update --init`.

## 3. Where to contribute, in priority order

### Lane 1: runtime images A and B (primary; 191 KB nobody else is working)

Targets: `asm/us/ovl_a`, `asm/us/ovl_b`; existing matches: `cloud/matches/ovl_{a,b}/` (25 singles) and
group matches in `cloud/work/frontier/dot_runtime_a_*`. The layout/splice/ROM path for these images exists
since 2026-10-11 (`ovl_layout`, `ovl_splice`, `ovl_rom`; lock `ovl_matched.lock.json`, sources `src/ovl_{a,b}/`):
the coordinator splices a ready PR like any game match and the ROM SHA-1 gate covers it. Deliver exactly as for game code (§4): a single as
`cloud/matches/ovl_a/NAME.c` or `cloud/matches/ovl_b/NAME.c` with the flags on line 1, a group as a
group directory with `claims`. Frame-only residuals (a few words, frame off by 8–24 B) are almost always
declared-local sets, a reused variable or float-literal spelling: see the Wave 16/17 lessons.

### Lane 2: game-code near-misses with a traced cause

The table in the frontier handoff §0 lists each open near-miss, its gap, the traced cause and the best
next lever, with the best draft's path under `cloud/work/frontier/w*/`. Take one by adding a line to
`CLAIMS.md`. func_800D816C (with func_800D91A0 at 8 rows, closing 6.4 KB) is the best-value open item
and starts from Astra's own draft. `python3 -m tools.conveyor.pipeline.frontier show NAME` output for
each is committed in the lane dirs when the DB is not available to you.

Method: **trace, don't sweep.** Run the workbench `diagnose` (`python3 tools/workbench.py diagnose
TARGET.o CANDIDATE.o --function FN`, see `docs/external/README.md`), then the forced-colour oracle and the
colouring trace, then write the one source lever. The instrumented compilers used this week
(`cloud/work/frontier/w16d/tools/patch_spl.py`, `w16b/tools/splitpatch.py`, `w17f/tools/prio_patch.py`,
`w17i/boostpatch.py`, `w17a/tools/patch_lbb.py`) patch the ido-static-recomp sources, which build on x86;
each tool's RESULTS.md gives the build steps. Mass-variant sweeps (hundreds of proposals) plateaued here
in wave 14 and are not wanted.

### Lane 3: Rush boot tail under IDO 7.1 (static)

`cloud/work/frontier/w17g/inv_table.md` lists the unmatched boot-tail `func_*` with sizes (w18c took 39 of them on 2026-10-11; its RESULTS.md lists the ~20 it could not close, with causes); they are
Rush engine code (no public source) compiled with IDO 7.1 (`-g0 -O2 -mips2 -G 0 -non_shared`, flags per
segment in `docs/COMPILER_SETTINGS.md`). Score with `IDO_DIR=tools/cloud/ido71 … --targets asm/us/boot_tail`.
Deliver as `cloud/matches/boot_tail/NAME.c`; the coordinator locks (`lock add --toolkit …`) and promotes
(`promote_batch.py`). The `w17g/lockonly/` nine landed with w18c.

### Lane 4: tooling (only with tests)

`python3 -m pytest tests/conveyor tests/cloud -m "not node_required"` must pass without the LAN. Good
items: a `--compiler` option for `score.py` (hook-protected file: patch in a PR, the coordinator applies),
the group permuter, the stranded tiny-setter finder (`targets.stranded_head`).

## 4. How to hand work back (packet protocol v2)

- **Deliverable first.** A match is a source file or a group directory the coordinator can install
  with the existing tools, plus the exact `score.py` output in the PR description. A README,
  semantics tests and a `verify.py` are supporting material, not the deliverable.
  - Single: `cloud/matches/<population>/NAME.c` (`cloud/matches/NAME.c` for game code), first line
    `/* flags: -g0 -O2 -mips2 -G 0 -non_shared */` (or the -O3 group flags), header with real semantics,
    arcade ancestor if proven, and every shaping device disclosed (compiled-out reads, unused locals that
    are the exact frame residual with the frame arithmetic, helper identity hypotheses). No false
    prototypes (a void callee declared returning s32), no pad arrays, no stand-ins pinned as matches.
  - Group: `group.json` (`members` = every function the group defines that is or becomes locked,
    including stubs given bodies; `claims` = the strict matches; `context` = compiled but not spliced;
    `keep`; `flags`), the `.c` files, `score.py group DIR` output **without** `--claims` showing MATCH for
    every member and no "extra words". A group superseding a locked one keeps its name and every old
    member byte-identical.
- **Receipts pin inputs, not tools.** Pin your sources, the target rows you used, and your own
  `verify.py`. Do not pin `tools/cloud/score.py`, anything under `tools/conveyor/`, lock files, the
  `Makefile`, or `asm/**/*.s` text (each splice adds a provenance line to the asm). Tool-hash pins have
  cost a receipt rebind on nearly every tooling change.
- **No `git show` of commits that are not ancestors of `master`**; replays must run on current master.
- **One test per packet, under 5 s without IDO and under 30 s with it**, using the shared conftest
  skips (`tests/conftest.py`). Larger experiments stay in the packet directory without a test.
- **PR shape:** one function or one group per PR, under ~30 files. Open it **ready** when `claims` is
  non-empty; keep research with empty claims as a draft (merged in weekly batches, never blocking).
  Branch names `dot/*`, `work/*`, `research/*` are fine.
- Paste the scorer output in the PR; say which population; add the `CLAIMS.md` line in the same PR.

CI runs on PRs and pushes to `master`: it strictly rescores changed `cloud/matches/**` against their
population (`--targets`), checks the static locks and runs the repository tests with IDO 5.3 and 7.1
installed. PRs cannot change `asm/us/**`, `*.lock.json`, `us.sha1`, `src/blob/blob.ld`, or locked
sources and group inputs; the coordinator regenerates protected files on the LAN.

## 5. Please do not

- Commit ROM data, extracted images, or compiled objects (`build/` is ignored; keep it that way).
- Edit generated or locked files: `asm/us/**`, `src/blob/blob.ld`, `*.lock.json`, `src/blob/*.c` and
  `src/blob/groups/*` that already match, `build/`.
- Change `tools/m2c_patches/` or the Conveyor pipeline without tests; they run unattended on the farm.
- Claim a match from a pipeline "score 0" or a relocation-blind score alone; `score.py` is strict.
- Merge population coverage numbers; report each separately.
- Work on a function someone else has claimed in `cloud/work/CLAIMS.md` without saying so there.

## 6. Map of the repo

| Path | What |
|---|---|
| [CLAUDE.md](CLAUDE.md) | shared rules and routing |
| [docs/README.md](docs/README.md) | documentation index |
| [docs/plans/2026-10-06-frontier-handoff.md](docs/plans/2026-10-06-frontier-handoff.md) | live frontier state, near-miss table, owner rules |
| [cloud/work/frontier/WAVE_BRIEF.md](cloud/work/frontier/WAVE_BRIEF.md) | the matching method and this month's lessons |
| [cloud/work/CLAIMS.md](cloud/work/CLAIMS.md) | who is working on what |
| `asm/us/blob/`, `asm/us/ovl_a/`, `asm/us/ovl_b/`, `asm/us/boot_tail/` | target words per population (ground truth) |
| [src/blob/](src/blob/), `src/rom/` | matched game functions (one TU each, plus `groups/`); static ROM TUs |
| [cloud/matches/](cloud/matches/) | delivered single-function matches, by population |
| [cloud/work/frontier/](cloud/work/frontier/) | lane dirs: RESULTS.md per lane, drafts, diagnostic tools |
| [tools/cloud/](tools/cloud/) | setup + scorer that need only this repo |
| [tools/conveyor/](tools/conveyor/README.md) | the matching pipeline (LAN); `compilers.py` for the per-segment compiler |
| [docs/external/README.md](docs/external/README.md) | the vendored n64-decomp-workbench (`diagnose`) and external notes |
| `cloud/work/ipa-groups/`, `cloud/work/near-miss/` | historical (2026-09-30) work indexes, superseded by the frontier tooling; kept for their fixtures |
