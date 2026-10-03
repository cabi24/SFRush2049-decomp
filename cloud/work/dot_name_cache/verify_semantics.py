"""Compare native bytes, GNU-linked candidate and optimized host C against an
independent state-transition oracle. Requires replay.py -- via environment.
"""
import ctypes,json,os,random,struct,subprocess,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[2]
sys.path.insert(0,str(ROOT/'tools/cloud'));import score
from native_machine import execute,signed
import replay

def fixed(s):return s+b'\0'*(13-len(s))
def oracle(names,ages,refs,name):
    ages=[(x+1)&65535 for x in ages];names=names[:];refs=refs[:]
    selected=next((i for i,s in enumerate(names) if s.split(b'\0')[0]==name[:-1]),None)
    if selected is None:
        selected=next((i for i,s in enumerate(names) if s[0]==0),None)
        if selected is None:
            used=set(x for x in refs if 0<=x<20)
            selected=next((i for i in range(20) if i not in used),None)
            if selected is None:
                oldest=0
                for i,age in enumerate(ages):
                    if oldest<age:oldest=signed(age,16);selected=i
                assert selected is not None,'native uninitialized-best case excluded'
                refs=[-1 if x==selected else x for x in refs]
        names[selected]=name+names[selected][len(name):]
    ages[selected]=0
    return selected,b''.join(names),struct.pack('>20H',*ages),struct.pack('>180i',*refs)

def run():
    rng=random.Random(0xF1D04);native=score.targets()['func_800F1D04'];start=score.image_symbols()['func_800F1D04']
    base=[fixed(('slot%02d'%i).encode()) for i in range(20)]
    cases=[]
    for i in range(20):
        for age in (0,1,32766,32767,32768,65534,65535):
            ages=[age]*20;cases.append(('hit',base,ages,list(range(20))*9,('slot%02d'%i).encode()+b'\0'))
            names=base[:];names[i]=fixed(b'');cases.append(('empty',names,ages,list(range(20))*9,b'new\0'))
            refs=[x for x in range(20) if x!=i];refs=(refs*10)[:180];cases.append(('unreferenced',base,ages,refs,b'new\0'))
    for slot in range(20):
        for age in (0,1,32766,32767,32768,65534,65535):
            ages=[0]*20;ages[slot]=age;cases.append(('eviction-boundary',base,ages,list(range(20))*9,b'new\0'))
    # Consecutive large ages show this is signed-short scan semantics, not
    # unsigned maximum-age LRU. Wrap-to-zero states are included when defined.
    cases += [('signed-age',base,[32767+i for i in range(20)],list(range(20))*9,b'new\0'),('zero-after-large',base,[32767]+[65535]*19,list(range(20))*9,b'new\0')]
    for _ in range(600):
        names=base[:];ages=[rng.randrange(65536) for _ in range(20)]
        refs=[rng.choice([-2147483648,-1,20,2147483647]+list(range(20))) for _ in range(180)]
        name=b'new\0'
        mode=rng.randrange(4)
        if mode==0:name=('slot%02d'%rng.randrange(20)).encode()+b'\0'
        elif mode==1:names[rng.randrange(20)]=fixed(b'')
        elif mode==2:refs=list(range(20))*9
        cases.append(('random',names,ages,refs,name))
    with tempfile.TemporaryDirectory() as tmp:
        d=Path(tmp);os.environ['REPLAY_DUMP_DIR']=tmp
        import contextlib,io
        with contextlib.redirect_stdout(io.StringIO()):proof=replay.run()
        linked=list(struct.unpack('>%dI'%((d/'linked.bin').stat().st_size//4),(d/'linked.bin').read_bytes()))
        subprocess.run(['cc','-std=c89','-O2','-fPIC','-shared',str(HERE/'host_bridge.c'),'-o',str(d/'host.so')],check=True)
        lib=ctypes.CDLL(str(d/'host.so'));U8=ctypes.c_ubyte;U16=ctypes.c_ushort;I32=ctypes.c_int
        count=0;uninit=0;groups={}
        for category,names,ages,refs,name in cases:
            expected=oracle(names,ages,refs,name)
            a=execute(native,start,names,ages,refs,name);b=execute(linked,start,names,ages,refs,name)
            assert a[:4]==expected,(category,count,'native',a[:1],expected[:1])
            assert b[:4]==expected,(category,count,'candidate',b[:1],expected[:1])
            assert a[4]==b[4],(category,'call trace')
            uninit+=bool(a[5]);groups[category]=groups.get(category,0)+1
            ins=(U8*260).from_buffer_copy(b''.join(names));ag=(U16*20)(*ages);rs=(I32*180)(*refs);nm=(U8*len(name)).from_buffer_copy(name)
            on=(U8*260)();oa=(U16*20)();orr=(I32*180)()
            result=lib.run_case(ins,ag,rs,nm,on,oa,orr)
            actual=(result,bytes(on),struct.pack('>20H',*oa),struct.pack('>180i',*orr))
            assert actual==expected,(category,count,'host');count+=1
        # Retail reads a speculative uninitialized best even for valid evictions.
        # Its all-wrap case is actually observable undefined input, so do not run
        # that case through C or assert a deterministic result.
        bad=[]
        for fill in (0,1):
            try:r=execute(native,start,base,[65535]*20,list(range(20))*9,b'new\0',stack_fill=fill);bad.append({'fill':fill,'result':r[0],'uninitialized_reads':len(r[5])})
            except AssertionError as e:bad.append({'fill':fill,'native_error':str(e)})
        result={'verdict':'PASS','cases':count,'categories':groups,'native_speculative_uninitialized_read_cases':uninit,'all_ages_wrap_counterexample':bad,'checks':['native/linked/host/oracle state','callee-save and stack restoration','native/linked exact call trace','bounded string and memory access'],'limits':['Strings must terminate within 13 bytes; non-overlapping input/cache','All-wrap full referenced cache has undefined selected slot; excluded from C','Callees modeled from verified byte strcmp/strcpy contracts; no whole-game execution']}
        print(json.dumps(result,indent=2));return result
if __name__=='__main__':run()
