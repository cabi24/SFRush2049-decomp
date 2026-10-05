/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct Link { struct Link *next, *prev; } Link;
typedef struct List { u8 indirect, doubly, pad[2]; u32 count; Link *head, *tail; } List;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct Entry {
    /* 0x00 */ s32 unk0[17];
    /* 0x44 */ s32 object;
    /* 0x48 */ s32 unk48;
} Entry;

extern OSMesgQueue D_80142728;
extern List D_80146170;
extern List D_80146188;
extern s16 D_8014A108;
extern Entry D_8014A118[];
s32 osRecvMesg(OSMesgQueue *mq, void *msg, s32 flag);
s32 osJamMesg(OSMesgQueue *mq, void *msg, s32 flag);
void func_800D52CC(s32 *arg0);
void func_8009211C(List *list, s32 *object);
void func_80091FBC(List *list, s32 *object, Link *before);

void func_800D5374(void)
{
    s32 i;
    s32 *obj;

    for (i = 0; i < D_8014A108; i++) {
        obj = (s32 *)D_8014A118[i].object;
        D_8014A118[i].object = -1;
        if (obj == (s32 *)-1) {
            continue;
        }
        osRecvMesg(&D_80142728, 0, 1);
        func_800D52CC(obj);
        if (((s8 *)obj)[9] != 0) {
            func_8009211C(&D_80146188, obj);
            ((s8 *)obj)[9] = 0;
        }
        func_80091FBC(&D_80146170, obj, D_80146170.head);
        ((s8 *)obj)[8] = 1;
        osJamMesg(&D_80142728, 0, 0);
    }
}
