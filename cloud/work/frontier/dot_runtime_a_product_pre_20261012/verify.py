#!/usr/bin/env python3
"""Strict input-bound D300 product proof; native/object bytes stay transient."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score,owndata
from tools.conveyor import compilers
GROUP=HERE/'groups/dot_cleanup_colortrace_20261011'
BASELINE=ROOT/'cloud/work/frontier/dot_cleanup_colortrace_20261011/group/group.c'
TEST=ROOT/'tests/cloud/test_runtime_a_product_pre.py'
NEW='func_8039D300'
MEMBERS={'func_8039D300':404,'func_8039D05C':676,'func_8039D494':528,'func_8039ED20':480,'func_8039E200':444,'func_8039E3BC':136}
CONTEXT=['func_803A0498','func_803A0FD8','func_803A1EAC','func_8039FF04','func_8039D6A4','func_8039EF08']
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
IMAGE_HASH='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
def require(ok,message):
    if not ok:raise ValueError(message)
def sha(data):return hashlib.sha256(data).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)
def image_data():
    asset=(ROOT/'assets/us/data.bin').read_bytes();require(sha(asset)==ASSET_HASH,'asset identity changed')
    require(struct.unpack_from('>I',asset,0x3850)[0]==0xB5C534,'loader pointer changed')
    image=zlib.decompress(asset[0xB5C534-0x283D0:],-15)
    require(len(image)==194128 and sha(image)==IMAGE_HASH,'image identity changed');return image
@contextmanager
def target_context():
    old=score.ASM_DIR;score.ASM_DIR=ROOT/'asm/us/ovl_a';key=owndata.artifact_dir(score.ASM_DIR)
    present=key in score._own_data;prior=score._own_data.get(key);image=image_data()
    score._own_data[key]=owndata.ImageData.from_image(image,0x8038A400)
    try:yield image
    finally:
        score.ASM_DIR=old
        if present:score._own_data[key]=prior
        else:score._own_data.pop(key,None)

def elf(path):
    """Independent ELF32 BE reader for GNU and complete symbol/relocation checks."""
    data = path.read_bytes()
    require(data[:7] == b'\x7fELF\x01\x02\x01' and struct.unpack_from('>H', data, 18)[0] == 8, 'not BE ELF32 MIPS')
    require(struct.unpack_from('>H',data,16)[0] in (1,2) and struct.unpack_from('>I',data,36)[0] in (0x10000000,0x10000001,0x10001000,0x10001001),'unexpected ELF type or MIPS2 ABI')
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



def gnu_verify(obj, work, expected, sizes=None):
    sizes = MEMBERS if sizes is None else sizes
    """Original REL entries, complete member symbols, independent GNU linking."""
    sections,symbols,relocs=elf(obj);ti,_,text=sections['.text']
    lines=['.set noreorder','.set noat'];externs=set();counts={}
    for name,size in sizes.items():
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
    require(not any(v[3]==0 for n,v in sy.items() if n),'GNU undefined symbol remains')
    require(all(sy[n][0]==a and sy[n][3]==0xfff1 for n,a in bindings.items()),'GNU external binding changed')
    require(all(not row[2]&2 or not row[5] or n in {'.review_'+m for m in MEMBERS} for n,(i,row,raw) in ss.items()),'GNU unexpected allocated section')
    for name,size in sizes.items():
        i,row,body=ss['.review_'+name];address=int(name[-8:],16)
        require(row[3]==address and len(body)==size and sy[name]==(address,size,2,i),'GNU extent mismatch')
        native=packed(score.targets()[name]);offsets=[x for x in range(0,max(size,len(native)),4) if body[x:x+4]!=native[x:x+4]]
        require(len(offsets)==expected[name],'GNU full-word mismatch')
        result[name]=dict(bytes=size,native_bytes=len(native),relocations=counts[name],differing_offsets=offsets,linked_sha256=sha(body))
    return dict(members=result,external_symbols={n:hex(a) for n,a in sorted(bindings.items())},owned_bytes=0)


def function(source, name):
    import re
    match = re.search(r'^[^\n;{}]*\b'+name+r'\([^;]*?\)\s*\{', source, re.M)
    require(match is not None, 'function missing: '+name)
    start=match.start(); opening=source.index('{',start); depth=1; end=opening+1
    while depth:
        require(end<len(source),'unterminated function')
        depth += (source[end]=='{')-(source[end]=='}'); end+=1
    return start,end,source[start:end]


def replace_function(source, name, body):
    a,b,_=function(source,name);return source[:a]+body+source[b:]



def inputs():
    with target_context() as image:
        source=(GROUP/'group.c').read_text();spec=json.loads((GROUP/'group.json').read_text())
        require(spec['members']==spec['claims']==list(MEMBERS),'all exact members must be claims')
        require(spec['context']==CONTEXT and spec['keep']==['func_803A1EAC'] and spec['files']==['group.c'] and spec['flags']==FLAGS,'group recipe changed')
        original=BASELINE.read_text();restored=replace_function(source,NEW,function(original,NEW)[2])
        def tokens(s):return re.findall(r'[A-Za-z_]\w*|(?:\d*\.)?\d+(?:[eE][+-]?\d+)?[A-Za-z]*|\S',re.sub(r'/\*.*?\*/','',s,flags=re.S))
        require(tokens(restored)==tokens(original),'unrelated source or existing member changed')
        for n in MEMBERS:
            if n!=NEW:require(function(source,n)[2]==function(original,n)[2],'old member not verbatim preserved')
        table=score.targets();manifest=score.target_manifest()
        meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));rows={x['name']:x for x in meta['functions']}
        selected={}
        for n in list(MEMBERS)+CONTEXT:
            row=rows[n];raw=packed(table[n]);address=int(row['address'],16)
            require(row['size']==len(raw) and address==int(n[-8:],16),'native extent disagreement')
            require(image[address-0x8038A400:address-0x8038A400+len(raw)]==raw,'native target/image disagreement')
            if n in MEMBERS:require(len(raw)==MEMBERS[n],'native member size changed')
            selected[n]={'extent':row,'words_sha256':sha(raw)}
        calls={}
        for n,words in table.items():
            at=[i*4 for i,v in enumerate(words) if v>>26==3 and (v&0x3ffffff)*4==0x39D300]
            if at:calls[n]=at
        require(calls=={'func_803A0FD8':[0x114],'func_803A1EAC':[0x19d4]},'genuine native caller census changed')
        addresses=score.image_symbols();names=set(re.findall(r'\b[A-Za-z_]\w*\b',source))
        symbols={n:hex(addresses.get(n,score.address_named(n))) for n in sorted(names) if addresses.get(n,score.address_named(n)) is not None}
        paths=[GROUP/'group.c',GROUP/'group.json',HERE/'verify.py',HERE/'host.c',HERE/'trace_check.py',TEST,BASELINE]
        return {'protocol_version':2,'sources':{str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths},'selected_targets':selected,'symbol_bindings':symbols,'native_callers':calls,'asset_sha256':ASSET_HASH,'image_sha256':IMAGE_HASH,'flags':FLAGS,'compiler':'stock IDO5.3; no static ROM TU rebuilt'}


def causal_sources(source):
    a,b,body=function(source,NEW)
    no_blocks=body.replace('        do {\n','').replace('        } while (0);\n','')
    left='D_803B2948[D_80111049[player][D_803B9FD0[player]]][component]'
    right='D_803B28C8[D_80110E85[player][D_803B9FD0[player]]][component]'
    require(left+' * '+right in body,'first-pair control disappeared')
    swap=body.replace(left+' * '+right,right+' * '+left)
    return {'prehoisted_baseline':(replace_function(source,NEW,function(BASELINE.read_text(),NEW)[2]),101),
            'no_store_boundaries':(source[:a]+no_blocks+source[b:],18),
            'opposite_pair_order':(source[:a]+swap+source[b:],4)}


def body_shape(obj,name):
    sections,symbols,relocs=elf(obj);ti,row,text=sections['.text'];start,size,kind,index=symbols[name]
    require(kind==2 and index==ti and size>0 and size%4==0,'invalid complete function')
    following=min((v[0] for v in symbols.values() if v[2]==2 and v[3]==ti and v[0]>start),default=len(text))
    require(following>=start+size and not any(text[start+size:following]),'nonzero function suffix')
    words=struct.unpack('>%dI'%(size//4),text[start:start+size]);stores=[];loads=[];fp=[]
    for at,word in enumerate(words):
        op=word>>26
        if op==57:stores.append(at*4)
        if op==49:loads.append(at*4)
        if op==17 and (word>>21&31)==16 and (word&63)==2:fp.append(at*4)
    return {'function_bytes':size,'float_store_offsets':stores,'float_load_offsets':loads,'float_multiply_offsets':fp}


def host_check(work):
    require(shutil.which('cc'),'host compiler unavailable')
    (work/'group.c').write_bytes((GROUP/'group.c').read_bytes());(work/'host.c').write_bytes((HERE/'host.c').read_bytes())
    cmd=['cc','-std=c89','-O2','-fno-fast-math','-ffp-contract=off','-ffunction-sections','-fdata-sections',str(work/'host.c'),'-Wl,--gc-sections','-o',str(work/'host')]
    subprocess.run(cmd,check=True,capture_output=True)
    result=subprocess.check_output([str(work/'host')],text=True).strip()
    require(result=='12636 finite product cases; unrelated output and inputs unchanged','host behavior changed: '+result)
    return {'cases':12636,'players':[0,3],'selections':[0,12],'vector_row_choices':3,'float_values':[0.0,'negative zero',0.5,1.0,2.0,-1.0,0.25,-4.0,0.1,0.3,-0.7,1.1],'final_float_bits':'equal','input_and_other_output_bytes':'unchanged','native_hardware_executed':False}


def replay(deep=False):
    binding=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','recognized stock IDO5.3 required')
    with target_context(),tempfile.TemporaryDirectory(prefix='d300-product-proof-') as temp:
        work=Path(temp);obj=work/'group.o';score.compile_group(GROUP,obj)
        sections,symbols,relocs=elf(obj);members={}
        for n,size in MEMBERS.items():
            value=score.compare(obj,n,show=0);require(value.accepted(),n+': '+value.summary())
            shape=body_shape(obj,n);require(shape['function_bytes']==size,'member complete extent mismatch')
            require(not value.notes,'member unexpectedly owns data')
            members[n]={'comparison':asdict(value),'shape':shape}
        for n in CONTEXT:require(not score.compare(obj,n,show=0).accepted(),'exact context must become member and claim')
        shape=members[NEW]['shape']
        require(shape['float_store_offsets']==[0x168,0x178,0x188] and len(shape['float_multiply_offsets'])==4,'native three-store/four-multiply sequence changed')
        result={'inputs':binding,'compiler_sha256':{n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},'members':members,'new_exact_code_bytes':404,'preserved_exact_code_bytes':2264,'new_landed_image_bytes':0,'new_rom_verified_bytes':0}
        if deep:
            result['gnu']=gnu_verify(obj,work,{n:0 for n in MEMBERS});controls={}
            for tag,(source,count) in causal_sources((GROUP/'group.c').read_text()).items():
                directory=work/tag;directory.mkdir();(directory/'group.c').write_text(source);(directory/'group.json').write_bytes((GROUP/'group.json').read_bytes());old=directory/'group.o';score.compile_group(directory,old)
                value=score.compare(old,NEW,show=0);require(value.differing==count and not any((value.errors,value.unresolved,value.unverified,value.extra_words)),'causal control changed: '+tag)
                sizes={n:body_shape(old,n)['function_bytes'] for n in MEMBERS}
                for n in MEMBERS:
                    if n!=NEW:require(score.compare(old,n,show=0).accepted(),'old member regressed in control')
                controls[tag]={'comparison':asdict(value),'shape':body_shape(old,NEW),'gnu':gnu_verify(old,directory,{n:count if n==NEW else 0 for n in MEMBERS},sizes)}
            result['controls']=controls;result['host']=host_check(work)
        return result


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text());require(inputs()==expected['inputs'],'input receipt mismatch')
    actual=replay(deep);wanted=dict(expected)
    if not deep:
        for key in ['gnu','controls','host']:wanted.pop(key,None)
    require(actual==wanted,'receipt mismatch');return actual

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--deep',action='store_true');p.add_argument('--record',action='store_true');p.add_argument('--score',action='store_true');args=p.parse_args()
    if args.record:
        (HERE/'verification.json').write_text(json.dumps(replay(True),indent=2)+'\n');print('Recorded full native/GNU/source proof')
    elif args.score:
        with target_context():
            sys.argv=['score.py','group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')];raise SystemExit(score.main())
    else:
        verify(args.deep);print('PASS: D300 404 new bytes; five exact old members 2264 bytes preserved; zero ROM claim')
