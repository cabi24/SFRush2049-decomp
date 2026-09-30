import re,sys
src=open(sys.argv[1]).read()
# K&R-ize prototypes of externs that look like function decls with '(' and '/* extern */'
out=[]
for l in src.split('\n'):
    m=re.match(r'^(M2C_UNK|s32|s16|s8|u8|u16|u32|f32|void|s64)\s+(\w+)\(.*\);\s*/\* extern \*/',l)
    if m: l=f'extern {m.group(1)} {m.group(2)}();'
    m=re.match(r'^extern M2C_UNK (D_\w+);',l)
    if m: l=f'extern u8 {m.group(1)}[];'
    out.append(l)
print('\n'.join(out))
