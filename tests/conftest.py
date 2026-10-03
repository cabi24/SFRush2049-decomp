"""Shared by tests/conveyor and tests/cloud.

A test that skips because a tool is missing reports green. Where the run is
supposed to have that tool (CI installs IDO and MIPS binutils), the skip is a
broken environment, not an exclusion: with REQUIRE_TOOLCHAIN=1 it fails.
Skips for absent private inputs (baserom, conveyor DB, corpus data) remain
skips: no runner is supposed to have them.
"""
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
