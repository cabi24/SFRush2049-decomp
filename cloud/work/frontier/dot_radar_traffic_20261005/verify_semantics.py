#!/usr/bin/env python3
"""Bounded native/oracle/host-C differential proof for the radar callback."""
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
FN='func_80108F40'
MASK=0xffffffff
OWNER,STACK,STOP=0x1000,0x6000,0xfffffff0

def signed(v,bits=32):
    v&=(1<<bits)-1
    return v-(1<<bits) if v&(1<<(bits-1)) else v

def f32(v):return struct.unpack('>f',struct.pack('>f',v))[0]
def fbits(v):return struct.unpack('>I',struct.pack('>f',v))[0]
def fvalue(v):return struct.unpack('>f',struct.pack('>I',v))[0]
def half(v):return (-1 if v<0 else 1)*(abs(v)//2)

def oracle(c):
    view,slot,count,total,flags,enabled,mode,width,height,live,solid,kind,mirror,mw,mh,override,px,py,pz,hide=c
    x,y,active=-321,654,1
    calls=[]
    def update():
        nonlocal hide
        calls.append(1)
        if calls.count(1)==1 and override!=99:hide=override
    def hidden(value):
        nonlocal hide
        if value!=hide:hide=value;update()
        return hide
    def result(value):return [value,x,y,hide,active,len(calls)]+calls+[0]*(16-len(calls))
    w=int(f32(width*0.125));h=height
    if view>=count or flags&8 or total<2:
        active=0;return result(hidden(1))
    if hidden(int(not enabled or mode==1)):return result(1)
    if not live:
        active=0;return result(hidden(1))
    v=0
    if slot!=view:calls.append(2);v=int(not solid)
    if hidden(v):return result(1)
    calls.append(3)
    if slot==view:px=py=pz=0
    x=int(f32(mw*0.5));y=16
    scale=f32(1/480)
    x=int(f32(x+f32(f32((-px if mirror else px)*scale)*mw)))
    y=int(f32(y+f32(f32(pz*-scale)*mh)))
    if hidden(int(x<w or x>mw-w or y<h or y>mh-h)):return result(1)
    x=signed(x+count*31+view*17-40,16)
    y=signed(y+count*29-view*11-20,16)
    if kind==2:calls.append(100+slot+2)
    elif kind==1:calls.append(105)
    x=signed(x-half(w),16);y=signed(y-half(h),16)
    update()
    return result(1)

class Native:
    def __init__(self,words=None):
        self.addresses=score.image_symbols()
        for name in ('D_8011617C','D_80116180','D_80156CE8','D_80140A04','D_80116028','D_801543CA','D_80151AD0','D_8014A250'):
            self.addresses.setdefault(name,int(name[2:],16))
        self.start=self.addresses[FN]
        words=score.targets()[FN] if words is None else words
        self.code={self.start+4*i:w for i,w in enumerate(words)}
        self.coverage=set()
    def run(self,c):
        view,slot,count,total,flags,enabled,mode,width,height,live,solid,kind,mirror,mw,mh,override,px,py,pz,hide=c
        a=self.addresses
        regions={OWNER:bytearray(48),STACK:bytearray(1024),a['D_8014A250']:bytearray(16*2056),
                 a['player_array']:bytearray(16*952),a['D_80116028']:bytearray(128),
                 a['D_80151AD0']:bytearray(2),a['D_801543CA']:bytearray(2),
                 a['state_word_a']:bytearray(4),a['D_80156CE8']:bytearray(1),a['D_80140A04']:bytearray(1),
                 a['D_8011617C']:bytearray(4),a['D_80116180']:bytearray(4),
                 0x801248cc:bytearray(struct.pack('>ff',f32(1/480),f32(1/480)))}
        def mem(address,size,value=None):
            assert address%size==0,(hex(address),size)
            for base,data in regions.items():
                off=address-base
                if 0<=off and off+size<=len(data):
                    if value is None:return int.from_bytes(data[off:off+size],'big')
                    data[off:off+size]=(value&((1<<(size*8))-1)).to_bytes(size,'big');return
            raise AssertionError(('outside regions',hex(address),size))
        for name,size,val in [('D_80151AD0',2,count),('D_801543CA',2,total),('state_word_a',4,flags),
                              ('D_80156CE8',1,enabled),('D_80140A04',1,mirror),('D_8011617C',4,mw),('D_80116180',4,mh)]:
            mem(a[name],size,val)
        for i in range(16):
            p=a['player_array']+952*i;m=a['D_8014A250']+2056*i
            mem(p+239,1,mode)
            for off in (44,60,76):mem(p+off,4,fbits(1.0))
            mem(m+1990,2,i+2);mem(m+1992,2,live);mem(m+1996,1,kind)
        for j,v in enumerate((px,py,pz)):mem(a['player_array']+slot*952+8+j*4,4,fbits(v))
        for i in range(4):
            for j in range(4):
                p=a['D_80116028']+i*32+j*8
                mem(p,4,(i+1)*31+j*17-40);mem(p+4,4,(i+1)*29-j*11-20)
        for off,v in ((14,-321),(16,654),(20,width),(22,height)):mem(OWNER+off,2,v)
        mem(OWNER+26,1,hide);mem(OWNER+40,4,0x1234);mem(OWNER+44,4,(view<<8)|slot)
        original=bytes(regions[OWNER]);r=[0]*32;f=[0]*32
        untouched={base:bytes(data) for base,data in regions.items() if base not in (OWNER,STACK)}
        saved_f=f[20:];r[4]=OWNER;r[29]=STACK+512;r[31]=STOP
        saved=[0x51e00000+i for i in range(9)]
        for reg,val in zip(list(range(16,24))+[30],saved):r[reg]=val
        pc,pending,steps,condition=self.start,None,0,False
        calls=[]
        while pc!=STOP:
            if pc in (a['func_800CF604'],a['Input_ApplyPadConfig'],a['func_800A61B0'],a['stat_race_update']):
                assert pending is None
                result=0
                if pc==a['Input_ApplyPadConfig']:
                    assert r[4]==OWNER;calls.append(1)
                    if calls.count(1)==1 and override!=99:mem(OWNER+26,1,override)
                elif pc==a['func_800CF604']:
                    assert signed(r[4])==slot;calls.append(2);result=solid
                elif pc==a['func_800A61B0']:
                    calls.append(3);assert r[6]==a['player_array']+view*952+44
                    v=[fvalue(mem(r[4]+4*i,4)) for i in range(3)]
                    for i in range(3):
                        m=[fvalue(mem(r[6]+4*(3*i+j),4)) for j in range(3)]
                        y=f32(f32(f32(v[0]*m[0])+f32(v[1]*m[1]))+f32(v[2]*m[2]))
                        mem(r[5]+4*i,4,fbits(y))
                else:
                    assert r[4]==OWNER and signed(r[5])==(slot+2 if kind==2 else 5)
                    assert signed(r[6])==int(f32(width*.125)) and signed(r[7])==height
                    calls.append(100+signed(r[5]))
                for reg in list(range(1,16))+[24,25]:r[reg]=(0xc10b0000+reg)&MASK
                r[2]=result&MASK;f[:20]=[0x40000000+i for i in range(20)]
                pc=r[31];continue
            assert pc in self.code and steps<1500, (hex(pc),steps)
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
                elif fn==38:r[rd]=r[rs]^r[rt]
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
                    if fn==0:f[sh]=fbits(f32(fvalue(f[rd])+fvalue(f[rt])))
                    elif fn==1:f[sh]=fbits(f32(fvalue(f[rd])-fvalue(f[rt])))
                    elif fn==2:f[sh]=fbits(f32(fvalue(f[rd])*fvalue(f[rt])))
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
            elif op==57:mem(address,4,f[rt])
            else:raise AssertionError(('opcode',op))
            r[:]=[x&MASK for x in r];r[0]=0
            pc=old if old is not None else next_pc;steps+=1
        assert all(bytes(regions[base])==before for base,before in untouched.items())
        assert f[20:]==saved_f
        assert r[29]==STACK+512 and r[31]==STOP
        assert [r[i] for i in list(range(16,24))+[30]]==saved
        allowed=set(range(14,18))|{26}|set(range(40,44))
        assert all(i in allowed or b==original[i] for i,b in enumerate(regions[OWNER]))
        return [signed(r[2]),signed(mem(OWNER+14,2),16),signed(mem(OWNER+16,2),16),
                signed(mem(OWNER+26,1),8),int(mem(OWNER+40,4)!=0),len(calls)]+calls+[0]*(16-len(calls))

def cases():
    base=[0,1,2,4,0,1,0,64,8,1,1,2,1,160,80,99,0,0,0,0]
    out=[]
    for index,values in [(0,[0,1,2,3,15]),(1,[0,1,2,3,15]),(2,[-1,0,1,2,3,4]),(3,[-1,0,1,2,4]),
                         (4,[0,8,16,24]),(5,[-1,0,1]),(6,[-128,-1,0,1,127]),(7,[-16,-1,0,1,8,64,256]),
                         (8,[-16,-1,0,1,8,16,32]),(9,[-1,0,1]),(10,[-1,0,1]),(11,[-1,0,1,2,3]),
                         (12,[-1,0,1]),(13,[1,80,160,320]),(14,[1,40,80,160]),(15,[-128,-1,0,1,127,99]),
                         (16,[-2000,-480,-240,-1,0,1,240,480,2000]),(18,[-2000,-480,-240,-1,0,1,240,480,2000]),
                         (19,[-128,-1,0,1,127])]:
        for v in values:
            c=base.copy();c[index]=v;out.append(c)
    for px,pz,mirror,kind,hide in itertools.product((-240,-100,0,100,240),(-100,-1,0,1,100),(0,1),(0,1,2),(0,1,-1)):
        c=base.copy();c[16]=px;c[18]=pz;c[12]=mirror;c[11]=kind;c[19]=hide;out.append(c)
    rng=random.Random(0x108f40)
    for _ in range(1200):
        c=base.copy();c[0]=rng.randrange(4);c[1]=rng.randrange(4);c[2]=rng.randint(1,4)
        c[3]=rng.choice([1,2,4]);c[4]=rng.choice([0,0,8]);c[5]=rng.choice([0,1,1]);c[6]=rng.choice([0,0,1])
        c[7]=rng.choice([8,64,128,256]);c[8]=rng.choice([4,8,16]);c[9]=rng.choice([0,1,1]);c[10]=rng.choice([0,1])
        c[11]=rng.randrange(4);c[12]=rng.randrange(2);c[15]=rng.choice([99,99,0,1,-1])
        c[16]=rng.randint(-1000,1000);c[17]=rng.randint(-20,20);c[18]=rng.randint(-500,500);c[19]=rng.choice([-1,0,1])
        out.append(c)
    return out

def host_function(directory,source=None,label='host'):
    output=directory/(label+'.so')
    cmd=['cc','-std=c89','-O1','-Wall','-Wextra','-Werror','-shared','-fPIC','-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all']
    if source:cmd.append('-DCANDIDATE="'+str(Path(source).resolve())+'"')
    subprocess.run(cmd+[str(HERE/'semantic_test.c'),'-o',str(output)],check=True,capture_output=True,text=True)
    lib=ctypes.CDLL(str(output));fn=lib.run_case
    fn.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];fn.restype=None
    def run(c):
        out=(ctypes.c_int*22)();fn((ctypes.c_int*20)(*c),out);return list(out)
    return run

