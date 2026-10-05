"""Replay established behavior cases against the eight actual adapted sources.

Only harness field/type spellings and the signed lookup stub are translated;
source-under-test text is never replaced. Pointer-bearing layouts are checked
with native IDO in verify.py, not asserted using LP64 host pointer sizes.
"""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
PACKETS = ROOT / 'cloud/work/boot_tail'
SOURCES = ROOT / 'cloud/work/boot_tail_promotion/voice_lists/sources'
CASES = {
    '8001B29C': ('BT03-high-larger', 'test_chain_8001B29C', 2305),
    '8001B3A0': ('BT03-high-larger', 'test_chain_8001B3A0', 2305),
    '8001B7C0': ('BT03-high-larger', 'test_chain_8001B7C0', 2305),
    '8001B8C4': ('BT03-high-runtime', 'test_handle_chain_keeps_stale_successors', 33),
    '8001B968': ('BT03-high-routing', 'test_packed_handle_getter', 257),
    '8001EB10': ('BT03-high-runtime-followon', 'test_voice_detach_and_pool_order', 64),
    '8001ECE0': ('BT03-high-chains', 'test_keyed_pool_insertion', 109),
    '8001F9D0': ('BT03-high-init', 'test_state_reset_call_order', 256)}


def run_case(address):
    packet, method, calls = CASES[address]
    spec = importlib.util.spec_from_file_location('legacy_voice_list_' + address,
                                                PACKETS / packet / 'test_semantics.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    executed = []

    class ActualAdaptedSource:
        def run_c(self, selected, body, match=True):
            assert selected == address
            source = SOURCES / ('func_' + address + '.c')
            if address.startswith('8001B'):
                names = {'VoiceState': 'VoiceState_80019C8C', 'next10': 'child',
                         'next_identifier': 'child', 'flags24': 'flags',
                         'channel4B': 'set', 'identifier60': 'identifier'}
                body = body.replace('u32 func_8001EDF4(', 'int func_8001EDF4(')
            else:
                names = {'VoicePrefix': 'VoiceState', 'child10': 'next_identifier',
                         'parent14': 'parent_identifier', 'flags': 'flags24',
                         'identifier': 'identifier60', 'valueBD': 'activeBD'}
                # The callback can mutate the real command word. The expected
                # model gets the same callback event; no source is rewritten.
                body = body.replace('unknown00[0]', 'command00')
                # Actual SequenceNode.value follows the accepted signed lookup.
                body = body.replace('free_nodes[0].value==state.identifier60',
                                    '(u32)free_nodes[0].value==state.identifier60')
                # These two historical assertions described the old pointerless
                # view. Native sizes/offsets now belong to verify.py's IDO proof.
                body = re.sub(r'^typedef char check_(?:size|fields)\[.*?;\n', '', body,
                              flags=re.MULTILINE)
            for old, new in names.items():
                body = re.sub(r'\b' + old + r'\b', new, body)
            with tempfile.TemporaryDirectory(prefix='adapted-voice-semantics-') as tmp:
                work = Path(tmp)
                harness = work / 'test.c'
                harness.write_text('#include <assert.h>\n#include <stddef.h>\n'
                    '#include <string.h>\n#include "' + str(source) + '"\n' + body)
                command = ['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra',
                    '-Werror', '-fsanitize=address,undefined', '-no-pie',
                    str(harness), '-o', str(work / 'test')]
                compiled = subprocess.run(command, capture_output=True, text=True)
                if compiled.returncode:
                    raise AssertionError(compiled.stderr)
                result = subprocess.run([str(work / 'test')], capture_output=True,
                    text=True, env=dict(os.environ,
                    ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',
                    UBSAN_OPTIONS='halt_on_error=1'))
                if result.returncode:
                    raise AssertionError(result.stdout + result.stderr)
            executed.append({'function': 'func_' + address, 'calls': calls,
                'source': str(source.relative_to(ROOT)),
                'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                'harness_source': str((PACKETS / packet / 'test_semantics.py').relative_to(ROOT)),
                'harness_method': method, 'result': 'PASS'})

    getattr(module.Semantics, method)(ActualAdaptedSource())
    assert len(executed) == 1
    return executed[0]


def run_all():
    rows = [run_case(address) for address in CASES]
    return {'result': 'PASS', 'calls': sum(row['calls'] for row in rows), 'rows': rows,
            'scope': 'Actual adapted C89 sources, ASan/UBSan; established bounded valid-record/list domains. Native IDO separately checks layouts.'}


if __name__ == '__main__':
    print(json.dumps(run_all(), indent=2))
