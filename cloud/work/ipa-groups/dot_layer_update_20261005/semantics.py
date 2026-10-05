"""Host/native differential checks with an independent transition oracle."""
import ctypes,importlib.util,json,random,subprocess
from pathlib import Path
_spec=importlib.util.spec_from_file_location("layer_update_native",Path(__file__).with_name("native.py"))
_native=importlib.util.module_from_spec(_spec);_spec.loader.exec_module(_native)
Machine,NAMES,bits,floating=_native.Machine,_native.NAMES,_native.bits,_native.floating

def corpus():
    values=[bits(v) for v in (-2,-1,-0.0,0.0,0.25,0.5,1,2)]
    def case(h,oldlevel,oldstyle,level,style,prefix=0,mutations=()):
        return {'layer':[h&0xffffffff,oldlevel,oldstyle],'input':[level,style],'prefix':prefix,
          'counts':[254,255,0,1,2,3,4,5],'values':[[bits(v) for v in (0.25,-0.5,0.75,0.5)] for _ in range(8)],'mutations':list(mutations)}
    for h in (-1,256,263,512):
        for ol in values:
            for os in (bits(0.5),bits(-0.0)):
                for lev in values:
                    for sty in (os,bits(-2),bits(1)):
                        yield case(h,ol,os,lev,sty,126)
    # Callback adversaries establish post-client reloading and pre-lock handle snapshot.
    for h in (256,263,-1,512):
        for event in (1,2,3,4):
            for field,new in ((0,257),(0,0xffffffff),(1,bits(0.25)),(2,bits(2)),(2,bits(0.5))):
                yield case(h,bits(0),bits(0),bits(0.75),bits(0.5),3,[(event,field,new)])
    rng=random.Random(0xdfb082049)
    for _ in range(1500):
        c=case(rng.choice([-1,512,256+rng.randrange(8)]),rng.choice(values),rng.choice(values),rng.choice(values),rng.choice(values),rng.randrange(127))
        c['counts']=[rng.randrange(256) for _ in range(8)]
        c['values']=[[rng.choice(values) for _ in range(4)] for _ in range(8)]
        if rng.randrange(2):
            field=rng.randrange(3);value=rng.choice([257,0xffffffff]) if field==0 else rng.choice(values)
            c['mutations']=[(rng.randrange(1,5),field,value)]
        yield c

def oracle(case):
    layer=list(case['layer']);counts=list(case['counts']);events=[];messages=[];lock_count=0
    def event(kind):
        nonlocal lock_count
        events.extend([kind]+layer)
        if kind==3:return
        lock_count+=1
        for sequence,field,value in case['mutations']:
            if sequence==lock_count:layer[field]=value
    def transform(handle,params):
        if not 256<=handle<264:return
        index=handle-256
        changed=[floating(v)!=-2 and floating(v)!=floating(w) for v,w in zip(params,case['values'][index])]
        if not any(changed):return
        counts[index]=(counts[index]+1)&255
        payload=[v if change else bits(-2) for v,change in zip(params,changed)]
        messages.extend([case['prefix']+len(messages)//9,65535,4,1]+payload+[index]);event(3)
    level,style=case['input']
    if floating(level)!=floating(layer[1]):
        layer[1]=level;handle=layer[0];event(1)
        clamped=bits(0) if floating(level)<0 else bits(1) if floating(level)>1 else level
        transform(handle,[clamped,bits(-2),bits(-2),bits(-2)]);event(2)
    if floating(style)!=floating(layer[2]):
        layer[2]=style;handle=layer[0];event(1)
        transform(handle,[bits(-2),bits(-2),bits(-2),style]);event(2)
    return layer+[0xa5a5a5a5]*2+counts+[len(messages)//9]+messages+[len(events)//4]+events

def host(work,source,here):
    work.mkdir(exist_ok=True);(work/'candidate.c').write_text(source);(work/'host.c').write_bytes((here/'host.c').read_bytes());lib=work/'host.so'
    subprocess.run(['cc','-std=c89','-O1','-shared','-fPIC','-fvisibility=hidden','-ffunction-sections','-fdata-sections','-Wl,--gc-sections','-fno-fast-math','-ffp-contract=off','-fno-strict-aliasing','-fsanitize=undefined','-fno-sanitize-recover=all',str(work/'host.c'),'-o',str(lib)],check=True,capture_output=True)
    dll=ctypes.CDLL(str(lib));fn=dll.run_case;fn.argtypes=[ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)];fn.restype=ctypes.c_uint32
    def call(c):
        data=c['layer']+c['input']+[c['prefix']]+c['counts']+sum(c['values'],[])+[len(c['mutations'])]+[v for t in c['mutations'] for v in t]
        inp=(ctypes.c_uint32*len(data))(*data);out=(ctypes.c_uint32*128)();size=fn(inp,out);assert size<=128;return list(out)[:size]
    return call

def verify(work,source,here,bodies,linked,addresses):
    native,gnu=Machine(addresses,bodies),Machine(addresses,linked);call=host(work/'host',source,here)
    cases=list(corpus())
    for i,c in enumerate(cases):
        expected=oracle(c);left=native.run(c);right=gnu.run(c)
        assert left==right,('native/link traces',i)
        assert left[0]==expected,('native/oracle',i,c,left[0],expected)
        assert call(c)==expected,('host/oracle',i,c,call(c),expected)
    assert native.coverage['mode_select_input']==set(range(0,152,4))
    return {'cases':len(cases),'native_executions':2*len(cases),'host_c89_ubsan_cases':len(cases),
       'executed_bodies':NAMES,'instruction_coverage':{n:sorted(s) for n,s in native.coverage.items()},
       'target_branch_outcomes':{hex(pc-addresses['mode_select_input']):sorted(v) for pc,v in native.branches.items() if addresses['mode_select_input']<=pc<addresses['mode_select_input']+152},
       'queue_callback_mutations':True,'complete_native_link_read_write_traces_equal':True,'stack_canaries_and_private_abi_checked':True}
