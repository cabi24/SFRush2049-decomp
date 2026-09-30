import sys
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
r=score._run([score.ido('cc'),'-S','-g0','-O2','-mips2','-G','0','-non_shared',sys.argv[1]],cwd='.'); print(r.returncode, r.stderr[:200], file=sys.stderr)
import re
t=open(sys.argv[1].split('/')[-1].replace('.c','.s')).read()
t=t[t.index('func_800DE860:'):]
for l in t.splitlines():
    if re.search(r'noalias|\.alias|\.loc| #|^\s*$',l): continue
    print(l)
