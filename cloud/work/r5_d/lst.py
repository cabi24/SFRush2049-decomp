import sys,subprocess,tempfile
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score
src,flags=sys.argv[1],(sys.argv[2] if len(sys.argv)>2 else '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul')
o=Path('/tmp/claude-0/-home-user-SFRush2049-decomp/37f33778-7a6e-51d1-85db-44651954a6c5/scratchpad/d.o')
score.compile_single(src,flags,o)
print(subprocess.run(['mips-linux-gnu-objdump','-d','-M','no-aliases,reg-names=32',str(o)],capture_output=True,text=True).stdout)
