"""Independent bounded MIPS interpreter, arithmetic oracle and C host contracts."""
import ctypes, hashlib, itertools, random, struct, subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent
ENTRY,SIZE=0x803A4134,524
OWNER,SP,STOP=0x81001000,0x81002000,0x81234560
RENAME,HIDDEN,UPDATE=0x800EF5B0,0x80094F88,0x80094EC8
COUNT,MODE,HIDES,VALUES,NAME=0x8014A108,0x8014A110,0x803BA028,0x803BA190,0x803B85D8
MASK=0xffffffff
FIELDS=(14,16,20,22,28,30,32,34)
def signed(v,bits=32):
    v&=(1<<bits)-1
    return v-(1<<bits) if v&(1<<(bits-1)) else v
def fbits(v):return struct.unpack('>I',struct.pack('>f',v))[0]
def fvalue(v):return struct.unpack('>f',struct.pack('>I',v))[0]
def f32(v):return fvalue(fbits(v))
def selected_value(bits,player,bar):return f32(fvalue(bits)*(0.5**(4*player+bar)))
def selected_hide(flag,player):return signed(flag+13*player,8)
def div8(v):return (-1 if v<0 else 1)*(abs(v)//8)
def initial(c):return [-321,654,c[8],c[9],-30000,1234,-5678,30000,c[7],1,signed((c[0]<<31)|(c[1]<<8)|(c[2]<<4)|c[3]|c[12])]
def oracle(c,portable=False):
    init,player,bar,segment,count,mode,hidden,hide,width,height,bits,hook,junk=c
    b=initial(c);trace=[];used=False
    def emit(event,arg):trace.extend([event,arg]+b+[count])
    def update():emit(3,0)
    def hide_call(value):
        nonlocal used
        emit(2,value)
        if b[8]!=value:
            b[8]=value;update()
            if not used:
                used=True
                if hook==2:b[2:4]=[-21,-17];b[10]=0x43210000
                if hook==3:b[8]=0
                if hook==4:b[8]=-1
        return b[8]
    if not init:
        if count>=3 and player<2:
            emit(1,0);b[:8]=[-21,17,123,-55,18,19,20,21];b[10]=0x12345678
            if hook==1:count=1;mode=0
        b[10]=signed(b[10]|0x80000000)
        if player<count and count==1 and mode!=2:
            b[0]=10;h=div8(b[3]);b[1]=signed(48+bar*2*h,16)
        else:
            hide_call(1);b[9]=0;emit(4,1);return trace
        b[6]=0;b[4]=signed((bar*2+int(segment==0))*h,16);b[5]=signed(b[4]+h-1,16)
    if hide_call(int(selected_hide(hidden,player)==1)):
        emit(4,1);return trace
    if segment==1:
        result=f32(f32(selected_value(bits,player,bar)*b[2])-1.0)
        assert -2147483648<=result<2147483648 and result==result
        if portable:assert -32769<result<32768,("outside direct C float-to-s16 domain",c,result)
        b[7]=max(2,signed(int(result),16))
    else:b[7]=signed(b[2]-1,16)
    update();emit(4,1);return trace

class Native:
    def __init__(self,words):
        assert len(words)==SIZE//4;self.words=words;self.coverage=set();self.branches=set()
    def run(self,c,salt=0):
        rng=random.Random(salt^0x4134);memory={}
        for lo,n in [(OWNER,64),(SP-64,96),(COUNT,2),(MODE,4),(HIDES,4),(VALUES,64)]:
            memory.update((a,rng.randrange(256)) for a in range(lo,lo+n))
        def mem(a,n,v=None):
            assert a%n==0 and all(a+i in memory for i in range(n)),('invalid memory',hex(a),n)
            if v is None:return int.from_bytes(bytes(memory[a+i] for i in range(n)),'big')
            for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):memory[a+i]=b
        b=initial(c)
        for off,v in zip(FIELDS,b[:8]):mem(OWNER+off,2,v)
        mem(OWNER+26,1,b[8]);mem(OWNER+40,4,0x11112220);mem(OWNER+44,4,b[10])
        mem(COUNT,2,c[4]);mem(MODE,4,c[5])
        for i in range(4):mem(HIDES+i,1,selected_hide(c[6],i))
        for i in range(16):mem(VALUES+i*4,4,fbits(selected_value(c[10],i//4,i%4)))
        before_memory=dict(memory);trace=[];used=False
        def emit(event,arg):
            trace.extend([event,arg]+[signed(mem(OWNER+off,2),16) for off in FIELDS]+[signed(mem(OWNER+26,1),8),int(mem(OWNER+40,4)!=0),signed(mem(OWNER+44,4)),signed(mem(COUNT,2),16)])
        r=[rng.getrandbits(32) for _ in range(32)];f=[rng.getrandbits(32) for _ in range(32)]
        r[0]=0;r[4]=OWNER;r[29]=SP;r[31]=STOP;before=list(r);saved_f=f[20:]
        pc=ENTRY;pending=None;lo=hi=0;steps=0
        allowed=set(range(SP-40,SP+4))|set(range(OWNER+14,OWNER+18))|set(range(OWNER+28,OWNER+36))|set(range(OWNER+40,OWNER+48))
        while pc!=STOP:
            assert steps<512;steps+=1
            if pc in (RENAME,HIDDEN,UPDATE):
                assert pending is None and r[4]==OWNER
                ret=r[31];rv=0xa5a5a5a5
                if pc==RENAME:
                    assert r[5]==NAME and r[6]==0;emit(1,0)
                    for off,val in zip(FIELDS,(-21,17,123,-55,18,19,20,21)):mem(OWNER+off,2,val)
                    mem(OWNER+44,4,0x12345678)
                    if c[11]==1:mem(COUNT,2,1);mem(MODE,4,0)
                elif pc==HIDDEN:
                    val=signed(r[5]);assert val in (0,1);emit(2,val)
                    if val!=signed(mem(OWNER+26,1),8):
                        mem(OWNER+26,1,val);emit(3,0)
                        if not used:
                            used=True
                            if c[11]==2:mem(OWNER+20,2,-21);mem(OWNER+22,2,-17);mem(OWNER+44,4,0x43210000)
                            if c[11]==3:mem(OWNER+26,1,0)
                            if c[11]==4:mem(OWNER+26,1,-1)
                    rv=signed(mem(OWNER+26,1),8)&MASK
                else:emit(3,0)
                for i in (1,2,3,*range(4,16),24,25):r[i]=rng.getrandbits(32)
                f[:20]=[rng.getrandbits(32) for _ in range(20)];lo=hi=rng.getrandbits(32)
                r[2]=rv;pc=ret;continue
            assert ENTRY<=pc<ENTRY+SIZE and pc%4==0,hex(pc)
            off=pc-ENTRY;self.coverage.add(off);w=self.words[off//4]
            op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=w&65535;si=signed(imm,16);address=(r[rs]+si)&MASK
            old,pending=pending,None;nextpc=pc+4
            if w==0:pass
            elif op==0:
                if fn==0:r[rd]=r[rt]<<sh
                elif fn==2:r[rd]=r[rt]>>sh
                elif fn==3:r[rd]=signed(r[rt])>>sh
                elif fn==8:pending=r[rs]
                elif fn==18:r[rd]=lo
                elif fn==25:lo=(r[rs]*r[rt])&MASK;hi=(r[rs]*r[rt])>>32
                elif fn==33:r[rd]=r[rs]+r[rt]
                elif fn==37:r[rd]=r[rs]|r[rt]
                elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
                else:raise AssertionError(('special',off,fn))
            elif op==1:
                assert rt==1;take=signed(r[rs])>=0;self.branches.add((off,take))
                if take:pending=pc+4+si*4
            elif op==3:
                r[31]=pc+8;pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);assert pending in (RENAME,HIDDEN,UPDATE)
            elif op in (4,5,20,21):
                take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt]);self.branches.add((off,take))
                if take:pending=pc+4+si*4
                elif op in (20,21):nextpc+=4
            elif op==9:r[rt]=address
            elif op==10:r[rt]=int(signed(r[rs])<si)
            elif op==11:r[rt]=int(r[rs]<(si&MASK))
            elif op==12:r[rt]=r[rs]&imm
            elif op==14:r[rt]=r[rs]^imm
            elif op==15:r[rt]=imm<<16
            elif op==17:
                if rs==0:r[rt]=f[rd]
                elif rs==4:f[rd]=r[rt]
                elif rs==20:assert fn==32;f[sh]=fbits(signed(f[rd]))
                elif rs==16:
                    if fn==1:f[sh]=fbits(fvalue(f[rd])-fvalue(f[rt]))
                    elif fn==2:f[sh]=fbits(fvalue(f[rd])*fvalue(f[rt]))
                    elif fn==13:
                        value=fvalue(f[rd]);assert -2147483648<=value<2147483648;f[sh]=int(value)&MASK
                    else:raise AssertionError(('float',off,fn))
                else:raise AssertionError(('cop1',off,rs))
            elif op in (32,33,35):
                n={32:1,33:2,35:4}[op];v=mem(address,n);r[rt]=signed(v,n*8) if n<4 else v
            elif op in (41,43):
                n={41:2,43:4}[op];assert all(address+i in allowed for i in range(n)),('store confinement',hex(address));mem(address,n,r[rt])
            elif op==49:f[rt]=mem(address,4)
            else:raise AssertionError(('opcode',off,op))
            r[:]=[v&MASK for v in r];r[0]=0;pc=old if old is not None else nextpc
        assert pending is None and f[20:]==saved_f
        assert all(r[i]==before[i] for i in (*range(16,24),28,29,30,31))
        mutable=allowed|set(range(OWNER+20,OWNER+24))|{OWNER+26}|set(range(COUNT,COUNT+2))|set(range(MODE,MODE+4))
        assert all(v==before_memory[a] for a,v in memory.items() if a not in mutable)
        emit(4,signed(r[2]));return trace

def cases():
    for init,player,bar,segment in itertools.product((0,1),range(4),range(4),range(16)):
        k=player*64+bar*16+segment
        for count,mode in ((-32768,0),(0,0),(1,0),(1,2),(2,0),(3,0),(4,2),(32767,-1)):
            yield (init,player,bar,segment,count,mode,(-128,0,1,127)[k%4],(-128,-1,0,1,127)[k%5],123,-55,fbits(.5),k%5,0x12345000)
    for height in (-32768,-32767,-9,-8,-7,-1,0,1,7,8,9,32767):
        for width,value in itertools.product((-32768,-32767,-2,-1,0,1,2,3,4,32767),(0,.0001,.1,.5,.9999,1)):
            result=f32(f32(width*f32(value))-1)
            if -32769<result<32768:
                yield (0,0,3,1,1,0,0,-1,width,height,fbits(value),0,0)
    for flag in range(-128,128):yield (1,0,0,1,1,0,flag,flag,123,55,fbits(.1),0,0)
    for player,bar,segment in itertools.product((0,1,3,4,15),(0,3,15),(0,1,15)):
        # Rejected uninitialized player IDs never index the player/value tables.
        if player>=4:yield (0,player,bar,segment,1,0,0,0,100,55,fbits(.5),0,0)
        elif segment!=1:yield (0,player,bar,segment,1,0,0,0,32767,-32768,fbits(.5),0,0)
    for value in (1.9999999,2.0,2.9999998,3.0,3.0000002,32767.0,32768.0,-32766.0,-32767.0):
        yield (1,0,0,1,1,0,0,0,1,55,fbits(value),0,0)

def host(directory,source=None,label='host'):
    out=directory/(label+'.so');cmd=['gcc','-std=c89','-O2','-Wall','-Wextra','-Werror','-shared','-fPIC','-ffp-contract=off','-fsanitize=undefined,bounds,float-cast-overflow','-fno-sanitize-recover=all']
    if source:cmd.append('-DCANDIDATE="'+str(source)+'"')
    subprocess.run(cmd+[str(HERE/'host.c'),'-o',str(out)],check=True,capture_output=True,text=True)
    lib=ctypes.CDLL(str(out));fn=lib.host_run;fn.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];fn.restype=ctypes.c_int
    def run(c):
        result=(ctypes.c_int*256)();n=fn((ctypes.c_int*13)(*c),result);return list(result[:n])
    return run

