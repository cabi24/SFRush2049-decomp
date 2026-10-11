"""Shared by tests/conveyor and tests/cloud.

A test that skips because a tool is missing reports green. Where the run is
supposed to have that tool (CI installs IDO and MIPS binutils), the skip is a
broken environment, not an exclusion: with REQUIRE_TOOLCHAIN=1 it fails.
Skips for absent private inputs (baserom, conveyor DB, corpus data) remain
skips: no runner is supposed to have them.
"""
import functools
import inspect
import os
import re
from pathlib import Path

import pytest

TOOLCHAIN_SKIP = re.compile(
    r"\bIDO\b|binutils|mips-linux-gnu|objdump|mips_to_c|C compiler|gcc absent",
    re.IGNORECASE)


def skip_reason(report):
    longrepr = report.longrepr
    if isinstance(longrepr, tuple) and len(longrepr) == 3:
        return str(longrepr[2])
    return str(longrepr)


def is_toolchain_skip(reason):
    return bool(TOOLCHAIN_SKIP.search(reason))


# tools/cloud/score.py's ido() exits (SystemExit) when the pinned IDO is not
# installed. Research packets often call it from fixtures or test bodies without
# their own guard; that is a missing tool, not a failure of the packet, so it is
# reported as a toolchain skip (and therefore still fails under
# REQUIRE_TOOLCHAIN=1, where IDO must be present).
IDO_MISSING = "run tools/cloud/setup.sh"


def _skip_instead_of_missing_ido_exit(outcome):
    # A SystemExit from score.ido(), or a subprocess-based replay whose failure
    # message carries the same missing-IDO diagnostic.
    exc = outcome.exception
    if isinstance(exc, (SystemExit, AssertionError)) and IDO_MISSING in str(exc):
        outcome.force_exception(pytest.skip.Exception("IDO missing: " + str(exc)))


def _missing_ido_skip(exc):
    return pytest.skip.Exception("IDO missing: " + str(exc))


def _guard_fixture(func):
    """Wrap a fixture function so a missing-IDO SystemExit becomes a skip
    *inside* the fixture: pytest's own bookkeeping then sees an ordinary skip
    (a module fixture skips every test that uses it)."""
    if getattr(func, "_ido_guarded", False):
        return func
    if inspect.isgeneratorfunction(func):
        @functools.wraps(func)
        def guarded(*args, **kwargs):
            try:
                result = yield from func(*args, **kwargs)
            except SystemExit as exc:
                if IDO_MISSING in str(exc):
                    raise _missing_ido_skip(exc) from None
                raise
            return result
    else:
        @functools.wraps(func)
        def guarded(*args, **kwargs):
            try:
                return func(*args, **kwargs)
            except SystemExit as exc:
                if IDO_MISSING in str(exc):
                    raise _missing_ido_skip(exc) from None
                raise
    guarded._ido_guarded = True
    return guarded


@pytest.hookimpl(tryfirst=True)
def pytest_fixture_setup(fixturedef, request):
    fixturedef.func = _guard_fixture(fixturedef.func)


@pytest.hookimpl(hookwrapper=True)
def pytest_pyfunc_call(pyfuncitem):
    _skip_instead_of_missing_ido_exit((yield))


@pytest.hookimpl(hookwrapper=True)
def pytest_runtest_makereport(item, call):
    outcome = yield
    report = outcome.get_result()
    if (report.skipped and os.environ.get("REQUIRE_TOOLCHAIN") == "1"
            and not hasattr(report, "wasxfail")):
        reason = skip_reason(report)
        if is_toolchain_skip(reason):
            report.outcome = "failed"
            report.longrepr = ("REQUIRE_TOOLCHAIN=1 but this test skipped for a "
                               "missing tool: " + reason)


