# Making Astra's cloud time count (plan, 2026-10-11)

Astra (GitHub `cabi24`) works from the GitHub copy of this repo on a cloud box: IDO 5.3 (and now 7.1) via
`tools/cloud/setup.sh`, the scorer, the committed target words, no ROM, no builder, no conveyor database.
Astra has opened 299 PRs since late September; 291 are merged. This plan says where that effort lands today,
where it leaks, and what the coordinator and Astra each change so that more of it reaches the cartridge.

## 1. Findings

**Where Astra's time goes now.** The current wave of work (PRs #309–#311, and ~40 merged packets under
`cloud/work/frontier/dot_runtime_a_*`, `cloud/work/runtime_[ab]_*`) targets the two **runtime images**
("overlay A/B") loaded at `0x8038A400`: image A (ROM stream `0xB5C534`, 193 functions, 152,952 B) and image B
(stream `0xB6FEC4`, 49 functions, 38,708 B). Their targets have been in the repo since 2026-10-03
(`asm/us/ovl_a`, `asm/us/ovl_b`; `score.py --targets`), and CI rescores `cloud/matches/ovl_{a,b}/*.c`.

**Every one of those matches is stranded.** There is no layout, splice, recompression or ROM-compose path for
the runtime images, and no coverage denominator reports them. Astra says so in every PR ("zero accepted-image or
cartridge coverage gain") and keeps going. Stranded today: 25 standalone matches in `cloud/matches/ovl_*`
(7,036 B, 13 of them Astra's), group matches inside the packets (roughly another 5 KB), and 1,508 B in the open
PRs #309/#310. That is about 14 KB of verified, byte-exact work with nowhere to go, against a 191 KB population
nobody else is working on.

**The brief Astra works from is stale.** `CloudHandoff.md` (last edited 2026-10-01) still says 125/912 game
functions and 23/230 static; the truth on 2026-10-11 is 972/1216 game (310,380 B, 47.97%) and 489/668 static
(87,756 B, 54.60%). Its work-item indexes (`cloud/work/ipa-groups/INDEX.md`, `cloud/work/near-miss/INDEX.md`)
date from 2026-09-30 and predate the frontier tooling, the trace-first method and the near-miss table in the
[frontier handoff](2026-10-06-frontier-handoff.md) §0. The brief also does not mention runtime images or the
IDO 7.1 boot tail at all, so Astra's best-fit targets are not where the brief points.

**Integration friction is high relative to yield.**
- Of the 141 PRs merged on 2026-10-06 (#164–#304), 1,020 B of game code and 304 B of static code landed; the
  rest was research, candidates without callers, or runtime-image work with no landing path.
- Research packets pin SHA-256 of mutable tools (`score.py`, `owndata.py`, `blob_unit.py`, `blob_group.py`,
  the pipeline). Nearly every tooling change rebinds receipts (four on 2026-10-11 alone for `blob_group.py`,
  one for `blob_unit.py`). Eight boot-tail packets compiled their ROM TU with the 5.3 compiler and broke the
  day the boot tail moved to IDO 7.1; each needed a patch and a rebind.
- 67 of the 79 files in `tests/cloud` are packet replays; the cloud suite takes 6¼ minutes with IDO on the
  builder and is the slowest step of every integration.
- PR #163 (100 files) has been held since 2026-10-06 because five of its own replays fail with pinned IDO on
  x86; it was never rebased. PRs #305–#308 ran 70-proposal "effort spread" sweeps on texture_rectangle and
  camera_transform and report zero improvement: the same mass-variant approach wave 14 abandoned here.

**What Astra does well.** Patient reconstruction with real callers and real context, honest reporting,
semantic test harnesses, and drafts that later close: the coordinator's wave-17 lane took func_800D91A0 from
603 rows to 8 starting from Astra's caller draft, and Astra's func_800D816C draft is the next step there. The
overlay-A work (PRs #309–#311) is careful and verifiable. The problem is not quality; it is that the output has
no landing path and the brief points at the wrong targets.

## 2. Principles

1. **Every match Astra produces must have a landing path the day it is merged.** No more "zero coverage gain"
   PRs by design.
2. **Astra works from the live frontier**, not from indexes frozen two weeks ago.
3. **Packets are cheap to carry:** bound to inputs, not to tool hashes; one small test each; replayable on
   current master.
4. **Trace, don't sweep.** The method that closed 17 of 25 lane targets in waves 15–17 is written down in
   `cloud/work/frontier/WAVE_BRIEF.md`; it works from the cloud too.
5. **Research drafts are welcome, labelled as such, small, and never block the queue.**

## 3. Workstreams

### A. Runtime-image landing path (coordinator; two tooling lanes; the biggest unlock)

Build for images A and B what `blob_layout` / `blob_splice` / `blob_group` / `blob_rom` do for the game code:
- `ovl_layout`: tile each image from `asm/us/ovl_{a,b}/extents.json` (opaque runs preserved), generate the
  linker script at `0x8038A400`, one `.text.<name>` input per function, data regions opaque.
- `ovl_splice` / group splice: the same gates (compiled body equals the extent, own `.rodata`/`.data`/`.bss`
  proofs via `owndata`), a separate lock file (`ovl_matched.lock.json`), the same `install_group.py` shape.
- `ovl_rom`: inflate the retail stream with the vendored zlib 1.0.4 and prove the exact deflate parameters
  reproduce it byte for byte (as `blob_rom` did for the game code), then compose at `0xB5C534` / `0xB6FEC4`
  via `compose_data.py` and run the full ROM SHA-1 gate. Until the stream reproduces, nothing lands.
- Reporting: `make progress` gains two rows ("overlay A", "overlay B"), never merged into the game or static
  denominators. CI's rescoring of `cloud/matches/ovl_*` already exists.
- Acceptance: the 25 existing standalone matches and Astra's #309/#310 bodies spliced, ROM exact, CI green.
Estimate: the game-code path took three stages (008/009 specs); this reuses all of it with a base/stream
parameter. Two Opus tooling lanes in worktrees, one for layout+splice, one for the stream round-trip and ROM
compose, each with tests; the coordinator integrates. Open owner question: whether the headline coverage line
gains the overlay rows (recommended: yes, as separate rows).

### B. Refresh the cloud brief (coordinator, half a day)

Rewrite `CloudHandoff.md` §1–§3 and §4:
- current numbers and the three populations (static, game, overlays A/B) with their target directories;
- **Lane 1: overlays A/B** (primary; 191 KB, scorable from the cloud with `--targets`, landing path from A);
- **Lane 2: game near-misses** from the frontier handoff §0 table, each with its traced cause and best next
  lever (func_800D91A0 + func_800D816C first: Astra's own drafts, 6.4 KB closes), plus how to read
  `frontier show` output committed alongside;
- **Lane 3: static boot tail under IDO 7.1**: `IDO_DIR=tools/cloud/ido71 python3 tools/cloud/score.py fn ... --targets asm/us/boot_tail`
  scores the ~115 remaining Rush boot-tail functions from the cloud (`cloud/work/frontier/w17g/inv_table.md`
  lists them; the lock/promotion path is the coordinator's);
- retire `cloud/work/ipa-groups/INDEX.md` and `cloud/work/near-miss/INDEX.md` to `docs/history/` (they are
  superseded by `frontier` and the handoff), and point to `WAVE_BRIEF.md` for method;
- a claims file (`cloud/work/CLAIMS.md`): one line per function or group being worked, by whom, since when;
  the coordinator's lane map goes in it too, so Astra and the lanes stop colliding (camera_transform,
  func_80087110 and the mode-select cluster all overlapped in the last two weeks).

### C. Packet protocol v2 (Astra adopts now; coordinator documents it in `CloudHandoff.md` §4)

- **Deliverable first, evidence second.** A match is delivered the way lanes deliver: `cloud/matches/<pop>/NAME.c`
  for a standalone `score.py fn` MATCH, or a group directory with `group.json` (`members`, `claims`, `context`,
  `keep`, `flags`) and `score.py group` output. The README and `verify.py` are supporting material.
- **Receipts pin inputs, not tools.** Pin the sources, the target words (`SHA256SUMS` rows) and `verify.py`
  itself. Do not pin `tools/cloud/score.py`, `tools/conveyor/**`, `matched.lock.json`, `Makefile`, or
  `asm/us/**/*.s` text (each splice adds a provenance line). The §5 patterns in the frontier handoff are the
  fixes we keep applying by hand; stop generating the breakage.
- **Compile a ROM TU with its pinned compiler** (`tools.conveyor.compilers.tu_cc`); never `score.ido("cc")`
  directly for a TU.
- **No `git show` of commits that are not ancestors of master**, no replays that need unmerged branches.
- **One test per packet, under 5 seconds without IDO and under 30 with it**, sharing the existing conftest
  skips. Packets that need more belong in the research directory without a test.
- **PR size:** one function or one group per PR, under ~30 files, opened ready (not draft) when its claims
  list is non-empty; research with empty claims stays draft and is merged in batches, never blocking.
- **#163:** rebase onto master or split into the one promotable piece (`voice_unblock_production`, 124 B
  static) plus research; the coordinator will not spend another integration day on a 100-file draft.

### D. Method (Astra adopts; coordinator shares the tools)

- Stop the effort-spread / hypothesis-batch sweeps (#305, #306, #308): 210 proposals, zero improvement, and
  the camera_transform residual is a register-allocation problem that sweeps cannot see.
- Use the trace-first sequence in `WAVE_BRIEF.md` ("Wave 15", "Wave 16", "Wave 17" sections): `diagnose`,
  then the forced-colour oracle, then the colouring trace, then one source lever. The instrumented uopt builds
  (`cloud/work/frontier/w16d/tools/patch_spl.py`, `w16b/tools/splitpatch.py`, `w17f/tools/prio_patch.py`,
  `w17i/boostpatch.py`, `w17a/tools/patch_lbb.py`) patch the ido-static-recomp sources, which build on x86 in
  the cloud; the coordinator will add a one-page "build the traced compiler in the cloud" recipe to the brief.
- The levers that closed functions this week are listed in `WAVE_BRIEF.md` and apply directly to Astra's
  frame-only residuals in #311 (two to seven words, frame off by 8–24 B): those are declared-local sets, reused
  variables and float-literal spelling, not new source.

### E. Cadence and feedback (coordinator)

- Integration within one day of a ready PR with non-empty claims; research PRs batched weekly.
- Each integration posts one comment on the PR: landed bytes, or the exact gate that refused and why.
- The handoff §0 table and `CLAIMS.md` are updated when a wave starts and ends.

## 4. Sequence

| When | Coordinator | Astra |
|---|---|---|
| Now | Hand Astra this plan; add the status banner to `CloudHandoff.md` | Keep working overlay A/B (it will land); switch to protocol v2 on the next PR; pause the sweeps |
| Days 1–3 | Launch the two overlay tooling lanes (A); rewrite the brief (B); write `CLAIMS.md` | Convert #309/#310 to ready; rebase or split #163; close or merge #305–#308 as research |
| Days 3–7 | Land the overlay path; splice the existing 25 + #309/#310; add the progress rows | func_800D816C (its own draft) to close func_800D91A0's cluster; frame-only residuals in #311 with the wave-17 levers |
| Week 2 | Retire the stale indexes; cloud recipe for the traced compiler | Boot tail under 7.1 from `inv_table.md`, smallest first |

## 5. Measures

- Bytes landed per Astra PR (today: about 9 B per merged PR over #164–#304; target: every ready PR lands).
- Stranded verified bytes (today ~14 KB; target 0 once A ships).
- Receipt rebinds per week caused by tool-hash pins (today several; target 0 after protocol v2).
- Cloud suite time on the builder (today 6¼ min; target under 4 with the per-packet budget).

## 6. Open questions for the owner

1. Do overlay A/B rows join the headline coverage report (recommended yes, separate rows)?
2. Merge #305–#308 as research history, or close them?
3. Is a weekly integration batch for research PRs acceptable, or should they keep merging as they arrive?
