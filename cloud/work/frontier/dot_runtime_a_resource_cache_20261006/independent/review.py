"""Independent native-source contract review; authenticated inputs are local only."""
import ctypes, hashlib, json, os, random, re, struct, subprocess, sys
from pathlib import Path
HERE=Path(__file__).resolve().parent
ROOT=Path(os.environ.get('RUSH_REVIEW_REPO',HERE.parents[4])).resolve()
OUT=HERE.parents[4]/'build/runtime_a_cache_independent'
OUT.mkdir(parents=True,exist_ok=True)
if not __debug__:raise RuntimeError('Python optimization is unsupported')
os.environ['TMPDIR']=str(OUT)
SOURCE=Path(sys.argv[1]).resolve()
ENTRY, END=0x80390BC0,0x80390D2C
CACHE,TABLE,RESULT,SP=0x803BA230,0x80142B08,0x80400000,0x807F0000
CALL1,CALL2=0x80097798,0x800BB02C

def sha(x): return hashlib.sha256(x).hexdigest()
def shell(*x): return subprocess.check_output([str(a) for a in x],stderr=subprocess.STDOUT).decode()
def sx(n,b=32):
 n&=(1<<b)-1;return n if n<1<<(b-1) else n-(1<<b)
def manifest(folder):
 answer={}
 for line in (folder/'SHA256SUMS').read_text().splitlines():
  digest,name=line.split('  '); assert sha((folder/name).read_bytes())==digest;answer[name]=digest
 return answer
def target(folder,name):
 for p in folder.glob('*.s'):
  m=re.search(r'^\.section \.text\.'+name+r',.*?(?=^\.section|\Z)',p.read_text(),re.M|re.S)
  if m:
   w=[int(x,16) for x in re.findall(r'^\s*\.word\s+(0x[0-9a-fA-F]+)',m[0],re.M)]
   return struct.pack('>%dI'%len(w),*w)
 raise AssertionError(name)
def elf(path):
 b=path.read_bytes(); assert b[:6]==b'\x7fELF\x01\x02'; offset=struct.unpack_from('>I',b,32)[0]; size,count,namesidx=struct.unpack_from('>HHH',b,46)
 ss=[struct.unpack_from('>10I',b,offset+i*size) for i in range(count)]
 def string(sec,off):
  start=sec[4]+off;return b[start:b.index(0,start)].decode()
 names={string(ss[namesidx],s[0]):s for s in ss};tables={};symbols={};relocs=[]
 for idx,s in enumerate(ss):
  if s[1]==2:
   syms=[]
   for off in range(s[4],s[4]+s[5],16):
    n,v,z,info,other,ndx=struct.unpack_from('>IIIBBH',b,off);syms.append(dict(name=string(ss[s[6]],n),value=v,size=z,info=info,section=ndx))
   tables[idx]=syms;symbols.update((x['name'],x) for x in syms)
 for s in ss:
  if s[1] in (4,9):
   assert s[1]==9
   for off in range(s[4],s[4]+s[5],8):
    loc,info=struct.unpack_from('>II',b,off);relocs.append(dict(offset=loc,type=info&255,symbol=tables[s[6]][info>>8]['name'],target_section=s[7]))
 s=names['.text']; return dict(text=b[s[4]:s[4]+s[5]],text_address=s[3],symbols=symbols,sections=names,relocs=relocs,entry=struct.unpack_from('>I',b,24)[0])

