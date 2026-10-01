# Handoff to Codex: coordinator for the Rush 2049 decompilation (replaces the Claude coordinating session and the cloud worker)

You are taking over as the **coordinator and integrator** of this project for the next several days. The Claude session that
ran it is out of budget; the Claude cloud worker (GitHub only) is finishing. You have a large token budget: use it by **fanning
out many parallel workers** on disjoint work and integrating their verified results. This file is self-contained; read the
links as you need them. Work in `/home/cburnes/projects/rush2049-decomp`.

## 0. Environment you need (check this first)

Your last session was sandboxed: no DNS for the builders and a read-only `.git`. Those two blocked acceptance of Task A.
This job needs **unsandboxed** access: `ssh Rocky` and `ssh watchman2` must work, `git commit`/`git push` must work, and
`~/.conveyor/` must be writable. First run `ssh Rocky true && ssh watchman2 true && git status`. If they fail, stop and tell
the user to relaunch you with the sandbox off (`codex --dangerously-bypass-approvals-and-sandbox`); do not fake results.

## 1. The project and where it stands

Goal: C source that compiles with SGI IDO 5.3 to a **byte-identical** *San Francisco Rush 2049* (N64, US) ROM. Read `CLAUDE.md`
(rules and routing), then `CloudHandoff.md`, `cloud/PLAYBOOK.md` (techniques; the bottom section holds the latest findings),
`docs/external/README.md`, `cloud/HANDOFF_AUTOMATION.md` (the cloud worker's toolkit and its section 5 of known hard problems).

Status at handoff (verify with `make progress`): **game code 497 of 1,210 functions, 56,416 of 647,072 bytes (8.72%) built
from C; static code 23 of 230 functions, 1,448 of 61,440 bytes (2.4%)**. Built ROM is SHA-1 exact. Never merge the static and
game denominators in any report. Session start of this session was 127 game functions / 11.7 KB.

Where the rest is (game, bytes of the image): 42% has a compiling candidate (mostly far-off m2c seeds; only about 9% is within
a few words), 34% has no candidate, 15% is not in any registered function (data, tables, the 7 refused heads).
44% of the game image is code compiled as **whole-program IPA groups**: such functions cannot match alone.

### Hard rules (all learned by getting them wrong)

1. **Strict scoring only.** A match is a bare `MATCH` from `python3 tools/cloud/score.py fn FILE.c FUNC --flags "..."` (x86 host) and
   then the **image gate** (`blob_splice`/`blob_group`: the body equals the image bytes in place), then the full-ROM SHA-1.
   Relocation-blind or stack-masked scores are leads, not matches (several "zero"s were false).
2. **Gates before any commit that changes the lock or the layout:** `python3 -m tools.conveyor.pipeline.blob_rom rom` prints
   `SHA-1 EXACT`; `blob_splice check` and `blob_group check` report 0 problems; pytest exits 0. **Capture pytest's exit code with
   `rc=$?` on its own line; never read `$?` after a pipe** (a failure was once masked and pushed).
3. `cloud/matches/<fn>.c` line 1 must be exactly `/* flags: <IDO flags> */` (nothing else on that line; extra text becomes part
   of the flags and breaks the compile).
4. A group with **stand-in callers** cannot be spliced (the splicer refuses calls into stand-ins). A claimed function that uses a
   jump table is fine now (`blob_group` places and checks the table).
5. After a layout change (head registration) **`git rm` region files the layout no longer lists** (compare
   `blob_layout.load()['regions'][*]['name']` with `asm/us/blob/*.s`), run `blob_tu generate`, `git add -f` new region files
   (new ones are git-ignored). A stale region file broke CI once.
6. Never `git add -A`; commit explicit paths. Never force-push or rewrite history. Never read or print `~/.conveyor/token`.
   No ROM bytes or `build/` in commits. Do not kill processes with `pkill -f <pattern>` if the pattern appears in your own command
   line (it kills your shell); kill by PID. Do not run registration or ROM rebuilds while another worker's splice is half-done
   (`git status` under `asm/us/blob/`, `src/blob/`, `blob_matched.lock.json` must be clean first).
7. A match that depends on a quirk (dead read, unused local, symbol defined in the unit) is fine but say so in the commit message.
8. Third-party notes without a licence (`docs/external/files/`, the Snowboard Kids doc) are git-ignored: read, do not commit.

## 2. Infrastructure (what exists)

- **Hosts.** The Pi (this machine, ARM, 16 KB pages) cannot run IDO. **Rocky** (x86, 20 cores) and **watchman2** (x86, the
  builder: ROM builds go through it) can. Compile and score on Rocky; `blob_rom rom` and splices use watchman2.
  IDO 5.3 toolkit on both: the **published** toolkit (what nodes run jobs from) is `d4c39cbc85750cd0...` (jobs/ code, includes the group_search job and the
  worker-leak fix); IDO itself and `bin/objdump` are in `~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/`.
  Changing anything under `tools/conveyor/jobs/` needs a rebuilt and re-published toolkit (README, "Toolkit updates").
- **Coordinator + farm** (systemd on the Pi): `conveyor-coordinator`, `conveyor-farm`. The farm runs ordinary permuter searches and,
  with `--group-search` (a drop-in at `/etc/systemd/system/conveyor-farm.service.d/group-search.conf`), harvests zero-score group
  searches and queues longer tiers. A systemd timer `external-docs.timer` refreshes `docs/external/` daily. `python3 -m tools.conveyor.cli status`,
  `... cli attention`, `... pipeline.group_jobs candidates|results|harvest|tiers`.
  The farm is currently out of promising ordinary work (the rest of the backlog is far-off seeds). Cancelled searches no longer leak
  workers (fixed and tested); if Rocky's load looks wrong, look for permuter processes older than 4.5 h and kill them by PID.
- **Vendored workbench** (`third_party/n64-decomp-workbench/`, CC0): `python3 tools/workbench.py diagnose|guide ...`. `python3 -m
  tools.conveyor.pipeline.diagnose one|triage` runs it on our data (accepted; `cloud/work/near-miss/VERDICTS.md` is its table).
- **Cloud worker toolkit** (`cloud/work/tools/amatch/`, `ipakit/`): deterministic search/triage; on our data its autopilot matched
  about 2% (6 of 387), the easy tail is gone; its `triage.py` and `ipakit` are still useful for choosing work.
- **Reference scripts** in `specs/014-codex-handoff/reference/` (splice a list of matches, promote a cloud group, line-reflow sweep,
  the Rocky compile+score+diagnose loop). Turn the ones you keep using into tested tools.
- **Rocky work dirs**: `~/agents/<ID>/wt` are full repo copies with `tools/cloud/ido` (use one per worker; A, B, C, D exist),
  `~/agents/wb/` holds `loop.sh`, `targets/<fn>.o` (target objects exported from the coordinator) and seeds `base/<fn>.c`.
  Export more target objects with `n64_target.target_o_sha` and the blob store (see `diagnose.py`).
  Keep the combined worker load under about 14 of Rocky's 20 cores: the farm and group searches use the rest.

## 3. What works (use it, in this order)

From the pilot (18 near-misses, 7 matched; logs `cloud/work/workbench_pilot_*.md`, summary at the end of `cloud/PLAYBOOK.md`):
read the **register lanes** of `diagnose` (pool v0/v1/a0-a3 versus ugen's temp ring t6-t9); fix the **frame** first (drop m2c `pad`
locals); reorder float/int web colouring with a **code-free dead read** (`f32 t = 0.0f; if (G) {}`); supply a missing temp-ring pop
with a **redundant mask**; put statements on the **same physical line** (scheduling barrier even at `-g0`); **re-derive the seed from the
asm** when the verdict stays `structure-mismatch`; the **`t6`-`t9` "wall" is whole-program**: those functions only match as non-exported
`-O3` group members with two call sites, so they need real groups with their real callers.

## 4. Work lanes to fan out (each is an independent pool of workers)

Run **6 to 10 workers at once**, each with its own function set and Rocky dir. Give every worker the template in section 6.
You, the coordinator, do not match functions yourself; you choose lanes, cut work into packets, verify, splice, commit, and report.

**Lane 1 — near-miss list (about 70 left).** `cloud/work/near-miss/VERDICTS.md` (closest first, with verdict and lever) and
`cloud/work/near-miss/INDEX.md`; earlier attempts in `cloud/work/hand_notes_A.md`, `hand_notes_B.md`, `workbench_pilot_*.md`. Packets of 4-6 functions of
the same verdict class. Still open from the pilot: `func_800CCE5C` (2 words; sp1C home one word off + a temp-ring pop), `func_8008A704` (2),
`state_update_global` (3), `func_8008C680` (4; ring is six wide here, target wraps at four), `func_800D18D8` (8), `func_8008B000` (15), `func_800A79F4` (44).

**Lane 2 — real groups for the whole-program wall (highest value, the largest block).** Functions that only match as groups with
stand-in callers: `input_aux_handler`, `func_800C7200` (4 words), `func_8008ABE4` (6), `func_800B7438` (group `cloud/work/ipa-groups/func_800B7438/`),
plus `cloud/work/workbench_pilot_W1/`. Build the group with the **real** callers (use `python3 -m tools.conveyor.pipeline.ipa` /
`build/ipa_groups.json`, 33 groups; `blob_group seed GROUP`; the cloud's 40 group dirs under `cloud/work/ipa-groups/` and their
`STATUS.md`; `cloud/work/tools/ipakit/groupgen.py`, `deps.py closure`). Scale it: for each IPA-shaped function the closure tool proposes a
group; a worker hand-writes the members and aims for strict `score.py group --claims` MATCH. Spliceable groups only (no stand-ins).
Also the 22 IPA-shaped newly registered heads.

**Lane 3 — static code (the untouched half; 202 functions have candidates, 40 are within 5 points per word).** 230 functions, 61,440 bytes,
only 23 done. This is ordinary separately-compiled libultra/libc code (no IPA), proven path: **read `.claude/skills/promote-match/SKILL.md`**
(static uses `lock`/`promote`, not the game splice). Start from `python3 -m tools.conveyor.pipeline.layout report -v`, the DB
(`n64_target` population `static`, `function_status`), and 12 static functions marked `matched` but not promoted (leads; some are known
false zeros: verify strictly). Public sources for libultra exist (other N64 decomps; the corpus path in `tools/conveyor/README.md`).
Treat as its own track with its own denominator.

**Lane 4 — new heads (45 registered, all `unmatched`).** 23 ordinary, 22 IPA-shaped; m2c cannot seed most (hundreds of words, pointer
tables), so workers write the first C from the asm. Prefer the small ones first (`func_8010C7F4`, `func_80105DA8`, `func_8010C2E4`...). Register
the six proven switch heads from `specs/013-workbench-integration/findings.md` (table there) in one pass when the tree is clean, then re-run gates.

**Lane 5 — integration and tooling (you).** (a) Splice everything workers deliver (reference scripts). (b) Task B from
`specs/013-workbench-integration/AGENT-PROMPT.md`: `func_800BB7F4` and `render_post_process` match the cloud scorer but fail the image gate
(they need their symbol defined in the unit so as1 shares one `lui at`); find the exact difference and splice via a one-member group
or record why not. (c) The 7 `tests/cloud/test_ipakit_*` failures (they assume the audited heads are unregistered): update them to the new layout.
(d) Re-sweep the 90 functions that need `-Wab,-r4300_mul` (sweeps now include it; old results do not). (e) Feed the pilot's levers
(dead read, redundant mask, same-line layout, frame-drop) into `cloud/work/tools/amatch/mutate.py` as verdict-routed mutators and re-run its
bench (`autopilot.py --bench`): the cloud worker measured a clean held-out gain of 0, so judge on held-out. (f) Keep `docs`: after each
milestone update `cloud/PLAYBOOK.md` and the wiki status (`docs/WIKI.md`, `rush2049:status`).

## 5. Rhythm

Loop: pick lane -> write packets -> launch workers -> collect `cloud/matches/<fn>.c` files and logs -> **re-score each strictly yourself** on
Rocky from the repo copy -> splice the verified ones (reference script) -> gates -> commit explicit paths -> push -> sync coordinator
statuses (`function_status` -> `matched`, cancel open jobs for that target) -> update `specs/014-codex-handoff/STATUS.md` (counts, what each lane
produced, blockers). Do this at least after every batch of matches. Push to `origin/master` after gates pass; CI (`.github/workflows/verify.yml`) must
stay green: it rescored changed `cloud/matches` and group claims strictly and runs `tests/conveyor`.
If the Claude cloud worker opens a PR, merge it locally after checking (CI sometimes fails on old merge bases; rescore everything that changed
against current master) and push; it may be retired.

## 6. Worker prompt template (instantiate once per worker; fill the bracketed parts)

> You are a hand-matching worker on the Rush 2049 decompilation (IDO 5.3, byte-identical goal). Repo `/home/cburnes/projects/rush2049-decomp`
> (shared; other workers edit it). Read `CLAUDE.md`, `cloud/PLAYBOOK.md` (techniques, pilot findings), `third_party/n64-decomp-workbench/docs/START_HERE.md`.
> IDO runs only on x86: use `ssh Rocky`. Your private copy is `~/agents/[ID]/wt` (`WT=$HOME/agents/[ID]/wt`); the loop is
> `ssh Rocky 'cd ~/agents/wb && WT=$HOME/agents/[ID]/wt ./loop.sh FUNCTION SOURCE.c ["flags"]'` (strict score + workbench diagnose); seeds are
> `~/agents/wb/base/<fn>.c` (whole TUs; edit only the target function) and target objects `~/agents/wb/targets/<fn>.o`. Work in `~/agents/[ID]/scratch/`.
> **Functions:** [list]. **Method:** run the loop on the seed, record the verdict and lanes, apply ONE lever per variant guided by the verdict
> (guide: `python3 tools/workbench.py guide`), at most about 40 runs per function, stop early on MATCH or when diagnosis gives nothing new; be skeptical of
> the tool and record where it misleads. **Deliver:** for each strict MATCH write the full TU to `cloud/matches/<fn>.c` with line 1 exactly
> `/* flags: <flags> */` and re-score that exact file; a log `cloud/work/<lane>_<ID>.md` (per function: start verdict/words, levers and word counts,
> final result, what moved it, honest verdict on the tool; plus any lever not in the guide). **Rules:** no git add/commit/push; do not edit `src/`, `asm/`,
> `tools/`, `third_party/`, the lock or `~/.conveyor`; do not touch other workers' Rocky dirs; kill processes by PID; never read `~/.conveyor/token`.
> Final message: matched (with flags), not matched (best words), summary.

For group work (Lane 2) change the deliverable to a group directory `cloud/work/ipa-groups/<name>/` (`group.c`, `group.json` with `claims`, `STATUS.md`)
verified with `python3 tools/cloud/score.py group <dir> --claims` (set `allow_unverified` only for unverified `.rodata` relocations).

## 7. What to report to the user

After the first integration batch and then daily: coverage line (`make progress`, game and static separately), what was spliced, what each lane
produced, what is blocked and why, and anything that needs a human decision (licences, deleting data, spending). Keep decisions that are not
irreversible to yourself; do not ask for approval on routine matching, splicing and pushing once gates pass.
