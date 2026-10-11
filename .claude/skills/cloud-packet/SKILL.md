---
name: cloud-packet
description: Package one Rush 2049 cloud match or research result for review using packet protocol v2, with input-bound evidence and a cheap routine check.
---

# Package a cloud result

Apply [CloudHandoff section 4](../../../CloudHandoff.md#4-how-to-hand-work-back-packet-protocol-v2)
and [the cloud plan, section C](../../../docs/plans/2026-10-11-astra-cloud-plan.md#c-packet-protocol-v2-astra-adopts-now-coordinator-documents-it-in-cloudhandoffmd-4).
This checklist uses the existing scorer, group format and packet-local verifier;
it adds no receipt schema or integration path. Run commands from the repo root.
The coordinator owns `CloudHandoff.md`, generated targets, locks and ROM gates.

## 1. Deliver source and claims first

- Start from current `origin/master`; read the [live frontier, section 0](../../../docs/plans/2026-10-06-frontier-handoff.md#0-wave-1317-update-2026-10-11-after-wave-17-and-the-ido-71-boot-tail-promotion)
  and [wave 15–17 method](../../../cloud/work/frontier/WAVE_BRIEF.md).
  Check [lane claims](../../../cloud/work/CLAIMS.md) before choosing a target and
  add your claim in the same PR. Trace a near-miss with
  `diagnose`, a focused oracle/trace and one source lever; do not restart sweeps.
- Identify the population as well as the name/address: `blob`, `ovl_a`, `ovl_b`
  or static/`boot_tail`. The overlays share addresses, not targets. Inspect current
  locks/source so an inherited match or already-landed body gets no new credit.
- A standalone strict MATCH goes in `cloud/matches/NAME.c` for blob, or
  `cloud/matches/{ovl_a,ovl_b,boot_tail}/NAME.c`. The first line is
  `/* flags: <the exact IDO flags> */`. Include purpose, known source ancestry,
  N64 differences and disclosed shaping quirks. Do not invent a `matches/blob/`
  directory: current submission discovery does not scan it.
- A group supplies its C files and `group.json`: `files`, `flags`, `members`,
  `claims`, `context`, `keep`. Claims are a subset of members. Keep authentic
  nonmatching callers/callees in `context`; preserve their source and relevant
  `keep` entries. Do not relabel a failed claimed body to make a check green.
  Separate newly claimed bytes from existing exact preservation members.
- Run the actual strict command, e.g.:

  ```sh
  python3 tools/cloud/score.py group cloud/work/frontier/PACKET/groups/GROUP --targets asm/us/ovl_a
  python3 tools/cloud/score.py fn cloud/matches/ovl_a/NAME.c NAME --targets asm/us/ovl_a --flags "EXACT FLAGS"
  ```

  For an installable group, every member must pass **without** `--claims` or
  `--allow-unverified`. `--claims` can exit zero with no claims, even on a build
  failure; that is research status, not validation. Preserve explicit owned-data
  refusals and coordinator-only proof requirements (such as split `.rodata`);
  never turn code-identical-but-unverified into plain MATCH.

## 2. Bind the evidence to inputs

- Keep the packet's existing receipt format. Hash submitted sources, required
  context/header inputs, `group.json` and its own verifier/helpers. Finalize the
  verifier before generating its receipt; a source/verifier edit requires replay.
- Bind each claimed native body by image, name, extent and SHA-256 of its target
  words. Load targets through the scorer's manifest-verified path. Retain only
  relevant checksum rows (for example `extents.json` and `symbols.json` from that
  population's `SHA256SUMS`), not a hash of the entire manifest.
- Do **not** freeze `tools/cloud/score.py`, `owndata.py`, `tools/conveyor/**`,
  locks, `Makefile`, or assembly text as receipt equality inputs. A selected
  `.s` manifest digest still pins assembly text: bind parsed native words instead.
  The scorer must continue checking current files against their current trusted
  manifest; excluding text from a receipt does not bypass that integrity gate.
- Record compiler identity, flags, commands and observed results. Host compiler,
  binutils versions and tool revisions may be diagnostic metadata, not frozen
  replay-equality requirements. Use the stock pinned compiler to prove a match;
  instrumented compilers diagnose the cause only.
- A replay must use the submitted checkout and current master dependencies. No
  fetching/cherry-picking another open PR and no `git show` of an unmerged-only
  commit. Archival PR/commit links are fine as historical provenance, not runtime
  inputs. If indispensable context is unmerged, disclose the dependency and keep
  the result draft until it is available or self-contained.

## 3. Select the production compiler correctly

For a ROM translation unit, resolve its compiler through the existing pin:

```python
from tools.cloud import score
from tools.conveyor import compilers

cc = compilers.tu_cc("lib_1a660", score.IDO / "cc")
```

Pass `cc` to the existing TU/asm-processor build, retaining that TU's flags and
include paths. Do not compile a production TU directly with `score.ido("cc")`.
The selector fails closed if its pinned compiler is absent. For standalone
boot-tail scoring, explicitly set `IDO_DIR=tools/cloud/ido71` when the target's
recorded compiler is 7.1; do not change the compiler for unrelated 5.3 targets.

## 4. Keep routine verification cheap

- One collected test per executable packet: input/claim checks plus one bounded
  strict replay. Measure under 5 seconds without IDO and under 30 with IDO.
  Run cheap input checks before compiling. Reuse `tests/conftest.py` handling of
  missing IDO; `REQUIRE_TOOLCHAIN=1` must fail a missing-tool skip. Do not swallow
  a scoring failure as an unavailable-tool skip.
- Current `check_submissions.py` discovers blob singles, population-qualified
  singles (selecting pinned 7.1 for Rush boot-tail targets), and
  `cloud/work/ipa-groups` groups (the latter default to blob). It does **not**
  discover frontier groups. The packet's one explicit test must cover the correct
  target directory and compiler when automatic discovery does not. Check current
  tooling before assuming CI covered a new location.
- Preserve valuable semantic checks, negative controls and compiler traces as
  opt-in research replay, with the exact command and limitations in the README.
  Do not merely rename many expensive checks into one expensive test. If no
  meaningful bounded check fits, retain a research draft outside the default
  suite; a docs-only historical handoff needs no executable test.

## 5. Review and publish within the authorized scope

Use one function/group per small PR (roughly fewer than 30 files). Validated
nonempty claims qualify for ready status; empty-claim research stays draft.
This status convention is not permission to publish or merge. Leave acceptance,
image splicing and the full-ROM gate to the independent checker/coordinator.
State code-match bytes separately from accepted-image/cartridge coverage, and
keep each population's denominator separate. Never include ROM bytes, raw
assembly dumps, compiler outputs, credentials or unrelated private material.

Use this compact README/PR handoff instead of another framework:

```text
Population and source/group path:
New claims (name, extent, bytes); existing preservation members; context:
Compiler and flags:
Exact strict scorer command and output:
Receipt/input check and one routine test (elapsed; passed/skipped/failed):
Optional deep-research replay; bounded findings and remaining uncertainty:
Base/head commits; dependencies available on master:
Landing gate and accepted-image/ROM coverage still unverified:
```

Before reporting publication complete, check the exact remote head, protected
path guard and applicable tests, then follow CI for that head to a terminal
result. Report a failed or unrun gate explicitly; a receipt is not ROM evidence.
