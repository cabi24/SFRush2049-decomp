import tempfile, struct, subprocess, re
def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f.name],
                             capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: r.append(re.sub(r'\s+', ' ', m.group(3)))
    return r

def norm_regs(x):
    x = re.sub(r'\$f\d+', 'F', x)
    x = re.sub(r'\b(zero|at|v[01]|a[0-3]|t\d|s[0-8]|ra|sp|gp|k[01])\b', 'R', x)
    x = re.sub(r'-?\d+\(R\)', 'M(R)', x)
    x = re.sub(r'0x[0-9a-f]+', 'N', x)
    return re.sub(r',-?\d+$', ',N', x)