def execute(body,records,id,kind,handle,mutation,seed):
 words=struct.unpack('>91I',body);rng=random.Random(seed);r=[0]+[rng.getrandbits(32) for _ in range(31)]
 r[4]=id&0xffffffff;r[5]=kind&0xffffffff;r[6]=RESULT;r[29]=SP;r[31]=0x81818180;initial=r[:]
 mem={}; trace=[];cov=set();branches=set();access=[]
 def put(a,v,n=4):
  for k,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):mem[a+k]=b
 def get(a,n=4):
  assert a%n==0;return int.from_bytes(bytes(mem[a+k] for k in range(n)),'big')
 for i,(h,loaded,ident,reserve) in enumerate(records):
  put(CACHE+i*12,h);put(CACHE+i*12+4,loaded,1);put(CACHE+i*12+5,ident,1)
  for j,b in enumerate(reserve):put(CACHE+i*12+6+j,b,1)
 put(RESULT,0x89abcdef)
 # Native address model permits signed indices; host C comparison only uses
 # nonnegative entries or branches that provably never index the table.
 put(TABLE+2*sx(id,16),0x5a5a,2)
 def snapshot():
  return [get(CACHE+i*12) for i in range(16)], [get(CACHE+i*12+4,1) for i in range(16)], [get(CACHE+i*12+5,1) for i in range(16)],get(RESULT),get(TABLE+2*sx(id,16),2)
 pc=ENTRY;pending=None
 for step in range(2500):
  if pc==initial[31]:
   assert pending is None and all(r[x]==initial[x] for x in list(range(16,24))+[28,29,30,31]); assert get(SP)==initial[4] and get(SP+4)==initial[5]
   return (r[2],snapshot(),trace),cov,branches,access
  if pc in (CALL1,CALL2):
   args=[r[4],r[5],r[6],r[7],get(r[29]+16)] if pc==CALL1 else [r[4],r[5],r[6]]
   trace.append([pc,args,snapshot()])
   if mutation:
    # Helpers may touch externally visible records. Selected handle is
    # overwritten after call one; second-call mutations survive the return.
    put(CACHE+12*3+4,0x80,1);put(CACHE+12*4,0x76543210)
    if pc==CALL2:put(RESULT,0x12345678);put(TABLE+2*sx(id,16),0xfedc,2)
   ret=r[31]
   for x in [1,2,3,*range(4,16),24,25]:r[x]=rng.getrandbits(32)
   r[2]=handle&0xffffffff if pc==CALL1 else rng.getrandbits(32);r[0]=0;pc=ret;assert pending is None;continue
  assert ENTRY<=pc<END and pc%4==0
  off=pc-ENTRY;cov.add(off);w=words[off//4];op,s,t,d=w>>26,w>>21&31,w>>16&31,w>>11&31;imm=sx(w,16);dest=None;skip=False
  if op==15:r[t]=(w&65535)<<16
  elif op==9:r[t]=(r[s]+imm)&0xffffffff
  elif op in (32,33,35,40,41,43):
   a=(r[s]+imm)&0xffffffff;n={32:1,33:2,35:4,40:1,41:2,43:4}[op]
   if op>=40:put(a,r[t],n);access.append(('write',a,n))
   else:r[t]=(sx(get(a,n),n*8) if op in (32,33) else get(a,n))&0xffffffff;access.append(('read',a,n))
  elif op in (1,4,5,20):
   if op==1:assert t==0;take=sx(r[s])<0
   else:take=(r[s]==r[t]) if op in (4,20) else (r[s]!=r[t])
   branches.add((off,take));dest=pc+4+imm*4 if take else pc+8
   if op==20 and not take:skip=True;dest=None
  elif op==3:r[31]=pc+8;dest=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
  elif op==0:
   fn=w&63
   if fn==0:r[d]=(r[t]<<(w>>6&31))&0xffffffff
   elif fn==3:r[d]=(sx(r[t])>>(w>>6&31))&0xffffffff
   elif fn==37:r[d]=r[s]|r[t]
   elif fn==33:r[d]=(r[s]+r[t])&0xffffffff
   elif fn==35:r[d]=(r[s]-r[t])&0xffffffff
   elif fn==42:r[d]=int(sx(r[s])<sx(r[t]))
   elif fn==8:dest=r[s]
   else:raise AssertionError((hex(pc),fn))
  else:raise AssertionError((hex(pc),op))
  r[0]=0
  if pending is not None:assert dest is None and not skip;pc,pending=pending,None
  else:pc,pending=pc+(8 if skip else 4),dest
 raise AssertionError('nontermination')

m=manifest(ROOT/'asm/us/ovl_a');manifest(ROOT/'asm/us/ovl_b');gm=manifest(ROOT/'asm/us/blob')
native=target(ROOT/'asm/us/ovl_a','func_80390BC0');assert len(native)==364
ext=json.loads((ROOT/'asm/us/ovl_a/extents.json').read_text());assert ext['image']=='A' and ext['image_sha256']=='0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
assert next(x for x in ext['functions'] if x['name']=='func_80390BC0')==dict(name='func_80390BC0',address='0x80390BC0',size=364,evidence=['jal','prologue'])
source_sha=sha(SOURCE.read_bytes());result=dict(source='cloud/work/frontier/dot_runtime_a_resource_cache_20261006/candidate.c',source_sha256=source_sha,target_sha256=sha(native),native_bytes=364,manifest=m,game_manifest=gm)
if '--native-only' not in sys.argv:
 flags=SOURCE.read_text().splitlines()[0][10:-3].split()
 shell(Path(os.environ['IDO_DIR'])/'cc',*flags,'-c','-o',OUT/'candidate.o',SOURCE)
 obj=elf(OUT/'candidate.o');symbols=json.loads((ROOT/'asm/us/ovl_a/symbols.json').read_text())['symbols'];bindings={}
 for x in obj['relocs']:
  n=x['symbol'];addr=symbols.get(n)
  if addr is None:assert re.fullmatch(r'D_[0-9A-F]{8}',n);addr='0x'+n[2:]
  bindings[n]=int(addr,16)
 (OUT/'whole.ld').write_text('OUTPUT_ARCH(mips)\nENTRY(func_80390BC0)\nSECTIONS { .text 0x80390BC0 : SUBALIGN(4) { *(.text) } }\n'+'\n'.join(f'{n} = 0x{v:X};' for n,v in bindings.items())+'\n')
 shell('mips-linux-gnu-ld','-EB','-T',OUT/'whole.ld','-o',OUT/'whole.elf',OUT/'candidate.o');linked=elf(OUT/'whole.elf'); frozen_text=linked['text']; mismatches=[i*4 for i,(a,b) in enumerate(zip(struct.unpack('>91I',native),struct.unpack('>91I',frozen_text[:364]))) if a!=b]
 assert mismatches==[0x120,0x124,0x12c,0x130,0x134],mismatches
 def validate(x):
  assert x['entry']==ENTRY and x['text_address']==ENTRY and x['symbols']['func_80390BC0']['value']==ENTRY and x['symbols']['func_80390BC0']['size']==364
  assert len(x['text'])==368 and x['text']==frozen_text and x['text'][364:]==b'\0'*4 and not x['relocs']
  assert all(x['symbols'][n]['value']==v and x['symbols'][n]['section']==0xfff1 for n,v in bindings.items())
  assert not any(s[5] for n,s in x['sections'].items() if s[2]&2 and n not in ('.text','.reginfo'))
 validate(linked);assert obj['symbols']['func_80390BC0']['value']==0 and obj['symbols']['func_80390BC0']['size']==364
 assert all(x['offset']<364 and x['type'] in (4,5,6) for x in obj['relocs'])
 assert not any(s[5] for n,s in obj['sections'].items() if s[2]&2 and n not in ('.text','.reginfo'))
 result.update(object_sha256=sha((OUT/'candidate.o').read_bytes()),elf_sha256=sha((OUT/'whole.elf').read_bytes()),all_relocations=obj['relocs'],bindings={n:hex(v) for n,v in bindings.items()},whole_linked_elf_audited=True,whole_linked_elf_match=False,strict_differing_native_word_offsets=[hex(x) for x in mismatches],zero_padding_bytes=4,owned_data_bytes=0)
 import copy
 negatives={}
 for name in ('entry','section','symbol','extent','padding','excess','binding','storage','relocation','text_drift'):
  x=copy.deepcopy(linked)
  if name=='entry':x['entry']+=4
  elif name=='section':x['text_address']+=4
  elif name=='symbol':x['symbols']['func_80390BC0']['value']+=4
  elif name=='extent':x['symbols']['func_80390BC0']['size']-=4
  elif name=='padding':x['text']=x['text'][:-1]+b'\1'
  elif name=='excess':x['text']+=b'\1\2\3\4'
  elif name=='binding':x['symbols']['D_803BA230']['value']+=4
  elif name=='storage':x['sections']['.unexpected']=(0,1,3,0,0,4,0,0,4,0)
  elif name=='relocation':x['relocs']=[{'type':6}]
  elif name=='text_drift':x['text']=b'\x01'+x['text'][1:]
  try:validate(x)
  except AssertionError:negatives[name]='rejected'
  else:raise AssertionError(('accepted corrupt ELF',name))
 result['negative_full_elf_views']=negatives
if not (HERE/'host.c').exists():raise FileNotFoundError('independent host harness missing')
if True:
 shell('gcc','-shared','-fPIC','-O2','-std=c99','-Wall','-Wextra','-Wno-unused-parameter','-DREVIEW_SOURCE="'+str(SOURCE)+'"',HERE/'host.c','-o',OUT/'host.so')
 lib=ctypes.CDLL(str(OUT/'host.so'));lib.run.argtypes=[ctypes.c_int,ctypes.c_int,ctypes.c_int,ctypes.c_int];lib.run.restype=ctypes.c_int
 class Rec(ctypes.Structure):_fields_=[('handle',ctypes.c_int),('loaded',ctypes.c_int8),('id',ctypes.c_int8),('reserve',ctypes.c_uint8*6)]
 assert ctypes.sizeof(Rec)==12 and Rec.loaded.offset==4 and Rec.id.offset==5
 records=(Rec*16).in_dll(lib,'D_803BA230');table=(ctypes.c_int16*32768).in_dll(lib,'D_80142B08');res=ctypes.c_uint32.in_dll(lib,'out');tr=(ctypes.c_uint32*128).in_dll(lib,'trace');trcount=ctypes.c_int.in_dll(lib,'trace_count')
 covered=set();branches=set();cases=0;rng=random.Random(9060)
 def run(recs,id,kind,handle,mut=0):
  global cases
  for i,(h,loaded,ident,reserve) in enumerate(recs):records[i].handle=sx(h);records[i].loaded=sx(loaded,8);records[i].id=sx(ident,8);records[i].reserve[:]=reserve
  if id>=0:table[id]=0x5a5a
  ret=lib.run(id,kind,sx(handle),mut)
  actual=(ret,([x.handle&0xffffffff for x in records],[x.loaded&255 for x in records],[x.id&255 for x in records],res.value,table[id]&65535 if id>=0 else 0x5a5a))
  expected,cov,dec,access=execute(native,recs,id,kind,handle,mut,cases)
  assert actual==expected[:2],(cases,id,kind,actual,expected[:2])
  if '--native-only' not in sys.argv:
   other,_,_,_=execute(linked['text'][:364],recs,id,kind,handle,mut,cases)
   assert other==expected,(cases,'compiled candidate diverges from native')
  # Flatten the independently obtained boundary snapshots to compare host trace.
  flat=[]
  for addr,args,snap in expected[2]:flat += [addr]+args+sum(snap[:3],[])+list(snap[3:])
  assert list(tr[:trcount.value])==flat,(cases,'trace')
  for i,rec in enumerate(records):assert list(rec.reserve)==recs[i][3]
  for op,a,n in access:
   assert (CACHE<=a and a+n<=CACHE+192) or (SP-48<=a and a+n<=SP+12) or (RESULT<=a and a+n<=RESULT+4) or a==TABLE+2*id,(op,hex(a),n)
  covered.update(cov);branches.update(dec);cases+=1
 def base():return [(rng.getrandbits(32),0,i,[rng.randrange(256) for _ in range(6)]) for i in range(16)]
 handles=[0,1,-1,32767,32768,-32768,0x7fffffff,0x80000000,0x12345678]
 kinds=[-32768,-89,-88,-1,0,1,32767]
 # All signed loaded bytes, record positions, negative sentinels, duplicates,
 # unmatched full arrays, preferred existing unloaded slots, helper mutations.
 for pos in range(16):
  for loaded in range(256):
   a=base();a[pos]=(a[pos][0],loaded,60,a[pos][3]);run(a,60,kinds[pos%7],handles[loaded%9])
  for sentinel in range(128,256):
   a=base();a[pos]=(a[pos][0],0,sentinel,a[pos][3]);run(a,60,kinds[pos%7],handles[sentinel%9])
 for i in range(1000):
  a=[(rng.getrandbits(32),rng.randrange(256),rng.randrange(256),[rng.randrange(256) for _ in range(6)]) for _ in range(16)]
  run(a,rng.randrange(128),rng.choice(kinds),rng.choice(handles),i%2)
 for id in [0,15,16,127,128,255,256,32767]:
  for kind in kinds:
   a=base();run(a,id,kind,-1);a[0]=(1,0,255,a[0][3]);run(a,id,kind,0x89abcdef,1)
 # Negative ids only with a loaded hit or full nonnegative table, neither of
 # which computes D_80142B08[id] or calls helpers.
 for id in [-32768,-129,-128,-1]:
  a=base();run(a,id,0,0)
  if id>=-128:a[15]=(0xdeadbeef,128,id&255,a[15][3]);run(a,id,0,0)
 a=base();a[0]=(11,0,255,a[0][3]);a[9]=(22,0,60,a[9][3]);run(a,60,1,33)
 a[15]=(44,255,60,a[15][3]);run(a,60,1,55)
 result['behavior']=dict(cases=cases,covered_words=len(covered),branch_outcomes=len(branches),host_source_vs_native_state_and_helper_trace=True,compiled_candidate_vs_native_state_and_helper_trace='--native-only' not in sys.argv,all_loaded_byte_patterns=True,all_negative_sentinel_bytes=True,helper_mutation=True,randomized_caller_saved_registers=True,stack_and_callee_saved_registers=True,untouched_reserved_bytes=True)
 assert len(covered)==91
assert sha(SOURCE.read_bytes())==source_sha,'source changed during review'
(OUT/'review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:v for k,v in result.items() if k not in ['manifest','game_manifest','all_relocations']},indent=2))
