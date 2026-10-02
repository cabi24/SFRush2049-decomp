/* flags: -g0 -O0 -mips2 -G 0 -non_shared */
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
            exponent /= 10;
        } while (exponent > 9);
        *--digit = exponent + '0';
        while (digit < buffer + sizeof(buffer)) *output++ = *digit++;
    } else {
        *output++ = '0';
        *output++ = exponent + '0';
    }
    return output;
}
