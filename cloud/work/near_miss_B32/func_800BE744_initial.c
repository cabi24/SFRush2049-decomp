/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef int s32;
s32 func_800BE744(u8 *string) {
    s32 count=0;
    u8 *cursor=string+1;
    u8 first=*string;
    if (first==0xFF) {
        while(cursor[0]!=0 || cursor[1]!=0) {
            cursor+=2;
            count++;
        }
    } else {
        if (first!=0) {
            do {count++;} while(*cursor++!=0);
        }
    }
    return count;
}
