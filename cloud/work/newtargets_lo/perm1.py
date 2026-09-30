import itertools,subprocess,re
terms={'H':'(w & 0xFFFF0000)','M':'((w & 0xFF00) >> 8)','L':'((w & 0xFF) << 8)'}
def expr(order,var):
    return ' | '.join(t.replace('w',var) for t in (terms[o] for o in order))
for order in itertools.permutations('HML'):
    e0=expr(order,'w'); e1=expr(order,'w1')
    src=f'''s32 func_8008AD6C(u32 *p) {{
    u32 w = p[0];
    u32 op = w & 0xFF000000;
    if (op == 0x05000000) {{
        p[0] = {e0};
    }} else if (op == 0x07000000 || op == 0x06000000) {{
        u32 w1 = p[1];
        p[0] = {e0};
        p++;
        *p = {e1};
    }}
    return 2;
}}
'''
    open('body/func_8008AD6C.c','w').write(src)
    out=subprocess.run(['./mk.sh','func_8008AD6C'],capture_output=True,text=True,env={'LINES_N':'40','PATH':'/usr/bin:/bin'}).stdout
    print(order,re.findall(r'(\d+)/\d+ words',out),[l.split()[-1:]+l.split()[-3:-2] for l in out.splitlines() if '+0x018' in l])
