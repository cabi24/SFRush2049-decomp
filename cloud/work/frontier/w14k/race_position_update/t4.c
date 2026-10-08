typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32;
extern s8 D_80149B70;
s32 object_manager_update(u8 *str, s16 maxlen);
u8 *func_800BE6A4(u8 *destination, u8 *source);
u8 *func_800BE4F0(u8 *destination, u8 *source);
s32 func_800BE744(u8 *string);

void race_position_update(u8 *dst, u8 *src, s16 maxWidth) {
    u8 tail[24];
    /*@{*/s32 width;/*@| s32 width; s32 k; @| s32 width; s32 mw; @| s32 width; s32 k; s32 mw; @}*/
    s32 wide;
    s32 len;
    s32 room;
    u8 *p;
    /*@{*/s32 n;/*@| s32 n; s32 spare; @| s32 n, idx; @}*/
    s32 q0, q1, q2, q3;
    /*@{*//*@| s32 mwv; @}*/
    /*@{*//*@| s32 wv = 0; @}*/

    width = object_manager_update(src, -1);
    /*@{*/if (maxWidth >= width) {/*@| if ((s32)maxWidth >= width) { @| if (maxWidth - width >= 0) { @}*/
        func_800BE6A4(dst, src);
        return;
    }
    /*@{*/wide = *dst == 255;/*@| wide = dst[0] == 255; @| wide = (*dst == 255); @}*/
    /*@{*/func_800BE6A4(dst, src);/*@| func_800BE6A4(dst, src); q0 = 0; @}*/
    len = func_800BE744(dst);
    if (len < 2) {
        return;
    }
    len -= 2;
    if (wide) {
        tail[0] = 255;
        func_800BE4F0(tail, "...");
        p = dst + len * 2;
        tail[7] = p[1];
        tail[8] = p[2];
        tail[9] = p[3];
        tail[10] = p[4];
        /*@{*/tail[11] = 0;
        tail[12] = 0;/*@| tail[11] = 0; tail[12] = 0; q1 = 0; @| tail[11] = 0; tail[12] = tail[11]; @}*/
        p[1] = 0;
        p[2] = 0;
    } else {
        func_800BE6A4(tail, "...");
        /*@{*/p = dst + len;/*@| p = dst; p += len; @}*/
        func_800BE4F0(tail, p);
        *p = 0;
    }
    /*@{*/room = maxWidth - object_manager_update(tail, -1);/*@| room = maxWidth; room = room - object_manager_update(tail, -1); @| {s32 t = object_manager_update(tail, -1); room = maxWidth - t;} @}*/
    /*@{*/n = len * 0 - 1;/*@| n = -1; @| n = 0 - 1; @}*/
    while (room - D_80149B70 < width) {
        /*@{*/len--;/*@| len -= 1; @}*/
        if (wide) {
            dst[len * 2 + 1] = 0;
            dst[len * 2 + 2] = 0;
        } else {
            /*@{*/dst[len] = 0;/*@| dst[len] = 0; dst[len + 0] = 0; @}*/
        }
        if (len == 0) {
            break;
        }
        /*@{*/width = object_manager_update(dst, n);/*@| width = object_manager_update(dst, n); q2 = width; @| {s32 t = object_manager_update(dst, n); width = t;} @}*/
    }
    func_800BE4F0(dst, tail);
}
