import itertools
pre='''typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern s8 D_8011ED00;
extern s8 D_8011ED04;
extern u8 *D_8002B020;
extern u8 *D_8002B024;
extern u8 D_80394F70[];
extern u8 D_8039B440[];
extern u8 D_803B9A50[];
extern u8 D_803BAE20[];
extern u8 D_00BE4C70[];
extern u8 D_00BDA100[];

void *NextMaxPath(u32 addr, u32 size);
void audio_loop_control(void *ptr, s32 flag);
void inflate_decompress(u8 *src, u8 *dst, s32 flag);
void bzero(void *p, u32 n);
'''
bodies={
'b1':'''    void *p;

    if (*loaded != 0) {
        return;
    }
    if (&name == 0) {
        return;
    }
    p = NextMaxPath((u32) dst, end - dst);
    audio_loop_control(p, 0);
    inflate_decompress(rom, dst, 1);
    bzero(bssStart, bssEnd - bssStart);
    *loaded = 1;
''',
'b2':'''    if (&name == 0) {
        return;
    }
    if (*loaded == 0) {
        audio_loop_control(NextMaxPath((u32) dst, end - dst), 0);
        inflate_decompress(rom, dst, 1);
        bzero(bssStart, bssEnd - bssStart);
        *loaded = 1;
    }
''',
'b3':'''    void *p;
    char **np = &name;

    if (*loaded != 0) {
        return;
    }
    p = NextMaxPath((u32) dst, end - dst);
    audio_loop_control(p, 0);
    inflate_decompress(rom, dst, 1);
    bzero(bssStart, bssEnd - bssStart);
    *loaded = 1;
    if (np) {
    }
''',
}
vals={'bssEnd':('u8 *bssEnd','D_8039B440','D_803BAE20','E'),'bssStart':('u8 *bssStart','D_80394F70','D_803B9A50','S'),'loaded':('s8 *loaded','&D_8011ED04','&D_8011ED00','L'),'rom':('u8 *rom','D_8002B024','D_8002B020','R')}
for bk,b in bodies.items():
    for perm in itertools.permutations(['bssEnd','bssStart','loaded','rom']):
        params=['u8 *dst','char *name','u8 *end',vals[perm[0]][0],'u8 *romStart',vals[perm[1]][0],vals[perm[2]][0],vals[perm[3]][0]]
        a1=['(u8 *) 0x8038A400','"Extra"','(u8 *) 0x803BB380',vals[perm[0]][1],'D_00BE4C70',vals[perm[1]][1],vals[perm[2]][1],vals[perm[3]][1]]
        a2=['(u8 *) 0x8038A400','"Start"','(u8 *) 0x803BB380',vals[perm[0]][2],'D_00BDA100',vals[perm[1]][2],vals[perm[2]][2],vals[perm[3]][2]]
        src=pre+"\nvoid PrevMaxPath(%s) {\n%s}\n\nvoid InitMaxPath(void) {\n    PrevMaxPath(%s);\n}\n\nvoid sync_maxpath_to_checkpoint(void) {\n    PrevMaxPath(%s);\n}\n"%(', '.join(params),b,', '.join(a1),', '.join(a2))
        open('PrevMaxPath/v1/%s_%s.c'%(bk,''.join(vals[x][3] for x in perm)),'w').write(src)