def verify(build,words,linked):
    native=Native(words);gnu=Native(linked);compiled=host(build);samples=list(cases());digest=hashlib.sha256()
    for i,c in enumerate(samples):
        want=oracle(c,portable=True);assert native.run(c,i)==want,('native',c);assert gnu.run(c,i+1)==want,('GNU',c);assert compiled(c)==want,('host',c,compiled(c),want);digest.update(repr(want).encode())
    assert set(range(0,SIZE,4))-native.coverage=={0x1E4},('uncovered',sorted(set(range(0,SIZE,4))-native.coverage))
    # Wider native conversion cases are deliberately excluded from C float->s16 claims.
    native_narrow=[]
    for value in (32769.,65536.,65537.,-32768.,-65535.,-65536.,1048576.):
        c=(1,0,0,1,1,0,0,0,1,55,fbits(value),0,0);want=oracle(c)
        assert native.run(c)==want and gnu.run(c)==want
        native_narrow.append({'input_f32':value,'right':want[-5]})
    return {'cases':len(samples),'native_executions':2*(len(samples)+len(native_narrow)),'coverage':len(native.coverage),'unreachable_offsets':['0x1e4'],'branches':sorted(native.branches),'trace_sha256':digest.hexdigest(),'native_only_narrowing':native_narrow},samples
