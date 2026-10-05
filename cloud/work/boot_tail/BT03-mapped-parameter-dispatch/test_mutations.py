#!/usr/bin/env python3
"""Negative controls change temporary test copies only, never the retained C."""
import argparse
import json
from pathlib import Path
import struct
import subprocess
import tempfile
import test_semantics as semantics
import verify

HERE = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    source = verify.SOURCE.read_text()
    mutations = {
        'wrong_type_for_selector_250': source.replace('case 250:\n        type = 2;', 'case 250:\n        type = 3;'),
        'wrong_delta_divisor': source.replace(' / time;', ' / (time + 1);'),
        'missing_conversion_call': source.replace('    func_8001E930(&ltime);', '    ltime *= 256U;'),
    }
    results = []
    with tempfile.TemporaryDirectory(prefix='bt03-parameter-negative-') as temp:
        folder = Path(temp)
        for name, mutated in mutations.items():
            assert mutated != source
            candidate = folder / (name + '.c')
            candidate.write_text(mutated)
            binary = folder / name
            subprocess.run(['gcc', '-std=c89', '-O2', '-include', str(candidate),
                            str(HERE / 'host_sanitized.c'), '-o', str(binary)], check=True, capture_output=True)
            result = subprocess.run([str(binary)], capture_output=True, text=True)
            assert result.returncode != 0, name
            assert 'Assertion' in result.stderr, result.stderr
            results.append(dict(mutation=name, oracle_rejected=True))
    proof = json.loads((HERE / 'dispatch_proof.json').read_text())
    mapping = [int(row['target'], 16) for row in proof['selector_to_target']]
    mapping[0], mapping[1] = mapping[1], mapping[0]
    targets = verify.score.targets()
    machine = semantics.Machine(targets[verify.NAME], verify.raw(mapping), targets['func_8001E930'])
    before = semantics.fixture(0)
    correct, _ = semantics.expected(before, 127, 17, 250)
    assert machine.run(before, [127, 17, 250]) != correct
    results.append(dict(mutation='swapped_actual_jump_table_cases_250_251', oracle_rejected=True))
    report = dict(result='PASS', source_sha256=verify.sha(verify.SOURCE), negative_controls=results)
    content = json.dumps(report, indent=2) + '\n'
    if args.output:
        args.output.write_text(content)
    print(content)


if __name__ == '__main__':
    main()
