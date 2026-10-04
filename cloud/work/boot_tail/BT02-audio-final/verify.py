#!/usr/bin/env python3
"""Replay three complete NONMATCHs with unchanged strict scorer and exact ELF sizes."""
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
P = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
SIZES = {'80011104': 840, '800114C0': 904, '800139D4': 688}
EXPECTED = {'80011104': (62, 0, 840), '800114C0': (199, 0, 904), '800139D4': (131, 1, 692)}
def digest(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()
def symbol_size(obj, name):
    data, sections = score._elf(obj)
    rows = [s for i, sec in enumerate(sections) if sec['type'] == 2
            for s in score._symbol_table(data, sections, i) if s['name'] == name]
    assert len(rows) == 1 and rows[0]['type'] == 2 and rows[0]['value'] == 0
    return rows[0]['size']
def record(obj, name):
    r = score.compare(obj, name, show=0)
    return {'strict_match': r.accepted(), 'differing_words': r.differing, 'total_words': r.total,
            'extra_nonzero_words': r.extra_words, 'unresolved': r.unresolved, 'unverified': r.unverified,
            'errors': r.errors, 'elf_function_bytes': symbol_size(obj, name)}
def run():
    pins = json.loads((P/'input_pins.json').read_text())
    for path, want in pins['source_hashes'].items():
        assert digest(ROOT/path) == want, path
    compiler = {p.name: digest(p) for p in score.IDO.iterdir() if p.is_file()}
    assert compiler == pins['compiler_files_sha256']
    score.ASM_DIR = ROOT/'asm/us/boot_tail'
    manifest = subprocess.check_output(['sha256sum', '-c', 'SHA256SUMS'], cwd=score.ASM_DIR, text=True).splitlines()
    def extent(path):
        return {r['address']: r['size'] for r in json.loads(path.read_text())['functions']}
    inventory = extent(ROOT/'specs/015-boot-tail-runtime/inventory.json')
    assert inventory == extent(score.ASM_DIR/'extents.json')
    assert len(inventory) == 439 and sum(inventory.values()) == 99120
    rows = []
    with tempfile.TemporaryDirectory(prefix='audio-final-verify-') as folder:
        folder = Path(folder)
        for address, size in SIZES.items():
            name = 'func_' + address
            for checkpoint in ['controls', 'nonmatch']:
                source = P/checkpoint/(name+'.c')
                assert source.read_text().splitlines()[0] == '/* flags: '+score.DEFAULT_FLAGS+' */'
                for opt in ['O2', 'O1']:
                    flags = score.DEFAULT_FLAGS.replace('O2', opt)
                    obj = folder/(name+checkpoint+opt+'.o')
                    score.compile_single(source, flags, obj)
                    row = record(obj, name)
                    assert not row['strict_match']
                    if opt == 'O2':
                        assert not (row['unresolved'] or row['unverified'] or row['errors'])
                        if checkpoint == 'nonmatch':
                            assert (row['differing_words'], row['extra_nonzero_words'], row['elf_function_bytes']) == EXPECTED[address]
                        words = score.text_words(obj)
                        resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, row['elf_function_bytes'], score.image_symbols())
                        assert not (masks or unresolved or unverified or errors)
                        row['full_function_relocations_resolved'] = True
                        row['full_relocated_equal_and_exact_extent'] = (row['elf_function_bytes'] == size and resolved[:size//4] == score.targets()[name])
                        assert not row['full_relocated_equal_and_exact_extent']
                    row.update(function=name, checkpoint=checkpoint, target_bytes=size, source=str(source.relative_to(ROOT)), source_sha256=digest(source), flags=flags, effective_flags=flags+' '+score.R4300_CC)
                    rows.append(row)
        getter = ROOT/'cloud/matches/boot_tail/func_80010A00.c'
        obj = folder/'getter.o'; score.compile_single(getter, score.DEFAULT_FLAGS, obj)
        assert score.compare(obj, getter.stem, show=0).accepted() and symbol_size(obj, getter.stem) == 12
    return {'schema_version':1, 'result':'PASS', 'base':'b5daa44a', 'claim_commit':'b33647f8', 'pinned_compiler_files':len(compiler), 'manifest':manifest, 'extent_functions':439, 'extent_bytes':99120, 'existing_getter_strict_exact_match':True, 'strict_match_functions':0, 'strict_match_bytes':0, 'complete_nonmatch_functions':3, 'complete_nonmatch_bytes':sum(SIZES.values()), 'results':rows,
            'harness_hashes':{name:digest(P/name) for name in ['verify.py','test_host.py','host_behavior.c','test_layout.py','diagnose.py']}}
if __name__ == '__main__':
    print(json.dumps(run(), indent=2))
