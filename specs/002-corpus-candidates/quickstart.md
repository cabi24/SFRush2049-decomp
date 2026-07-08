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

## 2. Register + ingest the corpus

```bash
python3 -m tools.conveyor.pipeline.corpus register ultralib reference/repos/ultralib \
    --repo-url https://github.com/decompals/ultralib --include-dirs include
python3 -m tools.conveyor.pipeline.corpus ingest
```

Expected: `ultralib: ~686 functions from ~640 files (686 new, 0 updated) @ e24c8367`.
Re-run ingest → `0 new, 0 updated` (idempotence, FR-003). Dirty-clone refusal check:

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

## 5. Honesty gates (SC-006, SC-007)

```bash
python3 -m tools.conveyor.cli report            # function_status: no target moved to
                                                # matched/verified by this feature alone
python3 -m tools.conveyor.pipeline.lock check   # lock intact, unchanged entries
git status --short work/                        # only corpus_match.c artifacts, nothing in src/
```

Confirm no `reloc_only_diff` target appears in `matched.lock.json` and no commit
touched `src/`.

## 6. Suite

```bash
python3 -m pytest tests/conveyor -m "not node_required" -q   # green, including
                                                             # test_corpus.py + test_reloc_blind.py
```
