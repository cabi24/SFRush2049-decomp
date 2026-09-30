import sys,re
sys.path.insert(0,'/home/user/SFRush2049-decomp')
from pathlib import Path
from tools.conveyor.pipeline import autodecomp as ad
S=Path(sys.argv[1]); ctx=ad._context(include_protos=False)
for fn in sys.argv[2:]:
    body=ad._clean_m2c((S/f'{fn}.m2c.c').read_text().strip())
    own=re.compile(rf"^[^\n]*\b{fn}\s*\([^;{{]*\)\s*;\s*$",re.M)
    (S/f'{fn}.c').write_text(ad._PRELUDE+own.sub("",ctx[1])+"\n"+ad._macros()+"\n"+body+"\n")
