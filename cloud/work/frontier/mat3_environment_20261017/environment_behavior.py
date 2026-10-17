"""Bounded StartEnvironment model versus native and compiled MIPS.

CreateCar, StartCar, and NewPoly are deterministic call-boundary stubs. This is
not an execution of those callees or the N64 runtime. No native words are stored.
"""
from pathlib import Path
import itertools, random, struct, sys
ROOT=Path(__file__).resolve().parents[4]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
ENTRY=0x800B1B48
CREATE,START,ALLOC=0x800B15B4,0x800B0C48,0x800A78BC
SHADOW_CB,ARROW_CB=0x8008D120,0x8008C884
CARS,MODELS=0x80152818,0x8014A250
DUMMY,RESOURCE,POLYS=0x8011AD90,0x80161368,0x8015B268
SHADOW,WHITE,COLORS,TIME=0x8011AD8C,0x8011B574,0x8011B558,0x801543CC
STATE,PLAYERS,WINDOWS,RACE,LINK,TEAM=0x801174B4,0x8014A108,0x80151AD0,0x8014A110,0x80153E88,0x8012E67C
STACK,RETURN,MASK=0x81010000,0x81234560,0xffffffff

def require(ok,message):
    if not ok: raise ValueError(message)
def signed(x,n=32):
    x&=(1<<n)-1
    return x-(1<<n) if x>>(n-1) else x

def load(m,a,n):
    require(a%n==0 and all(a+i in m for i in range(n)),f'invalid read {a:x}/{n}')
    return int.from_bytes(bytes(m[a+i] for i in range(n)),'big')
def store(m,a,n,v):
    require(a%n==0 and all(a+i in m for i in range(n)),f'invalid write {a:x}/{n}')
    for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):m[a+i]=b

def fixtures():
    rng=random.Random(ENTRY)
    for mask,players,race,flag in itertools.product(range(64),(1,4),(2,6),(0,0x100)):
        yield dict(mask=mask,players=players,race=race,flag=flag,windows=rng.randrange(5),
                   owners=[rng.choice((0,6)) for _ in range(6)],teams=[rng.randrange(6) for _ in range(6)],
                   colors=[[rng.randrange(256) for _ in range(4)] for _ in range(6)],
                   shadow=[rng.randrange(256) for _ in range(4)],texture=[rng.randrange(65536) for _ in range(2)],
                   time=struct.unpack('>I',struct.pack('>f',rng.uniform(0,1000)))[0])

def state(c):
    regions=[(CARS,6*952),(MODELS,6*2056),(DUMMY,48),(RESOURCE,16),(POLYS,32*88),
             (SHADOW,4),(WHITE,4),(COLORS,24),(TIME,4),(STATE,4),(PLAYERS,2),(WINDOWS,2),
             (RACE,4),(LINK,6*8),(TEAM,6),(STACK-256,512)]
    m={a+i:0xa5 for a,n in regions for i in range(n)}
    for i in range(6):
        store(m,MODELS+i*2056+1992,2,bool(c['mask']&(1<<i)))
        store(m,LINK+i*8+7,1,c['owners'][i]);store(m,TEAM+i,1,c['teams'][i])
        for j in range(4):store(m,COLORS+i*4+j,1,c['colors'][i][j])
    for j in range(4):store(m,SHADOW+j,1,c['shadow'][j]);store(m,WHITE+j,1,255)
    for j in range(2):store(m,RESOURCE+12+j*2,2,c['texture'][j])
    for a,n,v in [(TIME,4,c['time']),(STATE,4,c['flag']),(PLAYERS,2,c['players']),
                  (WINDOWS,2,c['windows']),(RACE,4,c['race'])]:store(m,a,n,v)
    return m

def visible(m):return {a:v for a,v in m.items() if not STACK-256<=a<STACK+256}

