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
