src=open('best_body.c').read()
old="""    if ((*surf == 3 || *surf == 4 || *surf == 7) && out[1] < 0.25f) {"""
assert old in src
tail_old=src[src.index(old):src.index("    return;\n    }\n    }\nmiss:")]
V={}
for var,decl in [('i',None),('off',None),('quad',None),('idx',None),('s','    s32 s;\n')]:
    t=tail_old.replace('*surf',var)
    v=src.replace(tail_old,"    %s = *surf;\n"%var+t)
    if decl: v=v.replace("    f32 m[3][3];\n","    f32 m[3][3];\n"+decl)
    V['b_'+var]=v
for k,v in V.items(): open(k+'.c','w').write(v)
