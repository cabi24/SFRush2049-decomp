import hashlib,json,sys,tempfile
from pathlib import Path
root=Path(sys.argv[1] if len(sys.argv)>1 else '.').resolve();p=root/'cloud/work/boot_tail/BT03-runtime-singleton'
sys.path.insert(0,str(root));sys.path.insert(0,str(p))
from tools.cloud import score
from test_native import fixture,event,activate,put,get,A,B,EVENTS,RESOURCE,GLOBAL
from native_replay import execute
score.ASM_DIR=root/'asm/us/boot_tail'
with tempfile.TemporaryDirectory() as t:
 obj=Path(t)/'candidate.o';score.compile_single(p/'nonmatch/func_80018634.c',score.DEFAULT_FLAGS,obj)
 words,mask,unres,unver,errors=score.relocate(obj,score.text_words(obj),0,840,score.image_symbols());assert not(mask or unres or unver or errors)
 candidate=words[:210]
native=score.targets()['func_80018634'];rows=[]
def check(label,r,e,calls,helper):
 for words in (native,candidate):
  result=execute(words,r,helper)
  assert result['result']==1,label
  assert result['regions']=={a:bytes(d) for a,d in e.items()},label
  assert result['calls']==calls,(label,result['calls'],calls)
  assert not result['uninitialized_stack_reads'],label
 rows.append({'case':label,'result':'PASS','native_and_candidate':True})
# Program callback changes a previously skipped controller byte in the saved event.
r=fixture();activate(r,0,event(r,0,0,0,0,7,255));event(r,0,1,1000,0xFFFF)
e={a:bytearray(v) for a,v in r.items()};put(e[A],0x574,RESOURCE+0x10C);put(e[A],0x580,0x2000,2);put(e[A],0x12C,EVENTS+12);put(e[EVENTS],5,9,1)
def mutate(d,args,m,n):
 if d==0x80017720:m(EVENTS+5,1,9)
check('program callback changes live controller byte',r,e,[(0x80017720,(A,7,0)),(0x80017824,(9,0))],mutate)
# Controller callback alone replaces context and current cursor. Tick uses B.
r=fixture();activate(r,0,event(r,0,0,0,0,255,9));event(r,0,1,1000,0xFFFF)
event(r,0,2,1000,0xFFFF);event(r,0,3,1000,0xFFFF);activate(r,0,EVENTS+24,context=B)
put(r[B],0x118,2);put(r[B],0x130,0xFFFFFFFF);put(r[B],0x11C,3)
e={a:bytearray(v) for a,v in r.items()};put(e[A],0x574,RESOURCE+0x10C);put(e[A],0x580,0x2000,2);put(e[GLOBAL],0,B);put(e[B],0x12C,EVENTS+36);put(e[B],0x130,1);put(e[B],0x134,3)
def replace(d,args,m,n):m(GLOBAL,4,B)
check('controller alone replaces live context and cursor before ticking',r,e,[(0x80017824,(9,0))],replace)
# A jump index with a nonzero high byte cannot be confused with a byte or host endian.
r=fixture();r[EVENTS]=bytearray(4096);activate(r,0,event(r,0,0,0,0xFFFE,first=1,second=0));put(r[EVENTS],256*12,1000)
put(r[A],0x118,65536);put(r[A],0x11C,3)
e={a:bytearray(v) for a,v in r.items()};put(e[A],0x12C,EVENTS+256*12);put(e[A],0x134,14);put(e[A],0xFC6,1,2)
check('big-endian16 jump index256 and postrestart carry',r,e,[(0x80018B3C,(10,))],lambda *a:None)
# First-loop suppression persists when multiple due tracks restart and terminate.
r=fixture()
for i in [0,63]:activate(r,i,event(r,i,0,0,0xFFFE,first=0,second=1));event(r,i,1,10,0xFFFF)
put(r[A],0x118,100);put(r[A],0x11C,99)
e={a:bytearray(v) for a,v in r.items()}
for i in [0,63]:put(e[A],0x12C+i*16,0);put(e[A],0x134+i*16,10)
put(e[A],0xFC6,1,2)
check('two restarts terminate with no ticks and one callback',r,e,[(0x80018B3C,(10,))],lambda *a:None)
print(json.dumps({'result':'PASS','additional_cases':len(rows),'rows':rows},indent=2))
