# w18e — runtime-image tooling merge (w18a + w18b) and the end-to-end landing path

Status: DONE (branch `worktree-agent-a1aa453c1435946f8`, not pushed). Verdict: the overlay
landing path is real end to end: locked C -> image gate -> rebuilt image -> exact stream ->
cartridge -> `ROM matches!`.

## Commands the coordinator runs (main checkout on the Pi, in order)

```bash
git merge --ff-only worktree-agent-a1aa453c1435946f8          # master + w18a merge + 3 commits
python3 -m pytest -q tests/conveyor -m "not node_required" -p no:cacheprovider --continue-on-collection-errors
python3 -m tools.conveyor.pipeline.ovl_image check             # constants + exact recompression vs the ROM
python3 -m tools.conveyor.pipeline.ovl_image extract           # build/ovl/{a,b}/image.bin
python3 -m tools.conveyor.pipeline.ovl_layout build --passthrough
python3 -m tools.conveyor.pipeline.ovl_splice rebuild          # IDO on watchman2 ($OVL_BUILDER_DIR, default /tmp/ovlsplice)
python3 -m tools.conveyor.pipeline.ovl_layout build            # gate with every locked body; publishes build/ovl/<x>/rebuilt.bin
python3 -m tools.conveyor.pipeline.blob_rom produce            # only if build/blob/game_code.deflate is absent/stale
python3 -m tools.conveyor.pipeline.ovl_rom rom --drill         # streams + sync + build + make test on ~/rush2049/repo (-j16)
make progress
```

Do this BEFORE any other ROM build after merging: the Makefile now needs
`build/ovl/ovl_{a,b}.deflate`, and `blob_rom rom` refuses without them. Static `promote`
builds on the builder use whatever is already in `~/rush2049/repo/build/`, so they keep
working once `ovl_rom rom` has synced the streams there once.

## Commits

1. `89f3c2a1` merge of w18a's seven commits as they are. Conflicts: README keeps both
   sections; `ovl_image.py` (add/add) keeps w18b's module verbatim in this commit.
2. `f46c41df` unification: one `ovl_image` (w18b's STREAMS, gated inflate, exact `deflate`/
   `prove`/`check`) plus w18a's `IMAGES` (same dict), `ImageError` (alias), `target_dir`,
   `expected_sha256`, `image_path`, `ensure`, `inflate_stream`, `text_end` per Stream.
   ovl_layout, ovl_splice, ovl_rom import only it. Bug fixed: `ovl_rom verify` did
   `rmtree(build/ovl/<x>/obj)`, which would have deleted ovl_splice's locked objects; it now
   works in `build/ovl/verify/<x>`.