def verify(directory,linked_words=None):
    native=Native();linked=Native(linked_words) if linked_words is not None else None
    host=host_function(directory);samples=cases()
    for c in samples:
        want=oracle(c)
        got=native.run(c);assert got==want,('native',c,got,want)
        got=host(c);assert got==want,('host',c,got,want)
        if linked:
            got=linked.run(c);assert got==want,('linked',c,got,want)
    source=(HERE/'candidate.c').read_text()
    changes={'wrong_hidden_return':('return blt->Hide;','return hide;'),
             'wrong_scale':('scale=1.0f/480.0f','scale=1.0f/240.0f'),
             'wrong_mirror':('blt->X+=-rpos[0]*scale','blt->X+=rpos[0]*scale'),
             'wrong_eligibility':('slot!=view && !func_800CF604(slot)','slot!=view && func_800CF604(slot)'),
             'wrong_center':('blt->X-=width/2;','blt->X-=width;')}
    mutants={}
    for label,(before,after) in changes.items():
        assert before in source
        p=directory/(label+'.c');p.write_text(source.replace(before,after));changed=host_function(directory,p,label)
        for c in samples:
            if changed(c)!=oracle(c):mutants[label]=list(c);break
        assert label in mutants,label
    return {'cases':len(samples),'native':True,'gnu_linked':linked is not None,'host_ubsan':True,
            'instruction_offsets_executed':len(native.coverage),'instruction_count':330,
            'uncovered_offsets':[i for i in range(0,1320,4) if i not in native.coverage],
            'detected_wrong_contracts':mutants}

if __name__=='__main__':
    import tempfile,json
    with tempfile.TemporaryDirectory(prefix='radar-semantics-') as d:print(json.dumps(verify(Path(d)),indent=2))
