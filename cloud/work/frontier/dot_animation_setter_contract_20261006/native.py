"""Bounded, fail-closed MIPS-II animation replay. No target instructions retained.

Finite binary32 arithmetic only. External callbacks are explicit O32 contracts;
their actual implementations and asynchronous clock mutation are not simulated.
"""
import struct

MASK = 0xffffffff
NODE, MODEL, STACK, STOP = 0x600000, 0x601000, 0x700080, 0xfffffffc


def signed(x, width=32):
    x &= (1 << width)-1
    return x-(1 << width) if x & (1 << (width-1)) else x


def bits(x): return struct.unpack('>I', struct.pack('>f', x))[0]
def floating(x): return struct.unpack('>f', struct.pack('>I', x))[0]
def rounded(x): return floating(bits(x))


def get(m, a, n):
    assert a % n == 0 and all(a+i in m for i in range(n)), ('unmapped read', hex(a), n)
    return int.from_bytes(bytes(m[a+i] for i in range(n)), 'big')


def put(m, a, n, v):
    assert a % n == 0 and all(a+i in m for i in range(n)), ('unmapped write', hex(a), n)
    for i, b in enumerate((v & ((1 << (8*n))-1)).to_bytes(n, 'big')): m[a+i] = b


def initial(case, a):
    mode, hold, step, timer, delta, count, base, frame, flags, ident, kind, mutation = case
    m = {}
    for start, size in [(NODE,24),(MODEL,92),(STACK-64,128),
                        (a['D_801170FC'],4),(a['D_8002EB94'],4),
                        (a['D_80117530'],48*8),(a['D_801427C0'],512),
                        (a['D_8012E700'],68*8)]:
        assert all(start+i not in m for i in range(size)), 'overlapping fixture regions'
        m.update({start+i:0xa5 for i in range(size)})
    put(m,NODE+4,2,step); put(m,NODE+12,4,MODEL); put(m,NODE+16,4,timer)
    for off,val in [(14,ident),(16,kind),(80,frame),(88,base),(90,count)]:put(m,MODEL+off,2,val)
    put(m,a['D_801170FC'],4,hold);put(m,a['D_8002EB94'],4,delta)
    for k in range(8):put(m,a['D_80117530']+48*k+18,2,flags if k==kind else 0xa55a)
    for k in range(256):put(m,a['D_801427C0']+2*k,2,(k*257+0x1234)&0xffff)
    return m


def summary(m, calls, a):
    return [signed(get(m,NODE+4,2),16),get(m,NODE+16,4),signed(get(m,MODEL+80,2),16),
            signed(get(m,a['D_801170FC'],4)),get(m,a['D_8002EB94'],4),
            *[get(m,a['D_8012E700']+68*k+20,2) for k in range(8)],
            len(calls), *[v for row in calls for v in row], *([0]*((2-len(calls))*6))]


def callback(m, calls, a, case, which, args):
    # Snapshot callback-visible state; deliberate bounded mutations are observable.
    ident=signed(get(m,MODEL+14,2),16)
    if which=='remove': assert args[:2] == [NODE,1]
    elif which=='stop': assert args[:3] == [ident&MASK,0,0]
    else: raise AssertionError('unknown callback')
    calls.append([1 if which=='remove' else 2,ident,
                  signed(get(m,NODE+4,2),16),get(m,NODE+16,4),
                  signed(get(m,MODEL+80,2),16),case[-1]])
    if case[-1]:
        if which=='stop':
            put(m,NODE+4,2,1234);put(m,MODEL+80,2,222)
            put(m,a['D_801170FC'],4,456);put(m,a['D_8002EB94'],4,bits(.5))
            put(m,a['D_8012E700']+7*68+20,2,0xface)
        else:
            put(m,NODE+16,4,bits(.25));put(m,a['D_8012E700']+20,2,0xbeef)


def oracle(case,a):
    m=initial(case,a);calls=[]
    mode,hold,step,timer,delta,count,base,frame,flags,ident,kind,mutation=case
    if signed(mode,16)==0:
        callback(m,calls,a,case,'remove',[NODE,1,0])
    elif hold==0:
        remaining=rounded(floating(timer)-floating(delta))
        put(m,NODE+16,4,bits(remaining))
        if remaining<=0:
            next_step=signed(step+1,16)
            put(m,NODE+4,2,next_step);put(m,NODE+16,4,bits(.0625))
            cleanup=False
            if next_step>=count:
                if flags&0x1000: cleanup=True
                elif flags&0x2000:
                    callback(m,calls,a,case,'stop',[ident,0,0]);cleanup=True
                else:
                    next_step=0;put(m,NODE+4,2,0)
            if cleanup:callback(m,calls,a,case,'remove',[NODE,1,0])
            else:
                next_frame=signed(base+next_step,16)
                assert 0<=next_frame<256
                if next_frame!=frame:
                    put(m,a['D_8012E700']+ident*68+20,2,get(m,a['D_801427C0']+2*next_frame,2))
                    put(m,MODEL+80,2,next_frame)
    return summary(m,calls,a),m


