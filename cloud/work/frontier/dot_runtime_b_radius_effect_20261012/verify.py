#!/usr/bin/env python3
"""Complete source-first NONMATCH proof and explicitly non-claimable read-barrier control."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
NAME, ADDRESS, SIZE, RODATA = 'func_8038D798', 0x8038D798, 736, 0x80394DD4
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
TEST = ROOT / 'tests/cloud/test_runtime_b_radius_effect.py'
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers


def require(value, message):
    if not value:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def image_bytes():
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8', 'asset identity')
    require(struct.unpack_from('>I', asset, 0x3854)[0] == 0xB6FEC4, 'loader pointer')
    image = zlib.decompress(asset[0xB6FEC4 - 0x283D0:], -15)
    require(len(image) == 43888 and sha(image) == 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd', 'image identity')
    return image


@contextmanager
def targets(image, population='ovl_b'):
    old = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us' / population
    key = owndata.artifact_dir(score.ASM_DIR)
    present, prior = key in score._own_data, score._own_data.get(key)
    score._own_data[key] = None if image is None else owndata.ImageData.from_image(image, 0x8038A400)
    try:
        yield
    finally:
        score.ASM_DIR = old
        if present:
            score._own_data[key] = prior
        else:
            score._own_data.pop(key, None)


def selected_inputs():
    image = image_bytes()
    rows = {}
    with targets(image):
        manifest = score.target_manifest()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))['functions']
        entry = next(e for e in extents if e['name'] == NAME)
        require(entry['address'] == hex(ADDRESS).upper().replace('X', 'x') and entry['size'] == SIZE, 'native extent')
        require(min(int(e['address'], 16) for e in extents if int(e['address'], 16) > ADDRESS) == ADDRESS + SIZE, 'next native boundary')
        native = packed(score.targets()[NAME])
        require(native == image[ADDRESS - 0x8038A400:ADDRESS - 0x8038A400 + SIZE], 'native/image binding')
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 8]
        require(literal == struct.pack('>2f', 0.35, 1.35), 'native literals')
        symbols = score.image_symbols()
        selected_symbols = {name: symbols.get(name, score.address_named(name)) for name in
                            ('D_80152818', 'D_8014A250', 'D_801543CA', 'func_800A61B0', 'func_8008C768', 'func_8038D3A4')}
        helper = packed(score.targets()['func_8038D3A4'])
    callers = []
    for population in ('blob', 'ovl_b'):
        with targets(image if population == 'ovl_b' else None, population):
            bodies, addresses = score.targets(), score.image_symbols()
            for name, words in bodies.items():
                sites = [i * 4 for i, word in enumerate(words) if word >> 26 == 3 and
                         (((addresses[name] + i * 4 + 4) & 0xf0000000) | ((word & 0x3ffffff) << 2)) == ADDRESS]
                if sites:
                    callers.append({'population': population, 'name': name, 'sites': sites, 'body_sha256': sha(packed(words))})
    require([(c['population'], c['name'], c['sites']) for c in callers] ==
            [('blob', 'func_8010C974', [2384]), ('ovl_b', 'func_8038E114', [3020,4040,4072,4268,4296])], 'caller census')
    return {'sources': {name: sha((HERE / name).read_bytes()) for name in ('group.c','group.json','verify.py','trace.py')},
            'test': sha(TEST.read_bytes()), 'native_extent': entry, 'native_body_sha256': sha(native),
            'native_literal_sha256': sha(literal), 'image_sha256': sha(image), 'symbols': selected_symbols,
            'damage_helper_sha256': sha(helper), 'callers': callers, 'flags': FLAGS, 'compiler': 'ido5.3'}


def inspect(path, size, linked=False):
    raw, secs = score._elf(path)
    hdr = struct.unpack_from('>16sHHIIIIIHHHHHH', raw)
    require(hdr[0][:9] == b'\x7fELF\x01\x02\x01\0\0' and hdr[1] == (2 if linked else 1) and hdr[2] == 8 and hdr[7] == (0x10000001 if size == SIZE else 0x10000000), 'ELF ABI: '+repr((hdr[0][:9],hdr[1],hdr[2],hex(hdr[7]),str(path))))
    symbols = {s['name']: s for i, sec in enumerate(secs) if sec['type'] == 2
               for s in score._symbol_table(raw, secs, i) if s['name']}
    require({n for n,s in symbols.items() if s['type'] == 2 and s['section'] not in (0,0xfff1)} == {NAME}, 'defined functions')
    fn = symbols[NAME]
    require((fn['value'],fn['size'],fn['section']) == (ADDRESS if linked else 0,size,score._text_index(secs)), 'full ELF function extent')
    content = {}
    for i, sec in enumerate(secs):
        sh = struct.unpack_from('>IIIIIIIIII',raw,hdr[6] + i * hdr[11])
        require(not linked or sec['type'] not in (4,9) or sec['size'] == 0, 'residual relocation')
        if sh[2] & 2 and sec['size']:
            if sec['name'] == '.text':
                require((sh[3],sec['size']) == (ADDRESS if linked else 0,(size+15)//16*16), 'text placement')
            elif sec['name'] == '.rodata':
                require((sh[3],sec['size']) == (RODATA if linked else 0,16), 'rodata placement')
            else:
                require(not linked and sec['name'] in ('.reginfo','.options'), 'unexpected allocation')
            if sec['name'] in ('.text','.rodata'):
                content[sec['name']] = raw[sec['off']:sec['off']+sec['size']]
    require(set(content) == {'.text','.rodata'}, 'missing allocated input')
    require(not any(content['.text'][size:]) and content['.rodata'][8:] == bytes(8), 'nonzero alignment suffix')
    return raw, secs, symbols, content


def gnu_link(obj, size, directory):
    raw, secs, syms, content = inspect(obj,size)
    addresses = score.image_symbols()
    undefined = {n: addresses.get(n,score.address_named(n)) for n,s in syms.items() if s['section'] == 0}
    relocations = []
    for sec in secs:
        if sec['type'] != 9:
            continue
        require(sec['info'] == score._text_index(secs), 'unexpected relocated section')
        names = score._symbol_table(raw,secs,sec['link'])
        for at in range(sec['off'],sec['off']+sec['size'],8):
            offset,info = struct.unpack_from('>II',raw,at)
            symbol = names[info >> 8]
            require(offset % 4 == 0 and 0 <= offset < size and (info & 255) in (4,5,6), 'relocation range/type')
            relocations.append({'offset':offset,'kind':info & 255,'symbol':symbol['name'] or secs[symbol['section']]['name']})
    unreferenced = {n for n,a in undefined.items() if a is None}
    require(unreferenced == {'fabsf'} and not any(r['symbol'] in unreferenced for r in relocations), 'unknown referenced external symbol')
    undefined = {n:a for n,a in undefined.items() if a is not None}
    script = directory / 'link.ld'
    script.write_text('\n'.join('%s = 0x%08x;' % x for x in sorted(undefined.items())) +
        '\nSECTIONS { .text 0x8038D798 : SUBALIGN(4) { *(.text) } .rodata 0x80394DD4 : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
    linked = directory / 'linked.elf'
    subprocess.run([shutil.which('mips-linux-gnu-ld'),'-EB','--hash-style=sysv','-T',str(script),'-o',str(linked),str(obj)],check=True,capture_output=True)
    _,_,linked_syms,linked_content = inspect(linked,size,True)
    require(not [n for n,s in linked_syms.items() if s['section'] == 0], 'undefined linked symbol: '+str([n for n,s in linked_syms.items() if s['section'] == 0]))
    require(all(linked_syms[n]['value'] == a and linked_syms[n]['section'] == 0xfff1 for n,a in undefined.items()), 'external address binding')
    native = packed(score.targets()[NAME])
    literal = image_bytes()[RODATA-0x8038A400:RODATA-0x8038A400+8]
    require(linked_content['.rodata'][:8] == literal, 'GNU full literal bytes')
    differences = [i for i in range(0,min(SIZE,size),4) if native[i:i+4] != linked_content['.text'][i:i+4]]
    return {'elf_flags':hex(struct.unpack_from('>I',raw,36)[0]),'function_bytes':size,'text_section_bytes':len(content['.text']),'owned_literal_bytes':8,'rodata_padding':8,
            'linked_function_sha256':sha(linked_content['.text'][:size]), 'linked_literal_sha256':sha(literal),
            'differing_offsets':differences,'extra_nonzero_words':sum(bool(int.from_bytes(linked_content['.text'][i:i+4],'big')) for i in range(SIZE,size,4)),
            'relocations':relocations,'external_bindings':undefined}


def forms(source):
    require(source.count('extern s16 D_801543CA;') == 1, 'count declaration drift')
    exact = source.replace('extern s16 D_801543CA;','extern volatile s16 D_801543CA;')
    homes = exact.replace('    Player *target;\n    f32 direction[3];\n    f32 transformed[3];\n    Player *source;\n    f32 delta[3];\n    f32 distance_squared;',
                          '    f32 direction[3];\n    f32 transformed[3];\n    f32 delta[3];\n    f32 distance_squared;\n    Player *target;\n    Player *source;')
    require(homes != exact,'home control drift')
    return [('plain_count',source,760,125,6),('diagnostic_volatile_count',exact,736,0,0),('diagnostic_old_homes',homes,736,13,0)]


def replay():
    binding = selected_inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3','requires recognized stock IDO5.3')
    require(shutil.which('mips-linux-gnu-ld') is not None,'GNU MIPS linker missing')
    compiler = {name:sha(Path(score.ido(name)).read_bytes()) for name in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')}
    image = image_bytes()
    outputs = {}
    with targets(image), tempfile.TemporaryDirectory(prefix='radius-effect-') as temp:
        for label,source,size,differing,extra in forms((HERE/'group.c').read_text()):
            directory = Path(temp)/label; directory.mkdir()
            (directory/'group.c').write_text(source)
            (directory/'group.json').write_bytes((HERE/'group.json').read_bytes())
            obj=directory/'candidate.o'
            spec=score.compile_group(directory,obj)
            require(spec['members']==spec['keep']==[NAME] and spec['claims']==spec['context']==[], 'empty-claims group contract')
            result=score.compare(obj,NAME,show=0)
            require(result.differing==differing and result.extra_words==extra and not result.errors and not result.unresolved, label+': '+result.summary())
            require(len(result.unverified)==(4 if label=='plain_count' else 0), 'expected own-data uncertainty')
            linked=gnu_link(obj,size,directory)
            require(len(linked['differing_offsets'])==differing and linked['extra_nonzero_words']==extra, 'GNU/scorer difference agreement')
            require(len(linked['relocations'])==(27 if label=='plain_count' else 25),'relocation census')
            if label=='diagnostic_volatile_count':
                require(result.accepted(),'diagnostic did not reproduce exact words')
                with targets(None):
                    missing=score.compare(obj,NAME,show=0)
                    require(not missing.accepted() and missing.unverified,'missing data accepted')
                for offset in (0,4):
                    corrupt=bytearray(image);corrupt[RODATA-0x8038A400+offset]^=1
                    with targets(bytes(corrupt)):
                        bad=score.compare(obj,NAME,show=0)
                        require(not bad.accepted() and bad.errors,'corrupt data accepted')
            outputs[label]={'scorer':asdict(result),'gnu':linked}
    return {'inputs':binding,'compiler_sha256':compiler,'forms':outputs,'claims':[],
            'new_accepted_strict_match_bytes':0,'new_image_bytes':0,'new_rom_bytes':0,
            'diagnostic_only_exact_bytes':736,'diagnostic_qualifier_originally_present':'unproven',
            'owned_data_negative_controls':'missing data and either corrupt literal rejected'}


def verify():
    expected=json.loads((HERE/'verification.json').read_text())
    require(selected_inputs()==expected['inputs'],'selected input drift')
    require(replay()==expected,'full receipt drift')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record',action='store_true');parser.add_argument('--score',action='store_true');parser.add_argument('--inputs-only',action='store_true')
    args=parser.parse_args()
    if args.record:
        (HERE/'verification.json').write_text(json.dumps(replay(),indent=2)+'\n')
        print('Recorded complete NONMATCH and non-claimable exact diagnostic')
    elif args.score:
        with targets(image_bytes()):
            sys.argv=['score.py','group',str(HERE),'--targets',str(ROOT/'asm/us/ovl_b')]
            raise SystemExit(score.main())
    elif args.inputs_only:
        require(selected_inputs()==json.loads((HERE/'verification.json').read_text())['inputs'],'selected input drift')
        print('PASS: selected source/native input bindings')
    else:
        verify();print('PASS: complete NONMATCH GNU proof; exact volatile diagnostic remains unclaimed')
