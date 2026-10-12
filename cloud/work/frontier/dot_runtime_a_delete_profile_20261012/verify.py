#!/usr/bin/env python3
"""Input-bound image-A profile-owner proof; transient native and ELF data never published."""
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
NEW = 'func_803A9088'
PRESERVED = {'func_80395E24':104, 'func_803A87D4':920, 'func_803A8B74':1300}
CHECKED = {NEW:3668, **PRESERVED}
CONTEXT = ['func_80396EE8','func_80396F44','func_803A7B58',NEW,'func_803A9EEC']
KEEP = ['func_803A9EEC']
BASELINE = ROOT / 'src/ovl_a/groups' / GROUP_NAME / 'group.c'
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
RODATA = 0x803B986C

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


def controls(source):
    a,b,body=function(source,NEW)
    pointer=body.replace('    Profile **node;', '    Slot *slot;\n    Profile **node;')
    pointer=pointer.replace('for(i=0;i<14;i++)', 'for(i=0,slot=D_803B470C[player];i<14;i++,slot++)')
    pointer=pointer.replace('D_803B470C[player][i].','slot->')
    source_order=source.replace('func_80395E24(s32 skip,Profile **owner)','func_80395E24(Profile **owner,s32 skip)')
    source_order=source_order.replace('func_80395E24(s32,Profile **)','func_80395E24(Profile **,s32)')
    source_order=source_order.replace('func_80395E24(player,node)','func_80395E24(node,player)')
    return {'baseline':BASELINE.read_text(), 'slot_pointer':source[:a]+pointer+source[b:],
            'shared_constant':source.replace('delta=65.f','delta=65.0f'),
            'wide_selected':source.replace('    s8 selected;','    s32 selected;'),
            'old_parameter_order':source_order}


