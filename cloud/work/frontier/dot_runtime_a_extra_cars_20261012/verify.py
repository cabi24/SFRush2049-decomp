#!/usr/bin/env python3
"""Input-bound image-A statistics proof; transient native and ELF data never published."""
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

GROUP_NAME = 'runtime_a_stats_menu_20261006'
GROUP = HERE / 'groups' / GROUP_NAME
NEW = ['func_803AB5FC']
NUMERIC = ['func_803AC54C', 'func_803AC7F4', 'func_803ACD5C']
MEMBERS = {'func_803AAF0C':1776, 'func_803AB5FC':588, 'func_803AC330':540,
           'func_803AC54C':680, 'func_803AC7F4':664,
           'func_803ACD5C':512, 'func_803ACF5C':376}
CONTEXT = ['func_803AA3D8', 'func_803AB848', 'func_803ABEC8',
           'func_803ACA94', 'func_803AD0D4', 'func_803AD57C']
KEEP = ['func_803AD57C']
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
RODATA = 0x803B98E8

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



def gnu_verify(obj, work, image, baseline=False):
    """Independent original-REL GNU link, including both entire jump tables."""
    sections, symbols, relocs = elf(obj)
    ti, _, raw = sections['.text']
    ri, _, rodata = sections['.rodata']
    require(set(n for n,s in sections.items() if s[1][2]&2 and s[1][5]) ==
            {'.text','.rodata','.reginfo'}, 'unexpected allocated storage')
    lines = ['.set noreorder', '.set noat']
    externs, counts = set(), {}
    def owner_at(addend):
        owners = [(n,v) for n,v in symbols.items() if v[2]==2 and v[3]==ti
                  and v[0] <= addend < v[0]+v[1]]
        require(len(owners)==1, 'unowned text addend')
        return owners[0]
    for name,size in MEMBERS.items():
        start,actual,kind,section=symbols[name]
        require((kind,section)==(2,ti) and (actual==size or baseline and name in NEW and actual==560),'complete function extent')
        size=actual
        following=min((v[0] for v in symbols.values() if v[2]==2 and v[3]==ti and v[0]>start),default=len(raw))
        require(following>=start+size and not any(raw[start+size:following]),'nonzero function suffix')
        words=list(struct.unpack('>%dI'%(size//4),raw[start:start+size])); entries=[]
        for loc,typ,symbol,dest in relocs:
            if dest!=ti or not start<=loc<start+size:continue
            require(typ in (4,5,6) and loc%4==0,'unsupported text relocation')
            off=loc-start
            if symbol=='.text':
                require(typ==4,'unexpected text relocation')
                addend=(words[off//4]&0x3ffffff)*4
                symbol,owner=owner_at(addend)
                words[off//4]=(words[off//4]&0xfc000000)|((addend-owner[0])//4)
            if symbol=='.rodata':symbol='packet_owned_rodata'
            require(not symbol.startswith('.'),'unowned allocation')
            entries.append((off,typ,symbol));externs.add(symbol)
        lines+=['.section .review_'+name+',"ax",@progbits','.balign 4','.globl '+name,
                '.type '+name+',@function',name+':']
        lines+=['.word 0x%08x'%w for w in words]+['.size %s,.-%s'%(name,name)]
        lines+=['.reloc %s+%d,%s,%s'%(name,o,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[t],s)
                for o,t,s in entries]
        counts[name]=len(entries)
    # Both tables form one contiguous 64-byte window, but the object section
    # contains unrelated, explicitly NONMATCH-context data outside this window.
    start,stop=28,92
    require(len(rodata)>=stop,'missing owned tables')
    words=list(struct.unpack('>16I',rodata[start:stop])); entries=[]
    for loc,typ,symbol,dest in relocs:
        if dest!=ri or not start<=loc<stop:continue
        require(typ==2 and symbol=='.text' and loc%4==0,'jump-table relocation')
        off=loc-start; name,owner=owner_at(words[off//4])
        require(name in NUMERIC[:2], 'table leaves its claimed owner')
        words[off//4]-=owner[0];entries.append((off,name));externs.add(name)
    require(len(entries)==16,'complete table entries required')
    lines+=['.section .review_owned,"a",@progbits','.globl packet_owned_start',
            'packet_owned_start:','.globl packet_owned_rodata','packet_owned_rodata = . - 28']
    lines+=['.word 0x%08x'%w for w in words]
    lines+=['.reloc packet_owned_start+%d,R_MIPS_32,%s'%(o,n) for o,n in entries]
    asm,reloc,script,linked=[work/n for n in ('review.s','review.o','review.ld','review.elf')]
    asm.write_text('\n'.join(lines)+'\n')
    addresses=score.image_symbols(); assignments=[]
    for name in sorted(externs-set(MEMBERS)-{'packet_owned_rodata'}):
        address=addresses.get(name,score.address_named(name));require(address is not None,'unknown symbol '+name)
        assignments.append('%s = 0x%X;'%(name,address))
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in MEMBERS)
        +'.review_owned 0x803B98E8 : SUBALIGN(4) { *(.review_owned) }\n'
        +'/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+'\n'.join(assignments))
    for cmd in (['mips-linux-gnu-as','-EB','-mips2','-o',str(reloc),str(asm)],
                ['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(reloc)]):
        require(shutil.which(cmd[0]),'GNU MIPS binutils missing: '+cmd[0])
        subprocess.run(cmd,check=True,capture_output=True)
    ss,sy,rr=elf(linked);require(not rr,'GNU unresolved relocations')
    differences={}
    for name,size in MEMBERS.items():
        index,row,body=ss['.review_'+name];address=int(name[-8:],16)
        actual=560 if baseline and name in NEW else size
        require(row[3]==address and len(body)==actual and sy[name]==(address,actual,2,index),'GNU complete extent')
        native=packed(score.targets()[name])
        offsets=[i for i in range(0,size,4) if i>=len(body) or body[i:i+4]!=native[i:i+4]]
        require(len(offsets)==(124 if baseline and name in NEW else 0),'GNU word mismatch '+name+' got '+str(len(offsets)))
        differences[name]=offsets
    index,row,body=ss['.review_owned']
    require(row[3]==RODATA and row[5]==64 and len(body)==64,'GNU table extent')
    require(sy['packet_owned_rodata']==(RODATA-28,0,0,index),'GNU own-data anchor')
    require(body==image[RODATA-0x8038A400:RODATA-0x8038A400+64],'GNU jump tables differ')
    return dict(relocations=counts,candidate_extents={n:(560 if baseline and n in NEW else size) for n,size in MEMBERS.items()},jump_table_relocations=16,owned_bytes=64,
                owned_start=hex(RODATA),owned_sha256=sha(body),full_extent_words_exact=not baseline,
                differing_offsets=differences)


def capacity_proof(image):
    """Closed numeric-only write set; no language strings feed these buffers."""
    import re
    def string_at(address):
        off=address-0x8038A400;end=image.index(0,off)
        require(end-off<64,'unbounded format input')
        return image[off:end+1]
    decimal=list(range(0x803B8B28,0x803B8B38,4))+list(range(0x803B8B68,0x803B8B9C,4))
    formats={}
    for address in decimal:
        raw=string_at(address);require(raw==b'%d\0','decimal format contract changed')
        formats[hex(address)]=dict(length=len(raw),sha256=sha(raw))
    fixed=string_at(0x803B8BF4);fmt=string_at(0x803B8C00)
    require(len(fixed)==10 and fixed[0]!=255,'fixed narrow copy contract changed')
    require(fmt==b'%.2d:%.2d.%.3d\0','time format contract changed')
    for a,raw in [(0x803B8BF4,fixed),(0x803B8C00,fmt)]:
        formats[hex(a)]=dict(length=len(raw),sha256=sha(raw))
    # Exact modulo ranges from unchanged full AC330 source: each field fits
    # its precision. Precision is a minimum; the range proves it never grows.
    require(max(len(str(n)) for n in range(100))==2,'minute bound')
    require(max(len(str(n)) for n in range(60))==2,'second bound')
    require(max(len(str(n)) for n in range(1000))==3,'millisecond bound')
    require(max(len(str(n)) for n in range(65536))==5,'u16 decimal bound')
    signed32=max(len(str(n)) for n in [-2147483648,2147483647])
    maxima={NUMERIC[0]:signed32+1,NUMERIC[1]:2+1+2+1+3+1,NUMERIC[2]:5+1}
    require(maxima=={NUMERIC[0]:12,NUMERIC[1]:10,NUMERIC[2]:6},'capacity calculation changed')
    require(all(n<=40 for n in maxima.values()),'buffer too small')
    # The dead switch alternatives are included conservatively. In current
    # loops AC54C case8/AC7F4 cases4..6/ACD5C case3 are unreachable.
    return dict(buffer_bytes=40,maximum_write_including_nul=maxima,
                numeric_field_domains={'u16':[0,65535],'signed32':[-2147483648,2147483647],
                    'time_modulo':[100,60,1000]},formats=formats,
                localization_strings_written_to_buffer=False,original_capacity_recovered=False)


def function_body(source,name):
    start=source.index('void '+name+'(void)\n{');stop=source.index('\n}\n',start)+3
    return start,stop,source[start:stop]


def controls(source):
    variants={}
    for key in ('signed_result','store_order','baseline'):
        a,b,body=function_body(source,NEW[0])
        if key in ('signed_result','baseline'):
            require(body.count('? (u32)width : (u32)object_manager_update')==2,'unsigned arms changed')
            body=body.replace('? (u32)width : (u32)object_manager_update','? width : object_manager_update')
        if key in ('store_order','baseline'):
            old='D_8014A118[0].owner=D_803BA800;D_8014A108=1;'
            require(body.count(old)==1,'profile setup changed')
            body=body.replace(old,'D_8014A108=1;D_8014A118[0].owner=D_803BA800;')
        variants[key]=source[:a]+body+source[b:]
    return variants


def original_source(source):
    """Undo only this packet's changes and the six-candidate inherited edit."""
    baseline=controls(source)['baseline']
    old='    u8 *data,*label;\n    s32 i;\n    s32 x,y;\n'
    new='    u8 *data;\n    s32 x,y;\n    u8 *label;\n    s32 i;\n'
    for name in NUMERIC:
        a,b,body=function_body(baseline,name)
        require(body.count('u8 text[40];')==1 and body.count(new)==1,'inherited numeric layout changed')
        body=body.replace('u8 text[40];','u8 text[48];').replace(new,old)
        baseline=baseline[:a]+body+baseline[b:]
    return baseline[:baseline.index('/* Image-A extra-car')]+baseline[baseline.index('typedef signed char'):]


def inputs():
    import re
    with target_context() as image:
        manifest=score.target_manifest()
        meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
        require((meta['image'],meta['base'],meta['rom_offset'])==('A','0x8038A400','0xB5C534'),'population identity')
        rows={r['name']:r for r in meta['functions']};targets=score.targets()
        spec=json.loads((GROUP/'group.json').read_text())
        require(spec['members']==spec['claims']==list(MEMBERS),'all verified members must be claims')
        require(spec['context']==CONTEXT and spec['keep']==KEEP,'context/keep changed')
        require(spec['files']==['group.c'] and spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared','recipe changed')
        source=(GROUP/'group.c').read_text()
        original=(ROOT/'cloud/work/lean/runtime_a_stats_menu_20261006/group.c').read_text()
        require(original_source(source)==original,'undeclared source drift outside AB5FC and inherited numeric declarations')
        files=[GROUP/'group.c',GROUP/'group.json',HERE/'verify.py',
               ROOT/'tests/cloud/test_runtime_a_extra_cars.py']
        result=dict(protocol_version=2,sources={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in files},targets={},symbols={},call_sites={},
            members=list(MEMBERS),claims=spec['claims'],context=CONTEXT,keep=KEEP,flags=spec['flags'],
            asset_sha256=ASSET_HASH,image_sha256=IMAGE_HASH,capacity=capacity_proof(image))
        for name in list(MEMBERS)+CONTEXT:
            raw=packed(targets[name]);row=rows[name];address=int(row['address'],16)
            require(row['size']==len(raw) and address==int(name[-8:],16),'native extent')
            require(image[address-0x8038A400:address-0x8038A400+len(raw)]==raw,'image/target mismatch')
            require(name not in MEMBERS or len(raw)==MEMBERS[name],'member size')
            result['targets'][name]=dict(address=row['address'],size=len(raw),words_sha256=sha(raw))
        addresses=score.image_symbols();tokens=set(re.findall(r'\b[A-Za-z_]\w*\b',source))
        result['symbols']={n:hex(addresses.get(n,score.address_named(n))) for n in sorted(tokens)
                           if addresses.get(n,score.address_named(n)) is not None}
        for callee in NEW:
            address=int(callee[-8:],16)
            result['call_sites'][callee]=[dict(caller=n,offset=hex(i*4)) for n,words in sorted(targets.items())
                for i,w in enumerate(words) if w>>26==3 and (w&0x3ffffff)*4==address&0xfffffff]
            require(result['call_sites'][callee]==[dict(caller='func_803AD57C',offset='0x7ac')],
                    'missing or unexpected native private caller')
        # Helper source files are witnesses; they are not rebuilt in this packet.
        # Pin actual target words, not source text with changing provenance lines.
        old=score.ASM_DIR
        try:
            score.ASM_DIR=ROOT/'asm/us/blob'
            result['helper_targets']={n:dict(words_sha256=sha(packed(score.targets()[n])),size=len(score.targets()[n])*4)
                                      for n in ('fcvt_wrapper','func_800BE6A4','net_state_validate','object_manager_update')}
        finally:score.ASM_DIR=old
        static=ROOT/'asm/us/nonmatchings/rom/lib_34a0/fcvt.s'
        native=re.findall(r'/\*\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s*\*/',static.read_text())
        require(len(native)==987,'static formatter full extent changed')
        result['formatter_core']=dict(address='0x80002CD0',size=len(native)*4,words_sha256=sha(bytes.fromhex(''.join(native))))
        return result


def replay(deep=False):
    binding=inputs();score.ido('cc')
    require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO 5.3 required')
    with target_context() as image,tempfile.TemporaryDirectory(prefix='stats-packet-') as temp:
        work=Path(temp);obj=work/'group.o';score.compile_group(GROUP,obj)
        sections,symbols,relocs=elf(obj);ti,_,text=sections['.text'];results={}
        for name,size in MEMBERS.items():
            value=score.compare(obj,name,show=0);require(value.accepted(),name+': '+value.summary())
            start,extent,kind,index=symbols[name]
            require((extent,kind,index)==(size,2,ti),'full STT_FUNC extent')
            following=min((v[0] for v in symbols.values() if v[2]==2 and v[3]==ti and v[0]>start),default=len(text))
            require(following>=start+size and not any(text[start+size:following]),'extra body words')
            results[name]=dict(comparison=asdict(value),notes=list(value.notes))
        contexts={n:asdict(score.compare(obj,n,show=0)) for n in CONTEXT}
        require(all(v['differing']>0 for v in contexts.values()),'context classification changed')
        causal={};baseline_gnu=None
        for label,source in controls((GROUP/'group.c').read_text()).items():
            folder=work/label;folder.mkdir();(folder/'group.c').write_text(source)
            (folder/'group.json').write_bytes((GROUP/'group.json').read_bytes())
            control=folder/'group.o';score.compile_group(folder,control);causal[label]={}
            for name in NEW:
                value=score.compare(control,name,show=0)
                require(value.differing>0 and not value.extra_words and not value.unresolved and not value.unverified and not value.errors,'invalid causal control')
                if label=='baseline':require(value.differing==124,'baseline changed')
                if label=='store_order':require(value.differing==4,'store-order control changed')
                causal[label][name]=asdict(value)
            if deep and label=='baseline':baseline_gnu=gnu_verify(control,folder,image,baseline=True)
            for name in set(MEMBERS)-set(NEW):require(score.compare(control,name,show=0).accepted(),'preserved candidate regression')
        key=owndata.artifact_dir(score.ASM_DIR);bound=score._own_data[key];negative={}
        try:
            for name,address in zip(NUMERIC[:2],[RODATA,RODATA+36]):
                score._own_data[key]=None;v=score.compare(obj,name,show=0)
                require(not v.accepted() and v.unverified,'missing table not rejected')
                bad=bytearray(image);bad[address-0x8038A400]^=1
                score._own_data[key]=owndata.ImageData.from_image(bytes(bad),0x8038A400)
                v=score.compare(obj,name,show=0);require(not v.accepted() and v.errors,'corrupt table not rejected')
                negative[name]=dict(missing_rejected=True,corrupt_rejected=True)
        finally:score._own_data[key]=bound
        result=dict(inputs=binding,compiler='stock IDO 5.3; no static ROM TU rebuilt',
            compiler_binary_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},members=results,contexts=contexts,controls=causal,
            owned_data_controls=negative,new_exact_code_bytes=588,preserved_exact_code_bytes=4548,
            new_landed_image_bytes=0,new_rom_verified_bytes=0)
        if deep:
            result['gnu']=gnu_verify(obj,work,image)
            result['baseline_gnu']=baseline_gnu
        return result


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text());require(inputs()==expected['inputs'],'input binding changed')
    actual=replay(deep)
    wanted=dict(expected)
    if not deep:
        wanted.pop('gnu',None);wanted.pop('baseline_gnu',None)
    require(actual==wanted,'receipt differs')
    return actual


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true')
    parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true')
    parser.add_argument('--inputs-only',action='store_true');args=parser.parse_args()
    if args.score:
        with target_context():
            sys.argv=[str(ROOT/'tools/cloud/score.py'),'group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')];score.main()
    elif args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    elif args.inputs_only:
        require(inputs()==json.loads((HERE/'verification.json').read_text())['inputs'],'input binding changed');print('PASS: source/input and capacity binding')
    else:
        result=verify(args.deep)
        for name in MEMBERS:print(name+': MATCH')
        print('PASS: 588 new exact bytes; 4548 prior candidate bytes preserved; zero accepted-image/ROM credit')
