import subprocess,re,itertools
hdr='#include "mdl_hdr.h"\n#define T D_8012E700\n'
def W(i,v,rd='(short)'): return '{ unsigned int t = T[%s%s].flags; T[%s].flags = t | %s; }'%(rd,i,i,v)
m2={
 'rec': lambda: f'''{W("idx","(f << 8)")}
        if (T[idx].child != -1) model_data_load(T[idx].child, 3, f);''',
 'rec_c': lambda: f'''{W("idx","(f << 8)")}
        c = T[idx].child;
        if (c != -1) model_data_load(c, 3, f);''',
 'rec_c2': lambda: f'''{W("idx","(f << 8)")}
        if ((c = T[idx].child) != -1) model_data_load(c, 3, f);''',
}
m3={
 'do': lambda: f'''do {{ {W("idx","(f << 8)")}
            if (T[idx].child != -1) model_data_load(T[idx].child, 3, f);
            idx = T[idx].sibling;
        }} while (idx != -1);''',
 'do_c': lambda: f'''do {{ {W("idx","(f << 8)")}
            c = T[idx].child;
            if (c != -1) model_data_load(c, 3, f);
            idx = T[idx].sibling;
        }} while (idx != -1);''',
 'for_c': lambda: f'''for (; idx != -1; idx = T[idx].sibling) {{ {W("idx","(f << 8)")}
            c = T[idx].child;
            if (c != -1) model_data_load(c, 3, f);
        }}''',
 'for': lambda: f'''for (; idx != -1; idx = T[idx].sibling) {{ {W("idx","(f << 8)")}
            if (T[idx].child != -1) model_data_load(T[idx].child, 3, f);
        }}''',
}
res=[]
for (k2,f2),(k3,f3),ctype,order in itertools.product(m2.items(),m3.items(),['short','int'],['2013','0123']):
    blocks={'2':f'if (mode == 2) {{ {f2()} }}','0':f'if (mode == 0) {{ {W("idx","0x80000000")} }}','1':f'if (mode == 1) {{ {W("idx","(f << 8)")} }}','3':f'if (mode == 3) {{ {f3()} }}'}
    body=' else '.join(blocks[c] for c in order)
    src=hdr+f'void model_data_load(int idx, int mode, int f) {{\n    {ctype} c;\n    {body}\n}}\n'
    open('sv4.c','w').write(src)
    r=subprocess.run(['./nr.sh','cloud/work/r5_e/sv4.c','model_data_load'],capture_output=True,text=True).stdout
    m=re.search(r'got (\d+) \(.*strict-equal (\d+); aligned exact (\d+)',r)
    if not m: continue
    res.append((int(m.group(3)),int(m.group(2)),int(m.group(1)),k2,k3,ctype,order))
    if int(m.group(3))==93: open('mdl_cand.c','w').write(src)
res.sort(reverse=True)
for x in res[:8]: print(x)
