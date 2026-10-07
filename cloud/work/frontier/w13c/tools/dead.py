import sys
src=open(sys.argv[1]).read().split('\n')
out=sys.argv[2]
for spec in sys.argv[3:]:
    name,line,text=spec.split(':',2)
    s=list(src); s.insert(int(line), '                '+text)
    open(f"{out}/{name}.c",'w').write('\n'.join(s))
