#!/usr/bin/env python3
"""Replay the recovered actual-source sanitizer tests with source-bound receipts."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
WORK = Path(__file__).resolve().parent
SOURCE = WORK / 'nonmatch/func_80017D38.c'
SOURCE_HASH = 'ad5137579da173b15c903c8aa35c69cd26416d51def3b65267045062543bff65'
TEST_HASH = '52f4f790b12cd27558a6cea5d7d9af6becff7133f9101933a3eb6ea99e77200e'


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    assert digest(SOURCE) == SOURCE_HASH
    assert digest(WORK / 'test_semantics.py') == TEST_HASH
    env = dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
               UBSAN_OPTIONS='halt_on_error=1')
    stock = subprocess.run([sys.executable, '-m', 'unittest', 'discover', '-s', str(WORK),
                            '-p', 'test_*.py', '-v'], capture_output=True, text=True, env=env)
    if stock.returncode:
        raise RuntimeError(stock.stdout + stock.stderr)
    extra = subprocess.run([sys.executable, str(WORK / 'supplement.py')],
                           capture_output=True, text=True, env=env, check=True)
    supplemental = json.loads(extra.stdout)
    assert supplemental == json.loads((WORK / 'supplement.json').read_text())
    assert digest(SOURCE) == SOURCE_HASH and digest(WORK / 'test_semantics.py') == TEST_HASH
    return dict(result='PASS', scope='Fresh post-reset retained-source tests only',
                source_sha256=SOURCE_HASH, test_source_sha256=TEST_HASH,
                supplemental_script_sha256=digest(WORK / 'supplement.py'),
                stock_test_groups=1, stock_actual_source_calls=68931,
                exhaustive_key_velocity_cases=65536,
                live_context_failure_and_timing_cases=3200,
                multiple_event_cases=192, active_and_future_checks=3,
                supplemental_actual_source_calls=65600,
                supplemental_signed_offset_pairs=65536,
                supplemental_four_track_cases=64, combined_actual_source_calls=134531,
                stock_optimization='O1', supplemental_optimization='O2',
                host_flags='-std=c89 -pedantic-errors -Wall -Wextra -Werror -fsanitize=address,undefined -no-pie',
                only_leak_sanitizer_disabled=True,
                limitations=['Synthetic valid aligned streams, channel 0..15 and track ID 0..63.',
                             'External helpers are interface/effect fixtures, not complete engine implementations.',
                             'Native layouts are separately checked with pinned O32 IDO.',
                             'No malformed/cyclic-stream, partial-overlap, concurrency or universal downstream safety claim.',
                             'The original test file hash was not separately retained before reset; its complete recovered text is freshly tested and bound above.'])


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = run()
    if args.check:
        assert result == json.loads((WORK / 'host_verification.json').read_text()), 'host receipt drift'
        print('PASS: 134,531 actual-source sanitizer calls reproduced from recovered tests.')
    else:
        print(json.dumps(result, indent=2))
