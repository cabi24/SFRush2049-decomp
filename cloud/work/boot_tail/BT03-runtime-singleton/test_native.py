#!/usr/bin/env python3
"""Replay target and compiled C against independently specified synthetic state."""
import hashlib
import json
from pathlib import Path
import random
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
P = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from native_replay import execute
A, B, RESOURCE, EVENTS, GLOBAL = 0x100000, 0x110000, 0x200000, 0x300000, 0x8004BE80
MASK = 0xFFFFFFFF

def put(data, offset, value, size=4):
    data[offset:offset+size] = (value & ((1 << (size*8))-1)).to_bytes(size, 'big')
def get(data, offset, size=4):
    return int.from_bytes(data[offset:offset+size], 'big')
def fixture():
    r = {A: bytearray(0x1000), B: bytearray(0x1000), RESOURCE: bytearray(0x1000), EVENTS: bytearray(64*48), GLOBAL: bytearray(4)}
    put(r[GLOBAL],0,A)
    for c in (A,B):
        put(r[c],0x10C,RESOURCE)
    put(r[RESOURCE],4,0x40); put(r[RESOURCE],8,0x80); put(r[RESOURCE],0x14,10)
    put(r[RESOURCE],0x40,0x100); put(r[RESOURCE],0x44,0x140)
    for i in range(64): r[RESOURCE][0x80+i] = i%16
    return r

def event(r, index, n, time, code, program=255, controller=255, first=0, second=0):
    off = index*48+n*12
    put(r[EVENTS],off,time);put(r[EVENTS],off+4,program,1);put(r[EVENTS],off+5,controller,1)
    put(r[EVENTS],off+8,code,2);put(r[EVENTS],off+10,first,1);put(r[EVENTS],off+11,second,1)
    return EVENTS+off

def activate(r, i, address, context=A):
    put(r[context],0x128+i*16,address);put(r[context],0x12C+i*16,address)

def inert(*args): pass

