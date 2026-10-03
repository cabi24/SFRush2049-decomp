/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
extern s8 D_80149B70;
extern int camera_shake_update(u16);
s16 camera_zoom_fov(s16 limit,u8 *text)
{
    s8 wide;
    u8 *start,*cursor,*space;
    int step;
    s16 width,count,lines,spacing,code;
    width=0; count=0;
    wide=text[0]==0xFF;
    if(wide) { start=text+1; step=2; }
    else { start=text; step=1; }
    spacing=D_80149B70;
    lines=0; space=0;
    do {
        cursor=start;
        for(;;) {
            if(wide) code=(cursor[0]<<8)|cursor[1];
            else code=cursor[0];
            if(code==32 && !width) { start+=step; cursor=start; continue; }
            if(!code) break;
            if(code==13) break;
            if(code==10) break;
            width+=camera_shake_update((u16)code);
            if(code!=32) width+=spacing;
            else if(space!=cursor-step) space=cursor;
            count++;
            cursor+=step;
            if(limit<width && space) { cursor=space; break; }
        }
        start=cursor+step;
        lines++;
        if(!code || lines==32767) break;
        count=0; width=0; space=0;
    } while(1);
    return lines;
}
