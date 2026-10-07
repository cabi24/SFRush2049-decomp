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
void func_80101904(void)
{
    char *text;
    /* Fixed, untuned research capacities inferred from the next observed live
     * stack objects: sp+304..344 and sp+64..276. Original declarations and safe
     * runtime string bounds are not established. */
    char value[40];
    s32 rows,columns;
    RGBA color;
    char label[212];
    s32 row,column,total,other;
    rows=D_80152744+1;
    columns=3;
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                if(column>0 && row>0) {
                    color.r=0;color.g=0;color.b=0;color.a=192;
                    switch(D_8012E67C[D_80143F54[row-1]]) {
                    case 0:color.b=128;break;
                    case 1:color.r=128;break;
                    case 2:color.g=128;color.r=128;break;
                    case 3:color.g=128;break;
                    }
                } else {
                    color.r=0;color.g=0;color.b=0;color.a=192;
                }
                func_80100D5C(column*60+70,60,&color,row*14+36,14);
            }
        }
    }
    render_helper(0.0f);
    D_80118E20[0]=1;D_80118E20[1]=1;
    font_set(11);
    for(row=0;row<rows;row++) {
        for(column=0;column<columns;column++) {
            if(row!=0 || column!=0) {
                if(column==0) {
                    font_set(11);
                    if(D_80143F54[row-1]<active_player_count) {
                        dispatch_handler(D_801146AC[D_8012E67C[D_80143F54[row-1]]]);
                        text=input_rec0[D_80143F54[row-1]].name[0]+20;
                    } else {
                        text=D_80120354;
                        dispatch_handler(4);
                    }
                    race_position_update(label,text,60);
                    state_utility((s16)(column*60+100),(s16)(row*14+43),label);
                } else if(row==0) {
                    if(column==1) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[130];
                    } else if(column==2) {
                        dispatch_handler(1);
                        text=countdown_state.fixed[12];
                    }
                } else {
                    total=0;
                    text=value;
                    if(column==1) {
                        total=D_80152BBB[input_rec0[D_80143F54[row-1]].vehicle].score;
                    } else if(column==2) {
                        for(other=0;other<active_player_count;other++) {
                            total+=(D_80149428[other][D_80143F54[row-1]]>=0 ? D_80149428[other][D_80143F54[row-1]] : -D_80149428[other][D_80143F54[row-1]]);
                        }
                    }
                    dispatch_handler(1);
                    fcvt_wrapper(value,D_8012035C,total);
                }
                if(column!=0) state_utility((s16)(column*60+100),(s16)(row*14+43),text);
            }
        }
    }
    render_helper(-1.0f);
    D_80118E20[1]=3;D_80118E20[0]=0;
}
