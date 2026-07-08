# Quickstart: Corpus Candidates end-to-end

Every step is a real command with its expected outcome. Run from the repo root on
the Pi unless marked otherwise. Expected numbers are as of 2026-07-08; they may
drift slightly but orders of magnitude should hold. This walkthrough doubles as the
feature's acceptance run (SC-001..SC-005).

## 0. Preconditions

```bash
git -C reference/repos/ultralib rev-parse HEAD   # e24c8367... (clone exists)
python3 -m pytest tests/conveyor -m "not node_required" -q   # all green before starting
curl -s -o /dev/null -w "%{http_code}\n" \
  -H "Authorization: Bearer $(cat ~/.conveyor/token)" \
  http://127.0.0.1:8323/api/v1/meta/toolkit_sha    # 200 (coordinator up)
python3 -m tools.conveyor.cli nodes               # watchman attached, caps builder,x86_64
```

## 1. Toolkit rebuild (watchman) — ships reloc-blind scoring + head-capture

```bash
# on watchman, repo synced first:
rsync -avz --exclude .git ~/projects/rush2049-decomp/ watchman:~/projects/rush2049-decomp/  # from Pi
ssh watchman 'cd ~/projects/rush2049-decomp && python3 -m tools.conveyor.bundles.build_toolkit \
    --ido <ido-dir> --objdump <objdump-path> --out /tmp/toolkit.tar.gz'   # paths per 001 quickstart §2
scp watchman:/tmp/toolkit.tar.gz /tmp/ && python3 -m tools.conveyor.cli publish-toolkit /tmp/toolkit.tar.gz
```

Expected: new toolkit sha pinned (differs from `23185a79...`). Sanity gate before
anything else:

```bash
python3 -m tools.conveyor.cli smoke   # SMOKE PASS, score 0 — and the result cell
                                      # now also carries score_reloc_blind: 0 (SC-003 half-proof)
```

> **MEASURED (2026-07-08)** — build command used on watchman (full paths):
> `build_toolkit --ido tools/ido-static-recomp/build/out --objdump $(command -v
> mips-linux-gnu-objdump) --permuter tools/decomp-permuter --shim
> tools/conveyor/seeds/shim --out /tmp/toolkit.tar.gz`. New toolkit pinned:
> `b613fc5dc3d01fa7c0c41a542b7617d76e8e41ec9d7611204dbb6f2a5ab6c045` (was
> `23185a79…`). `cli smoke --function strlen`: **SMOKE PASS**, `compile=ok
> score=0`; the stored result cell carries `score_reloc_blind: 0` (verified by
> reading the result blob) — SC-003 half-proof.

## 2. Register + ingest the corpus

```bash
python3 -m tools.conveyor.pipeline.corpus register ultralib reference/repos/ultralib \
    --repo-url https://github.com/decompals/ultralib \
    --include-dirs include,include/compiler/ido,include/PR
python3 -m tools.conveyor.pipeline.corpus ingest
```

Expected: `ultralib: ~686 functions from ~640 files (686 new, 0 updated) @ e24c8367`.
Re-run ingest → `0 new, 0 updated` (idempotence, FR-003). Dirty-clone refusal check:

> **MEASURED (2026-07-08)**: `ultralib: 702 functions from 380 files (702 new,
> 0 updated) @ e24c8367`; re-ingest → `(0 new, 0 updated)`. Dirty-clone check
> refused with exit 1, nothing recorded.
>
> Two **deviations from the plan's assumptions**, both accommodated by the
> feature's existing knobs (no design change):
> 1. **include_dirs** — research D1 said one `include` dir suffices. It does not:
>    `src/gu/guint.h` includes bare `"mbi.h"` and headers cross-reference via
>    `<PR/…>`, so ultralib's real build uses `-I include -I include/compiler/ido
>    -I include/PR` (from `makefiles/ido.mk`). Registered with all three; this is
>    the per-root `include_dirs` column doing its job. Also required extending
>    the shared `resolve_headers` to follow `<…>` includes (previously only
>    `"…"`), so the transitive header closure is complete.
> 2. **duplicate names** — 16 ultralib files carry two `#ifdef`-guarded
>    definitions of one function (e.g. debug/BBPlayer variants). The 3-part
>    `candidate_id` grammar admits one, so ingest keeps the first (deterministic);
>    without this, ingest counts churned every run (non-idempotent). Count is 702
>    unique, not ~686.

```bash
touch reference/repos/ultralib/DIRTY && python3 -m tools.conveyor.pipeline.corpus ingest; echo "exit=$?"
rm reference/repos/ultralib/DIRTY     # expect refusal message + exit=1, nothing recorded
```

## 3. Submit name pairings

```bash
python3 -m tools.conveyor.pipeline.corpus submit --dry-run   # ~85 pairings, 2 flagsets
python3 -m tools.conveyor.pipeline.corpus submit
```

Expected: ~85 pairings, jobs complete on watchman within ~15 min. Immediate
resubmit → 100% cache hits, 0 new jobs (SC-004).

