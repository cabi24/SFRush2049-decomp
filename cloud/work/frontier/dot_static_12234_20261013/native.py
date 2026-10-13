"""Finite o32 machine replay with mapped-memory and instruction fail-closed checks.

Native words come from protected repository targets, never from this packet.
The external command writer is a bounded synchronous hook, not the real helper.
"""
U32 = 0xffffffff
ADDRESS = 0x80012234
SP = 0x70001000
RETURN = 0x60000000
HEAD, FREE, COUNT, DMA, CURSOR, COMMAND = 0x80038344, 0x80038348, 0x80038028, 0x8003802c, 0x80038038, 0x8003803c
NODES, BUFFERS, QUEUE, ALTERNATE = 0x10000000, 0x11000000, 0x12000000, 0x13000000


def need(v, why):
    if not v:
        raise ValueError(why)


def signed(v, bits=32):
    v &= (1 << bits) - 1
    return v - (1 << bits) if v >> (bits - 1) else v


class Memory:
    def __init__(self, other=None):
        sizes = {HEAD:8, COUNT:2, DMA:4, CURSOR:8, NODES:20*12,
                 BUFFERS:256*12, QUEUE:12*12, ALTERNATE:12*12, SP-128:256}
        self.banks = ({a:bytearray([0xa5])*n for a,n in sizes.items()} if other is None else
                      {a:bytearray(b) for a,b in other.banks.items()})

    def locate(self, a, n):
        need(a % n == 0, ('alignment', hex(a), n))
        for base, data in self.banks.items():
            if base <= a and a+n <= base+len(data):
                return data,a-base
        raise ValueError(('unmapped',hex(a),n))

    def get(self,a,n):
        data,at=self.locate(a,n)
        return int.from_bytes(data[at:at+n],'big')

    def put(self,a,v,n):
        data,at=self.locate(a,n)
        data[at:at+n]=(v&((1<<(8*n))-1)).to_bytes(n,'big')

    def snapshot(self):
        return tuple((a,bytes(b)) for a,b in self.banks.items() if a!=SP-128)


def fixture(active, free, hit, expired, seed, mutation):
    m=Memory()
    key=(0,0xffffffff,0x12345678,0x80000000)[seed%4]
    for i in range(active+free):
        start=0 if i<active else active
        end=active if i<active else active+free
        p=NODES+i*20
        m.put(p,NODES+(i+1)*20 if i+1<end else 0,4)
        m.put(p+4,NODES+(i-1)*20 if i>start else 0,4)
        m.put(p+8,BUFFERS+i*256,4)
        # A later duplicate verifies first-match selection.
        m.put(p+12,key if i==hit or (hit>=0 and i==hit+1 and seed&1) else key^0x11223344,4)
        m.put(p+16,0 if i==expired else (1,2,65535)[(i+seed)%3],2)
    m.put(HEAD,NODES if active else 0,4)
    m.put(FREE,NODES+active*20 if free else 0,4)
    m.put(DMA,QUEUE,4)
    m.put(COUNT,(0,1,7)[seed%3],2)
    m.put(CURSOR,(0x12345000,0xfffffffc)[seed%2],4)
    m.put(COMMAND,(0,0x87654321)[seed%2],4)
    return m,dict(key=key,mutation=mutation,answer=(0,0xffffffff,0xabcdef00)[seed%3])


def helper(m,args,s,events,node):
    events.append((tuple(args),m.snapshot()))
    if s['mutation']&1:
        m.put(COUNT,3,2)
    if s['mutation']&2:
        m.put(DMA,ALTERNATE,4)
    if s['mutation']&4:
        m.put(node+8,BUFFERS+256*11,4)
    return s['answer']


def oracle(original,s):
    m=Memory(original);events=[]
    # Build a finite index view, then select; this is independent of native flow.
    nodes=[];p=m.get(HEAD,4)
    while p:
        need(p not in nodes,'cycle')
        nodes.append(p);p=m.get(p,4)
    matches=[p for p in nodes if m.get(p+12,4)==s['key']]
    if matches:
        selected=matches[0];m.put(selected+16,2,2)
        return (m.get(selected+8,4),events,m.snapshot())
    selected=m.get(FREE,4)
    if selected:
        following=m.get(selected,4);m.put(FREE,following,4)
        if following:m.put(following+4,0,4)
        prior=m.get(HEAD,4)
        if prior:m.put(prior+4,selected,4)
        m.put(selected,prior,4);m.put(HEAD,selected,4)
    else:
        available=[p for p in nodes if m.get(p+16,2)==0]
        need(available,'exhausted fixture')
        selected=available[0]
    m.put(selected+16,2,2);m.put(selected+12,s['key'],4)
    cursor=(m.get(CURSOR,4)+12)&U32;m.put(CURSOR,cursor,4)
    result=helper(m,[m.get(COMMAND,4),cursor],s,events,selected)
    m.put(COMMAND,result,4)
    record=m.get(DMA,4)+m.get(COUNT,2)*12
    m.put(record+4,s['key'],4);m.put(record,256,4);m.put(record+8,m.get(selected+8,4),4)
    m.put(COUNT,m.get(COUNT,2)+1,2)
    return (m.get(selected+8,4),events,m.snapshot())


