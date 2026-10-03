"""Execute protected integer MIPS words and compare a host-compiled reconstruction.
Research-only differential check; no ROM, relocation, or matching acceptance claim.
"""
from pathlib import Path
import argparse, ctypes, json, random, struct, subprocess, sys, tempfile
parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('source',type=Path)
parser.add_argument('--repo',type=Path,required=True)
parser.add_argument('--output',type=Path)
ARGS=parser.parse_args()
sys.path.insert(0, str(ARGS.repo.resolve()))
from tools.cloud import score
N='func_800A1644'; WORK=tempfile.TemporaryDirectory(prefix='rush-encoder-check-'); P=Path(WORK.name)
TEXT=score.image_symbols()[N]; WORDS=score.targets()[N]
TABLE=0x8011EAEC; INPUT=0x100000; OUTPUT=0x200000; SP=0x300000; STOP=0x70000000

def signed(x):
    x &= 0xffffffff
    return x-0x100000000 if x & 0x80000000 else x

def execute(data, table, overlap=False):
    mem={}; regs=[0]*32; regs[4]=INPUT if overlap else OUTPUT; regs[5]=INPUT; regs[6]=len(data); regs[29]=SP; regs[31]=STOP
    def put(a, v, width):
        for j in range(width):mem[(a+j)&0xffffffff]=(v>>(8*(width-j-1)))&255
    def get(a, width):
        addresses=[(a+j)&0xffffffff for j in range(width)]
        if any(address not in mem for address in addresses):
            raise AssertionError(f'uninitialized native read at {a:x}, width {width}')
        return sum(mem[address]<<(8*(width-j-1)) for j,address in enumerate(addresses))
    for i in range(256):put(TABLE+2*i,table[i],2)
    for i in range(132):put(regs[4]+i,0xa5,1)
    for i,b in enumerate(data):put(INPUT+i,b,1)
    pc=TEXT; pending=None; steps=0
    while pc!=STOP:
        steps+=1
        if steps>10000:raise AssertionError('execution did not terminate')
        if pc<TEXT or pc>=TEXT+4*len(WORDS):raise AssertionError(f'bad PC {pc:x}')
        word=WORDS[(pc-TEXT)//4]; op=word>>26; rs=(word>>21)&31; rt=(word>>16)&31; rd=(word>>11)&31; sh=(word>>6)&31; fn=word&63; imm=word&65535; simm=imm-65536 if imm&32768 else imm
        delayed=pending; pending=None; nextpc=pc+4
        if op==0:
            if fn==0:regs[rd]=regs[rt]<<sh
            elif fn==3:regs[rd]=signed(regs[rt])>>sh
            elif fn==8:pending=regs[rs]
            elif fn==0x21:regs[rd]=regs[rs]+regs[rt]
            elif fn==0x23:regs[rd]=regs[rs]-regs[rt]
            elif fn==0x25:regs[rd]=regs[rs]|regs[rt]
            elif fn==0x2b:regs[rd]=int(regs[rs]<regs[rt])
            else:raise AssertionError(f'unsupported SPECIAL {fn:x}')
        elif op in (1,4,5,6,20,21):
            if op==1:
                assert rt in (0,1)
                take=signed(regs[rs])<0 if rt==0 else signed(regs[rs])>=0
            elif op in (4,20):take=regs[rs]==regs[rt]
            elif op in (5,21):take=regs[rs]!=regs[rt]
            else:take=signed(regs[rs])<=0
            if take:pending=pc+4+simm*4
            elif op in (20,21):nextpc=pc+8
        elif op==9:regs[rt]=regs[rs]+simm
        elif op==10:regs[rt]=int(signed(regs[rs])<simm)
        elif op==12:regs[rt]=regs[rs]&imm
        elif op==15:regs[rt]=imm<<16
        elif op in (35,36,37):regs[rt]=get(regs[rs]+simm,{35:4,36:1,37:2}[op])
        elif op in (40,43):put(regs[rs]+simm,regs[rt],{40:1,43:4}[op])
        else:raise AssertionError(f'unsupported opcode {op:x} at {pc:x}')
        regs=[x&0xffffffff for x in regs];regs[0]=0
        pc=delayed if delayed is not None else nextpc
    assert regs[29]==SP
    out=INPUT if overlap else OUTPUT
    return bytes(get(out+i,1) for i in range(132)),steps

def main():
    source=ARGS.source.resolve()
    text=source.read_text()+'\nu16 D_8011EAEC[256];\n'
    host=P/'host.c';host.write_text(text)
    subprocess.run(['cc','-std=c89','-O2','-Wall','-Wextra','-fPIC','-shared',str(host),'-o',str(P/'host.so')],check=True)
    lib=ctypes.CDLL(str(P/'host.so'));fn=getattr(lib,N);fn.argtypes=[ctypes.c_void_p,ctypes.c_void_p,ctypes.c_ubyte];fn.restype=None
    array=(ctypes.c_uint16*256).in_dll(lib,'D_8011EAEC')
    rng=random.Random(0xA1644)
    cases=[bytes(), bytes([0]*16),bytes([15]*16),bytes([65]*16),bytes([66]*16),bytes([255]*16)]
    cases += [bytes([x]) for x in range(256)]
    cases += [bytes([1]*i+[x]+[15]*(15-i)) for i in range(16) for x in range(256)]
    cases += [bytes(rng.randrange(256) for _ in range(rng.choice([0,4,16]))) for _ in range(1000)]
    count=0;maxsteps=0
    for table_id in range(2):
        table=[rng.randrange(65536) for _ in range(256)] if table_id else [(i*257+0x1234)&65535 for i in range(256)]
        for i,x in enumerate(table):array[i]=x
        for data in cases:
            for overlap in [False,True]:
                original,steps=execute(data,table,overlap);maxsteps=max(maxsteps,steps)
                inp=(ctypes.c_ubyte*132)(*([0xa5]*132));out=(ctypes.c_ubyte*132)(*([0xa5]*132))
                for i,x in enumerate(data):inp[i]=x
                dest=inp if overlap else out
                fn(dest,inp,len(data))
                got=bytes(dest)
                if got!=original:raise AssertionError((data.hex(),table_id,overlap,original.hex(),got.hex()))
                count+=1
    result={'function':N,'target_words':len(WORDS),'differential_cases':count,'maximum_native_steps':maxsteps,'source':str(source),'status':'PASS','scope':'deterministic randomized tables; caller-observed lengths 4/16 plus lengths 0/1; exact in-place and distinct buffers; not a ROM or compiler matching gate'}
    if ARGS.output:
        ARGS.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))
if __name__=='__main__':main()
