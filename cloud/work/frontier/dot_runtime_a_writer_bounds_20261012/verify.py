#!/usr/bin/env python3
"""Authenticate existing image A, then run unmodified stock scoring/data gates."""
import argparse
import importlib.util
import re
import shutil
import subprocess
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP_NAME = 'dot_runtime_a_menu_renderers_20261011'
GROUP = HERE / 'groups' / GROUP_NAME
MEMBERS = ['func_803A6F2C', 'func_803A7180', 'func_803A7620', 'func_803A7918']
CONTEXT = []
NAME = 'func_803A7180'
SIZES = dict(zip(MEMBERS, (596,1184,760,576)))
ASSET_SHA256 = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
TABLE_ADDRESS = 0x803B9834
TABLE_SIZE = 20
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers
spec = importlib.util.spec_from_file_location('writer_capacity', HERE/'capacity.py')
capacity = importlib.util.module_from_spec(spec)
spec.loader.exec_module(capacity)


def require(value, message):
    if not value:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


@contextmanager
def target_scope(image=None):
    previous = score.ASM_DIR
    directory = ROOT / 'asm/us/ovl_a'
    key = owndata.artifact_dir(directory)
    missing = object()
    saved = score._own_data.get(key, missing)
    score.ASM_DIR = directory
    try:
        if image is not None:
            score._own_data[key] = image
        yield
    finally:
        score.ASM_DIR = previous
        if saved is missing:
            score._own_data.pop(key, None)
        else:
            score._own_data[key] = saved


def metadata():
    with target_scope():
        manifest = score.target_manifest()
        return json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))


def authenticate():
    """No git-history access or binary output; both asset and full image are pinned."""
    meta = metadata()
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(len(asset) == 12418096 and digest(asset) == ASSET_SHA256, 'asset identity mismatch')
    offset = int(meta['rom_offset'], 16)
    require(int.from_bytes(asset[0x3850:0x3854], 'big') == offset == 0xB5C534,
            'image A loader pointer mismatch')
    decompressor = zlib.decompressobj(-15)
    data = decompressor.decompress(asset[offset - 0x283D0:]) + decompressor.flush()
    require(decompressor.eof, 'image A stream is incomplete')
    require(len(data) == meta['size'] == 194128 and digest(data) == meta['image_sha256'],
            'image A identity mismatch')
    base = int(meta['base'], 16)
    require(base == 0x8038A400, 'unexpected image base')
    table = data[TABLE_ADDRESS - base:TABLE_ADDRESS - base + TABLE_SIZE]
    require(len(table) == TABLE_SIZE, 'table outside image')
    return data, base, table


def table_structure(obj, data, base, table):
    elf = owndata._Object(obj)
    rodata = [i for i, sec in enumerate(elf.sections) if sec['name'] == '.rodata']
    text = [i for i, sec in enumerate(elf.sections) if sec['name'] == '.text']
    require(len(rodata) == len(text) == 1, 'unexpected section set')
    rels = elf.relocations(rodata[0])
    require([r[0] for r in rels] == list(range(0, TABLE_SIZE, 4)), 'table relocation coverage changed')
    require(all(kind == owndata.R_MIPS_32 and symbol['section'] == text[0]
                for _offset, kind, symbol in rels), 'non-text switch-table entry')
    raw = elf.raw(rodata[0])
    require(len(raw) == 32 and not any(raw[TABLE_SIZE:]), 'nonzero data outside owned table')
    root_start = score.symbols(obj)['func_803A7918']
    resolved = []
    for offset, _kind, symbol in rels:
        addend = struct.unpack_from('>I', raw, offset)[0] + symbol['value']
        relative = addend - root_start
        require(relative % 4 == 0 and 0 <= relative < 576, 'table target leaves complete root body')
        resolved.append(0x803A7918 + relative)
    actual = struct.pack('>5I', *resolved)
    require(actual == table, 'complete table differs after independent entry mapping')
    return {'address': hex(TABLE_ADDRESS), 'size': TABLE_SIZE, 'entries': 5,
            'relocations': 'five R_MIPS_32 to in-root .text offsets',
            'sha256': digest(table), 'zero_section_alignment_bytes': len(raw) - TABLE_SIZE}


