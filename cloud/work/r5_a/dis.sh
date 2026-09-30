#!/bin/bash
# dis.sh file.c flags...
f=$1; shift
cd /home/user/SFRush2049-decomp/cloud/work/r5_a
python3 - "$f" "$@" <<'PY'
import sys,subprocess,tempfile
from pathlib import Path
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
f=sys.argv[1]; fl=sys.argv[2] if len(sys.argv)>2 else '-g0 -O2 -mips2 -G 0 -non_shared'
o=Path('/tmp/r5a.o'); score.compile_single(f,fl,o)
print(subprocess.run(['mips-linux-gnu-objdump','-dr','-M','no-aliases,reg-names=32',str(o)],capture_output=True,text=True).stdout)
PY
