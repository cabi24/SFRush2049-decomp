/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef float f32;
typedef struct Vec3 { f32 x,y,z; } Vec3;
typedef union Color4 { u32 word; u8 bytes[4]; } Color4;
typedef struct Vertex20 { f32 x,y,z; s16 s,t; u32 color; } Vertex20;
typedef struct Record88 { s16 count; u16 flags,texture,reserved; Vertex20 vertices[4]; } Record88;
typedef struct Panel56 { f32 x0,y0,x1,y1; u8 other[8]; f32 s[4],t[4]; } Panel56;
typedef struct Text36 { u32 name; s16 x,y,width,height,a,b,c,d; u32 e,f,callback,flags; } Text36;
extern int D_801391E4;
extern s16 D_80151AD0;
extern Panel56 D_80114264[][4];
extern u32 D_801145D4[][4];
extern Color4 D_8011464C;
extern char D_801202E8[];
extern u8 D_80140BDC;
extern void *D_80114638[];
extern Record88 *D_80114628[4];
extern u32 D_801174B4;
extern Text36 D_8011421C[],D_801141D4[];
extern void *D_80114624;
extern void *func_800B24EC();
extern Record88 *func_800A78BC();
extern void *sound_control(s16,s16,Text36 *,s16);
void particle_lifetime_set(void)
{
    int i,j;
    Color4 color;
    u16 texture;
    Vec3 quad[4];
    f32 scale,x0,y0,x1,y1;
    Record88 *record;
    D_801391E4 = 1025;
    for (i=0;i<4;i++) {
        color = D_8011464C;
        D_80114638[0] = func_800B24EC(D_801202E8,&texture,0,(s8)(D_80140BDC-1),1);
        scale = (f32)D_801391E4;
        y0 = D_80114264[D_80151AD0-1][i].y0 * scale;
        y1 = D_80114264[D_80151AD0-1][i].y1 * scale;
        x0 = D_80114264[D_80151AD0-1][i].x0 * scale;
        x1 = D_80114264[D_80151AD0-1][i].x1 * scale;
        quad[0].x=y0; quad[0].y=y1; quad[0].z=scale;
        quad[1].x=x0; quad[1].y=y1; quad[1].z=scale;
        quad[2].x=x0; quad[2].y=x1; quad[2].z=scale;
        quad[3].x=y0; quad[3].y=x1; quad[3].z=scale;
        D_80114628[i] = func_800A78BC(4,quad,texture,&color,D_801145D4[D_80151AD0][i]|0x1210,1);
        D_80114628[i]->flags &= 0x7FFF;
        if (D_80114638[D_80151AD0] == (void *)1) {
            record = D_80114628[i];
            for(j=0;j<record->count;j++) {
                record->vertices[j].s = (s16)D_80114264[D_80151AD0-1][i].s[j];
                record->vertices[j].t = (s16)D_80114264[D_80151AD0-1][i].t[j];
            }
        }
    }
    if (D_801174B4 & 0x80) D_80114624 = sound_control(0,0,D_8011421C,2);
    else D_80114624 = sound_control(0,0,D_801141D4,2);
}