def installer_check(obj, data, base, binding):
    """Read-only actual installer relocation path; never calls splice or writes locks."""
    from tools.conveyor.pipeline import blob_group, ovl_splice
    installed_spec = ovl_splice.packet_spec(GROUP)
    require(installed_spec['members'] == MEMBERS and installed_spec['context'] == CONTEXT,
            'packet-to-installer conversion changed members/context')
    extents = {name: {'vaddr': int(e['address'], 16), 'size': e['size']}
               for name, e in binding['targets'].items() if name in MEMBERS}
    slices, text_ndx = blob_group.member_slices(obj, MEMBERS + CONTEXT, extents)
    bodies = blob_group.relocate(obj, slices, text_ndx, score.image_symbols(), members=MEMBERS,
                                image=(data, base))
    result = {}
    for name in MEMBERS:
        extent = extents[name]
        expected = data[extent['vaddr'] - base:extent['vaddr'] - base + extent['size']]
        require(bodies[name] == expected, 'installer relocated body mismatch: ' + name)
        result[name] = {'bytes': len(bodies[name]), 'sha256': digest(bodies[name])}
    # The same strict data gate must reject a changed table rather than accept code alone.
    bad = bytearray(data)
    bad[TABLE_ADDRESS - base] ^= 1
    try:
        blob_group.relocate(obj, slices, text_ndx, score.image_symbols(), members=MEMBERS,
                            image=(bytes(bad), base))
    except blob_group.GroupError:
        pass
    else:
        raise ValueError('installer accepted a corrupted owned table')
    return {'members': result, 'corrupt_table_rejected': True, 'writes_production_state': False}


def inputs():
    paths = ['groups/'+GROUP_NAME+'/group.c','groups/'+GROUP_NAME+'/group.json',
             'verify.py','capacity.py','host.c','gnu.py','trace.py']
    # Relevant source dependencies are inputs, not mutable tooling pins.
    helper_paths=['src/blob/func_800A1644.c','src/blob/func_800BE4F0.c','src/blob/func_800BE6A4.c','src/ovl_a/func_80398BF0.c']
    targets={}
    with target_scope():
        for name,size in SIZES.items():
            words=score.targets()[name]
            require(len(words)*4==size,'native extent changed')
            targets[name]={'address':hex(int(name[-8:],16)),'size':size,
                           'words_sha256':digest(struct.pack('>%dI'%len(words),*words))}
        words=score.targets()['func_80398BF0']
        targets['review:func_80398BF0']={'size':len(words)*4,'words_sha256':digest(struct.pack('>%dI'%len(words),*words))}
    previous=score.ASM_DIR
    try:
        score.ASM_DIR=ROOT/'asm/us/blob'
        for name in ('func_800A1644','func_800BE4F0','func_800BE6A4','track_process_main',
                     'track_render_process','no_catchup','func_800A4E58','func_800A510C',
                     'func_800A5158','audio_frame_sync','func_80097164','fcvt_wrapper',
                     'sound_pitch_diff_halved','object_manager_update','state_utility','menu_input_process',
                     'func_800B66B0','audio_doppler_calc','func_800A473C','func_80092DCC'):
            words=score.targets()[name]
            targets['game:'+name]={'size':len(words)*4,'words_sha256':digest(struct.pack('>%dI'%len(words),*words))}
    finally:
        score.ASM_DIR=previous
    formatter=(ROOT/'src/rom/lib_34a0.c').read_text()
    # Review input for native decimal/string semantics; never rebuild this ROM TU.
    begin=formatter.index('int fcvt(char *fp,');end=formatter.index('/* PROMOTED',begin)
    core=formatter[begin:end]
    begin=formatter.index('int sprintf(char *output,');end=formatter.index('\n}',begin)+2
    wrapper=formatter[begin:end]
    return {'formatter_review_sha256':{'fcvt':digest(core.encode()),'sprintf':digest(wrapper.encode())},
            'sources_sha256':{p:digest((HERE/p).read_bytes()) for p in paths},
            'helper_sources_sha256':{p:digest((ROOT/p).read_bytes()) for p in helper_paths},
            'targets':targets,'asset_sha256':ASSET_SHA256}


