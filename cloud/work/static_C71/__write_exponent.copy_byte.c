/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
u8 *__write_exponent(register u8 *output, register int exponent, register int format) {
    u8 *digit;
    u8 buffer[308];
    *output++ = format;
    if (exponent < 0) {exponent = -exponent; *output++ = '-';}
    else *output++ = '+';
    digit = buffer + sizeof(buffer);
    if (exponent > 9) {
        do {
            *--digit = exponent % 10 + '0';
        } while ((exponent /= 10) > 9);
        *--digit = exponent + '0';
        for (; digit < buffer + sizeof(buffer); digit++) { register u8 byte = *digit; *output++ = byte; }
    } else {
        *output++ = '0';
        *output++ = exponent + '0';
    }
    return output;
}
