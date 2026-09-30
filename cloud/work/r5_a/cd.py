import sys; sys.path.insert(0,'.'); import run
keys=[f'{r}_{k}' for k in range(5) for r in 'abc']+['d_4']
o={k:0 for k in keys}; best=run.score(o)
improved=True
while improved:
    improved=False
    for k in keys:
        o2=dict(o); o2[k]^=1; s=run.score(o2)
        if s<best: best=s;o=o2;improved=True;print(best,o,flush=True)
print('final',best,o)
