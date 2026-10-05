#!/usr/bin/env python3
"""Replay the existing qualified emitter domain suite against the adapted source.

Only host harness names follow the shared native type. The candidate is included
from disk without rewriting it. Tracking metadata, never output floats, is
initialized by the existing definedness instrumentation.
"""
import importlib.util
from pathlib import Path
import re
import subprocess
import sys
import unittest

ROOT = Path(__file__).resolve().parents[4]
ORIGINAL = ROOT / 'cloud/work/boot_tail/BT03-emitter-handle'
SOURCE = ROOT / 'cloud/work/boot_tail_promotion/sources/func_8001DDE0.c'


def run():
    sys.path.insert(0, str(ORIGINAL))
    try:
        spec = importlib.util.spec_from_file_location('sample_contract_emitter_domain', ORIGINAL / 'test_semantics.py')
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
    finally:
        sys.path.pop(0)
    module.SOURCE = SOURCE
    original_run = subprocess.run

    def run_with_shared_names(command, **kwargs):
        command = list(command)
        # The existing tests generate host harness C and a native layout probe.
        # Update just those temporary files; the #included candidate is unchanged.
        if '-o' in command and any(str(p).endswith('.c') for p in command):
            for item in command:
                if str(item).endswith('.c'):
                    path = Path(item)
                    text = path.read_text()
                    for old, new in [('Emitter', 'StateNode'), ('flags', 'flags08'), ('identifier', 'identifier34')]:
                        text = re.sub(r'\b' + old + r'\b', new, text)
                    text = 'typedef unsigned char u8;\ntypedef unsigned short u16;\ntypedef unsigned int u32;\n' + text
                    path.write_text(text)
            command.insert(1, '-I' + str(ROOT / 'include'))
        return original_run(command, **kwargs)

    methods = ['test_domain_examples_and_prior_iteration_carry',
               'test_c89_actual_and_consumption_tracked_sources',
               'test_native_layout_and_source_freeze']
    suite = unittest.TestSuite(module.Semantics(name) for name in methods)
    # This is a single-process standalone runner, so the patch cannot leak into
    # unrelated tests. The original receipt test is replaced by verify.py's
    # current exact standalone AND actual-TU ELF/full-relocation proof.
    try:
        subprocess.run = run_with_shared_names
        result = unittest.TextTestRunner(verbosity=2).run(suite)
    finally:
        subprocess.run = original_run
    return result.wasSuccessful()


if __name__ == '__main__':
    raise SystemExit(0 if run() else 1)
