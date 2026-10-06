/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800A150C(output, input, limit) -- map an encoded string to character-map
 * indices.  A first byte of 255 selects big-endian two-byte codepoints (ended by a
 * zero pair); otherwise single bytes (ended by a zero byte).  Each codepoint is
 * looked up in the 256-entry u16 table D_8011EAEC and the first matching index is
 * written as a byte; unmapped codepoints produce nothing.  Stops when `limit`
 * bytes have been written, then zero-fills up to `limit` (no terminator).
 * No arcade ancestor established.  Logic from cloud/work/encoded_string_mapping
 * (A150C research, 1 word off).
 *
 * Shaping quirk: the byte-mode test is spelled `D_8011EAEC[index] - code == 0`.
 * uopt canonicalises `x == y` with a variable operand first (`bnel code,map`,
 * as in the two-byte loop), whatever the source order; retail's byte loop has
 * the table load first (`bnel t5,a1`), which the subtract-compare form keeps.
 */
typedef unsigned char u8;
typedef unsigned short u16;
extern u16 D_8011EAEC[256];

void func_800A150C(u8 *output, u8 *input, u8 limit)
{
    int count = 0, index;
    u8 *cursor = input, *destination = output;

    if (*input == 255) {
        cursor++;
        while (cursor[0] != 0 || cursor[1] != 0) {
            unsigned int code = (cursor[0] << 8) | cursor[1];
            cursor += 2;
            for (index = 0; index < 256; index++) {
                if (code == D_8011EAEC[index]) {
                    *destination++ = index;
                    count++;
                    break;
                }
            }
            if (count == limit) break;
        }
    } else {
        while (*cursor != 0) {
            unsigned int code;
            for (index = 0, code = *cursor; index < 256; index++) {
                if (D_8011EAEC[index] - code == 0) {
                    *destination++ = index;
                    count++;
                    break;
                }
            }
            if (count == limit) break;
            cursor++;
        }
    }
    while (count < limit) {
        *destination++ = 0;
        count++;
    }
}
