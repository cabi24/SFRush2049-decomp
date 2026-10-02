"""Native/IDO/host-C semantics including output aliases and IEEE edge cases.
The native interpreter is shared with the independently investigated C448 peer.
It is a bounded model, not a hardware FCSR/exception test.
"""
import ctypes, json, random, struct, subprocess, sys
from pathlib import Path
from native_machine import execute
HERE=Path(__file__).resolve().parent
ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'tools/cloud'))
import score

def words(data):return list(struct.unpack('>%dI'%(len(data)//4),data))
def nan(bits):return bits&0x7f800000==0x7f800000 and bits&0x7fffff!=0

def run():
    build=ROOT/'build/dot_low_cylinder_audit'
    assert build.exists(), 'run verify.py first'
    subprocess.run(['cc','-std=c89','-pedantic','-Wall','-Wextra','-Werror','-shared','-fPIC','-O2','-ffp-contract=off',str(HERE/'host_semantics.c'),'-o',str(build/'host.so')],check=True)
    lib=ctypes.CDLL(str(build/'host.so'));lib.run_case.argtypes=[ctypes.c_int,ctypes.c_int,ctypes.POINTER(ctypes.c_uint),ctypes.c_int,ctypes.POINTER(ctypes.c_uint)]
    target=score.targets()['func_8010C588'];candidate=words((build/'text.bin').read_bytes())
    rng=random.Random(0xc588);cases=[]
    # Strict vertical endpoints and nearest binary32 neighbors; radial surface.
    edge=[0,0x80000000,0x3f800000,0xc0000000,0xbfffffff,0xc0000001,0x40800000,0x407fffff,0x40800001,0x40600000,0x405fffff,0x40600001,0x7f800000,0xff800000,0x7fc12345,1,0x80000001,0x7f7fffff]
    for axis in range(3):
        for value in edge:
            for radius in [0,0x3f800000,0xc0600000,0x7f800000,0x7fc00001]:
                values=[0,0,0,0,0,0,radius,0x12345678];values[3+axis]=value;cases.append(values)
    for _ in range(1000):cases.append([rng.getrandbits(32) for _ in range(8)])
    count=0
    for values in cases:
        for alias in range(9):
            index=count%4; enabled=[0,1,-1,127,-128][count%5]
            data=struct.pack('>8I',*values);selector=0x100000;origin=0x200000;radius=origin+24;output=origin+28
            player=0x80152818+952*index+8;flag=0x8014aa3a+2056*index
            destination=[output,0,origin,origin+4,origin+8,radius,player,player+4,player+8][alias]
            # Radius and output are stored separately from the three origin components.
            regions=[(selector,struct.pack('>h',index)),(origin,data[:12]),(radius,data[24:]),(player,data[12:24]),(flag,bytes([enabled&255]))]
            native=execute(target,0x8010c588,regions,[selector,origin,radius,destination])
            compiled=execute(candidate,0x8010c588,regions,[selector,origin,radius,destination])
            def flatten(result):
                memory=dict(result[1]);return words(bytes(memory[origin])+bytes(memory[player])+bytes(memory[radius]))
            n,c=flatten(native),flatten(compiled)
            hostout=(ctypes.c_uint*8)();hostin=(ctypes.c_uint*8)(*values)
            ret=lib.run_case(index,enabled,hostin,alias,hostout)
            assert ret==native[0]==compiled[0],(count,values,alias,ret,native[0],compiled[0])
            for a,b,h in zip(n,c,hostout):assert (a==b==h) or (nan(a) and nan(b) and nan(h)),(count,values,alias,hex(a),hex(b),hex(h))
            if not enabled:
                assert (radius,4) in native[2] and (radius,4) in compiled[2]
                assert all(a!=origin and a!=player for a,w in native[2])
                assert n==values
            count+=1
    # Fail closed: unsupported instruction and unmapped input are rejected.
    for code,args in [([0xffffffff],[0,0,0,0]),(target,[0,0,0,0])]:
        try: execute(code,0x8010c588,[],args)
        except AssertionError:pass
        else:raise AssertionError('interpreter accepted invalid execution')
    report={'cases':count,'comparisons':'native vs linked IDO vs host C','aliases':9,'nan_rule':'NaN classes equivalent; payload propagation/FCSR not established','source':'func_8010C588.c','status':'PASS'}
    (build/'semantics.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':run()
