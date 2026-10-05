/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800BE4F0: strcat for the game's two string encodings, returns destination.
 * A string is either plain bytes (0-terminated) or "wide": a leading 255 followed by 2-byte
 * characters ending in a 0,0 pair.  If both are plain this is strcat.  Otherwise the result is
 * wide: a plain destination is first converted in place (copied to a 256-byte local buffer,
 * rewritten as 255 then 0,c pairs), then the source is appended as pairs (a plain source
 * gets a 0 high byte per character) and the 0,0 terminator is written.  Sibling of
 * func_800BE6A4 (the matching strcpy, which follows it in the image).  No arcade ancestor
 * (N64 text code).
 * Quirks the match depends on:
 *   - the return value is a separate local `ret` that doubles as the copy pointer into buf and
 *     is reset afterwards; `destination` itself is then advanced past the 255 (`*destination++`)
 *     so IDO cannot copy-propagate `ret` back into `destination` (retail keeps ret in v1);
 *   - `while (*in)` and `while (*out)` are truth tests, not `!= 0`: with `!= 0` the reloads of
 *     *source land in a0 instead of t1 (5 words off).
 * Also matches at -O2.
 */
typedef unsigned char u8;

u8 *func_800BE4F0(u8 *destination, u8 *source) {
    u8 *ret;
    u8 *out;
    u8 *in;
    u8 buf[256];
    u8 *p;

    ret = destination;
    out = destination;
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
            ret = destination;
            *destination++ = 255;
            out = destination;
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
            while (*in) {
                *out++ = 0;
                *out++ = *in++;
            }
        }
        *out++ = 0;
        *out = 0;
    } else {
        while (*out) {
            out++;
        }
        while ((*out++ = *in++) != 0) {}
    }
    return ret;
}