def writer_bound(data,base):
    previous=score.ASM_DIR
    try:
        score.ASM_DIR=ROOT/'asm/us/blob'
        return capacity.audit(data,base,score.targets(),score.image_symbols())
    finally:
        score.ASM_DIR=previous


def stack_census(words):
    accesses=[];addresses=[]
    for i,word in enumerate(words):
        op,base,reg,off=word>>26,(word>>21)&31,(word>>16)&31,word&65535
        off=off if off<32768 else off-65536
        if base==29 and op in (32,33,35,36,37,40,41,43,49,53,57,61):
            accesses.append([i*4,op,reg,off])
        if base==29 and op==9 and reg!=29: addresses.append([i*4,reg,off])
    require((words[0]&65535)==65536-168,'native frame changed')
    require({r[3] for r in accesses}=={16,20,24,36,52},'native stack-access domain changed')
    require({r[2] for r in addresses}=={52,68},'native buffer addresses changed')
    return {'frame':168,'accesses':accesses,'address_taking':addresses,
            'suffix_interval':[52,68],'text_interval':[68,144],
            'reconstructed_scalar_home_interval':[144,168],
            'live_objects_overlap':False,'scalar_home_note':'source-layout inference; not additional native accesses'}


def shell(*args):
    result=subprocess.run(list(map(str,args)),capture_output=True,text=True)
    require(result.returncode==0,result.stdout+result.stderr)
    return result.stdout


def host_check(strings,mapping,work):
    def literal(value): return '"'+''.join('\\%03o'%c for c in value)+'"'
    # Native payloads are local temporary compiler inputs, never committed.
    rows=[]
    for language in strings:
        labels=dict(language['labels']);labels.update({83:b'',84:b'',87:b''})
        rows.append('{'+','.join('[%d]=%s'%(i,literal(value)) for i,value in sorted(labels.items()))+'}')
    data='static char *native_labels[6][237]={'+','.join(rows)+'};\n'
    data+='static char *native_choices[6][2]={'+','.join('{'+','.join(literal(s) for s in l['choices'])+'}' for l in strings)+'};\n'
    data+='u16 D_8011EAEC[256]={'+','.join(map(str,mapping))+'};\n'
    (work/'language_data.h').write_text(data)
    source=(GROUP/'group.c').read_text()
    anchor='    char text[76];\n    char suffix[16];'
    require(source.count(anchor)==1,'host observer anchor changed')
    source=source.replace(anchor,anchor+'\n    observe_buffers(text,sizeof text,suffix,sizeof suffix);')
    (work/'candidate.h').write_text(source)
    native=''
    for old,new,path in [('func_800A1644','native_encode','src/blob/func_800A1644.c'),
                         ('func_800BE6A4','native_copy','src/blob/func_800BE6A4.c'),
                         ('func_800BE4F0','native_append','src/blob/func_800BE4F0.c')]:
        native+='#define '+old+' '+new+'\n#include "'+str(ROOT/path)+'"\n#undef '+old+'\n'
    (work/'native_helpers.h').write_text(native)
    cc=shutil.which('cc') or shutil.which('gcc');require(cc,'C compiler missing')
    shell(cc,'-std=c99','-O2','-Wall','-Wextra','-Werror','-Wno-unused-variable','-Wno-unused-parameter',
          '-ffunction-sections','-fdata-sections','-Wl,--gc-sections','-I',work,HERE/'host.c','-o',work/'host')
    result=json.loads(shell(work/'host'))
    require(result['maximum_text']==45 and result['maximum_suffix']==11,'host writer maximum changed')
    result.update(native_gameplay_equivalence=False,formatter='host hooks after native %d/%s path review',
                  helpers='actual matched encoder/copy/append C',source='candidate with one host-only buffer observer')
    return result


