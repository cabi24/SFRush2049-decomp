#!/usr/bin/env python3
"""Strict aggregate replay of source-frozen, independently reviewed wave 18."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

WORK = ROOT / 'cloud/work/boot_tail'


def run():
    manifest = json.loads((WORK / 'wave18/reviewed_sources.json').read_text())
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    results = []
    for entry in manifest['entries']:
        source = ROOT / entry['source_path']
        if hashlib.sha256(source.read_bytes()).hexdigest() != entry['source_sha256']:
            raise ValueError('reviewed source hash drift: ' + entry['name'])
        if len(score.targets()[entry['name']]) * 4 != entry['size']:
            raise ValueError('native extent drift: ' + entry['name'])
        with tempfile.TemporaryDirectory(prefix='boot-tail-wave18-') as tmp:
            obj = Path(tmp) / 'candidate.o'
            score.compile_single(source, entry['flags'], obj)
            result = score.compare(obj, entry['name'], show=0)
            words = score.text_words(obj)
            relocated, masks, unresolved, unverified, errors = score.relocate(
                obj, words, 0, min(len(words) * 4, entry['size']), score.image_symbols())
            want = score.targets()[entry['name']]
            exact = (relocated[:len(want)] == want and len(words) >= len(want)
                     and not any(words[len(want):]) and not masks
                     and not unresolved and not unverified and not errors)
            if result.unverified != entry['expected_stock_unverified']:
                raise ValueError('stock admission blocker drift: ' + entry['name'])
            proof = entry['independent_relocation']
            if hashlib.sha256((ROOT / proof['receipt_path']).read_bytes()).hexdigest() != proof['receipt_sha256']:
                raise ValueError('independent relocation receipt drift: ' + entry['name'])
            if hashlib.sha256((ROOT / entry['independent_review_path']).read_bytes()).hexdigest() != entry['independent_review_sha256']:
                raise ValueError('peer review receipt drift: ' + entry['name'])
            if result.accepted() != entry['expected_match']:
                raise ValueError('claimed status drift: ' + entry['name'])
            if result.differing != entry['expected_differing_words']:
                raise ValueError('residual drift: ' + entry['name'])
            if entry['expected_match'] and not exact:
                raise ValueError('full relocated words failed: ' + entry['name'])
            elf, secs = score._elf(obj)
            text_index = score._text_index(secs)
            function_symbols = [sym for i, sec in enumerate(secs) if sec['type'] == 2
                for sym in score._symbol_table(elf, secs, i)
                if sym['section'] == text_index and sym['type'] == 2]
            function_extent_exact = (len(function_symbols) == 1
                and function_symbols[0]['name'] == entry['name']
                and function_symbols[0]['value'] == 0
                and function_symbols[0]['size'] == entry['size'])
            if entry['expected_match'] and not function_extent_exact:
                raise ValueError('ELF function extent differs from native: ' + entry['name'])
            results.append(dict(name=entry['name'],source_sha256=entry['source_sha256'],
                flags=entry['flags'],effective_flags=entry['flags']+' -Wab,-r4300_mul',
                native_bytes=entry['size'],differing_words=result.differing,
                total_words=result.total,extra_words=result.extra_words,
                unresolved=result.unresolved,unverified=result.unverified,errors=result.errors,
                masked_relocations=len(masks),proof_state=entry.get('proof_state'),strict_match=result.accepted(),
                relocated_full_word_equality=exact,function_symbols=function_symbols,
                text_section_bytes=secs[text_index]['size'],function_extent_exact=function_extent_exact))
    return dict(schema_version=1,result='PASS',functions=len(results),
                matching_functions=sum(x['strict_match'] for x in results),
                matching_bytes=sum(x['native_bytes'] for x in results if x['strict_match']),
                nonmatching_functions=sum(not x['strict_match'] for x in results),
                results=results)


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check',action='store_true')
    args=parser.parse_args()
    text=json.dumps(run(),indent=2)+'\n'
    if args.check:
        if text != (WORK/'wave18/verification.json').read_text():
            raise SystemExit('aggregate receipt drift')
        print('Wave 18: three complete nonmatches reproduced; stock local-rodata gates retained')
    else:
        print(text,end='')
