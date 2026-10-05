"""Fail-closed bounded MIPS-II leaf executor; no native instruction data embedded.

Inputs are protected native words or independently GNU-linked candidate words.
Models binary32 operations and ordinary delay slots, not FCSR or exceptions.
"""
import math
import struct

BASE=0x800D08E4
LITERAL=0x80124138
MODEL=0x100000
TIRE=0x110000
STACK=0x200100
RETURN=0x300000
MODEL_SIZE=2056
TIRE_SIZE=92
TIRE_OFFSET=1072


def f32(x):return struct.unpack('>f',struct.pack('>f',x))[0]
def bits(x):return struct.unpack('>I',struct.pack('>f',x))[0]


class Machine:
    def __init__(self,words,literal,model,tire,scale,slot=-1):
        assert len(model)==MODEL_SIZE and len(tire)==TIRE_SIZE and slot in (-1,0,1,2,3)
        self.tire=TIRE if slot<0 else MODEL+TIRE_OFFSET+slot*TIRE_SIZE
        self.code={BASE+4*i:w for i,w in enumerate(words)}
        self.memory={MODEL+i:b for i,b in enumerate(model)}
        self.memory.update({self.tire+i:b for i,b in enumerate(tire)})
        self.memory.update({STACK+i:(i*37)&255 for i in range(-96,64)})
        self.memory.update({LITERAL+i:b for i,b in enumerate(literal)})
        self.original=dict(self.memory)
        self.r=[0x51230000+i*131 for i in range(32)]
        self.r[0]=0;self.r[4]=MODEL;self.r[5]=self.tire;self.r[6]=bits(scale)
        self.r[29]=STACK;self.r[31]=RETURN
        self.fp=[0x3f900000+i*127 for i in range(32)]
        self.entry_r=self.r[:];self.entry_fp=self.fp[:]
        self.pc=BASE;self.pending=None;self.visited=set();self.writes=[];self.reads=[]

    def read(self,address,count):
        assert address%count==0,('unaligned read',hex(address),count)
        assert all(address+i in self.memory for i in range(count)),('unmapped read',hex(address))
        self.reads.append((self.pc,address,count))
        return int.from_bytes(bytes(self.memory[address+i] for i in range(count)),'big')

    def write(self,address,count,value):
        assert address%count==0 and all(address+i in self.memory for i in range(count))
        assert (self.tire+24<=address and address+count<=self.tire+72) or (STACK-32<=address and address+count<=STACK)
        self.writes.append((self.pc,address,count))
        for i,b in enumerate((value&((1<<(8*count))-1)).to_bytes(count,'big')):self.memory[address+i]=b

    def value(self,reg):return struct.unpack('>f',struct.pack('>I',self.fp[reg]))[0]

    def run(self):
        for _ in range(256):
            if self.pc==RETURN:break
            assert self.pc in self.code,('unmapped pc',hex(self.pc))
            self.visited.add(self.pc);w=self.code[self.pc];op=w>>26
            rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;fd=(w>>6)&31;fn=w&63
            imm=w&65535;imm=imm-65536 if imm&32768 else imm
            branch=None
            if w==0:pass
            elif op==0 and fn==8:branch=self.r[rs]
            elif op==9:self.r[rt]=(self.r[rs]+imm)&0xffffffff
            elif op==15:self.r[rt]=(w&65535)<<16
            elif op==49:self.fp[rt]=self.read((self.r[rs]+imm)&0xffffffff,4)
            elif op==57:self.write((self.r[rs]+imm)&0xffffffff,4,self.fp[rt])
            elif op in (53,61):
                assert rt%2==0 and rt<31
                address=(self.r[rs]+imm)&0xffffffff
                if op==61:self.write(address,8,(self.fp[rt]<<32)|self.fp[rt+1])
                else:
                    value=self.read(address,8);self.fp[rt]=value>>32;self.fp[rt+1]=value&0xffffffff
            elif op==17:
                if rs==4:self.fp[rd]=self.r[rt]
                elif rs==16 and fn in (0,2,3):
                    a,b=self.value(rd),self.value(rt)
                    if fn==0:v=a+b
                    elif fn==2:v=a*b
                    else:
                        assert b!=0.0,'division outside finite fixture domain'
                        v=a/b
                    v=f32(v);assert math.isfinite(v),'nonfinite fixture result'
                    self.fp[fd]=bits(v)
                else:raise AssertionError(('unsupported cop1',rs,fn,hex(self.pc)))
            else:raise AssertionError(('unsupported opcode',op,fn,hex(self.pc)))
            self.r[0]=0;old=self.pending
            assert old is None or branch is None,'branch in delay slot'
            self.pending=branch;self.pc=old if old is not None else self.pc+4
        else:raise AssertionError('step limit')
        assert self.pending is None and self.r[29]==STACK
        assert all(self.r[i]==self.entry_r[i] for i in list(range(16,24))+[28,30,31])
        assert self.fp[20:]==self.entry_fp[20:]
        writable=set(range(self.tire+24,self.tire+72))|set(range(STACK-32,STACK))
        assert all(self.memory[k]==v for k,v in self.original.items() if k not in writable)
        return (bytes(self.memory[MODEL+i] for i in range(MODEL_SIZE)),
                bytes(self.memory[self.tire+i] for i in range(TIRE_SIZE)))
