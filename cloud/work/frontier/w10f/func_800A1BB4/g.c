typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef struct Sub { s8 pad0[5]; s8 on; s8 pad6[34]; } Sub;          /* 40 */
typedef struct Rec { s8 pad0[11]; s8 active; s8 pad12[116]; Sub sub[16]; s8 pad[4]; } Rec; /* 772 */
typedef struct ModelData { struct ModelLink *next; s8 pad4[13]; u8 wheel; s8 pad18[54]; s32 enabled; } ModelData;
typedef struct ModelLink { ModelData *data; } ModelLink;
typedef struct ModelList { ModelLink *head; s32 pad[3]; } ModelList;
extern Rec D_80144030[];
extern ModelList D_80144D68[];
void func_800A1BB4(s32 index) {
    ModelLink *p, *next;
    ModelData *data;
    if (D_80144030[index].active == 0) return;
    for (p = D_80144D68[index].head; p != 0; p = next) {
        data = p->data;
        next = data->next;
        if (data->enabled != 0 && D_80144030[index].sub[data->wheel].on != 0) goto found;
    }
    D_80144030[index].active = 0;
found:
    ;
}
