#!/bin/sh
# w22a (Pi): m2c on a boot-tail nonmatching .s (FPR abi names -> $fN, macros stripped, jump tables read from the ROM)
# usage: sh tools/m2c.sh SEG/FN [m2c args]
R=/home/cburnes/projects/rush2049-decomp
p=$1; shift; t=$(mktemp)
python3 - "$R/asm/us/nonmatchings/rom/$p.s" "$R/baserom.us.z64" > $t <<'PY'
import re, sys, struct
names = {}
for i, b in enumerate(["fv0","fv1","ft0","ft1","ft2","ft3","fa0","fa1","ft4","ft5","fs0","fs1","fs2","fs3","fs4","fs5"]):
    names[b] = 2*i; names[b+"f"] = 2*i+1
rom = open(sys.argv[2], 'rb').read()
lines = open(sys.argv[1]).read().splitlines()
# jump tables: jtbl_ADDR_main with bound from the preceding sltiu
tables = {}
for k, l in enumerate(lines):
    m = re.search(r'%hi\((jtbl_([0-9A-F]{8})\w*)\)', l)
    if m and m.group(1) not in tables:
        n = None
        for j in range(k, max(0, k - 8), -1):
            mm = re.search(r'sltiu\s+\$\w+, \$\w+, (0x[0-9A-Fa-f]+|\d+)', lines[j])
            if mm: n = int(mm.group(1), 0); break
        a = int(m.group(2), 16); o = a - 0x7FFFF400
        tables[m.group(1)] = [struct.unpack('>I', rom[o+4*i:o+4*i+4])[0] for i in range(n)]
want = {w for ws in tables.values() for w in ws}
out = [".set noat", ".set noreorder", ".section .text"]
have = set()
for l in lines:
    if l.startswith("nonmatching") or l.startswith("endlabel"): continue
    m = re.search(r'/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/', l)
    if m:
        a = int(m.group(1), 16)
        if a in want and a not in have and not out[-1].strip().startswith('.L%08X' % a):
            out.append('.L%08X:' % a)
        have.add(a)
    l = re.sub(r"/\*.*?\*/", "", l)
    l = re.sub(r"\$(f[a-z]+[0-9]f?)\b", lambda m: "$f%d" % names[m.group(1)] if m.group(1) in names else m.group(0), l)
    out.append(l.rstrip())
if tables:
    out.append(".section .rodata")
    for name, ws in tables.items():
        out.append("glabel " + name)
        out += [".word .L%08X" % w for w in ws]
print("\n".join(out))
PY
python3 $R/tools/mips_to_c/m2c.py "$@" $t; rm -f $t
