#!/usr/bin/env python3
"""var.py BASE.c FN KEEP VARIANTS.txt: for each variant block in VARIANTS.txt (separated by lines '=== name'),
replace the region between '/*<FN*/' and '/*FN>*/' markers in BASE.c with the block, run fg.sh, print the summary row count.
Extra '@@pre' section inside a block (lines after '@@pre') is inserted at the '/*<PRE>*/' marker."""
import sys, re, subprocess, tempfile, os
base, fn, keep, vf = sys.argv[1:5]
src = open(base).read()
blocks = re.split(r'^=== *(.*)$', open(vf).read(), flags=re.M)[1:]
here = os.path.dirname(os.path.abspath(__file__))
for name, body in zip(blocks[0::2], blocks[1::2]):
    pre = ''
    if '@@pre' in body:
        body, pre = body.split('@@pre', 1)
    s = re.sub(r'/\*<%s\*/.*?/\*%s>\*/' % (fn, fn), lambda m: body.strip('\n'), src, flags=re.S)
    s = s.replace('/*<PRE>*/', pre)
    with tempfile.NamedTemporaryFile('w', suffix='.c', delete=False) as f:
        f.write(s)
    out = subprocess.run([here + '/fg.sh', f.name, fn, keep], capture_output=True, text=True)
    last = (out.stdout.strip().splitlines() or [out.stderr.strip()[-300:]])[-1]
    print('%-28s %s' % (name.strip(), last))
    if '-v' in sys.argv: print(out.stdout)
    os.unlink(f.name)
