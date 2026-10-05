#!/usr/bin/env python3
"""Replay three bounded render-source hypotheses; no matches or production writes."""
import argparse
from contextlib import redirect_stdout
from dataclasses import asdict
import hashlib
import io
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

INPUTS = [
    'cloud/matches/sound_control.c',
    'cloud/work/frontier/w5a/sound_stop/best.c',
    'cloud/work/dot_pad_channel_reset_20261005/group.c',
    'cloud/work/near_miss_B52/func_800A7480_native.c',
    'cloud/matches/sfx_position_3d.c',
]
TARGETS = ['sound_stop', 'func_800A79DC', 'audio_channel_reset',
           'Input_ApplyPadConfig', 'Input_InitPadHandlers', 'func_800A7480',
           'display_list_flush']
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def input_text(path):
    p = ROOT / path
    if p.exists():
        return p.read_text()
    # Read tracked source omitted by a sparse checkout, never fabricate it.
    return subprocess.check_output(['git', 'show', 'HEAD:' + path], cwd=ROOT).decode()


def bindings():
    return {p: sha(input_text(p).encode()) for p in INPUTS}


def check_inputs(receipt):
    if bindings() != receipt['source_inputs']:
        raise ValueError('source snapshot changed')
    if sha(Path(__file__).read_bytes()) != receipt['verifier_sha256']:
        raise ValueError('verifier snapshot changed')


def native_summary():
    targets, symbols = score.targets(), score.image_symbols()
    return {n: dict(address=hex(symbols[n]), native_bytes=len(targets[n])*4,
                    sha256=sha(struct.pack('>%dI' % len(targets[n]), *targets[n])))
            for n in TARGETS}


def controls():
    donor = input_text(INPUTS[0])
    begin, end = donor.index('typedef signed char'), donor.index('typedef struct MultiBlit')
    header = donor[begin:end] + '''typedef struct RendererRecord { u8 unknown[22]; u8 state; u8 tail[9]; } RendererRecord;
extern RendererRecord D_80140BF0[];
extern Blit *D_80149450[];
extern s32 D_80149788;
'''
    recursive = header + '''void sound_stop(Blit *dbp)
{
    int i;
    if (!dbp) return;
    for (i=0; i<D_80149788; i++) {
        if (dbp == D_80149450[i]) break;
    }
    D_80140BF0[dbp->BLIdx].state = 2;
    D_80149788--;
    D_80149450[i] = D_80149450[D_80149788];
    D_80149450[D_80149788] = dbp;
    if (dbp->child) sound_stop(dbp->child);
}
'''
    helper = recursive.replace('void sound_stop(Blit *dbp)',
        'void func_800A79DC(int index) { D_80140BF0[index].state = 2; }\n\nvoid sound_stop(Blit *dbp)')
    helper = helper.replace('D_80140BF0[dbp->BLIdx].state = 2;', 'func_800A79DC(dbp->BLIdx);')
    loop = helper.replace('    if (!dbp) return;\n', '    while (dbp) {\n')
    loop = loop.replace('    if (dbp->child) sound_stop(dbp->child);', '    dbp = dbp->child;\n    }')
    scan = input_text(INPUTS[2])
    pos = scan.index('s32 audio_channel_reset(Sprite *config)')
    hidden = '''s32 Hidden(Sprite *config, s32 hide) {
    if (hide != config->unk1A) {
        config->unk1A = hide;
        Input_ApplyPadConfig(config);
    }
    return config->unk1A;
}
'''
    scan_hidden = scan[:pos]+hidden+scan[pos:]
    old = ' if(all!=config->unk1A) {\n  config->unk1A=all;\n  Input_ApplyPadConfig(config);\n }'
    assert scan_hidden.count(old) == 1
    scan_hidden = scan_hidden.replace(old, ' Hidden(config, all);')
    color = input_text(INPUTS[3])
    unsigned = color.replace('s8 r,g;u8 b,a;', 'u8 r,g,b,a;').replace('s8 r,s8 g,u8 b', 'u8 r,u8 g,u8 b')
    return {
        'remove_archived': (input_text(INPUTS[1]), ['sound_stop']),
        'remove_recursive': (recursive, ['sound_stop']),
        'remove_recursive_helper': (helper, ['sound_stop', 'func_800A79DC']),
        'remove_loop_helper': (loop, ['sound_stop', 'func_800A79DC']),
        'scan_archived': (scan, ['audio_channel_reset', 'Input_ApplyPadConfig', 'Input_InitPadHandlers']),
        'scan_hidden': (scan_hidden, ['audio_channel_reset', 'Input_ApplyPadConfig', 'Input_InitPadHandlers']),
        'color_archived': (color, ['func_800A7480']),
        'color_unsigned': (unsigned, ['func_800A7480']),
    }


