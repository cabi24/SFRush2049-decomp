/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef int s32;
s32 func_800BE744(u8 *string) {
    s32 count=0;
    if (*string==0xFF) {
        string++;
        while(string[0]!=0 || string[1]!=0) {
            string+=2;
            count++;
        }
    } else {
        while(*string!=0) {
            count++;
            string++;
        }
    }
    return count;
}
