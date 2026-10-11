#!/usr/bin/env python3
"""Independent full-body, ABI and bounded semantic review of availability C.

No original bytes or assembly are published. GNU links newly compiled bodies
with their complete relocation records. This is not a whole-image link.
"""
import argparse
import dataclasses
import hashlib
import importlib.util
import itertools
import json
from pathlib import Path
import random
import re
import struct
import subprocess
import sys
import tempfile
import time
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP = ROOT/'cloud/work/frontier/dot_runtime_a_callers_deep_20261011'
SUPPORT = ROOT/'cloud/work/frontier/dot_runtime_a_availability_boundary_20261006'
sys.path[:0] = [str(HERE), str(SUPPORT), str(ROOT/'tools/cloud')]
import native
from elf_reader import elf

SIZES = {'func_80392F28':220, 'func_80393004':180,
         'func_803930B8':264, 'func_803931C0':276, 'func_803932E4':168}
ENTRIES = [0x80392F28, 0x803930B8, 0x803931C0]
FLAG_CLEAR = (0x803BA84D, 0x803BA854, 0x803BA850, 0x803BA80C)
require = native.require
signed = native.signed
read = native.read
write = native.write


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def shell(*args, **kw):
    result = subprocess.run(list(map(str,args)), capture_output=True, text=True, **kw)
    require(result.returncode == 0 and 'runtime error:' not in result.stderr,
            (args,result.stdout,result.stderr))
    return result.stdout


def function(source, name):
    match = re.search(r'^(?:void|s32) '+name+r'\([^;\n]*\)\s*\{',source,re.M)
    require(match is not None, 'missing function '+name)
    start = match.start()
    end = source.index('{',start)+1
    depth = 1
    while depth:
        depth += (source[end] == '{')-(source[end] == '}')
        end += 1
    return source[start:end]