3. `5d6a58a8` compose by default (decision: default on, mirroring the game blob; no opt-out,
   since an opt-out would keep data.bin's extracted bytes in the slots).
   `ovl_layout build` publishes `rebuilt.bin` + `rebuilt.json` (sha256, lock digest);
   `ovl_rom produce|rom` compress it by default and refuse missing / altered / stale-lock
   images and drifted sources before writing anything (`--retail-stand-in` is the explicit
   round-trip mode, `--image-file` stays). `blob_rom` and `ovl_rom` sync the same set
   (SYNC_FILES + blob + both streams) and refuse when one is absent.
4. this RESULTS.

## Proof (Pi worktree + watchman2 `~/rush2049/scratch/frontier/w18e`, -j2)

Scratch: `rom/` = this worktree's tracked files + baserom, `build/out` -> toolkit 796ae99a
ido, `build71/out` copied from w18b's scratch; game blob copied from the main checkout
(verified == ROM stream at 0xB0CB10); splice compiles in `w18e/ovl`.

- `rm -rf build/ovl; ovl_image extract; ovl_layout build --passthrough`: A and B
  byte-identical.
- Fail-closed: `ovl_layout build` with no objects -> LockedBodyError (36 bodies), nothing
  published; `ovl_rom produce` -> "no rebuilt image ... run ovl_layout build".
- `OVL_BUILDER_DIR='~/rush2049/scratch/frontier/w18e/ovl' ovl_splice rebuild` (18 s):
  A 36 bodies, B 8 bodies, both images byte-identical.
- `ovl_layout build`: "36 locked C bodies" / "8 locked C bodies", byte-identical, rebuilt
  images published (25 singles + 12 groups = 44 lock entries).
- `ovl_rom rom --drill --builder-repo ~/rush2049/scratch/frontier/w18e/rom --jobs 2`:
  streams from the rebuilt images == ROM; **ROM matches!, MAKE=0 TEST=0**; drill A (rebuilt
  image byte +0x17B28) and drill B (+0x55B8): `ROM does NOT match`, MAKE=2 TEST=2; restored
  SHA-1 EXACT.
- Extra drill: with `build/ovl/ovl_b.deflate` removed, `make` stops with "No rule to make
  target 'build/ovl/ovl_b.deflate'"; restored -> ROM matches!.

## Landing a new overlay single or group (Pi, main checkout)

```bash
# 0. supersession only: drop what the new body replaces
python3 -m tools.conveyor.pipeline.ovl_splice revert --targets asm/us/ovl_a func_X
python3 -m tools.conveyor.pipeline.ovl_splice revert-group --targets asm/us/ovl_a GROUP
# 1. splice (each runs IDO on the builder, link/owndata/extent gates and the whole-image gate;
#    a refusal leaves lock and sources unchanged)
python3 -m tools.conveyor.pipeline.ovl_splice splice --targets asm/us/ovl_a path/func_X.c
python3 -m tools.conveyor.pipeline.ovl_splice splice --all-matches            # cloud/matches/ovl_{a,b}
python3 -m tools.conveyor.pipeline.ovl_splice group --targets asm/us/ovl_a cloud/work/frontier/<packet>
# 2. gates
python3 -m tools.conveyor.pipeline.ovl_layout build      # both images byte-identical; republishes rebuilt.bin
python3 -m tools.conveyor.pipeline.ovl_splice check      # also the pre-commit hook
# 3. cartridge
python3 -m tools.conveyor.pipeline.ovl_rom rom           # refuses a rebuilt image older than the lock
make progress
git add ovl_matched.lock.json src/ovl_a src/ovl_b && git commit
```

A fresh checkout (or a Pi whose `build/ovl` was cleaned) has no objects: run
`ovl_image extract; ovl_splice rebuild; ovl_layout build` before `ovl_rom rom` (both
`ovl_layout build` and `ovl_rom` fail closed until then).

## Tests

`pytest tests/conveyor -m "not node_required"` in this worktree (baserom + build/blob +
build/game_code.bin copied in): 33 failed + 2 collection errors, all environmental. The
same failures reproduce in a clean `git archive master` export (checked for
agent_loop, autodecomp_population, ido_syntax, reloc_extracted, reloc_split,
search_cleanup); the rest import the untracked `src.scorer` (26 ModuleNotFoundError,
incl. test_group_search, unit/test_reloc_blind, unit/test_target_assembly,
unit/test_hardening_006, unit/test_scoring, unit/test_typed_target_aliases) or need the
untracked decomp-permuter / m2c submodule. No real failures. All ovl_*, blob_rom,
cloud_guard, cloud_submissions and owned_data tests pass (141 passed, 1 skipped), including new ones for
the rebuilt-image default, stale/missing/altered refusal, drift refusal, sync refusal and
publish/withdraw.

## For the coordinator

- Not edited (instruction files, need the owner's call): CLAUDE.md line "The ROM build
  requires `build/blob/game_code.deflate`" should also name `build/ovl/ovl_{a,b}.deflate`;
  `.claude/skills/promote-match/SKILL.md` step 3 should add `ovl_layout build` +
  `ovl_rom rom` for overlay bodies.
- `make progress` still sums only static + game in "linked from C"; overlays have their
  own rows (now worded "compressed into the ROM at 0x...").
- Builder `/tmp` was used once for a hold file during the missing-stream drill
  (`/tmp/w18e-ovl_b.hold`, moved back).
