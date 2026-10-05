/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research NONMATCH: one symmetric equality-operand ordering difference.
 * Convert the game's byte / 0xFF-prefixed two-byte string into indices in
 * the original 256-entry unsigned-halfword character map, then zero-pad.
 * No arcade ancestor has been established. This is an ordinary three-input
 * leaf; separate consumed input/output cursors preserve its native data flow.
 * The byte-mode lookup initializer captures the real promoted codepoint after
 * initializing its index. Preserve the original equality stop: limit == 0
 * is not a no-write guarantee when the first input character maps.
 * No owned data, invented arguments, padding, artificial work or assembly.
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
                if (D_8011EAEC[index] == code) {
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
