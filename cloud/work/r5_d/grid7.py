import sys,itertools,os,tempfile,re
sys.path.insert(0,'.')
import harness
from multiprocessing import Pool
SP='/tmp/claude-0/-home-user-SFRush2049-decomp/37f33778-7a6e-51d1-85db-44651954a6c5/scratchpad'
BASE=open('v14.c').read()
def o1(t): # swap compare operands
    return re.sub(r'if \((D_[^<\n]+?) < (D_[^\n]+?)\) \{\n(\s+)D_80143F54\[j \+ 1\] = a;',lambda m:'if (%s > %s) {\n%sD_80143F54[j] = b;\n%sD_80143F54[j + 1] = a;\n%s//'%(m.group(2),m.group(1),m.group(3),m.group(3),m.group(3)),t).replace('//'+'          D_80143F54[j] = b;\n','').replace('\n          //          D_80143F54[j] = b;','')
def o2(t): # swap loads a/b order
    return t.replace("        a = D_80143F54[j];\n        b = D_80143F54[j + 1];\n","        b = D_80143F54[j + 1];\n        a = D_80143F54[j];\n")
def o3(t): # swap ia/ib
    return t.replace("        ia = D_8014A250[a].idx;\n        ib = D_8014A250[b].idx;\n","        ib = D_8014A250[b].idx;\n        ia = D_8014A250[a].idx;\n")
def o4(t): # swap store order
    return t.replace("          D_80143F54[j + 1] = a;\n          D_80143F54[j] = b;\n","          D_80143F54[j] = b;\n          D_80143F54[j + 1] = a;\n")
def o5(t): # tie statement order
    return t.replace("        D_80150B60++;\n        D_80150B68[i] = 1;\n","        D_80150B68[i] = 1;\n        D_80150B60++;\n")
def o6(t): # outer loop var separate decl 
    return t.replace("  s32 j;\n","  s32 j;\n  s32 k;\n").replace("for (i = 0; i < D_8014A108 - 1; i++) {\n      for (j","for (k = 0; k < D_8014A108 - 1; k++) {\n      for (j")
def o7(t): # count as -- style: D_80150B60 += 1
    return t.replace("D_80150B60++;","D_80150B60 += 1;")
def o8(t): # n copy: use local s16 n in tie loops
    return t.replace("for (i = 1; i < D_8014A108; i++)","n = D_8014A108;\n    for (i = 1; i < n; i++)")
def o9(t): # 1+j
    return t.replace("j + 1","1 + j")
OPS=[o2,o3,o4,o5,o6,o7,o8,o9]
def work(mask):
    t=BASE
    for b,op in zip(mask,OPS):
        if b: t=op(t)
    fd,path=tempfile.mkstemp(suffix='.c',dir=SP); os.write(fd,t.encode()); os.close(fd)
    r=harness.evaluate(path,'func_800F7F3C'); os.unlink(path); return r,mask
if __name__=='__main__':
    jobs=list(itertools.product([0,1],repeat=len(OPS)))
    with Pool(4) as P: res=P.map(work,jobs)
    res=[x for x in res if x[0]]; res.sort(key=lambda x:-x[0][1])
    for r in res[:8]: print(r)
