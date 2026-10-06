#!/usr/bin/env python3
"""Portable complete-body EB028 NONMATCH research verification.
Native data is read through current protected manifests; no payload is published.
"""
import argparse,copy,ctypes,dataclasses,hashlib,importlib.util,json,random,shutil,struct,subprocess,sys,tempfile
from pathlib import Path
HERE=Path(__file__).resolve().parent
BASE='dea99f09ab19b1d3b324ed7097162f7b378e7096'
NAME='func_800EB028';START=0x800eb028
NATIVE_SHA='c5d1ce1c14b6cd94b810c356e0c4908add438221380db07d39b2aa3cf748f59c'
FLAGS='-g0 -O3 -mips2 -G 0 -non_shared'
_spec=importlib.util.spec_from_file_location('_eb028_native',HERE/'native.py')
native_machine=importlib.util.module_from_spec(_spec);_spec.loader.exec_module(native_machine)
execute,bits,value,f32,signed,STACK=(getattr(native_machine,n) for n in ('execute','bits','value','f32','signed','STACK'))

def sha(b):return hashlib.sha256(b).hexdigest()
def packed(ws):return struct.pack('>%dI'%len(ws),*ws)
def run(cmd,**kw):
    p=subprocess.run(cmd,capture_output=True,**kw)
    assert p.returncode==0,(cmd,p.returncode,p.stderr.decode(errors='replace')[:3000])
    return p.stdout

def symbols(score,obj):
    data,secs=score._elf(obj)
    return [x for i,s in enumerate(secs) if s['type']==2 for x in score._symbol_table(data,secs,i)]

