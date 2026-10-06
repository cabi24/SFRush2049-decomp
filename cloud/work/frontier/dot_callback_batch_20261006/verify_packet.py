#!/usr/bin/env python3
"""Check authored packet sources and selected native words, not the live tree."""
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]

def digest(data):
    return hashlib.sha256(data).hexdigest()

def verify(tool_root):
    receipt = json.loads((HERE / 'bindings.json').read_text())
    for name, expected in receipt['authored_sources'].items():
        path = Path(name)
        assert not path.is_absolute() and '..' not in path.parts
        assert path.parts[:3] == ('cloud', 'work', 'frontier')
        assert path.suffix in ('.c', '.py', '.json') and not path.name.startswith('test_')
        assert digest((ROOT / path).read_bytes()) == expected, name
    for population, functions in receipt['native_targets'].items():
        # Fresh module state per population keeps the canonical scorer isolated
        # from other packet tests. No tool source or target file is changed.
        name = 'callback_binding_score_' + population
        spec = importlib.util.spec_from_file_location(name, Path(tool_root) / 'tools/cloud/score.py')
        score = importlib.util.module_from_spec(spec)
        # Direct-file loading uses the scorer's sibling-import fallback. Keep
        # both its search path and temporary modules local to this population.
        absent = object()
        prior_path = sys.path[:]
        prior_modules = {key: sys.modules.get(key, absent)
                         for key in (name, 'owndata')}
        try:
            sys.path.insert(0, str((Path(tool_root) / 'tools/cloud').resolve()))
            sys.modules.pop('owndata', None)
            sys.modules[name] = score
            spec.loader.exec_module(score)
            score.ASM_DIR = Path(tool_root) / 'asm/us' / population
            targets = score.targets()
            for function, expected in functions.items():
                words = targets[function]
                assert len(words) * 4 == expected['bytes'], function
                assert digest(struct.pack('>' + str(len(words)) + 'I', *words)) == expected['sha256'], function
        finally:
            sys.path[:] = prior_path
            for key, previous in prior_modules.items():
                if previous is absent:
                    sys.modules.pop(key, None)
                else:
                    sys.modules[key] = previous
    return {'authored_sources': len(receipt['authored_sources']),
            'native_targets': sum(map(len, receipt['native_targets'].values())),
            'claims': [], 'accepted_coverage_bytes': 0}

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('--tool-root', type=Path, default=ROOT)
    print(json.dumps(verify(parser.parse_args().tool_root), indent=2))