# --- pytest-xdist (make test-fast; docs/BUILDING.md "Fast test runs") --------
#
# Under xdist every test file is one scheduling group (`--dist loadgroup`), so a
# file's tests keep their order and share module fixtures on one worker, as in
# a serial run. Files in one CONCURRENCY_GROUPS entry run on the same worker,
# one after another, because they write the same non-unique path. Tests marked
# `serial` never run under xdist; `make test-fast` runs them afterwards, alone.
#
# The groups come from tools/test_write_audit.py (strace of a serial run, every
# file write attributed to the test that made it). Today all of them are IDO
# -O3 compiles: `cc -O3` runs ujoin, which writes `<source basename>.u` into the
# *current directory* (the repository root) and uld reads it back, so two
# files compiling same-named sources at once race on that .u. Re-run the audit
# after adding a test that compiles at -O3 from the repository root.
CONCURRENCY_GROUPS = {
    "ido-o3-cwd-candidate-layout": (
        "tests/conveyor/test_animation_setter_contract.py",   # layout.u
        "tests/conveyor/test_cone_start_donor.py",            # candidate.u layout.u
        "tests/conveyor/test_dot_constructor_prepend.py",     # candidate.u
        "tests/conveyor/test_dot_target_overlap.py",          # candidate.u layout.u
        "tests/conveyor/test_dot_target_sphere.py",           # candidate.u layout.u
        "tests/conveyor/test_entity_vector_boundaries.py",    # candidate.u
        "tests/conveyor/test_frontier_resource_initializer.py",  # candidate.u layout.u
        "tests/cloud/test_cone_motion_donor.py",              # candidate.u
        "tests/cloud/test_dot_model_clock.py",                # candidate.u
        "tests/cloud/test_dot_quadtree_donor.py",             # candidate.u
        "tests/cloud/test_heap_max_contract.py",              # abi.u
        "tests/cloud/test_object_sound_match.py",             # abi.u
        "tests/cloud/test_radar_traffic_contract.py",         # candidate.u
        "tests/cloud/test_runtime_a_float_product.py",        # candidate.u
        "tests/cloud/test_runtime_a_settings_bar.py",         # candidate.u
        "tests/cloud/test_target_orientation_research.py",    # abi.u candidate.u
        "tests/cloud/test_task_enqueue_research.py",          # candidate.u
    ),
    "ido-o3-cwd-locked-functions": (
        # score.py group-compiles every locked function under its own name.
        "tests/conveyor/test_cloud_score.py",
        "tests/conveyor/test_dot_entity_motion.py",           # func_8010E4E4.u
        "tests/conveyor/test_dot_fresh_small_20261005.py",    # func_800A43FC/800D4DFC/800DC628.u
        "tests/conveyor/test_dot_matrix_row02.py",            # func_800C40E8.u
        "tests/conveyor/test_dot_matrix_rows.py",             # func_800ACFF8.u
        "tests/conveyor/test_dot_slot_setter_boundary_20261006.py",  # func_80092BF4.u sfx_stop.u
        "tests/conveyor/test_encoded_string_mapping_reconstruction.py",  # func_800A150C.u
        "tests/conveyor/test_name_lookup_caller_contract.py",  # collision_sound_play.u func_800B24EC.u
    ),
    "ido-o3-cwd-layout": (
        "tests/cloud/test_layer_update_packet.py",            # layout.u
        "tests/cloud/test_node_pool_init_match.py",           # layout.u
    ),
    "ido-o3-cwd-best": (
        "tests/cloud/test_dot_fresh_masked_rng.py",           # best.u
        "tests/cloud/test_texture_rect_verification.py",      # best.u
    ),
}
_GROUP_OF = {path: name for name, paths in CONCURRENCY_GROUPS.items() for path in paths}


def _xdist_worker(config):
    return hasattr(config, "workerinput")


def pytest_configure(config):
    # xdist controller: a distribution mode other than loadgroup ignores the
    # groups above.
    if (getattr(config.option, "numprocesses", None)
            and getattr(config.option, "dist", "no") != "loadgroup"):
        raise pytest.UsageError(
            "parallel runs need --dist loadgroup (tests/conftest.py keeps files "
            "that share paths on one worker); use `make test-fast`")
    if getattr(config.option, "numprocesses", None):
        root = Path(__file__).resolve().parents[1]
        stale = sorted(p for p in _GROUP_OF if not (root / p).is_file())
        if stale:
            raise pytest.UsageError("CONCURRENCY_GROUPS names missing files "
                                    "(renamed? re-run tools/test_write_audit.py): "
                                    + ", ".join(stale))


@pytest.hookimpl(tryfirst=True)
def pytest_collection_modifyitems(config, items):
    if not _xdist_worker(config):
        return
    for item in items:
        if item.get_closest_marker("xdist_group") is None:
            path = item.nodeid.split("::", 1)[0]
            item.add_marker(pytest.mark.xdist_group(_GROUP_OF.get(path, path)))


def pytest_runtest_setup(item):
    if _xdist_worker(item.config) and item.get_closest_marker("serial"):
        pytest.fail("marked serial but selected under pytest-xdist: deselect "
                    "with -m 'not serial' and run it without -n (make test-fast "
                    "does both)", pytrace=False)
