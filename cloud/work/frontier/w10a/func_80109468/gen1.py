import itertools,re
src=open('w7d_best.c').read()
old="""    for (j = 0; j < D_801161C4; j++) {
        for (y = 0; y < D_801161C4 / 2; y++) {
            blt->info->data[j * D_801161C4 / 2 + y] = 0;"""
assert old in src
names=['i','j','x','y','px','pz','col','k','m']
for a,b in itertools.permutations(names,2):
    new=old.replace('(j ','(%s '%a).replace('j++','%s++'%a).replace('j = 0','%s = 0'%a).replace('j < ','%s < '%a)
    new=new.replace('y = 0','%s = 0'%b).replace('y < ','%s < '%b).replace('y++','%s++'%b).replace('+ y]','+ %s]'%b).replace('[j *','[%s *'%a)
    s=src.replace(old,new)
    decl=''
    if 'k' in (a,b): decl+='    s32 k;\n'
    if 'm' in (a,b): decl+='    s32 m;\n'
    s=s.replace('    s32 hide;\n','    s32 hide;\n'+decl)
    open('g1/%s_%s.c'%(a,b),'w').write(s)
