"""Protected native + GNU-linked + independent oracle + host UBSan comparison."""
import ctypes
import importlib.util
from pathlib import Path
import random
import struct
import subprocess
import sys

HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('direction_native',HERE/'native.py')
native=importlib.util.module_from_spec(spec); spec.loader.exec_module(native)
f=lambda x:native.floating(native.bits(x))
IDENTITY=(1.,0.,0.,0.,1.,0.,0.,0.,1.)
TABLES=([i*4093-32768 for i in range(16)], [32767-i*4001 for i in range(16)])


def oracle(case):
    enabled,state,sound,slot,mode,pos,car,matrix,tag=case
    if not enabled or state != 6: return [0xffffffff,0]+[0]*8
    delta=[f(f(x)-f(y)) for x,y in zip(pos,car)]
    output=[f(f(f(delta[0]*f(matrix[j]))+f(delta[1]*f(matrix[j+1])))+
              f(delta[2]*f(matrix[j+2]))) for j in (0,3,6)]
    z2,x2=f(output[2]*output[2]),f(output[0]*output[0])
    length=f(z2+x2); direction=0
    if length>1.:
        z2=f(z2*(1 if output[2]>=0 else -1))
        x2=f(x2*(1 if output[0]>=0 else -1))
        threshold=f(f(f(.924)*f(.924))*length)
        if x2>threshold: direction=4
        elif x2 < -threshold: direction=8
        if z2>threshold: direction|=1
        elif z2 < -threshold: direction|=2
    x,y=(0.,0.) if sound==6 else (TABLES[0][direction],TABLES[1][direction])
    return [tag&0xffffffff,1,sound&0xffffffff,slot,1,mode&255,
            native.bits(1.),native.bits(x),native.bits(y),1]


def cases():
    rows=[]
    for enabled in (0,1,-1,127,-128):
        for state in (0,5,6,7,255):
            for slot in (0,7):
                for sound in (-1,0,6,7):
                    rows.append((enabled,state,sound,slot,0x1234,(3.,2.,-4.),(0.,0.,0.),IDENTITY,0x87654321))
    for x in (-100.,-3.,-1.0001,-1.,-0.,0.,.0001,1.,1.0001,3.,100.):
        for z in (-100.,-3.,-1.0001,-1.,-0.,0.,.0001,1.,1.0001,3.,100.):
            for sound in (5,6):
                rows.append((1,6,sound,3,255,(x,31.,z),(0.,0.,0.),IDENTITY,0xffffffff))
    # Both sides of angular thresholds and unit-radius comparisons.
    for value in (.413845,.413846,.413847,1.,1.0000001192092896):
        for sx in (-1,1):
            for sz in (-1,1):
                rows.append((1,6,2,2,256,(sx*value,0.,float(sz)),(0.,0.,0.),IDENTITY,0))
                rows.append((1,6,2,2,511,(float(sx),0.,sz*value),(0.,0.,0.),IDENTITY,0x80000000))
    rng=random.Random(0xFEA00)
    for i in range(4096):
        pos=tuple(f(rng.uniform(-100,100)) for _ in range(3))
        car=tuple(f(rng.uniform(-100,100)) for _ in range(3))
        matrix=tuple(f(rng.uniform(-2,2)) for _ in range(9))
        rows.append((1,6,6 if i%13==0 else rng.randrange(-20,100),rng.randrange(8),
                     rng.getrandbits(32),pos,car,matrix,rng.getrandbits(32)))
    return rows


def host(source,work,label):
    out=work/(label+'.so')
    subprocess.run(['cc','-std=c89','-O2','-Wall','-Wextra','-Werror','-fPIC','-shared',
                    '-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all',
                    '-DCANDIDATE="'+str(source)+'"',str(HERE/'host.c'),'-o',str(out)],
                   check=True,capture_output=True)
    fn=ctypes.CDLL(str(out)).run_case
    fn.argtypes=[ctypes.c_int]*4+[ctypes.c_uint,ctypes.POINTER(ctypes.c_float),ctypes.c_uint,
                                  ctypes.POINTER(ctypes.c_short),ctypes.POINTER(ctypes.c_short),
                                  ctypes.POINTER(ctypes.c_uint)]
    fn.restype=None
    xs,ys=[(ctypes.c_short*16)(*a) for a in TABLES]
    def run(case):
        enabled,state,sound,slot,mode,pos,car,matrix,tag=case
        data=(ctypes.c_float*15)(*(pos+car+matrix)); result=(ctypes.c_uint*10)()
        fn(enabled,state,sound,slot,mode,data,tag,xs,ys,result)
        return list(result)
    return run


