#!/usr/bin/env python3
"""Input-bound image-A profile-controller proof; transient native and ELF data never published."""
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
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers

GROUP_NAME = 'runtime_a_profile_menu_20261006'
GROUP = HERE / 'groups' / GROUP_NAME
NEW = 'func_80396FA0'
PRESERVED = {'func_80395E24':104,'func_803A87D4':920,'func_803A8B74':1300,
             'func_80396EE8':92,'func_80396F44':92,'func_803A7B58':3196,
             'func_80394564':720,'func_80394B00':728}
CHECKED = {**PRESERVED,NEW:2872}
CONTEXT = ['func_803A9088','func_803A9EEC',NEW,'func_8039842C']
KEEP = ['func_803A9EEC','func_8039842C']
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
OWNED = {NEW:(0,56,0x803B9484),'func_803A7B58':(120,36,0x803B9848)}

def require(value, message):
    if not value:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def image_data():
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == ASSET_HASH, 'current-tree asset identity changed')
    require(struct.unpack_from('>I', asset, 0x3850)[0] == 0xB5C534, 'loader pointer changed')
    image = zlib.decompress(asset[0xB5C534 - 0x283D0:], -15)
    require(len(image) == 194128 and sha(image) == IMAGE_HASH, 'image-A identity changed')
    return image


@contextmanager
def target_context():
    old = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    key = owndata.artifact_dir(score.ASM_DIR)
    prior = score._own_data.get(key)
    image = image_data()
    score._own_data[key] = owndata.ImageData.from_image(image, 0x8038A400)
    try:
        yield image
    finally:
        score.ASM_DIR = old
        if prior is None:
            score._own_data.pop(key, None)
        else:
            score._own_data[key] = prior



def elf(path):
    """Independent ELF32 BE reader for GNU and complete symbol/relocation checks."""
    data = path.read_bytes()
    require(data[:7] == b'\x7fELF\x01\x02\x01' and struct.unpack_from('>H', data, 18)[0] == 8, 'not BE ELF32 MIPS')
    require(struct.unpack_from('>H', data, 16)[0] in (1,2) and struct.unpack_from('>I',data,36)[0]&0xf0000000==0x10000000,'ELF MIPS2 identity')
    base = struct.unpack_from('>I', data, 32)[0]
    stride, count, names_index = struct.unpack_from('>HHH', data, 46)
    require(stride == 40, 'section stride changed')
    rows = [struct.unpack_from('>10I', data, base + i * stride) for i in range(count)]
    names_row = rows[names_index]
    names = data[names_row[4]:names_row[4] + names_row[5]]
    sections, symbols, tables, relocs = {}, {}, {}, []
    for index, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        raw = data[row[4]:row[4] + row[5]]
        sections[name] = (index, row, raw)
        if row[1] == 2:
            string_row = rows[row[6]]
            strings = data[string_row[4]:string_row[4] + string_row[5]]
            table = []
            for offset in range(0, len(raw), 16):
                no, value, size, info, other, section = struct.unpack_from('>IIIBBH', raw, offset)
                name = strings[no:].split(b'\0')[0].decode()
                table.append(name)
                if name:
                    symbols[name] = (value, size, info & 15, section)
            tables[index] = table
    for row in rows:
        if row[1] in (4, 9):
            require(row[1] == 9, 'unexpected RELA')
            for offset in range(row[4], row[4] + row[5], 8):
                at, info = struct.unpack_from('>II', data, offset)
                relocs.append((at, info & 255, tables[row[6]][info >> 8], row[7]))
    return sections, symbols, relocs



def function(source, name):
    import re
    match = re.search(r'^(?:void|s32) '+name+r'\([^;]*?\)\s*\{', source, re.M)
    require(match is not None, 'function missing: '+name)
    start=match.start(); opening=source.index('{',start); depth=1; end=opening+1
    while depth:
        require(end<len(source),'unterminated function')
        depth += (source[end]=='{')-(source[end]=='}'); end+=1
    return start,end,source[start:end]


def replace_function(source, name, body):
    a,b,_=function(source,name);return source[:a]+body+source[b:]


