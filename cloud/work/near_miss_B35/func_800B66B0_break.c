/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed short s16;
typedef int s32;
s32 func_800B66B0(u8 *string,s16 limit) {
    s32 count=0;
    s32 position;
    if (*string==0xFF) {
        position=1;
        for (;;) {
            position+=2;
            count++;
            if (string[position-1]==0 && string[position-2]==0) break;
            if(limit>=0 && count>limit) break;
        }
    } else {
        position=0;
        for (;;) {
            position++;
            count++;
            if (*string++==0) break;
            if(limit>=0 && count>limit) break;
        }
    }
    return position;
}
