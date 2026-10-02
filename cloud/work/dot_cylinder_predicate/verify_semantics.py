#!/usr/bin/env python3
"""Compare native target instruction execution with the typed C reconstruction."""
import ctypes, hashlib, itertools, json, random, struct, subprocess, sys, tempfile
from pathlib import Path
from native_machine import execute, to_bits, to_float
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'tools/cloud'))
import score
pack=lambda xs: struct.pack('>%dI'%len(xs),*xs)
START=0x8010C448
FLAG=0x8014AA3A
PLAYERS=0x80152818

def run():
    words=score.targets()['func_8010C448']
    assert len(words)==80
    with tempfile.TemporaryDirectory() as tmp:
        so=Path(tmp)/'candidate.so'
        subprocess.run(['cc','-shared','-fPIC','-std=c99','-O2','-Wall','-Wextra','-Werror','-ffp-contract=off',str(HERE/'host_semantics.c'),'-o',str(so)],check=True)
        lib=ctypes.CDLL(str(so));lib.run_case.argtypes=[ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_uint32),ctypes.c_int]
        rng=random.Random(0xC448); cases=[]
        # Strict vertical and radial boundaries, signed zeros, quiet NaNs/infinities.
        vals=[to_bits(x) for x in [-float('inf'),-18.,-3.5,-2.,-0.,0.,1.,3.5,18.,float('inf'),float('nan')]]
        vals += [0xbfffffff,0xc0000001,0x418fffff,0x41900001]
        for y,x,r,alias,en in itertools.product(vals,[0,0x40600000,0x40600001,0x405fffff], [0,0xc0600000,0x7fc00000],[-1,6,7],[0,1,255]):
            cases.append((len(cases)%8,en,[0,0,0,x,y,0,r,0x41200000],alias))
        for n in range(6000):
            data=[rng.getrandbits(32) for _ in range(8)]
            if n%2: data[:7]=[to_bits(rng.randrange(-80,80)/4.) for _ in range(7)]
            cases.append((n%8,[0,1,128,255][n%4],data,n%9-1))
        for index,en,data,alias in cases:
            array=(ctypes.c_uint32*8)(*data)
            got=lib.run_case(index,en,array,alias)
            flag=FLAG+index*0x808; player=PLAYERS+index*0x3b8
            regions=[(0x100000,struct.pack('>h',index)),(0x200000,pack(data[:3])),(0x300000,pack([data[6]])),(0x400000,pack([data[7]])),(flag,bytes([en])),(player,bytes([0xA5])*8+pack(data[3:6])+bytes([0xA5])*(0x3b8-20))]
            dest=0 if alias<0 else 0x200000+4*alias if alias<3 else player+8+4*(alias-3) if alias<6 else 0x300000 if alias==6 else 0x400000
            want,after,reads,writes=execute(words,START,regions,[0x100000,0x200000,0x300000,dest])
            assert got==want,(index,en,data,alias,got,want)
            rd=dict(after)
            expect=list(struct.unpack('>3I',rd[0x200000]))+list(struct.unpack('>3I',rd[player][8:20]))+[int.from_bytes(rd[0x300000],'big'),int.from_bytes(rd[0x400000],'big')]
            for a,b in zip(array,expect):
                assert a==b or (to_float(a)!=to_float(a) and to_float(b)!=to_float(b)),(hex(a),hex(b))
            assert (0x300000,4) in reads, 'native radius load precedes disabled return'
            nonstack=[w for w in writes if not 0x700000<=w[0]<0x700100]
            assert nonstack==([(dest,4)] if en and dest else [])
        # Fail-closed regression checks.
        for badwords,badregions in [([0xffffffff],regions),(words,[])]:
            try: execute(badwords,START,badregions,[0x100000,0x200000,0x300000,0])
            except AssertionError: pass
            else: raise AssertionError('invalid input silently accepted')
    result={'cases':len(cases),'result':'PASS','target_sha256':hashlib.sha256(pack(words)).hexdigest(),'limitations':['No FCSR exception or signaling-NaN payload model','Valid accessible indices 0..7; native population count not established','Single-threaded host IEEE binary32 round-to-nearest'],'output_alias_modes':9}
    print(json.dumps(result,indent=2))
    return result
if __name__=='__main__':run()
