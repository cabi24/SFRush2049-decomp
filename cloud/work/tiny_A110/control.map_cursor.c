/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
extern u16 D_8011EAEC[256];
void func_800A150C(u8 *output,u8 *input,u8 limit)
{
    int count=0;
    u8 *cursor=input;
    if(*input==255) {
        cursor++;
        while(cursor[0]!=0 || cursor[1]!=0) {
            int index;
            u16 *map=D_8011EAEC;
            int code=(cursor[0]<<8)|cursor[1];
            cursor+=2;
            for(index=0;index<256;index++,map++) {
                if(code==*map) {
                    *output++=index;
                    count++;
                    break;
                }
            }
            if(count==limit)break;
        }
    } else {
        while(*cursor!=0) {
            int index;
            u16 *map=D_8011EAEC;
            int code=*cursor;
            for(index=0;index<256;index++,map++) {
                if(*map==code) {
                    *output++=index;
                    count++;
                    break;
                }
            }
            if(count==limit)break;
            cursor++;
        }
    }
    while(count<limit) {
        *output++=0;
        count++;
    }
}
