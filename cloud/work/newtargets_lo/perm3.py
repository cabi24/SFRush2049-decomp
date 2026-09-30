import subprocess,re,itertools
tmpl=open('body/entity_name_copy.c').read()
variants={
 'end':['end = base + n * size;','end = n * size + base;','end = base + size * n;','end = (u8 *)((u32)base + n * size);'],
 'p':['p = mid;','p = (u8 *)(u32)mid;','p = base + half * size;'],
}
best=[]
for e in variants['end']:
  for p in variants['p']:
    s=tmpl.replace('end = base + n * size;',e).replace('p = mid;',p)
    open('body/entity_name_copy.c','w').write(s)
    out=subprocess.run(['./mk.sh','entity_name_copy'],capture_output=True,text=True,env={'LINES_N':'40','PATH':'/usr/bin:/bin'}).stdout
    m=re.findall(r'(\d+)/\d+ words',out); print(e,p,m)
open('body/entity_name_copy.c','w').write(tmpl)
