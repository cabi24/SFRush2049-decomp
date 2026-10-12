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
NEW = 'func_80395E24'
MEMBERS = {NEW:104, 'func_803A87D4':920, 'func_803A8B74':1300}
CONTEXT = ['func_80396EE8','func_80396F44','func_803A7B58','func_803A9088','func_803A9EEC']
KEEP = ['func_803A9EEC']
BASELINE = ROOT / 'cloud/work/lean/runtime_a_profile_menu_20261006/group.c'
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'

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
    require(data[:6] == b'\x7fELF\x01\x02' and struct.unpack_from('>H', data, 18)[0] == 8, 'not BE ELF32 MIPS')
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



def gnu_verify(obj, work, expected):
    """Original REL entries, complete member symbols, independent GNU linking."""
    sections,symbols,relocs=elf(obj);ti,_,text=sections['.text']
    lines=['.set noreorder','.set noat'];externs=set();counts={}
    for name,size in MEMBERS.items():
        start,actual,kind,section=symbols[name]
        require((actual,kind,section)==(size,2,ti),'complete input function extent')
        following=min((v[0] for v in symbols.values() if v[2]==2 and v[3]==ti and v[0]>start),default=len(text))
        require(following>=start+size and not any(text[start+size:following]),'nonzero suffix')
        words=list(struct.unpack('>%dI'%(size//4),text[start:start+size]));entries=[]
        for loc,typ,symbol,dest in relocs:
            if dest!=ti or not start<=loc<start+size:continue
            require(typ in (4,5,6) and loc%4==0,'unsupported relocation')
            off=loc-start
            if symbol=='.text':
                require(typ==4,'unexpected text anchor');addend=(words[off//4]&0x3ffffff)*4
                owners=[(n,v) for n,v in symbols.items() if v[2]==2 and v[3]==ti and v[0]<=addend<v[0]+v[1]]
                require(len(owners)==1,'unowned text anchor');symbol,owner=owners[0]
                words[off//4]=(words[off//4]&0xfc000000)|((addend-owner[0])//4)
            require(symbol and not symbol.startswith('.'),'member owns unexpected local data')
            entries.append((off,typ,symbol));externs.add(symbol)
        counts[name]=len(entries)
        lines+=['.section .review_'+name+',"ax",@progbits','.balign 4','.globl '+name,
                '.type '+name+',@function',name+':']
        lines+=['.word 0x%08x'%w for w in words]+['.size %s,.-%s'%(name,name)]
        lines+=['.reloc %s+%d,%s,%s'%(name,o,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[t],s) for o,t,s in entries]
    addresses=score.image_symbols();bindings={n:addresses.get(n,score.address_named(n)) for n in externs-set(MEMBERS)}
    require(all(a is not None for a in bindings.values()),'unknown GNU symbol')
    asm,reloc,script,linked=[work/n for n in ('review.s','review.o','review.ld','review.elf')]
    asm.write_text('\n'.join(lines)+'\n')
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in MEMBERS)
        +'/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'
        +''.join('%s = 0x%X;\n'%(n,a) for n,a in sorted(bindings.items())))
    for cmd in (['mips-linux-gnu-as','-EB','-mips2','-o',str(reloc),str(asm)],['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(reloc)]):
        require(shutil.which(cmd[0]),'GNU MIPS binutils missing: '+cmd[0]);subprocess.run(cmd,check=True,capture_output=True)
    ss,sy,rr=elf(linked);require(not rr,'GNU unresolved relocations');result={}
    for name,size in MEMBERS.items():
        i,row,body=ss['.review_'+name];address=int(name[-8:],16)
        require(row[3]==address and len(body)==size and sy[name]==(address,size,2,i),'GNU extent mismatch')
        native=packed(score.targets()[name]);offsets=[x for x in range(0,size,4) if body[x:x+4]!=native[x:x+4]]
        require(len(offsets)==expected[name],'GNU full-word mismatch')
        result[name]=dict(bytes=size,relocations=counts[name],differing_offsets=offsets,linked_sha256=sha(body))
    return dict(members=result,external_symbols={n:hex(a) for n,a in sorted(bindings.items())},owned_bytes=0)


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
    _,_,prior=function(BASELINE.read_text(),NEW)
    eager=replace_function(source,NEW,prior)
    snapshot=source.replace('s32 found=0;\n    if(owner)for(i=0;i<D_8014A108;i++)if(i!=skip && owner==D_8014A118[i].owner)found=1;',
        's32 count,found=0;\n    if(owner) {count=D_8014A108;for(i=0;i<count;i++)if(i!=skip && owner==D_8014A118[i].owner)found=1;}')
    require(snapshot!=source,'guarded snapshot control disappeared')
    return {'eager_count':eager, 'guarded_snapshot':snapshot}


def inputs():
    import re
    with target_context() as image:
        source=(GROUP/'group.c').read_text();spec=json.loads((GROUP/'group.json').read_text())
        require(spec['members']==spec['claims']==list(MEMBERS),'verified member/claim set')
        require(spec['context']==CONTEXT and spec['keep']==KEEP and spec['files']==['group.c'],'group context changed')
        require(spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared','compiler flags changed')
        original=BASELINE.read_text();restored=source
        for name in (NEW,'func_803A8B74'):
            restored=replace_function(restored,name,function(original,name)[2])
        restored=restored.replace('extern s32 object_create(s32);','extern void *object_create(s32);')
        def tokens(text):
            return re.findall(r'[A-Za-z_]\w*|(?:\d*\.)?\d+(?:[eE][+-]?\d+)?[A-Za-z]*|\S',re.sub(r'/\*.*?\*/','',text,flags=re.S))
        require(tokens(restored)==tokens(original),'unrelated source changed')
        manifest=score.target_manifest();meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));rows={r['name']:r for r in meta['functions']};targets=score.targets()
        require((meta['image'],meta['base'],meta['rom_offset'])==('A','0x8038A400','0xB5C534'),'population identity')
        paths=[GROUP/'group.c',GROUP/'group.json',HERE/'verify.py',HERE/'host.c',BASELINE]
        result=dict(protocol_version=2,sources={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths},targets={},symbols={},call_sites={},members=list(MEMBERS),claims=list(MEMBERS),context=CONTEXT,keep=KEEP,flags=spec['flags'],asset_sha256=ASSET_HASH,image_sha256=IMAGE_HASH)
        for name in list(MEMBERS)+CONTEXT+['func_80395E8C','func_80396FA0']:
            raw=packed(targets[name]);row=rows[name];address=int(row['address'],16)
            require(row['size']==len(raw) and address==int(name[-8:],16),'native full extent')
            require(image[address-0x8038A400:address-0x8038A400+len(raw)]==raw,'native image identity')
            require(name not in MEMBERS or len(raw)==MEMBERS[name],'member extent')
            result['targets'][name]=dict(address=row['address'],size=len(raw),words_sha256=sha(raw))
        addresses=score.image_symbols();names=set(re.findall(r'\b[A-Za-z_]\w*\b',source))
        result['symbols']={n:hex(addresses.get(n,score.address_named(n))) for n in sorted(names) if addresses.get(n,score.address_named(n)) is not None}
        expected={NEW:{'func_80395E8C':[144,476,2152,2572],'func_80396FA0':[84,464,2324,2784],'func_803A7B58':[2548],'func_803A9088':[3496]},'func_803A87D4':{'func_803A9EEC':[184]},'func_803A8B74':{'func_803A9EEC':[272]}}
        for name in MEMBERS:
            address=int(name[-8:],16);sites={}
            for caller,words in sorted(targets.items()):
                offsets=[i*4 for i,w in enumerate(words) if w>>26==3 and (w&0x3ffffff)*4==(address&0xfffffff)]
                if offsets:sites[caller]=offsets
            require(sites==expected[name],'genuine caller census changed');result['call_sites'][name]=sites
        # The helper uses only two native input registers, a0 and a2; it is
        # a leaf with no frame or calls. Its skip is consumed by beql v0,a2.
        native=targets[NEW]
        require(native[0]>>26==4 and ((native[0]>>21)&31)==4 and ((native[0]>>16)&31)==0,'null-owner guard changed')
        require(native[9]>>26==20 and ((native[9]>>21)&31)==2 and ((native[9]>>16)&31)==6,'private skip register changed')
        return result


def host_check(work):
    require(shutil.which('cc'),'host C compiler missing')
    (work/'group.c').write_bytes((GROUP/'group.c').read_bytes())
    (work/'host.c').write_bytes((HERE/'host.c').read_bytes())
    cmd=['cc','-std=c89','-O2','-ffunction-sections','-fdata-sections',str(work/'host.c'),'-Wl,--gc-sections','-o',str(work/'host')]
    subprocess.run(cmd,check=True,capture_output=True)
    value=subprocess.check_output([str(work/'host')],text=True).strip()
    require(value=='175000 checked owner selections; no writes','host behavior changed: '+value)
    return dict(calls=175000,count_range=[-2,4],skip_range=[-2,5],owner_choices=5,slot_assignments=625,writes=0,native_gameplay_executed=False)


def replay(deep=False):
    binding=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO 5.3 required')
    with target_context(),tempfile.TemporaryDirectory(prefix='profile-owner-packet-') as tmp:
        work=Path(tmp);obj=work/'group.o';score.compile_group(GROUP,obj);sections,symbols,relocs=elf(obj);ti,_,text=sections['.text'];members={}
        for name,size in MEMBERS.items():
            value=score.compare(obj,name,show=0);require(value.accepted(),name+': '+value.summary());start,actual,kind,index=symbols[name]
            require((actual,kind,index)==(size,2,ti),'complete ELF function extent')
            following=min((v[0] for v in symbols.values() if v[2]==2 and v[3]==ti and v[0]>start),default=len(text));require(following>=start+size and not any(text[start+size:following]),'nonzero suffix')
            require(not value.notes,'unexpected member owned data');members[name]=dict(comparison=asdict(value),complete_extent=size,owned_bytes=0)
        contexts={n:asdict(score.compare(obj,n,show=0)) for n in CONTEXT};require(all(v['differing']>0 for v in contexts.values()),'context became exact; update members/claims')
        causal={};baseline_gnu=None
        for label,source in controls((GROUP/'group.c').read_text()).items():
            folder=work/label;folder.mkdir();(folder/'group.c').write_text(source);(folder/'group.json').write_bytes((GROUP/'group.json').read_bytes());control=folder/'group.o';score.compile_group(folder,control)
            value=score.compare(control,NEW,show=0);require(value.differing=={'eager_count':17,'guarded_snapshot':12}[label] and not value.extra_words and not value.errors and not value.unresolved and not value.unverified,'causal control changed')
            for name in MEMBERS:
                if name!=NEW:require(score.compare(control,name,show=0).accepted(),'preserved member regression')
            causal[label]=asdict(value)
            if deep and label=='eager_count':baseline_gnu=gnu_verify(control,folder,{NEW:17,'func_803A87D4':0,'func_803A8B74':0})
        result=dict(inputs=binding,compiler='stock IDO 5.3; no static ROM TU rebuilt',compiler_binary_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},members=members,contexts=contexts,controls=causal,new_exact_code_bytes=104,preserved_exact_code_bytes=2220,new_landed_image_bytes=0,new_rom_verified_bytes=0)
        if deep:
            result['gnu']=gnu_verify(obj,work,{n:0 for n in MEMBERS});result['baseline_gnu']=baseline_gnu
            result['host']=host_check(work)
        return result


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text());require(inputs()==expected['inputs'],'input binding changed');actual=replay(deep);wanted=dict(expected)
    if not deep:
        for key in ('gnu','baseline_gnu','host'):wanted.pop(key,None)
    require(actual==wanted,'receipt differs');return actual

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true');args=parser.parse_args()
    if args.score:
        with target_context():
            sys.argv=[str(ROOT/'tools/cloud/score.py'),'group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')];raise SystemExit(score.main())
    elif args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    else:
        verify(args.deep);print('PASS: 104 new image-A candidate bytes; 2220 preserved; zero accepted-image/ROM credit')
