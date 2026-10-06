"""Recompile the frozen sequence-constructor research and native behavior proof."""
import argparse
from dataclasses import asdict
import hashlib
import json
import os
import subprocess
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
sys.path.insert(0, str(WORK))
import native_behavior

NAME = 'func_800178B0'
SOURCE = WORK/(NAME+'.c')
BASE = 'cd22879d40b3de443cfde047b86e75e159b6cec6'
SIZES = {'BankRecord':8, 'ChannelSetup':8, 'Program':132, 'SequenceData':20,
         'GroupMap':2, 'SequenceOptions':32, 'SequenceStream':16,
         'SequenceTrack':40, 'SequenceContext':4088}
OFFSETS = {
    'Program':{'channels':4},
    'SequenceOptions':{'flags':0, 'enabled':4, 'speed':12, 'fade_time':14,
                       'volume':16, 'map_count':18, 'map':20, 'fade_count':24, 'fade_groups':28},
    'SequenceTrack':{'events':12, 'pitch':16, 'modulation':20, 'number':39},
    'SequenceContext':{'bank_a':4, 'map_a':8, 'bank_b':136, 'map_b':140,
                       'data':268, 'enabled':272, 'lookahead':288, 'tempo':292,
                       'streams':296, 'groups':1320, 'tracks':1384, 'master':3944,
                       'note_head':3960, 'note_tail':3964, 'programs':3968,
                       'active':4032, 'available':4033, 'speed':4034, 'group':4036,
                       'state':4037, 'counter':4038, 'request_and_result':4040, 'pending':4084}}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def compile_row(source, flags, obj):
    score.compile_single(source, flags, obj)
    result = score.compare(obj, NAME, show=0)
    data, sections = score._elf(obj)
    symbols = [entry for i, section in enumerate(sections) if section['type'] == 2
               for entry in score._symbol_table(data, sections, i) if entry['name'] == NAME]
    assert len(symbols) == 1 and symbols[0]['value'] == 0
    size = symbols[0]['size']
    words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, words, 0, len(words)*4, score.image_symbols())
    assert not (masks or unresolved or unverified or errors)
    owned = [s['name'] for s in sections
             if s['name'] in ('.data','.rodata','.rdata','.bss','.sdata','.sbss') and s['size']]
    assert not owned
    return {'source':str(source.relative_to(ROOT)), 'source_sha256':sha(source.read_bytes()),
            'flags':flags+' '+score.R4300_CC, 'function_bytes':size,
            'text_section_bytes':len(words)*4, 'tail_padding_bytes':len(words)*4-size,
            'tail_padding_all_zero':not any(words[size//4:]),
            'resolved_text_sha256':sha(struct.pack('>'+str(len(resolved))+'I', *resolved)),
            'frame_bytes':-(struct.unpack('>h',struct.pack('>H',words[0]&65535))[0]),
            'nonempty_owned_data_sections':owned, 'comparison':asdict(result), 'accepted':result.accepted()}


def run():
    score.ASM_DIR = ROOT/'asm/us/boot_tail'
    targets = score.targets()
    table = score.image_symbols()
    names = [NAME, 'func_8001558C', 'func_8001785C', 'func_8001C19C', 'func_8001B9F8',
             'func_80019A60', 'func_80020820', 'func_80017720', 'func_80020610', 'func_800175B4']
    native = {n:{'bytes':len(targets[n])*4,
                 'sha256':sha(struct.pack('>'+str(len(targets[n]))+'I', *targets[n]))}
              for n in names}
    assert native[NAME] == {'bytes':1160, 'sha256':'e2d3b7c8631d3ff01d9ec07b44abeab7d197f2eb0e93c9a68d6a9d1abcaf4b3a'}
    callers = []
    for name, words in targets.items():
        for i, word in enumerate(words):
            if word>>26 == 3 and (0x80000000|((word&0x3FFFFFF)<<2)) == 0x800178B0:
                callers.append({'function':name, 'site':hex(table[name]+i*4)})
    assert callers == [{'function':'func_8001558C','site':'0x80015660'},
                       {'function':'func_8001558C','site':'0x8001568c'}]
    terms = ['sizeof(void*)==4','sizeof(unsigned int)==4']
    terms += ['sizeof('+name+')=='+str(size) for name,size in SIZES.items()]
    terms += ['((unsigned int)&(('+name+'*)0)->'+field+')=='+str(offset)
              for name,fields in OFFSETS.items() for field,offset in fields.items()]
    with tempfile.TemporaryDirectory(prefix='sequence-constructor-') as tmp:
        tmp = Path(tmp)
        layout = tmp/'layout.c'
        layout.write_text('#include "'+str(SOURCE)+'"\ntypedef char native_layout[('+ ' && '.join(terms)+') ? 1 : -1];\n')
        score.compile_single(layout, score.DEFAULT_FLAGS, tmp/'layout.o')
        rows = [compile_row(SOURCE, score.DEFAULT_FLAGS, tmp/'best.o'),
                compile_row(SOURCE, score.DEFAULT_FLAGS.replace('-O2','-O1'), tmp/'o1.o')]
        assert rows[0]['function_bytes'] == 1160 and rows[0]['comparison']['differing'] == 206
        assert rows[0]['tail_padding_all_zero']
        for name in ('baseline','guard','store_order','packed','selected_index'):
            rows.append(compile_row(WORK/'controls'/(name+'.c'), score.DEFAULT_FLAGS, tmp/(name+'.o')))
        behavior = native_behavior.run(tmp/'best.o')
        host_flags = ['-std=c89','-pedantic-errors','-Wall','-Wextra','-Werror',
                      '-fsanitize=undefined,address','-fno-sanitize-recover=all','-no-pie']
        subprocess.run(['cc', *host_flags, str(WORK/'test_host.c'), '-o', str(tmp/'host')],
                       check=True, capture_output=True)
        host = subprocess.run([str(tmp/'host')], check=True, capture_output=True,
                              env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1'))
        assert not host.stdout and not host.stderr

    return {'status':'COMPLETE-NONMATCH', 'new_matching_bytes':0, 'base':BASE,
            'target':{'name':NAME,'start':'0x800178B0','end_exclusive':'0x80017D38','bytes':1160},
            'source_sha256':sha(SOURCE.read_bytes()),
            'harness_sha256':{name:sha((WORK/name).read_bytes()) for name in ('verify.py','native_behavior.py','test_packet.py','test_host.c')},
            'manifest_sha256':sha((score.ASM_DIR/'SHA256SUMS').read_bytes()),
            'native_inputs':native,'direct_tail_callers':callers, 'native_layout':{'sizes':SIZES,'offsets':OFFSETS},
            'compiled_rows':rows, 'behavior':behavior,
            'host_behavior':{'status':'PASS','cases':16896,'flags':host_flags,
                             'source_sha256':sha((WORK/'test_host.c').read_bytes()),
                             'leak_detection':False,'scope':'LP64 semantic fields, not native layout; all 256 slot masks, 32 option combinations plus null options, null/present defaults and master.'},
            'limits':['No source match, production change, lock change, image/compression/ROM gate or hosted CI claim.',
                      'Native helper models verify this body and its call boundaries; they are not full middleware execution.',
                      'All pointer offsets and indices use valid finite storage; malformed resources are outside the observed contract.']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = run()
    if args.check:
        assert result == json.loads((WORK/'evidence.json').read_text()), 'frozen evidence drift'
        print('PASS: complete source, full ELF and relocations, ABI layouts and native behavior replay.')
    elif args.output:
        args.output.write_text(json.dumps(result, indent=2)+'\n')
        print('Wrote research proof to '+str(args.output))
    else: print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