def function(score,obj):
    fs=[x for x in symbols(score,obj) if x['name']==NAME and x['type']==2 and x['section']];assert len(fs)==1
    f=fs[0];data,secs=score._elf(obj);sec=secs[f['section']]
    # Linked symbols are virtual addresses; relocatable symbols are section offsets.
    base=struct.unpack_from('>I',data,struct.unpack_from('>I',data,0x20)[0]+f['section']*40+12)[0]
    offset=f['value']-base
    raw=data[sec['off']+offset:sec['off']+offset+f['size']]
    assert len(raw)==f['size'] and len(raw)%4==0
    return f,list(struct.unpack('>%dI'%(len(raw)//4),raw))

def link(score,obj,work):
    data,secs=score._elf(obj);defs=[];addresses=score.image_symbols()
    for s in symbols(score,obj):
        if s['name'] and not s['section']:
            a=addresses.get(s['name'],score.address_named(s['name']));assert a is not None,s['name']
            defs.append('%s = 0x%08x;'%(s['name'],a))
    script='SECTIONS { .text 0x800EB028 : SUBALIGN(4) { *(.text) } .rodata 0x8012451C : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.options) *(.pdr) *(.mdebug) } }\n'+'\n'.join(defs)
    ld=work/'link.ld';elf=work/'linked.elf';ld.write_text(script)
    run(['mips-linux-gnu-ld','-EB','-T',str(ld),'-o',str(elf),str(obj)])
    fn,words=function(score,elf);assert fn['value']==START
    d,ss=score._elf(elf);rd=next(s for s in ss if s['name']=='.rodata');rodata=d[rd['off']:rd['off']+rd['size']]
    assert rd['size']==64 and rodata[60:]==b'\0'*4
    rels=[]
    for s in secs:
        if s['type']!=9:continue
        st=score._symbol_table(data,secs,s['link'])
        for at in range(s['off'],s['off']+s['size'],8):
            off,info=struct.unpack_from('>II',data,at)
            rels.append({'section':secs[s['info']]['name'],'offset':off,'kind':info&255,'symbol':st[info>>8]['name']})
    return words,rodata[:60],rels

CAR=0x100000;MODELS=0x8014a250;CAMERAS=0x80150b70;ACCEL=0x801526a8;GLOBALS=0x80110688;THRESHOLD=0x80124518;RODATA=0x8012451c;THREAD=0x80034840
SIZES={CAR:952,MODELS:2056*6,CAMERAS:152*4,ACCEL:48,GLOBALS:28,THRESHOLD:4}
CALLS={0x8000bd90:1,0x80007080:2,0x8008d6b0:3,0x8009e820:4,0x800e8cb8:5,0x800eafdc:6,0x800ea3f4:7,0x800ea2dc:8,0x800ea108:9,0x800e9e2c:10,0x800e9c70:11,0x800e95dc:12,0x800e8f10:13,0x800c69c0:14}
FIELDS={CAR:[(o,4) for o in [8,12,16,*range(44,116,4)]],MODELS:[(i*2056+o,n) for i in range(6) for o,n in [(x,4) for x in [*range(328,376,4),1884,1888]]+[(1732,2)]],CAMERAS:[(i*152+o,4) for i in range(4) for o in [*range(0,48,4),*range(96,148,4)]],ACCEL:[(o,4) for o in range(0,48,4)]}

def endian(data,fields):
    b=bytearray(data)
    for o,n in fields:b[o:o+n]=b[o:o+n][::-1]
    return b

def fixture(seed,view,mutation):
    rng=random.Random(seed);regions={a:bytearray((i*13+seed*7)&255 for i in range(n)) for a,n in SIZES.items()}
    def put(a,x,n=4):
        for b,d in regions.items():
            if b<=a and a+n<=b+len(d):d[a-b:a-b+n]=(x&((1<<(8*n))-1)).to_bytes(n,'big');return
        raise AssertionError(hex(a))
    for base,fields in FIELDS.items():
        for off,n in fields:put(base+off,bits(rng.uniform(-12,12)) if n==4 else (1 if seed&1 else -1),n)
    model=seed%6;slot=(seed//6)%4
    put(CAR+859,model,1);put(CAR+860,slot,1);put(CAR+861,view,1)
    forces=[-200000.,-100001.,-100000.,-99999.,-0.,0.,99999.,100000.,100001.,200000.]
    for j,off in enumerate(range(328,376,4)):put(MODELS+model*2056+off,bits(forces[(seed+j*3)%len(forces)]))
    for base,off in [(CAR,44),(CAR,80),(CAMERAS+slot*152,0),(CAMERAS+slot*152,96)]:
        for j in range(9):put(base+off+4*j,bits(rng.uniform(-2,2)))
    for j,x in enumerate([1.2,4.,-.55,3.2,0.,.85,.35]):put(GLOBALS+4*j,bits(x))
    put(THRESHOLD,bits(.3))
    return regions,mutation

class Hooks:
    def __init__(self,mutation):self.mutation=mutation;self.events=[]
    def __call__(self,dest,args,mem):
        assert dest in CALLS,('unknown call',hex(dest));tag=CALLS[dest]
        def ptr(p):
            if not p:return 0
            for base,n,mark in [(CAR,952,0x100000),(MODELS,2056*6,0x200000),(CAMERAS,152*4,0x300000),(ACCEL,48,0x400000)]:
                if base<=p<base+n:return mark+p-base
            if p==THREAD:return 0x500000
            if p==STACK-96+76:return 0x60004c
            if p==STACK-96+64:return 0x600040
            raise AssertionError(('argument pointer',hex(p)))
        def vec(p,n=3):return [mem(p+4*i) for i in range(n)]
        def event(id,a=0,b=0,c=0,d=0):self.events.extend([id,ptr(a),ptr(b),ptr(c),d])
        if tag in (1,2):assert args[0]==THREAD;self.events.append(tag);return
        if tag==3:
            a,b=args[:2];event(tag,a,b);v=vec(a,9);self.events.extend(v)
            for i,x in enumerate(v):mem(b+4*i,4,x)
        elif tag==4:
            a,b,m=args[:3];event(tag,a,b,m);v=vec(a);matrix=vec(m,9);self.events.extend(v+matrix)
            for i in range(3):
                x=f32(value(v[0])*value(matrix[i]));y=f32(value(v[1])*value(matrix[i+3]));z=f32(value(v[2])*value(matrix[i+6]))
                mem(b+4*i,4,bits(f32(f32(x+y)+z)))
        elif tag==5:
            a,b,c=args[:3];event(tag,a,b,c);self.events.extend(vec(b)+vec(c,9))
        elif tag==6:
            a=args[0];event(tag,a);t=f32(f32(value(mem(a+1884))+value(mem(a+1888)))*.5);threshold=value(mem(THRESHOLD));return bits(f32(t-threshold) if threshold<t else 0.)
        elif tag==14:
            a,b=args[:2];event(tag,a,b);v,w=vec(a),vec(b);self.events.extend(v+w)
            for i in range(3):mem(a+4*i,4,bits(f32(value(v[i])+f32(value(w[i])*.25))))
        else:
            mode=0;snapshot=0
            if tag in (11,12,13):mode=signed(args[0],16);car,p,m=args[1:4]
            else:
                car,p=args[:2];m=args[2] if tag!=7 else 0
                if tag==8:snapshot=args[3]
            assert car==CAR;event(tag,car,p,m,ptr(snapshot) if snapshot else mode)
            v=vec(p);self.events.extend(v)
            if m:self.events.extend(vec(m,9))
            if snapshot:self.events.extend(vec(snapshot))
            slot=signed(mem(car+860,1),8);model=signed(mem(car+859,1),8);camera=CAMERAS+slot*152;scale=f32(tag*.0625)
            for i in range(3):mem(camera+132+4*i,4,bits(f32(f32(value(v[i])+scale)+f32(i*.5))))
            for i in range(9):mem(camera+96+4*i,4,bits(f32(f32(value(mem(car+80+4*i))*.125)+scale)))
            if self.mutation&1:
                a=MODELS+model*2056+1732;mem(a,2,-signed(mem(a,2),16))
            if self.mutation&2:
                for i in range(3):mem(car+8+4*i,4,bits(f32(value(mem(car+8+4*i))+f32(2.+i))))
        return None

def host_runner(work,source_dir=HERE):
    so=work/'host.so';run(['cc','-shared','-fPIC','-O1','-std=c89','-ffp-contract=off','-fexcess-precision=standard','-fsanitize=undefined','-fno-sanitize-recover=all','-o',str(so),str(source_dir/'host.c')])
    lib=ctypes.CDLL(str(so));fn=lib.host_run;fn.argtypes=[ctypes.c_void_p]*5+[ctypes.c_int,ctypes.c_void_p];fn.restype=None
    def call(regions,mutation):
        buf={a:ctypes.create_string_buffer(bytes(endian(regions[a],FIELDS[a])),len(regions[a])) for a in FIELDS}
        vals=[value(int.from_bytes(regions[GLOBALS][i:i+4],'big')) for i in range(0,28,4)]+[value(int.from_bytes(regions[THRESHOLD],'big'))]
        gl=(ctypes.c_float*8)(*vals);events=(ctypes.c_uint32*1024)();fn(buf[CAR],buf[MODELS],buf[CAMERAS],buf[ACCEL],gl,mutation,events)
        output={a:bytes(b) for a,b in regions.items()}
        for a,b in buf.items():output[a]=bytes(endian(b.raw,FIELDS[a]))
        return output,list(events)[1:events[0]+1]
    return call

def behavior(native,compiled,rodata,work,cases=768):
    host=host_runner(work);cov=set();branches=set();candidate_cov={k:set() for k in compiled};digest=hashlib.sha256();pairs={k:set() for k in range(11)};mutations={k:set() for k in range(11)}
    for i in range(cases):
        view=i%256 if i<256 else i%12;seed=1000+i+i//12;regions,mutation=fixture(seed,view,(i//12)%4);expected,events=host(regions,mutation)
        if view<11:pairs[view].add((seed%6,(seed//6)%4));mutations[view].add(mutation)
        for name,words,rd in [('native',native,rodata),*[(k,v[0],v[1]) for k,v in compiled.items()]]:
            hook=Hooks(mutation)
            try:output,reads,writes=execute(words,START,list(regions.items())+[(RODATA,rd)],CAR,hook,cov if name=='native' else candidate_cov[name],branches if name=='native' else None,48 if name=='native' else 44)
            except AssertionError as exc:raise AssertionError((i,view,name,exc.args)) from exc
            got=dict(output);got.pop(RODATA)
            assert got==expected,('memory mismatch',i,view,name,next((hex(a) for a in got if got[a]!=expected[a]),None))
            assert hook.events==events,('call trace mismatch',i,view,name,hook.events,events)
        digest.update(packed(events))
    assert len(cov)==len(native),('uncovered native offsets',sorted(set(range(0,len(native)*4,4))-cov))
    regions,mutation=fixture(1000,0,0);negative=[]
    for label,index,word in [('unknown-opcode',0,0xfc000000),('wrong-call',44,(3<<26)|((0x80300000>>2)&0x3ffffff)),('redirected-save',1,(native[1]&0xffff0000)|12)]:
        changed=list(native);changed[index]=word
        try:execute(changed,START,list(regions.items())+[(RODATA,rodata)],CAR,Hooks(mutation))
        except AssertionError:negative.append(label)
    assert len(negative)==3,negative
    expected_pairs={(m,s) for m in range(6) for s in range(4)}
    assert all(p==expected_pairs for p in pairs.values()),pairs
    assert all(m=={0,1,2,3} for m in mutations.values()),mutations
    mutant_names=source_mutants(host,work)
    return {'model_slot_pairs_per_view':{str(k):len(v) for k,v in pairs.items()},'mutations_per_view':{str(k):sorted(v) for k,v in mutations.items()},'source_mutants_rejected':mutant_names,'cases':cases,'native_executions':cases,'compiled_executions':cases*len(compiled),'host_c89_ubsan_cases':cases,'native_instruction_coverage':len(cov),'native_instructions':len(native),'native_branch_outcomes':sorted([list(x) for x in branches]),'candidate_instruction_coverage':{k:len(v) for k,v in candidate_cov.items()},'call_trace_sha256':digest.hexdigest(),'negative_controls':negative}

def source_mutants(reference,work):
    source=(HERE/'candidate.c').read_text();rejected=[]
    changes=[('wrong_base_lift','camera->position[1] += 0.25f;','camera->position[1] += 0.5f;'),
             ('wrong_force_axes','for (j = 0; j < 3; j += 2)','for (j = 0; j < 3; j++)'),
             ('reversed_collision_predicate','if (m->collision_state < 0)','if (m->collision_state >= 0)'),
             ('lost_endpoint_snapshot','entity_iterate(camera->position, pos_in);','entity_iterate(camera->position, car->position);')]
    for label,old,new in changes:
        assert old in source;directory=work/label;directory.mkdir()
        (directory/'candidate.c').write_text(source.replace(old,new))
        (directory/'host.c').write_bytes((HERE/'host.c').read_bytes())
        mutant=host_runner(directory,directory)
        for i in range(48):
            regions,mutation=fixture(3000+i,i%12,(i//12)%4)
            if mutant(regions,mutation)!=reference(regions,mutation):rejected.append(label);break
    assert len(rejected)==4,rejected
    return rejected

def verify(root,output=None):
    root=Path(root).resolve();sys.path.insert(0,str(root));from tools.cloud import score
    native=score.targets()[NAME];assert len(native)==410 and sha(packed(native))==NATIVE_SHA
    data=score.own_data();assert data is not None;rodata=data.read(RODATA,60);assert rodata is not None
    assert list(struct.unpack('>11I',rodata[:44]))==[0x800eb0cc,0x800eb160,0x800eb408,0x800eb408,0x800eb534,0x800eb588,0x800eb5c4,0x800eb604,0x800eb4e0,0x800eb644,0x800eb478]
    assert data.read(GLOBALS,28)==packed([bits(x) for x in [1.2,4.,-.55,3.2,0.,.85,.35]])
    assert data.read(THRESHOLD,4)==packed([bits(.3)])
    assert (score.IDO/'cc').is_file() and shutil.which('mips-linux-gnu-ld'),'pinned IDO and MIPS GNU linker required'
    with tempfile.TemporaryDirectory(prefix='eb028-') as tmp:
        work=Path(tmp);objects={};builds={}
        for label,extra in [('exact_o3',[]),('canonical_r4300_control',['-Wab,-r4300_mul'])]:
            d=work/label;d.mkdir();obj=d/'candidate.o';run([str(score.IDO/'cc'),'-c',*FLAGS.split(),*extra,'-o',str(obj),str(HERE/'candidate.c')])
            fn,raw=function(score,obj);words,rd,relocs=link(score,obj,d);assert len(words)*4==fn['size']
            common=sum(a!=b for a,b in zip(native,words));diff=common+abs(len(native)-len(words));comp=dataclasses.asdict(score.compare(obj,NAME,show=0))
            own=score.owndata.verify(obj,NAME,native,address=START,image=data,start=fn['value'],addresses=lambda n:score.image_symbols().get(n,score.address_named(n)))
            if label=='canonical_r4300_control':assert own.ok and rd==rodata and diff==156 and len(words)==410
            else:assert len(words)==409 and diff==285 and rd[44:]==rodata[44:]
            builds[label]={'flags':FLAGS+(' '+extra[0] if extra else ''),'elf_function_bytes':fn['size'],'native_bytes':1640,'complete_differing_positions':diff,'common_differing_words':common,'missing_words':max(0,len(native)-len(words)),'excess_words':max(0,len(words)-len(native)),'linked_body_sha256':sha(packed(words)),'frame_bytes':-signed(words[0]&65535,16),'relocations':relocs,'rodata_meaningful_bytes':60,'rodata_zero_padding_bytes':4,'rodata_sha256':sha(rd),'native_rodata_equal':rd==rodata,'canonical_diagnostic':comp}
            objects[label]=(words,rd)
        behavior_result=behavior(native,objects,rodata,work)
    receipt={'schema':'rush-eb028-donor-v1','status':'COMPLETE-NONMATCH','base_commit':BASE,'target':{'name':NAME,'start':hex(START),'end':hex(START+1640),'native_bytes':1640,'native_sha256':NATIVE_SHA},'packet_files':{n:sha((HERE/n).read_bytes()) for n in ['candidate.c','host.c','native.py','verify.py']},'builds':builds,'behavior':behavior_result}
    if output:Path(output).write_text(json.dumps(receipt,indent=2)+'\n')
    return receipt

def portable(receipt):
    receipt=copy.deepcopy(receipt)
    for build in receipt.get('builds',{}).values():build.pop('canonical_diagnostic',None)
    return receipt

def main():
    p=argparse.ArgumentParser();p.add_argument('--root');p.add_argument('--record',action='store_true');p.add_argument('--output');a=p.parse_args()
    if a.root is None:
        if len(HERE.parents)<4:p.error('--root is required outside the repository packet location')
        a.root=str(HERE.parents[3])
    r=verify(a.root,a.output)
    if a.record:(HERE/'verification.json').write_text(json.dumps(r,indent=2)+'\n')
    elif (HERE/'verification.json').exists():assert portable(r)==portable(json.loads((HERE/'verification.json').read_text())),'receipt changed'
    print(json.dumps({'status':r['status'],'builds':{k:{x:v[x] for x in ['elf_function_bytes','complete_differing_positions']} for k,v in r['builds'].items()},'cases':r['behavior']['cases']}))
if __name__=='__main__':main()
