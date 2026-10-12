"""Small fail-closed MIPS-II interpreter for bounded callback ABI experiments."""
import struct
U32=0xffffffff
SP=0x70001000
RETURN=0x60000000
ADDRESS=0x8038c910

def need(value,message):
    if not value: raise ValueError(message)

def signed(value,bits=32):
    value &= (1<<bits)-1
    return value-(1<<bits) if value>>(bits-1) else value

class Memory:
    def __init__(self,seed=0,other=None):
        sizes={0x80152818:952*4,0x80139320:64*4,0x8014a250:2056*4,
               0x803943a4:269*12,0x8002eb90:4,0x80142972:2,SP-128:160}
        self.banks={a:bytearray(((i*71+seed*19)^0xa5)&255 for i in range(n)) for a,n in sizes.items()} if other is None else {a:bytearray(d) for a,d in other.banks.items()}
    def locate(self,a,n):
        need(a%n==0,('unaligned',hex(a),n))
        for base,data in self.banks.items():
            if base<=a and a+n<=base+len(data): return data,a-base
        raise ValueError(('unmapped',hex(a),n))
    def get(self,a,n):
        data,i=self.locate(a,n);return int.from_bytes(data[i:i+n],'big')
    def put(self,a,v,n):
        data,i=self.locate(a,n);data[i:i+n]=(v&((1<<(n*8))-1)).to_bytes(n,'big')
    def snapshot(self): return tuple((a,bytes(d)) for a,d in self.banks.items() if a!=SP-128)

def fixture(seed,player,active,mutate):
    m=Memory(seed);p=0x80152818+952*player;c=p+752
    m.put(c+6,-1 if active else seed&32767,2)
    m.put(0x8002eb90,int.from_bytes(struct.pack('>f',(seed-32)/8),'big'),4)
    result=signed(seed*15427+player*713,16)
    return m,dict(player=player,mutate=mutate,result=result,active=active)

def helper(m,address,args,scenario,events):
    player=scenario['player'];c=0x80152818+952*player+752;control=0x80139320+64*player;vehicle=0x8014a250+2056*player
    if address==0x800af5e0:
        need(args==[m.get(0x80142972,2),15,m.get(control,4),0x2000,player,U32,1],('creation ABI',args))
        need(m.get(c+4,2)==0 and m.get(c+8,2)==player and m.get(c+20,4)==0x8038aa8c,'callback fields at creation')
        need(m.get(c+16,4)==m.get(0x8002eb90,4),'timestamp at creation')
        events.append(('create',tuple(args),m.snapshot()))
        if scenario['mutate']:
            m.put(vehicle+8,m.get(vehicle+8,1)^255,1);m.put(c+4,123,2);m.put(c+6,-222,2);m.put(control+24,0x12345678,4)
        return scenario['result']&U32
    need(address==0x8008d6fc,'unknown helper')
    expected=[scenario['result']&U32,0x803943a4+(m.get(vehicle+8,1)+13)*12,0]
    need(args[:3]==expected,('position ABI',args[:3],expected))
    need(m.get(control+24,4)==scenario['result']&U32 and m.get(c+6,2)==scenario['result']&65535,'post-creation result stores')
    events.append(('position',tuple(args[:3]),m.snapshot()));return 0

def oracle(original,scenario):
    m=Memory(other=original);events=[];p=scenario['player'];c=0x80152818+952*p+752;control=0x80139320+64*p
    if signed(m.get(c+6,2),16)<0:
        m.put(c+4,0,2);m.put(c+20,0x8038aa8c,4);m.put(c+16,m.get(0x8002eb90,4),4);m.put(c+8,p,2)
        result=helper(m,0x800af5e0,[m.get(0x80142972,2),15,m.get(control,4),0x2000,p,U32,1],scenario,events)
        m.put(control+24,result,4);m.put(c+6,result,2)
        helper(m,0x8008d6fc,[signed(m.get(c+6,2),16)&U32,0x803943a4+(m.get(0x8014a250+2056*p+8,1)+13)*12,0],scenario,events)
    return m.snapshot(),events

