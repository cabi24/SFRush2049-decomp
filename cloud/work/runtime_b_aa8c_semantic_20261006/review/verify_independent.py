#!/usr/bin/env python3
"""Independent adversarial checks of the host-C AA8C semantic model.

Reads authenticated native targets, never emits native code/image bytes. This is
conditional semantic research under the packet's explicit service boundaries.
"""
import argparse, ctypes, hashlib, json, math, os, random, shutil, subprocess, sys, tempfile
from collections import Counter
from pathlib import Path
HERE=Path(__file__).resolve().parent
_early=argparse.ArgumentParser(add_help=False)
_early.add_argument('--packet',type=Path,default=HERE/'packet')
_early_args,_unused=_early.parse_known_args()
PACKET=_early_args.packet.resolve()
sys.path.insert(0,str(PACKET))
import fixture, native, targets, verify
from fixture import PLAYERS,PHYSICS,CACHE,SLOTS,GROUPS,SCENE,SOURCE,DEST
from native import fbits,fvalue,f32

class ProbeFixture(fixture.Fixture):
    def __init__(self,image,probe=None,**kwargs):
        super().__init__(image,**kwargs)
        self.probe=probe or {}
        p=self.probe;m=self.memory
        # Different sibling state exposes a wrongly refreshed retained state pointer.
        if p.get('distinct_siblings'):
            for owner in range(4):
                if owner==self.owner:continue
                state=PLAYERS+owner*0x3B8
                m.put(state+0x384,2,1);m.put(state+0x35C,128,1)
                m.put(state+0x3A0,128,1);m.put(state+0x34C,0xC0DE1200+owner)
                m.put(PHYSICS+owner*0x808+8,12,1)
        if 'seed' in p:
            rng=random.Random(p['seed'])
            for owner in range(4):
                source=self.objects[10+owner]['transform']
                for i in range(12):
                    # Non-dyadic mantissas with cancellation and varied exponents.
                    m.wf(source+4*i,math.ldexp(rng.uniform(-1,1),rng.randrange(-16,17)))
        if 'source_offset' in p:
            base=(SLOTS if kwargs.get('mode')==1 else GROUPS)+self.owner*(0x10C if kwargs.get('mode')==1 else 0x148)+0x14
            source=base+p['source_offset']
            self.objects[10+self.owner]['transform']=source
            for i,v in enumerate((1.3,-2.7,.125,-.75,1.0000001,2.2,.333333,-5.1,.0009,1e5,-1e-5,20.)):
                m.wf(source+4*i,v)
        m.reads.clear();m.writes.clear()
    def service(self,d,a,b,c,e):
        result=super().service(d,a,b,c,e)
        p=self.probe;m=self.memory
        if self.service_count == p.get('mutate_at'):
            owner=(self.owner+1)%4
            action=p['action']
            if action=='owner':
                m.put(self.descriptor+8,owner,2);m.put(self.descriptor+6,100+owner,2)
            elif action=='retained_transform':
                self.objects[10+self.owner]['transform']=DEST+self.owner*48
            elif action=='live_state':
                state=PLAYERS+self.owner*0x3B8
                m.put(state+0x35C,3,1);m.put(state+0x35D,1,1)
                m.put(state+0x3A0,5,1);m.put(PHYSICS+self.owner*0x808+8,12,1)
                m.put(0x80394884,0xFECDBA98)
            elif action=='source_writes':
                source=self.objects[10+self.owner]['transform']
                for i in range(12):m.wf(source+i*4,(i-5)*.1234567)
            else:raise AssertionError(action)
        return result

def compile_host(directory,source,optimization='-O0',sanitize=False):
    directory.mkdir(parents=True,exist_ok=True)
    cfile=directory/'semantic.c';cfile.write_text(source)
    so=directory/'semantic.so'
    cc=shutil.which('cc');assert cc,'host C compiler required'
    command=[cc,'-std=c99',optimization,'-ffp-contract=off','-fPIC','-shared','-Wall','-Wextra','-Werror','-I',str(PACKET),str(cfile),str(PACKET/'host_bus.c'),'-o',str(so)]
    if sanitize:command[1:1]=['-fsanitize=undefined','-fno-sanitize-recover=undefined']
    subprocess.run(command,check=True,capture_output=True)
    return verify.Host(so)

def compare(code,image,host,kwargs,probe):
    a,b=ProbeFixture(image,probe,**kwargs),ProbeFixture(image,probe,**kwargs)
    vm=native.Machine(code,a);vm.run();host.run(b)
    assert a.trace==b.trace,('trace mismatch',kwargs,probe,next(((i,x,y) for i,(x,y) in enumerate(zip(a.trace,b.trace)) if x!=y),('length',len(a.trace),len(b.trace))))
    assert a.memory.snapshot()==b.memory.snapshot(),('memory mismatch',kwargs,probe)
    assert a.objects==b.objects,('retained scene state mismatch',kwargs,probe)
    assert a.random_count==b.random_count
    # Observe all external memory stores, preserving order, not just final bytes.
    def stores(f):return [(a,n,v) for pc,a,n,v in f.memory.writes if not native.STACK-256<=a<native.STACK+256]
    assert stores(a)==stores(b),('store order mismatch',kwargs,probe)
    def offset_reads(f):return [(a,n) for pc,a,n in f.memory.reads if 0x803943A4<=a<0x80394920]
    assert offset_reads(a)==offset_reads(b),('offset load sequence mismatch',kwargs,probe)
    return vm,a

