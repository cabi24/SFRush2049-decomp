#!/usr/bin/env python3
"""Verify the complete donor-based range RNG, real context and bounded behavior."""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor.pipeline import blob_group
spec = importlib.util.spec_from_file_location('range_rng_semantics', HERE/'verify_semantics.py')
semantics = importlib.util.module_from_spec(spec)
spec.loader.exec_module(semantics)
FN = 'func_8008B2E4'
CONTEXT = 'func_8008B2B4'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'

BASE = 'cd22879d40b3de443cfde047b86e75e159b6cec6'

def base_bytes(path):
    return subprocess.run(['git', 'show', BASE + ':' + path], cwd=ROOT,
                          check=True, capture_output=True).stdout

def sha(data):
    return hashlib.sha256(data).hexdigest()

def inspect(obj, name):
    data, sections = score._elf(obj)
    ti = score._text_index(sections)
    syms = [s for i, sec in enumerate(sections) if sec['type'] == 2
            for s in score._symbol_table(data, sections, i)]
    fn, = [s for s in syms if s['name'] == name and s['type'] == 2 and s['section'] == ti]
    text = sections[ti]
    words = struct.unpack('>%dI' % (text['size']//4), data[text['off']:text['off']+text['size']])
    native = score.targets()[name]
    start = fn['value']
    got, masks, unresolved, unverified, errors = score.relocate(
        obj, words, start, start+fn['size'], score.image_symbols())
    body = got[start//4:(start+fn['size'])//4]
    comparison = score.compare(obj, name, show=0)
    return dict(asdict(comparison), canonical_verdict=comparison.summary(),
                symbol_bytes=fn['size'], symbol_offset=start, native_bytes=len(native)*4,
                full_extent_equal=list(body)==native and fn['size']==len(native)*4,
                all_relocations_resolved=not(masks or unresolved or unverified or errors),
                relocated_body_sha256=sha(struct.pack('>%dI'%len(body), *body)))

def verify(directory):
    directory.mkdir(parents=True, exist_ok=True)
    assert (HERE/'rand.c').read_bytes() == base_bytes('src/blob/func_8008B2B4.c')
    native, addresses = score.targets(), score.image_symbols()
    direct_call_word = 0x0c000000 | ((addresses[FN] >> 2) & 0x03ffffff)
    direct_callers = [name for name,words in native.items() if direct_call_word in words]
    assert direct_callers == []
    obj = directory/'candidate.o'
    score.compile_group(HERE, obj)
    result = {'base_revision': 'cd22879d40b3de443cfde047b86e75e159b6cec6',
              'status': 'STRICT_MATCH_PENDING_INDEPENDENT_REVIEW', 'claims': [FN],
              'range': ['0x8008B2E4','0x8008B32C'], 'new_candidate_bytes': 72,
              'accepted_byte_gain': 0, 'flags': FLAGS, 'direct_native_callers': direct_callers,
              'assembler_erratum_flag_added_by_scorer': score.R4300_CC,
              'source_sha256': {p:sha((HERE/p).read_bytes()) for p in ('range.c','rand.c','group.json')},

              'object': inspect(obj, FN), 'accepted_context': inspect(obj, CONTEXT),
              'selected_native_body_sha256':{name:sha(struct.pack('>%dI'%len(native[name]),*native[name])) for name in (FN,CONTEXT)}}
    assert all(result[key]['full_extent_equal'] and result[key]['all_relocations_resolved']
               and result[key]['canonical_verdict']=='MATCH' for key in ('object','accepted_context'))
    data, sections = score._elf(obj)
    ti = score._text_index(sections); text = sections[ti]
    assert text['size'] == 128 and data[text['off']+120:text['off']+128] == bytes(8)
    assert all(s['size']==0 for s in sections if s['name'] in ('.data','.rodata','.rdata','.bss','.sdata','.sbss','.lit4','.lit8'))
    relocs = []
    for section in sections:
        if section['type'] == 9 and section['info'] == ti:
            syms = score._symbol_table(data, sections, section['link'])
            for off in range(section['off'], section['off']+section['size'], 8):
                where, info = struct.unpack_from('>II', data, off)
                relocs.append({'offset':where, 'type':info & 255, 'symbol':syms[info >> 8]['name']})
    assert relocs == [{'offset':o,'type':t,'symbol':'D_8011735C'} for o,t in [(0,5),(4,6),(48,5),(52,6)]]
    result['relocations'] = relocs
    result['zero_alignment_bytes_outside_symbols'] = 8
    result['no_owned_data_or_literals'] = True
    # Independent GNU symbol-reader, linker and section extraction.
    readelf = subprocess.run(['mips-linux-gnu-readelf','-Ws',str(obj)], check=True, capture_output=True, text=True).stdout
    nm = subprocess.run(['mips-linux-gnu-nm','-S','--defined-only',str(obj)], check=True, capture_output=True, text=True).stdout
    assert re.search(r'00000000\s+48\s+FUNC.*func_8008B2B4', readelf)
    assert re.search(r'00000030\s+72\s+FUNC.*func_8008B2E4', readelf)
    assert re.search(r'00000000 00000030 T func_8008B2B4', nm)
    assert re.search(r'00000030 00000048 T func_8008B2E4', nm)
    script = directory/'link.ld'
    script.write_text('SECTIONS { .text 0x8008B2B4 : SUBALIGN(4) { *(.text) } }\nD_8011735C = 0x8011735C;\n')
    elf, binary = directory/'candidate.elf', directory/'candidate.bin'
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)], check=True, capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(elf),str(binary)], check=True, capture_output=True)
    linked_data, linked_sections = score._elf(elf)
    linked_symbols = [s for i,sec in enumerate(linked_sections) if sec['type']==2
                      for s in score._symbol_table(linked_data,linked_sections,i)]
    for name,size in ((CONTEXT,48),(FN,72)):
        symbol, = [s for s in linked_symbols if s['name']==name]
        assert symbol['value']==addresses[name] and symbol['size']==size and symbol['type']==2
    linked_bytes = binary.read_bytes()
    wanted = struct.pack('>30I', *(native[CONTEXT]+native[FN]))
    assert linked_bytes == wanted + bytes(8)
    result['gnu_link'] = {'full_pair_equal':True,'context_address':hex(addresses[CONTEXT]),
                          'candidate_address':hex(addresses[FN]),'function_bytes':[48,72],
                          'body_sha256':sha(linked_bytes[48:120])}
    # Current production ELF/relocation reader, without any splice or source write.
    names = [CONTEXT,FN]
    extents = {n:{'vaddr':addresses[n],'size':len(native[n])*4} for n in names}
    slices, index = blob_group.member_slices(obj,names,extents)
    relocated = blob_group.relocate(obj,slices,index,addresses,members=names)
    result['production_reader_equal'] = {n:relocated[n]==struct.pack('>%dI'%len(native[n]),*native[n]) for n in names}
    assert all(result['production_reader_equal'].values())
    # Causal controls: remove only the authentic outer mask or output local.
    source = (HERE/'range.c').read_text()
    variants = {'without_outer_mask':source.replace('func_8008B2B4() & 0x07FFF','func_8008B2B4()'),
                'direct_return':source.replace('    float rannum;\n\n    rannum =','    return').replace('\n\n    return(rannum);',''),
                'arcade_double_denominator':source.replace('32768.0f','32768.0')}
    result['controls'] = {}
    for label, content in variants.items():
        group = directory/label; group.mkdir()
        for name in ('group.json','rand.c'): shutil.copy2(HERE/name,group/name)
        (group/'range.c').write_text(content)
        candidate = group/'group.o'; score.compile_group(group,candidate)
        result['controls'][label] = inspect(candidate,FN)
        assert inspect(candidate,CONTEXT)['full_extent_equal']
    assert result['controls']['without_outer_mask']['differing'] == 3
    assert result['controls']['direct_return']['full_extent_equal']
    assert result['controls']['arcade_double_denominator']['differing'] == 12
    # Earlier standalone source remains the old lead, not the submitted recipe.
    old = directory/'archived.c'
    old.write_bytes(base_bytes('cloud/work/bigfish/libhunt/nearmiss/func_8008B2E4_frand.c'))
    score.compile_single(old,FLAGS,directory/'old.o')
    result['controls']['archived_standalone'] = inspect(directory/'old.o',FN)
    result['archived_source_sha256'] = sha(old.read_bytes())
    result['host_native_linked'] = semantics.verify(directory,native[FN],list(struct.unpack('>18I',linked_bytes[48:120])))
    result['compiler_sha256'] = {n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}
    result['packet_sha256'] = {p:sha((HERE/p).read_bytes()) for p in ['verify.py','verify_semantics.py','semantic_test.c','claim.json']}
    result['limitations'] = ['No full-game shadow, image, compression or ROM verification.',
                            'Host -fwrapv explicitly defines the unchanged accepted rand signed-overflow behavior.',
                            'Arithmetic proof excludes NaN, infinity, subnormal arithmetic, overflow and nondefault rounding.',
                            'No direct native callers were found; inlined uses and original whole-program roots are not proven.']
    return result

def portable(result):
    result=json.loads(json.dumps(result))
    for name in ('target_manifest_sha256','tools_sha256'):result.pop(name, None)
    return result

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output',type=Path)
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='range-rng-proof-') as tmp:
        result = verify(Path(tmp))
    if args.output: args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps({k:result[k] for k in ['status','object','accepted_context','host_native_linked']},indent=2))
