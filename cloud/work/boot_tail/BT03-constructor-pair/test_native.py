#!/usr/bin/env python3
"""Independent oracle against canonical native and full compiled candidate bodies.

Synthetic valid records only; helper behavior is modeled, not integrated game
execution. The native body has harmless speculative uninitialized stack loads;
two poison fills verify that these never influence calls, results, or memory.
"""
import hashlib,json,random,sys,tempfile
from pathlib import Path
from native_replay import execute,signed
P=Path(__file__).resolve().parent;ROOT=P.parents[3];sys.path.insert(0,str(ROOT))
from tools.cloud import score
MASK=0xFFFFFFFF;LAYERS=0x100000;KEYS=0x200000;VOICES=0x8004BEB8
ARITIES={0x80016F80:2,0x80016EE0:1,0x80019ED0:3,0x80024988:11,0x8001ECE0:1,0x80019F48:10}
def put(d,o,v,n=4):d[o:o+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')
def get(d,o,n=4):return int.from_bytes(d[o:o+n],'big')
def clip(x,hi):return max(0,min(hi,x))
def layer(id=1,lo=0,hi=127,transpose=0,volume=127,priority=0,pan=64):
    d=bytearray([0xA5])*12
    for o,v,n in [(0,id,2),(2,lo,1),(3,hi,1),(4,transpose,1),(5,volume,1),(6,priority,2),(8,pan,1)]:put(d,o,v,n)
    return d

def fixture(a,args,records=None,entry=None,rawpan=99,missing=False,ports=(),created=(),roots=()):
    ld=b''.join(records or [layer()]);kd=bytearray([0xA5])*2048
    key=args[2] if a==0x80019F48 else args[1]
    e=entry or (1,0,64,0)
    for o,v,n in [(0,e[0],2),(2,e[1],1),(3,e[2],1),(4,e[3],2)]:put(kd,(key&127)*8+o,v,n)
    if key&128:put(kd,key*8+3,rawpan,1)
    return dict(a=a,args=list(args),regions={LAYERS:bytearray(ld),KEYS:kd,VOICES:bytearray([0x5A])*(416*256)},count=len(ld)//12,missing=missing,ports=list(ports),created=list(created),roots=list(roots))

def behavior(f,call):
    """High-level independent arithmetic/dispatch oracle; helpers control effects."""
    a=f['args'];r={k:bytearray(v) for k,v in f['regions'].items()}
    def read(base,o,n=1):return get(r[base],o,n)
    def create(args):return call(0x80024988,args)
    if f['a']==0x80019F48:
        packed,allocation,key,vol,pan,channel,set_,offset,section,group=a
        call(0x80016F80,[packed>>16,'count'])
        result=MASK;previous=None
        if f['missing']:return result,r
        i=0;n=f['count']
        while i<n:
            o=12*i;i+=1;ident=read(LAYERS,o,2);masked=key%128
            if ident==65535 or not read(LAYERS,o+2)<=masked<=read(LAYERS,o+3):continue
            note=clip(masked+signed(read(LAYERS,o+4),8),127)
            p=call(0x80019ED0,[note,channel,set_])
            if 'count_after_port' in f:n=f['count_after_port']
            if p!=MASK:return p,r
            p=128 if read(LAYERS,o+8)&128 else clip(read(LAYERS,o+8)-64+pan,127)
            v=(vol*read(LAYERS,o+5)//127)%256
            priority=clip((packed>>8)%256+signed(read(LAYERS,o+6,2),16),255)
            data=(ident<<16)+(priority<<8)+(packed%256)
            if ident&0xC000:continue
            c=create([data,allocation,note|(key&128),v,p,channel,set_,offset,section,0,group])
            if result==MASK:
                previous=c
                if c!=MASK:result=call(0x8001ECE0,[VOICES+(c%256)*416])
            elif c!=MASK:
                put(r[VOICES],(previous%256)*416+16,c);put(r[VOICES],(c%256)*416+20,previous);previous=c
        return result,r
    packed,key,vol,pan,channel,set_,offset,section,group,priorityOffset=a
    ident=packed>>16;priority=clip((packed>>8)%256+signed(priorityOffset,16),255)
    data=(packed&0xFFFF00FF)|(priority<<8);kind=ident&0xC000
    if kind==0:
        p=call(0x80019ED0,[key,channel,set_])
        if p!=MASK:return p,r
        return create([data,ident,key,vol,pan,channel,set_,offset,section,1,group]),r
    if kind==0x8000:return call(0x80019F48,[data,ident,key,vol,pan,channel,set_,offset,section,group]),r
    if kind!=0x4000:return MASK,r
    call(0x80016EE0,[ident])
    if f['missing']:return MASK,r
    o=(key%128)*8;new=read(KEYS,o,2)
    if new==65535:return MASK,r
    pan=128 if read(KEYS,o+3)&128 else clip(read(KEYS,key*8+3)-64+pan,127)
    note=clip((key%128)+signed(read(KEYS,o+2),8),127)
    priority=clip(priority+signed(read(KEYS,o+4,2),16),255)
    data=(new<<16)+(priority<<8)+(packed%256)
    if new&0xC000:return call(0x80019F48,[data,ident,note|(key&128),vol,pan,channel,set_,offset,section,group]),r
    p=call(0x80019ED0,[note,channel,set_])
    if p!=MASK:return p,r
    return create([data,ident,note|(key&128),vol,pan,channel,set_,offset,section,1,group]),r

def cases():
    # Exhaust both raw-byte keys and all dispatch category edges.
    for key in range(256):
        for ident in (1,0x4001,0x8001,0xC001):
            args=[(ident<<16)|0x80FE,key,255,64,255,4,65535,0x1234,255,0xFFFF]
            yield 'dispatch',fixture(0x8001A270,args,entry=(1,-1,64,-32768))
    for key in (0,127,128,255):
        for ident in (1,0x4001,0x8001,0xC001,65535):
            for pan in (0,64,127,128,255):
                for delta in (-32768,-256,-1,0,255,32767):
                    args=[0x40017FFE,key,1,127,7,255,65535,0xABCD,255,delta&MASK]
                    yield 'keymap boundaries',fixture(0x8001A270,args,entry=(ident,-128,pan,delta),rawpan=0)
    for missing in (False,True):
        for port in (MASK,0,0x12345678):
            for made in (MASK,0x12340003):
                yield 'failures',fixture(0x8001A270,[0x400180FF,255,127,0,2,3,1,2,3,0],missing=missing,ports=[port],created=[made])
    for key in range(256):
        yield 'layer keys',fixture(0x80019F48,[0x800180FE,65535,key,255,255,255,4,65535,0x1234,255],records=[layer(),layer(2,transpose=-128,volume=255,priority=32767,pan=0),layer(3,transpose=127,priority=-32768,pan=128)])
    for ids in ((MASK,MASK),(MASK,0x12340004),(0x12340003,MASK),(0x12340003,0x56780004)):
        for roots in ((MASK,0x99),(0x98765432,0x99)):
            for ports in ((MASK,MASK),(MASK,0),(0x55,MASK)):
                yield 'allocation and root failures',fixture(0x80019F48,[0x800100FF,7,130,127,64,9,2,3,4,5],records=[layer(1),layer(2)],created=ids,roots=roots,ports=ports)
    for ident in (65535,0x4000,0x8000,0xC000,1):
        for lo,hi in ((0,127),(50,127),(0,49),(50,50)):
            for port in (MASK,0x1234):
                yield 'layer guards',fixture(0x80019F48,[0x8001FF7F,0,50,127,0,0,255,0,255,0],records=[layer(ident,lo,hi)],ports=[port])
    for missing in (False,True):
        f=fixture(0x80019F48,[0x80010000,0,0,0,0,0,0,0,0,0],missing=missing);f['count']=0;yield 'empty lookup',f
    for limit in (0,1,2,3):
        f=fixture(0x80019F48,[0x80010000,7,66,127,64,9,2,3,4,5],records=[layer(1),layer(2),layer(3)]);f['count_after_port']=limit;yield 'count reload after helper',f
    rng=random.Random(0x19F48)
    for _ in range(256):
        args=[0x80010000|rng.randrange(65536),rng.randrange(65536)]+[rng.randrange(256) for _ in range(5)]+[rng.randrange(65536),rng.randrange(65536),rng.randrange(256)]
        ls=[layer(rng.choice([1,2,3,65535,0x4001]),0,127,rng.randrange(-128,128),rng.randrange(256),rng.randrange(-32768,32768),rng.randrange(256)) for i in range(5)]
        yield 'random layers',fixture(0x80019F48,args,records=ls)

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail';code={};hashes={}
    with tempfile.TemporaryDirectory(prefix='constructor-native-') as td:
        for address in (0x80019F48,0x8001A270):
            name='func_%08X'%address;s=P/'nonmatch'/(name+'.c');o=Path(td)/(name+'.o');score.compile_single(s,score.DEFAULT_FLAGS,o)
            d,secs=score._elf(o);sym=[z for i,se in enumerate(secs)if se['type']==2 for z in score._symbol_table(d,secs,i)if z['name']==name][0]
            w=score.text_words(o);cw,m,u,v,e=score.relocate(o,w,0,sym['size'],score.image_symbols());assert not(m or u or v or e)
            code[address]=[score.targets()[name],cw[:sym['size']//4]];hashes[name]=hashlib.sha256(s.read_bytes()).hexdigest()
    count=steps=0;families={};uninit={}
    for label,f in cases():
        def caller():
            cursors={};events=[]
            def call(addr,args):
                args=list(args);events.append((addr,tuple(args)))
                slot=cursors.get(addr,0);cursors[addr]=slot+1
                seq=f['ports'] if addr==0x80019ED0 else f['created'] if addr==0x80024988 else f['roots'] if addr==0x8001ECE0 else []
                default=MASK if addr==0x80019ED0 else (0x12340000|(slot+3)) if addr==0x80024988 else 0x76543210
                return seq[slot] if slot<len(seq) else default
            return call,events
        call,events=caller();expected,regions=behavior(f,call)
        for which,words in enumerate(code[f['a']]):
            for fill in (0xA5,0x5A):
                call2,ev=caller();countptr=[None]
                def helper(addr,args,mem,ordinal):
                    if addr==0x80016F80:
                        call2(addr,[args[0],'count']);countptr[0]=args[1]
                        if not f['missing']:mem(args[1],2,f['count'])
                        return (0 if f['missing'] else LAYERS),ev[-1]
                    if addr==0x80016EE0:
                        call2(addr,args);return (0 if f['missing'] else KEYS),ev[-1]
                    result=call2(addr,args)
                    if addr==0x80019ED0 and 'count_after_port' in f:mem(countptr[0],2,f['count_after_port'])
                    return result,ev[-1]
                actual=execute(words,f['regions'],helper,f['args'],f['a'],ARITIES,stack_fill=fill)
                assert actual['result']==expected,(label,which,'result',actual['result'],expected)
                assert actual['calls']==events,(label,which,'calls',actual['calls'],events)
                assert actual['regions']=={k:bytes(v)for k,v in regions.items()},(label,which,'memory')
                for a,n in actual['uninitialized_stack_reads']:
                    # Only native pre-loop previous ID and null-return count loads are permitted.
                    rel=a-actual['stack_base'];allowed={(-12,4)} if f['a']==0x80019F48 else set()
                    if f['missing']:allowed.add((-2,2))
                    if which==1:allowed={(-16,4)} if f['a']==0x80019F48 else set()
                    assert (rel,n) in allowed,(label,which,'uninit',rel,n)
                    key='%08X:%s:%d:%d'%(f['a'],'native' if which==0 else 'candidate',rel,n);uninit[key]=uninit.get(key,0)+1
                steps+=actual['steps']
        count+=1;families[label]=families.get(label,0)+1
    return dict(result='PASS',cases=count,executions=count*4,steps=steps,families=families,source_sha256=hashes,speculative_stack_loads=uninit,limits='Valid synthetic resources, exact helper contracts, two stack poison fills. No arbitrary malformed-data safety or real helper/game/ROM integration claim.')
if __name__=='__main__':print(json.dumps(run(),indent=2))
