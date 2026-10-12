#!/usr/bin/env python3
"""Input-bound image-A profile-selection proof; transient native and ELF data never published."""
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
NEW = 'func_803A7B58'
PRESERVED = {'func_80395E24':104, 'func_803A87D4':920, 'func_803A8B74':1300}
PRESERVED.update({'func_80396EE8':92,'func_80396F44':92})
CHECKED = {**PRESERVED,NEW:3196}
CONTEXT = ['func_803A9088','func_803A9EEC','func_80396FA0']
KEEP = ['func_803A9EEC','func_80396FA0']
BASELINE = HERE / 'baseline_group.c'  # w26p: pre-install snapshot of src/ovl_a/groups/<GROUP_NAME>/group.c (wave26 46780d3e)
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
RODATA = 0x803B9848

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


def gnu_verify(obj,work,image,control=False):
    """Re-emit original member REL records; resolve all words with GNU as/ld."""
    sections,symbols,relocs=elf(obj);ti,_,raw=sections['.text'];ri,_,data=sections['.rodata']
    require(set(n for n,s in sections.items() if s[1][2]&2 and s[1][5])=={'.text','.rodata','.reginfo'},'unexpected allocated section')
    require(len(data)>=72,'missing owned data')
    lines=['.set noreorder','.set noat'];external=set();counts={};extents={}
    def owner_at(addend):
        rows=[(n,s) for n,s in symbols.items() if s[2]==2 and s[3]==ti and s[0]<=addend<s[0]+s[1]]
        require(len(rows)==1,'unowned text anchor');return rows[0]
    for name,size in CHECKED.items():
        start,actual,kind,section=symbols[name]
        require((kind,section)==(2,ti) and (actual==size or control and name==NEW),'complete input extent')
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
    words=list(struct.unpack('>9I',data[36:72]));entries=[]
    for loc,typ,symbol,dest in relocs:
        if dest!=ri or not 36<=loc<72:continue
        require(typ==2 and symbol=='.text' and loc%4==0,'owned table relocation')
        off=loc-36;name,owner=owner_at(words[off//4]);require(name==NEW,'table owner changed')
        words[off//4]-=owner[0];entries.append(off)
    require(entries==list(range(12,36,4)),'six complete table entries')
    lines+=['.section .review_owned,"a",@progbits','.globl packet_owned_start','packet_owned_start:', '.globl packet_owned_rodata','packet_owned_rodata = . - 36']
    lines+=['.word 0x%08x'%w for w in words]
    lines+=['.reloc packet_owned_start+%d,R_MIPS_32,%s'%(o,NEW) for o in entries]
    addresses=score.image_symbols();bindings={n:addresses.get(n,score.address_named(n)) for n in external-set(CHECKED)-{'packet_owned_rodata'}}
    require(all(a is not None for a in bindings.values()),'unknown external symbol')
    asm,reloc,script,linked=[work/n for n in ('review.s','review.o','review.ld','review.elf')]
    asm.write_text('\n'.join(lines)+'\n')
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in CHECKED)
        +'.review_owned 0x803B9848 : SUBALIGN(4) { *(.review_owned) }\n'
        +'/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'
        +''.join('%s = 0x%X;\n'%(n,a) for n,a in sorted(bindings.items())))
    for cmd in (['mips-linux-gnu-as','-EB','-mips2','-o',str(reloc),str(asm)],['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(linked),str(reloc)]):
        require(shutil.which(cmd[0]),'GNU tool absent: '+cmd[0]);subprocess.run(cmd,check=True,capture_output=True)
    ss,sy,rr=elf(linked);require(not rr,'remaining GNU relocations');require(not any(s[3]==0 for s in sy.values()),'undefined GNU symbol');result={}
    for name,size in CHECKED.items():
        i,row,body=ss['.review_'+name];address=int(name[-8:],16)
        require(row[3]==address and len(body)==extents[name] and sy[name]==(address,len(body),2,i),'GNU complete extent')
        native=packed(score.targets()[name]);offsets=[o for o in range(0,max(size,len(body)),4) if o>=len(body) or o>=len(native) or body[o:o+4]!=native[o:o+4]]
        require(not offsets if name!=NEW or not control else bool(offsets),'GNU full-word residual changed')
        result[name]=dict(bytes=len(body),native_bytes=size,relocations=counts[name],differing_offsets=offsets,linked_sha256=sha(body))
    require(set(n for n,s in ss.items() if s[1][2]&2 and s[1][5])=={'.review_'+n for n in CHECKED}|{'.review_owned'},'unexpected GNU allocated section')
    i,row,body=ss['.review_owned'];native=image[RODATA-0x8038A400:RODATA-0x8038A400+36]
    require(row[3]==RODATA and row[5]==len(body)==36 and sy['packet_owned_rodata']==(RODATA-36,0,0,i),'owned extent')
    if not control:require(body==native,'owned data mismatch')
    return dict(functions=result,owned_bytes=36,owned_start=hex(RODATA),owned_exact=body==native,owned_sha256=sha(body),table_relocations=6,external_symbols={n:hex(a) for n,a in sorted(bindings.items())})


def controls(source):
    body=function(source,NEW)[2]
    old=function(BASELINE.read_text(),NEW)[2].replace('func_80395E24(node,player)','func_80395E24(player,node)')
    old_capacity=body.replace('    s32 draw_x,y,x;\n    f32 scale=D_803B5D10;\n    Profile *profile;',
                              '    f32 scale=D_803B5D10;\n    Profile *profile;\n    s32 draw_x,y,x;').replace('text[32]','text[40]')
    return {n:replace_function(source,NEW,b) for n,b in {'baseline':old,'capacity40':old_capacity,
             'without_label_read':body.replace('                if(label) {}\n','')}.items()}


def stack_census(words):
    accesses=[];addresses=set();adjustments=[]
    for i,w in enumerate(words):
        op,base,reg,off=w>>26,(w>>21)&31,(w>>16)&31,w&65535
        off=off if off<32768 else off-65536
        if base==29 and op in (32,33,35,36,37,40,41,43,49,53,57,61):accesses.append([i*4,op,reg,off])
        if base==29 and op==9:
            if reg==29:adjustments.append([i*4,off])
            else:addresses.add(off)
    require(adjustments==[[0x10,-176],[0xc70,176]],'native frame census changed')
    require(addresses=={92,112,120},'native address-taken stack set changed')
    return dict(frame=176,adjustments=adjustments,address_taken=sorted(addresses),accesses=accesses,
                live_regions=dict(position=[92,104],point=[112,116],text=[120,152],x=[164,168],y=[168,172],draw_x=[172,176]),
                inferred_nonaccessed_homes=dict(node=[88,92],seen=[104,108],i=[108,112],limit=[116,120],label=[152,156],profile=[156,160],scale=[160,164]),
                uncertainty='Text32 and nonaccessed scalar homes are reconstruction; exact original capacity/declarations are unknown')


def inputs():
    import re
    with target_context() as image:
        source=(GROUP/'group.c').read_text();spec=json.loads((GROUP/'group.json').read_text())
        require(spec['members']==spec['claims']==list(CHECKED),'every exact member must be claimed')
        require(spec['keep']==KEEP and spec['context']==CONTEXT and spec['files']==['group.c'],'genuine recipe changed')
        require(spec['flags']=='-g0 -O3 -mips2 -G 0 -non_shared','stock flags changed')
        body=function(source,NEW)[2]
        require('text[32]' in body and 'point[2]' in body,'capacity drift')
        uses=[line.strip() for line in body.splitlines() if re.search(r'(?<![.\w])text\b',line)]
        require(len(uses)==3 and uses[1]=='fcvt_wrapper(text,D_803B899C,countdown_state.text[29],player+1);'
                and uses[2]=='state_utility(sound_pitch_diff_halved(text,x),y,text);','temporary text address-use census changed')
        paths=[GROUP/'group.c',GROUP/'group.json',HERE/'verify.py',HERE/'capacity.py',HERE/'host.c',HERE/'trace.py',
               ROOT/'tests/cloud/test_runtime_a_profile_selection.py',BASELINE,HERE/'consumer_audit.md',HERE/'consumer_audit.json',
               ROOT/'src/rom/lib_34a0.c',ROOT/'include/static_formatter_context.h']
        result=dict(protocol_version=2,files={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in paths},
                    targets={},helper_targets={},symbols={},calls={},flags=spec['flags'],members=list(CHECKED),claims=spec['claims'],
                    context=CONTEXT,keep=KEEP,asset_sha256=ASSET_HASH,image_sha256=IMAGE_HASH,text_address_uses=uses)
        manifest=score.target_manifest();meta=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest));rows={r['name']:r for r in meta['functions']};targets=score.targets()
        for name in CHECKED:
            address=int(name[-8:],16);sites={}
            for caller,words in sorted(targets.items()):
                offsets=[i*4 for i,w in enumerate(words) if w>>26==3 and (w&0x3ffffff)*4==address&0xfffffff]
                if offsets:sites[caller]=offsets
            result['calls'][name]=sites
        require(result['calls'][NEW]=={'func_803A9EEC':[0x98]},'native selection caller changed')
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
            helpers=['fcvt_wrapper','sound_pitch_diff_halved','object_manager_update','state_utility','menu_input_process',
                     'func_800B66B0','audio_doppler_calc','func_800A473C','func_80092DCC',
                     'func_800A4E58','func_800A510C','func_800A5158','func_800DB1E0','brake_light_update']
            result['helper_targets']={n:dict(size=len(score.targets()[n])*4,words_sha256=sha(packed(score.targets()[n]))) for n in helpers}
        finally:score.ASM_DIR=old
        # Static memcpy has no blob manifest. Bind only its eleven tracked target
        # words and native extent, not its mutable assembly spelling or ROM status.
        static=ROOT/'asm/us/nonmatchings/rom/lib_8800/memcpy.s'
        rows=re.findall(r'/\* [0-9A-Fa-f]+ ([0-9A-Fa-f]{8}) ([0-9A-Fa-f]{8}) \*/',static.read_text())
        rows=[(int(a,16),int(w,16)) for a,w in rows if 0x80007c68<=int(a,16)<0x80007c94]
        require([a for a,w in rows]==list(range(0x80007c68,0x80007c94,4)),'static memcpy complete target extent')
        result['helper_targets']['memcpy']=dict(size=44,words_sha256=sha(packed([w for a,w in rows])),
              authentication='current tracked words only; no independent static ROM authentication claim')
        audit=json.loads((HERE/'consumer_audit.json').read_text())
        for name,row in audit.items():
            actual=result['helper_targets'][name]
            require(actual['size']==row['bytes'] and actual['words_sha256']==row['words_sha256'],'consumer audit binding changed: '+name)
        result['native_stack']=stack_census(targets[NEW])
        import importlib.util
        sp=importlib.util.spec_from_file_location('profile_capacity',HERE/'capacity.py');c=importlib.util.module_from_spec(sp);sp.loader.exec_module(c)
        result['capacity'],_=c.audit(image,0x8038A400)
        return result


def host_proof(work,sanitize=False):
    import importlib.util,os
    shutil.copyfile(HERE/'host.c',work/'host.c');shutil.copyfile(GROUP/'group.c',work/'group.c')
    # Compare against the current-master body, with only its owner-call source
    # argument order reconciled to the genuine current one-formal helper ABI.
    base=function(BASELINE.read_text(),NEW)[2].replace(NEW,'baseline_803A7B58').replace('func_80395E24(node,player)','func_80395E24(player,node)')
    (work/'baseline.h').write_text(base)
    sp=importlib.util.spec_from_file_location('host_capacity',HERE/'capacity.py');c=importlib.util.module_from_spec(sp);sp.loader.exec_module(c)
    _,strings=c.audit(image_data(),0x8038A400)
    rows=['"'+''.join('\\%03o'%ch for ch in s)+'"' for s in strings]
    (work/'language_data.h').write_text('static char *native_labels[6]={'+','.join(rows)+'};\n')
    cmd=['cc','-std=c99','-O1' if sanitize else '-O2','-ffunction-sections','-fdata-sections',str(work/'host.c'),'-Wl,--gc-sections','-o',str(work/'host')]
    if sanitize:cmd+=['-g','-fsanitize=address,undefined','-fno-omit-frame-pointer']
    subprocess.run(cmd,check=True,capture_output=True);env=os.environ.copy()
    if sanitize:env['ASAN_OPTIONS']='detect_leaks=0'
    value=subprocess.check_output([str(work/'host')],text=True,env=env).strip()
    require(value=='5760 actual-C selection renderer cases; exact stub-event equivalence; 60 formatter extrema cases','host proof changed: '+value)
    return dict(cases=5760,independent_formatter_extrema_cases=60,sanitizers=sanitize,comparison='actual renderer complete helper events versus current-master body',native_gameplay=False)


def selective(obj,work,image):
    from tools.conveyor.pipeline import blob_group,ovl_splice,ovl_layout
    spec=ovl_splice.packet_spec(GROUP);root=work/'installer';folder=root/GROUP_NAME;folder.mkdir(parents=True)
    (folder/'group.json').write_text(json.dumps(spec));(folder/'group.c').write_bytes((GROUP/'group.c').read_bytes())
    objects=work/'objects';objects.mkdir();shutil.copyfile(obj,blob_group.object_path(GROUP_NAME,objects))
    path=work/'image.bin';path.write_bytes(image);doc=ovl_layout.derive('a',image_path=path,write=False)
    bodies=blob_group.group_bodies(GROUP_NAME,doc,extern=score.image_symbols(),obj_dir=objects,root=root)
    require(set(bodies)==set(CHECKED),'selective member set')
    require(all(ovl_splice.mismatch(doc,n,b) is None for n,b in bodies.items()),'selective exact body')
    return {n:len(b) for n,b in sorted(bodies.items())}


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


def replay(deep=False):
    binding=inputs();score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock compiler required')
    with target_context() as image,tempfile.TemporaryDirectory(prefix='profile-selection-proof-') as tmp:
        work=Path(tmp);obj=work/'group.o';score.compile_group(GROUP,obj);sections,symbols,relocs=elf(obj);ti,_,text=sections['.text'];checked={};census=emitted_census(obj)
        for name,size in CHECKED.items():
            value=score.compare(obj,name,show=0);start,extent,kind,index=symbols[name]
            require((extent,kind,index)==(size,2,ti),'complete native extent')
            following=min((s[0] for s in symbols.values() if s[2]==2 and s[3]==ti and s[0]>start),default=len(text))
            require(following>=start+size and not any(text[start+size:following]),'nonzero suffix')
            require(value.accepted(),'not strict exact '+name+': '+value.summary());checked[name]=dict(comparison=asdict(value),extent=extent,notes=list(value.notes))
        contexts={n:asdict(score.compare(obj,n,show=0)) for n in CONTEXT};require(all(q['differing'] for q in contexts.values()),'unclaimed exact context')
        causal={};gnu_controls={}
        for label,source in controls((GROUP/'group.c').read_text()).items():
            folder=work/label;folder.mkdir();(folder/'group.c').write_text(source);(folder/'group.json').write_bytes((GROUP/'group.json').read_bytes());control=folder/'group.o';score.compile_group(folder,control)
            q=score.compare(control,NEW,show=0);require(q.differing and not q.unresolved,'causal control not expected NONMATCH')
            if label=='capacity40':require(q.differing==2,'capacity40 frame-only control changed')
            if label=='without_label_read':require(q.differing==11,'priority control changed')
            for name in PRESERVED:require(score.compare(control,name,show=0).accepted(),'preserved control regression')
            causal[label]=dict(comparison=asdict(q),extent=elf(control)[1][NEW][1])
            if deep:
                gnu_controls[label]=gnu_verify(control,folder,image,control=True)
                if label=='capacity40':require(gnu_controls[label]['functions'][NEW]['differing_offsets']==[0x10,0xc70],'capacity40 residual is not solely native frame adjustments')
        key=owndata.artifact_dir(score.ASM_DIR);prior=score._own_data[key]
        try:
            score._own_data[key]=None;require(score.compare(obj,NEW,show=0).unverified,'missing owned data accepted')
            corrupted=bytearray(image);corrupted[RODATA-0x8038A400]^=1;score._own_data[key]=owndata.ImageData.from_image(bytes(corrupted),0x8038A400)
            require(score.compare(obj,NEW,show=0).errors,'corrupt owned data accepted')
        finally:score._own_data[key]=prior
        result=dict(inputs=binding,compiler='stock IDO5.3; no static ROM TU rebuilt',compiler_sha256={n:sha(Path(score.ido(n)).read_bytes()) for n in ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']},
                    checked=checked,contexts=contexts,emitted_census=census,controls=causal,owned_negative_controls=dict(missing_rejected=True,corruption_rejected=True),new_exact_code_bytes=3196,preserved_exact_code_bytes=2508,new_landed_image_bytes=0,new_rom_verified_bytes=0)
        if deep:
            result['gnu']=gnu_verify(obj,work,image);result['gnu_controls']=gnu_controls
            result['stock_trace']=json.loads(subprocess.check_output([sys.executable,str(HERE/'trace.py')],text=True))
            result['host']=host_proof(work);result['selective_installer']=selective(obj,work,image)
        return result


def verify(deep=False):
    expected=json.loads((HERE/'verification.json').read_text());require(inputs()==expected['inputs'],'input binding changed')
    actual=replay(deep);wanted=dict(expected)
    if not deep:
        for n in ['gnu','gnu_controls','stock_trace','host','selective_installer']:wanted.pop(n,None)
    require(actual==wanted,'receipt changed');return actual

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true');parser.add_argument('--sanitize',action='store_true');parser.add_argument('--inputs-only',action='store_true');args=parser.parse_args()
    if args.score:
        with target_context():sys.argv=[str(ROOT/'tools/cloud/score.py'),'group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')];raise SystemExit(score.main())
    elif args.record:(HERE/'verification.json').write_text(json.dumps(replay(args.deep),indent=2,sort_keys=True)+'\n')
    elif args.sanitize:
        with tempfile.TemporaryDirectory(prefix='profile-selection-host-') as tmp:print(json.dumps(host_proof(Path(tmp),True)))
    elif args.inputs_only:require(inputs()==json.loads((HERE/'verification.json').read_text())['inputs'],'binding changed');print('PASS inputs')
    else:verify(args.deep);print('PASS A7B58 stock exact3196B;2508 inherited bytes preserved')
