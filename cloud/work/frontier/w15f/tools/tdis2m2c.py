"""tdis.py output -> m2c-readable asm (glabel, $regs, .L labels). usage: tdis2m2c.py NAME > f.s"""
import re, subprocess, sys
name = sys.argv[1]
out = subprocess.run([sys.executable, 'cloud/work/tools/tdis.py', name], capture_output=True, text=True).stdout
lines = [l for l in out.splitlines() if re.match(r'\s+[0-9a-f]{8}:', l)]
regs = r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b'
targets = set()
ins = []
for l in lines:
    a, rest = l.strip().split(':', 1)
    rest = rest.strip()
    m = re.match(r'(\S+)\s*(.*)', rest)
    op, args = m.group(1), m.group(2)
    args = re.sub(r'\s*<.*>', '', args)
    if op == 'jal':
        sym = re.search(r'<([^,>]+)', rest)
        args = sym.group(1) if sym else 'func_%08X' % int(args, 16)
    elif op.startswith('b') or op == 'j':
        t = re.search(r'0x([0-9a-f]{8})', args)
        if t:
            targets.add(t.group(1)); args = args.replace('0x' + t.group(1), '.L' + t.group(1))
    args = re.sub(regs, lambda m: '$' + m.group(1), args)
    args = args.replace('$f', '$f').replace('c1_fcsr', '$31')
    args = re.sub(r'(?<![\w$])(f\d+)\b', r'$\1', args)
    ins.append((a, op, args))
print('glabel %s' % name)
for a, op, args in ins:
    if a in targets:
        print('.L%s:' % a)
    print('    %s %s' % (op, args))
