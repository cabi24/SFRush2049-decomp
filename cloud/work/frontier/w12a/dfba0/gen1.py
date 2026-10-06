src=open('comp/mode.c').read()
top="    for (i=0;i<4;i++) {\n        if (model->contact[i]==0) {\n"
assert top in src
V={}
def add(name, stmts, where='top'):
    if where=='top':
        V[name]=src.replace(top,"    for (i=0;i<4;i++) {\n"+stmts+"        if (model->contact[i]==0) {\n")
add('e_all',"        if (count0) {}\n        if (count1) {}\n        if (count2) {}\n        if (i) {}\n")
add('e_cnt',"        if (count0) {}\n        if (count1) {}\n        if (count2) {}\n")
add('e_one',"        if (count0|count1|count2|i) {}\n")
add('e_one2',"        if (count0 || count1 || count2 || i) {}\n")
for k,v in V.items(): open('dfba0/'+k+'.c','w').write(v)
