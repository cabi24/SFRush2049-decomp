"""Read-only camera/helper code-shape audit. Emits metadata, never target words.

This is a diagnostic, not the canonical match gate or a semantic equivalence test.
Run from the repository root with the existing IDO environment available.
"""
import argparse
from collections import Counter
from dataclasses import asdict
import json
from pathlib import Path
import sys

REPO = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(REPO))
from tools.cloud import score

GPRS = 'zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp s8 ra'.split()


def signed16(value):
    value &= 65535
    return value - 65536 if value & 32768 else value


def shape(words, addresses):
    """Collect narrowly defined opcode facts; not a general liveness analysis."""
    names = {value: key for key, value in addresses.items()}
    calls = []
    saves = []
    frame = None
    fp_writes = set()
    for i, word in enumerate(words):
        op, rs, rt = word >> 26, word >> 21 & 31, word >> 16 & 31
        if i == 0 and op == 9 and rs == 29 and rt == 29:
            frame = -signed16(word)
        if (i < 20 and rs == 29 and frame is not None
                and 0 <= signed16(word) < frame
                and ((op == 43 and rt in list(range(16, 24)) + [30, 31])
                     or (op == 61 and rt >= 20))):
            saves.append({'register': GPRS[rt] if op == 43 else 'f%d/f%d' % (rt, rt + 1),
                          'bytes': 4 if op == 43 else 8, 'stack_offset': signed16(word)})
        if op == 3:
            address = 0x80000000 | ((word & 0x3ffffff) << 2)
            calls.append({'offset': '0x%x' % (4 * i), 'callee': names.get(address, '0x%08x' % address)})
        if op in (49, 53):
            fp_writes.add(rt)
        if op == 17:
            if rs == 4:  # mtc1
                fp_writes.add(word >> 11 & 31)
            elif rs in (16, 17, 20, 21) and (word & 63) < 48:
                fp_writes.add(word >> 6 & 31)
    return {'extent_words': len(words), 'stack_frame_bytes': frame,
            'prologue_saves': saves, 'direct_calls': calls,
            'call_counts': dict(Counter(c['callee'] for c in calls)),
            'syntactic_fp_destinations': ['f%d' % n for n in sorted(fp_writes)]}


def slice_object(path, name, addresses):
    syms = score.symbols(path)
    words = score.text_words(path)
    start = syms[name]
    end = min((p for p in syms.values() if p > start), default=len(words) * 4)
    resolved, masks, unresolved, unverified, errors = score.relocate(path, words, start, end, addresses)
    return resolved[start // 4:end // 4], {'unresolved': unresolved, 'section_relative': unverified, 'errors': errors}


def audit(path):
    addresses = score.image_symbols()
    targets = score.targets()
    names = ('camera_transform', 'MP_TargetSteerPos')
    native = {n: shape(targets[n], addresses) for n in names}
    compiled = {}
    for name in names:
        if name not in score.symbols(path):
            compiled[name] = {'absent': True}
            continue
        words, relocations = slice_object(path, name, addresses)
        compiled[name] = dict(shape(words, addresses), relocation_diagnostics=relocations)
    helper_addr = addresses['MP_TargetSteerPos']
    helper_call = (3 << 26) | ((helper_addr >> 2) & 0x3ffffff)
    native_callers = [{'function': n, 'offset': '0x%x' % (i * 4)}
                      for n, words in targets.items() for i, w in enumerate(words) if w == helper_call]
    return {'scope': 'Syntactic code-shape metadata; no retail equivalence claim. Extent can include linker padding.',
            'native': native, 'compiled': compiled, 'native_direct_helper_callers': native_callers,
            'canonical_camera_comparison': asdict(score.compare(path, 'camera_transform', show=0))}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('object', type=Path)
    args = parser.parse_args()
    print(json.dumps(audit(args.object), indent=2))
