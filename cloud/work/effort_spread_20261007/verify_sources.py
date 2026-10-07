"""Syntax and bounded source-interpreter validation; never invokes a C compiler."""
from pathlib import Path
import re,json,itertools,random,hashlib,time,datetime
from pycparser import c_parser,c_ast as C
R=Path(__file__).parent

def parse(s):
 s=re.sub(r'/\*.*?\*/','',s,flags=re.S)
 lines=s.splitlines();out=[];skip=False
 for l in lines:
  if l.startswith('#') or skip:
   skip=l.endswith('\\');continue
  out.append(l)
 return c_parser.CParser().parse('\n'.join(out)).ext[-1].body

def trace(body,args,state):
 v=dict(zip(['x','y','right','bottom','s','t'],args));v.update(state);v['D_80149438']=0;packets=[]
 def ev(n):
  if isinstance(n,C.ID):return v[n.name]
  if isinstance(n,C.Constant):return int(n.value,0)
  if isinstance(n,C.BinaryOp):
   a=ev(n.left)
   if n.op=='&&':return bool(a) and bool(ev(n.right))
   if n.op=='||':return bool(a) or bool(ev(n.right))
   b=ev(n.right)
   return {'+':lambda:a+b,'-':lambda:a-b,'<<':lambda:a<<b,'&':lambda:a&b,'<':lambda:a<b,'>':lambda:a>b,'==':lambda:a==b}[n.op]()
  if isinstance(n,C.UnaryOp):
   if n.op=='&':return n.expr.name
   if n.op=='*':return v[ev(n.expr)]
   if n.op=='!':return not ev(n.expr)
   if n.op=='-':return -ev(n.expr)
   raise AssertionError(n.op)
  raise AssertionError(type(n))
 def ex(n):
  if n is None:return None
  if isinstance(n,C.Compound):
   children=n.block_items or []; index=0
   while index<len(children):
    control=ex(children[index])
    if isinstance(control,tuple) and control[0]=='goto':
     labels=[j for j,child in enumerate(children) if isinstance(child,C.Label) and child.name==control[1]]
     if labels:
      assert len(labels)==1 and labels[0]>index, 'only forward within-block goto supported'
      index=labels[0];continue
    if control:return control
    index+=1
  elif isinstance(n,C.Decl):
   if n.init is not None:v[n.name]=ev(n.init)
  elif isinstance(n,C.If):return ex(n.iftrue if ev(n.cond) else n.iffalse)
  elif isinstance(n,C.Assignment):
   a=ev(n.rvalue);name=n.lvalue.name
   if n.op=='+=':a+=v[name]
   else:assert n.op=='='
   assert -2**31<=a<2**31
   v[name]=a
  elif isinstance(n,C.Return):return 'return'
  elif isinstance(n,C.DoWhile):
   control=ex(n.stmt);assert ev(n.cond)==0
   return None if control=='break' else control
  elif isinstance(n,C.Goto):return ('goto',n.name)
  elif isinstance(n,C.Label):return ex(n.stmt)
  elif isinstance(n,C.Break):return 'break'
  elif isinstance(n,C.Switch):
   key=ev(n.cond)
   selected=next((c for c in n.stmt.block_items if isinstance(c,C.Case) and ev(c.expr)==key),None)
   if selected is None:selected=next((c for c in n.stmt.block_items if isinstance(c,C.Default)),None)
   for case in n.stmt.block_items:
    assert isinstance(case,(C.Case,C.Default))
    if case is selected:
     for child in case.stmts:
      control=ex(child)
      if control:return None if control=='break' else control
     break
  elif isinstance(n,C.FuncCall):
   assert n.name.name=='gSPTextureRectangle'
   a=n.args.exprs;ptr=a[0];assert isinstance(ptr,C.UnaryOp) and ptr.op=='p++'
   if isinstance(ptr.expr,C.ID):assert ptr.expr.name=='D_80149438'
   else:assert isinstance(ptr.expr,C.UnaryOp) and ptr.expr.op=='*' and ev(ptr.expr.expr)=='D_80149438'
   packets.append(tuple(ev(x) for x in a[1:]));v['D_80149438']+=3
  else:raise AssertionError(type(n))
 ex(body);return packets,v['D_80149438']
def cases():
 bounds={'D_8012E60C':2,'D_8012E668':3,'D_8012E610':21,'D_8012E674':19}
 rects=[(4,5,10,12),(2,3,2,3),(0,0,22,20),(4,5,3,12),(4,5,10,4),(0,4,8,9),(4,0,8,9),(4,5,25,12),(4,5,10,25)]
 for low,scale,mode,rect in itertools.product(range(16),(0,0x8000),(0,1,-1),rects):yield (*rect,40,48),{**bounds,'D_8012E608':low|scale|0x10000,'D_8014A248':mode}
 rng=random.Random(87110)
 for _ in range(256):yield (*(rng.randint(0,32) for _ in range(4)),rng.randint(40,80),rng.randint(40,80)),{**bounds,'D_8012E608':rng.getrandbits(20),'D_8014A248':rng.choice((0,1,-1))}


def verify(root):
 base=(root/'baseline.c').read_text();plan=json.loads((root/'batch.json').read_text());cases_list=list(cases());reference=parse(base);want=[trace(reference,a,s) for a,s in cases_list];rows=[]
 for key,prediction in plan['predictions'].items():
  source=(root/prediction['source']).read_text()
  assert hashlib.sha256(source.encode()).hexdigest()==prediction['sha256']
  assert source.split('void func_80087110(')[0]==base.split('void func_80087110(')[0]
  body=parse(source)
  for (args,state),expected in zip(cases_list,want):assert trace(body,args,state)==expected,key
  rows.append({'id':key,'status':'bounded_pass','cases':len(cases_list)})
 return rows

if __name__=='__main__':
 import sys
 roots=[Path(x) for x in sys.argv[1:]] or [R/name for name in ['low','medium','high','xhigh']]
 result={root.name:verify(root) for root in roots}
 # Negative controls establish that genuine changes are detectable.
 source=(roots[0]/'baseline.c').read_text();reference=parse(source)
 for before,after in [('step=512','step=513'),('offset=16','offset=15'),('t+=height','t+=height+1')]:
  mutant=parse(source.replace(before,after,1))
  assert any(trace(mutant,a,s)!=trace(reference,a,s) for a,s in cases())
 print(json.dumps({'status':'passed','lanes':result,'detected_mutants':3,'scope':'Bounded actual-source control skeleton and SDK argument tuples; not whole-C or native proof.'}))
