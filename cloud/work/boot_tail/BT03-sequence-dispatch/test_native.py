#!/usr/bin/env python3
"""Compare canonical native words and freshly compiled C on synthetic inputs."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
P=Path(__file__).resolve().parent
ROOT=P.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
from native_replay import execute
STATE,RESULT,GLOBAL=0x100000,0x200000,0x80043EB8
MASK=0xFFFFFFFF

def put(d,o,v,n=4):d[o:o+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')
def get(d,o,n=4):return int.from_bytes(d[o:o+n],'big')
def fixture(flags,tag=0):
    r={STATE:bytearray([0xA5])*40,RESULT:bytearray([0xCC])*4,GLOBAL:bytearray([0x5A])*(4088*8)}
    for o,v,n in [(0,0x1234|tag,4),(4,0xFFFF,2),(8,0x5678|tag,4),(12,0xABCD,2),(16,0x345600,4),(20,0x1234,2),(22,0x5678,2),(24,255,1),(28,0xDEAD1234,4),(32,0x87654321,4),(36,0xFEDC,2),(38,flags,1)]:put(r[STATE],o,v,n)
    return r

def logical(regions,unlocked,result,first_ok=True,next_ok=True,created=0x7654321,mutation=None,state_address=STATE):
    r={a:bytearray(d) for a,d in regions.items()};calls=[]
    def mem(a,n=4,v=None):
        for b,d in r.items():
            if b<=a and a+n<=b+len(d):
                if v is None:return get(d,a-b,n)
                put(d,a-b,v,n);return
        raise AssertionError('reference unmapped')
    def field(o,n=4):return mem(state_address+o,n)
    def call(a,args):
        calls.append((a,tuple(args)))
        if mutation:mutation(a,args,mem,len(calls)-1)
    identifier=field(0);call(0x80017644,[identifier])
    if not first_ok:return r,calls
    flags=field(38,1)
    if flags&4:
        base=GLOBAL+3*4088
        r[GLOBAL][3*4088+0xFC8:3*4088+0xFF0]=bytes(mem(state_address+i,1) for i in range(40))
        mem(base+0xFEE,1,mem(base+0xFEE,1)&~4)
        mem(base+0xFF4,1,1);mem(base+0xFF0,4,result)
        mem(result,4,field(0)|0x80000000)
        return r,calls
    service=0x80019194 if unlocked else 0x80019370
    mode=2 if flags&1 else 3 if flags&64 else 1
    call(service,[0,field(4,2),field(0),mode])
    if not result:return r,calls
    flags=field(38,1)
    if flags&2:
        call(0x80017644,[field(8)])
        if not next_ok:mem(result,4,MASK);return r,calls
        call(0x80018FEC if unlocked else 0x8001906C,[field(8)])
        call(service,[field(24,1),field(12,2),field(8),0])
        if field(38,1)&16:call(0x800190AC if unlocked else 0x80019144,[field(8),field(28),field(32)])
        if field(38,1)&32:call(0x80018F20 if unlocked else 0x80018FA4,[field(8),field(36,2)])
        mem(result,4,field(8));return r,calls
    f=4|(16 if flags&8 else 0)|(2 if flags&32 else 0)|(1 if flags&16 else 0)
    options=(f,field(28) if f&1 else None,field(32) if f&1 else None,field(36,2) if f&2 else None,field(12,2),field(24,1),0)
    call(0x8001558C if unlocked else 0x800156E8,[field(20,2),field(22,2),field(16),options]+([1] if unlocked else []))
    mem(result,4,created)
    if created!=MASK and field(38,1)&128:call(0x800190AC if unlocked else 0x80019144,[mem(result),0,0])
    return r,calls

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail';source=P/'nonmatch/func_80019490.c'
    with tempfile.TemporaryDirectory(prefix='sequence-native-') as tmp:
        obj=Path(tmp)/'candidate.o';score.compile_single(source,score.DEFAULT_FLAGS,obj)
        words=score.text_words(obj);candidate,masks,u,v,e=score.relocate(obj,words,0,960,score.image_symbols());assert not(masks or u or v or e)
    native=score.targets()['func_80019490'];cases=0;steps=0;families={}
    def check(label,r,unlocked,result=RESULT,first_ok=True,next_ok=True,created=0x7654321,mutation=None,state_address=STATE):
        nonlocal cases,steps
        expected,calls=logical(r,unlocked,result,first_ok,next_ok,created,mutation,state_address)
        for words in [native,candidate[:240]]:
            translations=[0]
            def helper(address,args,mem,ordinal):
                event=(address,args)
                ret=0xBAAD0002
                if address==0x80017644:
                    k=translations[0];translations[0]+=1
                    ret=(((args[0]&0x80000000)|(3 if k==0 else 6)) if (first_ok if k==0 else next_ok) else MASK)
                if address in (0x8001558C,0x800156E8):
                    p=args[3];f=mem(p,4)
                    options=(f,mem(p+4,4) if f&1 else None,mem(p+8,4) if f&1 else None,mem(p+12,2) if f&2 else None,mem(p+14,2),mem(p+16,1),mem(p+24,1))
                    assert f&8==0 and options[-1]==0
                    event=(address,args[:3]+(options,)+args[4:]);ret=created
                if mutation:mutation(address,event[1],mem,ordinal)
                return ret,event
            actual=execute(words,r,helper,[state_address,result,unlocked],stack_fill=0xA5 if cases%2 else 0x5A)
            assert actual['regions']=={a:bytes(d) for a,d in expected.items()}, (label,'memory')
            assert actual['calls']==calls,(label,'calls',actual['calls'],calls)
            assert not actual['uninitialized_stack_reads'],(label,actual['uninitialized_stack_reads'])
            steps+=actual['steps']
        cases+=1;families[label.split(':')[0]]=families.get(label.split(':')[0],0)+1
    for flags in range(256):
        for unlocked in (0,1,255):
            for tag in (0,0x80000000):check('all flags and modes:%d'%flags,fixture(flags,tag),unlocked)
            if not flags&4:check('null result:%d'%flags,fixture(flags),unlocked,result=0)
            check('first translation invalid:%d'%flags,fixture(flags),unlocked,first_ok=False)
            if flags&2 and not flags&4:check('second translation invalid:%d'%flags,fixture(flags),unlocked,next_ok=False)
            if not flags&6:check('creation failure:%d'%flags,fixture(flags),unlocked,created=MASK)
    for unlocked in (0,1):
        for flags in (0,2,4,16,32,128,178,255):
            for offset in (0,8,28):check('result aliases state',fixture(flags),unlocked,result=STATE+offset)
        def mutate_service(a,args,mem,i):
            if i==1:mem(STATE+38,1,0xB2);mem(STATE+8,4,0x7654)
        check('service changes later flags and identifier',fixture(0),unlocked,mutation=mutate_service)
        def mutate_pair(a,args,mem,i):
            if a in (0x800190AC,0x80019144):mem(STATE+38,1,2);mem(STATE+8,4,0x2468)
        check('pair helper changes later guarded value',fixture(0x32),unlocked,mutation=mutate_pair)
        def mutate_create(a,args,mem,i):
            if a in (0x8001558C,0x800156E8):mem(STATE+38,1,128)
        check('creation helper enables final pair',fixture(0),unlocked,mutation=mutate_create)
    for unlocked in (0,1):
        for flags in (0,2,4,16,32,128,178,255):
            r=fixture(flags,0x80000000)
            r[GLOBAL][3*4088+0xFC8:3*4088+0xFF0]=r[STATE]
            check('embedded deferred-state caller',r,unlocked,state_address=GLOBAL+3*4088+0xFC8)
    return {'result':'PASS','cases':cases,'native_and_candidate_executions':cases*2,'steps':steps,'families':families,'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'limits':'Synthetic aligned valid requests; deferred result must be non-null. External bodies are reviewed contract stubs, caller-saved registers poisoned. Option fields are read only under actual callee guards. No ROM/gameplay test.'}
if __name__=='__main__':print(json.dumps(run(),indent=2))
