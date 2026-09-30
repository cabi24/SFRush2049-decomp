import itertools,subprocess,sys,re
hdr=open('d.c').read().split('u32 func_800E79F8')[0]
decl={'max':'u32 max;','n':'Node *n;','hh':'Head *hh;'}
res=[]
for p in itertools.permutations(decl):
  for initpos in (0,1):
    body='u32 func_800E79F8(Head *h) {\n'+'\n'.join('    '+decl[k] for k in p)+'\n'
    body+='    osRecvMesg(&D_80152770, 0, 1);\n'
    if initpos==0: body+='    max = 0;\n'
    body+='    hh = h ? h : D_801527C8;\n'
    if initpos==1: body+='    max = 0;\n'
    body+='    for (n = hh->first; n != 0; n = n->next) {\n        if (n->used == 0 && max < n->size) max = n->size;\n    }\n    osJamMesg(&D_80152770, 0, 0);\n    return max;\n}\n'
    open('p.c','w').write(hdr+body)
    o=subprocess.run(['python3','tools/cloud/score.py','fn','cloud/work/newtargets_mid/p.c','func_800E79F8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='/home/user/SFRush2049-decomp').stdout
    m=re.search(r'(\d+)/\d+ words',o)
    print(p,initpos,'MATCH' if 'MATCH' in o else (m.group(1) if m else o[-80:]))
