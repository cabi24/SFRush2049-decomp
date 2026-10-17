/* Native sync_entry_register body retained; unused generated declarations omitted. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Slot18 { s16 f0; s8 f2; s8 f3; u8 pad[0x14]; } Slot18;
#define NULL ((void *)0)
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
extern OSMesgQueue D_80142728, D_801427A8;
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osSendMesg(OSMesgQueue *,void *,s32);
Slot18 *func_80091B00(void);

void sync_entry_register(s32 arg0, s32 arg1) {
    Slot18 *sp1C;
    Slot18 *temp_v0;

    osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
    temp_v0 = func_80091B00();
    sp1C = temp_v0;
    M2C_FIELD(temp_v0, s8 *, 2) = 0;
    M2C_FIELD(temp_v0, s32 *, 4) = arg0;
    M2C_FIELD(temp_v0, s8 *, 8) = (s8) arg1;
    osSendMesg((OSMesgQueue *) &D_80142728, NULL, 0);
    osSendMesg((OSMesgQueue *) &D_801427A8, sp1C, 0);
}
