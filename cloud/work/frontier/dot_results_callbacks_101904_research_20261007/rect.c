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
void func_80100D5C(s32 x,s32 width,RGBA *color,s32 y,s32 height)
{
    {Gfx *command=D_80149438++;
    command->w0=0xE7000000;command->w1=0;
    }
    {Gfx *command=D_80149438++;
    command->w0=0xEE000000;command->w1=0x00100000;
    }
    {Gfx *command=D_80149438++;
    command->w0=0xFA000000;
    command->w1=color->a|((u32)color->r<<24)|((u32)color->g<<16)|((u32)color->b<<8);
    }
    {Gfx *command=D_80149438++;
    command->w0=0xFCFFFFFF;command->w1=0xFFFDF6FB;
    }
    {Gfx *command=D_80149438++;
    command->w0=(((x+width)&1023)<<14)|0xF6000000|(((y+height)&1023)<<2);
    command->w1=((x&1023)<<14)|((y&1023)<<2);
    }
    D_8014A248=-1;
}
