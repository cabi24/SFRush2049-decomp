typedef signed char s8; typedef unsigned char u8; typedef int s32;
typedef struct Sub { s8 pad0[5]; s8 on; s8 pad6[34]; } Sub;          /* 40 */
typedef struct Rec { s8 pad0[11]; s8 active; s8 pad12[116]; Sub sub[16]; s8 pad[4]; } Rec; /* 772 */
typedef struct ModelData { struct ModelLink *next; s8 pad4[13]; u8 wheel; s8 pad18[54]; s32 enabled; } ModelData;
typedef struct ModelLink { ModelData *data; } ModelLink;
typedef struct ModelList { ModelLink *head; s32 pad[3]; } ModelList;
extern Rec D_80144030[];
extern ModelList D_80144D68[];
void func_800A1BB4(s32 index) {
    ModelData *data;
    ModelLink *p, *next;
    if (D_80144030[index].active != 0) {
        p = D_80144D68[index].head;
        while (p != 0) {
            data = p->data;
            next = data->next;
            if (data->enabled != 0) {
                if (((s8 *)&D_80144030[index])[133 + data->wheel * 40] != 0) break;
            }
            p = next;
        }
        if (p == 0) D_80144030[index].active = 0;
    }
}