def emitted_census(obj):
    sections,symbols,relocs=elf(obj);ti,_,text=sections['.text']
    functions={n:q for n,q in symbols.items() if q[2]==2 and q[3]!=0}
    require(set(functions)==set(CHECKED)|set(CONTEXT),'unaccounted defined STT_FUNC')
    cursor=0;result={}
    for name,(start,size,kind,section) in sorted(functions.items(),key=lambda q:q[1][0]):
        require(section==ti and start%4==0 and size>0 and size%4==0,'bad emitted function extent')
        require(cursor<=start and start+size<=len(text),'overlapping or truncated emitted function')
        require(not any(text[cursor:start]),'unaccounted nonzero inter-function text')
        result[name]=dict(offset=start,bytes=size,preceding_zero_padding=start-cursor)
        cursor=start+size
    require(not any(text[cursor:]),'unaccounted final nonzero text suffix')
    return dict(functions=result,text_bytes=len(text),final_zero_padding=len(text)-cursor)


def gnu_verify(obj,work,image):
    """Resolve original REL records through independent GNU as/ld, never masks."""
    sections,symbols,relocs=elf(obj);ti,_,raw=sections['.text'];ri,_,data=sections['.rodata']
    require(set(n for n,s in sections.items() if s[1][2]&2 and s[1][5])=={'.text','.rodata','.reginfo'},'unexpected allocated section')
    lines=['.set noreorder','.set noat'];external=set();counts={}
    def owner_at(addend):
        rows=[(n,s) for n,s in symbols.items() if s[2]==2 and s[3]==ti and s[0]<=addend<s[0]+s[1]]
        require(len(rows)==1,'unowned text anchor');return rows[0]
    for name,size in CHECKED.items():
        start,actual,kind,section=symbols[name]
        require((actual,kind,section)==(size,2,ti),'complete input extent')
        following=min((s[0] for s in symbols.values() if s[2]==2 and s[3]==ti and s[0]>start),default=len(raw))
        require(following>=start+size and not any(raw[start+size:following]),'nonzero suffix')
        words=list(struct.unpack('>%dI'%(size//4),raw[start:start+size]));entries=[]
        for loc,typ,symbol,dest in relocs:
            if dest!=ti or not start<=loc<start+size:continue
            require(typ in (4,5,6) and loc%4==0,'unsupported text relocation')
            off=loc-start
            if symbol=='.text':
                require(typ==4,'unsupported text anchor');addend=(words[off//4]&0x3ffffff)*4
                symbol,owner=owner_at(addend);words[off//4]=(words[off//4]&0xfc000000)|((addend-owner[0])//4)
            if symbol=='.rodata':
                require(name in OWNED,'unexpected data owner');symbol='owned_anchor_'+name
            require(symbol and not symbol.startswith('.'),'unowned local data')
            entries.append((off,typ,symbol));external.add(symbol)
        lines+=['.section .review_'+name+',"ax",@progbits','.balign 4','.globl '+name,'.type '+name+',@function',name+':']
        lines+=['.word 0x%08x'%w for w in words]+['.size %s,.-%s'%(name,name)]
        lines+=['.reloc %s+%d,%s,%s'%(name,o,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[t],s) for o,t,s in entries]
        counts[name]=len(entries)
    tables={}
    for name,(offset,size,address) in OWNED.items():
        require(offset+size<=len(data),'missing owned extent');words=list(struct.unpack('>%dI'%(size//4),data[offset:offset+size]));entries=[]
        for loc,typ,symbol,dest in relocs:
            if dest!=ri or not offset<=loc<offset+size:continue
            require(typ==2 and symbol=='.text' and loc%4==0,'owned table relocation')
            off=loc-offset;owner_name,owner=owner_at(words[off//4]);require(owner_name==name,'table owner changed')
            words[off//4]-=owner[0];entries.append(off)
        expected=list(range(0,56,4)) if name==NEW else list(range(12,36,4))
        require(entries==expected,'incomplete table entries');tables[name]=len(entries)
        label='owned_start_'+name
        lines+=['.section .owned_'+name+',"a",@progbits','.globl '+label,label+':','.globl owned_anchor_'+name,'owned_anchor_%s = . - %d'%(name,offset)]
        lines+=['.word 0x%08x'%w for w in words]
        lines+=['.reloc %s+%d,R_MIPS_32,%s'%(label,o,name) for o in entries]
    addresses=score.image_symbols();bindings={n:addresses.get(n,score.address_named(n)) for n in external-set(CHECKED)-{'owned_anchor_'+n for n in OWNED}}
    require(all(a is not None for a in bindings.values()),'unknown external symbol')
    asm,reloc,script,linked=[work/n for n in ('review.s','review.o','review.ld','review.elf')]
    asm.write_text('\n'.join(lines)+'\n')
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in CHECKED)
        +''.join('.owned_%s 0x%X : SUBALIGN(4) { *(.owned_%s) }\n'%(n,r[2],n) for n,r in OWNED.items())
        +'/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'
        +''.join('%s = 0x%X;\n'%(n,a) for n,a in sorted(bindings.items())))
    for cmd in (['mips-linux-gnu-as','-EB','-mips2','-o',str(reloc),str(asm)],['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(reloc)]):
        require(shutil.which(cmd[0]),'GNU tool absent: '+cmd[0]);subprocess.run(cmd,check=True,capture_output=True)
    ss,sy,rr=elf(linked);require(not rr,'remaining GNU relocations');require(not any(s[3]==0 for s in sy.values()),'undefined GNU symbol');result={}
    for name,size in CHECKED.items():
        i,row,body=ss['.review_'+name];address=int(name[-8:],16)
        require(row[3]==address and len(body)==size and sy[name]==(address,size,2,i),'GNU complete extent')
        native=packed(score.targets()[name]);offsets=[o for o in range(0,size,4) if body[o:o+4]!=native[o:o+4]]
        require(len(offsets)==(31 if name==NEW else 0),'GNU full-word residual changed')
        result[name]=dict(bytes=size,relocations=counts[name],differing_offsets=offsets,linked_sha256=sha(body))
    require(set(n for n,s in ss.items() if s[1][2]&2 and s[1][5])=={'.review_'+n for n in CHECKED}|{'.owned_'+n for n in OWNED},'unexpected GNU allocated section')
    owned={}
    for name,(offset,size,address) in OWNED.items():
        i,row,body=ss['.owned_'+name];native=image[address-0x8038A400:address-0x8038A400+size]
        require(row[3]==address and row[5]==len(body)==size and sy['owned_anchor_'+name]==(address-offset,0,0,i),'owned extent')
        require(body==native,'owned data mismatch');owned[name]=dict(bytes=size,address=hex(address),sha256=sha(body),table_relocations=tables[name])
    return dict(functions=result,owned=owned,external_symbols={n:hex(a) for n,a in sorted(bindings.items())})


def stack_census(words):
    accesses=[];addresses=set();adjustments=[]
    for i,w in enumerate(words):
        op,base,reg,off=w>>26,(w>>21)&31,(w>>16)&31,w&65535
        off=off if off<32768 else off-65536
        if base==29 and op in (32,33,35,36,37,40,41,43,49,53,57,61):accesses.append([i*4,op,reg,off])
        if base==29 and op==9:
            if reg==29:adjustments.append([i*4,off])
            else:addresses.add(off)
    require(adjustments==[[0x2c,-88],[0xb2c,88]],'native frame census changed')
    require(not addresses,'unexpected address-taken storage')
    return dict(frame=88,adjustments=adjustments,address_taken=[],accesses=accesses,
                live_expression_homes=[28,32,36,40,44],argument_home=88,
                uncertainty='Original inactive locals remain unknown; no frame-filler local is retained')


def inputs():
    import re
    with target_context() as image:
        source=(GROUP/'group.c').read_text();spec=json.loads((GROUP/'group.json').read_text())
        require(spec['members']==spec['claims']==list(PRESERVED),'every inherited exact member must be claimed')
        require(spec['keep']==KEEP and spec['context']==CONTEXT and spec['files']==['group.c'],'genuine recipe changed')
        require(spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared','stock flags changed')
        require('residual0' not in source,'diagnostic frame residual leaked into source')
        require('func_800A3424(FileInfo **,s32)' in source,'accepted active-flag prototype changed')
        require('text[32]' in function(source,'func_803A7B58')[2],'inherited capacity changed')
        paths=[GROUP/'group.c',GROUP/'group.json',HERE/'verify.py',HERE/'baseline_controller.c',HERE/'host.c',HERE/'trace.py',
               ROOT/'tests/cloud/test_runtime_a_profile_controller.py',ROOT/'src/blob/func_800A3424.c',
               ROOT/'src/ovl_a/groups/runtime_a_profile_menu_20261006/group.c',
               ROOT/'src/ovl_a/groups/dot_runtime_a_availability_refresh_20261007/group.c',
               ROOT/'src/ovl_a/groups/dot_runtime_a_menu_cleanup_20261007/group.c']
        result=dict(protocol_version=2,files={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths},
                    targets={},helper_targets={},symbols={},calls={},flags=spec['flags'],members=spec['members'],claims=spec['claims'],
                    context=CONTEXT,keep=KEEP,asset_sha256=ASSET_HASH,image_sha256=IMAGE_HASH)
        manifest=score.target_manifest();meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));rows={r['name']:r for r in meta['functions']};targets=score.targets()
        for name in CHECKED:
            address=int(name[-8:],16);sites={}
            for caller,words in sorted(targets.items()):
                offsets=[i*4 for i,w in enumerate(words) if w>>26==3 and (w&0x3ffffff)*4==address&0xfffffff]
                if offsets:sites[caller]=offsets
            result['calls'][name]=sites
        require(result['calls'][NEW]=={'func_8039842C':[0x1c0]},'native controller caller changed')
        names=set(CHECKED)|set(CONTEXT)|{caller for sites in result['calls'].values() for caller in sites}
        for name in sorted(names):
            raw=packed(targets[name]);row=rows[name];address=int(row['address'],16)
            require(row['size']==len(raw) and address==int(name[-8:],16),'native full extent')
            require(image[address-0x8038A400:address-0x8038A400+len(raw)]==raw,'target image identity')
            if name in CHECKED:require(len(raw)==CHECKED[name],'checked target extent')
            result['targets'][name]=dict(address=hex(address),size=len(raw),words_sha256=sha(raw))
        addresses=score.image_symbols();tokens=set(re.findall(r'\b[A-Za-z_]\w*\b',source))
        result['symbols']={n:hex(addresses.get(n,score.address_named(n))) for n in sorted(tokens) if addresses.get(n,score.address_named(n)) is not None}
        old=score.ASM_DIR
        try:
            score.ASM_DIR=ROOT/'asm/us/blob'
            helpers=['func_800A3424','func_800CCEFC','resource_type_select','audio_doppler','audio_distance_atten']
            result['helper_targets']={n:dict(size=len(score.targets()[n])*4,words_sha256=sha(packed(score.targets()[n]))) for n in helpers}
        finally:score.ASM_DIR=old
        result['native_stack']=stack_census(targets[NEW])
        result['native_owned']={n:dict(address=hex(a),size=z,sha256=sha(image[a-0x8038A400:a-0x8038A400+z])) for n,(o,z,a) in OWNED.items()}
        result['private_input']={'register':'s0','native_first_rs':(targets[NEW][0]>>16)&31,'caller':'func_8039842C','offset':0x1c0}
        require(result['private_input']['native_first_rs']==16,'native argument register changed')
        return result


def controls(source):
    body=function(source,NEW)[2]
    old=function((HERE/'baseline_controller.c').read_text(),NEW)[2]
    old=old.replace('D_8012E6E0','D_8012E6D8.head').replace('D_8012E6E4','D_8012E6D8.tail')
    no_owner=body.replace('    Profile **owner;\n','').replace('owner=D_803BAC80[player];\n            D_8014A118[player].owner=owner;','D_8014A118[player].owner=D_803BAC80[player];')
    sparse=body.replace('        case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:break;\n','')
    return {'old_controller':replace_function(source,NEW,old),'no_owner':replace_function(source,NEW,no_owner),
            'sparse_switch':replace_function(source,NEW,sparse),
            'stale_narrow_prototype':source.replace('func_800A3424(FileInfo **,s32)','func_800A3424(FileInfo **,s8)')}


def selective(obj,work,image):
    from tools.conveyor.pipeline import blob_group,ovl_splice,ovl_layout
    spec=ovl_splice.packet_spec(GROUP);root=work/'installer';folder=root/GROUP_NAME;folder.mkdir(parents=True)
    (folder/'group.json').write_text(json.dumps(spec));(folder/'group.c').write_bytes((GROUP/'group.c').read_bytes())
    objects=work/'objects';objects.mkdir();shutil.copyfile(obj,blob_group.object_path(GROUP_NAME,objects))
    path=work/'image.bin';path.write_bytes(image);doc=ovl_layout.derive('a',image_path=path,write=False)
    bodies=blob_group.group_bodies(GROUP_NAME,doc,extern=score.image_symbols(),obj_dir=objects,root=root)
    require(set(bodies)==set(PRESERVED),'selective inherited-only member set')
    require(all(ovl_splice.mismatch(doc,n,b) is None for n,b in bodies.items()),'selective exact body')
    return {n:len(b) for n,b in sorted(bodies.items())}


def host_proof(work,sanitize=False):
    source=(GROUP/'group.c').read_text();old=function((HERE/'baseline_controller.c').read_text(),NEW)[2]
    old=old.replace('D_8012E6E0','D_8012E6D8.head').replace('D_8012E6E4','D_8012E6D8.tail').replace('void '+NEW+'(', 'void baseline_controller(')
    generated=work/'controller_host.c';generated.write_text('#define memset packet_memset\n'+source+'\n'+old+'\n#undef memset\n'+(HERE/'host.c').read_text())
    flags=['-std=c99','-O1','-ffunction-sections','-fdata-sections','-Wno-pointer-to-int-cast','-Wno-int-to-pointer-cast']
    if sanitize:flags+=['-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-pie','-no-pie']
    executable=work/'host';subprocess.run(['cc',*flags,str(generated),'-Wl,--gc-sections','-lm','-o',str(executable)],check=True,capture_output=True)
    import os
    env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1')
    value=json.loads(subprocess.check_output([str(executable)],env=env,text=True));require(value['cases']>=10000,'host matrix incomplete');return value


def replay(deep=False):
    binding=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock compiler required')
    with target_context() as image,tempfile.TemporaryDirectory(prefix='profile-controller-proof-') as tmp:
        work=Path(tmp);obj=work/'group.o';score.compile_group(GROUP,obj);sections,symbols,relocs=elf(obj);ti,_,text=sections['.text'];checked={};census=emitted_census(obj)
        for name,size in CHECKED.items():
            value=score.compare(obj,name,show=0);start,extent,kind,index=symbols[name]
            require((extent,kind,index)==(size,2,ti),'complete native extent')
            require(value.differing==(31 if name==NEW else 0) and not(value.extra_words or value.unresolved or value.unverified or value.errors),'strict residual changed: '+name)
            checked[name]=dict(comparison=asdict(value),extent=extent,notes=list(value.notes))
        contexts={n:asdict(score.compare(obj,n,show=0)) for n in CONTEXT};require(all(q['differing'] for q in contexts.values()),'unclaimed exact context')
        causal={}
        for label,source in controls((GROUP/'group.c').read_text()).items():
            folder=work/label;folder.mkdir();(folder/'group.c').write_text(source);(folder/'group.json').write_bytes((GROUP/'group.json').read_bytes());control=folder/'group.o';score.compile_group(folder,control)
            q=score.compare(control,NEW,show=0);require(q.differing>31 and not q.unresolved,'causal control not worse')
            for name in PRESERVED:require(score.compare(control,name,show=0).accepted(),'preserved control regression '+name)
            causal[label]=dict(comparison=asdict(q),extent=elf(control)[1][NEW][1])
        key=owndata.artifact_dir(score.ASM_DIR);prior=score._own_data[key];negative={}
        try:
            for name,(offset,size,address) in OWNED.items():
                score._own_data[key]=None;require(score.compare(obj,name,show=0).unverified,'missing owned data accepted')
                corrupted=bytearray(image);corrupted[address-0x8038A400]^=1;score._own_data[key]=owndata.ImageData.from_image(bytes(corrupted),0x8038A400)
                require(score.compare(obj,name,show=0).errors,'corrupt owned data accepted');negative[name]=dict(missing_rejected=True,corruption_rejected=True)
        finally:score._own_data[key]=prior
        result=dict(inputs=binding,compiler='stock IDO5.3; no static ROM TU rebuilt',compiler_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
                    checked=checked,contexts=contexts,emitted_census=census,controls=causal,owned_negative_controls=negative,new_exact_code_bytes=0,preserved_exact_code_bytes=sum(PRESERVED.values()),new_landed_image_bytes=0,new_rom_verified_bytes=0)
        if deep:
            result['gnu']=gnu_verify(obj,work,image);result['host']=host_proof(work);result['selective_installer']=selective(obj,work,image)
            result['stock_trace']=json.loads(subprocess.check_output([sys.executable,str(HERE/'trace.py')],text=True))
        return result


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text());require(inputs()==expected['inputs'],'input binding changed')
    actual=replay(deep);wanted=dict(expected)
    if not deep:
        for n in ['gnu','stock_trace','host','selective_installer']:wanted.pop(n,None)
    require(actual==wanted,'receipt changed');return actual

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true');parser.add_argument('--sanitize',action='store_true');parser.add_argument('--inputs-only',action='store_true');args=parser.parse_args()
    if args.score:
        with target_context():sys.argv=[str(ROOT/'tools/cloud/score.py'),'group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')];raise SystemExit(score.main())
    elif args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    elif args.sanitize:
        with tempfile.TemporaryDirectory(prefix='profile-controller-host-') as tmp:print(json.dumps(host_proof(Path(tmp),True)))
    elif args.inputs_only:require(inputs()==json.loads((HERE/'verification.json').read_text())['inputs'],'binding changed');print('PASS inputs')
    else:verify(args.deep);print('PASS 96FA0 research 31/718; 7152 prior exact bytes preserved; zero new coverage')
