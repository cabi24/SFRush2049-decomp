"""Finite prefix replay: range planning only, stopping before format dispatch.

No native bytes are embedded. Floating emulation covers only finite nonnegative
positions representable as u32, the ordinary floor result, and the emitted
unsigned conversion fallback. Helper implementations and the remaining body
are outside this bounded proof.
"""
import math
import random
import struct
U32=0xffffffff
SP=0x70001000
AUDIO=0x10000000
BLOCK=0x11000000
INDEX=0x12000000
BASE=0x80012d18

def need(x,why):
    if not x:raise ValueError(why)
def signed(x,b=32):
    x&=(1<<b)-1
    return x-(1<<b) if x>>(b-1) else x

def oracle(c):
    rate,pos,length,start,loop,pending,tail=c
    position=int(pos);frames=((rate*192+4095)>>12)+6;end=(start+loop)&U32
    off=0;state=2;second=0;second_addr=None
    if loop and (pending or not tail) and end>=position:
        if end>=((position+frames)&U32):
            branch='fit';address=position;count=frames-6;mode=-1
        elif start>=position:
            branch='lead';address=position;count=(start-position+loop)&U32;mode=signed(start-position)
        elif loop<((frames-end+position)&U32) or ((frames-end+position)&U32)>=((position-start)&U32):
            branch='whole';address=start;count=loop;mode=0;off=(position-start)&U32
        else:
            branch='wrap';address=position;count=(end-position)&U32;second=(frames-end+position-6)&U32;second_addr=start;mode=0
    elif ((position+frames)&U32)>=length:
        branch='tail';address=position;count=(length-position+32)&U32;mode=-2;state=0
    else:
        branch='plain';address=position;count=frames-6;mode=-1
    return dict(address=address,count=count,second=second,second_addr=second_addr if second else None,
                padded=(count+6)&U32,second_padded=(second+6)&U32 if second else None,
                offset=off,mode=mode,state=state),branch