def execute(words,original,scenario,upper):
    m=Memory(other=original);events=[];covered=set();regs=[0x13579bdf*(i+1)&U32 for i in range(32)];fp=[0]*32
    regs[0]=0;regs[4]=(upper<<16)|scenario['player'];regs[29]=SP;regs[31]=RETURN;initial=regs[:];pc=0;pending=None
    for _ in range(1000):
        if pc==RETURN:
            need(pending is None,'pending return')
            need(all(regs[r]==initial[r] for r in list(range(16,24))+[28,29,30,31]),'callee-saved corruption')
            return m.snapshot(),events,covered
        need(pc%4==0 and 0<=pc<len(words)*4,('bad PC',pc))
        covered.add(pc);w=words[pc//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;shift=w>>6&31;fn=w&63;imm=signed(w,16)
        a=(regs[rs]+imm)&U32;current,pending=pending,None;nextpc=pc+4
        if op==0:
            if fn==0:regs[rd]=regs[rt]<<shift&U32
            elif fn==3:regs[rd]=signed(regs[rt])>>shift&U32
            elif fn==0x21:regs[rd]=(regs[rs]+regs[rt])&U32
            elif fn==0x23:regs[rd]=(regs[rs]-regs[rt])&U32
            elif fn==0x25:regs[rd]=regs[rs]|regs[rt]
            elif fn==8:pending=('jump',regs[rs]);need(rs==31,'unexpected indirect jump')
            else:raise ValueError(('unknown SPECIAL',fn))
        elif op==9:regs[rt]=(regs[rs]+imm)&U32
        elif op==15:regs[rt]=(w&65535)<<16
        elif op in (0x21,0x24,0x25,0x23):
            n={0x21:2,0x24:1,0x25:2,0x23:4}[op];v=m.get(a,n);regs[rt]=(signed(v,16) if op==0x21 else v)&U32
        elif op in (0x29,0x2b):m.put(a,regs[rt],2 if op==0x29 else 4)
        elif op==0x31:fp[rt]=m.get(a,4)
        elif op==0x39:m.put(a,fp[rt],4)
        elif op==1:
            need(rt==1 and current is None,'unsupported REGIMM');pending=('jump',pc+4+imm*4 if signed(regs[rs])>=0 else pc+8)
        elif op==3:
            need(current is None,'nested delay call');regs[31]=ADDRESS+pc+8;pending=('call',0x80000000|((w&0x3ffffff)<<2))
        else:raise ValueError(('unsupported opcode',op))
        regs[0]=0
        if current:
            need(pending is None,'control in delay slot');kind,dest=current
            if kind=='jump':nextpc=dest
            else:
                args=regs[4:8]+[m.get(regs[29]+i,4) for i in (16,20,24)]
                result=helper(m,dest,args,scenario,events)
                for r in [1,2,3]+list(range(4,16))+[24,25]:regs[r]=(0xabcdef00^r*0x1020304^len(events))&U32
                for r in range(20):fp[r]=0xdead0000+r
                regs[2]=result
        pc=nextpc
    raise ValueError('execution budget')

def prove(bodies):
    covered={n:set() for n in bodies};cases=0
    for seed in range(64):
        for player in range(4):
            for active in (False,True):
                for mutate in (False,True):
                    m,s=fixture(seed,player,active,mutate);expected=oracle(m,s)
                    for n,words in bodies.items():
                        state,events,sites=execute(words,m,s,(0,0x5555,0xffff)[seed%3])
                        need((state,events)==expected,('native/compiled fixture divergence',n,seed,player,active,mutate));covered[n]|=sites
                    cases+=1
    need(all(len(covered[n])==len(words) for n,words in bodies.items()),'unexecuted callback instruction')
    try:execute([0xffffffff],fixture(0,0,True,False)[0],fixture(0,0,True,False)[1],0)
    except ValueError:pass
    else:raise ValueError('unknown instruction accepted')
    return dict(fixtures=cases,executions=cases*len(bodies),complete_instruction_coverage={n:len(v) for n,v in covered.items()},domain='four backed players; sampled model bytes; signed16 helper results; synchronous side-effecting contract hooks')
