#!/usr/bin/env python3
"""Strict B-image texture-ring initializer proof; no binary artifacts published."""
import contextlib
import hashlib
import io
import json
import os
import shutil
from pathlib import Path
import struct
import subprocess
import sys
import tempfile

PACKET = Path(__file__).resolve().parent
PROJECT = PACKET.parents[2]
ROOT = Path(os.environ.get('RUSH_REPO', str(PROJECT))).resolve()
sys.path.insert(0, str(ROOT))
from tools.cloud import score
SOURCE = PROJECT / 'cloud/matches/ovl_b/func_8038A8CC.c'
NAME, ADDRESS, SIZE = 'func_8038A8CC', 0x8038A8CC, 144
BASE = 'cd22879d40b3de443cfde047b86e75e159b6cec6'
IMAGE_SHA = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
GLOBALS = {'D_80399A70':0x80399A70, 'D_80394D14':0x80394D14,
           'D_80399AD8':0x80399AD8, 'D_80140BDC':0x80140BDC,
           'func_800B24EC':0x800B24EC}
RING, TEXTURE, COUNT = 0x80399A70, 0x80399AD8, 0x80140BDC
U32, STACK, RETURN = 0xffffffff, 0x70000000, 0x60000000


def portable_receipt(receipt):
    """Compare packet proof; base-context and whole-tree digests are provenance."""
    result = json.loads(json.dumps(receipt))
    for path in (
        'asm/us/ovl_b/SHA256SUMS',
        'asm/us/ovl_b/extents.json',
        'asm/us/ovl_b/ovl_b_8038a400.s',
        'asm/us/ovl_b/symbols.json',
        'tools/cloud/owndata.py',
        'tools/cloud/score.py',
    ):
        result.get('inputs_sha256', {}).pop(path, None)
    result.get('accepted_helper', {}).pop('sha256', None)
    result.get('packet_sha256', {}).pop('test_packet.py', None)
    if not result.get('inputs_sha256'):
        result.pop('inputs_sha256', None)
    return result


def sha_bytes(b): return hashlib.sha256(b).hexdigest()
def sha(p): return sha_bytes(Path(p).read_bytes())
def signed(n,bits):
    n &= (1<<bits)-1
    return n-(1<<bits) if n>>(bits-1) else n

def run(args,**kw):
    p=subprocess.run([str(a) for a in args],capture_output=True,text=True,**kw)
    assert p.returncode==0,(args,p.returncode,p.stdout,p.stderr)
    return p

def pinned(path):
    return subprocess.check_output(['git','-C',str(ROOT),'show',BASE+':'+path])

def put(mem,a,v,n):
    assert a % n == 0
    for j in range(n):
        assert a+j in mem,('unmapped write',hex(a+j))
        mem[a+j]=(v>>(8*(n-j-1)))&255

def get(mem,a,n):
    assert a % n == 0
    assert all(a+j in mem for j in range(n)),('unmapped read',hex(a))
    return int.from_bytes(bytes(mem[a+j] for j in range(n)),'big')

def fixture(count,variant):
    mem={}
    for base,length in [(RING-8,120),(TEXTURE-8,20),(COUNT-4,12),(STACK-48,64)]:
        for i in range(length):mem[base+i]=((i*71+variant*19+count*3)^0xa5)&255
    mem[COUNT]=count
    return mem

def effect(mem,count,variant):
    mem[RING]=(variant-8)&255
    mem[RING+1]=(variant*17+3)&255
    put(mem,RING+2,variant*4097,2)
    for i in range(25): put(mem,RING+4+i*4,(0x81000000+variant*256+i*4)&U32,4)
    put(mem,TEXTURE,variant*4099,2)
    mem[COUNT]=(count+113)&255

def expected(count,variant):
    mem=fixture(count,variant)
    mem[RING]=0;put(mem,RING+2,0,2)
    snapshot={a:v for a,v in mem.items() if not STACK-48 <= a < STACK+16}
    assert len(snapshot)==136
    effect(mem,count,variant)
    for i in range(25):put(mem,RING+4+i*4,0,4)
    return {a:v for a,v in mem.items() if not STACK-48 <= a < STACK+16},snapshot

