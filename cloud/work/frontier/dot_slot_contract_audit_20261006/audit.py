#!/usr/bin/env python3
"""Read-only native slot-selector census; never emits instructions or ROM bytes."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

BASE = 'cd22879d40b3de443cfde047b86e75e159b6cec6'
SOURCES = (
    'tools/cloud/score.py',
    'cloud/work/s20261004/D/notes.md',
    'cloud/work/s20261004/E/notes.md',
    'cloud/work/s20261004/E/src/helper.h',
    'cloud/work/game_C104/REPORT.md',
    'cloud/work/dot_menu_options_root_20261005/slot_state_setup.c',
    'src/blob/groups/credits_scroll_grp/group.json',
    'src/blob/groups/credits_scroll_grp/gr3_c.c',
    'src/blob/groups/slot_sound/group.c',
)
HELPERS = ('slot_state_setup', 'object_create', 'sound_update_channel',
           'func_80096288', 'object_byte9_set')
GPRS = {16: 's0', 17: 's1', 20: 's4', 21: 's5', 23: 's7'}


class AuditError(ValueError):
    pass


def sha(data):
    return hashlib.sha256(data).hexdigest()


def body_hash(words):
    return sha(struct.pack('>%dI' % len(words), *words))


def address(value):
    return '0x%08X' % value


def direct_call(word, pc):
    """MIPS JAL, using the architectural high PC bits; no computed-call guess."""
    if word >> 26 == 3:
        return ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
    return None


def capture(word):
    op, rs, rt, rd, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word & 63
    if op == 0 and fn in (33, 37) and {rs, rt} == {0, 2}:
        return GPRS.get(rd, 'r%d' % rd)
    if op == 43 and rs == 29 and rt == 2:
        return 'stack_word'
    return None


def writes_v0(word):
    """Conservative integer-destination decode for capture-scan termination."""
    op, rs, rt, rd, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word & 63
    if op == 0:
        return rd == 2 and fn not in (8, 12, 13, 17, 19, 24, 25, 26, 27)
    if op in (8, 9, 10, 11, 12, 13, 14, 15, 32, 33, 34, 35, 36, 37, 38, 48, 52, 55):
        return rt == 2
    if op in (16, 17):
        return rs in (0, 1, 2) and rt == 2
    return False


def return_capture(words, index, next_call):
    """First simple v0 capture before/within the next JAL's delay slot.

    Stops when v0 is redefined. This is a syntactic observation, not CFG liveness.
    No capture is inferred across a long unpaired sequence.
    """
    for pos in range(index + 2, next_call + 2):
        kind = capture(words[pos])
        if kind:
            return kind
        if writes_v0(words[pos]):
            return None
    return None


def base_bytes(name):
    return subprocess.run(['git', 'show', BASE + ':' + name], cwd=ROOT,
                          check=True, capture_output=True).stdout


def source_hashes(root=ROOT):
    # Historical context is read from full Git history, never the integration tree.
    result = {}
    result['audit.py'] = sha((root / HERE.relative_to(ROOT) / 'audit.py').read_bytes())
    return result


def portable(result):
    result = json.loads(json.dumps(result))
    for name in SOURCES:
        result['source_sha256'].pop(name, None)
    return result


def inspect_site(words, start, site, selector, acquire, release):
    """Validate a claimed no-copy call site independently of census membership."""
    if site < start or (site - start) % 4:
        raise AuditError('wrong site address')
    index = (site - start) // 4
    if index >= len(words) or direct_call(words[index], site) != selector:
        raise AuditError('wrong site: not a selector JAL')
    calls = [(i, direct_call(w, start + i * 4)) for i, w in enumerate(words)
             if direct_call(w, start + i * 4) is not None]
    earlier = [(i, c) for i, c in calls if i < index]
    later = [(i, c) for i, c in calls if i > index]
    if not earlier or not later or earlier[-1][1] != acquire or later[0][1] != release:
        raise AuditError('site lacks adjacent acquire/release call labels')
    next_index = later[0][0]
    if next_index + 1 >= len(words):
        raise AuditError('truncated call delay slot')
    if return_capture(words, index, next_index) is not None:
        raise AuditError('site captures selector return')
    return True


def collect():
    # Both APIs authenticate current file contents even after a warm-cache read.
    targets, symbols = score.targets(), score.image_symbols()
    by_address = {symbols[n]: n for n in targets}
    locks = json.loads(base_bytes('blob_matched.lock.json'))
    accepted = {symbols[n] for n in locks if n in symbols}
    selector = symbols['slot_state_setup']
    acquire, release = symbols['osRecvMesg'], symbols['osJamMesg']
    wrapper = symbols['object_create']
    callers, wrapper_callers = {}, {}
    no_copy, unpaired = [], []
    captures = Counter()
    paired = 0
    for name in sorted(targets, key=lambda n: symbols[n]):
        words, start = targets[name], symbols[name]
        calls = [(i, direct_call(w, start + i * 4)) for i, w in enumerate(words)
                 if direct_call(w, start + i * 4) is not None]
        selected = [(j, i) for j, (i, callee) in enumerate(calls) if callee == selector]
        wrapped = [address(start + 4 * i) for i, callee in calls if callee == wrapper]
        if wrapped:
            wrapper_callers[name] = wrapped
        if not selected:
            continue
        row_captures = Counter()
        for j, i in selected:
            site = {'function': name, 'address': address(start + i * 4)}
            is_pair = (j > 0 and j + 1 < len(calls) and
                       calls[j - 1][1] == acquire and calls[j + 1][1] == release)
            if not is_pair:
                unpaired.append(site)
                continue
            paired += 1
            next_index = calls[j + 1][0]
            if next_index + 1 >= len(words):
                raise AuditError('truncated call delay slot')
            kind = return_capture(words, i, next_index)
            if kind:
                captures[kind] += 1
                row_captures[kind] += 1
            else:
                inspect_site(words, start, start + i * 4, selector, acquire, release)
                no_copy.append(site)
        other = sorted({by_address[callee] for _, callee in calls
                        if callee in by_address and callee != selector and callee not in accepted})
        callers[name] = {
            'start': address(start), 'bytes': 4 * len(words),
            'body_sha256': body_hash(words), 'accepted': start in accepted,
            'sites': [address(start + i * 4) for _, i in selected],
            'captures': dict(sorted(row_captures.items())),
            'other_unaccepted_direct_game_calls': other,
        }
    bounded = {n: r['bytes'] for n, r in callers.items()
               if not r['accepted'] and not r['other_unaccepted_direct_game_calls']}
    return {
        'format': 1, 'historical_base': BASE, 'acceptance_as_of_commit': BASE,
        'scope': 'Registered game-body direct JAL census only. Call-label adjacency and simple return captures are syntactic; queue identity, CFG liveness, indirect/overlay calls and matching readiness are not proved.',
        'source_sha256': source_hashes(),
        'helpers': {n: {'start': address(symbols[n]), 'bytes': 4 * len(targets[n]),
                        'body_sha256': body_hash(targets[n])} for n in HELPERS},
        'summary': {
            'callers': len(callers), 'sites': sum(len(r['sites']) for r in callers.values()),
            'adjacent_call_label_pairs': paired, 'captures': dict(sorted(captures.items())),
            'accepted_callers': [n for n, r in callers.items() if r['accepted']],
            'unaccepted_caller_bytes': sum(r['bytes'] for r in callers.values() if not r['accepted']),
            'only_selector_unaccepted_direct_game_dependency': bounded,
            'bounded_dependency_bytes': sum(bounded.values()),
        },
        'no_copy_sites': no_copy, 'outside_adjacent_wrapper_sequence': unpaired,
        'wrapper_callers': wrapper_callers, 'callers': callers,
    }


def verify(saved=None):
    if saved is None:
        saved = json.loads((HERE / 'evidence.json').read_text())
    current = collect()
    actual, expected = portable(current), portable(saved)
    for key in actual:
        if actual[key] != expected[key]:
            raise AuditError('source-bound audit differs: %s' % key)
    return current


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write', action='store_true', help='regenerate this packet evidence, never native data')
    args = parser.parse_args()
    current = collect() if args.write else verify()
    if args.write:
        (HERE / 'evidence.json').write_text(json.dumps(current, indent=2) + '\n')
    print(json.dumps(current['summary'], indent=2))
    print('No C candidate, compiler, runtime, image or ROM claim.')


if __name__ == '__main__':
    main()
