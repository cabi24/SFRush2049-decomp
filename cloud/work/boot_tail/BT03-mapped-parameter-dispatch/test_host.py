#!/usr/bin/env python3
"""Compile actual C89 source with ASan/UBSan and execute a memory-wide oracle."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import tempfile
import verify

HERE = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='bt03-parameter-host-') as temp:
        binary = Path(temp) / 'test'
        command = ['gcc', '-std=c89', '-Wall', '-Wextra', '-Werror', '-O2',
                   '-fsanitize=address,undefined', '-fno-sanitize-recover=all', '-fno-omit-frame-pointer',
                   '-include', str(verify.SOURCE), str(HERE / 'host_sanitized.c'), '-o', str(binary)]
        subprocess.run(command, check=True, capture_output=True)
        environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0')
        result = subprocess.run([str(binary)], check=True, capture_output=True, text=True, env=environment)
        report = json.loads(result.stdout)
        assert report['calls'] == 76024
        report.update(source_sha256=verify.sha(verify.SOURCE),
                      harness_sha256=verify.sha(HERE / 'host_sanitized.c'),
                      compiler=subprocess.check_output(['gcc', '--version'], text=True).splitlines()[0],
                      sanitizer_stderr=result.stderr,
                      leak_detection='disabled: LeakSanitizer cannot run under the execution environment ptrace; tested code performs no dynamic allocation',
                      coverage='every u16 duration, all valid selectors, all 256 type values, complete record bytes and layout')
    content = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(content)
    print(content)


if __name__ == '__main__':
    main()
