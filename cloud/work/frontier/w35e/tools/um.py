#!/usr/bin/env python3
"""um.py (builder-side) GROUPDIR: run the group pipeline and print umerge's output"""
import sys, json, re, shlex, tempfile
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
g = Path(sys.argv[1]); spec = json.loads((g/'group.json').read_text())
with tempfile.TemporaryDirectory() as tmp:
    w = Path(tmp)
    for n in spec['files']: (w/n).write_text((g/n).read_text())
    (w/'keep.txt').write_text(''.join(k+'\n' for k in spec['keep']))
    flags = shlex.split(spec['flags']); units = [re.sub(r'\.c$', '.u', f) for f in spec['files']]
    common = ["-mips2","-EB","-g0","-O3"]
    steps = [
        [score.ido("cc"),"-j",*flags,*spec['files']],
        [score.ido("uld"),"-L/usr/lib/mips2/nonshared","-_SYSTYPE_SVR4","-mips2","-non_shared","-g0","-no_AutoGnum","-kp","keep.txt",*units,"-ko","linked"],
        [score.ido("usplit"),"-mips2","-o","split","-t","st","linked"],
        [score.ido("umerge"),"-v","-Olimit","5000",*common,"split","-o","merged","-t","st"],
    ]
    for st in steps:
        p = score._run(st, cwd=w)
        if st[0].endswith('umerge'): print(p.stdout); print(p.stderr)
