#!/usr/bin/env python3
"""Read-only D08 preflight. Emit metadata only; never archive native words."""
import argparse
import contextlib
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import struct
import sys
import tempfile
from collections import Counter
from dataclasses import asdict

ROOT = Path(__file__).resolve().parents[3]
spec = importlib.util.spec_from_file_location('d08_score', ROOT / 'tools/cloud/score.py')
score = importlib.util.module_from_spec(spec)
sys.modules[spec.name] = score
spec.loader.exec_module(score)
CANDIDATES = ['render_large_objects', 'entity_spawn_init', 'func_8009F058', 'stunt_combo_display']
CONTEXT = ['func_800F92C8', 'func_800DE860']

def direct_calls(words):
    """MIPS JAL targets only; indirect calls/tail jumps deliberately excluded."""
    return Counter(0x80000000 | ((w & 0x03ffffff) << 2) for w in words if w >> 26 == 3)

def audit(replay=False):
    targets = score.targets()  # verifies protected manifest before any decoding
    syms = score.image_symbols()
    aliases = {}
    for name, addr in syms.items():
        aliases.setdefault(addr, []).append(name)
    locks = json.loads((ROOT / 'blob_matched.lock.json').read_text())
    selected = set(CANDIDATES + CONTEXT)
    result = {'schema': 1, 'claims': [], 'scope': 'reconnaissance; no new reconstruction or coverage', 'targets': {}}
    calls = {name: direct_calls(words) for name, words in targets.items()}
    for name in sorted(selected):
        words = targets[name]
        addr = syms[name]
        entry_adjusts = [w & 0xffff for w in words[:32] if w >> 16 == 0x27bd and w & 0x8000]
        result['targets'][name] = {
            'address': f'0x{addr:08X}', 'end_exclusive': f'0x{addr+4*len(words):08X}',
            'native_bytes': len(words)*4, 'native_words': len(words),
            'native_sha256': hashlib.sha256(struct.pack('>'+'I'*len(words), *words)).hexdigest(),
            'accepted_lock_present': name in locks,
            'entry_frame_bytes': 65536-entry_adjusts[0] if entry_adjusts else 0,
            'direct_callees': {f'0x{dst:08X}': {'count': n, 'aliases': sorted(aliases.get(dst, []))} for dst,n in sorted(calls[name].items())},
            'direct_callers': {caller: c[addr] for caller,c in sorted(calls.items()) if addr in c},
            'indirect_jalr_count': sum((w >> 26 == 0 and w & 63 == 9) for w in words),
        }
    source = ROOT / 'cloud/work/ipa-groups/render_large_objects/group.c'
    result['baseline_source_sha256'] = hashlib.sha256(source.read_bytes()).hexdigest()
    result['accepted_context_source_identical'] = source.read_bytes() == (ROOT / 'src/blob/groups/render_large_objects/group.c').read_bytes()
    if replay:
        result['compiler'] = {'flags': '-g0 -O3 -mips2 -G 0 -non_shared; as1 -r4300_mul', 'sha256': {tool: hashlib.sha256((score.IDO/tool).read_bytes()).hexdigest() for tool in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}}
        with tempfile.TemporaryDirectory() as tmp:
            obj = Path(tmp)/'baseline.o'
            score.compile_group(source.parent, obj)
            allsyms = score.symbols(obj)
            elf, secs = score._elf(obj)
            elf_functions = {s['name']: s for i, sec in enumerate(secs) if sec['type'] == 2 for s in score._symbol_table(elf, secs, i) if s['type'] == 2}
            text_len = len(score.text_words(obj))*4
            result['replay'] = {}
            for name in ['render_large_objects'] + CONTEXT:
                start = allsyms[name]
                end = min([v for v in allsyms.values() if v > start] + [text_len])
                with contextlib.redirect_stdout(io.StringIO()):
                    cmp = score.compare(obj, name)
                result['replay'][name] = dict(asdict(cmp), candidate_comparison_extent_bytes=end-start, candidate_elf_size_bytes=elf_functions[name]['size'], trailing_padding_bytes=end-start-elf_functions[name]['size'], accepted_object_match=cmp.accepted())
    return result

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--replay', action='store_true', help='fresh IDO compile and strict resolved comparison')
    args = parser.parse_args()
    print(json.dumps(audit(args.replay), indent=2, sort_keys=True))
