/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;

u8 *func_800BE4F0(u8 *destination, u8 *source) {
    u8 *start;
    u8 *out;
    u8 *in;
    u8 buf[256];
    u8 *p;

    in = source;
    out = destination;
    start = destination;
    if (*start == 255 || *source == 255) {
        if (*destination == 255) {
            out++;
            while (out[0] != 0 || out[1] != 0) {
                out += 2;
            }
        } else {
            p = buf;
            while ((*p++ = *destination++) != 0) {}
            *start = 255;
            destination = start;
            out = start + 1;
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
    return destination;
}