def execute(words,count,variant):
    assert len(words)*4==SIZE
    mem=fixture(count,variant)
    regs=[((i+1)*0x01020304 ^ (variant*0x10205+count))&U32 for i in range(32)]
    regs[0]=0;regs[29]=STACK;regs[31]=RETURN
    initial=regs[:];pc=ADDRESS;pending=None;covered=set();reads=[];writes=[];calls=[]
    for steps in range(300):
        assert ADDRESS<=pc<ADDRESS+SIZE and pc%4==0,('bad pc',hex(pc))
        old_pending=pending;pending=None
        w=words[(pc-ADDRESS)//4];covered.add(pc-ADDRESS)
        op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sh=(w>>6)&31;fn=w&63
        imm=signed(w,16);a=(regs[rs]+imm)&U32
        if op==0:
            if fn==0:regs[rd]=(regs[rt]<<sh)&U32
            elif fn==3:regs[rd]=(signed(regs[rt],32)>>sh)&U32
            elif fn==0x25:regs[rd]=regs[rs]|regs[rt]
            elif fn==8:
                assert old_pending is None and rs==31
                pending=('jump',regs[rs])
            else:raise AssertionError(('unknown SPECIAL',fn))
        elif op==9:regs[rt]=a
        elif op==15:regs[rt]=(w&65535)<<16
        elif op in (0x24,0x23):
            n=1 if op==0x24 else 4; regs[rt]=get(mem,a,n);reads.append((a,n))
        elif op in (0x28,0x29,0x2b):
            n={0x28:1,0x29:2,0x2b:4}[op];put(mem,a,regs[rt],n);writes.append((a,regs[rt]&((1<<(8*n))-1),n))
        elif op==5:
            assert old_pending is None
            pending=('jump',(pc+4+imm*4)&U32 if regs[rs]!=regs[rt] else pc+8)
        elif op==3:
            assert old_pending is None
            regs[31]=pc+8
            pending=('call',((pc+4)&0xf0000000)|((w&0x3ffffff)<<2))
        else:raise AssertionError(('unknown opcode',op))
        regs[0]=0
        pc+=4
        if old_pending:
            assert pending is None, 'branch in delay slot'
            kind,target=old_pending
            if kind=='call':
                assert target==GLOBALS['func_800B24EC']
                args=regs[4:8]+[get(mem,regs[29]+16,4)]
                want=[GLOBALS['D_80394D14'],TEXTURE,0,signed(count-1,8)&U32,1]
                assert args==want,(args,want)
                snap={a:v for a,v in mem.items() if not STACK-48 <= a < STACK+16}
                assert len(snap)==136
                assert snap==expected(count,variant)[1]
                calls.append({'target':target,'args':args})
                effect(mem,count,variant)
                pc=regs[31]
                # Genuine external boundary may clobber every caller-save GPR.
                for r in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25]:
                    regs[r]=(0xf0000000+r*0x12345+variant)&U32
            elif target==RETURN:
                break
            else:pc=target
    else:raise AssertionError('did not return')
    final={a:v for a,v in mem.items() if not STACK-48 <= a < STACK+16}
    assert len(final)==136
    assert all(a in final for a in (RING,RING+1,RING+2,RING+103,TEXTURE,COUNT))
    assert final==expected(count,variant)[0]
    assert len(calls)==1
    assert reads.count((COUNT,1))==1
    assert set(reads)=={(COUNT,1),(STACK-4,4)}
    assert writes[:3]==[(STACK-4,RETURN,4),(RING,0,1),(RING+2,0,2)]
    assert writes[3]==(STACK-16,1,4)
    assert sorted(writes[4:])==[(RING+4+i*4,0,4) for i in range(25)]
    assert len(writes)==29
    for r in list(range(16,24))+[28,29,30,31]:assert regs[r]==initial[r]
    for a in range(STACK-48,STACK+16):
        if not STACK-32<=a<STACK:assert mem[a]==fixture(count,variant)[a]
    return covered

def inspect(obj):
    data,sections=score._elf(obj)
    symbols=[s for i,t in enumerate(sections) if t['type']==2 for s in score._symbol_table(data,sections,i)]
    fn=next(s for s in symbols if s['name']==NAME and s['type']==2)
    assert fn['size']==SIZE
    shoff=struct.unpack_from('>I',data,0x20)[0];entsize=struct.unpack_from('>H',data,0x2e)[0]
    for i,t in enumerate(sections):
        flags=struct.unpack_from('>I',data,shoff+i*entsize+8)[0]
        assert not (flags&2 and t['size'] and t['name'] not in ('.text','.reginfo'))
    text=sections[score._text_index(sections)]
    raw=data[text['off']:text['off']+text['size']]
    assert len(raw)==SIZE
    return data,sections,fn,raw

def native_link_script():
    # Both output placement and input subalignment are explicit: ADDRESS is
    # word-aligned but GNU 2.42 otherwise rounds this section up to 16 bytes.
    return ('SECTIONS { .text 0x%08X : SUBALIGN(4) { *(.text) } '
            '/DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n' % ADDRESS +
            '\n'.join('%s = 0x%08x;' % (k, v) for k, v in GLOBALS.items()))