def function_symbol(obj, name):
    data, sections = score._elf(obj)
    found = [s for i, sec in enumerate(sections) if sec['type'] == 2
             for s in score._symbol_table(data, sections, i)
             if s['name'] == name and s['type'] == 2 and s['section'] == score._text_index(sections)]
    assert len(found) == 1
    return found[0]


def inspect(obj, name, work):
    sym = function_symbol(obj, name)
    start, end = sym['value'], sym['value'] + sym['size']
    assert sym['size'] > 0 and sym['size'] % 4 == 0
    addresses, wanted = score.image_symbols(), score.targets()[name]
    with redirect_stdout(io.StringIO()):
        comparison = score.compare(obj, name, show=0)
    # Unlike the canonical target-sized score, this resolves the ENTIRE ELF body.
    words, masks, unresolved, unverified, errors = score.relocate(
        obj, score.text_words(obj), start, end, addresses)
    assert not any((masks, unresolved, unverified, errors)), (name, masks, unresolved, unverified, errors)
    complete = words[start//4:end//4]
    data, secs = score._elf(obj)
    assert not any(s['size'] for s in secs if s['name'] in ('.data','.rodata','.rdata','.bss'))
    symbols = [s for i, sec in enumerate(secs) if sec['type'] == 2
               for s in score._symbol_table(data, secs, i)]
    # Every relocation resolves through the GNU linker. Known global function
    # names are assigned their native addresses even when defined as context in
    # this research TU; placement of those context bytes is verified separately.
    names = {s['name'] for s in symbols if s['name'] and (s['section']==0 or s['type']==2)}
    unknown = [n for n in names if n not in addresses and score.address_named(n) is None and
               any(s['name']==n and s['section']==0 for s in symbols)]
    assert not unknown, unknown
    script = work / (name + '.ld')
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n' % (addresses[name]-start)
        + ''.join('%s = 0x%x;\n' % (n, addresses.get(n, score.address_named(n)))
                  for n in sorted(names) if n in addresses or score.address_named(n) is not None))
    elf, binary = work/(name+'.elf'), work/(name+'.bin')
    subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(elf),str(obj)],check=True,capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(elf),str(binary)],check=True,capture_output=True)
    body = binary.read_bytes()[start:end]
    assert body == struct.pack('>%dI' % len(complete), *complete)
    differing = [4*i for i in range(max(len(complete),len(wanted)))
                 if i>=len(complete) or i>=len(wanted) or complete[i]!=wanted[i]]
    relocs = sum(1 for sec in secs if sec['type']==9 and sec['info']==score._text_index(secs)
                 for at in range(sec['off'],sec['off']+sec['size'],8)
                 if start <= struct.unpack_from('>I',data,at)[0] < end)
    return dict(canonical=asdict(comparison), elf_bytes=sym['size'], native_bytes=len(wanted)*4,
                whole_elf_differing_words=len(differing), differing_offsets=differing,
                complete_gnu_relocation_equality=True, relocations=relocs, own_data_bytes=0,
                strict_match=(len(complete)==len(wanted) and not differing and comparison.summary() == "MATCH"),
                gnu_linked_body_sha256=sha(body))


def compiler_result():
    results = {}
    with tempfile.TemporaryDirectory(prefix='render-preflight-') as tmp:
        work = Path(tmp)
        for label, (text, names) in controls().items():
            d=work/label; d.mkdir(); src=d/'candidate.c'; obj=d/'candidate.o'; src.write_text(text)
            score.compile_single(src, FLAGS, obj)
            results[label] = dict(source_sha256=sha(text.encode()), flags=FLAGS, claims=[],
                                 functions={n:inspect(obj,n,d) for n in names})
    return results


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--compiler',action='store_true')
    parser.add_argument('--record',action='store_true')
    args=parser.parse_args()
    path=HERE/'verification.json'
    actual=dict(claims=[],source_inputs=bindings(),verifier_sha256=sha(Path(__file__).read_bytes()),native=native_summary())
    if args.record or args.compiler:
        actual['compiler']=compiler_result()
        actual['toolchain']={name:sha(Path(score.ido(name)).read_bytes()) for name in ('cc','cfe','uopt','ugen','as1')}
    if args.record:
        path.write_text(json.dumps(actual,indent=2,sort_keys=True)+'\n')
    else:
        expected=json.loads(path.read_text());check_inputs(expected)
        for key,value in actual.items():
            assert value==expected[key], key
    print('Compiler hypotheses replayed; no new matching claim.' if args.compiler or args.record else 'Source/native bindings verified; compiler replay not requested.')

if __name__=='__main__':
    main()