> **MEASURED (2026-07-08)**: `86 name pairings; 5 jobs submitted (172 cells),
> 0 cache hits, skipped: no_target_o=0 unextractable=0`. 85 distinct paired
> targets (one target name-matches two candidate files). Jobs completed on
> watchman (16 cores) in **~6 s** wall — far under 15 min (single-function TUs).
> Immediate resubmit: `0 jobs submitted (172 cells), 5 cache hits` — **SC-004
> holds, 100% cache**.
>
> **Compile/evidence (SC-001):** 146/172 cells compiled ok; **72 of 85 paired
> targets gained scored evidence** — this is **below SC-001's ≥80**. The 13
> uncovered targets are candidates that fail *reduced-TU* compilation (research
> D5): 12 call file-local `static` helpers that reduction strips ("static
> function declared and referenced, but not defined" — the `src/sched/sched.c`
> cluster, `sprintf`, several `io/*` managers), and 1 (`__osInsertTimer`) hits
> the shared brace-matcher's `#ifdef`-conditional-brace limitation. Both are
> properties of the *reduced-TU + shared-extractor* design, recorded rather than
> worked around by substituting a different design (per the handoff). The
> 85-name-match / ≥80 estimate did not account for intra-TU static dependencies.
> Fixing this needs either keeping the target's static callees in the TU
> (breaks single-function scoring) or a preprocessor-aware extractor — a
> follow-up, not a config change.

An extra source-preparation step was required for correctness: the confirmed
flagsets omit IDO's `-Xcpluscomm`, but ultralib is a modern decomp full of `//`
comments, so cfe rejected them as syntax errors (and apostrophes inside `//`
comments produced "unterminated char constant"). `corpus submit` now strips
comments (literal-aware, score-neutral) from the bundled source and headers.
This keeps the two confirmed flagsets **unchanged** (research D6) — comments
never affect codegen.

## 4. Ingest results + inspect

```bash
python3 -m tools.conveyor.pipeline.corpus ingest-results
python3 -m tools.conveyor.pipeline.corpus report
```

Expected (SC-001, SC-002, SC-003, SC-005):

- ≥80 targets with scored evidence in the per-target table.
- `osCreateMesgQueue`: best_true > 0, best_reloc_blind = 0, flagged
  `reloc_only_diff`, artifact at `work/.../osCreateMesgQueue/corpus_match.c` with
  full provenance header.
- `strlen`, `guMtxIdentF`: best_true = 0 AND best_reloc_blind = 0, following the
  normal promotion-eligibility path (they're already locked; nothing regresses).
- Flag summary distinguishes true-0 / reloc_only_diff / neither with counts.

> **MEASURED (2026-07-08)** — `ingest-results`: `ingested 181 new cells from 26
> done jobs (5 stale-toolkit jobs discarded)` / `rankings: 33 targets ->
> candidate_identified, 39 flagged no_ancestry, 20 high-confidence` /
> `reloc_only_diff: 19 flagged (19 newly), 19 artifacts written`.
> `corpus report`: `pairing coverage: 85 targets name-match a corpus candidate;
> 72 have scored evidence` and `flag summary: 12 true-0 (promotion path), 19
> reloc_only_diff, 41 neither` (SC-005 answered in one command).
>
> Recorded acceptance evidence (`corpus report --target …`, SC-002 + SC-003):
>
> ```
> osCreateMesgQueue:  true=20  reloc_blind=0  ultralib:src/os/createmesgqueue.c:osCreateMesgQueue  [-g0 -O1 …]   (SC-002)
> strlen:             true=0   reloc_blind=0  ultralib:src/libc/string.c:strlen                     [-g0 -O2 …]   (SC-003)
> guMtxIdentF:        true=0   reloc_blind=0  ultralib:src/gu/mtxutil.c:guMtxIdentF                 [-g0 -O2 …]   (SC-003)
> ```
>
> `osCreateMesgQueue` → `status=candidate_identified human_flag=reloc_only_diff`,
> artifact at `work/libultra/os/osCreateMesgQueue/corpus_match.c` with the full
> provenance header (Origin/Source@commit/Flags/Scores). SC-001's ≥80 was not
> reached (72 — see §3 note).

## 5. Honesty gates (SC-006, SC-007)

```bash
python3 -m tools.conveyor.cli report            # function_status: no target moved to
                                                # matched/verified by this feature alone
python3 -m tools.conveyor.pipeline.lock check   # lock intact, unchanged entries
git status --short work/                        # only corpus_match.c artifacts, nothing in src/
```

Confirm no `reloc_only_diff` target appears in `matched.lock.json` and no commit
touched `src/`.

> **MEASURED (2026-07-08)** — `cli report`: `functions: 1131 tracked, 0 matched
> (0%), 0 verified` and `matched=0 verified=0` — **nothing moved to
> matched/verified by this feature** (SC-007). `reloc_only_diff` count in the
> attention queue: **19**. `lock check`: `all 2 locked functions intact`;
> `matched.lock.json` sha1 unchanged (`0cb02974…`). `git status --short work/`:
> only the 19 `work/**/corpus_match.c` artifacts; `git status --short src/`
> shows nothing from this feature (the lone `src/game/game.c` entry is a
> pre-existing, unrelated working-tree change left untouched).

## 6. Suite

```bash
python3 -m pytest tests/conveyor -m "not node_required" -q   # green, including
                                                             # test_corpus.py + test_reloc_blind.py
```
