/*@@HDR 0 3030@@*/
s32 func_80091BA8_unused(s32 arg0);
/*@@HDR 3031 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3538@@*/

/*@@HDR 3539 3697@@*/
#endif

typedef struct { u8 pad[0x10]; s32 type; u8 pad2[0x28]; s32 link; } ResEntry;
extern ResEntry *func_80091BA8(s32 arg0);
s32 results_time_display(s32 arg0) {
    s32 result; ResEntry *e;
    osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
    e = func_80091BA8(arg0);
    if (e != NULL) {
        if (e->type == 1 || e->type == 3) { result = 1; }
        else if (e->type != 2) { result = 0; }
        else { result = entity_state_check(e->link); }
    } else { result = 0; }
    osJamMesg((OSMesgQueue *) &D_80142728, NULL, 0);
    return result;
}
