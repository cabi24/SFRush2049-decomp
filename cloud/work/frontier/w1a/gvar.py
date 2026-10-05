#!/usr/bin/env python3
"""gvar.py GROUPDIR MARK VARIANTS.txt [-v]: for each '=== name' block in VARIANTS.txt, replace the
/*<MARK*/ ... /*MARK>*/ region of every .c file in GROUPDIR with the block, strict-score the group on
the builder (scg.sh) and print one summary line per variant (per-member MATCH / N words)."""
import sys, re, subprocess, tempfile, os, shutil
gdir, mark, vf = sys.argv[1:4]
here = os.path.dirname(os.path.abspath(__file__))
blocks = re.split(r'^=== *(.*)$', open(vf).read(), flags=re.M)[1:]
for name, body in zip(blocks[0::2], blocks[1::2]):
    t = tempfile.mkdtemp()
    for f in os.listdir(gdir):
        s = open(os.path.join(gdir, f)).read()
        if f.endswith('.c'):
            s = re.sub(r'/\*<%s\*/.*?/\*%s>\*/' % (mark, mark), lambda m: body.strip('\n'), s, flags=re.S)
        open(os.path.join(t, f), 'w').write(s)
    out = subprocess.run([here + '/scg.sh', t, 'gvar_' + mark], capture_output=True, text=True)
    txt = out.stdout + out.stderr
    res = []
    cur = None
    for line in txt.splitlines():
        m = re.match(r'^(\w+):$', line)
        if m: cur = m.group(1)
        elif cur and ('MATCH' in line or 'differ' in line):
            res.append('%s=%s' % (cur, 'MATCH' if line.strip() == 'MATCH' else line.strip().replace(' words differ', '')[:40]))
            cur = None
    print('%-22s %s' % (name.strip(), '  '.join(res) if res else txt[-300:]))
    if '-v' in sys.argv: print(txt)
    shutil.rmtree(t)
