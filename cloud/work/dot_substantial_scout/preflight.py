#!/usr/bin/env python3
"""Read-only D02 native identity/call audit; emits metadata, never instruction arrays."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[3]
TARGETS = ('func_800D91A0', 'func_80102F30', 'func_80100E58', 'func_8010C974')

def direct_calls(words, address):
    return [(address + 4*i, ((address + 4*i + 4) & 0xf0000000) | ((w & 0x3ffffff) << 2))
            for i, w in enumerate(words) if w >> 26 == 3]

def entry_frame(words):
    # Limit discovery to the entry block, before its first control transfer.
    for w in words:
        if w & 0xffff0000 == 0x27bd0000:
            imm = w & 0xffff
            signed = imm - 65536 if imm & 32768 else imm
            return -signed if signed < 0 else None
        if w >> 26 in (1, 2, 3, 4, 5, 6, 7, 20, 21, 22, 23) or (w >> 26 == 17 and (w >> 21) & 31 == 8) or (w >> 26 == 0 and w & 63 in (8, 9)):
            break
    return None

def audit(root=ROOT):
    spec = importlib.util.spec_from_file_location('scout_score', root/'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(score)
    targets = score.targets()  # canonical loader verifies protected manifest
    syms = {k: int(v, 16) for k, v in json.loads(score.verified_bytes(root/'asm/us/blob/symbols.json', score.target_manifest()))['symbols'].items()}
    reverse = {}
    for name, addr in syms.items():
        reverse.setdefault(addr, []).append(name)
    edges = {n: direct_calls(w, syms[n]) for n, w in targets.items() if n in syms}
    locks = json.loads((root/'blob_matched.lock.json').read_text())
    rows = []
    for name in TARGETS:
        words, address = targets[name], syms[name]
        calls = edges[name]
        rows.append(dict(target=name, address=f'0x{address:08X}', native_bytes=4*len(words),
                         native_words=len(words), native_sha256=hashlib.sha256(struct.pack(f'>{len(words)}I', *words)).hexdigest(),
                         entry_frame_bytes=entry_frame(words), accepted_name_lock=name in locks,
                         accepted_interval_overlaps=sorted(n for n in locks if n in syms and n in targets and syms[n] < address+4*len(words) and address < syms[n]+4*len(targets[n])),
                         direct_call_count=len(calls),
                         callees=[dict(address=f'0x{a:08X}', names=sorted(reverse.get(a, [])), count=sum(b==a for _,b in calls)) for a in sorted({b for _,b in calls})],
                         direct_callers=sorted(n for n,e in edges.items() if any(a==address for _,a in e))))
    return dict(schema=1, classification='SCOUT_ONLY_NO_MATCH', claims=[], targets=rows)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--check', type=Path)
    args = parser.parse_args()
    result = audit()
    if args.check:
        assert result == json.loads(args.check.read_text()), 'native audit changed'
        print('D02 native identity, frames and direct callgraph verified')
    else:
        print(json.dumps(result, indent=2))
