/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * RESEARCH ONLY: complete behavioral reconstruction, NOT an instruction match.
 * func_800A1644, 0x800A1644, 716 native bytes / 179 instructions.
 * N64 Controller Pak name-code encoder; no arcade equivalent identified.
 * Removes trailing codes 0 and 15, normalizes internal 0 to 15, and emits
 * table-translated single-byte text or 0xff-prefixed big-endian two-byte text.
 * The two-byte path ends in two zero bytes; the single-byte path in one.
 *
 * Ordinary three-input ABI: destination, source, low-byte length.
 * All four direct caller sites use length 16 (name) or 4 (extension).
 * The 64-byte local workspace is a reconstruction inference consistent with
 * the native frame and consumed locals, NOT a recovered original declaration.
 * No padding, fabricated helper, extra formal, or volatile pressure is used.
 * Compile with the genuine-function-only O3 group.json recipe alongside this
 * source. Keep out of accepted sources/locks until native and ROM gates pass.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
extern u16 D_8011EAEC[];

void func_800A1644(u8 *dst, u8 *src, u8 length)
{
    s32 i;
    s32 extended = 0;
    u8 normalized[64];
    u8 code;
    u8 *out;

    for (i = length - 1; i >= 0; i--) {
        if (src[i] != 0 && src[i] != 15) {
            break;
        }
    }
    length = i + 1;
    for (; i >= 0; i--) {
        code = src[i];
        if (code == 0) {
            normalized[i] = 15;
        } else {
            normalized[i] = code;
            if (code >= 66) {
                extended = 1;
            }
        }
    }
    if (extended) {
        *dst = 255;
        out = dst + 1;
        for (i = 0; i < length; i++) {
            *out++ = D_8011EAEC[normalized[i]] >> 8;
            *out++ = D_8011EAEC[normalized[i]];
        }
        *out++ = 0;
        *out = 0;
    } else {
        for (i = 0; i < length; i++) {
            dst[i] = D_8011EAEC[normalized[i]];
        }
        dst[i] = 0;
    }
}
