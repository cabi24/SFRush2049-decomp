#!/usr/bin/env python3
"""Bounded fail-closed O32 replay; no retail ROM or execution-engine claim."""
import ctypes, dataclasses, hashlib, json, os, random, struct, subprocess, sys, tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
NAME='func_8010D85C'; START=0x8010D85C
NODE=0x10000000; STATE=0x10001000; TEXT=0x10002000; FOUND=0x10003000; SP=0x10009000
GLOBAL=0x801170FC; COUNT=0x80140BDC; BANK=0x80118E08; SLOTS=0x8012E738
PATTERNS=[0x80121DA0,0x80121DA4,0x80121DA8]
FIND=0x800A464C; LOAD=0x800B0F68; REMOVE=0x8009079C; CALLBACK=0x80094888
signed=lambda x,n=32:x-(1<<n) if x&(1<<(n-1)) else x

def execute(words,x):
    enabled,blocked,flags,first,second,last,byte,count,slot,newslot,handle=x
    mem={};calls=[];writes=[]
    regions=[(NODE,32),(STATE,112),(TEXT,16),(FOUND,8),(SP-512,1024),(GLOBAL,4),(COUNT,1),(BANK,12),(SLOTS,68*8)]
    def access(a,n,v=None):
        assert n==1 or a%n==0,('unaligned',hex(a))
        assert any(b<=a and a+n<=b+l for b,l in regions),('bounds',hex(a))
        if v is None:return int.from_bytes(bytes(mem.get(a+i,0xA5) for i in range(n)),'big')
        v&=(1<<(8*n))-1
        for i,b in enumerate(v.to_bytes(n,'big')):mem[a+i]=b
        writes.append((a,n,v))
    access(NODE+4,2,0x5432);access(NODE+12,4,STATE);access(NODE+16,4,0x3f800000);access(NODE+20,4,0x12345678)
    access(STATE+4,1,flags);access(STATE+14,2,slot);access(STATE+90,2,0x4321);access(STATE+108,4,TEXT)
    access(FOUND+4,1,byte);access(GLOBAL,4,blocked);access(COUNT,1,count)
    for i in range(3):access(BANK+4*i,4,0x67800000+i)
    original=dict(mem);writes.clear()
    r=[0xAC000000+i*17 for i in range(32)];r[0]=0;r[4]=NODE;r[5]=enabled&0xffffffff;r[29]=SP;r[31]=0xfffffffc
    saved=r[16:24]+[r[28],r[30]];f=[0]*32;pc=START;pending=None;steps=0
    while pc!=0xfffffffc:
        assert START<=pc<START+4*len(words) and steps<180
        w=words[(pc-START)//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=w&65535;si=signed(imm,16)
        old,pending=pending,None;npc=pc+4;a=(r[rs]+si)&0xffffffff
        if not w:pass
        elif op==0:
            if fn==8:pending=r[rs]
            elif fn==0:r[rd]=(r[rt]<<sh)&0xffffffff
            elif fn==3:r[rd]=(signed(r[rt])>>sh)&0xffffffff
            elif fn==33:r[rd]=(r[rs]+r[rt])&0xffffffff
            elif fn==37:r[rd]=r[rs]|r[rt]
            else:raise AssertionError('unknown R %08x'%w)
        elif op==15:r[rt]=imm<<16
        elif op==9:r[rt]=a
        elif op==12:r[rt]=r[rs]&imm
        elif op==13:r[rt]=r[rs]|imm
        elif op in (4,5,20,21):
            take=(r[rs]==r[rt]) if op in (4,20) else r[rs]!=r[rt]
            if take:pending=pc+4+si*4
            elif op in (20,21):npc+=4
        elif op==3:r[31]=pc+8;pending=('call',((pc+4)&0xf0000000)|((w&0x3ffffff)<<2),pc+8)
        elif op in (32,33,35,36):
            n={32:1,33:2,35:4,36:1}[op];v=access(a,n);r[rt]=(signed(v,n*8) if op in (32,33) else v)&0xffffffff
        elif op in (40,41,43):access(a,{40:1,41:2,43:4}[op],r[rt])
        elif op==17 and rs==4:f[rd]=r[rt]
        elif op==57:access(a,4,f[rt])
        else:raise AssertionError('unknown %08x'%w)
        r[0]=0;steps+=1
        if isinstance(old,tuple):
            target=old[1];result=0
            if target==REMOVE:
                assert r[4:6]==[NODE,1];calls.append(0)
            elif target==FIND:
                which=PATTERNS.index(r[5]);assert r[4]==TEXT+(8 if which==2 else 0)
                assert access(STATE+90,2)==10;calls.append(10+which)
                result=FOUND if [first,second,last][which] else 0
            elif target==LOAD:
                kind=2 if first else 1 if second else 0
                assert r[4]==0x67800000+kind and r[6]==0 and signed(r[7])==signed((count-1)&255,8)
                assert access(r[29]+16,4)==1;assert SP-512<=r[5]<SP
                access(r[5],2,0x1234);calls.append(20+kind)
                access(STATE+14,2,newslot);access(STATE+108,4,TEXT+8);access(STATE+4,1,flags^0x80)
                result=handle
            else:raise AssertionError(('unknown call',hex(target)))
            for i in list(range(1,16))+[24,25]:r[i]=0xBD000000+i
            r[2]=result&0xffffffff;pc=old[2]
        else:pc=old if old is not None else npc
    assert r[29]==SP and saved==r[16:24]+[r[28],r[30]]
    allowed={NODE+4,NODE+5,*range(NODE+16,NODE+24),STATE+4,STATE+14,STATE+15,STATE+90,STATE+91,*range(STATE+108,STATE+112),*range(SLOTS+68*newslot,SLOTS+68*newslot+4)}
    assert all(SP-512<=a<SP+512 or a in allowed or v==original.get(a,0xA5) for a,v in mem.items())
    return [access(NODE+4,2),access(NODE+16,4),int(access(NODE+20,4)==CALLBACK),access(STATE+4,1),access(STATE+90,2),access(STATE+14,2),*([access(SLOTS+68*i,4) for i in range(8)]),len(calls),*calls]

def main():
    sys.path.insert(0,str(ROOT/'tools/cloud'));import score
    want=score.targets()[NAME]
    with tempfile.TemporaryDirectory() as td:
        d=Path(td);obj=d/'candidate.o';score.compile_single(HERE/'candidate.c',score.DEFAULT_FLAGS,obj)
        raw=score.text_words(obj);got,masks,unresolved,unverified,errors=score.relocate(obj,raw,0,len(raw)*4,score.image_symbols())
        assert not any((masks,unresolved,unverified,errors))
        # Independent GNU linker resolution, not a second use of scorer relocation.
        script=d/'link.ld';script.write_text('SECTIONS { . = 0x8010D85C; .text : { *(.text) } }\n'+''.join('%s = 0x%x;\n'%(k,v) for k,v in score.image_symbols().items() if k!=NAME))
        subprocess.run(['mips-linux-gnu-ld','-EB','-T',str(script),'-o',str(d/'linked.elf'),str(obj)],check=True)
        subprocess.run(['mips-linux-gnu-objcopy','-O','binary','-j','.text',str(d/'linked.elf'),str(d/'linked.bin')],check=True)
        independent=list(struct.unpack('>%dI'%(len((d/'linked.bin').read_bytes())//4),(d/'linked.bin').read_bytes()))
        assert independent==got
        data,sections=score._elf(obj)
        symbol=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['name']==NAME][0]
        layout=d/'layout.c';layout.write_text('#include "'+str(HERE/'candidate.c')+'"\n#define CHECK(n,e) typedef char n[(e)?1:-1]\nCHECK(node_size,sizeof(ResourceNode)==24);\nCHECK(state_size,sizeof(ResourceState)==112);\nCHECK(slot_size,sizeof(ResourceSlot)==68);\n')
        score.compile_single(layout,score.DEFAULT_FLAGS,d/'layout.o')
        subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-O1','-shared','-fPIC',str(HERE/'host.c'),'-o',str(d/'host.so')],check=True)
        lib=ctypes.CDLL(str(d/'host.so'));lib.run.argtypes=[ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)]
        rng=random.Random(0x10d85c);cases=[]
        for mode in range(64):
            for byte in [0,1,47,48,49,127,128,175,176,255]:
                for count in [0,1,127,128,129,255]:
                    cases.append([0 if mode&1 else 0x10001,int(bool(mode&2)),0x5a|int(bool(mode&4)),int(bool(mode&8)),int(bool(mode&16)),int(bool(mode&32)),byte,count,2,7,rng.getrandbits(32)])
        for i in range(10000):cases.append([rng.getrandbits(32),rng.randrange(2),rng.randrange(256),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(256),rng.randrange(256),rng.randrange(8),rng.randrange(8),rng.getrandbits(32)])
        for x in cases:
            a=execute(want,x);b=execute(got,x);out=(ctypes.c_uint32*20)();lib.run((ctypes.c_uint32*11)(*x),out);c=list(out)[:len(a)];assert a==b==c,(x,a,b,c)
        subprocess.run(['cc','-std=c99','-Wall','-Wextra','-Werror','-O1','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-DHOST_MAIN',str(HERE/'host.c'),'-o',str(d/'host')],check=True)
        subprocess.run([str(d/'host')],env=dict(os.environ,ASAN_OPTIONS='detect_leaks=0'),check=True)
        result={'claims':[],'status':'NONMATCH','target_bytes':len(want)*4,'candidate_text_bytes':len(got)*4,'candidate_function_bytes':symbol['size'],'candidate_alignment_bytes':len(got)*4-symbol['size'],'target_words':['%08x'%v for v in want],'linked_candidate_words':['%08x'%v for v in got],'cases':len(cases),'sanitizer_cases':100000,'source_sha256':hashlib.sha256((HERE/'candidate.c').read_bytes()).hexdigest(),'comparison':dataclasses.asdict(score.compare(obj,NAME,show=0)),'differing_word_offsets':[i*4 for i,w in enumerate(want) if i>=len(got) or got[i]!=w],'unresolved':unresolved,'unverified':unverified,'errors':errors}
        print(json.dumps(result,indent=2))
        if len(sys.argv)>1:Path(sys.argv[1]).write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
