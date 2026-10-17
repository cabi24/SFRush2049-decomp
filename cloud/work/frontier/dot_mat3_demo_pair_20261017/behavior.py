"""Independent bounded demo model versus both native/candidate instruction bodies.

Only the pair executes. Other gameplay calls are deterministic event/argument
stubs. ReleaseEnviron alone advances the fixture's small linked list. This does
not validate any other gameplay, audio, rendering, timing or heap side effects.
"""
import random,struct
ROOT,HELPER,STACK,RETURN=0x800FD024,0x800FCDF8,0x81010000,0x81234560
MASK=0xffffffff
A={'inited':0x8011414C,'blit':0x80114150,'ready':0x80114154,'track':0x80114158,'demo':0x8011415C,'windows':0x80151AD0,'state':0x801174B4,'next':0x801174B8,'extra':0x801174BC,'race':0x8014A110,'cars':0x80152744,'models':0x8014A250,'env':0x801391F0,'players':0x8014A118,'tracknum':0x8014978C,'wrapped':0x80138660,'allow5':0x80156994,'reset':0x80157244,'edges':0x8015694C}
WIDTH={k:4 for k in A};WIDTH.update({k:1 for k in ('inited','ready','demo','cars','tracknum','allow5','reset')});WIDTH.update(track=2,windows=2)
CALL={'newblit':0x800B37E8,'timer':0x800014F0,'controls':0x800B6138,'stopblit':0x800B358C,'timeout':0x800015BC,'fakeinit':0x800EC190,'preplay':0x800FB2C8,'carsetup':0x800F8D9C,'drive':0x800EC0DC,'cptime':0x800FBE60,'play':0x800FA9B4,'enddrive':0x800D5828,'modelstop':0x800D5798,'listeners':0x800D5374,'hud':0x800D58CC,'release':0x8009079C,'skids':0x800B0580,'removecar':0x800B1F30,'unload':0x800B912C}
BLIT=0x82110000

def require(ok,msg):
 if not ok:raise RuntimeError(msg)
def signed(v,n=32):
 v&=(1<<n)-1;return v-(1<<n) if v>>(n-1) else v
def read(m,a,n):
 require(a%n==0 and all(a+i in m for i in range(n)),f'invalid read {a:x}/{n}');return int.from_bytes(bytes(m[a+i] for i in range(n)),'big')
def write(m,a,n,v):
 require(a%n==0 and all(a+i in m for i in range(n)),f'invalid write {a:x}/{n}')
 for i,b in enumerate((v&((1<<(8*n))-1)).to_bytes(n,'big')):m[a+i]=b

def fixtures():
 rng=random.Random(ROOT)
 for case in range(768):
  c={k:rng.randrange(2) for k in ('inited','ready','demo','allow5','reset')}
  c.update(track=(-1,0,3,4,5)[case%5],windows=3,state=(0,8,0x2000000,0x2000008)[case%4],next=99,extra=0x123,race=3,cars=(-1,0,1,2,3,4)[case%6],tracknum=case%6,wrapped=7,edges=case%8,blit=BLIT if case%3 else 0)
  c.update(preplay=case%2,deadlines=(bool(case&2),bool(case&4)),envcount=case%4,slots=[rng.randrange(4) for _ in range(4)],players=[rng.randrange(256) for _ in range(4)])
  yield c

def initial(c):
 regions=[(a,WIDTH[k]) for k,a in A.items() if k not in ('models','players','env')]+[(A['env'],4),(A['models'],4*2056),(A['players'],4*76),(STACK-1024,2048)]
 regions +=[(0x82200000+i*32,24) for i in range(3)]
 m={a+i:0xa5 for a,n in regions for i in range(n)}
 for k in A:
  if k in c and k not in ('players','models','env'):write(m,A[k],WIDTH[k],c[k])
 for i in range(4):write(m,A['models']+2056*i+1990,2,c['slots'][i]);write(m,A['players']+76*i,1,c['players'][i])
 write(m,A['env'],4,0x82200000 if c['envcount'] else 0)
 for i in range(3):write(m,0x82200000+32*i,4,0x82200000+32*(i+1) if i+1<c['envcount'] else 0)
 return m

def visible(m):return {a:v for a,v in m.items() if not STACK-1024<=a<STACK+1024}

