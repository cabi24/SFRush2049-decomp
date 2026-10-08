#!/usr/bin/env python3
"""Native source-object/domain evidence. No native bytes or disassembly emitted."""
import argparse,hashlib,importlib.util,itertools,json,struct,subprocess,sys,zlib
from pathlib import Path
BASE='6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
IBASE={'blob':0x80086A50,'ovl_a':0x8038A400,'ovl_b':0x8038A400}
ROM={'blob':0xB0CB10,'ovl_a':0xB5C534,'ovl_b':0xB6FEC4}
NAMES={'blob':['init_state_continue','world_physics_tick','InitMaxPath','PrevMaxPath'],
       'ovl_a':['func_80390418'],
       'ovl_b':['func_8038AA8C','func_8038A408','func_8038C910','func_8038F568','func_8038FCE0']}
COUNT,TOTAL,MODE,CONFIG,PHYSICS=0x8014A108,0x801543CA,0x8014A110,0x80153E88,0x8014A250
RET=0xfffffffc

def git(root,path): return subprocess.check_output(['git','-C',str(root),'show',BASE+':'+path])
def signed(v,n=32):
 v &= (1<<n)-1
 return v-(1<<n) if v>>(n-1) else v

def load(root):
 # Support canonical score.py sibling imports from any working directory.
 sys.path.insert(0, str(root / 'tools/cloud'))
 spec=importlib.util.spec_from_file_location('source_object_score',root/'tools/cloud/score.py')
 score=importlib.util.module_from_spec(spec);sys.modules[spec.name]=score;spec.loader.exec_module(score)
 asset=git(root,'assets/us/data.bin');images={};codes={};targets={}
 for group,names in NAMES.items():
  z=zlib.decompressobj(-15);img=z.decompress(asset[ROM[group]-0x283D0:]);assert z.eof
  images[group]=img;codes[group]={};score.ASM_DIR=root/'asm/us'/group;ts=score.targets()
  syms=json.loads(git(root,'asm/us/'+group+'/symbols.json'))['symbols']
  for name in names:
   words=ts[name];addr=int(syms[name],16);raw=struct.pack('>'+'I'*len(words),*words)
   assert img[addr-IBASE[group]:addr-IBASE[group]+len(raw)]==raw,(group,name,'native changed')
   codes[group].update({addr+4*i:w for i,w in enumerate(words)})
   targets[group+':'+name]={'address':hex(addr),'size':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
 return images,codes,targets

class Memory:
 def __init__(self):self.data={};self.writes=[]
 def region(self,a,n):self.data.update({a+i:0xa5 for i in range(n)})
 def put(self,a,v,n=4):
  assert a%n==0 and all(a+i in self.data for i in range(n)),('write unmapped',hex(a),n)
  self.data.update({a+i:b for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big'))});self.writes.append((a,n))
 def get(self,a,n=4):
  assert a%n==0 and all(a+i in self.data for i in range(n)),('read unmapped',hex(a),n)
  return int.from_bytes(bytes(self.data[a+i] for i in range(n)),'big')

class Machine:
 def __init__(self,code,m):self.code=code;self.m=m;self.r=[None]*32;self.r[0]=0;self.r[31]=RET;self.coverage=set()
 def reg(self,i):
  assert self.r[i] is not None,('undefined register',hex(self.pc),i)
  return self.r[i]
 def run(self,start,stop=RET):
  pc=start;pending=None
  for _ in range(10000):
   if pc==stop:return
   self.pc=pc;assert pc in self.code,('unknown pc',hex(pc));self.coverage.add(pc)
   w=self.code[pc];op=w>>26;rs=(w>>21)&31;rt=(w>>16)&31;rd=(w>>11)&31;sh=(w>>6)&31;fn=w&63;imm=signed(w,16)
   old=pending;pending=None;nxt=pc+4
   if op==0:
    if fn==0:self.r[rd]=self.reg(rt)<<sh
    elif fn==3:self.r[rd]=signed(self.reg(rt))>>sh
    elif fn==0x21:self.r[rd]=self.reg(rs)+self.reg(rt)
    elif fn==0x23:self.r[rd]=self.reg(rs)-self.reg(rt)
    elif fn==0x25:self.r[rd]=self.reg(rs)|self.reg(rt)
    elif fn==0x2a:self.r[rd]=int(signed(self.reg(rs))<signed(self.reg(rt)))
    elif fn==0x2b:self.r[rd]=int(self.reg(rs)<self.reg(rt))
    elif fn==8:pending=self.reg(rs)
    else:raise AssertionError(('SPECIAL',hex(pc),fn))
   elif op in (9,10,11,12,13):
    v=self.reg(rs)
    self.r[rt]={9:lambda:v+imm,10:lambda:int(signed(v)<imm),11:lambda:int(v<(imm&0xffffffff)),12:lambda:v&(w&65535),13:lambda:v|(w&65535)}[op]()
   elif op==15:self.r[rt]=(w&65535)<<16
   elif op in (4,5,6,7,20,21,22,23):
    v=self.reg(rs);k=op&15
    take= (v==self.reg(rt)) if k==4 else (v!=self.reg(rt)) if k==5 else (signed(v)<=0) if k==6 else (signed(v)>0)
    if take:pending=pc+4+imm*4
    elif op>=20:nxt+=4
   elif op in (32,33,35,36):
    n=4 if op==35 else 2 if op==33 else 1;v=self.m.get((self.reg(rs)+imm)&0xffffffff,n)
    self.r[rt]=signed(v,n*8) if op in (32,33) else v
   elif op in (40,41,43):
    n={40:1,41:2,43:4}[op];self.m.put((self.reg(rs)+imm)&0xffffffff,self.reg(rt),n)
   elif op==17 and rs==4:
    self.reg(rt) # mtc1; no subsequent FP use in these execution slices
   else:raise AssertionError(('opcode',hex(pc),op))
   self.r=[None if v is None else v&0xffffffff for v in self.r];self.r[0]=0
   assert old is None or pending is None,'branch in delay slot'
   pc=old if old is not None else nxt
  raise AssertionError('step limit')

def mem():
 m=Memory()
 for a,n in [(COUNT,2),(TOTAL,2),(MODE,4),(0x80142724,2),(0x8014978C,1),(CONFIG,48),(PHYSICS,6*0x808),(0x80156CF0,64),(0x803B6A6C,4)]:m.region(a,n)
 return m

def verify(root):
 images,codes,targets=load(root);coverage={g:set() for g in codes};cases=[]
 # Full actual setup body, no service stubs or inferred writes.
 for mode in (4,5,6):
  for count in range(7):
   for ai in (-1,0,1,6,32767):
    m=mem();m.put(COUNT,count,2);m.put(MODE,mode);m.put(0x80142724,ai,2);m.put(0x8014978C,3,1)
    before=bytes(m.data[CONFIG+i] for i in range(48));cpu=Machine(codes['blob'],m);cpu.run(0x800FAF6C);coverage['blob']|=cpu.coverage
    assert m.get(TOTAL,2)==count
    active=[]
    for i in range(6):
     assert m.get(CONFIG+i*8+6,1)==(176 if i<count else 0)
     assert m.get(CONFIG+i*8+7,1)==(6 if i<count else 7)
     assert bytes(m.data[CONFIG+i*8+j] for j in range(1,5))==before[i*8+1:i*8+5]
     # Actual world_physics_tick flag projection, stopping before the active branch.
     cpu=Machine(codes['blob'],m);cpu.r[16]=PHYSICS+i*0x808;cpu.r[18]=CONFIG+i*8
     cpu.run(0x800EC46C,0x800EC484);coverage['blob']|=cpu.coverage
     if m.get(PHYSICS+i*0x808+0x7c8,2):active.append(i)
    assert active==list(range(count))
    cases.append((mode,count,ai,active))
 # Native UI scan/clamp: count is consecutive controller statuses equal to one.
 ui=0
 for statuses in itertools.product((0,1,2),repeat=4):
  connected=next((i for i,x in enumerate(statuses) if x!=1),4)
  for count in range(1,8):
   m=mem();m.put(COUNT,count,2)
   for i,v in enumerate(statuses):m.put(0x80156CF0+i*16,v,1)
   cpu=Machine(codes['ovl_a'],m);cpu.run(0x80390560,0x803905D4);coverage['ovl_a']|=cpu.coverage
   assert m.get(0x803B6A6C)==connected
   assert m.get(COUNT,2)==min(count,max(1,connected))
   ui+=1
 # Metadata cannot establish original data object ownership.
 syms=json.loads(git(root,'asm/us/ovl_b/symbols.json'))['symbols'];addrs={int(v,16) for v in syms.values()}
 absent=[a for a in (0x803943A4,0x80394884,0x80394888,0x803948B8) if a not in addrs];assert len(absent)==4
 e=json.loads(git(root,'asm/us/ovl_b/extents.json'));assert IBASE['ovl_b']+len(images['ovl_b'])==0x80394F70
 assert e['text_end']=='0x80393B34'
 # Address overlap arithmetic is metadata, never an object declaration.
 model8=[{'model':i,'start':hex(0x803943A4+8*156+i*12),'end':hex(0x803943A4+8*156+i*12+12)} for i in range(13)]
 return {'status':'SOURCE_OBJECT_BLOCKED_WITH_NARROWED_OWNER_CONTRACT','base_commit':BASE,
 'targets':targets,'fixture_counts':{'setup_full_native':len(cases),'flag_projection':len(cases)*6,'UI_scan_clamp':ui},
 'coverage':{g:len(v) for g,v in coverage.items()},
 'owner_result':'After actual mode-6 configuration with count 1..4 and the physics projection, slots 4/5 are inactive. Counts 5/6 are native counterexamples; no whole-program invariant claimed.',
 'ui_result':'For positive incoming count, a scan of four 16-byte controller entries clamps it to 1..4. It does not establish every caller passed through this path.',
 'counterexamples':[{'count':n,'active_owners':list(range(n))} for n in (5,6)],
 'image_metadata':{'initialized_start':'0x8038a400','initialized_end':'0x80394f70','bss_end_from_loader':'0x8039b440','queried_object_symbols_absent':[hex(a) for a in absent]},
 'mode8_address_intervals':model8,
 'limits':['Original data declaration, extent and effective type remain unavailable.','Setup ordering and every active-count/config producer are not closed.','No IDO compile, strict match, or gameplay proof.'],
 'owned_files':{p:hashlib.sha256((Path(__file__).parent/p).read_bytes()).hexdigest() for p in ('verify.py','README.md')}}

if __name__=='__main__':
 p=argparse.ArgumentParser();p.add_argument('--reference-root',type=Path,required=True);p.add_argument('--output',type=Path);a=p.parse_args();r=verify(a.reference_root.resolve());s=json.dumps(r,indent=2)+'\n'
 if a.output:a.output.write_text(s)
 print(s)
