#!/bin/sh
# anchors.sh FILE.c [extra UARGS]: unit-score func_80105480 (tag w32a) and print its frame-top-relative stack anchors
# (DrawCircuitCode str, DrawScores sprintf buffer, ellipse label, last font-set old) next to retail's.
cd /home/cburnes/projects/rush2049-decomp
export TAG=w32a
UARGS="$(cat cloud/work/frontier/w32a/hs/uargs_hs.txt) $2" timeout 200 cloud/work/frontier/tools/trace/us.sh func_80105480 "$1" 2>&1 | tail -1
python3 - <<'PY'
import re,subprocess
o=subprocess.run(['mips-linux-gnu-objdump','-d','--no-show-raw-insn','build/blob_unit/w32a/unit.o'],capture_output=True,text=True).stdout
f=o[o.index('<func_80105480>:'):]; f=f[:f.index('\n\n')]
fr=int(re.search(r'addiu\s+sp,sp,-(\d+)',f).group(1))
adds=[int(x) for x in re.findall(r'addiu\s+\w+,sp,(\d+)',f)]
olds=[int(x) for x in re.findall(r'sw\s+v0,(\d+)\(sp\)',f)]
print('ours  frame',fr,'addiu-sp (T-rel):',sorted(set(fr-a for a in adds)),'sw v0 (T-rel):',sorted(set(fr-a for a in olds)))
print('retail frame 584 addiu-sp (T-rel): [88, 192, 448] sw v0 (T-rel): [460, 488]')
PY