def model(c):
 s=dict(c);e=[];d=0
 def event(n,*a):e.append((n,*a))
 def timeout():
  nonlocal d
  result=c['deadlines'][min(d,1)];d+=1;event('timeout');return result
 def play(show):
  if show:
   if not s['demo']:
    s['demo']=1;s['windows']=1
    if not s['state']&0x2000000:s['race']=0;event('fakeinit',s['track'])
   if not s['ready']:
    event('preplay',s['state']&8,0)
    if c['preplay']:event('carsetup');event('drive');event('cptime');s['ready']=1;return
   if s['ready']:event('play')
  else:
   s['demo']=0;s['ready']=0
   for i in range(max(s['cars'],0)):event('enddrive',c['slots'][i])
   event('modelstop');event('listeners');event('hud')
   for i in range(c['envcount']):event('release',0x82200000+32*i,1)
   s['envcount']=0;event('skids')
   for i in range(max(s['cars'],0)):event('removecar',i)
   s['players']=list(range(4))
   if not s['state']&0x2000000:event('unload',s['tracknum'])
 def end():
  if s['blit']:event('stopblit',s['blit']);s['blit']=0
  play(False);s['inited']=0
 if not s['inited']:
  event('newblit',0,0,0x80114128,1);s['blit']=BLIT;event('timer',0x41f00000);s['track']+=1;s['wrapped']=0
  if s['track']>=6:s['wrapped']=1;s['track']=0
  if not s['allow5'] and s['track']>=5:s['wrapped']=1;s['track']=0
  s['inited']=1;event('controls')
 play(True)
 if s['reset'] and s['ready']:end();s['next']=4
 elif (s['edges']&7 and s['ready']) or timeout():
  extra=bool(s['edges']&1);end()
  if s['track']==5 and timeout():s['next']=2
  else:
   s['next']=4
   if extra:s['extra']=16
 m=initial(c)
 for k in A:
  if k in s and k not in ('players','models','env'):write(m,A[k],WIDTH[k],s[k])
 write(m,A['env'],4,0x82200000 if s['envcount'] else 0)
 for i,v in enumerate(s['players']):write(m,A['players']+76*i,1,v)
 return visible(m),e

def execute(pair,c):
 code={a+4*i:w for a,words in pair.items() for i,w in enumerate(words)};m=initial(c)
 r=[0xa5000000+13*i for i in range(32)];f=[0xa6000000+17*i for i in range(32)];r[0]=0;r[29]=STACK;r[31]=RETURN;before=r[:];fbefore=f[:]
 pc=ROOT;pending=None;lo=0;events=[];coverage=set();branches=set();deadlines=0
 byaddr={a:n for n,a in CALL.items()}
 for step in range(100000):
  if pc==RETURN:break
  require(pc in code,f'escaped instruction {pc:x}');coverage.add(pc);w=code[pc];op=w>>26;rs=w>>21&31;rt=w>>16&31;rd=w>>11&31;sh=w>>6&31;fn=w&63;imm=signed(w,16);a=(r[rs]+imm)&MASK;old=pending;pending=None;nxt=pc+4
  if op==0:
   if fn==0:r[rd]=r[rt]<<sh
   elif fn==2:r[rd]=r[rt]>>sh
   elif fn==3:r[rd]=signed(r[rt])>>sh
   elif fn==8:pending=r[rs]
   elif fn==18:r[rd]=lo
   elif fn==25:lo=r[rs]*r[rt]&MASK
   elif fn==33:r[rd]=r[rs]+r[rt]
   elif fn==35:r[rd]=r[rs]-r[rt]
   elif fn==36:r[rd]=r[rs]&r[rt]
   elif fn==37:r[rd]=r[rs]|r[rt]
   elif fn==42:r[rd]=int(signed(r[rs])<signed(r[rt]))
   elif fn==43:r[rd]=int(r[rs]<r[rt])
   else:raise RuntimeError(f'unexpected SPECIAL {fn}')
  elif op==3:
   dest=((pc+4)&0xf0000000)|((w&0x3ffffff)<<2);r[31]=pc+8;pending=dest if dest in pair else ('call',dest,pc+8)
  elif op==9:r[rt]=a
  elif op==10:r[rt]=int(signed(r[rs])<imm)
  elif op==11:r[rt]=int(r[rs]<(imm&MASK))
  elif op==12:r[rt]=r[rs]&(w&65535)
  elif op==13:r[rt]=r[rs]|(w&65535)
  elif op==15:r[rt]=(w&65535)<<16
  elif op==17 and rs==4:f[rd]=r[rt]
  elif op==1:
   take=signed(r[rs])<0 if rt in (0,2) else signed(r[rs])>=0;require(rt in (0,1,2,3),'REGIMM');branches.add((pc,take))
   if take:pending=pc+4+imm*4
   elif rt>=2:nxt+=4
  elif op in (4,5,6,7,20,21,22,23):
   take=r[rs]==r[rt] if op in (4,20) else (r[rs]!=r[rt] if op in (5,21) else (signed(r[rs])<=0 if op in (6,22) else signed(r[rs])>0));branches.add((pc,take))
   if take:pending=pc+4+imm*4
   elif op>=20:nxt+=4
  elif op in (32,33,35,36,37):
   n=4 if op==35 else (2 if op in (33,37) else 1);v=read(m,a,n);r[rt]=signed(v,8*n) if op in (32,33) else v
  elif op in (40,41,43):write(m,a,4 if op==43 else (2 if op==41 else 1),r[rt])
  else:raise RuntimeError(f'unexpected opcode {op}')
  r=[x&MASK for x in r];r[0]=0;require(old is None or pending is None,'nested delay slot')
  if isinstance(old,tuple):
   dest=old[1];require(dest in byaddr,f'unknown callee {dest:x}');name=byaddr[dest];result=0xbadfeed
   if name=='newblit':events.append((name,*r[4:8]));result=BLIT
   elif name=='timer':events.append((name,f[12]))
   elif name=='timeout':events.append((name,));result=int(c['deadlines'][min(deadlines,1)]);deadlines+=1
   elif name=='preplay':events.append((name,r[4],r[5]));result=c['preplay']
   elif name in ('fakeinit','enddrive','removecar','unload'):events.append((name,signed(r[4])))
   elif name=='stopblit':events.append((name,r[4]))
   elif name=='release':
    events.append((name,r[4],r[5]));require(r[4]==read(m,A['env'],4) and r[5]==1,'release stub arguments');write(m,A['env'],4,read(m,r[4],4))
   else:events.append((name,))
   for j in list(range(1,16))+[24,25]:r[j]=0xbad00000+31*j
   for j in range(20):f[j]=0xbad30000+j
   r[2]=result;pc=old[2]
  else:pc=old if old is not None else nxt
 else:raise RuntimeError('execution bound')
 require(r[16:24]==before[16:24] and r[28:]==before[28:] and f[20:]==fbefore[20:],'saved register/stack ABI')
 return visible(m),events,coverage,branches

