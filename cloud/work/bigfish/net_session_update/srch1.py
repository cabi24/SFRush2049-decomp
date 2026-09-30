import sys,random,re; sys.path.insert(0,'.')
import ev
from multiprocessing import Pool
base=open('base.c').read()
i0=base.index('    s32 n, i, k, j, r, cnt;'); i1=base.index('    p = &input_rec0')
decl_re=(i0,i1)
DECL={'n':'s32 n;','i':'s32 i;','k':'s32 k;','j':'s32 j;','r':'s32 r;','cnt':'s32 cnt;','cap':'s32 cap;','p':'InRec *p;','v':'Sess *v;'}
def mk(order):
    top=order[:6]; mid=order[6:]
    d=''.join('    '+DECL[x]+'\n' for x in top)+'    s32 idx[6];\n'+''.join('    '+DECL[x]+'\n' for x in mid)+'    Info *info;\n    u8 pad[120];\n'
    return base[:i0]+d+base[i1:]
def f(order):
    r=ev.evaluate(mk(order))
    return order,r
if __name__=='__main__':
    random.seed(int(sys.argv[1]) if len(sys.argv)>1 else 1)
    names=list(DECL)
    orders=[]
    for _ in range(int(sys.argv[2]) if len(sys.argv)>2 else 60):
        o=names[:]; random.shuffle(o); orders.append(o)
    with Pool(4) as p:
        res=p.map(f,orders)
    res=[(r,o) for o,r in res if r]
    res.sort(key=lambda x:(-x[0]['exact']))
    for r,o in res[:8]: print(r,o)
    print('...'); 
    for r,o in res[-3:]: print(r,o)
