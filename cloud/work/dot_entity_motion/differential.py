#!/usr/bin/env python3
"""Three-way complete native / linked IDO / host C callback-contract replay."""
from pathlib import Path
import ctypes,json,struct,subprocess,random,math
from native_machine import execute,to_bits,to_float
P=Path(__file__).resolve().parent;R=P.parents[2];B=R/'build/dot_entity_motion_audit'
S={k:int(v,16) for k,v in json.loads((R/'asm/us/blob/symbols.json').read_text())['symbols'].items()}
START=S['func_8010E4E4'];STATE,OBJECT,MODEL,REPLACEMENT=0x10000,0x20000,0x30000,0x40000
subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-fPIC','-shared',str(P/'host_semantics.c'),'-o',str(B/'host.so')],check=True)
lib=ctypes.CDLL(str(B/'host.so'));U=ctypes.c_uint32;lib.host_case.argtypes=[ctypes.POINTER(U),ctypes.POINTER(U)]
words=lambda p:list(struct.unpack('>'+str(p.stat().st_size//4)+'I',p.read_bytes()))
native=words(B/'target.bin');linked=words(B/'text.bin')
def native_case(code,x):
    regions=[]
    def add(base,size):
        b=bytearray(size);regions.append((base,b));return b
    def put(b,offset,value,n=4):b[offset:offset+n]=(value&((1<<(8*n))-1)).to_bytes(n,'big')
    state=add(STATE,20);obj=add(OBJECT,112);model=add(MODEL,24);add(REPLACEMENT,112)
    put(state,12,OBJECT);put(state,16,x[9]);put(obj,108,MODEL);put(obj,14,-7,2);put(obj,16,2,2)
    for i in range(3):put(model,4*i,x[i]);put(model,12+4*i,x[3+i]);put(obj,56+4*i,x[6+i])
    for name,value in [('D_801170FC',x[16]),('D_8002EB94',x[10]),('D_801249CC',x[11])]:put(add(S[name],4),0,value)
    a=add(S['D_80121DDC'],12)
    for i in range(3):put(a,i*4,x[12+i])
    k=add(S['D_80117530'],48*4);put(k,2*48+18,x[17],2);put(k,3*48+18,0x2000,2)
    add(S['D_80143FC8'],4)
    events=[];output=[0]*20
    def call(dest,args,mem):
        if dest==S['sound_position_set']:
            assert args[1]==OBJECT+20
            events.append(1)
            for i in range(3):
                value=mem(args[0]+4*i,4);output[9+i]=value;mem(OBJECT+20+4*i,4,value)
            if x[18]:
                mem(STATE+16,4,to_bits(2.0));mem(S['D_8002EB94'],4,to_bits(4.0))
                mem(OBJECT+16,2,3);mem(OBJECT+14,2,-12);mem(STATE+12,4,REPLACEMENT)
        elif dest==S['entity_transform_apply']:
            assert args[:2]==[STATE,1];events.append(4)
        elif dest==S['entity_spawn_callback']:
            assert args[1:3]==[0,0];events.append(2);output[18]=args[0]
        elif dest==S['func_800AFA84']:
            assert args[:2]==[S['D_80143FC8'],OBJECT];events.append(3)
        else:raise AssertionError(('unexpected callback',hex(dest)))
    _,after,reads,writes=execute(code,START,regions,[STATE,x[15],0,0],call)
    def get(address):
        for base,data in after:
            if base<=address and address+4<=base+len(data):return int.from_bytes(data[address-base:address-base+4],'big')
        raise AssertionError(hex(address))
    for i in range(3):output[i]=get(MODEL+12+4*i);output[3+i]=get(OBJECT+56+4*i);output[6+i]=get(OBJECT+20+4*i)
    output[12]=get(STATE+16);output[13]=len(events);output[14:14+len(events)]=events;output[19]=get(S['D_8002EB94'])
    if (x[15]&65535)==0 or x[16]:
        assert not any(OBJECT<=a<OBJECT+112 or MODEL<=a<MODEL+24 for a,w in reads+writes),'early return accessed object'
    return output, [(base, bytes(data)) for base, data in after]

def canonical(a):return [0x7fc00000 if i in list(range(13))+[19] and math.isnan(to_float(v)) else v for i,v in enumerate(a)]
def check(x):
    result=(U*20)();lib.host_case((U*19)(*x),result)
    n,nmem=native_case(native,x);l,lmem=native_case(linked,x);h=list(result)
    assert nmem==lmem, ("non-stack memory mismatch",x)
    assert canonical(n)==canonical(l)==canonical(h),(x,n,l,h)
base=list(map(to_bits,[1.0,-2.0,3.0,4.0,-5.0,6.0,7.0,8.0,-9.0,1.0,0.5,2.0,0.25,-0.5,1.0]))+[1,0,0x2000,0]
cases=0
for mode in [0,1,2,0xffff,0x8000,0x10000,0xffff0000,0x7fff]:
 for pause in [0,1,0xffffffff]:
  for flags in [0,0x2000,0xdfff,0xffff]:
   for mutation in [0,1]:
    for timer in [0,0x80000000,to_bits(0.5),to_bits(0.5)-1,to_bits(0.5)+1,to_bits(-1),0x7f800000,0xff800000,0x7fc12345]:
     x=base.copy();x[15:19]=[mode,pause,flags,mutation];x[9]=timer;check(x);cases+=1
special=[0,0x80000000,1,0x80000001,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc12345,to_bits(-1),to_bits(1)]
for component in range(15):
 for value in special:
  x=base.copy();x[component]=value;check(x);cases+=1
rng=random.Random(0x8010e4e4)
for i in range(4000):
 x=[rng.getrandbits(32) for _ in range(15)]+[rng.choice([0,1,0xffff]),rng.choice([0,0,0,1]),rng.randrange(65536),rng.randrange(2)]
 check(x);cases+=1
proof={'cases':cases,'result':'PASS','models':['full native instructions','fresh linked IDO instructions','host C with callback stubs'],'limitations':['round-to-nearest binary32; NaNs compared by class','no signaling-NaN payload or FCSR exception equivalence','well-formed nonoverlapping typed records and valid kind indices','callback stubs cover ordering, arguments and selected mutations, not full game callees'],'claims':[]}
(B/'semantics.json').write_text(json.dumps(proof,indent=2)+'\n');print(json.dumps(proof,indent=2))
