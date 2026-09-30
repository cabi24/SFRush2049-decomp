import sys,subprocess
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score
from pathlib import Path
src=sys.argv[1]; fl=sys.argv[2] if len(sys.argv)>2 else '-O2'
o='/tmp/claude-0/my.o'
score.compile_single(src,f'-g0 {fl} -mips2 -G 0 -non_shared',Path(o))
print(subprocess.run(['mips-linux-gnu-objdump','-dr','-M','reg-names=32',o],capture_output=True,text=True).stdout)
