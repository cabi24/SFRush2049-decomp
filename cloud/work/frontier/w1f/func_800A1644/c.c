typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

extern u16 D_8011EAEC[];

void func_800A1644(u8 *out, u8 *in, u8 len) {
    s32 i; s32 wide; u8 buf[64]; u8 *p; u8 *q;

    p = out;
    q = in;
    wide = 0;
    for (i = len - 1; i >= 0; i--) {
        if (q[i] != 0 && q[i] != 15) {
            break;
        }
    }
    len = i + 1;
    for (; i >= 0; i--) {
        if (q[i] == 0) {
            buf[i] = 15;
        } else {
            buf[i] = q[i];
            if (buf[i] > 65) {
                wide = 1;
            }
        }
    }
    if (wide) {
        *p++ = 255;
        for (i = 0; i < len; i++) {
            *p++ = D_8011EAEC[buf[i]] >> 8;
            *p++ = D_8011EAEC[buf[i]] & 0xFF;
        }
        *p++ = 0;
        *p = 0;
    } else {
        for (i = 0; i < len; i++) {
            out[i] = D_8011EAEC[buf[i]];
        }
        out[i] = 0;
    }
}