class Memory:
    def __init__(self):
        self.banks={SP-512:bytearray([0xa5])*768,AUDIO:bytearray(128),BLOCK:bytearray(0x3000),INDEX:bytearray(2),0x80038030:bytearray(8),0x800382d0:bytearray(8)}
    def loc(self,a,n):
        need(a%n==0,('alignment',a,n))
        for base,b in self.banks.items():
            if base<=a and a+n<=base+len(b):return b,a-base
        raise ValueError(('unmapped',hex(a),n))
    def get(self,a,n):
        b,i=self.loc(a,n);return int.from_bytes(b[i:i+n],'big')
    def put(self,a,v,n):
        b,i=self.loc(a,n);b[i:i+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')

def execute(words,c,layout):
    rate,pos,length,start,loop,pending,tail=c;m=Memory()
    for a,v,n in [(AUDIO+4,rate,2),(AUDIO+8,int.from_bytes(struct.pack('>d',pos),'big'),8),(AUDIO+48,length,4),(AUDIO+52,start,4),(AUDIO+56,loop,4),(AUDIO+1,pending,1),(AUDIO+60,tail,4),(0x800382d0,BLOCK,4),(0x800382d4,INDEX,4),(INDEX,1,2),(BLOCK+2576,3,2),(0x80038030,0xffffffe0,4),(0x80038034,0x12345678,4)]:m.put(a,v,n)
    reg=[0x1234567*(i+1)&U32 for i in range(32)];fp=[0]*32
    reg[0]=0;reg[4]=AUDIO;reg[5]=192;reg[6]=7;reg[29]=SP;reg[31]=0x60000000
    fcsr=0;seen=set();events=[];pc=0
    stop=[i*4 for i,w in enumerate(words) if w>>26==0x25 and w>>21&31==16 and w&65535==2]
    need(len(stop)==1,'unique flags dispatch boundary');stop=stop[0]
    def double(r):return struct.unpack('>d',struct.pack('>II',fp[r+1],fp[r]))[0]
    def setdouble(r,v):fp[r+1],fp[r]=struct.unpack('>II',struct.pack('>d',v))
    def operation(at,delay=False):
        nonlocal fcsr
        need(at%4==0 and 0<=at<4*len(words),'PC');seen.add(at)
        w=words[at//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=w&65535;si=signed(imm,16);branch=None
        if op==0:
            if fn==0:reg[rd]=reg[rt]<<sh&U32
            elif fn==2:reg[rd]=reg[rt]>>sh
            elif fn==3:reg[rd]=signed(reg[rt])>>sh&U32
            elif fn==33:reg[rd]=(reg[rs]+reg[rt])&U32
            elif fn==35:reg[rd]=(reg[rs]-reg[rt])&U32
            elif fn==37:reg[rd]=reg[rs]|reg[rt]
            elif fn==43:reg[rd]=int(reg[rs]<reg[rt])
            else:raise ValueError(('unknown SPECIAL',fn))
        elif op==1:
            need(rt in (0,1,2,3),'REGIMM');branch=('branch',signed(reg[rs])<0 if rt in (0,2) else signed(reg[rs])>=0,at+4+si*4,rt in (2,3))
        elif op in (4,5,20,21):branch=('branch',reg[rs]==reg[rt] if op in (4,20) else reg[rs]!=reg[rt],at+4+si*4,op>=20)
        elif op==3:reg[31]=BASE+at+8;branch=('call',0x80000000|((w&0x3ffffff)<<2))
        elif op==9:reg[rt]=(reg[rs]+si)&U32
        elif op==12:reg[rt]=reg[rs]&imm
        elif op==15:reg[rt]=imm<<16
        elif op in (35,36,37,40,41,43,53):
            a=(reg[rs]+si)&U32
            if op in (35,36,37):reg[rt]=m.get(a,{35:4,36:1,37:2}[op])
            elif op==53:
                v=m.get(a,8);fp[rt+1]=v>>32;fp[rt]=v&U32
            else:m.put(a,reg[rt],{40:1,41:2,43:4}[op])
        elif op==17:
            if rs==0:reg[rt]=fp[rd]
            elif rs==4:fp[rd]=reg[rt]
            elif rs==2:need(rd==31,'FCSR');reg[rt]=fcsr
            elif rs==6:need(rd==31,'FCSR');fcsr=reg[rt]
            elif rs==17:
                if fn==1:setdouble(sh,double(rd)-double(rt))
                elif fn==36:
                    need(fcsr&3==1,'truncate mode');value=math.trunc(double(rd))
                    if not -2147483648<=value<=2147483647:fcsr|=0x40;fp[sh]=0x80000000
                    else:fp[sh]=value&U32
                else:raise ValueError(('unknown double operation',fn))
            else:raise ValueError(('unknown COP1',rs))
        else:raise ValueError(('unknown opcode',op))
        reg[0]=0
        if delay:need(branch is None,'control in delay slot')
        return branch
    for _ in range(600):
        if pc==stop:break
        branch=operation(pc)
        if branch is None:pc+=4;continue
        if branch[0]=='branch':
            _,taken,dest,likely=branch
            if taken or not likely:operation(pc+4,True)
            pc=dest if taken else pc+8
        else:
            operation(pc+4,True);dest=branch[1]
            if dest==0x8002c5e0:
                need(reg[4:6]==[0x12345678,0x30],'command helper ABI');answer=0x87654321
            elif dest==0x8001e688:
                need(double(12)==pos,'floor helper ABI');setdouble(0,float(math.floor(pos)));answer=0xdead1234
            else:raise ValueError(('unexpected helper',hex(dest)))
            events.append(dest)
            for r in [1,2,3]+list(range(4,16))+[24,25]:reg[r]=(0xdeadbeef^r*12345)&U32
            reg[2]=answer;pc+=8
    else:raise ValueError('instruction budget')
    need(events==[0x8002c5e0,0x8001e688] and reg[16]==AUDIO and reg[29]==SP-layout['frame'] and fcsr==0,'prefix call/stack/FCSR contract')
    need(m.get(0x80038030,4)==0x30 and m.get(0x80038034,4)==0x87654321,'prefix command writes')
    need(m.get(reg[29]+layout['cmd'],4)==BLOCK+2576+16+3*80,'command pointer home')
    get=lambda name,n=4:m.get(reg[29]+layout[name],n)
    second=m.get(reg[29]+layout['length']+4,4)
    actual=dict(address=get('address'),count=get('length'),second=second,
                second_addr=m.get(reg[29]+layout['address']+4,4) if second else None,
                padded=get('padded'),second_padded=m.get(reg[29]+layout['padded']+4,4) if second else None,
                offset=reg[13],mode=signed(reg[12]),state=get('state',1))
    return actual,seen

def cases():
    # Deliberate six-leaf examples plus boundary and high-bit unsigned cases.
    for rate in (0,1,4096,16384,65535):
        for position in (0.,20.25,99.5,100.,110.75,127.,128.,140.,200.,2147483647.,2147483648.,4294967000.):
            for start,loop in ((0,0),(0,128),(100,128),(100,16),(0x80000000,128),(0xfffffff0,32)):
                for pending,tail in ((0,0),(0,1),(1,1)):
                    yield rate,position,400,start,loop,pending,tail
    rng=random.Random(0x12d18)
    for _ in range(200):yield rng.randrange(65536),float(rng.randrange(20000))+0.25,rng.randrange(20000),rng.randrange(20000),rng.randrange(3000),rng.randrange(2),rng.randrange(2)

def prove(bodies):
    coverage={n:set() for n in bodies};leaves=set();count=0
    for c in cases():
        expected,leaf=oracle(c);leaves.add(leaf)
        for n,(words,layout) in bodies.items():
            actual,seen=execute(words,c,layout);need(actual==expected,('planner behavior',n,c,actual,expected));coverage[n].update(seen)
        count+=1
    need(leaves=={'fit','lead','whole','wrap','tail','plain'},'six range leaves')
    return dict(fixtures=count,executions=count*len(bodies),range_leaves=sorted(leaves),prefix_executed_instructions={n:len(v) for n,v in coverage.items()},full_body_equivalence=False)
