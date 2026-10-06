import sys
src=open('base.c').read()
OLD="""                lo = curve->level;
                hi = lo[4];
                if (hi < sample) {
                    sample = hi;
                }
                for (segment = 0; segment < 4; segment++, lo++) {"""
assert OLD in src
V={}
V['segfirst']="""                segment = 0;
                lo = curve->level;
                hi = lo[4];
                if (hi < sample) {
                    sample = hi;
                }
                for (; segment < 4; segment++, lo++) {"""
V['seghi']="""                hi = curve->level[4];
                segment = 0;
                lo = curve->level;
                if (hi < sample) {
                    sample = hi;
                }
                for (; segment < 4; segment++, lo++) {"""
V['commafor']="""                hi = curve->level[4];
                if (hi < sample) {
                    sample = hi;
                }
                for (segment = 0, lo = curve->level; segment < 4; segment++, lo++) {"""
V['commafor2']="""                lo = curve->level;
                hi = lo[4];
                if (hi < sample) {
                    sample = hi;
                }
                for (segment = 0, lo = curve->level; segment < 4; segment++, lo++) {"""
V['hicv']="""                hi = curve->level[4];
                if (hi < sample) {
                    sample = hi;
                }
                lo = curve->level;
                for (segment = 0; segment < 4; segment++, lo++) {"""
for k,v in V.items():
    s=src.replace(OLD,v)
    open('v1/%s.c'%k,'w').write(s)
# dead early segment
s=src.replace("            curve = &D_80120EEC[i];\n","            segment = 0;\n            curve = &D_80120EEC[i];\n",1)
open('v1/deadtop.c','w').write(s)
s=src.replace("    car = &D_8014A250[index];\n","    segment = 0;\n    car = &D_8014A250[index];\n",1)
open('v1/deadfn.c','w').write(s)
