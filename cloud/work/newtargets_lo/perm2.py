import itertools,subprocess,re
head=open('body/func_8008BEA4.c').read().split('void func_8008BEA4')[0]
tail_='''    if (flag == 0 || !ok) {
        if (D_8014978C == 3 || D_8014978C == 5) {
            CP(D_8011B58C);
        } else {
            CP(D_8011B57C);
        }
    } else {
        if (D_8014978C == 3 || D_8014978C == 5) {
            CP(D_8011B59C);
        } else {
            CP(D_8011B58C);
        }
    }
}
'''
for decl in itertools.permutations(['u32 f;','s32 ok;','Out *o;','s32 i;']):
  for ostage in (0,1,2):
    body='void func_8008BEA4(In *in, s16 flag) {\n'+'\n'.join('    '+d for d in decl)+'\n'
    body+='    i = in->idx;\n'
    if ostage==0: body+='    o = D_8013FEF4[i].o;\n'
    body+='    f = D_80152900[i].flags;\n'
    if ostage==1: body+='    o = D_8013FEF4[i].o;\n'
    body+='    ok = (f & 0x1000) != 0;\n    if (!ok) {\n        ok = (f & 0x10) == 0;\n    }\n'
    if ostage==2: body+='    o = D_8013FEF4[i].o;\n'
    open('body/func_8008BEA4.c','w').write(head+body+tail_)
    out=subprocess.run(['./mk.sh','func_8008BEA4'],capture_output=True,text=True,env={'LINES_N':'40','PATH':'/usr/bin:/bin'}).stdout
    m=re.findall(r'(\d+)/\d+ words',out)
    print(decl,ostage,m or out.strip()[-20:])
