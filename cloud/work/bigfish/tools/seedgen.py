import sys
sys.path.insert(0,'/home/user/SFRush2049-decomp')
from pathlib import Path
from tools.conveyor.pipeline import autodecomp as ad
S=Path(sys.argv[1])
print(ad.M2C)
for n in sys.argv[2:]:
    diag={}
    s=ad.m2c_seed(n,0,{n:S/'seed'/f'{n}.s'},diagnostics=diag)
    print(n, None if s is None else len(s.splitlines()), str(diag.get(n))[:300])
    if s: (S/'seed'/f'{n}.full.c').write_text(s)
