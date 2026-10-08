#!/usr/bin/env python3
"""Compare complete native execution with host-compiled independent semantic C."""
import argparse,ctypes,hashlib,json,os,shutil,subprocess,tempfile
from pathlib import Path
from targets import load,BASE,EXPECTED,IMAGE_HASH
from native import Machine,STACK
from fixture import Fixture,PLAYERS,PHYSICS,CACHE,SLOTS,GROUPS
from native import fvalue,fbits
HERE=Path(__file__).resolve().parent

class Host:
    def __init__(self,path):
        self.lib=ctypes.CDLL(str(path))
        U=ctypes.c_uint32
        self.read_type=ctypes.CFUNCTYPE(U,U,U)
        self.write_type=ctypes.CFUNCTYPE(None,U,U,U)
        self.call_type=ctypes.CFUNCTYPE(U,U,U,U,U,U)
        self.lib.set_bus.argtypes=[self.read_type,self.write_type,self.call_type]
        self.lib.func_8038AA8C.argtypes=[U,ctypes.c_int16]
    def run(self,fixture):
        errors=[]
        def safe_read(a,n):
            try:return fixture.memory.get(a,n)
            except Exception as exc:errors.append(exc);return 0
        def safe_write(a,n,v):
            try:fixture.memory.put(a,v,n)
            except Exception as exc:errors.append(exc)
        def safe_call(d,a,b,c,e):
            try:return fixture.service(d,a,b,c,e)
            except Exception as exc:errors.append(exc);return 0
        callbacks=(self.read_type(safe_read),self.write_type(safe_write),self.call_type(safe_call))
        self.lib.set_bus(*callbacks);self.lib.func_8038AA8C(fixture.descriptor,fixture.update)
        if errors:raise errors[0]

def compare(code,image,host,kwargs,coverage,branches):
    actual,expected=Fixture(image,**kwargs),Fixture(image,**kwargs)
    native=Machine(code,actual);native.run();host.run(expected)
    if actual.trace!=expected.trace:
        for i,(a,b) in enumerate(zip(actual.trace,expected.trace)):
            if a!=b:raise AssertionError(('trace',kwargs,i,a,b))
        raise AssertionError(('trace length',kwargs,len(actual.trace),len(expected.trace),actual.trace,expected.trace))
    if actual.memory.snapshot()!=expected.memory.snapshot():
        for (a,x),(b,y) in zip(actual.memory.snapshot(),expected.memory.snapshot()):
            if x!=y:
                first=next(i for i in range(len(x)) if x[i]!=y[i])
                raise AssertionError(('memory',kwargs,hex(a+first),x[first:first+16].hex(),y[first:first+16].hex()))
    assert actual.objects==expected.objects,('scene state',kwargs)
    assert actual.random_count==expected.random_count
    # Offset loads must remain, including otherwise dead mode-8 alias loads.
    a=[(a,n) for _,a,n in actual.memory.reads if 0x803943A4<=a<0x80394920]
    b=[(a,n) for _,a,n in expected.memory.reads if 0x803943A4<=a<0x80394920]
    assert a==b,('offset/color read sequence',kwargs,a,b)
    coverage.update(native.coverage);branches.update(native.branches)

