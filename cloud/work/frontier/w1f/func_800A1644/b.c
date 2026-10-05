typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

extern u16 D_8011EAEC[];

void func_800A1644(u8 *out, u8 *in, u8 len) {
    DECLS

    wide = 0;
    for (i = len - 1; i >= 0; i--) {
        if (in[i] != 0 && in[i] != 15) {
            break;
        }
    }
    len = i + 1;
    for (; i >= 0; i--) {
        if (in[i] == 0) {
            buf[i] = 15;
        } else {
            buf[i] = in[i];
            if (buf[i] > 65) {
                wide = 1;
            }
        }
    }
    if (wide) {
        p = out;
        *p++ = 255;
        for (i = 0; i < len; i++) {
            *p++ = D_8011EAEC[buf[i]] >> 8;
            *p++ = D_8011EAEC[buf[i]];
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
