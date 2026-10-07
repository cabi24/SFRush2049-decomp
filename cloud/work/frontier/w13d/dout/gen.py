import sys
# gen.py BASE OUTPREFIX tail1 tail2 ... ; replaces final "    if(parent){}\n}" with given tail ('|' = newline)
base=open(sys.argv[1]).read(); old="    if(parent){}\n}"
assert old in base
for i,t in enumerate(sys.argv[3:]):
    body="".join("    %s\n"%x for x in t.split('|') if x)
    open("%s%d.c"%(sys.argv[2],i),'w').write(base.replace(old,body+"}",1))
    print("%s%d.c"%(sys.argv[2],i), t)
