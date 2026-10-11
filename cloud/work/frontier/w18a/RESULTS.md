# w18a — runtime-image (overlay A/B) layout + splice + lock path

Status: DONE (branch `worktree-agent-a8111ac58222be354`, not pushed).

## Commands the coordinator runs (Pi, repo root, after merging)

```bash
python3 -m tools.conveyor.pipeline.ovl_image extract                 # build/ovl/{a,b}/image.bin from baserom (sha-checked)
python3 -m tools.conveyor.pipeline.ovl_layout build --passthrough    # image gate with no C, both images
python3 -m tools.conveyor.pipeline.ovl_splice rebuild                # compile every locked body on watchman2 + gate
python3 -m tools.conveyor.pipeline.ovl_layout build                  # image gate with every locked body
python3 -m tools.conveyor.pipeline.ovl_splice check                  # also run by make check-matched / pre-commit
make progress                                                        # overlay A / overlay B rows
# new work
python3 -m tools.conveyor.pipeline.ovl_splice splice --all-matches   # cloud/matches/ovl_{a,b}/*.c not yet locked
python3 -m tools.conveyor.pipeline.ovl_splice splice --targets asm/us/ovl_a path/func_X.c
python3 -m tools.conveyor.pipeline.ovl_splice group --targets asm/us/ovl_a cloud/work/frontier/<packet>
python3 -m tools.conveyor.pipeline.ovl_splice revert|revert-group --targets asm/us/ovl_<x> NAME
```

Builder directory: `$OVL_BUILDER_DIR` (default `/tmp/ovlsplice`; singles in `<dir>/<x>`, groups in
`<dir>/<x>-group`). This lane ran with `OVL_BUILDER_DIR='~/rush2049/scratch/frontier/w18a/ovl'`.

## Commits

1. `ovl_layout` + shared `ovl_image` (+ small parametrisation of blob_tu/blob_build/blob_splice/blob_group).
2. `ovl_splice` singles + groups, `ovl_matched.lock.json`, check, `make check-matched` + pre-commit hook.
3. Coverage rows in `blob_rom coverage` / `make progress`.
4. README section (tools/conveyor/README.md, "Runtime images A/B").
5. The splice itself: lock + `src/ovl_a`, `src/ovl_b` (drop this commit if you want to re-run it yourself).
6. `guard_paths`: locked `src/ovl_<x>/` sources protected in PRs once the base has the lock.

## Design choice: parametrise, don't copy

The game-code modules already operate on a layout *document*; only two helpers read the game's base
constant. Changes there are keyword arguments with game defaults (no behaviour change, game tests pass):
`blob_tu.render_region(base=, source_dir=)`, `blob_build.locate/describe_difference` read the base from the
document, `blob_splice.compile_on_builder(workdir=, staging=)`, `blob_group.compile_group(workdir=, staging_root=)`.
`ovl_layout` builds a blob_layout-format document from `asm/us/ovl_<x>/extents.json` and calls
`blob_layout.regions`, `blob_tu.render_region`/`render_linker_script`, `blob_build.build`. `ovl_splice` calls
`blob_splice.link_function` (owndata proofs, extent/excess gates) with `image=(retail bytes, 0x8038A400)` and
the image's `symbols.json`, and `blob_group.group_bodies/compile_group` with its own root/obj dir. Its own code
is only what differs: per-image lock, paths, packet `claims` -> members (other members -> context), and a
`spliced_bodies` that reads the image's lock (blob_splice's is bound to the game lock and src/blob/groups).

Layout facts: functions tile `[0x8038A400, text_end)` with no gaps; the data tail (A 41,176 B, B 5,180 B) is one
opaque run; all switch tables (A 25, B 6) are proven inside it (a table inside a function is a LayoutError).
The tail is kept as ONE run on purpose: blob_group's independent-region proof requires each own-rodata window
to lie inside one opaque run, so splitting it would only refuse more.

## Results (Pi + watchman2)

- All-passthrough link: A and B byte-identical (A sha256 `0d6702c3…6667`, B `b55fc2d1…e7cd`).
- **All 25 `cloud/matches/ovl_*` singles spliced, none refused**: A 17 (5,396 B), B 8 (1,640 B) = 7,036 B.
- **12 `dot_runtime_a_*` group packets with claims spliced, none refused**: 19 functions, 5,820 B in A
  (availability_refresh, cleanup_group, config_caller_group, config_parent_group, joint_transforms,
  menu_cleanup, menu_state, option_eligibility, scene_init_group, seven_option_group, seven_option_init,
  text_helpers). The 5 packets with empty claims (availability_producer, object_initializer, option_parent,
  option_scene, render_context) are research and refused by design ("no claims").
- Totals: overlay A 36/193 functions, 12,856/194,128 B (8.41% of 152,952 B text); overlay B 8/49,
  1,640/43,888 B (4.24% of 38,708 B text). Both images byte-identical with every body in place, also from a
  fresh `build/ovl` (`rm -rf build/ovl; extract; rebuild; build`).
- Drills: a one-constant change refuses at the extent comparison with lock, source and object unchanged; a
  one-bit flip in a locked group body fails the whole-image gate naming `func_8039B00C`; a missing object fails
  `ovl_layout build` with LockedBodyError (never passthrough); an edited locked source fails the pre-commit hook.
- None of the 25 singles has own data; group members' own data went through blob_group's owndata proofs.

## Gaps / for the coordinator

- Image coverage only. The rows say "image only, stream … not yet composed"; composing the streams into the
  ROM (and the full-ROM SHA-1) is w18b's `ovl_rom`. Shared interface: `tools/conveyor/pipeline/ovl_image.py`
  (`IMAGES`, `inflate(image, rom=None)`, `inflate_stream(rom, offset)`, `image_path`, `ensure`). w18b's version
  was not present, so this is the minimal one; ovl_rom's own inflate can switch to it. stdlib zlib is used to
  inflate (deterministic; result hash-checked against extents.json); only compression needs zlib 1.0.4.
- `make progress` still prints "linked from C" as static + game only (labelled); overlays are never summed in.
- Astra's PR #309/#310 bodies are not in the repo, so not spliced.
- No `--revert-group/--revert-single` supersession flags like `install_group.py`; use `revert`/`revert-group`
  then `group`.
- Objects live in `build/ovl/<x>/obj` (Pi only); a fresh checkout must `ovl_splice rebuild` before
  `ovl_layout build` can gate locked bodies (fail-closed otherwise).
- Pre-existing, unrelated test collection errors in this worktree (`src.scorer` missing): test_group_search,
  unit/test_reloc_blind and ~30 other unit tests; all blob/ovl/guard tests pass.
