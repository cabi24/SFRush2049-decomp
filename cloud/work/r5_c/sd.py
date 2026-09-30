import sys,tempfile,subprocess
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
with tempfile.TemporaryDirectory() as t:
    obj=Path(t)/'o.o'; score.compile_single(sys.argv[1],'-g0 -O2 -mips2 -G 0 -non_shared',obj)
    print(subprocess.run(['mips-linux-gnu-objdump','-d','-M','reg-names=o32',str(obj)],capture_output=True,text=True).stdout)
