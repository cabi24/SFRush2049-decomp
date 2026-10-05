/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (also MATCH at -O2) */
/*
 * Encode a fixed-width code string (callers pass lengths 16 and 4; called
 * from track_process_main and track_render_process) through the u16 table
 * D_8011EAEC.  Trailing 0 / 15 codes are trimmed, remaining 0 codes become
 * 15 (blank).  If every code is <= 65 the output is one byte per code plus a
 * zero terminator; otherwise it is 0xFF, two bytes (high, low) per code and
 * two zero bytes.  No arcade ancestor identified.
 *
 * Shaping facts, each verified necessary:
 * - both pointer parameters are copied to locals first and only the copies
 *   are used (using `out` / `in` directly moves them out of a0 / a1 or
 *   spills them; this was the wall in near_miss_B105);
 * - no byte temporary: src[i] / buf[i] are re-read in the source;
 * - `& 0xFF` on the low-byte store (one extra temp-ring step, no code).
 * buf[64] is the capacity implied by the 104-byte frame with buf at sp+32.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

extern u16 D_8011EAEC[];

void func_800A1644(u8 *out, u8 *in, u8 len) {
    s32 i;
    s32 wide;
    u8 buf[64];
    u8 *dst;
    u8 *src;

    dst = out;
    src = in;
    wide = 0;
    for (i = len - 1; i >= 0; i--) {
        if (src[i] != 0 && src[i] != 15) {
            break;
        }
    }
    len = i + 1;
    for (; i >= 0; i--) {
        if (src[i] == 0) {
            buf[i] = 15;
        } else {
            buf[i] = src[i];
            if (buf[i] > 65) {
                wide = 1;
            }
        }
    }
    if (wide) {
        *dst++ = 255;
        for (i = 0; i < len; i++) {
            *dst++ = D_8011EAEC[buf[i]] >> 8;
            *dst++ = D_8011EAEC[buf[i]] & 0xFF;
        }
        *dst++ = 0;
        *dst = 0;
    } else {
        for (i = 0; i < len; i++) {
            dst[i] = D_8011EAEC[buf[i]];
        }
        dst[i] = 0;
    }
}
