"""Bounded o32 replay against independent record-batch field semantics.

Authentic words are read from protected targets, never embedded here. External
message/command calls are synchronous controlled hooks, not real hardware code.
"""
U32=0xffffffff
ADDRESS=0x80010450
SP=0x70001000
RETURN=0x60000000
GLOBALS=0x80037fa0
PENDING,REQUESTS,ACK,MESSAGE=GLOBALS,0x80037fa8,0x80037fd8,0x80037fe0
COUNT,DMA,CURSOR,COMMAND=0x80038028,0x8003802c,0x80038038,0x8003803c
QUEUE,ALTERNATE=0x12000000,0x13000000

def need(v,why):
    if not v:raise ValueError(why)

def signed(v,bits=32):
    v&=(1<<bits)-1
    return v-(1<<bits) if v>>(bits-1) else v

class Memory:
    def __init__(self,other=None):
        sizes={GLOBALS:0x40,COUNT:2,DMA:4,CURSOR:8,QUEUE:12*12,ALTERNATE:12*12,
               QUEUE+65535*12:12,ALTERNATE+65535*12:12,SP-128:256}
        self.banks=({a:bytearray([0xa5])*n for a,n in sizes.items()} if other is None else
                    {a:bytearray(b) for a,b in other.banks.items()})
    def locate(self,a,n):
        need(a%n==0,('alignment',hex(a),n))
        for base,data in self.banks.items():
            if base<=a and a+n<=base+len(data):return data,a-base
        raise ValueError(('unmapped',hex(a),n))
    def get(self,a,n):
        data,at=self.locate(a,n);return int.from_bytes(data[at:at+n],'big')
    def put(self,a,v,n):
        data,at=self.locate(a,n);data[at:at+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')
    def snapshot(self):return tuple((a,bytes(b)) for a,b in self.banks.items() if a!=SP-128)

def fixture(pending,ack,seed,send_mode,write_mode,second_zero):
    m=Memory()
    m.put(PENDING,pending,1);m.put(ACK,ack,1)
    m.put(COUNT,(0,1,7,65535)[seed%4],2);m.put(DMA,QUEUE,4)
    m.put(CURSOR,(0x12345000,0xfffffffc)[seed%2],4);m.put(COMMAND,(0,0x87654321)[seed%2],4)
    for i in range(4):
        m.put(REQUESTS+i*12,0x23456000+i*256+seed,4)
        m.put(REQUESTS+i*12+4,(0x87650000+i*256+seed)&U32,4)
        m.put(REQUESTS+i*12+8,(0,16,0xffffffff,0x80000000)[(i+seed)%4],4)
    return m,dict(send_mode=send_mode,write_mode=write_mode,second_zero=second_zero,
                  answer=(0,0xffffffff,0xabcdef00)[seed%3])

def send(m,args,s,events):
    events.append(('send',tuple(args),m.snapshot()))
    if s['send_mode']==1:m.put(PENDING,0,1)
    if s['send_mode']==2:m.put(PENDING,4,1)
    return s['answer']

def writer(m,args,s,events):
    events.append(('writer',tuple(args),m.snapshot()))
    if s['write_mode']&1:m.put(COUNT,3,2)
    if s['write_mode']&2:m.put(DMA,ALTERNATE,4)
    if s['write_mode']&4:
        # First iteration terminates after recording this live request.
        m.put(PENDING,0,1);m.put(REQUESTS+4,0xfedcba98,4)
    return s['answer']

def oracle(original,s):
    m=Memory(original);events=[]
    if m.get(ACK,1):send(m,[MESSAGE,0,0],s,events)
    initial=m.get(PENDING,1)
    if initial:
        if s['second_zero']:m.put(PENDING,0,1)
        # Index-based record mapping deliberately avoids native control-flow shape.
        for i in range(4):
            if i>=m.get(PENDING,1):break
            cursor=(m.get(CURSOR,4)+12)&U32;m.put(CURSOR,cursor,4)
            result=writer(m,[m.get(COMMAND,4),cursor],s,events);m.put(COMMAND,result,4)
            record=m.get(DMA,4)+m.get(COUNT,2)*12
            request=REQUESTS+i*12
            for out,inp in ((4,4),(0,8),(8,0)):m.put(record+out,m.get(request+inp,4),4)
            m.put(COUNT,m.get(COUNT,2)+1,2)
        m.put(PENDING,0,1);m.put(ACK,1,1)
    else:m.put(ACK,0,1)
    return events,m.snapshot()

def execute(words,original,s):
    m=Memory(original);regs=[0x1234567*(i+1)&U32 for i in range(32)]
    regs[0]=0;regs[29]=SP;regs[31]=RETURN;initial=regs[:]
    events=[];coverage=set();branches=set();lo=0;pc=0;pending=None;count_reads=0
    for _ in range(3000):
        if pc==RETURN:
            need(pending is None,'pending return')
            need(all(regs[r]==initial[r] for r in list(range(16,24))+[28,29,30,31]),'o32 callee-saved corruption')
            return (events,m.snapshot()),coverage,branches
        need(pc%4==0 and 0<=pc<4*len(words),('PC',pc));coverage.add(pc)
        w=words[pc//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63
        imm=signed(w,16);a=(regs[rs]+imm)&U32;current,pending,nextpc=pending,None,pc+4
        if op==0:
            if fn==0:regs[rd]=regs[rt]<<sh&U32
            elif fn==0x21:regs[rd]=(regs[rs]+regs[rt])&U32
            elif fn==0x25:regs[rd]=regs[rs]|regs[rt]
            elif fn==0x2a:regs[rd]=int(signed(regs[rs])<signed(regs[rt]))
            elif fn==0x19:lo=(regs[rs]*regs[rt])&U32
            elif fn==0x12:regs[rd]=lo
            elif fn==8:
                need(rs==31,'indirect jump');pending=('jump',regs[rs])
            else:raise ValueError(('unknown SPECIAL',fn))
        elif op==9:regs[rt]=(regs[rs]+imm)&U32
        elif op==12:regs[rt]=regs[rs]&(w&65535)
        elif op==15:regs[rt]=(w&65535)<<16
        elif op in (0x23,0x25,0x24):
            if a==PENDING and op==0x24:
                count_reads+=1
                if count_reads==2 and s['second_zero']:m.put(PENDING,0,1)
            regs[rt]=m.get(a,{0x23:4,0x25:2,0x24:1}[op])
        elif op in (0x2b,0x29,0x28):m.put(a,regs[rt],{0x2b:4,0x29:2,0x28:1}[op])
        elif op in (4,5,6):
            need(current is None,'branch in delay slot')
            taken=(regs[rs]==regs[rt] if op==4 else regs[rs]!=regs[rt] if op==5 else signed(regs[rs])<=0)
            if not(op==4 and rs==rt==0):branches.add((pc,taken))
            pending=('jump',pc+4+imm*4 if taken else pc+8)
        elif op==3:
            need(current is None,'call in delay slot');regs[31]=ADDRESS+pc+8
            pending=('call',0x80000000|((w&0x3ffffff)<<2))
        else:raise ValueError(('unknown opcode',op))
        regs[0]=0
        if current:
            need(pending is None,'control in delay slot');kind,dest=current
            if kind=='jump':nextpc=dest
            else:
                if dest==0x8002c5e0:answer=writer(m,regs[4:6],s,events)
                elif dest==0x800075e0:answer=send(m,regs[4:7],s,events)
                else:raise ValueError(('wrong helper',hex(dest)))
                for r in [1,2,3]+list(range(4,16))+[24,25]:regs[r]=(0xdeadbeef^(r*0x1020304)^len(events))&U32
                regs[2]=answer;lo=0xdeadcafe
        pc=nextpc
    raise ValueError('instruction budget')

def cases():
    for p in range(5):
        for ack in (0,1,255):
            for seed in range(6):
                for send_mode in range(3):
                    for write_mode in (0,1,2,4,7):
                        yield p,ack,seed,send_mode,write_mode,0
    yield 2,1,0,0,0,1

def prove(bodies):
    coverage={n:set() for n in bodies};branches={n:set() for n in bodies};count=0
    for case in cases():
        m,s=fixture(*case);expected=oracle(m,s)
        for name,words in bodies.items():
            actual,seen,edges=execute(words,m,s);need(actual==expected,('behavior/ABI',name,case))
            coverage[name].update(seen);branches[name].update(edges)
        count+=1
    return dict(fixtures=count,executions=count*len(bodies),covered_instructions={n:len(v) for n,v in coverage.items()},
                unexecuted_offsets={n:sorted(set(range(0,len(bodies[n])*4,4))-v) for n,v in coverage.items()},
                conditional_outcomes={n:sorted([list(x) for x in v]) for n,v in branches.items()})