def run(root):
    code,image=load(root)
    cc=shutil.which('cc');assert cc,'host C compiler required'
    coverage,branches=set(),set();count=0
    with tempfile.TemporaryDirectory(prefix='aa8c-host-',dir=os.environ.get('TMPDIR')) as td:
        so=Path(td)/'semantic.so'
        command=[cc,'-std=c99','-O0','-ffp-contract=off','-fPIC','-shared','-Wall','-Wextra','-Werror',str(HERE/'semantic.c'),str(HERE/'host_bus.c'),'-o',str(so)]
        subprocess.run(command,check=True)
        host=Host(so)
        def check(**kw):
            nonlocal count
            compare(code,image,host,kw,coverage,branches);count+=1
        for owner in range(4):
            for mode in range(9):
                for cached in (9,mode,0,1):
                    for model in (0,6,12):
                        check(owner=owner,mode=mode,cached=cached,model=model)
            for cached in (0,1,8,9,127,128,255):
                for update,inhibit in ((0,0),(1,1),(1,128),(65536,0),(65537,0)):
                    check(owner=owner,cached=cached,update=update,inhibit=inhibit)
            for mode in (0,1):
                for view,view_type in ((0,0),(1,1),(-1,0),(2,2),(3,-1)):
                    for trigger in (4,5,127,128,255):
                        check(owner=owner,mode=mode,cached=mode,trigger=trigger,view=view,view_type=view_type,
                              state138=2,state139=2,state13A=1,value140=0.,value144=.07,
                              slot_active=1,slot_timer=.04)
            for flags,alpha in ((0,0),(1,0),(1,47),(1,48),(1,255),(0xFFFFFFFF,128)):
                check(owner=owner,mode=2,cached=2,flags=flags,alpha=alpha)
            for state138 in (-1,0,3,4):
                for state139 in (-1,0,3,4):
                    for timer in (.2,.016,0.,-.5):
                        check(owner=owner,mode=0,cached=0,state138=state138,state139=state139,
                              state13A=1,value140=timer,value144=timer)
            for mode in (0,1):
                for fraction in (0.,.249,.499,.749,.999):
                    for alias in (None,'slot','group'):
                        check(owner=owner,mode=mode,cached=mode,trigger=5,randoms=(fraction,),source_alias=alias)
            for threshold in (0.0333333,0.0666667):
                for bits in (fbits(threshold)-1,fbits(threshold),fbits(threshold)+1):
                    for mode in (0,1):
                        check(owner=owner,mode=mode,cached=mode,slot_active=1,slot_timer=fvalue(bits),
                              state13A=1,value144=fvalue(bits),dt=0.)
        for owner in range(4):
            for cached in (0,1):
                for mask in range(32):
                    for extra in (False,True):
                        check(owner=owner,cached=cached,update=0,live_mask=mask,extra_live=extra)
                check(owner=owner,cached=cached,update=0,
                      handle_values=(-1,0x1234FFFF,0x80000001,0xFFFF8000,0),extra_value=0x7FFFFFFF)
        for destination in (0x8008E06C,0x8008D6B0,0x8008E398,0x80090254,0x8008B2E4):
            def change_owner(f,d,destination=destination):
                if d==destination and not getattr(f,'mutated',False):
                    f.mutated=True
                    f.memory.put(f.descriptor+8,(f.owner+1)%4,2)
                    f.memory.put(f.descriptor+6,101+f.owner,2)
            for mode in (0,1):
                check(mode=mode,cached=1-mode,trigger=5,mutation=change_owner)
        def switch_cache(f,d):
            if d==0x8008B32C and not getattr(f,'mutated',False):
                f.mutated=True;f.memory.put(CACHE+f.owner,0,1)
        check(mode=1,cached=1,trigger=5,mutation=switch_cache)
        # The proved registration envelope is six. These guards/mode-8 cases
        # do not touch the unresolved out-of-bounds attachment-record domain.
        for owner in (4,5):
            for mode,cached,update in ((8,9,1),(8,8,1),(8,9,0),(3,3,1)):
                check(owner=owner,mode=mode,cached=cached,update=update)
        wrong=[]
        mutations=[
            ('erase mode8 loads', '        /* These three physical reads occur even for genuine mode 8. */',
             '        if (RS8(state + 0x384u) == 8) return;', dict(mode=8,cached=9)),
            ('resource word stride', '(u32)resource*2u', '(u32)resource*4u', dict(mode=2,cached=9)),
            ('wrong extra resource slot', 'R32(0x80399B58u)', 'R32(0x80399B54u)', dict(mode=0,cached=9)),
            ('wrong cleanup trailing byte', 'W8(GROUP(player) + 0x139u, 255)', 'W8(GROUP(player) + 0x138u, 255)', dict(cached=0,update=0)),
            ('random initial texture instead of slot0', 'texture = RU16(0x80399B08u);',
             'texture = RU16(0x80399B08u + (u32)RS8(GROUP(OWNER()) + 0x139u)*2u);',
             dict(mode=0,cached=0,trigger=5,randoms=(.625,))),
            ('short group muzzle duration', '0.0666667f <= RF', '0.0333333f <= RF',
             dict(mode=0,cached=0,state13A=1,value144=.04,dt=0.)),
            ('wrong slot pair wrap', 'if (selected >= 5) selected = 1;', 'if (selected >= 5) selected = 0;',
             dict(mode=1,cached=1,trigger=5,randoms=(.999,))),
            ('narrow sentinel comparison', 'if (R32(address) != 0xFFFFFFFFu)', 'if ((s16)R32(address) != -1)',
             dict(cached=0,update=0,handle_values=(-1,0x1234FFFF,0x80000001,0xFFFF8000,0))),
            ('early sentinel store', 'scene_remove((s16)R32(address));\n            W32(address, 0xFFFFFFFFu);',
             '{ s16 old = (s16)R32(address); W32(address, 0xFFFFFFFFu); scene_remove(old); }',
             dict(cached=0,update=0)),
            ('drop fresh second cached-kind test', 'if (RS8(CACHE(OWNER())) != 0) return;',
             'if (RS8(state + 0x384u) != 0) return;', dict(mode=1,cached=1,trigger=5,mutation=switch_cache)),
        ]
        original=(HERE/'semantic.c').read_text()
        for index,(label,old,new,kw) in enumerate(mutations):
            assert old in original,label
            mutant=Path(td)/('mutant%d.c'%index)
            mutant.write_text(original.replace(old,new))
            obj=Path(td)/('mutant%d.so'%index)
            subprocess.run([cc,'-std=c99','-O0','-ffp-contract=off','-fPIC','-shared','-I',str(HERE),
                            str(mutant),str(HERE/'host_bus.c'),'-o',str(obj)],check=True)
            try:compare(code,image,Host(obj),kw,set(),set())
            except AssertionError:wrong.append(label)
            else:raise AssertionError(('wrong source survived',label))
        # Structural unreachable words: each follows the delay slot of an
        # unconditional branch and no direct branch/switch enters either word.
        dead={0x8038AB8C,0x8038C488}
        assert set(code)-coverage==dead
        jump_targets={((a+4)&0xF0000000)|((w&0x3FFFFFF)<<2) for a,w in code.items() if w>>26 in (2,3)}
        direct_targets={a+4+(((w&65535)-65536 if w&32768 else w&65535)*4)
                        for a,w in code.items() if w>>26 in (1,4,5,20,21) or (w>>26==17 and ((w>>21)&31)==8)}
        switch_targets={int.from_bytes(image[a-0x8038A400:a-0x8038A400+4],'big') for a in range(0x80394D20,0x80394D44,4)}
        assert not dead & (jump_targets|direct_targets|switch_targets)
        for a in dead:
            w=code[a-8];assert w>>26==4 and ((w>>21)&31)==0 and ((w>>16)&31)==0
        calls={a for a,w in code.items() if w>>26==3}
        assert calls<=coverage
        # Fail closed for corrupt opcode/unmapped owner; neither is gameplay.
        for label,broken,owner in (('unsupported opcode',{**code,0x8038AA8C:0xFFFFFFFF},0),
                                  ('unmapped owner',code,32767)):
            fixture=Fixture(image)
            fixture.memory.put(fixture.descriptor+8,owner,2)
            try:Machine(broken,fixture).run()
            except AssertionError:wrong.append(label)
            else:raise AssertionError(('negative control survived',label))
        return {'paired_fixtures':count,'covered_words':len(coverage),'total_words':len(code),
                'branch_outcomes':len(branches),'covered_direct_calls':len(calls),
                'structurally_unreachable_words':[hex(x) for x in sorted(dead)],'wrong_contracts_rejected':wrong,
                'domain':'owners 0..3 for attachment paths; guarded non-attachment owners 4/5; modes 0..8; models 0,6,12; finite f32 animation fixtures',
                'host_compiler':subprocess.check_output([cc,'--version'],text=True).splitlines()[0],
                'host_flags':'-std=c99 -O0 -ffp-contract=off -fPIC -shared -Wall -Wextra -Werror',
                'IDO_invocations':0,'matching_claims':[]}


def replay(root):
    return {'status':'COMPLETE PHYSICAL-ADDRESS SEMANTIC MODEL; original C object/domain unresolved; no MIPS candidate',
        'base':BASE,'native_targets':{n:{'address':hex(a),'size':size,'sha256':sha} for n,(a,size,sha) in EXPECTED.items()},
        'image_sha256':IMAGE_HASH,'behavior':run(root.resolve()),
        'packet_sha256':{name:hashlib.sha256((HERE/name).read_bytes()).hexdigest() for name in
        ('semantic.c','semantic_bus.h','host_bus.c','native.py','fixture.py','targets.py','verify.py','README.md')}}

def comparable(receipt):
    receipt=json.loads(json.dumps(receipt))
    receipt['behavior'].pop('host_compiler',None)
    return receipt

def main():
    p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True)
    p.add_argument('--output',type=Path,default=HERE/'verification.json');p.add_argument('--check',action='store_true')
    args=p.parse_args();r=replay(args.reference_root)
    if args.check:assert comparable(r)==comparable(json.loads(args.output.read_text())), 'receipt replay differs'
    else:args.output.write_text(json.dumps(r,indent=2)+'\n')
    print(json.dumps(r,indent=2))
if __name__=='__main__':main()
