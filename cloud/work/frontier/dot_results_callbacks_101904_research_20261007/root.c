/* Shared source model for native results callbacks. External record gaps are
 * observed offsets/strides, not compiler-pressure stack storage. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Gfx {u32 w0,w1;} Gfx;
typedef struct RGBA {u8 r,g,b,a;} RGBA;
typedef struct PlayerRow {u8 vehicle;u8 other[71];char **name;} PlayerRow;
typedef struct VehicleScore {s8 score;u8 other[951];} VehicleScore;
typedef struct PlayerStats {s32 words[30];} PlayerStats;
typedef struct TextIndices {u8 other[90];u16 menu;} TextIndices;
typedef struct TextState {void *word0;char **fixed;void *word8;TextIndices *indices;char **strings;} TextState;
extern s8 D_80152744,D_80143F54[],D_8012E67C[];
extern s8 D_80149428[][4];
extern s16 active_player_count;
extern PlayerRow input_rec0[];
extern VehicleScore D_80152BBB[];
extern PlayerStats D_80152038[];
extern s32 D_801146AC[],D_80118E20[2],D_801491E0,gameplay_mode;
extern u8 D_80114724[];
extern char D_80120354[],D_8012035C[],D_80120344[],D_8012034C[],D_80120350[];
extern OSMesgQueue D_801461D0;
extern TextState countdown_state;
extern Gfx *D_80149438;
extern s32 D_8014A248;
extern void render_helper(f32);
extern void dispatch_handler(s32);
extern void race_position_update(char *,char *,s32);
extern void state_utility(s16,s16,char *);
extern s32 fcvt_wrapper(char *,char *,...);
extern s32 osRecvMesg(OSMesgQueue *,void **,s32),osJamMesg(OSMesgQueue *,void *,s32);
extern s32 slot_state_setup(s32);
extern void func_80100D5C(s32,s32,RGBA *,s32,s32);
extern void func_80101904(void),func_80101D84(void),func_80100E58(void);
extern s32 func_800F84B0(s32),object_bytes_sum_global(void);
static void gfx_lock(void) {osRecvMesg(&D_801461D0,0,1);}
static void gfx_unlock(void) {osJamMesg(&D_801461D0,0,0);}
static s32 font_set(s32 font) {
    s32 previous;
    gfx_lock();previous=slot_state_setup(font);gfx_unlock();
    return previous;
}
/* The only native direct caller of both callbacks. */
void func_8010221C(void)
{
    char *text;
    s32 item,color;
    s16 x,y;
    render_helper(0.0f);
    D_80118E20[0]=1;D_80118E20[1]=3;
    font_set(13);
    dispatch_handler(1);
    x=D_80114724[0];y=D_80114724[1];
    text=countdown_state.fixed[63];
    state_utility(x,y,text);
    y=118;
    font_set(10);
    for(item=0;item<7;item++) {
        if(func_800F84B0(item)) {
            text=countdown_state.strings[countdown_state.indices->menu+item];
            if(item==0 && gameplay_mode!=4 && gameplay_mode!=6)
                text=countdown_state.fixed[0];
            color=1;
            if(item==D_801491E0) color=22;
            dispatch_handler(color);
            state_utility(x,y,text);
            y+=object_bytes_sum_global();
        }
    }
    D_80118E20[1]=3;D_80118E20[0]=0;
    render_helper(-1.0f);
    switch(gameplay_mode) {
    case 4:func_80101D84();break;
    case 6:func_80101904();break;
    case 0:case 2:case 3:case 5:func_80100E58();break;
    }
}