def execute(words,start,case,a):
    m=initial(case,a); initial_memory=dict(m);calls=[]
    r=[0xa5000000+i for i in range(32)];f=[0x7fa00000+i for i in range(32)]
    r[0]=0;r[4]=NODE;r[5]=case[0]&MASK;r[29]=STACK;r[31]=STOP
    original_r=list(r);original_f=list(f)
    pc,pending,condition=start,None,False
    seen,branches,writes,reads=set(),set(),[],[]
    hook={a['entity_transform_apply']:'remove',a['entity_spawn_callback']:'stop'}
    for _ in range(400):
        if pc==STOP:break
        if pc in hook:
            assert pending is None
            dest=r[31];callback(m,calls,a,case,hook[pc],[r[4],r[5],r[6]])
            for k in [1,2,3,*range(4,16),24,25]:r[k]=(0xc1000000+k*19+len(calls))&MASK
            for k in range(20):f[k]=0x7fa10000+k
            pc=dest;continue
        assert start<=pc<start+len(words)*4 and (pc-start)%4==0, ('escaped',hex(pc))
        off=pc-start;seen.add(off);w=words[off//4]
        op,rs,rt,rd,sa=w>>26,(w>>21)&31,(w>>16)&31,(w>>11)&31,(w>>6)&31
        imm=w&0xffff;simm=signed(imm,16);address=(r[rs]+simm)&MASK
        delayed,pending,next_pc=pending,None,pc+4
        if w==0:pass
        elif op==0:
            fn=w&63
            if fn==0:r[rd]=(r[rt]<<sa)&MASK
            elif fn==3:r[rd]=signed(r[rt])>>sa & MASK
            elif fn==8:pending=r[rs]
            elif fn==33:r[rd]=(r[rs]+r[rt])&MASK
            elif fn==35:r[rd]=(r[rs]-r[rt])&MASK
            elif fn==37:r[rd]=r[rs]|r[rt]
            elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
            else:raise AssertionError(('unsupported special',hex(w)))
        elif op==3:r[31]=pc+8;pending=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
        elif op in (4,5,20,21):
            test=(r[rs]==r[rt]);test=not test if op in (5,21) else test
            branches.add((off,test))
            if test:pending=pc+4+simm*4
            elif op in (20,21):next_pc+=4
        elif op==9:r[rt]=(r[rs]+simm)&MASK
        elif op==12:r[rt]=r[rs]&imm
        elif op==15:r[rt]=imm<<16
        elif op in (33,35,37,49):
            n=2 if op in (33,37) else 4;v=get(m,address,n);reads.append((address,n))
            if op==49:f[rt]=v
            else:r[rt]=signed(v,16)&MASK if op==33 else v
        elif op in (41,43,57):
            n=2 if op==41 else 4;v=f[rt] if op==57 else r[rt]
            if STACK-64<=address<STACK+64:assert (address,n) in [(STACK-4,4),(STACK,4),(STACK+4,4)],'wrong stack store'
            put(m,address,n,v);writes.append((address,n))
        elif op==17:
            if rs==4:f[rd]=r[rt]
            elif rs==8:
                assert rt in (2,3),'unexpected float branch mode'
                test=condition if rt&1 else not condition
                branches.add((off,test))
                if test:pending=pc+4+simm*4
                else:next_pc+=4
            elif rs==16:
                ft,fs,fd,fn=rt,rd,sa,w&63
                if fn==1:f[fd]=bits(floating(f[fs])-floating(f[ft]))
                elif fn==60:condition=floating(f[fs])<floating(f[ft])
                else:raise AssertionError(('unsupported float',hex(w)))
            else:raise AssertionError(('unsupported cop1',hex(w)))
        else:raise AssertionError(('unsupported opcode',hex(w)))
        r[0]=0
        if delayed is not None:
            assert pending is None,'branch in delay slot'
            next_pc=delayed
        pc=next_pc
    else:raise AssertionError('instruction budget exhausted')
    assert pending is None and r[29]==STACK and r[31]==STOP
    assert all(r[k]==original_r[k] for k in [*range(16,24),28,30])
    assert f[20:]==original_f[20:]
    stack_written=set(a for x,n in writes if STACK-64<=x<STACK+64 for a in range(x,x+n))
    assert get(m,STACK-4,4)==STOP and get(m,STACK+4,4)==case[0]&MASK
    for key,val in initial_memory.items():
        if STACK-64<=key<STACK+64 and key not in stack_written:assert m[key]==val,'stack canary'
    result=summary(m,calls,a)
    return result,m,seen,branches