def static_reachable(code,image):
    """Delay-slot-aware conservative CFG; direct service calls may return."""
    todo=[native.ROOT];seen=set()
    while todo:
        pc=todo.pop()
        if pc in seen:continue
        assert pc in code,('unrecognized static target',hex(pc))
        seen.add(pc);word=code[pc];op=word>>26
        rs,rt=(word>>21)&31,(word>>16)&31
        control=op in (1,3,4,5,20,21) or op==17 and rs==8 or op==0 and word&63==8
        if not control:
            todo.append(pc+4);continue
        assert pc+4 in code
        seen.add(pc+4)
        if op==0:
            if rs!=31:
                assert pc==0x8038AE28 and rs==25,'unknown non-return indirect jump'
                # Authenticated native nine-way switch table. Do not emit bytes.
                for address in range(0x80394D20,0x80394D44,4):
                    offset=address-0x8038A400
                    todo.append(int.from_bytes(image[offset:offset+4],'big'))
        elif op==3:
            target=((pc+4)&0xf0000000)|((word&0x3FFFFFF)<<2)
            if target in code:todo.append(target)
            todo.append(pc+8)
        else:
            target=pc+4+4*native.signed(word,16)
            if op in (4,20) and rs==rt:todo.append(target)
            elif op in (5,21) and rs==rt:todo.append(pc+8)
            else:todo.extend((target,pc+8))
    return seen

