"""Native/GNU/unchanged-host-C and independent binary32 oracle comparison."""
import ctypes
import importlib.util
import math
from pathlib import Path
import random
import subprocess
import sys
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('object_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec);spec.loader.exec_module(native)
f,bits=native.f,native.bits
IDENTITY=(1.,0.,0.,0.,1.,0.,0.,0.,1.)

def oracle(case):
    available,player,index,flags,values=case
    matrix=list(values[:9]);velocity=values[9:12];position=values[12:15]
    out=[7,flags&255]
    rotate=False
    if available:
        dot=f(f(velocity[0]*matrix[6])+f(velocity[1]*matrix[7]));dot=f(f(matrix[8]*velocity[2])+dot)
        rotate=dot>0.
        if rotate:
            angle=f(3.1415927);sin=f(math.sin(angle));cos=f(math.cos(angle))
            for i in range(3):
                a,b=matrix[i],matrix[6+i]
                matrix[i]=f(f(b*sin)+f(a*cos));matrix[6+i]=f(f(b*cos)-f(a*sin))
        out[1]&=~6
    out += [bits(v) for v in matrix]
    out += [int(bool(available))]
    out += ([(-2000+index*31)&0xffffffff,player,2,*[bits(v) for v in position],1] if available else [0]*7)
    out += [int(bool(available)),int(bool(available)),123+int(bool(available)),123+int(bool(available))]
    out += ([0,10000+index*73,bits(f(.0333333)),1,1,1] if available else [0]*6)
    return out,rotate

def cases():
    rows=[]
    for available in (0,1):
        for player in (0,7):
            for index in (0,15):
                for flags in (0,1,6,255,0x1234):
                    for speed in (-100.,-1.,-0.,0.,1e-30,1.,100.):
                        rows.append((available,player,index,flags,IDENTITY+(1.,2.,speed)+(9.,8.,7.)))
    # Exact cancellation and neighboring finite binary32 values exercise the strict sign decision.
    for a in (0.,-0.,1.,-1.,1e10,-1e10,1e-20,-1e-20):
        for b in (0.,-0.,1.,-1.,1.0000001192092896,-1.0000001192092896):
            matrix=IDENTITY[:6]+(f(a),f(b),1.)
            rows.append((1,3,5,0xfd,matrix+(1.,1.,f(-f(a+b)))+(2.,3.,4.)))
    rng=random.Random(0x10dbb8)
    for i in range(4096):
        values=tuple(f(rng.uniform(-1000,1000)) for _ in range(15))
        rows.append((int(i%7!=0),rng.randrange(8),rng.randrange(16),rng.getrandbits(32),values))
    return rows

def host(source,work,label):
    output=work/(label+'.so')
    command=['cc','-std=c89','-O2','-Wall','-Wextra','-Werror','-Wno-unknown-pragmas','-fPIC','-shared',
             '-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all',
             '-DCANDIDATE="'+str(source.resolve())+'"',str(HERE/'host.c'),
             str(ROOT/'src/blob/func_80090284.c'),str(ROOT/'src/blob/func_80090E9C.c'),'-lm','-o',str(output)]
    proc=subprocess.run(command,text=True,capture_output=True)
    assert proc.returncode==0,proc.stderr
    fn=ctypes.CDLL(str(output)).run_case
    fn.argtypes=[ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_uint,ctypes.POINTER(ctypes.c_float),ctypes.POINTER(ctypes.c_uint)]
    fn.restype=None
    def run(case):
        available,player,index,flags,values=case
        data=(ctypes.c_float*15)(*values);out=(ctypes.c_uint*29)()
        fn(available,player,index,flags,data,out)
        return list(out)
    return run

def verify(work,linked,source):
    addresses=score.image_symbols();targets=score.targets();names=['func_8010DBB8','func_80090284','func_80090E9C']
    literal={0x801249c4:score.own_data().read(0x801249c4,8),0x801239d0:score.own_data().read(0x801239d0,8)}
    machines=[native.Machine(addresses,{n:linked if n==names[0] and k else targets[n] for n in names},literal) for k in (0,1)]
    run_host=host(source,work,'host');corpus=cases();rotations=0
    for i,case in enumerate(corpus):
        expected,rotate=oracle(case);rotations+=rotate
        actual=run_host(case);assert actual==expected,('host',i,case,actual,expected)
        runs=[m.run(case) for m in machines]
        assert runs[0]==runs[1],('native/GNU',i)
        assert runs[0][0]==expected,('native/oracle',i,runs[0][0],expected)
    text=source.read_text();mutants={
        'inclusive_direction':text.replace(' > 0.0f',' >= 0.0f'),
        'wrong_dot_axis':text.replace('actor->matrix[2]','actor->matrix[1]'),
        'wrong_state':text.replace('actor->state=7','actor->state=6'),
        'wrong_flags':text.replace('actor->flags &= ~6','actor->flags &= ~4'),
        'wrong_sound_mode':text.replace('actor->position,2','actor->position,1'),
        'wrong_literal':text.replace('0.0333333f','0.0333334f'),
    }
    rejected={}
    for name,body in mutants.items():
        assert body!=text
        src=work/(name+'.c');src.write_text(body);fn=host(src,work,name)
        mismatch=next((i for i,c in enumerate(corpus) if fn(c)!=oracle(c)[0]),None)
        assert mismatch is not None,('surviving mutant',name)
        rejected[name]=mismatch
    coverage={n:sorted(v) for n,v in machines[0].coverage.items()}
    assert coverage['func_8010DBB8']==list(range(0,324,4))
    return {'cases':len(corpus),'rotation_cases':rotations,'unchanged_host_candidate_and_two_native_callees':True,
            'host_ubsan':True,'all_native_GNU_reads_writes_calls_memory_equal':True,'binary32_oracle_equal':True,
            'executed_instruction_offsets':coverage,'rejected_source_mutants':rejected,
            'limits':['Finite binary32 input domain and valid eight-player/sixteen-definition fixtures.',
                      'Real allocator and yaw instructions execute; sinf/cosf and final sound API are explicit call contracts.',
                      'No FCSR flags, signaling NaN payloads, arbitrary aliases or gameplay claims.']}