def replay(deep=False):
    score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO 5.3 required')
    binding=inputs();data,base,table=authenticate();bound,strings,mapping=writer_bound(data,base)
    spec=json.loads((GROUP/'group.json').read_text())
    require(spec['members']==spec['claims']==MEMBERS and not spec['context'] and spec['keep']==['func_803A7918'],
            'group ownership or genuine ABI root changed')
    require(not spec.get('allow_unverified'),'no unverified waiver allowed')
    baseline=ROOT/'src/ovl_a/groups'/GROUP_NAME
    # Compare executable text independently of explanatory headers/comments.
    strip=lambda s: re.sub(r'/\*.*?\*/','',s,flags=re.S).strip()
    # w25p: once installed, production equals this packet; before, it is the text[100] baseline.
    installed=strip((baseline/'group.c').read_text())
    require(strip((GROUP/'group.c').read_text()) in (installed,) or
            strip((GROUP/'group.c').read_text()).replace('char text[76];','char text[100];')==installed,
            'change exceeds sole bounded capacity edit')
    meta=metadata();extents={e['name']:e for e in meta['functions']}
    with target_scope():
        for name,size in SIZES.items():
            address=int(name[-8:],16)
            following=min(int(e['address'],16) for e in meta['functions'] if int(e['address'],16)>address)
            require(int(extents[name]['address'],16)==address and extents[name]['size']==size and following==address+size,'native boundary mismatch')
            require(data[address-base:address-base+size]==struct.pack('>%dI'%(size//4),*score.targets()[name]),'target/image body binding changed')
    image=owndata.ImageData.from_image(data,base)
    with tempfile.TemporaryDirectory(prefix='a7180-writer-') as tmp,target_scope(image):
        work=Path(tmp);obj=work/'group.o';score.compile_group(GROUP,obj)
        results={n:score.compare(obj,n,show=0) for n in MEMBERS}
        require(all(r.accepted() for r in results.values()),'a complete member failed strict scoring')
        elf=owndata._Object(obj)
        symbols=[s for i,sec in enumerate(elf.sections) if sec['type']==2 for s in elf.symbols(i)]
        ti=next(i for i,s in enumerate(elf.sections) if s['name']=='.text');text=elf.raw(ti)
        funcs={s['name']:s for s in symbols if s['type']==2 and s['section']==ti}
        for n,size in SIZES.items():
            f=funcs[n];end=f['value']+size
            following=min((s['value'] for s in funcs.values() if s['value']>f['value']),default=len(text))
            require(f['size']==size and following>=end and not any(text[end:following]),'function extent/extra word changed')
        owned=table_structure(obj,data,base,table)
        key=owndata.artifact_dir(score.ASM_DIR)
        for entry in range(5):
            corrupted=bytearray(data);corrupted[TABLE_ADDRESS-base+entry*4]^=1
            score._own_data[key]=owndata.ImageData.from_image(corrupted,base)
            require(not score.compare(obj,'func_803A7918',show=0).accepted(),'corrupt table entry accepted')
        score._own_data[key]=None
        absent=score.compare(obj,'func_803A7918',show=0)
        require(not absent.accepted() and len(absent.unverified)==2,'missing table accepted')
        score._own_data[key]=image
        out={'inputs':binding,'comparisons':{n:asdict(r) for n,r in results.items()},'capacity':bound,
             'stack':stack_census(score.targets()[NAME]),'owned_table':owned,
             'controls':{'all_five_owned_entries_rejected':True,'missing_data_unverified_references':2},
             'new_candidate_bytes':1184,'preserved_candidate_bytes':1932,'accepted_image_bytes':0,'rom_bytes':0}
        # Baseline and one independently safe smaller control are bounded causal tests.
        controls={}
        for capacity_value in (100,72):
            folder=work/('capacity%d'%capacity_value);folder.mkdir()
            (folder/'group.c').write_text((GROUP/'group.c').read_text().replace('char text[76];','char text[%d];'%capacity_value))
            (folder/'group.json').write_bytes((GROUP/'group.json').read_bytes())
            score.compile_group(folder,folder/'group.o')
            r=score.compare(folder/'group.o',NAME,show=0)
            require(r.differing=={100:2,72:6}[capacity_value] and not any((r.errors,r.unresolved,r.unverified,r.extra_words)),'capacity control changed')
            require(all(score.compare(folder/'group.o',n,show=0).accepted() for n in MEMBERS if n!=NAME),'control regressed preserved member')
            controls[str(capacity_value)]=asdict(r)
        out['capacity_controls']=controls
        if deep:
            g=importlib.util.spec_from_file_location('writer_gnu',HERE/'gnu.py');gm=importlib.util.module_from_spec(g);g.loader.exec_module(gm)
            out['gnu']=gm.check(obj,work,data,base,SIZES,score,owndata)
            baseline_work=work/'baseline_gnu';baseline_work.mkdir()
            out['baseline_gnu']=gm.check(work/'capacity100/group.o',baseline_work,data,base,SIZES,score,owndata,{NAME:[0,0x494]})
            out['installer']=installer_check(obj,data,base,binding)
            out['host']=host_check(strings,mapping,work)
            out['trace']=json.loads(shell(sys.executable,HERE/'trace.py'))
            # Match the real text helpers as input proofs; no static ROM TU is rebuilt.
            previous=score.ASM_DIR
            try:
                score.ASM_DIR=ROOT/'asm/us/blob'
                for name in ('func_800A1644','func_800BE6A4','func_800BE4F0'):
                    helper=work/(name+'.o');score.compile_single(ROOT/'src/blob'/(name+'.c'),score.DEFAULT_FLAGS,helper)
                    require(score.compare(helper,name,show=0).accepted(),'native writer helper source no longer exact')
            finally:score.ASM_DIR=previous
            getter=work/'getter.o'
            score.compile_single(ROOT/'src/ovl_a/func_80398BF0.c',score.DEFAULT_FLAGS,getter)
            require(score.compare(getter,'func_80398BF0',show=0).accepted(),'native Pak list getter no longer exact')
            abi=work/'abi.c';abi.write_text('#include "'+str(GROUP/'group.c')+'"\n'
                '#define CHECK(n,e) typedef char n[(e)?1:-1]\n'
                'CHECK(ptr32,sizeof(void*)==4);CHECK(word32,sizeof(s32)==4);CHECK(short16,sizeof(s16)==2);\n'
                'CHECK(name,(u32)&((Record*)0)->name==18);CHECK(suffix,(u32)&((Record*)0)->suffix==53);CHECK(blocks,(u32)&((Record*)0)->blocks==68);\n'
                'CHECK(labels,(u32)&((Language*)0)->labels==4);CHECK(indices,(u32)&((Language*)0)->indices==12);CHECK(choices,(u32)&((Language*)0)->choices==16);\n'
                'CHECK(file_action,(u32)&((TextIndices*)0)->file_action==94);\n')
            score.compile_single(abi,score.DEFAULT_FLAGS,work/'abi.o')
            out['abi']='native scalar/pointer widths and every accessed Record/Language/index offset checked; no static ROM TU rebuilt'
        return out


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text())
    actual=json.loads(json.dumps(replay(deep)))
    require(all(expected[k]==v for k,v in actual.items()),'input-bound receipt mismatch')
    return actual


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true')
    args=parser.parse_args()
    if args.score:
        data,base,_=authenticate();previous=sys.argv
        try:
            sys.argv=['score.py','group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')]
            with target_scope(owndata.ImageData.from_image(data,base)):return score.main()
        finally:sys.argv=previous
    if args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    else:verify(args.deep)
    print('PASS: A7180 1184 fresh exact candidate bytes; 1932 preserved; all-writer bound45; no new ROM coverage')
    return 0


if __name__=='__main__':sys.exit(main())
