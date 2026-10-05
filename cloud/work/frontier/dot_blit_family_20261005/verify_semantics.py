#!/usr/bin/env python3
"""Protected native words, independently linked code, oracle and UBSan host C."""
import ctypes
import itertools
from pathlib import Path
import random
import struct
import subprocess
import sys
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
FN='func_800EF62C'
MASK=0xffffffff
OWNER,STACK,STOP=0x1000,0x6000,0xfffffff0

def signed(v,bits=32):
    v&=(1<<bits)-1
    return v-(1<<bits) if v&(1<<(bits-1)) else v

def f32(v): return struct.unpack('>f',struct.pack('>f',v))[0]
def fbits(v): return struct.unpack('>I',struct.pack('>f',v))[0]
def fvalue(v): return struct.unpack('>f',struct.pack('>I',v))[0]
def half(v): return (-1 if v<0 else 1)*(abs(v)//2)

def oracle(c):
    slot,kind,count,total,enabled,flag,mode,hide,width,height,value=c
    out=[1,-321,654,width,height,-30000,1234,-5678,30000,17,hide,1,0,0]
    if slot>=count:
        if slot>=total: out[11]=0
        if hide!=1: out[10]=1;out[13]+=1
        return out
    hidden=int(not enabled or bool(flag) or mode==1)
    if hidden!=hide: out[10]=hidden;out[13]+=1
    if hidden:return out
    if count>=2:
        out[12]=2 if count==2 else 3
        width=height=-1
        out[3:9]=[-1]*6
        out[9]=0
    out[1]=signed(count*101+slot*17-220-half(width),16)
    out[2]=signed(count*43-slot*31-90,16)
    if kind==0:out[6]=signed(half(height)-1,16)
    elif kind==1:
        amount=min(abs(f32(f32(value*f32(1.35))*f32(.0001))),1.0)
        out[8]=signed(int(f32((width-1)*amount)),16)
        out[5]=half(height)
        out[6]=signed(height-1,16)
    out[9]=254;out[13]+=1
    return out

class Native:
    def __init__(self,words=None):
        self.addresses=score.image_symbols()
        for name in ('D_8014A250','D_80152818','D_80115B68','D_80151AD0','D_8014A108','D_8015B25C','D_80120D9C','D_80120DAC'):
            self.addresses.setdefault(name,int(name[2:],16))
        self.start=self.addresses[FN]
        words=score.targets()[FN] if words is None else words
        self.code={self.start+4*i:w for i,w in enumerate(words)}
        self.coverage=set()
    def run(self,c):
        slot,kind,count,total,enabled,flag,mode,hide,width,height,value=c
        a=self.addresses
        regions={OWNER:bytearray(48),STACK:bytearray(512),a['D_8014A250']:bytearray(4*2056),
                 a['D_80152818']:bytearray(4*952),a['D_80115B68']:bytearray(128),
                 a['D_80151AD0']:bytearray(2),a['D_8014A108']:bytearray(2),
                 a['D_8015B25C']:bytearray(1),0x801245A4:bytearray(struct.pack('>ff',1.35,.0001))}
        def mem(address,size,value=None):
            assert address%size==0
            for base,data in regions.items():
                off=address-base
                if 0<=off and off+size<=len(data):
                    if value is None:return int.from_bytes(data[off:off+size],'big')
                    data[off:off+size]=(value&((1<<(size*8))-1)).to_bytes(size,'big');return
            raise AssertionError(('outside regions',hex(address),size))
        for name,size,val in [('D_80151AD0',2,count),('D_8014A108',2,total),('D_8015B25C',1,enabled)]:mem(a[name],size,val)
        for i in range(4):
            car=a['D_8014A250']+2056*i
            mem(car+10,1,flag);mem(car+1990,2,i);mem(car+2000,2,value)
            mem(a['D_80152818']+952*i+239,1,mode)
            for j in range(4):
                p=a['D_80115B68']+i*32+j*8
                mem(p,4,(i+1)*101+j*17-220);mem(p+4,4,(i+1)*43-j*31-90)
        for off,val in [(14,-321),(16,654),(20,width),(22,height),(28,-30000),(30,1234),(32,-5678),(34,30000)]:mem(OWNER+off,2,val)
        mem(OWNER+24,1,17);mem(OWNER+26,1,hide);mem(OWNER+40,4,0x2000);mem(OWNER+44,4,(slot<<4)|kind)
        untouched={base:bytes(data) for base,data in regions.items() if base not in (OWNER,STACK)}
        original=bytes(regions[OWNER]);r=[0]*32;f=[0x3f000000+i for i in range(32)]
        saved_f=f[20:]
        r[4]=OWNER;r[29]=STACK+256;r[31]=STOP
        saved=[0x51e00000+i for i in range(9)]
        for reg,val in zip(list(range(16,24))+[30],saved):r[reg]=val
        pc,pending,steps,condition=self.start,None,0,False
        updates=renamed=0
        while pc!=STOP:
            if pc in (a['func_800EF5B0'],a['Input_ApplyPadConfig']):
                assert pending is None and r[4]==OWNER
                if pc==a['func_800EF5B0']:
                    assert r[6]==0 and r[5] in (a['D_80120D9C'],a['D_80120DAC'])
                    renamed=2 if r[5]==a['D_80120D9C'] else 3
                    for off in (20,22,28,30,32,34):mem(OWNER+off,2,-1)
                    mem(OWNER+24,1,0)
                else:updates+=1
                for reg in list(range(1,16))+[24,25]:r[reg]=0xc10b0000+reg
                f[:20]=[0x40000000+i for i in range(20)]
                pc=r[31];continue
            assert pc in self.code and steps<500
            self.coverage.add(pc-self.start)
            w=self.code[pc];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sh=(w>>6)&31;fn=w&63
            imm=w&65535;si=signed(imm,16);address=(r[rs]+si)&MASK
            old,pending=pending,None;next_pc=pc+4
            if not w:pass
            elif op==0:
                if fn==0:r[rd]=r[rt]<<sh
                elif fn==2:r[rd]=r[rt]>>sh
                elif fn==3:r[rd]=signed(r[rt])>>sh
                elif fn==8:pending=r[rs]
                elif fn==33:r[rd]=r[rs]+r[rt]
                elif fn==35:r[rd]=r[rs]-r[rt]
                elif fn==37:r[rd]=r[rs]|r[rt]
                elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
                elif fn==43:r[rd]=int(r[rs]<r[rt])
                else:raise AssertionError(('special',fn))
            elif op==1:
                assert rt==1
                if signed(r[rs])>=0:pending=pc+4+4*si
            elif op in (2,3):
                if op==3:r[31]=pc+8
                pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
            elif op in (4,5,20,21):
                take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
                if take:pending=pc+4+4*si
                elif op in (20,21):next_pc+=4
            elif op==9:r[rt]=address
            elif op==10:r[rt]=int(signed(r[rs])<si)
            elif op==11:r[rt]=int(r[rs]<(si&MASK))
            elif op==12:r[rt]=r[rs]&imm
            elif op==14:r[rt]=r[rs]^imm
            elif op==15:r[rt]=imm<<16
            elif op==17:
                if rs==0:r[rt]=f[rd]
                elif rs==4:f[rd]=r[rt]
                elif rs==8:
                    take=condition if rt&1 else not condition
                    if take:pending=pc+4+4*si
                    elif rt&2:next_pc+=4
                elif rs==20:
                    assert fn==32;f[sh]=fbits(signed(f[rd]))
                elif rs==16:
                    if fn==2:f[sh]=fbits(f32(fvalue(f[rd])*fvalue(f[rt])))
                    elif fn==6:f[sh]=f[rd]
                    elif fn==7:f[sh]=f[rd]^0x80000000
                    elif fn==13:f[sh]=int(fvalue(f[rd]))&MASK
                    elif fn==60:condition=fvalue(f[rd])<fvalue(f[rt])
                    else:raise AssertionError(('float',fn))
                else:raise AssertionError(('cop1',rs))
            elif op==32:r[rt]=signed(mem(address,1),8)
            elif op==33:r[rt]=signed(mem(address,2),16)
            elif op==35:r[rt]=mem(address,4)
            elif op==40:mem(address,1,r[rt])
            elif op==41:mem(address,2,r[rt])
            elif op==43:mem(address,4,r[rt])
            elif op==49:f[rt]=mem(address,4)
            else:raise AssertionError(('opcode',op))
            r[:]=[x&MASK for x in r];r[0]=0
            pc=old if old is not None else next_pc;steps+=1
        assert all(bytes(regions[base])==before for base,before in untouched.items())
        assert f[20:]==saved_f
        assert r[29]==STACK+256 and r[31]==STOP
        assert [r[i] for i in list(range(16,24))+[30]]==saved
        allowed=set(range(14,18))|set(range(20,25))|{26}|set(range(28,36))|set(range(40,44))
        assert all(i in allowed or b==original[i] for i,b in enumerate(regions[OWNER]))
        return [signed(r[2])]+[signed(mem(OWNER+i,2),16) for i in (14,16,20,22,28,30,32,34)]+[mem(OWNER+24,1),signed(mem(OWNER+26,1),8),int(mem(OWNER+40,4)!=0),renamed,updates]

def cases():
    out=[]
    for slot,kind,count in itertools.product((0,1,3,15),(0,1,2,15),(1,2,3,4)):
        for enabled,flag,mode,hide in [(0,0,0,0),(1,1,0,0),(1,0,1,0),(1,0,0,0),(1,0,0,1),(1,0,0,-1)]:
            for value in (-32768,-7408,-1,0,1,7408,32767):
                out.append((slot,kind,count,4,enabled,flag,mode,hide,123,55,value))
    for width,height,value in itertools.product((-32768,-1,0,1,32767),(-32768,-1,0,1,32767),(-32768,-7407,-1,0,1,7407,32767)):
        out.append((0,1,1,1,1,0,0,0,width,height,value))
    rng=random.Random(0xEF62C)
    for _ in range(1500):
        count=rng.randint(1,4);slot=rng.randrange(16)
        out.append((slot,rng.randrange(16),count,rng.randint(0,4),rng.choice((-1,0,1)),rng.choice((-1,0,1)),rng.choice((-128,0,1,127)),rng.choice((-128,-1,0,1,127)),rng.randint(-32768,32767),rng.randint(-32768,32767),rng.randint(-32768,32767)))
    return out

def host_function(directory,source=None,label='host'):
    output=directory/(label+'.so')
    cmd=['cc','-std=c89','-O1','-Wall','-Wextra','-Werror','-shared','-fPIC','-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all']
    if source:cmd.append('-DCANDIDATE="'+str(Path(source).resolve())+'"')
    subprocess.run(cmd+[str(HERE/'semantic_test.c'),'-o',str(output)],check=True,capture_output=True,text=True)
    lib=ctypes.CDLL(str(output));fn=lib.run_case
    fn.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];fn.restype=None
    def run(c):
        out=(ctypes.c_int*14)();fn((ctypes.c_int*11)(*c),out);return list(out)
    return run

