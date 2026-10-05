#!/usr/bin/env python3
"""Host-only layout/semantic checks; compiler proof never includes these mocks.

Run from any directory: python3 cloud/work/frontier/dot_route_tracker/host_tests.py
The 64-bit host's route-pointer ABI is not the N64 ABI. The pointer-free tracker
layout is validated exactly; native pointer fields remain a separate proof.
"""

import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

HERE = Path(__file__).resolve().parent


def run():
    compiler = shutil.which("gcc")
    if compiler is None:
        raise RuntimeError("gcc is required; host tests have not run")
    results = []
    common = [
        "-std=c89", "-pedantic-errors", "-O2", "-Wall", "-Wextra",
        "-Werror", "-Wno-unused-but-set-variable", "-ffp-contract=off",
    ]
    configurations = [
        ("gcc_c89_o2", []),
        ("gcc_c89_o2_asan_ubsan", [
            "-g", "-fsanitize=address,undefined,float-cast-overflow",
            "-fno-sanitize-recover=all", "-fno-omit-frame-pointer", "-no-pie",
        ]),
    ]
    with tempfile.TemporaryDirectory(prefix="route-tracker-host-") as temp:
        for label, extra in configurations:
            executable = Path(temp) / label
            command = [compiler, *common, *extra, str(HERE / "host_test.c"),
                       "-o", str(executable)]
            subprocess.run(command, check=True, text=True, capture_output=True)
            env = dict(os.environ)
            env["ASAN_OPTIONS"] = "detect_leaks=0:halt_on_error=1"
            env["UBSAN_OPTIONS"] = "halt_on_error=1:print_stacktrace=1"
            output = subprocess.check_output(
                [str(executable)], env=env, text=True, stderr=subprocess.STDOUT
            )
            controls = []
            for argument, expected_failure in [
                ("--negative-index19", "state mismatch:"),
                ("--negative-total", "totals mismatch"),
            ]:
                result = subprocess.run([str(executable), argument], env=env,
                                        text=True, capture_output=True)
                if result.returncode != 1 or expected_failure not in result.stderr:
                    raise AssertionError(
                        f"Negative control {argument} did not detect its injected error: "
                        f"exit={result.returncode}, stderr={result.stderr!r}"
                    )
                controls.append({"control": argument, "result": "REJECTED_AS_EXPECTED",
                                 "exit_code": result.returncode, "diagnostic": result.stderr.strip()})
            results.append({"configuration": label, "flags": common + extra,
                            "result": "PASS", "output": output.strip(),
                            "test_only_negative_controls": controls})
    return {
        "result": "PASS",
        "compiler": subprocess.check_output([compiler, "--version"], text=True).splitlines()[0],
        "runs": results,
        "cases_per_run": {"mixer": 395264, "priority": 23094, "total": 418358},
        "matrix": {
            "tracker_count": "every integer 0..10",
            "route_count": "every integer 0..15",
            "first_last": "every pair 0..count-1; (0,0) for empty count",
            "modes": [-32768, -7, -1, 0, 1, 2, 3, 4],
            "geometry_profiles": 8,
            "priority_queries": "each who 0..9, route 0..16 and 32767, all route counts and profiles",
            "known_priority_cases": "18 hand-expected fixtures, each with two out-of-range checks",
        },
        "limits": [
            "Host C/oracle comparison, not execution against native target instructions.",
            "Only func_800BA61C, func_800BA2B8, and audio_channel_alloc are deterministic host doubles.",
            "The real audio_priority_find and audio_mixer_main bodies are included unchanged from group.c.",
            "Compiler proof must use accepted real callees, never this test translation unit.",
            "Exact 812-byte tracker layout; 64-bit route pointer layout does not prove the N64 pointer ABI.",
            "Finite floats and valid native input bounds; invalid negative route/who, invalid counts, modes above 4, and point-counter overflow are excluded.",
            "Route point counts cover 0..8 and 16, not all possible u16 values; no NaN/Inf behavior or exhaustive floating-point proof.",
            "Leak detection disabled; no heap allocation occurs in the C harness.",
        ],
        "source_sha256": {
            name: hashlib.sha256((HERE / name).read_bytes()).hexdigest()
            for name in ("group.c", "host_test.c", "host_tests.py")
        },
    }


if __name__ == "__main__":
    print(json.dumps(run(), indent=2))
