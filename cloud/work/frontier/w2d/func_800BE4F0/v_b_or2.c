/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;

u8 *func_800BE4F0(u8 *destination, u8 *source) {
    u8 *ret;
    u8 *out;
    u8 *in;
    u8 buf[256];
    u8 *p;

    out = destination;
    ret = out;
    in = source;
    if (*destination == 255 || *source == 255) {
        if (*destination == 255) {
            out++;
            while (out[0] != 0 || out[1] != 0) {
                out += 2;
            }
        } else {
            p = buf;
            while ((*p++ = *ret++) != 0) {}
            *destination = 255;
            ret = destination;
            out = ret + 1;
            p = buf;
            while (*p != 0) {
                *out++ = 0;
                *out++ = *p++;
            }
        }
        if (*source == 255) {
            in++;
            while (in[0] != 0 || in[1] != 0) {
                *out++ = *in++;
                *out++ = *in++;
            }
        } else {
            while (*in != 0) {
                *out++ = 0;
                *out++ = *in++;
            }
        }
        *out++ = 0;
        *out = 0;
    } else {
        while (*out != 0) {
            out++;
        }
        while ((*out++ = *in++) != 0) {}
    }
    return ret;
}
