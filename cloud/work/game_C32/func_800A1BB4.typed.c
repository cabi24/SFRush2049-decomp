/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef int s32;
typedef signed char s8;
typedef unsigned char u8;
typedef struct ModelLink ModelLink;
typedef struct ModelData {ModelLink *next; u8 pad04[13]; u8 wheel; u8 pad12[54]; s32 enabled;} ModelData;
struct ModelLink {ModelData *data;};
typedef struct ModelList {ModelLink *head; s32 reserved[3];} ModelList;
extern u8 D_80144030[][772];
extern ModelList D_80144D68[];
void func_800A1BB4(s32 index) {
    ModelLink *p, *next;
    ModelData *data;
    if (*(s8 *)&D_80144030[index][11]) {
        p = D_80144D68[index].head;
        while (p) {
            data = p->data;
            next = data->next;
            if (data->enabled && *(s8 *)&D_80144030[index][133 + data->wheel * 40]) break;
            p = next;
        }
        if (!p) D_80144030[index][11] = 0;
    }
}
