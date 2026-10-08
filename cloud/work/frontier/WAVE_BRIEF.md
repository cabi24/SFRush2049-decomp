# Matching-wave brief (frontier plan, workstream D)

Read first: `CLAUDE.md`, `docs/plans/2026-10-04-frontier-plan.md` (§1 model, §2 rules, §3 D per-function
procedure — follow that procedure), and the quirks list in `cloud/PLAYBOOK.md`.

## Goal
Strict `MATCH` from `tools/cloud/score.py` for as many of your assigned game functions as you can, as
IDO 5.3 / C89 source. Names are historical labels, not semantics.

## Facts you need
- The game is one whole-program **-O3** unit. Default flags: `-g0 -O3 -mips2 -G 0 -non_shared`; also try `-O2`
  on the first structurally right draft and keep whichever is closer.
- `python3 -m tools.conveyor.pipeline.frontier show NAME` gives callees, callers that must be in the unit, IPA
  register facts and paths of prior source. `grep -r NAME cloud/work docs dot_handoff.md` finds older attempts
  and their stop notes; start from the best prior source.
- `python3 cloud/work/tools/tdis.py NAME` disassembles the retail function. Matched house style: `src/blob/*.c`
  and `cloud/matches/*.c`. Arcade source for semantics: `reference/repos/rushtherock/` (find the ancestor by
  behaviour and constants, never by name).
- Classify from the disassembly before writing (plan §3 D step 2): register-parameter callee, unsaved
  callee-saved registers, four-wide `t6`-`t9` ring, inlined callee (descending `ra,t5,t4…` loads then stores).
  Any of these means it cannot match alone: build a real `-O3` group (`group.json` + sources; examples in
  `src/blob/groups/*/` and `cloud/work/frontier/agentB/Input_ApplyPadConfig/group/`) with the real partner
  functions. Already-matched partners' sources are in `src/blob/` — reuse them unchanged as context/members.
  If the real partners are unmatched, prove the body with stand-in callers and report it as **provisional**
  (not a match, not spliceable).