def prove():
    score.ASM_DIR=ROOT/'asm/us/ovl_b'
    native=score.targets()[NAME]
    manifests=score.target_manifest()
    extents=json.loads(score.verified_bytes(score.ASM_DIR/'extents.json',manifests))
    entry=next(f for f in extents['functions'] if f['name']==NAME)
    consumer_entry=next(f for f in extents['functions'] if f['name']=='func_8038A408')
    assert consumer_entry['address']=='0x8038A408' and consumer_entry['size']==1220
    assert (extents['image'],extents['image_sha256'],entry['address'],entry['size'])==('B',IMAGE_SHA,'0x8038A8CC',SIZE)
    addresses=score.image_symbols()
    for key,value in dict(GLOBALS,**{NAME:ADDRESS}).items():assert addresses.get(key,score.address_named(key))==value
    accepted=pinned('src/blob/func_800B24EC.c')
    assert sha_bytes(accepted)=='405f7b1f5c31e4e4c19b82d745a778e85c2046903cea6d439d5d0564e925712b'
    lock=json.loads(pinned('blob_matched.lock.json'))["func_800B24EC"]
    assert lock['source_sha256']==sha_bytes(accepted) and lock['verified']=='image_gate'
    assert b'NameEntry *func_800B24EC(char *name, s16 *out, s8 lo, s8 hi, s32 err)' in accepted
    receipt={'status':'MATCH','base':BASE,'image':'B','image_sha256':IMAGE_SHA,'address':hex(ADDRESS),'bytes':SIZE,
             'source_sha256':sha(SOURCE),'flags':FLAGS+' -Wab,-r4300_mul',

             'packet_sha256':{p.name:sha(p) for p in [PACKET/'verify.py',PACKET/'host_test.c']},
             'accepted_helper':{'path':'src/blob/func_800B24EC.c','commit':BASE,'compiled_or_executed_here':False},
             'consumer':{'name':'func_8038A408','address':'0x8038A408','bytes':1220,'sha256':sha_bytes(struct.pack('>305I',*score.targets()['func_8038A408']))}}
    with tempfile.TemporaryDirectory(prefix='b-a8cc-') as name:
        tmp=Path(name);obj=tmp/'candidate.o'
        score.compile_single(SOURCE,FLAGS,obj)
        with contextlib.redirect_stdout(io.StringIO()):result=score.compare(obj,NAME)
        assert result.accepted(),vars(result)
        data,sections,fn,raw=inspect(obj);assert fn['value']==0
        words=score.text_words(obj)
        relocated,masks,unresolved,unverified,errors=score.relocate(obj,words,0,SIZE,addresses)
        assert not any((masks,unresolved,unverified,errors));assert relocated==native
        relocs=[]
        for t in sections:
            if t['type']!=9:continue
            assert t['info']==score._text_index(sections)
            syms=score._symbol_table(data,sections,t['link'])
            for at in range(t['off'],t['off']+t['size'],8):
                off,info=struct.unpack_from('>II',data,at)
                relocs.append({'offset':off,'type':info&255,'symbol':syms[info>>8]['name']})
        assert {r['symbol'] for r in relocs}==set(GLOBALS)
        assert sorted((r['offset'],r['type'],r['symbol']) for r in relocs)==[
            (0,5,'D_80399A70'),(4,6,'D_80399A70'),(12,5,'D_80140BDC'),(28,6,'D_80140BDC'),
            (36,5,'D_80394D14'),(40,5,'D_80399AD8'),(64,6,'D_80399AD8'),(68,6,'D_80394D14'),
            (72,4,'func_800B24EC'),(80,5,'D_80399A70'),(84,5,'D_80399A70'),(88,5,'D_80399A70'),
            (92,6,'D_80399A70'),(96,6,'D_80399A70'),(100,6,'D_80399A70')]
        assert all(0<=r['offset']<SIZE and r['type'] in [4,5,6] for r in relocs)
        script=tmp/'native.ld'
        script.write_text(native_link_script())
        linked=tmp/'linked.elf';run(['mips-linux-gnu-ld','-EB','-T',script,'-o',linked,obj])
        linkeddata,linkedsecs,linkedfn,linkedraw=inspect(linked)
        shoff=struct.unpack_from('>I',linkeddata,0x20)[0];entsize=struct.unpack_from('>H',linkeddata,0x2e)[0]
        textaddr=struct.unpack_from('>I',linkeddata,shoff+score._text_index(linkedsecs)*entsize+12)[0]
        assert textaddr==ADDRESS and linkedfn['value']==ADDRESS
        linkedwords=list(struct.unpack('>36I',linkedraw));assert linkedwords==native
        readelf=run(['mips-linux-gnu-readelf','-sW',linked]).stdout
        assert any(NAME==line.split()[-1] and line.split()[1:3]==['8038a8cc','144'] for line in readelf.splitlines() if line.split())
        layout=tmp/'layout.c'
        layout.write_text('#define offsetof(t,m) ((unsigned int)&((t *)0)->m)\n#include "'+str(SOURCE)+'"\n'+
                          'typedef char ring_size[(sizeof(RingState)==104)?1:-1];\n'+
                          'typedef char ring_next[(offsetof(RingState,next)==2)?1:-1];\n'+
                          'typedef char ring_objects[(offsetof(RingState,objects)==4)?1:-1];\n'+
                          'typedef char pointer_size[(sizeof(void*)==4)?1:-1];\n')
        score.compile_single(layout,FLAGS,tmp/'layout.o')
        assert inspect(tmp/'layout.o')[3]==raw
        coverage=set()
        for count in range(256):
            for variant in range(16):
                for body in [native,relocated,linkedwords]:coverage |= execute(body,count,variant)
        assert coverage==set(range(0,SIZE,4))
        bad=native[:];bad[0]=0xffffffff
        try:execute(bad,0,0)
        except AssertionError:pass
        else:raise AssertionError('unknown instruction accepted')
        common=['gcc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-fsanitize=undefined','-fno-sanitize-recover=all']
        exe=tmp/'host';run(common+['-DCANDIDATE_PATH="'+str(SOURCE)+'"',PACKET/'host_test.c','-o',exe])
        host=run([exe]);assert host.stdout.strip()=='4096 complete-state host cases passed' and not host.stderr
        mutants={'omit_last_slot':('i < 25','i < 24'),
                 'wrong_high_bound':('D_80140BDC - 1','D_80140BDC'),
                 'wrong_error_policy':('D_80140BDC - 1, 1','D_80140BDC - 1, 0'),
                 'clobber_padding':('D_80399A70.next = 0;','D_80399A70.next = 0; D_80399A70.unknown01 = 0;'),
                 'clear_after_callback':('D_80399A70.objects[i] = 0;','D_80399A70.objects[i] = 0; D_80399A70.next = 0;')}
        rejected=[];source=SOURCE.read_text()
        for name,(old,new) in mutants.items():
            assert source.count(old)==1
            path=tmp/(name+'.c');path.write_text(source.replace(old,new))
            run(common+['-DCANDIDATE_PATH="'+str(path)+'"',PACKET/'host_test.c','-o',exe])
            adverse=subprocess.run([str(exe)],capture_output=True,text=True)
            assert adverse.returncode==1 and 'mismatch' in adverse.stderr
            score.compile_single(path,FLAGS,obj)
            with contextlib.redirect_stdout(io.StringIO()):mr=score.compare(obj,NAME)
            assert not mr.accepted();rejected.append(name)
        receipt['elf']={'function_size':SIZE,'text_size':SIZE,'alignment_bytes_outside_function':0,'owned_data_bytes':0,
                        'relocations':relocs,'strict_differing_words':0,'gnu_differing_words':0,'body_sha256':sha_bytes(linkedraw),
                        'native_layout_assertions':4}
        receipt['behavior']={'host_cases':4096,'native_project_gnu_cases':4096,'native_executions':12288,'instruction_offsets':len(coverage),
                             'complete_callback_snapshots_and_final_state':True,'canaries_and_saved_registers':True,'count_reads_per_case':1,
                             'source_mutants_rejected':rejected,'unknown_opcode_rejected':True,
                             'scope':'Valid ring with 25 pointer slots; all 256 count bytes; 16 callback effects. Texture helper is an O32 contract hook, not its actual implementation. LP64 hosted layout is not native layout.'}
    return receipt

def tool_provenance():
    names=[('ido_'+name,score.ido(name)) for name in ['cc','cfe','ugen','uopt','as1']]
    names += [(name,shutil.which(name)) for name in ['mips-linux-gnu-ld','mips-linux-gnu-readelf','gcc']]
    return {name:sha(path) for name,path in names}

if __name__=='__main__':
    import argparse
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('output',nargs='?');parser.add_argument('--check',action='store_true');parser.add_argument('--tool-provenance',type=Path)
    args=parser.parse_args();receipt=prove();text=json.dumps(receipt,indent=2,sort_keys=True)+'\n'
    if args.tool_provenance:args.tool_provenance.write_text(json.dumps(tool_provenance(),indent=2,sort_keys=True)+'\n')
    if args.check:assert portable_receipt(receipt)==portable_receipt(json.loads((PACKET/'verification.json').read_text()));print('Frozen source-bound proof reproduced')
    elif args.output:Path(args.output).write_text(text)
    else:print(text,end='')
