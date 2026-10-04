#!/usr/bin/env python3
"""Replay the exact bounded initial and directed source controls, read-only."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    count = 0
    with tempfile.TemporaryDirectory(prefix='c13-controller-controls-') as tmp:
        for receipt in ('initial_controls.json', 'directed_controls.json'):
            for row in json.loads((WORK / receipt).read_text()):
                source = ROOT / row['source_path']
                data = source.read_bytes()
                if hashlib.sha256(data).hexdigest() != row['source_sha256']:
                    raise ValueError('source hash drift: ' + row['source_path'])
                obj = Path(tmp) / 'control.o'
                score.compile_single(source, row['flags'], obj)
                result = score.compare(obj, row['name'], show=0)
                elf, sections = score._elf(obj)
                symbol = next(sym for idx, sec in enumerate(sections) if sec["type"] == 2
                              for sym in score._symbol_table(elf, sections, idx)
                              if sym["name"] == row["name"])
                values = dict(elf_function_size=symbol["size"], differing_words=result.differing, total_words=result.total,
                              extra_words=result.extra_words, unresolved=result.unresolved,
                              unverified=result.unverified, errors=result.errors,
                              strict_match=result.accepted())
                if any(row[key] != value for key, value in values.items()):
                    raise ValueError('control receipt drift: ' + row['source_path'])
                if source.read_bytes() != data:
                    raise ValueError('source changed during replay')
                count += 1
    return {'result': 'PASS', 'control_rows': count, 'matching_credit': 0}


if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
