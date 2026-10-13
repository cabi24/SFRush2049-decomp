"""Input-bound, full-extent NONMATCH research; never an installer replacement."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
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
GROUP=HERE/'group'
NAME='func_80397FFC'
PRESERVED=['func_80394564','func_80394834','func_80394B00','func_803949CC']
CALLEE='func_80397AD8'
CONTEXT=['func_8039842C']
DATA={NAME:[('root_data','.rodata',0x803B94E4,8)],
      CALLEE:[('callee_rodata','.rodata',0x803B94BC,40),('callee_data','.data',0x803B5EE8,4)]}
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
ASSET_SHA='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
sys.path.insert(0,str(ROOT))
from tools.cloud import score,owndata
from tools.conveyor import compilers
from tools.conveyor.pipeline import ovl_splice
spec=importlib.util.spec_from_file_location('availability_root_elf',HERE/'elf.py')
elf=importlib.util.module_from_spec(spec);spec.loader.exec_module(elf)
require=elf.require

def sha(data):return hashlib.sha256(data).hexdigest()
def packed(words):return struct.pack('>%dI'%len(words),*words)
def shell(*args,**kwargs):
    r=subprocess.run(list(map(str,args)),capture_output=True,text=True,**kwargs)
    require(r.returncode==0,(args,r.stdout,r.stderr));return r.stdout

def image_bytes():
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(len(asset)==12418096 and sha(asset)==ASSET_SHA,'asset identity')
    require(struct.unpack_from('>I',asset,0x3850)[0]==0xB5C534,'loader identity')
    meta=json.loads(score.verified_bytes(ROOT/'asm/us/ovl_a/extents.json',score.target_manifest()))
    decoder=zlib.decompressobj(-15);image=decoder.decompress(asset[0xB5C534-0x283D0:])+decoder.flush()
    require(decoder.eof and len(image)==meta['size'] and sha(image)==meta['image_sha256'],'image identity')
    return image,meta

@contextmanager
def targets():
    prior=score.ASM_DIR;old=score.own_data
    score.ASM_DIR=ROOT/'asm/us/ovl_a'
    try:
        image,_=image_bytes();score.own_data=lambda:owndata.ImageData.from_image(image,0x8038A400)
        yield
    finally:score.ASM_DIR=prior;score.own_data=old

def census(words,name):
    call=0x0c000000|((int(name[-8:],16)>>2)&0x03ffffff)
    return {n:[i*4 for i,w in enumerate(body) if w==call] for n,body in words.items() if call in body}

def inputs():
    with targets():
        image,meta=image_bytes();words=score.targets();rows={r['name']:r for r in meta['functions']}
        g=json.loads((GROUP/'group.json').read_text())
        require(g['claims']==[] and g['members']==PRESERVED+[NAME,CALLEE] and g['context']==CONTEXT,'research roles')
        require(g['keep']==['func_8039842C'] and g['files']==['group.c'] and g['flags']==FLAGS,'genuine kept caller/stock recipe')
        calls={n:census(words,n) for n in (NAME,'func_80397AD8')}
        require(calls[NAME]=={'func_8039842C':[364]},'setup native caller census')
        require(calls['func_80397AD8']=={NAME:[392]},'slot native caller census')
        require(words[NAME][3]==0x27bdffb8 and words[NAME][4]==0xafbf0024,'native internal root frame and sole return-address save')
        require(words[NAME][0x174//4]==0xafbe0000 and words['func_80397AD8'][0]==0x27bdffb0 and words['func_80397AD8'][8]==0x83b80053,'native stack+0 player word and callee low-byte read')
        bound={}
        for n in PRESERVED+[NAME,CALLEE]+CONTEXT:
            row=rows[n];a=int(row['address'],16);size=row['size'];body=packed(words[n])
            require(len(body)==size and image[a-0x8038A400:a-0x8038A400+size]==body,'complete target/image identity')
            next_=min(int(r['address'],16) for r in rows.values() if int(r['address'],16)>a)
            require(a+size==next_,'next native boundary')
            bound[n]={'address':row['address'],'size':size,'next_boundary':hex(next_),'words_sha256':sha(body)}
        tokens=set(re.findall(r'\b[A-Za-z_]\w*\b',(GROUP/'group.c').read_text()));table=score.image_symbols()
        external={n:hex(table.get(n,score.address_named(n))) for n in sorted(tokens) if table.get(n,score.address_named(n)) is not None}
        # The declaration is inherited from this matched table-lookup source;
        # the packet does not introduce volatility merely to tune a score.
        volatile_source=ROOT/'src/blob/func_800B24EC.c'
        require('extern volatile u8 gNumModels;' in volatile_source.read_text(),'table-count declaration provenance')
        files=['group/group.c','group/group.json','baseline.c','baseline.json','context_baseline.c','verify.py','elf.py','trace.py','host.c']
        return {'targets':bound,'calls':calls,'symbols':external,'image_sha256':sha(image),'asset_sha256':ASSET_SHA,
                'root_owned_data':{'address':'0x803B94E4','size':8,'sha256':sha(image[0x803B94E4-0x8038A400:0x803B94EC-0x8038A400])},
                'callee_owned_data':[{
                    'section':sec,'address':hex(a),'size':size,'sha256':sha(image[a-0x8038A400:a-0x8038A400+size])}
                    for label,sec,a,size in DATA[CALLEE]],
                'texture_lookup_source_sha256':sha(volatile_source.read_bytes()),
                'sources':{n:sha((HERE/n).read_bytes()) for n in files},
                'test_sha256':sha((ROOT/'tests/cloud/test_runtime_a_availability_root_research.py').read_bytes()),
                'compiler':'ido5.3','flags':FLAGS,'claims':[],'installation_proposed':False}

def root_body(source):
    start=source.index('void func_80397FFC(void)');end=source.index('\nextern Owner *',start)
    return source[start:end]

def forms():
    candidate=(GROUP/'group.c').read_text();prior=(HERE/'context_baseline.c').read_text()
    old=(HERE/'baseline.c').read_text();g=json.loads((GROUP/'group.json').read_text())
    research_g=dict(g,members=PRESERVED+[NAME],context=[CALLEE]+CONTEXT)
    baseline_g=json.loads((HERE/'baseline.json').read_text())
    complete=prior.replace(root_body(prior),root_body(old))
    parent=complete[:complete.index('/* Complete native-led 97AD8')]
    parent=parent.replace('extern void func_80397AD8(s8);','extern void func_80397AD8(s32);')
    parent_g=dict(research_g,context=['func_8039842C'])
    framed=prior.replace('for (i = 0; i < D_8014A108; i++) {\n        D_803BAC08[i] = ambient_sound_set(-20,0,-10,10,192,0,0,0);\n    }','for (i = 0; i < D_8014A108; i++) D_803BAC08[i] = ambient_sound_set(-20,0,-10,10,192,0,0,0);')
    require(framed!=prior,'loop-line control anchor')
    return [('inherited_kept_root',old,baseline_g,260,1120,104),('genuine_parent',parent,parent_g,176,1032,64),
            ('complete_context',complete,research_g,31,1072,64),('screen_count',framed,research_g,17,1072,72),
            ('inherited_root15',prior,research_g,15,1072,72),('candidate',candidate,g,15,1072,72)]

def extent_evidence(obj,expected_functions):
    sections,syms,rels=elf.read(obj);ti,_,raw=sections['.text']
    functions=sorted((v[0],n,v[1]) for n,v in syms.items() if v[2]==2 and v[3]==ti)
    require({n for _,n,_ in functions}==set(expected_functions),'complete defined-function set')
    out={}
    for i,(off,n,size) in enumerate(functions):
        end=functions[i+1][0] if i+1<len(functions) else len(raw)
        require(size>0 and size%4==0 and off+size<=end and not any(raw[off+size:end]),'complete extent and zero suffix '+n)
        out[n]={'size':size,'zero_suffix_bytes':end-off-size}
    for n,(i,row,data) in sections.items():
        require(not(row[2]&2) or not row[5] or n in ('.text','.rodata','.data','.reginfo','.options'),'unexpected input storage')
    return out

def gnu(obj,directory,names,expected):
    """GNU link complete selected bodies and every original REL record.

    Each owned slice is compared in full at its native address. Interior call
    addends, unsupported sections, unaccounted records and new allocations fail.
    Unclaimed parent context data are outside this selected-body proof.
    """
    sections,syms,rels=elf.read(obj);ti,_,text=sections['.text'];image,_=image_bytes()
    owned={};slices={};storage={}
    if CALLEE in names:
        raw_data=sections['.data'][2]
        require(len(raw_data)==16 and not any(raw_data[4:]),'complete color allocation and zero alignment suffix')
        storage['.data']={'complete_bytes':16,'selected_object_offset':0,'selected_object_bytes':4,'zero_alignment_bytes':12}
    for n in names:
        for label,section,address,size in DATA.get(n,[]):
            data=sections[section][2];native=image[address-0x8038A400:address-0x8038A400+size]
            places=[i for i in range(0,len(data)-size+1,4) if data[i:i+size]==native]
            require(len(places)==1,'unique complete owned slice '+label)
            if n==CALLEE and section=='.data':require(places==[0],'color allocation starts at zero')
            slices[n,section]=(label,places[0],address,size)
            owned[label]=(section,address,data[places[0]:places[0]+size])
    lines=['.set noreorder','.set noat'];external=set();counts={};coverage=set()
    for n in names:
        start,size,kind,index=syms[n];require(kind==2 and index==ti,'selected function type')
        words=list(struct.unpack('>%dI'%(size//4),text[start:start+size]));records=[];data_sites={}
        for off,typ,sym,dest in rels:
            if dest!=ti or not start<=off<start+size:continue
            require(typ in (4,5,6) and off%4==0,'original relocation type/alignment');local=off-start
            require((off,typ,sym,dest) not in coverage,'duplicate original relocation');coverage.add((off,typ,sym,dest))
            if sym=='.text':
                require(typ==4,'unsupported text reference');addend=(words[local//4]&0x03ffffff)*4
                owners=[name for name,v in syms.items() if v[2]==2 and v[3]==ti and v[0]==addend]
                require(len(owners)==1,'non-entry internal call');sym=owners[0];words[local//4]&=0xfc000000
            if sym in ('.rodata','.data'):
                require((n,sym) in slices and typ in (5,6),'unowned selected data')
                label,offset,address,extent=slices[n,sym]
                if typ==5:require(words[local//4]&65535==0,'nonzero owned HI addend')
                else:
                    addend=words[local//4]&65535;require(offset<=addend<offset+extent,'owned slice addend')
                    words[local//4]=(words[local//4]&0xffff0000)|(addend-offset)
                data_sites[sym]=data_sites.get(sym,0)+1;sym=label
            require(not sym.startswith('.'),'unsupported local section');records.append((local,typ,sym));external.add(sym)
        required=({'.rodata':4} if n==NAME else {'.rodata':20,'.data':2} if n==CALLEE else {})
        require(data_sites==required,('complete selected data reference count',n,data_sites))
        lines+=['.section .review_'+n+',"ax",@progbits','.balign 4','.globl '+n,'.type '+n+',@function',n+':']
        lines+=['.word 0x%08x'%w for w in words]+['.size %s,.-%s'%(n,n)]
        lines+=['.reloc %s+%d,%s,%s'%(n,o,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[t],s) for o,t,s in records];counts[n]=len(records)
    selected={r for r in rels if r[3]==ti and any(syms[n][0]<=r[0]<syms[n][0]+syms[n][1] for n in names)}
    require(selected==coverage,'unaccounted selected original relocation')
    for label,(section,address,data) in owned.items():
        lines+=['.section .'+label+',"a",@progbits','.globl '+label,label+':','.byte '+','.join(str(x) for x in data)]
    asm=directory/'gnu.s';assembled=directory/'gnu.o';linked=directory/'gnu.elf';script=directory/'gnu.ld';asm.write_text('\n'.join(lines)+'\n')
    table=score.image_symbols();anchors={}
    for n in sorted(external-set(names)-set(owned)):
        a=table.get(n,score.address_named(n));require(a is not None,'unknown GNU binding '+n);anchors[n]=a
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in names)+
        ''.join('.%s 0x%X : SUBALIGN(4) { *(.%s) }\n'%(label,address,label) for label,(section,address,data) in owned.items())+
        '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+''.join('%s = 0x%X;\n'%(n,a) for n,a in anchors.items()))
    shell('mips-linux-gnu-as','-EB','-mips2','-o',assembled,asm);shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,assembled)
    ss,sy,rr=elf.read(linked);require(not rr and all(v[3]!=0 for n,v in sy.items() if n),'no unresolved GNU state')
    require(all(sy[n][0]==a for n,a in anchors.items()),'GNU external binding drift')
    require({n for n,(i,r,d) in ss.items() if r[2]&2 and r[5]}=={'.review_'+n for n in names}|{'.'+n for n in owned},'linked allocation set')
    owned_proof={}
    for label,(section,address,data) in owned.items():
        require(ss['.'+label][2]==data and ss['.'+label][1][3]==address,'full owned bytes '+label)
        owned_proof[label]={'address':hex(address),'size':len(data),'sha256':sha(data)}
    result={}
    for n in names:
        idx,row,raw=ss['.review_'+n];a=int(n[-8:],16);size=syms[n][1]
        require(row[3]==a and len(raw)==size and sy[n]==(a,size,2,idx),'complete GNU extent')
        body=list(struct.unpack('>%dI'%(size//4),raw));native=score.targets()[n]
        different=[i*4 for i in range(max(len(body),len(native))) if (body[i] if i<len(body) else None)!=(native[i] if i<len(native) else None)]
        if n in PRESERVED+[CALLEE]:require(body==native,'GNU exact-body drift '+n)
        else:
            actual=sum(o<1072 for o in different)
            require(actual=={260:262,176:177}.get(expected,expected),('GNU/scorer root disagreement',expected,actual))
        result[n]={'native_interval_differing_words':sum(o<len(native)*4 for o in different),'extra_nonzero_words':sum(bool(w) for w in body[len(native):]),'missing_native_tail_bytes':max(0,len(native)*4-size),'complete_bytes':size,'differing_offsets':different,'relocation_count':counts[n],'body_sha256':sha(raw)}
    return {'functions':result,'owned_data_bytes':sum(len(x[2]) for x in owned.values()),'owned_data':owned_proof,'selected_storage':storage,'external_bindings':{n:hex(a) for n,a in anchors.items()}}

def replay():
    bound=inputs()
    try:ovl_splice.packet_spec(GROUP)
    except ovl_splice.SpliceError as exc:require('no claims (research only), nothing to splice' in str(exc),'unexpected installer refusal')
    else:raise ValueError('empty-claims packet reached installer selection')
    score.ido('cc');require(compilers.identify_dir(score.IDO)=='ido5.3','stock IDO5.3 required')
    passes={n:sha(Path(score.ido(n)).read_bytes()) for n in ('cc','cfe','uld','usplit','umerge','uopt','ugen','as1')};results={}
    with targets(),tempfile.TemporaryDirectory(prefix='availability-context-') as tmp:
        for label,source,recipe,differing,size,frame in forms():
            directory=Path(tmp)/label;directory.mkdir();(directory/'group.c').write_text(source);(directory/'group.json').write_text(json.dumps(recipe))
            obj=directory/'group.o';score.compile_group(directory,obj);names=recipe['members']+recipe.get('context',[])
            ex=extent_evidence(obj,names);require(ex[NAME]['size']==size,'root complete extent '+label)
            result=score.compare(obj,NAME,show=0);require(result.differing==differing and not result.unresolved,'root control result '+label)
            if label=='inherited_kept_root':
                require(len(result.errors)==3 and result.errors[0].startswith('unpaired R_MIPS_HI16 for D_803B5ED0'),'known truncated-slice baseline relocation diagnostic')
            elif label!='genuine_parent':require(not result.errors,'control relocation error '+label)
            if label not in ('inherited_kept_root','genuine_parent'):require(not result.unverified and not result.extra_words,'root full verified data/extent '+label)
            selected=[n for n in PRESERVED if n in names]+[NAME]
            if label=='candidate':
                require(score.compare(obj,CALLEE,show=0).accepted(),'new callee strict match')
                require(ex[CALLEE]['size']==1308,'complete new callee extent');selected.append(CALLEE)
            for n in PRESERVED:
                if n in names:require(score.compare(obj,n,show=0).accepted(),'strict preservation '+n)
            proof=gnu(obj,directory,selected,differing)
            raw=score.text_words(obj);start=score.symbols(obj)[NAME]//4;body=raw[start:start+size//4]
            frames=[65536-(w&65535) for w in body if w>>16==0x27bd and w&0x8000]
            require(frames==[frame],'single expected native-style frame')
            results[label]={'comparison':asdict(result),'frame':frame,'extents':ex,'gnu':proof,'callee_comparison':asdict(score.compare(obj,CALLEE,show=0)) if CALLEE in names else None}
        # Declaration-order controls isolate the two native stack object homes.
        candidate=(GROUP/'group.c').read_text();declaration='    u8 color[4]={255,255,255,255};\n    f32 scale=1.0f;\n    s16 count;'
        require(candidate.count(declaration)==1,'consumed scale declaration anchor')
        order_controls={}
        for label,replacement,offset in [
            ('scale_before_color','    f32 scale=1.0f;\n    u8 color[4]={255,255,255,255};\n    s16 count;',20),
            ('scale_after_count','    u8 color[4]={255,255,255,255};\n    s16 count;\n    f32 scale=1.0f;',348)]:
            directory=Path(tmp)/label;directory.mkdir();(directory/'group.c').write_text(candidate.replace(declaration,replacement))
            (directory/'group.json').write_text((GROUP/'group.json').read_text());control=directory/'control.o';score.compile_group(directory,control)
            r=score.compare(control,CALLEE,show=0);require(r.differing==1 and not(r.errors or r.unverified or r.extra_words or r.unresolved),'declaration-order control')
            ex=extent_evidence(control,PRESERVED+[NAME,CALLEE]+CONTEXT);require(ex[CALLEE]['size']==1308,'order control full extent')
            own=score.symbols(control)[CALLEE]//4;got=score.text_words(control)[own+offset//4]
            require(got=={20:0x27b40048,348:0x27b3004a}[offset],'specific stack-home control')
            order_controls[label]={'comparison':asdict(r),'different_offset':offset,'complete_bytes':1308,'frame':80}
        # Both malformed instruction and missing/corrupt selected literals fail closed.
        raw=obj.read_bytes();secs,syms,rels=elf.read(obj);m=bytearray(raw);m[secs['.text'][1][4]+syms[NAME][0]+3]^=1
        bad=Path(tmp)/'bad.o';bad.write_bytes(m);require(score.compare(bad,NAME,show=0).differing!=15,'instruction corruption rejected')
        rd=secs['.rodata'];literal=struct.pack('>ff',1.43,1.1);at=rd[2].index(literal);m=bytearray(raw);m[rd[1][4]+at]^=1;bad.write_bytes(m)
        require(score.compare(bad,NAME,show=0).errors or score.compare(bad,NAME,show=0).unverified,'literal corruption rejected')
        over=bytearray(raw);table=secs['.symtab'];changed=0
        for at in range(0,len(table[2]),16):
            no,val,size,info,other,index=struct.unpack_from('>IIIBBH',table[2],at)
            if (val,size,info&15,index)==syms[NAME]:struct.pack_into('>I',over,table[1][4]+at+8,size+4);changed+=1
        require(changed==1,'unique native root extent symbol');bad.write_bytes(over)
        try:extent_evidence(bad,names)
        except ValueError:pass
        else:raise ValueError('overlong function extent accepted')
        m=bytearray(raw);m[secs['.text'][1][4]+syms[CALLEE][0]+3]^=1;bad.write_bytes(m)
        require(not score.compare(bad,CALLEE,show=0).accepted(),'callee instruction corruption rejected')
        image,_=image_bytes()
        for label,section,address,size in DATA[CALLEE]:
            native=image[address-0x8038A400:address-0x8038A400+size];row=secs[section]
            offset=row[2].index(native);m=bytearray(raw);m[row[1][4]+offset]^=1;bad.write_bytes(m)
            r=score.compare(bad,CALLEE,show=0);require(not r.accepted() and (r.errors or r.unverified),'callee owned-data corruption rejected '+label)
            try:gnu(bad,Path(tmp),PRESERVED+[NAME,CALLEE],15)
            except ValueError:pass
            else:raise ValueError('GNU callee corrupt data accepted '+label)
        for label in ('nonzero_color_alignment','expanded_color_allocation'):
            m=bytearray(raw)
            if label=='nonzero_color_alignment':m[secs['.data'][1][4]+4]^=1
            else:
                section_table=struct.unpack_from('>I',raw,32)[0]
                struct.pack_into('>I',m,section_table+40*secs['.data'][0]+20,20)
            bad.write_bytes(m)
            try:gnu(bad,Path(tmp),PRESERVED+[NAME,CALLEE],15)
            except ValueError as exc:require('complete color allocation' in str(exc),'wrong storage rejection')
            else:raise ValueError('invalid color allocation accepted '+label)
        old_data=score.own_data
        try:
            score.own_data=lambda:None;require(score.compare(obj,NAME,show=0).unverified,'missing owned image data rejected');require(score.compare(obj,CALLEE,show=0).unverified,'missing callee owned image data rejected')
        finally:score.own_data=old_data
    return {'protocol_version':2,'claims':[],'installation_proposed':False,'current_installer_refuses_empty_claims':True,'new_individually_strict_bytes':1308,'new_installable_bytes':0,'accepted_image_bytes':0,'rom_bytes':0,
            'preserved_accepted_bytes':1428,'preserved_prior_candidate_bytes':728,'inputs':bound,'compiler_passes':passes,'results':results,'declaration_order_controls':order_controls,
            'negative_controls':['instruction corruption rejected','root literal corruption rejected','overlong function extent rejected','missing root image data rejected','callee instruction corruption rejected','callee complete rodata corruption rejected','callee complete color corruption rejected','missing callee image data rejected','nonzero color alignment rejected','expanded color allocation rejected']}

def host(sanitize=False):
    with tempfile.TemporaryDirectory(prefix='availability-host-') as tmp:
        d=Path(tmp);source=(GROUP/'group.c').read_text();prefix=source[:source.index('void func_80397FFC(void)')]
        # Retain actual layout declarations but not helper implementations.
        start=prefix.index('void func_80394564(s32 player)');end=prefix.index('typedef struct Owner Owner;')
        prefix=prefix[:start]+'void func_80394564(s32);\n'+prefix[end:]
        old=root_body((HERE/'baseline.c').read_text()).replace('void func_80397FFC(void)','void baseline_root(void)')
        unit=d/'host.c';unit.write_text(prefix+'\n'+root_body(source)+'\n'+old+'\n'+(HERE/'host.c').read_text())
        exe=d/'host';flags=['-std=c99','-O1','-fno-fast-math','-ffp-contract=off']
        if sanitize:flags+=['-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-sanitize-recover=all']
        shell('cc',*flags,unit,'-o',exe);out=shell(exe,env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'))
        require(out=='20480 actual-root logical-host equivalence cases\n','host matrix count');return out.strip()

def verify():
    receipt=json.loads((HERE/'verification.json').read_text());require(inputs()==receipt['inputs'],'input drift')
    actual=replay();require(actual==receipt,'proof drift');return actual

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--record',action='store_true');parser.add_argument('--deep',action='store_true');parser.add_argument('--score',action='store_true');args=parser.parse_args()
    if args.record:(HERE/'verification.json').write_text(json.dumps(replay(),indent=2,sort_keys=True)+'\n')
    else:verify()
    if args.deep:print(host());print(host(True)+' (ASan/UBSan)')
    if args.score:
        with targets():
            sys.argv=['score.py','group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')];require(score.main()==1,'NONMATCH canonical failure')
    print('PASS: 97AD8 strict 327/327 (1308 bytes); root 15/268 NONMATCH; empty claims and zero installable/ROM bytes.')