def gnu_verify(obj,work,image,baseline=False):
    """Re-emit original member REL records; resolve all words with GNU as/ld."""
    sections,symbols,relocs=elf(obj);ti,_,raw=sections['.text'];ri,_,data=sections['.rodata']
    require(set(n for n,s in sections.items() if s[1][2]&2 and s[1][5])=={'.text','.rodata','.reginfo'},'unexpected allocated section')
    require(len(data)>=36,'missing owned data')
    lines=['.set noreorder','.set noat'];external=set();counts={};extents={}
    def owner_at(addend):
        rows=[(n,s) for n,s in symbols.items() if s[2]==2 and s[3]==ti and s[0]<=addend<s[0]+s[1]]
        require(len(rows)==1,'unowned text anchor');return rows[0]
    for name,size in CHECKED.items():
        start,actual,kind,section=symbols[name]
        require((actual,kind,section)==((3628 if baseline and name==NEW else size),2,ti),'complete input extent')
        following=min((s[0] for s in symbols.values() if s[2]==2 and s[3]==ti and s[0]>start),default=len(raw))
        require(following>=start+actual and not any(raw[start+actual:following]),'nonzero suffix')
        words=list(struct.unpack('>%dI'%(actual//4),raw[start:start+actual]));entries=[];extents[name]=actual
        for loc,typ,symbol,dest in relocs:
            if dest!=ti or not start<=loc<start+actual:continue
            require(typ in (4,5,6) and loc%4==0,'unsupported text relocation')
            off=loc-start
            if symbol=='.text':
                require(typ==4,'unsupported text anchor');addend=(words[off//4]&0x3ffffff)*4
                symbol,owner=owner_at(addend);words[off//4]=(words[off//4]&0xfc000000)|((addend-owner[0])//4)
            if symbol=='.rodata':
                require(name==NEW,'preserved member local data changed');symbol='packet_owned_rodata'
            require(symbol and not symbol.startswith('.'),'unowned local data')
            entries.append((off,typ,symbol));external.add(symbol)
        lines+=['.section .review_'+name+',"ax",@progbits','.balign 4','.globl '+name,'.type '+name+',@function',name+':']
        lines+=['.word 0x%08x'%w for w in words]+['.size %s,.-%s'%(name,name)]
        lines+=['.reloc %s+%d,%s,%s'%(name,o,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[t],s) for o,t,s in entries]
        counts[name]=len(entries)
    words=list(struct.unpack('>9I',data[:36]));entries=[]
    for loc,typ,symbol,dest in relocs:
        if dest!=ri or not 0<=loc<36:continue
        require(typ==2 and symbol=='.text' and loc%4==0,'owned table relocation')
        name,owner=owner_at(words[loc//4]);require(name==NEW,'table owner changed')
        words[loc//4]-=owner[0];entries.append(loc)
    require(entries==list(range(12,36,4)),'six complete table entries')
    lines+=['.section .review_owned,"a",@progbits','.globl packet_owned_rodata','packet_owned_rodata:']
    lines+=['.word 0x%08x'%w for w in words]
    lines+=['.reloc packet_owned_rodata+%d,R_MIPS_32,%s'%(o,NEW) for o in entries]
    addresses=score.image_symbols();bindings={n:addresses.get(n,score.address_named(n)) for n in external-set(CHECKED)-{'packet_owned_rodata'}}
    require(all(a is not None for a in bindings.values()),'unknown external symbol')
    asm,reloc,script,linked=[work/n for n in ('review.s','review.o','review.ld','review.elf')]
    asm.write_text('\n'.join(lines)+'\n')
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in CHECKED)
        +'.review_owned 0x803B986C : SUBALIGN(4) { *(.review_owned) }\n'
        +'/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'
        +''.join('%s = 0x%X;\n'%(n,a) for n,a in sorted(bindings.items())))
    for cmd in (['mips-linux-gnu-as','-EB','-mips2','-o',str(reloc),str(asm)],['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(reloc)]):
        require(shutil.which(cmd[0]),'GNU tool absent: '+cmd[0]);subprocess.run(cmd,check=True,capture_output=True)
    ss,sy,rr=elf(linked);require(not rr,'remaining GNU relocations');require(not any(s[3]==0 for s in sy.values()),'undefined GNU symbol');result={}
    for name,size in CHECKED.items():
        i,row,body=ss['.review_'+name];address=int(name[-8:],16)
        require(row[3]==address and len(body)==extents[name] and sy[name]==(address,len(body),2,i),'GNU complete extent')
        native=packed(score.targets()[name]);offsets=[o for o in range(0,size,4) if body[o:o+4]!=native[o:o+4]]
        require(len(offsets)==0 if name!=NEW else (len(offsets)>=562 if baseline else len(offsets)==15),'GNU full-word residual changed')
        result[name]=dict(bytes=len(body),native_bytes=size,relocations=counts[name],differing_offsets=offsets,linked_sha256=sha(body))
    i,row,body=ss['.review_owned'];native=image[RODATA-0x8038A400:RODATA-0x8038A400+36]
    require(row[3]==RODATA and row[5]==len(body)==36 and sy['packet_owned_rodata']==(RODATA,0,0,i),'owned extent')
    if not baseline:require(body==native,'owned data mismatch')
    return dict(functions=result,owned_bytes=36,owned_start=hex(RODATA),owned_exact=body==native,owned_sha256=sha(body),table_relocations=6,external_symbols={n:hex(a) for n,a in sorted(bindings.items())})


def inputs():
    import re
    with target_context() as image:
        source=(GROUP/'group.c').read_text();spec=json.loads((GROUP/'group.json').read_text());base=BASELINE.read_text()
        require(spec['members']==spec['claims']==list(PRESERVED),'preserved members/claims changed')
        require(spec['context']==CONTEXT and spec['keep']==KEEP and spec['files']==['group.c'],'genuine group context changed')
        require(spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared','stock flags changed')
        restored=controls(source)['old_parameter_order']
        restored=replace_function(restored,NEW,function(base,NEW)[2])
        restored=restored.replace('#define PROFILE_DISTANCE(x) ((x)>=0 ? (x) : -(x))\n','')
        require(restored[restored.index('typedef signed char'):]==base[base.index('typedef signed char'):],'unrelated source changed')
        require('u8 *label,text[128];' in source and 's16 point[2]' in source,'live capacities changed')
        manifest=score.target_manifest();meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));rows={r['name']:r for r in meta['functions']};targets=score.targets()
        paths=[GROUP/'group.c',GROUP/'group.json',HERE/'verify.py',HERE/'host.c',HERE/'trace.py',ROOT/'tests/cloud/test_runtime_a_delete_profile.py',BASELINE]
        result=dict(protocol_version=2,files={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths},flags=spec['flags'],members=list(PRESERVED),claims=list(PRESERVED),context=CONTEXT,keep=KEEP,targets={},symbols={},calls={},asset_sha256=ASSET_HASH,image_sha256=IMAGE_HASH)
        for name in sorted(set(CHECKED)|set(CONTEXT)):
            raw=packed(targets[name]);row=rows[name];address=int(row['address'],16)
            require(row['size']==len(raw) and address==int(name[-8:],16),'full native row')
            require(image[address-0x8038A400:address-0x8038A400+len(raw)]==raw,'target/image mismatch')
            require(name not in CHECKED or len(raw)==CHECKED[name],'checked native extent')
            result['targets'][name]=dict(address=hex(address),size=len(raw),words_sha256=sha(raw))
        names=set(re.findall(r'\b[A-Za-z_]\w*\b',source));addresses=score.image_symbols()
        result['symbols']={n:hex(addresses.get(n,score.address_named(n))) for n in sorted(names) if addresses.get(n,score.address_named(n)) is not None}
        for name in CHECKED:
            address=int(name[-8:],16);sites={}
            for caller,words in sorted(targets.items()):
                offsets=[i*4 for i,w in enumerate(words) if w>>26==3 and (w&0x3ffffff)*4==(address&0xfffffff)]
                if offsets:sites[caller]=offsets
            result['calls'][name]=sites
        require(set(result['calls'][NEW])=={'func_803A9EEC'},'genuine callback census changed')
        return result


def host_check(work,sanitize=False):
    require(shutil.which('cc'),'host compiler missing')
    (work/'group.c').write_bytes((GROUP/'group.c').read_bytes());(work/'host.c').write_bytes((HERE/'host.c').read_bytes())
    base=BASELINE.read_text();body=function(base,'func_80395E24')[2]+'\n'+function(base,NEW)[2]
    body=body.replace('func_80395E24','baseline_80395E24').replace(NEW,'baseline_803A9088');(work/'baseline.h').write_text(body)
    cmd=['cc','-std=c99','-O1' if sanitize else '-O2','-ffunction-sections','-fdata-sections',str(work/'host.c'),'-Wl,--gc-sections','-o',str(work/'host')]
    if sanitize:cmd+=['-g','-fsanitize=address,undefined','-fno-omit-frame-pointer']
    subprocess.run(cmd,check=True,capture_output=True)
    import os
    env=os.environ.copy()
    if sanitize:env['ASAN_OPTIONS']='detect_leaks=0'
    value=subprocess.check_output([str(work/'host')],text=True,env=env).strip()
    require(value=='2880 actual-C renderer cases; exact stub-event equivalence','host result changed: '+value)
    return dict(cases=2880,baseline='current accepted group source; target remains NONMATCH context',comparison='exact complete event sequences',external_helpers='deterministic stubs',sanitizers=sanitize,gameplay_executed=False)


def replay(deep=False):
    binding=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO 5.3 required')
    with target_context() as image,tempfile.TemporaryDirectory(prefix='delete-profile-proof-') as tmp:
        work=Path(tmp);obj=work/'group.o';score.compile_group(GROUP,obj);sections,symbols,relocs=elf(obj);ti,_,text=sections['.text'];checked={}
        for name,size in CHECKED.items():
            value=score.compare(obj,name,show=0);start,extent,kind,index=symbols[name]
            require((extent,kind,index)==(size,2,ti),'complete ELF extent')
            following=min((s[0] for s in symbols.values() if s[2]==2 and s[3]==ti and s[0]>start),default=len(text));require(following>=start+size and not any(text[start+size:following]),'nonzero suffix')
            require(not value.unresolved and not value.unverified and not value.errors and not value.extra_words,'verification uncertainty')
            require(value.differing==(15 if name==NEW else 0),'word residual changed')
            checked[name]=dict(comparison=asdict(value),extent=extent,owned_notes=list(value.notes))
        contexts={n:asdict(score.compare(obj,n,show=0)) for n in CONTEXT if n!=NEW}
        require(all(c['differing']>0 for c in contexts.values()),'new exact context must be claimed')
        own_key=owndata.artifact_dir(score.ASM_DIR);original_image=score._own_data[own_key]
        negative={}
        try:
            broken=bytearray(image);broken[RODATA-0x8038A400]^=1
            for label,value in [('missing',owndata.ImageData([])),('corrupt',owndata.ImageData.from_image(bytes(broken),0x8038A400))]:
                score._own_data[own_key]=value;check=score.compare(obj,NEW,show=0)
                require(bool(check.unverified or check.errors),'owned-data negative control was accepted')
                negative[label+'_rejected']=True
        finally:score._own_data[own_key]=original_image
        causal={};baseline_gnu=None
        for label,source in controls((GROUP/'group.c').read_text()).items():
            folder=work/label;folder.mkdir();(folder/'group.c').write_text(source);(folder/'group.json').write_bytes((GROUP/'group.json').read_bytes());control=folder/'group.o';score.compile_group(folder,control)
            value=score.compare(control,NEW,show=0)
            require(value.differing>0,'unexpected exact control')
            for name in PRESERVED:require(score.compare(control,name,show=0).accepted(),'preserved regression '+name)
            causal[label]=dict(comparison=asdict(value),extent=elf(control)[1][NEW][1])
            if deep and label=='baseline':baseline_gnu=gnu_verify(control,folder,image,True)
        result=dict(inputs=binding,compiler_by_translation_unit={'group.c':'stock IDO 5.3'},compiler_binary_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},checked=checked,contexts=contexts,controls=causal,owned_negative_controls=negative,new_exact_code_bytes=0,preserved_exact_code_bytes=2324,new_landed_image_bytes=0,new_rom_verified_bytes=0)
        if deep:result.update(gnu=gnu_verify(obj,work,image),baseline_gnu=baseline_gnu,host=host_check(work))
        return result


def verify(deep=False):
    wanted=json.loads((HERE/'verification.json').read_text());require(inputs()==wanted['inputs'],'input binding changed');actual=replay(deep)
    if not deep:
        for key in ('gnu','baseline_gnu','host'):wanted.pop(key,None)
    require(actual==wanted,'receipt differs');return actual


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true');parser.add_argument('--sanitize',action='store_true');args=parser.parse_args()
    if args.score:
        with target_context():
            sys.argv=[str(ROOT/'tools/cloud/score.py'),'group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')];raise SystemExit(score.main())
    elif args.sanitize:
        with tempfile.TemporaryDirectory(prefix='delete-profile-sanitize-') as tmp:print(json.dumps(host_check(Path(tmp),True),sort_keys=True))
    elif args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    else:verify(args.deep);print('PASS: A9088 NONMATCH 15/917; zero new exact bytes; 2324 preserved')
