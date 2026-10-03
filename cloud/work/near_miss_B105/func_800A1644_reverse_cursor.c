/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8; typedef unsigned short u16;
extern u16 D_8011EAEC[];
void func_800A1644(u8 *output,u8 *input,u8 length)
{
    u8 translated[64];
    u8 *cursor;
    int i,wide=0;
    for(i=length-1;i>=0;i--) {
        if(input[i]!=0 && input[i]!=15) break;
    }
    length=i+1;
    if(i>=0) {
        input+=i;
        cursor=translated+i;
        do {
            u8 value=*input;
            if(value==0) *cursor=15;
            else {
                *cursor=value;
                if(value>=66) wide=1;
            }
            cursor--; input--;
        } while(cursor>=translated);
    }
    if(wide) {
        *output++=255;
        for(i=0;i<length;i++) {
            *output++=D_8011EAEC[translated[i]]>>8;
            *output++=D_8011EAEC[translated[i]];
        }
        *output++=0;
        *output=0;
    } else {
        for(i=0;i<length;i++) output[i]=D_8011EAEC[translated[i]];
        output[i]=0;
    }
}
