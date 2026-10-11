# w19d: test-speed tooling (pytest-xdist, second CI-equivalent host)

Status: DONE (2026-10-11). Commits on the lane branch:
`a32de59d` xdist support / `make test-fast` / `tools/test_write_audit.py`;
`c6e5b52c` `tools/ci_equiv.sh` + docs/BUILDING.md (CI-equivalent hosts, rocky,
fast runs) + handoff §3 step 6 pointer; this file. Not pushed.

## Timings

Selection everywhere: `REQUIRE_TOOLCHAIN=1 ... -m "not node_required"`. Every
run below gave the same counts: conveyor **2152 passed, 51 skipped**; cloud
**909 passed, 1 skipped, 27 subtests passed**. No failures in any parallel run
(6 full parallel runs: 2 on watchman2, 4 on rocky).

| Host / mode | conveyor | cloud | both |
|---|---:|---:|---:|
| watchman2, serial (CI command), `taskset -c 0-3` | 140.0 s | 397.2 s | 537 s |
| watchman2, `make test-fast JOBS=4`, `taskset -c 0-3` | **37.0 s** | **125.3 s** | **162 s** (3.3x) |
| watchman2, serial, unpinned (ci_equiv, commit c6e5b52c, civenv without xdist) | 138 s | 386 s | 524 s |
| watchman2, `CI_FAST=4` unpinned (ci_equiv) | 38 s | 111 s | 149 s |
| rocky, serial (ci_equiv, master e46f0645) | 119 s | 308 s | 427 s |
| rocky, `CI_FAST=8` | 37 s | 96 s | 133 s |
| rocky, `CI_FAST=16` | 31 s | 90 s | 121 s |
| rocky, `CI_FAST=20` | **31 s** | **80 s** | **111 s** (3.8x) |

Times are pytest's own plus about 1 s for the serial pass. Beyond about 8 workers the
cloud suite is bound by single files: `test_dot_nearest_path_context.py` (one
module fixture, 52 s serial on watchman2, 79 s under -n 16 contention) and
`test_mutate.py` (60 s); conveyor by `test_runtime_b_reset_match.py` and the
two IDO groups below.

## Serial list and concurrency groups

**No test is marked `serial`.** The strace audit (every file write of a serial
run attributed to its test, both suites, rocky) found no test that needs to run
alone: no writes to `$HOME`, the conveyor DB, `.git`, `/tmp/blob*`, or
`build/blob_unit/gate` in CI-equivalent runs (those paths are only touched
through the builder/ssh paths, which the suites mock). Writes under `build/`
go to per-packet directories (`build/menu_row_caller`, `build/list_control`,
...) written and read by one test file each. The amatch cache is content-addressed
and written by rename. The two tracked receipts rewritten during the run
(`cloud/work/dot_vertex_bounds/verification.json`, `dot_pool_object/...`) are
read only by their own test file.

The one cross-file hazard: **IDO `-O3` writes `<source basename>.u` into the
current directory** (`cc -O3` -> `ujoin -o candidate.u`, then `uld` reads it;
verified directly on rocky; plain `-O2` does not). Tests run from the repo
root, so two files compiling a same-named source at once race on
`./candidate.u`. Those files share an xdist group (`tests/conftest.py`
`CONCURRENCY_GROUPS`) and run one after another on one worker:

| group | files (shared .u) |
|---|---|
| ido-o3-cwd-candidate-layout | conveyor: animation_setter_contract (layout), cone_start_donor (candidate, layout), dot_constructor_prepend (candidate), dot_target_overlap (candidate, layout), dot_target_sphere (candidate, layout), entity_vector_boundaries (candidate), frontier_resource_initializer (candidate, layout); cloud: cone_motion_donor, dot_model_clock, dot_quadtree_donor, radar_traffic_contract, runtime_a_float_product, runtime_a_settings_bar, task_enqueue_research (candidate), heap_max_contract, object_sound_match (abi), target_orientation_research (abi, candidate) |
| ido-o3-cwd-locked-functions | conveyor: cloud_score (every locked function by name), dot_entity_motion (func_8010E4E4), dot_fresh_small_20261005 (func_800A43FC/800D4DFC/800DC628), dot_matrix_row02 (func_800C40E8), dot_matrix_rows (func_800ACFF8), dot_slot_setter_boundary_20261006 (func_80092BF4, sfx_stop), encoded_string_mapping_reconstruction (func_800A150C), name_lookup_caller_contract (collision_sound_play, func_800B24EC) |
| ido-o3-cwd-layout | cloud: layer_update_packet, node_pool_init_match (layout) |
| ido-o3-cwd-best | cloud: dot_fresh_masked_rng, texture_rect_verification (best) |

