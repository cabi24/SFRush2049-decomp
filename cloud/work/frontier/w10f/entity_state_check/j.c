typedef signed char s8; typedef unsigned char u8; typedef int s32; typedef unsigned int u32;
typedef struct {
    s8 active;      /* 0 */
    s8 b1;          /* 1 */
    u8 pad2[6];
    s32 id;         /* 8 */
    s32 pad12;
    u32 handle;     /* 16 */
    s32 pad20;
} Entry;            /* 24 */
extern Entry *D_80144C48;
extern u32 func_800201D0(u32);
s32 entity_state_check(s32 id) {
    Entry *e;
    u8 slot;
    if (id != -1) {
        slot = id;
        e = D_80144C48;
        if (id == e[slot].id) {
            if (e[slot].b1 != 0) {
                return func_800201D0((e + slot)->handle) != -1;
            }
            return 1;
        }
    }
    return 0;
}
