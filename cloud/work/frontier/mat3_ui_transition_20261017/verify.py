"""Reproduce the complete strict group and negative controls without integration."""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

GROUP = HERE / 'groups/frontier_slot18_alloc'
NAME = 'func_800D5A04'
ALLOC = 'func_80091B00'
MEMBERS = [NAME, ALLOC, 'func_800D5828', 'func_800D5524', 'records_screen',
           'players_frame_update', 'func_800B0580', 'entity_audio_update',
           'object_type7_create', 'object_type1_create', 'func_800D6348',
           'sync_entry_register', 'player_conditional_check']
SINGLES = ['func_800D5828', 'func_800D5524', 'records_screen',
           'players_frame_update', 'func_800B0580', 'entity_audio_update']

def require(ok, reason):
    if not ok:
        raise RuntimeError(reason)

def sha(data):
    return hashlib.sha256(data).hexdigest()

def compare(obj, name):
    with contextlib.redirect_stdout(io.StringIO()):
        return score.compare(obj, name)

def evidence():
    spec = json.loads((GROUP / 'group.json').read_text())
    # w34i: frontier_slot18_alloc was superseded (+ func_800D5A04, PR #383); its pre-install copy (handoff §5).
    old_dir = ROOT / 'cloud/work/frontier/superseded/frontier_slot18_alloc'
    old = json.loads((old_dir / 'group.json').read_text())
    require(spec['name'] == 'frontier_slot18_alloc', 'preserve original group name')
    require(spec['members'] == MEMBERS and spec['claims'] == MEMBERS, 'complete 13-member claims')
    require(spec['context'] == [] and not spec.get('allow_unverified'), 'no hidden members or waiver')
    require(set(old['members']) <= set(MEMBERS), 'preserve every old member')
    require(set(old['keep']) <= set(spec['keep']), 'preserve genuine caller roots')
    require(NAME in spec['keep'] and ALLOC not in spec['keep'], 'real kept/internal boundary')
    require('player_conditional_check' in spec['keep'], 'preserve additional defined locked helper')
    for file in old['files']:
        require((GROUP/file).read_bytes() == (old_dir/file).read_bytes(), 'unchanged allocator-group source: '+file)
    for n in SINGLES:
        require((GROUP/(n+'.c')).read_bytes() == (ROOT/'src/blob'/(n+'.c')).read_bytes(), 'unchanged locked source: '+n)
    words = score.targets()
    addresses = score.image_symbols()
    call = 0x0c000000 | ((addresses[ALLOC] >> 2) & 0x03ffffff)
    offsets = [i*4 for i,w in enumerate(words[NAME]) if w == call]
    require(offsets == [0xf0], 'genuine allocator call at the native offset')
    require(len(words[NAME]) == 107, 'full native extent')
    return {
        'base': '58425072b49e95f0fbe5c4c239988fb4885f7c5c',
        'population': 'game', 'new_source_exact_bytes': 428,
        'new_accepted_image_bytes': 0, 'new_rom_verified_bytes': 0,
        'allocator_call_offsets': offsets,
        'targets': {n: {'address': hex(addresses[n]), 'bytes': len(words[n])*4,
                       'sha256': sha(struct.pack('>%dI'%len(words[n]), *words[n]))}
                    for n in MEMBERS},
        'sources': {str(p.relative_to(HERE)): sha(p.read_bytes())
                    for p in [*(GROUP/f for f in spec['files']), GROUP/'group.json', HERE/'verify.py', HERE/'trace_verify.py']},
        'test_sha256': sha((ROOT/'tests/cloud/test_ui_transition_real_allocator.py').read_bytes())}