def execute(words,original,s):
    m=Memory(original)
    regs=[0x1234567*(i+1)&U32 for i in range(32)]
    regs[0]=0;regs[4]=s['key'];regs[29]=SP;regs[31]=RETURN
    initial=regs[:];events=[];coverage=set();branches=set();lo=0
    pc=0;pending=None
    for _ in range(2000):
        if pc==RETURN:
            need(pending is None,'pending return')
            need(all(regs[r]==initial[r] for r in list(range(16,24))+[28,29,30,31]),'o32 callee-saved corruption')
            return (regs[2],events,m.snapshot()),coverage,branches
        need(pc%4==0 and 0<=pc<4*len(words),('PC',pc))
        coverage.add(pc)
        w=words[pc//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63
        imm=signed(w,16);a=(regs[rs]+imm)&U32
        current,pending,nextpc=pending,None,pc+4
        if op==0:
            if fn==0:regs[rd]=regs[rt]<<sh&U32
            elif fn==0x21:regs[rd]=(regs[rs]+regs[rt])&U32
            elif fn==0x25:regs[rd]=regs[rs]|regs[rt]
            elif fn==0x19:lo=(regs[rs]*regs[rt])&U32
            elif fn==0x12:regs[rd]=lo
            elif fn==8:
                need(rs==31,'indirect jump');pending=('jump',regs[rs])
            else:raise ValueError(('unknown SPECIAL',fn))
        elif op==9:regs[rt]=(regs[rs]+imm)&U32
        elif op==15:regs[rt]=(w&65535)<<16
        elif op in (0x23,0x25):regs[rt]=m.get(a,4 if op==0x23 else 2)
        elif op in (0x2b,0x29):m.put(a,regs[rt],4 if op==0x2b else 2)
        elif op in (4,5,0x14,0x15):
            need(current is None,'branch in delay slot')
            taken=regs[rs]==regs[rt] if op in (4,0x14) else regs[rs]!=regs[rt]
            branches.add((pc,taken))
            if op in (0x14,0x15) and not taken:nextpc=pc+8
            else:pending=('jump',pc+4+imm*4 if taken else pc+8)
        elif op==3:
            need(current is None,'call in delay slot');regs[31]=ADDRESS+pc+8
            pending=('call',0x80000000|((w&0x3ffffff)<<2))
        else:raise ValueError(('unknown opcode',op))
        regs[0]=0
        if current:
            need(pending is None,'control in delay slot')
            kind,dest=current
            if kind=='jump':nextpc=dest
            else:
                need(dest==0x8002c5e0,'wrong helper')
                selected=m.get(regs[29]+28,4)
                answer=helper(m,regs[4:6],s,events,selected)
                for r in [1,2,3]+list(range(4,16))+[24,25]:regs[r]=(0xdeadbeef^(r*0x1020304)^len(events))&U32
                regs[2]=answer;lo=0xdeadcafe
        pc=nextpc
    raise ValueError('instruction budget')


def cases():
    for active in range(7):
        for free in (0,1,3):
            for hit in range(-1,active):
                for seed in range(6):
                    if hit<0 and free==0 and active==0:continue
                    expired=(seed%active) if active else -1
                    for mutation in (0,1,2,4,7):
                        yield (active,free,hit,expired,seed,mutation)


def prove(bodies):
    coverage={n:set() for n in bodies};branches={n:set() for n in bodies};count=0
    for case in cases():
        m,s=fixture(*case);expected=oracle(m,s)
        for name,words in bodies.items():
            actual,seen,edges=execute(words,m,s)
            need(actual==expected,('behavior/ABI',name,case))
            coverage[name].update(seen);branches[name].update(edges)
        count+=1
    # Halfword count wrap uses a sparse mapping of the two final records.
    m,s=fixture(2,1,-1,0,1,0);m.put(COUNT,65535,2)
    m.banks[QUEUE+65535*12]=bytearray([0xa5])*24
    expected=oracle(m,s)
    for name,words in bodies.items():
        actual,seen,edges=execute(words,m,s);need(actual==expected,('behavior/ABI',name,'count wrap'))
        coverage[name].update(seen);branches[name].update(edges)
    count+=1
    return dict(fixtures=count,executions=count*len(bodies),
                covered_instructions={n:len(v) for n,v in coverage.items()},
                unexecuted_offsets={n:sorted(set(range(0,len(bodies[n])*4,4))-v) for n,v in coverage.items()},
                conditional_outcomes={n:sorted([list(x) for x in v]) for n,v in branches.items()})
