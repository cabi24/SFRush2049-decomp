/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
extern s32 D_80118E20,D_80118E24;
extern s8 D_80149B70;
extern s16 D_80149D92,D_80149D9E;
extern s16 object_bytes_sum_global(void);
extern s16 camera_zoom_fov(s16,u8 *);
extern s32 camera_shake_update(u16);
extern s32 object_manager_update(u8 *,s16);
extern void menu_input_process(u8 *,s16);
void camera_auto_follow(s16 x,s16 y,s16 width,s16 height,
                        s16 page,s16 skip_lines,u8 *text)
{
    s16 line_height=object_bytes_sum_global();
    s16 spacing=D_80149B70;
    s16 cursor_y=y;
    s16 bottom;
    s8 wide;
    s32 step;
    u8 *line_start,*scan,*last_space;
    s16 line_width,count,pages,skipped,reserve;
    s16 ch;
    u8 saved_byte;
    if(D_80118E24!=3) {
        s16 lines=camera_zoom_fov(width,text);
        if(D_80118E24==1) cursor_y=y-(lines*line_height)/2;
        else cursor_y=y-lines*line_height;
    }
    bottom=cursor_y+height;
    wide=(*text==255);
    line_start=text;
    if(wide) { line_start=text+1; step=2; }
    else step=1;
    line_width=0;
    count=0;
    last_space=0;
    skipped=0;
    if(page<0) { pages=page; bottom+=line_height; }
    else pages=0;
    if(pages<page) reserve=line_height;
    else reserve=0;
    for(;;) {
        scan=line_start;
        for(;;) {
            if(wide) ch=(scan[0]<<8)|scan[1];
            else ch=scan[0];
            if(ch==32 && line_width==0) {
                line_start+=step;
                scan=line_start;
                continue;
            }
            if(ch==0 || ch==13 || ch==10) break;
            line_width+=camera_shake_update((u16)ch);
            if(ch!=32) line_width+=spacing;
            else if(last_space!=scan-step) last_space=scan;
            if(width<line_width && last_space) {
                if(wide) count-=(scan-last_space)/2;
                else count-=scan-last_space;
                scan=last_space;
                break;
            }
            count++;
            scan+=step;
        }
        if(pages<page) {
            cursor_y+=line_height;
        } else if(skipped<skip_lines) {
            skipped++;
        } else {
            if(count>0) {
                s32 pixels;
                s16 draw_x;
                if(wide) {
                    saved_byte=line_start[-1];
                    line_start[-1]=255;
                }
                pixels=object_manager_update(line_start-(wide ? 1 : 0),count);
                if(D_80118E20==1) draw_x=x-(pixels>>1);
                else if(D_80118E20==2) draw_x=x-pixels;
                else draw_x=x;
                if(draw_x!=-32768) D_80149D92=draw_x;
                if(cursor_y!=-32768) D_80149D9E=cursor_y;
                menu_input_process(line_start-(wide ? 1 : 0),count);
                if(wide) line_start[-1]=saved_byte;
            }
            cursor_y+=line_height;
            skipped++;
        }
        if(ch==0) break;
        line_start=scan+step;
        if(bottom-line_height-reserve<cursor_y) {
            pages++;
            cursor_y=y;
            if(page<pages) break;
            if(pages==page) reserve=0;
        }
        count=0;
        line_width=0;
        last_space=0;
    }
}
