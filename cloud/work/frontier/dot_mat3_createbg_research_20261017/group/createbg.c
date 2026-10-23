/* Native createBG adaptation of the complete front.c donor.
 * Original local order is retained. Native scale, texture-result store and
 * two-blit count differ from the later platform. Table indices and extents
 * follow the original source and separately verified native initialization. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef union Color4 {struct {u8 r,g,b,a;} c;u32 rgba;} Color4;
typedef struct Vertex20 {float x,y,z;s16 s,t;u32 color;} Vertex20;
typedef struct Mesh {s16 count;u16 flags,parameter,unknown;Vertex20 vertices[4];} Mesh;
typedef struct Texture36 {u8 prefix[16];u16 width,height;u8 middle[8];u32 flags;u32 tail;} Texture36;
typedef struct BackRect56 { f32 minX,maxX,minY,maxY,dx,dy; f32 offX[4],offY[4]; } BackRect56;
typedef struct Blit Blit;
typedef struct MultiBlit {
 const char *texname;
 s16 dulx,duly,width,height,top,bot,left,right;
 u32 zdepth,alpha;
 s32 (*animfunc)(Blit *);
 u32 animid;
} MultiBlit;
static BackRect56 D_80114264[4][4] = {
    {
        { -1.0f, 0.0f, -1.0f, 0.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { 1.0f, 0.0f, -1.0f, 0.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { 1.0f, 0.0f, 1.0f, 0.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { -1.0f, 0.0f, 1.0f, 0.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
    },
    {
        { -2.0f, 0.0f, -1.0f, 1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { 2.0f, 0.0f, -1.0f, 1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { 2.0f, 0.0f, 1.0f, -1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { -2.0f, 0.0f, 1.0f, -1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
    },
    {
        { -1.0f, 1.0f, -1.0f, 1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { 1.0f, -1.0f, -1.0f, 1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { 1.0f, -1.0f, 1.0f, -1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { -1.0f, 1.0f, 1.0f, -1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
    },
    {
        { -1.0f, 1.0f, -1.0f, 1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { 1.0f, -1.0f, -1.0f, 1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { 1.0f, -1.0f, 1.0f, -1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
        { -1.0f, 1.0f, 1.0f, -1.0f, 0.2f, 0.2f,
          {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f} },
    },
};


static int D_801145E4[4][4] = {
    {1, 1, 1, 1},
    {2, 2, 1, 1},
    {4, 8, 2, 1},
    {4, 8, 2, 1},
};

extern Mesh *D_80114628[4];
extern Texture36 *D_80114638;
extern s32 D_8011463C[];
extern Color4 D_8011464C;
extern char D_801202E8[];
extern volatile u8 D_80140BDC;
extern s16 D_80151AD0;
extern s32 D_801391E4;
extern u32 state_word_a;
extern Blit *D_80114624;
extern MultiBlit D_8011421C[],D_801141D4[];
Texture36 *func_800B24EC(const char *,u16 *,s8,s8,s32);
Mesh *func_800A78BC(s32,f32 (*)[3],u16,Color4 *,u16,s32);
Blit *sound_control(s16,s16,const MultiBlit *,s16);
void native_ShowPoly(Mesh *poly) { poly->flags &= ~0x8000; }

void particle_lifetime_set(void)
{
    int polyIdx;
    float polyVerts[4][3];
    unsigned short texIdx;
    Color4 backColor;
    Mesh *backPoly;
    int vertIdx;
    float backDistance; /* Native global snapshot; absent from later MAT3 DWARF. */

    D_801391E4 = 1025;
    for (polyIdx = 0; polyIdx < 4; polyIdx++) {
        backColor = D_8011464C;
        D_80114638 = func_800B24EC(D_801202E8, &texIdx,
                                  0, D_80140BDC - 1, 1);
        backDistance = D_801391E4;
        polyVerts[0][0] = backDistance * D_80114264[D_80151AD0 - 1][polyIdx].maxX;
        polyVerts[0][1] = backDistance * D_80114264[D_80151AD0 - 1][polyIdx].maxY;
        polyVerts[0][2] = backDistance;
        polyVerts[1][0] = backDistance * D_80114264[D_80151AD0 - 1][polyIdx].minX;
        polyVerts[1][1] = backDistance * D_80114264[D_80151AD0 - 1][polyIdx].maxY;
        polyVerts[1][2] = backDistance;
        polyVerts[2][0] = backDistance * D_80114264[D_80151AD0 - 1][polyIdx].minX;
        polyVerts[2][1] = backDistance * D_80114264[D_80151AD0 - 1][polyIdx].minY;
        polyVerts[2][2] = backDistance;
        polyVerts[3][0] = backDistance * D_80114264[D_80151AD0 - 1][polyIdx].maxX;
        polyVerts[3][1] = backDistance * D_80114264[D_80151AD0 - 1][polyIdx].minY;
        polyVerts[3][2] = backDistance;
        D_80114628[polyIdx] = func_800A78BC(4, polyVerts, texIdx, &backColor,
                                         D_801145E4[D_80151AD0 - 1][polyIdx] | 0x1210, 1);
        native_ShowPoly(D_80114628[polyIdx]);
        if (D_8011463C[D_80151AD0 - 1] == 1) {
            backPoly = D_80114628[polyIdx];
            for (vertIdx = 0; vertIdx < backPoly->count; vertIdx++) {
                backPoly->vertices[vertIdx].s = D_80114264[D_80151AD0 - 1][polyIdx].offX[vertIdx];
                backPoly->vertices[vertIdx].t = D_80114264[D_80151AD0 - 1][polyIdx].offY[vertIdx];
            }
        }
    }
    if (state_word_a & 0x80)
        D_80114624 = sound_control(0, 0, D_8011421C, 2);
    else
        D_80114624 = sound_control(0, 0, D_801141D4, 2);
}

