import re, sys
src, dst = sys.argv[1], sys.argv[2]
lines = open(src).read().split("\n")
NM = ('char nm[256]; uint32_t ln = MEM_U32(0x1001c8d0); uint32_t k; if (ln > 255) ln = 255; '
      'for (k = 0; k < ln; k++) nm[k] = MEM_S8(0x1001c4d0 + k); nm[ln] = 0; ')
targets = {}
# write-backs: find lines MEM_U32(R + 0) = X; within 80 lines after a "R = 0x1001c4b4;" line
base_regs = {}
out = []
fn = None
for i, l in enumerate(lines):
    if l.startswith("static ") and "(uint8_t *mem" in l and l.rstrip().endswith("{"):
        fn = l.split("(")[0].split()[-1]; base_regs = {}
    m = re.match(r"\s*(\w+) = 0x1001c4b4;", l)
    if m:
        base_regs[m.group(1)] = i
    out.append(l)
    m = re.match(r"\s*MEM_U32\((\w+) \+ 0\) = (\w+);", l)
    if m and m.group(1) in base_regs and i - base_regs[m.group(1)] < 120:
        out.append("{ " + NM + 'fprintf(stderr, "AREA %s:%d proc=%s area=%d\\n", "' + fn + '", ' + str(i) + ', nm, (int)MEM_U32(0x1001c4b4)); }')
    if l == "L47fd90:":
        out.append("{ " + NM + 'fprintf(stderr, "GETTEMP proc=%s size=%u found=%u area=%d\\n", nm, a2, v0, (int)MEM_U32(0x1001c4b4)); }')
open(dst, "w").write("\n".join(out))
