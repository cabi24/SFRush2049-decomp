import re,sys
p=sys.argv[1]; L=open(p).read().split('\n')
st=[i for i,l in enumerate(L) if l.startswith('static void f_cupcosts(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2) {')][0]
en=st+1
while not L[en].startswith('static '): en+=1
n=0
for i in range(st,en):
    m=re.match(r'^f20\.f\[0\] = f20\.f\[0\] \+ (f\d+)\.f\[0\];$',L[i])
    if m:
        L[i]='{ %s cup_log(mem, s3, s2, s1, %d, %s.f[0]); }'%(L[i],i-st,m.group(1)); n+=1
helper='''
#include <stdlib.h>
static int cup_web=-3;
static void cup_log(uint8_t *mem, uint32_t lr, uint32_t col, uint32_t lu, int site, float inc) {
    if (cup_web==-3) { const char *e=getenv("CUPWEB"); cup_web = e ? atoi(e) : -1; }
    if (cup_web<0 || lr==0) return;
    if ((int)MEM_U32(lr+4)!=cup_web) return;
    uint32_t bb = lu ? MEM_U32(lu+0) : 0;
    fprintf(stderr, "[CUP] web=%d color=%d site=%d lu_bb=%d w=%d inc=%.3f\\n", (int)MEM_U32(lr+4), (int)col, site,
        bb ? (int)MEM_U16(bb+8) : -1, bb ? (int)MEM_U32(bb+44) : -1, (double)inc);
}
'''
L.insert(st,helper)
open(p,'w').write('\n'.join(L)); print('patched',n,'sites')