/* Actual native updateBG writer. Float shift[2] and mixed zero spellings are
 * inherited locked reconstruction choices, not original DWARF declarations. */
extern f32 D_8002EB94;
void func_800FC9F8(void) {
    s32 slot;
    s32 i;
    Mesh *list;
    f32 shift[2];

    for (slot = 0; slot < 4; slot++) {
        list = D_80114628[slot];
        if (list != 0) {
            shift[0] = 0.0f;
            shift[1] = 0.0f;
            for (i = 0; i < list->count; i++) {
                list->vertices[i].s += D_80114264[D_80151AD0 - 1][slot].dx * D_8002EB94 * (D_80114638->width << 5);
                list->vertices[i].t += D_80114264[D_80151AD0 - 1][slot].dy * D_8002EB94 * (D_80114638->height << 5);
            }
            for (i = 0; i < list->count; i++) {
                if (list->vertices[i].s >= D_80114638->width << 5) {
                    shift[0] -= 1.0f;
                }
                if (list->vertices[i].s <= -D_80114638->width << 5) {
                    shift[0] += 1.0f;
                }
                if (list->vertices[i].t >= D_80114638->height << 5) {
                    shift[1] -= 1.0f;
                }
                if (list->vertices[i].t <= D_80114638->height << 5) {
                    shift[1] += 1.0f;
                }
            }
            if (shift[0] == -4.0f) {
                shift[0] = -D_80114638->width << 5;
            } else if (shift[0] == 4.0f) {
                shift[0] = D_80114638->width << 5;
            } else {
                shift[0] = 0;
            }
            if (shift[1] == -4.0f) {
                shift[1] = -D_80114638->height << 5;
            } else if (shift[1] == 4.0f) {
                shift[1] = D_80114638->height << 5;
            } else {
                shift[1] = 0;
            }
            for (i = 0; i < list->count; i++) {
                list->vertices[i].s += shift[0];
                list->vertices[i].t += shift[1];
                D_80114264[D_80151AD0 - 1][slot].offX[i] = list->vertices[i].s;
                D_80114264[D_80151AD0 - 1][slot].offY[i] = list->vertices[i].t;
            }
            D_8011463C[D_80151AD0 - 1] = 1;
        }
    }
}
