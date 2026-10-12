#!/usr/bin/env python3
"""Full native/GNU/installer proof for two genuine image-B HUD siblings."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
MEMBERS = {'func_80391650': 532, 'func_80391864': 668}
CONTEXT = 'func_80391B00'
NAMES = [*MEMBERS, CONTEXT]
ADDRESS, RODATA = 0x80391650, 0x80394F08
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
BASELINE = ROOT / 'cloud/work/frontier/dot_runtime_b_status_scene_20261012'
TEST = ROOT / 'tests/cloud/test_runtime_b_kind_scene.py'
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers


def require(ok, message):
    if not ok:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def image_bytes():
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8', 'asset identity changed')
    require(struct.unpack_from('>I', asset, 0x3854)[0] == 0xB6FEC4, 'loader pointer changed')
    image = zlib.decompress(asset[0xB6FEC4 - 0x283D0:], -15)
    require(len(image) == 43888 and sha(image) == 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd', 'image identity changed')
    return image


@contextmanager
def targets(image):
    old = score.ASM_DIR
    directory = ROOT / 'asm/us/ovl_b'
    key = owndata.artifact_dir(directory)
    present, previous = key in score._own_data, score._own_data.get(key)
    score.ASM_DIR = directory
    score._own_data[key] = None if image is None else owndata.ImageData.from_image(image, 0x8038A400)
    try:
        yield
    finally:
        score.ASM_DIR = old
        if present:
            score._own_data[key] = previous
        else:
            score._own_data.pop(key, None)


def native_calls(words, address):
    return [i * 4 for i, w in enumerate(words) if w >> 26 == 3 and
            ((w & 0x3ffffff) << 2 | 0x80000000) == address]


def stack_zero(word):
    return (word >> 26, word >> 21 & 31, word >> 16 & 31, word & 65535) == (43, 29, 0, 0)


def inputs():
    image = image_bytes()
    source=(HERE/'group.c').read_text();baseline=(BASELINE/'group.c').read_text()
    def region(text,begin,end=None):
        start=text.index(begin)
        return text[start:text.index(end,start) if end else len(text)]
    require(region(source,'s32 func_80391650(void *unused)','s32 func_80391864(void *unused)') ==
            region(baseline,'s32 func_80391650(void *unused)','s32 func_80391864(void *unused)'), 'accepted sibling source changed')
    require(region(source,'void func_80391B00(void)')==region(baseline,'void func_80391B00(void)'), 'genuine parent source changed')
    with targets(image):
        table = score.targets()
        rows = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', score.target_manifest()))['functions']
        extents = {n: next(row for row in rows if row['name'] == n) for n in NAMES}
        for n, row in extents.items():
            addr = int(n[-8:], 16)
            require(int(row['address'], 16) == addr and row['size'] == len(table[n])*4,
                    'native name/extent disagreement')
            require(packed(table[n]) == image[addr-0x8038A400:addr-0x8038A400+row['size']], 'target/image disagreement')
        require([extents[n]['size'] for n in NAMES] == [532, 668, 2380], 'native extent changed')
        callers = {n: {c: native_calls(w, int(n[-8:],16)) for c,w in table.items()
                       if native_calls(w, int(n[-8:],16))} for n in MEMBERS}
        require(callers == {'func_80391650': {CONTEXT: [0x5c]}, 'func_80391864': {CONTEXT: [0x54]}}, 'caller census changed')
        require(all(stack_zero(table[CONTEXT][o//4+1]) for o in (0x54,0x5c)), 'native zero argument changed')
        symbols = score.image_symbols()
        helpers = ['sound_call_minimal','string_copy_format','func_8008E398','model_data_load',
                   'model_transform_setup','func_8008B32C','func_80090E9C','car_lights_render','func_8008E06C']
        names = helpers + ['D_80151AD0','D_80152818','D_80395C00','D_80395CD0','D_80150B70',
                           'D_80393B68','D_80140BDC','D_80393B74','D_8012E700','D_80116178',
                           'D_80394210','D_803942A4','D_801427C0','D_803941D0']
        bindings = {n: symbols.get(n, score.address_named(n)) for n in names}
        require(all(isinstance(x,int) for x in bindings.values()), 'unknown binding')
    old = score.ASM_DIR
    try:
        score.ASM_DIR = ROOT/'asm/us/blob'; game=score.targets(); symbols=score.image_symbols()
        helper_words={}
        for n in helpers:
            matching=[k for k in game if symbols.get(k,score.address_named(k))==bindings[n]]
            require(len(matching)==1,'ambiguous helper target')
            k=matching[0];helper_words[n]={'target':k,'size':len(game[k])*4,'sha256':sha(packed(game[k]))}
    finally: score.ASM_DIR=old
    files=[HERE/f for f in ('group.c','group.json','verify.py','layout_check.c','host.c','trace.py')]
    files += [TEST, BASELINE/'group.c',BASELINE/'group.json']
    owned = image[RODATA-0x8038A400:RODATA-0x8038A400+44]
    require(owned[:12] == struct.pack('>fff',0.025,0.069,0.175), 'literal order changed')
    return {'sources_sha256':{str(f.relative_to(ROOT)):sha(f.read_bytes()) for f in files},
            'population':'ovl_b','extents':extents,'native_words_sha256':{n:sha(packed(table[n])) for n in NAMES},
            'native_callers':callers,'native_stack_argument':{'offset':0,'value':0},
            'bindings':bindings,'helper_words':helper_words,'flags':FLAGS,'compiler':'ido5.3',
            'owned_data':{'address':RODATA,'size':44,'sha256':sha(owned)},'image_sha256':sha(image)}


def inspect(obj, linked=False):
    raw, secs=score._elf(obj); header=struct.unpack_from('>16sHHIIIIIHHHHHH',raw)
    require(header[0][:9]==b'\x7fELF\x01\x02\x01\0\0' and header[1]==(2 if linked else 1) and
            header[2]==8 and header[7] in (0x10000000,0x10000001),'ELF ABI changed')
    syms={s['name']:s for i,sec in enumerate(secs) if sec['type']==2
          for s in score._symbol_table(raw,secs,i) if s['name']}
    ti=score._text_index(secs)
    funcs={n:s for n,s in syms.items() if s['type']==2 and s['section'] not in (0,0xfff1)}
    require(set(funcs)==set(NAMES),'unaccounted function')
    cursor=ADDRESS if linked else 0
    for n,f in sorted(funcs.items(),key=lambda x:x[1]['value']):
        require(f['section']==ti and f['value']==cursor and f['size']>0 and f['size']%4==0,'bad whole extent')
        cursor+=f['size']
    allocated={}
    for i,sec in enumerate(secs):
        row=struct.unpack_from('>IIIIIIIIII',raw,header[6]+i*header[11])
        if linked: require(sec['type'] not in (4,9) or not sec['size'],'residual relocation')
        if row[2]&2 and sec['size']:
            if sec['name'] in ('.text','.rodata'):
                require(row[3]==((ADDRESS if sec['name']=='.text' else RODATA) if linked else 0),'section address changed')
                allocated[sec['name']]=raw[sec['off']:sec['off']+sec['size']]
            else: require(not linked and sec['name'] in ('.reginfo','.options'),'unrecognized allocation')
    require(set(allocated)=={'.text','.rodata'},'missing section')
    suffix=allocated['.text'][cursor-(ADDRESS if linked else 0):]
    require(len(suffix)<16 and not any(suffix),'nonzero/excess suffix')
    return raw,secs,syms,funcs,allocated


def link_complete(obj, temp, tag):
    raw,secs,syms,funcs,allocated=inspect(obj)
    addresses=score.image_symbols()
    undefined={n:addresses.get(n,score.address_named(n)) for n,s in syms.items() if s['section']==0}
    require(all(v is not None for v in undefined.values()),'unknown external')
    counts={'.text':0,'.rodata':0};per_function={n:0 for n in NAMES}
    for sec in secs:
        if sec['type']!=9:continue
        target=secs[sec['info']]; require(target['name'] in counts,'unknown relocation section')
        for pos in range(sec['off'],sec['off']+sec['size'],8):
            off,info=struct.unpack_from('>II',raw,pos)
            require(off%4==0 and off+4<=target['size'],'relocation outside section')
            require(info&255 in ((4,5,6) if target['name']=='.text' else (2,)),'unknown relocation kind')
            counts[target['name']]+=1
            if target['name']=='.text':
                found=[n for n,f in funcs.items() if f['value']<=off<f['value']+f['size']]
                require(len(found)==1,'relocation not owned by complete function');per_function[found[0]]+=1
    script=temp/(tag+'.ld');script.write_text('\n'.join('%s = 0x%08x;' % x for x in sorted(undefined.items()))+
       '\nSECTIONS { .text 0x80391650 : SUBALIGN(4) { *(.text) } .rodata 0x80394F08 : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
    out=temp/(tag+'.elf')
    subprocess.run(['mips-linux-gnu-ld','-EB','--hash-style=sysv','-T',str(script),'-o',str(out),str(obj)],check=True,capture_output=True)
    _,_,bound,linked_functions,linked=inspect(out,True)
    require(not [n for n,s in bound.items() if s['section']==0],'unresolved linked name')
    require(all(bound[n]['value']==a and bound[n]['section']==0xfff1 for n,a in undefined.items()),'external address changed')
    bodies={n:linked['.text'][f['value']:f['value']+f['size']] for n,f in funcs.items()}
    deltas={}
    for n,body in bodies.items():
        target=packed(score.targets()[n]);shared=min(len(body),len(target))
        diff=sum(body[i:i+4]!=target[i:i+4] for i in range(0,shared,4))
        diff+=(len(target)-shared)//4
        extra=sum(any(body[i:i+4]) for i in range(len(target),len(body),4))
        deltas[n]={'differing':diff,'nonzero_excess':extra,'compiled_bytes':len(body),'native_bytes':len(target)}
    return bodies,linked,{'relocations':counts,'per_function_text_relocations':per_function,
            'whole_sections':{n:len(b) for n,b in allocated.items()},'complete_extents':{n:{'offset':f['value'],'size':f['size']} for n,f in funcs.items()},
            'full_word_deltas':deltas,'external_bindings':undefined}


def installer_check(obj,work,image):
    from tools.conveyor.pipeline import blob_group,ovl_splice
    spec=ovl_splice.packet_spec(HERE);name=spec['name']
    require(spec['members']==list(MEMBERS) and spec['context']==[CONTEXT],'installer membership')
    root=work/'installer-groups';objects=work/'installer-objects';group=root/name
    group.mkdir(parents=True);objects.mkdir()
    (group/'group.json').write_text(json.dumps(spec));shutil.copyfile(HERE/'group.c',group/'group.c')
    shutil.copyfile(obj,objects/(name+'.o'));native=work/'installer-image.bin';native.write_bytes(image)
    document={'image':{'path':str(native),'base':'0x8038A400'},'regions':[{'entries':[
        dict(target_id=n,kind='function',vaddr=int(n[-8:],16),size=len(score.targets()[n])*4) for n in NAMES]}]}
    bodies=blob_group.group_bodies(name,document=document,extern=score.image_symbols(),obj_dir=objects,root=root)
    require(set(bodies)==set(MEMBERS),'installer output contains context or lacks member')
    replaced=bytearray(image)
    for n,b in bodies.items():
        require(len(b)==MEMBERS[n] and ovl_splice.mismatch(document,n,b) is None,'installer full body differs')
        start=int(n[-8:],16)-0x8038A400;replaced[start:start+len(b)]=b
    require(bytes(replaced)==image,'selective image differs')
    return {'members':{n:len(b) for n,b in bodies.items()},'nonmatch_parent_excluded':True,
            'selective_image_sha256':sha(bytes(replaced)),'production_state_written':False,'rom_verified':False}


def private_saves(words):
    return {'integer': sorted({(w>>16)&31 for w in words if w>>26==43 and (w>>21)&31==29 and 16<=((w>>16)&31)<=23 or w>>26==43 and (w>>21)&31==29 and ((w>>16)&31)==30}),
            'float_pairs': sorted({(w>>16)&31 for w in words if w>>26==61 and (w>>21)&31==29 and ((w>>16)&31)>=20})}


def replay(deep=False):
    binding=inputs();score.ido('cc')
    require(compilers.identify_dir(score.IDO)=='ido5.3','not recognized stock IDO5.3')
    compiler={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}
    require(shutil.which('mips-linux-gnu-ld') is not None,'GNU linker missing')
    image=image_bytes()
    with targets(image),tempfile.TemporaryDirectory(prefix='dot-b-kind-') as name:
        tmp=Path(name);obj=tmp/'group.o';spec=score.compile_group(HERE,obj)
        require(spec['members']==spec['claims']==list(MEMBERS) and spec['context']==[CONTEXT] and
                spec['keep']==[CONTEXT] and spec['name']=='runtime_b_hud_scenes_20261006','group contract')
        results={n:score.compare(obj,n,show=0) for n in NAMES}
        require(all(results[n].accepted() for n in MEMBERS),'claimed member did not match')
        require(not results[CONTEXT].accepted(),'verified context must be a member')
        bodies,linked,facts=link_complete(obj,tmp,'candidate')
        for n,size in MEMBERS.items():
            require(bodies[n]==packed(score.targets()[n]) and len(bodies[n])==size,'GNU complete member differs')
        require(linked['.rodata'][:44]==image[RODATA-0x8038A400:RODATA-0x8038A400+44],'GNU full literals/table differ')
        require(facts['complete_extents'][CONTEXT]['offset']==1200,'native following boundary')
        # Native/compiled complete parent both call the exact private entries with zero in stack argument 0.
        parent=list(struct.unpack('>%dI'%(len(bodies[CONTEXT])//4),bodies[CONTEXT]))
        compiled_callers={n:native_calls(parent,int(n[-8:],16)) for n in MEMBERS}
        require(compiled_callers=={'func_80391650':[0x5c],'func_80391864':[0x54]},'compiled caller boundary changed')
        require(all(stack_zero(parent[o//4+1]) for o in (0x54,0x5c)),'compiled zero argument changed')
        native_saves=private_saves(score.targets()[CONTEXT][:0x54//4])
        compiled_saves=private_saves(parent[:0x54//4])
        require(native_saves==compiled_saves=={'integer':[16,17,18,19,20,21,22,23,30],
                                             'float_pairs':[20,22,24,26,28,30]}, 'genuine private caller preservation changed')
        for n,offset in [('func_80391650',0),('func_80391864',4),('func_80391864',12)]:
            with targets(None):
                bad=score.compare(obj,n,show=0);require(not bad.accepted() and bad.unverified,'absent owned accepted')
            corrupt=bytearray(image);corrupt[RODATA-0x8038A400+offset]^=1
            with targets(bytes(corrupt)):
                bad=score.compare(obj,n,show=0);require(not bad.accepted() and bad.errors,'corrupt owned accepted')
        subprocess.run([score.ido('cc'),'-g0','-O2','-mips2','-G','0','-non_shared','-I',str(HERE),'-c',str(HERE/'layout_check.c'),'-o',str(tmp/'layout.o')],check=True,capture_output=True,cwd=tmp)
        record={'inputs':binding,'compiler_sha256':compiler,'comparison':{n:asdict(x) for n,x in results.items()},
                'gnu_proof':facts,'installer':installer_check(obj,tmp,image),'compiled_private_calls':compiled_callers,'native_and_compiled_caller_saves':native_saves,
                'owned_data_sha256':sha(linked['.rodata'][:44]),'controls':{'missing_data':'rejected','bad_literal':'rejected','bad_table':'rejected','native_c89_layout':'passed'},
                'new_strict_verified_bytes':668,'preserved_candidate_bytes':532,'new_owned_bytes':40,'preserved_owned_bytes':4,
                'new_image_landed_bytes':0,'new_rom_verified_bytes':0}
        if deep: deep_checks(tmp)
        return record


def deep_checks(tmp):
    source=(HERE/'group.c').read_text();baseline=(BASELINE/'group.c').read_text()
    start=source.index('s32 func_80391864(void *unused)');end=source.index('s32 func_80391650(void *);',start)
    body=source[start:end]
    def variant(new): return source[:start]+new+source[end:]
    no_player=''.join(line for line in body.splitlines(True) if line.strip() not in ('s32 player_kind;', 'player_kind = D_80152818[i].kind;')).replace('if (player_kind != kind)','if (D_80152818[i].kind != kind)').replace('D_80395C00[i].kind = player_kind;','D_80395C00[i].kind = D_80152818[i].kind;')
    variants={'baseline':baseline,'no_player_carrier':variant(no_player),
              'point_order':variant(body.replace('    Point point;\n    s32 i, kind, resource;','    s32 i, kind, resource;\n    Point point;')),
              'uv_pointer':variant(body.replace('    s32 player_kind;','    s32 player_kind;\n    Status *status;').replace('        player_kind = D_80152818[i].kind;','        status = &D_80395C00[i];\n        player_kind = D_80152818[i].kind;',1).replace('D_80395C00[i].uv','status->uv'))}
    for tag,text in variants.items():
        directory=tmp/tag;directory.mkdir();(directory/'group.c').write_text(text);(directory/'group.json').write_bytes((HERE/'group.json').read_bytes())
        obj=tmp/(tag+'.o');score.compile_group(directory,obj)
        old=score.compare(obj,'func_80391864',show=0)
        require(score.compare(obj,'func_80391650',show=0).accepted(),'control lost inherited member')
        bodies,linked,facts=link_complete(obj,tmp,tag)
        d=facts['full_word_deltas']['func_80391864']
        require(d['differing']==old.differing and d['nonzero_excess']==old.extra_words,'GNU control disagrees')
        if tag=='baseline':require(old.differing==130 and old.extra_words==1 and d['compiled_bytes']==676,'baseline changed')
        else:require(old.differing>0,'causal rollback no longer causal')
        print('PASS causal',tag,old.summary(),'complete bytes',d['compiled_bytes'])
    host_test(tmp,False);host_test(tmp,True)


def host_test(tmp,sanitize=False):
    baseline=(BASELINE/'group.c').read_text();a=baseline.index('s32 func_80391864(void *unused)');b=baseline.index('s32 func_80391650(void *);',a)
    (tmp/'baseline.inc').write_text(baseline[a:b].replace('func_80391864','baseline_kind_scene'))
    out=tmp/('host-san' if sanitize else 'host')
    cmd=['cc','-std=c99','-O2','-fstrict-aliasing','-ffunction-sections','-fdata-sections','-Wl,--gc-sections','-Wno-pointer-to-int-cast','-Wno-int-to-pointer-cast','-I',str(HERE),'-I',str(tmp),str(HERE/'host.c'),'-o',str(out)]
    if sanitize:cmd[1:1]=['-fsanitize=address,undefined','-fno-sanitize-recover=all']
    subprocess.run(cmd,check=True,capture_output=True)
    import os
    env=dict(os.environ);env['ASAN_OPTIONS']='detect_leaks=0'
    result=subprocess.run([str(out)],check=True,capture_output=True,text=True,env=env)
    print(result.stdout.strip(),'(ASan/UBSan)' if sanitize else '(ordinary)')


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text())
    require(inputs()==expected['inputs'],'input receipt changed')
    actual=replay(deep);require(actual==expected,'verification receipt changed');return actual


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--score',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--inputs-only',action='store_true');args=parser.parse_args()
    if args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2)+'\n');print('Recorded full two-member proof')
    elif args.inputs_only:require(inputs()==json.loads((HERE/'verification.json').read_text())['inputs'],'input receipt changed');print('PASS inputs')
    elif args.score:
        with targets(image_bytes()):
            sys.argv=['score.py','group',str(HERE),'--targets',str(ROOT/'asm/us/ovl_b')];raise SystemExit(score.main())
    else:verify(args.deep);print('PASS complete GNU/native members, owned literals/table, caller, installer and receipt')
