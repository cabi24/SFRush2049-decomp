/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef struct Sub { s8 pad0[5]; s8 on; s8 pad6[34]; } Sub;
typedef struct Rec { s8 pad0[11]; s8 active; s8 pad12[116]; Sub sub[16]; s8 pad[4]; } Rec;
typedef struct ModelLink { struct ModelData *data; } ModelLink;
typedef struct ModelData { ModelLink *next; s8 pad4[13]; u8 wheel; s8 pad18[54]; s32 enabled; } ModelData;
typedef struct ModelList { ModelLink *head; s32 pad[3]; } ModelList;
extern Rec D_80144030[];
extern ModelList D_80144D68[];

void func_800A1BB4(s32 index) {
    ModelData *data;
    ModelLink *p, *next;
    if (D_80144030[index].active == 0) return;
    p = D_80144D68[index].head;
    if (p == 0) { D_80144030[index].active = 0; return; }
    while (1) { if (p == 0) break;
        data = p->data;
        next = data->next;
        p = data->enabled != 0;
        if (p) {
            if (D_80144030[index].sub[data->wheel].on != 0) break;
        }
        p = next;
    }
    if (p == 0) D_80144030[index].active = 0;
}
