"""Bounded controller-initialization model; queue and poll boundaries are stubs."""
from pathlib import Path
import sys,struct,itertools,json,ctypes,subprocess,tempfile
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
sys.path.insert(0,str(ROOT/'tools/cloud'));import score
ENTRY,MODE,STATE,CREATE,SEND,POLL=0x800E7038,0x800C84C0,0x800C9158,0x80006A00,0x800075E0,0x800C95DC
FLAG,READY,QUERY,CAL,MODES,STATES,QUEUE,MESSAGES=0x80111954,0x80111968,0x80111950,0x80149AF8,0x80149B74,0x80149B64,0x801497A8,0x80152730
STACK,RETURN,MASK=0x81010000,0x81234560,0xffffffff
NAMES=[('func_800E7038',ENTRY),('player_mode_set',MODE),('player_state_set',STATE)]
def require(c,m):
 if not c:raise ValueError(m)
def signed(x,n=32):
 x&=(1<<n)-1;return x-(1<<n) if x>>(n-1) else x
def load(m,a,n):
 require(a%n==0 and all(a+i in m for i in range(n)),f'invalid read {a:x}/{n}')
 return int.from_bytes(bytes(m[a+i] for i in range(n)),'big')
def store(m,a,n,v):
 require(a%n==0 and all(a+i in m for i in range(n)),f'invalid write {a:x}/{n}')
 for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):m[a+i]=b

def cases():
 for flag,ready,script,cal in itertools.product((0,1,127,255),(0,1,255),((0,),(1,),(255,),(128,0),(128,128,1),(128,128,128,255)),(0,70,127)):
  yield dict(flag=flag,ready=ready,script=script,cal=cal)
def state(c):
 m={a+i:0xa5 for a,n in [(FLAG,1),(READY,1),(QUERY,4),(CAL,8),(MODES,4),(STATES,4),(STACK-128,256)] for i in range(n)}
 store(m,FLAG,1,c['flag']);store(m,READY,1,c['ready'])
 for i in range(8):store(m,CAL+i,1,(c['cal']+i)&255)
 for i in range(4):store(m,MODES+i,1,33+i);store(m,STATES+i,1,79+i)
 return m

def visible(m):return {a:v for a,v in m.items() if not STACK-128<=a<STACK+128 and not QUERY<=a<QUERY+4}
def model(c):
 m=state(c);events=[]
 if not c['flag']:
  for i in range(4):store(m,MODES+i,1,1);store(m,STATES+i,1,1)
 store(m,FLAG,1,1)
 for i in range(8):store(m,CAL+i,1,70)
 if not c['ready']:store(m,READY,1,1);events+=['create','send']
 events+=['poll'];return visible(m),events,len(c['script'])

def candidate(obj):
 obj=Path(obj);data,secs=score._elf(obj);ti=score._text_index(secs);words=score.text_words(obj);symbols=[s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i)];code={}
 for name,address in NAMES:
  found=[s for s in symbols if s['type']==2 and s['section']==ti and s['name']==name];require(len(found)==1,'ambiguous function');s=found[0];start=s['value'];end=start+s['size'];resolved,masks,unresolved,unverified,errors=score.relocate(obj,words,start,end,score.image_symbols());require(not(masks or unresolved or unverified or errors),'uncertain relocation')
  code.update({address+4*i:w for i,w in enumerate(resolved[start//4:end//4])})
 return code

def execute(code,c):
 m=state(c);r=[(0xa5000000+i*13)&MASK for i in range(32)];r[0]=0;r[29]=STACK;r[31]=RETURN;before=r[:];pc=ENTRY;pending=None;events=[];cov=set();reads=0
 for step in range(3000):
  if pc==RETURN:break
  require(pc in code,f'escaped {pc:x}');cov.add(pc);w=code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=signed(w,16);a=(r[rs]+imm)&MASK;old=pending;pending=None;nxt=pc+4
  if op==0:
   if fn==0:r[rd]=r[rt]<<sh
   elif fn==8:pending=r[rs]
   elif fn==33:r[rd]=r[rs]+r[rt]
   elif fn==37:r[rd]=r[rs]|r[rt]
   else:raise ValueError(f'unsupported SPECIAL {fn}')
  elif op==3:
   dest=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);r[31]=pc+8;pending=dest if dest in code else ('call',dest,pc+8)
  elif op==9:r[rt]=a
  elif op==10:r[rt]=int(signed(r[rs])<imm)
  elif op==15:r[rt]=(w&65535)<<16
  elif op in (4,5,20,21):
   take=(r[rs]==r[rt]) if op in (4,20) else (r[rs]!=r[rt])
   if take:pending=pc+4+imm*4
   elif op>=20:nxt+=4
  elif op in (32,35,36):
   n=4 if op==35 else 1
   if a==QUERY:
    require(n==1,'query status is not byte-wide');require(reads<len(c['script']),'extra query read');v=c['script'][reads];reads+=1
   else:v=load(m,a,n)
   r[rt]=signed(v,8) if op==32 else v
  elif op in (40,43):store(m,a,1 if op==40 else 4,r[rt])
  else:raise ValueError(f'unsupported opcode {op}')
  r=[v&MASK for v in r];r[0]=0;require(old is None or pending is None,'branch in delay slot')
  if isinstance(old,tuple):
   dest=old[1]
   if dest==CREATE:require(r[4:7]==[QUEUE,MESSAGES,1],'queue creation protocol');events.append('create')
   elif dest==SEND:require(r[4:7]==[QUEUE,0,0],'queue seed protocol');events.append('send')
   elif dest==POLL:require(reads==len(c['script']),'poll before query complete');events.append('poll')
   else:raise ValueError('unexpected boundary')
   for k in list(range(1,16))+[24,25]:r[k]=(0xbad00000+k*31)&MASK
   pc=old[2]
  else:pc=old if old is not None else nxt
 else:raise ValueError('execution bound')
 require(r[16:24]==before[16:24] and r[28:]==before[28:],'saved register ABI');return visible(m),events,reads,cov

def verify(obj):
 native={address+4*i:w for name,address in NAMES for i,w in enumerate(score.targets()[name])};compiled=candidate(obj);coverage=[set(),set()];count=0
 for c in cases():
  expected=model(c)
  for j,code in enumerate((native,compiled)):
   actual=execute(code,c);require(actual[:3]==expected,f'case {count}, program {j}');coverage[j]|=actual[3]
  count+=1
 return dict(status='PASS',fixtures=count,executions=2*count,native_words_executed=len(coverage[0]),candidate_words_executed=len(coverage[1]),native_total_words=len(native),native_init_words_executed=sum(ENTRY<=pc<ENTRY+252 for pc in coverage[0]),boundary_stubs=['osCreateMesgQueue','osSendMesg','controller_poll'],real_native_helpers=['player_state_set','player_mode_set'],claims=[])
if __name__=='__main__':print(json.dumps(verify(sys.argv[1]),indent=2))