Every other test file is its own group (`--dist loadgroup`), so tests in a file
keep their order and module fixtures, as in the serial run. Re-running
`tools/test_write_audit.py` on rocky after the change reports "2 implied
group(s), 0 not covered" for conveyor and "3 implied group(s), 0 not covered"
for cloud.

Guards in tests/conftest.py: `-n` without `--dist loadgroup` is a usage error;
a renamed or removed file in `CONCURRENCY_GROUPS` is a usage error under `-n`;
a `serial` test selected under xdist fails with instructions. Runs without
xdist (CI, the Pi, civenv) are unchanged: the hooks return early and
`numprocesses` is absent.

Audit limitations: reads are not traced, so a test that only *reads* another
file's output would not show (checked by hand for every fixed `build/` dir
found: each name occurs only in its own test and packet). A test that fails
under strace stops its writes early: `test_selector_d9058_contract.py::test_fresh_frozen_replay`
fails there because LeakSanitizer refuses to run under ptrace (passes normally;
its writes go to `build/selector_caller_contract/replay-<random>`).

## Commands

```bash
# parallel run of both suites (needs pytest-xdist in the venv)
REQUIRE_TOOLCHAIN=1 make test-fast JOBS=4 PYTEST=~/rush2049/scratch/civenv/bin/python
# which runs, per suite:
python -m pytest tests/<suite> -rs -p no:cacheprovider -n 4 --dist loadgroup -m "(not node_required) and not serial"
python -m pytest tests/<suite> -rs -p no:cacheprovider -m "(not node_required) and serial"

# CI-equivalent run from the Pi (bundle HEAD + remote branches)
git bundle create /tmp/X.bundle HEAD --remotes=origin
tools/ci_equiv.sh watchman2 /tmp/X.bundle            # plain, ciclone6 + civenv
tools/ci_equiv.sh rocky /tmp/X.bundle                # plain, rocky's ci.env
CI_FAST=20 tools/ci_equiv.sh rocky /tmp/X.bundle     # make test-fast JOBS=20
# a lane on watchman2 (own clone, logs and bundle dir):
CI_CLONE=$HOME/rush2049/scratch/ciclone-w19d CI_LOGS=$HOME/rush2049/scratch/frontier/w19d/ci-logs \
  CI_BUNDLES=rush2049/scratch/frontier/w19d tools/ci_equiv.sh watchman2 /tmp/X.bundle

# re-derive the concurrency groups (Linux + strace; ~3 min conveyor, ~6 min cloud)
python3 tools/test_write_audit.py tests/conveyor
python3 tools/test_write_audit.py tests/cloud
```

## rocky setup (done, no sudo)

`~/rush2049/scratch/{ciclone,civenv,ci.env,ci-bundles,ci-logs,w19d}`,
`~/rush2049/opt/{mips-binutils,sanitizers,rpms,src}`. IDO 5.3 + 7.1 by
`tools/cloud/setup.sh` (sha256-verified). MIPS binutils 2.45 built from the GNU
tarball (sha256 c50c0e7f...bfbef5d2, full value in BUILDING.md). gcc was present but its sanitizer runtimes
were not (every -fsanitize host test failed at link: `cannot find /usr/lib64/libubsan.so.1.0.0`);
fixed without sudo by unpacking the Rocky-signed libasan/libubsan RPMs (signature
verified against /etc/pki/rpm-gpg/RPM-GPG-KEY-Rocky-10) and exporting
LIBRARY_PATH/LD_LIBRARY_PATH in ci.env. `sudo dnf install libasan libubsan`
would make that unnecessary. Skip reasons on rocky are identical to watchman2's
(both suites diffed), including `test_ido_syntax.py` (no mips-linux-gnu-gcc on
either host). The pre-existing `~/rush2049/{agent,cache,tmp}` on rocky were not touched.

## watchman2 footprint

Only `~/rush2049/scratch/frontier/w19d` (venv with civenv's pins + pytest-xdist
3.8.0, logs, bundles) and `~/rush2049/scratch/ciclone-w19d` (copy of ciclone6).
civenv and ciclone6 were not modified. A first ci_equiv run created
`~/rush2049/scratch/ci-logs` and `ci-bundles` with only this lane's files. Those were
moved into the lane directory and removed; the script now takes `CI_LOGS` and
`CI_BUNDLES` overrides.

## Follow-ups for the coordinator

- To use `CI_FAST` on watchman2 with the default venv: `~/rush2049/scratch/civenv/bin/pip install pytest-xdist==3.8.0`.
- CI itself (verify.yml) is unchanged. Adding `pytest-xdist` there and
  `make test-fast JOBS=4` would cut the 20-minute job, but that is an owner decision.
- The root-cause fix for the `.u` hazard would be running IDO `-O3` compiles
  with `cwd=` a temp directory. There are about 700 call sites (score.py plus
  research packets), so the groups are the low-risk fix.
