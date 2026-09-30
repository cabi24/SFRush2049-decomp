import sys, subprocess, tempfile
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud')
import score
src=sys.argv[1]; flags=sys.argv[2] if len(sys.argv)>2 else '-g0 -O2 -mips2 -G 0 -non_shared'
with tempfile.TemporaryDirectory() as t:
    o=Path(t)/'o.o'; score.compile_single(src,flags,o)
    print(subprocess.run(['mips-linux-gnu-objdump','-dr','-mips3',str(o)],capture_output=True,text=True).stdout)
