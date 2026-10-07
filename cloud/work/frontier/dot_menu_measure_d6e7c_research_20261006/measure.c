/* Native menu width/height measurement at 0x800D6E7C. */
/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct TextIndices {
    u8 prefix[32];u16 title;u8 middle[44];u16 labels,values;
} TextIndices;
typedef struct TextOwner {u8 prefix[136];u8 *name;} TextOwner;
typedef struct TextState {
    u32 word0;TextOwner *owner;u32 word8;TextIndices *indices;u8 **strings;
} TextState;
typedef struct MenuMetrics {s32 left,width,height,value_left;} MenuMetrics;
extern TextState countdown_state;
extern u8 *D_80110030[];
extern MenuMetrics D_80110650;
extern u8 D_8012F6E8[],D_8012F6EC[];
extern OSMesgQueue D_801461D0;
extern s32 osRecvMesg(OSMesgQueue *,void **,s32),osJamMesg(OSMesgQueue *,void *,s32);
extern s32 slot_state_setup(s32),object_manager_update(u8 *,s16),object_bytes_sum_global(void);
extern u8 *func_800BE6A4(u8 *,u8 *);
static void gfx_lock(void) { osRecvMesg(&D_801461D0,0,1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0,0,0); }
static s32 font_set(s32 font) {
    s32 old;
    gfx_lock();old=slot_state_setup(font);gfx_unlock();
    return old;
}
void drone_collision_avoid(void)
{
    /* Fixed research hypothesis: native scratch starts at sp+84 and the next
       observed live scalar is at sp+128. Capacity44 is not independently proved;
       it has not been tuned and does not establish a safe runtime string bound. */
    u8 text[44];
    u8 *cursor;
    u32 label_width,value_width,rows;
    s32 row,item;
    font_set(10);
    label_width=0;
    value_width=0;
    rows=0;
    for(row=0;row<4;row++) {
        if(label_width<(u32)object_manager_update(countdown_state.strings[countdown_state.indices->labels+row],-1))
            label_width=object_manager_update(countdown_state.strings[countdown_state.indices->labels+row],-1);
        switch(row) {
        case 0:case 1:
            if(value_width<(u32)object_manager_update(D_8012F6E8,-1))
                value_width=object_manager_update(D_8012F6EC,-1);
            break;
        case 2:
            for(item=0;item<14;item++) {
                cursor=text;
                if(item<12) func_800BE6A4(text,D_80110030[item]);
                else if(item==12) func_800BE6A4(text,countdown_state.owner->name);
                else if(item==13) func_800BE6A4(text,countdown_state.strings[countdown_state.indices->title]);
                while(*cursor) {
                    if(*cursor>='a' && *cursor<='z') *cursor^=0x20;
                    cursor++;
                }
                if(value_width<(u32)object_manager_update(text,-1))
                    value_width=object_manager_update(text,-1);
            }
            break;
        case 3:
            if(value_width<(u32)object_manager_update(countdown_state.strings[countdown_state.indices->values],-1))
                value_width=object_manager_update(countdown_state.strings[countdown_state.indices->values],-1);
            if(value_width<(u32)object_manager_update(countdown_state.strings[countdown_state.indices->values+1],-1))
                value_width=object_manager_update(countdown_state.strings[countdown_state.indices->values+1],-1);
            break;
        }
        rows++;
    }
    D_80110650.width=label_width+value_width+10;
    D_80110650.height=object_bytes_sum_global()*rows;
    D_80110650.left=160-D_80110650.width/2;
    D_80110650.value_left=D_80110650.left+label_width+5;
}