- A function's own float literals / jump tables / local statics score as `MATCH (N section-relative
  relocations unverified …)` in the (unpatched) scorer. Write natural literals, never `extern f32 D_8012xxxx`,
  and report it as "code identical, own-rodata unverified": the splice verifies the bytes against the image, so
  the integrator can land it. Check each literal's bits against the retail word yourself (the splice refused a
  `9.5493f` that should have been `9.549305f`).
- To test a function inside the real whole-program unit (an internal function with its real callers, or a
  caller that inlines a locked callee): from the repo root on the Pi,
  `python3 -m tools.conveyor.pipeline.blob_unit --tag <you> score NAME [NAME…] --with path/cand.c`
  (3–5 s; `--internal FN`, `--keep FN`, `--block FN`, `--neighbours`). Always pass your own `--tag`.
- Read §0 "What wave 1 changed in the method" in the plan before starting: arcade declaration lists and literal
  spellings, deleted-static stubs (`frontier stubs`, `frontier show NAME`), one variable = one register, and
  the large-function tools in `cloud/work/frontier/w1b/` (`o3s.sh`, `bscore.py`, `gen.py`).
- Literal types matter (`1` vs `1.0f`), declaration order sets stack slots, each named local costs a slot,
  use SDK GBI macros for display-list words.

## Wave 9 techniques (try these first; each closed a large function)
- **Inlined locked helpers:** big leaf functions often inline already-locked kept functions (rand
  `func_8008B2B4`/`func_8008B2E4`, LCG `func_800D50E4`, `func_800B930C` get_next_checkpoint). Paste the
  locked definition verbatim into the candidate; their inlined locals also explain "frame filler" words.
- **Caller-less `jr ra; nop` stubs right before a function are deleted inlined statics.** Define them (static,
  or non-static with `--internal NAME`) and size the frame: each inlined call reserves ~8 bytes in call order.
- **Index global arrays directly** (`D_80152038[i].f`) instead of through a pointer local: a store through a
  pointer local aliases every global and kills PRE webs. Conversely launder a pointer via `(T *)(u32)p` when
  retail keeps loads behind stores.
- **Retry `extern f32 D_8012xxxx` near-misses with natural literals** — the scorer now verifies own rodata.
- **`single` near-misses: try `blob_unit score NAME --internal NAME`** — the label can be wrong when callers
  are locked.
- **Constant sharing follows signedness:** a `1` stored to a signed byte shares the s32 `1` web; `u8` does not.
- **Tied colouring priorities resolve by first appearance in source**; splitting `x = a; x += b;` moves it.
- Check older drafts' global addresses against the retail disassembly before tuning (w9e found a wrong one).

## Shared tracing toolkit
`cloud/work/frontier/tools/trace/` (read its README.md): `install.sh <your builder scratch>` once, then
`us.sh` (unit score + rows), `udiff.py --ops/--norm`, `ctrace.sh`/`sum.sh` (colouring decisions, numintf,
callee-saved cost), `force.sh` (forced colouring oracle), `spill.sh`/`spcensus.sh` (spill-temp/frame layout),
`ugt.sh` (ugen order), `as1t.sh` (scheduler). Set `TAG=<lane>`. Run `force.sh` with the suspected webs before
writing variants: if forcing reaches 0 rows the residual is colour-only.

## Wave 12 rules
- Unused local declarations are acceptable ONLY when they are the exact frame residual (every
  function-level named local takes a slot, used or not) and are disclosed in the header. Pad arrays that
  stand in for unknown structure elsewhere are not.
- A group that gives a locked stub a real body must list that stub in "members" (and say which
  prefer_definition/force_internal overrides it needs). Superseding groups keep the SAME name and every
  old member.
- `cloud/matches/NAME.c` is ONLY for functions that `score.py fn` matches standalone (CI rescores every changed
  file there standalone). A function that is EQUAL only in the whole-program unit or in a group goes in your
  lane's `groups/<name>/` dir with the context files it needs (see w12l), never in cloud/matches.
- Prove a residual is colour-only with `tools/trace/force.sh` before writing variants. For a colour tie,
  try the compiled-out `if (x) {}` priority lever (w11a) and `t = a; t -= b;` splits (w11c) first.

## Wave 14: mass variant testing (vbatch + workbench generators)
- `cloud/work/frontier/tools/vbatch/` (README.md): `vgen.py` expands a template with choice points
  (`/*@{*/a/*@| b @}*/`) into hundreds of variants, and `wbgen.py` writes every mechanical one-edit neighbour
  (commutative swaps, copy removal, operand hoists) using the vendored workbench's sweep generators.
  `vbatch.sh` scores a whole directory on the builder in seconds and ranks by strict count.
- Diagnose first: `python3 -m tools.conveyor.pipeline.diagnose one NAME --source BEST.c`, then
  `python3 tools/workbench.py guide`. Hundreds of variants are cheap. Plan at least 3 rounds and 300
  variants per function, and stop after 3 rounds with no strict improvement.
- **Named float locals change the FP registers.** place_cars_in_order's last 8 words: a named `f32 f`
  for a sum later converted to u32 is a coloured web (it got $f0). Written as one expression, the sum and
  the conversion stay in ugen's temp ring ($f18/$f4, as in retail). If retail's FP registers are all ring
  temps ($f4–$f18) but yours puts one in $f0/$f2 or a callee-saved $f20+, inline the named local.
- Pad arrays (`u8 pad[20]`) to fix a frame are still not acceptable (Wave 12 rules), even when a sweep finds
  them load-bearing. Report the frame residual instead.

## Permission denials
If any tool call is denied by a permission/safety check, do NOT retry it in another form (different paths,
globs, quoting, tools or hosts). Record it in RESULTS.md and continue without it.

## Builder (IDO runs only there)
```
ssh watchman2 'cp -r ~/rush2049/scratch/frontier/base ~/rush2049/scratch/frontier/<you>'      # once
scp cand.c watchman2:rush2049/scratch/frontier/<you>/cand/NAME.c
ssh watchman2 'cd ~/rush2049/scratch/frontier/<you> && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
…  python3 tools/cloud/score.py group cand/<groupdir>          # -O3 group directory
```
Work ONLY inside your own scratch copy on the builder. Never touch `~/rush2049/repo`, `/tmp/blobsplice`,
`/tmp/blobgroup`; never run make or the conveyor there. At most 2 cores; the box is shared and other agents
are running. Helper scripts (aligned diffs, batch scoring) are in `cloud/work/frontier/w1f/` and `w1b/` (the older `agentC/full.py` crashes on runs of `nop`).

## Discipline
- Run `python3 tools/workbench.py diagnose` (docs/external/README.md) on a near-miss before searching.
- Stop a function only after 3 batch rounds (300+ variants) with no strict improvement (Wave 14); then
  write it up and move on.
- Only what the scorer printed counts. Quote it exactly.

## Deliverables (local repo only)
Do not commit, splice, or edit `src/blob`, `asm/`, `*.lock.json`, `tools/`, `include/`, or other agents' dirs.
- Strict MATCH: `cloud/matches/NAME.c` (line 1 `/* flags: … */`; header comment with real semantics, arcade
  ancestor if proven, and any shaping quirk). For a group match: the group dir at
  `cloud/work/frontier/<you>/groups/<group>/` with `"claims"` listing the strict members.
- Otherwise: `cloud/work/frontier/<you>/NAME/best.c` + notes.
- `cloud/work/frontier/<you>/RESULTS.md`: per function — state (MATCH / code-identical-unverified /
  provisional / N words off), flags, exact scorer command and output, residual lane, what was tried, best next
  hypothesis, struct layouts and global types recovered.
Final message: the table, plus anything that generalises.