def model(c):
    m=state(c);events=[];next_poly=0
    def new_poly(color,texture,flags):
        nonlocal next_poly
        events.append(('poly',4,DUMMY,texture,tuple(color),flags,1))
        address=POLYS+88*next_poly;next_poly+=1
        return address
    for slot in range(6):
        if c['mask']&(1<<slot):events.extend([('create',slot,15,1),('start',slot)])
    for slot in range(6):
        if not c['mask']&(1<<slot):continue
        v=CARS+952*slot+512
        store(m,v+20,4,SHADOW_CB);store(m,v+8,2,slot);store(m,v+16,4,c['time']);store(m,SHADOW+3,1,192)
        poly=new_poly(c['shadow'][:3]+[192],c['texture'][0],0x8200|(1 if c['flag']&0x100 else 15))
        store(m,v+6,2,(poly-POLYS)//88)
        if c['players']<2 or c['owners'][slot]!=6:continue
        v=CARS+952*slot+536
        color=c['colors'][c['teams'][slot] if c['race']==6 else slot]
        for target in range(c['windows']):
            if target==slot:continue
            store(m,v+20,4,ARROW_CB);store(m,v+8,2,slot);store(m,v+4,2,target)
            poly=new_poly(color,c['texture'][1],(1<<target)|(0x3200 if c['race']==6 else 0x1200))
            store(m,poly+6,2,1);store(m,v+6,2,(poly-POLYS)//88);v+=24
        while v<CARS+952*slot+584:
            store(m,v+20,4,0);store(m,v+6,2,-1);v+=24
    return visible(m),events

def execute(code,c):
    m=state(c);r=[0xa5000000+13*i for i in range(32)];f=[0xa6000000+17*i for i in range(32)];fbefore=f[:]
    r[0]=0;r[29]=STACK;r[31]=RETURN;before=r[:]
    pc=ENTRY;pending=None;lo=0;events=[];next_poly=0;coverage=set();branches=set()
    for step in range(20000):
        if pc==RETURN:break
        require(pc in code,f'escaped instruction {pc:x}')
        coverage.add(pc);w=code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=signed(w,16)
        a=(r[rs]+imm)&MASK;old=pending;pending=None;nxt=pc+4
        if op==0:
            if fn==0:r[rd]=r[rt]<<sh
            elif fn==3:r[rd]=signed(r[rt])>>sh
            elif fn==4:r[rd]=r[rt]<<(r[rs]&31)
            elif fn==8:pending=r[rs]
            elif fn==18:r[rd]=lo
            elif fn==26:
                divisor=signed(r[rt]);require(divisor!=0,'divide by zero');lo=int(signed(r[rs])/divisor)&MASK
            elif fn==33:r[rd]=r[rs]+r[rt]
            elif fn==35:r[rd]=r[rs]-r[rt]
            elif fn==37:r[rd]=r[rs]|r[rt]
            elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
            elif fn==43:r[rd]=int(r[rs]<r[rt])
            else:raise ValueError(f'unexpected SPECIAL {fn}')
        elif op==3:
            dest=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);r[31]=pc+8;pending=('call',dest,pc+8)
        elif op==9:r[rt]=a
        elif op==10:r[rt]=int(signed(r[rs])<imm)
        elif op==12:r[rt]=r[rs]&(w&65535)
        elif op==13:r[rt]=r[rs]|(w&65535)
        elif op==15:r[rt]=(w&65535)<<16
        elif op in (4,5,6,20,21,22):
            take=(r[rs]==r[rt]) if op in (4,20) else ((r[rs]!=r[rt]) if op in (5,21) else signed(r[rs])<=0)
            branches.add((pc,take))
            if take:pending=pc+4+imm*4
            elif op>=20:nxt+=4
        elif op in (32,33,35,36,37):
            n=4 if op==35 else (2 if op in (33,37) else 1);v=load(m,a,n);r[rt]=signed(v,8*n) if op in (32,33) else v
        elif op in (40,41,43):store(m,a,4 if op==43 else (2 if op==41 else 1),r[rt])
        elif op==49:f[rt]=load(m,a,4)
        elif op==57:store(m,a,4,f[rt])
        else:raise ValueError(f'unexpected opcode {op}')
        r=[x&MASK for x in r];r[0]=0
        require(old is None or pending is None,'nested branch delay slot')
        if isinstance(old,tuple):
            dest=old[1];result=0xbadfeed
            if dest==CREATE:
                require(r[4]<6 and r[5]==15 and r[6]==1,'CreateCar arguments');events.append(('create',r[4],r[5],r[6]))
            elif dest==START:
                require(r[4]<6,'StartCar slot');events.append(('start',r[4]))
            elif dest==ALLOC:
                flags=load(m,r[29]+16,4);indexed=load(m,r[29]+20,4)
                require(r[4]==4 and r[5]==DUMMY and indexed==1,'NewPoly arguments')
                events.append(('poly',r[4],r[5],r[6],tuple(load(m,r[7]+j,1) for j in range(4)),flags,indexed))
                result=POLYS+next_poly*88;next_poly+=1;require(next_poly<=32,'allocation bound')
            else:raise ValueError(f'unexpected call {dest:x}')
            for j in list(range(1,16))+[24,25]:r[j]=(0xbad00000+31*j)&MASK
            for j in range(20):f[j]=0xbad00000+j
            r[2]=result;pc=old[2]
        else:pc=old if old is not None else nxt
    else:raise ValueError('execution bound exceeded')
    require(r[16:24]==before[16:24] and r[28:]==before[28:] and f[20:]==fbefore[20:],'saved register/stack ABI')
    return visible(m),events,coverage,branches

def candidate(obj):
    obj=Path(obj);words=score.text_words(obj);start=score.symbols(obj)['music_control']
    data,sections=score._elf(obj);text_index=score._text_index(sections)
    found=[symbol for i,section in enumerate(sections) if section['type']==2 for symbol in score._symbol_table(data,sections,i) if symbol['name']=='music_control' and symbol['type']==2 and symbol['section']==text_index]
    require(len(found)==1,'ambiguous function extent');end=start+found[0]['size']
    got,masks,u,v,e=score.relocate(obj,words,start,end,score.image_symbols())
    require(not(masks or u or v or e),'uncertain relocation')
    return {ENTRY+4*i:w for i,w in enumerate(got[start//4:end//4])}

def verify(obj):
    native={ENTRY+4*i:w for i,w in enumerate(score.targets()['music_control'])};compiled=candidate(obj)
    coverage=[set(),set()];branches=[set(),set()];count=0
    for i,case in enumerate(fixtures()):
        expected=model(case)
        for j,code in enumerate((native,compiled)):
            got=execute(code,case)
            if got[:2]!=expected:
                different=[hex(a) for a in got[0] if got[0][a]!=expected[0][a]]
                raise ValueError(f'behavior mismatch program {j}, case {i}, bytes {different[:12]}, events match {got[1]==expected[1]}')
            coverage[j]|=got[2];branches[j]|=got[3]
        count+=1
    return dict(status='PASS',fixtures=count,executions=count*2,executed_words=[len(c) for c in coverage],branch_outcomes=[len(b) for b in branches],
                limits='Stable globals and deterministic CreateCar/StartCar/NewPoly stubs; no callee or runtime equivalence claim',matching_claim=False)

if __name__=='__main__':
    import json
    print(json.dumps(verify(sys.argv[1]),indent=2))