def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail'
    source=P/'nonmatch/func_80018634.c'
    with tempfile.TemporaryDirectory(prefix='runtime-native-') as tmp:
        obj=Path(tmp)/'candidate.o';score.compile_single(source,score.DEFAULT_FLAGS,obj)
        words=score.text_words(obj);candidate,masks,unresolved,unverified,errors=score.relocate(obj,words,0,840,score.image_symbols())
        assert not (masks or unresolved or unverified or errors)
    native=score.targets()['func_80018634']
    rows=[]
    def check(label,r,expected,ret,calls=(),helper=inert):
        results=[]
        for words in (native,candidate[:210]):
            result=execute(words,r,helper)
            assert result['result']==ret, label
            assert result['regions']=={a:bytes(d) for a,d in expected.items()}, label
            assert result['calls']==list(calls), (label,result['calls'],calls)
            assert not result['uninitialized_stack_reads'], label
            results.append(result)
        rows.append({'case':label,'result':'PASS','native_steps':results[0]['steps'],'candidate_steps':results[1]['steps'],'calls':len(calls)})
    r=fixture();check('all64 inactive',r,r,0)
    rng=random.Random(18634)
    for trial in range(16):
        r=fixture();put(r[A],0x118,[0,1,65535,0xFFFFFFFF][trial%4]);put(r[A],0x11C,[0,1,0xFFFFFFFF,123][trial%4]);put(r[A],0x120,trial)
        for i in range(64):
            activate(r,i,event(r,i,0,100000,0xFFFF));put(r[A],0x130+i*16,rng.randrange(2**32));put(r[A],0x134+i*16,rng.randrange(100))
        e={a:bytearray(v) for a,v in r.items()}
        for i in range(64):
            total=(get(r[A],0x130+i*16)+get(r[A],0x118))&MASK
            put(e[A],0x130+i*16,total&65535);put(e[A],0x134+i*16,get(r[A],0x134+i*16)+(total>>16)+get(r[A],0x11C))
        check('pending clocks including wrap %d'%trial,r,e,1)
    for stop in (False,True):
        r=fixture();r[A][0xFC5]=stop
        for i in range(64): activate(r,i,event(r,i,0,0,0xFFFE if stop else 0xFFFF))
        e={a:bytearray(v) for a,v in r.items()}
        for i in range(64):put(e[A],0x12C+i*16,0)
        check('stop-loop sentinel' if stop else 'termination sentinel',r,e,1)
        check('no stale activity after termination',e,e,0)
    for bits in range(16):
        r=fixture();i=63
        for c in (A,B):r[c][0x568:0xF68]=bytes([0xA5])*0xA00
        put(r[RESOURCE],0x104,0x300 if bits&1 else 0);put(r[RESOURCE],0x108,0x340 if bits&2 else 0)
        program=7 if bits&4 else 255; controller=9 if bits&8 else 255
        activate(r,i,event(r,i,0,0,0,program,controller,128,127));event(r,i,1,1000,0xFFFF)
        e={a:bytearray(v) for a,v in r.items()};o=0x568+i*40
        for off in (0,4,8,0x1C,0x20):put(e[A],o+off,0)
        put(e[A],o+12,RESOURCE+0x10C);put(e[A],o+16,RESOURCE+0x300 if bits&1 else 0);put(e[A],o+20,RESOURCE+0x340 if bits&2 else 0)
        put(e[A],o+24,0x2000,2);put(e[A],o+26,0,2)
        for off,value in [(36,15),(37,127),(38,128),(39,63)]:put(e[A],o+off,value,1)
        put(e[A],0x12C+i*16,EVENTS+i*48+12)
        calls=[]
        if bits&4:calls.append((0x80017720,(A,7,15)))
        if bits&8:calls.append((0x80017824,(9,15)))
        def inspect(destination,args,memory,n):
            assert memory(A+o+12,4)==RESOURCE+0x10C and memory(A+o+24,2)==0x2000
            assert memory(A+o+38,1)==128 and memory(A+o+39,1)==63
        check('pattern optional/callback matrix %d'%bits,r,e,1,calls,inspect)
    r=fixture();put(r[A],0xFC6,65535,2);put(r[A],0x118,65535);put(r[A],0x11C,2)
    for i in range(64):
        activate(r,i,event(r,i,0,0,0xFFFE,first=0,second=1));event(r,i,1,1000,0xFFFF)
    e={a:bytearray(v) for a,v in r.items()};put(e[A],0xFC6,0,2)
    for i in range(64):put(e[A],0x12C+i*16,EVENTS+i*48+12);put(e[A],0x130+i*16,65535);put(e[A],0x134+i*16,12)
    check('all64 restart only one callback with loop-count wrap',r,e,1,[(0x80018B3C,(10,))])
    r=fixture();activate(r,0,event(r,0,0,0,0,7,9));event(r,0,1,1000,0xFFFF)
    activate(r,0,EVENTS,context=B);r[B][0x58C]=11
    e={a:bytearray(v) for a,v in r.items()};o=0x568
    put(e[A],o+12,RESOURCE+0x10C);put(e[A],o+24,0x2000,2)
    put(e[GLOBAL],0,B);put(e[B],0x12C,EVENTS+12)
    def replace(destination,args,memory,n):
        if destination==0x80017720:memory(GLOBAL,4,B)
    check('program callback replaces live context',r,e,1,[(0x80017720,(A,7,0)),(0x80017824,(9,11))],replace)
    r=fixture();activate(r,0,event(r,0,0,0,0xFFFE,first=0,second=1));event(r,0,1,1000,0xFFFF)
    activate(r,0,EVENTS+12,context=B);put(r[B],0xFC6,40,2)
    e={a:bytearray(v) for a,v in r.items()};put(e[A],0x12C,EVENTS+12);put(e[A],0x134,10);put(e[GLOBAL],0,B);put(e[B],0xFC6,41,2)
    def replace_loop(destination,args,memory,n):memory(GLOBAL,4,B)
    check('loop callback count lands in replacement context',r,e,1,[(0x80018B3C,(10,))],replace_loop)
    r=fixture();i=10
    activate(r,i,event(r,i,0,0,0,7,255,1,2));event(r,i,1,0,1,255,9,254,253);event(r,i,2,0,0xFFFF)
    put(r[A],0x118,100);put(r[A],0x11C,100)
    e={a:bytearray(v) for a,v in r.items()};o=0x568+i*40
    put(e[A],o+12,RESOURCE+0x14C);put(e[A],o+24,0x2000,2)
    for off,value in [(36,10),(37,253),(38,254),(39,10)]:put(e[A],o+off,value,1)
    put(e[A],0x12C+i*16,0)
    check('two immediate patterns then termination skips timer',r,e,1,[(0x80017720,(A,7,10)),(0x80017824,(9,10))])
    r=fixture();activate(r,0,event(r,0,0,2,0xFFFF));put(r[A],0x134,0xFFFFFFFF);put(r[A],0x120,2);put(r[A],0x11C,3)
    e={a:bytearray(v) for a,v in r.items()};put(e[A],0x134,2)
    check('unsigned horizon wraps before event comparison',r,e,1)
    return {'result':'PASS','cases':len(rows),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'results':rows,'limits':'Bounded fail-closed MIPS-II integer replay with synthetic mapped resources and explicit helper contracts; not full game/emulator integration. Caller-clobbered registers are poisoned after each stub and callee-saves/stack restoration are checked.'}
if __name__=='__main__':print(json.dumps(run(),indent=2))
