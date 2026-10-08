typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32;
extern s8 D_80149B70;
s32 object_manager_update(u8 *str, s16 maxlen);
u8 *func_800BE6A4(u8 *destination, u8 *source);
u8 *func_800BE4F0(u8 *destination, u8 *source);
s32 func_800BE744(u8 *string);

void race_position_update(u8 *dst, u8 *src, s16 maxWidth) {
    s32 width;
    s32 wide;
    s32 len;
    s32 room;
    u8 *p;
    u8 tail[24];
    s32 n;

    width = object_manager_update(src, -1);
    if (maxWidth >= width) {
        func_800BE6A4(dst, src);
        return;
    }
    wide = *dst == 255;
    func_800BE6A4(dst, src);
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
    n = -1;
    while (room - D_80149B70 < width) {
        len--;
        if (wide) {
            dst[len * 2 + 1] = 0;
            dst[len * 2 + 2] = 0;
        } else {
            dst[len] = 0;
        }
        if (len == 0) {
            break;
        }
        width = object_manager_update(dst, n);
    }
    func_800BE4F0(dst, tail);
}