def verify(native,compiled):
 coverage=[set(),set()];branches=[set(),set()];count=0
 for c in fixtures():
  expected=model(c)
  for i,pair in enumerate((native,compiled)):
   m,e,cov,br=execute(pair,c)
   require((m,e)==expected,f'fixture {count} model mismatch: events {e!r} expected {expected[1]!r}; memory changes {[(hex(a),v,expected[0][a]) for a,v in m.items() if v!=expected[0][a]][:8]!r}')
   coverage[i]|=cov;branches[i]|=br
  count+=1
 for pair,cov in zip((native,compiled),coverage):
  for a,words in pair.items():require({pc-a for pc in cov if a<=pc<a+len(words)*4}==reachable(words),'reachable pair instruction coverage')
 return {'fixtures':count,'executions':count*2,'instruction_coverage':[{hex(a):sum(a<=pc<a+4*len(words) for pc in cov) for a,words in pair.items()} for cov,pair in zip(coverage,(native,compiled))],'uncovered_offsets':[{hex(a):[4*i for i in range(len(words)) if a+4*i not in cov] for a,words in pair.items()} for cov,pair in zip(coverage,(native,compiled))],'branch_outcomes':[len(x) for x in branches],'callee_implementation_side_effects_validated':False,'both_pair_bodies_execute':True}

def reachable(words):
 """Conservative intraprocedural CFG, including branch-likely annulment."""
 pending=[0];seen=set();result=set()
 while pending:
  off=pending.pop()
  if off in seen:continue
  require(0<=off<4*len(words) and off%4==0,'CFG escape');seen.add(off);result.add(off);w=words[off//4];op=w>>26;rs=w>>21&31;rt=w>>16&31;fn=w&63
  if op==0 and fn==8:require(rs==31,'unexpected indirect jump');result.add(off+4)
  elif op==3:result.add(off+4);pending.append(off+8)
  elif op in (1,4,5,6,7,20,21,22,23):
   result.add(off+4);target=off+4+signed(w,16)*4
   if op in (4,20) and rs==rt:pending.append(target)
   elif op in (5,21) and rs==rt:pending.append(off+8)
   else:pending.extend([target,off+8])
  else:pending.append(off+4)
 return result
