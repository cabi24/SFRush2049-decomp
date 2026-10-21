import sys
src=open('swm.c').read()
V={}
def v(name, *pairs):
    s=src
    for a,b in pairs:
        assert a in s, (name,a)
        s=s.replace(a,b)
    V[name]=s
tex_old='gTexDefPtrs[modelIdx].texDefs[i].texBase += IMAGptr;'
tex_new='gTexDefPtrs[modelIdx].texDefs[i].texBase = gTexDefPtrs[modelIdx].texDefs[i].texBase + IMAGptr;'
pal_old='gPalDefPtrs[modelIdx].palDefs[i].palBase += IMAGptr;'
pal_new='gPalDefPtrs[modelIdx].palDefs[i].palBase = gPalDefPtrs[modelIdx].palDefs[i].palBase + IMAGptr;'
v('B1_tex', (tex_old,tex_new))
v('B2_pal', (pal_old,pal_new))
v('B3_both', (tex_old,tex_new),(pal_old,pal_new))
v('B4_imagu', ('    s32 IMAGptr;','    u32 IMAGptr;'))
v('B5_imagr', (tex_old,'gTexDefPtrs[modelIdx].texDefs[i].texBase = IMAGptr + gTexDefPtrs[modelIdx].texDefs[i].texBase;'))
v('A1_offint', ('    u8 *offset;','    s32 offset;'),
   ('offset = *(u8 **)gWaveInstance[idx].ptr - (u32)gWaveInstance[idx].lastPtr;','offset = (s32)*(u8 **)gWaveInstance[idx].ptr - (s32)gWaveInstance[idx].lastPtr;'),
   ('func_80096BBC(&wad, (s32)WADptr, (s32)offset);','func_80096BBC(&wad, (s32)WADptr, offset);'),
   ('(first ? WADptr : offset)','(first ? (s32)WADptr : offset)'))
for k in sys.argv[1:] or V:
    open('v/%s.c'%k,'w').write(V[k])
    print(k)
