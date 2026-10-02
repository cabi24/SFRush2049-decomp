/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct PadConfig PadConfig;
typedef s32 (*Callback)(PadConfig *);
struct PadConfig {
    void *data;void *bytes4;void *extra8;u16 half12;s16 x,y;u16 half18;
    s16 width,height;u8 status,flag;s8 disabled;u8 other27;
    s16 left,bottom,right,top,other36;u16 other38;
    Callback callback;u32 packed;void *link48;u16 half52;u16 other54;
    s32 word56;PadConfig *next;
};
typedef struct {
    void *data;s16 x,y,width,height,left,bottom,right,top;
    s32 flags18,status;Callback callback;u32 packed;
} InputRecord36;
extern PadConfig *func_800B3704();
extern void Input_ApplyPadConfig(PadConfig *);
extern void sound_stop(PadConfig *);
PadConfig *sound_control(s16 x,s16 y,InputRecord36 *input,s16 count)
{
    InputRecord36 *record;
    PadConfig *first,*previous,*current;
    s32 i;
    if(count<=0) return 0;
    first=0;
    previous=0;
    record=input;
    for(i=0;i<count;i++) {
        current=func_800B3704(record->data,record->x+x,record->y+y,record->packed&0x80000000u);
        current->status=record->status;
        current->half18=record->flags18;
        current->right=record->right;
        current->left=record->left;
        current->top=record->top;
        current->bottom=record->bottom;
        if(record->width>=0) current->width=record->width;
        if(record->height>=0) current->height=record->height;
        Input_ApplyPadConfig(current);
        if(record->callback) {
            current->packed=record->packed&0x7fffffffu;
            if(record->data==(void *)-1) {
                current->extra8=(void *)record->callback;
                current->bytes4=current;
                Input_ApplyPadConfig(current);
            } else {
                Callback action=record->callback;
                current->callback=action;
                if(!action(current)) {
                    sound_stop(current);
                    return first;
                }
            }
        }
        if(i==0) first=current;
        else previous->next=current;
        previous=current;
        record++;
    }
    return first;
}