def gnu_proof(obj, directory):
    """Link the untouched object, then compare every byte of the new function.

    Anchor .text on the genuine internal allocator: its local call relocation
    is section-relative. Exported function/undefined symbols receive native
    addresses through ordinary linker-script assignments. This proves function
    relocations, not the full image layout, and never edits instruction words.
    """
    data, sections = score._elf(obj)
    functions = score.symbols(obj)
    addresses = score.image_symbols()
    definitions = {}
    require(not any(s['name'] in ('.rodata', '.data', '.bss') and s['size'] for s in sections), 'unexpected owned data')
    for i, section in enumerate(sections):
        if section['type'] != 2:
            continue
        for symbol in score._symbol_table(data, sections, i):
            n = symbol['name']
            if n and (symbol['section'] == 0 or symbol['type'] == 2):
                address = addresses.get(n, score.address_named(n))
                if address is not None:
                    definitions[n] = address
            if n == NAME:
                require(symbol['type'] == 2 and symbol['size'] == 428, 'complete ELF function extent')
    base = addresses[ALLOC] - functions[ALLOC]
    script = directory/'proof.ld'
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n'%base +
                      ''.join('%s = 0x%x;\n'%pair for pair in sorted(definitions.items())))
    elf = directory/'proof.elf'
    result = subprocess.run(['mips-linux-gnu-ld', '-T', str(script), '-o', str(elf), str(obj)], capture_output=True, text=True)
    require(result.returncode == 0, 'GNU link failed: '+result.stderr)
    words = score.text_words(elf)
    start = functions[NAME]//4
    require(words[start:start+107] == score.targets()[NAME], 'GNU full 428-byte comparison failed')

def main():
    inputs = evidence()
    receipt = HERE/'RECEIPT.json'
    if '--write-receipt' in sys.argv:
        receipt.write_text(json.dumps(inputs, indent=2, sort_keys=True)+'\n')
    require(inputs == json.loads(receipt.read_text()), 'receipt changed')
    with tempfile.TemporaryDirectory(prefix='rush-ui-transition-') as temp:
        d = Path(temp)
        obj = d/'group.o'
        score.compile_group(GROUP, obj)
        summaries = {}
        for n in MEMBERS:
            result = compare(obj, n)
            require(result.accepted(False), n+': '+result.summary())
            summaries[n] = result.summary()
        require(set(score.symbols(obj)) == set(MEMBERS), 'all emitted function definitions must be members')
        gnu_proof(obj, d)
        wrong = d/'wrong_operation'
        shutil.copytree(GROUP, wrong)
        source = wrong/'group.c'
        text = source.read_text()
        require(text.count('->operation=7;') == 1, 'negative-control source anchor')
        source.write_text(text.replace('->operation=7;', '->operation=6;'))
        score.compile_group(wrong, d/'wrong.o')
        bad = compare(d/'wrong.o', NAME)
        require(bad.differing > 0 and not bad.accepted(False), 'wrong operation not rejected')
        external = d/'external_allocator'
        shutil.copytree(GROUP, external)
        spec = json.loads((external/'group.json').read_text())
        old = json.loads((ROOT/'cloud/work/frontier/superseded/frontier_slot18_alloc/group.json').read_text())
        removed = set(old['members'] + old['context'])
        spec['files'] = [f for f in spec['files'] if f not in old['files']]
        for k in ('members', 'claims', 'keep'):
            spec[k] = [n for n in spec[k] if n not in removed]
        (external/'group.json').write_text(json.dumps(spec, indent=2)+'\n')
        score.compile_group(external, d/'external.o')
        baseline = compare(d/'external.o', NAME)
        require(baseline.differing == 13 and not baseline.extra_words and not baseline.unresolved and not baseline.unverified and not baseline.errors,
                'external allocator control must restore clean 13/107 residual')
        print(json.dumps({'members': summaries, 'new_source_exact_bytes': 428,
                          'new_accepted_image_bytes': 0, 'new_rom_verified_bytes': 0,
                          'gnu_original_object': '428/428 bytes exact',
                          'wrong_operation_differences': bad.differing,
                          'external_allocator_differences': baseline.differing}, indent=2))

if __name__ == '__main__':
    main()
