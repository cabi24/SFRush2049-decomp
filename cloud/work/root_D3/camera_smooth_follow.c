/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef unsigned char u8;
typedef int s32;
void camera_smooth_follow(u32 *dl,s32 x,s32 y,s32 endx,s32 endy) {
    s32 width=-1,height=-1;
    s32 dimension;
    u8 op;
    for(;;dl+=2) {
        op=(dl[0]&0xFF000000)>>24;
        if((op&0xC0)==0x40 || (op&0xC0)==0x80 || (op>=9&&op<0x40) || (op>=0xC0&&op<0xD6)) continue;
        if(op==0xF2) {
            if(x>=0) {
                dimension=((dl[1]&0x00FFF000)>>12)+4;
                if(width<0) width=dimension;
                else if(dimension<width) {
                    do {x>>=1;
                        width>>=1;} while(dimension<width);
                }
                dl[0]=(dl[0]&0xFF000FFF)|(x<<12);
            }
            if(y>=0) {
                dimension=(dl[1]&0xFFF)+4;
                if(height<0) height=dimension;
                else if(dimension<height) {
                    do {y>>=1;
                        height>>=1;} while(dimension<height);
                }
                dl[0]=(dl[0]&0xFFFFF000)|y;
            }
            if(endx>=0) dl[1]=(dl[1]&0xFF000FFF)|(endx<<12);
            if(endy>=0) dl[1]=(dl[1]&0xFFFFF000)|endy;
        } else if(op==0xDF) return;
    }
}