def verify(directory,linked_words=None):
    native=Native();linked=Native(linked_words) if linked_words is not None else None
    host=host_function(directory);samples=cases()
    for c in samples:
        want=oracle(c)
        assert native.run(c)==want,('native',c,native.run(c),want)
        assert host(c)==want,('host',c,host(c),want)
        if linked:assert linked.run(c)==want,('linked',c)
    source=(ROOT/'cloud/matches/func_800EF62C.c').read_text()
    changes={'wrong_coefficient':('1.35f','1.0f'),
             'no_clamp':('if(amount>1.0f) amount=1.0f;',''), 'wrong_slot':('>>4','>>3'),
             'wrong_half':('blt->Top=blt->Height/2;','blt->Top=blt->Height>>1;'),
             'wrong_mode':('.mode == 1','.mode != 0'),
             'missing_final_update':('    blt->Alpha=254;\n    Input_ApplyPadConfig(blt);','    blt->Alpha=254;')}
    mutants={}
    for label,(before,after) in changes.items():
        assert source.count(before)==1
        p=directory/(label+'.c');p.write_text(source.replace(before,after));changed=host_function(directory,p,label)
        for c in samples:
            if changed(c)!=oracle(c):mutants[label]=list(c);break
        assert label in mutants,label
    return {'cases':len(samples),'native':True,'gnu_linked':linked is not None,'host_ubsan':True,
            'instruction_offsets_executed':len(native.coverage),'instruction_count':178,
            'uncovered_offsets':[i for i in range(0,712,4) if i not in native.coverage],
            'detected_wrong_contracts':mutants}
