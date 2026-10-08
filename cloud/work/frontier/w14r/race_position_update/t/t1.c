typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32;
extern s8 D_80149B70;
s32 object_manager_update(u8 *str, s16 maxlen);
u8 *func_800BE6A4(u8 *destination, u8 *source);
u8 *func_800BE4F0(u8 *destination, u8 *source);
s32 func_800BE744(u8 *string);

void race_position_update(u8 *dst, u8 *src, s16 maxWidth) {
    u8 tail[24];
    /*@{DO*/s32 width;
    s32 wide;/*@| s32 wide;
    s32 width; @}*/
    s32 len;
    s32 room;
    u8 *p;
    s32 n;
    /*@{QQ*/s32 q0, q1, q2, q3;/*@| @}*/

    width = object_manager_update(src, -1);
    if (maxWidth >= width) {
        func_800BE6A4(dst, src);
        return;
    }
    /*@{WS*/wide = *dst == 255;/*@| wide = 255 == *dst; @}*/
    func_800BE6A4(dst, src);
    len = func_800BE744(dst);
    if (/*@{LT*/len < 2/*@| 2 > len @}*/) {
        return;
    }
    /*@{LE*/len -= 2;/*@| len = len - 2; @}*/
    if (wide) {
        tail[0] = 255;
        func_800BE4F0(tail, "...");
        p = dst + /*@{SP*/len * 2/*@| 2 * len @}*/;
        tail[7] = p[1];
        tail[8] = p[2];
        tail[9] = p[3];
        tail[10] = p[4];
        tail[11] = 0;
        tail[12] = 0;
        p[1] = 0;
        p[2] = 0;
    } else {
        func_800BE6A4(tail, "...");
        p = dst + len;
        func_800BE4F0(tail, p);
        *p = 0;
    }
    room = maxWidth - object_manager_update(tail, -1);
    /*@{NI*/n = len * 0 - 1;/*@| @}*/
    while (/*@{WC*/room - D_80149B70 < width/*@| width > room - D_80149B70 @}*/) {
        len--;
        if (wide) {
            dst[/*@{IX*/len * 2 + 1/*@| 1 + len * 2 @}*/] = 0;
            dst[/*@{IX*/len * 2 + 2/*@| 2 + len * 2 @}*/] = 0;
        } else {
            dst[len] = 0;
        }
        if (len == 0) {
            break;
        }
        width = object_manager_update(dst, /*@{NI*/n/*@| len * 0 - 1 @}*/);
    }
    func_800BE4F0(dst, tail);
}