def verify(work,linked_words,source):
    addresses=score.image_symbols(); targets=score.targets()
    machines=[native.Machine(addresses,words,targets['func_800A61B0'])
              for words in (targets['stat_lap_split'],linked_words)]
    run_host=host(source,work,'host')
    corpus=cases(); directions=set()
    for index,case in enumerate(corpus):
        expected=oracle(case)
        assert run_host(case)==expected, ('host',index,case,expected,run_host(case))
        runs=[m.run(case,TABLES) for m in machines]
        assert runs[0]==runs[1], ('native/linked trace',index)
        result,calls,reads,writes,matrix_calls=runs[0]
        got=[result,len(calls)]+(list(calls[0]) if calls else [0]*7)+[matrix_calls]
        assert got==expected, ('native',index,case,expected,got)
        if calls and case[2]!=6: directions.add((calls[0][5],calls[0][6]))
        assert all(native.STACK<=address<native.STACK+1024 for address,_ in writes)
        if not case[0]: assert reads==[(addresses['D_8010FFC0'],1),(native.STACK+452,4)]
    text=source.read_text()
    mutants={
      'wrong_threshold':text.replace('(.924f * .924f)','0.8f'),
      'wrong_axis':text.replace('forward_squared = output[2] * output[2];','forward_squared = output[1] * output[1];'),
      'wrong_state':text.replace('slot * 8] != 6','slot * 8] != 5'),
      'missing_center':text.replace('if (sound == 6)','if (sound == 106)'),
      'unsigned_pan':text.replace('extern s16 D_8011F020[], D_8011F040[];','extern unsigned short D_8011F020[], D_8011F040[];'),
      'wrong_failure':text.replace('return -1;','return -2;'),
    }
    rejected={}
    for name,body in mutants.items():
        assert body!=text
        # Type-changing mutant needs a consistent synthetic definition too.
        if name=='unsigned_pan':
            body=body.replace('typedef short s16;','typedef unsigned short s16;').replace('extern unsigned short D_8011F020[], D_8011F040[];','extern s16 D_8011F020[], D_8011F040[];')
        path=work/(name+'.c');path.write_text(body);fn=host(path,work,name)
        failure=next((i for i,c in enumerate(corpus) if fn(c)!=oracle(c)),None)
        assert failure is not None, ('survived',name)
        rejected[name]=failure
    coverage=sorted(machines[0].coverage)
    # Conservative static CFG: both conditional outcomes, actual unconditional
    # branch destinations, actual delay slots, and returning ordinary calls.
    # This proves the duplicated negation is unreachable independently of cases.
    words=targets['stat_lap_split'];seen=set();pending=[0]
    while pending:
        pc=pending.pop()
        if pc in seen: continue
        assert 0<=pc<608
        seen.add(pc);w=words[pc//4];op=w>>26
        rs,rt=(w>>21)&31,(w>>16)&31
        if op==0 and w&63==8:
            seen.add(pc+4)
        elif op==3:
            seen.add(pc+4);pending.append(pc+8)
        elif op in (4,5,20,21) or (op==17 and rs==8):
            seen.add(pc+4)
            pending.append(pc+4+4*native.signed(w&65535,16))
            if not (op==4 and rs==rt): pending.append(pc+8)
        else: pending.append(pc+4)
    assert sorted(seen)==coverage
    return {'cases':len(corpus),'native_linked_access_traces_equal':True,
            'host_ubsan':True,'real_native_matrix_callee':True,
            'sound_callee':'synthetic seven-argument callback with caller-save clobbers',
            'distinct_pan_pairs':len(directions),'executed_instruction_offsets':coverage,
            'unexecuted_instruction_offsets':sorted(set(range(0,608,4))-set(coverage)),
            'dynamic_coverage_equals_conservative_static_cfg':True,
            'rejected_mutants':rejected,
            'domain':'Finite binary32 inputs, eight valid player slots, synthetic signed pan tables; no FCSR/NaN claims.'}
