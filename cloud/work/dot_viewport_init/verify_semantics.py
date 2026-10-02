#!/usr/bin/env python3
"""Fail-closed native-word versus literal host-C differential execution.
Requires IDO_DIR for candidate compilation; no ROM input. RNE float conversion.
"""
import argparse, ctypes, dataclasses, hashlib, json, os, random, struct, subprocess, sys, tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
NAME='func_800A5A40'; START=0x800A5A40
W,H=0x8002AFC0,0x8002AFC4
V,B,VIEW=0x8011EA30,0x80149870,0x8017A510
F1,F2,CTX=0x80146204,0x8017A63C,0x80154188
CALL=0x800A5908; SP=0x10008000; RETURN=0xFFFFFFFC
signed=lambda x,n=32:x-(1<<n) if x&(1<<(n-1)) else x
fbits=lambda n:struct.unpack('>I',struct.pack('>f',n))[0]

def execute(words, initial, after):
    mem={}; writes=[]; captures=[]
    regions=[(W,8),(V,16),(B,8),(VIEW,8),(F1,1),(F2,1),(CTX,4),(SP-256,512)]
    def access(address,width,value=None):
        assert any(base<=address and address+width<=base+size for base,size in regions),hex(address)
        assert width==1 or address%width==0
        if value is None:return int.from_bytes(bytes(mem.get(address+i,0xCC) for i in range(width)),'big')
        value &= (1<<(width*8))-1
        for i,v in enumerate(value.to_bytes(width,'big')):mem[address+i]=v
        writes.append((address,width,value))
    access(W,4,initial[0]);access(H,4,initial[1]);access(CTX,4,0x12345678)
    access(F1,1,0xA5);access(F2,1,0x5A)
    for offset in range(16):access(V+offset,1,0x37)
    r=[(0xCA000000+i*0x10101)&0xFFFFFFFF for i in range(32)];r[0]=0;r[29]=SP;r[31]=RETURN
    saved=r[16:24]+[r[28],r[30]];f=[0]*32;pc=START;pending=None;steps=0
    while pc!=RETURN:
        assert START<=pc<START+len(words)*4 and steps<100,(hex(pc),steps)
        ins=words[(pc-START)//4];op=ins>>26;rs=(ins>>21)&31;rt=(ins>>16)&31;rd=(ins>>11)&31
        sh=(ins>>6)&31;fn=ins&63;imm=ins&65535;si=signed(imm,16)
        old=pending;pending=None;npc=pc+4;addr=(r[rs]+si)&0xFFFFFFFF
        if ins==0:pass
        elif op==0 and fn==8:pending=r[rs]
        elif op==0 and fn==3:r[rd]=(signed(r[rt])>>sh)&0xFFFFFFFF
        elif op==0 and fn==37:r[rd]=r[rs]|r[rt]
        elif op==15:r[rt]=imm<<16
        elif op==9:r[rt]=addr
        elif op==1 and rt==1:
            if signed(r[rs])>=0:pending=pc+4+si*4
        elif op==3:
            target=((pc+4)&0xF0000000)|((ins&0x3FFFFFF)<<2)
            assert target==CALL
            r[31]=pc+8;pending=('call',pc+8)
        elif op in (35,37):r[rt]=access(addr,{35:4,37:2}[op])
        elif op in (40,41,43):access(addr,{40:1,41:2,43:4}[op],r[rt])
        elif op==57:access(addr,4,f[rt])
        elif op==17 and rs==4:f[rd]=r[rt]
        elif op==17 and rs==20 and fn==32:f[sh]=fbits(signed(f[rd]))
        else:raise AssertionError('Unsupported %08x at %08x'%(ins,pc))
        r[0]=0;steps+=1
        if isinstance(old,tuple):
            assert access(F1,1)==1 and access(F2,1)==0
            args=r[4:8]+[access(r[29]+i,4) for i in range(16,36,4)]
            assert args[:4]==[0,V,B,0x12345678]
            captures.append(args)
            access(W,4,after[0]);access(H,4,after[1]);access(B,4,0xA5A5A5A5);access(B+4,4,0xA5A5A5A5)
            for i in list(range(1,16))+[24,25]:r[i]=0xBD000000+i
            for i in range(20):f[i]=0x7FC12345
            pc=old[1]
        else:pc=old if old is not None else npc
    assert len(captures)==1 and r[29]==SP and saved==r[16:24]+[r[28],r[30]]
    assert access(B,4)==0 and access(VIEW,4)==V and access(VIEW+4,4)==B
    assert access(B+4,2)==(after[0]&65535) and access(B+6,2)==(after[1]&65535)
    assert access(W,4)==(after[0]&0xFFFFFFFF) and access(H,4)==(after[1]&0xFFFFFFFF)
    assert access(F1,1)==1 and access(F2,1)==0 and access(CTX,4)==0x12345678
    assert all(access(V+i,1)==0x37 for i in range(16))
    return captures[0]+[access(B+4,2),access(B+6,2)]

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--output',type=Path,required=True);args=ap.parse_args()
    root=HERE.parents[2];sys.path.insert(0,str(root/'tools/cloud'));import score
    want=score.targets()[NAME];assert len(want)==63
    rng=random.Random(0xA5A40);cases=[]
    edges=[-2147483648,-2147483647,-16777217,-65537,-65536,-32769,-3,-2,-1,0,1,2,3,32767,65535,65536,16777217,2147483646,2147483647]
    for w in edges:
        for h in edges:cases.extend([(w,h,w,h),(w,h,~w,~h)])
    cases += [tuple(signed(rng.getrandbits(32)) for _ in range(4)) for _ in range(20000)]
    with tempfile.TemporaryDirectory() as td:
        d=Path(td);obj=d/'candidate.o';score.compile_single(HERE/'candidate.c',score.DEFAULT_FLAGS,obj)
        raw=score.text_words(obj);got,masks,unresolved,unverified,errors=score.relocate(obj,raw,0,len(raw)*4,score.image_symbols())
        assert not any((masks,unresolved,unverified,errors))
        data,sections=score._elf(obj)
        fs=[s for i,sec in enumerate(sections) if sec['type']==2 for s in score._symbol_table(data,sections,i) if s['name']==NAME]
        assert len(fs)==1 and fs[0]['size']==248
        subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-O2','-shared','-fPIC',str(HERE/'host.c'),'-o',str(d/'host.so')],check=True)
        lib=ctypes.CDLL(str(d/'host.so'));lib.run.argtypes=[ctypes.c_int]*4+[ctypes.POINTER(ctypes.c_uint32)]
        for w,h,aw,ah in cases:
            result=(ctypes.c_uint32*11)();lib.run(w,h,aw,ah,result)
            target=execute(want,(w,h),(aw,ah));candidate=execute(got[:62],(w,h),(aw,ah))
            assert target==candidate==list(result),(w,h,aw,ah,target,candidate,list(result))
        proof={'claims':[],'status':'NONMATCH','cases':len(cases),'native_executions':2*len(cases),'source_sha256':hashlib.sha256((HERE/'candidate.c').read_bytes()).hexdigest(),'target_words':63,'target_bytes':252,'candidate_bytes':248,'differing_word_offsets':[i*4 for i,w in enumerate(want) if got[i]!=w],'zero_alignment_words':len(got)-62,'unresolved':unresolved,'unverified':unverified,'errors':errors,'target_sha256':hashlib.sha256(struct.pack('>63I',*want)).hexdigest(),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'comparison':dataclasses.asdict(score.compare(obj,NAME,show=0))}
        assert proof['comparison']['differing']==15
        args.output.write_text(json.dumps(proof,indent=2)+'\n');print(json.dumps(proof,indent=2))
if __name__=='__main__':main()
