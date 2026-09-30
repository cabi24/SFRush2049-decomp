import sys,tempfile,subprocess
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
src,fn=sys.argv[1:3]; flags=sys.argv[3] if len(sys.argv)>3 else '-g0 -O2 -mips2 -G 0 -non_shared'
with tempfile.TemporaryDirectory() as t:
    o=Path(t)/'o.o'; score.compile_single(src,flags+' -r4300_mul' if False else flags,o)
    print(subprocess.run(['mips-linux-gnu-objdump','-d','-r','-M','gpr-names=32',str(o)],capture_output=True,text=True).stdout)
