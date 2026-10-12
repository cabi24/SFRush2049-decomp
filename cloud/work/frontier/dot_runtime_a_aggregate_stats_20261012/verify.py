#!/usr/bin/env python3
"""Input-bound proof of eleven complete genuine statistics-group bodies."""
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
GROUP_NAME='runtime_a_stats_menu_20261006'
GROUP=HERE/'groups'/GROUP_NAME
BASELINE=ROOT/'cloud/work/frontier/dot_runtime_a_track_stats_20261012/groups'/GROUP_NAME/'group.c'
NEW='func_803ABEC8'
MEMBERS={'func_803AAF0C':1776,'func_803AB5FC':588,'func_803AB848':1664,'func_803ABEC8':1128,'func_803AC330':540,
         'func_803AC54C':680,'func_803AC7F4':664,'func_803ACA94':712,'func_803ACD5C':512,
         'func_803ACF5C':376,'func_803AD0D4':1184}
CONTEXT=['func_803AA3D8','func_803AD57C']
KEEP=['func_803AD57C']
IMAGE_HASH='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
RODATA=0x803B98CC
TABLES=[NEW,'func_803AC54C','func_803AC7F4','func_803ACA94','func_803AD0D4']


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

def gnu_verify(obj, work, image, control=False):
    """Independent original-REL GNU link, including all five entire jump tables."""
    selected=MEMBERS
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
    for name,size in selected.items():
        start,actual,kind,section=symbols[name]
        require((kind,section)==(2,ti) and (actual==size or control and name==NEW),'complete function extent')
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
    # Five tables form one contiguous 180-byte window, but the object section
    # contains unrelated, explicitly NONMATCH-context data outside this window.
    start,stop=0,180
    require(len(rodata)>=stop,'missing owned tables')
    words=list(struct.unpack('>45I',rodata[start:stop])); entries=[]
    for loc,typ,symbol,dest in relocs:
        if dest!=ri or not start<=loc<stop:continue
        require(typ==2 and symbol=='.text' and loc%4==0,'jump-table relocation')
        off=loc-start; name,owner=owner_at(words[off//4])
        require(name in TABLES, 'table leaves its claimed owner')
        words[off//4]-=owner[0];entries.append((off,name));externs.add(name)
    require(len(entries)==45,'complete table entries required')
    lines+=['.section .review_owned,"a",@progbits','.globl packet_owned_start',
            'packet_owned_start:','.globl packet_owned_rodata','packet_owned_rodata = .']
    lines+=['.word 0x%08x'%w for w in words]
    lines+=['.reloc packet_owned_start+%d,R_MIPS_32,%s'%(o,n) for o,n in entries]
    asm,reloc,script,linked=[work/n for n in ('review.s','review.o','review.ld','review.elf')]
    asm.write_text('\n'.join(lines)+'\n')
    addresses=score.image_symbols(); assignments=[]
    for name in sorted(externs-set(selected)-{'packet_owned_rodata'}):
        address=addresses.get(name,score.address_named(name));require(address is not None,'unknown symbol '+name)
        assignments.append('%s = 0x%X;'%(name,address))
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in selected)
        +'.review_owned 0x803B98CC : SUBALIGN(4) { *(.review_owned) }\n'
        +'/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+'\n'.join(assignments))
    for cmd in (['mips-linux-gnu-as','-EB','-mips2','-o',str(reloc),str(asm)],
                ['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(reloc)]):
        require(shutil.which(cmd[0]),'GNU MIPS binutils missing: '+cmd[0])
        subprocess.run(cmd,check=True,capture_output=True)
    ss,sy,rr=elf(linked);require(not rr,'GNU unresolved relocations')
    require(all(v[3]!=0 for v in sy.values()),'GNU undefined symbol')
    require(set(n for n,v in ss.items() if v[1][2]&2 and v[1][5])=={'.review_'+n for n in selected}|{'.review_owned'},'GNU unexpected allocated section')
    differences={}
    for name,size in selected.items():
        index,row,body=ss['.review_'+name];address=int(name[-8:],16)
        actual=symbols[name][1]
        require(row[3]==address and len(body)==actual and sy[name]==(address,actual,2,index),'GNU complete extent')
        native=packed(score.targets()[name])
        offsets=[i for i in range(0,max(size,len(body)),4) if i>=len(body) or i>=len(native) or body[i:i+4]!=native[i:i+4]]
        require((bool(offsets) if control and name==NEW else not offsets),'GNU word mismatch '+name+' got '+str(len(offsets)))
        differences[name]=offsets
    index,row,body=ss['.review_owned']
    require(row[3]==RODATA and row[5]==180 and len(body)==180,'GNU table extent')
    require(sy['packet_owned_rodata']==(RODATA,0,0,index),'GNU own-data anchor')
    require(body[28:]==image[RODATA-0x8038A400+28:RODATA-0x8038A400+180],'GNU preserved tables differ')
    if not control:require(body[:28]==image[RODATA-0x8038A400:RODATA-0x8038A400+28],'GNU new table differs')
    return dict(relocations=counts,candidate_extents={n:symbols[n][1] for n in selected},
                jump_table_relocations=45,owned_bytes=180,owned_start=hex(RODATA),owned_sha256=sha(body),
                new_table_exact=body[:28]==image[RODATA-0x8038A400:RODATA-0x8038A400+28],
                differing_offsets=differences)


def focus(source):
    start=source.index('void '+NEW+'(void)\n{');stop=source.index('\n}\n',start)+3
    return start,stop,source[start:stop]


def controls(source):
    start,stop,body=focus(source)
    scan=body.replace('track=PROFILE_DATA+1292+i*64','data=PROFILE_DATA+1292+i*64').replace('U32_AT(track,12)','U32_AT(data,12)').replace('U16_AT(track,60)','U16_AT(data,60)').replace(',*track','')
    counter=re.sub(r'\brow\b','i',body).replace('s16 i,i;','s16 i;')
    quotient=body.replace('    u32 divisor;','    u32 divisor,scaled;').replace('fcvt_wrapper(text,D_803B8B9C,(distance*divisor/6)/divisor,(distance*divisor/6)%divisor);','scaled=distance*divisor/6;\n                fcvt_wrapper(text,D_803B8B9C,scaled/divisor,scaled%divisor);')
    variants={'shared_scan_pointer':scan,'shared_counter':counter,'named_quotient':quotient,'baseline':focus(BASELINE.read_text())[2]}
    return {n:source[:start]+text+source[stop:] for n,text in variants.items()}


def capacity(image):
    formats={}
    expected={0x803B8B9C:b'%01d.%01d\0',0x803B8BA8:b'%01d.%01d\0',0x803B8BB4:b'%d\0',0x803B8BB8:b'%d\0',0x803B8BBC:b'%d/%d\0',0x803B8BC4:b'%d/%d\0',0x803B8BCC:b'%d/%d\0',0x803B8BD4:b'%d/%d\0'}
    for address,want in expected.items():
        at=address-0x8038A400;raw=image[at:image.index(0,at)+1]
        require(raw==want,'native format contract changed '+hex(address)+' '+repr(raw))
        formats[hex(address)]=dict(length=len(raw),sha256=sha(raw))
    return dict(buffer_bytes=48,capacity_unchanged=True,formats=formats,
                maximum_write_including_nul=12,contract='Existing formatter contract; signed 32-bit decimal total at most11 chars; unsigned divided distance at most9 digits plus decimal, one remainder digit, and a NUL; u16 and bit counts bounded.')


def inputs():
    with target_context() as image:
        manifest=score.target_manifest();meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
        require((meta['image'],meta['base'],meta['rom_offset'])==('A','0x8038A400','0xB5C534'),'population identity')
        rows={r['name']:r for r in meta['functions']};targets=score.targets()
        spec=json.loads((GROUP/'group.json').read_text());source=(GROUP/'group.c').read_text()
        require(spec['members']==spec['claims']==list(MEMBERS),'all exact members must be claims')
        require(spec['context']==CONTEXT and spec['keep']==KEEP,'genuine context changed')
        require(spec['files']==['group.c'] and spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared','recipe changed')
        a,b,_=focus(source);oa,ob,_=focus(BASELINE.read_text());base=BASELINE.read_text()
        # A single disclosed header is the sole edit outside the target body.
        header_start=source.index('/* Image-A aggregate statistics ABEC8:')
        header_stop=source.index('*/',header_start)+3
        clean=source[:header_start]+source[header_stop:];ca,cb,_=focus(clean)
        require(clean[:ca]==base[:oa] and clean[cb:]==base[ob:],'preserved source changed')
        files=[GROUP/'group.c',GROUP/'group.json',HERE/'verify.py',HERE/'trace.py',HERE/'host.c',
               ROOT/'tests/cloud/test_runtime_a_aggregate_stats.py',BASELINE]
        result=dict(protocol_version=2,sources={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in files},
                    targets={},symbols={},members=list(MEMBERS),claims=spec['claims'],context=CONTEXT,keep=KEEP,
                    asset_sha256=ASSET_HASH,image_sha256=IMAGE_HASH,capacity=capacity(image))
        for name in list(MEMBERS)+CONTEXT:
            raw=packed(targets[name]);row=rows[name];address=int(row['address'],16)
            require(row['size']==len(raw) and address==int(name[-8:],16),'native extent')
            require(image[address-0x8038A400:address-0x8038A400+len(raw)]==raw,'image/target mismatch')
            require(name not in MEMBERS or len(raw)==MEMBERS[name],'member size')
            if name==NEW:require(min(int(r['address'],16) for r in rows.values() if int(r['address'],16)>address)==address+len(raw),'next native boundary')
            result['targets'][name]=dict(address=row['address'],size=len(raw),words_sha256=sha(raw))
        addresses=score.image_symbols();tokens=set(re.findall(r'\b[A-Za-z_]\w*\b',source))
        result['symbols']={n:hex(addresses.get(n,score.address_named(n))) for n in sorted(tokens) if addresses.get(n,score.address_named(n)) is not None}
        result['call_sites']=[dict(caller=n,offset=hex(i*4)) for n,words in sorted(targets.items()) for i,w in enumerate(words)
                             if w>>26==3 and (w&0x3ffffff)*4==int(NEW[-8:],16)&0xfffffff]
        require(result['call_sites']==[dict(caller='func_803AD57C',offset='0x78c')],'native caller census')
        caller=targets['func_803AD57C'];body=targets[NEW]
        saved=sorted({(word>>16)&31 for word in caller[:25] if word>>26==43 and (word>>21)&31==29})
        require(set(range(16,24))|{30,31}<=set(saved),'genuine parent must own all private integer saves')
        require(body[0]==0x27bdff68 and body[4]==0xafbf001c,'native152-byte frame and ra save')
        require(caller[0x790//4]==0,'native no-argument call delay slot')
        result['private_abi']=dict(no_formals=True,caller_saves=saved,native_frame=152,call_offset='0x78c')
        old=score.ASM_DIR
        try:
            score.ASM_DIR=ROOT/'asm/us/blob'
            result['helper_targets']={n:dict(words_sha256=sha(packed(score.targets()[n])),size=len(score.targets()[n])*4)
                for n in ('fcvt_wrapper','func_800BE6A4','func_800B78A4','func_800B669C','object_create','dispatch_handler','sound_pitch_diff_calc','state_utility','object_bytes_sum_global','crowd_cheer_play')}
        finally:score.ASM_DIR=old
        return result


def replay(deep=False):
    binding=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO 5.3 required')
    with target_context() as image,tempfile.TemporaryDirectory(prefix='aggregate-stats-proof-') as temp:
        work=Path(temp);obj=work/'group.o';score.compile_group(GROUP,obj)
        sections,symbols,relocs=elf(obj);ti,_,text=sections['.text'];members={}
        for name,size in MEMBERS.items():
            value=score.compare(obj,name,show=0);require(value.accepted(),name+': '+value.summary())
            start,extent,kind,index=symbols[name];require((extent,kind,index)==(size,2,ti),'full STT_FUNC extent')
            following=min((v[0] for v in symbols.values() if v[2]==2 and v[3]==ti and v[0]>start),default=len(text))
            require(following>=start+size and not any(text[start+size:following]),'nonzero function suffix')
            members[name]=dict(comparison=asdict(value),notes=list(value.notes))
        contexts={n:asdict(score.compare(obj,n,show=0)) for n in CONTEXT};require(all(v['differing'] for v in contexts.values()),'NONMATCH context changed')
        causal={};gnu_controls={}
        for label,source in controls((GROUP/'group.c').read_text()).items():
            folder=work/label;folder.mkdir();(folder/'group.c').write_text(source);(folder/'group.json').write_bytes((GROUP/'group.json').read_bytes())
            control=folder/'group.o';score.compile_group(folder,control);value=score.compare(control,NEW,show=0)
            require(value.differing and not value.unresolved,'invalid causal control')
            if label=='baseline':require(value.differing==243 and not value.extra_words,'fresh baseline changed')
            causal[label]=asdict(value)
            for name in MEMBERS:
                if name!=NEW:require(score.compare(control,name,show=0).accepted(),'preserved body regression')
            if deep:gnu_controls[label]=gnu_verify(control,folder,image,control=True)
        key=owndata.artifact_dir(score.ASM_DIR);bound=score._own_data[key];negative={}
        try:
            for name,address in zip(TABLES,[RODATA,RODATA+28,RODATA+64,RODATA+92,RODATA+124]):
                score._own_data[key]=None;value=score.compare(obj,name,show=0);require(not value.accepted() and value.unverified,'missing owned table not rejected')
                bad=bytearray(image);bad[address-0x8038A400]^=1;score._own_data[key]=owndata.ImageData.from_image(bytes(bad),0x8038A400)
                value=score.compare(obj,name,show=0);require(not value.accepted() and value.errors,'corrupt owned table not rejected')
                negative[name]=dict(missing_rejected=True,corrupt_rejected=True)
        finally:score._own_data[key]=bound
        result=dict(inputs=binding,compiler='stock IDO 5.3; no static ROM TU rebuilt',
                    compiler_binary_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
                    members=members,contexts=contexts,controls=causal,owned_data_controls=negative,
                    new_exact_code_bytes=1128,preserved_exact_code_bytes=8696,new_owned_bytes=28,new_landed_image_bytes=0,new_rom_verified_bytes=0)
        if deep:
            result['gnu']=gnu_verify(obj,work,image);result['gnu_controls']=gnu_controls
            result['stock_trace']=json.loads(subprocess.check_output([sys.executable,str(HERE/'trace.py')],text=True))
            result['host_behavior']=host_proof(work)
            result['selective_installer']=selective(obj,work,image)
        return result


def selective(obj,work,image):
    from tools.conveyor.pipeline import blob_group,ovl_splice,ovl_layout
    spec=ovl_splice.packet_spec(GROUP)
    root=work/'installer';folder=root/GROUP_NAME;folder.mkdir(parents=True)
    (folder/'group.json').write_text(json.dumps(spec));(folder/'group.c').write_bytes((GROUP/'group.c').read_bytes())
    objects=work/'objects';objects.mkdir();shutil.copyfile(obj,blob_group.object_path(GROUP_NAME,objects))
    path=work/'image.bin';path.write_bytes(image)
    doc=ovl_layout.derive('a',image_path=path,write=False)
    bodies=blob_group.group_bodies(GROUP_NAME,doc,extern=score.image_symbols(),obj_dir=objects,root=root)
    require(set(bodies)==set(MEMBERS),'selective installer output set')
    require(all(ovl_splice.mismatch(doc,n,b) is None for n,b in bodies.items()),'selective installer body mismatch')
    return {n:len(b) for n,b in sorted(bodies.items())}


def host_proof(work):
    compiler=shutil.which('cc');require(compiler,'C compiler missing')
    exe=work/'host';command=[compiler,'-std=c99','-O2','-fno-strict-aliasing','-ffunction-sections','-fdata-sections',str(HERE/'host.c'),'-Wl,--gc-sections','-o',str(exe)]
    subprocess.run(command,check=True,capture_output=True)
    return json.loads(subprocess.check_output([str(exe)],text=True))


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text());require(inputs()==expected['inputs'],'input binding changed')
    actual=replay(deep);wanted=dict(expected)
    if not deep:
        for n in ('gnu','gnu_controls','stock_trace','host_behavior','selective_installer'):wanted.pop(n,None)
    require(actual==wanted,'receipt differs');return actual


if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true')
    parser.add_argument('--score',action='store_true');parser.add_argument('--inputs-only',action='store_true');args=parser.parse_args()
    if args.score:
        with target_context():
            sys.argv=[str(ROOT/'tools/cloud/score.py'),'group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')];status=score.main()
        raise SystemExit(status)
    elif args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    elif args.inputs_only:
        require(inputs()==json.loads((HERE/'verification.json').read_text())['inputs'],'input binding changed');print('PASS: inputs')
    else:
        verify(args.deep);print('PASS: 1128 new exact candidate bytes; 8696 prior bytes preserved; no accepted-image/ROM credit')
