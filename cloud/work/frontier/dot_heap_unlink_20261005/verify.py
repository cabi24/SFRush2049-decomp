#!/usr/bin/env python3
"""Source-bound, whole-ELF, GNU-relocation and bounded unlink contract proof."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
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
spec = importlib.util.spec_from_file_location('unlink_behavior', HERE / 'behavior.py')
behavior = importlib.util.module_from_spec(spec); spec.loader.exec_module(behavior)
FN = 'func_800E7A98'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
CONTEXT = ROOT / 'src/blob/groups/codex_heap_release_a25'
A29 = ROOT / 'cloud/work/ipa-groups/codex_heap_unlink_a29'

def sha(data): return hashlib.sha256(data).hexdigest()
def packed(words): return struct.pack('>%dI' % len(words), *words)
def run(args, **kw): return subprocess.run(args, check=True, text=True, capture_output=True, **kw)

def symbol_table(obj):
    data, sections = score._elf(obj)
    return [s for i, section in enumerate(sections) if section['type'] == 2 for s in score._symbol_table(data, sections, i)]

def function(obj, name):
    found = [s for s in symbol_table(obj) if s['name'] == name and s['type'] == 2]
    assert len(found) == 1, ('ambiguous function', name)
    return found[0]

def inspect(obj, name):
    fn = function(obj, name); start, end = fn['value'], fn['value'] + fn['size']
    data, sections = score._elf(obj); ti = score._text_index(sections)
    for other in symbol_table(obj):
        if other['type'] == 2 and other['section'] == ti and other['name'] != name:
            assert not (other['value'] < end and other['value'] + other['size'] > start), 'overlapping function'
    result = score.compare(obj, name, show=0)
    row = {'elf_bytes': fn['size'], 'native_bytes': len(score.targets()[name]) * 4, 'canonical': asdict(result)}
    words, masks, unresolved, unverified, errors = score.relocate(obj, score.text_words(obj), start, end, score.image_symbols())
    if name == FN:
        assert not (masks or unresolved or unverified or errors), 'candidate has incomplete relocation'
        body = words[start // 4:end // 4]; target = score.targets()[name]
        row.update(full_extent_differing_offsets=[4 * i for i in range(max(len(body), len(target)))
                   if i >= len(body) or i >= len(target) or body[i] != target[i]],
                   relocated_sha256=sha(packed(body)), native_sha256=sha(packed(target)))
        return row, body
    assert result.accepted() and fn['size'] == len(score.targets()[name]) * 4, ('context regression', name, result.summary())
    return row

def build(directory, body=None):
    directory.mkdir()
    group = json.loads((CONTEXT / 'group.json').read_text())
    for name in group['files']: shutil.copyfile(CONTEXT / name, directory / name)
    if body is not None:
        file = directory / 'car_damage_visual.c'; accepted = file.read_text()
        file.write_text(accepted + '\n' + body)
        assert file.read_text().startswith(accepted)
        group['members'].append(FN); group['keep'].append(FN)
    group['claims'] = []
    (directory / 'group.json').write_text(json.dumps(group))
    obj = directory / 'group.o'; score.compile_group(directory, obj)
    return obj, group

def gnu_link(obj, directory):
    # Independently GNU-link an exact compiler function slice. Preserve all raw
    # instruction/addend words. Convert only section-relative direct-call addends
    # into the exact named function at that object offset, never a target word.
    data, sections = score._elf(obj); ti = score._text_index(sections)
    fn = function(obj, FN); start, end = fn['value'], fn['value'] + fn['size']
    raw = score.text_words(obj)[start // 4:end // 4]
    funcs = {s['value']: s['name'] for s in symbol_table(obj) if s['type'] == 2 and s['section'] == ti}
    relocs = {}
    for sec in sections:
        if sec['type'] != 9 or sec['info'] != ti: continue
        syms = score._symbol_table(data, sections, sec['link'])
        for at in range(sec['off'], sec['off'] + sec['size'], 8):
            site, info = struct.unpack_from('>II', data, at)
            if not start <= site < end: continue
            symbol = syms[info >> 8]; kind = info & 255; relative = site - start
            name = symbol['name']
            if symbol['section'] == ti:
                assert kind == 4 and symbol['type'] == 3, 'unexpected local relocation'
                destination = symbol['value'] + ((raw[relative // 4] & 0x3FFFFFF) << 2)
                assert destination in funcs
                name = funcs[destination]; raw[relative // 4] &= 0xFC000000
            assert kind in (4, 5, 6) and name in score.image_symbols()
            assert relative not in relocs
            relocs[relative] = (kind, name)
    source = ['.set noreorder', '.text', '.globl ' + FN, '.type ' + FN + ',@function', FN + ':']
    kinds = {4:'R_MIPS_26', 5:'R_MIPS_HI16', 6:'R_MIPS_LO16'}
    for offset, word in enumerate(raw):
        source.extend(['slot_%d:' % offset, '.word 0x%08x' % word])
        if 4 * offset in relocs:
            kind, name = relocs[4 * offset]
            source.append('.reloc slot_%d,%s,%s' % (offset, kinds[kind], name))
    source.append('.size %s,.-%s' % (FN, FN))
    assembly = directory / 'slice.s'; assembly.write_text('\n'.join(source) + '\n')
    slice_obj = directory / 'slice.o'
    run(['mips-linux-gnu-as', '-EB', '-mips2', '-o', str(slice_obj), str(assembly)])
    script = directory / 'slice.ld'
    script.write_text('SECTIONS { .text 0x800E7A98 : SUBALIGN(4) { *(.text) } }\n' +
                      ''.join('%s = 0x%x;\n' % (name, score.image_symbols()[name]) for name in sorted({x[1] for x in relocs.values()})))
    elf = directory / 'slice.elf'; run(['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(elf), str(slice_obj)])
    linked, sections = score._elf(elf); text = sections[score._text_index(sections)]
    linked_fn = function(elf, FN)
    assert linked_fn['value'] == 0x800E7A98 and linked_fn['size'] == fn['size']
    body = linked[text['off']:text['off'] + fn['size']]
    alignment = linked[text['off'] + fn['size']:text['off'] + text['size']]
    assert not any(alignment)
    return {'bytes': fn['size'], 'relocations': [{'offset':off, 'kind':kind, 'symbol':name}
            for off,(kind,name) in sorted(relocs.items())], 'body_sha256':sha(body),
            'excluded_zero_alignment_bytes':len(alignment), 'compiler_slice_not_native_fixture':True}, list(struct.unpack('>%dI' % (len(body) // 4), body))

def host(directory, corpus):
    exe = directory / 'host'
    flags = ['cc', '-std=c89', '-O1', '-Wall', '-Wextra', '-Werror', '-fsanitize=undefined', '-fno-sanitize-recover=all']
    run(flags + [str(HERE / 'host.c'), '-o', str(exe)])
    expected = behavior.expected_text(corpus)
    assert run([str(exe)], input=behavior.corpus_text(corpus)).stdout == expected
    mutations = {
        'wrong_tag':('audio_reverb_update((u32)chosen, 1)', 'audio_reverb_update((u32)chosen, 0)'),
        'missing_unlink':('current->next = chosen->next;', '(void)current;'),
        'wrong_successor':('current->next = chosen->next;', 'current->next = NULL;'),
        'select_before_receive':('    osRecvMesg((OSMesgQueue *)&D_80152770, NULL, 1);\n    chosen = func_800A51D8(heap);',
                               '    chosen = func_800A51D8(heap);\n    osRecvMesg((OSMesgQueue *)&D_80152770, NULL, 1);')}
    failures = {}
    for label,(old,new) in mutations.items():
        d = directory / label; d.mkdir()
        text = (HERE/'candidate.c').read_text(); assert old in text
        (d/'candidate.c').write_text(text.replace(old,new)); shutil.copyfile(HERE/'host.c',d/'host.c')
        run(flags + [str(d/'host.c'), '-o', str(d/'host')])
        p = subprocess.run([str(d/'host')], input=behavior.corpus_text(corpus), text=True, capture_output=True)
        assert p.returncode != 0, ('survived mutation', label)
        failures[label] = 'rejected by contract assertion'
    return {'c89_ubsan_cases':len(corpus), 'source_body_unchanged':True,
            'host_only_pointer_transport':'u32 typedef is unsigned long; no native ABI claim', 'mutants':failures}

def verify(directory):
    source = (HERE/'candidate.c').read_text()
    base_obj, base_spec = build(directory/'baseline')
    obj, group = build(directory/'candidate', source)
    candidate, body = inspect(obj,FN)
    assert candidate['elf_bytes'] == 172 and candidate['full_extent_differing_offsets'] == [92,96,116,120]
    assert candidate['canonical']['differing'] == 4 and candidate['canonical']['extra_words'] == 0
    native_words = score.targets()[FN]
    for i, (native_word, candidate_word) in enumerate(zip(native_words, body)):
        if native_word != candidate_word:
            shift = 16 if i * 4 in (92, 96) else 21
            assert ((native_word >> shift) & 31) == 4
            assert ((candidate_word >> shift) & 31) == 7
            assert (native_word & ~(31 << shift)) == (candidate_word & ~(31 << shift)), 'residual not one pointer register'
    context = {}
    for name in base_spec['members'] + base_spec.get('context',[]):
        before, after = inspect(base_obj,name), inspect(obj,name)
        assert before == after, ('changed accepted context',name)
        context[name] = after
    linked_proof, linked = gnu_link(obj,directory)
    assert linked == body, 'GNU and project relocation disagree'
    behavioral, corpus = behavior.verify(score.targets()[FN],body,linked)
    host_proof = host(directory,corpus)
    archive_obj = directory/'a29.o'; score.compile_group(A29,archive_obj)
    archive = inspect(archive_obj,FN)[0]
    controls = {}
    replacements = {
        'separate_successor_local':source.replace('Heap *chosen, *current;', 'Heap *chosen, *current, *next;').replace('heap = current->next;', 'next = current->next;').replace('chosen == heap','chosen == next').replace('current = heap;','current = next;'),
        'direct_default_selection':source.replace('chosen = func_800A51D8(heap);','chosen = heap;\n    if (chosen == NULL) chosen = D_801527C8;')}
    for label,text in replacements.items():
        control_obj,_ = build(directory/label,text); controls[label] = inspect(control_obj,FN)[0]
    native = score.targets()[FN]; syms = score.image_symbols()
    callers = [{'caller':name,'site':hex(syms[name]+4*i)} for name,words in score.targets().items()
               for i,w in enumerate(words) if w >> 26 == 3 and ((syms[name]&0xF0000000)|((w&0x3FFFFFF)<<2)) == syms[FN]]
    result = {'status':'NONMATCH','claims':[],'accepted_byte_gain':0,'base':'e0e734babdac3c6a79d2f87f7f895e34aa170148',
        'flags':FLAGS,'candidate':candidate,'gnu_link':linked_proof,'accepted_context':context,
        'archive_a29':archive,'controls':controls,'behavior':behavioral,'host':host_proof,
        'native':{'address':hex(syms[FN]),'end_exclusive':hex(syms[FN]+4*len(native)),'bytes':4*len(native),
                  'sha256':sha(packed(native)),'direct_callers':callers},
        'source_sha256':{n:sha((HERE/n).read_bytes()) for n in ['candidate.c','host.c','behavior.py','verify.py']},
        'context_source_sha256':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in [CONTEXT/'group.json']+[CONTEXT/n for n in base_spec['files']]},
        'archive_source_sha256':sha((A29/'group.c').read_bytes()),
        'compiler_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}}
    return result

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__); parser.add_argument('--write',action='store_true'); args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='heap-unlink-proof-') as tmp: result = verify(Path(tmp))
    if args.write: (HERE/'verification.json').write_text(json.dumps(result,indent=2,sort_keys=True)+'\n')
    else: assert result == json.loads((HERE/'verification.json').read_text()), 'receipt drift'
    print(json.dumps({'status':result['status'],'differing':result['candidate']['canonical']['differing'],
                      'context_bodies':len(result['accepted_context']),'behavior':result['behavior'],'host':result['host']},indent=2))