def run(root):
    code,image=targets.load(root)
    statically_dead=set(code)-static_reachable(code,image)
    assert statically_dead=={0x8038AB8C,0x8038C488},('unexpected static dead code',statically_dead)
    source=(PACKET/'semantic.c').read_text()
    cases=[]
    for owner in range(4):
        for cached in (0,1,8,9,127,128,255):
            for update,inhibit in ((0,0),(1,1),(1,128),(65536,0),(65537,0)):
                cases.append(('entry_guards',dict(owner=owner,cached=cached,update=update,inhibit=inhibit),{}))
        for mode in (0,1):
            for mask in range(32):
                for extra in (False,True):
                    cases.append(('cleanup_masks',dict(owner=owner,mode=mode,cached=1-mode,update=0,live_mask=mask,extra_live=extra),{}))
            cases.append(('cleanup_widths',dict(owner=owner,cached=mode,update=0,handle_values=(-1,0x1234FFFF,0x80000001,0xFFFF8000,0),extra_value=0x7FFFFFFF),{}))
        for flags,alpha in ((0,0),(1,0),(1,47),(1,48),(1,255),(0xFFFFFFFF,128)):
            cases.append(('alpha_clamp',dict(owner=owner,mode=2,cached=2,flags=flags,alpha=alpha),{}))
        for mode in range(9):
            for model in range(13):
                for cached in (9,mode):
                    cases.append(('all_models',dict(owner=owner,mode=mode,cached=cached,model=model),dict(seed=owner*117+mode*13+model)))
    for mode in (0,1):
        for cached in (9,mode):
            for delta in (0,4,8,12,20,28,32,36,40,44,48,52):
                cases.append(('source_overlap',dict(mode=mode,cached=cached,trigger=5),dict(source_offset=delta)))
    for mode in (0,1):
        for numerator in (0,1,8191,8192,16383,16384,24575,24576,32767):
            cases.append(('rng_grid_edges',dict(mode=mode,cached=mode,trigger=5,randoms=(numerator/32768.,)),{}))
        for bits in (0,0x80000000,1,0x80000001,0x007FFFFF,0x00800000,fbits(.0333333)-1,fbits(.0333333),fbits(.0333333)+1,fbits(.0666667)-1,fbits(.0666667),fbits(.0666667)+1):
            cases.append(('timer_float_edges',dict(mode=mode,cached=mode,slot_active=1,slot_timer=fvalue(bits),state138=2,value140=fvalue(bits),state13A=1,value144=fvalue(bits),dt=0.),{}))
    rng=random.Random(0xAA8C)
    for i in range(256):
        mode=rng.randrange(2)
        cases.append(('rounded_float',dict(owner=rng.randrange(4),mode=mode,cached=mode,
          trigger=rng.choice((0,4,5,127,128,255)),view=rng.choice((0,1,3,31,32,127,128,255)),
          view_type=rng.choice((0,1,2,127,128,255)),state138=rng.choice((-1,0,1,2,3,4)),
          state139=rng.randrange(5),state13A=rng.choice((0,1,0x8000,0xFFFF)),slot_active=rng.choice((0,1,0x80000000)),
          value140=rng.uniform(-.1,.1),value144=rng.uniform(-.1,.1),slot_timer=rng.uniform(-.1,.1),
          value13C=rng.uniform(-2,3),dt=rng.uniform(-.03,.03),randoms=tuple(rng.random() for _ in range(13))),dict(seed=i)))
    coverage=set();branches=set();counts=Counter();mutations=[]
    with tempfile.TemporaryDirectory(prefix='aa8c-independent-',dir=os.environ.get('TMPDIR')) as td:
        td=Path(td);host=compile_host(td/'original',source)
        for mode in (0,1):
            for cached in (9,mode):
                kw=dict(mode=mode,cached=cached,trigger=5,state138=1,state139=3,state13A=1,value140=0.,value144=.07,slot_active=1,slot_timer=.04)
                vm,baseline=compare(code,image,host,kw,{})
                for at in range(1,baseline.service_count+1):
                    for action in ('owner','retained_transform','live_state','source_writes'):
                        cases.append(('helper_mutation',kw,dict(mutate_at=at,action=action,distinct_siblings=True)))
        for label,kw,probe in cases:
            vm,_=compare(code,image,host,kw,probe)
            coverage.update(vm.coverage);branches.update(vm.branches);counts[label]+=1
        # Actual host compiler optimizes the same source, with sanitizers enabled.
        optimized=compile_host(td/'optimized',source,'-O3',True)
        for label,kw,probe in cases:
            compare(code,image,optimized,kw,probe)
        controls=[
          ('wrong resource','case 2: resource = 218;','case 2: resource = 217;',dict(mode=2,cached=9),{}),
          ('narrow sentinel','if (R32(address) != 0xFFFFFFFFu)','if ((s16)R32(address) != -1)',dict(update=0,cached=0,handle_values=(0x1234FFFF,-1,-1,-1,-1)),{}),
          ('wrong extra ID','R32(0x80399B58u)','R32(0x80399B54u)',dict(mode=0,cached=9),{}),
          ('wrong trigger signedness','RS8(state + 0x3A0u) >= 5','RU8(state + 0x3A0u) >= 5',dict(mode=1,cached=1,trigger=128),{}),
          ('wrong texture reset','texture = RU16(0x80399B08u);','texture = RU16(0x80399B08u + 2u*RU8(GROUP(OWNER()) + 0x139u));',dict(mode=0,cached=0,trigger=5,randoms=(.875,)),{}),
          ('skip mode8 physical loads','offset[0] = RF(OFFSET_ADDR());','offset[0] = RS8(state + 0x384u) == 8 ? 0.0f : RF(OFFSET_ADDR());',dict(mode=8,cached=9),{}),
          ('erase live cache reread','if (RS8(CACHE(OWNER())) != 0) return;','if (RS8(state + 0x384u) != 0) return;',dict(mode=1,cached=1,trigger=5,mutation=lambda f,d: f.memory.put(CACHE+f.owner,0,1) if d==0x8008B32C else None),{}),
          ('refresh retained player','state = PLAYER(owner);','state = PLAYER(owner);',{},{}),
        ]
        # Retained-state mutation changes descriptor owner in the first color service.
        controls[-1]=('refresh retained player','source = scene_transform','state = PLAYER(OWNER()); source = scene_transform',dict(mode=0,cached=9,trigger=5),dict(mutate_at=1,action='owner',distinct_siblings=True))
        for index,(label,old,new,kw,probe) in enumerate(controls):
            assert old in source,label
            changed=source.replace(old,new)
            mutated=compile_host(td/('wrong-'+str(index)),changed)
            try:compare(code,image,mutated,kw,probe)
            except AssertionError:mutations.append(label)
            else:raise AssertionError(('wrong source survived',label))
    return dict(status='INDEPENDENT_FULL_BODY_SEMANTIC_REVIEW_PASS',base_commit=targets.BASE,
       native_targets={n:dict(address=hex(a),size=s,sha256=h) for n,(a,s,h) in targets.EXPECTED.items()},
       packet_sha256={n:hashlib.sha256((PACKET/n).read_bytes()).hexdigest() for n in ('semantic.c','semantic_bus.h','host_bus.c','native.py','fixture.py','targets.py','verify.py')},
       review_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
       extra_pairs_per_host_configuration=len(cases),host_configurations=['cc C99 O0, ffp-contract=off','cc C99 O3, ffp-contract=off, UBSan no-recovery'],
       fixture_categories=dict(counts),statically_unreachable_words=[hex(a) for a in sorted(statically_dead)],native_words=len(coverage),branch_outcomes=len(branches),uncovered=[hex(a) for a in sorted(set(code)-coverage)],
       source_negative_controls_rejected=mutations,
       limits=['shared explicit service effect models','no native scene-helper integration or FCSR proof','owners 4/5 attachment-storage domain unresolved','original external C objects unresolved','no IDO compilation, strict MATCH, or gameplay claim'])

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--packet',type=Path,default=HERE/'packet');p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--output',type=Path)
    a=p.parse_args();r=json.dumps(run(a.reference_root.resolve()),indent=2)+'\n'
    if a.output:a.output.write_text(r)
    else:print(r,end='')
