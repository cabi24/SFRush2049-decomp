/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned short u16;
typedef signed short s16;
typedef float f32;
typedef unsigned char u8;
typedef unsigned int u32;
typedef union Color4 {struct {u8 r,g,b,a;} c;u32 rgba;} Color4;
typedef struct Vertex20 {float x,y,z;s16 s,t;u32 color;} Vertex20;
typedef struct Mesh {s16 count;u16 flags,parameter,unknown;Vertex20 vertices[4];} Mesh;
extern Mesh D_8015B268[];
extern int D_801613B4,D_8014269C;
extern void func_8008C074(Mesh *, int, f32 (*)[3], u16, Color4 *, u16, int);
Mesh *func_800A78BC(int active, f32 (*vertices)[3], u16 parameter, Color4 *color, u16 flags, int indexed)
{
    int i;
    Mesh *record;
    for (i = 0; i < D_801613B4; i++) {
        if (!D_8015B268[i].count) break;
    }
    if (i >= 200) return 0;
    record = &D_8015B268[i];
    if (i >= D_801613B4) D_801613B4 = i + 1;
    if (D_8014269C < D_801613B4) D_8014269C = D_801613B4;
    record->flags = flags;
    if (indexed) {
        record->flags |= 0x400;
        record->parameter = parameter;
    }
    func_8008C074(record, active, vertices, parameter, color, 0, indexed);
    return record;
}