def gnu_link(score, obj, temp):
    sections,symbols,relocations = elf(obj)
    text_index,_,raw = sections['.text']
    lines = ['.set noreorder','.set noat']
    external,counts = set(),{}
    for name,size in SIZES.items():
        start,extent,kind,index = symbols[name]
        require((extent,kind,index)==(size,2,text_index),('wrong ELF extent',name,symbols[name]))
        require(not any(v[2]==2 and v[3]==index and start<v[0]<start+extent for v in symbols.values()),'overlapping function')
        words = list(struct.unpack('>%dI'%(size//4),raw[start:start+size]))
        records = []
        for offset,typ,symbol,dest in relocations:
            if dest != text_index or not start <= offset < start+size:
                continue
            require(typ in (4,5,6) and offset%4==0,'unsupported relocation')
            local = offset-start
            if symbol == '.text':
                require(typ == 4,'non-call text relocation')
                addend = (words[local//4] & 0x3FFFFFF)*4
                owners = [(n,v) for n,v in symbols.items()
                          if v[2]==2 and v[3]==text_index and v[0]<=addend<v[0]+v[1]]
                require(len(owners)==1,'ambiguous local call owner')
                symbol,owner = owners[0]
                words[local//4] = (words[local//4] & 0xFC000000)|((addend-owner[0])//4)
            require(not symbol.startswith('.'),'unowned local data')
            records.append((local,typ,symbol))
            external.add(symbol)
        lines += ['.section .review_'+name+',"ax",@progbits','.balign 4',
                  '.globl '+name,'.type '+name+',@function',name+':']
        lines += ['.word 0x%08x'%w for w in words]
        lines += ['.size %s,.-%s'%(name,name)]
        lines += ['.reloc %s+%d,%s,%s'%(name,off,{4:'R_MIPS_26',5:'R_MIPS_HI16',6:'R_MIPS_LO16'}[typ],symbol)
                  for off,typ,symbol in records]
        counts[name] = len(records)
    source,obj2,linked,script = [temp/name for name in ('review.s','review.o','review.elf','review.ld')]
    source.write_text('\n'.join(lines)+'\n')
    assignments = []
    addresses = score.image_symbols()
    for name in sorted(external-set(SIZES)):
        address = addresses.get(name,score.address_named(name))
        require(address is not None,'unresolved '+name)
        assignments.append('%s = 0x%X;'%(name,address))
    script.write_text('SECTIONS {\n'+''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n'%(n,n[-8:],n) for n in SIZES)
                      +'/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n'+'\n'.join(assignments))
    shell('mips-linux-gnu-as','-EB','-mips2','-o',obj2,source)
    shell('mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj2)
    ss,sy,rr = elf(linked)
    require(not rr,'unresolved GNU relocation')
    result = {}
    for name,size in SIZES.items():
        index,row,body = ss['.review_'+name]
        require(row[3]==int(name[-8:],16) and len(body)==size,'wrong linked extent')
        require(sy[name]==(int(name[-8:],16),size,2,index),'wrong linked symbol')
        result[name] = list(struct.unpack('>%dI'%(size//4),body))
    return result,counts


def cases():
    def base(**kw):
        c = dict(entry=0,argument=0,level=0,sublevel=193,group=0,pointer=0,player=0,
                 cursors=[0]*6,masks=[1]*6,enabled=[1]*4,
                 helpers=[[1]*4,[0]*4,[0]*4],nodes=[])
        c.update(kw)
        return c
    for level in range(-128,128):
        for old in (0,1,3,127,128,193,254,255):
            yield base(level=level,sublevel=old)
    # All five observed producer paths in every combination across four slots.
    for states in itertools.product(range(5),repeat=4):
        enabled = [(-128 if i%2 else 1) if s else 0 for i,s in enumerate(states)]
        helpers = [[0 if s==1 else -7 for s in states],
                   [1 if s==2 else 0 for s in states],
                   [-1 if s==3 else 0 for s in states]]
        for nodes in ([],[0,1,2,3,3,2,1,0,255,128,3]):
            for entry in (1,2):
                yield base(entry=entry,enabled=enabled,helpers=helpers,nodes=nodes,
                           player=sum(states)%4,level=31,group=5)
    # Every cyclic search start/end pair in each real 13-element row.
    for group in range(6):
        for start in range(13):
            for wanted in range(13):
                cursors=[0]*6;cursors[group]=start
                masks=[1]*6;masks[group]=1<<wanted
                yield base(entry=2,argument=1,level=29+(start%3),group=group,cursors=cursors,masks=masks)
    for level in range(-128,128):
        for pointer in (0,0x80701000):
            yield base(entry=2,argument=1,level=level,pointer=pointer,group=2,
                       cursors=[0,0,12,0,0,0],masks=[1]*6)
    for mode in (-2147483648,-1,2,3,2147483647):
        yield base(entry=2,argument=mode,level=-91,sublevel=254,group=4)
    rng=random.Random(0x931C0)
    for _ in range(1024):
        yield base(entry=1,enabled=[rng.choice((0,1,-1,-128,127)) for _ in range(4)],
                   helpers=[[rng.choice((0,0,-1,1)) for _ in range(4)] for _ in range(3)],
                   nodes=[rng.choice((0,1,2,3,4,127,128,255)) for _ in range(rng.randrange(24))])


def state(case, salt):
    spans = [(0x803BA7C0,0x803BA890),(0x803B654C,0x803B65AC),
             (0x80156CE8,0x80156D38),(0x8012E6D0,0x8012E6F0),
             (native.SP-64,native.SP+16),(0x80700000,0x80700C00)]
    memory = {a: ((a*37+salt*73)^0xA5)&255 for lo,hi in spans for a in range(lo,hi)}
    for address,size,value in ((0x803BA858,1,case['level']),(0x803BA860,1,case['sublevel']),
                               (0x803BA880,4,case['group']),(0x803BA800,4,case['pointer']),
                               (0x803B65A4,1,case['player'])):
        write(memory,address,size,value)
    for i in range(6):
        write(memory,0x803BA818+4*i,4,case['cursors'][i])
        for j in range(13):
            write(memory,0x803B6554+13*i+j,1,0x80 if case['masks'][i]>>j&1 else 0)
    for i,value in enumerate(case['enabled']):
        write(memory,0x80156CF0+16*i,1,value)
    write(memory,0x8012E6E0,4,0x80700000 if case['nodes'] else 0)
    for i,slot in enumerate(case['nodes']):
        node=0x80700000+128*i
        for address,value in ((node,node+16),(node+16,node+128 if i+1<len(case['nodes']) else 0),
                              (node+24,node+32),(node+32,node+48)):
            write(memory,address,4,value)
        write(memory,node+64,1,slot)
    return memory


def oracle(initial, case):
    memory=dict(initial);calls=[]
    def leaf():
        level=signed(read(memory,0x803BA858,1),8)
        group=next((i for i,bound in enumerate((12,20,24,25,29)) if level<bound),5)
        write(memory,0x803BA880,4,group)
        if group==4:write(memory,0x803BA860,1,level-25)
        for a in range(0x803BA7FC,0x803BA800):write(memory,a,1,1)
    def producer():
        for i in range(4):
            count=-4
            if case['enabled'][i]:
                calls.append((native.HELPERS[0],i));count=-1
                if case['helpers'][0][i]:
                    calls.append((native.HELPERS[1],i));count=-2
                    if not case['helpers'][1][i]:
                        calls.append((native.HELPERS[2],i));count=-3
                        if not case['helpers'][2][i]:count=case['nodes'].count(i)
            write(memory,0x803BA830+4*i,4,count)
    if case['entry']==0:leaf()
    elif case['entry']==1:producer()
    else:
        mode=case['argument'];write(memory,0x803BA804,4,mode)
        if mode==0:
            producer()
            count=signed(read(memory,0x803BA830+4*case['player'],4))
            for address in (0x803BA7E0,0x803BA7F0):
                for j in range(11):
                    value = 1 if j<2 else (int(count<1) if address==0x803BA7F0 else 0) if j==2 else int(j-3<count)
                    write(memory,address+j,1,value)
            for address in FLAG_CLEAR:write(memory,address,1,0)
        elif mode==1:
            if not case['pointer'] and case['level']>=29:
                write(memory,0x803BA858,1,0)
                group=case['group'];cursor=case['cursors'][group]
                while not case['masks'][group]>>cursor&1:cursor=(cursor+1)%13
                write(memory,0x803BA818+4*group,4,cursor)
            leaf()
        write(memory,0x803BA7D4,4,1)
    return memory,calls


def snapshot(memory):
    fields = [(0x803BA880,4),(0x803BA860,1),(0x803BA858,1),(0x803BA804,4),(0x803BA7D4,4)]
    fields += [(a,1) for a in range(0x803BA7FC,0x803BA800)]
    fields += [(0x803BA830+4*i,4) for i in range(4)]
    fields += [(0x803BA818+4*i,4) for i in range(6)]
    fields += [(a,1) for a in FLAG_CLEAR]
    fields += [(a+j,1) for a in (0x803BA7E0,0x803BA7F0) for j in range(11)]
    return [signed(read(memory,a,n)) if n==4 else read(memory,a,n) for a,n in fields]


def audit_calls(obj):
    sections,symbols,rels=elf(obj);text,_,raw=sections['.text']
    records=[]
    for location,typ,symbol,dest in rels:
        if typ!=4 or dest!=text:continue
        word=int.from_bytes(raw[location:location+4],'big')
        if symbol=='.text':
            addend=(word&0x3FFFFFF)*4
            owners=[n for n,v in symbols.items() if v[2]==2 and v[3]==text and v[0]==addend]
            require(len(owners)==1,'non-entry local call')
            symbol=owners[0]
        if symbol not in SIZES:continue
        owners=[(n,v) for n,v in symbols.items() if v[2]==2 and v[3]==text and v[0]<=location<v[0]+v[1]]
        require(len(owners)==1,'ambiguous caller')
        name,owner=owners[0]
        records.append(dict(caller=name,callee=symbol,offset=hex(location-owner[0])))
    require(sum(r['callee']=='func_80392F28' for r in records)>=3,'lost genuine selector callers')
    require({r['caller'] for r in records if r['callee']=='func_80392F28'}=={'func_803931C0','func_80394058'},'unexpected selector callers')
    return records


def verify(group=GROUP):
    start=time.monotonic()
    spec=importlib.util.spec_from_file_location('availability_family_review_score',ROOT/'tools/cloud/score.py')
    score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
    score.ASM_DIR=ROOT/'asm/us/ovl_a'
    manifest=score.target_manifest();targets=score.targets()
    extents=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifest))
    require((extents['image'],extents['base'],extents['rom_offset'])==('A','0x8038A400','0xB5C534'),'wrong image')
    asset=(ROOT/'assets/us/data.bin').read_bytes()
    require(sha(asset)=='f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8','asset drift')
    require(int.from_bytes(asset[0x3850:0x3854],'big')==0xB5C534,'loader pointer drift')
    image=zlib.decompress(asset[0xB5C534-0x283D0:],-15)
    require(len(image)==194128 and sha(image)=='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667','image drift')
    source=(group/'group.c').read_text();config_raw=(group/'group.json').read_bytes();config=json.loads(config_raw)
    require(config['keep']==['func_80394058'] and config['flags']=='-g0 -O3 -mips2 -G 0 -non_shared','unexpected recipe')
    archived=ROOT/'cloud/work/frontier/dot_runtime_a_availability_wave_20261011'
    require(function(source,'func_80392F28')==function((archived/'selector.c').read_text(),'func_80392F28'),'leaf body changed')
    require(function(source,'func_803930B8')==function((archived/'producer.c').read_text(),'func_803930B8'),'producer body changed')
    require(function(source,'func_803931C0')==function((archived/'mode.c').read_text(),'func_803931C0'),'mode body changed')
    with tempfile.TemporaryDirectory(prefix='availability-review-') as td:
        temp=Path(td);obj=temp/'group.o'
        # Snapshot the reviewed inputs before compiling; worker may continue elsewhere.
        (temp/'group.c').write_text(source);(temp/'group.json').write_text(json.dumps(config))
        score.compile_group(temp,obj)
        linked,relocations=gnu_link(score,obj,temp)
        scores={};bindings={}
        for name,size in SIZES.items():
            extent=next(x for x in extents['functions'] if x['name']==name)
            require(extent['size']==size and extent['address']=='0x'+name[-8:],'extent drift')
            require(len(targets[name])*4==size and linked[name]==targets[name],'full linked body differs '+name)
            result=score.compare(obj,name,show=0)
            require(result.accepted() and result.extra_words==0,'strict score failed '+name)
            scores[name]=dataclasses.asdict(result)
            bindings[name]=dict(bytes=size,sha256=sha(packed(linked[name])),relocations=relocations[name],evidence=extent['evidence'])
        callsites=audit_calls(obj)
        family_native={int(n[-8:],16):targets[n] for n in SIZES}
        family_linked={int(n[-8:],16):linked[n] for n in SIZES}
        coverage=set();branches=set();counts=[0,0,0];host_cases=[]
        for index,case in enumerate(cases()):
            initial=state(case,index);expected,calls=oracle(initial,case)
            a=native.run(family_native,initial,ENTRIES[case['entry']],case['argument'],case['helpers'],index)
            b=native.run(family_linked,initial,ENTRIES[case['entry']],case['argument'],case['helpers'],index)
            require(a==b,('route trace differs',index))
            require(a['calls']==calls,('helper call sequence differs',index))
            require(all(value==a['memory'][address] for address,value in expected.items() if not native.SP-64<=address<native.SP),('memory/oracle mismatch',index))
            allowed=set(range(native.SP-48,native.SP))
            if case['entry']==0 or (case['entry']==2 and case['argument']==1):
                allowed|=set(range(0x803BA880,0x803BA884))|set(range(0x803BA7FC,0x803BA800))|{0x803BA860}
            if case['entry']==1 or (case['entry']==2 and case['argument']==0):
                allowed|=set(range(0x803BA830,0x803BA840))
            if case['entry']==2:
                allowed|=set(range(0x803BA804,0x803BA808))|set(range(0x803BA7D4,0x803BA7D8))
                if case['argument']==0:
                    allowed|=set(FLAG_CLEAR)|{address+j for address in (0x803BA7E0,0x803BA7F0) for j in range(11)}
                elif case['argument']==1 and not case['pointer'] and case['level']>=29:
                    allowed|={0x803BA858}|set(range(0x803BA818+4*case['group'],0x803BA81C+4*case['group']))
            require(all(all(address+j in allowed for j in range(size)) for address,size,_ in a['writes']), 'unexpected store address')
            require(a['registers'][29]==native.SP and a['registers'][31]==native.RETURN,'stack/return corruption')
            if case['entry']==0:
                require(all(a['registers'][r]==a['initial_registers'][r] for r in (*range(6,14),*range(16,24),26,27,28,30)),'leaf preserved register corruption')
            coverage.update(a['coverage']);branches.update(a['branches']);counts[case['entry']]+=1
            host_cases.append((case,snapshot(initial),snapshot(expected)))
        covered={n:sum(int(n[-8:],16)<=p<int(n[-8:],16)+size for p in coverage) for n,size in SIZES.items()}
        # The producer contains one redundant reload unreachable with its branch-likely schedule.
        require(covered['func_80392F28']==55,'leaf coverage incomplete')
        require(covered['func_803930B8']==65,'producer reachable coverage changed')
        require(covered['func_803931C0']==69,'dispatcher coverage incomplete')
        negatives=[]
        for label,words in [('unknown_opcode',[0xFC000000]),('escaped_code',[0])]:
            try:native.run({ENTRIES[0]:words},state(host_cases[0][0],0),ENTRIES[0],0,[[0]*4]*3)
            except ValueError:negatives.append(label)
        require(len(negatives)==2,'decoder failed open')
        baseline=temp/'standalone.o';score.compile_single(archived/'selector.c','-g0 -O2 -mips2 -G 0 -non_shared',baseline)
        old=score.compare(baseline,'func_80392F28',show=0)
        require(old.differing==2 and old.total==55 and not old.accepted(),'standalone control changed')
        compiled_controls={}
        for label,before,after,target in (
                ('wrong_level_boundary','if (level < 12)','if (level < 13)','func_80392F28'),
                ('wrong_cleanup_count','i < 23','i < 22','func_803932E4')):
            require(source.count(before)==1,'ambiguous negative control')
            changed=source.replace(before,after)
            (temp/'group.c').write_text(changed)
            other=temp/(label+'.o');score.compile_group(temp,other)
            result=score.compare(other,target,show=0)
            require(not result.accepted(),'negative source control accepted')
            compiled_controls[label]=dataclasses.asdict(result)
        host_count=host_verify(source,host_cases,temp)
        cleanup=cleanup_verify(source,family_native,family_linked,temp)
    return dict(status='PASS',source_sha256=sha(source.encode()),group_sha256=sha(config_raw),
                reviewed_functions=list(SIZES),new_candidate_bytes=928,accepted_bytes=0,
                scores=scores,bindings=bindings,callsites=callsites,
                source_bodies_unchanged_from_archived=True,
                semantics=dict(cases_per_route=dict(zip(('selector','producer','dispatcher_family'),counts)),
                               host_C89_UBSan_bounds_cases=host_count,complete_trace_equality=True,
                               instruction_coverage=covered,branch_outcomes=len(branches),
                               guards_and_helper_order=True,negative_decoder_checks=negatives),
                cleanup_semantics=cleanup,
                standalone_negative_control=dataclasses.asdict(old),
                compiled_negative_controls=compiled_controls,
                protected_manifest=manifest,
                support_sha256={str(p.relative_to(ROOT)):sha(p.read_bytes()) for p in (HERE/'native.py',HERE/'elf_reader.py',ROOT/'tools/cloud/score.py')},
                elapsed_seconds=round(time.monotonic()-start,3),
                limits=['No complete 94058/9338C execution or original translation-unit layout claim.',
                        'External helpers are bounded outcome fixtures; their implementations are not verified here.',
                        '930B8 and mode-zero 931C0 intentionally clobber s0-s7/fp under private whole-group ABI.',
                        'List cases assume finite valid nodes and nonoverflowing counts; menu rows contain a reachable nonzero item.',
                        'GNU route links complete extracted functions, not a contiguous original image.',
                        'No accepted-image, cartridge, compression, hardware or gameplay claim.'])


def host_verify(source, cases, temp):
    declarations='''#include <stdio.h>
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
typedef struct { s8 enabled; unsigned char remaining[15]; } PlayerSlot;
typedef struct { unsigned char prefix[16]; unsigned char slot; } ListOwner;
typedef struct { ListOwner *owner; } ListObject;
typedef struct ListNode ListNode;
typedef struct { ListNode *next; s32 unknown; ListObject *object; } ListPayload;
struct ListNode { ListPayload *payload; };
typedef struct { s32 prefix[2]; ListNode *head; } ListRoot;
PlayerSlot D_80156CF0[4]; ListRoot D_8012E6D8;
s8 D_803BA858,D_803BA860,D_803BA7FC[2],D_803B65A4;
u8 D_803BA7FE[2],D_803BA7E0[11],D_803BA7F0[11];
s32 D_803BA880,D_803BA830[4],D_803BA804,D_803BA818[6],D_803BA7D4;
ListNode *D_803BA800;
s8 D_803BA84D,D_803BA854,D_803BA850,D_803BA80C,D_803B6554[6][13];
static int helpers[3][4];
s32 func_800A1A3C(s32 i) { return helpers[0][i]; }
s32 func_800A35F8(s32 i) { return helpers[1][i]; }
s32 func_80094FC4(s32 i) { return helpers[2][i]; }
'''
    body='\n'.join(function(source,n) for n in ('func_80392F28','func_80393004','func_803930B8','func_803931C0'))
    driver='''
static ListNode nodes[24]; static ListPayload payloads[24];
static ListObject objects[24]; static ListOwner owners[24];
int main(void) {
    int entry,mode,level,sublevel,group,pointer,player,i,j,n,v;
    while (scanf("%d%d%d%d%d%d%d",&entry,&mode,&level,&sublevel,&group,&pointer,&player)==7) {
        D_803BA858=(s8)level; D_803BA860=(s8)sublevel; D_803BA880=group;
        D_803BA800=pointer ? nodes : 0; D_803B65A4=(s8)player;
        for(i=0;i<6;i++)scanf("%d",&D_803BA818[i]);
        for(i=0;i<6;i++){scanf("%d",&v); for(j=0;j<13;j++)D_803B6554[i][j]=(v>>j&1)?-128:0;}
        for(i=0;i<4;i++){scanf("%d",&v); D_80156CF0[i].enabled=(s8)v;}
        for(i=0;i<3;i++)for(j=0;j<4;j++)scanf("%d",&helpers[i][j]);
        scanf("%d",&n); for(i=0;i<n;i++){
            scanf("%d",&v); nodes[i].payload=&payloads[i]; payloads[i].next=i+1<n?&nodes[i+1]:0;
            payloads[i].object=&objects[i]; objects[i].owner=&owners[i]; owners[i].slot=(u8)v;
        }
        D_8012E6D8.head=n?nodes:0;
        scanf("%d%d",&D_803BA804,&D_803BA7D4);
        for(i=0;i<2;i++){scanf("%d",&v);D_803BA7FC[i]=(s8)v;}
        for(i=0;i<2;i++){scanf("%d",&v);D_803BA7FE[i]=(u8)v;}
        for(i=0;i<4;i++)scanf("%d",&D_803BA830[i]);
        scanf("%d",&v);D_803BA84D=(s8)v;scanf("%d",&v);D_803BA854=(s8)v;
        scanf("%d",&v);D_803BA850=(s8)v;scanf("%d",&v);D_803BA80C=(s8)v;
        for(i=0;i<11;i++){scanf("%d",&v);D_803BA7E0[i]=(u8)v;}
        for(i=0;i<11;i++){scanf("%d",&v);D_803BA7F0[i]=(u8)v;}
        if(entry==0)func_80392F28(); else if(entry==1)func_803930B8(); else func_803931C0(mode);
        printf("%d %u %u %d %d",D_803BA880,(u8)D_803BA860,(u8)D_803BA858,D_803BA804,D_803BA7D4);
        for(i=0;i<2;i++)printf(" %u",(u8)D_803BA7FC[i]);for(i=0;i<2;i++)printf(" %u",D_803BA7FE[i]);
        for(i=0;i<4;i++)printf(" %d",D_803BA830[i]);for(i=0;i<6;i++)printf(" %d",D_803BA818[i]);
        printf(" %u %u %u %u",(u8)D_803BA84D,(u8)D_803BA854,(u8)D_803BA850,(u8)D_803BA80C);
        for(i=0;i<11;i++)printf(" %u",D_803BA7E0[i]);for(i=0;i<11;i++)printf(" %u",D_803BA7F0[i]);
        printf("\\n");
    }
    return 0;
}
'''
    path=temp/'host.c';path.write_text(declarations+body+driver)
    binary=temp/'host'
    shell('gcc','-std=c89','-pedantic-errors','-O2','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',path,'-o',binary)
    inputs=[]
    for case,current,expected in cases:
        row=[case[k] for k in ('entry','argument','level','sublevel','group','pointer','player')]
        row+=case['cursors']+case['masks']+case['enabled']+sum(case['helpers'],[])+[len(case['nodes'])]+case['nodes']
        row+=current[3:13]+current[19:]
        inputs.append(' '.join(map(str,row)))
    output=shell(binary,input='\n'.join(inputs)+'\n')
    rows=[list(map(int,line.split())) for line in output.splitlines()]
    require(len(rows)==len(cases),'host case count')
    for i,(row,(_,_,expected)) in enumerate(zip(rows,cases)):
        require(row==expected,('host differential mismatch',i,row,expected))
    return len(cases)


def cleanup_verify(source, protected, linked, temp):
    entry=0x803932E4
    sentinel_values=(-2147483648,-65537,-32769,-32768,-1,0,1,32767,32768,65535,65536,2147483647)
    cases=[]
    for pointers in itertools.product((0,1),repeat=2):
        cases.append(([-1]*23,pointers))
        for index in range(23):
            for value in sentinel_values:
                values=[-1]*23;values[index]=value;cases.append((values,pointers))
    rng=random.Random(0x932E4)
    for i in range(256):
        cases.append(([signed(rng.getrandbits(32)) if j%4 else -1 for j in range(23)],(i&1,i>>1&1)))
    coverage=set();branches=set();inputs=[];expected_rows=[]
    for index,(values,pointers) in enumerate(cases):
        memory={a:((a*53+index*29)^0xC3)&255 for lo,hi in ((0x803B5F20,0x803B6540),
                (0x803BA7C8,0x803BA7D8),(native.SP-40,native.SP+16)) for a in range(lo,hi)}
        for i,value in enumerate(values):write(memory,0x803B5F2C+64*i,4,value)
        write(memory,0x803B6530,4,0x80701000 if pointers[0] else 0)
        write(memory,0x803B652C,4,0x80702000 if pointers[1] else 0)
        expected=dict(memory);calls=[]
        for i,value in enumerate(values):
            if value!=-1:
                calls.append((native.CLEANUP_HELPERS[0],signed(value,16)&0xFFFFFFFF))
                write(expected,0x803B5F2C+64*i,4,-1)
        write(expected,0x803BA7D0,4,0)
        for flag,address,dest in ((pointers[0],0x803B6530,native.CLEANUP_HELPERS[1]),
                                  (pointers[1],0x803B652C,native.CLEANUP_HELPERS[2])):
            if flag:
                calls.append((dest,read(memory,address,4)))
                write(expected,address,4,0)
        calls.append((native.CLEANUP_HELPERS[3],None));write(expected,0x803B6528,1,0)
        a=native.run(protected,memory,entry,0,[],index)
        b=native.run(linked,memory,entry,0,[],index)
        require(a==b and a['calls']==calls,('cleanup trace/call mismatch',index))
        require(all(a['memory'][address]==value for address,value in expected.items()
                    if not native.SP-24<=address<native.SP),('cleanup memory/guard mismatch',index))
        require(all(a['registers'][r]==a['initial_registers'][r] for r in (*range(19,24),26,27,28,29,30,31)),
                'cleanup preserved-register corruption')
        allowed=set(range(native.SP-24,native.SP))|{0x803B6528}|set(range(0x803B652C,0x803B6534))|set(range(0x803BA7D0,0x803BA7D4))
        allowed|={0x803B5F2C+64*i+j for i in range(23) if values[i]!=-1 for j in range(4)}
        require(all(all(address+j in allowed for j in range(size)) for address,size,_ in a['writes']),'cleanup unexpected write')
        coverage.update(a['coverage']);branches.update(a['branches'])
        inputs.append(' '.join(map(str,list(values)+list(pointers))))
        expected_rows.append([len(calls)]+[signed(value) if value is not None else 0 for _,value in calls])
    require(sum(entry<=pc<entry+168 for pc in coverage)==42,'cleanup coverage incomplete')
    declarations='''#include <stdio.h>
typedef short s16; typedef int s32; typedef signed char s8;
typedef struct { s32 fields[16]; } MenuRecord;
MenuRecord D_803B5F28[23]; s32 D_803BA7D0;
void *D_803B6530,*D_803B652C; s8 D_803B6528;
static int a,b,events[26],count;
void sound_call_minimal(s16 value) { events[count++]=value; }
void sound_handles_array_clear(void *p) { if(p!=&a)count=99; else events[count++]=0x80701000; }
void sound_stop(void *p) { if(p!=&b)count=99; else events[count++]=0x80702000; }
void ambient_sounds_clear(void) { events[count++]=0; }
'''
    driver='''
int main(void) {
    int i,j,x,y;
    while(scanf("%d",&D_803B5F28[0].fields[1])==1) {
        for(i=1;i<23;i++)scanf("%d",&D_803B5F28[i].fields[1]);
        scanf("%d%d",&x,&y);D_803B6530=x?&a:0;D_803B652C=y?&b:0;
        D_803BA7D0=123;D_803B6528=97;count=0;
        for(i=0;i<23;i++)for(j=0;j<16;j++)if(j!=1)D_803B5F28[i].fields[j]=i*16+j;
        func_803932E4();
        if(D_803BA7D0 || D_803B6528 || D_803B6530 || D_803B652C)return 3;
        for(i=0;i<23;i++)for(j=0;j<16;j++)if(D_803B5F28[i].fields[j]!=(j==1?-1:i*16+j))return 4;
        printf("%d",count);for(i=0;i<count;i++)printf(" %d",events[i]);printf("\\n");
    }
    return 0;
}
'''
    path=temp/'cleanup-host.c';path.write_text(declarations+function(source,'func_803932E4')+driver)
    binary=temp/'cleanup-host'
    shell('gcc','-std=c89','-pedantic-errors','-O2','-fsanitize=undefined,bounds','-fno-sanitize-recover=all',path,'-o',binary)
    rows=[list(map(int,line.split())) for line in shell(binary,input='\n'.join(inputs)+'\n').splitlines()]
    require(rows==expected_rows,'cleanup host differential mismatch')
    return dict(cases_per_route=len(cases),host_C89_UBSan_bounds_cases=len(cases),
                complete_trace_equality=True,instructions_covered=42,branch_outcomes=len(branches),
                signed16_narrowing_and_sentinel_distinguished=True,
                all_23_records_and_surrounding_guards=True,pointer_gates_and_helper_order=True,
                private_clobbers=['s0','s1','s2'],preserved_saved_registers=['s3','s4','s5','s6','s7','fp','sp','ra'])


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--group',type=Path,default=GROUP)
    parser.add_argument('--output',type=Path)
    args=parser.parse_args()
    receipt=verify(args.group)
    if args.output:args.output.write_text(json.dumps(receipt,indent=2)+'\n')
    print(json.dumps(receipt,indent=2))
